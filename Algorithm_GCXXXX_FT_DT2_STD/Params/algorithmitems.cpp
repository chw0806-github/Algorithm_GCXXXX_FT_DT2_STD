#include "algorithmitems.h"

/*-------------    内存管理     ---------------*/

//指针置空
bool AlgorithmItems::reset_gbuf_pointer()
{

/***************         结果区指针             **********/
    //单byte 指针
    g_buf_raw_= NULL;
    g_buf_r_ = NULL;
    g_buf_gr_= NULL;
    g_buf_gb_= NULL;
    g_buf_b_ = NULL;

    g_buf_raw_tmp_= NULL;
    g_buf_r_tmp_ = NULL;
    g_buf_gr_tmp_= NULL;
    g_buf_gb_tmp_= NULL;
    g_buf_b_tmp_= NULL;

    //双byte 指针
    g_buf_raw_16_ = NULL;
    g_buf_r_16_ = NULL;
    g_buf_gr_16_ = NULL;
    g_buf_gb_16_ = NULL;
    g_buf_b_16_ = NULL;

    g_buf_raw_16_tmp_ = NULL;
    g_buf_r_16_tmp_ = NULL;
    g_buf_gr_16_tmp_ = NULL;
    g_buf_gb_16_tmp_ = NULL;
    g_buf_b_16_tmp_= NULL;


/***************         计算区指针             **********/
    g_buf_tmp_ = NULL;
    g_binary_r_ = NULL;
    g_binary_gr_ = NULL;
    g_binary_gb_ = NULL;
    g_binary_b_ = NULL;
    g_binary_ = NULL;

    return true;
}

//内存管理初始化
bool AlgorithmItems::init(uchar *data_start,int width_,int height_,
                          int framecnt_,int resultcnt_,int buf_type)
{
    bool res=Pointer_manage.Create(data_start,width_,height_,
                          framecnt_,resultcnt_,buf_type);

    flag_init=res;
    return flag_init;
}

//指针申请空间
bool AlgorithmItems::New_Pointer_Buf(uchar** ptr, Memory_Area Area,Buf_type type,
                                     int width_,int height_,int famer_cnt)
{
    return Pointer_manage.new_buf_(ptr,Area,type, width_,
                                   height_,famer_cnt);
}
//指针释放空间
bool AlgorithmItems::Free_Pointer_Buf(uchar** ptr)
{
    return Pointer_manage.free_buf_(ptr);
}

bool AlgorithmItems::Free_All_Buf()
{
    Pointer_manage.Free_all_buf();
    reset_gbuf_pointer();

    return true;
}


//结果区 申请1份raw8 1份raw16
bool AlgorithmItems::New_result_buf(int w,int h)
{
    bool res_flag=true;
    res_flag = New_Pointer_Buf (&g_buf_raw_,Result_area,
                                Buf_uchar_8,w,h,2);
    if(res_flag)
    {
        g_buf_r_ =   g_buf_raw_ + w*h;
        g_buf_gr_ =  g_buf_raw_ + w*h+ w*h*1/4;
        g_buf_gb_ =  g_buf_raw_ + w*h+ w*h*2/4;
        g_buf_b_ =   g_buf_raw_ + w*h+ w*h*3/4;
    }
    else
        return false;

    res_flag = New_Pointer_Buf (&g_buf_raw_16_,Result_area,Buf_uchar_16,w,h,2);
    if(res_flag)
    {
        g_buf_r_16_ =   g_buf_raw_16_  + w*h * 2;
        g_buf_gr_16_ =  g_buf_raw_16_  + w*h * 2 +  w*h * 1/2;
        g_buf_gb_16_ =  g_buf_raw_16_  + w*h * 2 +  w*h * 2/2;
        g_buf_b_16_ =   g_buf_raw_16_  + w*h * 2 +  w*h * 3/2;
    }
    else
        return false;

    return true;
}
//结果区 2份raw16
bool AlgorithmItems::New_result_buf_two_raw16(int w,int h)
{
    bool res_flag=true;
    res_flag = New_Pointer_Buf (&g_buf_raw_16_,Result_area,Buf_uchar_16,w,h,4);
    if(res_flag)
    {
        g_buf_r_16_ =   g_buf_raw_16_  + w*h * 2;
        g_buf_gr_16_ =  g_buf_raw_16_  + w*h * 2 +  w*h * 1/2;
        g_buf_gb_16_ =  g_buf_raw_16_  + w*h * 2 +  w*h * 2/2;
        g_buf_b_16_ =   g_buf_raw_16_  + w*h * 2 +  w*h * 3/2;

        g_buf_raw_16_tmp_ =  g_buf_raw_16_+  w*h * 4;
        g_buf_r_16_tmp_   =  g_buf_raw_16_+  w*h * 6 ;
        g_buf_gr_16_tmp_  =  g_buf_raw_16_+  w*h * 6 +  w*h * 1/2;
        g_buf_gb_16_tmp_  =  g_buf_raw_16_+  w*h * 6 +  w*h * 2/2;
        g_buf_b_16_tmp_   =  g_buf_raw_16_+  w*h * 6 +  w*h * 3/2;
    }
    else
        return false;

    return true;
}



bool AlgorithmItems::Free_result_buf()
{
    bool flag=true;

    flag=Pointer_manage.Free_result_buf();

    if(flag)
    {
        g_buf_raw_ = NULL;
        g_buf_r_ =  NULL;
        g_buf_gr_ =  NULL;
        g_buf_gb_ =  NULL;
        g_buf_b_ =   NULL;

        g_buf_raw_tmp_=NULL;
        g_buf_r_tmp_ = NULL;
        g_buf_gr_tmp_= NULL;
        g_buf_gb_tmp_= NULL;
        g_buf_b_tmp_ = NULL;


        g_buf_raw_16_ = NULL;
        g_buf_r_16_ = NULL;
        g_buf_gr_16_ = NULL;
        g_buf_gb_16_ = NULL;
        g_buf_b_16_ = NULL;

        g_buf_raw_16_tmp_=NULL;
        g_buf_r_16_tmp_=NULL;
        g_buf_gr_16_tmp_=NULL;
        g_buf_gb_16_tmp_=NULL;
        g_buf_b_16_tmp_=NULL;
    }

    return flag;
}


//查找坏点与prnu所需要 共计5张
bool AlgorithmItems::New_PRNU(int w,int h)
{
    bool flag_newbuf = New_Pointer_Buf (&g_binary_,Alculate_area,
                                   Buf_uchar_8,w,h,2);
    if(flag_newbuf)
    {
        g_binary_r_ =   g_binary_  + w*h;
        g_binary_gr_ =  g_binary_r_+ w*h*1/4;
        g_binary_gb_ =  g_binary_r_+ w*h*2/4;
        g_binary_b_ =   g_binary_r_+ w*h*3/4;
    }
    else
        return false;

    flag_newbuf = New_Pointer_Buf (&g_buf_tmp_,Alculate_area,
                                        Buf_uchar_16,w,h,1);
    if(!flag_newbuf)
        return false;


    Free_Pointer_Buf(&g_buf_raw_16_);
    flag_newbuf = New_Pointer_Buf ( (uchar **) (&g_buf_delta_raw_),
                                        Result_area,Buf_float,w,h,1);
    if(!flag_newbuf)
        return false;

    flag_newbuf = New_Pointer_Buf ( (uchar **) (&g_buf_delta_r_),
                                        Alculate_area,Buf_float,w,h,1);
    if(flag_newbuf)
    {
        g_buf_delta_gr_ =  g_buf_delta_r_  + w*h*1/4;
        g_buf_delta_gb_ =  g_buf_delta_r_  + w*h*2/4;
        g_buf_delta_b_ =   g_buf_delta_r_  + w*h*3/4;
    }
    else
        return false;

    flag_newbuf = New_Pointer_Buf ( &g_buf_blemish_raw_,Alculate_area,
                                         Buf_uchar_8,w,h,2);
    if(flag_newbuf)
    {
        g_buf_blemish_r_ =   g_buf_blemish_raw_+ w*h;
        g_buf_blemish_gr_ =  g_buf_blemish_r_  + w*h*1/4;
        g_buf_blemish_gb_ =  g_buf_blemish_r_  + w*h*2/4;
        g_buf_blemish_b_ =   g_buf_blemish_r_  + w*h*3/4;
    }
    else
        return false;

    return true;
}

//g_binary_ 没有free
bool AlgorithmItems::Free_PRNU()
{
    bool flag_ = Free_Pointer_Buf(&g_buf_blemish_raw_);
    flag_= Free_Pointer_Buf((uchar **) (&g_buf_delta_raw_));
    flag_= Free_Pointer_Buf((uchar **) (&g_buf_delta_r_));

    return flag_;
}


/*-------------    图片格式转换 分通道    ---------------*/

//  将多张 raw16 舍弃无效位，返回 unsigned short 指针
bool AlgorithmItems::Convert_Raw16_to_Ushort(const ParamManage& manage, uchar *pData,
                                              unsigned short *pshort, int frameCnt, int datatype)
{
    if (!flag_init)
        return false;

    int w = manage.width_;
    int h = manage.height_;
    int total = w * h * frameCnt;

    // 根据位深计算掩码
    unsigned short mask;
    switch (datatype)
    {
        case 8:
            mask = 0x00FF;
            g_str_msg_all[site_].append("image data type is raw8\n");
            break;
        case 10:
            mask = 0x03FF;
            g_str_msg_all[site_].append("image data type is raw10\n");
            break;
        case 12:
            mask = 0x0FFF;
            g_str_msg_all[site_].append("image data type is raw12\n");
            break;
        case 14:
            mask = 0x3FFF;
            g_str_msg_all[site_].append("image data type is raw14\n");
            break;
        case 16:
            mask = 0xFFFF;
            g_str_msg_all[site_].append("image data type is raw16\n");
            break;
        default:
            g_str_msg_all[site_].append(
                QString("image data type is wrong! datatype=%1\n").arg(datatype).toStdString());
            return false;
    }

    unsigned short *src = reinterpret_cast<unsigned short *>(pData);

    // 修正：< 而不是 <=
    for (int i = 0; i < total; ++i)
        pshort[i] = src[i] & mask;

    return true;
}

//  将多张 raw10 合并后 转变为 raw16，返回 unsigned short 指针
bool  AlgorithmItems::Convert_Raw10_to_Raw16(const ParamManage& manage,uchar *pData_in,
                                              uchar *pData_out,int frameCnt)
{
    if(!flag_init)
        return false;

    int w = manage.width_;
    int h = manage.height_;

    if(!New_result_buf(w,h))                     //为结果区指针分配空间
        return false;

    g_buf_raw_16_=pData_out;

    if(frameCnt>1)
        AlgorithmFuncs::SeveralFrameToOne (pData_in, g_buf_raw_16_ , w, h ,frameCnt, 10);

    TestItem::split_channel_raw10_to_raw16_multiframe(manage.bayer_, w, h, 1, g_buf_raw_16_,
                                                      g_buf_r_16_, g_buf_gr_16_, g_buf_gb_16_, g_buf_b_16_);

    AlgorithmFuncs::RGGB_Map_Integrate_Raw16 (g_buf_r_16_,g_buf_gr_16_,g_buf_gb_16_,g_buf_b_16_,pData_out,
                                              w,h,manage.bayer_);

    //存储图片
    if(true)
    {
        FILE *fp = fopen("./r_raw16.raw","wb");
        if (fp!=NULL)
            fwrite(g_buf_r_16_,1,w*h/2,fp);
        fclose(fp);

        fp = fopen("./gr_raw16.raw","wb");
        if (fp!=NULL)
            fwrite(g_buf_gr_16_,1,w*h/2,fp);
        fclose(fp);

        fp = fopen("./gb_raw16.raw","wb");
        if (fp!=NULL)
            fwrite(g_buf_gb_16_,1,w*h/2,fp);
        fclose(fp);

        fp = fopen("./b_raw16.raw","wb");
        if (fp!=NULL)
            fwrite(g_buf_b_16_,1,w*h/2,fp);
        fclose(fp);

        fp = fopen("./all_raw16.raw","wb");
        if (fp!=NULL)
            fwrite(g_buf_raw_16_,1,w*h*2,fp);
        fclose(fp);

    }

    Free_result_buf();

    return true;
}


//  分通道   将多张 raw16或raw8图片 合并为 单张raw8图，并分通道
bool  AlgorithmItems::Split_Channel(const ParamManage& manage, Split_Channel_type type,bool OB_flag,
                    uchar *pData_in,int framecnt,int datatype, uchar *pImage_save)
{
    if(!flag_init)
    {
        g_str_msg_all[site_]+="Algorithm init failed!";
        return false;
    }

    int w = manage.width_;
    int h = manage.height_;

    if(OB_flag)
    {
        bool res_flag = New_Pointer_Buf (&g_buf_raw_tmp_,Result_area,
                                    Buf_uchar_8,w,h,2);
        if(res_flag)
        {
            g_buf_r_tmp_ = g_buf_raw_tmp_ + w*h;
            g_buf_gr_tmp_= g_buf_raw_tmp_ + w*h+ w*h*1/4;
            g_buf_gb_tmp_= g_buf_raw_tmp_ + w*h+ w*h*2/4;
            g_buf_b_tmp_ = g_buf_raw_tmp_ + w*h+ w*h*3/4;
        }
        else
        {
            g_str_msg_all[site_]+="Algorithm init failed!";
            return false;
        }
    }

    int flag_raw8_image_return = manage.flag_raw8_image_;
    int flag_save_channel=manage.flag_split_channel_;

    if(!New_result_buf(w,h))                     //为结果区指针分配空间
    {
        g_str_msg_all[site_]+="New_result_buf failed!";
        return false;
    }

/*    if (datatype==8)
    {
        uchar *pMerge = g_buf_raw_;
        AlgorithmFuncs::SeveralFrameToOne (pData_in,pMerge,w,h,framecnt,datatype);
        //----------Split Channel----------------

        if(type==Split_Bayer)
        {
            TestItem::split_channel(manage.bayer_,w,h,g_buf_raw_,g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_);
            if(OB_flag)
                TestItem::split_channel(manage.bayer_,w,h,g_buf_raw_,g_buf_r_tmp_,
                                        g_buf_gr_tmp_,g_buf_gb_tmp_,g_buf_b_tmp_);
        }
        else
        {
            TestItem::split_channel_4C(manage.bayer_,w,h,g_buf_raw_,g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_);
            if(OB_flag)
                TestItem::split_channel_4C(manage.bayer_,w,h,g_buf_raw_,g_buf_r_tmp_,
                                           g_buf_gr_tmp_,g_buf_gb_tmp_,g_buf_b_tmp_);
        }
        g_str_msg_all[site_].append ("image data type is raw8\n");
        if(flag_raw8_image_return)
            memcpy(pImage_save,g_buf_raw_,w*h*sizeof(uchar));
    }
    else */if (datatype == 8 || datatype == 10 || datatype == 12 || datatype == 14 || datatype == 16)
    {
        uchar *pMerge = g_buf_raw_16_;
        AlgorithmFuncs::SeveralFrameToOne (pData_in,pMerge,w,h,framecnt,datatype);
        //----------Split Channel----------------
        if(type==Split_Bayer)
            TestItem::split_channel_raw16_to_raw8(manage.bayer_, w, h, g_buf_raw_16_,
                                              g_buf_r_16_, g_buf_gr_16_, g_buf_gb_16_, g_buf_b_16_,
                                              g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_, g_buf_raw_,datatype);
        else    //_4C 格式的图片,不太清楚
            TestItem::split_channel_raw16_to_raw8_4C(manage.bayer_, w, h, g_buf_raw_16_,
                                                     g_buf_r_16_, g_buf_gr_16_, g_buf_gb_16_, g_buf_b_16_,
                                                     g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_, g_buf_raw_,datatype);

        //仅支持Bayer结构 测OB
        if(OB_flag && type==Split_Bayer)
            TestItem::split_channel_raw16_to_low_raw8(manage.bayer_,w,h,g_buf_raw_16_,
                                                      g_buf_r_tmp_,g_buf_gr_tmp_,g_buf_gb_tmp_,g_buf_b_tmp_,g_buf_raw_tmp_);

        g_str_msg_all[site_].append ("image data type is raw16\n");
        if(flag_raw8_image_return)
            memcpy(pImage_save,g_buf_raw_,w*h*sizeof(uchar));
    }

    int w_s = w/2;
    int h_s = h/2;
    if(flag_save_channel)
    {
        QString image_title="site"+QString::number(site_);
        cv::Mat img_tmp_r = cv::Mat(h_s,w_s,CV_8U);
        for (int i=0;i<h_s;i++)
        {
            uchar *p=img_tmp_r.ptr<uchar>(i);
            for(int j=0;j<w_s;j++)
                p[j]=g_buf_r_[i*w_s+j];
        }
        cv::imwrite(QString("./"+image_title+"_r_split_image.bmp").toStdString().c_str(),img_tmp_r);

        cv::Mat img_tmp_gr = cv::Mat(h_s,w_s,CV_8U);
        for (int i=0;i<h_s;i++)
        {
            uchar *p=img_tmp_gr.ptr<uchar>(i);
            for(int j=0;j<w_s;j++)
                p[j]=g_buf_gr_[i*w_s+j];
        }
        cv::imwrite(QString("./"+image_title+"_gr_split_image.bmp").toStdString().c_str(),img_tmp_gr);

        cv::Mat img_tmp_gb = cv::Mat(h_s,w_s,CV_8U);
        for (int i=0;i<h_s;i++)
        {
            uchar *p=img_tmp_gb.ptr<uchar>(i);
            for(int j=0;j<w_s;j++)
                p[j]=g_buf_gb_[i*w_s+j];
        }
        cv::imwrite(QString("./"+image_title+"_gb_split_image.bmp").toStdString().c_str(),img_tmp_gb);

        cv::Mat img_tmp_b = cv::Mat(h_s,w_s,CV_8U);
        for (int i=0;i<h_s;i++)
        {
            uchar *p=img_tmp_b.ptr<uchar>(i);
            for(int j=0;j<w_s;j++)
                p[j]=g_buf_b_[i*w_s+j];
        }
        cv::imwrite(QString("./"+image_title+"_b_split_image.bmp").toStdString().c_str(),img_tmp_b);
    }

    Pointer_manage.un_locked();
    return true;
}



//NEW
void  AlgorithmItems::GetROIMean_Doublebyte_RGB(unsigned short *pData,
                                        int width, int height,
                                        int col, int line,
                                        int win_w, int win_h,double &meanr_, double &meangr_, double &meangb_, double &meanb_)
{
    int cnt = 0;
    double mean_r = 0;
    double mean_gr = 0;
    double mean_gb = 0;
    double mean_b = 0;
//    if(line+win_h>=height || col+win_w>=width)
//        return -1;
    for (int i=line; i<line+win_h; i=(i+2))
    {
        for (int j=col; j<col+win_w; j=(j+2))
        {
            pData[width*i+j]= pData[width*i+j]&0x03ff;
            mean_r+= pData[width*i+j];

            pData[width*i+j+1]= pData[width*i+j+1]&0x03ff;
            mean_gr+= pData[width*i+j+1];

            pData[width*(i+1)+j]= pData[width*(i+1)+j]&0x03ff;
            mean_gb+= pData[width*(i+1)+j];

            pData[width*(i+1)+j+1]= pData[width*(i+1)+j+1]&0x03ff;
            mean_b+= pData[width*(i+1)+j+1];
            cnt+=1;
        }
    }
    meanr_ = (mean_r/cnt);
     meangr_ = mean_gr/cnt;
     meangb_ = mean_gb/cnt;
    meanb_ = mean_b/cnt;
   // return mean_t/cnt;
}




//  分通道  将两张 raw16分为4通道
bool  AlgorithmItems::Split_Channel_TwoRaw16(const ParamManage& manage,uchar *pData_long_exp,
                                                uchar *pData_short_exp,int frameCnt,bool flag_short_exp,
                                             int raw_bits)
{
    if(!flag_init)
        return false;

    int w = manage.width_;
    int h = manage.height_;

    if(!New_result_buf_two_raw16(w,h))                     //为结果区指针分配空间
        return false;

    if(frameCnt==1)
    {
        g_buf_raw_16_=pData_long_exp;
        g_buf_raw_16_tmp_=pData_short_exp;
    }
    else
    {
        AlgorithmFuncs::SeveralFrameToOne (pData_long_exp,g_buf_raw_16_,w,h,frameCnt,raw_bits);
        if(flag_short_exp)
            AlgorithmFuncs::SeveralFrameToOne (pData_short_exp,g_buf_raw_16_tmp_,w,h,frameCnt,raw_bits);
    }

    TestItem::split_channel_raw16(manage.bayer_,w, h, g_buf_raw_16_,
                              g_buf_r_16_, g_buf_gr_16_, g_buf_gb_16_, g_buf_b_16_,raw_bits);

    if(flag_short_exp)
        TestItem::split_channel_raw16(manage.bayer_,w, h, g_buf_raw_16_tmp_,
                                      g_buf_r_16_tmp_, g_buf_gr_16_tmp_,
                                      g_buf_gb_16_tmp_, g_buf_b_16_tmp_,raw_bits);

    Pointer_manage.un_locked();

    return true;
}

/*-------------    通用功能函数     ---------------*/


// DAG 测试
Subitem_Result  AlgorithmItems::DAG(const ParamManage& manage, uchar* pData)
{
    if(manage.dag_version == "DAG_V1.0")//逐pixel计算卡控
        return DAG_V1_0(manage, pData);
    if(manage.dag_version == "DAG_V1.1")
        return DAG_V1_1(manage, pData);//计算col 均值

    Subitem_Result res("DAG", false);
    res.g_msg = false;
    return res;
}

Subitem_Result  AlgorithmItems::DAG_V1_0(const ParamManage& manage, uchar* pData)
{
    Subitem_Result res("DAG",true);

    if(manage.param_dag.dag_open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("------------------------DAG V1.0-----------------------------\n"));
        time_begin=clock();

        // 将 pData 转换为 short* 类型
        short* ptr = (short*) pData;

        int width = manage.width_;
        int x_start = width - manage.param_dag.roi_col_;

        int TestRow = manage.param_dag.roi_row_;
        int TestCol = manage.param_dag.roi_col_;
        int SpecVal = manage.param_dag.val_;
        int max_val = 0;


        for (int row = 0; row < TestRow; ++row)  // 访问 0 到 3 行
        {
            for (int i = 0; i < TestCol; ++i)
            {
                int val = ptr[(row * width) + x_start + i];  // 每行偏移量
                g_str_msg_all[site_] += boost::str(boost::format("%.4f|") % val);

                if(val>max_val)
                    max_val = val;

                if(val < SpecVal)
                {
                    res.is_passed = false;
                    res.bin=manage.param_dag.bin_dag_;
                    res.grade= manage.param_dag.grade_dag_;
                }
            }
        }
         g_str_log_all[site_] += boost::str(boost::format("%.4f|") % max_val);
         g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }

    return res;
}
Subitem_Result  AlgorithmItems::DAG_V1_1(const ParamManage& manage, uchar* pData)
{
    Subitem_Result res("DAG_V1.1",true);

    if(manage.param_dag.dag_open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("------------------------DAG V1.1-----------------------------\n"));
        time_begin=clock();

        // 将 pData 转换为 short* 类型
        short* ptr = (short*) pData;

        int width = manage.width_;
        int x_start = width - manage.param_dag.roi_col_;

        int TestRow = manage.param_dag.roi_row_;
        int TestCol = manage.param_dag.roi_col_;
        int SpecVal = manage.param_dag.val_;
        int cnt = 0;
        int val = 0;
        int col_val_avg = 0;
        int col_val_avg_max = 0;


        for (int col = x_start; col < x_start+TestCol; ++col)  //
        {
            cnt = 0;
            val = 0;
            col_val_avg = 0;
            for (int row = 0; row < TestRow; ++row)
            {
                val += ptr[(row * width)  + col];  // 每行偏移量
                cnt++;
            }
            col_val_avg = val/cnt;
            if(col_val_avg>col_val_avg_max)
                col_val_avg_max = col_val_avg;
            g_str_msg_all[site_] += boost::str(boost::format("%.4f|") % col_val_avg);
            if(col_val_avg < SpecVal)
            {
                res.is_passed = false;
                res.bin=manage.param_dag.bin_dag_;
                res.grade= manage.param_dag.grade_dag_;
            }
        }
         g_str_log_all[site_] += boost::str(boost::format("%.4f|") % col_val_avg_max);
         g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }

    return res;
}



// 通用全局Linearity   (黑屏、白屏、Large_Gain、Range)
Subitem_Result  AlgorithmItems::Light_Linearity_Ushort(const ParamManage& manage)
{
    if(manage.light_linearity_version == "LightLinearity_V1.0")
        return Light_Linearity_Ushort_V1_0(manage);

    Subitem_Result res("Light_Linearity", false);
    res.g_msg = false;
    return res;

}


Subitem_Result  AlgorithmItems::Light_Linearity_Ushort_V1_0(const ParamManage& manage)
{
    Subitem_Result res("Light_Linearity",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_light_linearity_.linearity_open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("------------------------Light Linearity Test V1.0-----------------------------\n"));
        time_begin=clock();

        double linearity[16] = {0};
        //---------------------Whole Region Light Linearity------------------
        TestItem::test_channel_linearity_col_ushort(w,h,
                                                    manage.param_light_linearity_.roi_x_,
                                                    manage.param_light_linearity_.roi_y_,
                                                    manage.param_light_linearity_.roi_w_,
                                                    manage.param_light_linearity_.roi_h_,
                                                    linearity,
                                                    manage.param_light_linearity_.max_dn,
                                                    g_buf_r_16_,g_buf_gr_16_,g_buf_gb_16_,g_buf_b_16_);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|")
                                         % linearity[0] % linearity[1] % linearity[2]  % linearity[3]
                                         % linearity[4] % linearity[5] % linearity[6] % linearity[7]
                                         % linearity[8] % linearity[9]% linearity[10] % linearity[11]
                                         % linearity[12] % linearity[13]% linearity[14] % linearity[15]);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());
        g_str_msg_all[site_]+=boost::str(boost::format("slope_r:%.2f, intercept_r:%.2f, r_2_r:%.2f max_dn:%.2f\n"
                                                       "slope_gr:%.2f, intercept_gr:%.2f, r_2_gr:%.2f max_dn:%.2f\n"
                                                       "slope_gb:%.2f, intercept_gb:%.2f, r_2_gb:%.2f max_dn:%.2f\n"
                                                       "slope_b:%.2f, intercept_b:%.2f, r_2_b:%.2f max_dn:%.2f \n")
                                         % linearity[0] % linearity[1] % linearity[2]  % linearity[3]
                                         % linearity[4] % linearity[5] % linearity[6] % linearity[7]
                                         % linearity[8] % linearity[9]% linearity[10] % linearity[11]
                                         % linearity[12] % linearity[13]% linearity[14] % linearity[15]);
        if (linearity[2]<manage.param_light_linearity_.linear_spec_ ||
                linearity[6]<manage.param_light_linearity_.linear_spec_ ||
                linearity[10]<manage.param_light_linearity_.linear_spec_ ||
                linearity[14]<manage.param_light_linearity_.linear_spec_)
        {
            res.is_passed=false;
            res.bin=manage.param_light_linearity_.bin_linearity_;
            res.grade=manage.param_light_linearity_.grade_linearity_;
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }


    return res;
}





Subitem_Result  AlgorithmItems::DAG_CycleStd(const ParamManage& manage, uchar* pData,int test_frameCnt)
{
    if(manage.dag_mean_std_version == "DAG_StdMean_V1.0")
        return DAG_CycleStd_V1_0(manage, pData, test_frameCnt);

    Subitem_Result res("DAG_CycleStd", false);
    res.g_msg = false;
    return res;


}


Subitem_Result  AlgorithmItems::DAG_CycleStd_V1_0(const ParamManage& manage, uchar* pData, int test_frameCnt)
{
    Subitem_Result res("DAG_CycleStd",true);

    if(manage.param_dag_std_mean.open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("------------------------DAG_StdMeAN CycleStd V1.0-----------------------------\n"));
        time_begin=clock();

        int w_raw=manage.width_;
        int h_raw=manage.height_;

        double mean_r = 0;
        double mean_gr = 0;
        double mean_gb = 0;
        double mean_b = 0;
        double mean_grgb = 0;
        QVector<double> Mean_lum_;
        for(int i = 0;i < test_frameCnt;i++)
        {
            uchar *buf_save1 = pData+i*w_raw*h_raw*2;
            GetROIMean_Doublebyte_RGB((ushort *)buf_save1,w_raw,h_raw,0,0,w_raw,h_raw,mean_r,mean_gr,mean_gb,mean_b);
            mean_grgb = (mean_gr + mean_gb) / 2;

            Mean_lum_.push_back(mean_grgb);

            QString msg = QString("mean_grgb_%1: %2\n").arg(i).arg(mean_grgb);
            g_str_msg_all[site_] += msg.toStdString();

            g_str_log_all[site_]+=boost::str(boost::format("%.2f|") % mean_grgb );
        }

        // 计算均值
        double sum = 0;
        for (double value : Mean_lum_) {
            sum += value;
        }
        double mean_avg = sum / Mean_lum_.size();

        g_str_msg_all[site_]+=boost::str(boost::format("mean_avg:%.2f\n") % mean_avg);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|") % mean_avg );

        // 计算每个数据点与均值的差的平方
        double varianceSum = 0;
        for (double value : Mean_lum_) {
            varianceSum += std::pow(value - mean_avg, 2);
            g_str_msg_all[site_]+=boost::str(boost::format("value-mean_avg:%.2f\n") % (value - mean_avg));

            g_str_log_all[site_]+=boost::str(boost::format("%.2f|") % (value - mean_avg) );
        }

        // 计算方差
        double variance = varianceSum / Mean_lum_.size();

        // 计算标准差
        double standardDeviation = std::sqrt(variance);

        g_str_msg_all[site_]+=boost::str(boost::format("variance:%.2f, standardDeviation:%.2f\n")
                               % variance % standardDeviation );


        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|") % variance % standardDeviation);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());


        if(standardDeviation > manage.param_dag_std_mean.std_)
        {
            res.is_passed=false;
            res.bin=manage.param_dag_std_mean.bin_dag_mean_;
            res.grade=manage.param_dag_std_mean.grade_dag_mean_;
        }

        g_str_msg_all[site_]+=boost::str(boost::format("SPEC:\n "
                                                       "standardDeviation_:%.4f ,spec_std_:%.4f \n")
                                       %standardDeviation % manage.param_dag_std_mean.std_);

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }

    return res;
}





