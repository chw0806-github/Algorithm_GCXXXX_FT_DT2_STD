#include "testitems.h"
#include <QObject>
#include <QFile>
#include <QTextStream>

namespace {

/** 双字节 RAW 中第二字节有效低位掩码：10→0x03，12→0x0F，14→0x3F，16→0xFF */
inline uchar high_byte_mask_for_raw_bits(int raw_bits)
{
    switch(raw_bits)
    {
        case 10: return 0x03;  // 2位有效
        case 12: return 0x0F;  // 4位有效
        case 14: return 0x3F;  // 6位有效
        case 16: return 0xFF;  // 8位全有效
        default: return 0xFF;  // 默认16bit
    }
}

} // namespace


void TestItem::distribute_pointer_by_RGB_BAYER(RGB_BAYER bayer,uchar **pb1 ,uchar **pb2,uchar **pb3,uchar **pb4,
                                               uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_)
{
    switch (bayer) {
    case RG:
        *pb1 = g_buf_r_;
        *pb2 = g_buf_gr_;
        *pb3 = g_buf_gb_;
        *pb4 = g_buf_b_;
        break;
    case BG:
        *pb1 = g_buf_b_;
        *pb2 = g_buf_gb_;
        *pb3 = g_buf_gr_;
        *pb4 = g_buf_r_;
        break;
    case GR:
        *pb1 = g_buf_gr_;
        *pb2 = g_buf_r_;
        *pb3 = g_buf_b_;
        *pb4 = g_buf_gb_;
        break;
    case GB:
        *pb1 = g_buf_gb_;
        *pb2 = g_buf_b_;
        *pb3 = g_buf_r_;
        *pb4 = g_buf_gr_;
        break;
    default:
        break;
    }
}

void TestItem::split_channel(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_,
                             uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    distribute_pointer_by_RGB_BAYER(bayer,&pb1,&pb2,&pb3,&pb4,
                                    g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_);

    for (int i=0; i<h; i=i+2)
    {
        for (int j=0; j<w; j=j+2)
        {
            *pb1++ = g_buf_raw_[w*i+ j];
            *pb2++ = g_buf_raw_[w*i+ j+1];
        }
        for (int j=0; j<w; j=j+2)
        {
            *pb3++ = g_buf_raw_[w*(i+1)+ j];
            *pb4++ = g_buf_raw_[w*(i+1)+ j+1];
        }
    }

}

void TestItem::split_channel_4C(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_, uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    distribute_pointer_by_RGB_BAYER(bayer,&pb1,&pb2,&pb3,&pb4,
                                    g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_);

    for (int i=0; i<h; i=i+4)
    {
        for(int k=0;k!=2;k++)
        {
            for (int j=0; j<w; j=j+4)
            {
                *pb1++ = g_buf_raw_[w*(i+k)+ j];
                *pb1++ = g_buf_raw_[w*(i+k)+ j+1];
                *pb2++ = g_buf_raw_[w*(i+k)+ j+2];
                *pb2++ = g_buf_raw_[w*(i+k)+ j+3];
            }
        }
        for(int k=2;k!=4;k++)
        {
            for (int j=0; j<w; j=j+4)
            {
                *pb3++ = g_buf_raw_[w*(i+k)+ j];
                *pb3++ = g_buf_raw_[w*(i+k)+ j+1];
                *pb4++ = g_buf_raw_[w*(i+k)+ j+2];
                *pb4++ = g_buf_raw_[w*(i+k)+ j+3];
            }
        }
    }
}


void TestItem::split_channel_raw16(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                                   uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                                   int raw_bits)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    distribute_pointer_by_RGB_BAYER(bayer,&pb1,&pb2,&pb3,&pb4,
                                    g_buf_r_16_,g_buf_gr_16_,g_buf_gb_16_,g_buf_b_16_);

    const uchar hi_mask = high_byte_mask_for_raw_bits(raw_bits);

    for (int i=0; i<h; i=i+2)
    {
        for (int j=0; j<2*w; j=j+4)
        {
            *pb1++ = g_buf_raw_16_[2*w*i+j];
            *pb1++ = g_buf_raw_16_[2*w*i+j+1] & hi_mask;

            *pb2++ = g_buf_raw_16_[2*w*i+j+2];
            *pb2++ = g_buf_raw_16_[2*w*i+j+3] & hi_mask;
        }
        for (int j=0; j<2*w; j=j+4)
        {
            *pb3++ = g_buf_raw_16_[2*w*(i+1) +j];
            *pb3++ = g_buf_raw_16_[2*w*(i+1) +j+1] & hi_mask;

            *pb4++ = g_buf_raw_16_[2*w*(i+1) +j+2];
            *pb4++ = g_buf_raw_16_[2*w*(i+1) +j+3] & hi_mask;
        }
    }
}

void TestItem::split_channel_raw16_4C(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                                      uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                                      int raw_bits)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;

    distribute_pointer_by_RGB_BAYER(bayer,&pb1,&pb2,&pb3,&pb4,
                                    g_buf_r_16_,g_buf_gr_16_,g_buf_gb_16_,g_buf_b_16_);
    const uchar hi_mask = high_byte_mask_for_raw_bits(raw_bits);
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<2*w; j+=2)
        {
            int l = i/2;
            int m = j/4;
            if (l%2 == 0 && m%2 == 0)
            {
                *pb1++ = g_buf_raw_16_[2*w*i+j];
                *pb1++ = g_buf_raw_16_[2*w*i+j+1] & hi_mask;
            }
            else if(l%2 == 0 && m%2 == 1)
            {
                *pb2++ = g_buf_raw_16_[2*w*i+j];
                *pb2++ = g_buf_raw_16_[2*w*i+j+1] & hi_mask;
            }
            else if(l%2 == 1 && m%2 == 0)
            {
                *pb3++ = g_buf_raw_16_[2*w*i+j];
                *pb3++ = g_buf_raw_16_[2*w*i+j+1] & hi_mask;
            }
            else if(l%2 == 1 && m%2 == 1)
            {
                *pb4++ = g_buf_raw_16_[2*w*i+j];
                *pb4++ = g_buf_raw_16_[2*w*i+j+1] & hi_mask;
            }
        }
    }

}


void TestItem::split_channel_raw16_to_raw8(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                                           uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                                           uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_,
                                           int raw_bits)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    distribute_pointer_by_RGB_BAYER(bayer,&pb1,&pb2,&pb3,&pb4,
                                    g_buf_r_16_,g_buf_gr_16_,g_buf_gb_16_,g_buf_b_16_);

    const uchar hi_mask = high_byte_mask_for_raw_bits(raw_bits);

    for (int i=0; i<h; i=i+2)
    {
        for (int j=0; j<2*w; j=j+4)
        {
            *pb1++ = g_buf_raw_16_[2*w*i+j];
            *pb1++ = g_buf_raw_16_[2*w*i+j+1] & hi_mask;

            *pb2++ = g_buf_raw_16_[2*w*i+j+2];
            *pb2++ = g_buf_raw_16_[2*w*i+j+3] & hi_mask;
        }
        for (int j=0; j<2*w; j=j+4)
        {
            *pb3++ = g_buf_raw_16_[2*w*(i+1) +j];
            *pb3++ = g_buf_raw_16_[2*w*(i+1) +j+1] & hi_mask;

            *pb4++ = g_buf_raw_16_[2*w*(i+1) +j+2];
            *pb4++ = g_buf_raw_16_[2*w*(i+1) +j+3] & hi_mask;
        }
    }

    MultiThread::raw16_to_raw8 *p_transfer [Channel_Num];
    for(int i=0; i< Channel_Num; i++)
    {
        p_transfer[i] = new MultiThread::raw16_to_raw8();
    }

    p_transfer[R]->setparam(g_buf_r_16_,g_buf_r_,w/2,h/2, raw_bits);
    p_transfer[Gr]->setparam(g_buf_gr_16_,g_buf_gr_,w/2,h/2, raw_bits);
    p_transfer[Gb]->setparam(g_buf_gb_16_,g_buf_gb_,w/2,h/2, raw_bits);
    p_transfer[B]->setparam(g_buf_b_16_,g_buf_b_,w/2,h/2, raw_bits);
    for(int i=0; i< Channel_Num; i++)
    {
        p_transfer[i]->start();
    }

    for(int i=0; i< Channel_Num; i++)
    {
        p_transfer[i]->wait();
    }

    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,g_buf_raw_,w,h,bayer);

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_transfer[i];
    }
}

void TestItem::split_channel_raw16_to_raw8_4C(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                                              uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                                              uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_,
                                              int raw_bits)
{
    clock_t start_time = clock();
    std::vector<uchar *> pbptr(4);
    distribute_pointer_by_RGB_BAYER(bayer,&pbptr[0],&pbptr[1],&pbptr[2],&pbptr[3],
                                    g_buf_r_16_,g_buf_gr_16_,g_buf_gb_16_,g_buf_b_16_);

    const uchar hi_mask = high_byte_mask_for_raw_bits(raw_bits);

    for (int i=0; i<h; i=i+4)
    {
        for(int k=0;k!=4;++k)
        {
            int ptr_i = 2*(k/2);
            for (int j=0; j<2*w; j=j+8)
            {
                int loca=2*w*(i+k)+j;
                *pbptr[ptr_i]++ = g_buf_raw_16_[loca ];
                *pbptr[ptr_i]++ = g_buf_raw_16_[loca+1] & hi_mask;
                *pbptr[ptr_i]++ = g_buf_raw_16_[loca+2];
                *pbptr[ptr_i]++ = g_buf_raw_16_[loca+3] & hi_mask;
                *pbptr[ptr_i+1]++ = g_buf_raw_16_[loca+4];
                *pbptr[ptr_i+1]++ = g_buf_raw_16_[loca+5] & hi_mask;
                *pbptr[ptr_i+1]++ = g_buf_raw_16_[loca+6];
                *pbptr[ptr_i+1]++ = g_buf_raw_16_[loca+7] & hi_mask;
            }
        }
    }


    clock_t end_time = clock();
    cast_time_step1 =end_time-start_time;

    MultiThread::raw16_to_raw8 *p_transfer [Channel_Num];
    for(int i=0; i< Channel_Num; i++)
    {
        p_transfer[i] = new MultiThread::raw16_to_raw8();
    }
    p_transfer[R]->setparam(g_buf_r_16_,g_buf_r_,w/2,h/2, raw_bits);
    p_transfer[Gr]->setparam(g_buf_gr_16_,g_buf_gr_,w/2,h/2, raw_bits);
    p_transfer[Gb]->setparam(g_buf_gb_16_,g_buf_gb_,w/2,h/2, raw_bits);
    p_transfer[B]->setparam(g_buf_b_16_,g_buf_b_,w/2,h/2, raw_bits);
    for(int i=0; i< Channel_Num; i++)
    {
        p_transfer[i]->start();
    }

    for(int i=0; i< Channel_Num; i++)
    {
        p_transfer[i]->wait();
    }

    start_time = clock();
    AlgorithmFuncs::RGGB_Map_Integrate_4C (g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,g_buf_raw_,w,h,bayer);
    end_time = clock();
    cast_time_step2 =end_time-start_time;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_transfer[i];
    }
}

//该低8位获取的函数,均支持 bit10,12,14,16
void TestItem::split_channel_raw16_to_low_raw8(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                                               uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    distribute_pointer_by_RGB_BAYER(bayer,&pb1,&pb2,&pb3,&pb4,
                                    g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_);

    for (int i=0; i<h; i=i+2)
    {
        for (int j=0; j<2*w; j=j+4)
        {
            *pb1++ = g_buf_raw_16_[2*w*i+j];
            *pb2++ = g_buf_raw_16_[2*w*i+j+2];
        }
        for (int j=0; j<2*w; j=j+4)
        {
            *pb3++ = g_buf_raw_16_[2*w*(i+1) +j];
            *pb4++ = g_buf_raw_16_[2*w*(i+1) +j+2];
        }
    }

    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,g_buf_raw_,w,h,bayer);

}



void TestItem::split_4C_same_channel_1_to_4_raw8(int w, int h, uchar *g_buf_, uchar *g_buf_1_, uchar *g_buf_2_,
                                                 uchar *g_buf_3_, uchar *g_buf_4_)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;

    pb1 = g_buf_1_;
    pb2 = g_buf_2_;
    pb3 = g_buf_3_;
    pb4 = g_buf_4_;


    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (i%2 == 0 && j%2 == 0)
                *pb1++ = g_buf_[w*i+j];
            else if(i%2 == 0 && j%2 == 1)
                *pb2++ = g_buf_[w*i+j];
            else if(i%2 == 1 && j%2 == 0)
                *pb3++ = g_buf_[w*i+j];
            else if(i%2 == 1 && j%2 == 1)
                *pb4++ = g_buf_[w*i+j];
        }
    }
}


void TestItem::split_4C_same_channel_1_to_4_raw16(int w, int h, uchar *g_buf_, uchar *g_buf_1_, uchar *g_buf_2_,
                                                  uchar *g_buf_3_, uchar *g_buf_4_, int raw_bits)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;

    pb1 = g_buf_1_;
    pb2 = g_buf_2_;
    pb3 = g_buf_3_;
    pb4 = g_buf_4_;

    const uchar hi_mask = high_byte_mask_for_raw_bits(raw_bits);

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<2*w; j+=2)
        {
            int l=j/2;
            if (i%2 == 0 && l%2 == 0)
            {
                *pb1++ = g_buf_[2*w*i+j];
                *pb1++ = g_buf_[2*w*i+j+1] & hi_mask;
            }
            else if(i%2 == 0 && l%2 == 1)
            {
                *pb2++ = g_buf_[2*w*i+j];
                *pb2++ = g_buf_[2*w*i+j+1] & hi_mask;
            }
            else if(i%2 == 1 && l%2 == 0)
            {
                *pb3++ = g_buf_[2*w*i+j];
                *pb3++ = g_buf_[2*w*i+j+1] & hi_mask;
            }
            else if(i%2 == 1 && l%2 == 1)
            {
                *pb4++ = g_buf_[2*w*i+j];
                *pb4++ = g_buf_[2*w*i+j+1] & hi_mask;
            }
        }
    }
}


void TestItem::split_channel_short(RGB_BAYER bayer,int w, int h, short *g_buf_raw_, short *g_buf_r_, short *g_buf_gr_, short *g_buf_gb_, short *g_buf_b_)
{
    short *pb1 = NULL;
    short *pb2 = NULL;
    short *pb3 = NULL;
    short *pb4 = NULL;
    if (bayer == RG)
    {
        pb1 = g_buf_r_;
        pb2 = g_buf_gr_;
        pb3 = g_buf_gb_;
        pb4 = g_buf_b_;
    }
    else if (bayer == BG)
    {
        pb1 = g_buf_b_;
        pb2 = g_buf_gb_;
        pb3 = g_buf_gr_;
        pb4 = g_buf_r_;
    }
    else if (bayer == GR)
    {
        pb1 = g_buf_gr_;
        pb2 = g_buf_r_;
        pb3 = g_buf_b_;
        pb4 = g_buf_gb_;
    }
    else if (bayer == GB)
    {
        pb1 = g_buf_gb_;
        pb2 = g_buf_b_;
        pb3 = g_buf_r_;
        pb4 = g_buf_gr_;
    }
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (i%2 == 0 && j%2 == 0)
                *pb1++ = g_buf_raw_[w*i+j];
            else if(i%2 == 0 && j%2 == 1)
                *pb2++ = g_buf_raw_[w*i+j];
            else if(i%2 == 1 && j%2 == 0)
                *pb3++ = g_buf_raw_[w*i+j];
            else if(i%2 == 1 && j%2 == 1)
                *pb4++ = g_buf_raw_[w*i+j];
        }
    }
}


void TestItem::split_channel_raw16_multiframe(RGB_BAYER bayer,int w, int h, int frame_cnt, uchar *g_buf_raw_16_,
                                              uchar *g_buf_r_16_multiframe_, uchar *g_buf_gr_16_multiframe_,
                                              uchar *g_buf_gb_16_multiframe_, uchar *g_buf_b_16_multiframe_,
                                              int raw_bits)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    if (bayer == RG)
    {
        pb1 = g_buf_r_16_multiframe_;
        pb2 = g_buf_gr_16_multiframe_;
        pb3 = g_buf_gb_16_multiframe_;
        pb4 = g_buf_b_16_multiframe_;
    }
    else if (bayer == BG)
    {
        pb1 = g_buf_b_16_multiframe_;
        pb2 = g_buf_gb_16_multiframe_;
        pb3 = g_buf_gr_16_multiframe_;
        pb4 = g_buf_r_16_multiframe_;
    }
    else if (bayer == GR)
    {
        pb1 = g_buf_gr_16_multiframe_;
        pb2 = g_buf_r_16_multiframe_;
        pb3 = g_buf_b_16_multiframe_;
        pb4 = g_buf_gb_16_multiframe_;
    }
    else if (bayer == GB)
    {
        pb1 = g_buf_gb_16_multiframe_;
        pb2 = g_buf_b_16_multiframe_;
        pb3 = g_buf_r_16_multiframe_;
        pb4 = g_buf_gr_16_multiframe_;
    }
    const uchar hi_mask = high_byte_mask_for_raw_bits(raw_bits);
    for (int cnt=0; cnt<frame_cnt; cnt++)
    {
        for (int i=0; i<h; ++i)
        {
            for (int j=0; j<2*w; j+=2)
            {
                int l=j/2;
                if (i%2 == 0 && l%2 == 0)
                {
                    *pb1++ = g_buf_raw_16_[2*w*i+j+cnt*2*w*h];
                    *pb1++ = g_buf_raw_16_[2*w*i+j+1+cnt*2*w*h] & hi_mask;
                }
                else if(i%2 == 0 && l%2 == 1)
                {
                    *pb2++ = g_buf_raw_16_[2*w*i+j+cnt*2*w*h];
                    *pb2++ = g_buf_raw_16_[2*w*i+j+1+cnt*2*w*h] & hi_mask;
                }
                else if(i%2 == 1 && l%2 == 0)
                {
                    *pb3++ = g_buf_raw_16_[2*w*i+j+cnt*2*w*h];
                    *pb3++ = g_buf_raw_16_[2*w*i+j+1+cnt*2*w*h] & hi_mask;
                }
                else if(i%2 == 1 && l%2 == 1)
                {
                    *pb4++ = g_buf_raw_16_[2*w*i+j+cnt*2*w*h];
                    *pb4++ = g_buf_raw_16_[2*w*i+j+1+cnt*2*w*h] & hi_mask;
                }
            }
        }
    }
}

void TestItem::split_channel_raw10_to_raw16_multiframe(RGB_BAYER bayer,int width, int height, int frame_cnt, uchar *g_buf_raw_10_,
                                                       uchar *g_buf_r_16_multiframe_, uchar *g_buf_gr_16_multiframe_,
                                                       uchar *g_buf_gb_16_multiframe_, uchar *g_buf_b_16_multiframe_)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    int w = width*5/4;
    int h = height;
    if (bayer == RG)
    {
        pb1 = g_buf_r_16_multiframe_;
        pb2 = g_buf_gr_16_multiframe_;
        pb3 = g_buf_gb_16_multiframe_;
        pb4 = g_buf_b_16_multiframe_;
    }
    else if (bayer == BG)
    {
        pb1 = g_buf_b_16_multiframe_;
        pb2 = g_buf_gb_16_multiframe_;
        pb3 = g_buf_gr_16_multiframe_;
        pb4 = g_buf_r_16_multiframe_;
    }
    else if (bayer == GR)
    {
        pb1 = g_buf_gr_16_multiframe_;
        pb2 = g_buf_r_16_multiframe_;
        pb3 = g_buf_b_16_multiframe_;
        pb4 = g_buf_gb_16_multiframe_;
    }
    else if (bayer == GB)
    {
        pb1 = g_buf_gb_16_multiframe_;
        pb2 = g_buf_b_16_multiframe_;
        pb3 = g_buf_r_16_multiframe_;
        pb4 = g_buf_gr_16_multiframe_;
    }
    for (int cnt=0; cnt<frame_cnt; cnt++)
    {
        for (int i=0; i<h; ++i)
        {
            for (int j=0; j<w; ++j)
            {
                int offset = (j+1)%5;
                if (offset==0)
                    continue;
                int low2bit_pos= (j/5 + 1)*5-1;
                BYTE low2bit;
                if (offset==1)
                    low2bit = g_buf_raw_10_[w*i+low2bit_pos+cnt*w*h] & 0x03;
                else if (offset==2)
                    low2bit = (g_buf_raw_10_[w*i+low2bit_pos+cnt*w*h] & 0x0C)>>2;
                else if (offset==3)
                    low2bit = (g_buf_raw_10_[w*i+low2bit_pos+cnt*w*h] & 0x30)>>4;
                else if (offset==4)
                    low2bit = (g_buf_raw_10_[w*i+low2bit_pos+cnt*w*h] & 0xC0)>>6;

                int m = j-j/5;

                if (i%2 == 0 && m%2 == 0)
                {
                    *pb1++ = ((g_buf_raw_10_[w*i+j+cnt*w*h] & 0x3f)<<2) | (low2bit & 0x03);
                    *pb1++ = (g_buf_raw_10_[w*i+j+cnt*w*h] & 0xC0)>>6;
                }
                else if(i%2 == 0 && m%2 == 1)
                {
                    *pb2++ = ((g_buf_raw_10_[w*i+j+cnt*w*h] & 0x3f)<<2) | (low2bit & 0x03);
                    *pb2++ = (g_buf_raw_10_[w*i+j+cnt*w*h] & 0xC0)>>6;
                }
                else if(i%2 == 1 && m%2 == 0)
                {
                    *pb3++ = ((g_buf_raw_10_[w*i+j+cnt*w*h] & 0x3f)<<2) | (low2bit & 0x03);
                    *pb3++ = (g_buf_raw_10_[w*i+j+cnt*w*h] & 0xC0)>>6;
                }
                else if(i%2 == 1 && m%2 == 1)
                {
                    *pb4++ = ((g_buf_raw_10_[w*i+j+cnt*w*h] & 0x3f)<<2) | (low2bit & 0x03);
                    *pb4++ = (g_buf_raw_10_[w*i+j+cnt*w*h] & 0xC0)>>6;
                }
            }
        }
    }
}

void TestItem::test_channel_colmean_max_min(int width, int height,double &mean_r_max, double &mean_gr_max, double &mean_gb_max, double &mean_b_max,
                                            double &mean_r_min, double &mean_gr_min, double &mean_gb_min, double &mean_b_min,
                                            int &mean_r_max_pos, int &mean_gr_max_pos, int &mean_gb_max_pos, int &mean_b_max_pos,
                                            int &mean_r_min_pos, int &mean_gr_min_pos, int &mean_gb_min_pos, int &mean_b_min_pos,
                                            double &pixel_r_max,double &pixel_gr_max,double &pixel_gb_max,double &pixel_b_max,double &mean_center_left,double &mean_center_right,
                                            uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_)
{
    MultiThread::get_roi_channel_col_mean_min *p_response[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i] = new MultiThread::get_roi_channel_col_mean_min();
    }


    p_response[R]->setparam(g_buf_r_,width/2,height/2);
    p_response[Gr]->setparam(g_buf_gr_,width/2,height/2);
    p_response[Gb]->setparam(g_buf_gb_,width/2,height/2);
    p_response[B]->setparam(g_buf_b_,width/2,height/2);

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->wait();
    }

    mean_r_max = p_response[R]->mean_max;
    mean_gr_max = p_response[Gr]->mean_max;
    mean_gb_max = p_response[Gb]->mean_max;
    mean_b_max = p_response[B]->mean_max;
    mean_r_max_pos = p_response[R]->max_pos;
    mean_gr_max_pos = p_response[Gr]->max_pos;
    mean_gb_max_pos = p_response[Gb]->max_pos;
    mean_b_max_pos = p_response[B]->max_pos;
    mean_r_min = p_response[R]->mean_min;
    mean_gr_min = p_response[Gr]->mean_min;
    mean_gb_min = p_response[Gb]->mean_min;
    mean_b_min = p_response[B]->mean_min;
    mean_r_min_pos = p_response[R]->min_pos;
    mean_gr_min_pos = p_response[Gr]->min_pos;
    mean_gb_min_pos = p_response[Gb]->min_pos;
    mean_b_min_pos = p_response[B]->min_pos;
    pixel_r_max = p_response[R]->pixel_max;
    pixel_gr_max = p_response[GR]->pixel_max;
    pixel_gb_max = p_response[GB]->pixel_max;
    pixel_b_max = p_response[B]->pixel_max;
    double mean_left_r = p_response[R]->mean_center_left;
    double mean_left_gr = p_response[GR]->mean_center_left;
    double mean_left_gb = p_response[GB]->mean_center_left;
    double mean_left_b = p_response[B]->mean_center_left;
    double mean_right_r = p_response[R]->mean_center_right;
    double mean_right_gr = p_response[GR]->mean_center_right;
    double mean_right_gb = p_response[GB]->mean_center_right;
    double mean_right_b = p_response[B]->mean_center_right;
    mean_center_left = (mean_left_r+mean_left_gr+mean_left_gb+mean_left_b)/4;
    mean_center_right = (mean_right_r+mean_right_gr+mean_right_gb+mean_right_b)/4;


    for(int i=0; i< Channel_Num; i++)
    {
        delete p_response[i];
    }
}


