#include "LagTest.h"

int LagTest::TestFunc_Lag_bayer(ParamManage& manage,uchar *pData_Lag_HL_first_doublebyte,
                                uchar *pData_Lag_HL_end_doublebyte,int datatype)
{
    logMsg (std::string("into bayer lag frame test, check params first!"));
    if (manage.width_ <= 0 ||
        manage.height_ <= 0 ||
        (datatype != 10 && datatype != 12 && datatype != 14 && datatype != 16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    //分通道
    bool result_Split=Algorithm_all[site_].Split_Channel_TwoRaw16(manage,pData_Lag_HL_first_doublebyte,
                                                        pData_Lag_HL_end_doublebyte,1,true,datatype);
    if(!result_Split)
        return -1;

    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    items_results.push_back( Lag_Mean_Test(manage) );
    items_results.push_back( ROI_Mean_Test(manage) );


    Subitem_Result final_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);
    return  final_result.bin;
}

/*-------------    Lag 测试相关函数     ---------------*/
Subitem_Result LagTest::Lag_Mean_Test(const ParamManage& manage)
{
    if(manage.lag_grid_roi_version == "lag_grid_roi_V1.0")
        return Lag_Mean_Test_V1_0(manage);
}


Subitem_Result LagTest::Lag_Mean_Test_V1_0(const ParamManage& manage)
{
    Subitem_Result result_lag_mean("Lag Mean Test",true);

    int w = manage.width_;
    int h = manage.height_;

    if (manage.param_lag_grid_roi_.mean_lag_open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Lag Mean Test V1.0---------------------------\n"));

        double Mean_R_HL_first=0;
        double Mean_Gr_HL_first=0;
        double Mean_Gb_HL_first=0;
        double Mean_B_HL_first=0;
        TestItem::test_channel_mean_raw16(w,h,Mean_R_HL_first,Mean_Gr_HL_first,
                                          Mean_Gb_HL_first,Mean_B_HL_first,
                                          Algorithm_all[site_].g_buf_r_16_,Algorithm_all[site_].g_buf_gr_16_,
                                          Algorithm_all[site_].g_buf_gb_16_,Algorithm_all[site_].g_buf_b_16_);

        g_str_msg_all[site_]+=boost::str(boost::format("Mean_R_HL_first:%.4f, Mean_Gr_HL_first:%.4f, Mean_Gb_HL_first:%.4f, Mean_B_HL_first:%.4f\n")
                               % Mean_R_HL_first % Mean_Gr_HL_first % Mean_Gb_HL_first % Mean_B_HL_first);

        double Mean_R_HL_end=0;
        double Mean_Gr_HL_end=0;
        double Mean_Gb_HL_end=0;
        double Mean_B_HL_end=0;
        TestItem::test_channel_mean_raw16(w,h,Mean_R_HL_end,Mean_Gr_HL_end,
                                          Mean_Gb_HL_end,Mean_B_HL_end,
                                          Algorithm_all[site_].g_buf_r_16_tmp_,Algorithm_all[site_].g_buf_gr_16_tmp_,
                                          Algorithm_all[site_].g_buf_gb_16_tmp_,Algorithm_all[site_].g_buf_b_16_tmp_);

        g_str_msg_all[site_]+=boost::str(boost::format("Mean_R_HL_end:%.4f, Mean_Gr_HL_end:%.4f, Mean_Gb_HL_end:%.4f, Mean_B_HL_end:%.4f\n")
                               % Mean_R_HL_end % Mean_Gr_HL_end % Mean_Gb_HL_end % Mean_B_HL_end);

        double lag_r_value = Mean_R_HL_first - Mean_R_HL_end;
        double lag_gr_value = Mean_Gr_HL_first - Mean_Gr_HL_end;
        double lag_gb_value = Mean_Gb_HL_first - Mean_Gb_HL_end;
        double lag_b_value = Mean_B_HL_first - Mean_B_HL_end;


        if(lag_r_value>manage.param_lag_grid_roi_.lag_r_ ||
                lag_gr_value>manage.param_lag_grid_roi_.lag_gr_ ||
                lag_gb_value>manage.param_lag_grid_roi_.lag_gb_ ||
                lag_b_value>manage.param_lag_grid_roi_.lag_b_)
        {
            result_lag_mean.is_passed = false;
            result_lag_mean.bin = manage.param_lag_grid_roi_.bin_lag_;
            result_lag_mean.grade= manage.param_lag_grid_roi_.grade_lag_;
        }

        logMsg (std::string("after lag test"));
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|")
                               % lag_r_value % lag_gr_value % lag_gb_value % lag_b_value);
        g_str_msg_all[site_]+=boost::str(boost::format("lag_r:%.4f, lag_gr:%.4f, lag_gb:%.4f, lag_b:%.4f\n")
                               % lag_r_value % lag_gr_value % lag_gb_value % lag_b_value);

        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                        "lag_r_value=%.4f (MAX: %.4f) \n"
                                                        "lag_gr_value=%.4f (MAX: %.4f) \n"
                                                        "lag_gb_value=%.4f (MAX: %.4f) \n"
                                                        "lag_b_value=%.4f (MAX: %.4f) \n")
                                            % lag_r_value
                                            % manage.param_lag_grid_roi_.lag_r_
                                            % lag_gr_value
                                            % manage.param_lag_grid_roi_.lag_gr_
                                            % lag_gb_value
                                            % manage.param_lag_grid_roi_.lag_gb_
                                            % lag_b_value
                                            % manage.param_lag_grid_roi_.lag_b_);

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_lag_mean.name.c_str());

    }
    else
    {
        result_lag_mean.g_msg = false;
    }

    return result_lag_mean;
}