// 通用中心区域Response   (黑屏、白屏、Large_Gain、Range)
Subitem_Result  AlgorithmItems::Light_Response(const ParamManage& manage)
{
    if(manage.light_response_version == "LightResponse_V1.0")
        return Light_Response_V1_0(manage);

    Subitem_Result res("Light_Response", false);
    res.g_msg = false;
    return res;
}

Subitem_Result  AlgorithmItems::Light_Response_V1_0(const ParamManage& manage)
{

    Subitem_Result res("Light_Response",true);
    int w=manage.width_;
    int h=manage.height_;

    mean_gr_roi = -1;
    mean_gb_roi = -1;
    double mean_r_roi = -1;
    double mean_b_roi = -1;

    if(manage.param_light_response_.response_open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("------------------------ROI Light Response V1.0-----------------------------\n"));

        //---------------------Whole Region Light Response------------------
        TestItem::test_roi_channel_mean( w,h,
                                         manage.param_light_response_.roi_x_,
                                         manage.param_light_response_.roi_y_,
                                         manage.param_light_response_.roi_w_,
                                         manage.param_light_response_.roi_h_,
                                         mean_r_roi,mean_gr_roi,mean_gb_roi,mean_b_roi,
                                         g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        g_str_logtmp[site_]+=boost::str(boost::format("%.2f|%.2f|") % mean_gr_roi % mean_gb_roi );

        g_str_msg_all[site_]+=boost::str(boost::format("mean_r:%.2f, mean_gr:%.2f, mean_gb:%.2f, mean_b:%.2f\n")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);

        g_str_msg_all[site_]+=boost::str(boost::format("SPEC:\n "
                                                       "R:%d ~ %d, GR:%d ~ %d, GB:%d ~ %d, B:%d ~ %d\n")
                                         % manage.param_light_response_.limit_r_min_ % manage.param_light_response_.limit_r_max_ % manage.param_light_response_.limit_gr_min_ % manage.param_light_response_.limit_gr_max_
                                          % manage.param_light_response_.limit_gb_min_ % manage.param_light_response_.limit_gb_max_ % manage.param_light_response_.limit_b_min_ % manage.param_light_response_.limit_b_max_);



        if (mean_r_roi>manage.param_light_response_.limit_r_max_ ||
                mean_r_roi<manage.param_light_response_.limit_r_min_ ||
                mean_gr_roi>manage.param_light_response_.limit_gr_max_ ||
                mean_gr_roi<manage.param_light_response_.limit_gr_min_ ||
                mean_gb_roi>manage.param_light_response_.limit_gb_max_ ||
                mean_gb_roi<manage.param_light_response_.limit_gb_min_ ||
                mean_b_roi>manage.param_light_response_.limit_b_max_ ||
                mean_b_roi<manage.param_light_response_.limit_b_min_)
        {
            res.is_passed=false;

            if (manage.param_light_response_.error_min_open && (mean_r_roi<manage.param_light_response_.error_min ||
                                                                mean_gr_roi<manage.param_light_response_.error_min ||
                                                                mean_gb_roi<manage.param_light_response_.error_min ||
                                                                mean_b_roi<manage.param_light_response_.error_min ))
            {
                res.bin=161;
                res.grade=9;
            }
            else if (manage.param_light_response_.error_max_open && (mean_r_roi>manage.param_light_response_.error_max ||
                                                                     mean_gr_roi>manage.param_light_response_.error_max ||
                                                                     mean_gb_roi>manage.param_light_response_.error_max ||
                                                                     mean_b_roi>manage.param_light_response_.error_max ))
            {
                res.bin=162;
                res.grade=9;
            }
            else
            {
                res.bin=manage.param_light_response_.bin_response_;
                res.grade=manage.param_light_response_.grade_response_;
            }
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }


    return res;
}



Subitem_Result  AlgorithmItems::Ref_Col_Light(const ParamManage& manage)
{

    if(manage.light_ref_version == "lightRef_V1.0")
        return Ref_Col_Light_V1_0(manage);

    Subitem_Result res("lightRef", false);
    res.g_msg = false;
    return res;


}


Subitem_Result  AlgorithmItems::Ref_Col_Light_V1_0(const ParamManage& manage)
{

    Subitem_Result res("Ref_col_light",true);
    int w=manage.width_;
    int h=manage.height_;


    if(manage.param_light_ref_.ref_open_mode_ == 1)
    {

        g_str_msg_all[site_]+=boost::str(boost::format("------------------------Ref Col Light Max Min-----------------------------\n"));
        time_begin=clock();

        double mean_r_max = -1;
        double mean_gr_max = -1;
        double mean_gb_max = -1;
        double mean_b_max = -1;
        double mean_r_min = -1;
        double mean_gr_min = -1;
        double mean_gb_min = -1;
        double mean_b_min = -1;
        int mean_r_max_pos = -1;
        int mean_gr_max_pos = -1;
        int mean_gb_max_pos = -1;
        int mean_b_max_pos = -1;
        int mean_r_min_pos = -1;
        int mean_gr_min_pos = -1;
        int mean_gb_min_pos = -1;
        int mean_b_min_pos = -1;
        double pixel_r_max = -1;
        double pixel_gr_max = -1;
        double pixel_gb_max = -1;
        double pixel_b_max = -1;
        double mean_center_left = -1;
        double mean_center_right = -1;




            //---------------------Whole Region Light Response------------------


            TestItem::test_channel_colmean_max_min(w,h,mean_r_max, mean_gr_max,mean_gb_max,mean_b_max,
                                                        mean_r_min, mean_gr_min,mean_gb_min,mean_b_min,
                                                        mean_r_max_pos,mean_gr_max_pos,mean_gb_max_pos,mean_b_max_pos,
                                                        mean_r_min_pos,mean_gr_min_pos,mean_gb_min_pos,mean_b_min_pos,
                                                   pixel_r_max,pixel_gr_max,pixel_gb_max,pixel_b_max,mean_center_left,mean_center_right,
                                                   g_buf_r_tmp_, g_buf_gr_tmp_,
                                                   g_buf_gb_tmp_,g_buf_b_tmp_);



            double Ref_Split_screen_left_right_dif = 0.00;
            Ref_Split_screen_left_right_dif = mean_center_left - mean_center_right;

            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % mean_r_max % mean_gr_max % mean_gb_max % mean_b_max);
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % mean_r_max_pos % mean_gr_max_pos % mean_gb_max_pos % mean_b_max_pos);
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % mean_r_min % mean_gr_min % mean_gb_min % mean_b_min);
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % mean_r_min_pos % mean_gr_min_pos % mean_gb_min_pos % mean_b_min_pos);
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % pixel_r_max % pixel_gr_max % pixel_gb_max % pixel_b_max);
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|")
                                   % mean_center_left % mean_center_right % Ref_Split_screen_left_right_dif);
            g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

            g_str_msg_all[site_]+=boost::str(boost::format("Ref col light Max Min test"));
            g_str_msg_all[site_]+=boost::str(boost::format("mean_r_max:%.2f ,mean_gr_max:%.2f, mean_gb_max:%.2f, mean_b_max:%.2f\n")
                                   % mean_r_max % mean_gr_max % mean_gb_max % mean_b_max );
            g_str_msg_all[site_]+=boost::str(boost::format("mean_r_max_pos:%d ,mean_gr_max_pos:%d, mean_gb_max_pos:%d, mean_b_max_pos:%d\n")
                                   % mean_r_max_pos % mean_gr_max_pos % mean_gb_max_pos % mean_b_max_pos );
            g_str_msg_all[site_]+=boost::str(boost::format("mean_r_min:%.2f ,mean_gr_min:%.2f, mean_gb_min:%.2f, mean_b_min:%.2f\n")
                                   % mean_r_min % mean_gr_min % mean_gb_min % mean_b_min );
            g_str_msg_all[site_]+=boost::str(boost::format("mean_r_min_pos:%d ,mean_gr_min_pos:%d, mean_gb_min_pos:%d, mean_b_min_pos:%d\n")
                                   % mean_r_min_pos % mean_gr_min_pos % mean_gb_min_pos % mean_b_min_pos );
            g_str_msg_all[site_]+=boost::str(boost::format("pixel_r_max:%.2f ,pixel_gr_max:%.2f, pixel_gb_max:%.2f, pixel_b_max:%.2f\n")
                                    % pixel_r_max % pixel_gr_max % pixel_gb_max % pixel_b_max);
            g_str_msg_all[site_]+=boost::str(boost::format("Split_screen:mean_center_left:%.2f,mean_center_right:%.2f,dif:%.2f\n")
                                    % mean_center_left % mean_center_right % Ref_Split_screen_left_right_dif);



            g_str_msg_all[site_] += boost::str(boost::format(
                "SPEC:\n"
                "mean_r_max= %.4f (SPEC: %.4f ~ %.4f) \n"
                "mean_gr_max= %.4f (SPEC: %.4f ~ %.4f) \n"
                "mean_gb_max= %.4f (SPEC: %.4f ~ %.4f) \n"
                "mean_b_max= %.4f (SPEC: %.4f ~ %.4f) \n"
                "mean_r_min= %.4f (SPEC: %.4f ~ %.4f) \n"
                "mean_gr_min= %.4f (SPEC: %.4f ~ %.4f) \n"
                "mean_gb_min= %.4f (SPEC: %.4f ~ %.4f) \n"
                "mean_b_min= %.4f (SPEC: %.4f ~ %.4f) \n"
                "Ref_Split_screen_left_right_dif= %.4f (SPEC: %.4f) \n")
                % mean_r_max % manage.param_light_ref_.col_ref_MAX_min % manage.param_light_ref_.col_ref_MAX_max
                % mean_gr_max % manage.param_light_ref_.col_ref_MAX_min % manage.param_light_ref_.col_ref_MAX_max
                % mean_gb_max % manage.param_light_ref_.col_ref_MAX_min % manage.param_light_ref_.col_ref_MAX_max
                % mean_b_max % manage.param_light_ref_.col_ref_MAX_min % manage.param_light_ref_.col_ref_MAX_max
                % mean_r_min % manage.param_light_ref_.col_ref_MIN_min % manage.param_light_ref_.col_ref_MIN_max
                % mean_gr_min % manage.param_light_ref_.col_ref_MIN_min % manage.param_light_ref_.col_ref_MIN_max
                % mean_gb_min % manage.param_light_ref_.col_ref_MIN_min % manage.param_light_ref_.col_ref_MIN_max
                % mean_b_min % manage.param_light_ref_.col_ref_MIN_min % manage.param_light_ref_.col_ref_MIN_max
                % Ref_Split_screen_left_right_dif % manage.param_light_ref_.Ref_Split_screen_ROI_dif
            );



            if (mean_r_max>manage.param_light_ref_.col_ref_MAX_max ||
                    mean_r_max<manage.param_light_ref_.col_ref_MAX_min ||
                    mean_gr_max>manage.param_light_ref_.col_ref_MAX_max ||
                    mean_gr_max<manage.param_light_ref_.col_ref_MAX_min ||
                    mean_gb_max>manage.param_light_ref_.col_ref_MAX_max ||
                    mean_gb_max<manage.param_light_ref_.col_ref_MAX_min ||
                    mean_b_max>manage.param_light_ref_.col_ref_MAX_max ||
                    mean_b_max<manage.param_light_ref_.col_ref_MAX_min)
            {
                res.is_passed=false;
                res.bin=manage.param_light_ref_.bin_ref_col;
                res.grade=manage.param_light_ref_.grade_ref_col;
            }
            else  if (mean_r_min>manage.param_light_ref_.col_ref_MIN_max ||
                      mean_r_min<manage.param_light_ref_.col_ref_MIN_min ||
                      mean_gr_min>manage.param_light_ref_.col_ref_MIN_max ||
                      mean_gr_min<manage.param_light_ref_.col_ref_MIN_min ||
                      mean_gb_min>manage.param_light_ref_.col_ref_MIN_max ||
                      mean_gb_min<manage.param_light_ref_.col_ref_MIN_min ||
                      mean_b_min>manage.param_light_ref_.col_ref_MIN_max ||
                      mean_b_min<manage.param_light_ref_.col_ref_MIN_min)
            {
                res.is_passed=false;
                res.bin=manage.param_light_ref_.bin_ref_col;
                res.grade=manage.param_light_ref_.grade_ref_col;
            }
            if(fabs(Ref_Split_screen_left_right_dif)>manage.param_light_ref_.Ref_Split_screen_ROI_dif)
            {
                res.is_passed=false;
                res.bin=manage.param_light_ref_.bin_ref_Split_screen;
                res.grade=manage.param_light_ref_.grade_ref_Split_screen;
            }


        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
        res.g_msg = false;

    return res;
}

//Mosaic  (Large_Gain)
Subitem_Result AlgorithmItems::Mosaic(const ParamManage& manage, int datatype)
{

    time_begin=clock();
    Subitem_Result res("Mosaic",true);
    int w=manage.width_;
    int h=manage.height_;
    if (manage.param_Mosaic_.open_mode_ == 1)
    {


        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Mosaic-----------------------------\n"));

        unsigned short *pshort = new unsigned short[w*h];

        unsigned short *src = (unsigned short *)g_buf_raw_16_;

//        g_str_msg_.append ("image data type is raw16\n");
//        for(int i=0; i<=w*h; ++i)
//            pshort[i]=src[i]&0x03ff;

        if (datatype==10)
        {
            unsigned short *pshort_tmp = pshort;
            g_str_msg_.append ("image data type is raw10\n");

                uchar *p_tmp = g_buf_raw_16_;
                for (int i=0; i<w*h*5/4; ++i)
                {
                    if ((i+1)%5 != 0)
                    {
                        uchar val = p_tmp[i];
                        int grp = (i+1)/5;
                        uchar val_2L = p_tmp[grp*5+4];
                        int offset = (i+1)%5;

                        if (offset == 4)
                            *pshort_tmp++ = (val<<2)|((val_2L&0xc0)>>6);
                        else if (offset == 3)
                            *pshort_tmp++ = (val<<2)|((val_2L&0x30)>>4);
                        else if (offset == 2)
                            *pshort_tmp++ = (val<<2)|((val_2L&0x0c)>>2);
                        else if (offset == 1)
                            *pshort_tmp++ = (val<<2)|(val_2L&0x03);
                    }
                }


        }
        else
        {
            g_str_msg_.append ("image data type is wrong!\n");
            res.is_passed = false;
            res.bin= -2;
            res. grade = 10;

            return res;
        }

        //--------------------计算each column sum----------------

        int  cnt_mosaic_line_diff_2=0;
        int  cnt_mosaic_line_diff_6=0;

        double sum = 0;

        QVector <double>  sum_column;

        for (int i= 0; i < w; i+=1)
        {
            sum=0;

            for (int j=0; j<h; j+=1)
            {
                sum += pshort[w*j+i] ;

            }
            sum_column.append(sum);

        }


        //--------------------计算diff=2的mosaic column cnt----------------
        for (int i = 0; i < w-2; ++i)
        {
            if  (sum_column[i]==sum_column[i+2])

                cnt_mosaic_line_diff_2 +=1;
            //            g_str_msg_+=boost::str(boost::format( "cnt_mosaic_line_diff_2_pos:%d\n" )
            //                                              %i);

        }


        //--------------------计算diff=6的mosaic column cnt----------------
        for(int i = 0; i < w-6; ++i)
        {
            if  (sum_column[i]==sum_column[i+6])

            { cnt_mosaic_line_diff_6 +=1;
                //            g_str_msg_+=boost::str(boost::format( "cnt_mosaic_line_diff_6_pos:%d\n" )
                //                                                  %i);
            }

        }

        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|")
                                         % cnt_mosaic_line_diff_2 % cnt_mosaic_line_diff_6);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        g_str_msg_all[site_]+=boost::str(boost::format("cnt_mosaic_line_diff_2:%d\n"
                                                       "cnt_mosaic_line_diff_6:%d\n" )
                                         % cnt_mosaic_line_diff_2 %cnt_mosaic_line_diff_6);
        g_str_msg_all[site_] += boost::str(boost::format(
            "SPEC:\n"
            "cnt_mosaic_line_diff_2= %.4f (SPEC: %.4f) \n"
            "cnt_mosaic_line_diff_6= %.4f (SPEC: %.4f) \n")
            % cnt_mosaic_line_diff_2 % manage.param_Mosaic_.Mosaic_lines_cnt
            % cnt_mosaic_line_diff_6 % manage.param_Mosaic_.Mosaic_lines_cnt
        );



        //------------------------------------------------------------------------

        if(cnt_mosaic_line_diff_6 >  manage.param_Mosaic_.Mosaic_lines_cnt
                ||  cnt_mosaic_line_diff_2 >  manage.param_Mosaic_.Mosaic_lines_cnt  )
        {


            res.is_passed = false;
            res.bin= manage.param_Mosaic_.bin_;
            res. grade = manage.param_Mosaic_.grade_;
        }

        delete [] pshort;

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";


    }
    else
    {
        res.g_msg = false;
    }

    return res;



}





// Relative Illumination  (黑屏、白屏、Large_Gain、Range)
Subitem_Result AlgorithmItems::Relative_Illumination(const ParamManage& manage)
{
    if(manage.relative_illu_version == "relativeIllumination_V1.0")
        return Relative_Illumination_V1_0(manage);

    Subitem_Result res("Relative_Illumination", false);
    res.g_msg = false;
    return res;
}


Subitem_Result AlgorithmItems::Relative_Illumination_V1_0(const ParamManage& manage)
{
    Subitem_Result res("Relative_Illumination",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_relative_illu_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Relative Illumination V1.0-----------------------------\n"));

        double RelIllum = AlgorithmFuncs::TestRelativeIlumination (
                    g_buf_raw_,w,h,manage.param_relative_illu_.w_win_,
                    manage.param_relative_illu_.h_win_);
        if (RelIllum>manage.param_relative_illu_.limit_max_ratio_ ||
                RelIllum<manage.param_relative_illu_.limit_min_ratio_)
        {
            res.is_passed = false;
            res.bin=manage.param_relative_illu_.bin_;
            res.grade= manage.param_relative_illu_.grade_;
        }
        g_str_msg_all[site_]+=boost::str(boost::format("Relative Illumination:%.2f\n")
                               % RelIllum);
        g_str_log_all[site_]+=boost::str(boost::format("%.2f|")
                               % RelIllum);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        g_str_msg_all[site_]+=boost::str(boost::format("SPEC:\n "
                                                       "ratio:%d ~ %d\n")
                                         % manage.param_relative_illu_.limit_min_ratio_ % manage.param_relative_illu_.limit_max_ratio_);

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }


    return res;
}


//Non_Uniformity_Max  (黑屏、白屏、Large_Gain)
Subitem_Result AlgorithmItems::Non_Uniformity(const ParamManage& manage)
{
    if(manage.non_uniformity_version == "NonUniformity_V1.0")
        return Non_Uniformity_V1_0(manage);

    Subitem_Result res("Non_Uniformity", false);
    res.g_msg = false;
    return res;
}



Subitem_Result AlgorithmItems::Non_Uniformity_V1_0(const ParamManage& manage)
{


    Subitem_Result res("Non_Uniformity",true);

    int w=manage.width_;
    int h=manage.height_;

    if (manage.param_non_uniformity_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Non_Uniformity_Max V1.0-----------------------------\n"));

        double non_uniformity_r = 0;
        double non_uniformity_gr = 0;
        double non_uniformity_gb = 0;
        double non_uniformity_b = 0;

        double non_uniformity_r_max = 0;
        double non_uniformity_gr_max = 0;
        double non_uniformity_gb_max = 0;
        double non_uniformity_b_max = 0;

        int non_uniformity_r_max_pos = 0;
        int non_uniformity_gr_max_pos = 0;
        int non_uniformity_gb_max_pos = 0;
        int non_uniformity_b_max_pos = 0;

        double non_uniformity_mirror_diff_r_max = 0;
        double non_uniformity_mirror_diff_gr_max = 0;
        double non_uniformity_mirror_diff_gb_max = 0;
        double non_uniformity_mirror_diff_b_max = 0;

        int non_uniformity_mirror_diff_r_max_pos = 0;
        int non_uniformity_mirror_diff_gr_max_pos = 0;
        int non_uniformity_mirror_diff_gb_max_pos = 0;
        int non_uniformity_mirror_diff_b_max_pos = 0;

        double non_uniformity_updown_diff_r_max = 0;
        double non_uniformity_updown_diff_gr_max = 0;
        double non_uniformity_updown_diff_gb_max = 0;
        double non_uniformity_updown_diff_b_max = 0;

        int non_uniformity_updown_diff_r_max_pos = 0;
        int non_uniformity_updown_diff_gr_max_pos = 0;
        int non_uniformity_updown_diff_gb_max_pos = 0;
        int non_uniformity_updown_diff_b_max_pos = 0;


        TestItem::test_non_uniformity_bychannel(w,h,manage.param_non_uniformity_.w_cell_,
                                                manage.param_non_uniformity_.h_cell_,
                                                non_uniformity_r, non_uniformity_gr, non_uniformity_gb, non_uniformity_b,
                                                non_uniformity_r_max, non_uniformity_gr_max,
                                                non_uniformity_gb_max, non_uniformity_b_max,
                                                non_uniformity_r_max_pos, non_uniformity_gr_max_pos,
                                                non_uniformity_gb_max_pos, non_uniformity_b_max_pos,
                                                non_uniformity_mirror_diff_r_max, non_uniformity_mirror_diff_gr_max,
                                                non_uniformity_mirror_diff_gb_max, non_uniformity_mirror_diff_b_max,
                                                non_uniformity_mirror_diff_r_max_pos, non_uniformity_mirror_diff_gr_max_pos,
                                                non_uniformity_mirror_diff_gb_max_pos, non_uniformity_mirror_diff_b_max_pos,
                                                non_uniformity_updown_diff_r_max, non_uniformity_updown_diff_gr_max,
                                                non_uniformity_updown_diff_gb_max, non_uniformity_updown_diff_b_max,
                                                non_uniformity_updown_diff_r_max_pos, non_uniformity_updown_diff_gr_max_pos,
                                                non_uniformity_updown_diff_gb_max_pos, non_uniformity_updown_diff_b_max_pos,
                                                g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_,g_str_msg_all[site_],
                                                manage.param_non_uniformity_.log_print_);


        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % non_uniformity_r % non_uniformity_gr % non_uniformity_gb % non_uniformity_b);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|%d|%d|%d|%d|")
                               % non_uniformity_r_max % non_uniformity_gr_max % non_uniformity_gb_max % non_uniformity_b_max
                               % non_uniformity_r_max_pos % non_uniformity_gr_max_pos % non_uniformity_gb_max_pos % non_uniformity_b_max_pos);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|%d|%d|%d|%d|")
                               % non_uniformity_mirror_diff_r_max % non_uniformity_mirror_diff_gr_max
                               % non_uniformity_mirror_diff_gb_max % non_uniformity_mirror_diff_b_max
                               % non_uniformity_mirror_diff_r_max_pos % non_uniformity_mirror_diff_gr_max_pos
                               % non_uniformity_mirror_diff_gb_max_pos % non_uniformity_mirror_diff_b_max_pos);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|%d|%d|%d|%d|")
                               % non_uniformity_updown_diff_r_max % non_uniformity_updown_diff_gr_max
                               % non_uniformity_updown_diff_gb_max % non_uniformity_updown_diff_b_max
                               % non_uniformity_updown_diff_r_max_pos % non_uniformity_updown_diff_gr_max_pos
                               % non_uniformity_updown_diff_gb_max_pos % non_uniformity_updown_diff_b_max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());


        g_str_msg_all[site_]+=boost::str(boost::format("non_uniformity_r:%.2f,non_uniformity_gr:%.2f\n"
                                             "non_uniformity_gb:%.2f,non_uniformity_b:%.2f\n")
                               % non_uniformity_r % non_uniformity_gr % non_uniformity_gb % non_uniformity_b);

        g_str_msg_all[site_]+=boost::str(boost::format("non_uniformity_r_max:%.2f,  non_uniformity_gr_max:%.2f\n"
                                             "non_uniformity_gb_max:%.2f, non_uniformity_b_max:%.2f\n"
                                             "r_max_pos:%d,  gr_max_pos:%d\n"
                                             "gb_max_pos:%d,  b_max_pos:%d\n")
                               % non_uniformity_r_max % non_uniformity_gr_max
                               % non_uniformity_gb_max % non_uniformity_b_max
                               % non_uniformity_r_max_pos % non_uniformity_gr_max_pos
                               % non_uniformity_gb_max_pos % non_uniformity_b_max_pos);

        g_str_msg_all[site_]+=boost::str(boost::format("non_uniformity_mirror_diff_r_max:%.2f,  non_uniformity_mirror_diff_gr_max:%.2f\n"
                                             "non_uniformity_mirror_diff_gb_max:%.2f,  non_uniformity_mirror_diff_b_max:%.2f\n"
                                             "non_uniformity_mirror_diff_r_max_pos:%d,  non_uniformity_mirror_diff_gr_max_pos:%d\n"
                                             "non_uniformity_mirror_diff_gb_max_pos:%d,  non_uniformity_mirror_diff_b_max_pos:%d\n")
                               % non_uniformity_mirror_diff_r_max % non_uniformity_mirror_diff_gr_max
                               % non_uniformity_mirror_diff_gb_max % non_uniformity_mirror_diff_b_max
                               % non_uniformity_mirror_diff_r_max_pos % non_uniformity_mirror_diff_gr_max_pos
                               % non_uniformity_mirror_diff_gb_max_pos % non_uniformity_mirror_diff_b_max_pos);

        g_str_msg_all[site_]+=boost::str(boost::format("non_uniformity_updown_diff_r_max:%.2f,  non_uniformity_updown_diff_gr_max:%.2f\n"
                                             "non_uniformity_updown_diff_gb_max:%.2f,  non_uniformity_updown_diff_b_max:%.2f\n"
                                             "non_uniformity_updown_diff_r_max_pos:%d,  non_uniformity_updown_diff_gr_max_pos:%d\n"
                                             "non_uniformity_updown_diff_gb_max_pos:%d,  non_uniformity_updown_diff_b_max_pos:%d\n")
                               % non_uniformity_updown_diff_r_max % non_uniformity_updown_diff_gr_max
                               % non_uniformity_updown_diff_gb_max % non_uniformity_updown_diff_b_max
                               % non_uniformity_updown_diff_r_max_pos % non_uniformity_updown_diff_gr_max_pos
                               % non_uniformity_updown_diff_gb_max_pos % non_uniformity_updown_diff_b_max_pos);


        //——————————————————————————————————————————————

        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                        "non_uniformity_r=%.4f (SPEC: %.4f)  \n"
                                                         "non_uniformity_gr=%.4f (SPEC: %.4f) \n"
                                                         "non_uniformity_gb=%.4f (SPEC: %.4f) \n"
                                                         "non_uniformity_b=%.4f (SPEC: %.4f)  \n"

                                                         "non_uniformity_r_max=%.4f (MAX: %.4f)  \n"
                                                         "non_uniformity_gr_max=%.4f (MAX: %.4f) \n"
                                                         "non_uniformity_gb_max=%.4f (MAX: %.4f) \n"
                                                         "non_uniformity_b_max=%.4f (MAX: %.4f)  \n"

                                                         "non_uniformity_mirror_diff_r_max=%.4f (MAX: %.4f)  \n"
                                                         "non_uniformity_mirror_diff_gr_max=%.4f (MAX: %.4f) \n"
                                                         "non_uniformity_mirror_diff_gb_max=%.4f (MAX: %.4f) \n"
                                                         "non_uniformity_mirror_diff_b_max=%.4f (MAX: %.4f)  \n"

                                                         "non_uniformity_updown_diff_r_max=%.4f (MAX: %.4f)  \n"
                                                         "non_uniformity_updown_diff_gr_max=%.4f (MAX: %.4f) \n"
                                                         "non_uniformity_updown_diff_gb_max=%.4f (MAX: %.4f) \n"
                                                         "non_uniformity_updown_diff_b_max=%.4f (MAX: %.4f)  \n")
                                           % non_uniformity_r
                                           % manage.param_non_uniformity_.spec_n_uniform_r_
                                           % non_uniformity_gr
                                           % manage.param_non_uniformity_.spec_n_uniform_gr_
                                           % non_uniformity_gb
                                           % manage.param_non_uniformity_.spec_n_uniform_gb_
                                           % non_uniformity_b
                                           % manage.param_non_uniformity_.spec_n_uniform_b_
                                           //
                                           % non_uniformity_r_max
                                           % manage.param_non_uniformity_.max_n_uniform_r_
                                           % non_uniformity_gr_max
                                           % manage.param_non_uniformity_.max_n_uniform_gr_
                                           % non_uniformity_gb_max
                                           % manage.param_non_uniformity_.max_n_uniform_gb_
                                           % non_uniformity_b_max
                                           % manage.param_non_uniformity_.max_n_uniform_b_
                                           //
                                           % non_uniformity_mirror_diff_r_max
                                           % manage.param_non_uniformity_.max_mirror_diff_n_uniform_r_
                                           % non_uniformity_mirror_diff_gr_max
                                           % manage.param_non_uniformity_.max_mirror_diff_n_uniform_gr_
                                           % non_uniformity_mirror_diff_gb_max
                                           % manage.param_non_uniformity_.max_mirror_diff_n_uniform_gb_
                                           % non_uniformity_mirror_diff_b_max
                                           % manage.param_non_uniformity_.max_mirror_diff_n_uniform_b_
                                           //
                                           % non_uniformity_updown_diff_r_max
                                           % manage.param_non_uniformity_.max_updown_diff_n_uniform_r_
                                           % non_uniformity_updown_diff_gr_max
                                           % manage.param_non_uniformity_.max_updown_diff_n_uniform_gr_
                                           % non_uniformity_updown_diff_gb_max
                                           % manage.param_non_uniformity_.max_updown_diff_n_uniform_gb_
                                           % non_uniformity_updown_diff_b_max
                                           % manage.param_non_uniformity_.max_updown_diff_n_uniform_b_
                                           );


        if (non_uniformity_r>manage.param_non_uniformity_.spec_n_uniform_r_ ||
                non_uniformity_gr>manage.param_non_uniformity_.spec_n_uniform_gr_ ||
                non_uniformity_gb>manage.param_non_uniformity_.spec_n_uniform_gb_ ||
                non_uniformity_b>manage.param_non_uniformity_.spec_n_uniform_b_)
        {
            res.is_passed = false;
            res.bin= manage.param_non_uniformity_.bin_mean_;
            res. grade = manage.param_non_uniformity_.grade_mean_;

        }

        if (non_uniformity_r_max>manage.param_non_uniformity_.max_n_uniform_r_ ||
                non_uniformity_gr_max>manage.param_non_uniformity_.max_n_uniform_gr_ ||
                non_uniformity_gb_max>manage.param_non_uniformity_.max_n_uniform_gb_ ||
                non_uniformity_b_max>manage.param_non_uniformity_.max_n_uniform_b_)
        {
           res. is_passed = false;
            if(manage.param_non_uniformity_.grade_max_>res.grade )
            {
                res.bin = manage.param_non_uniformity_.bin_max_;
                res.grade = manage.param_non_uniformity_.grade_max_;
            }
        }

        if (non_uniformity_mirror_diff_r_max>manage.param_non_uniformity_.max_mirror_diff_n_uniform_r_ ||
                non_uniformity_mirror_diff_gr_max>manage.param_non_uniformity_.max_mirror_diff_n_uniform_gr_ ||
                non_uniformity_mirror_diff_gb_max>manage.param_non_uniformity_.max_mirror_diff_n_uniform_gb_ ||
                non_uniformity_mirror_diff_b_max>manage.param_non_uniformity_.max_mirror_diff_n_uniform_b_)
        {
            res.is_passed= false;
            if(manage.param_non_uniformity_.grade_mirror_max_>res.grade )
            {
                res.bin = manage.param_non_uniformity_.bin_mirror_max_;
               res. grade = manage.param_non_uniformity_.grade_mirror_max_;
            }
        }

        if (non_uniformity_updown_diff_r_max>manage.param_non_uniformity_.max_updown_diff_n_uniform_r_ ||
                non_uniformity_updown_diff_gr_max>manage.param_non_uniformity_.max_updown_diff_n_uniform_gr_ ||
                non_uniformity_updown_diff_gb_max>manage.param_non_uniformity_.max_updown_diff_n_uniform_gb_ ||
                non_uniformity_updown_diff_b_max>manage.param_non_uniformity_.max_updown_diff_n_uniform_b_)
        {
            res.is_passed = false;
            if(manage.param_non_uniformity_.grade_updown_max_> res.grade )
            {
                res.bin = manage.param_non_uniformity_.bin_updown_max_;
                res.grade = manage.param_non_uniformity_.grade_updown_max_;
            }
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";


    }
    else
    {
        res.g_msg = false;
    }


    return res;
}





