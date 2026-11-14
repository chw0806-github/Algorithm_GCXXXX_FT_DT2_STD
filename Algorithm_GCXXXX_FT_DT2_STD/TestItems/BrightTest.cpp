#include "BrightTest.h"

int  BrightTest::TestFunc_Normal_Bright_AllExceptPog(ParamManage& test_manage,uchar * pData,uchar *pImage_save)
{
    if (test_frameCnt<=0)           //check图片数量
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }
    logMsg (std::string("into white frame test, check params first!"));

    //check配置参数
    if (test_manage.width_<=0 ||
        test_manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        return -2;
    }

    clock_t time_begin;
    clock_t time_end;

    time_begin=clock();
    //分配空间并分通道
    bool result_Split= Algorithm_all[site_].Split_Channel(test_manage, test_channel_type,false,
                                                pData,test_frameCnt,test_datatype,pImage_save);
    time_end=clock();
    g_str_msg_all[site_]+="\n cost_time_split:"+QString::number(time_end-time_begin).toStdString()+"\n";
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }
    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    logMsg ("Start flow test!",0);
    items_results.push_back(Algorithm_all[site_].Light_Response(test_manage));
    items_results.push_back(Algorithm_all[site_].Relative_Illumination(test_manage));
    items_results.push_back(Algorithm_all[site_].Non_Uniformity(test_manage));
    items_results.push_back(Algorithm_all[site_].ImageDirt(test_manage));     //



    items_results.push_back(Algorithm_all[site_].Deadrow_Deadcol_bright(test_manage,true));
    items_results.push_back(Algorithm_all[site_].Edge_Deadrow_Deadcol_bright(test_manage));

    vector<Subitem_Result> ratio_res_ = Algorithm_all[site_].Color_Ratio( test_manage);
    items_results.insert(items_results.end(),ratio_res_.begin(),ratio_res_.end());

    vector<Subitem_Result> cs_shading_res_17x13  = Color_Shading( test_manage);
    items_results.insert(items_results.end(),cs_shading_res_17x13.begin(),cs_shading_res_17x13.end());

    vector<Subitem_Result> cs_shading_res_5x5  = Color_Shading_5_5( test_manage);
    items_results.insert(items_results.end(),cs_shading_res_5x5.begin(),cs_shading_res_5x5.end());

    vector<Subitem_Result> cs_shading_15x15 = Color_Shading_15_15(test_manage);
    items_results.insert(items_results.end(), cs_shading_15x15.begin(), cs_shading_15x15.end());

    vector<Subitem_Result> ratio_GI_ = Color_GI( test_manage);
    items_results.insert(items_results.end(),ratio_GI_.begin(),ratio_GI_.end());

    vector<Subitem_Result> ratio_GI_5x5 = Color_GI_5_5( test_manage);
    items_results.insert(items_results.end(),ratio_GI_5x5.begin(),ratio_GI_5x5.end());

    if(!Algorithm_all[site_].New_PRNU(test_manage.width_,test_manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }
    items_results.push_back(Algorithm_all[site_].PKG_Test(test_manage));
    items_results.push_back(Algorithm_all[site_].Pixel_Defect_Test_NO_Test_Info( test_manage,External_Configuration));  
    items_results.push_back(Algorithm_all[site_].ROI_PRNU( test_manage));
    items_results.push_back(Optical_Center(test_manage));
    Algorithm_all[site_].Free_PRNU();

    Subitem_Result bright_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);

    int code =bright_result.bin;

    return code;
}


int  BrightTest::TestFunc_Normal_Bright_ALL(ParamManage& test_manage,uchar * pData,uchar *pImage_save)
{
    if (test_frameCnt<=0)           //check图片数量
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }
    logMsg (std::string("into white frame test, check params first!"));

    //check配置参数
    if (test_manage.width_<=0 ||
        test_manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        return -2;
    }

    clock_t time_begin;
    clock_t time_end;

    time_begin=clock();
    //分配空间并分通道
    bool result_Split= Algorithm_all[site_].Split_Channel(test_manage, test_channel_type,false,
                                                pData,test_frameCnt,test_datatype,pImage_save);
    time_end=clock();
    g_str_msg_all[site_]+="\n cost_time_split:"+QString::number(time_end-time_begin).toStdString()+"\n";
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }
    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    logMsg ("Start flow test!",0);
    items_results.push_back(Algorithm_all[site_].Light_Response(test_manage));
    items_results.push_back(Algorithm_all[site_].Relative_Illumination(test_manage));
    items_results.push_back(Algorithm_all[site_].Non_Uniformity(test_manage));
    items_results.push_back(Algorithm_all[site_].ImageDirt(test_manage));
    items_results.push_back(Algorithm_all[site_].POG_Test(test_manage,POG_Bright));

    items_results.push_back(Algorithm_all[site_].Deadrow_Deadcol_bright(test_manage,true));
    items_results.push_back(Algorithm_all[site_].Edge_Deadrow_Deadcol_bright(test_manage));

    vector<Subitem_Result> ratio_res_ = Algorithm_all[site_].Color_Ratio( test_manage);
    items_results.insert(items_results.end(),ratio_res_.begin(),ratio_res_.end());

    vector<Subitem_Result> cs_shading_res_17x13  = Color_Shading( test_manage);
    items_results.insert(items_results.end(),cs_shading_res_17x13.begin(),cs_shading_res_17x13.end());

    vector<Subitem_Result> cs_shading_res_5x5  = Color_Shading_5_5( test_manage);
    items_results.insert(items_results.end(),cs_shading_res_5x5.begin(),cs_shading_res_5x5.end());

    vector<Subitem_Result> cs_shading_15x15 = Color_Shading_15_15(test_manage);
    items_results.insert(items_results.end(), cs_shading_15x15.begin(), cs_shading_15x15.end());

    vector<Subitem_Result> ratio_GI_ = Color_GI( test_manage);
    items_results.insert(items_results.end(),ratio_GI_.begin(),ratio_GI_.end());

    vector<Subitem_Result> ratio_GI_5x5 = Color_GI_5_5( test_manage);
    items_results.insert(items_results.end(),ratio_GI_5x5.begin(),ratio_GI_5x5.end());



    if(!Algorithm_all[site_].New_PRNU(test_manage.width_,test_manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }
    items_results.push_back(Algorithm_all[site_].PKG_Test(test_manage));
    items_results.push_back(Algorithm_all[site_].Blemish_Test(test_manage));
    items_results.push_back(Algorithm_all[site_].Pixel_Defect_Test(test_manage,External_Configuration));
    items_results.push_back(Algorithm_all[site_].Single_Pixel_Defect_Test(test_manage,External_Configuration));//只想卡单点，且可以分梯度
    items_results.push_back(Algorithm_all[site_].ROI_PRNU( test_manage));
    items_results.push_back(Optical_Center(test_manage));
    Algorithm_all[site_].Free_PRNU();

    Subitem_Result bright_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);

    int code =bright_result.bin;

    return code;
}


