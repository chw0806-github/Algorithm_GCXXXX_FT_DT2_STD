#include "DarkCurrent.h"

int DarkCurrent::TestFunc_Darkcurrent(ParamManage& manage,uchar *pData_long_exp,uchar *pData_short_exp)
{
    logMsg (std::string("into TestFunc_Darkcurrent test, check params first!"));
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16) )
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    uchar *pImage_save=NULL;
    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合
    ParamManage manage_not_return_image=manage;
    manage_not_return_image.flag_raw8_image_=0;

    //long Response
    logMsg ("Start long Response!");
    if(!Algorithm_all[site_].Split_Channel(manage_not_return_image, test_channel_type,false,
                                           pData_long_exp,1,test_datatype,pImage_save))
        return -1;

    items_results.push_back(Algorithm_all[site_].Light_Response(manage));
    Algorithm_all[site_]. Free_result_buf();

    //short Response
    logMsg ("Start short Response!");
    if(!Algorithm_all[site_].Split_Channel(manage_not_return_image, test_channel_type,false,
                                           pData_short_exp,1,test_datatype,pImage_save))
        return -1;

    items_results.push_back(Algorithm_all[site_].Light_Response(manage));
    Algorithm_all[site_]. Free_result_buf();

    logMsg ("Start TestFunc_Darkcurrent_test!");
    bool result_Split= Algorithm_all[site_].Split_Channel_TwoRaw16(manage,pData_long_exp,pData_short_exp,1,true,test_datatype);
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }

    items_results.push_back(Global_Darkcurrent_test(manage,(short *) pData_long_exp,(short *) pData_short_exp));

    Subitem_Result dark_test_final_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);

    return dark_test_final_result.bin;
}