void TestItem::test_channel_mean(int w, int h, double &mean_r, double &mean_gr, double &mean_gb, double &mean_b,
                                 uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_)
{
    MultiThread::get_channel_mean *p_response[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i] = new MultiThread::get_channel_mean();
    }
    p_response[R]->setparam(g_buf_r_,w/2,h/2);
    p_response[Gr]->setparam(g_buf_gr_,w/2,h/2);
    p_response[Gb]->setparam(g_buf_gb_,w/2,h/2);
    p_response[B]->setparam(g_buf_b_,w/2,h/2);

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->wait();
    }

    mean_r = p_response[R]->mean;
    mean_gr = p_response[Gr]->mean;
    mean_gb = p_response[Gb]->mean;
    mean_b = p_response[B]->mean;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_response[i];
    }
}


void TestItem::test_roi_channel_mean(int width, int height, int win_x, int win_y, int win_w, int win_h, double &mean_r, double &mean_gr, double &mean_gb, double &mean_b,
                                 uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_)
{
    MultiThread::get_roi_channel_mean *p_response[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i] = new MultiThread::get_roi_channel_mean();
    }
    p_response[R]->setparam(g_buf_r_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_response[Gr]->setparam(g_buf_gr_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_response[Gb]->setparam(g_buf_gb_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_response[B]->setparam(g_buf_b_,width/2,height/2,win_x,win_y,win_w,win_h);

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->wait();
    }

    mean_r = p_response[R]->mean;
    mean_gr = p_response[Gr]->mean;
    mean_gb = p_response[Gb]->mean;
    mean_b = p_response[B]->mean;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_response[i];
    }
}

void TestItem::test_channel_linearity_col_ushort(int width, int height, int win_x, int win_y, int win_w, int win_h, double* linearity, double max_dn,
                                 uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_)
{
    //double linearity[12] = {0};
    MultiThread::get_mutil_roi_linearity_col_ushort *p_response[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i] = new MultiThread::get_mutil_roi_linearity_col_ushort();
    }
    p_response[R]->setparam((ushort *)g_buf_r_16_,width/2,height/2,win_x,win_y,win_w,win_h, linearity, max_dn);
    p_response[Gr]->setparam((ushort *)g_buf_gr_16_,width/2,height/2,win_x,win_y,win_w,win_h,linearity+4, max_dn);
    p_response[Gb]->setparam((ushort *)g_buf_gb_16_,width/2,height/2,win_x,win_y,win_w,win_h,linearity+8, max_dn);
    p_response[B]->setparam((ushort *)g_buf_b_16_,width/2,height/2,win_x,win_y,win_w,win_h,linearity+12, max_dn);

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->wait();
    }

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_response[i];
    }
}


void TestItem::test_MxN_roi_std_mean(uchar *g_buf_raw_, int width, int height,
                                     int skip_col, int skip_row, int Col_M, int Row_N,
                                     QVector <double> &mean, QVector <double> &std)
{
    QVector <cv::Rect> roi_rect;
    int step_col = (width-2*skip_col)/Col_M;
    int step_row = (height-2*skip_row)/Row_N;

    for (int i=0; i<Row_N; ++i)
    {
        int y=skip_row+i*step_row;
        for(int j=0; j<Col_M; ++j)
        {
            int x=skip_col+j*step_col;
            roi_rect.append(cv::Rect(x,y,step_col,step_row));
        }
    }

    int thread_num =roi_rect.size();
    vector<MultiThread::get_roi_mean_std_by_rect *> p_roi(thread_num);
//    MultiThread::get_roi_mean_std_by_rect *p_roi[thread_num];
    for (int i=0; i < thread_num; i++)
    {
        p_roi[i] = new MultiThread::get_roi_mean_std_by_rect();
    }

    for (int i=0; i < thread_num; i++)
    {
        p_roi[i]->setparam(g_buf_raw_,roi_rect[i],width,height);
    }

    for (int i=0; i < thread_num; i++)
    {
        p_roi[i]->start();
    }

    for (int i=0; i < thread_num; i++)
    {
        p_roi[i]->wait();
    }

    for (int i=0; i < thread_num; i++)
    {
        mean.append(p_roi[i]->mean);
        std.append(p_roi[i]->std);
    }

    for(int i=0; i< thread_num; i++)
    {
        delete p_roi[i];
    }
}


void TestItem::test_roi_channel_pixel_percent_DN_raw8(int width, int height, cv::Rect &roi,
                                                      uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                                      double r_x_percent, double gr_x_percent, double gb_x_percent, double b_x_percent,
                                                      int &r_x_dn,int &gr_x_dn,int &gb_x_dn,int &b_x_dn,
                                                      int log_print, std::string &msg)
{


    MultiThread::get_roi_channel_pixel_percent_DN_raw8 *p_percent[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i] = new MultiThread::get_roi_channel_pixel_percent_DN_raw8();
    }
    p_percent[R]->setparam(g_buf_r_, width/2, height/2, r_x_percent, roi.x, roi.y, roi.width, roi.height);
    p_percent[Gr]->setparam(g_buf_gr_,width/2, height/2, gr_x_percent, roi.x, roi.y, roi.width, roi.height);
    p_percent[Gb]->setparam(g_buf_gb_, width/2, height/2, gb_x_percent, roi.x, roi.y, roi.width, roi.height);
    p_percent[B]->setparam(g_buf_b_, width/2, height/2, b_x_percent, roi.x, roi.y, roi.width, roi.height);

    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i]->wait();
    }

    r_x_dn = p_percent[R]->x_pos_dn;
    gr_x_dn = p_percent[Gr]->x_pos_dn;
    gb_x_dn = p_percent[Gb]->x_pos_dn;
    b_x_dn = p_percent[B]->x_pos_dn;


    msg+=boost::str(boost::format("R channel %.2f percent pixel corresponding DN:%d\n"
                                  "Gr channel %.2f percent pixel corresponding DN:%d\n"
                                  "Gb channel %.2f percent pixel corresponding DN:%d\n"
                                  "B channel %.2f percent pixel corresponding DN:%d\n")
                                % r_x_percent % r_x_dn
                                % gr_x_percent % gr_x_dn
                                % gb_x_percent % gb_x_dn
                                % b_x_percent % b_x_dn );

    if(log_print)
    {
        QFile f1("./Histogram_R_log.txt");
        if(f1.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f1);
            for(int i=0; i<256; i++)
            {
                str<<i<<"   "<<QString::number(p_percent[R]->histogram[i]).toStdString().c_str();
                str<<"\n";
            }
            f1.close();
        }

        QFile f2("./Histogram_Gr_log.txt");
        if(f2.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f2);
            for(int i=0; i<256; i++)
            {
                str<<i<<"   "<<QString::number(p_percent[Gr]->histogram[i]).toStdString().c_str();
                str<<"\n";
            }
            f2.close();
        }

        QFile f3("./Histogram_Gb_log.txt");
        if(f3.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f3);
            for(int i=0; i<256; i++)
            {
                str<<i<<"   "<<QString::number(p_percent[Gb]->histogram[i]).toStdString().c_str();
                str<<"\n";
            }
            f3.close();
        }

        QFile f4("./Histogram_B_log.txt");
        if(f4.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f4);
            for(int i=0; i<256; i++)
            {
                str<<i<<"   "<<QString::number(p_percent[B]->histogram[i]).toStdString().c_str();
                str<<"\n";
            }
            f4.close();
        }
    }

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_percent[i];
    }
}





void TestItem::test_roi_channel_pixel_percent_DN_short(int width, int height, cv::Rect &roi,
                                                       short *g_buf_r_, short *g_buf_gr_, short *g_buf_gb_, short *g_buf_b_,
                                                       double r_x_percent, double gr_x_percent, double gb_x_percent, double b_x_percent,
                                                       int &r_x_dn,int &gr_x_dn,int &gb_x_dn,int &b_x_dn,
                                                       int log_print, std::string &msg)
{


    MultiThread::get_roi_channel_pixel_percent_DN_short *p_percent[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i] = new MultiThread::get_roi_channel_pixel_percent_DN_short();
    }
    p_percent[R]->setparam(g_buf_r_, width/2, height/2, r_x_percent, roi.x, roi.y, roi.width, roi.height);
    p_percent[Gr]->setparam(g_buf_gr_,width/2, height/2, gr_x_percent, roi.x, roi.y, roi.width, roi.height);
    p_percent[Gb]->setparam(g_buf_gb_, width/2, height/2, gb_x_percent, roi.x, roi.y, roi.width, roi.height);
    p_percent[B]->setparam(g_buf_b_, width/2, height/2, b_x_percent, roi.x, roi.y, roi.width, roi.height);

    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i]->wait();
    }

    r_x_dn = p_percent[R]->x_pos_dn;
    gr_x_dn = p_percent[Gr]->x_pos_dn;
    gb_x_dn = p_percent[Gb]->x_pos_dn;
    b_x_dn = p_percent[B]->x_pos_dn;


    msg+=boost::str(boost::format("R channel %.2f percent pixel corresponding DN:%d\n"
                                  "Gr channel %.2f percent pixel corresponding DN:%d\n"
                                  "Gb channel %.2f percent pixel corresponding DN:%d\n"
                                  "B channel %.2f percent pixel corresponding DN:%d\n")
                                % r_x_percent % r_x_dn
                                % gr_x_percent % gr_x_dn
                                % gb_x_percent % gb_x_dn
                                % b_x_percent % b_x_dn );

    if(log_print)
    {
        QFile f1("./Histogram_R_log.txt");
        if(f1.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f1);
            for(int i=0; i<2047; i++)
            {
                str<<i-1023<<"   "<<QString::number(p_percent[R]->histogram[i]).toStdString().c_str();
                str<<"\n";
            }
            f1.close();
        }

        QFile f2("./Histogram_Gr_log.txt");
        if(f2.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f2);
            for(int i=0; i<2047; i++)
            {
                str<<i-1023<<"   "<<QString::number(p_percent[Gr]->histogram[i]).toStdString().c_str();
                str<<"\n";
            }
            f2.close();
        }

        QFile f3("./Histogram_Gb_log.txt");
        if(f3.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f3);
            for(int i=0; i<2047; i++)
            {
                str<<i-1023<<"   "<<QString::number(p_percent[Gb]->histogram[i]).toStdString().c_str();
                str<<"\n";
            }
            f3.close();
        }

        QFile f4("./Histogram_B_log.txt");
        if(f4.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f4);
            for(int i=0; i<2047; i++)
            {
                str<<i-1023<<"   "<<QString::number(p_percent[B]->histogram[i]).toStdString().c_str();
                str<<"\n";
            }
            f4.close();
        }
    }

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_percent[i];
    }
}



void TestItem::test_roi_channel_pixel_percent_DN_raw16(int width, int height, cv::Rect &roi,
                                                      short *g_buf_r_, short *g_buf_gr_, short *g_buf_gb_, short *g_buf_b_,
                                                      double r_x_percent, double gr_x_percent, double gb_x_percent, double b_x_percent,
                                                      int &r_x_dn,int &gr_x_dn,int &gb_x_dn,int &b_x_dn, std::string &msg)
{

    int w = roi.br().x -roi.x +1;
    int h = roi.br().y -roi.y +1;
    logMsg ("Mark1");
    logMsg (QString("w:%1").arg(QString::number(w)).toStdString().c_str());
    logMsg (QString("h:%1").arg(QString::number(h)).toStdString().c_str());


    short *g_roi_ = new short[4*w*h];
    short *g_roi_r_ = g_roi_;
    short *g_roi_gr_ = g_roi_ + 1*w*h;
    short *g_roi_gb_ = g_roi_ + 2*w*h;
    short *g_roi_b_ = g_roi_ + 3*w*h;

    short *g_r_ = g_roi_r_;
    short *g_gr_ = g_roi_gr_;
    short *g_gb_ = g_roi_gb_;
    short *g_b_ = g_roi_b_;


    for (int i=roi.y; i<=roi.br().y; i++)
        for (int j=roi.x; j<=roi.br().x; j++)
        {
            *g_r_++ = g_buf_r_[i*width+j];
            *g_gr_++ = g_buf_gr_[i*width+j];
            *g_gb_++ = g_buf_gb_[i*width+j];
            *g_b_++ = g_buf_b_[i*width+j];
        }

    logMsg ("Mark2");


    MultiThread::get_roi_channel_pixel_percent_DN_raw16 *p_percent[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i] = new MultiThread::get_roi_channel_pixel_percent_DN_raw16();
    }
    p_percent[R]->setparam(g_roi_r_, w, h, r_x_percent);
    p_percent[Gr]->setparam(g_roi_gr_, w, h, gr_x_percent);
    p_percent[Gb]->setparam(g_roi_gb_, w, h, gb_x_percent);
    p_percent[B]->setparam(g_roi_b_, w, h, b_x_percent);

    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_percent[i]->wait();
    }
    logMsg ("Mark3");

    r_x_dn = p_percent[R]->x_pos_dn;
    gr_x_dn = p_percent[Gr]->x_pos_dn;
    gb_x_dn = p_percent[Gb]->x_pos_dn;
    b_x_dn = p_percent[B]->x_pos_dn;


    msg+=boost::str(boost::format("R channel %.2f percent pixel corresponding DN:%d\n"
                                  "Gr channel %.2f percent pixel corresponding DN:%d\n"
                                  "Gb channel %.2f percent pixel corresponding DN:%d\n"
                                  "B channel %.2f percent pixel corresponding DN:%d\n")
                                % r_x_percent % r_x_dn
                                % gr_x_percent % gr_x_dn
                                % gb_x_percent % gb_x_dn
                                % b_x_percent % b_x_dn );

    logMsg ("Mark4");

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_percent[i];
    }
    delete [] g_roi_;
}



void TestItem::test_channel_mean_raw16(int w, int h, double &mean_r, double &mean_gr, double &mean_gb, double &mean_b,
                                       uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_)
{
    MultiThread::get_channel_mean_raw16 *p_response[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i] = new MultiThread::get_channel_mean_raw16();
    }
    p_response[R]->setparam(g_buf_r_16_,w/2,h/2);
    p_response[Gr]->setparam(g_buf_gr_16_,w/2,h/2);
    p_response[Gb]->setparam(g_buf_gb_16_,w/2,h/2);
    p_response[B]->setparam(g_buf_b_16_,w/2,h/2);

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->wait();
    }

    mean_r = p_response[R]->mean;
    mean_gr = p_response[Gr]->mean;
    mean_gb = p_response[Gb]->mean;
    mean_b = p_response[B]->mean;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_response[i];
    }
}


void TestItem::test_roi_channel_mean_raw16(int width, int height, int win_x, int win_y, int win_w, int win_h,
                                           double &mean_r, double &mean_gr, double &mean_gb, double &mean_b,
                                           uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_)
{
    MultiThread::get_roi_channel_mean_raw16 *p_response[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i] = new MultiThread::get_roi_channel_mean_raw16();
    }
    p_response[R]->setparam((unsigned short *)g_buf_r_16_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_response[Gr]->setparam((unsigned short *)g_buf_gr_16_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_response[Gb]->setparam((unsigned short *)g_buf_gb_16_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_response[B]->setparam((unsigned short *)g_buf_b_16_,width/2,height/2,win_x,win_y,win_w,win_h);

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_response[i]->wait();
    }

    mean_r = p_response[R]->mean;
    mean_gr = p_response[Gr]->mean;
    mean_gb = p_response[Gb]->mean;
    mean_b = p_response[B]->mean;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_response[i];
    }
}