Subitem_Result  AlgorithmItems::ImageDirt(const ParamManage& manage)
{
    if(manage.imageDirt_normal_bright_version == "ImageDirt_V1.0")
        return ImageDirt_V1_0(manage);

    Subitem_Result res("ImageDirt_Test", false);
    res.g_msg = false;
    return res;
}


Subitem_Result  AlgorithmItems::ImageDirt_V1_0(const ParamManage& manage)
{

    Subitem_Result res("ImageDirt_Test",true);
    int w=manage.width_;
    int h=manage.height_;

    if (mean_gr_roi>1 && manage.param_ImageDirt_normal_bright_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------ImageDirt Test V1.0-----------------------------\n"));

        if(mean_gr_roi<=1)
            g_str_log_all[site_]+=boost::str(boost::format("-1|-1|-1|-1|-1|-1|-1|-1|-1|-1|"));
        else
        {
        img_tmp.release();
//        img_tmp = cv::Mat((h-400),(w-400),CV_8U);
//        for (int i=0;i<(h-400);i++)
//        {
//            uchar *p=img_tmp.ptr<uchar>(i);
//            for(int j=0;j<(w-400);j++)
//                p[j]=g_buf_raw_[(i+200)*w+(j+200)];
//        }

        img_tmp = cv::Mat((h-80),(w-40),CV_8U);
        for (int i=0;i<(h-80);i++)
        {
            uchar *p=img_tmp.ptr<uchar>(i);
            for(int j=0;j<(w-40);j++)
                p[j]=g_buf_raw_[(i+40)*w+(j+20)];
        }
//        img_tmp = cv::Mat(h,w,CV_8U);
//        for (int i=0;i<h;i++)
//        {
//            uchar *p=img_tmp.ptr<uchar>(i);
//            for(int j=0;j<w;j++)
//                p[j]=g_buf_raw_[i*w+j];
//        }

            Param_ImageDirtDetect_Bright param_ImageDirt=manage.param_ImageDirt_normal_bright_;

            int miscount=0;
            double max_pog_=0;
            int max_pos_=0;
            double min_pog_=0;
            int min_pos_=0;

            int dark_bar_cnt = 0;
            int GFDA_pog_cnt=0;
            int GFDA_scratch_cnt=0;
            int GFDA_spot_cnt=0;
            int LEDA_scratch_cnt=0;
            int LEDA_spot_cnt=0;
            int LDMA_stain_cnt=0;
            std::string msg_c;

            miscount = AlgorithmFuncs::Bright_ImageDirttDetect(img_tmp, (w-40),(h-80),
                                                               param_ImageDirt.pre_blur_size_,
                                                               param_ImageDirt.img_resize_,
                                                               param_ImageDirt.lsc_ratio_,
                                                               param_ImageDirt.gfda_blur_r1_,
                                                               param_ImageDirt.gfda_blur_r2_,
                                                               param_ImageDirt.gfda_pog_spec_p_,
                                                               param_ImageDirt.gfda_pog_spec_n_,
                                                               max_pog_,max_pos_,
                                                               min_pog_,min_pos_,
                                                               param_ImageDirt.gfda_scratch_spec_p_,
                                                               param_ImageDirt.gfda_spot_spec_p_,
                                                               param_ImageDirt.gfda_spot_spec_n_,
                                                               param_ImageDirt.gfda_spot_limit_size_,
                                                               param_ImageDirt.leda_ratio_n_,
                                                               param_ImageDirt.leda_ratio_p_,
                                                               param_ImageDirt.leda_spot_spec_p_,
                                                               param_ImageDirt.leda_spot_limit_size_,
                                                               param_ImageDirt.ldma_stain_skip_,
                                                               param_ImageDirt.ldma_stain_r_kernel_,
                                                               param_ImageDirt.ldma_stain_r_outer_,
                                                               param_ImageDirt.ldma_stain_spec_,
                                                               param_ImageDirt.save_image_,dark_bar_cnt,
                                                               GFDA_pog_cnt,GFDA_scratch_cnt,GFDA_spot_cnt,
                                                               LEDA_spot_cnt,
                                                               LDMA_stain_cnt,msg_c);


            if( dark_bar_cnt > param_ImageDirt.darkbar_miscount_ )
            {
            res.is_passed = false;
            res.bin = manage.param_ImageDirt_normal_bright_.bin_darkbar_;
            res.grade = manage.param_ImageDirt_normal_bright_.grade_darkbar_;
            g_str_msg_all[site_]+=boost::str(boost::format("find dark_bar!\n"));
            }
            else if( GFDA_pog_cnt > param_ImageDirt.pog_miscount_ )
            {
            res.is_passed = false;
            res.bin = manage.param_ImageDirt_normal_bright_.bin_pog_;
            res.grade = manage.param_ImageDirt_normal_bright_.grade_pog_;
            g_str_msg_all[site_]+=boost::str(boost::format("find pog!\n"));
            }
            else if( (GFDA_scratch_cnt/*+LEDA_scratch_cnt*/) > param_ImageDirt.scratch_miscount_)
            {

            res.is_passed = false;
            res.bin = manage.param_ImageDirt_normal_bright_.bin_scratch_;
            res.grade = manage.param_ImageDirt_normal_bright_.grade_scratch_;
            g_str_msg_all[site_]+=boost::str(boost::format("find scratch line!\n"));
            }
            else if( (GFDA_spot_cnt+LEDA_spot_cnt) > param_ImageDirt.spot_miscount_)
            {
            res.is_passed = false;
            res.bin = manage.param_ImageDirt_normal_bright_.bin_spot_;
            res.grade = manage.param_ImageDirt_normal_bright_.grade_spot_;
            g_str_msg_all[site_]+=boost::str(boost::format("find spot!\n"));

            }
            else if( LDMA_stain_cnt > param_ImageDirt.stain_miscount_ )
            {
            res.is_passed = false;
            res.bin = manage.param_ImageDirt_normal_bright_.bin_stain_;
            res.grade = manage.param_ImageDirt_normal_bright_.grade_stain_;
            g_str_msg_all[site_]+=boost::str(boost::format("find stain!\n"));
            }

            g_str_msg_all[site_]+=msg_c;

            g_str_msg_all[site_]+=boost::str(boost::format("max value in pog process:%.2f, pos: %d \n"
                                             "min value in pog process:%.2f, pos :%d \n")
                               % max_pog_ % max_pos_ % min_pog_ % min_pos_);
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%d|%.2f|%d|")
                               % max_pog_ % max_pos_ % min_pog_ % min_pos_);

            g_str_msg_all[site_]+=boost::str(boost::format("dark_bar_cnt:%d \nGFDA_pog_cnt:%d \nGFDA_scratch_cnt:%d \nGFDA_spot_cnt:%d \n"
                                             "LEDA_spot_cnt:%d \nLDMA_stain_cnt:%d \n ")
                              % dark_bar_cnt % GFDA_pog_cnt % GFDA_scratch_cnt % GFDA_spot_cnt
                               % LEDA_spot_cnt % LDMA_stain_cnt  );
            g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|")
                               %dark_bar_cnt % GFDA_pog_cnt % GFDA_scratch_cnt % GFDA_spot_cnt
                               %  LEDA_spot_cnt % LDMA_stain_cnt );
        }

            g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

            time_end=clock();
            g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";


    }
    else
    {
        res.g_msg = false;
    }


    return res;
}



// POG_Test (黑屏、白屏、Large_Gain、Range)
// img_tmp 影响 Large_Gain 中的Pixel Cluster Test in Smooth Image测试
Subitem_Result  AlgorithmItems::POG_Test(const ParamManage& manage,POG_type type)
{
    if(manage.pog_version == "pog_V1.0")
        return POG_Test_V1_0(manage, type);

    Subitem_Result res("POG_Test", false);
    res.g_msg = false;
    return res;

}


Subitem_Result  AlgorithmItems::POG_Test_V1_0(const ParamManage& manage,POG_type type)
{
    time_begin=clock();

    Subitem_Result res("POG_Test",true);
    int w=manage.width_;
    int h=manage.height_;

    if( ( manage.param_pog_.open_mode_ == 1 && type==POG_Normal ) ||
        (manage.param_pog_normal_bright_.open_mode_ == 1 && type==POG_Bright))
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------POG Test V1.0-----------------------------\n"));

        if(mean_gr_roi<=1)
            g_str_log_all[site_]+=boost::str(boost::format("-1|-1|-1|-1|-1|-1|"));
        else
        {

        img_tmp.release();
        img_tmp = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_buf_raw_[i*w+j];
        }

        double max_pog=0;
        double min_pog=0;
        int max_pos_index[2]={0};
        int min_pos_index[2]={0};

        int miscount=0;
        if(type==POG_Bright)
        {
            miscount =  AlgorithmFuncs::find_pog_normal_bright(img_tmp,
                                                               manage.param_pog_normal_bright_.crop_,
                                                               manage.param_pog_normal_bright_.r1_,
                                                               manage.param_pog_normal_bright_.r2_,
                                                               manage.param_pog_normal_bright_.spec_p_,
                                                               manage.param_pog_normal_bright_.spec_n_,
                                                               manage.param_pog_normal_bright_.scratch_p_,
                                                               manage.param_pog_normal_bright_.scratch_n_,
                                                               manage.param_pog_normal_bright_.resize_w_,
                                                               manage.param_pog_normal_bright_.resize_h_,
                                                               manage.param_pog_normal_bright_.limit_size_,
                                                               manage.param_pog_normal_bright_.lsc_ratio_,
                                                               &max_pog,max_pos_index,
                                                               &min_pog,min_pos_index,
                                                               manage.param_pog_normal_bright_.blemish_skip_,
                                                               manage.param_pog_normal_bright_.r_kernel_,
                                                               manage.param_pog_normal_bright_.r_outer_,
                                                               manage.param_pog_normal_bright_.blemish_spec_,
                                                               manage.param_pog_normal_bright_.save_image_);

            if(miscount > manage.param_pog_normal_bright_.miscount_)
            {
                res.is_passed = false;
                res.bin = manage.param_pog_normal_bright_.bin_pog_;
                res.grade = manage.param_pog_normal_bright_.grade_pog_;

                g_str_msg_all[site_]+=boost::str(boost::format("find pog!\n"));
            }
        }
        else if(type==POG_Normal)
        {
            miscount = AlgorithmFuncs::find_pog(img_tmp,
                                                manage.param_pog_.r1_,
                                                manage.param_pog_.r2_,
                                                manage.param_pog_.resize_w_,
                                                manage.param_pog_.resize_h_,
                                                manage.param_pog_.spec_p_,
                                                manage.param_pog_.spec_n_,
                                                manage.param_pog_.save_image_,
                                                &max_pog,max_pos_index,
                                                &min_pog,min_pos_index,
                                                DN);
            if(miscount > manage.param_pog_.miscount_)
            {
                res.is_passed = false;
                res.bin = manage.param_pog_.bin_pog_;
                res.grade = manage.param_pog_.grade_pog_;

                g_str_msg_all[site_]+=boost::str(boost::format("find pog!\n"));
            }

        }

        if(std::isinf(min_pog) || std::isinf(max_pog))
        {
            g_str_msg_all[site_]+=boost::str(boost::format("max value / min value = INF\n"));
            max_pog = -1;
            min_pog = -1;
        }


        g_str_msg_all[site_]+=boost::str(boost::format("max value in pog process:%.2f, pos_X:%d, pos_Y:%d\n"
                                             "min value in pog process:%.2f, pos_X:%d, pos_Y:%d\n")
                               % max_pog % max_pos_index[1] % max_pos_index[0]
                % min_pog % min_pos_index[1] % min_pos_index[0] );

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%d|%d|%.2f|%d|%d|")
                               % max_pog % max_pos_index[1] % max_pos_index[0]
                % min_pog % min_pos_index[1] % min_pos_index[0] );
        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }


    return res;
}
//白屏 Color_Ratio
vector<Subitem_Result> AlgorithmItems::Color_Ratio( ParamManage& manage)
{
    if(manage.color_ratio_version == "color_ratio_V1.0")
        return Color_Ratio_V1_0(manage);
    else if(manage.color_ratio_version == "color_ratio_V1.1")
        return Color_Ratio_V1_1(manage);

    Subitem_Result res("Color_Ratio", false);
    res.g_msg = false;
    vector<Subitem_Result> result_ratio_test;
    result_ratio_test.push_back(res);
    return result_ratio_test;
}