int  BrightTest::TestFunc_IIC_Bright(ParamManage& test_manage,uchar * pData,uchar *pImage_save)
{
    if (test_frameCnt<=0)           //check图片数量
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }
    logMsg (std::string("into white frame test, check params first!"));

    //check配置参数
    if (test_manage.width_<=0 ||
        test_manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        return -2;
    }

    clock_t time_begin;
    clock_t time_end;

    time_begin=clock();
    //分配空间并分通道
    bool result_Split= Algorithm_all[site_].Split_Channel(test_manage, test_channel_type,false,
                                                pData,test_frameCnt,test_datatype,pImage_save);
    time_end=clock();
    g_str_msg_all[site_]+="\n cost_time_split:"+QString::number(time_end-time_begin).toStdString()+"\n";
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }
    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    logMsg ("Start flow test!",0);

    items_results.push_back(Algorithm_all[site_].Deadrow_Deadcol_bright_IIC(test_manage));


    if(!Algorithm_all[site_].New_PRNU(test_manage.width_,test_manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }

    Algorithm_all[site_].Free_PRNU();

    Subitem_Result bright_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);

    int code =bright_result.bin;

    return code;
}


int  BrightTest::TestFunc_Normal_Bright_POG(ParamManage& test_manage,uchar * pData,uchar *pImage_save)
{
    if (test_frameCnt<=0)           //check图片数量
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }
    logMsg (std::string("into white frame test, check params first!"));

    //check配置参数
    if (test_manage.width_<=0 ||
        test_manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        return -2;
    }

    clock_t time_begin;
    clock_t time_end;

    time_begin=clock();
    //分配空间并分通道
    bool result_Split= Algorithm_all[site_].Split_Channel(test_manage, test_channel_type,false,
                                                pData,test_frameCnt,test_datatype,pImage_save);
    time_end=clock();
    g_str_msg_all[site_]+="\n cost_time_split:"+QString::number(time_end-time_begin).toStdString()+"\n";
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }
    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    logMsg ("Start flow test!",0);

    items_results.push_back(Algorithm_all[site_].Light_Response(test_manage));
    items_results.push_back(Algorithm_all[site_].POG_Test(test_manage,POG_Bright));
    items_results.push_back(Algorithm_all[site_].ImageDirt(test_manage));


    if(!Algorithm_all[site_].New_PRNU(test_manage.width_,test_manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }
    items_results.push_back(Algorithm_all[site_].PKG_Test(test_manage));
    items_results.push_back(Algorithm_all[site_].Blemish_Test(test_manage));
    items_results.push_back(Algorithm_all[site_].Pixel_Defect_Test(test_manage,External_Configuration));
    items_results.push_back(Algorithm_all[site_].Single_Pixel_Defect_Test(test_manage,External_Configuration));//只想卡单点，且可以分梯度


    Algorithm_all[site_].Free_PRNU();

    Subitem_Result bright_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);

    int code =bright_result.bin;

    return code;
}





//白屏GI单独卡控
vector<Subitem_Result> BrightTest::Color_GI( ParamManage& manage)
{
    if(manage.gi_ratio_version == "gi_ratio_V1.0")
        return Color_GI_V1_0(manage);
    else if(manage.gi_ratio_version == "gi_ratio_V1.1")
        return Color_GI_V1_1(manage);

    Subitem_Result res("GI", false);
    res.g_msg = false;
    vector<Subitem_Result> Color_GI_test_result;
    Color_GI_test_result.push_back(res);
    return Color_GI_test_result;
}


vector<Subitem_Result> BrightTest::Color_GI_V1_0( ParamManage& manage)
{

    int w=manage.width_;
    int h=manage.height_;
    Subitem_Result result_GI ("GI",true);
    //Subitem_Result result_cs_MA ("Color_Shading_MA",true);

    if ((manage.param_GI.ratio_open_mode_ == 1))
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- Global GI V1.0-----------------------------\n"));


        Param_Color_Ratio  param_color_ratio_=manage.param_color_ratio_;
        //Param_Color_Shading  param_color_shading_ = manage.param_color_shading;
        Color_Ratio_Value  ratio_value;

        TestItem::test_roi_colorratio( Algorithm_all[site_].mean_gr_roi, Algorithm_all[site_].mean_gb_roi, w, h,
                                       manage.param_GI.skip,
                                       manage.param_GI.ROI_X_Cnt,
                                       manage.param_GI.ROI_Y_Cnt,
                                       Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                       Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
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

        double roi_gg_ratio_for_cs_max = *std::max_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double roi_gg_ratio_for_cs_min = *std::min_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());



        if ( Algorithm_all[site_].mean_gr_roi>10)
        {
            double gg_roi_max_1 = 0.00;
            double gg_roi_min_1 = 0.00;
            double GI = 0.000;

            gg_roi_max_1 = std::fabs(roi_gg_ratio_for_cs_max - 1);
            gg_roi_min_1 = std::fabs(roi_gg_ratio_for_cs_min - 1);
            GI = std::max(gg_roi_max_1,gg_roi_min_1);



            g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|")
                                   % roi_gg_ratio_for_cs_max
                                   % roi_gg_ratio_for_cs_min
                                   % GI
                                   );
            g_str_msg_all[site_]+=boost::str(boost::format("ROI:%d x %d\n")
                                   % manage.param_GI.ROI_X_Cnt
                                   % manage.param_GI.ROI_Y_Cnt);

            g_str_msg_all[site_]+=boost::str(boost::format("ROI Color GI:\n"
                                                 "Gr/Gb_MAX=%.4f,Gr/Gb_MIN=%.4f,GI(Gr/Gb-1)=%.4f\n"
                                                 )
                                   % roi_gg_ratio_for_cs_max
                                   % roi_gg_ratio_for_cs_min
                                   % GI
                                   );


            g_str_msg_all[site_]+=boost::str(boost::format(
                                                 "SPEC:\n"
                                                 "(GI(Gr/Gb-1)=%.4f, SPEC:%.4f)\n"
                                                 )
                                             % GI % manage.param_GI.GI_Ratio );

            if(GI > manage.param_GI.GI_Ratio)
            {
                result_GI.is_passed = false;
                result_GI.bin = manage.param_GI.bin_GI_;
                result_GI.grade = manage.param_GI.grade_GI_;
            }

        }
        else
        {
            g_str_msg_all[site_].append ("mean_g is low, not test GI!!!\n");
            g_str_log_all[site_]+=boost::str(boost::format("-1|-1|-1|"));
        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_GI.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
    {
        result_GI.g_msg = false;
    }

    vector<Subitem_Result> Color_GI_test_result;
    Color_GI_test_result.push_back(result_GI);
   // Color_Shading_test_result.push_back(result_cs_MA);

    return Color_GI_test_result;
}