void TestItem::test_real_roi_prnu(int width, int height, int win_x, int win_y, int win_w, int win_h,
                                  double &prnu_r, double &prnu_gr, double &prnu_gb, double &prnu_b,
                                  float *g_delta_r_, float *g_delta_gr_, float *g_delta_gb_, float *g_delta_b_,
                                 uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_)
{
    MultiThread::get_real_roi_prnu *p_roi_prnu[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_roi_prnu[i] = new MultiThread::get_real_roi_prnu();
    }
    p_roi_prnu[R]->setparam(g_delta_r_,g_buf_r_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_roi_prnu[Gr]->setparam(g_delta_gr_,g_buf_gr_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_roi_prnu[Gb]->setparam(g_delta_gb_,g_buf_gb_,width/2,height/2,win_x,win_y,win_w,win_h);
    p_roi_prnu[B]->setparam(g_delta_b_,g_buf_b_,width/2,height/2,win_x,win_y,win_w,win_h);

    for (int i=0; i < Channel_Num; i++)
    {
        p_roi_prnu[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_roi_prnu[i]->wait();
    }

    prnu_r = p_roi_prnu[R]->prnu;
    prnu_gr = p_roi_prnu[Gr]->prnu;
    prnu_gb = p_roi_prnu[Gb]->prnu;
    prnu_b = p_roi_prnu[B]->prnu;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_roi_prnu[i];
    }
}



void TestItem::test_channel_prnu(int w, int h, int w_cell, int h_cell,
                                 double &prnu_r, double &prnu_gr, double &prnu_gb, double &prnu_b,
                                 uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_)
{
    MultiThread::get_channel_uniformity *p_uniformity[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_uniformity[i] = new MultiThread::get_channel_uniformity();
    }
    p_uniformity[R]->setparam(g_buf_r_,w/2,h/2,w_cell/2,h_cell/2);
    p_uniformity[Gr]->setparam(g_buf_gr_,w/2,h/2,w_cell/2,h_cell/2);
    p_uniformity[Gb]->setparam(g_buf_gb_,w/2,h/2,w_cell/2,h_cell/2);
    p_uniformity[B]->setparam(g_buf_b_,w/2,h/2,w_cell/2,h_cell/2);

    for (int i=0; i < Channel_Num; i++)
    {
        p_uniformity[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_uniformity[i]->wait();
    }

    prnu_r = p_uniformity[R]->prnu;
    prnu_gr = p_uniformity[Gr]->prnu;
    prnu_gb = p_uniformity[Gb]->prnu;
    prnu_b = p_uniformity[B]->prnu;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_uniformity[i];
    }

}


void TestItem::test_channel_uniformity_max(int w, int h, int w_cell, int h_cell,
                                           double &prnu_r, double &prnu_gr, double &prnu_gb, double &prnu_b,
                                           double &prnu_r_max, double &prnu_gr_max, double &prnu_gb_max, double &prnu_b_max,
                                           int &r_max_pos, int &gr_max_pos, int &gb_max_pos, int &b_max_pos,
                                           uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, std::string &msg)
{
    MultiThread::get_channel_uniformity_max *p_uniformity_max[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_uniformity_max[i] = new MultiThread::get_channel_uniformity_max();
    }
    p_uniformity_max[R]->setparam(g_buf_r_,w/2,h/2,w_cell/2,h_cell/2);
    p_uniformity_max[Gr]->setparam(g_buf_gr_,w/2,h/2,w_cell/2,h_cell/2);
    p_uniformity_max[Gb]->setparam(g_buf_gb_,w/2,h/2,w_cell/2,h_cell/2);
    p_uniformity_max[B]->setparam(g_buf_b_,w/2,h/2,w_cell/2,h_cell/2);

    for (int i=0; i < Channel_Num; i++)
    {
        p_uniformity_max[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_uniformity_max[i]->wait();
    }

    prnu_r = p_uniformity_max[R]->prnu;
    prnu_gr = p_uniformity_max[Gr]->prnu;
    prnu_gb = p_uniformity_max[Gb]->prnu;
    prnu_b = p_uniformity_max[B]->prnu;

    prnu_r_max = p_uniformity_max[R]->prnu_eachcell_max;
    prnu_gr_max = p_uniformity_max[Gr]->prnu_eachcell_max;
    prnu_gb_max = p_uniformity_max[Gb]->prnu_eachcell_max;
    prnu_b_max = p_uniformity_max[B]->prnu_eachcell_max;

    r_max_pos = p_uniformity_max[R]->max_cell_pos;
    gr_max_pos = p_uniformity_max[Gr]->max_cell_pos;
    gb_max_pos = p_uniformity_max[Gb]->max_cell_pos;
    b_max_pos = p_uniformity_max[B]->max_cell_pos;

    logMsg (std::string("Mark1"));

//    int m = w/w_cell;
//    int n = h/h_cell;

//    msg += boost::str(boost::format("--------------uniformity-R------------------\n"));

//    for (int i=0; i<n; i++)
//    {
//        for(int j=0; j<m; j++)
//        {
//            msg += boost::str(boost::format("%6.3f   ")
//                              % (p_uniformity_max[R]->prnu_eachcell.at(i*m+j)));
//        }
//        msg += boost::str(boost::format("\n"));
//    }

//    msg += boost::str(boost::format("--------------uniformity-Gr------------------\n"));

//    for (int i=0; i<n; i++)
//    {
//        for(int j=0; j<m; j++)
//        {
//            msg += boost::str(boost::format("%6.3f   ")
//                              % (p_uniformity_max[Gr]->prnu_eachcell.at(i*m+j)));
//        }
//        msg += boost::str(boost::format("\n"));
//    }

//    msg += boost::str(boost::format("--------------uniformity-Gb------------------\n"));

//    for (int i=0; i<n; i++)
//    {
//        for(int j=0; j<m; j++)
//        {
//            msg += boost::str(boost::format("%6.3f   ")
//                              % (p_uniformity_max[Gb]->prnu_eachcell.at(i*m+j)));
//        }
//        msg += boost::str(boost::format("\n"));
//    }

//    msg += boost::str(boost::format("--------------uniformity-B------------------\n"));

//    for (int i=0; i<n; i++)
//    {
//        for(int j=0; j<m; j++)
//        {
//            msg += boost::str(boost::format("%6.3f   ")
//                              % (p_uniformity_max[B]->prnu_eachcell.at(i*m+j)));
//        }
//        msg += boost::str(boost::format("\n"));
//    }



    for(int i=0; i< Channel_Num; i++)
    {
        delete p_uniformity_max[i];
    }
}




void TestItem::test_non_uniformity_bychannel(int w, int h, int w_cell, int h_cell,
                                             double &n_uniform_r, double &n_uniform_gr, double &n_uniform_gb, double &n_uniform_b,
                                             double &n_uniform_r_max, double &n_uniform_gr_max,
                                             double &n_uniform_gb_max, double &n_uniform_b_max,
                                             int &r_max_pos, int &gr_max_pos, int &gb_max_pos, int &b_max_pos,
                                             double &n_uniform_mirror_diff_r_max, double &n_uniform_mirror_diff_gr_max,
                                             double &n_uniform_mirror_diff_gb_max, double &n_uniform_mirror_diff_b_max,
                                             int &mirror_diff_r_max_pos, int &mirror_diff_gr_max_pos,
                                             int &mirror_diff_gb_max_pos, int &mirror_diff_b_max_pos,
                                             double &n_uniform_updown_diff_r_max, double &n_uniform_updown_diff_gr_max,
                                             double &n_uniform_updown_diff_gb_max, double &n_uniform_updown_diff_b_max,
                                             int &updown_diff_r_max_pos, int &updown_diff_gr_max_pos,
                                             int &updown_diff_gb_max_pos, int &updown_diff_b_max_pos,
                                             uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                             std::string &msg, int flag_log_print)
{
    MultiThread::get_non_uniformity_bychannel *p_non_uniformity[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_non_uniformity[i] = new MultiThread::get_non_uniformity_bychannel();
    }
    p_non_uniformity[R]->setparam(g_buf_r_,w/2,h/2,w_cell/2,h_cell/2);
    p_non_uniformity[Gr]->setparam(g_buf_gr_,w/2,h/2,w_cell/2,h_cell/2);
    p_non_uniformity[Gb]->setparam(g_buf_gb_,w/2,h/2,w_cell/2,h_cell/2);
    p_non_uniformity[B]->setparam(g_buf_b_,w/2,h/2,w_cell/2,h_cell/2);

    for (int i=0; i < Channel_Num; i++)
    {
        p_non_uniformity[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_non_uniformity[i]->wait();
    }

    n_uniform_r = p_non_uniformity[R]->n_uniform_;
    n_uniform_gr = p_non_uniformity[Gr]->n_uniform_;
    n_uniform_gb = p_non_uniformity[Gb]->n_uniform_;
    n_uniform_b = p_non_uniformity[B]->n_uniform_;

    n_uniform_r_max = p_non_uniformity[R]->n_uniform_eachcell_max_;
    n_uniform_gr_max = p_non_uniformity[Gr]->n_uniform_eachcell_max_;
    n_uniform_gb_max = p_non_uniformity[Gb]->n_uniform_eachcell_max_;
    n_uniform_b_max = p_non_uniformity[B]->n_uniform_eachcell_max_;

    r_max_pos = p_non_uniformity[R]->max_cell_pos_;
    gr_max_pos = p_non_uniformity[Gr]->max_cell_pos_;
    gb_max_pos = p_non_uniformity[Gb]->max_cell_pos_;
    b_max_pos = p_non_uniformity[B]->max_cell_pos_;


    n_uniform_mirror_diff_r_max = p_non_uniformity[R]->n_uniform_mirror_diff_eachcell_max_;
    n_uniform_mirror_diff_gr_max = p_non_uniformity[Gr]->n_uniform_mirror_diff_eachcell_max_;
    n_uniform_mirror_diff_gb_max = p_non_uniformity[Gb]->n_uniform_mirror_diff_eachcell_max_;
    n_uniform_mirror_diff_b_max = p_non_uniformity[B]->n_uniform_mirror_diff_eachcell_max_;

    mirror_diff_r_max_pos = p_non_uniformity[R]->mirror_diff_max_cell_pos_;
    mirror_diff_gr_max_pos = p_non_uniformity[Gr]->mirror_diff_max_cell_pos_;
    mirror_diff_gb_max_pos = p_non_uniformity[Gb]->mirror_diff_max_cell_pos_;
    mirror_diff_b_max_pos = p_non_uniformity[B]->mirror_diff_max_cell_pos_;


    n_uniform_updown_diff_r_max = p_non_uniformity[R]->n_uniform_updown_diff_eachcell_max_;
    n_uniform_updown_diff_gr_max = p_non_uniformity[Gr]->n_uniform_updown_diff_eachcell_max_;
    n_uniform_updown_diff_gb_max = p_non_uniformity[Gb]->n_uniform_updown_diff_eachcell_max_;
    n_uniform_updown_diff_b_max = p_non_uniformity[B]->n_uniform_updown_diff_eachcell_max_;

    updown_diff_r_max_pos = p_non_uniformity[R]->updown_diff_max_cell_pos_;
    updown_diff_gr_max_pos = p_non_uniformity[Gr]->updown_diff_max_cell_pos_;
    updown_diff_gb_max_pos = p_non_uniformity[Gb]->updown_diff_max_cell_pos_;
    updown_diff_b_max_pos = p_non_uniformity[B]->updown_diff_max_cell_pos_;



    int m = w/w_cell;
    int n = h/h_cell;

    if(flag_log_print)
    {
        QFile f1("./Non_Uniformity_R_log.txt");
        if(f1.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f1);
            std::string msg;

            msg += boost::str(boost::format("--------------Non_Uniformity_R------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[R]->n_uniform_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f1.close();
        }

        QFile f2("./Non_Uniformity_Gr_log.txt");
        if(f2.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f2);
            std::string msg;

            msg += boost::str(boost::format("--------------Non_Uniformity_Gr------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[Gr]->n_uniform_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f2.close();
        }

        QFile f3("./Non_Uniformity_Gb_log.txt");
        if(f3.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f3);
            std::string msg;

            msg += boost::str(boost::format("--------------Non_Uniformity_Gb------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[Gb]->n_uniform_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f3.close();
        }

        QFile f4("./Non_Uniformity_B_log.txt");
        if(f4.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f4);
            std::string msg;

            msg += boost::str(boost::format("--------------Non_Uniformity_B------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[B]->n_uniform_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f4.close();
        }




        QFile f5("./Mirror_Diff_Non_Uniformity_R_log.txt");
        if(f5.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f5);
            std::string msg;

            msg += boost::str(boost::format("--------------Mirror_Diff_Non_Uniformity_R------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[R]->n_uniform_mirror_diff_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f5.close();
        }

        QFile f6("./Mirror_Diff_Non_Uniformity_Gr_log.txt");
        if(f6.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f6);
            std::string msg;

            msg += boost::str(boost::format("--------------Mirror_Diff_Non_Uniformity_Gr------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[Gr]->n_uniform_mirror_diff_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f6.close();
        }

        QFile f7("./Mirror_Diff_Non_Uniformity_Gb_log.txt");
        if(f7.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f7);
            std::string msg;

            msg += boost::str(boost::format("--------------Mirror_Diff_Non_Uniformity_Gb------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[Gb]->n_uniform_mirror_diff_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f7.close();
        }

        QFile f8("./Mirror_Diff_Non_Uniformity_B_log.txt");
        if(f8.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f8);
            std::string msg;

            msg += boost::str(boost::format("--------------Mirror_Diff_Non_Uniformity_B------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[B]->n_uniform_mirror_diff_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f8.close();
        }




        QFile f9("./Updown_Diff_Non_Uniformity_R_log.txt");
        if(f9.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f9);
            std::string msg;

            msg += boost::str(boost::format("--------------Updown_Diff_Non_Uniformity_R------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[R]->n_uniform_updown_diff_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f9.close();
        }

        QFile f10("./Updown_Diff_Non_Uniformity_Gr_log.txt");
        if(f10.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f10);
            std::string msg;

            msg += boost::str(boost::format("--------------Updown_Diff_Non_Uniformity_Gr------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[Gr]->n_uniform_updown_diff_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f10.close();
        }

        QFile f11("./Updown_Diff_Non_Uniformity_Gb_log.txt");
        if(f11.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f11);
            std::string msg;

            msg += boost::str(boost::format("--------------Updown_Diff_Non_Uniformity_Gb------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[Gb]->n_uniform_updown_diff_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f11.close();
        }

        QFile f12("./Updown_Diff_Non_Uniformity_B_log.txt");
        if(f12.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f12);
            std::string msg;

            msg += boost::str(boost::format("--------------Updown_Diff_Non_Uniformity_B------------------\n"));

            for (int i=0; i<n; i++)
            {
                for(int j=0; j<m; j++)
                {
                    msg += boost::str(boost::format("%6.3f   ")
                                      % (p_non_uniformity[B]->n_uniform_updown_diff_eachcell_.at(i*m+j)));
                }
                msg += boost::str(boost::format("\n"));
            }
            str<<QString::fromStdString(msg);

            f12.close();
        }
    }


    for(int i=0; i< Channel_Num; i++)
    {
        delete p_non_uniformity[i];
    }
}







void TestItem::test_roi_colorratio(double mean_gr, double mean_gb, int w, int h,
                                   int skip, int roi_x_cnt, int roi_y_cnt,
                                   uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                   QVector<double> &roi_rg_ratio,
                                   QVector<double> &roi_bg_ratio,
                                   QVector<double> &roi_gg_ratio,
                                   QMap <QPair<QString,double>,double> &GG_ratio,
                                   QMap <QPair<QString,double>,double> &RG_ratio,
                                   QMap <QPair<QString,double>,double> &BG_ratio,
                                   QMap <QString,double> &G_G_diff,
                                   QMap <QString,double> &GG_Ratio_target,
                                   QMap <QString,double> &RG_Ratio_target,
                                   QMap <QString,double> &BG_Ratio_target)
{

    int total_roi_cnt = roi_x_cnt*roi_y_cnt;
    int roi_x_pitch = (w/2 - 2*skip/2)/roi_x_cnt;
    int roi_y_pitch = (h/2 - 2*skip/2)/roi_y_cnt;

    if (mean_gr>0 && mean_gb>0)
    {
        QVector<cv::Rect> ROI;
        for(int i=0; i<roi_y_cnt; i++)
        {
            int roi_y = skip/2 + i*roi_y_pitch;
            for(int j=0; j<roi_x_cnt; j++)
            {
                int roi_x = skip/2 + j*roi_x_pitch;
                ROI.append(cv::Rect(roi_x,roi_y,roi_x_pitch,roi_y_pitch));
            }
        }

        QVector<double> roi_g_g_diff_ratio;
        for(int i=0;i!=ROI.size();++i)
        {
            double r_g_ratio;
            double b_g_ratio;
            double g_g_ratio;
            double g_g_diff;
            AlgorithmFuncs::get_roi_colorratio(g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,
                                               w/2,h/2,ROI.at(i).x,ROI.at(i).y,ROI.at(i).width,ROI.at(i).height,
                                               g_g_ratio,r_g_ratio,b_g_ratio,g_g_diff);
            roi_rg_ratio.push_back(r_g_ratio);
            roi_bg_ratio.push_back(b_g_ratio);
            roi_gg_ratio.push_back(g_g_ratio);
            roi_g_g_diff_ratio.push_back(g_g_diff);
        }

        double ratio_g_g_center = roi_gg_ratio[total_roi_cnt/2];
        double ratio_r_g_center = roi_rg_ratio[total_roi_cnt/2];
        double ratio_b_g_center = roi_bg_ratio[total_roi_cnt/2];
        double gr_gb_diff_center = roi_g_g_diff_ratio[total_roi_cnt/2];

        double ratio_g_g_left_up = roi_gg_ratio[0];
        double ratio_r_g_left_up = roi_rg_ratio[0];
        double ratio_b_g_left_up = roi_bg_ratio[0];
        double gr_gb_diff_left_up = roi_g_g_diff_ratio[0];

        double ratio_g_g_right_up = roi_gg_ratio[roi_x_cnt-1];
        double ratio_r_g_right_up = roi_rg_ratio[roi_x_cnt-1];
        double ratio_b_g_right_up = roi_bg_ratio[roi_x_cnt-1];
        double gr_gb_diff_right_up = roi_g_g_diff_ratio[roi_x_cnt-1];

        double ratio_g_g_left_down = roi_gg_ratio[total_roi_cnt-roi_x_cnt];
        double ratio_r_g_left_down = roi_rg_ratio[total_roi_cnt-roi_x_cnt];
        double ratio_b_g_left_down = roi_bg_ratio[total_roi_cnt-roi_x_cnt];
        double gr_gb_diff_left_down = roi_g_g_diff_ratio[total_roi_cnt-roi_x_cnt];

        double ratio_g_g_right_down = roi_gg_ratio[total_roi_cnt-1];
        double ratio_r_g_right_down = roi_rg_ratio[total_roi_cnt-1];
        double ratio_b_g_right_down = roi_bg_ratio[total_roi_cnt-1];
        double gr_gb_diff_right_down = roi_g_g_diff_ratio[total_roi_cnt-1];


        GG_ratio[QPair<QString,double>("leftup",GG_Ratio_target.value("leftup"))]=(ratio_g_g_left_up>10)? -1 : ratio_g_g_left_up;
        GG_ratio[QPair<QString,double>("rightup",GG_Ratio_target.value("rightup"))]=(ratio_g_g_right_up>10)? -1 : ratio_g_g_right_up;
        GG_ratio[QPair<QString,double>("center",GG_Ratio_target.value("center"))]=(ratio_g_g_center>10)? -1 : ratio_g_g_center;
        GG_ratio[QPair<QString,double>("leftdown",GG_Ratio_target.value("leftdown"))]=(ratio_g_g_left_down>10)? -1 : ratio_g_g_left_down;
        GG_ratio[QPair<QString,double>("rightdown",GG_Ratio_target.value("rightdown"))]=(ratio_g_g_right_down>10)? -1 : ratio_g_g_right_down;

        RG_ratio[QPair<QString,double>("leftup",RG_Ratio_target.value("leftup"))]=(ratio_r_g_left_up>10)? -1 : ratio_r_g_left_up;
        RG_ratio[QPair<QString,double>("rightup",RG_Ratio_target.value("rightup"))]=(ratio_r_g_right_up>10)? -1 : ratio_r_g_right_up;
        RG_ratio[QPair<QString,double>("center",RG_Ratio_target.value("center"))]=(ratio_r_g_center>10)? -1 : ratio_r_g_center;
        RG_ratio[QPair<QString,double>("leftdown",RG_Ratio_target.value("leftdown"))]=(ratio_r_g_left_down>10)? -1 : ratio_r_g_left_down;
        RG_ratio[QPair<QString,double>("rightdown",RG_Ratio_target.value("rightdown"))]=(ratio_r_g_right_down>10)? -1 : ratio_r_g_right_down;

        BG_ratio[QPair<QString,double>("leftup",BG_Ratio_target.value("leftup"))]=(ratio_b_g_left_up>10)? -1 : ratio_b_g_left_up;
        BG_ratio[QPair<QString,double>("rightup",BG_Ratio_target.value("rightup"))]=(ratio_b_g_right_up>10)? -1 : ratio_b_g_right_up;
        BG_ratio[QPair<QString,double>("center",BG_Ratio_target.value("center"))]=(ratio_b_g_center>10)? -1 : ratio_b_g_center;
        BG_ratio[QPair<QString,double>("leftdown",BG_Ratio_target.value("leftdown"))]=(ratio_b_g_left_down>10)? -1 : ratio_b_g_left_down;
        BG_ratio[QPair<QString,double>("rightdown",BG_Ratio_target.value("rightdown"))]=(ratio_b_g_right_down>10)? -1 : ratio_b_g_right_down;

        G_G_diff["leftup"]=(gr_gb_diff_left_up>10)? -1 : gr_gb_diff_left_up;
        G_G_diff["rightup"]=(gr_gb_diff_right_up>10)? -1 : gr_gb_diff_right_up;
        G_G_diff["center"]=(gr_gb_diff_center>10)? -1 : gr_gb_diff_center;
        G_G_diff["leftdown"]=(gr_gb_diff_left_down>10)? -1 : gr_gb_diff_left_down;
        G_G_diff["rightdown"]=(gr_gb_diff_right_down>10)? -1 : gr_gb_diff_right_down;

    }
    else
    {
        for (int i=0; i<total_roi_cnt; i++)
        {
            roi_rg_ratio.append(-1);
            roi_bg_ratio.append(-1);
            roi_gg_ratio.append(-1);
        }

        GG_ratio[QPair<QString,double>("leftup",GG_Ratio_target.value("leftup"))]=-1;
        GG_ratio[QPair<QString,double>("rightup",GG_Ratio_target.value("rightup"))]=-1;
        GG_ratio[QPair<QString,double>("center",GG_Ratio_target.value("center"))]=-1;
        GG_ratio[QPair<QString,double>("leftdown",GG_Ratio_target.value("leftdown"))]=-1;
        GG_ratio[QPair<QString,double>("rightdown",GG_Ratio_target.value("rightdown"))]=-1;

        RG_ratio[QPair<QString,double>("leftup",RG_Ratio_target.value("leftup"))]=-1;
        RG_ratio[QPair<QString,double>("rightup",RG_Ratio_target.value("rightup"))]=-1;
        RG_ratio[QPair<QString,double>("center",RG_Ratio_target.value("center"))]=-1;
        RG_ratio[QPair<QString,double>("leftdown",RG_Ratio_target.value("leftdown"))]=-1;
        RG_ratio[QPair<QString,double>("rightdown",RG_Ratio_target.value("rightdown"))]=-1;

        BG_ratio[QPair<QString,double>("leftup",BG_Ratio_target.value("leftup"))]=-1;
        BG_ratio[QPair<QString,double>("rightup",BG_Ratio_target.value("rightup"))]=-1;
        BG_ratio[QPair<QString,double>("center",BG_Ratio_target.value("center"))]=-1;
        BG_ratio[QPair<QString,double>("leftdown",BG_Ratio_target.value("leftdown"))]=-1;
        BG_ratio[QPair<QString,double>("rightdown",BG_Ratio_target.value("rightdown"))]=-1;

        G_G_diff["leftup"]=-1;
        G_G_diff["rightup"]=-1;
        G_G_diff["center"]=-1;
        G_G_diff["leftdown"]=-1;
        G_G_diff["rightdown"]=-1;
    }
}


void TestItem::test_roi_colorratio_5X5(double mean_gr, double mean_gb, int w, int h,
                                       int skip, int roi_x_cnt, int roi_y_cnt,
                                       uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                       QVector<double> &roi_rg_ratio,
                                       QVector<double> &roi_bg_ratio,
                                       QVector<double> &roi_gg_ratio,
                                       QMap <QPair<QString,double>,double> &GG_ratio,
                                       QMap <QPair<QString,double>,double> &RG_ratio,
                                       QMap <QPair<QString,double>,double> &BG_ratio,
                                       QMap <QString,double> &G_G_diff,
                                       QMap <QString,double> &GG_Ratio_target,
                                       QMap <QString,double> &RG_Ratio_target,
                                       QMap <QString,double> &BG_Ratio_target)
{

    int total_roi_cnt = roi_x_cnt*roi_y_cnt;
    int roi_x_pitch = (w/2 - 2*skip/2)/roi_x_cnt;
    int roi_y_pitch = (h/2 - 2*skip/2)/roi_y_cnt;

    if (mean_gr>0 && mean_gb>0)
    {
        QVector<cv::Rect> ROI;
        for(int i=0; i<roi_y_cnt; i++)
        {
            int roi_y = skip/2 + i*roi_y_pitch;
            for(int j=0; j<roi_x_cnt; j++)
            {
                int roi_x = skip/2 + j*roi_x_pitch;
                ROI.append(cv::Rect(roi_x,roi_y,roi_x_pitch,roi_y_pitch));
            }
        }

        QVector<double> roi_g_g_diff_ratio;
        for(int i=0;i!=ROI.size();++i)
        {
            double r_g_ratio;
            double b_g_ratio;
            double g_g_ratio;
            double g_g_diff;
            AlgorithmFuncs::get_roi_colorratio_5X5(g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,
                                                   w/2,h/2,ROI.at(i).x,ROI.at(i).y,ROI.at(i).width,ROI.at(i).height,
                                                   g_g_ratio,r_g_ratio,b_g_ratio,g_g_diff);
            roi_rg_ratio.push_back(r_g_ratio);
            roi_bg_ratio.push_back(b_g_ratio);
            roi_gg_ratio.push_back(g_g_ratio);
            roi_g_g_diff_ratio.push_back(g_g_diff);
        }

        double ratio_g_g_center = roi_gg_ratio[total_roi_cnt/2];
        double ratio_r_g_center = roi_rg_ratio[total_roi_cnt/2];
        double ratio_b_g_center = roi_bg_ratio[total_roi_cnt/2];
        double gr_gb_diff_center = roi_g_g_diff_ratio[total_roi_cnt/2];

        double ratio_g_g_left_up = roi_gg_ratio[0];
        double ratio_r_g_left_up = roi_rg_ratio[0];
        double ratio_b_g_left_up = roi_bg_ratio[0];
        double gr_gb_diff_left_up = roi_g_g_diff_ratio[0];

        double ratio_g_g_right_up = roi_gg_ratio[roi_x_cnt-1];
        double ratio_r_g_right_up = roi_rg_ratio[roi_x_cnt-1];
        double ratio_b_g_right_up = roi_bg_ratio[roi_x_cnt-1];
        double gr_gb_diff_right_up = roi_g_g_diff_ratio[roi_x_cnt-1];

        double ratio_g_g_left_down = roi_gg_ratio[total_roi_cnt-roi_x_cnt];
        double ratio_r_g_left_down = roi_rg_ratio[total_roi_cnt-roi_x_cnt];
        double ratio_b_g_left_down = roi_bg_ratio[total_roi_cnt-roi_x_cnt];
        double gr_gb_diff_left_down = roi_g_g_diff_ratio[total_roi_cnt-roi_x_cnt];

        double ratio_g_g_right_down = roi_gg_ratio[total_roi_cnt-1];
        double ratio_r_g_right_down = roi_rg_ratio[total_roi_cnt-1];
        double ratio_b_g_right_down = roi_bg_ratio[total_roi_cnt-1];
        double gr_gb_diff_right_down = roi_g_g_diff_ratio[total_roi_cnt-1];


        GG_ratio[QPair<QString,double>("leftup",GG_Ratio_target.value("leftup"))]=(ratio_g_g_left_up>10)? -1 : ratio_g_g_left_up;
        GG_ratio[QPair<QString,double>("rightup",GG_Ratio_target.value("rightup"))]=(ratio_g_g_right_up>10)? -1 : ratio_g_g_right_up;
        GG_ratio[QPair<QString,double>("center",GG_Ratio_target.value("center"))]=(ratio_g_g_center>10)? -1 : ratio_g_g_center;
        GG_ratio[QPair<QString,double>("leftdown",GG_Ratio_target.value("leftdown"))]=(ratio_g_g_left_down>10)? -1 : ratio_g_g_left_down;
        GG_ratio[QPair<QString,double>("rightdown",GG_Ratio_target.value("rightdown"))]=(ratio_g_g_right_down>10)? -1 : ratio_g_g_right_down;

        RG_ratio[QPair<QString,double>("leftup",RG_Ratio_target.value("leftup"))]=(ratio_r_g_left_up>10)? -1 : ratio_r_g_left_up;
        RG_ratio[QPair<QString,double>("rightup",RG_Ratio_target.value("rightup"))]=(ratio_r_g_right_up>10)? -1 : ratio_r_g_right_up;
        RG_ratio[QPair<QString,double>("center",RG_Ratio_target.value("center"))]=(ratio_r_g_center>10)? -1 : ratio_r_g_center;
        RG_ratio[QPair<QString,double>("leftdown",RG_Ratio_target.value("leftdown"))]=(ratio_r_g_left_down>10)? -1 : ratio_r_g_left_down;
        RG_ratio[QPair<QString,double>("rightdown",RG_Ratio_target.value("rightdown"))]=(ratio_r_g_right_down>10)? -1 : ratio_r_g_right_down;

        BG_ratio[QPair<QString,double>("leftup",BG_Ratio_target.value("leftup"))]=(ratio_b_g_left_up>10)? -1 : ratio_b_g_left_up;
        BG_ratio[QPair<QString,double>("rightup",BG_Ratio_target.value("rightup"))]=(ratio_b_g_right_up>10)? -1 : ratio_b_g_right_up;
        BG_ratio[QPair<QString,double>("center",BG_Ratio_target.value("center"))]=(ratio_b_g_center>10)? -1 : ratio_b_g_center;
        BG_ratio[QPair<QString,double>("leftdown",BG_Ratio_target.value("leftdown"))]=(ratio_b_g_left_down>10)? -1 : ratio_b_g_left_down;
        BG_ratio[QPair<QString,double>("rightdown",BG_Ratio_target.value("rightdown"))]=(ratio_b_g_right_down>10)? -1 : ratio_b_g_right_down;

        G_G_diff["leftup"]=(gr_gb_diff_left_up>10)? -1 : gr_gb_diff_left_up;
        G_G_diff["rightup"]=(gr_gb_diff_right_up>10)? -1 : gr_gb_diff_right_up;
        G_G_diff["center"]=(gr_gb_diff_center>10)? -1 : gr_gb_diff_center;
        G_G_diff["leftdown"]=(gr_gb_diff_left_down>10)? -1 : gr_gb_diff_left_down;
        G_G_diff["rightdown"]=(gr_gb_diff_right_down>10)? -1 : gr_gb_diff_right_down;

    }
    else
    {
        for (int i=0; i<total_roi_cnt; i++)
        {
            roi_rg_ratio.append(-1);
            roi_bg_ratio.append(-1);
            roi_gg_ratio.append(-1);
        }

        GG_ratio[QPair<QString,double>("leftup",GG_Ratio_target.value("leftup"))]=-1;
        GG_ratio[QPair<QString,double>("rightup",GG_Ratio_target.value("rightup"))]=-1;
        GG_ratio[QPair<QString,double>("center",GG_Ratio_target.value("center"))]=-1;
        GG_ratio[QPair<QString,double>("leftdown",GG_Ratio_target.value("leftdown"))]=-1;
        GG_ratio[QPair<QString,double>("rightdown",GG_Ratio_target.value("rightdown"))]=-1;

        RG_ratio[QPair<QString,double>("leftup",RG_Ratio_target.value("leftup"))]=-1;
        RG_ratio[QPair<QString,double>("rightup",RG_Ratio_target.value("rightup"))]=-1;
        RG_ratio[QPair<QString,double>("center",RG_Ratio_target.value("center"))]=-1;
        RG_ratio[QPair<QString,double>("leftdown",RG_Ratio_target.value("leftdown"))]=-1;
        RG_ratio[QPair<QString,double>("rightdown",RG_Ratio_target.value("rightdown"))]=-1;

        BG_ratio[QPair<QString,double>("leftup",BG_Ratio_target.value("leftup"))]=-1;
        BG_ratio[QPair<QString,double>("rightup",BG_Ratio_target.value("rightup"))]=-1;
        BG_ratio[QPair<QString,double>("center",BG_Ratio_target.value("center"))]=-1;
        BG_ratio[QPair<QString,double>("leftdown",BG_Ratio_target.value("leftdown"))]=-1;
        BG_ratio[QPair<QString,double>("rightdown",BG_Ratio_target.value("rightdown"))]=-1;

        G_G_diff["leftup"]=-1;
        G_G_diff["rightup"]=-1;
        G_G_diff["center"]=-1;
        G_G_diff["leftdown"]=-1;
        G_G_diff["rightdown"]=-1;
    }
}



void TestItem::test_colorratio(double mean_gr, double mean_gb, int w, int h,
                               int pos_x, int pos_y, int w_rect, int h_rect,
                               uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                               QMap <QPair<QString,double>,double> &GG_ratio,
                               QMap <QPair<QString,double>,double> &RG_ratio,
                               QMap <QPair<QString,double>,double> &BG_ratio,
                               QMap <QString,double> &G_G_diff,
                               QMap <QString,double> &GG_Ratio_target,
                               QMap <QString,double> &RG_Ratio_target,
                               QMap <QString,double> &BG_Ratio_target)
{

    if (mean_gr>0 && mean_gb>0)
    {
        MultiThread::get_image_colorratio *p_colorratio[5];
        for(int i=0; i<5; ++i)
        {
            p_colorratio[i]= new MultiThread::get_image_colorratio();
        }

        p_colorratio[0]->setparam(g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,pos_x,pos_y,w/2,h/2,w_rect,h_rect); //center
        p_colorratio[1]->setparam(g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,0,0,w/2,h/2,w_rect,h_rect); //leftup
        p_colorratio[2]->setparam(g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,w/2-w_rect,0,w/2,h/2,w_rect,h_rect); //rightup
        p_colorratio[3]->setparam(g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,0,h/2-h_rect,w/2,h/2,w_rect,h_rect); //leftdown
        p_colorratio[4]->setparam(g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,w/2-w_rect,h/2-h_rect,w/2,h/2,w_rect,h_rect); //rightdown

        for(int i=0; i<5; ++i)
        {
            p_colorratio[i]->start();
        }

        for(int i=0; i<5; ++i)
        {
            p_colorratio[i]->wait();
        }

        double ratio_g_g_center = p_colorratio[0]->g_g_ratio;
        double ratio_r_g_center = p_colorratio[0]->r_g_ratio;
        double ratio_b_g_center = p_colorratio[0]->b_g_ratio;
        double gr_gb_diff_center = p_colorratio[0]->gr_gb_diff;

        double ratio_g_g_left_up = p_colorratio[1]->g_g_ratio;
        double ratio_r_g_left_up = p_colorratio[1]->r_g_ratio;
        double ratio_b_g_left_up = p_colorratio[1]->b_g_ratio;
        double gr_gb_diff_left_up = p_colorratio[1]->gr_gb_diff;

        double ratio_g_g_right_up = p_colorratio[2]->g_g_ratio;
        double ratio_r_g_right_up = p_colorratio[2]->r_g_ratio;
        double ratio_b_g_right_up = p_colorratio[2]->b_g_ratio;
        double gr_gb_diff_right_up = p_colorratio[2]->gr_gb_diff;

        double ratio_g_g_left_down = p_colorratio[3]->g_g_ratio;
        double ratio_r_g_left_down = p_colorratio[3]->r_g_ratio;
        double ratio_b_g_left_down = p_colorratio[3]->b_g_ratio;
        double gr_gb_diff_left_down = p_colorratio[3]->gr_gb_diff;

        double ratio_g_g_right_down = p_colorratio[4]->g_g_ratio;
        double ratio_r_g_right_down = p_colorratio[4]->r_g_ratio;
        double ratio_b_g_right_down = p_colorratio[4]->b_g_ratio;
        double gr_gb_diff_right_down = p_colorratio[4]->gr_gb_diff;


        GG_ratio[QPair<QString,double>("leftup",GG_Ratio_target.value("leftup"))]=(ratio_g_g_left_up>10)? -1 : ratio_g_g_left_up;
        GG_ratio[QPair<QString,double>("rightup",GG_Ratio_target.value("rightup"))]=(ratio_g_g_right_up>10)? -1 : ratio_g_g_right_up;
        GG_ratio[QPair<QString,double>("center",GG_Ratio_target.value("center"))]=(ratio_g_g_center>10)? -1 : ratio_g_g_center;
        GG_ratio[QPair<QString,double>("leftdown",GG_Ratio_target.value("leftdown"))]=(ratio_g_g_left_down>10)? -1 : ratio_g_g_left_down;
        GG_ratio[QPair<QString,double>("rightdown",GG_Ratio_target.value("rightdown"))]=(ratio_g_g_right_down>10)? -1 : ratio_g_g_right_down;

        RG_ratio[QPair<QString,double>("leftup",RG_Ratio_target.value("leftup"))]=(ratio_r_g_left_up>10)? -1 : ratio_r_g_left_up;
        RG_ratio[QPair<QString,double>("rightup",RG_Ratio_target.value("rightup"))]=(ratio_r_g_right_up>10)? -1 : ratio_r_g_right_up;
        RG_ratio[QPair<QString,double>("center",RG_Ratio_target.value("center"))]=(ratio_r_g_center>10)? -1 : ratio_r_g_center;
        RG_ratio[QPair<QString,double>("leftdown",RG_Ratio_target.value("leftdown"))]=(ratio_r_g_left_down>10)? -1 : ratio_r_g_left_down;
        RG_ratio[QPair<QString,double>("rightdown",RG_Ratio_target.value("rightdown"))]=(ratio_r_g_right_down>10)? -1 : ratio_r_g_right_down;

        BG_ratio[QPair<QString,double>("leftup",BG_Ratio_target.value("leftup"))]=(ratio_b_g_left_up>10)? -1 : ratio_b_g_left_up;
        BG_ratio[QPair<QString,double>("rightup",BG_Ratio_target.value("rightup"))]=(ratio_b_g_right_up>10)? -1 : ratio_b_g_right_up;
        BG_ratio[QPair<QString,double>("center",BG_Ratio_target.value("center"))]=(ratio_b_g_center>10)? -1 : ratio_b_g_center;
        BG_ratio[QPair<QString,double>("leftdown",BG_Ratio_target.value("leftdown"))]=(ratio_b_g_left_down>10)? -1 : ratio_b_g_left_down;
        BG_ratio[QPair<QString,double>("rightdown",BG_Ratio_target.value("rightdown"))]=(ratio_b_g_right_down>10)? -1 : ratio_b_g_right_down;

        G_G_diff["leftup"]=(gr_gb_diff_left_up>10)? -1 : gr_gb_diff_left_up;
        G_G_diff["rightup"]=(gr_gb_diff_right_up>10)? -1 : gr_gb_diff_right_up;
        G_G_diff["center"]=(gr_gb_diff_center>10)? -1 : gr_gb_diff_center;
        G_G_diff["leftdown"]=(gr_gb_diff_left_down>10)? -1 : gr_gb_diff_left_down;
        G_G_diff["rightdown"]=(gr_gb_diff_right_down>10)? -1 : gr_gb_diff_right_down;

        for(int i=0; i< 5; i++)
        {
            delete p_colorratio[i];
        }

    }
    else
    {
        GG_ratio[QPair<QString,double>("leftup",GG_Ratio_target.value("leftup"))]=-1;
        GG_ratio[QPair<QString,double>("rightup",GG_Ratio_target.value("rightup"))]=-1;
        GG_ratio[QPair<QString,double>("center",GG_Ratio_target.value("center"))]=-1;
        GG_ratio[QPair<QString,double>("leftdown",GG_Ratio_target.value("leftdown"))]=-1;
        GG_ratio[QPair<QString,double>("rightdown",GG_Ratio_target.value("rightdown"))]=-1;

        RG_ratio[QPair<QString,double>("leftup",RG_Ratio_target.value("leftup"))]=-1;
        RG_ratio[QPair<QString,double>("rightup",RG_Ratio_target.value("rightup"))]=-1;
        RG_ratio[QPair<QString,double>("center",RG_Ratio_target.value("center"))]=-1;
        RG_ratio[QPair<QString,double>("leftdown",RG_Ratio_target.value("leftdown"))]=-1;
        RG_ratio[QPair<QString,double>("rightdown",RG_Ratio_target.value("rightdown"))]=-1;

        BG_ratio[QPair<QString,double>("leftup",BG_Ratio_target.value("leftup"))]=-1;
        BG_ratio[QPair<QString,double>("rightup",BG_Ratio_target.value("rightup"))]=-1;
        BG_ratio[QPair<QString,double>("center",BG_Ratio_target.value("center"))]=-1;
        BG_ratio[QPair<QString,double>("leftdown",BG_Ratio_target.value("leftdown"))]=-1;
        BG_ratio[QPair<QString,double>("rightdown",BG_Ratio_target.value("rightdown"))]=-1;

        G_G_diff["leftup"]=-1;
        G_G_diff["rightup"]=-1;
        G_G_diff["center"]=-1;
        G_G_diff["leftdown"]=-1;
        G_G_diff["rightdown"]=-1;
    }
}


void TestItem::test_deadrow_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                      int w, int h, int skip, int radius, double spec_dol,
                                      double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                      double &max_row_diff_R, double &max_row_diff_Gr, double &max_row_diff_Gb, double &max_row_diff_B,
                                      int &max_row_pos, double &row_max_min_diff, int &cnt_dead_row, int &cnt_dol, Spec_Mode mode, std::string &msg)
{
    MultiThread::get_channel_deadrow *p_deadrow[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i] = new MultiThread::get_channel_deadrow();
    }

    p_deadrow[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol,0,0,0,0,0,mode,"");
    p_deadrow[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol,0,0,0,0,0,mode,"");
    p_deadrow[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol,0,0,0,0,0,mode,"");
    p_deadrow[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol,0,0,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Max Row Diff value:%.3f; row position:%d\n")
                           % p_deadrow[R]->max_diff_ % p_deadrow[R]->max_pos_);

    msg+=boost::str(boost::format("Gr channel Max Row Diff value:%.3f; row position:%d\n")
                           % p_deadrow[Gr]->max_diff_ % p_deadrow[Gr]->max_pos_);

    msg+=boost::str(boost::format("Gb channel Max Row Diff value:%.3f; row position:%d\n")
                           % p_deadrow[Gb]->max_diff_ % p_deadrow[Gb]->max_pos_);

    msg+=boost::str(boost::format("B channel Max Row Diff value:%.3f; row position:%d\n")
                           % p_deadrow[B]->max_diff_ % p_deadrow[B]->max_pos_);

    double max_row_diff=0;
    for (int i=0; i < Channel_Num; i++)
    {
        if(max_row_diff < p_deadrow[i]->max_diff_)
        {
            max_row_diff = p_deadrow[i]->max_diff_;
            max_row_pos = p_deadrow[i]->max_pos_;
        }
        if(row_max_min_diff < p_deadrow[i]->row_max_min_diff_)
        {
            row_max_min_diff = p_deadrow[i]->row_max_min_diff_;
        }

        cnt_dead_row += p_deadrow[i]->cnt_deadrow_;
        cnt_dol += p_deadrow[i]->cnt_dol_;
        msg += p_deadrow[i]->msg_;
    }

    max_row_diff_R =  p_deadrow[R]->max_diff_;
    max_row_diff_Gr =  p_deadrow[Gr]->max_diff_;
    max_row_diff_Gb =  p_deadrow[Gb]->max_diff_;
    max_row_diff_B =  p_deadrow[B]->max_diff_;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_deadrow[i];
    }
}



void TestItem::test_deadcol_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                      int w, int h, int skip, int radius, double spec_dol,
                                      double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                      double &max_col_diff_R, double &max_col_diff_Gr, double &max_col_diff_Gb, double &max_col_diff_B,
                                       int &max_col_pos, double &col_max_min_diff,int &cnt_dead_col, int &cnt_dol, Spec_Mode mode, std::string &msg)
{
    MultiThread::get_channel_deadcol *p_deadcol[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i] = new MultiThread::get_channel_deadcol();
    }

    p_deadcol[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol,0,0,0,0,0,mode,"");
    p_deadcol[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol,0,0,0,0,0,mode,"");
    p_deadcol[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol,0,0,0,0,0,mode,"");
    p_deadcol[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol,0,0,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Max col Diff value:%.3f; col position:%d\n")
                           % p_deadcol[R]->max_diff_ % p_deadcol[R]->max_pos_);

    msg+=boost::str(boost::format("Gr channel Max col Diff value:%.3f; col position:%d\n")
                           % p_deadcol[Gr]->max_diff_ % p_deadcol[Gr]->max_pos_);

    msg+=boost::str(boost::format("Gb channel Max col Diff value:%.3f; col position:%d\n")
                           % p_deadcol[Gb]->max_diff_ % p_deadcol[Gb]->max_pos_);

    msg+=boost::str(boost::format("B channel Max col Diff value:%.3f; col position:%d\n")
                           % p_deadcol[B]->max_diff_ % p_deadcol[B]->max_pos_);

    double max_col_diff=0;
    for (int i=0; i < Channel_Num; i++)
    {
        if(max_col_diff < p_deadcol[i]->max_diff_)
        {
            max_col_diff = p_deadcol[i]->max_diff_;
            max_col_pos = p_deadcol[i]->max_pos_;
        }
        if(col_max_min_diff < p_deadcol[i]->col_max_min_diff_)
        {
            col_max_min_diff = p_deadcol[i]->col_max_min_diff_;
        }

        cnt_dead_col +=  p_deadcol[i]->cnt_deadcol_;
        cnt_dol += p_deadcol[i]->cnt_dol_;
        msg += p_deadcol[i]->msg_;
    }

    max_col_diff_R =  p_deadcol[R]->max_diff_;
    max_col_diff_Gr =  p_deadcol[Gr]->max_diff_;
    max_col_diff_Gb =  p_deadcol[Gb]->max_diff_;
    max_col_diff_B =  p_deadcol[B]->max_diff_;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_deadcol[i];
    }
}



void TestItem::test_edge_deadrow_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                           int w, int h, int skip, int radius, double spec_dol,
                                           double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                           double &max_row_diff_R, double &max_row_diff_Gr, double &max_row_diff_Gb, double &max_row_diff_B,
                                           int &max_row_pos, double &row_max_min_diff, int &cnt_dead_row, int &cnt_dol, Spec_Mode mode, std::string &msg)
{
    MultiThread::get_edge_channel_deadrow *p_deadrow[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i] = new MultiThread::get_edge_channel_deadrow();
    }

    p_deadrow[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol,0,0,0,0,0,mode,"");
    p_deadrow[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol,0,0,0,0,0,mode,"");
    p_deadrow[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol,0,0,0,0,0,mode,"");
    p_deadrow[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol,0,0,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Edge Max Row Diff value:%.3f; Edge row position:%d\n")
                           % p_deadrow[R]->max_diff_ % p_deadrow[R]->max_pos_);

    msg+=boost::str(boost::format("Gr channel Edge Max Row Diff value:%.3f; Edge row position:%d\n")
                           % p_deadrow[Gr]->max_diff_ % p_deadrow[Gr]->max_pos_);

    msg+=boost::str(boost::format("Gb channel Edge Max Row Diff value:%.3f; Edge row position:%d\n")
                           % p_deadrow[Gb]->max_diff_ % p_deadrow[Gb]->max_pos_);

    msg+=boost::str(boost::format("B channel Edge Max Row Diff value:%.3f; Edge row position:%d\n")
                           % p_deadrow[B]->max_diff_ % p_deadrow[B]->max_pos_);

    double max_row_diff=0;
    for (int i=0; i < Channel_Num; i++)
    {
        if(max_row_diff < p_deadrow[i]->max_diff_)
        {
            max_row_diff = p_deadrow[i]->max_diff_;
            max_row_pos = p_deadrow[i]->max_pos_;
        }
        if(row_max_min_diff < p_deadrow[i]->row_max_min_diff_)
        {
            row_max_min_diff = p_deadrow[i]->row_max_min_diff_;
        }

        cnt_dead_row += p_deadrow[i]->cnt_deadrow_;
        cnt_dol += p_deadrow[i]->cnt_dol_;
        msg += p_deadrow[i]->msg_;
    }

    max_row_diff_R =  p_deadrow[R]->max_diff_;
    max_row_diff_Gr =  p_deadrow[Gr]->max_diff_;
    max_row_diff_Gb =  p_deadrow[Gb]->max_diff_;
    max_row_diff_B =  p_deadrow[B]->max_diff_;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_deadrow[i];
    }
}





void TestItem::test_edge_deadcol_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                           int w, int h, int skip, int radius, double spec_dol,
                                           double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                           double &max_col_diff_R, double &max_col_diff_Gr, double &max_col_diff_Gb, double &max_col_diff_B,
                                           int &max_col_pos, double &col_max_min_diff,int &cnt_dead_col, int &cnt_dol, Spec_Mode mode, std::string &msg)
{
    MultiThread::get_edge_channel_deadcol *p_deadcol[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i] = new MultiThread::get_edge_channel_deadcol();
    }

    p_deadcol[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol,0,0,0,0,0,mode,"");
    p_deadcol[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol,0,0,0,0,0,mode,"");
    p_deadcol[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol,0,0,0,0,0,mode,"");
    p_deadcol[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol,0,0,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Edge Max col Diff value:%.3f; Edge col position:%d\n")
                           % p_deadcol[R]->max_diff_ % p_deadcol[R]->max_pos_);

    msg+=boost::str(boost::format("Gr channel Edge Max col Diff value:%.3f; Edge col position:%d\n")
                           % p_deadcol[Gr]->max_diff_ % p_deadcol[Gr]->max_pos_);

    msg+=boost::str(boost::format("Gb channel Edge Max col Diff value:%.3f; Edge col position:%d\n")
                           % p_deadcol[Gb]->max_diff_ % p_deadcol[Gb]->max_pos_);

    msg+=boost::str(boost::format("B channel Edge Max col Diff value:%.3f; Edge col position:%d\n")
                           % p_deadcol[B]->max_diff_ % p_deadcol[B]->max_pos_);

    double max_col_diff=0;
    for (int i=0; i < Channel_Num; i++)
    {
        if(max_col_diff < p_deadcol[i]->max_diff_)
        {
            max_col_diff = p_deadcol[i]->max_diff_;
            max_col_pos = p_deadcol[i]->max_pos_;
        }
        if(col_max_min_diff < p_deadcol[i]->col_max_min_diff_)
        {
            col_max_min_diff = p_deadcol[i]->col_max_min_diff_;
        }

        cnt_dead_col +=  p_deadcol[i]->cnt_deadcol_;
        cnt_dol += p_deadcol[i]->cnt_dol_;
        msg += p_deadcol[i]->msg_;
    }

    max_col_diff_R =  p_deadcol[R]->max_diff_;
    max_col_diff_Gr =  p_deadcol[Gr]->max_diff_;
    max_col_diff_Gb =  p_deadcol[Gb]->max_diff_;
    max_col_diff_B =  p_deadcol[B]->max_diff_;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_deadcol[i];
    }
}



void TestItem::find_cluster(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                            uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                            int w, int h, int skip, int radius,
                            double spec_r, double spec_gr, double spec_gb, double spec_b,
                            Spec_Mode mode, RGB_BAYER bayer,
                            QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                            int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                            int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                            int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                            int &other_cluster_ch, int &total_cluster_cnt, uchar *buf_map, std::string &g_str_msg_)
{


    logMsg(QString("cluster actual G channel spec: %1 mode: %2").arg(spec_gr,mode).toStdString().c_str());


    MultiThread::bychannel_binarization *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_binarization();
    }

    p_binary[R]->setparam(g_buf_r_,g_binary_r_,w/2,h/2,skip,radius,spec_r,mode);
    p_binary[Gr]->setparam(g_buf_gr_,g_binary_gr_,w/2,h/2,skip,radius,spec_gr,mode);
    p_binary[Gb]->setparam(g_buf_gb_,g_binary_gb_,w/2,h/2,skip,radius,spec_gb,mode);
    p_binary[B]->setparam(g_buf_b_,g_binary_b_,w/2,h/2,skip,radius,spec_b,mode);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }

    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_binary_r_,g_binary_gr_,g_binary_gb_,g_binary_b_,g_binary_,w,h,bayer);