vector<Subitem_Result> AlgorithmItems::Color_Ratio_V1_0( ParamManage& manage)
{

    time_begin=clock();
    int w=manage.width_;
    int h=manage.height_;

    Subitem_Result result_ratio ("ratio",true) ;
    Subitem_Result result_ratio_MA ("ratio_MA",true) ;
    Subitem_Result result_grid ("ratio_grid",true);
    Subitem_Result result_golden_RG ("ratio_golden_RG",true);
    Subitem_Result result_golden_BG ("ratio_golden_BG",true);
    Param_Color_Ratio  param_color_ratio_=manage.param_color_ratio_;
    result_golden_RG.is_passed = false;
    result_golden_BG.is_passed = false;

    if(manage.param_color_ratio_.ratio_open_mode_ == 1)
    {


        g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- ROI Color Ratio V1_0-----------------------------\n"));

        Color_Ratio_Value  ratio_value;

        double factor_center_c1= param_color_ratio_.factor_center_class1;
        double factor_center_c2= param_color_ratio_.factor_center_class2;

        double factor_corner_c1= param_color_ratio_.factor_corner_class1;
        double factor_corner_c2= param_color_ratio_.factor_corner_class2;

        double factor_c1 =0;
        double factor_c2 =0;
        double factor_golden = param_color_ratio_.factor_golden;



        TestItem::test_roi_colorratio( mean_gr_roi, mean_gb_roi, w, h,
                                       param_color_ratio_.skip,
                                       param_color_ratio_.ROI_X_Cnt,
                                       param_color_ratio_.ROI_Y_Cnt,
                                       g_buf_r_, g_buf_gr_,
                                       g_buf_gb_, g_buf_b_,
                                       ratio_value.roi_rg_ratio,
                                       ratio_value.roi_bg_ratio,
                                       ratio_value.roi_gg_ratio,
                                       ratio_value.GG_ratio,
                                       ratio_value.RG_ratio,
                                       ratio_value.BG_ratio,
                                       ratio_value.G_G_diff,
                                       param_color_ratio_.GG_Ratio_target,
                                       param_color_ratio_.RG_Ratio_target,
                                       param_color_ratio_.BG_Ratio_target);

        double roi_rg_ratio_max = *std::max_element(ratio_value.roi_rg_ratio.begin(),ratio_value.roi_rg_ratio.end());
        double roi_bg_ratio_max = *std::max_element(ratio_value.roi_bg_ratio.begin(),ratio_value.roi_bg_ratio.end());
        double roi_gg_ratio_max = *std::max_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double roi_rg_ratio_min = *std::min_element(ratio_value.roi_rg_ratio.begin(),ratio_value.roi_rg_ratio.end());
        double roi_bg_ratio_min = *std::min_element(ratio_value.roi_bg_ratio.begin(),ratio_value.roi_bg_ratio.end());
        double roi_gg_ratio_min = *std::min_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double roi_rg_ratio_mean = 0;
        double roi_bg_ratio_mean = 0;
        double roi_gg_ratio_mean = 0;
        double roi_rg_ratio_std = AlgorithmFuncs::Get_Double_DatasStdandMean(ratio_value.roi_rg_ratio,roi_rg_ratio_mean);
        double roi_bg_ratio_std = AlgorithmFuncs::Get_Double_DatasStdandMean(ratio_value.roi_bg_ratio,roi_bg_ratio_mean);
        double roi_gg_ratio_std = AlgorithmFuncs::Get_Double_DatasStdandMean(ratio_value.roi_gg_ratio,roi_gg_ratio_mean);


        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|")
                               % roi_rg_ratio_max
                               % roi_bg_ratio_max
                               % roi_gg_ratio_max
                               % roi_rg_ratio_min
                               % roi_bg_ratio_min
                               % roi_gg_ratio_min
                               % roi_rg_ratio_mean
                               % roi_bg_ratio_mean
                               % roi_gg_ratio_mean
                               % roi_rg_ratio_std
                               % roi_bg_ratio_std
                               % roi_gg_ratio_std
                               );





        g_str_msg_all[site_]+=boost::str(boost::format("ROI Color Ratio:\n"
                                                       "roi_rg_ratio_max=%.4f,roi_bg_ratio_max=%.4f,roi_gg_ratio_max=%.4f\n"
                                                       "roi_rg_ratio_min=%.4f,roi_bg_ratio_min=%.4f,roi_gg_ratio_min=%.4f\n"
                                                       "roi_rg_ratio_mean=%.4f,roi_bg_ratio_mean=%.4f,roi_gg_ratio_mean=%.4f\n"
                                                       "roi_rg_ratio_std=%.4f,roi_bg_ratio_std=%.4f,roi_gg_ratio_std=%.4f\n")
                                         % roi_rg_ratio_max
                                         % roi_bg_ratio_max
                                         % roi_gg_ratio_max
                                         % roi_rg_ratio_min
                                         % roi_bg_ratio_min
                                         % roi_gg_ratio_min
                                         % roi_rg_ratio_mean
                                         % roi_bg_ratio_mean
                                         % roi_gg_ratio_mean
                                         % roi_rg_ratio_std
                                         % roi_bg_ratio_std
                                         % roi_gg_ratio_std
                                         );

        for ( QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.GG_ratio.begin();ratio != ratio_value.GG_ratio.end();++ratio)
        {
            if(ratio.key().first == "center")
                factor_c2 = factor_center_c2;
            else
                factor_c2 = factor_corner_c2;


            if ((ratio.value()+param_color_ratio_.GG_Ratio_target_offset.value(ratio.key().first)) >((ratio.key().second ) *(1+factor_c2))
                 || (ratio.value()+param_color_ratio_.GG_Ratio_target_offset.value(ratio.key().first)) < ((ratio.key().second)*(1-factor_c2)) )
            {
                result_ratio.is_passed = false;

                g_str_msg_all[site_] += boost::str(boost::format("GG %s FAIL at ratio.value:%.4f, ratio.offset%.4f, ratio.value_offset%.4f, ratio.spec_max%.4f, spec_min%.4f; factor_c2: %.4f\n")
                                                     % ratio.key().first.toUtf8().constData()
                                                     % ratio.value()
                                                     % param_color_ratio_.GG_Ratio_target_offset.value(ratio.key().first)
                                                     % (ratio.value() + param_color_ratio_.GG_Ratio_target_offset.value(ratio.key().first))
                                                     % (ratio.key().second * (1 + factor_c2))
                                                     % (ratio.key().second * (1 - factor_c2))
                                                     % factor_c2);
            }
        }

        for ( QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.RG_ratio.begin();ratio != ratio_value.RG_ratio.end();++ratio)
        {
            if(ratio.key().first == "center")
                factor_c2 = factor_center_c2;
            else
                factor_c2 = factor_corner_c2;


            if ((ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value(ratio.key().first)) >((ratio.key().second ) *(1+factor_c2))
                 || (ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value(ratio.key().first)) < ((ratio.key().second )*(1-factor_c2)) )
            {
                result_ratio.is_passed = false;
                g_str_msg_all[site_]+=boost::str(boost::format("RG %s FAIL at ratio.value:%.4f, ratio.offset%.4f, ratio.value_offset%.4f, ratio.spec_max%.4f, spec_min%.4f; factor_c2: %.4f\n")
                                                 % ratio.key().first.toUtf8().constData()
                                                 % ratio.value()
                                                 % param_color_ratio_.RG_Ratio_target_offset.value(ratio.key().first)
                                                 % (ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value(ratio.key().first))
                                                 % ((ratio.key().second) *(1+factor_c2))
                                                 % ((ratio.key().second) *(1-factor_c2))
                                                 % factor_c2);
            }

            if(ratio.key().first == "center")
            {
                if ((ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value("center")) <((ratio.key().second)) *(1+factor_golden)
                    && (ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value("center")) >((ratio.key().second))*(1-factor_golden) )
                {
                    result_golden_RG.is_passed = true;
                    g_str_msg_all[site_]+=boost::str(boost::format("RG GOLDEN CENTER at ratio.value:%.4f, ratio.offset%.4f,ratio.value_offset%.4f,ratio.spec_max%.4f, spec_min%.4f; factor_c2: %.4f\n")
                                                     % ratio.value()
                                                     % param_color_ratio_.RG_Ratio_target_offset.value("center")
                                                     % (ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value("center"))
                                                     % ((ratio.key().second) *(1+factor_golden))
                                                     % ((ratio.key().second) *(1-factor_golden))
                                                     % factor_c2);
                }
            }

        }

        for ( QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.BG_ratio.begin();ratio != ratio_value.BG_ratio.end();++ratio)
        {
            if(ratio.key().first == "center")
                factor_c2 = factor_center_c2;
            else
                factor_c2 = factor_corner_c2;

            if ((ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value(ratio.key().first)) >((ratio.key().second ) *(1+factor_c2))
                 || (ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value(ratio.key().first)) < ((ratio.key().second )*(1-factor_c2)) )
            {
                result_ratio.is_passed = false;
                g_str_msg_all[site_]+=boost::str(boost::format("BG %s FAIL at ratio.value:%.4f, ratio.offset%.4f, ratio.value_offset%.4f, ratio.spec_max%.4f, spec_min%.4f; factor_c2: %.4f\n")
                                                 % ratio.key().first.toUtf8().constData()
                                                 % ratio.value()
                                                 % param_color_ratio_.BG_Ratio_target_offset.value(ratio.key().first)
                                                 % (ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value(ratio.key().first))
                                                 % ((ratio.key().second) *(1+factor_c2))
                                                 % ((ratio.key().second) *(1-factor_c2))
                                                 % factor_c2);
            }

            //————————————————————

            if(ratio.key().first == "center")
            {
                if ((ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value("center")) <((ratio.key().second)) *(1+factor_golden)
                    && (ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value("center")) >((ratio.key().second))*(1-factor_golden) )
                {
                    result_golden_BG.is_passed = true;
                    g_str_msg_all[site_]+=boost::str(boost::format("BG GOLDEN CENTER at ratio.value:%.4f, ratio.offset%.4f, ratio.value_offset%.4f, ratio.spec_max%.4f, spec_min%.4f; factor_c2: %.4f\n")
                                                     % ratio.value()
                                                     % param_color_ratio_.BG_Ratio_target_offset.value("center")
                                                     % (ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value("center"))
                                                     % ((ratio.key().second) *(1+factor_golden))
                                                     % ((ratio.key().second) *(1-factor_golden))
                                                     % factor_c2);
                }
            }

        }


        if (result_ratio.is_passed)
        {
            for ( QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.GG_ratio.begin();ratio != ratio_value.GG_ratio.end();++ratio)
            {
                if(ratio.key().first == "center")
                    factor_c1 = factor_center_c1;
                else
                    factor_c1 = factor_corner_c1;

                if ((ratio.value()+param_color_ratio_.GG_Ratio_target_offset.value(ratio.key().first)) >((ratio.key().second ) *(1+factor_c1))
                     || (ratio.value()+param_color_ratio_.GG_Ratio_target_offset.value(ratio.key().first)) < ((ratio.key().second )*(1-factor_c1)) )
                {
                    result_ratio_MA.is_passed = false;
                    g_str_msg_all[site_]+=boost::str(boost::format("GG %s FAIL at ratio.value:%.4f, ratio.offset%.4f, ratio.value_offset%.4f, ratio.spec_max%.4f, spec_min%.4f; factor_c1: %.4f\n")
                                                     % ratio.key().first.toUtf8().constData()
                                                     % ratio.value()
                                                     % param_color_ratio_.GG_Ratio_target_offset.value(ratio.key().first)
                                                     % (ratio.value()+param_color_ratio_.GG_Ratio_target_offset.value(ratio.key().first))
                                                     % ((ratio.key().second) *(1+factor_c1))
                                                     % ((ratio.key().second) *(1-factor_c1))
                                                     % factor_c1);
                }



            }

            for ( QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.RG_ratio.begin();ratio != ratio_value.RG_ratio.end();++ratio)
            {
                if(ratio.key().first == "center")
                    factor_c1 = factor_center_c1;
                else
                    factor_c1 = factor_corner_c1;


                if ((ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value(ratio.key().first)) >((ratio.key().second) *(1+factor_c1))
                     || (ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value(ratio.key().first)) < ((ratio.key().second )*(1-factor_c1)) )
                {
                    result_ratio_MA.is_passed = false;
                    g_str_msg_all[site_]+=boost::str(boost::format("RG %s FAIL at ratio.value:%.4f, ratio.offset%.4f, ratio.value_offset%.4f, ratio.spec_max%.4f, spec_min%.4f; factor_c1: %.4f\n")
                                                     % ratio.key().first.toUtf8().constData()
                                                     % ratio.value()
                                                     % param_color_ratio_.RG_Ratio_target_offset.value(ratio.key().first)
                                                     % (ratio.value()+param_color_ratio_.RG_Ratio_target_offset.value(ratio.key().first))
                                                     % ((ratio.key().second) *(1+factor_c1))
                                                     % ((ratio.key().second) *(1-factor_c1))
                                                     % factor_c1);
                }



            }

            for ( QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.BG_ratio.begin();ratio != ratio_value.BG_ratio.end();++ratio)
            {
                if(ratio.key().first == "center")
                    factor_c1 = factor_center_c1;
                else
                    factor_c1 = factor_corner_c1;

                if ((ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value(ratio.key().first)) >((ratio.key().second ) *(1+factor_c1))
                     || (ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value(ratio.key().first)) < ((ratio.key().second )*(1-factor_c1)) )
                {
                    result_ratio_MA.is_passed = false;
                    g_str_msg_all[site_]+=boost::str(boost::format("BG %s at ratio.value:%.4f, ratio.offset%.4f, ratio.value_offset%.4f, ratio.spec_max%.4f, spec_min%.4f; factor_c1: %.4f\n")
                                                     % ratio.key().first.toUtf8().constData()
                                                     % ratio.value()
                                                     % param_color_ratio_.BG_Ratio_target_offset.value(ratio.key().first)
                                                     % (ratio.value()+param_color_ratio_.BG_Ratio_target_offset.value(ratio.key().first))
                                                     % ((ratio.key().second) *(1+factor_c1))
                                                     % ((ratio.key().second) *(1-factor_c1))
                                                     % factor_c1);
                }



            }

        }




        //----------Center Region Gr/Gb Ratio -------------------
        double gr_gb_target = param_color_ratio_.GG_Ratio_target.value("center");
        double gr_gb_ratio_value = ratio_value.GG_ratio.value(QPair<QString,double>("center",param_color_ratio_.GG_Ratio_target.value("center")));
        double gr_gb_ratio_spec = param_color_ratio_.factor_center_gr_gb_ratio;


        if ((gr_gb_ratio_value+param_color_ratio_.GG_Ratio_target_offset.value("center")) > (gr_gb_target*(1+gr_gb_ratio_spec))
                || (gr_gb_ratio_value+param_color_ratio_.GG_Ratio_target_offset.value("center")) < (gr_gb_target*(1-gr_gb_ratio_spec)))

        {
            result_ratio.is_passed = false;
            g_str_msg_all[site_]+=boost::str(boost::format("FAIL at Center Gr/Gb ratio:gr_gb_ratio_value1%.4f,"
                                                           "gr_gb_ratio_value%.4f,"
                                                           "gr_gb_ratio_max%.4f,"
                                                           "gr_gb_ratio_min%.4f,"
                                                           "gr_gb_ratio_spec: %.4f\n")
                                             % gr_gb_ratio_value
                                             % (gr_gb_ratio_value+param_color_ratio_.GG_Ratio_target_offset.value("center"))
                                             % (gr_gb_target*(1+gr_gb_ratio_spec))
                                             % (gr_gb_target*(1-gr_gb_ratio_spec))
                                             % gr_gb_ratio_spec);
        }


        for (QMap <QString,double>::Iterator diff = ratio_value.G_G_diff.begin();
             diff != ratio_value.G_G_diff.end();++diff)
        {
            if (diff.value() > param_color_ratio_.gr_gb_diff_spec)
            {
                result_grid.is_passed = false;
            }
        }

        //可优化可读性
        ratio_value.gg_leftup = ratio_value.GG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.GG_Ratio_target.value("leftup")));
        ratio_value.rg_leftup = ratio_value.RG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.RG_Ratio_target.value("leftup")));
        ratio_value.bg_leftup = ratio_value.BG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.BG_Ratio_target.value("leftup")));

        ratio_value.gg_rightup = ratio_value.GG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.GG_Ratio_target.value("rightup")));
        ratio_value.rg_rightup = ratio_value.RG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.RG_Ratio_target.value("rightup")));
        ratio_value.bg_rightup = ratio_value.BG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.BG_Ratio_target.value("rightup")));

        ratio_value.gg_leftdown = ratio_value.GG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.GG_Ratio_target.value("leftdown")));
        ratio_value.rg_leftdown = ratio_value.RG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.RG_Ratio_target.value("leftdown")));
        ratio_value.bg_leftdown = ratio_value.BG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.BG_Ratio_target.value("leftdown")));

        ratio_value.gg_rightdown = ratio_value.GG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.GG_Ratio_target.value("rightdown")));
        ratio_value.rg_rightdown = ratio_value.RG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.RG_Ratio_target.value("rightdown")));
        ratio_value.bg_rightdown = ratio_value.BG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.BG_Ratio_target.value("rightdown")));

        ratio_value.gg_center = ratio_value.GG_ratio.value(QPair<QString,double>("center",param_color_ratio_.GG_Ratio_target.value("center")));
        ratio_value.rg_center = ratio_value.RG_ratio.value(QPair<QString,double>("center",param_color_ratio_.RG_Ratio_target.value("center")));
        ratio_value.bg_center = ratio_value.BG_ratio.value(QPair<QString,double>("center",param_color_ratio_.BG_Ratio_target.value("center")));



        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|")
                               % ratio_value.gg_leftup
                               % ratio_value.rg_leftup
                               % ratio_value.bg_leftup
                               % ratio_value.G_G_diff.value("leftup")
                               % ratio_value.gg_rightup
                               % ratio_value.rg_rightup
                               % ratio_value.bg_rightup
                               % ratio_value.G_G_diff.value("rightup")
                               % ratio_value.gg_leftdown
                               % ratio_value.rg_leftdown
                               % ratio_value.bg_leftdown
                               % ratio_value.G_G_diff.value("leftdown")
                               % ratio_value.gg_rightdown
                               % ratio_value.rg_rightdown
                               % ratio_value.bg_rightdown
                               % ratio_value.G_G_diff.value("rightdown")
                               % ratio_value.gg_center
                               % ratio_value.rg_center
                               % ratio_value.bg_center
                               % ratio_value.G_G_diff.value("center")
                               );
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_ratio.name.c_str());



        g_str_logtmp[site_]+=boost::str(boost::format("%.4f|%.4f|") % ratio_value.rg_center % ratio_value.bg_center );

        g_str_msg_all[site_]+=boost::str(boost::format("color ratio:\n"
                                                       "ratio_g_g_left_up=%.4f,ratio_r_g_left_up=%.4f,ratio_b_g_left_up=%.4f,gr_gb_diff_left_up=%.4f\n"
                                                       "ratio_g_g_right_up=%.4f,ratio_r_g_right_up=%.4f,ratio_b_g_right_up=%.4f,gr_gb_diff_right_up=%.4f\n"
                                                       "ratio_g_g_left_down=%.4f,ratio_r_g_left_down=%.4f,ratio_b_g_left_down=%.4f,gr_gb_diff_left_down=%.4f\n"
                                                       "ratio_g_g_right_down=%.4f,ratio_r_g_right_down=%.4f,ratio_b_g_right_down=%.4f,gr_gb_diff_right_down=%.4f\n"
                                                       "ratio_g_g_center1=%.4f,ratio_r_g_center1=%.4f,ratio_b_g_center1=%.4f,gr_gb_diff_center1=%.4f\n"
                                                       "ratio_g_g_center_offset=%.4f,ratio_r_g_center_offset=%.4f,ratio_b_g_center_offset=%.4f\n"
                                                       "ratio_g_g_center=%.4f,ratio_r_g_center=%.4f,ratio_b_g_center=%.4f\n"
                                                       )
                                         % ratio_value.gg_leftup
                                         % ratio_value.rg_leftup
                                         % ratio_value.bg_leftup
                                         % ratio_value.G_G_diff.value("leftup")
                                         % ratio_value.gg_rightup
                                         % ratio_value.rg_rightup
                                         % ratio_value.bg_rightup
                                         % ratio_value.G_G_diff.value("rightup")
                                         % ratio_value.gg_leftdown
                                         % ratio_value.rg_leftdown
                                         % ratio_value.bg_leftdown
                                         % ratio_value.G_G_diff.value("leftdown")
                                         % ratio_value.gg_rightdown
                                         % ratio_value.rg_rightdown
                                         % ratio_value.bg_rightdown
                                         % ratio_value.G_G_diff.value("rightdown")
                                         % ratio_value.gg_center
                                         % ratio_value.rg_center
                                         % ratio_value.bg_center
                                         % ratio_value.G_G_diff.value("center")
                                         % param_color_ratio_.GG_Ratio_target_offset.value("center")
                                         % param_color_ratio_.RG_Ratio_target_offset.value("center")
                                         % param_color_ratio_.BG_Ratio_target_offset.value("center")

                                         % (ratio_value.gg_center+param_color_ratio_.GG_Ratio_target_offset.value("center"))
                                         % (ratio_value.rg_center+param_color_ratio_.RG_Ratio_target_offset.value("center"))
                                         % (ratio_value.bg_center+param_color_ratio_.BG_Ratio_target_offset.value("center"))

                                         );


        //——————————————————————————————————————————————————————————————————————————————


        g_str_msg_all[site_] += boost::str(boost::format(
            "SPEC:\n"
            "ratio_g_g_left_up ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_g_g_right_up ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_g_g_left_down ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_g_g_right_down ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_g_g_center ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"

            "ratio_r_g_left_up ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_r_g_right_up ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_r_g_left_down ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_r_g_right_down ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_r_g_center ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"

            "ratio_b_g_left_up ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_b_g_right_up ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_b_g_left_down ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_b_g_right_down ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"
            "ratio_b_g_center ratio.value:%.4f, ratio.offset:%.4f, ratio.value_offset:%.4f, ratio.spec_min:%.4f, spec_max:%.4f; factor_c2: %.4f\n"

            "Center Gr/Gb ratio:gr_gb_ratio_value:%.4f," "offset:%.4f," "value_offset:%.4f," "gr_gb_ratio_min:%.4f," "gr_gb_ratio_max:%.4f," "gr_gb_ratio_spec: %.4f\n"
            "G_G_diff_leftup:%.4f," "G_G_diff_rightup:%.4f," "G_G_diff_leftdown:%.4f," "G_G_diff_rightdown:%.4f," "G_G_diff_center:%.4f," "gr_gb_diff_spec: %.4f\n"
            )
            % ratio_value.gg_leftup % param_color_ratio_.GG_Ratio_target_offset.value("leftup") % (ratio_value.gg_leftup + param_color_ratio_.RG_Ratio_target_offset.value("leftup")) % (ratio_value.gg_leftup * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.gg_leftup * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.gg_rightup % param_color_ratio_.GG_Ratio_target_offset.value("rightup") % (ratio_value.gg_rightup + param_color_ratio_.RG_Ratio_target_offset.value("rightup")) % (ratio_value.gg_rightup * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.gg_rightup * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.gg_leftdown % param_color_ratio_.GG_Ratio_target_offset.value("leftdown") % (ratio_value.gg_leftdown + param_color_ratio_.RG_Ratio_target_offset.value("leftdown")) % (ratio_value.gg_leftdown * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.gg_leftdown * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.gg_rightdown % param_color_ratio_.GG_Ratio_target_offset.value("rightdown") % (ratio_value.gg_rightdown + param_color_ratio_.RG_Ratio_target_offset.value("rightdown")) % (ratio_value.gg_rightdown * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.gg_rightdown * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.gg_center % param_color_ratio_.GG_Ratio_target_offset.value("center") % (ratio_value.gg_center + param_color_ratio_.RG_Ratio_target_offset.value("center")) % (ratio_value.gg_center * (1 - param_color_ratio_.factor_center_class2)) % (ratio_value.gg_center * (1 + param_color_ratio_.factor_center_class2)) % param_color_ratio_.factor_center_class2

            % ratio_value.bg_leftup % param_color_ratio_.RG_Ratio_target_offset.value("leftup") % (ratio_value.rg_leftup + param_color_ratio_.RG_Ratio_target_offset.value("leftup")) % (ratio_value.rg_leftup * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.rg_leftup * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.bg_rightup % param_color_ratio_.RG_Ratio_target_offset.value("rightup") % (ratio_value.rg_rightup + param_color_ratio_.RG_Ratio_target_offset.value("rightup")) % (ratio_value.rg_rightup * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.rg_rightup * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.bg_leftdown % param_color_ratio_.RG_Ratio_target_offset.value("leftdown") % (ratio_value.rg_leftdown + param_color_ratio_.RG_Ratio_target_offset.value("leftdown")) % (ratio_value.rg_leftdown * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.rg_leftdown * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.bg_rightdown % param_color_ratio_.RG_Ratio_target_offset.value("rightdown") % (ratio_value.rg_rightdown + param_color_ratio_.RG_Ratio_target_offset.value("rightdown")) % (ratio_value.rg_rightdown * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.rg_rightdown * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.bg_center % param_color_ratio_.RG_Ratio_target_offset.value("center") % (ratio_value.rg_center + param_color_ratio_.RG_Ratio_target_offset.value("center")) % (ratio_value.rg_center * (1 - param_color_ratio_.factor_center_class2)) % (ratio_value.rg_center * (1 + param_color_ratio_.factor_center_class2)) % param_color_ratio_.factor_center_class2

            % ratio_value.bg_leftup % param_color_ratio_.BG_Ratio_target_offset.value("leftup") % (ratio_value.bg_leftup + param_color_ratio_.BG_Ratio_target_offset.value("leftup")) % (ratio_value.bg_leftup * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.bg_leftup * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.bg_rightup % param_color_ratio_.BG_Ratio_target_offset.value("rightup") % (ratio_value.bg_rightup + param_color_ratio_.BG_Ratio_target_offset.value("rightup")) % (ratio_value.bg_rightup * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.bg_rightup * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.bg_leftdown % param_color_ratio_.BG_Ratio_target_offset.value("leftdown") % (ratio_value.bg_leftdown + param_color_ratio_.BG_Ratio_target_offset.value("leftdown")) % (ratio_value.bg_leftdown * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.bg_leftdown * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.bg_rightdown % param_color_ratio_.BG_Ratio_target_offset.value("rightdown") % (ratio_value.bg_rightdown + param_color_ratio_.BG_Ratio_target_offset.value("rightdown")) % (ratio_value.bg_rightdown * (1 - param_color_ratio_.factor_corner_class2)) % (ratio_value.bg_rightdown * (1 + param_color_ratio_.factor_corner_class2)) % param_color_ratio_.factor_corner_class2
            % ratio_value.bg_center % param_color_ratio_.BG_Ratio_target_offset.value("center") % (ratio_value.bg_center + param_color_ratio_.BG_Ratio_target_offset.value("center")) % (ratio_value.bg_center * (1 - param_color_ratio_.factor_center_class2)) % (ratio_value.bg_center * (1 + param_color_ratio_.factor_center_class2)) % param_color_ratio_.factor_center_class2

            % gr_gb_ratio_value % param_color_ratio_.GG_Ratio_target_offset.value("center") % (gr_gb_ratio_value+param_color_ratio_.GG_Ratio_target_offset.value("center")) % (gr_gb_target*(1-gr_gb_ratio_spec)) % (gr_gb_target*(1+gr_gb_ratio_spec)) % gr_gb_ratio_spec
            % ratio_value.G_G_diff.value("leftup") % ratio_value.G_G_diff.value("rightup") % ratio_value.G_G_diff.value("leftdown") % ratio_value.G_G_diff.value("rightdown") % ratio_value.G_G_diff.value("center") % param_color_ratio_.gr_gb_diff_spec

            );

       time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";


    }
    else
    {
        result_ratio.g_msg = false;
        result_ratio_MA.g_msg = false;
        result_grid.g_msg = false;
        result_golden_RG.g_msg = false;
    }

    if(!result_ratio.is_passed)
    {
        result_ratio.bin=param_color_ratio_.bin_color_ratio_;
        result_ratio.grade = param_color_ratio_.grade_color_ratio_;
    }
    if(!result_ratio_MA.is_passed)
    {
        result_ratio_MA.bin=param_color_ratio_.bin_color_ratio_MA_;
        result_ratio_MA.grade=param_color_ratio_.grade_color_ratio_MA_;
    }
    if(!result_grid.is_passed)
    {
        result_grid.bin=param_color_ratio_.bin_GI_;
        result_grid.grade=param_color_ratio_.grade_GI_;
    }
    if(result_golden_RG.is_passed && result_golden_BG.is_passed)
    {
        result_golden_RG.is_passed=false;
        result_golden_RG.bin=param_color_ratio_.bin_color_ratio_golden_;
        result_golden_RG.grade=param_color_ratio_.grade_color_ratio_golden_;
    }

    vector<Subitem_Result> result_ratio_test;
    result_ratio_test.push_back(result_ratio);
    result_ratio_test.push_back(result_ratio_MA);
    result_ratio_test.push_back(result_grid);
    result_ratio_test.push_back(result_golden_RG);

    return result_ratio_test;
}

vector<Subitem_Result> AlgorithmItems::Color_Ratio_V1_1( ParamManage& manage)
{
    time_begin=clock();
    int w=manage.width_;
    int h=manage.height_;

    Subitem_Result result_ratio ("ratio_V1.1",true) ;
    Subitem_Result result_ratio_MA ("ratio_MA_V1.1",true) ;
    Subitem_Result result_grid ("ratio_grid_V1.1",true);
    Param_Color_Ratio  param_color_ratio_=manage.param_color_ratio_;

    if(manage.param_color_ratio_.ratio_open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- ROI Color Ratio V1_1-----------------------------\n"));

        Color_Ratio_Value  ratio_value;

        double factor_center_c1= param_color_ratio_.factor_center_class1;
        double factor_center_c2= param_color_ratio_.factor_center_class2;

        double factor_corner_c1= param_color_ratio_.factor_corner_class1;
        double factor_corner_c2= param_color_ratio_.factor_corner_class2;

        double GG_Ratio_offset= param_color_ratio_.GG_Ratio_target["offset"];
        double BG_Ratio_offset= param_color_ratio_.BG_Ratio_target["offset"];
        double RG_Ratio_offset= param_color_ratio_.RG_Ratio_target["offset"];

        double GG_Ratio_rate= param_color_ratio_.GG_Ratio_target["rate"];
        double BG_Ratio_rate= param_color_ratio_.BG_Ratio_target["rate"];
        double RG_Ratio_rate= param_color_ratio_.RG_Ratio_target["rate"];

        double factor_c1 =0;
        double factor_c2 =0;

        g_str_msg_all[site_]+=boost::str(boost::format("rate GG:%.4f, BG:%.4f, RG:%.4f\n")
                              % (GG_Ratio_rate)  % (BG_Ratio_rate) % (RG_Ratio_rate));



        TestItem::test_roi_colorratio( mean_gr_roi, mean_gb_roi, w, h,
                                       param_color_ratio_.skip,
                                       param_color_ratio_.ROI_X_Cnt,
                                       param_color_ratio_.ROI_Y_Cnt,
                                       g_buf_r_, g_buf_gr_,
                                       g_buf_gb_, g_buf_b_,
                                       ratio_value.roi_rg_ratio,
                                       ratio_value.roi_bg_ratio,
                                       ratio_value.roi_gg_ratio,
                                       ratio_value.GG_ratio,
                                       ratio_value.RG_ratio,
                                       ratio_value.BG_ratio,
                                       ratio_value.G_G_diff,
                                       param_color_ratio_.GG_Ratio_target,
                                       param_color_ratio_.RG_Ratio_target,
                                       param_color_ratio_.BG_Ratio_target);

        double roi_rg_ratio_max = *std::max_element(ratio_value.roi_rg_ratio.begin(),ratio_value.roi_rg_ratio.end());
        double roi_bg_ratio_max = *std::max_element(ratio_value.roi_bg_ratio.begin(),ratio_value.roi_bg_ratio.end());
        double roi_gg_ratio_max = *std::max_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double roi_rg_ratio_min = *std::min_element(ratio_value.roi_rg_ratio.begin(),ratio_value.roi_rg_ratio.end());
        double roi_bg_ratio_min = *std::min_element(ratio_value.roi_bg_ratio.begin(),ratio_value.roi_bg_ratio.end());
        double roi_gg_ratio_min = *std::min_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double roi_rg_ratio_mean = 0;
        double roi_bg_ratio_mean = 0;
        double roi_gg_ratio_mean = 0;
        double roi_rg_ratio_std = AlgorithmFuncs::Get_Double_DatasStdandMean(ratio_value.roi_rg_ratio,roi_rg_ratio_mean);
        double roi_bg_ratio_std = AlgorithmFuncs::Get_Double_DatasStdandMean(ratio_value.roi_bg_ratio,roi_bg_ratio_mean);
        double roi_gg_ratio_std = AlgorithmFuncs::Get_Double_DatasStdandMean(ratio_value.roi_gg_ratio,roi_gg_ratio_mean);


        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|")
                               % roi_rg_ratio_max
                               % roi_bg_ratio_max
                               % roi_gg_ratio_max
                               % roi_rg_ratio_min
                               % roi_bg_ratio_min
                               % roi_gg_ratio_min
                               % roi_rg_ratio_mean
                               % roi_bg_ratio_mean
                               % roi_gg_ratio_mean
                               % roi_rg_ratio_std
                               % roi_bg_ratio_std
                               % roi_gg_ratio_std
                               );

        g_str_msg_all[site_]+=boost::str(boost::format("ROI Color Ratio:\n"
                                             "roi_rg_ratio_max=%.4f,roi_bg_ratio_max=%.4f,roi_gg_ratio_max=%.4f\n"
                                             "roi_rg_ratio_min=%.4f,roi_bg_ratio_min=%.4f,roi_gg_ratio_min=%.4f\n"
                                             "roi_rg_ratio_mean=%.4f,roi_bg_ratio_mean=%.4f,roi_gg_ratio_mean=%.4f\n"
                                             "roi_rg_ratio_std=%.4f,roi_bg_ratio_std=%.4f,roi_gg_ratio_std=%.4f\n")
                               % roi_rg_ratio_max
                               % roi_bg_ratio_max
                               % roi_gg_ratio_max
                               % roi_rg_ratio_min
                               % roi_bg_ratio_min
                               % roi_gg_ratio_min
                               % roi_rg_ratio_mean
                               % roi_bg_ratio_mean
                               % roi_gg_ratio_mean
                               % roi_rg_ratio_std
                               % roi_bg_ratio_std
                               % roi_gg_ratio_std
                               );

         /*  GC2607 OFFSET -> 按比值 */
        for ( QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.GG_ratio.begin();
             ratio != ratio_value.GG_ratio.end();++ratio)
        {
            if(ratio.key().first == "center")
                factor_c2 = factor_center_c2;
            else
                factor_c2 = factor_corner_c2;

            if ((ratio.value()/GG_Ratio_rate - GG_Ratio_offset) >ratio.key().second *(factor_c2) /*|| ratio.value() < ratio.key().second*(factor_c2) */)
            {
                result_ratio.is_passed = false;
                g_str_msg_all[site_]+=boost::str(boost::format("FAIL at GG_ratio_class2 :%.4f,spec: %.4f\n")
                                       % (ratio.value()/GG_Ratio_rate  - GG_Ratio_offset)  % factor_c2);
            }
        }


        for (QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.RG_ratio.begin();
             ratio != ratio_value.RG_ratio.end();++ratio)
        {
            if(ratio.key().first == "center")
                factor_c2 = factor_center_c2;
            else
                factor_c2 = factor_corner_c2;

            if ((ratio.value()/RG_Ratio_rate - RG_Ratio_offset) >ratio.key().second *(1+factor_c2) || (ratio.value()/RG_Ratio_rate - RG_Ratio_offset) < ratio.key().second*(1-factor_c2) )
            {
                result_ratio.is_passed = false;
                g_str_msg_all[site_]+=boost::str(boost::format("FAIL at RG_ratio_class2 :%.4f, %.4f, %.4f; spec: %.4f\n")
                                       % (ratio.value()/RG_Ratio_rate - RG_Ratio_offset)  % (ratio.key().second *(1+factor_c2)) % (ratio.key().second*(1-factor_c2)) % factor_c2);
            }
        }

        for (QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.BG_ratio.begin();
             ratio != ratio_value.BG_ratio.end();++ratio)
        {
            if(ratio.key().first == "center")
                factor_c2 = factor_center_c2;
            else
                factor_c2 = factor_corner_c2;

            if ((ratio.value()/BG_Ratio_rate - BG_Ratio_offset) >ratio.key().second *(1+factor_c2) || (ratio.value()/BG_Ratio_rate - BG_Ratio_offset) < ratio.key().second*(1-factor_c2) )
            {
                result_ratio.is_passed = false;
                g_str_msg_all[site_]+=boost::str(boost::format("FAIL at BG_ratio_class2 :%.4f, %.4f, %.4f; spec: %.4f\n")
                                       % (ratio.value()/BG_Ratio_rate - BG_Ratio_offset)  % (ratio.key().second *(1+factor_c2)) % (ratio.key().second*(1-factor_c2)) % factor_c2);
            }
        }

        if (result_ratio.is_passed)
        {
            for (QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.GG_ratio.begin();
                 ratio != ratio_value.GG_ratio.end();++ratio)
            {
                if(ratio.key().first == "center")
                    factor_c1 = factor_center_c1;
                else
                    factor_c1 = factor_corner_c1;

                if ((ratio.value()/GG_Ratio_rate - GG_Ratio_offset) >ratio.key().second *(factor_c1)  )
                {
                    result_ratio_MA.is_passed = false;
                    g_str_msg_all[site_]+=boost::str(boost::format("FAIL at GG_ratio_class1:%.4f, spec: %.4f\n")
                                           % (ratio.value()/GG_Ratio_rate - GG_Ratio_offset)  % factor_c1);
                }
            }
            for (QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.RG_ratio.begin();
                 ratio != ratio_value.RG_ratio.end();++ratio)
            {
                if(ratio.key().first == "center")
                    factor_c1 = factor_center_c1;
                else
                    factor_c1 = factor_corner_c1;

                if ((ratio.value()/RG_Ratio_rate - RG_Ratio_offset) >ratio.key().second *(1+factor_c1) || (ratio.value()/RG_Ratio_rate - RG_Ratio_offset) < ratio.key().second*(1-factor_c1) )
                {
                    result_ratio_MA.is_passed = false;
                    g_str_msg_all[site_]+=boost::str(boost::format("FAIL at RG_ratio_class1 :%.4f, %.4f, %.4f; spec: %.4f\n")
                                           % (ratio.value()/RG_Ratio_rate - RG_Ratio_offset)  % (ratio.key().second *(1+factor_c1)) % (ratio.key().second*(1-factor_c1)) % factor_c1);
                }
            }
            for (QMap <QPair<QString,double>,double>::Iterator ratio = ratio_value.BG_ratio.begin();
                 ratio != ratio_value.BG_ratio.end();++ratio)
            {
                if(ratio.key().first == "center")
                    factor_c1 = factor_center_c1;
                else
                    factor_c1= factor_corner_c1;

                if ((ratio.value()/BG_Ratio_rate - BG_Ratio_offset) >ratio.key().second *(1+factor_c1) || (ratio.value()/BG_Ratio_rate - BG_Ratio_offset) < ratio.key().second*(1-factor_c1) )
                {
                    result_ratio_MA.is_passed = false;
                    g_str_msg_all[site_]+=boost::str(boost::format("FAIL at BG_ratio_class1:%.4f, %.4f, %.4f; spec: %.4f\n")
                                           % (ratio.value()/BG_Ratio_rate - BG_Ratio_offset)  % (ratio.key().second *(1+factor_c1)) % (ratio.key().second*(1-factor_c1)) % factor_c1);
                }
            }
        }




        //----------Center Region Gr/Gb Ratio -------------------
        double gr_gb_target = param_color_ratio_.GG_Ratio_target.value("center");
        double gr_gb_ratio_value = ratio_value.GG_ratio.value(QPair<QString,double>("center",param_color_ratio_.GG_Ratio_target.value("center")));
        double gr_gb_ratio_spec = param_color_ratio_.factor_center_gr_gb_ratio;
        if ((gr_gb_ratio_value-GG_Ratio_offset) > gr_gb_target*(gr_gb_ratio_spec) )
        {
            result_ratio.is_passed = false;
            g_str_msg_all[site_]+=boost::str(boost::format("FAIL at Center Gr/Gb ratio:%.4f, spec: %.4f\n")
                                   % (gr_gb_ratio_value-GG_Ratio_offset)  % gr_gb_ratio_spec);
        }

        if(roi_gg_ratio_max > gr_gb_ratio_spec)
        {
            result_grid.is_passed = false;
            g_str_msg_all[site_]+=boost::str(boost::format("FAIL at (Gr/Gb-1)ratio max:%.4f,spec: %.4f\n")
                                             % roi_gg_ratio_max  % (gr_gb_ratio_spec));
        }

        for (QMap <QString,double>::Iterator diff = ratio_value.G_G_diff.begin();
             diff != ratio_value.G_G_diff.end();++diff)
        {
            if (diff.value() > param_color_ratio_.gr_gb_diff_spec)
            {
                result_grid.is_passed = false;
                g_str_msg_all[site_]+=boost::str(boost::format("FAIL at (Gr-Gb) MAX:%.4f,spec: %.4f\n")
                                                 % diff.value()   % (param_color_ratio_.gr_gb_diff_spec));
            }

        }

        //可优化可读性
        ratio_value.gg_leftup = ratio_value.GG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.GG_Ratio_target.value("leftup")));
        ratio_value.rg_leftup = ratio_value.RG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.RG_Ratio_target.value("leftup")));
        ratio_value.bg_leftup = ratio_value.BG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.BG_Ratio_target.value("leftup")));

        ratio_value.gg_rightup = ratio_value.GG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.GG_Ratio_target.value("rightup")));
        ratio_value.rg_rightup = ratio_value.RG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.RG_Ratio_target.value("rightup")));
        ratio_value.bg_rightup = ratio_value.BG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.BG_Ratio_target.value("rightup")));

        ratio_value.gg_leftdown = ratio_value.GG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.GG_Ratio_target.value("leftdown")));
        ratio_value.rg_leftdown = ratio_value.RG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.RG_Ratio_target.value("leftdown")));
        ratio_value.bg_leftdown = ratio_value.BG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.BG_Ratio_target.value("leftdown")));

        ratio_value.gg_rightdown = ratio_value.GG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.GG_Ratio_target.value("rightdown")));
        ratio_value.rg_rightdown = ratio_value.RG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.RG_Ratio_target.value("rightdown")));
        ratio_value.bg_rightdown = ratio_value.BG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.BG_Ratio_target.value("rightdown")));

        ratio_value.gg_center = ratio_value.GG_ratio.value(QPair<QString,double>("center",param_color_ratio_.GG_Ratio_target.value("center")));
        ratio_value.rg_center = ratio_value.RG_ratio.value(QPair<QString,double>("center",param_color_ratio_.RG_Ratio_target.value("center")));
        ratio_value.bg_center = ratio_value.BG_ratio.value(QPair<QString,double>("center",param_color_ratio_.BG_Ratio_target.value("center")));


        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|")
                                         % ratio_value.gg_center
                                         % ratio_value.rg_center
                                         % ratio_value.bg_center
                                         % (ratio_value.gg_center/GG_Ratio_rate - GG_Ratio_offset)
                                         % (ratio_value.rg_center/RG_Ratio_rate - RG_Ratio_offset)
                                         % (ratio_value.bg_center/BG_Ratio_rate - BG_Ratio_offset)
                                         % ratio_value.G_G_diff.value("center")
                                         );
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_ratio.name.c_str());

        g_str_logtmp[site_]+=boost::str(boost::format("%.4f|%.4f|") % ratio_value.rg_center % ratio_value.bg_center );

        g_str_msg_all[site_]+=boost::str(boost::format("color ratio:\n"
                                                       "ratio_g_g_left_up=%.4f,ratio_r_g_left_up=%.4f,ratio_b_g_left_up=%.4f,gr_gb_diff_left_up=%.4f\n"
                                                       "ratio_g_g_right_up=%.4f,ratio_r_g_right_up=%.4f,ratio_b_g_right_up=%.4f,gr_gb_diff_right_up=%.4f\n"
                                                       "ratio_g_g_left_down=%.4f,ratio_r_g_left_down=%.4f,ratio_b_g_left_down=%.4f,gr_gb_diff_left_down=%.4f\n"
                                                       "ratio_g_g_right_down=%.4f,ratio_r_g_right_down=%.4f,ratio_b_g_right_down=%.4f,gr_gb_diff_right_down=%.4f\n"
                                                       "ratio_g_g_center=%.4f,ratio_r_g_center=%.4f,ratio_b_g_center=%.4f,gr_gb_diff_center=%.4f\n"
                                                       "ratio_g_g_center_rate_offset=%.4f,ratio_r_g_center_rate_offset=%.4f,ratio_b_g_center_rate_offset=%.4f\n")
                                         % ratio_value.gg_leftup
                                         % ratio_value.rg_leftup
                                         % ratio_value.bg_leftup
                                         % ratio_value.G_G_diff.value("leftup")
                                         % ratio_value.gg_rightup
                                         % ratio_value.rg_rightup
                                         % ratio_value.bg_rightup
                                         % ratio_value.G_G_diff.value("rightup")
                                         % ratio_value.gg_leftdown
                                         % ratio_value.rg_leftdown
                                         % ratio_value.bg_leftdown
                                         % ratio_value.G_G_diff.value("leftdown")
                                         % ratio_value.gg_rightdown
                                         % ratio_value.rg_rightdown
                                         % ratio_value.bg_rightdown
                                         % ratio_value.G_G_diff.value("rightdown")
                                         % ratio_value.gg_center
                                         % ratio_value.rg_center
                                         % ratio_value.bg_center
                                         % ratio_value.G_G_diff.value("center")
                                         % (ratio_value.gg_center/GG_Ratio_rate - GG_Ratio_offset)
                                         % (ratio_value.rg_center/RG_Ratio_rate - RG_Ratio_offset)
                                         % (ratio_value.bg_center/BG_Ratio_rate - BG_Ratio_offset)
                                         );
    }
    else
    {
        result_ratio.g_msg = false;
        result_ratio_MA.g_msg = false;
        result_grid.g_msg = false;
    }

    if(!result_ratio.is_passed)
    {
        result_ratio.bin=param_color_ratio_.bin_color_ratio_;
        result_ratio.grade = param_color_ratio_.grade_color_ratio_;
    }
    if(!result_ratio_MA.is_passed)
    {
        result_ratio_MA.bin=param_color_ratio_.bin_color_ratio_MA_;
        result_ratio_MA.grade=param_color_ratio_.grade_color_ratio_MA_;
    }
    if(!result_grid.is_passed)
    {
        result_grid.bin=param_color_ratio_.bin_GI_;
        result_grid.grade=param_color_ratio_.grade_GI_;
    }

    vector<Subitem_Result> result_ratio_test;
    result_ratio_test.push_back(result_ratio);
    result_ratio_test.push_back(result_ratio_MA);
    result_ratio_test.push_back(result_grid);

    return result_ratio_test;
}