Subitem_Result DarkCurrent::TestFunc_Darkcurrent_test(const ParamManage& manage,Darkcurrent_type type,
                                          short * p_long_exp,short * p_short_exp)
{
    Subitem_Result Darkcurrent_res("TestFunc_Darkcurrent_res",true);
    int w = manage.width_;
    int h = manage.height_;

    int skip = manage.param_darkcurrent_spec_.skip_;
    int dark_area_line_total_cnt = manage.param_darkcurrent_spec_.dark_area_line_total_cnt_;
    int dark_area_line_center_cnt = manage.param_darkcurrent_spec_.dark_area_line_center_cnt_;
    int roi_w= manage.param_darkcurrent_spec_.roi_w_;
    int roi_h= manage.param_darkcurrent_spec_.roi_h_;

    QVector < QPair<double,double> > spec;
    spec.append(QPair<double,double>(manage.param_darkcurrent_spec_.DC_dark_area_min_,
                                     manage.param_darkcurrent_spec_.DC_dark_area_max_));
    spec.append(QPair<double,double>(manage.param_darkcurrent_spec_.DC_leftup_min_,
                                     manage.param_darkcurrent_spec_.DC_leftup_max_));
    spec.append(QPair<double,double>(manage.param_darkcurrent_spec_.DC_rightup_min_,
                                     manage.param_darkcurrent_spec_.DC_rightup_max_));
    spec.append(QPair<double,double>(manage.param_darkcurrent_spec_.DC_leftdown_min_,
                                     manage.param_darkcurrent_spec_.DC_leftdown_max_));
    spec.append(QPair<double,double>(manage.param_darkcurrent_spec_.DC_rightdown_min_,
                                     manage.param_darkcurrent_spec_.DC_rightdown_max_));
    spec.append(QPair<double,double>(manage.param_darkcurrent_spec_.DC_center_min_,
                                     manage.param_darkcurrent_spec_.DC_center_max_));
    spec.append(QPair<double,double>(manage.param_darkcurrent_spec_.DC_whole_area_min_,
                                     manage.param_darkcurrent_spec_.DC_whole_area_max_));

    if(type==Darkcurrent_normal)  //不需要分通道
    {
        QVector <cv::Rect> roi;
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(skip,(dark_area_line_total_cnt-dark_area_line_center_cnt)/2,w-1-2*skip,dark_area_line_center_cnt));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(skip,skip+dark_area_line_total_cnt,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(w-1-skip-roi_w,skip+dark_area_line_total_cnt,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(skip,h-1-skip-roi_h,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(w-1-skip-roi_w,h-1-skip-roi_h,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(w/2-1-roi_w/2,h/2-1-roi_h/2,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(skip,skip,w-2*skip,h-2*skip));

        short *p_diff = NULL;
        bool flag = Algorithm_all[site_].New_Pointer_Buf( (uchar**) &p_diff,
                                   Alculate_area,Buf_short,w,h,1);
        if(!flag)
        {
            Darkcurrent_res.bin =-2;
            Darkcurrent_res.grade =50;
            return Darkcurrent_res;
        }

        for(int i=0; i<w*h; ++i)
            p_diff[i]= p_long_exp[i] - p_short_exp[i];

        QVector <double> roi_dark_current;

        for(int n=0; n<7; n++)
            roi_dark_current.append(AlgorithmFuncs::GetROIMean_short (p_diff, w, h, roi.at(n).x, roi.at(n).y, roi.at(n).width, roi.at(n).height));

        for(int n=0; n<7; n++)
            if(roi_dark_current.at(n)<spec.at(n).first || roi_dark_current.at(n)>spec.at(n).second)
            {
                Darkcurrent_res.is_passed = false;
                Darkcurrent_res.bin = manage.param_darkcurrent_spec_.bin_darkcurrent_;
                Darkcurrent_res.grade = manage.param_darkcurrent_spec_.grade_darkcurrent_;
            }

        logMsg (std::string("after darkcurrent test"));

        for(int n=0; n<7; n++)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("Roi_%d_Darkcurrent:%.4f\n")
                                   % n % roi_dark_current.at(n));

            g_str_log_all[site_]+=boost::str(boost::format("%.4f|")
                                   % roi_dark_current.at(n));
        }
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % Darkcurrent_res.name.c_str());
    }

    else if(type==Darkcurrent_4Channel)
    {
        short *p_long_r = (short *) Algorithm_all[site_].g_buf_r_16_;
        short *p_long_gr = (short *) Algorithm_all[site_].g_buf_gr_16_;
        short *p_long_gb = (short *) Algorithm_all[site_].g_buf_gb_16_;
        short *p_long_b = (short *) Algorithm_all[site_].g_buf_b_16_;

        short *p_short_r = (short *) Algorithm_all[site_].g_buf_r_16_tmp_;
        short *p_short_gr = (short *) Algorithm_all[site_].g_buf_gr_16_tmp_;
        short *p_short_gb = (short *) Algorithm_all[site_].g_buf_gb_16_tmp_;
        short *p_short_b = (short *) Algorithm_all[site_].g_buf_b_16_tmp_;

        int size = w*h/4;
        short *p_diff = NULL;
        bool flag = Algorithm_all[site_].New_Pointer_Buf( (uchar**) &p_diff,
                                   Alculate_area,Buf_short,w,h,1);
        if(!flag)
        {
            Darkcurrent_res.bin =-2;
            Darkcurrent_res.grade =50;
            return Darkcurrent_res;
        }

        short *p_diff_r = p_diff;
        short *p_diff_gr = p_diff + size*1;
        short *p_diff_gb = p_diff + size*2;
        short *p_diff_b = p_diff + size*3;

        for(int i=0; i<size; ++i)
        {
            p_diff_r[i]=p_long_r[i] - p_short_r[i];
            p_diff_gr[i]=p_long_gr[i] - p_short_gr[i];
            p_diff_gb[i]=p_long_gb[i] - p_short_gb[i];
            p_diff_b[i]=p_long_b[i] - p_short_b[i];
        }

        skip = skip/2;
        dark_area_line_total_cnt = dark_area_line_total_cnt/2;
        dark_area_line_center_cnt =dark_area_line_center_cnt/2;
        roi_w= roi_w/2;
        roi_h= roi_h/2;

        int w_ch = w/2;
        int h_ch = h/2;

        QVector <cv::Rect> roi;
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(skip,(dark_area_line_total_cnt-dark_area_line_center_cnt)/2,w_ch-1-2*skip,dark_area_line_center_cnt));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(skip,skip+dark_area_line_total_cnt,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(w_ch-1-skip-roi_w,skip+dark_area_line_total_cnt,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(skip,h_ch-1-skip-roi_h,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(w_ch-1-skip-roi_w,h_ch-1-skip-roi_h,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(w_ch/2-1-roi_w/2,h_ch/2-1-roi_h/2,roi_w,roi_h));
        roi.append(cv::Rect(0,0,w-1,h-1) & cv::Rect(skip,skip,w_ch-2*skip,h_ch-2*skip));


        MultiThread::get_mutil_roi_mean_short *p_roi[4*7];
        for (int i=0; i<28; i++)
            p_roi[i] = new MultiThread::get_mutil_roi_mean_short();

        for (int i=0; i<7; i++)
        {
            p_roi[i]->setparam(p_long_r,p_short_r,p_diff_r,w_ch,h_ch,roi.at(i).x,roi.at(i).y,roi.at(i).width,roi.at(i).height);
            p_roi[i+7]->setparam(p_long_gr,p_short_gr,p_diff_gr,w_ch,h_ch,roi.at(i).x,roi.at(i).y,roi.at(i).width,roi.at(i).height);
            p_roi[i+14]->setparam(p_long_gb,p_short_gb,p_diff_gb,w_ch,h_ch,roi.at(i).x,roi.at(i).y,roi.at(i).width,roi.at(i).height);
            p_roi[i+21]->setparam(p_long_b,p_short_b,p_diff_b,w_ch,h_ch,roi.at(i).x,roi.at(i).y,roi.at(i).width,roi.at(i).height);
        }

        for (int i=0; i<28; i++)
            p_roi[i]->start();

        for (int i=0; i<28; i++)
            p_roi[i]->wait();


        QVector <double> roi_dark_current;
        for(int n=0; n<28; n++)
        {
            roi_dark_current.append(p_roi[n]->value_log.at(2));

            if(n>20)
                g_str_msg_all[site_]+=boost::str(boost::format("B_Channel_Roi_%d_Long_Exp_Mean:%.4f, Short_Exp_Mean:%.4f\n")
                                       % (n-21) % p_roi[n]->value_log.at(0) % p_roi[n]->value_log.at(1));
            else if(n>13)
                g_str_msg_all[site_]+=boost::str(boost::format("Gb_Channel_Roi_%d_Long_Exp_Mean:%.4f, Short_Exp_Mean:%.4f\n")
                                       % (n-14) % p_roi[n]->value_log.at(0) % p_roi[n]->value_log.at(1));
            else if(n>6)
                g_str_msg_all[site_]+=boost::str(boost::format("Gr_Channel_Roi_%d_Long_Exp_Mean:%.4f, Short_Exp_Mean:%.4f\n")
                                       % (n-7) % p_roi[n]->value_log.at(0) % p_roi[n]->value_log.at(1));
            else
                g_str_msg_all[site_]+=boost::str(boost::format("R_Channel_Roi_%d_Long_Exp_Mean:%.4f, Short_Exp_Mean:%.4f\n")
                                       % n % p_roi[n]->value_log.at(0) % p_roi[n]->value_log.at(1));

            g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|")
                                   % p_roi[n]->value_log.at(0) % p_roi[n]->value_log.at(1));
        }

        bool result_flag = true;
        for(int n=0; n<7; n++)
        {
            if(roi_dark_current.at(n)<spec.at(n).first || roi_dark_current.at(n)>spec.at(n).second)
                result_flag = false;
            if(roi_dark_current.at(n+7)<spec.at(n).first || roi_dark_current.at(n+7)>spec.at(n).second)
                result_flag = false;
            if(roi_dark_current.at(n+14)<spec.at(n).first || roi_dark_current.at(n+14)>spec.at(n).second)
                result_flag = false;
            if(roi_dark_current.at(n+21)<spec.at(n).first || roi_dark_current.at(n+21)>spec.at(n).second)
                result_flag = false;
        }

        if(!result_flag)
        {
            Darkcurrent_res.is_passed = false;
            Darkcurrent_res.bin = manage.param_darkcurrent_spec_.bin_darkcurrent_;
            Darkcurrent_res.grade = manage.param_darkcurrent_spec_.grade_darkcurrent_;
        }
        logMsg (std::string("after darkcurrent test"));

        for(int n=0; n<28; n++)
        {
            if(n>20)
                g_str_msg_all[site_]+=boost::str(boost::format("B_Channel_Roi_%d_Darkcurrent:%.4f\n")
                                       % (n-21) % roi_dark_current.at(n));
            else if(n>13)
                g_str_msg_all[site_]+=boost::str(boost::format("Gb_Channel_Roi_%d_Darkcurrent:%.4f\n")
                                       % (n-14) % roi_dark_current.at(n));
            else if(n>6)
                g_str_msg_all[site_]+=boost::str(boost::format("Gr_Channel_Roi_%d_Darkcurrent:%.4f\n")
                                       % (n-7) % roi_dark_current.at(n));
            else
                g_str_msg_all[site_]+=boost::str(boost::format("R_Channel_Roi_%d_Darkcurrent:%.4f\n")
                                       % n % roi_dark_current.at(n));

            g_str_log_all[site_]+=boost::str(boost::format("%.4f|")
                                   % roi_dark_current.at(n));
        }
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % Darkcurrent_res.name.c_str());

        Algorithm_all[site_].Free_Pointer_Buf( (uchar**) &p_diff);
    }

    return Darkcurrent_res;

}