//    cv::Mat img_tmp = cv::Mat(h/2,w/2,CV_8U);
//    for (int i=0;i<h/2;i++)
//    {
//        uchar *p=img_tmp.ptr<uchar>(i);
//        for(int j=0;j<w/2;j++)
//            p[j]=g_binary_[i*w/2+j];
//    }
//    cv::imwrite(QString("./cluster_binary_image.bmp").toStdString().c_str(),img_tmp);



    total_cluster_cnt = AlgorithmFuncs::FindALLClusterBayer_in_binary(g_binary_, buf_map,
                                                                      w, h,
                                                                      cnts_cluster,
                                                                      cnt_2x4_pix,
                                                                      cnt_2x2_pix,
                                                                      cnt_1x2_2x1_pix,
                                                                      cnt_2x2_diagonal_pix,
                                                                      cnt_1x2_ch,
                                                                      cnt_2x1_ch,
                                                                      cnt_2x2_leftdiagonal,
                                                                      cnt_2x2_rightdiagonal,
                                                                      cnt_white_pix,
                                                                      cnt_dark_pix,
                                                                      paticle,
                                                                      other_cluster,
                                                                      other_cluster_ch,
                                                                      g_str_msg_);

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_binary[i];
    }
}



void TestItem::find_cluster_delta(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                  double *g_buf_delta_r_,double *g_buf_delta_gr_,double *g_buf_delta_gb_,double *g_buf_delta_b_, double *g_buf_delta_raw_,
                                  uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                                  int w, int h, int skip, int radius,
                                  double spec_r, double spec_gr, double spec_gb, double spec_b,
                                  Spec_Mode mode, RGB_BAYER bayer,
                                  QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                                  int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                                  int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                                  int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                                  int &other_cluster_ch, int &total_cluster_cnt, uchar *buf_map, std::string &g_str_msg_)
{


    logMsg(QString("cluster actual G channel spec: %1 mode: %2").arg(spec_gr,mode).toStdString().c_str());

    MultiThread::bychannel_binarization_delta *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_binarization_delta();
    }

    p_binary[R]->setparam(g_buf_r_,g_binary_r_,w/2,h/2,skip,radius,spec_r,mode,g_buf_delta_r_);
    p_binary[Gr]->setparam(g_buf_gr_,g_binary_gr_,w/2,h/2,skip,radius,spec_gr,mode,g_buf_delta_gr_);
    p_binary[Gb]->setparam(g_buf_gb_,g_binary_gb_,w/2,h/2,skip,radius,spec_gb,mode,g_buf_delta_gb_);
    p_binary[B]->setparam(g_buf_b_,g_binary_b_,w/2,h/2,skip,radius,spec_b,mode,g_buf_delta_b_);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }

    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_binary_r_,g_binary_gr_,g_binary_gb_,g_binary_b_,g_binary_,w,h,bayer);

    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_buf_delta_r_,g_buf_delta_gr_,g_buf_delta_gb_,g_buf_delta_b_,g_buf_delta_raw_,w,h,bayer);

