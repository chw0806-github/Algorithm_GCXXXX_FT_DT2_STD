#include "LargeGainTest.h"

int  LargeGainTest::TestFunc_Large_Gain_Bright_Bayer(ParamManage& manage,uchar *pData,uchar *pImage_save)
{
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }

    logMsg (std::string("into dark frame test, check params first!"));
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    //分配空间并分通道
    bool result_Split=Algorithm_all[site_].Split_Channel(manage, test_channel_type,false,
                                    pData,test_frameCnt,test_datatype,pImage_save);
    if(!result_Split)
        return -1;

    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合
    items_results.push_back( Algorithm_all[site_].Light_Response(manage) );
    items_results.push_back( Algorithm_all[site_].Relative_Illumination(manage));
    items_results.push_back( Algorithm_all[site_].Non_Uniformity(manage) );
    items_results.push_back( Algorithm_all[site_].POG_Test(manage,POG_Bright));
    items_results.push_back( Algorithm_all[site_].Deadrow_Deadcol_bright(manage,true));

    items_results.push_back( Algorithm_all[site_].Mosaic(manage, test_datatype));

    vector<Subitem_Result> ratio_res_ = Algorithm_all[site_].Color_Ratio(manage);
    items_results.insert(items_results.end(),ratio_res_.begin(),ratio_res_.end());


    if(!Algorithm_all[site_].New_PRNU(manage.width_,manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }


    items_results.push_back(Algorithm_all[site_].Pixel_Defect_Test(manage,External_Configuration));//卡团簇+单点
    items_results.push_back(Algorithm_all[site_].Single_Pixel_Defect_Test(manage,External_Configuration));//只想卡单点，且可以分梯度
    items_results.push_back(PD_Test_Channel(manage));//分通道卡单点，4053等新算法

     items_results.push_back(Algorithm_all[site_].Blemish_Test(manage));
    items_results.push_back(Algorithm_all[site_].ROI_PRNU( manage));
    Algorithm_all[site_].Free_PRNU();

    Subitem_Result large_gain_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);
    return large_gain_result.bin;
}


int  LargeGainTest::TestFunc_Large_Gain_IIC_Bayer(ParamManage& manage,uchar *pData,uchar *pImage_save)
{
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }

    logMsg (std::string("into dark frame test, check params first!"));
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    //分配空间并分通道
    bool result_Split=Algorithm_all[site_].Split_Channel(manage, test_channel_type,false,
                                    pData,test_frameCnt,test_datatype,pImage_save);
    if(!result_Split)
        return -1;

    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    items_results.push_back( Algorithm_all[site_].Deadrow_Deadcol_bright_IIC(manage));

    if(!Algorithm_all[site_].New_PRNU(manage.width_,manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }

    Algorithm_all[site_].Free_PRNU();

    Subitem_Result large_gain_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);
    return large_gain_result.bin;
}


//Pixel Defect Test (黑屏、白屏)
Subitem_Result LargeGainTest::PD_Test( const ParamManage& manage, Pixel_Defect_type type)
{

    if(manage.pixel_defect_version == "pixel_defect_V1.0")
        return PD_Test_V1_0(manage, type);
    Subitem_Result res("Pixel_Defect_Gray", false);
    res.g_msg = false;
    return res;
}