Subitem_Result AlgorithmItems::PKG_Test(const ParamManage& manage)
{
    if(manage.pkg_version == "mpkg_V1.0")
        return PKG_Test_V1_0(manage);
    if(manage.pkg_version == "mpkg_V1.1")
        return PKG_Test_V1_1(manage);

    Subitem_Result res("PKG_Test", false);
    res.g_msg = false;
    return res;
}

Subitem_Result AlgorithmItems::PKG_Test_V1_0(const ParamManage& manage)
{

    Subitem_Result res("PKG_Test",true);

    int w=manage.width_;
    int h=manage.height_;
    int code_PKG(0);
    cv::Mat image=cv::Mat(cv::Size(w,h),CV_8UC1,g_buf_raw_);
    if(!image.data||image.empty())
    {
        g_str_msg_all[site_]+=boost::str(boost::format("cv::Mat construct fail!\n"));
    }
    if(manage.param_pkg_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------PKG Test V1.0-----------------------------\n"));

        g_str_msg_all[site_]+=boost::str(boost::format("start test PKG\n"));
        code_PKG= AlgorithmFuncs::FindPkG_WF (image,manage.param_pkg_,g_str_msg_all[site_]);

    }
    else
    {
        res.g_msg = false;
    }
    if (code_PKG>0)
    {
        res.is_passed = false;
        res.bin = code_PKG;
        res.grade = 5;
        g_str_msg_all[site_]+=boost::str(boost::format("find PKG!\n"));
    }

    return res;
}

Subitem_Result AlgorithmItems::PKG_Test_V1_1(const ParamManage& manage)
{
    time_begin=clock();

    Subitem_Result res("PKG_Test_V1.1",true);

    int w=manage.width_;
    int h=manage.height_;
    int code_PKG(0);
    cv::Mat image=cv::Mat(cv::Size(w,h),CV_8UC1,g_buf_raw_);
    if(!image.data||image.empty())
    {
        g_str_msg_all[site_]+=boost::str(boost::format("cv::Mat construct fail!\n"));
    }
    if(manage.param_pkg_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------PKG Test V1.1-----------------------------\n"));

        //PD单点检测
        std::vector<MClusterInf>pd_all_single_particle;
        PD_all_single_pix_find(manage, PKG, pd_all_single_particle);

        g_str_msg_all[site_]+=boost::str(boost::format("start test PKG\n"));
        code_PKG= AlgorithmFuncs::FindPkG_WF_1_1 (image,manage.param_pkg_,g_str_msg_all[site_],pd_all_single_particle);

    }
    else
    {
        res.g_msg = false;
    }
    if (code_PKG>0)
    {
        res.is_passed = false;
        res.bin = code_PKG;
        res.grade = 5;
        g_str_msg_all[site_]+=boost::str(boost::format("find PKG!\n"));
    }

    time_end=clock();
    g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";


    return res;
}



Subitem_Result AlgorithmItems::PD_all_single_pix_find(const ParamManage& manage,PD_single_type type, std::vector<MClusterInf> &pd_all_single_particle)
{
    time_begin=clock();

    Subitem_Result res("pd_all_single_particle",false,
                       0,
                       0);

    int w=manage.width_;
    int h=manage.height_;



        double diff_spec_r_= 0;
        double diff_spec_gr_= 0;
        double diff_spec_gb_ = 0;
        double diff_spec_b_= 0;

        double diff_blemish_spec_r_= 0;
        double diff_blemish_spec_gr_= 0;
        double diff_blemish_spec_gb_ = 0;
        double diff_blemish_spec_b_= 0;
        int cnt_skip_= 0;
        int r_area_= 5;
        int save_image_= 0;

        Spec_Mode spec_mode_=Ratio;

        if(type==Blemish)
        {
            diff_spec_r_= manage.param_blemish_old_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_blemish_old_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_blemish_old_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_blemish_old_.ratio_diff_spec_b_;

            cnt_skip_ = manage.param_blemish_old_.cnt_skip_;
            r_area_ = manage.param_blemish_old_.r_area_;
            save_image_ = manage.param_blemish_old_.save_image_;

        }
        else if(type==PKG)
        {
            diff_spec_r_= manage.param_pkg_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_pkg_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pkg_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_pkg_.ratio_diff_spec_b_;
        }

        TestItem::find_all_single_pixel_defect(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,
                                    g_buf_delta_r_, g_buf_delta_gr_, g_buf_delta_gb_, g_buf_delta_b_, g_buf_delta_raw_,
                                    g_binary_r_, g_binary_gr_, g_binary_gb_, g_binary_b_, g_binary_,
                                    g_buf_blemish_r_, g_buf_blemish_gr_, g_buf_blemish_gb_, g_buf_blemish_b_, g_buf_blemish_raw_,
                                    w, h, cnt_skip_, r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,save_image_, g_str_msg_all[site_], pd_all_single_particle);


    time_end=clock();
    g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";


    return res;

}


Subitem_Result AlgorithmItems::Blemish_Test(const ParamManage& manage)
{
    if(manage.blemish_version == "blemish_V1.0")
        return Blemish_Test_V1_0(manage);
    else if(manage.blemish_version == "blemish_V1.1")
        return Blemish_Test_V1_1(manage, Pixel_Defect_bright);

    Subitem_Result res("Blemish_Test", false);
    res.g_msg = false;
    return res;
}


Subitem_Result AlgorithmItems::Blemish_Test_V1_0(const ParamManage& manage)
{
    time_begin=clock();
    Subitem_Result res("Blemish_Test",true);

    int w=manage.width_;
    int h=manage.height_;
    int code_Blemish(0);
    cv::Mat image=cv::Mat(cv::Size(w,h),CV_8UC1,g_buf_raw_);
    if(!image.data||image.empty())
    {
        g_str_msg_all[site_]+=boost::str(boost::format("cv::Mat construct fail!\n"));
    }
    if(manage.param_blemish_old_.open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Blemish Test V1.0-----------------------------\n"));

        g_str_msg_all[site_]+=boost::str(boost::format("start test Blemish\n"));
        code_Blemish= AlgorithmFuncs::FindBlemsh_WF (image,manage.param_blemish_old_,g_str_msg_all[site_]);

    }
    else
        res.g_msg = false;

    if (code_Blemish>0)
    {
        res.is_passed = false;
        res.bin = code_Blemish;
        res.grade = manage.param_blemish_old_.grade_;
        g_str_msg_all[site_]+=boost::str(boost::format("find param_blemish!\n"));
    }

    return res;
}



Subitem_Result AlgorithmItems::Blemish_Test_V1_1(const ParamManage& manage,Pixel_Defect_type type)
{
    time_begin=clock();
    Subitem_Result res("Blemish_Test_V1.1",true);

    int w=manage.width_;
    int h=manage.height_;
    int code_Blemish(0);
    cv::Mat image=cv::Mat(cv::Size(w,h),CV_8UC1,g_buf_raw_);
    if(!image.data||image.empty())
    {
        g_str_msg_all[site_]+=boost::str(boost::format("cv::Mat construct fail!\n"));
    }
    if(manage.param_blemish_old_.open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Blemish Test V1.1-----------------------------\n"));

        //PD单点检测
        std::vector<MClusterInf>pd_all_single_particle;
        PD_all_single_pix_find(manage, Blemish, pd_all_single_particle);

        g_str_msg_all[site_]+=boost::str(boost::format("start test Blemish\n"));
        code_Blemish= AlgorithmFuncs::FindBlemsh_WF_NEW (image,manage.param_blemish_old_,g_str_msg_all[site_],pd_all_single_particle);

    }
    else
        res.g_msg = false;

    if (code_Blemish>0)
    {
        res.is_passed = false;
        res.bin = code_Blemish;
        res.grade = manage.param_blemish_old_.grade_;
        g_str_msg_all[site_]+=boost::str(boost::format("find param_blemish!\n"));
    }

    time_end=clock();
    g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";


    return res;
}


//Pixel Defect Test (黑屏、白屏)
Subitem_Result AlgorithmItems::Pixel_Defect_Test_NO_Test_Info( ParamManage& manage,Pixel_Defect_type type)
{
    Subitem_Result res("Pixel_Defect",false,
                       0,
                       0);

    res.g_msg = false;


    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_pixel_defect_.open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Pixel Defect Test NO_Test_Info-----------------------------\n"
                                                       "Version: %s\n")
                                        % manage.pixel_defect_version.toStdString());
        time_begin=clock();

        int cnt_white_pix = 0;
        int cnt_dark_pix = 0;

        int cnt_1x2_ch = 0;
        int cnt_2x1_ch = 0;
        int cnt_2x2_leftdiagonal_ch = 0;
        int cnt_2x2_rightdiagonal_ch = 0;

        int cnt_2x4_pix = 0;
        int cnt_2x2_pix = 0;
        int cnt_1x2_2x1_pix = 0;
        int cnt_2x2_diagonal_pix = 0;

        int paticle =0;
        int other_cluster =0;
        int other_cluster_ch =0;

        int total_cluster_cnt = 0;
        QMap<int,int> cnts_cluster;

        int blemish_cnt =0;
        int cnt_white_pix_blemish = 0;
        int cnt_dark_pix_blemish = 0;

        double diff_spec_r_= 0;
        double diff_spec_gr_= 0;
        double diff_spec_gb_ = 0;
        double diff_spec_b_= 0;

        double diff_blemish_spec_r_= 0;
        double diff_blemish_spec_gr_= 0;
        double diff_blemish_spec_gb_ = 0;
        double diff_blemish_spec_b_= 0;

        Spec_Mode spec_mode_=Ratio;

        if(type==Pixel_Defect_bright)
        {
            diff_spec_r_= manage.param_pixel_defect_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.ratio_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_spec_r_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_spec_gr_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_spec_gb_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_spec_b_;
        }
        else if(type==Pixel_Defect_dark)
        {
            diff_spec_r_= manage.param_pixel_defect_.DN_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.DN_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.DN_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.DN_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_dn_spec_;
            spec_mode_=DN;
        }

        std::vector<std::string> g_str_msg_all_temp(4);
        TestItem::find_pixel_defect(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,
                                    g_buf_delta_r_, g_buf_delta_gr_, g_buf_delta_gb_, g_buf_delta_b_, g_buf_delta_raw_,
                                    g_binary_r_, g_binary_gr_, g_binary_gb_, g_binary_b_, g_binary_,
                                    g_buf_blemish_r_, g_buf_blemish_gr_, g_buf_blemish_gb_, g_buf_blemish_b_, g_buf_blemish_raw_,
                                    w, h, manage.param_pixel_defect_.cnt_skip_, manage.param_pixel_defect_.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,
                                    manage.param_pixel_defect_.blemish_global_size_limit_,
                                    manage.param_pixel_defect_.blemish_bychannel_size_limit_,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,g_buf_tmp_,
                                    manage.param_pixel_defect_.save_image_, g_str_msg_all_temp[site_]);

        time_end=clock();
        g_str_msg_all[site_]+=boost::str(boost::format("test pd but no test pd info!\n"));
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = true;
        g_str_msg_all[site_]+=boost::str(boost::format("need PixelDefect.data, set code -2 \n"));
        res.bin= -2;
        res.grade = 10;
        return res;

    }


    res.bin= 1;
    res.grade = 1;

    return res;


}
//PD Test (黑屏、白屏、灰屏)
Subitem_Result AlgorithmItems::Single_Pixel_Defect_Test( ParamManage& manage,Pixel_Defect_type type)
{
    if(manage.single_pixel_defect_version == "PD_V1.0")
        return Single_Pixel_Defect_Test_V1_0(manage, type);
    Subitem_Result res("Single_Pixel_Defect", false);
    res.g_msg = false;
    return res;
}

Subitem_Result AlgorithmItems::Single_Pixel_Defect_Test_V1_0( ParamManage& manage,Pixel_Defect_type type)
{

//    Subitem_Result res("Single_Pixel_Defect",false,
//                       0,
//                       0);
        Subitem_Result res("Single_Pixel_Defect",true,
                           1,
                           0);

    if(manage.param_single_pixel_defect_.type_ == 0)
        type = Pixel_Defect_dark;
    else if(manage.param_single_pixel_defect_.type_ == 1)
        type = Pixel_Defect_bright;

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_single_pixel_defect_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------PD_Test_V1_0-----------------------------\n"));

        int cnt_white_pix = 0;
        int cnt_dark_pix = 0;

        int cnt_1x2_ch = 0;
        int cnt_2x1_ch = 0;
        int cnt_2x2_leftdiagonal_ch = 0;
        int cnt_2x2_rightdiagonal_ch = 0;

        int cnt_2x4_pix = 0;
        int cnt_2x2_pix = 0;
        int cnt_1x2_2x1_pix = 0;
        int cnt_2x2_diagonal_pix = 0;

        int paticle =0;
        int other_cluster =0;
        int other_cluster_ch =0;

        int total_cluster_cnt = 0;
        QMap<int,int> cnts_cluster;

        int blemish_cnt =0;
        int cnt_white_pix_blemish = 0;
        int cnt_dark_pix_blemish = 0;

        double diff_spec_r_= 0;
        double diff_spec_gr_= 0;
        double diff_spec_gb_ = 0;
        double diff_spec_b_= 0;

        double diff_blemish_spec_r_= 0;
        double diff_blemish_spec_gr_= 0;
        double diff_blemish_spec_gb_ = 0;
        double diff_blemish_spec_b_= 0;

        Spec_Mode spec_mode_=Ratio;

        if(type==Pixel_Defect_bright)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_bright \n"));

            diff_spec_r_= manage.param_single_pixel_defect_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_single_pixel_defect_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_single_pixel_defect_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_single_pixel_defect_.ratio_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_single_pixel_defect_.blemish_spec_r_;
            diff_blemish_spec_gr_= manage.param_single_pixel_defect_.blemish_spec_gr_;
            diff_blemish_spec_gb_ = manage.param_single_pixel_defect_.blemish_spec_gb_;
            diff_blemish_spec_b_= manage.param_single_pixel_defect_.blemish_spec_b_;
        }
        else if(type==Pixel_Defect_dark)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_dark \n"));

            diff_spec_r_= manage.param_single_pixel_defect_.DN_diff_spec_r_;
            diff_spec_gr_= manage.param_single_pixel_defect_.DN_diff_spec_gr_;
            diff_spec_gb_ =manage.param_single_pixel_defect_.DN_diff_spec_gb_;
            diff_spec_b_= manage.param_single_pixel_defect_.DN_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_single_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gr_= manage.param_single_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gb_ = manage.param_single_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_b_= manage.param_single_pixel_defect_.blemish_dn_spec_;
            spec_mode_=DN;
        }

        TestItem::find_pixel_defect(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,
                                    g_buf_delta_r_, g_buf_delta_gr_, g_buf_delta_gb_, g_buf_delta_b_, g_buf_delta_raw_,
                                    g_binary_r_, g_binary_gr_, g_binary_gb_, g_binary_b_, g_binary_,
                                    g_buf_blemish_r_, g_buf_blemish_gr_, g_buf_blemish_gb_, g_buf_blemish_b_, g_buf_blemish_raw_,
                                    w, h, manage.param_single_pixel_defect_.cnt_skip_, manage.param_single_pixel_defect_.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,
                                    manage.param_single_pixel_defect_.blemish_global_size_limit_,
                                    manage.param_single_pixel_defect_.blemish_bychannel_size_limit_,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,g_buf_tmp_,
                                    manage.param_single_pixel_defect_.save_image_, g_str_msg_all[site_]);


            Subitem_Result res_singe_wp("single_pix_WP_",true,1,0);
            Subitem_Result res_singe_dp("single_pix_DP_",true,1,0);

            if (cnt_white_pix>=manage.param_single_pixel_defect_.limit_wp1_ &&
                    cnt_white_pix<manage.param_single_pixel_defect_.limit_wp2_)
            {
                res_singe_wp.bin =manage.param_single_pixel_defect_.bin_limit_wp1_ ;
                res_singe_wp.grade = manage.param_single_pixel_defect_.grade_limit_wp1_;
                res_singe_wp.is_passed = false;
            }
            else if (cnt_white_pix>=manage.param_single_pixel_defect_.limit_wp2_ &&
                     cnt_white_pix<manage.param_single_pixel_defect_.limit_wp3_)
            {
                res_singe_wp.bin =manage.param_single_pixel_defect_.bin_limit_wp2_ ;
                res_singe_wp.grade = manage.param_single_pixel_defect_.grade_limit_wp2_;
                res_singe_wp.is_passed = false;
            }
            else if (cnt_white_pix>=manage.param_single_pixel_defect_.limit_wp3_)
            {
                res_singe_wp.bin =manage.param_single_pixel_defect_.bin_limit_wp3_ ;
                res_singe_wp.grade = manage.param_single_pixel_defect_.grade_limit_wp3_;
                res_singe_wp.is_passed = false;
            }

            if (cnt_dark_pix>=manage.param_single_pixel_defect_.limit_dp1_ &&
                    cnt_dark_pix< manage.param_single_pixel_defect_.limit_dp2_)
            {
                res_singe_dp.bin = manage.param_single_pixel_defect_.bin_limit_dp1_ ;
                res_singe_dp.grade = manage.param_single_pixel_defect_.grade_limit_dp1_;
                res_singe_dp.is_passed = false;
            }
            else if (cnt_dark_pix>=manage.param_single_pixel_defect_.limit_dp2_ &&
                     cnt_dark_pix< manage.param_single_pixel_defect_.limit_dp3_)
            {
                res_singe_dp.bin = manage.param_single_pixel_defect_.bin_limit_dp2_ ;
                res_singe_dp.grade = manage.param_single_pixel_defect_.grade_limit_dp2_;
                res_singe_dp.is_passed = false;
            }
            else if (cnt_dark_pix>=manage.param_single_pixel_defect_.limit_dp3_ )
            {
                res_singe_dp.bin = manage.param_single_pixel_defect_.bin_limit_dp3_ ;
                res_singe_dp.grade = manage.param_single_pixel_defect_.grade_limit_dp3_;
                res_singe_dp.is_passed = false;
            }

            if(res_singe_dp.grade > res_singe_wp.grade)
            {
                res.bin = res_singe_dp.bin;
                res.grade = res_singe_dp.grade;
                res.is_passed = res_singe_dp.is_passed;
            }
            else
            {
                res.bin = res_singe_wp.bin;
                res.grade = res_singe_wp.grade;
                res.is_passed = res_singe_wp.is_passed;
            }
            // jundge

            //通过外配来卡黑点、白点还是白点+黑点
            int pixel_count_to_check = 0;
            if(manage.param_single_pixel_defect_.single_pix_type_ == 0)          // cnt_dark_pix
            {
                pixel_count_to_check = cnt_dark_pix;
                g_str_msg_all[site_]+=boost::str(boost::format("cnt_dark_pix check\n"));
            }
            else if(manage.param_single_pixel_defect_.single_pix_type_ == 1)     // cnt_white_pix
            {
                pixel_count_to_check = cnt_white_pix;
                g_str_msg_all[site_]+=boost::str(boost::format("cnt_white_pix check\n"));
            }
            else if(manage.param_single_pixel_defect_.single_pix_type_ == 2)     // cnt_white_pix + cnt_dark_pix
            {
                pixel_count_to_check = cnt_white_pix + cnt_dark_pix;
                g_str_msg_all[site_]+=boost::str(boost::format("cnt_white_pix + cnt_dark_pix check\n"));
            }


            if ((pixel_count_to_check) > manage.param_single_pixel_defect_.single_pix_limit_)
            {
                res.bin = manage.param_single_pixel_defect_.bin_single_pix_;
                res.grade = manage.param_single_pixel_defect_.grade10_;
                res.is_passed = false;
            }

        g_str_msg_all[site_]+=boost::str(boost::format( "cnt_white_pix:%d, cnt_dark_pix:%d\n")
                                         % cnt_white_pix % cnt_dark_pix);


        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|")
                                         % cnt_white_pix % cnt_dark_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }



    return res;
}


//Pixel Defect Test (黑屏、白屏)
Subitem_Result AlgorithmItems::Pixel_Defect_Test( ParamManage& manage,Pixel_Defect_type type)
{
    if(manage.pixel_defect_version == "pixel_defect_V1.0")
        return Pixel_Defect_Test_V1_0(manage, type);
    else if(manage.pixel_defect_version == "pixel_defect_V1.1")   //GC8613 PD 单点卡控专用
        return Pixel_Defect_Test_V1_1(manage, type);
    else if(manage.pixel_defect_version == "pixel_defect_V1.2")   //GC8613T,GC8613UA 双坏点卡控 20 个数量专用
        return Pixel_Defect_Test_V1_2(manage, type);
    else if(manage.pixel_defect_version == "pixel_defect_V1.3")   //支持单点分段卡控外配，白点、黑点单独卡控
        return Pixel_Defect_Test_V1_3(manage, type);
    else if(manage.pixel_defect_version == "pixel_defect_V1.4")   //兼容卡团簇&卡单点（通过外配卡黑白点）&单点分段卡控
        return Pixel_Defect_Test_V1_4(manage, type);

    Subitem_Result res("Pixel_Defect", false);
    res.g_msg = false;
    return res;
}


Subitem_Result AlgorithmItems::Pixel_Defect_Test_V1_0( ParamManage& manage,Pixel_Defect_type type)
{

    Subitem_Result res("Pixel_Defect",false,
                       0,
                       0);

    if(manage.param_pixel_defect_.type_ == 0)
        type = Pixel_Defect_dark;
    else if(manage.param_pixel_defect_.type_ == 1)
        type = Pixel_Defect_bright;

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_pixel_defect_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Pixel Defect Test V1.0-----------------------------\n"));

        int cnt_white_pix = 0;
        int cnt_dark_pix = 0;

        int cnt_1x2_ch = 0;
        int cnt_2x1_ch = 0;
        int cnt_2x2_leftdiagonal_ch = 0;
        int cnt_2x2_rightdiagonal_ch = 0;

        int cnt_2x4_pix = 0;
        int cnt_2x2_pix = 0;
        int cnt_1x2_2x1_pix = 0;
        int cnt_2x2_diagonal_pix = 0;

        int paticle =0;
        int other_cluster =0;
        int other_cluster_ch =0;

        int total_cluster_cnt = 0;
        QMap<int,int> cnts_cluster;

        int blemish_cnt =0;
        int cnt_white_pix_blemish = 0;
        int cnt_dark_pix_blemish = 0;

        double diff_spec_r_= 0;
        double diff_spec_gr_= 0;
        double diff_spec_gb_ = 0;
        double diff_spec_b_= 0;

        double diff_blemish_spec_r_= 0;
        double diff_blemish_spec_gr_= 0;
        double diff_blemish_spec_gb_ = 0;
        double diff_blemish_spec_b_= 0;

        Spec_Mode spec_mode_=Ratio;

        if(type==Pixel_Defect_bright)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_bright \n"));

            diff_spec_r_= manage.param_pixel_defect_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.ratio_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_spec_r_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_spec_gr_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_spec_gb_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_spec_b_;
        }
        else if(type==Pixel_Defect_dark)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_dark \n"));

            diff_spec_r_= manage.param_pixel_defect_.DN_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.DN_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.DN_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.DN_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_dn_spec_;
            spec_mode_=DN;
        }

        TestItem::find_pixel_defect(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,
                                    g_buf_delta_r_, g_buf_delta_gr_, g_buf_delta_gb_, g_buf_delta_b_, g_buf_delta_raw_,
                                    g_binary_r_, g_binary_gr_, g_binary_gb_, g_binary_b_, g_binary_,
                                    g_buf_blemish_r_, g_buf_blemish_gr_, g_buf_blemish_gb_, g_buf_blemish_b_, g_buf_blemish_raw_,
                                    w, h, manage.param_pixel_defect_.cnt_skip_, manage.param_pixel_defect_.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,
                                    manage.param_pixel_defect_.blemish_global_size_limit_,
                                    manage.param_pixel_defect_.blemish_bychannel_size_limit_,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,g_buf_tmp_,
                                    manage.param_pixel_defect_.save_image_, g_str_msg_all[site_]);

        g_str_msg_all[site_]+=boost::str(boost::format("Total Pixel Cluster Defect including global and bychannel Cnt:%d\n")
                               % total_cluster_cnt);
        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;
        cnt_2x2_diagonal_pix = cnt_2x2_diagonal_pix/2;

        int cnt_cluster_ch = cnt_1x2_ch+cnt_2x1_ch+
                cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch + other_cluster_ch;
        int cnt_cluster_global = cnt_1x2_2x1_pix + cnt_2x2_diagonal_pix;

        int cnts_cluster_2all = cnt_1x2_2x1_pix+cnt_2x2_diagonal_pix+cnt_1x2_ch+cnt_2x1_ch+cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch;
        int cnts_cluster_2more = cnt_2x4_pix + cnt_2x2_pix + other_cluster;

        int cnt_all_1x2_2x1_pix= cnt_1x2_ch+cnt_2x1_ch + cnt_1x2_2x1_pix ;                                        //全部1*2 2*1坏点
        int cnt_all_diagonal_2x2_pix= cnt_2x2_rightdiagonal_ch+cnt_2x2_leftdiagonal_ch + cnt_2x2_diagonal_pix ;   //全部diagonal 2*2坏点

        if (paticle>0)
        {
            res.bin = manage.param_pixel_defect_.bin3_;
            res.grade = manage.param_pixel_defect_.grade3_;
        }
        else if(other_cluster>0)
        {
            res.bin = manage.param_pixel_defect_.bin2_;
            res.grade = manage.param_pixel_defect_.grade2_;
        }
        else if (cnt_2x4_pix >0)
        {
            res.bin = manage.param_pixel_defect_.bin2x4_;
            res.grade  = manage.param_pixel_defect_.grade4_;
        }
        else if (cnt_2x2_pix >0)
        {
             res.bin  = manage.param_pixel_defect_.bin2x2_global_;
            res.grade  = manage.param_pixel_defect_.grade9_;
        }

        else if (cnt_cluster_ch>0)
        {
            // all defect clusters in channel are all 1x2 type
            if ((cnt_1x2_ch+cnt_2x1_ch) != 0 &&
                    cnt_cluster_ch == (cnt_1x2_ch+cnt_2x1_ch))
            {
                res.bin = manage.param_pixel_defect_.bin1x2_2x1_ch_;
                 res.grade= manage.param_pixel_defect_.grade7_;
            }
            // all defect clusters in channel are all 2x2 diagonal type
            else if ((cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch) != 0 &&
                     cnt_cluster_ch == (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch))
            {
                res.bin  = manage.param_pixel_defect_.bin2x2_diagonal_ch_;
                res.grade= manage.param_pixel_defect_.grade8_;
            }

            // all defect clusters in channel are 1x2 type and 2x2 type
            else if ((cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch) != 0 &&
                     (cnt_1x2_ch+cnt_2x1_ch) != 0 &&
                     cnt_cluster_ch == (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch+cnt_1x2_ch+cnt_2x1_ch))
            {
                res.bin = manage.param_pixel_defect_.bin1_;
                res.grade = manage.param_pixel_defect_.grade1_;
            }
            else
            {
                res.bin = manage.param_pixel_defect_.bin3_;
                res.grade = manage.param_pixel_defect_.grade3_;
            }
        }

        else if (cnt_cluster_global > 0)
        {
            int grade_min = std::min(manage.param_pixel_defect_.grade1_,
                                     std::min(manage.param_pixel_defect_.grade2_,
                                              manage.param_pixel_defect_.grade3_));
            int bin_min = std::min(manage.param_pixel_defect_.bin1_,
                                   std::min(manage.param_pixel_defect_.bin2_,
                                            manage.param_pixel_defect_.bin3_));

            if (cnt_1x2_2x1_pix != 0 &&
                    cnt_cluster_global == cnt_1x2_2x1_pix)
            {
                if (cnt_cluster_ch == 0)
                {
                    res.bin = manage.param_pixel_defect_.bin1x2_2x1_;
                    res.grade = manage.param_pixel_defect_.grade5_;
                }
            }
            else if (cnt_2x2_diagonal_pix != 0 &&
                     cnt_cluster_global == cnt_2x2_diagonal_pix)
            {
                res.bin = manage.param_pixel_defect_.bin2x2_diagonal_;
                res.grade = manage.param_pixel_defect_.grade6_;
            }
            else
            {
                res.bin = bin_min;
                res.grade = grade_min;
            }
        }

        if (paticle ==0 &&
                other_cluster ==0 &&
                cnt_2x4_pix ==0 &&
                cnt_2x2_pix ==0 &&
                cnt_cluster_ch == 0 &&
                cnt_all_1x2_2x1_pix <= manage.param_pixel_defect_.all_1x2_2x1_spec &&
                cnt_all_diagonal_2x2_pix < 5)
        {
            res.bin = 2001;
            res.grade = 0;
            if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp1_ &&
                    cnt_white_pix<manage.param_pixel_defect_.limit_wp2_)
            {
                res.bin= 2002;
                res.grade = 1;
            }
            else if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp2_ &&
                     cnt_white_pix<manage.param_pixel_defect_.limit_wp3_)
            {
                res.bin = 2003;
                res.grade = 1;
            }
            else if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp3_)
            {
                res.bin= 2004;
                res.grade = 1;
            }
             if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp1_ &&
                     cnt_dark_pix< manage.param_pixel_defect_.limit_dp2_)
            {
                res.bin = 2006;
                res.grade = 1;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp2_ &&
                     cnt_dark_pix< manage.param_pixel_defect_.limit_dp3_)
            {
                res.bin= 2007;
                res.grade = 1;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp3_ )
            {
                res.bin = 2008;
                res.grade = 1;
            }


            // jundge
            if (cnt_white_pix > manage.param_pixel_defect_.single_pix_limit_ ||
                    cnt_cluster_global > manage.param_pixel_defect_.single_pix_limit_ )
            {
                res.bin = manage.param_pixel_defect_.bin_single_pix_;
                res.grade = manage.param_pixel_defect_.grade10_;
            }
            else if(blemish_cnt >0)
            {
                res.bin = manage.param_pixel_defect_.bin_blemish_;
                res.grade = manage.param_pixel_defect_.grade_blemish_;
            }
            else if(cnt_white_pix_blemish >manage.param_pixel_defect_.blemish_limit_wp3_ ||
                    cnt_dark_pix_blemish >manage.param_pixel_defect_.blemish_limit_dp3_)
            {
                res.bin = manage.param_pixel_defect_.bin_blemish_;
                res.grade = manage.param_pixel_defect_.grade_blemish_;
            }
        }


        int global_cluster_cnt =cnt_cluster_global + paticle + other_cluster + cnt_2x4_pix + cnt_2x2_pix;
        g_str_msg_all[site_]+=boost::str(boost::format("bychannel total clusters:%d\n"
                                                       "cnts of 1x2 or 2x1 bychannel:%d\n"
                                                       "cnts of 2x2 diagonal bychannel:%d\n"
                                                       "cnts of all 1x2 2x1 cluster:%d\n"
                                                       "cnts of all 2x2 cluster:%d\n")
                                         % cnt_cluster_ch % (cnt_1x2_ch+cnt_2x1_ch)
                                         % (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch)
                                         % cnt_all_1x2_2x1_pix % cnt_all_diagonal_2x2_pix);

        g_str_msg_all[site_]+=boost::str(boost::format("global total clusters:%d\n"
                                             "particle cnt:%d\n"
                                             "global 2x4 cluster cnt:%d\n"
                                             "global 2x2 cluster cnt:%d\n"
                                             "global 1x2_2x1_cluster cnt:%d\n"
                                             "global 2x2_diagonal_cluster cnt:%d\n"
                                             "cnt_white_pix:%d, cnt_dark_pix:%d\n")
                               % global_cluster_cnt
                               % paticle
                               % cnt_2x4_pix
                               % cnt_2x2_pix
                               % cnt_1x2_2x1_pix
                               % cnt_2x2_diagonal_pix
                               % cnt_white_pix % cnt_dark_pix);


        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|")
                                         % cnt_cluster_ch % global_cluster_cnt % paticle % cnt_white_pix % cnt_dark_pix);
        //        if(type==Pixel_Defect_dark)
        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|")
                                         % cnt_white_pix % cnt_dark_pix % cnt_all_1x2_2x1_pix % cnt_all_diagonal_2x2_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        for (int i=0; i<15; ++i)
        {
            if(i==1 || i==3 || i==5 || i==6)
                cnts_cluster[i] = cnts_cluster.value(i)/2;
            //            g_str_log_all[site_]+=boost::str(boost::format("%d|")
            //                                   % cnts_cluster.value(i));
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }



    return res;
}