//    cv::Mat img_tmp = cv::Mat(h/2,w/2,CV_8U);
//    for (int i=0;i<h/2;i++)
//    {
//        uchar *p=img_tmp.ptr<uchar>(i);
//        for(int j=0;j<w/2;j++)
//            p[j]=g_binary_[i*w/2+j];
//    }
//    cv::imwrite(QString("./cluster_binary_image.bmp").toStdString().c_str(),img_tmp);



    total_cluster_cnt = AlgorithmFuncs::FindALLClusterBayer_in_binary(g_binary_, buf_map,
                                                                      w, h,
                                                                      cnts_cluster,
                                                                      cnt_2x4_pix,
                                                                      cnt_2x2_pix,
                                                                      cnt_1x2_2x1_pix,
                                                                      cnt_2x2_diagonal_pix,
                                                                      cnt_1x2_ch,
                                                                      cnt_2x1_ch,
                                                                      cnt_2x2_leftdiagonal,
                                                                      cnt_2x2_rightdiagonal,
                                                                      cnt_white_pix,
                                                                      cnt_dark_pix,
                                                                      paticle,
                                                                      other_cluster,
                                                                      other_cluster_ch,
                                                                      g_str_msg_);


    cnt_white_pix=0;
    cnt_dark_pix=0;
    for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            if(g_binary_[i*w+j]==255)
            {
                if(g_buf_delta_raw_[i*w+j]>0)
                    cnt_white_pix++;
                else
                    cnt_dark_pix++;
            }

        }

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_binary[i];
    }
}



void TestItem::find_dark_and_white_pixel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                         float *g_buf_delta_r_,float *g_buf_delta_gr_,float *g_buf_delta_gb_,float *g_buf_delta_b_, float *g_buf_delta_raw_,
                                         uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                                         uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
                                         int w, int h, int skip, int radius,
                                         double spec_r, double spec_gr, double spec_gb, double spec_b,
                                         double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
                                         Spec_Mode mode, RGB_BAYER bayer,
                                         QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                                         int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                                         int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                                         int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                                         int &other_cluster_ch, int &total_cluster_cnt,
                                         int blemish_global_size_limit, int blemish_bychannel_size_limit,
                                         int &blemish,int &cnt_white_pix_blemish, int &cnt_dark_pix_blemish,
                                         uchar *buf_map, int save_imgae, std::string &g_str_msg_)
{

    MultiThread::bychannel_pixel_defect_binarization *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_pixel_defect_binarization();
    }

    p_binary[R]->setparam(g_buf_r_,g_buf_delta_r_,g_binary_r_,g_blemish_r_,w/2,h/2,skip,radius,spec_r,spec_blemish_r,mode);
    p_binary[Gr]->setparam(g_buf_gr_,g_buf_delta_gr_,g_binary_gr_,g_blemish_gr_,w/2,h/2,skip,radius,spec_gr,spec_blemish_gr,mode);
    p_binary[Gb]->setparam(g_buf_gb_,g_buf_delta_gb_,g_binary_gb_,g_blemish_gb_,w/2,h/2,skip,radius,spec_gb,spec_blemish_gb,mode);
    p_binary[B]->setparam(g_buf_b_,g_buf_delta_b_,g_binary_b_,g_blemish_b_,w/2,h/2,skip,radius,spec_b,spec_blemish_b,mode);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }


    //若为4C结构，此处存在问题
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_binary_r_,g_binary_gr_,g_binary_gb_,
                                            g_binary_b_,g_binary_,w,h,bayer);
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_blemish_r_,g_blemish_gr_,g_blemish_gb_,
                                             g_blemish_b_,g_blemish_,w,h,bayer);
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_buf_delta_r_,g_buf_delta_gr_,g_buf_delta_gb_,
                                             g_buf_delta_b_,g_buf_delta_raw_,w,h,bayer);

    if(save_imgae)
    {
        cv::Mat img_tmp1 = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp1.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_binary_[i*w+j];
        }
        cv::imwrite(QString("./Pixel_Defect_binary_image.bmp").toStdString().c_str(),img_tmp1);


        cv::Mat img_tmp2 = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp2.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_blemish_[i*w+j];
        }
        cv::imwrite(QString("./Blemish_binary_image.bmp").toStdString().c_str(),img_tmp2);
    }

    clock_t start_time = clock();
    total_cluster_cnt = AlgorithmFuncs::FindALLClusterBayer_in_binary(g_binary_, buf_map,
                                                                      w, h,
                                                                      cnts_cluster,
                                                                      cnt_2x4_pix,
                                                                      cnt_2x2_pix,
                                                                      cnt_1x2_2x1_pix,
                                                                      cnt_2x2_diagonal_pix,
                                                                      cnt_1x2_ch,
                                                                      cnt_2x1_ch,
                                                                      cnt_2x2_leftdiagonal,
                                                                      cnt_2x2_rightdiagonal,
                                                                      cnt_white_pix,
                                                                      cnt_dark_pix,
                                                                      paticle,
                                                                      other_cluster,
                                                                      other_cluster_ch,
                                                                      g_str_msg_);

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (buf_map[w*i+j] > 128)
            {
                if(g_buf_delta_raw_[w*i+j] >0)
                    cnt_white_pix++;
                else
                    cnt_dark_pix++;}
        }
    }
//    cnt_white_pix = cnt_white_pix - cnt_1x2_2x1_pix - cnt_2x2_diagonal_pix;
//    cnt_dark_pix = cnt_dark_pix - cnt_1x2_2x1_pix - cnt_2x2_diagonal_pix;

//    if(cnt_white_pix<0)
//        cnt_white_pix =0;
//    if(cnt_dark_pix<0)
//        cnt_dark_pix =0;


    g_str_msg_+=boost::str(boost::format("single white pixel %d, single dark pixel %d\n")
                           % (cnt_white_pix) % (cnt_dark_pix) );

    clock_t end_time = clock();
    cast_time_step1 =end_time-start_time;
    start_time = clock();


    memcpy(buf_map,g_blemish_,w*h*sizeof(uchar));

    //------------------blemish-----------------------
    blemish = AlgorithmFuncs::Findblemish_in_blemish_binary(g_blemish_,w,h, blemish_global_size_limit,
                                                            blemish_bychannel_size_limit,g_str_msg_);


    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (buf_map[w*i+j] > 128)
            {
                if(g_buf_delta_raw_[w*i+j] >0)
                    cnt_white_pix_blemish++;
                else
                    cnt_dark_pix_blemish++;}
        }
    }
    g_str_msg_+=boost::str(boost::format("single blemish white pixel %d, single blemish dark pixel %d\n")
                           % (cnt_white_pix_blemish) % (cnt_dark_pix_blemish) );
    end_time = clock();
    cast_time_step2 =end_time-start_time;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_binary[i];
    }
}



void TestItem::find_all_single_pixel_defect(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                 float *g_buf_delta_r_,float *g_buf_delta_gr_,float *g_buf_delta_gb_,float *g_buf_delta_b_, float *g_buf_delta_raw_,
                                 uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                                 uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
                                 int w, int h, int skip, int radius,
                                 double spec_r, double spec_gr, double spec_gb, double spec_b,
                                 double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
                                 Spec_Mode mode, RGB_BAYER bayer, int save_imgae, std::string &g_str_msg_, std::vector<MClusterInf> &pd_all_single_particle)
{
    MultiThread::bychannel_pixel_defect_binarization *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_pixel_defect_binarization();
    }

    p_binary[R]->setparam(g_buf_r_,g_buf_delta_r_,g_binary_r_,g_blemish_r_,w/2,h/2,skip,radius,spec_r,spec_blemish_r,mode);
    p_binary[Gr]->setparam(g_buf_gr_,g_buf_delta_gr_,g_binary_gr_,g_blemish_gr_,w/2,h/2,skip,radius,spec_gr,spec_blemish_gr,mode);
    p_binary[Gb]->setparam(g_buf_gb_,g_buf_delta_gb_,g_binary_gb_,g_blemish_gb_,w/2,h/2,skip,radius,spec_gb,spec_blemish_gb,mode);
    p_binary[B]->setparam(g_buf_b_,g_buf_delta_b_,g_binary_b_,g_blemish_b_,w/2,h/2,skip,radius,spec_b,spec_blemish_b,mode);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }

    //若为4C结构，此处存在问题
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_binary_r_,g_binary_gr_,g_binary_gb_,
                                            g_binary_b_,g_binary_,w,h,bayer);

    if(save_imgae)
    {
        cv::Mat img_tmp1 = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp1.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_binary_[i*w+j];
        }
        cv::imwrite(QString("./Pixel_Defect_binary_image.bmp").toStdString().c_str(),img_tmp1);


        cv::Mat img_tmp2 = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp2.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_blemish_[i*w+j];
        }
        cv::imwrite(QString("./Blemish_binary_image.bmp").toStdString().c_str(),img_tmp2);
    }


    // other type
    int i, j;

    for (i = 0; i < h; ++i)
    {
        for (j = 0; j < w; ++j)
        {
            if (g_binary_[i * w + j] == 255)    //单点
            {
                pd_all_single_particle.push_back (MClusterInf(j, i, 1, 1, 0, 0));
            }
        }
    }

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_binary[i];
    }

}



void TestItem::find_pixel_defect(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                 float *g_buf_delta_r_,float *g_buf_delta_gr_,float *g_buf_delta_gb_,float *g_buf_delta_b_, float *g_buf_delta_raw_,
                                 uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                                 uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
                                 int w, int h, int skip, int radius,
                                 double spec_r, double spec_gr, double spec_gb, double spec_b,
                                 double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
                                 Spec_Mode mode, RGB_BAYER bayer,
                                 QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                                 int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                                 int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                                 int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                                 int &other_cluster_ch, int &total_cluster_cnt,
                                 int blemish_global_size_limit, int blemish_bychannel_size_limit,
                                 int &blemish,int &cnt_white_pix_blemish, int &cnt_dark_pix_blemish,
                                 uchar *buf_map, int save_imgae, std::string &g_str_msg_)
{

    MultiThread::bychannel_pixel_defect_binarization *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_pixel_defect_binarization();
    }

    p_binary[R]->setparam(g_buf_r_,g_buf_delta_r_,g_binary_r_,g_blemish_r_,w/2,h/2,skip,radius,spec_r,spec_blemish_r,mode);
    p_binary[Gr]->setparam(g_buf_gr_,g_buf_delta_gr_,g_binary_gr_,g_blemish_gr_,w/2,h/2,skip,radius,spec_gr,spec_blemish_gr,mode);
    p_binary[Gb]->setparam(g_buf_gb_,g_buf_delta_gb_,g_binary_gb_,g_blemish_gb_,w/2,h/2,skip,radius,spec_gb,spec_blemish_gb,mode);
    p_binary[B]->setparam(g_buf_b_,g_buf_delta_b_,g_binary_b_,g_blemish_b_,w/2,h/2,skip,radius,spec_b,spec_blemish_b,mode);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }

    //若为4C结构，此处存在问题
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_binary_r_,g_binary_gr_,g_binary_gb_,
                                            g_binary_b_,g_binary_,w,h,bayer);
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_blemish_r_,g_blemish_gr_,g_blemish_gb_,
                                             g_blemish_b_,g_blemish_,w,h,bayer);
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_buf_delta_r_,g_buf_delta_gr_,g_buf_delta_gb_,
                                             g_buf_delta_b_,g_buf_delta_raw_,w,h,bayer);

    if(save_imgae)
    {
        cv::Mat img_tmp1 = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp1.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_binary_[i*w+j];
        }
        cv::imwrite(QString("./Pixel_Defect_binary_image.bmp").toStdString().c_str(),img_tmp1);


        cv::Mat img_tmp2 = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp2.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_blemish_[i*w+j];
        }
        cv::imwrite(QString("./Blemish_binary_image.bmp").toStdString().c_str(),img_tmp2);
    }

    clock_t start_time = clock();
    total_cluster_cnt = AlgorithmFuncs::FindALLClusterBayer_in_binary(g_binary_, buf_map,
                                                                      w, h,
                                                                      cnts_cluster,
                                                                      cnt_2x4_pix,
                                                                      cnt_2x2_pix,
                                                                      cnt_1x2_2x1_pix,
                                                                      cnt_2x2_diagonal_pix,
                                                                      cnt_1x2_ch,
                                                                      cnt_2x1_ch,
                                                                      cnt_2x2_leftdiagonal,
                                                                      cnt_2x2_rightdiagonal,
                                                                      cnt_white_pix,
                                                                      cnt_dark_pix,
                                                                      paticle,
                                                                      other_cluster,
                                                                      other_cluster_ch,
                                                                      g_str_msg_);

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (g_binary_[w*i+j] > 128)
            {
                if(g_buf_delta_raw_[w*i+j] >0)
                    cnt_white_pix++;
                else
                    cnt_dark_pix++;}
        }
    }
    cnt_white_pix = cnt_white_pix - cnt_1x2_2x1_pix - cnt_2x2_diagonal_pix;
    cnt_dark_pix = cnt_dark_pix - cnt_1x2_2x1_pix - cnt_2x2_diagonal_pix;

    if(cnt_white_pix<0)
        cnt_white_pix =0;
    if(cnt_dark_pix<0)
        cnt_dark_pix =0;


    g_str_msg_+=boost::str(boost::format("single white pixel %d, single dark pixel %d\n")
                           % (cnt_white_pix) % (cnt_dark_pix) );

    clock_t end_time = clock();
    cast_time_step1 =end_time-start_time;
    start_time = clock();


    //------------------blemish-----------------------
    blemish = AlgorithmFuncs::Findblemish_in_blemish_binary(g_blemish_,w,h, blemish_global_size_limit,
                                                            blemish_bychannel_size_limit,g_str_msg_);

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (g_blemish_[w*i+j] > 128)
            {
                if(g_buf_delta_raw_[w*i+j] >0)
                    cnt_white_pix_blemish++;
                else
                    cnt_dark_pix_blemish++;}
        }
    }

    g_str_msg_+=boost::str(boost::format("single blemish white pixel %d, single blemish dark pixel %d\n")
                           % (cnt_white_pix_blemish) % (cnt_dark_pix_blemish) );
    end_time = clock();
    cast_time_step2 =end_time-start_time;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_binary[i];
    }
}



void TestItem::find_channel_single_white_dark_pixel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                       float *g_buf_delta_r_,float *g_buf_delta_gr_,float *g_buf_delta_gb_,float *g_buf_delta_b_, float *g_buf_delta_raw_,
                                       uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                                       uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
                                       int w, int h, int skip, int radius,
                                       double spec_r, double spec_gr, double spec_gb, double spec_b,
                                       double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
                                       Spec_Mode mode, RGB_BAYER bayer,
                                       QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                                       int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                                       int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                                       int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                                       int &other_cluster_ch, int &total_cluster_cnt,
                                       int blemish_global_size_limit, int blemish_bychannel_size_limit,
                                       int &blemish,int &cnt_white_pix_blemish, int &cnt_dark_pix_blemish,
                                       uchar *buf_map, int save_imgae, std::string &g_str_msg_,
                                       int ROI_Channel_X,int ROI_Channel_Y,int ROI_Channel_W,int ROI_Channel_H,
                                       int &cnt_dark_pix_r, int &cnt_dark_pix_gr, int &cnt_dark_pix_gb, int &cnt_dark_pix_b,
                                       int &cnt_white_pix_r, int &cnt_white_pix_gr, int &cnt_white_pix_gb, int &cnt_white_pix_b,
                                       int &crop_cnt_dark_pix_r, int &crop_cnt_dark_pix_gr, int &crop_cnt_dark_pix_gb, int &crop_cnt_dark_pix_b,
                                       int &crop_cnt_white_pix_r, int &crop_cnt_white_pix_gr, int &crop_cnt_white_pix_gb, int &crop_cnt_white_pix_b)
{

    MultiThread::bychannel_pixel_defect_binarization *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_pixel_defect_binarization();
    }

    p_binary[R]->setparam(g_buf_r_,g_buf_delta_r_,g_binary_r_,g_blemish_r_,w/2,h/2,skip,radius,spec_r,spec_blemish_r,mode);
    p_binary[Gr]->setparam(g_buf_gr_,g_buf_delta_gr_,g_binary_gr_,g_blemish_gr_,w/2,h/2,skip,radius,spec_gr,spec_blemish_gr,mode);
    p_binary[Gb]->setparam(g_buf_gb_,g_buf_delta_gb_,g_binary_gb_,g_blemish_gb_,w/2,h/2,skip,radius,spec_gb,spec_blemish_gb,mode);
    p_binary[B]->setparam(g_buf_b_,g_buf_delta_b_,g_binary_b_,g_blemish_b_,w/2,h/2,skip,radius,spec_b,spec_blemish_b,mode);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }


    //若为4C结构，此处存在问题
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_binary_r_,g_binary_gr_,g_binary_gb_,
                                            g_binary_b_,g_binary_,w,h,bayer);
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_blemish_r_,g_blemish_gr_,g_blemish_gb_,
                                             g_blemish_b_,g_blemish_,w,h,bayer);
    AlgorithmFuncs::RGGB_Map_Integrate_chao (g_buf_delta_r_,g_buf_delta_gr_,g_buf_delta_gb_,
                                             g_buf_delta_b_,g_buf_delta_raw_,w,h,bayer);

    if(save_imgae)
    {
        cv::Mat img_tmp1 = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp1.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_binary_[i*w+j];
        }
        cv::imwrite(QString("./Pixel_Defect_binary_image.bmp").toStdString().c_str(),img_tmp1);


        cv::Mat img_tmp2 = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp2.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_blemish_[i*w+j];
        }
        cv::imwrite(QString("./Blemish_binary_image.bmp").toStdString().c_str(),img_tmp2);
    }

    clock_t start_time = clock();
    total_cluster_cnt = AlgorithmFuncs::FindALLClusterBayer_in_binary(g_binary_, buf_map,
                                                                      w, h,
                                                                      cnts_cluster,
                                                                      cnt_2x4_pix,
                                                                      cnt_2x2_pix,
                                                                      cnt_1x2_2x1_pix,
                                                                      cnt_2x2_diagonal_pix,
                                                                      cnt_1x2_ch,
                                                                      cnt_2x1_ch,
                                                                      cnt_2x2_leftdiagonal,
                                                                      cnt_2x2_rightdiagonal,
                                                                      cnt_white_pix,
                                                                      cnt_dark_pix,
                                                                      paticle,
                                                                      other_cluster,
                                                                      other_cluster_ch,
                                                                      g_str_msg_);

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (buf_map[w*i+j] > 128)
            {
                if(g_buf_delta_raw_[w*i+j] >0)
                    cnt_white_pix++;
                else
                    cnt_dark_pix++;}
        }
    }

    //新增
    //分通道单点
    for (int i=0; i<h/2; ++i)
    {
        for (int j=0; j<w/2; ++j)
        {
            if (g_binary_r_[(w/2)*i+j] > 128)
            {
                if(g_buf_delta_r_[(w/2)*i+j] >0)
                    cnt_white_pix_r++;
                else
                    cnt_dark_pix_r++;
            }

            if (g_binary_gr_[(w/2)*i+j] > 128)
            {
                if(g_buf_delta_gr_[(w/2)*i+j] >0)
                    cnt_white_pix_gr++;
                else
                    cnt_dark_pix_gr++;
            }

            if (g_binary_gb_[(w/2)*i+j] > 128)
            {
                if(g_buf_delta_gb_[(w/2)*i+j] >0)
                    cnt_white_pix_gb++;
                else
                    cnt_dark_pix_gb++;
            }

            if (g_binary_b_[(w/2)*i+j] > 128)
            {
                if(g_buf_delta_b_[(w/2)*i+j] >0)
                    cnt_white_pix_b++;
                else
                    cnt_dark_pix_b++;
            }
        }
    }

    //中心区域单点
    for (int i=ROI_Channel_Y; i<(ROI_Channel_Y+ROI_Channel_H); ++i)
    {
        for (int j=ROI_Channel_X; j<(ROI_Channel_X+ROI_Channel_W); ++j)
        {
            if (g_binary_r_[(w/2)*i+j] > 128)
            {
                if(g_buf_delta_r_[(w/2)*i+j] >0)
                    crop_cnt_white_pix_r++;
                else
                    crop_cnt_dark_pix_r++;
            }
            if (g_binary_gr_[(w/2)*i+j] > 128)
            {
                if(g_buf_delta_gr_[(w/2)*i+j] >0)
                    crop_cnt_white_pix_gr++;
                else
                    crop_cnt_dark_pix_gr++;
            }
            if (g_binary_gb_[(w/2)*i+j] > 128)
            {
                if(g_buf_delta_gb_[(w/2)*i+j] >0)
                    crop_cnt_white_pix_gb++;
                else
                    crop_cnt_dark_pix_gb++;
            }
            if (g_binary_b_[(w/2)*i+j] > 128)
            {
                if(g_buf_delta_b_[(w/2)*i+j] >0)
                    crop_cnt_white_pix_b++;
                else
                    crop_cnt_dark_pix_b++;
            }
        }
    }



//    cnt_white_pix = cnt_white_pix - cnt_1x2_2x1_pix - cnt_2x2_diagonal_pix;
//    cnt_dark_pix = cnt_dark_pix - cnt_1x2_2x1_pix - cnt_2x2_diagonal_pix;

//    if(cnt_white_pix<0)
//        cnt_white_pix =0;
//    if(cnt_dark_pix<0)
//        cnt_dark_pix =0;


    g_str_msg_+=boost::str(boost::format("single white pixel %d, single dark pixel %d\n")
                           % (cnt_white_pix) % (cnt_dark_pix) );

    //新增
    g_str_msg_+=boost::str(boost::format("R channel single white pixel %d, single dark pixel %d\n")
                           % (cnt_white_pix_r) % (cnt_dark_pix_r) );
    g_str_msg_+=boost::str(boost::format("Gr channel single white pixel %d, single dark pixel %d\n")
                           % (cnt_white_pix_gr) % (cnt_dark_pix_gr) );
    g_str_msg_+=boost::str(boost::format("Gb channel single white pixel %d, single dark pixel %d\n")
                           % (cnt_white_pix_gb) % (cnt_dark_pix_gb) );
    g_str_msg_+=boost::str(boost::format("B channel single white pixel %d, single dark pixel %d\n")
                           % (cnt_white_pix_b) % (cnt_dark_pix_b) );

    g_str_msg_+=boost::str(boost::format("R channel crop center 200*200 single white pixel %d, single dark pixel %d\n")
                           % (crop_cnt_white_pix_r) % (crop_cnt_dark_pix_r) );
    g_str_msg_+=boost::str(boost::format("Gr channel crop center 200*200 single white pixel %d, single dark pixel %d\n")
                           % (crop_cnt_white_pix_gr) % (crop_cnt_dark_pix_gr) );
    g_str_msg_+=boost::str(boost::format("Gb channel crop center 200*200 single white pixel %d, single dark pixel %d\n")
                           % (crop_cnt_white_pix_gb) % (crop_cnt_dark_pix_gb) );
    g_str_msg_+=boost::str(boost::format("B channel crop center 200*200 single white pixel %d, single dark pixel %d\n")
                           % (crop_cnt_white_pix_b) % (crop_cnt_dark_pix_b) );





    clock_t end_time = clock();
    cast_time_step1 =end_time-start_time;
    start_time = clock();


    memcpy(buf_map,g_blemish_,w*h*sizeof(uchar));

    //------------------blemish-----------------------
    blemish = AlgorithmFuncs::Findblemish_in_blemish_binary(g_blemish_,w,h, blemish_global_size_limit,
                                                            blemish_bychannel_size_limit,g_str_msg_);


    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (buf_map[w*i+j] > 128)
            {
                if(g_buf_delta_raw_[w*i+j] >0)
                    cnt_white_pix_blemish++;
                else
                    cnt_dark_pix_blemish++;}
        }
    }
    g_str_msg_+=boost::str(boost::format("single blemish white pixel %d, single blemish dark pixel %d\n")
                           % (cnt_white_pix_blemish) % (cnt_dark_pix_blemish) );
    end_time = clock();
    cast_time_step2 =end_time-start_time;

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_binary[i];
    }
}