Subitem_Result LagTest::ROI_Mean_Test(const ParamManage& manage)
{
    if(manage.lag_grid_roi_version == "lag_grid_roi_V1.0")
        return ROI_Mean_Test_V1_0(manage);
}


Subitem_Result LagTest::ROI_Mean_Test_V1_0(const ParamManage& manage)
{
    int w = manage.width_;
    int h = manage.height_;

    Subitem_Result result_roi_lag("ROI Mean Test",true);

    if (manage.param_lag_grid_roi_.roi_lag_open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Roi Lag Test---------------------------\n"));

        short *p_short = NULL;
        if( !Algorithm_all[site_].New_Pointer_Buf( (uchar **)&p_short,Alculate_area,Buf_short,w,h,2) )
        {
            result_roi_lag.bin =-2;
            result_roi_lag.grade =50;
            return result_roi_lag;
        }

        p_diff_ =  p_short ;
        p_diff_r =  p_short + w*h ;
        p_diff_gr = p_short + w*h+ w*h/4;
        p_diff_gb = p_short + w*h+ w*h/4*2;
        p_diff_b =  p_short + w*h+ w*h/4*3;

        short *p_HL_first= (short*) Algorithm_all[site_].g_buf_r_16_;
        short *p_HL_end= (short*) Algorithm_all[site_].g_buf_r_16_tmp_;

        // 求解四个通道差值
        for(int i=0; i<w*h; ++i)
            p_diff_[i]= (p_HL_first[i]&0x03ff) - (p_HL_end[i]&0x03ff);

        double lag_row_up = 0;
        double lag_row_down = 0;
        double lag_col_left = 0;
        double lag_col_right = 0;
        int row_up = manage.param_lag_grid_roi_.row_up_;
        int row_down = manage.param_lag_grid_roi_.row_down_;
        int col_left = manage.param_lag_grid_roi_.col_left_;
        int col_right = manage.param_lag_grid_roi_.col_right_;
        for(int i=0; i<h; ++i)
        {
            for(int j=0; j<w; ++j)
            {
                if(i<row_up)
                    lag_row_up += p_diff_[i*w+j];
                if(i>=h-row_down)
                    lag_row_down += p_diff_[i*w+j];
                if(j<col_left)
                    lag_col_left += p_diff_[i*w+j];
                if(j>=w-col_right)
                    lag_col_right += p_diff_[i*w+j];
            }
        }
        lag_row_up = lag_row_up/(w*row_up);
        lag_row_down = lag_row_down/(w*row_down);
        lag_col_left = lag_col_left/(h*col_left);
        lag_col_right = lag_col_right/(h*col_right);

        logMsg (std::string("after edge lag test"));
//        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|")
//                               % lag_row_up % lag_row_down % lag_col_left % lag_col_right);
        g_str_msg_all[site_]+=boost::str(boost::format("lag_row_up:%.4f, lag_row_down:%.4f, lag_col_left:%.4f, lag_col_right:%.4f\n")
                               % lag_row_up % lag_row_down % lag_col_left % lag_col_right);

        //Lag Test
        TestItem::split_channel_short(manage.bayer_,w, h, p_diff_,
                                      p_diff_r, p_diff_gr, p_diff_gb, p_diff_b);

        logMsg (std::string("Mark2"));
        MultiThread::get_grid_roi_mean_raw16 *p_grid_roi[Channel_Num];
        for (int i=0; i < Channel_Num; i++)
        {
            p_grid_roi[i] = new MultiThread::get_grid_roi_mean_raw16();
        }

        int x_cnt = manage.param_lag_grid_roi_.X_Grid_Cnt_;
        int y_cnt = manage.param_lag_grid_roi_.Y_Grid_Cnt_;

        p_grid_roi[R]->setparam(p_diff_r,  w/2, h/2, x_cnt, y_cnt);
        p_grid_roi[Gr]->setparam(p_diff_gr,w/2, h/2, x_cnt, y_cnt);
        p_grid_roi[Gb]->setparam(p_diff_gb,w/2, h/2, x_cnt, y_cnt);
        p_grid_roi[B]->setparam(p_diff_b,  w/2, h/2, x_cnt, y_cnt);

        for (int i=0; i < Channel_Num; i++)
            p_grid_roi[i]->start();

        for (int i=0; i < Channel_Num; i++)
            p_grid_roi[i]->wait();

        logMsg (std::string("Mark3"));


        QVector<double> lag_r_GridROI_Mean(p_grid_roi[R]->GridROI_Mean);
        QVector<double> lag_gr_GridROI_Mean(p_grid_roi[Gr]->GridROI_Mean);
        QVector<double> lag_gb_GridROI_Mean(p_grid_roi[Gb]->GridROI_Mean);
        QVector<double> lag_b_GridROI_Mean(p_grid_roi[B]->GridROI_Mean);

        qSort(lag_r_GridROI_Mean.begin(),lag_r_GridROI_Mean.end(),qLess<double>());
        qSort(lag_gr_GridROI_Mean.begin(),lag_gr_GridROI_Mean.end(),qLess<double>());
        qSort(lag_gb_GridROI_Mean.begin(),lag_gb_GridROI_Mean.end(),qLess<double>());
        qSort(lag_b_GridROI_Mean.begin(),lag_b_GridROI_Mean.end(),qLess<double>());

        for(int cnt=0; cnt<1; cnt++)
        {
            double max_lag_r = lag_r_GridROI_Mean.at(lag_r_GridROI_Mean.size()-cnt-1);
            int max_lag_r_pos = p_grid_roi[R]->GridROI_Mean.indexOf(max_lag_r);

            double min_lag_r = lag_r_GridROI_Mean.at(cnt);
            int min_lag_r_pos = p_grid_roi[R]->GridROI_Mean.indexOf(min_lag_r);


            double max_lag_gr = lag_gr_GridROI_Mean.at(lag_gr_GridROI_Mean.size()-cnt-1);
            int max_lag_gr_pos = p_grid_roi[Gr]->GridROI_Mean.indexOf(max_lag_gr);

            double min_lag_gr = lag_gr_GridROI_Mean.at(cnt);
            int min_lag_gr_pos = p_grid_roi[Gr]->GridROI_Mean.indexOf(min_lag_gr);


            double max_lag_gb = lag_gb_GridROI_Mean.at(lag_gb_GridROI_Mean.size()-cnt-1);
            int max_lag_gb_pos = p_grid_roi[Gb]->GridROI_Mean.indexOf(max_lag_gb);

            double min_lag_gb = lag_gb_GridROI_Mean.at(cnt);
            int min_lag_gb_pos = p_grid_roi[Gb]->GridROI_Mean.indexOf(min_lag_gb);

            double max_lag_b = lag_b_GridROI_Mean.at(lag_b_GridROI_Mean.size()-cnt-1);
            int max_lag_b_pos = p_grid_roi[B]->GridROI_Mean.indexOf(max_lag_b);

            double min_lag_b = lag_b_GridROI_Mean.at(cnt);
            int min_lag_b_pos = p_grid_roi[B]->GridROI_Mean.indexOf(min_lag_b);


            g_str_log_all[site_]+=boost::str(boost::format("%.4f|%d|%.4f|%d|%.4f|%d|%.4f|%d|%.4f|%d|%.4f|%d|%.4f|%d|%.4f|%d|")
                                   % max_lag_r % max_lag_r_pos % min_lag_r % min_lag_r_pos % max_lag_gr % max_lag_gr_pos % min_lag_gr % min_lag_gr_pos
                                   % max_lag_gb % max_lag_gb_pos % min_lag_gb % min_lag_gb_pos % max_lag_b % max_lag_b_pos % min_lag_b % min_lag_b_pos);

            if(cnt==0)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("max_lag_r_1:%.4f,  max_lag_r_1_pos:%d,  min_lag_r_1:%.4f,  min_lag_r_1_pos:%d\n"
                                                     "max_lag_gr_1:%.4f, max_lag_gr_1_pos:%d, min_lag_gr_1:%.4f, min_lag_gr_1_pos:%d\n"
                                                     "max_lag_gb_1:%.4f, max_lag_gb_1_pos:%d, min_lag_gb_1:%.4f, min_lag_gb_1_pos:%d\n"
                                                     "max_lag_b_1:%.4f,  max_lag_b_1_pos:%d,  min_lag_b_1:%.4f,  min_lag_b_1_pos:%d\n")
                                       % max_lag_r % max_lag_r_pos % min_lag_r % min_lag_r_pos % max_lag_gr % max_lag_gr_pos % min_lag_gr % min_lag_gr_pos
                                       % max_lag_gb % max_lag_gb_pos % min_lag_gb % min_lag_gb_pos % max_lag_b % max_lag_b_pos % min_lag_b % min_lag_b_pos);


                if( max_lag_r>manage.param_lag_grid_roi_.Roi_Lag_R_Max_ ||
                        min_lag_r<manage.param_lag_grid_roi_.Roi_Lag_R_Min_ ||
                        max_lag_gr>manage.param_lag_grid_roi_.Roi_Lag_Gr_Max_ ||
                        min_lag_gr<manage.param_lag_grid_roi_.Roi_Lag_Gr_Min_ ||
                        max_lag_gb>manage.param_lag_grid_roi_.Roi_Lag_Gb_Max_ ||
                        min_lag_gb<manage.param_lag_grid_roi_.Roi_Lag_Gb_Min_ ||
                        max_lag_b>manage.param_lag_grid_roi_.Roi_Lag_B_Max_ ||
                        min_lag_b<manage.param_lag_grid_roi_.Roi_Lag_B_Min_
                        )
                {
                    result_roi_lag.is_passed = false;
                    result_roi_lag.bin = manage.param_lag_grid_roi_.bin_roi_lag_;
                    result_roi_lag.grade = manage.param_lag_grid_roi_.grade_roi_lag_;
                }
            }
            else if(cnt==1)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("max_lag_r_2:%.4f,  max_lag_r_2_pos:%d,  min_lag_r_2:%.4f,  min_lag_r_2_pos:%d\n"
                                                     "max_lag_gr_2:%.4f, max_lag_gr_2_pos:%d, min_lag_gr_2:%.4f, min_lag_gr_2_pos:%d\n"
                                                     "max_lag_gb_2:%.4f, max_lag_gb_2_pos:%d, min_lag_gb_2:%.4f, min_lag_gb_2_pos:%d\n"
                                                     "max_lag_b_2:%.4f,  max_lag_b_2_pos:%d,  min_lag_b_2:%.4f,  min_lag_b_2_pos:%d\n")
                                       % max_lag_r % max_lag_r_pos % min_lag_r % min_lag_r_pos % max_lag_gr % max_lag_gr_pos % min_lag_gr % min_lag_gr_pos
                                       % max_lag_gb % max_lag_gb_pos % min_lag_gb % min_lag_gb_pos % max_lag_b % max_lag_b_pos % min_lag_b % min_lag_b_pos);
            }
            else if(cnt==2)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("max_lag_r_3:%.4f,  max_lag_r_3_pos:%d,  min_lag_r_3:%.4f,  min_lag_r_3_pos:%d\n"
                                                     "max_lag_gr_3:%.4f, max_lag_gr_3_pos:%d, min_lag_gr_3:%.4f, min_lag_gr_3_pos:%d\n"
                                                     "max_lag_gb_3:%.4f, max_lag_gb_3_pos:%d, min_lag_gb_3:%.4f, min_lag_gb_3_pos:%d\n"
                                                     "max_lag_b_3:%.4f,  max_lag_b_3_pos:%d,  min_lag_b_3:%.4f,  min_lag_b_3_pos:%d\n")
                                       % max_lag_r % max_lag_r_pos % min_lag_r % min_lag_r_pos % max_lag_gr % max_lag_gr_pos % min_lag_gr % min_lag_gr_pos
                                       % max_lag_gb % max_lag_gb_pos % min_lag_gb % min_lag_gb_pos % max_lag_b % max_lag_b_pos % min_lag_b % min_lag_b_pos);
            }


            g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                            "max_lag_r=%.4f (Max: %.4f, Min: %.4f) \n"
                                                            "min_lag_r=%.4f (Max: %.4f, Min: %.4f) \n"
                                                            "max_lag_gr=%.4f (Max: %.4f, Min: %.4f) \n"
                                                            "min_lag_gr=%.4f (Max: %.4f, Min: %.4f) \n"
                                                            "max_lag_gb=%.4f (Max: %.4f, Min: %.4f) \n"
                                                            "min_lag_gb=%.4f (Max: %.4f, Min: %.4f) \n"
                                                            "max_lag_b=%.4f (Max: %.4f, Min: %.4f) \n"
                                                            "min_lag_b=%.4f (Max: %.4f, Min: %.4f) \n")
                                                % max_lag_r % manage.param_lag_grid_roi_.Roi_Lag_R_Max_ % manage.param_lag_grid_roi_.Roi_Lag_R_Min_
                                                % min_lag_r % manage.param_lag_grid_roi_.Roi_Lag_R_Max_ % manage.param_lag_grid_roi_.Roi_Lag_R_Min_
                                                % max_lag_gr % manage.param_lag_grid_roi_.Roi_Lag_Gr_Max_ % manage.param_lag_grid_roi_.Roi_Lag_Gr_Min_
                                                % min_lag_gr % manage.param_lag_grid_roi_.Roi_Lag_Gr_Max_ % manage.param_lag_grid_roi_.Roi_Lag_Gr_Min_
                                                % max_lag_gb % manage.param_lag_grid_roi_.Roi_Lag_Gb_Max_ % manage.param_lag_grid_roi_.Roi_Lag_Gb_Min_
                                                % min_lag_gb % manage.param_lag_grid_roi_.Roi_Lag_Gb_Max_ % manage.param_lag_grid_roi_.Roi_Lag_Gb_Min_
                                                % max_lag_b % manage.param_lag_grid_roi_.Roi_Lag_B_Max_ % manage.param_lag_grid_roi_.Roi_Lag_B_Min_
                                                % min_lag_b % manage.param_lag_grid_roi_.Roi_Lag_B_Max_ % manage.param_lag_grid_roi_.Roi_Lag_B_Min_);




        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_roi_lag.name.c_str());

        for(int i=0; i< Channel_Num; i++)
            delete p_grid_roi[i];

        Algorithm_all[site_].Free_Pointer_Buf((uchar **)&p_short);
    }
    else
    {
        result_roi_lag.g_msg = false;
    }

    return result_roi_lag;
}