Subitem_Result AlgorithmItems::Pixel_Defect_Test_V1_1( ParamManage& manage,Pixel_Defect_type type)
{

    Subitem_Result res("Pixel_Defect_V1.1",false,
                       0,
                       0);

    if(manage.param_pixel_defect_.type_ == 0)
        type = Pixel_Defect_dark;
    else if(manage.param_pixel_defect_.type_ == 1)
        type = Pixel_Defect_bright;

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_pixel_defect_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Pixel Defect Test V1.1-----------------------------\n"));

        int cnt_white_pix = 0;
        int cnt_dark_pix = 0;

        int cnt_1x2_ch = 0;
        int cnt_2x1_ch = 0;
        int cnt_2x2_leftdiagonal_ch = 0;
        int cnt_2x2_rightdiagonal_ch = 0;

        int cnt_2x4_pix = 0;
        int cnt_2x2_pix = 0;
        int cnt_1x2_2x1_pix = 0;
        int cnt_2x2_diagonal_pix = 0;

        int paticle =0;
        int other_cluster =0;
        int other_cluster_ch =0;

        int total_cluster_cnt = 0;
        QMap<int,int> cnts_cluster;

        int blemish_cnt =0;
        int cnt_white_pix_blemish = 0;
        int cnt_dark_pix_blemish = 0;

        double diff_spec_r_= 0;
        double diff_spec_gr_= 0;
        double diff_spec_gb_ = 0;
        double diff_spec_b_= 0;

        double diff_blemish_spec_r_= 0;
        double diff_blemish_spec_gr_= 0;
        double diff_blemish_spec_gb_ = 0;
        double diff_blemish_spec_b_= 0;

        Spec_Mode spec_mode_=Ratio;

        if(type==Pixel_Defect_bright)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_bright \n"));

            diff_spec_r_= manage.param_pixel_defect_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.ratio_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_spec_r_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_spec_gr_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_spec_gb_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_spec_b_;
        }
        else if(type==Pixel_Defect_dark)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_dark \n"));

            diff_spec_r_= manage.param_pixel_defect_.DN_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.DN_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.DN_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.DN_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_dn_spec_;
            spec_mode_=DN;
        }

        TestItem::find_pixel_defect(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,
                                    g_buf_delta_r_, g_buf_delta_gr_, g_buf_delta_gb_, g_buf_delta_b_, g_buf_delta_raw_,
                                    g_binary_r_, g_binary_gr_, g_binary_gb_, g_binary_b_, g_binary_,
                                    g_buf_blemish_r_, g_buf_blemish_gr_, g_buf_blemish_gb_, g_buf_blemish_b_, g_buf_blemish_raw_,
                                    w, h, manage.param_pixel_defect_.cnt_skip_, manage.param_pixel_defect_.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,
                                    manage.param_pixel_defect_.blemish_global_size_limit_,
                                    manage.param_pixel_defect_.blemish_bychannel_size_limit_,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,g_buf_tmp_,
                                    manage.param_pixel_defect_.save_image_, g_str_msg_all[site_]);

        if (cnt_white_pix > manage.param_pixel_defect_.single_pix_limit_)
        {
            res.bin = manage.param_pixel_defect_.bin_single_pix_;
            res.grade = manage.param_pixel_defect_.grade10_;
        }

        g_str_msg_all[site_]+=boost::str(boost::format("SPEC:\n "
                                                       "cnt_white_pix:%d, spec:%.4f \n")
                                         % cnt_white_pix % manage.param_pixel_defect_.single_pix_limit_);

        g_str_msg_all[site_]+=boost::str(boost::format("cnt_white_pix:%d\n")
                               % cnt_white_pix);

        g_str_log_all[site_]+=boost::str(boost::format("%d|")
                                         % cnt_white_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());


        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }



    return res;
}



Subitem_Result AlgorithmItems::Pixel_Defect_Test_V1_2( ParamManage& manage,Pixel_Defect_type type)
{

    Subitem_Result res("Pixel_Defect_V1.2",false,
                       0,
                       0);

    if(manage.param_pixel_defect_.type_ == 0)
        type = Pixel_Defect_dark;
    else if(manage.param_pixel_defect_.type_ == 1)
        type = Pixel_Defect_bright;

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_pixel_defect_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Pixel Defect Test V1.2-----------------------------\n"));

        int cnt_white_pix = 0;
        int cnt_dark_pix = 0;

        int cnt_1x2_ch = 0;
        int cnt_2x1_ch = 0;
        int cnt_2x2_leftdiagonal_ch = 0;
        int cnt_2x2_rightdiagonal_ch = 0;

        int cnt_2x4_pix = 0;
        int cnt_2x2_pix = 0;
        int cnt_1x2_2x1_pix = 0;
        int cnt_2x2_diagonal_pix = 0;

        int paticle =0;
        int other_cluster =0;
        int other_cluster_ch =0;

        int total_cluster_cnt = 0;
        QMap<int,int> cnts_cluster;

        int blemish_cnt =0;
        int cnt_white_pix_blemish = 0;
        int cnt_dark_pix_blemish = 0;

        double diff_spec_r_= 0;
        double diff_spec_gr_= 0;
        double diff_spec_gb_ = 0;
        double diff_spec_b_= 0;

        double diff_blemish_spec_r_= 0;
        double diff_blemish_spec_gr_= 0;
        double diff_blemish_spec_gb_ = 0;
        double diff_blemish_spec_b_= 0;

        Spec_Mode spec_mode_=Ratio;

        if(type==Pixel_Defect_bright)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_bright \n"));

            diff_spec_r_= manage.param_pixel_defect_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.ratio_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_spec_r_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_spec_gr_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_spec_gb_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_spec_b_;
        }
        else if(type==Pixel_Defect_dark)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_dark \n"));

            diff_spec_r_= manage.param_pixel_defect_.DN_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.DN_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.DN_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.DN_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_dn_spec_;
            spec_mode_=DN;
        }

        TestItem::find_pixel_defect(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,
                                    g_buf_delta_r_, g_buf_delta_gr_, g_buf_delta_gb_, g_buf_delta_b_, g_buf_delta_raw_,
                                    g_binary_r_, g_binary_gr_, g_binary_gb_, g_binary_b_, g_binary_,
                                    g_buf_blemish_r_, g_buf_blemish_gr_, g_buf_blemish_gb_, g_buf_blemish_b_, g_buf_blemish_raw_,
                                    w, h, manage.param_pixel_defect_.cnt_skip_, manage.param_pixel_defect_.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,
                                    manage.param_pixel_defect_.blemish_global_size_limit_,
                                    manage.param_pixel_defect_.blemish_bychannel_size_limit_,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,g_buf_tmp_,
                                    manage.param_pixel_defect_.save_image_, g_str_msg_all[site_]);


        g_str_msg_all[site_]+=boost::str(boost::format("Total Pixel Cluster Defect including global and bychannel Cnt:%d\n")
                               % total_cluster_cnt);
        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;
        cnt_2x2_diagonal_pix = cnt_2x2_diagonal_pix/2;

        int cnt_cluster_ch = /*cnt_1x2_ch+cnt_2x1_ch+*/
                cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch + other_cluster_ch;
        int cnt_cluster_global = cnt_1x2_2x1_pix + cnt_2x2_diagonal_pix;

        int cnt_1x2_2x1_ch_all = cnt_1x2_ch+cnt_2x1_ch;

        int cnts_cluster_2all = cnt_1x2_2x1_pix+cnt_2x2_diagonal_pix+cnt_1x2_ch+cnt_2x1_ch+cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch;
        int cnts_cluster_2more = cnt_2x4_pix + cnt_2x2_pix + other_cluster;

        int cnt_all_1x2_2x1_pix= cnt_1x2_ch+cnt_2x1_ch + cnt_1x2_2x1_pix ;                                        //全部1*2 2*1坏点
        int cnt_all_diagonal_2x2_pix= cnt_2x2_rightdiagonal_ch+cnt_2x2_leftdiagonal_ch + cnt_2x2_diagonal_pix ;   //全部diagonal 2*2坏点

        if (paticle>0)
        {
            res.bin = manage.param_pixel_defect_.bin3_;
            res.grade = manage.param_pixel_defect_.grade3_;
        }
        else if(other_cluster>0)
        {
            res.bin = manage.param_pixel_defect_.bin2_;
            res.grade = manage.param_pixel_defect_.grade2_;
        }
        else if (cnt_2x4_pix >0)
        {
            res.bin = manage.param_pixel_defect_.bin2x4_;
            res.grade  = manage.param_pixel_defect_.grade4_;
        }
        else if (cnt_2x2_pix >0)
        {
             res.bin  = manage.param_pixel_defect_.bin2x2_global_;
            res.grade  = manage.param_pixel_defect_.grade9_;
        }

        else if (cnt_cluster_ch>0)
        {
            // all defect clusters in channel are all 1x2 type
//            if ((cnt_1x2_ch+cnt_2x1_ch) != 0 &&
//                    cnt_cluster_ch == (cnt_1x2_ch+cnt_2x1_ch))
//            {
//                res.bin = manage.param_pixel_defect_.bin1x2_2x1_ch_;
//                 res.grade= manage.param_pixel_defect_.grade7_;
//            }
            // all defect clusters in channel are all 2x2 diagonal type
           /* else*/ if ((cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch) != 0 &&
                     cnt_cluster_ch == (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch))
            {
                res.bin  = manage.param_pixel_defect_.bin2x2_diagonal_ch_;
                res.grade= manage.param_pixel_defect_.grade8_;
            }

            // all defect clusters in channel are 1x2 type and 2x2 type
            else if ((cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch) != 0 &&
                     (cnt_1x2_ch+cnt_2x1_ch) != 0 &&
                     cnt_cluster_ch == (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch+cnt_1x2_ch+cnt_2x1_ch))
            {
                res.bin = manage.param_pixel_defect_.bin1_;
                res.grade = manage.param_pixel_defect_.grade1_;
            }
            else
            {
                res.bin = manage.param_pixel_defect_.bin3_;
                res.grade = manage.param_pixel_defect_.grade3_;
            }
        }

        else if (cnt_cluster_global > 0)
        {
            int grade_min = std::min(manage.param_pixel_defect_.grade1_,
                                     std::min(manage.param_pixel_defect_.grade2_,
                                              manage.param_pixel_defect_.grade3_));
            int bin_min = std::min(manage.param_pixel_defect_.bin1_,
                                   std::min(manage.param_pixel_defect_.bin2_,
                                            manage.param_pixel_defect_.bin3_));

            if (cnt_1x2_2x1_pix != 0 &&
                    cnt_cluster_global == cnt_1x2_2x1_pix)
            {
                if (cnt_cluster_ch == 0)
                {
                    res.bin = manage.param_pixel_defect_.bin1x2_2x1_;
                    res.grade = manage.param_pixel_defect_.grade5_;
                }
            }
            else if (cnt_2x2_diagonal_pix != 0 &&
                     cnt_cluster_global == cnt_2x2_diagonal_pix)
            {
                res.bin = manage.param_pixel_defect_.bin2x2_diagonal_;
                res.grade = manage.param_pixel_defect_.grade6_;
            }
            else
            {
                res.bin = bin_min;
                res.grade = grade_min;
            }
        }
        else if(cnt_1x2_2x1_ch_all>manage.param_pixel_defect_.all_1x2_2x1_spec)    //这里的双坏点写死 20
        {
            res.bin = manage.param_pixel_defect_.bin1x2_2x1_ch_;
             res.grade= manage.param_pixel_defect_.grade7_;
        }
        if (paticle ==0 &&
                other_cluster ==0 &&
                cnt_2x4_pix ==0 &&
                cnt_2x2_pix ==0 &&
                cnt_cluster_ch == 0 &&
                cnt_all_1x2_2x1_pix <= manage.param_pixel_defect_.all_1x2_2x1_spec &&
                cnt_all_diagonal_2x2_pix < 5)
        {
            res.bin = 2001;
            res.grade = 0;
            if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp1_ &&
                    cnt_white_pix<manage.param_pixel_defect_.limit_wp2_)
            {
                res.bin= 2002;
                res.grade = 1;
            }
            else if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp2_ &&
                     cnt_white_pix<manage.param_pixel_defect_.limit_wp3_)
            {
                res.bin = 2003;
                res.grade = 1;
            }
            else if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp3_)
            {
                res.bin= 2004;
                res.grade = 1;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp1_ &&
                     cnt_dark_pix< manage.param_pixel_defect_.limit_dp2_)
            {
                res.bin = 2006;
                res.grade = 1;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp2_ &&
                     cnt_dark_pix< manage.param_pixel_defect_.limit_dp3_)
            {
                res.bin= 2007;
                res.grade = 1;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp3_ )
            {
                res.bin = 2008;
                res.grade = 1;
            }


            // jundge
            if (cnt_white_pix > manage.param_pixel_defect_.single_pix_limit_ ||
                    cnt_cluster_global > manage.param_pixel_defect_.single_pix_limit_ )
            {
                res.bin = manage.param_pixel_defect_.bin_single_pix_;
                res.grade = manage.param_pixel_defect_.grade10_;
            }
            else if(blemish_cnt >0)
            {
                res.bin = manage.param_pixel_defect_.bin_blemish_;
                res.grade = manage.param_pixel_defect_.grade_blemish_;
            }
            else if(cnt_white_pix_blemish >manage.param_pixel_defect_.blemish_limit_wp3_ ||
                    cnt_dark_pix_blemish >manage.param_pixel_defect_.blemish_limit_dp3_)
            {
                res.bin = manage.param_pixel_defect_.bin_blemish_;
                res.grade = manage.param_pixel_defect_.grade_blemish_;
            }
        }


        int global_cluster_cnt =cnt_cluster_global + paticle + other_cluster + cnt_2x4_pix + cnt_2x2_pix;
        g_str_msg_all[site_]+=boost::str(boost::format("bychannel total clusters:%d\n"
                                                       "cnts of 1x2 or 2x1 bychannel:%d\n"
                                                       "cnts of 2x2 diagonal bychannel:%d\n"
                                                       "cnts of all 1x2 2x1 cluster:%d\n"
                                                       "cnts of all 2x2 cluster:%d\n")
                                         % cnt_cluster_ch % (cnt_1x2_ch+cnt_2x1_ch)
                                         % (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch)
                                         % cnt_all_1x2_2x1_pix % cnt_all_diagonal_2x2_pix);

        g_str_msg_all[site_]+=boost::str(boost::format("global total clusters:%d\n"
                                             "particle cnt:%d\n"
                                             "global 2x4 cluster cnt:%d\n"
                                             "global 2x2 cluster cnt:%d\n"
                                             "global 1x2_2x1_cluster cnt:%d\n"
                                             "global 2x2_diagonal_cluster cnt:%d\n"
                                             "cnt_white_pix:%d, cnt_dark_pix:%d\n")
                               % global_cluster_cnt
                               % paticle
                               % cnt_2x4_pix
                               % cnt_2x2_pix
                               % cnt_1x2_2x1_pix
                               % cnt_2x2_diagonal_pix
                               % cnt_white_pix % cnt_dark_pix);


//        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|")
//                               % cnt_cluster_ch % global_cluster_cnt % paticle % cnt_white_pix % cnt_dark_pix);
        if(type==Pixel_Defect_dark)
            g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|")
                                             % cnt_white_pix % cnt_dark_pix % cnt_all_1x2_2x1_pix % cnt_all_diagonal_2x2_pix);
            g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        for (int i=0; i<15; ++i)
        {
            if(i==1 || i==3 || i==5 || i==6)
                cnts_cluster[i] = cnts_cluster.value(i)/2;
            //            g_str_log_all[site_]+=boost::str(boost::format("%d|")
            //                                   % cnts_cluster.value(i));
        }


        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }



    return res;
}