void TestItem::test_channel_darkshading(int w, int h, int x_cnt, int y_cnt,
                                        double &mean_ob_r_max, double &mean_ob_gr_max, double &mean_ob_gb_max, double &mean_ob_b_max,
                                        double &mean_ob_r_min, double &mean_ob_gr_min, double &mean_ob_gb_min, double &mean_ob_b_min,
                                        int &r_max_ob_pos, int &gr_max_ob_pos, int &gb_max_ob_pos, int &b_max_ob_pos,
                                        int &r_min_ob_pos, int &gr_min_ob_pos, int &gb_min_ob_pos, int &b_min_ob_pos,
                                        uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, std::string &msg)
{
    MultiThread::get_darkshading *p_darkshading[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_darkshading[i] = new MultiThread::get_darkshading();
    }
    p_darkshading[R]->setparam(g_buf_r_,w/2,h/2,x_cnt,y_cnt,msg);
    p_darkshading[Gr]->setparam(g_buf_gr_,w/2,h/2,x_cnt,y_cnt,msg);
    p_darkshading[Gb]->setparam(g_buf_gb_,w/2,h/2,x_cnt,y_cnt,msg);
    p_darkshading[B]->setparam(g_buf_b_,w/2,h/2,x_cnt,y_cnt,msg);

    for (int i=0; i < Channel_Num; i++)
    {
        p_darkshading[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_darkshading[i]->wait();
    }

    mean_ob_r_max = p_darkshading[R]->mean_ob_eachcell_max;
    mean_ob_gr_max = p_darkshading[Gr]->mean_ob_eachcell_max;
    mean_ob_gb_max = p_darkshading[Gb]->mean_ob_eachcell_max;
    mean_ob_b_max = p_darkshading[B]->mean_ob_eachcell_max;

    mean_ob_r_min = p_darkshading[R]->mean_ob_eachcell_min;
    mean_ob_gr_min = p_darkshading[Gr]->mean_ob_eachcell_min;
    mean_ob_gb_min = p_darkshading[Gb]->mean_ob_eachcell_min;
    mean_ob_b_min = p_darkshading[B]->mean_ob_eachcell_min;

    r_max_ob_pos = p_darkshading[R]->max_ob_pos;
    gr_max_ob_pos = p_darkshading[Gr]->max_ob_pos;
    gb_max_ob_pos = p_darkshading[Gb]->max_ob_pos;
    b_max_ob_pos = p_darkshading[B]->max_ob_pos;


    r_min_ob_pos = p_darkshading[R]->min_ob_pos;
    gr_min_ob_pos = p_darkshading[Gr]->min_ob_pos;
    gb_min_ob_pos = p_darkshading[Gb]->min_ob_pos;
    b_min_ob_pos = p_darkshading[B]->min_ob_pos;




    logMsg (std::string("Mark1"));


    for(int i=0; i< Channel_Num; i++)
    {
        delete p_darkshading[i];
    }
}




void TestItem::test_deadrow_bychannel_breakline(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                                int w, int h, int skip, int radius, double spec_dol,
                                                double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                                double &max_row_diff_R, double &max_row_diff_Gr, double &max_row_diff_Gb, double &max_row_diff_B,
                                                int &max_row_pos, double &row_max_min_diff, int &cnt_dead_row, int &cnt_dol,
                                                double breakline_row_spec_r, double breakline_row_spec_gr,
                                                double breakline_row_spec_gb, double breakline_row_spec_b,
                                                int &breakline_row_cnt, double &breakline_row_max_value,
                                                int &breakline_row_max_pos, Spec_Mode mode, std::string &msg)
{
    MultiThread::get_channel_deadrow_add_breakline *p_deadrow[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i] = new MultiThread::get_channel_deadrow_add_breakline();
    }

    p_deadrow[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol,0,0,0,0,0,breakline_row_spec_r,0,0,0,mode,"");
    p_deadrow[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol,0,0,0,0,0,breakline_row_spec_gr,0,0,0,mode,"");
    p_deadrow[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol,0,0,0,0,0,breakline_row_spec_gb,0,0,0,mode,"");
    p_deadrow[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol,0,0,0,0,0,breakline_row_spec_b,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadrow[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Max Row Diff value:%.3f; row position:%d\n")
                           % p_deadrow[R]->max_diff_ % p_deadrow[R]->max_pos_);

    msg+=boost::str(boost::format("Gr channel Max Row Diff value:%.3f; row position:%d\n")
                           % p_deadrow[Gr]->max_diff_ % p_deadrow[Gr]->max_pos_);

    msg+=boost::str(boost::format("Gb channel Max Row Diff value:%.3f; row position:%d\n")
                           % p_deadrow[Gb]->max_diff_ % p_deadrow[Gb]->max_pos_);

    msg+=boost::str(boost::format("B channel Max Row Diff value:%.3f; row position:%d\n")
                           % p_deadrow[B]->max_diff_ % p_deadrow[B]->max_pos_);



    msg+=boost::str(boost::format("R channel Max Row Breakline Diff value:%.3f; row breakline position:%d\n")
                           % p_deadrow[R]->breakline_row_max_value_ % p_deadrow[R]->breakline_row_max_pos_);

    msg+=boost::str(boost::format("Gr channel Max Row Breakline Diff value:%.3f; row breakline position:%d\n")
                           % p_deadrow[Gr]->breakline_row_max_value_ % p_deadrow[Gr]->breakline_row_max_pos_);

    msg+=boost::str(boost::format("Gb channel Max Row Breakline Diff value:%.3f; row breakline position:%d\n")
                           % p_deadrow[Gb]->breakline_row_max_value_ % p_deadrow[Gb]->breakline_row_max_pos_);

    msg+=boost::str(boost::format("B channel Max Row Breakline Diff value:%.3f; row breakline position:%d\n")
                           % p_deadrow[B]->breakline_row_max_value_ % p_deadrow[B]->breakline_row_max_pos_);



    double max_row_diff=0;
    for (int i=0; i < Channel_Num; i++)
    {
        if(max_row_diff < p_deadrow[i]->max_diff_)
        {
            max_row_diff = p_deadrow[i]->max_diff_;
            max_row_pos = p_deadrow[i]->max_pos_;
        }
        if(row_max_min_diff < p_deadrow[i]->row_max_min_diff_)
        {
            row_max_min_diff = p_deadrow[i]->row_max_min_diff_;
        }

        cnt_dead_row += p_deadrow[i]->cnt_deadrow_;
        cnt_dol += p_deadrow[i]->cnt_dol_;
        msg += p_deadrow[i]->msg_;
    }

    max_row_diff_R =  p_deadrow[R]->max_diff_;
    max_row_diff_Gr =  p_deadrow[Gr]->max_diff_;
    max_row_diff_Gb =  p_deadrow[Gb]->max_diff_;
    max_row_diff_B =  p_deadrow[B]->max_diff_;





    for (int i=0; i < Channel_Num; i++)
    {
        if(breakline_row_max_value < p_deadrow[i]->breakline_row_max_value_)
        {
            breakline_row_max_value = p_deadrow[i]->breakline_row_max_value_;
            breakline_row_max_pos = 2*p_deadrow[i]->breakline_row_max_pos_;
        }

        breakline_row_cnt += p_deadrow[i]->breakline_row_cnt_;
    }



//    QFile f1("./R_log.txt");
//    if(f1.open(QIODevice::Text|QIODevice::WriteOnly))
//    {
//        QTextStream str(&f1);
//        for(int i=0; i<p_deadrow[R]->delta_each_.size(); i++)
//        {
//        str<<QString::number(p_deadrow[R]->delta_each_.at(i)).toStdString().c_str();
//        str<<"\n";
//        }
//        f1.close();
//    }

//    QFile f2("./Gr_log.txt");
//    if(f2.open(QIODevice::Text|QIODevice::WriteOnly))
//    {
//        QTextStream str(&f2);
//        for(int i=0; i<p_deadrow[Gr]->delta_each_.size(); i++)
//        {
//        str<<QString::number(p_deadrow[Gr]->delta_each_.at(i)).toStdString().c_str();
//        str<<"\n";
//        }
//        f2.close();
//    }

//    QFile f3("./Gb_log.txt");
//    if(f3.open(QIODevice::Text|QIODevice::WriteOnly))
//    {
//        QTextStream str(&f3);
//        for(int i=0; i<p_deadrow[Gb]->delta_each_.size(); i++)
//        {
//        str<<QString::number(p_deadrow[Gb]->delta_each_.at(i)).toStdString().c_str();
//        str<<"\n";
//        }
//        f3.close();
//    }

//    QFile f4("./B_log.txt");
//    if(f4.open(QIODevice::Text|QIODevice::WriteOnly))
//    {
//        QTextStream str(&f4);
//        for(int i=0; i<p_deadrow[B]->delta_each_.size(); i++)
//        {
//        str<<QString::number(p_deadrow[B]->delta_each_.at(i)).toStdString().c_str();
//        str<<"\n";
//        }
//        f4.close();
//    }


    for(int i=0; i< Channel_Num; i++)
    {
        delete p_deadrow[i];
    }
}




void TestItem::test_deadcol_bychannel_breakline(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                                int w, int h, int skip, int radius, double spec_dol,
                                                double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                                double &max_col_diff_R, double &max_col_diff_Gr, double &max_col_diff_Gb, double &max_col_diff_B,
                                                int &max_col_pos, double &col_max_min_diff, int &cnt_dead_col, int &cnt_dol,
                                                double breakline_col_spec_r, double breakline_col_spec_gr,
                                                double breakline_col_spec_gb, double breakline_col_spec_b,
                                                int &breakline_col_cnt, double &breakline_col_max_value,
                                                int &breakline_col_max_pos, Spec_Mode mode, std::string &msg)
{
    MultiThread::get_channel_deadcol_add_breakline *p_deadcol[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i] = new MultiThread::get_channel_deadcol_add_breakline();
    }

    p_deadcol[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol,0,0,0,0,0,breakline_col_spec_r,0,0,0,mode,"");
    p_deadcol[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol,0,0,0,0,0,breakline_col_spec_gr,0,0,0,mode,"");
    p_deadcol[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol,0,0,0,0,0,breakline_col_spec_gb,0,0,0,mode,"");
    p_deadcol[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol,0,0,0,0,0,breakline_col_spec_b,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_deadcol[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Max col Diff value:%.3f; col position:%d\n")
                           % p_deadcol[R]->max_diff_ % p_deadcol[R]->max_pos_);

    msg+=boost::str(boost::format("Gr channel Max col Diff value:%.3f; col position:%d\n")
                           % p_deadcol[Gr]->max_diff_ % p_deadcol[Gr]->max_pos_);

    msg+=boost::str(boost::format("Gb channel Max col Diff value:%.3f; col position:%d\n")
                           % p_deadcol[Gb]->max_diff_ % p_deadcol[Gb]->max_pos_);

    msg+=boost::str(boost::format("B channel Max col Diff value:%.3f; col position:%d\n")
                           % p_deadcol[B]->max_diff_ % p_deadcol[B]->max_pos_);



    msg+=boost::str(boost::format("R channel Max col Breakline Diff value:%.3f; col breakline position:%d\n")
                           % p_deadcol[R]->breakline_col_max_value_ % p_deadcol[R]->breakline_col_max_pos_);

    msg+=boost::str(boost::format("Gr channel Max col Breakline Diff value:%.3f; col breakline position:%d\n")
                           % p_deadcol[Gr]->breakline_col_max_value_ % p_deadcol[Gr]->breakline_col_max_pos_);

    msg+=boost::str(boost::format("Gb channel Max col Breakline Diff value:%.3f; col breakline position:%d\n")
                           % p_deadcol[Gb]->breakline_col_max_value_ % p_deadcol[Gb]->breakline_col_max_pos_);

    msg+=boost::str(boost::format("B channel Max col Breakline Diff value:%.3f; col breakline position:%d\n")
                           % p_deadcol[B]->breakline_col_max_value_ % p_deadcol[B]->breakline_col_max_pos_);



    double max_col_diff=0;
    for (int i=0; i < Channel_Num; i++)
    {
        if(max_col_diff < p_deadcol[i]->max_diff_)
        {
            max_col_diff = p_deadcol[i]->max_diff_;
            max_col_pos = p_deadcol[i]->max_pos_;
        }
        if(col_max_min_diff < p_deadcol[i]->col_max_min_diff_)
        {
            col_max_min_diff = p_deadcol[i]->col_max_min_diff_;
        }

        cnt_dead_col += p_deadcol[i]->cnt_deadcol_;
        cnt_dol += p_deadcol[i]->cnt_dol_;
        msg += p_deadcol[i]->msg_;
    }

    max_col_diff_R =  p_deadcol[R]->max_diff_;
    max_col_diff_Gr =  p_deadcol[Gr]->max_diff_;
    max_col_diff_Gb =  p_deadcol[Gb]->max_diff_;
    max_col_diff_B =  p_deadcol[B]->max_diff_;





    for (int i=0; i < Channel_Num; i++)
    {
        if(breakline_col_max_value < p_deadcol[i]->breakline_col_max_value_)
        {
            breakline_col_max_value = p_deadcol[i]->breakline_col_max_value_;
            breakline_col_max_pos = 2*p_deadcol[i]->breakline_col_max_pos_;
        }

        breakline_col_cnt += p_deadcol[i]->breakline_col_cnt_;
    }



//    QFile f1("./R_log.txt");
//    if(f1.open(QIODevice::Text|QIODevice::WriteOnly))
//    {
//        QTextStream str(&f1);
//        for(int i=0; i<p_deadcol[R]->delta_each_.size(); i++)
//        {
//        str<<QString::number(p_deadcol[R]->delta_each_.at(i)).toStdString().c_str();
//        str<<"\n";
//        }
//        f1.close();
//    }

//    QFile f2("./Gr_log.txt");
//    if(f2.open(QIODevice::Text|QIODevice::WriteOnly))
//    {
//        QTextStream str(&f2);
//        for(int i=0; i<p_deadcol[Gr]->delta_each_.size(); i++)
//        {
//        str<<QString::number(p_deadcol[Gr]->delta_each_.at(i)).toStdString().c_str();
//        str<<"\n";
//        }
//        f2.close();
//    }

//    QFile f3("./Gb_log.txt");
//    if(f3.open(QIODevice::Text|QIODevice::WriteOnly))
//    {
//        QTextStream str(&f3);
//        for(int i=0; i<p_deadcol[Gb]->delta_each_.size(); i++)
//        {
//        str<<QString::number(p_deadcol[Gb]->delta_each_.at(i)).toStdString().c_str();
//        str<<"\n";
//        }
//        f3.close();
//    }

//    QFile f4("./B_log.txt");
//    if(f4.open(QIODevice::Text|QIODevice::WriteOnly))
//    {
//        QTextStream str(&f4);
//        for(int i=0; i<p_deadcol[B]->delta_each_.size(); i++)
//        {
//        str<<QString::number(p_deadcol[B]->delta_each_.at(i)).toStdString().c_str();
//        str<<"\n";
//        }
//        f4.close();
//    }


    for(int i=0; i< Channel_Num; i++)
    {
        delete p_deadcol[i];
    }
}






void TestItem::test_row_defect_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                         int w, int h, int skip, int radius,
                                         double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                                         double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                         double *max_row_diff, int *max_row_pos, double *row_max_min_diff,
                                         int &cnt_dead_row, int &cnt_dol, double *dol_std_max, int *dol_std_max_pos,
                                         double breakline_row_spec_r, double breakline_row_spec_gr,
                                         double breakline_row_spec_gb, double breakline_row_spec_b,
                                         int &breakline_row_cnt, double *breakline_row_max_value,
                                         int *breakline_row_max_pos, Spec_Mode mode, std::string &msg,
                                         int log_print_flag)
{
    MultiThread::get_row_defect_bychannel *p_row_defect[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_row_defect[i] = new MultiThread::get_row_defect_bychannel();
    }

    p_row_defect[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol_R,0,0,0,0,0,0,0,breakline_row_spec_r,0,0,0,mode,"");
    p_row_defect[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol_Gr,0,0,0,0,0,0,0,breakline_row_spec_gr,0,0,0,mode,"");
    p_row_defect[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol_Gb,0,0,0,0,0,0,0,breakline_row_spec_gb,0,0,0,mode,"");
    p_row_defect[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol_B,0,0,0,0,0,0,0,breakline_row_spec_b,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_row_defect[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_row_defect[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Row Max Diff value:%.3f; Max Row position:%d\n"
                                  "R channel Row Max-Min Diff:%.3f\n"
                                  "R channel Row Dol max std value:%.3f; Max Row Dol position:%d\n"
                                  "R channel Row Breakline max delta value:%.3f; Max Row Breakline position:%d\n")
                    % p_row_defect[R]->max_diff_ % p_row_defect[R]->max_pos_
                    % p_row_defect[R]->row_max_min_diff_
                    % p_row_defect[R]->dol_std_max_ % p_row_defect[R]->dol_std_max_pos_
                    % p_row_defect[R]->breakline_row_max_value_ % p_row_defect[R]->breakline_row_max_pos_);


    msg+=boost::str(boost::format("Gr channel Row Max Diff value:%.3f; Max Row position:%d\n"
                                  "Gr channel Row Max-Min Diff:%.3f\n"
                                  "Gr channel Row Dol max std value:%.3f; Max Row Dol position:%d\n"
                                  "Gr channel Row Breakline max delta value:%.3f; Max Row Breakline position:%d\n")
                    % p_row_defect[Gr]->max_diff_ % p_row_defect[Gr]->max_pos_
                    % p_row_defect[Gr]->row_max_min_diff_
                    % p_row_defect[Gr]->dol_std_max_ % p_row_defect[Gr]->dol_std_max_pos_
                    % p_row_defect[Gr]->breakline_row_max_value_ % p_row_defect[Gr]->breakline_row_max_pos_);


    msg+=boost::str(boost::format("Gb channel Row Max Diff value:%.3f; Max Row position:%d\n"
                                  "Gb channel Row Max-Min Diff:%.3f\n"
                                  "Gb channel Row Dol max std value:%.3f; Max Row Dol position:%d\n"
                                  "Gb channel Row Breakline max delta value:%.3f; Max Row Breakline position:%d\n")
                    % p_row_defect[Gb]->max_diff_ % p_row_defect[Gb]->max_pos_
                    % p_row_defect[Gb]->row_max_min_diff_
                    % p_row_defect[Gb]->dol_std_max_ % p_row_defect[Gb]->dol_std_max_pos_
                    % p_row_defect[Gb]->breakline_row_max_value_ % p_row_defect[Gb]->breakline_row_max_pos_);


    msg+=boost::str(boost::format("B channel Row Max Diff value:%.3f; Max Row position:%d\n"
                                  "B channel Row Max-Min Diff:%.3f\n"
                                  "B channel Row Dol max std value:%.3f; Max Row Dol position:%d\n"
                                  "B channel Row Breakline max delta value:%.3f; Max Row Breakline position:%d\n")
                    % p_row_defect[B]->max_diff_ % p_row_defect[B]->max_pos_
                    % p_row_defect[B]->row_max_min_diff_
                    % p_row_defect[B]->dol_std_max_ % p_row_defect[B]->dol_std_max_pos_
                    % p_row_defect[B]->breakline_row_max_value_ % p_row_defect[B]->breakline_row_max_pos_);




    for (int i=0; i < Channel_Num; i++)
    {
        max_row_diff[i] = p_row_defect[i]->max_diff_;
        max_row_pos[i] = p_row_defect[i]->max_pos_;
        row_max_min_diff[i] = p_row_defect[i]->row_max_min_diff_;
        dol_std_max[i] = p_row_defect[i]->dol_std_max_;
        dol_std_max_pos[i] = p_row_defect[i]->dol_std_max_pos_;
        breakline_row_max_value[i] = p_row_defect[i]->breakline_row_max_value_;
        breakline_row_max_pos[i] = p_row_defect[i]->breakline_row_max_pos_;


        cnt_dead_row += p_row_defect[i]->cnt_deadrow_;
        cnt_dol += p_row_defect[i]->cnt_dol_;
        breakline_row_cnt += p_row_defect[i]->breakline_row_cnt_;
        msg += p_row_defect[i]->msg_;
    }


    //——————————————————————————————————————————————————————

    msg+=boost::str(boost::format("SPEC: \n"
                                  "R channel Row Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "R channel Row Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "R channel Row Breakline max delta value:%.3f, (SPEC: %.3f); \n"

                                  "Gr channel Row Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Row Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Row Breakline max delta value:%.3f, (SPEC: %.3f);\n"

                                  "Gb channel Row Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Row Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Row Breakline max delta value:%.3f, (SPEC: %.3f); \n"

                                  "B channel Row Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Row Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Row Breakline max delta value:%.3f, (SPEC: %.3f); \n"
                                  "\n")
                    % p_row_defect[R]->max_diff_ % spec_R
                    % p_row_defect[R]->dol_std_max_ % spec_dol_R
                    % p_row_defect[R]->breakline_row_max_value_ % breakline_row_spec_r

                    % p_row_defect[Gr]->max_diff_ % spec_Gr
                    % p_row_defect[Gr]->dol_std_max_ % spec_dol_Gr
                    % p_row_defect[Gr]->breakline_row_max_value_ % breakline_row_spec_gr

                    % p_row_defect[Gb]->max_diff_ % spec_Gb
                    % p_row_defect[Gb]->dol_std_max_ % spec_dol_Gb
                    % p_row_defect[Gb]->breakline_row_max_value_ % breakline_row_spec_gb

                    % p_row_defect[B]->max_diff_ % spec_B
                    % p_row_defect[B]->dol_std_max_ % spec_dol_B
                    % p_row_defect[B]->breakline_row_max_value_ % breakline_row_spec_b);




    if(log_print_flag)
    {
        QFile f1("./Row_Delta_R_log.txt");
        if(f1.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f1);
            for(int i=0; i<p_row_defect[R]->delta_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[R]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f1.close();
        }

        QFile f2("./Row_Delta_Gr_log.txt");
        if(f2.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f2);
            for(int i=0; i<p_row_defect[Gr]->delta_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[Gr]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f2.close();
        }

        QFile f3("./Row_Delta_Gb_log.txt");
        if(f3.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f3);
            for(int i=0; i<p_row_defect[Gb]->delta_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[Gb]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f3.close();
        }

        QFile f4("./Row_Delta_B_log.txt");
        if(f4.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f4);
            for(int i=0; i<p_row_defect[B]->delta_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[B]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f4.close();
        }


        QFile f5("./Row_Mean_R_log.txt");
        if(f5.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f5);
            for(int i=0; i<p_row_defect[R]->mean_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[R]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f5.close();
        }

        QFile f6("./Row_Mean_Gr_log.txt");
        if(f6.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f6);
            for(int i=0; i<p_row_defect[Gr]->mean_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[Gr]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f6.close();
        }

        QFile f7("./Row_Mean_Gb_log.txt");
        if(f7.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f7);
            for(int i=0; i<p_row_defect[Gb]->mean_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[Gb]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f7.close();
        }

        QFile f8("./Row_Mean_B_log.txt");
        if(f8.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f8);
            for(int i=0; i<p_row_defect[B]->mean_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[B]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f8.close();
        }

        QFile f9("./Row_Diff_R_log.txt");
        if(f9.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f9);
            for(int i=0; i<p_row_defect[R]->diff_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[R]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f9.close();
        }

        QFile f10("./Row_Diff_Gr_log.txt");
        if(f10.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f10);
            for(int i=0; i<p_row_defect[Gr]->diff_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[Gr]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f10.close();
        }

        QFile f11("./Row_Diff_Gb_log.txt");
        if(f11.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f11);
            for(int i=0; i<p_row_defect[Gb]->diff_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[Gb]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f11.close();
        }

        QFile f12("./Row_Diff_B_log.txt");
        if(f12.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f12);
            for(int i=0; i<p_row_defect[B]->diff_each_.size(); i++)
            {
                str<<QString::number(p_row_defect[B]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f12.close();
        }
    }


    for(int i=0; i< Channel_Num; i++)
    {
        delete p_row_defect[i];
    }
}








void TestItem::test_col_defect_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                         int w, int h, int skip, int radius,
                                         double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                                         double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                         double *max_col_diff, int *max_col_pos, double *col_max_min_diff,
                                         int &cnt_dead_col, int &cnt_dol, double *dol_std_max, int *dol_std_max_pos,
                                         double breakline_col_spec_r, double breakline_col_spec_gr,
                                         double breakline_col_spec_gb, double breakline_col_spec_b,
                                         int &breakline_col_cnt, double *breakline_col_max_value,
                                         int *breakline_col_max_pos, Spec_Mode mode, std::string &msg,
                                         int log_print_flag)
{
    MultiThread::get_col_defect_bychannel *p_col_defect[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i] = new MultiThread::get_col_defect_bychannel();
    }

    p_col_defect[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol_R,0,0,0,0,0,0,0,breakline_col_spec_r,0,0,0,mode,"");
    p_col_defect[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol_Gr,0,0,0,0,0,0,0,breakline_col_spec_gr,0,0,0,mode,"");
    p_col_defect[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol_Gb,0,0,0,0,0,0,0,breakline_col_spec_gb,0,0,0,mode,"");
    p_col_defect[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol_B,0,0,0,0,0,0,0,breakline_col_spec_b,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "R channel Col Max-Min Diff:%.3f\n"
                                  "R channel Col Dol max std value:%.3f; Max Col Dol position:%d\n"
                                  "R channel Col Breakline max delta value:%.3f; Max Col Breakline position:%d\n")
                    % p_col_defect[R]->max_diff_ % p_col_defect[R]->max_pos_
                    % p_col_defect[R]->col_max_min_diff_
                    % p_col_defect[R]->dol_std_max_ % p_col_defect[R]->dol_std_max_pos_
                    % p_col_defect[R]->breakline_col_max_value_ % p_col_defect[R]->breakline_col_max_pos_);


    msg+=boost::str(boost::format("Gr channel Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "Gr channel Col Max-Min Diff:%.3f\n"
                                  "Gr channel Col Dol max std value:%.3f; Max Col Dol position:%d\n"
                                  "Gr channel Col Breakline max delta value:%.3f; Max Col Breakline position:%d\n")
                    % p_col_defect[Gr]->max_diff_ % p_col_defect[Gr]->max_pos_
                    % p_col_defect[Gr]->col_max_min_diff_
                    % p_col_defect[Gr]->dol_std_max_ % p_col_defect[Gr]->dol_std_max_pos_
                    % p_col_defect[Gr]->breakline_col_max_value_ % p_col_defect[Gr]->breakline_col_max_pos_);


    msg+=boost::str(boost::format("Gb channel Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "Gb channel Col Max-Min Diff:%.3f\n"
                                  "Gb channel Col Dol max std value:%.3f; Max Col Dol position:%d\n"
                                  "Gb channel Col Breakline max delta value:%.3f; Max Col Breakline position:%d\n")
                    % p_col_defect[Gb]->max_diff_ % p_col_defect[Gb]->max_pos_
                    % p_col_defect[Gb]->col_max_min_diff_
                    % p_col_defect[Gb]->dol_std_max_ % p_col_defect[Gb]->dol_std_max_pos_
                    % p_col_defect[Gb]->breakline_col_max_value_ % p_col_defect[Gb]->breakline_col_max_pos_);


    msg+=boost::str(boost::format("B channel Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "B channel Col Max-Min Diff:%.3f\n"
                                  "B channel Col Dol max std value:%.3f; Max Col Dol position:%d\n"
                                  "B channel Col Breakline max delta value:%.3f; Max Col Breakline position:%d\n")
                    % p_col_defect[B]->max_diff_ % p_col_defect[B]->max_pos_
                    % p_col_defect[B]->col_max_min_diff_
                    % p_col_defect[B]->dol_std_max_ % p_col_defect[B]->dol_std_max_pos_
                    % p_col_defect[B]->breakline_col_max_value_ % p_col_defect[B]->breakline_col_max_pos_);




    for (int i=0; i < Channel_Num; i++)
    {
        max_col_diff[i] = p_col_defect[i]->max_diff_;
        max_col_pos[i] = p_col_defect[i]->max_pos_;
        col_max_min_diff[i] = p_col_defect[i]->col_max_min_diff_;
        dol_std_max[i] = p_col_defect[i]->dol_std_max_;
        dol_std_max_pos[i] = p_col_defect[i]->dol_std_max_pos_;
        breakline_col_max_value[i] = p_col_defect[i]->breakline_col_max_value_;
        breakline_col_max_pos[i] = p_col_defect[i]->breakline_col_max_pos_;


        cnt_dead_col += p_col_defect[i]->cnt_deadcol_;
        cnt_dol += p_col_defect[i]->cnt_dol_;
        breakline_col_cnt += p_col_defect[i]->breakline_col_cnt_;
        msg += p_col_defect[i]->msg_;
    }


    msg+=boost::str(boost::format("SPEC: \n"
                                  "R channel Col Max Diff value:%.3f, (SPEC: %.3f);\n"
                                  "R channel Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "R channel Col Breakline max delta value:%.3f, (SPEC: %.3f); \n"

                                  "Gr channel Col Max Diff value:%.3f, (SPEC: %.3f);\n"
                                  "Gr channel Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Col Breakline max delta value:%.3f, (SPEC: %.3f); \n"

                                  "Gb channel Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Col Breakline max delta value:%.3f, (SPEC: %.3f);\n"

                                  "B channel Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Col Breakline max delta value:%.3f, (SPEC: %.3f); \n"
                                  "\n"
                                  )
                    % p_col_defect[R]->max_diff_ % spec_R
                    % p_col_defect[R]->dol_std_max_ % spec_dol_R
                    % p_col_defect[R]->breakline_col_max_value_ % breakline_col_spec_r
                    % p_col_defect[Gr]->max_diff_ % spec_Gr
                    % p_col_defect[Gr]->dol_std_max_ % spec_dol_Gr
                    % p_col_defect[Gr]->breakline_col_max_value_ % breakline_col_spec_gr
                    % p_col_defect[Gb]->max_diff_ % spec_Gb
                    % p_col_defect[Gb]->dol_std_max_ % spec_dol_Gb
                    % p_col_defect[Gb]->breakline_col_max_value_ % breakline_col_spec_gb
                    % p_col_defect[B]->max_diff_ % spec_B
                    % p_col_defect[B]->dol_std_max_ % spec_dol_B
                    % p_col_defect[B]->breakline_col_max_value_ % breakline_col_spec_b );



    if(log_print_flag)
    {
        QFile f1("./Col_Delta_R_log.txt");
        if(f1.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f1);
            for(int i=0; i<p_col_defect[R]->delta_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[R]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f1.close();
        }

        QFile f2("./Col_Delta_Gr_log.txt");
        if(f2.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f2);
            for(int i=0; i<p_col_defect[Gr]->delta_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gr]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f2.close();
        }

        QFile f3("./Col_Delta_Gb_log.txt");
        if(f3.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f3);
            for(int i=0; i<p_col_defect[Gb]->delta_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gb]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f3.close();
        }

        QFile f4("./Col_Delta_B_log.txt");
        if(f4.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f4);
            for(int i=0; i<p_col_defect[B]->delta_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[B]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f4.close();
        }


        QFile f5("./Col_Mean_R_log.txt");
        if(f5.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f5);
            for(int i=0; i<p_col_defect[R]->mean_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[R]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f5.close();
        }

        QFile f6("./Col_Mean_Gr_log.txt");
        if(f6.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f6);
            for(int i=0; i<p_col_defect[Gr]->mean_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gr]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f6.close();
        }

        QFile f7("./Col_Mean_Gb_log.txt");
        if(f7.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f7);
            for(int i=0; i<p_col_defect[Gb]->mean_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gb]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f7.close();
        }

        QFile f8("./Col_Mean_B_log.txt");
        if(f8.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f8);
            for(int i=0; i<p_col_defect[B]->mean_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[B]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f8.close();
        }

        QFile f9("./Col_Diff_R_log.txt");
        if(f9.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f9);
            for(int i=0; i<p_col_defect[R]->diff_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[R]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f9.close();
        }

        QFile f10("./Col_Diff_Gr_log.txt");
        if(f10.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f10);
            for(int i=0; i<p_col_defect[Gr]->diff_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gr]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f10.close();
        }

        QFile f11("./Col_Diff_Gb_log.txt");
        if(f11.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f11);
            for(int i=0; i<p_col_defect[Gb]->diff_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gb]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f11.close();
        }

        QFile f12("./Col_Diff_B_log.txt");
        if(f12.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f12);
            for(int i=0; i<p_col_defect[B]->diff_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[B]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f12.close();
        }
    }


    for(int i=0; i< Channel_Num; i++)
    {
        delete p_col_defect[i];
    }
}