vector<Subitem_Result> BrightTest::Color_GI_V1_1( ParamManage& manage)
{

    int w=manage.width_;
    int h=manage.height_;
    Subitem_Result result_GI ("GI_V1.1",true);

    if ((manage.param_GI.ratio_open_mode_ == 1))
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- Global GI V1.1-----------------------------\n"));

        Param_Color_Ratio  param_color_ratio_=manage.param_color_ratio_;
        Color_Ratio_Value  ratio_value;

        TestItem::test_roi_colorratio( Algorithm_all[site_].mean_gr_roi, Algorithm_all[site_].mean_gb_roi, w, h,
                                       manage.param_GI.skip,
                                       manage.param_GI.ROI_X_Cnt,
                                       manage.param_GI.ROI_Y_Cnt,
                                       Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                       Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
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

        double roi_gg_ratio_for_cs_max = *std::max_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double roi_gg_ratio_for_cs_min = *std::min_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());



        if ( Algorithm_all[site_].mean_gr_roi>10)
        {
            double gg_roi_max_1 = 0.00;
            double gg_roi_min_1 = 0.00;
            double GI = 0.000;

            gg_roi_max_1 = std::fabs(roi_gg_ratio_for_cs_max - 1);
            gg_roi_min_1 = std::fabs(roi_gg_ratio_for_cs_min - 1);
            GI = std::max(gg_roi_max_1,gg_roi_min_1);



            g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|")
                                   % roi_gg_ratio_for_cs_max
                                   % roi_gg_ratio_for_cs_min
                                   % GI
                                   );
            g_str_msg_all[site_]+=boost::str(boost::format("ROI:%d x %d\n")
                                   % manage.param_GI.ROI_X_Cnt
                                   % manage.param_GI.ROI_Y_Cnt);

            g_str_msg_all[site_]+=boost::str(boost::format("ROI Color GI:\n"
                                                 "Gr/Gb_MAX=%.4f,Gr/Gb_MIN=%.4f,GI(Gr/Gb-1)=%.4f\n"
                                                 )
                                   % roi_gg_ratio_for_cs_max
                                   % roi_gg_ratio_for_cs_min
                                   % GI
                                   );


            g_str_msg_all[site_] += boost::str(boost::format(
                "SPEC:\n"
                "GI=%.4f (SPEC Class1: %.4f ~ %.4f)\n"
                "GI=%.4f (SPEC Class2: >= %.4f)\n"
            ) % GI % manage.param_GI.GI_Ratio_class1 % manage.param_GI.GI_Ratio_class2
              % GI % manage.param_GI.GI_Ratio_class2);


            if(GI > manage.param_GI.GI_Ratio_class1 && GI < manage.param_GI.GI_Ratio_class2)
            {
                result_GI.is_passed = false;
                result_GI.bin = manage.param_GI.bin_GI_class1;
                result_GI.grade = manage.param_GI.grade_GI_class1;
            }
            else if( GI >= manage.param_GI.GI_Ratio_class2)
            {
                result_GI.is_passed = false;
                result_GI.bin = manage.param_GI.bin_GI_class2;
                result_GI.grade = manage.param_GI.grade_GI_class2;
            }

        }
        else
        {
            g_str_msg_all[site_].append ("mean_g is low, not test GI!!!\n");
            g_str_log_all[site_]+=boost::str(boost::format("-1|-1|-1|"));
        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_GI.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
    {
        result_GI.g_msg = false;
    }

    vector<Subitem_Result> Color_GI_test_result;
    Color_GI_test_result.push_back(result_GI);

    return Color_GI_test_result;
}


vector<Subitem_Result> BrightTest::Color_GI_5_5( ParamManage& manage)
{
    if(manage.gi_5_5_ratio_version == "gi_5_5_ratio_V1.0")
        return Color_GI_5_5_V1_0(manage);

    Subitem_Result res("GI_5_5", false);
    res.g_msg = false;
    vector<Subitem_Result> Color_GI_5_5_test_result;
    Color_GI_5_5_test_result.push_back(res);
    return Color_GI_5_5_test_result;
}



