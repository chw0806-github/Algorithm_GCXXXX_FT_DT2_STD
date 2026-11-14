#include "MapBinarization.h"

//分通道 坏点查找 raw图二值化
int  MapBinarization::FindDeadPixel_bright(ParamManage& manage,uchar *pData_in,
                                           uchar *pData_out, uchar *pImage_save)
{
    clock_t time_begin=clock();
    clock_t time_end=clock();

    g_str_msg_all[site_].clear();
    //-----------------Parameter Check--------------------
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }

    logMsg (std::string("into dark frame test, check params first!"));
    if (manage.width_<=0 ||manage.height_<=0 || (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    if(test_channel_type==Split_Bayer)
    {
        if( !Algorithm_all[site_].Split_Channel(manage,Split_Bayer,false,
                                      pData_in,test_frameCnt,test_datatype,pImage_save))
            return -1;

        if( !Static_MapBinarization_bright(manage,Binarization_bayer, pData_out))
            return -1;
    }
    else if(test_channel_type==Split_4C)
    {
        if( !Algorithm_all[site_].Split_Channel(manage,Split_4C,false,
                                      pData_in,test_frameCnt,test_datatype,pImage_save))
            return -1;

        time_begin=clock();
        if( !Static_MapBinarization_bright(manage,Binarization_4C, pData_out))
            return -1;
        time_end=clock();
    }

//    cost_time_step1=time_end-time_begin;
//    g_str_msg_+=boost::str(boost::format("-------------------------- Step 1 -----------------------------\n"));
//    g_str_msg_+="\n cost_time_new:"+QString::number(cost_time_step1).toStdString()+"\n";

    return 1;
}
//分通道 坏点查找 raw图二值化
int  MapBinarization::FindDeadPixel_dark(ParamManage& manage,uchar *pData_in, uchar *pData_out,
                                    uchar *pImage_save)
{
    clock_t time_begin=clock();
    clock_t time_end=clock();

    g_str_msg_all[site_].clear();
    //-----------------Parameter Check--------------------
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }

    logMsg (std::string("into dark frame test, check params first!"));
    if (manage.width_<=0 ||manage.height_<=0 || (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    if(test_channel_type==Split_Bayer)
    {
        if( !Algorithm_all[site_].Split_Channel(manage,Split_Bayer,false,
                                      pData_in,test_frameCnt,test_datatype,pImage_save))
            return -1;

        if( !Static_MapBinarization_dark(manage,Binarization_bayer, pData_out))
            return -1;
    }
    else if(test_channel_type==Split_4C)
    {
        if( !Algorithm_all[site_].Split_Channel(manage,Split_4C,false,
                                      pData_in,test_frameCnt,test_datatype,pImage_save))
            return -1;

        time_begin=clock();
        if( !Static_MapBinarization_dark(manage,Binarization_4C, pData_out))
            return -1;
        time_end=clock();
    }

//    cost_time_step1=time_end-time_begin;
//    g_str_msg_+=boost::str(boost::format("-------------------------- Step 1 -----------------------------\n"));
//    g_str_msg_+="\n cost_time_new:"+QString::number(cost_time_step1).toStdString()+"\n";

    return 1;
}


//分通道 闪点查找并 raw图二值化  double byte
int MapBinarization::Flicker_pix_STD_Binarization(ParamManage& manage,uchar *pData_in, uchar *pData_out)
{
    if (manage.width_<=0 ||manage.height_<=0 || (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    if(!Algorithm_all[site_].Convert_Raw16_to_Ushort(manage,pData_in,(unsigned short *)pData_in,
                                                     test_frameCnt,test_datatype))
    {
        g_str_msg_all[site_].append ("Convert_Raw16_to_Ushort wrong!\n");

        return -1;
    }

    logMsg ("Start Flicker_MapBinarization_chao !");
    if(!Flicker_MapBinarization_chao(manage,pData_in,test_frameCnt, pData_out))
         return -1;

    return 1;
}


/*-------------    坏点查找 功能函数      ---------------*/

//静态 坏点查询
bool MapBinarization::Static_MapBinarization_bright(const ParamManage& manage,
                                           MapBinarization_type type, uchar *pData_out)
{
    int w = manage.width_;
    int h = manage.height_;

    bool flag_newbuf = Algorithm_all[site_].New_Pointer_Buf (&Algorithm_all[site_].g_binary_,Result_area,
                                   Buf_uchar_8,w,h,2);
    if(flag_newbuf)
    {
        Algorithm_all[site_].g_binary_r_ =   Algorithm_all[site_].g_binary_  + w*h;
        Algorithm_all[site_].g_binary_gr_ =  Algorithm_all[site_].g_binary_r_+ w*h*1/4;
        Algorithm_all[site_].g_binary_gb_ =  Algorithm_all[site_].g_binary_r_+ w*h*2/4;
        Algorithm_all[site_].g_binary_b_ =   Algorithm_all[site_].g_binary_r_+ w*h*3/4;
    }
    else
        return false;

    MultiThread::bychannel_binarization *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_binarization();
    }

    p_binary[R]->setparam(Algorithm_all[site_].g_buf_r_,Algorithm_all[site_].g_binary_r_,w/2,h/2,
                          manage.param_binary_.skip,
                          manage.param_binary_.bright_binary_radius_,
                          manage.param_binary_.bright_binary_spec_r_,Ratio);
    p_binary[Gr]->setparam(Algorithm_all[site_].g_buf_gr_,Algorithm_all[site_].g_binary_gr_,w/2,h/2,
                           manage.param_binary_.skip,
                           manage.param_binary_.bright_binary_radius_,
                           manage.param_binary_.bright_binary_spec_gr_,Ratio);
    p_binary[Gb]->setparam(Algorithm_all[site_].g_buf_gb_,Algorithm_all[site_].g_binary_gb_,w/2,h/2,
                           manage.param_binary_.skip,
                           manage.param_binary_.bright_binary_radius_,
                           manage.param_binary_.bright_binary_spec_gb_,Ratio);
    p_binary[B]->setparam(Algorithm_all[site_].g_buf_b_,Algorithm_all[site_].g_binary_b_,w/2,h/2,
                          manage.param_binary_.skip,
                          manage.param_binary_.bright_binary_radius_,
                          manage.param_binary_.bright_binary_spec_b_,Ratio);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }

    if(type==Binarization_4C)
        AlgorithmFuncs::RGGB_Map_Integrate_4C (Algorithm_all[site_].g_binary_r_,Algorithm_all[site_].g_binary_gr_,
                                               Algorithm_all[site_].g_binary_gb_,Algorithm_all[site_].g_binary_b_,
                                               Algorithm_all[site_].g_binary_,
                                               w,h,manage.bayer_);
    else if(type==Binarization_bayer)
        AlgorithmFuncs::RGGB_Map_Integrate_chao (Algorithm_all[site_].g_binary_r_,Algorithm_all[site_].g_binary_gr_,
                                            Algorithm_all[site_].g_binary_gb_,Algorithm_all[site_].g_binary_b_,
                                            Algorithm_all[site_].g_binary_,
                                            w,h, manage.bayer_);

    memcpy(pData_out,Algorithm_all[site_].g_binary_,w*h*sizeof(uchar));

    if(manage.flag_split_channel_)
    {
         QString image_title="site"+QString::number(site_);
        cv::Mat img_tmp = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=Algorithm_all[site_].g_binary_[i*w+j];
        }
        cv::imwrite(QString("./"+image_title+"binary_image.bmp").toStdString().c_str(),img_tmp);
    }

    return true;
}

//静态 坏点查询
bool MapBinarization::Static_MapBinarization_dark(const ParamManage& manage,
                                           MapBinarization_type type, uchar *pData_out)
{
    int w = manage.width_;
    int h = manage.height_;

    bool flag_newbuf = Algorithm_all[site_].New_Pointer_Buf (&Algorithm_all[site_].g_binary_,Result_area,
                                   Buf_uchar_8,w,h,2);

    if(flag_newbuf)
    {
        Algorithm_all[site_].g_binary_r_ =   Algorithm_all[site_].g_binary_  + w*h;
        Algorithm_all[site_].g_binary_gr_ =  Algorithm_all[site_].g_binary_r_+ w*h*1/4;
        Algorithm_all[site_].g_binary_gb_ =  Algorithm_all[site_].g_binary_r_+ w*h*2/4;
        Algorithm_all[site_].g_binary_b_ =   Algorithm_all[site_].g_binary_r_+ w*h*3/4;
    }
    else
        return false;

    MultiThread::bychannel_binarization *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_binarization();
    }

    p_binary[R]->setparam(Algorithm_all[site_].g_buf_r_,Algorithm_all[site_].g_binary_r_,w/2,h/2,
                          manage.param_binary_.skip,
                          manage.param_binary_.dark_binary_radius_,
                          manage.param_binary_.dark_binary_spec_r_,DN);
    p_binary[Gr]->setparam(Algorithm_all[site_].g_buf_gr_,Algorithm_all[site_].g_binary_gr_,w/2,h/2,
                           manage.param_binary_.skip,
                           manage.param_binary_.dark_binary_radius_,
                           manage.param_binary_.dark_binary_spec_gr_,DN);
    p_binary[Gb]->setparam(Algorithm_all[site_].g_buf_gb_,Algorithm_all[site_].g_binary_gb_,w/2,h/2,
                           manage.param_binary_.skip,
                           manage.param_binary_.dark_binary_radius_,
                           manage.param_binary_.dark_binary_spec_gb_,DN);
    p_binary[B]->setparam(Algorithm_all[site_].g_buf_b_,Algorithm_all[site_].g_binary_b_,w/2,h/2,
                          manage.param_binary_.skip,
                          manage.param_binary_.dark_binary_radius_,
                          manage.param_binary_.dark_binary_spec_b_,DN);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }

    if(type==Binarization_4C)
        AlgorithmFuncs::RGGB_Map_Integrate_4C (Algorithm_all[site_].g_binary_r_,Algorithm_all[site_].g_binary_gr_,
                                               Algorithm_all[site_].g_binary_gb_,Algorithm_all[site_].g_binary_b_,
                                               Algorithm_all[site_].g_binary_,
                                               w,h,manage.bayer_);
    else if(type==Binarization_bayer)
        AlgorithmFuncs::RGGB_Map_Integrate_chao (Algorithm_all[site_].g_binary_r_,Algorithm_all[site_].g_binary_gr_,
                                            Algorithm_all[site_].g_binary_gb_,Algorithm_all[site_].g_binary_b_,
                                            Algorithm_all[site_].g_binary_,
                                            w,h, manage.bayer_);

    memcpy(pData_out,Algorithm_all[site_].g_binary_,w*h*sizeof(uchar));

    if(manage.flag_split_channel_)
    {
        QString image_title="site"+QString::number(site_);
        cv::Mat img_tmp = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=Algorithm_all[site_].g_binary_[i*w+j];
        }
        cv::imwrite(QString("./"+image_title+"binary_image.bmp").toStdString().c_str(),img_tmp);
    }

    return true;
}