//仅用于黑屏，区分亮暗线
void TestItem::test_col_defect_bychannel_bright_dark(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                                          int w, int h, int skip, int radius,
                                                          double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                                                          double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                                          double spec_bright_R, double spec_bright_Gr, double spec_bright_Gb, double spec_bright_B,
                                                          double spec_dark_R, double spec_dark_Gr, double spec_dark_Gb, double spec_dark_B,
                                                          double *max_col_diff, int *max_col_pos, double *col_max_min_diff,
                                                          int &cnt_dead_col, int &cnt_dol, double *dol_std_max, int *dol_std_max_pos,
                                                          double breakline_col_spec_r, double breakline_col_spec_gr,
                                                          double breakline_col_spec_gb, double breakline_col_spec_b,
                                                          int &breakline_col_cnt, double *breakline_col_max_value,
                                                          int *breakline_col_max_pos, Spec_Mode mode, std::string &msg,
                                                          int log_print_flag,double *max_bright_col_diff, int *max_bright_col_pos,double *max_dark_col_diff, int *max_dark_col_pos)
{
    MultiThread::get_col_defect_bychannel_bright_dark *p_col_defect[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i] = new MultiThread::get_col_defect_bychannel_bright_dark();
    }

    p_col_defect[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol_R,spec_bright_R,spec_dark_R,0,0,0,0,0,0,0,breakline_col_spec_r,0,0,0,mode,"",0,0,0,0,0);
    p_col_defect[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol_Gr,spec_bright_Gr,spec_dark_Gr,0,0,0,0,0,0,0,breakline_col_spec_gr,0,0,0,mode,"",0,0,0,0,0);
    p_col_defect[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol_Gb,spec_bright_Gb,spec_dark_Gb,0,0,0,0,0,0,0,breakline_col_spec_gb,0,0,0,mode,"",0,0,0,0,0);
    p_col_defect[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol_B,spec_bright_B,spec_dark_B,0,0,0,0,0,0,0,breakline_col_spec_b,0,0,0,mode,"",0,0,0,0,0);

    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "R channel Bright Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "R channel Dark Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "R channel Col Max-Min Diff:%.3f\n"
                                  "R channel Col Dol max std value:%.3f; Max Col Dol position:%d\n"
                                  "R channel Col Breakline max delta value:%.3f; Max Col Breakline position:%d\n")
                    % p_col_defect[R]->max_diff_ % p_col_defect[R]->max_pos_
                    % p_col_defect[R]->max_bright_col_diff_ % p_col_defect[R]->max_bright_col_pos_
                    % p_col_defect[R]->max_dark_col_diff_ % p_col_defect[R]->max_dark_col_pos_
                    % p_col_defect[R]->col_max_min_diff_
                    % p_col_defect[R]->dol_std_max_ % p_col_defect[R]->dol_std_max_pos_
                    % p_col_defect[R]->breakline_col_max_value_ % p_col_defect[R]->breakline_col_max_pos_);


    msg+=boost::str(boost::format("Gr channel Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "Gr channel Bright Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "Gr channel Dark Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "Gr channel Col Max-Min Diff:%.3f\n"
                                  "Gr channel Col Dol max std value:%.3f; Max Col Dol position:%d\n"
                                  "Gr channel Col Breakline max delta value:%.3f; Max Col Breakline position:%d\n")
                    % p_col_defect[Gr]->max_diff_ % p_col_defect[Gr]->max_pos_
                    % p_col_defect[Gr]->max_bright_col_diff_ % p_col_defect[Gr]->max_bright_col_pos_
                    % p_col_defect[Gr]->max_dark_col_diff_ % p_col_defect[Gr]->max_dark_col_pos_
                    % p_col_defect[Gr]->col_max_min_diff_
                    % p_col_defect[Gr]->dol_std_max_ % p_col_defect[Gr]->dol_std_max_pos_
                    % p_col_defect[Gr]->breakline_col_max_value_ % p_col_defect[Gr]->breakline_col_max_pos_);


    msg+=boost::str(boost::format("Gb channel Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "Gb channel Bright Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "Gb channel Dark Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "Gb channel Col Max-Min Diff:%.3f\n"
                                  "Gb channel Col Dol max std value:%.3f; Max Col Dol position:%d\n"
                                  "Gb channel Col Breakline max delta value:%.3f; Max Col Breakline position:%d\n")
                    % p_col_defect[Gb]->max_diff_ % p_col_defect[Gb]->max_pos_
                    % p_col_defect[Gb]->max_bright_col_diff_ % p_col_defect[Gb]->max_bright_col_pos_
                    % p_col_defect[Gb]->max_dark_col_diff_ % p_col_defect[Gb]->max_dark_col_pos_
                    % p_col_defect[Gb]->col_max_min_diff_
                    % p_col_defect[Gb]->dol_std_max_ % p_col_defect[Gb]->dol_std_max_pos_
                    % p_col_defect[Gb]->breakline_col_max_value_ % p_col_defect[Gb]->breakline_col_max_pos_);


    msg+=boost::str(boost::format("B channel Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "B channel Bright Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "B channel Dark Col Max Diff value:%.3f; Max Col position:%d\n"
                                  "B channel Col Max-Min Diff:%.3f\n"
                                  "B channel Col Dol max std value:%.3f; Max Col Dol position:%d\n"
                                  "B channel Col Breakline max delta value:%.3f; Max Col Breakline position:%d\n")
                    % p_col_defect[B]->max_diff_ % p_col_defect[B]->max_pos_
                    % p_col_defect[B]->max_bright_col_diff_ % p_col_defect[B]->max_bright_col_pos_
                    % p_col_defect[B]->max_dark_col_diff_ % p_col_defect[B]->max_dark_col_pos_
                    % p_col_defect[B]->col_max_min_diff_
                    % p_col_defect[B]->dol_std_max_ % p_col_defect[B]->dol_std_max_pos_
                    % p_col_defect[B]->breakline_col_max_value_ % p_col_defect[B]->breakline_col_max_pos_);



    for (int i=0; i < Channel_Num; i++)
    {
        max_col_diff[i] = p_col_defect[i]->max_diff_;
        max_col_pos[i] = p_col_defect[i]->max_pos_;
        //新增
        max_bright_col_diff[i] = p_col_defect[i]->max_bright_col_diff_;
        max_bright_col_pos[i] = p_col_defect[i]->max_bright_col_pos_;
        max_dark_col_diff[i] = p_col_defect[i]->max_dark_col_diff_;
        max_dark_col_pos[i] = p_col_defect[i]->max_dark_col_pos_;

        col_max_min_diff[i] = p_col_defect[i]->col_max_min_diff_;
        dol_std_max[i] = p_col_defect[i]->dol_std_max_;
        dol_std_max_pos[i] = p_col_defect[i]->dol_std_max_pos_;
        breakline_col_max_value[i] = p_col_defect[i]->breakline_col_max_value_;
        breakline_col_max_pos[i] = p_col_defect[i]->breakline_col_max_pos_;


        cnt_dead_col += p_col_defect[i]->cnt_deadcol_;
        cnt_dol += p_col_defect[i]->cnt_dol_;
        breakline_col_cnt += p_col_defect[i]->breakline_col_cnt_;
        msg += p_col_defect[i]->msg_;
    }


    msg+=boost::str(boost::format("SPEC: \n"
                                  "R channel Col Max Diff value:%.3f, (SPEC: %.3f);\n"
                                  "R channel Bright Col Max Diff value:%.3f, (SPEC: %.3f);\n"
                                  "R channel Dark Col Max Diff value:%.3f, (SPEC: %.3f);\n"
                                  "R channel Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "R channel Col Breakline max delta value:%.3f, (SPEC: %.3f); \n"

                                  "Gr channel Col Max Diff value:%.3f, (SPEC: %.3f);\n"
                                  "Gr channel Bright Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Dark Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Col Breakline max delta value:%.3f, (SPEC: %.3f); \n"

                                  "Gb channel Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Bright Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Dark Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Col Breakline max delta value:%.3f, (SPEC: %.3f);\n"

                                  "B channel Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Bright Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Dark Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Col Breakline max delta value:%.3f, (SPEC: %.3f); \n"
                                  "\n"
                                  )
                    % p_col_defect[R]->max_diff_ % spec_R
                    % p_col_defect[R]->max_bright_col_diff_ % spec_bright_R
                    % p_col_defect[R]->max_dark_col_diff_ % spec_dark_R
                    % p_col_defect[R]->dol_std_max_ % spec_dol_R
                    % p_col_defect[R]->breakline_col_max_value_ % breakline_col_spec_r

                    % p_col_defect[Gr]->max_diff_ % spec_Gr
                    % p_col_defect[Gr]->max_bright_col_diff_ % spec_bright_Gr
                    % p_col_defect[Gr]->max_dark_col_diff_ % spec_dark_Gr
                    % p_col_defect[Gr]->dol_std_max_ % spec_dol_Gr
                    % p_col_defect[Gr]->breakline_col_max_value_ % breakline_col_spec_gr

                    % p_col_defect[Gb]->max_diff_ % spec_Gb
                    % p_col_defect[Gb]->max_bright_col_diff_ % spec_bright_Gb
                    % p_col_defect[Gb]->max_dark_col_diff_ % spec_dark_Gb
                    % p_col_defect[Gb]->dol_std_max_ % spec_dol_Gb
                    % p_col_defect[Gb]->breakline_col_max_value_ % breakline_col_spec_gb

                    % p_col_defect[B]->max_diff_ % spec_B
                    % p_col_defect[B]->max_bright_col_diff_ % spec_bright_B
                    % p_col_defect[B]->max_dark_col_diff_ % spec_dark_B
                    % p_col_defect[B]->dol_std_max_ % spec_dol_B
                    % p_col_defect[B]->breakline_col_max_value_ % breakline_col_spec_b );


    if(log_print_flag)
    {
        QFile f1("./Col_Delta_R_log.txt");
        if(f1.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f1);
            for(int i=0; i<p_col_defect[R]->delta_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[R]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f1.close();
        }

        QFile f2("./Col_Delta_Gr_log.txt");
        if(f2.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f2);
            for(int i=0; i<p_col_defect[Gr]->delta_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gr]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f2.close();
        }

        QFile f3("./Col_Delta_Gb_log.txt");
        if(f3.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f3);
            for(int i=0; i<p_col_defect[Gb]->delta_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gb]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f3.close();
        }

        QFile f4("./Col_Delta_B_log.txt");
        if(f4.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f4);
            for(int i=0; i<p_col_defect[B]->delta_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[B]->delta_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f4.close();
        }


        QFile f5("./Col_Mean_R_log.txt");
        if(f5.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f5);
            for(int i=0; i<p_col_defect[R]->mean_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[R]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f5.close();
        }

        QFile f6("./Col_Mean_Gr_log.txt");
        if(f6.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f6);
            for(int i=0; i<p_col_defect[Gr]->mean_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gr]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f6.close();
        }

        QFile f7("./Col_Mean_Gb_log.txt");
        if(f7.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f7);
            for(int i=0; i<p_col_defect[Gb]->mean_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gb]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f7.close();
        }

        QFile f8("./Col_Mean_B_log.txt");
        if(f8.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f8);
            for(int i=0; i<p_col_defect[B]->mean_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[B]->mean_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f8.close();
        }

        QFile f9("./Col_Diff_R_log.txt");
        if(f9.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f9);
            for(int i=0; i<p_col_defect[R]->diff_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[R]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f9.close();
        }

        QFile f10("./Col_Diff_Gr_log.txt");
        if(f10.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f10);
            for(int i=0; i<p_col_defect[Gr]->diff_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gr]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f10.close();
        }

        QFile f11("./Col_Diff_Gb_log.txt");
        if(f11.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f11);
            for(int i=0; i<p_col_defect[Gb]->diff_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[Gb]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f11.close();
        }

        QFile f12("./Col_Diff_B_log.txt");
        if(f12.open(QIODevice::Text|QIODevice::WriteOnly))
        {
            QTextStream str(&f12);
            for(int i=0; i<p_col_defect[B]->diff_each_.size(); i++)
            {
                str<<QString::number(p_col_defect[B]->diff_each_.at(i)).toStdString().c_str();
                str<<"\n";
            }
            f12.close();
        }
    }


    for(int i=0; i< Channel_Num; i++)
    {
        delete p_col_defect[i];
    }
}