vector<Subitem_Result> BrightTest::Color_GI_5_5_V1_0( ParamManage& manage)
{
    Algorithm_all[site_].time_begin=clock();
    g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- ROI GI 5*5 V1.0-----------------------------\n"));

    int w=manage.width_;
    int h=manage.height_;
    Subitem_Result result_GI_5x5 ("GI_5x5",true);


    if (manage.param_GI_5x5.ratio_open_mode_ == 1 )
    {
        Param_Color_Ratio  param_color_ratio_=manage.param_color_ratio_;
        Color_Ratio_Value  ratio_value;


        TestItem::test_roi_colorratio_5X5( Algorithm_all[site_].mean_gr_roi, Algorithm_all[site_].mean_gb_roi, w, h,
                                           manage.param_GI_5x5.skip,
                                           manage.param_GI_5x5.ROI_X_Cnt,
                                           manage.param_GI_5x5.ROI_Y_Cnt,
                                           Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                           Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
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

        //可优化可读性
        ratio_value.gg_leftup_5X5 = ratio_value.GG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.GG_Ratio_target.value("leftup")));

        ratio_value.gg_rightup_5X5 = ratio_value.GG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.GG_Ratio_target.value("rightup")));

        ratio_value.gg_leftdown_5X5 = ratio_value.GG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.GG_Ratio_target.value("leftdown")));

        ratio_value.gg_rightdown_5X5 = ratio_value.GG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.GG_Ratio_target.value("rightdown")));

        ratio_value.gg_center_5X5 = ratio_value.GG_ratio.value(QPair<QString,double>("center",param_color_ratio_.GG_Ratio_target.value("center")));


        g_str_msg_all[site_]+=boost::str(boost::format("ROI:%d x %d\n")
                                         % manage.param_GI_5x5.ROI_X_Cnt
                                         % manage.param_GI_5x5.ROI_Y_Cnt
                                         );


        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|")
                                         % ratio_value.gg_leftup_5X5
                                         % ratio_value.gg_rightup_5X5
                                         % ratio_value.gg_leftdown_5X5
                                         % ratio_value.gg_rightdown_5X5
                                         % ratio_value.gg_center_5X5
                                         );
        g_str_msg_all[site_]+=boost::str(boost::format("ROI Color GI_5x5:\n"
                                                       "GI_LEFTUP_5X5=%.4f,GI_RIGHTUP_5X5=%.4f,GI_LEFTDOWN_5X5=%.4f,GI_RIGHTDOWN_5X5=%.4f,GI_CENTER_5X5=%.4f\n"
                                                       )
                                         % ratio_value.gg_leftup_5X5
                                         % ratio_value.gg_rightup_5X5
                                         % ratio_value.gg_leftdown_5X5
                                         % ratio_value.gg_rightdown_5X5
                                         % ratio_value.gg_center_5X5
                                         );
        if(ratio_value.gg_leftup_5X5 > manage.param_GI_5x5.TargetGI_Leftup ||
                ratio_value.gg_rightup_5X5 > manage.param_GI_5x5.TargetGI_Rightup||
                ratio_value.gg_leftdown_5X5 > manage.param_GI_5x5.TargetGI_Leftdown ||
                ratio_value.gg_rightdown_5X5 > manage.param_GI_5x5.TargetGI_Rightdown||
                ratio_value.gg_center_5X5 > manage.param_GI_5x5.TargetGI_Center)
        {
            result_GI_5x5.is_passed = false;
            result_GI_5x5.bin = manage.param_GI_5x5.bin_GI_5x5;
            result_GI_5x5.grade = manage.param_GI_5x5.grade_GI_5x5;
        }


        g_str_msg_all[site_] += boost::str(boost::format(
            "SPEC:\n"
            "gg_leftup_5X5=%.4f,   (SPEC MAX: %.4f)\n"
            "gg_rightup_5X5=%.4f,  (SPEC MAX: %.4f)\n"
            "gg_center_5X5=%.4f,   (SPEC MAX: %.4f)\n"
            "gg_leftdown_5X5=%.4f, (SPEC MAX: %.4f)\n"
            "gg_rightdown_5X5=%.4f,(SPEC MAX: %.4f)\n"
        ) % ratio_value.gg_leftup_5X5    % manage.param_GI_5x5.TargetGI_Leftup
          % ratio_value.gg_rightup_5X5   % manage.param_GI_5x5.TargetGI_Rightup
          % ratio_value.gg_center_5X5    % manage.param_GI_5x5.TargetGI_Center
          % ratio_value.gg_leftdown_5X5  % manage.param_GI_5x5.TargetGI_Leftdown
          % ratio_value.gg_rightdown_5X5 % manage.param_GI_5x5.TargetGI_Rightdown);

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_GI_5x5.name.c_str());

    }
    else
    {
        result_GI_5x5.g_msg = false;
    }

    vector<Subitem_Result> Color_GI_5x5_test_result;
    Color_GI_5x5_test_result.push_back(result_GI_5x5);


    Algorithm_all[site_].time_end=clock();
    g_str_msg_all[site_]+="\n cast_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    return  Color_GI_5x5_test_result;
}




//白屏 Color_Shading
vector<Subitem_Result> BrightTest::Color_Shading( ParamManage& manage)
{
    if(manage.color_shading_version == "color_shading_V1.0")
        return Color_Shading_V1_0(manage);
    else if(manage.color_shading_version == "color_shading_V1.1")
        return Color_Shading_V1_1(manage);


    Subitem_Result res("Color_Shading", false);
    res.g_msg = false;
    vector<Subitem_Result> Color_Shading_test_result;
    Color_Shading_test_result.push_back(res);
    return Color_Shading_test_result;
}