//8张图 闪点查询
bool MapBinarization::Flicker_MapBinarization(const ParamManage& manage,
                                              int frame_cnt, uchar *pData_out)
{
    int w = manage.width_;
    int h = manage.height_;

    uchar* g_binary_=NULL;
    uchar* g_binary_r_=NULL;
    uchar* g_binary_gr_=NULL;
    uchar* g_binary_gb_=NULL;
    uchar* g_binary_b_=NULL;

    bool flag_newbuf = Algorithm_all[site_].New_Pointer_Buf (&g_binary_,Alculate_area,
                                   Buf_uchar_8,w,h,2);

    if(flag_newbuf)
    {
        g_binary_r_ =   g_binary_ +  w*h ;
        g_binary_gr_ =  g_binary_r_+ w*h/4 ;
        g_binary_gb_ =  g_binary_r_+ w*h/4 *2;
        g_binary_b_ =   g_binary_r_+ w*h/4 *3;
    }
    else
        return false;

    MultiThread::bychannel_flicker_binarization_raw16 *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i] = new MultiThread::bychannel_flicker_binarization_raw16();
    }

    p_binary[R]->setparam(Algorithm_all[site_].g_buf_r_16_, g_binary_r_, w/2,h/2, frame_cnt,
                          manage.param_binary_.flicker_pix_binary_spec_);
    p_binary[Gr]->setparam(Algorithm_all[site_].g_buf_gr_16_, g_binary_gr_, w/2,h/2, frame_cnt,
                           manage.param_binary_.flicker_pix_binary_spec_);
    p_binary[Gb]->setparam(Algorithm_all[site_].g_buf_gb_16_, g_binary_gb_, w/2,h/2, frame_cnt,
                           manage.param_binary_.flicker_pix_binary_spec_);
    p_binary[B]->setparam(Algorithm_all[site_].g_buf_b_16_, g_binary_b_, w/2,h/2, frame_cnt,
                          manage.param_binary_.flicker_pix_binary_spec_);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }

    AlgorithmFuncs::RGGB_Map_Integrate_chao(g_binary_r_,g_binary_gr_,g_binary_gb_,
                                        g_binary_b_,g_binary_,w,h,manage.bayer_);
    if(manage.flag_split_channel_)
    {
        QString image_title="site"+QString::number(site_);
        cv::Mat img_tmp = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_binary_[i*w+j];
        }
        cv::imwrite(QString("./"+image_title+"binary_flicker_image.bmp").toStdString().c_str(),img_tmp);
    }

    memcpy(pData_out,g_binary_,w*h*sizeof(uchar));

    Algorithm_all[site_].Free_Pointer_Buf(&g_binary_);

    return 1;
}