Subitem_Result AlgorithmItems::Pixel_Defect_Test_V1_3( ParamManage& manage,Pixel_Defect_type type)
{

    Subitem_Result res("Pixel_Defect_V1.3",false,
                       0,
                       0);

    if(manage.param_pixel_defect_.type_ == 0)
        type = Pixel_Defect_dark;
    else if(manage.param_pixel_defect_.type_ == 1)
        type = Pixel_Defect_bright;

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_pixel_defect_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Pixel Defect Test V1.3-----------------------------\n"));

        int cnt_white_pix = 0;
        int cnt_dark_pix = 0;

        int cnt_1x2_ch = 0;
        int cnt_2x1_ch = 0;
        int cnt_2x2_leftdiagonal_ch = 0;
        int cnt_2x2_rightdiagonal_ch = 0;

        int cnt_2x4_pix = 0;
        int cnt_2x2_pix = 0;
        int cnt_1x2_2x1_pix = 0;
        int cnt_2x2_diagonal_pix = 0;

        int paticle =0;
        int other_cluster =0;
        int other_cluster_ch =0;

        int total_cluster_cnt = 0;
        QMap<int,int> cnts_cluster;

        int blemish_cnt =0;
        int cnt_white_pix_blemish = 0;
        int cnt_dark_pix_blemish = 0;

        double diff_spec_r_= 0;
        double diff_spec_gr_= 0;
        double diff_spec_gb_ = 0;
        double diff_spec_b_= 0;

        double diff_blemish_spec_r_= 0;
        double diff_blemish_spec_gr_= 0;
        double diff_blemish_spec_gb_ = 0;
        double diff_blemish_spec_b_= 0;

        Spec_Mode spec_mode_=Ratio;

        if(type==Pixel_Defect_bright)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_bright \n"));

            diff_spec_r_= manage.param_pixel_defect_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.ratio_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_spec_r_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_spec_gr_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_spec_gb_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_spec_b_;
        }
        else if(type==Pixel_Defect_dark)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_dark \n"));

            diff_spec_r_= manage.param_pixel_defect_.DN_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.DN_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.DN_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.DN_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_dn_spec_;
            spec_mode_=DN;
        }

        TestItem::find_pixel_defect(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,
                                    g_buf_delta_r_, g_buf_delta_gr_, g_buf_delta_gb_, g_buf_delta_b_, g_buf_delta_raw_,
                                    g_binary_r_, g_binary_gr_, g_binary_gb_, g_binary_b_, g_binary_,
                                    g_buf_blemish_r_, g_buf_blemish_gr_, g_buf_blemish_gb_, g_buf_blemish_b_, g_buf_blemish_raw_,
                                    w, h, manage.param_pixel_defect_.cnt_skip_, manage.param_pixel_defect_.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,
                                    manage.param_pixel_defect_.blemish_global_size_limit_,
                                    manage.param_pixel_defect_.blemish_bychannel_size_limit_,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,g_buf_tmp_,
                                    manage.param_pixel_defect_.save_image_, g_str_msg_all[site_]);

        g_str_msg_all[site_]+=boost::str(boost::format("Total Pixel Cluster Defect including global and bychannel Cnt:%d\n")
                               % total_cluster_cnt);
        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;
        cnt_2x2_diagonal_pix = cnt_2x2_diagonal_pix/2;

        int cnt_cluster_ch = cnt_1x2_ch+cnt_2x1_ch+
                cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch + other_cluster_ch;
        int cnt_cluster_global = cnt_1x2_2x1_pix + cnt_2x2_diagonal_pix;

        int cnts_cluster_2all = cnt_1x2_2x1_pix+cnt_2x2_diagonal_pix+cnt_1x2_ch+cnt_2x1_ch+cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch;


        int cnt_all_1x2_2x1_pix= cnt_1x2_ch+cnt_2x1_ch + cnt_1x2_2x1_pix ;                                        //全部1*2 2*1坏点
        int cnt_all_diagonal_2x2_pix= cnt_2x2_rightdiagonal_ch+cnt_2x2_leftdiagonal_ch + cnt_2x2_diagonal_pix ;   //全部diagonal 2*2坏点

        if (paticle>0)
        {
            res.bin = manage.param_pixel_defect_.bin3_;
            res.grade = manage.param_pixel_defect_.grade3_;
        }
        else if(other_cluster>0)
        {
            res.bin = manage.param_pixel_defect_.bin2_;
            res.grade = manage.param_pixel_defect_.grade2_;
        }
        else if (cnt_2x4_pix >0)
        {
            res.bin = manage.param_pixel_defect_.bin2x4_;
            res.grade  = manage.param_pixel_defect_.grade4_;
        }
        else if (cnt_2x2_pix >0)
        {
             res.bin  = manage.param_pixel_defect_.bin2x2_global_;
            res.grade  = manage.param_pixel_defect_.grade9_;
        }

        else if (cnt_cluster_ch>0)
        {
            // all defect clusters in channel are all 1x2 type
            if ((cnt_1x2_ch+cnt_2x1_ch) != 0 &&
                    cnt_cluster_ch == (cnt_1x2_ch+cnt_2x1_ch))
            {
                res.bin = manage.param_pixel_defect_.bin1x2_2x1_ch_;
                 res.grade= manage.param_pixel_defect_.grade7_;
            }
            // all defect clusters in channel are all 2x2 diagonal type
            else if ((cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch) != 0 &&
                     cnt_cluster_ch == (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch))
            {
                res.bin  = manage.param_pixel_defect_.bin2x2_diagonal_ch_;
                res.grade= manage.param_pixel_defect_.grade8_;
            }

            // all defect clusters in channel are 1x2 type and 2x2 type
            else if ((cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch) != 0 &&
                     (cnt_1x2_ch+cnt_2x1_ch) != 0 &&
                     cnt_cluster_ch == (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch+cnt_1x2_ch+cnt_2x1_ch))
            {
                res.bin = manage.param_pixel_defect_.bin1_;
                res.grade = manage.param_pixel_defect_.grade1_;
            }
            else
            {
                res.bin = manage.param_pixel_defect_.bin3_;
                res.grade = manage.param_pixel_defect_.grade3_;
            }
        }

        else if (cnt_cluster_global > 0)
        {
            int grade_min = std::min(manage.param_pixel_defect_.grade1_,
                                     std::min(manage.param_pixel_defect_.grade2_,
                                              manage.param_pixel_defect_.grade3_));
            int bin_min = std::min(manage.param_pixel_defect_.bin1_,
                                   std::min(manage.param_pixel_defect_.bin2_,
                                            manage.param_pixel_defect_.bin3_));

            if (cnt_1x2_2x1_pix != 0 &&
                    cnt_cluster_global == cnt_1x2_2x1_pix)
            {
                if (cnt_cluster_ch == 0)
                {
                    res.bin = manage.param_pixel_defect_.bin1x2_2x1_;
                    res.grade = manage.param_pixel_defect_.grade5_;
                }
            }
            else if (cnt_2x2_diagonal_pix != 0 &&
                     cnt_cluster_global == cnt_2x2_diagonal_pix)
            {
                res.bin = manage.param_pixel_defect_.bin2x2_diagonal_;
                res.grade = manage.param_pixel_defect_.grade6_;
            }
            else
            {
                res.bin = bin_min;
                res.grade = grade_min;
            }
        }

        if (paticle ==0 &&
                other_cluster ==0 &&
                cnt_2x4_pix ==0 &&
                cnt_2x2_pix ==0 &&
                cnt_cluster_ch == 0 &&
                cnt_all_1x2_2x1_pix <= manage.param_pixel_defect_.all_1x2_2x1_spec &&
                cnt_all_diagonal_2x2_pix < 5)
        {

            Subitem_Result res_singe_wp("single_pix_WP_",false,0,0);
            Subitem_Result res_singe_dp("single_pix_DP_",false,0,0);

            if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp1_ &&
                    cnt_white_pix<manage.param_pixel_defect_.limit_wp2_)
            {
                res_singe_wp.bin =manage.param_pixel_defect_.bin_limit_wp1_ ;
                res_singe_wp.grade = manage.param_pixel_defect_.grade_limit_wp1_;
            }
            else if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp2_ &&
                     cnt_white_pix<manage.param_pixel_defect_.limit_wp3_)
            {
                res_singe_wp.bin =manage.param_pixel_defect_.bin_limit_wp2_ ;
                res_singe_wp.grade = manage.param_pixel_defect_.grade_limit_wp2_;
            }
            else if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp3_)
            {
                res_singe_wp.bin =manage.param_pixel_defect_.bin_limit_wp3_ ;
                res_singe_wp.grade = manage.param_pixel_defect_.grade_limit_wp3_;
            }

             if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp1_ &&
                     cnt_dark_pix< manage.param_pixel_defect_.limit_dp2_)
            {
                res_singe_dp.bin = manage.param_pixel_defect_.bin_limit_dp1_ ;
                res_singe_dp.grade = manage.param_pixel_defect_.grade_limit_dp1_;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp2_ &&
                     cnt_dark_pix< manage.param_pixel_defect_.limit_dp3_)
            {
                 res_singe_dp.bin = manage.param_pixel_defect_.bin_limit_dp2_ ;
                 res_singe_dp.grade = manage.param_pixel_defect_.grade_limit_dp2_;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp3_ )
            {
                 res_singe_dp.bin = manage.param_pixel_defect_.bin_limit_dp3_ ;
                 res_singe_dp.grade = manage.param_pixel_defect_.grade_limit_dp3_;
            }

             if(res_singe_dp.grade > res_singe_wp.grade)
             {
                 res.bin = res_singe_dp.bin;
                 res.grade = res_singe_dp.grade;
             }
             else
             {
                 res.bin = res_singe_wp.bin;
                 res.grade = res_singe_wp.grade;
             }



            // jundge
            if ((cnt_white_pix+ cnt_dark_pix)> manage.param_pixel_defect_.single_pix_limit_ ||
                    cnt_cluster_global > manage.param_pixel_defect_.single_pix_limit_ )
            {
                res.bin = manage.param_pixel_defect_.bin_single_pix_;
                res.grade = manage.param_pixel_defect_.grade10_;
            }
            else if(blemish_cnt >0)
            {
                res.bin = manage.param_pixel_defect_.bin_blemish_;
                res.grade = manage.param_pixel_defect_.grade_blemish_;
            }
            else if(cnt_white_pix_blemish >manage.param_pixel_defect_.blemish_limit_wp3_ ||
                    cnt_dark_pix_blemish >manage.param_pixel_defect_.blemish_limit_dp3_)
            {
                res.bin = manage.param_pixel_defect_.bin_blemish_;
                res.grade = manage.param_pixel_defect_.grade_blemish_;
            }
        }


        int global_cluster_cnt =cnt_cluster_global + paticle + other_cluster + cnt_2x4_pix + cnt_2x2_pix;
        g_str_msg_all[site_]+=boost::str(boost::format("bychannel total clusters:%d\n"
                                                       "cnts of 1x2 or 2x1 bychannel:%d\n"
                                                       "cnts of 2x2 diagonal bychannel:%d\n"
                                                       "cnts of all 1x2 2x1 cluster:%d\n"
                                                       "cnts of all 2x2 cluster:%d\n")
                                         % cnt_cluster_ch % (cnt_1x2_ch+cnt_2x1_ch)
                                         % (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch)
                                         % cnt_all_1x2_2x1_pix % cnt_all_diagonal_2x2_pix);

        g_str_msg_all[site_]+=boost::str(boost::format("global total clusters:%d\n"
                                             "particle cnt:%d\n"
                                             "global 2x4 cluster cnt:%d\n"
                                             "global 2x2 cluster cnt:%d\n"
                                             "global 1x2_2x1_cluster cnt:%d\n"
                                             "global 2x2_diagonal_cluster cnt:%d\n"
                                             "cnt_white_pix:%d, cnt_dark_pix:%d\n")
                               % global_cluster_cnt
                               % paticle
                               % cnt_2x4_pix
                               % cnt_2x2_pix
                               % cnt_1x2_2x1_pix
                               % cnt_2x2_diagonal_pix
                               % cnt_white_pix % cnt_dark_pix);


        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|%d|")
                                         % cnt_cluster_ch % global_cluster_cnt % paticle % cnt_white_pix % cnt_dark_pix % cnt_all_1x2_2x1_pix % cnt_all_diagonal_2x2_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());



        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }



    return res;
}
Subitem_Result AlgorithmItems::Pixel_Defect_Test_V1_4( ParamManage& manage,Pixel_Defect_type type)
{

//    Subitem_Result res("Pixel_Defect",false,
//                       0,
//                       0);
        Subitem_Result res("Pixel_Defect_V1.4",true,
                           1,
                           0);

    if(manage.param_pixel_defect_.type_ == 0)
        type = Pixel_Defect_dark;
    else if(manage.param_pixel_defect_.type_ == 1)
        type = Pixel_Defect_bright;

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_pixel_defect_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Pixel Defect Test V1.4-----------------------------\n"));

        int cnt_white_pix = 0;
        int cnt_dark_pix = 0;

        int cnt_1x2_ch = 0;
        int cnt_2x1_ch = 0;
        int cnt_2x2_leftdiagonal_ch = 0;
        int cnt_2x2_rightdiagonal_ch = 0;

        int cnt_2x4_pix = 0;
        int cnt_2x2_pix = 0;
        int cnt_1x2_2x1_pix = 0;
        int cnt_2x2_diagonal_pix = 0;

        int paticle =0;
        int other_cluster =0;
        int other_cluster_ch =0;

        int total_cluster_cnt = 0;
        QMap<int,int> cnts_cluster;

        int blemish_cnt =0;
        int cnt_white_pix_blemish = 0;
        int cnt_dark_pix_blemish = 0;

        double diff_spec_r_= 0;
        double diff_spec_gr_= 0;
        double diff_spec_gb_ = 0;
        double diff_spec_b_= 0;

        double diff_blemish_spec_r_= 0;
        double diff_blemish_spec_gr_= 0;
        double diff_blemish_spec_gb_ = 0;
        double diff_blemish_spec_b_= 0;

        Spec_Mode spec_mode_=Ratio;

        if(type==Pixel_Defect_bright)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_bright \n"));

            diff_spec_r_= manage.param_pixel_defect_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.ratio_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_spec_r_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_spec_gr_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_spec_gb_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_spec_b_;
        }
        else if(type==Pixel_Defect_dark)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("type==Pixel_Defect_dark \n"));

            diff_spec_r_= manage.param_pixel_defect_.DN_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_.DN_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_.DN_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_.DN_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_.blemish_dn_spec_;
            diff_blemish_spec_b_= manage.param_pixel_defect_.blemish_dn_spec_;
            spec_mode_=DN;
        }

        TestItem::find_pixel_defect(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,
                                    g_buf_delta_r_, g_buf_delta_gr_, g_buf_delta_gb_, g_buf_delta_b_, g_buf_delta_raw_,
                                    g_binary_r_, g_binary_gr_, g_binary_gb_, g_binary_b_, g_binary_,
                                    g_buf_blemish_r_, g_buf_blemish_gr_, g_buf_blemish_gb_, g_buf_blemish_b_, g_buf_blemish_raw_,
                                    w, h, manage.param_pixel_defect_.cnt_skip_, manage.param_pixel_defect_.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,
                                    manage.param_pixel_defect_.blemish_global_size_limit_,
                                    manage.param_pixel_defect_.blemish_bychannel_size_limit_,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,g_buf_tmp_,
                                    manage.param_pixel_defect_.save_image_, g_str_msg_all[site_]);


        g_str_msg_all[site_]+=boost::str(boost::format("Total Pixel Cluster Defect including global and bychannel Cnt:%d\n")
                                         % total_cluster_cnt);
        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;
        cnt_2x2_diagonal_pix = cnt_2x2_diagonal_pix/2;

        int cnt_cluster_ch = /*cnt_1x2_ch+cnt_2x1_ch+*/
                cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch + other_cluster_ch;
        int cnt_cluster_global = cnt_1x2_2x1_pix + cnt_2x2_diagonal_pix;

        int cnt_1x2_2x1_ch_all = cnt_1x2_ch+cnt_2x1_ch;

        int cnts_cluster_2all = cnt_1x2_2x1_pix+cnt_2x2_diagonal_pix+cnt_1x2_ch+cnt_2x1_ch+cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch;
        int cnts_cluster_2more = cnt_2x4_pix + cnt_2x2_pix + other_cluster;

        int cnt_all_1x2_2x1_pix= cnt_1x2_ch+cnt_2x1_ch + cnt_1x2_2x1_pix ;                                        //全部1*2 2*1坏点
        int cnt_all_diagonal_2x2_pix= cnt_2x2_rightdiagonal_ch+cnt_2x2_leftdiagonal_ch + cnt_2x2_diagonal_pix ;   //全部diagonal 2*2坏点

        if (paticle>0)
        {
            res.bin = manage.param_pixel_defect_.bin3_;
            res.grade = manage.param_pixel_defect_.grade3_;
            res.is_passed = false;
        }
        else if(other_cluster>0)
        {
            res.bin = manage.param_pixel_defect_.bin2_;
            res.grade = manage.param_pixel_defect_.grade2_;
            res.is_passed = false;
        }
        else if (cnt_2x4_pix >0)
        {
            res.bin = manage.param_pixel_defect_.bin2x4_;
            res.grade  = manage.param_pixel_defect_.grade4_;
            res.is_passed = false;
        }
        else if (cnt_2x2_pix >0)
        {
            res.bin  = manage.param_pixel_defect_.bin2x2_global_;
            res.grade  = manage.param_pixel_defect_.grade9_;
            res.is_passed = false;
        }

        else if (cnt_cluster_ch>0)
        {
            // all defect clusters in channel are all 1x2 type
            //            if ((cnt_1x2_ch+cnt_2x1_ch) != 0 &&
            //                    cnt_cluster_ch == (cnt_1x2_ch+cnt_2x1_ch))
            //            {
            //                res.bin = manage.param_pixel_defect_.bin1x2_2x1_ch_;
            //                 res.grade= manage.param_pixel_defect_.grade7_;
            //            }
            // all defect clusters in channel are all 2x2 diagonal type
            /* else*/ if ((cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch) != 0 &&
                          cnt_cluster_ch == (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch))
            {
                res.bin  = manage.param_pixel_defect_.bin2x2_diagonal_ch_;
                res.grade= manage.param_pixel_defect_.grade8_;
            }

            // all defect clusters in channel are 1x2 type and 2x2 type
            else if ((cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch) != 0 &&
                     (cnt_1x2_ch+cnt_2x1_ch) != 0 &&
                     cnt_cluster_ch == (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch+cnt_1x2_ch+cnt_2x1_ch))
            {
                res.bin = manage.param_pixel_defect_.bin1_;
                res.grade = manage.param_pixel_defect_.grade1_;
            }
            else
            {
                res.bin = manage.param_pixel_defect_.bin3_;
                res.grade = manage.param_pixel_defect_.grade3_;
            }
            res.is_passed = false;
        }

        else if (cnt_cluster_global > 0)
        {
            int grade_min = std::min(manage.param_pixel_defect_.grade1_,
                                     std::min(manage.param_pixel_defect_.grade2_,
                                              manage.param_pixel_defect_.grade3_));
            int bin_min = std::min(manage.param_pixel_defect_.bin1_,
                                   std::min(manage.param_pixel_defect_.bin2_,
                                            manage.param_pixel_defect_.bin3_));

            if (cnt_1x2_2x1_pix != 0 &&
                    cnt_cluster_global == cnt_1x2_2x1_pix)
            {
                if (cnt_cluster_ch == 0)
                {
                    res.bin = manage.param_pixel_defect_.bin1x2_2x1_;
                    res.grade = manage.param_pixel_defect_.grade5_;
                    res.is_passed = false;
                }
            }
            else if (cnt_2x2_diagonal_pix != 0 &&
                     cnt_cluster_global == cnt_2x2_diagonal_pix)
            {
                res.bin = manage.param_pixel_defect_.bin2x2_diagonal_;
                res.grade = manage.param_pixel_defect_.grade6_;
                res.is_passed = false;
            }
            else
            {
                res.bin = bin_min;
                res.grade = grade_min;
                res.is_passed = false;
            }
        }
        else if(cnt_1x2_2x1_ch_all>manage.param_pixel_defect_.all_1x2_2x1_spec)    //这里的双坏点写死 20
        {
            res.bin = manage.param_pixel_defect_.bin1x2_2x1_ch_;
            res.grade= manage.param_pixel_defect_.grade7_;
            res.is_passed = false;
        }
        if (paticle ==0 &&
                other_cluster ==0 &&
                cnt_2x4_pix ==0 &&
                cnt_2x2_pix ==0 &&
                cnt_cluster_ch == 0 &&
                cnt_all_1x2_2x1_pix <= manage.param_pixel_defect_.all_1x2_2x1_spec &&
                cnt_all_diagonal_2x2_pix < 5)
        {


            Subitem_Result res_singe_wp("single_pix_WP_",true,1,0);
            Subitem_Result res_singe_dp("single_pix_DP_",true,1,0);

            if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp1_ &&
                    cnt_white_pix<manage.param_pixel_defect_.limit_wp2_)
            {
                res_singe_wp.bin =manage.param_pixel_defect_.bin_limit_wp1_ ;
                res_singe_wp.grade = manage.param_pixel_defect_.grade_limit_wp1_;
                res_singe_wp.is_passed = false;
            }
            else if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp2_ &&
                     cnt_white_pix<manage.param_pixel_defect_.limit_wp3_)
            {
                res_singe_wp.bin =manage.param_pixel_defect_.bin_limit_wp2_ ;
                res_singe_wp.grade = manage.param_pixel_defect_.grade_limit_wp2_;
                res_singe_wp.is_passed = false;
            }
            else if (cnt_white_pix>=manage.param_pixel_defect_.limit_wp3_)
            {
                res_singe_wp.bin =manage.param_pixel_defect_.bin_limit_wp3_ ;
                res_singe_wp.grade = manage.param_pixel_defect_.grade_limit_wp3_;
                res_singe_wp.is_passed = false;
            }

            if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp1_ &&
                    cnt_dark_pix< manage.param_pixel_defect_.limit_dp2_)
            {
                res_singe_dp.bin = manage.param_pixel_defect_.bin_limit_dp1_ ;
                res_singe_dp.grade = manage.param_pixel_defect_.grade_limit_dp1_;
                res_singe_dp.is_passed = false;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp2_ &&
                     cnt_dark_pix< manage.param_pixel_defect_.limit_dp3_)
            {
                res_singe_dp.bin = manage.param_pixel_defect_.bin_limit_dp2_ ;
                res_singe_dp.grade = manage.param_pixel_defect_.grade_limit_dp2_;
                res_singe_dp.is_passed = false;
            }
            else if (cnt_dark_pix>=manage.param_pixel_defect_.limit_dp3_ )
            {
                res_singe_dp.bin = manage.param_pixel_defect_.bin_limit_dp3_ ;
                res_singe_dp.grade = manage.param_pixel_defect_.grade_limit_dp3_;
                res_singe_dp.is_passed = false;
            }

            if(res_singe_dp.grade > res_singe_wp.grade)
            {
                res.bin = res_singe_dp.bin;
                res.grade = res_singe_dp.grade;
                res.is_passed = res_singe_dp.is_passed;
            }
            else
            {
                res.bin = res_singe_wp.bin;
                res.grade = res_singe_wp.grade;
                res.is_passed = res_singe_wp.is_passed;
            }
            // jundge
            //            if (cnt_white_pix > manage.param_pixel_defect_.single_pix_limit_ ||
            //                    cnt_cluster_global > manage.param_pixel_defect_.single_pix_limit_ )
            //            {
            //                res.bin = manage.param_pixel_defect_.bin_single_pix_;
            //                res.grade = manage.param_pixel_defect_.grade10_;
            //            }

            //通过外配来卡黑点、白点还是白点na+黑点
            int pixel_count_to_check = 0;
            if(manage.param_pixel_defect_.single_pix_type_ == 0)          // cnt_dark_pix
            {
                pixel_count_to_check = cnt_dark_pix;
                g_str_msg_all[site_]+=boost::str(boost::format("cnt_dark_pix check\n"));
            }
            else if(manage.param_pixel_defect_.single_pix_type_ == 1)     // cnt_white_pix
            {
                pixel_count_to_check = cnt_white_pix;
                g_str_msg_all[site_]+=boost::str(boost::format("cnt_white_pix check\n"));
            }
            else if(manage.param_pixel_defect_.single_pix_type_ == 2)     // cnt_white_pix + cnt_dark_pix
            {
                pixel_count_to_check = cnt_white_pix + cnt_dark_pix;
                g_str_msg_all[site_]+=boost::str(boost::format("cnt_white_pix + cnt_dark_pix check\n"));
            }


            if ((pixel_count_to_check) > manage.param_pixel_defect_.single_pix_limit_)
            {
                res.bin = manage.param_pixel_defect_.bin_single_pix_;
                res.grade = manage.param_pixel_defect_.grade10_;
                res.is_passed = false;
            }

            else if(blemish_cnt >0)
            {
                res.bin = manage.param_pixel_defect_.bin_blemish_;
                res.grade = manage.param_pixel_defect_.grade_blemish_;
                res.is_passed = false;
            }
            else if(cnt_white_pix_blemish >manage.param_pixel_defect_.blemish_limit_wp3_ ||
                    cnt_dark_pix_blemish >manage.param_pixel_defect_.blemish_limit_dp3_)
            {
                res.bin = manage.param_pixel_defect_.bin_blemish_;
                res.grade = manage.param_pixel_defect_.grade_blemish_;
                res.is_passed = false;
            }
        }


        int global_cluster_cnt =cnt_cluster_global + paticle + other_cluster + cnt_2x4_pix + cnt_2x2_pix;
        g_str_msg_all[site_]+=boost::str(boost::format("bychannel total clusters:%d\n"
                                                       "cnts of 1x2 or 2x1 bychannel:%d\n"
                                                       "cnts of 2x2 diagonal bychannel:%d\n"
                                                       "cnts of all 1x2 2x1 cluster:%d\n"
                                                       "cnts of all 2x2 cluster:%d\n")
                                         % cnt_cluster_ch % (cnt_1x2_ch+cnt_2x1_ch)
                                         % (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch)
                                         % cnt_all_1x2_2x1_pix % cnt_all_diagonal_2x2_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|")

                                         %  cnt_cluster_ch
                                         % (cnt_1x2_ch+cnt_2x1_ch)
                                         % (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch)
                                         % cnt_all_1x2_2x1_pix
                                         % cnt_all_diagonal_2x2_pix);


        g_str_msg_all[site_]+=boost::str(boost::format("global total clusters:%d\n"
                                                       "particle cnt:%d\n"
                                                       "global 2x4 cluster cnt:%d\n"
                                                       "global 2x2 cluster cnt:%d\n"
                                                       "global 1x2_2x1_cluster cnt:%d\n"
                                                       "global 2x2_diagonal_cluster cnt:%d\n"
                                                       "cnt_white_pix:%d, cnt_dark_pix:%d\n")
                                         % global_cluster_cnt
                                         % paticle
                                         % cnt_2x4_pix
                                         % cnt_2x2_pix
                                         % cnt_1x2_2x1_pix
                                         % cnt_2x2_diagonal_pix
                                         % cnt_white_pix % cnt_dark_pix);


        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|%d|%d|")
                                         % global_cluster_cnt
                                         % paticle
                                         % cnt_2x4_pix
                                         % cnt_2x2_pix
                                         % cnt_1x2_2x1_pix
                                         % cnt_2x2_diagonal_pix
                                         % cnt_white_pix % cnt_dark_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());


        //        for (int i=0; i<15; ++i)
        //        {
        //            if(i==1 || i==3 || i==5 || i==6)
        //                cnts_cluster[i] = cnts_cluster.value(i)/2;

        //        }


        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        res.g_msg = false;
    }



    return res;
}







//死线  (白屏、Large_Gain、Range)
Subitem_Result  AlgorithmItems::Deadrow_Deadcol_bright(const ParamManage& manage,bool log_flag)
{
    if(manage.deadrow_deadcol_bright_version == "deadrow_deadcol_bright_V1.0")
        return Deadrow_Deadcol_bright_V1_0(manage,log_flag);
    if(manage.deadrow_deadcol_bright_version == "deadrow_deadcol_bright_V1.1")
        return Deadrow_Deadcol_bright_V1_1(manage,log_flag);


    Subitem_Result res("Deadrow_Deadcol", false);
    res.g_msg = false;
    return res;
}


Subitem_Result  AlgorithmItems::Deadrow_Deadcol_bright_V1_0(const ParamManage& manage,bool log_flag)
{


    Subitem_Result result_line("Deadrow_Deadcol_Bright",true);

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_line_defect_bright_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadrow and Deadcol Bright V1.0-----------------------------\n"));

        double max_row_diff[4]={0};
        int max_row_pos[4]={0};
        double row_max_min_diff[4]={0};

        int cnt_dead_row=0;
        int cnt_row_dol =0;
        double row_dol_std_max[4]={0};
        int row_dol_std_max_pos[4]={0};

        int cnt_breakline_row =0;
        double breakline_row_max_value[4]={0};
        int breakline_row_max_pos[4]={0};

        TestItem::test_row_defect_bychannel(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,w, h,
                                            manage.param_line_defect_bright_.strip_row_,
                                            manage.param_line_defect_bright_.r_area_,
                                            manage.param_line_defect_bright_.spec_row_dol_r_,
                                            manage.param_line_defect_bright_.spec_row_dol_gr_,
                                            manage.param_line_defect_bright_.spec_row_dol_gb_,
                                            manage.param_line_defect_bright_.spec_row_dol_b_,
                                            manage.param_line_defect_bright_.deadrow_spec_r_TA_,
                                            manage.param_line_defect_bright_.deadrow_spec_gr_TA_,
                                            manage.param_line_defect_bright_.deadrow_spec_gb_TA_,
                                            manage.param_line_defect_bright_.deadrow_spec_b_TA_,
                                            max_row_diff, max_row_pos, row_max_min_diff, cnt_dead_row,
                                            cnt_row_dol, row_dol_std_max, row_dol_std_max_pos,
                                            manage.param_line_defect_bright_.breakrow_spec_r_,
                                            manage.param_line_defect_bright_.breakrow_spec_gr_,
                                            manage.param_line_defect_bright_.breakrow_spec_gb_,
                                            manage.param_line_defect_bright_.breakrow_spec_b_,
                                            cnt_breakline_row, breakline_row_max_value, breakline_row_max_pos,
                                            Ratio, g_str_msg_all[site_], manage.param_line_defect_bright_.log_print_);


        double max_col_diff[4]={0};
        int max_col_pos[4]={0};
        double col_max_min_diff[4]={0};

        int cnt_dead_col=0;
        int cnt_col_dol =0;
        double col_dol_std_max[4]={0};
        int col_dol_std_max_pos[4]={0};

        int cnt_breakline_col =0;
        double breakline_col_max_value[4]={0};
        int breakline_col_max_pos[4]={0};


        TestItem::test_col_defect_bychannel(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,w, h,
                                            manage.param_line_defect_bright_.strip_col_,
                                            manage.param_line_defect_bright_.r_area_,
                                            manage.param_line_defect_bright_.spec_col_dol_r_,
                                            manage.param_line_defect_bright_.spec_col_dol_gr_,
                                            manage.param_line_defect_bright_.spec_col_dol_gb_,
                                            manage.param_line_defect_bright_.spec_col_dol_b_,
                                            manage.param_line_defect_bright_.deadcol_spec_r_TA_,
                                            manage.param_line_defect_bright_.deadcol_spec_gr_TA_,
                                            manage.param_line_defect_bright_.deadcol_spec_gb_TA_,
                                            manage.param_line_defect_bright_.deadcol_spec_b_TA_,
                                            max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                            cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                            manage.param_line_defect_bright_.breakcol_spec_r_,
                                            manage.param_line_defect_bright_.breakcol_spec_gr_,
                                            manage.param_line_defect_bright_.breakcol_spec_gb_,
                                            manage.param_line_defect_bright_.breakcol_spec_b_,
                                            cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                            Ratio, g_str_msg_all[site_], manage.param_line_defect_bright_.log_print_);

//        start=clock();
//        cost_time_old=start-end;

        if(log_flag)
        {
            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
                                             % max_row_diff[0] % max_row_diff[1] % max_row_diff[2] % max_row_diff[3]
                    % max_row_pos[0] % max_row_pos[1] % max_row_pos[2] % max_row_pos[3]
                    % row_max_min_diff[0] % row_max_min_diff[1] % row_max_min_diff[2] % row_max_min_diff[3]);

            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                             % row_dol_std_max[0] % row_dol_std_max[1] % row_dol_std_max[2] % row_dol_std_max[3]
                    % row_dol_std_max_pos[0] % row_dol_std_max_pos[1] % row_dol_std_max_pos[2] % row_dol_std_max_pos[3]);

            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                             % breakline_row_max_value[0] % breakline_row_max_value[1] % breakline_row_max_value[2] % breakline_row_max_value[3]
                    % breakline_row_max_pos[0] % breakline_row_max_pos[1] % breakline_row_max_pos[2] % breakline_row_max_pos[3]);


            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
                                             % max_col_diff[0] % max_col_diff[1] % max_col_diff[2] % max_col_diff[3]
                    % max_col_pos[0] % max_col_pos[1] % max_col_pos[2] % max_col_pos[3]
                    % col_max_min_diff[0] % col_max_min_diff[1] % col_max_min_diff[2] % col_max_min_diff[3]);

            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                             % col_dol_std_max[0] % col_dol_std_max[1] % col_dol_std_max[2] % col_dol_std_max[3]
                    % col_dol_std_max_pos[0] % col_dol_std_max_pos[1] % col_dol_std_max_pos[2] % col_dol_std_max_pos[3]);

            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                             % breakline_col_max_value[0] % breakline_col_max_value[1] % breakline_col_max_value[2] % breakline_col_max_value[3]
                    % breakline_col_max_pos[0] % breakline_col_max_pos[1] % breakline_col_max_pos[2] % breakline_col_max_pos[3]);


            g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|")
                                             % cnt_dead_row % cnt_dead_col %cnt_row_dol %cnt_col_dol %cnt_breakline_row %cnt_breakline_col);
            g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_line.name.c_str());

        }

        logMsg (std::string("after dead line"));

        int cnt_dol = cnt_row_dol + cnt_col_dol;
        int cnt_break_line = cnt_breakline_row + cnt_breakline_col;

        if (cnt_dead_col>0 || cnt_dead_row>0 || cnt_dol>0 || cnt_break_line>0)
            result_line.is_passed = false;

        if (cnt_dead_col>0)
        {
            if(max_col_diff[0]>=manage.param_line_defect_bright_.deadcol_spec_r_MA_ ||
                    max_col_diff[1]>=manage.param_line_defect_bright_.deadcol_spec_gr_MA_ ||
                    max_col_diff[2]>=manage.param_line_defect_bright_.deadcol_spec_gb_MA_ ||
                    max_col_diff[3]>=manage.param_line_defect_bright_.deadcol_spec_b_MA_ )
            {
                result_line.bin = manage.param_line_defect_bright_.bin_dead_col_;
                result_line.grade = manage.param_line_defect_bright_.grade_dead_col_;
            }
            else
            {
                result_line.bin = manage.param_line_defect_bright_.bin_dead_col_MA_;
                result_line.grade = manage.param_line_defect_bright_.grade_dead_col_MA;
            }
        }
        if (cnt_dead_row>0 && manage.param_line_defect_bright_.grade_dead_row_>result_line.grade)
        {
            if(max_row_diff[0]>=manage.param_line_defect_bright_.deadrow_spec_r_MA_ ||
                    max_row_diff[1]>=manage.param_line_defect_bright_.deadrow_spec_gr_MA_ ||
                    max_row_diff[2]>=manage.param_line_defect_bright_.deadrow_spec_gb_MA_ ||
                    max_row_diff[3]>=manage.param_line_defect_bright_.deadrow_spec_b_MA_ )
            {
                result_line.bin = manage.param_line_defect_bright_.bin_dead_row_;
                result_line.grade = manage.param_line_defect_bright_.grade_dead_row_;
            }
            else
            {
                result_line.bin = manage.param_line_defect_bright_.bin_dead_row_MA_;
                result_line.grade = manage.param_line_defect_bright_.grade_dead_row_MA;
            }
        }
        if (cnt_dol>0 && manage.param_line_defect_bright_.grade_dol_>result_line.grade)
        {
            result_line.bin = manage.param_line_defect_bright_.bin_dol_;
            result_line.grade = manage.param_line_defect_bright_.grade_dol_;
        }
        if (cnt_break_line>0 && manage.param_line_defect_bright_.grade_break_line_>result_line.grade)
        {
            result_line.bin = manage.param_line_defect_bright_.bin_break_line_;
            result_line.grade = manage.param_line_defect_bright_.grade_break_line_;
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        result_line.g_msg = false;
    }


    return result_line;
}