//白屏 Color_Shading
vector<Subitem_Result> BrightTest::Color_Shading_V1_0( ParamManage& manage)
{

    int w=manage.width_;
    int h=manage.height_;
    Subitem_Result result_cs ("Color_Shading",true);
    Subitem_Result result_cs_MA ("Color_Shading_MA",true);

    if ((manage.param_color_ratio_.ratio_open_mode_ == 1 && manage.param_color_shading.open_mode_ == 1))
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- ROI Color Shading V1.0-----------------------------\n"));

        Param_Color_Ratio  param_color_ratio_=manage.param_color_ratio_;
        Param_Color_Shading  param_color_shading_ = manage.param_color_shading;
        Color_Ratio_Value  ratio_value;

        TestItem::test_roi_colorratio( Algorithm_all[site_].mean_gr_roi, Algorithm_all[site_].mean_gb_roi, w, h,
                                       param_color_shading_.skip,
                                       param_color_shading_.ROI_X_Cnt,
                                       param_color_shading_.ROI_Y_Cnt,
                                       Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                       Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
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

        double roi_rg_ratio_for_cs_max = *std::max_element(ratio_value.roi_rg_ratio.begin(),ratio_value.roi_rg_ratio.end());
        double roi_bg_ratio_for_cs_max = *std::max_element(ratio_value.roi_bg_ratio.begin(),ratio_value.roi_bg_ratio.end());
        double roi_gg_ratio_for_cs_max = *std::max_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double roi_rg_ratio_for_cs_min = *std::min_element(ratio_value.roi_rg_ratio.begin(),ratio_value.roi_rg_ratio.end());
        double roi_bg_ratio_for_cs_min = *std::min_element(ratio_value.roi_bg_ratio.begin(),ratio_value.roi_bg_ratio.end());
        double roi_gg_ratio_for_cs_min = *std::min_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double gg_center_for_cs = ratio_value.GG_ratio.value(QPair<QString,double>("center",manage.param_color_ratio_.GG_Ratio_target.value("center")));
        double rg_center_for_cs = ratio_value.RG_ratio.value(QPair<QString,double>("center",manage.param_color_ratio_.RG_Ratio_target.value("center")));
        double bg_center_for_cs = ratio_value.BG_ratio.value(QPair<QString,double>("center",manage.param_color_ratio_.BG_Ratio_target.value("center")));


        if ( Algorithm_all[site_].mean_gr_roi>10)
        {
            double rg_roi_max_center_1 = 0.00;
            double rg_roi_min_center_1 = 0.00;
            double rg_shading = 0.00;
            double bg_roi_max_center_1 = 0.00;
            double bg_roi_min_center_1 = 0.00;
            double bg_shading = 0.00;

            rg_roi_max_center_1 = std::fabs(roi_rg_ratio_for_cs_max/rg_center_for_cs - 1);
            rg_roi_min_center_1 = std::fabs(roi_rg_ratio_for_cs_min/rg_center_for_cs - 1);
            rg_shading = std::max(rg_roi_max_center_1,rg_roi_min_center_1);

            bg_roi_max_center_1 = std::fabs(roi_bg_ratio_for_cs_max/bg_center_for_cs - 1);
            bg_roi_min_center_1 = std::fabs(roi_bg_ratio_for_cs_min/bg_center_for_cs - 1);
            bg_shading = std::max(bg_roi_max_center_1,bg_roi_min_center_1);

            g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|")
                                   % rg_roi_max_center_1
                                   % rg_roi_min_center_1
                                   % rg_shading
                                   % bg_roi_max_center_1
                                   % bg_roi_min_center_1
                                   % bg_shading
                                   );

            g_str_msg_all[site_]+=boost::str(boost::format("17x13 ROI Color Ratio Max/Center-1:\n"
                                                 "rg_roi_max/center-1=%.4f,rg_roi_min/center-1=%.4f,rg_shading=%.4f\n"
                                                 "bg_roi_max/center-1=%.4f,bg_roi_min/center-1=%.4f,bg_shading=%.4f\n")
                                   % rg_roi_max_center_1
                                   % rg_roi_min_center_1
                                   % rg_shading
                                   % bg_roi_max_center_1
                                   % bg_roi_min_center_1
                                   % bg_shading
                                   );

            double shading_factor_center_c1_for_cs=manage.param_color_shading.factor_center_class1;
            double shading_factor_center_c2_for_cs=manage.param_color_shading.factor_center_class2;

            if(rg_shading > shading_factor_center_c2_for_cs ||bg_shading > shading_factor_center_c2_for_cs)
            {
                result_cs.is_passed = false;
                result_cs.bin = param_color_shading_.bin_shanding;
                result_cs.grade = param_color_shading_.grade_shanding;
            }
            else if(rg_shading > shading_factor_center_c1_for_cs ||bg_shading > shading_factor_center_c1_for_cs)
            {
                result_cs_MA.is_passed = false;
                result_cs_MA.bin = param_color_shading_.bin_shanding_MA;
                result_cs_MA.grade = param_color_shading_.grade_shanding_MA;
            }

            g_str_msg_all[site_]+=boost::str(boost::format(
                                                 "SPEC:\n"
                                                 "rg_shading=%.4f, bg_shading=%.4f, (SPEC_c2 :%.4f)\n"
                                                 "rg_shading=%.4f, bg_shading=%.4f, (SPEC_c1 :%.4f)\n")
                                             % rg_shading % bg_shading % shading_factor_center_c2_for_cs
                                             % rg_shading % bg_shading % shading_factor_center_c1_for_cs);

        }
        else
        {
            g_str_msg_all[site_].append ("mean_g is low, not test color shading!!!\n");
            g_str_log_all[site_]+=boost::str(boost::format("-1|-1|-1|-1|-1|-1|"));
        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_cs.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
    {
        result_cs.g_msg = false;
        result_cs_MA.g_msg = false;

    }

    vector<Subitem_Result> Color_Shading_test_result;
    Color_Shading_test_result.push_back(result_cs);
    Color_Shading_test_result.push_back(result_cs_MA);


    return Color_Shading_test_result;

}


vector<Subitem_Result> BrightTest::Color_Shading_V1_1( ParamManage& manage)
{

    int w=manage.width_;
    int h=manage.height_;
    Subitem_Result result_cs ("Color_Shading_V1.1",true);
    Subitem_Result result_cs_MA ("Color_Shading_MA_V1.1",true);

    if ((manage.param_color_ratio_.ratio_open_mode_ == 1 && manage.param_color_shading.open_mode_ == 1))
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- ROI Color Shading V1.1-----------------------------\n"));

        Param_Color_Ratio  param_color_ratio_=manage.param_color_ratio_;
        Param_Color_Shading  param_color_shading_ = manage.param_color_shading;
        Color_Ratio_Value  ratio_value;

        TestItem::test_roi_colorratio( Algorithm_all[site_].mean_gr_roi, Algorithm_all[site_].mean_gb_roi, w, h,
                                       param_color_shading_.skip,
                                       param_color_shading_.ROI_X_Cnt,
                                       param_color_shading_.ROI_Y_Cnt,
                                       Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                       Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
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

        double roi_rg_ratio_for_cs_max = *std::max_element(ratio_value.roi_rg_ratio.begin(),ratio_value.roi_rg_ratio.end());
        double roi_bg_ratio_for_cs_max = *std::max_element(ratio_value.roi_bg_ratio.begin(),ratio_value.roi_bg_ratio.end());
        double roi_gg_ratio_for_cs_max = *std::max_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double roi_rg_ratio_for_cs_min = *std::min_element(ratio_value.roi_rg_ratio.begin(),ratio_value.roi_rg_ratio.end());
        double roi_bg_ratio_for_cs_min = *std::min_element(ratio_value.roi_bg_ratio.begin(),ratio_value.roi_bg_ratio.end());
        double roi_gg_ratio_for_cs_min = *std::min_element(ratio_value.roi_gg_ratio.begin(),ratio_value.roi_gg_ratio.end());

        double gg_center_for_cs = ratio_value.GG_ratio.value(QPair<QString,double>("center",manage.param_color_ratio_.GG_Ratio_target.value("center")));
        double rg_center_for_cs = ratio_value.RG_ratio.value(QPair<QString,double>("center",manage.param_color_ratio_.RG_Ratio_target.value("center")));
        double bg_center_for_cs = ratio_value.BG_ratio.value(QPair<QString,double>("center",manage.param_color_ratio_.BG_Ratio_target.value("center")));


        if ( Algorithm_all[site_].mean_gr_roi>10)
        {
            double rg_roi_max_center_1 = 0.00;
            double rg_roi_min_center_1 = 0.00;
            double rg_shading = 0.00;
            double bg_roi_max_center_1 = 0.00;
            double bg_roi_min_center_1 = 0.00;
            double bg_shading = 0.00;

            rg_roi_max_center_1 = std::fabs(roi_rg_ratio_for_cs_max/rg_center_for_cs - 1);
            rg_roi_min_center_1 = std::fabs(roi_rg_ratio_for_cs_min/rg_center_for_cs - 1);
            rg_shading = std::max(rg_roi_max_center_1,rg_roi_min_center_1);

            bg_roi_max_center_1 = std::fabs(roi_bg_ratio_for_cs_max/bg_center_for_cs - 1);
            bg_roi_min_center_1 = std::fabs(roi_bg_ratio_for_cs_min/bg_center_for_cs - 1);
            bg_shading = std::max(bg_roi_max_center_1,bg_roi_min_center_1);

            g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|")
                                   % rg_roi_max_center_1
                                   % rg_roi_min_center_1
                                   % rg_shading
                                   % bg_roi_max_center_1
                                   % bg_roi_min_center_1
                                   % bg_shading
                                   );

            g_str_msg_all[site_]+=boost::str(boost::format("17x13 ROI Color Ratio Max/Center-1:\n"
                                                 "rg_roi_max/center-1=%.4f,rg_roi_min/center-1=%.4f,rg_shading=%.4f\n"
                                                 "bg_roi_max/center-1=%.4f,bg_roi_min/center-1=%.4f,bg_shading=%.4f\n")
                                   % rg_roi_max_center_1
                                   % rg_roi_min_center_1
                                   % rg_shading
                                   % bg_roi_max_center_1
                                   % bg_roi_min_center_1
                                   % bg_shading
                                   );

            double shading_factor_center_rg_c1_for_cs=manage.param_color_shading.factor_center_rg_class1;
            double shading_factor_center_rg_c2_for_cs=manage.param_color_shading.factor_center_rg_class2;
            double shading_factor_center_bg_c1_for_cs=manage.param_color_shading.factor_center_bg_class1;
            double shading_factor_center_bg_c2_for_cs=manage.param_color_shading.factor_center_bg_class2;

            if(rg_shading > shading_factor_center_rg_c2_for_cs)
            {
                result_cs.is_passed = false;
                result_cs.bin = param_color_shading_.bin_shanding;
                result_cs.grade = param_color_shading_.grade_shanding;
            }
            else if(rg_shading > shading_factor_center_rg_c1_for_cs )
            {
                result_cs_MA.is_passed = false;
                result_cs_MA.bin = param_color_shading_.bin_shanding_MA;
                result_cs_MA.grade = param_color_shading_.grade_shanding_MA;
            }
            if(bg_shading > shading_factor_center_bg_c2_for_cs )
            {
                result_cs.is_passed = false;
                result_cs.bin = param_color_shading_.bin_shanding;
                result_cs.grade = param_color_shading_.grade_shanding;
            }
            else if(bg_shading > shading_factor_center_bg_c1_for_cs )
            {
                result_cs_MA.is_passed = false;
                result_cs_MA.bin = param_color_shading_.bin_shanding_MA;
                result_cs_MA.grade = param_color_shading_.grade_shanding_MA;
            }


            g_str_msg_all[site_]+=boost::str(boost::format(
                                                 "SPEC:\n"
                                                 "rg_shading=%.4f,  (SPEC_c2 :%.4f),bg_shading=%.4f, (SPEC_c2 :%.4f)\n"
                                                 "rg_shading=%.4f,  (SPEC_c1 :%.4f),bg_shading=%.4f, (SPEC_c1 :%.4f)\n")
                                             % rg_shading % shading_factor_center_rg_c2_for_cs % bg_shading % shading_factor_center_bg_c2_for_cs
                                             % rg_shading % shading_factor_center_rg_c1_for_cs % bg_shading % shading_factor_center_bg_c1_for_cs);
        }
        else
        {
            g_str_msg_all[site_].append ("mean_g is low, not test color shading!!!\n");
            g_str_log_all[site_]+=boost::str(boost::format("-1|-1|-1|-1|-1|-1|"));
        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_cs.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
    {
        result_cs.g_msg = false;
        result_cs_MA.g_msg = false;
    }

    vector<Subitem_Result> Color_Shading_test_result;
    Color_Shading_test_result.push_back(result_cs);
    Color_Shading_test_result.push_back(result_cs_MA);


    return Color_Shading_test_result;
}



vector<Subitem_Result> BrightTest::Color_Shading_5_5( ParamManage& manage)
{
    if(manage.color_shading_5x5_version == "color_shading_5_5_V1.0")
        return Color_Shading_5_5_V1_0(manage);


    Subitem_Result res("Color_Shading_5*5", false);
    res.g_msg = false;
    vector<Subitem_Result> Color_Shading_test_result;
    Color_Shading_test_result.push_back(res);
    return Color_Shading_test_result;
}

vector<Subitem_Result> BrightTest::Color_Shading_5_5_V1_0( ParamManage& manage)
{

    int w=manage.width_;
    int h=manage.height_;
    Subitem_Result result_cs ("Color_Shading_5*5",true);
    //Subitem_Result result_cs_MA ("Color_Shading_MA",true);

    if (manage.param_color_shading_5x5.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- ROI Color Shading 5*5 -----------------------------\n"));

        Param_Color_Ratio  param_color_ratio_=manage.param_color_ratio_;
        Param_Color_Shading_5x5  param_color_shading_ = manage.param_color_shading_5x5;
        Color_Ratio_Value  ratio_value;

        TestItem::test_roi_colorratio( Algorithm_all[site_].mean_gr_roi, Algorithm_all[site_].mean_gb_roi, w, h,
                                       manage.param_color_shading_5x5.skip,
                                       manage.param_color_shading_5x5.ROI_X_Cnt,
                                       manage.param_color_shading_5x5.ROI_Y_Cnt,
                                       Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                       Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
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

        //可优化可读性
        ratio_value.rg_leftup = ratio_value.RG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.RG_Ratio_target.value("leftup")));
        ratio_value.bg_leftup = ratio_value.BG_ratio.value(QPair<QString,double>("leftup",param_color_ratio_.BG_Ratio_target.value("leftup")));

        ratio_value.rg_rightup = ratio_value.RG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.RG_Ratio_target.value("rightup")));
        ratio_value.bg_rightup = ratio_value.BG_ratio.value(QPair<QString,double>("rightup",param_color_ratio_.BG_Ratio_target.value("rightup")));

        ratio_value.rg_leftdown = ratio_value.RG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.RG_Ratio_target.value("leftdown")));
        ratio_value.bg_leftdown = ratio_value.BG_ratio.value(QPair<QString,double>("leftdown",param_color_ratio_.BG_Ratio_target.value("leftdown")));

        ratio_value.rg_rightdown = ratio_value.RG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.RG_Ratio_target.value("rightdown")));
        ratio_value.bg_rightdown = ratio_value.BG_ratio.value(QPair<QString,double>("rightdown",param_color_ratio_.BG_Ratio_target.value("rightdown")));

        ratio_value.rg_center = ratio_value.RG_ratio.value(QPair<QString,double>("center",param_color_ratio_.RG_Ratio_target.value("center")));
        ratio_value.bg_center = ratio_value.BG_ratio.value(QPair<QString,double>("center",param_color_ratio_.BG_Ratio_target.value("center")));


        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|")
                                         % ratio_value.rg_leftup
                                         % ratio_value.bg_leftup
                                         % ratio_value.rg_rightup
                                         % ratio_value.bg_rightup
                                         % ratio_value.rg_leftdown
                                         % ratio_value.bg_leftdown
                                         % ratio_value.rg_rightdown
                                         % ratio_value.bg_rightdown
                                         % ratio_value.rg_center
                                         % ratio_value.bg_center
                                         );
        g_str_msg_all[site_]+=boost::str(boost::format("color ratio:\n"
                                                       "ratio_r_g_left_up=%.4f,ratio_b_g_left_up=%.4f\n"
                                                       "ratio_r_g_right_up=%.4f,ratio_b_g_right_up=%.4f\n"
                                                       "ratio_r_g_left_down=%.4f,ratio_b_g_left_down=%.4f\n"
                                                       "ratio_r_g_right_down=%.4f,ratio_b_g_right_down=%.4f\n"
                                                       "ratio_r_g_center=%.4f,ratio_b_g_center=%.4f\n")
                                         % ratio_value.rg_leftup
                                         % ratio_value.bg_leftup
                                         % ratio_value.rg_rightup
                                         % ratio_value.bg_rightup
                                         % ratio_value.rg_leftdown
                                         % ratio_value.bg_leftdown
                                         % ratio_value.rg_rightdown
                                         % ratio_value.bg_rightdown
                                         % ratio_value.rg_center
                                         % ratio_value.bg_center
                                         );

        double RG1 = 2.000;
        double RG2 = 2.000;
        double RG3 = 2.000;
        double BG1 = 2.000;
        double BG2 = 2.000;
        double BG3 = 2.000;

        if(ratio_value.rg_rightdown !=0 && ratio_value.rg_leftdown !=0 && ratio_value.bg_leftdown !=0 && ratio_value.rg_leftdown !=0)
        {
            RG1 = ratio_value.rg_leftup/ratio_value.rg_rightdown;
            RG2 = ratio_value.rg_rightup/ratio_value.rg_leftdown;
            RG3 = (4*ratio_value.rg_center)/(ratio_value.rg_leftup+ratio_value.rg_rightup+ratio_value.rg_leftdown+ratio_value.rg_rightdown);
            BG1 = ratio_value.bg_leftup/ratio_value.bg_rightdown;
            BG2 = ratio_value.bg_rightup/ratio_value.bg_leftdown;
            BG3 = (4*ratio_value.bg_center)/(ratio_value.bg_leftup+ratio_value.bg_rightup+ratio_value.bg_leftdown+ratio_value.bg_rightdown);
        }

        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|")
                                         % RG1
                                         % RG2
                                         % RG3
                                         % BG1
                                         % BG2
                                         % BG3
                                         );
        g_str_msg_all[site_]+=boost::str(boost::format("color shading:\n"
                                                       "RG1=%.4f,RG2=%.4f,RG3=%.4f\n"
                                                       "BG1=%.4f,BG2=%.4f,BG3=%.4f\n")
                                         % RG1
                                         % RG2
                                         % RG3
                                         % BG1
                                         % BG2
                                         % BG3
                                         );


        g_str_msg_all[site_] += boost::str(boost::format("SPEC:\n"
                                                         "RG1=%.4f (SPEC: %.4f ~ %.4f) \n"
                                                         "RG2=%.4f (SPEC: %.4f ~ %.4f) \n"
                                                         "RG3=%.4f (SPEC: %.4f ~ %.4f) \n"
                                                         "BG1=%.4f (SPEC: %.4f ~ %.4f) \n"
                                                         "BG2=%.4f (SPEC: %.4f ~ %.4f) \n"
                                                         "BG3=%.4f (SPEC: %.4f ~ %.4f) \n")
                                             % RG1
                                             % manage.param_color_shading_5x5.rg1_min
                                             % manage.param_color_shading_5x5.rg1_max
                                             % RG2
                                             % manage.param_color_shading_5x5.rg2_min
                                             % manage.param_color_shading_5x5.rg2_max
                                             % RG3
                                             % manage.param_color_shading_5x5.rg3_min
                                             % manage.param_color_shading_5x5.rg3_max
                                             % BG1
                                             % manage.param_color_shading_5x5.bg1_min
                                             % manage.param_color_shading_5x5.bg1_max
                                             % BG2
                                             % manage.param_color_shading_5x5.bg2_min
                                             % manage.param_color_shading_5x5.bg2_max
                                             % BG3
                                             % manage.param_color_shading_5x5.bg3_min
                                             % manage.param_color_shading_5x5.bg3_max);

        if(RG3 > manage.param_color_shading_5x5.rg3_max || RG3 < manage.param_color_shading_5x5.rg3_min)
        {
            result_cs.is_passed = false;
            result_cs.bin = param_color_shading_.bin_shanding;
            result_cs.grade = param_color_shading_.grade_shanding;
        }
        else if(RG1 > manage.param_color_shading_5x5.rg1_max || RG1 < manage.param_color_shading_5x5.rg1_min)
        {
            result_cs.is_passed = false;
            result_cs.bin = param_color_shading_.bin_shanding;
            result_cs.grade = param_color_shading_.grade_shanding;
        }
        else if(RG2 > manage.param_color_shading_5x5.rg2_max || RG2 < manage.param_color_shading_5x5.rg2_min)
        {
            result_cs.is_passed = false;
            result_cs.bin = param_color_shading_.bin_shanding;
            result_cs.grade = param_color_shading_.grade_shanding;
        }
        else if(BG3 > manage.param_color_shading_5x5.bg3_max || BG3 < manage.param_color_shading_5x5.bg3_min)
        {
            result_cs.is_passed = false;
            result_cs.bin = param_color_shading_.bin_shanding;
            result_cs.grade = param_color_shading_.grade_shanding;
        }
        else if(BG1 > manage.param_color_shading_5x5.bg1_max || BG1 < manage.param_color_shading_5x5.bg1_min)
        {
            result_cs.is_passed = false;
            result_cs.bin = param_color_shading_.bin_shanding;
            result_cs.grade = param_color_shading_.grade_shanding;
        }
        else if(BG2 > manage.param_color_shading_5x5.bg2_max || BG2 < manage.param_color_shading_5x5.bg2_min)
        {
            result_cs.is_passed = false;
            result_cs.bin = param_color_shading_.bin_shanding;
            result_cs.grade = param_color_shading_.grade_shanding;
        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_cs.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";


    }
    else
    {
        result_cs.g_msg = false;
    }

    vector<Subitem_Result> Color_Shading_test_result;
    Color_Shading_test_result.push_back(result_cs);

    return Color_Shading_test_result;
}



