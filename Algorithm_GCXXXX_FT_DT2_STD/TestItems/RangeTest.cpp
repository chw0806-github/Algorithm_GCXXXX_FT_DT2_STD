#include "RangeTest.h"

int  RangeTest::TestFunc_Range_Bayer(ParamManage& manage,uchar *pData,uchar *pImage_save)
{
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    //分配空间并分通道
    bool result_Split= Algorithm_all[site_].Split_Channel(manage, Split_Bayer,false,
                                    pData,test_frameCnt,test_datatype,pImage_save);
    if(!result_Split)
        return -1;

    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    items_results.push_back( Algorithm_all[site_].Light_Response(manage) );
    items_results.push_back( Algorithm_all[site_].Relative_Illumination(manage) );
    items_results.push_back( Algorithm_all[site_].Deadrow_Deadcol_bright(manage,false) );
    items_results.push_back( ROI_MEAN_and_STD(manage) );

    Subitem_Result range_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);
    logMsg (boost::str(boost::format("final bin:%d") % range_result.bin));
    return range_result.bin;
}

Subitem_Result RangeTest::ROI_MEAN_and_STD(const ParamManage& manage)
{
    if(manage.lag_roi_std_version == "lag_roi_std_V1.0")
        return ROI_MEAN_and_STD_V1_0(manage);

    Subitem_Result res("ROI_MEAN_and_STD", false);
    res.g_msg = false;
    return res;
}