Subitem_Result DarkCurrent::Global_Darkcurrent_test(const ParamManage& manage,  short * p_long_exp,short * p_short_exp)
{
    Subitem_Result Darkcurrent_res("Global_Darkcurrent_res",true);

    int skip_x = manage.param_dakrcurrent_global_.skip_x_;
    int skip_y = manage.param_dakrcurrent_global_.skip_y_;
    int roi_w= manage.param_dakrcurrent_global_.roi_w_;
    int roi_h= manage.param_dakrcurrent_global_.roi_h_;
    int width = manage.width_;
    int height = manage.height_;
    int dark_height = manage.param_dakrcurrent_global_.dark_height_;

    QVector <cv::Rect> roi;
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect(skip_x,skip_y,roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect((width/2 - roi_w/2),skip_y,roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect((width-roi_w-skip_x),skip_y,roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect(skip_x,(height/2-roi_h/2),roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect((width/2 - roi_w/2),(height/2-roi_h/2),roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect((width-roi_w-skip_x),(height/2-roi_h/2),roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect(skip_x,(height-roi_h-skip_y),roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect((width/2-roi_w/2),(height-roi_h-skip_y),roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect((width-roi_w-skip_x),(height-roi_h-skip_y),roi_w,roi_h));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect(roi_w,2,(width-roi_w),(dark_height/2-4)));
    roi.append(cv::Rect(0,0,width-1,height-1) & cv::Rect(roi_w,(dark_height/2+2),(width-roi_w),(dark_height/2-4)));

    short *p_diff = NULL;
    bool flag = Algorithm_all[site_].New_Pointer_Buf( (uchar**) &p_diff,
                                                      Alculate_area,Buf_short,width,height,1);
    if(!flag)
    {
        Darkcurrent_res.bin =-2;
        return Darkcurrent_res;
    }

    for(int i=0; i<width*height; ++i)
        p_diff[i]= p_long_exp[i] - p_short_exp[i];

    QVector <double> roi_dark_current;

    for(int n=0; n<11; n++)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("x: %.4f, y: %.4f, width: %.4f, height: %.4f \n") % roi.at(n).x % roi.at(n).y % roi.at(n).width % roi.at(n).height);
        roi_dark_current.append(AlgorithmFuncs::GetROIMean_short (p_diff, width, height, roi.at(n).x, roi.at(n).y, roi.at(n).width, roi.at(n).height));
    }

    for(int n=0; n<11; n++)
        if(roi_dark_current.at(n)<manage.param_dakrcurrent_global_.roi_dkc_spec_min_.at(n) ||
                roi_dark_current.at(n)>manage.param_dakrcurrent_global_.roi_dkc_spec_max_.at(n))
        {
            Darkcurrent_res.is_passed = false;
            Darkcurrent_res.bin = manage.param_dakrcurrent_global_.bin_darkcurrent_;
            Darkcurrent_res.grade = manage.param_dakrcurrent_global_.grade_darkcurrent_;
        }

    logMsg (std::string("after darkcurrent test"));

    for(int n=0; n<11; n++)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("Roi_%d_Darkcurrent:%.4f\n")
                                         % n % roi_dark_current.at(n));

        g_str_log_all[site_]+=boost::str(boost::format("%.4f|")
                                         % roi_dark_current.at(n));
    }
    g_str_log_all[site_]+=boost::str(boost::format("%s|") % Darkcurrent_res.name.c_str());

    return Darkcurrent_res;

}