vector<Subitem_Result> BrightTest::Color_Shading_15_15(ParamManage &manage)
{
    if(manage.color_shading_15x15_version == "color_shading_15x15_V1.0")
        return Color_Shading_15_15_V1_0(manage);

    Subitem_Result res("Color_Shading_15*15", false);
    res.g_msg = false;
    vector<Subitem_Result> Color_Shading_test_result;
    Color_Shading_test_result.push_back(res);
    return Color_Shading_test_result;
}


vector<Subitem_Result> BrightTest::Color_Shading_15_15_V1_0(ParamManage &manage)
{
    int w=manage.width_;
    int h=manage.height_;
    Subitem_Result result_cs_15x15 ("Color_Shading_15x15",true);

    if (manage.param_color_shading_15x15.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();
        g_str_msg_all[site_]+=boost::str(boost::format("-------------------------- ROI Color Shading 15x15 V1.0-----------------------------\n"));

        Param_Color_Shading_15x15  param_color_shading_ = manage.param_color_shading_15x15;

        int x_cnt = param_color_shading_.ROI_X_Cnt;
        int y_cnt = param_color_shading_.ROI_Y_Cnt;
        int x_center_index = x_cnt / 2;
        int y_center_index = y_cnt / 2;

        double out_DiffCorner, out_Maxdiff;
        std::string msg;

        TestItem::TestFunc_LSC(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                               Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_, w, h, param_color_shading_.skip,
                               x_cnt, y_cnt,  x_center_index, y_center_index, 8, false, out_DiffCorner, out_Maxdiff, msg);


        if(out_DiffCorner > manage.param_color_shading_15x15.max_diff_corner || out_Maxdiff > manage.param_color_shading_15x15.max_diff_edge)
        {
            result_cs_15x15.bin = manage.param_color_shading_15x15.bin_shanding;
            result_cs_15x15.grade = manage.param_color_shading_15x15.grade_shanding;
            result_cs_15x15.is_passed = false;
        }

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+=boost::str((boost::format("Max Diff Corner: %.4f SPEC:(%.4f)\nMax Diff Edge: %.4f SPEC:(%.4f)\n")
                                          % out_DiffCorner % manage.param_color_shading_15x15.max_diff_corner
                                          % out_Maxdiff % manage.param_color_shading_15x15.max_diff_edge));
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|")%out_DiffCorner%out_Maxdiff);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_cs_15x15.name.c_str());

    }


    vector<Subitem_Result> Color_Shading_test_result;
    Color_Shading_test_result.push_back(result_cs_15x15);
    return Color_Shading_test_result;

}