Subitem_Result LargeGainTest::PD_Test_V1_0( const ParamManage& manage, Pixel_Defect_type type)
{
    Subitem_Result res("Pixel_Defect_Gray",true);

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_pixel_defect_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------LargeGain Pixel Defect Test V1.0-----------------------------\n"));

        if(manage.param_pixel_defect_.type_ == 0)
            type = Pixel_Defect_dark;
        else if(manage.param_pixel_defect_.type_ == 1)
            type = Pixel_Defect_bright;

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

        TestItem::find_dark_and_white_pixel(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                            Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
                                            Algorithm_all[site_].g_buf_delta_r_, Algorithm_all[site_].g_buf_delta_gr_,
                                            Algorithm_all[site_].g_buf_delta_gb_, Algorithm_all[site_].g_buf_delta_b_, Algorithm_all[site_].g_buf_delta_raw_,
                                            Algorithm_all[site_].g_binary_r_, Algorithm_all[site_].g_binary_gr_,
                                            Algorithm_all[site_].g_binary_gb_, Algorithm_all[site_].g_binary_b_, Algorithm_all[site_].g_binary_,
                                            Algorithm_all[site_].g_buf_blemish_r_, Algorithm_all[site_].g_buf_blemish_gr_,
                                            Algorithm_all[site_].g_buf_blemish_gb_, Algorithm_all[site_].g_buf_blemish_b_, Algorithm_all[site_].g_buf_blemish_raw_,
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
                                            blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,Algorithm_all[site_].g_buf_tmp_,
                                            manage.param_pixel_defect_.save_image_, g_str_msg_all[site_]);

        g_str_msg_all[site_]+=boost::str(boost::format("Total Pixel Cluster Defect including global and bychannel Cnt:%d\n")
                               % total_cluster_cnt);
        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;
        cnt_2x2_diagonal_pix = cnt_2x2_diagonal_pix/2;

        int cnt_cluster_ch = cnt_1x2_ch+cnt_2x1_ch+
                cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch + other_cluster_ch;
        int cnt_cluster_global = cnt_1x2_2x1_pix + cnt_2x2_diagonal_pix;


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



        int global_cluster_cnt =cnt_cluster_global + paticle + other_cluster + cnt_2x4_pix + cnt_2x2_pix;
        g_str_msg_all[site_]+=boost::str(boost::format("bychannel total clusters:%d\n"
                                             "cnts of 1x2 or 2x1 bychannel:%d\n"
                                             "cnts of 2x2 diagonal bychannel:%d\n")
                               % cnt_cluster_ch % (cnt_1x2_ch+cnt_2x1_ch) % (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch));

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


        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|")
                               % cnt_white_pix % cnt_dark_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());


        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                        "cnt_dark_pix=%d, single_pix_limit=%d \n")
                                            % cnt_dark_pix
                                            % manage.param_pixel_defect_.single_pix_limit_);


        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_begin-Algorithm_all[site_].time_end).toStdString()+"\n";


    }
    else
    {
        res.g_msg = false;
    }


    return res;
}


Subitem_Result LargeGainTest::PD_Test_Channel( const ParamManage& manage)
{
    if(manage.pixel_defect_channel_single_version == "pixel_defect_channel_single_V1.0")
        return PD_Test_Channel_V1_0(manage);
    Subitem_Result res("Pixel_Defect_Channel_Single", false);
    res.g_msg = false;
    return res;
}