Subitem_Result RangeTest::ROI_MEAN_and_STD_V1_0(const ParamManage& manage)
{
    int w = manage.width_;
    int h = manage.height_;

    Subitem_Result result_roi_std("ROI_MEAN_and_STD",true);

    if(manage.param_roi_std_.open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------ROI MEAN and STD V1.0---------------------------\n"));

        QVector <double> roi_mean;
        QVector <double> roi_std;

        double min_roi_mean = 0;
        int min_roi_mean_pos = 0;

        double max_roi_mean = 0;
        int max_roi_mean_pos = 0;

        double min_roi_std = 0;
        int min_roi_std_pos = 0;

        double max_roi_std = 0;
        int max_roi_std_pos = 0;

        uchar *pdata[Channel_Num];
        pdata[0]=Algorithm_all[site_].g_buf_r_;
        pdata[1]=Algorithm_all[site_].g_buf_gr_;
        pdata[2]=Algorithm_all[site_].g_buf_gb_;
        pdata[3]=Algorithm_all[site_].g_buf_b_;

        for (int ch=0; ch<Channel_Num; ch++)
        {
            roi_mean.clear();
            roi_std.clear();
            TestItem::test_MxN_roi_std_mean(pdata[ch], w/2, h/2,
                                            manage.param_roi_std_.strip_col_/2,
                                            manage.param_roi_std_.strip_row_/2,
                                            manage.param_roi_std_.col_roi_cnt_,
                                            manage.param_roi_std_.row_roi_cnt_,
                                            roi_mean, roi_std);

            min_roi_mean = *std::min_element(roi_mean.begin(),roi_mean.end());
            min_roi_mean_pos = roi_mean.indexOf(min_roi_mean);

            max_roi_mean = *std::max_element(roi_mean.begin(),roi_mean.end());
            max_roi_mean_pos = roi_mean.indexOf(max_roi_mean);


            min_roi_std = *std::min_element(roi_std.begin(),roi_std.end());
            min_roi_std_pos = roi_std.indexOf(min_roi_std);

            max_roi_std = *std::max_element(roi_std.begin(),roi_std.end());
            max_roi_std_pos = roi_std.indexOf(max_roi_std);

            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%d|%.2f|%d|%.2f|%d|%.2f|%d|")
                                   % min_roi_mean % min_roi_mean_pos % max_roi_mean % max_roi_mean_pos
                                   % min_roi_std % min_roi_std_pos % max_roi_std % max_roi_std_pos);
            if(ch==R)
            {
                g_str_msg_all[site_]+=boost::str(boost::format(" min_roi_r_mean:%.2f,  min_roi_r_mean_pos:%d,  max_roi_r_mean:%.2f,  max_roi_r_mean_pos:%d\n"
                                                     " min_roi_r_std:%.2f,   min_roi_r_std_pos:%d,   max_roi_r_std:%.2f,   max_roi_r_std_pos:%d\n")
                                       % min_roi_mean % min_roi_mean_pos % max_roi_mean % max_roi_mean_pos
                                       % min_roi_std % min_roi_std_pos % max_roi_std % max_roi_std_pos);

                if(min_roi_mean<manage.param_roi_std_.r_mean_min_spec_ ||
                        max_roi_mean>manage.param_roi_std_.r_mean_max_spec_ ||
                        min_roi_std<manage.param_roi_std_.r_std_min_spec_ ||
                        max_roi_std>manage.param_roi_std_.r_std_max_spec_)

                    result_roi_std.is_passed=false;
            }
            else if(ch==Gr)
            {
                g_str_msg_all[site_]+=boost::str(boost::format(" min_roi_gr_mean:%.2f,  min_roi_gr_mean_pos:%d,  max_roi_gr_mean:%.2f,  max_roi_gr_mean_pos:%d\n"
                                                     " min_roi_gr_std:%.2f,   min_roi_gr_std_pos:%d,   max_roi_gr_std:%.2f,   max_roi_gr_std_pos:%d\n")
                                       % min_roi_mean % min_roi_mean_pos % max_roi_mean % max_roi_mean_pos
                                       % min_roi_std % min_roi_std_pos % max_roi_std % max_roi_std_pos);

                if(min_roi_mean<manage.param_roi_std_.gr_mean_min_spec_ ||
                        max_roi_mean>manage.param_roi_std_.gr_mean_max_spec_ ||
                        min_roi_std<manage.param_roi_std_.gr_std_min_spec_ ||
                        max_roi_std>manage.param_roi_std_.gr_std_max_spec_)

                    result_roi_std.is_passed=false;
            }
            else if(ch==Gb)
            {
                g_str_msg_all[site_]+=boost::str(boost::format(" min_roi_gb_mean:%.2f,  min_roi_gb_mean_pos:%d,  max_roi_gb_mean:%.2f,  max_roi_gb_mean_pos:%d\n"
                                                     " min_roi_gb_std:%.2f,   min_roi_gb_std_pos:%d,   max_roi_gb_std:%.2f,   max_roi_gb_std_pos:%d\n")
                                       % min_roi_mean % min_roi_mean_pos % max_roi_mean % max_roi_mean_pos
                                       % min_roi_std % min_roi_std_pos % max_roi_std % max_roi_std_pos);

                if(min_roi_mean<manage.param_roi_std_.gb_mean_min_spec_ ||
                        max_roi_mean>manage.param_roi_std_.gb_mean_max_spec_ ||
                        min_roi_std<manage.param_roi_std_.gb_std_min_spec_ ||
                        max_roi_std>manage.param_roi_std_.gb_std_max_spec_)
                    result_roi_std.is_passed=false;
            }
            else if(ch==B)
            {
                g_str_msg_all[site_]+=boost::str(boost::format(" min_roi_b_mean:%.2f,  min_roi_b_mean_pos:%d,  max_roi_b_mean:%.2f,  max_roi_b_mean_pos:%d\n"
                                                     " min_roi_b_std:%.2f,   min_roi_b_std_pos:%d,   max_roi_b_std:%.2f,   max_roi_b_std_pos:%d\n")
                                       % min_roi_mean % min_roi_mean_pos % max_roi_mean % max_roi_mean_pos
                                       % min_roi_std % min_roi_std_pos % max_roi_std % max_roi_std_pos);

                if(min_roi_mean<manage.param_roi_std_.b_mean_min_spec_ ||
                        max_roi_mean>manage.param_roi_std_.b_mean_max_spec_ ||
                        min_roi_std<manage.param_roi_std_.b_std_min_spec_ ||
                        max_roi_std>manage.param_roi_std_.b_std_max_spec_)
                    result_roi_std.is_passed=false;
            }
        }
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_roi_std.name.c_str());
    }
    else
    {
        result_roi_std.g_msg = false;
    }

    if(result_roi_std.is_passed==false)
    {
        result_roi_std.bin = manage.param_roi_std_.bin_;
        result_roi_std.grade = manage.param_roi_std_.grade_;
    }

    return result_roi_std;
}