//白屏 Optical Center Test
Subitem_Result BrightTest::Optical_Center( ParamManage& manage)
{
    if(manage.optical_center_version == "Opticalcenter_V1.0")
        return Optical_Center_V1_0(manage);

    Subitem_Result res("result_optical_center", false);
    res.g_msg = false;
    return res;
}




Subitem_Result BrightTest::Optical_Center_V1_0( ParamManage& manage)
{
    Subitem_Result result_optical_center("result_optical_center",true);

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_optical_center_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Optical Center Test---------------------------\n"));

        //中心亮度
        int luminance_center = 0;
        int ROI_w=w/5;
        int ROI_h=h/5;
        for (int i=(ROI_h*2);i<(ROI_h*3);i++)
        {

            for(int j=(ROI_w*2);j<(ROI_w*3);j++)
            {

                luminance_center+=Algorithm_all[site_].g_buf_raw_[i*w+j];

            }
        }

        luminance_center/=(ROI_w*ROI_h);
        g_str_log_all[site_]+=boost::str(boost::format("%d|")
                                         % luminance_center );
        g_str_msg_all[site_]+=boost::str(boost::format("luminance_center is :  %d\n")
                               % luminance_center );

        //光心



        cv::Mat img_tmp = cv::Mat(h,w,CV_8U);
        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp.ptr<uchar>(i);
            for(int j=0;j<w;j++)
            {
                if(Algorithm_all[site_].g_buf_raw_[i*w+j]>120 && Algorithm_all[site_].g_buf_raw_[i*w+j]<200)
                    p[j]=Algorithm_all[site_].g_buf_raw_[i*w+j]*3.1875-382.5;
                else if(Algorithm_all[site_].g_buf_raw_[i*w+j]<=120)
                    p[j]=0;
                else if(Algorithm_all[site_].g_buf_raw_[i*w+j]>=200)
                    p[j]=255;
            }
        }




        long long sum_x=0;
        long long sum_y=0;
        long long sum_lux=0;

        for (int i=0;i<h;i++)
        {
            uchar *p=img_tmp.ptr<uchar>(i);
            for(int j=0;j<w;j++)
            {
                sum_x += j * p[j];
                sum_y += i * p[j];
                sum_lux +=  p[j];
            }
        }

        int x_opt =0;
        int y_opt =0;
        if(sum_lux>0)
        {
            x_opt= sum_x/sum_lux;
            y_opt= sum_y/sum_lux;
        }
        else
        {
            x_opt= -1;
            y_opt= -1;
        }

        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|")
                                         % x_opt % y_opt );
        g_str_msg_all[site_]+=boost::str(boost::format("Optical Illumination center X_pos: %d, Y_pos: %d\n")
                                         % x_opt % y_opt );

        int x_diff= std::abs(x_opt-w/2);
        int y_diff= std::abs(y_opt-h/2);

        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|")
                                         % (x_opt-w/2) % (y_opt-h/2) );
        g_str_msg_all[site_]+=boost::str(boost::format("Optical Illumination center X_Shift: %d, Y_Shift: %d\n")
                                         % (x_opt-w/2) % (y_opt-h/2)  );

        g_str_msg_all[site_] += boost::str(boost::format(
                                                         "SPEC:\n"
                                                         "x_diff= %.4f (SPEC: %.4f) \n"
                                                         "y_diff= %.4f (SPEC: %.4f) \n")
                                           % x_diff
                                           % manage.param_optical_center_.spec_x_
                                           % y_diff
                                           % manage.param_optical_center_.spec_y_
                                           );



        if (x_diff >= manage.param_optical_center_.spec_x_ ||
                y_diff >= manage.param_optical_center_.spec_y_ )
        {
            result_optical_center.is_passed=false;
            result_optical_center.bin=manage.param_optical_center_.bin_;
            result_optical_center.grade=manage.param_optical_center_.grade_;
        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % result_optical_center.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
    {
        result_optical_center.g_msg = false;
    }


    return result_optical_center;
}