Subitem_Result  AlgorithmItems::Deadrow_Deadcol_bright_V1_1(const ParamManage& manage,bool log_flag)
{


    Subitem_Result result_line("Deadrow_Deadcol_Bright_V1.1",true);

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_line_defect_bright_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadrow and Deadcol Bright V1.1-----------------------------\n"));

        double max_row_diff[4]={0};
        int max_row_pos[4]={0};
        double row_max_min_diff[4]={0};

        int cnt_dead_row=0;
        int cnt_row_dol =0;
        double row_dol_std_max[4]={0};
        int row_dol_std_max_pos[4]={0};

        int cnt_breakline_row =0;
        double breakline_row_max_value[4]={0};
        int breakline_row_max_pos[4]={0};

        TestItem::test_row_defect_bychannel(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,w, h,
                                            manage.param_line_defect_bright_.strip_row_,
                                            manage.param_line_defect_bright_.r_area_,
                                            manage.param_line_defect_bright_.spec_row_dol_r_,
                                            manage.param_line_defect_bright_.spec_row_dol_gr_,
                                            manage.param_line_defect_bright_.spec_row_dol_gb_,
                                            manage.param_line_defect_bright_.spec_row_dol_b_,
                                            manage.param_line_defect_bright_.deadrow_spec_r_TA_,
                                            manage.param_line_defect_bright_.deadrow_spec_gr_TA_,
                                            manage.param_line_defect_bright_.deadrow_spec_gb_TA_,
                                            manage.param_line_defect_bright_.deadrow_spec_b_TA_,
                                            max_row_diff, max_row_pos, row_max_min_diff, cnt_dead_row,
                                            cnt_row_dol, row_dol_std_max, row_dol_std_max_pos,
                                            manage.param_line_defect_bright_.breakrow_spec_r_level_1,
                                            manage.param_line_defect_bright_.breakrow_spec_gr_level_1,
                                            manage.param_line_defect_bright_.breakrow_spec_gb_level_1,
                                            manage.param_line_defect_bright_.breakrow_spec_b_level_1,
                                            cnt_breakline_row, breakline_row_max_value, breakline_row_max_pos,
                                            Ratio, g_str_msg_all[site_], manage.param_line_defect_bright_.log_print_);


        double max_col_diff[4]={0};
        int max_col_pos[4]={0};
        double col_max_min_diff[4]={0};

        int cnt_dead_col=0;
        int cnt_col_dol =0;
        double col_dol_std_max[4]={0};
        int col_dol_std_max_pos[4]={0};

        int cnt_breakline_col =0;
        double breakline_col_max_value[4]={0};
        int breakline_col_max_pos[4]={0};


        TestItem::test_col_defect_bychannel(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,w, h,
                                            manage.param_line_defect_bright_.strip_col_,
                                            manage.param_line_defect_bright_.r_area_,
                                            manage.param_line_defect_bright_.spec_col_dol_r_,
                                            manage.param_line_defect_bright_.spec_col_dol_gr_,
                                            manage.param_line_defect_bright_.spec_col_dol_gb_,
                                            manage.param_line_defect_bright_.spec_col_dol_b_,
                                            manage.param_line_defect_bright_.deadcol_spec_r_TA_,
                                            manage.param_line_defect_bright_.deadcol_spec_gr_TA_,
                                            manage.param_line_defect_bright_.deadcol_spec_gb_TA_,
                                            manage.param_line_defect_bright_.deadcol_spec_b_TA_,
                                            max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                            cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                            manage.param_line_defect_bright_.breakcol_spec_r_level_1,
                                            manage.param_line_defect_bright_.breakcol_spec_gr_level_1,
                                            manage.param_line_defect_bright_.breakcol_spec_gb_level_1,
                                            manage.param_line_defect_bright_.breakcol_spec_b_level_1,
                                            cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                            Ratio, g_str_msg_all[site_], manage.param_line_defect_bright_.log_print_);

//        start=clock();
//        cost_time_old=start-end;

        if(log_flag)
        {
            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
                                             % max_row_diff[0] % max_row_diff[1] % max_row_diff[2] % max_row_diff[3]
                    % max_row_pos[0] % max_row_pos[1] % max_row_pos[2] % max_row_pos[3]
                    % row_max_min_diff[0] % row_max_min_diff[1] % row_max_min_diff[2] % row_max_min_diff[3]);

            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                             % row_dol_std_max[0] % row_dol_std_max[1] % row_dol_std_max[2] % row_dol_std_max[3]
                    % row_dol_std_max_pos[0] % row_dol_std_max_pos[1] % row_dol_std_max_pos[2] % row_dol_std_max_pos[3]);

            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                             % breakline_row_max_value[0] % breakline_row_max_value[1] % breakline_row_max_value[2] % breakline_row_max_value[3]
                    % breakline_row_max_pos[0] % breakline_row_max_pos[1] % breakline_row_max_pos[2] % breakline_row_max_pos[3]);


            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
                                             % max_col_diff[0] % max_col_diff[1] % max_col_diff[2] % max_col_diff[3]
                    % max_col_pos[0] % max_col_pos[1] % max_col_pos[2] % max_col_pos[3]
                    % col_max_min_diff[0] % col_max_min_diff[1] % col_max_min_diff[2] % col_max_min_diff[3]);

            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                             % col_dol_std_max[0] % col_dol_std_max[1] % col_dol_std_max[2] % col_dol_std_max[3]
                    % col_dol_std_max_pos[0] % col_dol_std_max_pos[1] % col_dol_std_max_pos[2] % col_dol_std_max_pos[3]);

            g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                             % breakline_col_max_value[0] % breakline_col_max_value[1] % breakline_col_max_value[2] % breakline_col_max_value[3]
                    % breakline_col_max_pos[0] % breakline_col_max_pos[1] % breakline_col_max_pos[2] % breakline_col_max_pos[3]);


            g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|")
                                             % cnt_dead_row % cnt_dead_col %cnt_row_dol %cnt_col_dol %cnt_breakline_row %cnt_breakline_col);
            g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_line.name.c_str());

        }

        logMsg (std::string("after dead line"));

        int cnt_dol = cnt_row_dol + cnt_col_dol;
        int cnt_break_line = cnt_breakline_row + cnt_breakline_col;

        if (cnt_dead_col>0 || cnt_dead_row>0 || cnt_dol>0 || cnt_break_line>0)
            result_line.is_passed = false;

        if (cnt_dead_col>0)
        {
            if(max_col_diff[0]>=manage.param_line_defect_bright_.deadcol_spec_r_MA_ ||
                    max_col_diff[1]>=manage.param_line_defect_bright_.deadcol_spec_gr_MA_ ||
                    max_col_diff[2]>=manage.param_line_defect_bright_.deadcol_spec_gb_MA_ ||
                    max_col_diff[3]>=manage.param_line_defect_bright_.deadcol_spec_b_MA_ )
            {
                result_line.bin = manage.param_line_defect_bright_.bin_dead_col_;
                result_line.grade = manage.param_line_defect_bright_.grade_dead_col_;
            }
            else
            {
                result_line.bin = manage.param_line_defect_bright_.bin_dead_col_MA_;
                result_line.grade = manage.param_line_defect_bright_.grade_dead_col_MA;
            }
        }
        if (cnt_dead_row>0 && manage.param_line_defect_bright_.grade_dead_row_>result_line.grade)
        {
            if(max_row_diff[0]>=manage.param_line_defect_bright_.deadrow_spec_r_MA_ ||
                    max_row_diff[1]>=manage.param_line_defect_bright_.deadrow_spec_gr_MA_ ||
                    max_row_diff[2]>=manage.param_line_defect_bright_.deadrow_spec_gb_MA_ ||
                    max_row_diff[3]>=manage.param_line_defect_bright_.deadrow_spec_b_MA_ )
            {
                result_line.bin = manage.param_line_defect_bright_.bin_dead_row_;
                result_line.grade = manage.param_line_defect_bright_.grade_dead_row_;
            }
            else
            {
                result_line.bin = manage.param_line_defect_bright_.bin_dead_row_MA_;
                result_line.grade = manage.param_line_defect_bright_.grade_dead_row_MA;
            }
        }
        if (cnt_dol>0 && manage.param_line_defect_bright_.grade_dol_>result_line.grade)
        {
            result_line.bin = manage.param_line_defect_bright_.bin_dol_;
            result_line.grade = manage.param_line_defect_bright_.grade_dol_;
        }
        if (cnt_break_line>0 && (manage.param_line_defect_bright_.grade_break_line_level1>result_line.grade || manage.param_line_defect_bright_.grade_break_line_level2>result_line.grade))
        {
            if( breakline_row_max_value[0] > manage.param_line_defect_bright_.breakrow_spec_r_level_2 ||
                    breakline_row_max_value[1] > manage.param_line_defect_bright_.breakrow_spec_gr_level_2 ||
                    breakline_row_max_value[2] > manage.param_line_defect_bright_.breakrow_spec_gb_level_2 ||
                    breakline_row_max_value[3] > manage.param_line_defect_bright_.breakrow_spec_b_level_2 ||
                    breakline_col_max_value[0] > manage.param_line_defect_bright_.breakcol_spec_r_level_2 ||
                    breakline_col_max_value[1] > manage.param_line_defect_bright_.breakcol_spec_gr_level_2 ||
                    breakline_col_max_value[2] > manage.param_line_defect_bright_.breakcol_spec_gb_level_2 ||
                    breakline_col_max_value[3] > manage.param_line_defect_bright_.breakcol_spec_b_level_2 )
            {
                result_line.bin = manage.param_line_defect_bright_.bin_break_line_level2;
                result_line.grade = manage.param_line_defect_bright_.grade_break_line_level2;
            }
            else  if( breakline_row_max_value[0] > manage.param_line_defect_bright_.breakrow_spec_r_level_1 ||
                      breakline_row_max_value[1] > manage.param_line_defect_bright_.breakrow_spec_gr_level_1 ||
                      breakline_row_max_value[2] > manage.param_line_defect_bright_.breakrow_spec_gb_level_1 ||
                      breakline_row_max_value[3] > manage.param_line_defect_bright_.breakrow_spec_b_level_1 ||
                      breakline_col_max_value[0] > manage.param_line_defect_bright_.breakcol_spec_r_level_1 ||
                      breakline_col_max_value[1] > manage.param_line_defect_bright_.breakcol_spec_gr_level_1 ||
                      breakline_col_max_value[2] > manage.param_line_defect_bright_.breakcol_spec_gb_level_1 ||
                      breakline_col_max_value[3] > manage.param_line_defect_bright_.breakcol_spec_b_level_1 )
              {
                  result_line.bin = manage.param_line_defect_bright_.bin_break_line_level1;
                  result_line.grade = manage.param_line_defect_bright_.grade_break_line_level1;
              }
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        result_line.g_msg = false;
    }


    return result_line;
}




//IIC死线  (白屏、Large_Gain、Range)
Subitem_Result  AlgorithmItems::Deadrow_Deadcol_bright_IIC(const ParamManage& manage)
{
    if(manage.line_defect_bright_iic_version == "line_defect_bright_iic_V1.0")
        return Deadrow_Deadcol_bright_IIC_V1_0(manage);

    Subitem_Result res("Deadrow_Deadcol_bright_IIC", false);
    res.g_msg = false;
    return res;
}


Subitem_Result  AlgorithmItems::Deadrow_Deadcol_bright_IIC_V1_0(const ParamManage& manage)
{
    time_begin=clock();

    Subitem_Result result_line("Deadrow_Deadcol_bright_IIC",true);

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_line_defect_bright_iic_.open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadrow_Deadcol_bright_IIC V1.0-----------------------------\n"));

        double max_row_diff[4]={0};
        int max_row_pos[4]={0};
        double row_max_min_diff[4]={0};

        int cnt_dead_row=0;
        int cnt_row_dol =0;
        double row_dol_std_max[4]={0};
        int row_dol_std_max_pos[4]={0};

        int cnt_breakline_row =0;
        double breakline_row_max_value[4]={0};
        int breakline_row_max_pos[4]={0};

        TestItem::test_row_defect_bychannel(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,w, h,
                                            manage.param_line_defect_bright_iic_.strip_row_,
                                            manage.param_line_defect_bright_iic_.r_area_,
                                            manage.param_line_defect_bright_iic_.spec_row_dol_r_,
                                            manage.param_line_defect_bright_iic_.spec_row_dol_gr_,
                                            manage.param_line_defect_bright_iic_.spec_row_dol_gb_,
                                            manage.param_line_defect_bright_iic_.spec_row_dol_b_,
                                            manage.param_line_defect_bright_iic_.deadrow_spec_r_TA_,
                                            manage.param_line_defect_bright_iic_.deadrow_spec_gr_TA_,
                                            manage.param_line_defect_bright_iic_.deadrow_spec_gb_TA_,
                                            manage.param_line_defect_bright_iic_.deadrow_spec_b_TA_,
                                            max_row_diff, max_row_pos, row_max_min_diff, cnt_dead_row,
                                            cnt_row_dol, row_dol_std_max, row_dol_std_max_pos,
                                            manage.param_line_defect_bright_iic_.breakrow_spec_r_,
                                            manage.param_line_defect_bright_iic_.breakrow_spec_gr_,
                                            manage.param_line_defect_bright_iic_.breakrow_spec_gb_,
                                            manage.param_line_defect_bright_iic_.breakrow_spec_b_,
                                            cnt_breakline_row, breakline_row_max_value, breakline_row_max_pos,
                                            Ratio, g_str_msg_all[site_], manage.param_line_defect_bright_iic_.log_print_);

        double max_col_diff[4]={0};
        int max_col_pos[4]={0};
        double col_max_min_diff[4]={0};

        int cnt_dead_col=0;
        int cnt_col_dol =0;
        double col_dol_std_max[4]={0};
        int col_dol_std_max_pos[4]={0};

        int cnt_breakline_col =0;
        double breakline_col_max_value[4]={0};
        int breakline_col_max_pos[4]={0};


        TestItem::test_col_defect_bychannel(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,w, h,
                                            manage.param_line_defect_bright_iic_.strip_col_,
                                            manage.param_line_defect_bright_iic_.r_area_,
                                            manage.param_line_defect_bright_iic_.spec_col_dol_r_,
                                            manage.param_line_defect_bright_iic_.spec_col_dol_gr_,
                                            manage.param_line_defect_bright_iic_.spec_col_dol_gb_,
                                            manage.param_line_defect_bright_iic_.spec_col_dol_b_,
                                            manage.param_line_defect_bright_iic_.deadcol_spec_r_TA_,
                                            manage.param_line_defect_bright_iic_.deadcol_spec_gr_TA_,
                                            manage.param_line_defect_bright_iic_.deadcol_spec_gb_TA_,
                                            manage.param_line_defect_bright_iic_.deadcol_spec_b_TA_,
                                            max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                            cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                            manage.param_line_defect_bright_iic_.breakcol_spec_r_,
                                            manage.param_line_defect_bright_iic_.breakcol_spec_gr_,
                                            manage.param_line_defect_bright_iic_.breakcol_spec_gb_,
                                            manage.param_line_defect_bright_iic_.breakcol_spec_b_,
                                            cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                            Ratio, g_str_msg_all[site_], manage.param_line_defect_bright_iic_.log_print_);


         g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|")
                                % max_row_diff[0] % max_row_diff[1] % max_row_diff[2] % max_row_diff[3]);

         g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|")
                                % row_dol_std_max[0] % row_dol_std_max[1] % row_dol_std_max[2] % row_dol_std_max[3]);

         g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|")
                                % breakline_row_max_value[0] % breakline_row_max_value[1] % breakline_row_max_value[2] % breakline_row_max_value[3]);

         g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|")
                                % max_col_diff[0] % max_col_diff[1] % max_col_diff[2] % max_col_diff[3]);

         g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|")
                                % col_dol_std_max[0] % col_dol_std_max[1] % col_dol_std_max[2] % col_dol_std_max[3]);

         g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|")
                                % breakline_col_max_value[0] % breakline_col_max_value[1] % breakline_col_max_value[2] % breakline_col_max_value[3]);

         g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|")
                                % cnt_dead_row % cnt_dead_col %cnt_row_dol %cnt_col_dol %cnt_breakline_row %cnt_breakline_col);
         g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_line.name.c_str());




        logMsg (std::string("after dead line"));

        int cnt_dol = cnt_row_dol + cnt_col_dol;
        int cnt_break_line = cnt_breakline_row + cnt_breakline_col;

        if (cnt_dead_col>0 || cnt_dead_row>0 || cnt_dol>0 || cnt_break_line>0)
            result_line.is_passed = false;

        if (cnt_dead_col>0)
        {
            if(max_col_diff[0]>=manage.param_line_defect_bright_iic_.deadcol_spec_r_MA_ ||
                    max_col_diff[1]>=manage.param_line_defect_bright_iic_.deadcol_spec_gr_MA_ ||
                    max_col_diff[2]>=manage.param_line_defect_bright_iic_.deadcol_spec_gb_MA_ ||
                    max_col_diff[3]>=manage.param_line_defect_bright_iic_.deadcol_spec_b_MA_ )
            {
                result_line.bin = manage.param_line_defect_bright_iic_.bin_dead_col_;
                result_line.grade = manage.param_line_defect_bright_iic_.grade_dead_col_;
            }
            else
            {
                result_line.bin = manage.param_line_defect_bright_iic_.bin_dead_col_MA_;
                result_line.grade = manage.param_line_defect_bright_iic_.grade_dead_col_MA;
            }
        }
        if (cnt_dead_row>0 && manage.param_line_defect_bright_iic_.grade_dead_row_>result_line.grade)
        {
            if(max_row_diff[0]>=manage.param_line_defect_bright_iic_.deadrow_spec_r_MA_ ||
                    max_row_diff[1]>=manage.param_line_defect_bright_iic_.deadrow_spec_gr_MA_ ||
                    max_row_diff[2]>=manage.param_line_defect_bright_iic_.deadrow_spec_gb_MA_ ||
                    max_row_diff[3]>=manage.param_line_defect_bright_iic_.deadrow_spec_b_MA_ )
            {
                result_line.bin = manage.param_line_defect_bright_iic_.bin_dead_row_;
                result_line.grade = manage.param_line_defect_bright_iic_.grade_dead_row_;
            }
            else
            {
                result_line.bin = manage.param_line_defect_bright_iic_.bin_dead_row_MA_;
                result_line.grade = manage.param_line_defect_bright_iic_.grade_dead_row_MA;
            }
        }
        if (cnt_dol>0 && manage.param_line_defect_bright_iic_.grade_dol_>result_line.grade)
        {
            result_line.bin = manage.param_line_defect_bright_iic_.bin_dol_;
            result_line.grade = manage.param_line_defect_bright_iic_.grade_dol_;
        }
        if (cnt_break_line>0 && manage.param_line_defect_bright_iic_.grade_break_line_>result_line.grade)
        {
            result_line.bin = manage.param_line_defect_bright_iic_.bin_break_line_;
            result_line.grade = manage.param_line_defect_bright_iic_.grade_break_line_;
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
        result_line.g_msg = false;


    return result_line;
}



//边缘死线 (白屏、Large_Gain、Range)
Subitem_Result  AlgorithmItems::Edge_Deadrow_Deadcol_bright(const ParamManage& manage)
{
    if(manage.edge_line_defect_bright_version == "edge_line_defect_bright_V1.0")
        return Edge_Deadrow_Deadcol_bright_V1_0(manage);

    Subitem_Result res("Edge_Deadrow_Deadcol_bright", false);
    res.g_msg = false;
    return res;
}


Subitem_Result  AlgorithmItems::Edge_Deadrow_Deadcol_bright_V1_0(const ParamManage& manage)
{
    Subitem_Result result_edge_line ("Edge_Deadrow_Deadcol_bright",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_edge_line_defect_bright_.open_mode_ == 1)
    {
        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Edge Deadrow and Deadcol V1.0-----------------------------\n"));

        double max_edge_row_diff[4]={0};
        int max_edge_row_pos[4]={0};

        int cnt_edge_dead_row=0;
        int cnt_edge_row_dol=0;

        double edge_row_dol_std_max[4]={0};
        int edge_row_dol_std_max_pos[4]={0};


        TestItem::test_edge_row_defect_bychannel(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,w, h,
                                                 manage.param_edge_line_defect_bright_.strip_row_,
                                                 manage.param_edge_line_defect_bright_.r_area_,
                                                 manage.param_edge_line_defect_bright_.spec_row_dol_r_,
                                                 manage.param_edge_line_defect_bright_.spec_row_dol_gr_,
                                                 manage.param_edge_line_defect_bright_.spec_row_dol_gb_,
                                                 manage.param_edge_line_defect_bright_.spec_row_dol_b_,
                                                 manage.param_edge_line_defect_bright_.deadrow_spec_r_TA_,
                                                 manage.param_edge_line_defect_bright_.deadrow_spec_gr_TA_,
                                                 manage.param_edge_line_defect_bright_.deadrow_spec_gb_TA_,
                                                 manage.param_edge_line_defect_bright_.deadrow_spec_b_TA_,
                                                 max_edge_row_diff, max_edge_row_pos, cnt_edge_dead_row, cnt_edge_row_dol,
                                                 edge_row_dol_std_max, edge_row_dol_std_max_pos, Ratio, g_str_msg_all[site_]);


        double max_edge_col_diff[4]={0};
        int max_edge_col_pos[4]={0};

        int cnt_edge_dead_col=0;
        int cnt_edge_col_dol=0;

        double edge_col_dol_std_max[4]={0};
        int edge_col_dol_std_max_pos[4]={0};


        TestItem::test_edge_col_defect_bychannel(g_buf_r_, g_buf_gr_, g_buf_gb_, g_buf_b_,w, h,
                                                 manage.param_edge_line_defect_bright_.strip_col_,
                                                 manage.param_edge_line_defect_bright_.r_area_,
                                                 manage.param_edge_line_defect_bright_.spec_col_dol_r_,
                                                 manage.param_edge_line_defect_bright_.spec_col_dol_gr_,
                                                 manage.param_edge_line_defect_bright_.spec_col_dol_gb_,
                                                 manage.param_edge_line_defect_bright_.spec_col_dol_b_,
                                                 manage.param_edge_line_defect_bright_.deadcol_spec_r_TA_,
                                                 manage.param_edge_line_defect_bright_.deadcol_spec_gr_TA_,
                                                 manage.param_edge_line_defect_bright_.deadcol_spec_gb_TA_,
                                                 manage.param_edge_line_defect_bright_.deadcol_spec_b_TA_,
                                                 max_edge_col_diff, max_edge_col_pos, cnt_edge_dead_col, cnt_edge_col_dol,
                                                 edge_col_dol_std_max, edge_col_dol_std_max_pos, Ratio, g_str_msg_all[site_]);


        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                               % max_edge_row_diff[0] % max_edge_row_diff[1] % max_edge_row_diff[2] % max_edge_row_diff[3]
                % max_edge_row_pos[0] % max_edge_row_pos[1] % max_edge_row_pos[2] % max_edge_row_pos[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                               % edge_row_dol_std_max[0] % edge_row_dol_std_max[1] % edge_row_dol_std_max[2] % edge_row_dol_std_max[3]
                % edge_row_dol_std_max_pos[0] % edge_row_dol_std_max_pos[1] % edge_row_dol_std_max_pos[2] % edge_row_dol_std_max_pos[3]);


        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                               % max_edge_col_diff[0] % max_edge_col_diff[1] % max_edge_col_diff[2] % max_edge_col_diff[3]
                % max_edge_col_pos[0] % max_edge_col_pos[1] % max_edge_col_pos[2] % max_edge_col_pos[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                               % edge_col_dol_std_max[0] % edge_col_dol_std_max[1] % edge_col_dol_std_max[2] % edge_col_dol_std_max[3]
                % edge_col_dol_std_max_pos[0] % edge_col_dol_std_max_pos[1] % edge_col_dol_std_max_pos[2] % edge_col_dol_std_max_pos[3]);


        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|")
                               % cnt_edge_dead_row % cnt_edge_dead_col %cnt_edge_row_dol %cnt_edge_col_dol);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_edge_line.name.c_str());


        logMsg (std::string("after dead line"));
        int cnt_edge_dol = cnt_edge_row_dol + cnt_edge_col_dol;

        if (cnt_edge_dead_col>0 || cnt_edge_dead_row>0 || cnt_edge_dol>0)
            result_edge_line.is_passed = false;

        if (cnt_edge_dead_col>0)
        {
            if(max_edge_col_diff[0]>=manage.param_edge_line_defect_bright_.deadcol_spec_r_MA_ ||
                    max_edge_col_diff[1]>=manage.param_edge_line_defect_bright_.deadcol_spec_gr_MA_ ||
                    max_edge_col_diff[2]>=manage.param_edge_line_defect_bright_.deadcol_spec_gb_MA_ ||
                    max_edge_col_diff[3]>=manage.param_edge_line_defect_bright_.deadcol_spec_b_MA_ )
            {
                result_edge_line.bin = manage.param_edge_line_defect_bright_.bin_dead_col_;
                result_edge_line.grade = manage.param_edge_line_defect_bright_.grade_dead_col_;
            }
            else
            {
                result_edge_line.bin = manage.param_edge_line_defect_bright_.bin_dead_col_MA_;
                result_edge_line.grade = manage.param_edge_line_defect_bright_.grade_dead_col_MA;
            }
        }
        if (cnt_edge_dead_row>0 && manage.param_edge_line_defect_bright_.grade_dead_row_>result_edge_line.grade)
        {
            if(max_edge_row_diff[0]>=manage.param_edge_line_defect_bright_.deadrow_spec_r_MA_ ||
                    max_edge_row_diff[1]>=manage.param_edge_line_defect_bright_.deadrow_spec_gr_MA_ ||
                    max_edge_row_diff[2]>=manage.param_edge_line_defect_bright_.deadrow_spec_gb_MA_ ||
                    max_edge_row_diff[3]>=manage.param_edge_line_defect_bright_.deadrow_spec_b_MA_ )
            {
                result_edge_line.bin = manage.param_edge_line_defect_bright_.bin_dead_row_;
                result_edge_line.grade = manage.param_edge_line_defect_bright_.grade_dead_row_;
            }
            else
            {
                result_edge_line.bin = manage.param_edge_line_defect_bright_.bin_dead_row_MA_;
                result_edge_line.grade = manage.param_edge_line_defect_bright_.grade_dead_row_MA;
            }
        }
        if (cnt_edge_dol>0 && manage.param_edge_line_defect_bright_.grade_dol_>result_edge_line.grade)
        {
            result_edge_line.bin = manage.param_edge_line_defect_bright_.bin_dol_;
            result_edge_line.grade = manage.param_edge_line_defect_bright_.grade_dol_;
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        result_edge_line.g_msg = false;
    }


    return result_edge_line;
}



//白屏 ROI PRNU
Subitem_Result AlgorithmItems::ROI_PRNU(const ParamManage& manage)
{
    if(manage.roi_prnu_version == "ROI_PRNU_V1.0")
        return ROI_PRNU_V1_0(manage);

    Subitem_Result res("ROI_PRNU", false);
    res.g_msg = false;
    return res;

}

Subitem_Result AlgorithmItems::ROI_PRNU_V1_0(const ParamManage& manage)
{

    Subitem_Result  result_real_roi_prnu("ROI_PRNU",true);
    int w=manage.width_;
    int h=manage.height_;

    if( (manage.param_pixel_defect_.open_mode_ == 1 && manage.param_real_roi_prnu_.open_mode_ == 1) ||
            (manage.param_single_pixel_defect_.open_mode_ == 1 && manage.param_real_roi_prnu_.open_mode_ == 1) )
    {

        time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------ROI PRNU V1.0-----------------------------\n"));

        double roi_prnu_r = -1;
        double roi_prnu_gr = -1;
        double roi_prnu_gb = -1;
        double roi_prnu_b = -1;

        if(mean_gr_roi>1 )
            TestItem::test_real_roi_prnu(w,h,
                                         manage.param_real_roi_prnu_.X_ROI,
                                         manage.param_real_roi_prnu_.Y_ROI,
                                         manage.param_real_roi_prnu_.W_ROI,
                                         manage.param_real_roi_prnu_.H_ROI,
                                         roi_prnu_r,roi_prnu_gr,roi_prnu_gb,roi_prnu_b,
                                         g_buf_delta_r_,g_buf_delta_gr_,g_buf_delta_gb_,g_buf_delta_b_,
                                         g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_);


        g_str_msg_all[site_]+=boost::str(boost::format("roi_prnu_r:%.2f,roi_prnu_gr:%.2f\n"
                                             "roi_prnu_gb:%.2f,roi_prnu_b:%.2f\n")
                               % roi_prnu_r % roi_prnu_gr % roi_prnu_gb % roi_prnu_b);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % roi_prnu_r % roi_prnu_gr % roi_prnu_gb % roi_prnu_b);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_real_roi_prnu.name.c_str());


        g_str_msg_all[site_]+=boost::str(boost::format(
                                             "SPEC:\n"
                                             "roi_prnu_r:%.2f (SPEC: %.4f),roi_prnu_gr:%.2f (SPEC: %.4f)\n"
                                             "roi_prnu_gb:%.2f (SPEC: %.4f),roi_prnu_b:%.2f (SPEC: %.4f)\n"
                                             )
                                         % roi_prnu_r
                                         % manage.param_real_roi_prnu_.spec_prnu_r_
                                         % roi_prnu_gr
                                         % manage.param_real_roi_prnu_.spec_prnu_gr_
                                         % roi_prnu_gb
                                         % manage.param_real_roi_prnu_.spec_prnu_gb_
                                         % roi_prnu_b
                                         % manage.param_real_roi_prnu_.spec_prnu_b_);


        if (roi_prnu_r>manage.param_real_roi_prnu_.spec_prnu_r_ ||
                roi_prnu_gr>manage.param_real_roi_prnu_.spec_prnu_gr_ ||
                roi_prnu_gb>manage.param_real_roi_prnu_.spec_prnu_gb_ ||
                roi_prnu_b>manage.param_real_roi_prnu_.spec_prnu_b_)
        {
            result_real_roi_prnu.is_passed = false;
            result_real_roi_prnu.bin = manage.param_real_roi_prnu_.bin_;
            result_real_roi_prnu.grade = manage.param_real_roi_prnu_.grade_;
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        result_real_roi_prnu.g_msg = false;
    }


    return result_real_roi_prnu;
}



//黑屏 ROI DSNU
Subitem_Result  AlgorithmItems::ROI_DSNU(const ParamManage& manage)
{
    if(manage.roi_prnu_version == "ROI_PRNU_V1.0")
        return ROI_DSNU_V1_0(manage);

    Subitem_Result res("ROI_DSNU", false);
    res.g_msg = false;
    return res;
}

Subitem_Result  AlgorithmItems::ROI_DSNU_V1_0(const ParamManage& manage)
{
    time_begin=clock();
    Subitem_Result  result_real_roi_dsnu("ROI_DSNU",true);
    int width=manage.width_;
    int height=manage.height_;

    if(manage.param_pixel_defect_.open_mode_ == 1 &&
       manage.param_real_roi_prnu_.open_mode_ == 1 )
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------ROI DSNU V1.0-----------------------------\n"));

        double roi_dsnu_r = -1;
        double roi_dsnu_gr = -1;
        double roi_dsnu_gb = -1;
        double roi_dsnu_b = -1;

        if(mean_gr_roi>1 )
            TestItem::test_real_roi_prnu(width,height,
                                         manage.param_real_roi_prnu_.X_ROI,
                                         manage.param_real_roi_prnu_.Y_ROI,
                                         manage.param_real_roi_prnu_.W_ROI,
                                         manage.param_real_roi_prnu_.H_ROI,
                                         roi_dsnu_r,roi_dsnu_gr,roi_dsnu_gb,roi_dsnu_b,
                                         g_buf_delta_r_,g_buf_delta_gr_,g_buf_delta_gb_,g_buf_delta_b_,
                                         g_buf_r_,g_buf_gr_,g_buf_gb_,g_buf_b_);


        g_str_msg_all[site_]+=boost::str(boost::format("roi_dsnu_r:%.2f,roi_dsnu_gr:%.2f\n"
                                             "roi_dsnu_gb:%.2f,roi_dsnu_b:%.2f\n")
                               % roi_dsnu_r % roi_dsnu_gr % roi_dsnu_gb % roi_dsnu_b);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % roi_dsnu_r % roi_dsnu_gr % roi_dsnu_gb % roi_dsnu_b);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_real_roi_dsnu.name.c_str());

        g_str_msg_all[site_]+=boost::str(boost::format(
                                             "SPEC:\n"
                                             "roi_prnu_r:%.2f (SPEC: %.4f),roi_prnu_gr:%.2f (SPEC: %.4f)\n"
                                             "roi_prnu_gb:%.2f (SPEC: %.4f),roi_prnu_b:%.2f (SPEC: %.4f)\n"
                                             )
                                         % roi_dsnu_r
                                         % manage.param_real_roi_prnu_.spec_prnu_r_
                                         % roi_dsnu_gr
                                         % manage.param_real_roi_prnu_.spec_prnu_gr_
                                         % roi_dsnu_gb
                                         % manage.param_real_roi_prnu_.spec_prnu_gb_
                                         % roi_dsnu_b
                                         % manage.param_real_roi_prnu_.spec_prnu_b_);


        if (roi_dsnu_r>manage.param_real_roi_prnu_.spec_prnu_r_ ||
                roi_dsnu_gr>manage.param_real_roi_prnu_.spec_prnu_gr_ ||
                roi_dsnu_gb>manage.param_real_roi_prnu_.spec_prnu_gb_ ||
                roi_dsnu_b>manage.param_real_roi_prnu_.spec_prnu_b_)
        {
            result_real_roi_dsnu.is_passed = false;
            result_real_roi_dsnu.bin = manage.param_real_roi_prnu_.bin_;
            result_real_roi_dsnu.grade = manage.param_real_roi_prnu_.grade_;
        }

        time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(time_end-time_begin).toStdString()+"\n";

    }
    else
    {
        result_real_roi_dsnu.g_msg = false;
    }


    return result_real_roi_dsnu;
}




//计算最终bin
Subitem_Result AlgorithmItems::Calculate_The_Final_Bin(const vector<Subitem_Result>& items_results)
{
    Subitem_Result final_result("The initial value",true,1,0,true);

    for(int i=0;i!=items_results.size();++i)
    {
        const Subitem_Result& item=items_results[i];

        if(item.g_msg == false)
            continue;

        std::string item_log =item.name;
        QString log_;
        if(item.is_passed)
            log_+=" is Pass. Bin: "+ QString::number(item.bin)+" Grade: "+ QString::number(item.grade)+"\n";
        else
            log_+=" is Failed. Bin: "+ QString::number(item.bin)+" Grade: "+ QString::number(item.grade)+"\n";

        g_str_msg_all[site_]+=item_log+log_.toStdString();

        if(!item.is_passed && item.grade>final_result.grade)
            final_result=item;
    }
    return final_result;
}