void TestItem::test_edge_row_defect_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                              int w, int h, int skip, int radius,
                                              double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                                              double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                              double *max_row_diff, int *max_row_pos, int &cnt_dead_row, int &cnt_dol,
                                              double *dol_std_max, int *dol_std_max_pos, Spec_Mode mode, std::string &msg)
{
    MultiThread::get_edge_row_defect_bychannel *p_row_defect[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_row_defect[i] = new MultiThread::get_edge_row_defect_bychannel();
    }

    p_row_defect[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol_R,0,0,0,0,0,0,mode,"");
    p_row_defect[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol_Gr,0,0,0,0,0,0,mode,"");
    p_row_defect[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol_Gb,0,0,0,0,0,0,mode,"");
    p_row_defect[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol_B,0,0,0,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_row_defect[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_row_defect[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Edge Row Max Diff value:%.3f; Max Edge Row position:%d\n"
                                  "R channel Edge Row Dol max std value:%.3f; Max Edge Row Dol position:%d\n")
                    % p_row_defect[R]->max_diff_ % p_row_defect[R]->max_pos_
                    % p_row_defect[R]->dol_std_max_ % p_row_defect[R]->dol_std_max_pos_);


    msg+=boost::str(boost::format("Gr channel Edge Row Max Diff value:%.3f; Max Edge Row position:%d\n"
                                  "Gr channel Edge Row Dol max std value:%.3f; Max Edge Row Dol position:%d\n")
                    % p_row_defect[Gr]->max_diff_ % p_row_defect[Gr]->max_pos_
                    % p_row_defect[Gr]->dol_std_max_ % p_row_defect[Gr]->dol_std_max_pos_);


    msg+=boost::str(boost::format("Gb channel Edge Row Max Diff value:%.3f; Max Edge Row position:%d\n"
                                  "Gb channel Edge Row Dol max std value:%.3f; Max Edge Row Dol position:%d\n")
                    % p_row_defect[Gb]->max_diff_ % p_row_defect[Gb]->max_pos_
                    % p_row_defect[Gb]->dol_std_max_ % p_row_defect[Gb]->dol_std_max_pos_);


    msg+=boost::str(boost::format("B channel Edge Row Max Diff value:%.3f; Max Edge Row position:%d\n"
                                  "B channel Edge Row Dol max std value:%.3f; Max Edge Row Dol position:%d\n")
                    % p_row_defect[B]->max_diff_ % p_row_defect[B]->max_pos_
                    % p_row_defect[B]->dol_std_max_ % p_row_defect[B]->dol_std_max_pos_);


    for (int i=0; i < Channel_Num; i++)
    {
        max_row_diff[i] = p_row_defect[i]->max_diff_;
        max_row_pos[i] = p_row_defect[i]->max_pos_;
        dol_std_max[i] = p_row_defect[i]->dol_std_max_;
        dol_std_max_pos[i] = p_row_defect[i]->dol_std_max_pos_;


        cnt_dead_row += p_row_defect[i]->cnt_deadrow_;
        cnt_dol += p_row_defect[i]->cnt_dol_;
        msg += p_row_defect[i]->msg_;
    }

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_row_defect[i];
    }


    msg+=boost::str(boost::format("SPEC:\n"
                                  "R channel Edge Row Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "R channel Edge Row Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Edge Row Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Edge Row Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Edge Row Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Edge Row Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Edge Row Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Edge Row Dol max std value:%.3f, (SPEC: %.3f); \n"

                                  )
                    % p_row_defect[R]->max_diff_ % spec_R
                    % p_row_defect[R]->dol_std_max_ % spec_dol_R
                    % p_row_defect[Gr]->max_diff_ % spec_Gr
                    % p_row_defect[Gr]->dol_std_max_ % spec_dol_Gr
                    % p_row_defect[Gb]->max_diff_ % spec_Gb
                    % p_row_defect[Gb]->dol_std_max_ % spec_dol_Gb
                    % p_row_defect[B]->max_diff_ % spec_B
                    % p_row_defect[B]->dol_std_max_ % spec_dol_B );


}







void TestItem::test_edge_col_defect_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                              int w, int h, int skip, int radius,
                                              double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                                              double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                              double *max_col_diff, int *max_col_pos, int &cnt_dead_col, int &cnt_dol,
                                              double *dol_std_max, int *dol_std_max_pos, Spec_Mode mode, std::string &msg)
{
    MultiThread::get_edge_col_defect_bychannel *p_col_defect[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i] = new MultiThread::get_edge_col_defect_bychannel();
    }

    p_col_defect[R]->setparam(g_buf_r_,w/2,h/2,radius,skip,spec_R,spec_dol_R,0,0,0,0,0,0,mode,"");
    p_col_defect[Gr]->setparam(g_buf_gr_,w/2,h/2,radius,skip,spec_Gr,spec_dol_Gr,0,0,0,0,0,0,mode,"");
    p_col_defect[Gb]->setparam(g_buf_gb_,w/2,h/2,radius,skip,spec_Gb,spec_dol_Gb,0,0,0,0,0,0,mode,"");
    p_col_defect[B]->setparam(g_buf_b_,w/2,h/2,radius,skip,spec_B,spec_dol_B,0,0,0,0,0,0,mode,"");

    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i]->start();
    }

    for (int i=0; i < Channel_Num; i++)
    {
        p_col_defect[i]->wait();
    }

    msg+=boost::str(boost::format("R channel Edge Col Max Diff value:%.3f; Max Edge Col position:%d\n"
                                  "R channel Edge Col Dol max std value:%.3f; Max Edge Col Dol position:%d\n")
                    % p_col_defect[R]->max_diff_ % p_col_defect[R]->max_pos_
                    % p_col_defect[R]->dol_std_max_ % p_col_defect[R]->dol_std_max_pos_);


    msg+=boost::str(boost::format("Gr channel Edge Col Max Diff value:%.3f; Max Edge Col position:%d\n"
                                  "Gr channel Edge Col Dol max std value:%.3f; Max Edge Col Dol position:%d\n")
                    % p_col_defect[Gr]->max_diff_ % p_col_defect[Gr]->max_pos_
                    % p_col_defect[Gr]->dol_std_max_ % p_col_defect[Gr]->dol_std_max_pos_);


    msg+=boost::str(boost::format("Gb channel Edge Col Max Diff value:%.3f; Max Edge Col position:%d\n"
                                  "Gb channel Edge Col Dol max std value:%.3f; Max Edge Col Dol position:%d\n")
                    % p_col_defect[Gb]->max_diff_ % p_col_defect[Gb]->max_pos_
                    % p_col_defect[Gb]->dol_std_max_ % p_col_defect[Gb]->dol_std_max_pos_);


    msg+=boost::str(boost::format("B channel Edge Col Max Diff value:%.3f; Max Edge Col position:%d\n"
                                  "B channel Edge Col Dol max std value:%.3f; Max Edge Col Dol position:%d\n")
                    % p_col_defect[B]->max_diff_ % p_col_defect[B]->max_pos_
                    % p_col_defect[B]->dol_std_max_ % p_col_defect[B]->dol_std_max_pos_);


    for (int i=0; i < Channel_Num; i++)
    {
        max_col_diff[i] = p_col_defect[i]->max_diff_;
        max_col_pos[i] = p_col_defect[i]->max_pos_;
        dol_std_max[i] = p_col_defect[i]->dol_std_max_;
        dol_std_max_pos[i] = p_col_defect[i]->dol_std_max_pos_;


        cnt_dead_col += p_col_defect[i]->cnt_deadcol_;
        cnt_dol += p_col_defect[i]->cnt_dol_;
        msg += p_col_defect[i]->msg_;
    }

    for(int i=0; i< Channel_Num; i++)
    {
        delete p_col_defect[i];
    }



    msg+=boost::str(boost::format("SPEC:\n"
                                  "R channel Edge Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "R channel Edge Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Edge Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gr channel Edge Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Edge Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "Gb channel Edge Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Edge Col Max Diff value:%.3f, (SPEC: %.3f); \n"
                                  "B channel Edge Col Dol max std value:%.3f, (SPEC: %.3f); \n"
                                  )
                    % p_col_defect[R]->max_diff_  % spec_R
                    % p_col_defect[R]->dol_std_max_ % spec_dol_R
                    % p_col_defect[Gr]->max_diff_  % spec_Gr
                    % p_col_defect[Gr]->dol_std_max_  % spec_dol_Gr
                    % p_col_defect[Gb]->max_diff_  % spec_Gb
                    % p_col_defect[Gb]->dol_std_max_  % spec_dol_Gb
                    % p_col_defect[B]->max_diff_  % spec_B
                    % p_col_defect[B]->dol_std_max_  % spec_dol_B
                    );

}


void TestItem::test_roi_ob_bychannel(int width, int height, int REF,
                                     QVector<cv::Rect> &roi,
                                     QVector<double> &ob_r_roi,
                                     QVector<double> &ob_gr_roi,
                                     QVector<double> &ob_gb_roi,
                                     QVector<double> &ob_b_roi,
                                     uchar *g_buf_r_, uchar *g_buf_gr_,
                                     uchar *g_buf_gb_, uchar *g_buf_b_)
{

    int Ref = REF;
    int ROI_CNT = roi.size();
    vector<MultiThread::get_roi_channel_mean *> p_response(Channel_Num*ROI_CNT);
//    MultiThread::get_roi_channel_mean *p_response[Channel_Num*ROI_CNT];
    for (int i=0; i < Channel_Num*ROI_CNT; i++)
    {
        p_response[i] = new MultiThread::get_roi_channel_mean();
    }


    for (int roi_cnt = 0; roi_cnt <ROI_CNT; roi_cnt++)
    {
        p_response[roi_cnt*Channel_Num+0]->setparam(g_buf_r_,width/2,height/2,roi.at(roi_cnt).x,roi.at(roi_cnt).y,
                                                  roi.at(roi_cnt).width,roi.at(roi_cnt).height);

        p_response[roi_cnt*Channel_Num+1]->setparam(g_buf_gr_,width/2,height/2,roi.at(roi_cnt).x,roi.at(roi_cnt).y,
                                                    roi.at(roi_cnt).width,roi.at(roi_cnt).height);

        p_response[roi_cnt*Channel_Num+2]->setparam(g_buf_gb_,width/2,height/2,roi.at(roi_cnt).x,roi.at(roi_cnt).y,
                                                    roi.at(roi_cnt).width,roi.at(roi_cnt).height);

        p_response[roi_cnt*Channel_Num+3]->setparam(g_buf_b_,width/2,height/2,roi.at(roi_cnt).x,roi.at(roi_cnt).y,
                                                    roi.at(roi_cnt).width,roi.at(roi_cnt).height);
    }

    for (int i=0; i < ROI_CNT*Channel_Num; i++)
    {
        p_response[i]->start();
    }

    for (int i=0; i < ROI_CNT*Channel_Num; i++)
    {
        p_response[i]->wait();
    }

    for (int roi_cnt = 0; roi_cnt <ROI_CNT; roi_cnt++)
    {
        double mean_r = p_response[roi_cnt*Channel_Num+0]->mean;
        double mean_gr = p_response[roi_cnt*Channel_Num+1]->mean;
        double mean_gb = p_response[roi_cnt*Channel_Num+2]->mean;
        double mean_b = p_response[roi_cnt*Channel_Num+3]->mean;
        ob_r_roi.append(mean_r-Ref);
        ob_gr_roi.append(mean_gr-Ref);
        ob_gb_roi.append(mean_gb-Ref);
        ob_b_roi.append(mean_b-Ref);
    }

    for(int i=0; i< ROI_CNT*Channel_Num; i++)
    {
        delete p_response[i];
    }
}



void TestItem::test_roi_shading_to_center_bychannel(int width, int height,
                                     QVector<cv::Rect> &roi,
                                     QVector<double> &ob_r_roi,
                                     QVector<double> &ob_gr_roi,
                                     QVector<double> &ob_gb_roi,
                                     QVector<double> &ob_b_roi,
                                     double &center_mean_r,     // ★ 引用传出
                                     double &center_mean_gr,    // ★ 引用传出
                                     double &center_mean_gb,    // ★ 引用传出
                                     double &center_mean_b,     // ★ 引用传出
                                     uchar *g_buf_r_, uchar *g_buf_gr_,
                                     uchar *g_buf_gb_, uchar *g_buf_b_)
{
    const int CENTER_IDX = 2;
    int ROI_CNT = roi.size();
    vector<MultiThread::get_roi_channel_mean *> p_response(Channel_Num*ROI_CNT);

    for (int i=0; i < Channel_Num*ROI_CNT; i++)
    {
        p_response[i] = new MultiThread::get_roi_channel_mean();
    }

    for (int roi_cnt = 0; roi_cnt <ROI_CNT; roi_cnt++)
    {
        p_response[roi_cnt*Channel_Num+0]->setparam(g_buf_r_,width/2,height/2,roi.at(roi_cnt).x,roi.at(roi_cnt).y,
                                                  roi.at(roi_cnt).width,roi.at(roi_cnt).height);

        p_response[roi_cnt*Channel_Num+1]->setparam(g_buf_gr_,width/2,height/2,roi.at(roi_cnt).x,roi.at(roi_cnt).y,
                                                    roi.at(roi_cnt).width,roi.at(roi_cnt).height);

        p_response[roi_cnt*Channel_Num+2]->setparam(g_buf_gb_,width/2,height/2,roi.at(roi_cnt).x,roi.at(roi_cnt).y,
                                                    roi.at(roi_cnt).width,roi.at(roi_cnt).height);

        p_response[roi_cnt*Channel_Num+3]->setparam(g_buf_b_,width/2,height/2,roi.at(roi_cnt).x,roi.at(roi_cnt).y,
                                                    roi.at(roi_cnt).width,roi.at(roi_cnt).height);
    }

    for (int i=0; i < ROI_CNT*Channel_Num; i++)
    {
        p_response[i]->start();
    }

    for (int i=0; i < ROI_CNT*Channel_Num; i++)
    {
        p_response[i]->wait();
    }

    // ★ 提取中心区域均值（直接赋值给引用参数，传递出去）
    center_mean_r  = p_response[CENTER_IDX * Channel_Num + 0]->mean;
    center_mean_gr = p_response[CENTER_IDX * Channel_Num + 1]->mean;
    center_mean_gb = p_response[CENTER_IDX * Channel_Num + 2]->mean;
    center_mean_b  = p_response[CENTER_IDX * Channel_Num + 3]->mean;

    for (int roi_cnt = 0; roi_cnt <ROI_CNT; roi_cnt++)
    {
        double mean_r = p_response[roi_cnt*Channel_Num+0]->mean;
        double mean_gr = p_response[roi_cnt*Channel_Num+1]->mean;
        double mean_gb = p_response[roi_cnt*Channel_Num+2]->mean;
        double mean_b = p_response[roi_cnt*Channel_Num+3]->mean;
        ob_r_roi.append(mean_r  - center_mean_r);
        ob_gr_roi.append(mean_gr - center_mean_gr);
        ob_gb_roi.append(mean_gb - center_mean_gb);
        ob_b_roi.append(mean_b  - center_mean_b);
    }

    for(int i=0; i< ROI_CNT*Channel_Num; i++)
    {
        delete p_response[i];
    }
}



int TestItem::test_pixel_cluster_in_smooth_image(cv::Mat srcImage, int resize_w, int resize_h, double lsc_ratio,
                                                  int r, int skip, double spec, int defect_size_limit,
                                                 Spec_Mode mode,std::string &msg, int save_image)
{
    int defect =0;
    //----------------------Smooth---------------------------

    cv::Mat srcImage_32;
    srcImage.convertTo(srcImage_32,CV_32FC1);

    cv::Mat image_mblur;
    cv::medianBlur(srcImage_32,image_mblur,3);

    cv::Mat grayImage(image_mblur.size(),CV_32FC1);
    double s=1.0/16;
    cv::Mat kern = (cv::Mat_<double>(3,3) << s,2*s,s,2*s,4*s,2*s,s,2*s,s);
    cv::filter2D(image_mblur,grayImage,image_mblur.depth(),kern);

    cv::Mat src_small;
    cv::resize(grayImage,src_small,cv::Size(resize_w,resize_h),0,0,cv::INTER_LINEAR);

    cv::Mat  lsc_img = src_small;


    double mean_corner =0;
    for(int i=0; i<30; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        for(int j=0; j<40; j++)
        {
            mean_corner += p_row_small[j];

        }
    }

    double mean_center =0;
    for(int i=src_small.rows/2-15; i<src_small.rows/2+15; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        for(int j=src_small.cols/2-20; j<src_small.cols/2+20; j++)
        {
            mean_center += p_row_small[j];

        }
    }

    double delta = (mean_center/mean_corner -1)*lsc_ratio;

    double factor = delta/((resize_w/2)*(resize_w/2) + (resize_h/2)*(resize_h/2));

    for(int i=0; i<src_small.rows; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        float *p_row_lsc = lsc_img.ptr<float>(i);
        int y = std::abs(i-resize_h/2);
        for(int j=0; j<src_small.cols; j++)
        {
            int x = std::abs(j-resize_w/2);
            p_row_lsc[j] = p_row_small[j] * ((x*x + y*y) * factor + 1);
        }
    }



    cv::Mat  enhance;
    double  minV, maxV;
    int  minId[2] = {}, maxId[2] = {};	// minnimum Index, maximum Index

    cv::minMaxIdx(lsc_img, &minV, &maxV, minId, maxId);

    double max_ds = 250;
    double min_ds = 50;

    enhance = (lsc_img - minV)*(max_ds-min_ds)/(maxV-minV)+min_ds;

    cv::Mat blur_mat;
    cv::blur(enhance,blur_mat,cv::Size(5,5));

    cv::Mat buf_mat;
    blur_mat.convertTo(buf_mat,CV_8U);
    if(save_image)
        cv::imwrite(QString("./smooth_image.bmp").toStdString().c_str(),buf_mat);


    uchar *g_mat_buf = new uchar[resize_w*resize_h];

    for (int i=0;i<resize_h;i++)
    {
        uchar *p=buf_mat.ptr<uchar>(i);
        for(int j=0;j<resize_w;j++)
            g_mat_buf[i*resize_w+j]=p[j];
    }
    //--------------------Pixel Cluster Test in Smooth Image----------------------

    uchar *g_binary_map = new uchar[resize_w*resize_h];

    int w =resize_w;
    int h =resize_h;
    int skipCnt =skip;
    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        for (int j=skipCnt; j<w-skipCnt; ++j)
        {
            double mean_t[4] = {0.0};
            int cnt = 0;
            int row_start = i-r;
            int row_end = i+r;

            if (row_start<0)
            {
                row_start = 0;
                row_end = row_start+2*r;
            }
            else if (row_end>(h-1))
            {
                row_end = h-1;
                row_start = row_end-2*r;
            }
            int col_start = j-r;
            int col_end = j+r;
            if (col_start<0)
            {
                col_start = 0;
                col_end = col_start+2*r;
            }
            else if (col_end>(w-1))
            {
                col_end = w-1;
                col_start = col_end-2*r;
            }


            for(int y=0; y<r; y++)
                for(int x=0; x<r; x++)
                {
                    mean_t[0]+=(double)g_mat_buf[w*(row_start+y)+(col_start+x)];
                    mean_t[1]+=(double)g_mat_buf[w*(row_start+y)+(col_start+x+r+1)];
                    mean_t[2]+=(double)g_mat_buf[w*(row_start+y+r+1)+(col_start+x)];
                    mean_t[3]+=(double)g_mat_buf[w*(row_start+y+r+1)+(col_start+x+r+1)];
                    cnt++;
                }

            if (cnt <= 1)
            {
                continue;
            }

            for(int n=0; n<4; n++)
                mean_t[n]/=cnt;

            double compareValue[4] = {0.0};

            if(mode==Ratio)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)g_mat_buf[w*i+j]-mean_t[n])/mean_t[n];
            }
            else if(mode==DN)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)g_mat_buf[w*i+j]-mean_t[n]);
            }

            int flag=0;
            for(int n=0; n<4; n++)
                if(compareValue[n]>spec)
                    flag++;


            if (flag>0)
            {
                g_binary_map[w*i+j] = 255;
            }
            else
            {
                g_binary_map[w*i+j] = 0;
            }
        }
    }

    std::vector<AlgorithmFuncs::MPDInf> clusters;
    AlgorithmFuncs::defectCounter (g_binary_map,w,h,128,clusters);
    BOOST_FOREACH(const AlgorithmFuncs::MPDInf &cluster, clusters)
    {
        if (cluster.w_>defect_size_limit ||
                cluster.h_>defect_size_limit)
        {
            defect+=1;
            if (defect<=5)
            {
                msg+=boost::str(boost::format("pixel defect location x:%d y:%d w:%d h:%d\n")
                                % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
        }
    }

    delete []g_mat_buf;
    delete []g_binary_map;

    return defect;
}


int TestItem::TestFunc_LSC(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, int w, int h, int skip, int roi_x_cnt, int roi_y_cnt, int set_x_index, int set_y_index, int PixelBitCnt, bool debugmode, double &out_DiffCorner, double &out_Maxdiff, string &msg)
{
    // ========== 1. ROI 均值计算 ==========
    const int total_roi_cnt = roi_x_cnt * roi_y_cnt;
    if (total_roi_cnt <= 0 || roi_x_cnt < 5 || roi_y_cnt < 5) {
        msg.append("ROI count too small for color shading evaluation!\n");
        return -1;
    }

    const int ch_w = w / 2;
    const int ch_h = h / 2;
    const int roi_x_pitch = (ch_w - skip) / roi_x_cnt;
    const int roi_y_pitch = (ch_h - skip) / roi_y_cnt;

    std::vector<double> R_mean(total_roi_cnt), Gr_mean(total_roi_cnt);
    std::vector<double> Gb_mean(total_roi_cnt), B_mean(total_roi_cnt);

    for (int i = 0; i < roi_y_cnt; i++)
    {
        for (int j = 0; j < roi_x_cnt; j++)
        {
            const int idx = i * roi_x_cnt + j;
            const int roi_x = skip / 2 + j * roi_x_pitch;
            const int roi_y = skip / 2 + i * roi_y_pitch;

            R_mean[idx]  = AlgorithmFuncs::GetROIChannelMean(g_buf_r_,  ch_w, ch_h, roi_x, roi_y, roi_x_pitch, roi_y_pitch);
            Gr_mean[idx] = AlgorithmFuncs::GetROIChannelMean(g_buf_gr_, ch_w, ch_h, roi_x, roi_y, roi_x_pitch, roi_y_pitch);
            Gb_mean[idx] = AlgorithmFuncs::GetROIChannelMean(g_buf_gb_, ch_w, ch_h, roi_x, roi_y, roi_x_pitch, roi_y_pitch);
            B_mean[idx]  = AlgorithmFuncs::GetROIChannelMean(g_buf_b_,  ch_w, ch_h, roi_x, roi_y, roi_x_pitch, roi_y_pitch);
        }
    }

    // ========== 2. 计算 R*B/(Gr*Gb) 比率图 ==========
    constexpr double EPS = 1e-8;
    std::vector<double> RB_GrGb(total_roi_cnt);
    for (int i = 0; i < total_roi_cnt; i++) {
        RB_GrGb[i] = (R_mean[i] * B_mean[i]) /
                     (Gr_mean[i] * Gb_mean[i] + EPS);
    }

    // ========== 3. 四角色差 (DiffCorner) ==========
    auto corner_avg = [&](int row_start, int col_start) -> double {
        double sum = 0.0;
        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                sum += RB_GrGb[(row_start + di) * roi_x_cnt + (col_start + dj)];
        return sum / 4.0;
    };

    const double CornerLT = corner_avg(0, 0);
    const double CornerLB = corner_avg(roi_y_cnt - 2, 0);
    const double CornerRT = corner_avg(0, roi_x_cnt - 2);
    const double CornerRB = corner_avg(roi_y_cnt - 2, roi_x_cnt - 2);

    const double diffcorner[6] = {
        std::abs(CornerLT / (CornerLB + EPS) - 1.0) * 100.0,
        std::abs(CornerLT / (CornerRT + EPS) - 1.0) * 100.0,
        std::abs(CornerLT / (CornerRB + EPS) - 1.0) * 100.0,
        std::abs(CornerLB / (CornerRT + EPS) - 1.0) * 100.0,
        std::abs(CornerLB / (CornerRB + EPS) - 1.0) * 100.0,
        std::abs(CornerRT / (CornerRB + EPS) - 1.0) * 100.0
    };
    out_DiffCorner = *std::max_element(diffcorner, diffcorner + 6);

    // ========== 4. 上下/左右色差 (Maxdiff) ==========
    const int safe_x = std::max(2, std::min(set_x_index, roi_x_cnt - 3));
    const int safe_y = std::max(2, std::min(set_y_index, roi_y_cnt - 3));

    double Top = 0.0, Bottom = 0.0;
    for (int d = -2; d <= 2; d++) {
        Top    += RB_GrGb[0 * roi_x_cnt + (safe_x + d)];
        Bottom += RB_GrGb[(roi_y_cnt - 1) * roi_x_cnt + (safe_x + d)];
    }
    Top /= 5.0; Bottom /= 5.0;

    double Left = 0.0, Right = 0.0;
    for (int d = -2; d <= 2; d++) {
        Left  += RB_GrGb[(safe_y + d) * roi_x_cnt + 0];
        Right += RB_GrGb[(safe_y + d) * roi_x_cnt + (roi_x_cnt - 1)];
    }
    Left /= 5.0; Right /= 5.0;

    const double Diff_TB = std::abs(Top / (Bottom + EPS) - 1.0) * 100.0;
    const double Diff_RL = std::abs(Right / (Left + EPS) - 1.0) * 100.0;
    out_Maxdiff = std::max(Diff_TB, Diff_RL);

    return 1;
}