//8张图 闪点查询  将图片分为4部分，多线程计算闪点
bool MapBinarization::Flicker_MapBinarization_chao(const ParamManage& manage,uchar *pData,
                                                   int frame_cnt, uchar *pData_out)
{
    int w = manage.width_;
    int h = manage.height_;

    uchar* g_binary_=NULL;
    bool flag_newbuf = Algorithm_all[site_].New_Pointer_Buf (&g_binary_,Result_area,
                                   Buf_uchar_8,w,h,1);
    if(!flag_newbuf)
        return false;

    MultiThread::bychannel_flicker_binarization_raw16_chao *p_binary[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
        p_binary[i] = new MultiThread::bychannel_flicker_binarization_raw16_chao();

    p_binary[0]->setparam(pData, g_binary_, w,h,0, frame_cnt,
                          manage.param_binary_.flicker_pix_binary_spec_);
    p_binary[1]->setparam(pData, g_binary_, w,h,h/4, frame_cnt,
                           manage.param_binary_.flicker_pix_binary_spec_);
    p_binary[2]->setparam(pData, g_binary_, w,h,h*2/4, frame_cnt,
                           manage.param_binary_.flicker_pix_binary_spec_);
    p_binary[3]->setparam(pData, g_binary_, w,h,h*3/4, frame_cnt,
                          manage.param_binary_.flicker_pix_binary_spec_);

    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->start();
    }
    for (int i=0; i < Channel_Num; i++)
    {
        p_binary[i]->wait();
    }

    memcpy(pData_out,g_binary_,w*h*sizeof(uchar));

    Algorithm_all[site_].Free_Pointer_Buf(&g_binary_);

    if(manage.flag_split_channel_)
    {
        QString image_title="site"+QString::number(site_);
        cv::Mat img_tmp = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp.ptr<uchar>(i);
            for(int j=0;j<w;j++)
                p[j]=g_binary_[i*w+j];
        }
        cv::imwrite(QString("./"+image_title+"binary_flicker_chao_image.bmp").toStdString().c_str(),img_tmp);
    }

    return 1;
}