//Pixel Defect Test (灰屏DN 单点卡控),   Pixel Defect Test (灰屏Ratio 单点卡控)
Subitem_Result LargeGainTest::PD_Test_Channel_V1_0(const ParamManage &manage)
{
    if (manage.param_pixel_defect_channel_single_.type_ == 0)
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Pixel Defect Gray DN Test-----------------------------\n"));
    else
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Pixel Defect Gray Ratio Test-----------------------------\n"));


    Algorithm_all[site_].time_begin=clock();

    Subitem_Result res("Pixel_Defect_Gray_Ratio",true);

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_pixel_defect_channel_single_.open_mode_ == 1)
    {
        int cnt_white_pix = 0;
        int cnt_dark_pix = 0;
        int cnt_white_pix_r = 0;
        int cnt_dark_pix_r = 0;
        int cnt_white_pix_gr = 0;
        int cnt_dark_pix_gr = 0;
        int cnt_white_pix_gb = 0;
        int cnt_dark_pix_gb = 0;
        int cnt_white_pix_b = 0;
        int cnt_dark_pix_b = 0;
        int crop_cnt_white_pix_r = 0;
        int crop_cnt_dark_pix_r = 0;
        int crop_cnt_white_pix_gr = 0;
        int crop_cnt_dark_pix_gr = 0;
        int crop_cnt_white_pix_gb = 0;
        int crop_cnt_dark_pix_gb = 0;
        int crop_cnt_white_pix_b = 0;
        int crop_cnt_dark_pix_b = 0;

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

        int cnt_single = 0;
        bool channel_single_white_flag = 0;
        bool channel_single_dark_flag = 0;

        Spec_Mode spec_mode_/*=Ratio*/;


        if(manage.param_pixel_defect_channel_single_.type_ == 0)
        {
            spec_mode_ = DN;

            diff_spec_r_= manage.param_pixel_defect_channel_single_.DN_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_channel_single_.DN_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_channel_single_.DN_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_channel_single_.DN_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_channel_single_.blemish_dn_spec_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_channel_single_.blemish_dn_spec_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_channel_single_.blemish_dn_spec_;
            diff_blemish_spec_b_= manage.param_pixel_defect_channel_single_.blemish_dn_spec_;

        }
        else if(manage.param_pixel_defect_channel_single_.type_ == 1)
        {
            spec_mode_ = Ratio;

            diff_spec_r_= manage.param_pixel_defect_channel_single_.ratio_diff_spec_r_;
            diff_spec_gr_= manage.param_pixel_defect_channel_single_.ratio_diff_spec_gr_;
            diff_spec_gb_ =manage.param_pixel_defect_channel_single_.ratio_diff_spec_gb_;
            diff_spec_b_= manage.param_pixel_defect_channel_single_.ratio_diff_spec_b_;

            diff_blemish_spec_r_= manage.param_pixel_defect_channel_single_.blemish_spec_r_;
            diff_blemish_spec_gr_= manage.param_pixel_defect_channel_single_.blemish_spec_gr_;
            diff_blemish_spec_gb_ = manage.param_pixel_defect_channel_single_.blemish_spec_gb_;
            diff_blemish_spec_b_= manage.param_pixel_defect_channel_single_.blemish_spec_b_;
        }






        TestItem::find_channel_single_white_dark_pixel(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                                       Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
                                                       Algorithm_all[site_].g_buf_delta_r_, Algorithm_all[site_].g_buf_delta_gr_,
                                                       Algorithm_all[site_].g_buf_delta_gb_, Algorithm_all[site_].g_buf_delta_b_, Algorithm_all[site_].g_buf_delta_raw_,
                                                       Algorithm_all[site_].g_binary_r_, Algorithm_all[site_].g_binary_gr_,
                                                       Algorithm_all[site_].g_binary_gb_, Algorithm_all[site_].g_binary_b_, Algorithm_all[site_].g_binary_,
                                                       Algorithm_all[site_].g_buf_blemish_r_, Algorithm_all[site_].g_buf_blemish_gr_,
                                                       Algorithm_all[site_].g_buf_blemish_gb_, Algorithm_all[site_].g_buf_blemish_b_, Algorithm_all[site_].g_buf_blemish_raw_,
                                                       w, h, manage.param_pixel_defect_channel_single_.cnt_skip_, manage.param_pixel_defect_channel_single_.r_area_,
                                                       diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                                       diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                                       spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                                       cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                                       cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                                       cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                                       other_cluster_ch, total_cluster_cnt,
                                                       manage.param_pixel_defect_channel_single_.blemish_global_size_limit_,
                                                       manage.param_pixel_defect_channel_single_.blemish_bychannel_size_limit_,
                                                       blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,Algorithm_all[site_].g_buf_tmp_,
                                                       manage.param_pixel_defect_channel_single_.save_image_, g_str_msg_all[site_],
                                                       manage.param_pixel_defect_channel_single_.ROI_Channel_X,manage.param_pixel_defect_channel_single_.ROI_Channel_Y,
                                                       manage.param_pixel_defect_channel_single_.ROI_Channel_W,manage.param_pixel_defect_channel_single_.ROI_Channel_H,
                                                       cnt_dark_pix_r, cnt_dark_pix_gr,cnt_dark_pix_gb,cnt_dark_pix_b,
                                                       cnt_white_pix_r,cnt_white_pix_gr,cnt_white_pix_gb,cnt_white_pix_b,
                                                       crop_cnt_dark_pix_r,crop_cnt_dark_pix_gr,crop_cnt_dark_pix_gb,crop_cnt_dark_pix_b,
                                                       crop_cnt_white_pix_r,crop_cnt_white_pix_gr,crop_cnt_white_pix_gb,crop_cnt_white_pix_b);

        g_str_msg_all[site_]+=boost::str(boost::format("Total Pixel Cluster Defect including global and bychannel Cnt:%d\n")
                               % total_cluster_cnt);
        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;
        cnt_2x2_diagonal_pix = cnt_2x2_diagonal_pix/2;

        int cnt_cluster_ch = cnt_1x2_ch+cnt_2x1_ch+
                cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch + other_cluster_ch;
        int cnt_cluster_global = cnt_1x2_2x1_pix + cnt_2x2_diagonal_pix;
        int cnt_single_totle = cnt_white_pix+cnt_dark_pix;

        if (manage.param_pixel_defect_channel_single_.only_bright_dark_pix_mode == "bright")
        {
            cnt_single = cnt_white_pix;
            channel_single_white_flag = 1;
        }
        else if (manage.param_pixel_defect_channel_single_.only_bright_dark_pix_mode == "dark")
        {
            cnt_single = cnt_dark_pix;
            channel_single_dark_flag = 1;
        }
        else
        {
            cnt_single = cnt_single_totle;
            channel_single_dark_flag = 1;
            channel_single_white_flag = 1;
        }



        if (cnt_single >= manage.param_pixel_defect_channel_single_.single_pix_limit_Class1&&
                cnt_single<manage.param_pixel_defect_channel_single_.single_pix_limit_Class2)
        {
            res.bin = manage.param_pixel_defect_channel_single_.bin_single_pix_Class1;
            res.grade = manage.param_pixel_defect_channel_single_.grade_single_pix;
            res.is_passed = false;
        }
        else if (cnt_single>=manage.param_pixel_defect_channel_single_.single_pix_limit_Class2 &&
                cnt_single<manage.param_pixel_defect_channel_single_.single_pix_limit_Class3)
        {
            res.bin = manage.param_pixel_defect_channel_single_.bin_single_pix_Class2;
            res.grade = manage.param_pixel_defect_channel_single_.grade_single_pix;
            res.is_passed = false;
        }
        else if (cnt_single>=manage.param_pixel_defect_channel_single_.single_pix_limit_Class3 &&
                 cnt_single<manage.param_pixel_defect_channel_single_.single_pix_limit_Class4)
        {
            res.bin = manage.param_pixel_defect_channel_single_.bin_single_pix_Class3;
            res.grade = manage.param_pixel_defect_channel_single_.grade_single_pix;
            res.is_passed = false;
        }
        else if (cnt_single>=manage.param_pixel_defect_channel_single_.single_pix_limit_Class4 &&
                 cnt_single<manage.param_pixel_defect_channel_single_.single_pix_limit_Class5)
        {
            res.bin = manage.param_pixel_defect_channel_single_.bin_single_pix_Class4;
            res.grade = manage.param_pixel_defect_channel_single_.grade_single_pix;
            res.is_passed = false;
        }
        else if (cnt_single>=manage.param_pixel_defect_channel_single_.single_pix_limit_Class5)
        {
            res.bin = manage.param_pixel_defect_channel_single_.bin_single_pix_Class5;
            res.grade = manage.param_pixel_defect_channel_single_.grade_single_pix;
            res.is_passed = false;
        }

        //新增
        //黑点卡控
        if(channel_single_dark_flag)
        {
            if((crop_cnt_dark_pix_gr+crop_cnt_dark_pix_gb>=manage.param_pixel_defect_channel_single_.crop_single_pix_dark_limit_gr_gb)&&
                    (res.grade < manage.param_pixel_defect_channel_single_.grade_crop_single_pix_dark_channel_gr_gb))
            {
                res.grade = manage.param_pixel_defect_channel_single_.grade_crop_single_pix_dark_channel_gr_gb;
                res.is_passed = false;
                res.bin= manage.param_pixel_defect_channel_single_.bin_crop_single_pix_dark_channel_gr_gb;
            }
            if(((cnt_dark_pix_r>=manage.param_pixel_defect_channel_single_.single_pix_dark_limit_r)||
                (cnt_dark_pix_gr>=manage.param_pixel_defect_channel_single_.single_pix_dark_limit_gr)||
                (cnt_dark_pix_gb>=manage.param_pixel_defect_channel_single_.single_pix_dark_limit_gb)||
                (cnt_dark_pix_b>=manage.param_pixel_defect_channel_single_.single_pix_dark_limit_b))&&
                    (res.grade < manage.param_pixel_defect_channel_single_.grade_single_pix_dark_channel))
            {
                res.grade = manage.param_pixel_defect_channel_single_.grade_single_pix_dark_channel;
                res.is_passed = false;
                res.bin= manage.param_pixel_defect_channel_single_.bin_single_pix_dark_channel;
                g_str_msg_all[site_]+=boost::str(boost::format("channel_single_pix_dark_limit_fail\n"));
            }
            if(((crop_cnt_dark_pix_r>=manage.param_pixel_defect_channel_single_.crop_single_pix_dark_limit_r)||
                (crop_cnt_dark_pix_gr>=manage.param_pixel_defect_channel_single_.crop_single_pix_dark_limit_gr)||
                (crop_cnt_dark_pix_gb>=manage.param_pixel_defect_channel_single_.crop_single_pix_dark_limit_gb)||
                (crop_cnt_dark_pix_b>=manage.param_pixel_defect_channel_single_.crop_single_pix_dark_limit_b))&&
                    (res.grade < manage.param_pixel_defect_channel_single_.grade_crop_single_pix_dark_channel))
            {
                res.grade = manage.param_pixel_defect_channel_single_.grade_crop_single_pix_dark_channel;
                res.is_passed = false;
                res.bin= manage.param_pixel_defect_channel_single_.bin_crop_single_pix_dark_channel;
                g_str_msg_all[site_]+=boost::str(boost::format("channel_crop_single_pix_dark_limit_fail\n"));
            }
        }

        //白点卡控
        if(channel_single_white_flag)
        {
            if((crop_cnt_white_pix_gr+crop_cnt_white_pix_gb>=manage.param_pixel_defect_channel_single_.crop_single_pix_bright_limit_gr_gb)&&
                    (res.grade < manage.param_pixel_defect_channel_single_.grade_crop_single_pix_bright_channel_gr_gb))
            {
                res.grade = manage.param_pixel_defect_channel_single_.grade_crop_single_pix_bright_channel_gr_gb;
                res.is_passed = false;
                res.bin= manage.param_pixel_defect_channel_single_.bin_crop_single_pix_bright_channel_gr_gb;
            }
            if(((cnt_white_pix_r>=manage.param_pixel_defect_channel_single_.single_pix_bright_limit_r)||
                (cnt_white_pix_gr>=manage.param_pixel_defect_channel_single_.single_pix_bright_limit_gr)||
                (cnt_white_pix_gb>=manage.param_pixel_defect_channel_single_.single_pix_bright_limit_gb)||
                (cnt_white_pix_b>=manage.param_pixel_defect_channel_single_.single_pix_bright_limit_b))&&
                    (res.grade < manage.param_pixel_defect_channel_single_.grade_single_pix_bright_channel))
            {
                res.grade = manage.param_pixel_defect_channel_single_.grade_single_pix_bright_channel;
                res.is_passed = false;
                res.bin= manage.param_pixel_defect_channel_single_.bin_single_pix_bright_channel;
                g_str_msg_all[site_]+=boost::str(boost::format("channel_single_pix_white_limit_fail\n"));
            }

            if(((crop_cnt_white_pix_r>=manage.param_pixel_defect_channel_single_.crop_single_pix_bright_limit_r)||
                (crop_cnt_white_pix_gr>=manage.param_pixel_defect_channel_single_.crop_single_pix_bright_limit_gr)||
                (crop_cnt_white_pix_gb>=manage.param_pixel_defect_channel_single_.crop_single_pix_bright_limit_gb)||
                (crop_cnt_white_pix_b>=manage.param_pixel_defect_channel_single_.crop_single_pix_bright_limit_b))&&
                    (res.grade < manage.param_pixel_defect_channel_single_.grade_crop_single_pix_bright_channel))
            {
                res.grade = manage.param_pixel_defect_channel_single_.grade_crop_single_pix_bright_channel;
                res.is_passed = false;
                res.bin= manage.param_pixel_defect_channel_single_.bin_crop_single_pix_bright_channel;
                g_str_msg_all[site_]+=boost::str(boost::format("channel_crop_single_pix_white_limit_fail\n"));
            }
        }


        int global_cluster_cnt =cnt_cluster_global + paticle + other_cluster + cnt_2x4_pix + cnt_2x2_pix;
        g_str_msg_all[site_]+=boost::str(boost::format("bychannel total clusters:%d\n"
                                             "cnts of 1x2 or 2x1 bychannel:%d\n"
                                             "cnts of 2x2 diagonal bychannel:%d\n")
                               % cnt_cluster_ch % (cnt_1x2_ch+cnt_2x1_ch) % (cnt_2x2_leftdiagonal_ch+cnt_2x2_rightdiagonal_ch));

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

        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|")
                               % cnt_white_pix % cnt_dark_pix %cnt_single_totle);
        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|")
                               % cnt_dark_pix_r % cnt_dark_pix_gr %cnt_dark_pix_gb %cnt_dark_pix_b %(cnt_dark_pix_gr+cnt_dark_pix_gb)
                               % cnt_white_pix_r % cnt_white_pix_gr %cnt_white_pix_gb %cnt_white_pix_b %(cnt_white_pix_gr+cnt_white_pix_gb));
        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|")
                               % crop_cnt_dark_pix_r % crop_cnt_dark_pix_gr %crop_cnt_dark_pix_gb %crop_cnt_dark_pix_b %(crop_cnt_dark_pix_gr+crop_cnt_dark_pix_gb)
                               % crop_cnt_white_pix_r % crop_cnt_white_pix_gr %crop_cnt_white_pix_gb %crop_cnt_white_pix_b %(crop_cnt_white_pix_gr+crop_cnt_white_pix_gb));
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());
    }
    else
    {
        res.is_passed=true;
        g_str_msg_all[site_]+=boost::str(boost::format("close Pixel Defect test!\n"));
        if(manage.param_pixel_defect_channel_single_.log_align_)
        {
            g_str_log_all[site_]+=boost::str(boost::format("-1|-1|"));
            g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());
        }

    }

    Algorithm_all[site_].time_end=clock();
    g_str_msg_all[site_]+="\n cast_time:"+QString::number(Algorithm_all[site_].time_begin-Algorithm_all[site_].time_end).toStdString()+"\n";

    return res;
}











