#include "Darktest.h"

int DarkTest::TestFunc_Normal_Dark_ALL(ParamManage& manage,uchar *pData,uchar *pImage_save)
{
    //-----------------Parameter Check--------------------
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }

    logMsg (std::string("into dark frame test, check params first!"),site_);
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16) )
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    logMsg("check params pass!",site_);
    //分配空间并分通道
    bool result_Split=Algorithm_all[site_].Split_Channel(manage, test_channel_type,true,
                                                         pData,test_frameCnt,test_datatype,pImage_save);
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }

    logMsg("Split_Channel  pass!",site_);
    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    items_results.push_back(Algorithm_all[site_].Light_Response(manage));
    items_results.push_back(Algorithm_all[site_].Non_Uniformity(manage));
    items_results.push_back(Algorithm_all[site_].POG_Test(manage,POG_Normal));

    logMsg("ROI_DSNU  start!",site_);
    if(!Algorithm_all[site_].New_PRNU(manage.width_,manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }
    items_results.push_back(Histogram_Speical_DN_Count(manage));//此项顺序不可调整
    items_results.push_back(Algorithm_all[site_].Pixel_Defect_Test(manage,External_Configuration));
    items_results.push_back(Algorithm_all[site_].Single_Pixel_Defect_Test(manage,External_Configuration));//只想卡单点，且可以分梯度
    items_results.push_back(PD_DataCollect(manage));
    items_results.push_back(Algorithm_all[site_].ROI_DSNU(manage));
    Algorithm_all[site_].Free_PRNU();
    logMsg("ROI_DSNU  end!",site_);
    items_results.push_back( Deadrow_Deadcol(manage,false));
    items_results.push_back( FPN_DL_Bright_Dark(manage,false));

    vector<Subitem_Result>  res_OB=OB_and_DarkShading(manage);
    items_results.insert(items_results.end(),res_OB.begin(),res_OB.end());
    items_results.push_back(Algorithm_all[site_].Ref_Col_Light(manage));
    items_results.push_back(Global_LUM_DL_test_CP(manage));
    items_results.push_back(OB_and_GRGB(manage));

    logMsg("OB_and_DarkShading  end!",site_);

    Subitem_Result dark_test_final_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);
    return dark_test_final_result.bin;
}



int DarkTest::TestFunc_IIC_Dark_(ParamManage& manage,uchar *pData,uchar *pImage_save)
{
    //-----------------Parameter Check--------------------
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }

    logMsg (std::string("into dark frame test, check params first!"),site_);
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16) )
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    logMsg("check params pass!",site_);
    //分配空间并分通道
    bool result_Split=Algorithm_all[site_].Split_Channel(manage, test_channel_type,true,
                                                         pData,test_frameCnt,test_datatype,pImage_save);
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }

    logMsg("Split_Channel  pass!",site_);
    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合

    logMsg("ROI_DSNU  start!",site_);
    if(!Algorithm_all[site_].New_PRNU(manage.width_,manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }

    items_results.push_back(Algorithm_all[site_].Pixel_Defect_Test_NO_Test_Info(manage,Pixel_Defect_dark));

    Algorithm_all[site_].Free_PRNU();
    logMsg("ROI_DSNU  end!",site_);
    items_results.push_back( Deadrow_Deadcol_IIC(manage,false));
    logMsg("OB_and_DarkShading  end!",site_);

    Subitem_Result dark_test_final_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);
    return dark_test_final_result.bin;
}


int DarkTest::TestFunc_IIC_LIGHT_Dark_(ParamManage& manage,uchar *pData,uchar *pImage_save)
{
    //-----------------Parameter Check--------------------
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }

    logMsg (std::string("into dark frame test, check params first!"),site_);
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16) )
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    logMsg("check params pass!",site_);
    //分配空间并分通道
    bool result_Split=Algorithm_all[site_].Split_Channel(manage, test_channel_type,true,
                                                         pData,test_frameCnt,test_datatype,pImage_save);
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }

    logMsg("Split_Channel  pass!",site_);
    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合
    items_results.push_back(Algorithm_all[site_].Light_Response(manage));
    logMsg("ROI_DSNU  start!",site_);
    if(!Algorithm_all[site_].New_PRNU(manage.width_,manage.height_))
    {
        g_str_msg_all[site_].append ("New_PRNU failed!\n");
        return -1;
    }

    items_results.push_back(Algorithm_all[site_].Pixel_Defect_Test_NO_Test_Info(manage,Pixel_Defect_dark));

    Algorithm_all[site_].Free_PRNU();
    logMsg("ROI_DSNU  end!",site_);
    items_results.push_back( Deadrow_Deadcol_IIC(manage,false));
    logMsg("OB_and_DarkShading  end!",site_);

    Subitem_Result dark_test_final_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);
    return dark_test_final_result.bin;
}



int DarkTest::TestFunc_DAG_(ParamManage& manage,uchar *pData,uchar *pImage_save)
{
    //-----------------Parameter Check--------------------
    if (test_frameCnt<=0)
    {
        g_str_msg_all[site_].append ("frameCnt<=0!!!\n");
        return -1;
    }

    logMsg (std::string("into dark frame test, check params first!"),site_);
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16) )
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    logMsg("check params pass!",site_);
    //分配空间并分通道
    bool result_Split=Algorithm_all[site_].Split_Channel(manage, test_channel_type,true,
                                                         pData,test_frameCnt,test_datatype,pImage_save);
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }

    logMsg("Split_Channel  pass!",site_);

    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合
    items_results.push_back(Algorithm_all[site_].Light_Response(manage));
    items_results.push_back(Algorithm_all[site_].DAG(manage,pData));
    items_results.push_back(Algorithm_all[site_].Light_Linearity_Ushort(manage));
    items_results.push_back(Algorithm_all[site_].DAG_CycleStd(manage,pData,test_frameCnt));

    Subitem_Result dark_test_final_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);
    return dark_test_final_result.bin;

}



//黑屏 Deadrow and Deadcol  注：需要使用g_binary_
Subitem_Result DarkTest::Deadrow_Deadcol(const ParamManage& manage,bool simple_test_COM)
{
    if(manage.fpn_dl_version == "fpn_dl_V1.0")
       return Deadrow_Deadcol_V1_0( manage,simple_test_COM);
    else if(manage.fpn_dl_version == "fpn_dl_V1.1")
       return Deadrow_Deadcol_V1_1(manage,simple_test_COM);

    Subitem_Result res("Deadrow_Deadcol", false);
    res.g_msg = false;
    return res;
}


Subitem_Result DarkTest::Deadrow_Deadcol_V1_0(const ParamManage& manage,bool simple_test_COM)
{
    Algorithm_all[site_].time_begin=clock();
    Subitem_Result resule_line("Deadrow_Deadcol_Dark",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_fpn_dl_.open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadrow and Deadcol Dark V1.0-----------------------------\n"));
        int replace_DN = 0;
        if(Algorithm_all[site_].mean_gr_roi >0)
            replace_DN = Algorithm_all[site_].mean_gr_roi;
        else
            replace_DN = 16;

        //坏点抹掉后进行线条查找  简测时不需要补
        if(!simple_test_COM)
        {
            for(int i=0; i<h; i++)
                for(int j=0; j<w; j++)
                {
                    if(Algorithm_all[site_].g_binary_[i*w+j]>128)
                        Algorithm_all[site_].g_buf_raw_[i*w+j]=replace_DN;
                }

            TestItem::split_channel(manage.bayer_,w,h,
                                    Algorithm_all[site_].g_buf_raw_,Algorithm_all[site_].g_buf_r_,
                                    Algorithm_all[site_].g_buf_gr_,Algorithm_all[site_].g_buf_gb_,
                                    Algorithm_all[site_].g_buf_b_);
        }


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


        TestItem::test_row_defect_bychannel(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                            Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                            manage.param_fpn_dl_.strip_row_,
                                            manage.param_fpn_dl_.r_area_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.deadrow_spec_,
                                            manage.param_fpn_dl_.deadrow_spec_,
                                            manage.param_fpn_dl_.deadrow_spec_,
                                            manage.param_fpn_dl_.deadrow_spec_,
                                            max_row_diff, max_row_pos, row_max_min_diff, cnt_dead_row,
                                            cnt_row_dol, row_dol_std_max, row_dol_std_max_pos,
                                            manage.param_fpn_dl_.break_row_spec_,
                                            manage.param_fpn_dl_.break_row_spec_,
                                            manage.param_fpn_dl_.break_row_spec_,
                                            manage.param_fpn_dl_.break_row_spec_,
                                            cnt_breakline_row, breakline_row_max_value, breakline_row_max_pos,
                                            DN, g_str_msg_all[site_], manage.param_fpn_dl_.log_print_);


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


        TestItem::test_col_defect_bychannel(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                            Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                            manage.param_fpn_dl_.strip_col_,
                                            manage.param_fpn_dl_.r_area_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_.fpn_level_1_spec_,
                                            max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                            cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                            manage.param_fpn_dl_.break_col_spec_,
                                            manage.param_fpn_dl_.break_col_spec_,
                                            manage.param_fpn_dl_.break_col_spec_,
                                            manage.param_fpn_dl_.break_col_spec_,
                                            cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                            DN, g_str_msg_all[site_], manage.param_fpn_dl_.log_print_);


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




        logMsg (std::string("after dead line"));

        int cnt_dol = cnt_row_dol + cnt_col_dol;
        int cnt_break_line = cnt_breakline_row + cnt_breakline_col;
        if (cnt_dead_col>0 || cnt_dead_row>0 || cnt_dol>0 || cnt_break_line>0)
        {
            resule_line.is_passed = false;
        }

        int cnt_fpn_level_1=0;
        int cnt_fpn_level_2=0;
        int cnt_hard_dead_col=0;
        if(cnt_dead_col>0)
        {
            for (int i=0; i<4; i++)
            {
                if(max_col_diff[i]>manage.param_fpn_dl_.deadcol_spec_)
                    cnt_hard_dead_col++;
                else if(max_col_diff[i]>manage.param_fpn_dl_.fpn_level_2_spec_)
                    cnt_fpn_level_2++;
                else
                    cnt_fpn_level_1++;
            }
        }

        if (cnt_hard_dead_col>0)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_dead_col_;
            resule_line.grade= manage.param_fpn_dl_.grade_dead_col_;
        }
        else if (cnt_fpn_level_2 >0)
        {
            resule_line.bin= manage.param_fpn_dl_.bin_fpn_level_2_;
            resule_line.grade = manage.param_fpn_dl_.grade_fpn_level_2_;
        }
        else if (cnt_fpn_level_1 >0)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_fpn_level_1_;
            resule_line.grade = manage.param_fpn_dl_.grade_fpn_level_1_;
        }

        if (cnt_dead_row>0 && manage.param_fpn_dl_.grade_dead_row_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_dead_row_;
            resule_line.grade = manage.param_fpn_dl_.grade_dead_row_;
        }

        if (cnt_dol>0 && manage.param_fpn_dl_.grade_dol_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_dol_;
            resule_line.grade = manage.param_fpn_dl_.grade_dol_;
        }

        if (cnt_break_line>0 && manage.param_fpn_dl_.grade_break_line_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_break_line_;
            resule_line.grade = manage.param_fpn_dl_.grade_break_line_;
        }

//        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|")
//                               % cnt_hard_dead_col %cnt_fpn_level_2 %cnt_fpn_level_1);

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % resule_line.name.c_str());

    }
    else
    {
         resule_line.g_msg = false;
    }

    Algorithm_all[site_].time_end=clock();
    g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    return resule_line;
}


Subitem_Result DarkTest::Deadrow_Deadcol_V1_1(const ParamManage& manage,bool simple_test_COM)
{
    Algorithm_all[site_].time_begin=clock();
    Subitem_Result resule_line("Deadrow_Deadcol_Dark_V1.1",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_fpn_dl_.open_mode_ ==  1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadrow and Deadcol Dark V1.1-----------------------------\n"));
        int replace_DN = 0;
        if(Algorithm_all[site_].mean_gr_roi >0)
            replace_DN = Algorithm_all[site_].mean_gr_roi;
        else
            replace_DN = 16;

        //坏点抹掉后进行线条查找  简测时不需要补
        if(!simple_test_COM)
        {
            for(int i=0; i<h; i++)
                for(int j=0; j<w; j++)
                {
                    if(Algorithm_all[site_].g_binary_[i*w+j]>128)
                        Algorithm_all[site_].g_buf_raw_[i*w+j]=replace_DN;
                }

            TestItem::split_channel(manage.bayer_,w,h,
                                    Algorithm_all[site_].g_buf_raw_,Algorithm_all[site_].g_buf_r_,
                                    Algorithm_all[site_].g_buf_gr_,Algorithm_all[site_].g_buf_gb_,
                                    Algorithm_all[site_].g_buf_b_);
        }


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


        TestItem::test_row_defect_bychannel(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                            Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                            manage.param_fpn_dl_.strip_row_,
                                            manage.param_fpn_dl_.r_area_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.deadrow_spec_level1,
                                            manage.param_fpn_dl_.deadrow_spec_level1,
                                            manage.param_fpn_dl_.deadrow_spec_level1,
                                            manage.param_fpn_dl_.deadrow_spec_level1,
                                            max_row_diff, max_row_pos, row_max_min_diff, cnt_dead_row,
                                            cnt_row_dol, row_dol_std_max, row_dol_std_max_pos,
                                            manage.param_fpn_dl_.break_row_spec_,
                                            manage.param_fpn_dl_.break_row_spec_,
                                            manage.param_fpn_dl_.break_row_spec_,
                                            manage.param_fpn_dl_.break_row_spec_,
                                            cnt_breakline_row, breakline_row_max_value, breakline_row_max_pos,
                                            DN, g_str_msg_all[site_], manage.param_fpn_dl_.log_print_);


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


        TestItem::test_col_defect_bychannel(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                            Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                            manage.param_fpn_dl_.strip_col_,
                                            manage.param_fpn_dl_.r_area_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.spec_dol_,
                                            manage.param_fpn_dl_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_.fpn_level_1_spec_,
                                            max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                            cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                            manage.param_fpn_dl_.break_col_spec_,
                                            manage.param_fpn_dl_.break_col_spec_,
                                            manage.param_fpn_dl_.break_col_spec_,
                                            manage.param_fpn_dl_.break_col_spec_,
                                            cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                            DN, g_str_msg_all[site_], manage.param_fpn_dl_.log_print_);



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




        logMsg (std::string("after dead line"));

        int cnt_dol = cnt_row_dol + cnt_col_dol;
        int cnt_break_line = cnt_breakline_row + cnt_breakline_col;
        if (cnt_dead_col>0 || cnt_dead_row>0 || cnt_dol>0 || cnt_break_line>0)
        {
            resule_line.is_passed = false;
        }

        int cnt_fpn_level_1=0;
        int cnt_fpn_level_2=0;
        int cnt_hard_dead_col=0;
        int cnt_hard_dead_col_MA=0;
        int cnt_hard_dead_row=0;
        int cnt_hard_dead_row_MA=0;
        double iii= 0;
        if(cnt_dead_col>0)
        {
            for (int i=0; i<4; i++)
            {
                if(max_col_diff[i]>manage.param_fpn_dl_.deadcol_spec_level2)
                {
                    cnt_hard_dead_col++;
                    iii = max_col_diff[i];

                }
                else if(max_col_diff[i]>manage.param_fpn_dl_.deadcol_spec_level1)
                    cnt_hard_dead_col_MA++;

                if(max_col_diff[i]>manage.param_fpn_dl_.fpn_level_2_spec_)
                    cnt_fpn_level_2++;

                else
                    cnt_fpn_level_1++;
            }
        }

        g_str_msg_all[site_]+=boost::str(boost::format("cnt_hard_dead_col: %d, cnt_hard_dead_col_level2: %d, manage.param_fpn_dl_.deadcol_spec_level1: %d，manage.param_fpn_dl_.deadcol_spec_level2: %d\n")
                                         % cnt_hard_dead_col % cnt_hard_dead_col_MA % manage.param_fpn_dl_.deadcol_spec_level1 % manage.param_fpn_dl_.deadcol_spec_level2 );

        g_str_msg_all[site_]+=boost::str(boost::format("cnt_hard_dead_row: %d, cnt_hard_dead_row_level2: %d, manage.param_fpn_dl_.deadrow_spec_level1: %d，manage.param_fpn_dl_.deadrow_spec_level2: %d\n")
                                         % cnt_hard_dead_row % cnt_hard_dead_row_MA % manage.param_fpn_dl_.deadrow_spec_level1 % manage.param_fpn_dl_.deadrow_spec_level2 );

        if(cnt_dead_row>0)
        {
            for (int i=0; i<4; i++)
            {
                if(max_row_diff[i]>manage.param_fpn_dl_.deadrow_spec_level2)
                    cnt_hard_dead_row++;
                else if(max_row_diff[i]>manage.param_fpn_dl_.deadrow_spec_level1)
                    cnt_hard_dead_row_MA++;

            }
        }

        if (cnt_hard_dead_col>0)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_dead_col_level2;
            resule_line.grade= manage.param_fpn_dl_.grade_dead_col_level2;
        }
        else if (cnt_fpn_level_2 >0)
        {
            resule_line.bin= manage.param_fpn_dl_.bin_fpn_level_2_;
            resule_line.grade = manage.param_fpn_dl_.grade_fpn_level_2_;
        }
        else if (cnt_fpn_level_1 >0)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_fpn_level_1_;
            resule_line.grade = manage.param_fpn_dl_.grade_fpn_level_1_;
        }
        else if (cnt_hard_dead_col_MA >0)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_dead_col_level1;
            resule_line.grade= manage.param_fpn_dl_.grade_dead_col_level1;
        }

        if (cnt_hard_dead_row>0 && manage.param_fpn_dl_.grade_dead_row_level2>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_dead_row_level2;
            resule_line.grade = manage.param_fpn_dl_.grade_dead_row_level2;
        }
        else if (cnt_hard_dead_row_MA>0 && manage.param_fpn_dl_.grade_dead_row_level1>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_dead_row_level1;
            resule_line.grade = manage.param_fpn_dl_.grade_dead_row_level1;
        }

        if (cnt_dol>0 && manage.param_fpn_dl_.grade_dol_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_dol_;
            resule_line.grade = manage.param_fpn_dl_.grade_dol_;
        }

        if (cnt_break_line>0 && manage.param_fpn_dl_.grade_break_line_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_.bin_break_line_;
            resule_line.grade = manage.param_fpn_dl_.grade_break_line_;
        }

        //        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|")
        //                               % cnt_hard_dead_col %cnt_fpn_level_2 %cnt_fpn_level_1);

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % resule_line.name.c_str());

    }
    else
    {

        resule_line.g_msg = false;

    }

    Algorithm_all[site_].time_end=clock();
    g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    return resule_line;
}


//黑屏 IIC Deadrow and Deadcol
Subitem_Result DarkTest::Deadrow_Deadcol_IIC(const ParamManage& manage,bool simple_test_COM)
{
    if(manage.fpn_dl_iic_version == "fpn_dl_iic_V1.0")
        return Deadrow_Deadcol_IIC_V1_0(manage, simple_test_COM);

    Subitem_Result res("Deadrow_Deadcol", false);
    res.g_msg = false;
    return res;
}

Subitem_Result DarkTest::Deadrow_Deadcol_IIC_V1_0(const ParamManage& manage,bool simple_test_COM)
{
    Subitem_Result resule_line("Deadrow_Deadcol_IIC",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_fpn_dl_iic_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadrow_Deadcol_IIC V1.0-----------------------------\n"));

        int replace_DN = 0;
        if(Algorithm_all[site_].mean_gr_roi >0)
            replace_DN = Algorithm_all[site_].mean_gr_roi;
        else
            replace_DN = 16;

        //坏点抹掉后进行线条查找  简测时不需要补
        if(!simple_test_COM)
        {
            for(int i=0; i<h; i++)
                for(int j=0; j<w; j++)
                {
                    if(Algorithm_all[site_].g_binary_[i*w+j]>128)
                        Algorithm_all[site_].g_buf_raw_[i*w+j]=replace_DN;
                }

            TestItem::split_channel(manage.bayer_,w,h,
                                    Algorithm_all[site_].g_buf_raw_,Algorithm_all[site_].g_buf_r_,
                                    Algorithm_all[site_].g_buf_gr_,Algorithm_all[site_].g_buf_gb_,
                                    Algorithm_all[site_].g_buf_b_);
        }


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


        TestItem::test_row_defect_bychannel(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                            Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                            manage.param_fpn_dl_iic_.strip_row_,
                                            manage.param_fpn_dl_iic_.r_area_,
                                            manage.param_fpn_dl_iic_.spec_dol_,
                                            manage.param_fpn_dl_iic_.spec_dol_,
                                            manage.param_fpn_dl_iic_.spec_dol_,
                                            manage.param_fpn_dl_iic_.spec_dol_,
                                            manage.param_fpn_dl_iic_.deadrow_spec_,
                                            manage.param_fpn_dl_iic_.deadrow_spec_,
                                            manage.param_fpn_dl_iic_.deadrow_spec_,
                                            manage.param_fpn_dl_iic_.deadrow_spec_,
                                            max_row_diff, max_row_pos, row_max_min_diff, cnt_dead_row,
                                            cnt_row_dol, row_dol_std_max, row_dol_std_max_pos,
                                            manage.param_fpn_dl_iic_.break_row_spec_,
                                            manage.param_fpn_dl_iic_.break_row_spec_,
                                            manage.param_fpn_dl_iic_.break_row_spec_,
                                            manage.param_fpn_dl_iic_.break_row_spec_,
                                            cnt_breakline_row, breakline_row_max_value, breakline_row_max_pos,
                                            DN, g_str_msg_all[site_], manage.param_fpn_dl_iic_.log_print_);


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


        TestItem::test_col_defect_bychannel(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                            Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                            manage.param_fpn_dl_iic_.strip_col_,
                                            manage.param_fpn_dl_iic_.r_area_,
                                            manage.param_fpn_dl_iic_.spec_dol_,
                                            manage.param_fpn_dl_iic_.spec_dol_,
                                            manage.param_fpn_dl_iic_.spec_dol_,
                                            manage.param_fpn_dl_iic_.spec_dol_,
                                            manage.param_fpn_dl_iic_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_iic_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_iic_.fpn_level_1_spec_,
                                            manage.param_fpn_dl_iic_.fpn_level_1_spec_,
                                            max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                            cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                            manage.param_fpn_dl_iic_.break_col_spec_,
                                            manage.param_fpn_dl_iic_.break_col_spec_,
                                            manage.param_fpn_dl_iic_.break_col_spec_,
                                            manage.param_fpn_dl_iic_.break_col_spec_,
                                            cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                            DN, g_str_msg_all[site_], manage.param_fpn_dl_iic_.log_print_);


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



        logMsg (std::string("after dead line"));

        int cnt_dol = cnt_row_dol + cnt_col_dol;
        int cnt_break_line = cnt_breakline_row + cnt_breakline_col;
        if (cnt_dead_col>0 || cnt_dead_row>0 || cnt_dol>0 || cnt_break_line>0)
        {
            resule_line.is_passed = false;
        }

        int cnt_fpn_level_1=0;
        int cnt_fpn_level_2=0;
        int cnt_hard_dead_col=0;
        if(cnt_dead_col>0)
        {
            for (int i=0; i<4; i++)
            {
                if(max_col_diff[i]>manage.param_fpn_dl_iic_.deadcol_spec_)
                    cnt_hard_dead_col++;
                else if(max_col_diff[i]>manage.param_fpn_dl_iic_.fpn_level_2_spec_)
                    cnt_fpn_level_2++;
                else
                    cnt_fpn_level_1++;
            }
        }

        if (cnt_hard_dead_col>0)
        {
            resule_line.bin = manage.param_fpn_dl_iic_.bin_dead_col_;
            resule_line.grade= manage.param_fpn_dl_iic_.grade_dead_col_;
        }
        else if (cnt_fpn_level_2 >0)
        {
            resule_line.bin= manage.param_fpn_dl_iic_.bin_fpn_level_2_;
            resule_line.grade = manage.param_fpn_dl_iic_.grade_fpn_level_2_;
        }
        else if (cnt_fpn_level_1 >0)
        {
            resule_line.bin = manage.param_fpn_dl_iic_.bin_fpn_level_1_;
            resule_line.grade = manage.param_fpn_dl_iic_.grade_fpn_level_1_;
        }

        if (cnt_dead_row>0 && manage.param_fpn_dl_iic_.grade_dead_row_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_iic_.bin_dead_row_;
            resule_line.grade = manage.param_fpn_dl_iic_.grade_dead_row_;
        }

        if (cnt_dol>0 && manage.param_fpn_dl_iic_.grade_dol_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_iic_.bin_dol_;
            resule_line.grade = manage.param_fpn_dl_iic_.grade_dol_;
        }

        if (cnt_break_line>0 && manage.param_fpn_dl_iic_.grade_break_line_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_iic_.bin_break_line_;
            resule_line.grade = manage.param_fpn_dl_iic_.grade_break_line_;
        }

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % resule_line.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";


    }
    else
    {
        resule_line.g_msg = false;
    }

    return resule_line;
}





vector<Subitem_Result> DarkTest::OB_and_DarkShading(const ParamManage& manage)
{
    if(manage.ob_darkshading_version == "ob_darkShading_V1.0")
       return OB_and_DarkShading_V1_0( manage);
    else if(manage.ob_darkshading_version == "ob_darkShading_V1.1")    //卡中心
       return OB_and_DarkShading_V1_1( manage);
    else if(manage.ob_darkshading_version == "ob_darkShading_V1.2")    //darkshading max 分段（20c3需求）
       return OB_and_DarkShading_V1_2( manage);
    else if(manage.ob_darkshading_version == "ob_darkShading_V1.3")    //darkshading max 分段（20c3需求）
       return OB_and_DarkShading_V1_3( manage);


    Subitem_Result res("OB Test", false);
    res.g_msg = false;
    vector<Subitem_Result> resluts;
    resluts.push_back(res);
    return resluts;
}



vector<Subitem_Result> DarkTest::OB_and_DarkShading_V1_0(const ParamManage& manage)
{
    int w=manage.width_;
    int h=manage.height_;

    Subitem_Result ob_result("OB Test",true);
    Subitem_Result ob_ob_max_min_diff_result("OB Max Min Diff_result",true);
    Subitem_Result darkshading_result("DarkShading",true);

    if(manage.param_ob_darkshading_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------OB and DarkShading V1.0-----------------------------\n"));

        //---------------------Whole Region Light Response------------------

        double mean_r_roi = -1;
        double mean_gr_roi = -1;
        double mean_gb_roi = -1;
        double mean_b_roi = -1;

        TestItem::test_roi_channel_mean( w,h,
                                         manage.param_light_response_.roi_x_,
                                         manage.param_light_response_.roi_y_,
                                         manage.param_light_response_.roi_w_,
                                         manage.param_light_response_.roi_h_,
                                         mean_r_roi,mean_gr_roi,mean_gb_roi,mean_b_roi,
                                         Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                         Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);
        g_str_msg_all[site_]+=boost::str(boost::format("mean_r:%.2f, mean_gr:%.2f, mean_gb:%.2f, mean_b:%.2f\n")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);

        //-----------------------OB Test-----------------------------
        int roi_w_ = manage.param_ob_darkshading_.OB_region_r_;
        int roi_h_ = manage.param_ob_darkshading_.OB_region_r_;
        int skip_ = manage.param_ob_darkshading_.skip_;
        double ob_ref_ = manage.param_ob_darkshading_.ref;
        double ob_Golden_dif = manage.param_ob_darkshading_.Golden_dif;
        double mean_r_roi_dif = fabs(mean_r_roi-ob_ref_);
        double mean_gr_roi_dif = fabs(mean_gr_roi-ob_ref_);
        double mean_gb_roi_dif = fabs(mean_gb_roi-ob_ref_);
        double mean_b_roi_dif = fabs(mean_b_roi-ob_ref_);


        double min_diff_spec_class1 = manage.param_ob_darkshading_.min_diff_ref_class1_;
        double max_diff_spec_class1 = manage.param_ob_darkshading_.max_diff_ref_class1_;

        double min_diff_spec_class2 = manage.param_ob_darkshading_.min_diff_ref_class2_;
        double max_diff_spec_class2 = manage.param_ob_darkshading_.max_diff_ref_class2_;

        QVector<cv::Rect> OB_ROI;
        QVector<double> roi_r_ob;
        QVector<double> roi_gr_ob;
        QVector<double> roi_gb_ob;
        QVector<double> roi_b_ob;

        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,skip_,roi_w_,roi_h_));//leftup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,skip_,roi_w_,roi_h_));//rightup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect((w/2-1)/2-roi_w_/2,(h/2-1)/2-roi_h_/2,roi_w_,roi_h_));//center
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//leftdown
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//rightdown

        TestItem::test_roi_ob_bychannel(w, h, ob_ref_, OB_ROI,
                                        roi_r_ob, roi_gr_ob, roi_gb_ob, roi_b_ob,
                                        Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                        Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);


        double max_roi_r_ob = *std::max_element(roi_r_ob.begin(),roi_r_ob.end());
        double max_roi_gr_ob = *std::max_element(roi_gr_ob.begin(),roi_gr_ob.end());
        double max_roi_gb_ob = *std::max_element(roi_gb_ob.begin(),roi_gb_ob.end());
        double max_roi_b_ob = *std::max_element(roi_b_ob.begin(),roi_b_ob.end());
        double max_roi_rgb_ob = std::max(std::max(max_roi_r_ob,max_roi_gr_ob),std::max(max_roi_gb_ob,max_roi_b_ob));

        double min_roi_r_ob = *std::min_element(roi_r_ob.begin(),roi_r_ob.end());
        double min_roi_gr_ob = *std::min_element(roi_gr_ob.begin(),roi_gr_ob.end());
        double min_roi_gb_ob = *std::min_element(roi_gb_ob.begin(),roi_gb_ob.end());
        double min_roi_b_ob = *std::min_element(roi_b_ob.begin(),roi_b_ob.end());
        double min_roi_rgb_ob = std::min(std::min(min_roi_r_ob,min_roi_gr_ob),std::min(min_roi_gb_ob,min_roi_b_ob));

        double rgb_ob_max_min_diff = max_roi_rgb_ob - min_roi_rgb_ob;

        if (max_roi_rgb_ob > max_diff_spec_class2 ||  min_roi_rgb_ob < min_diff_spec_class2)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class2_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class2_;
        }
        else if ((max_roi_rgb_ob>=max_diff_spec_class1 && max_roi_rgb_ob <= max_diff_spec_class2)||
                 (min_roi_rgb_ob>=min_diff_spec_class2 && min_roi_rgb_ob <= min_diff_spec_class1) )
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class1_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class1_;
        }
        else if(mean_r_roi_dif < ob_Golden_dif || mean_gr_roi_dif < ob_Golden_dif || mean_gb_roi_dif < ob_Golden_dif || mean_b_roi_dif < ob_Golden_dif)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_Golden_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_Golden_;
        }

        if (rgb_ob_max_min_diff > manage.param_ob_darkshading_.ob_max_min_diff_)
        {
            ob_ob_max_min_diff_result.is_passed = false;
            ob_ob_max_min_diff_result.bin = manage.param_ob_darkshading_.bin_ob_max_min_diff_;
            ob_ob_max_min_diff_result.grade = manage.param_ob_darkshading_.grade_ob_max_min_diff_;
        }


        for(int i=0; i<OB_ROI.size(); i++)
        {
            if(i==0)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==1)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==2)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Center ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==3)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==4)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
        }

        g_str_msg_all[site_]+=boost::str(boost::format("ALL_ROIs_RGB_OB_MAX:%.2f, ALL_ROIs_RGB_OB_MIN:%.2f,\n")
                               % max_roi_rgb_ob % min_roi_rgb_ob);

        g_str_msg_all[site_]+=boost::str(boost::format("ALL_ROIs_RGB_OB_MAX_MIN_DIFF:%.2f,\n")
                               % rgb_ob_max_min_diff );

        for(int i=0; i<OB_ROI.size(); i++)
        {
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
        }

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|")
                               % max_roi_rgb_ob % min_roi_rgb_ob);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|")
                               % rgb_ob_max_min_diff);


        //-----------------------DarkShading--------------------------
        double max_mean_r_DarkShading=0;
        double max_mean_gr_DarkShading=0;
        double max_mean_gb_DarkShading=0;
        double max_mean_b_DarkShading=0;

        double min_mean_r_DarkShading=0;
        double min_mean_gr_DarkShading=0;
        double min_mean_gb_DarkShading=0;
        double min_mean_b_DarkShading=0;

        int max_r_DarkShading_pos=0;
        int max_gr_DarkShading_pos=0;
        int max_gb_DarkShading_pos=0;
        int max_b_DarkShading_pos=0;

        int min_r_DarkShading_pos=0;
        int min_gr_DarkShading_pos=0;
        int min_gb_DarkShading_pos=0;
        int min_b_DarkShading_pos=0;


        TestItem::test_channel_darkshading(w,h,manage.param_ob_darkshading_.x_cnt_,manage.param_ob_darkshading_.y_cnt_,
                                           max_mean_r_DarkShading,max_mean_gr_DarkShading,max_mean_gb_DarkShading,max_mean_b_DarkShading,
                                           min_mean_r_DarkShading,min_mean_gr_DarkShading,min_mean_gb_DarkShading,min_mean_b_DarkShading,
                                           max_r_DarkShading_pos,max_gr_DarkShading_pos,max_gb_DarkShading_pos,max_b_DarkShading_pos,
                                           min_r_DarkShading_pos,min_gr_DarkShading_pos,min_gb_DarkShading_pos,min_b_DarkShading_pos,
                                           Algorithm_all[site_].g_buf_r_tmp_,Algorithm_all[site_].g_buf_gr_tmp_,Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_, g_str_msg_all[site_]);

        logMsg(std::string("After darkshading test"));


        double max_rgb_darkshading = std::max(std::max(max_mean_r_DarkShading,max_mean_gr_DarkShading),std::max(max_mean_gb_DarkShading,max_mean_b_DarkShading));
        double min_rgb_darkshading = std::min(std::min(min_mean_r_DarkShading,min_mean_gr_DarkShading),std::min(min_mean_gb_DarkShading,min_mean_b_DarkShading));

        double max_min_diff_darkshading = max_rgb_darkshading - min_rgb_darkshading;


        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%d|%d|%d|%d|%d|%d|%d|%d|%.2f|")
                               % max_mean_r_DarkShading % max_mean_gr_DarkShading % max_mean_gb_DarkShading % max_mean_b_DarkShading
                               % min_mean_r_DarkShading % min_mean_gr_DarkShading % min_mean_gb_DarkShading % min_mean_b_DarkShading
                               % max_r_DarkShading_pos % max_gr_DarkShading_pos % max_gb_DarkShading_pos % max_b_DarkShading_pos
                               % min_r_DarkShading_pos % min_gr_DarkShading_pos % min_gb_DarkShading_pos % min_b_DarkShading_pos
                               % max_min_diff_darkshading);



        g_str_msg_all[site_]+=boost::str(boost::format("Darkshading:\n"
                                             " max_mean_r_DarkShading:%.2f ,max_mean_gr_DarkShading:%.2f , max_mean_gb_DarkShading:%.2f , max_mean_b_DarkShading:%.2f\n"
                                             " min_mean_r_DarkShading:%.2f ,min_mean_gr_DarkShading:%.2f , min_mean_gb_DarkShading:%.2f , min_mean_b_DarkShading:%.2f\n"
                                             " max_r_DarkShading_pos:%d ,max_gr_DarkShading_pos:%d , max_gb_DarkShading_pos:%d , max_b_DarkShading_pos:%d\n"
                                             " min_r_DarkShading_pos:%d ,min_gr_DarkShading_pos:%d , min_gb_DarkShading_pos:%d , min_b_DarkShading_pos:%d\n"
                                             " max_min_diff_Darkshading:%.2f\n")
                               % max_mean_r_DarkShading % max_mean_gr_DarkShading % max_mean_gb_DarkShading % max_mean_b_DarkShading
                               % min_mean_r_DarkShading % min_mean_gr_DarkShading % min_mean_gb_DarkShading % min_mean_b_DarkShading
                               % max_r_DarkShading_pos % max_gr_DarkShading_pos % max_gb_DarkShading_pos % max_b_DarkShading_pos
                               % min_r_DarkShading_pos % min_gr_DarkShading_pos % min_gb_DarkShading_pos % min_b_DarkShading_pos
                               % max_min_diff_darkshading);

        if(max_mean_r_DarkShading>manage.param_ob_darkshading_.r_DarkShading_max_||
                max_mean_gr_DarkShading>manage.param_ob_darkshading_.gr_DarkShading_max_||
                max_mean_gb_DarkShading>manage.param_ob_darkshading_.gb_DarkShading_max_||
                max_mean_b_DarkShading>manage.param_ob_darkshading_.b_DarkShading_max_||
                min_mean_r_DarkShading<manage.param_ob_darkshading_.r_DarkShading_min_||
                min_mean_gr_DarkShading<manage.param_ob_darkshading_.gr_DarkShading_min_||
                min_mean_gb_DarkShading<manage.param_ob_darkshading_.gb_DarkShading_min_||
                min_mean_b_DarkShading<manage.param_ob_darkshading_.b_DarkShading_min_)
        {
            darkshading_result.is_passed=false;
            darkshading_result.bin = manage.param_ob_darkshading_.bin_DarkShading_;
            darkshading_result.grade = manage.param_ob_darkshading_.grade_DarkShading_;
        }



        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                         "max_roi_rgb_ob=%.4f (SPEC MAX_Class2: %.4f) \n"
                                                         "min_roi_rgb_ob=%.4f (SPEC MIN_Class2: %.4f) \n"
                                                         "max_roi_rgb_ob=%.4f (SPEC MAX_Class1: %.4f ~ MAX_Class2: %.4f) \n"
                                                         "min_roi_rgb_ob=%.4f (SPEC MIN_Class1: %.4f ~ MIN_Class2: %.4f) \n"
                                                         "rgb_ob_max_min_diff=%.4f (MAX Diff: %.4f) \n"
                                                         )
                                                   % max_roi_rgb_ob % max_diff_spec_class2
                                                   % min_roi_rgb_ob % min_diff_spec_class2
                                                   % max_roi_rgb_ob % max_diff_spec_class1 % max_diff_spec_class2
                                                   % min_roi_rgb_ob % min_diff_spec_class1 % min_diff_spec_class2
                                                   % rgb_ob_max_min_diff % manage.param_ob_darkshading_.ob_max_min_diff_
                                                   );


        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                        "max_mean_r_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_gr_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_gb_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_b_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "min_mean_r_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_gr_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_gb_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_b_DarkShading=%.4f (MIN: %.4f) \n"
                                                         )
                                            % max_mean_r_DarkShading
                                            % manage.param_ob_darkshading_.r_DarkShading_max_
                                            % max_mean_gr_DarkShading
                                            % manage.param_ob_darkshading_.gr_DarkShading_max_
                                            % max_mean_gb_DarkShading
                                            % manage.param_ob_darkshading_.gb_DarkShading_max_
                                            % max_mean_b_DarkShading
                                            % manage.param_ob_darkshading_.b_DarkShading_max_
                                            % min_mean_r_DarkShading
                                            % manage.param_ob_darkshading_.r_DarkShading_min_
                                            % min_mean_gr_DarkShading
                                            % manage.param_ob_darkshading_.gr_DarkShading_min_
                                            % min_mean_gb_DarkShading
                                            % manage.param_ob_darkshading_.gb_DarkShading_min_
                                            % min_mean_b_DarkShading
                                            % manage.param_ob_darkshading_.b_DarkShading_min_
                                           );

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % darkshading_result.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";


    }
    else
    {
        ob_result.g_msg = false;
        ob_ob_max_min_diff_result.g_msg = false;
        darkshading_result.g_msg = false;
    }
    vector<Subitem_Result> resluts;
    resluts.push_back(ob_result);
    resluts.push_back(ob_ob_max_min_diff_result);
    resluts.push_back(darkshading_result);

    return resluts;
}

vector<Subitem_Result> DarkTest::OB_and_DarkShading_V1_1(const ParamManage& manage)
{
    int w=manage.width_;
    int h=manage.height_;

    Subitem_Result ob_result("OB Test_V1.1",true);
    Subitem_Result ob_ob_max_min_diff_result("OB Max Min Diff_result_V1.1",true);
    Subitem_Result darkshading_result("DarkShading_V1.1",true);

    if(manage.param_ob_darkshading_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------OB and DarkShading V1.1-----------------------------\n"));

        //---------------------Whole Region Light Response------------------

        double mean_r_roi = -1;
        double mean_gr_roi = -1;
        double mean_gb_roi = -1;
        double mean_b_roi = -1;

        TestItem::test_roi_channel_mean( w,h,
                                         manage.param_light_response_.roi_x_,
                                         manage.param_light_response_.roi_y_,
                                         manage.param_light_response_.roi_w_,
                                         manage.param_light_response_.roi_h_,
                                         mean_r_roi,mean_gr_roi,mean_gb_roi,mean_b_roi,
                                         Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                         Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);
        g_str_msg_all[site_]+=boost::str(boost::format("mean_r:%.2f, mean_gr:%.2f, mean_gb:%.2f, mean_b:%.2f\n")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);

        //-----------------------OB Test-----------------------------
        int roi_w_ = manage.param_ob_darkshading_.OB_region_r_;
        int roi_h_ = manage.param_ob_darkshading_.OB_region_r_;
        int skip_ = manage.param_ob_darkshading_.skip_;
        double ob_ref_ = manage.param_ob_darkshading_.ref;
        double ob_Golden_dif = manage.param_ob_darkshading_.Golden_dif;
        double mean_r_roi_dif = fabs(mean_r_roi-ob_ref_);
        double mean_gr_roi_dif = fabs(mean_gr_roi-ob_ref_);
        double mean_gb_roi_dif = fabs(mean_gb_roi-ob_ref_);
        double mean_b_roi_dif = fabs(mean_b_roi-ob_ref_);


        double min_diff_spec_class1 = manage.param_ob_darkshading_.min_diff_ref_class1_;
        double max_diff_spec_class1 = manage.param_ob_darkshading_.max_diff_ref_class1_;

        double min_diff_spec_class2 = manage.param_ob_darkshading_.min_diff_ref_class2_;
        double max_diff_spec_class2 = manage.param_ob_darkshading_.max_diff_ref_class2_;

        QVector<cv::Rect> OB_ROI;
        QVector<double> roi_r_ob;
        QVector<double> roi_gr_ob;
        QVector<double> roi_gb_ob;
        QVector<double> roi_b_ob;

        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,skip_,roi_w_,roi_h_));//leftup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,skip_,roi_w_,roi_h_));//rightup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect((w/2-1)/2-roi_w_/2,(h/2-1)/2-roi_h_/2,roi_w_,roi_h_));//center
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//leftdown
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//rightdown

        TestItem::test_roi_ob_bychannel(w, h, ob_ref_, OB_ROI,
                                        roi_r_ob, roi_gr_ob, roi_gb_ob, roi_b_ob,
                                        Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                        Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);



        /*1、判断Center的最大值和最小值*/
        double center_roi_r_ob = roi_r_ob.at(2);
        double center_roi_gr_ob = roi_gr_ob.at(2);
        double center_roi_gb_ob = roi_gb_ob.at(2);
        double center_roi_b_ob = roi_b_ob.at(2);

        double max_center_roi_rgb_ob = std::max(std::max(center_roi_r_ob,center_roi_gr_ob),std::max(center_roi_gb_ob,center_roi_b_ob));
        double min_center_roi_rgb_ob = std::min(std::min(center_roi_r_ob,center_roi_gr_ob),std::min(center_roi_gb_ob,center_roi_b_ob));


        if (max_center_roi_rgb_ob > manage.param_ob_darkshading_.max_diff_center_Class2_ ||  min_center_roi_rgb_ob < manage.param_ob_darkshading_.min_diff_center_Class1_)   //中心不满足要求
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class2_;       //2324
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class2_;
        }

        /*2、判断四角的最大值和最小值*/
        roi_r_ob.remove(2);
        roi_gr_ob.remove(2);
        roi_gb_ob.remove(2);
        roi_b_ob.remove(2);

        QVector<double> roi_r_ob_no_center;
        QVector<double> roi_gr_ob_no_center;
        QVector<double> roi_gb_ob_no_center;
        QVector<double> roi_b_ob_no_center;

        roi_r_ob_no_center = roi_r_ob;
        roi_gr_ob_no_center = roi_gr_ob;
        roi_gb_ob_no_center = roi_gb_ob;
        roi_b_ob_no_center = roi_b_ob;


        double max_roi_r_ob = *std::max_element(roi_r_ob_no_center.begin(),roi_r_ob_no_center.end());
        double max_roi_gr_ob = *std::max_element(roi_gr_ob_no_center.begin(),roi_gr_ob_no_center.end());
        double max_roi_gb_ob = *std::max_element(roi_gb_ob_no_center.begin(),roi_gb_ob_no_center.end());
        double max_roi_b_ob = *std::max_element(roi_b_ob_no_center.begin(),roi_b_ob_no_center.end());
        double max_roi_rgb_ob = std::max(std::max(max_roi_r_ob,max_roi_gr_ob),std::max(max_roi_gb_ob,max_roi_b_ob));

        double min_roi_r_ob = *std::min_element(roi_r_ob_no_center.begin(),roi_r_ob_no_center.end());
        double min_roi_gr_ob = *std::min_element(roi_gr_ob_no_center.begin(),roi_gr_ob_no_center.end());
        double min_roi_gb_ob = *std::min_element(roi_gb_ob_no_center.begin(),roi_gb_ob_no_center.end());
        double min_roi_b_ob = *std::min_element(roi_b_ob_no_center.begin(),roi_b_ob_no_center.end());
        double min_roi_rgb_ob = std::min(std::min(min_roi_r_ob,min_roi_gr_ob),std::min(min_roi_gb_ob,min_roi_b_ob));

        double rgb_ob_max_min_diff = max_roi_rgb_ob - min_roi_rgb_ob;


        if (max_roi_rgb_ob > max_diff_spec_class2 ||  min_roi_rgb_ob < min_diff_spec_class2)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class2_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class2_;
        }
        else if ((max_roi_rgb_ob>=max_diff_spec_class1 && max_roi_rgb_ob <= max_diff_spec_class2)||
                 (min_roi_rgb_ob>=min_diff_spec_class2 && min_roi_rgb_ob <= min_diff_spec_class1) )
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class1_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class1_;
        }
        else if(mean_r_roi_dif < ob_Golden_dif || mean_gr_roi_dif < ob_Golden_dif || mean_gb_roi_dif < ob_Golden_dif || mean_b_roi_dif < ob_Golden_dif)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_Golden_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_Golden_;
        }

        if (rgb_ob_max_min_diff > manage.param_ob_darkshading_.ob_max_min_diff_)
        {
            ob_ob_max_min_diff_result.is_passed = false;
            ob_ob_max_min_diff_result.bin = manage.param_ob_darkshading_.bin_ob_max_min_diff_;
            ob_ob_max_min_diff_result.grade = manage.param_ob_darkshading_.grade_ob_max_min_diff_;
        }


        for(int i=0; i<OB_ROI.size(); i++)
        {
            if(i==0)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==1)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==2)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Center ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==3)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==4)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
        }

        g_str_msg_all[site_]+=boost::str(boost::format("ALL_ROIs_RGB_OB_MAX:%.2f, ALL_ROIs_RGB_OB_MIN:%.2f,\n")
                               % max_roi_rgb_ob % min_roi_rgb_ob);

        g_str_msg_all[site_]+=boost::str(boost::format("ALL_ROIs_RGB_OB_MAX_MIN_DIFF:%.2f,\n")
                               % rgb_ob_max_min_diff );

        for(int i=0; i<OB_ROI.size(); i++)
        {
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
        }

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|")
                               % max_roi_rgb_ob % min_roi_rgb_ob);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|")
                               % rgb_ob_max_min_diff);


        //-----------------------DarkShading--------------------------
        double max_mean_r_DarkShading=0;
        double max_mean_gr_DarkShading=0;
        double max_mean_gb_DarkShading=0;
        double max_mean_b_DarkShading=0;

        double min_mean_r_DarkShading=0;
        double min_mean_gr_DarkShading=0;
        double min_mean_gb_DarkShading=0;
        double min_mean_b_DarkShading=0;

        int max_r_DarkShading_pos=0;
        int max_gr_DarkShading_pos=0;
        int max_gb_DarkShading_pos=0;
        int max_b_DarkShading_pos=0;

        int min_r_DarkShading_pos=0;
        int min_gr_DarkShading_pos=0;
        int min_gb_DarkShading_pos=0;
        int min_b_DarkShading_pos=0;


        TestItem::test_channel_darkshading(w,h,manage.param_ob_darkshading_.x_cnt_,manage.param_ob_darkshading_.y_cnt_,
                                           max_mean_r_DarkShading,max_mean_gr_DarkShading,max_mean_gb_DarkShading,max_mean_b_DarkShading,
                                           min_mean_r_DarkShading,min_mean_gr_DarkShading,min_mean_gb_DarkShading,min_mean_b_DarkShading,
                                           max_r_DarkShading_pos,max_gr_DarkShading_pos,max_gb_DarkShading_pos,max_b_DarkShading_pos,
                                           min_r_DarkShading_pos,min_gr_DarkShading_pos,min_gb_DarkShading_pos,min_b_DarkShading_pos,
                                           Algorithm_all[site_].g_buf_r_tmp_,Algorithm_all[site_].g_buf_gr_tmp_,Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_, g_str_msg_all[site_]);

        logMsg(std::string("After darkshading test"));


        double max_rgb_darkshading = std::max(std::max(max_mean_r_DarkShading,max_mean_gr_DarkShading),std::max(max_mean_gb_DarkShading,max_mean_b_DarkShading));
        double min_rgb_darkshading = std::min(std::min(min_mean_r_DarkShading,min_mean_gr_DarkShading),std::min(min_mean_gb_DarkShading,min_mean_b_DarkShading));

        double max_min_diff_darkshading = max_rgb_darkshading - min_rgb_darkshading;


        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%d|%d|%d|%d|%d|%d|%d|%d|%.2f|")
                               % max_mean_r_DarkShading % max_mean_gr_DarkShading % max_mean_gb_DarkShading % max_mean_b_DarkShading
                               % min_mean_r_DarkShading % min_mean_gr_DarkShading % min_mean_gb_DarkShading % min_mean_b_DarkShading
                               % max_r_DarkShading_pos % max_gr_DarkShading_pos % max_gb_DarkShading_pos % max_b_DarkShading_pos
                               % min_r_DarkShading_pos % min_gr_DarkShading_pos % min_gb_DarkShading_pos % min_b_DarkShading_pos
                               % max_min_diff_darkshading);



        g_str_msg_all[site_]+=boost::str(boost::format("Darkshading:\n"
                                             " max_mean_r_DarkShading:%.2f ,max_mean_gr_DarkShading:%.2f , max_mean_gb_DarkShading:%.2f , max_mean_b_DarkShading:%.2f\n"
                                             " min_mean_r_DarkShading:%.2f ,min_mean_gr_DarkShading:%.2f , min_mean_gb_DarkShading:%.2f , min_mean_b_DarkShading:%.2f\n"
                                             " max_r_DarkShading_pos:%d ,max_gr_DarkShading_pos:%d , max_gb_DarkShading_pos:%d , max_b_DarkShading_pos:%d\n"
                                             " min_r_DarkShading_pos:%d ,min_gr_DarkShading_pos:%d , min_gb_DarkShading_pos:%d , min_b_DarkShading_pos:%d\n"
                                             " max_min_diff_Darkshading:%.2f\n")
                               % max_mean_r_DarkShading % max_mean_gr_DarkShading % max_mean_gb_DarkShading % max_mean_b_DarkShading
                               % min_mean_r_DarkShading % min_mean_gr_DarkShading % min_mean_gb_DarkShading % min_mean_b_DarkShading
                               % max_r_DarkShading_pos % max_gr_DarkShading_pos % max_gb_DarkShading_pos % max_b_DarkShading_pos
                               % min_r_DarkShading_pos % min_gr_DarkShading_pos % min_gb_DarkShading_pos % min_b_DarkShading_pos
                               % max_min_diff_darkshading);

        if(max_mean_r_DarkShading>manage.param_ob_darkshading_.r_DarkShading_max_||
                max_mean_gr_DarkShading>manage.param_ob_darkshading_.gr_DarkShading_max_||
                max_mean_gb_DarkShading>manage.param_ob_darkshading_.gb_DarkShading_max_||
                max_mean_b_DarkShading>manage.param_ob_darkshading_.b_DarkShading_max_||
                min_mean_r_DarkShading<manage.param_ob_darkshading_.r_DarkShading_min_||
                min_mean_gr_DarkShading<manage.param_ob_darkshading_.gr_DarkShading_min_||
                min_mean_gb_DarkShading<manage.param_ob_darkshading_.gb_DarkShading_min_||
                min_mean_b_DarkShading<manage.param_ob_darkshading_.b_DarkShading_min_)
        {
            darkshading_result.is_passed=false;
            darkshading_result.bin = manage.param_ob_darkshading_.bin_DarkShading_;
            darkshading_result.grade = manage.param_ob_darkshading_.grade_DarkShading_;
        }



        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                         "max_roi_rgb_ob=%.4f (SPEC MAX_Class2: %.4f) \n"
                                                         "min_roi_rgb_ob=%.4f (SPEC MIN_Class2: %.4f) \n"
                                                         "max_roi_rgb_ob=%.4f (SPEC MAX_Class1: %.4f ~ MAX_Class2: %.4f) \n"
                                                         "min_roi_rgb_ob=%.4f (SPEC MIN_Class1: %.4f ~ MIN_Class2: %.4f) \n"
                                                         "rgb_ob_max_min_diff=%.4f (MAX Diff: %.4f) \n"
                                                         )
                                                   % max_roi_rgb_ob % max_diff_spec_class2
                                                   % min_roi_rgb_ob % min_diff_spec_class2
                                                   % max_roi_rgb_ob % max_diff_spec_class1 % max_diff_spec_class2
                                                   % min_roi_rgb_ob % min_diff_spec_class1 % min_diff_spec_class2
                                                   % rgb_ob_max_min_diff % manage.param_ob_darkshading_.ob_max_min_diff_
                                                   );


        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                        "max_mean_r_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_gr_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_gb_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_b_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "min_mean_r_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_gr_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_gb_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_b_DarkShading=%.4f (MIN: %.4f) \n"
                                                         )
                                            % max_mean_r_DarkShading
                                            % manage.param_ob_darkshading_.r_DarkShading_max_
                                            % max_mean_gr_DarkShading
                                            % manage.param_ob_darkshading_.gr_DarkShading_max_
                                            % max_mean_gb_DarkShading
                                            % manage.param_ob_darkshading_.gb_DarkShading_max_
                                            % max_mean_b_DarkShading
                                            % manage.param_ob_darkshading_.b_DarkShading_max_
                                            % min_mean_r_DarkShading
                                            % manage.param_ob_darkshading_.r_DarkShading_min_
                                            % min_mean_gr_DarkShading
                                            % manage.param_ob_darkshading_.gr_DarkShading_min_
                                            % min_mean_gb_DarkShading
                                            % manage.param_ob_darkshading_.gb_DarkShading_min_
                                            % min_mean_b_DarkShading
                                            % manage.param_ob_darkshading_.b_DarkShading_min_
                                           );

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % darkshading_result.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";


    }
    else
    {
        ob_result.g_msg = false;
        ob_ob_max_min_diff_result.g_msg = false;
        darkshading_result.g_msg = false;
    }
    vector<Subitem_Result> resluts;
    resluts.push_back(ob_result);
    resluts.push_back(ob_ob_max_min_diff_result);
    resluts.push_back(darkshading_result);

    return resluts;
}
vector<Subitem_Result> DarkTest::OB_and_DarkShading_V1_2(const ParamManage& manage)
{

     //黑屏OB darkshading V1.0算法：1.0基础上增加shading的max分段（GC20C3需求）
    int w=manage.width_;
    int h=manage.height_;

    Subitem_Result ob_result("OB Test_V1.2",true);
    Subitem_Result ob_ob_max_min_diff_result("OB Max Min Diff_result_V1.2",true);
    Subitem_Result darkshading_result("DarkShading_V1.2",true);

    if(manage.param_ob_darkshading_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------OB and DarkShading V1.2-----------------------------\n"));

        //---------------------Whole Region Light Response------------------

        double mean_r_roi = -1;
        double mean_gr_roi = -1;
        double mean_gb_roi = -1;
        double mean_b_roi = -1;

        TestItem::test_roi_channel_mean( w,h,
                                         manage.param_light_response_.roi_x_,
                                         manage.param_light_response_.roi_y_,
                                         manage.param_light_response_.roi_w_,
                                         manage.param_light_response_.roi_h_,
                                         mean_r_roi,mean_gr_roi,mean_gb_roi,mean_b_roi,
                                         Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                         Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);
        g_str_msg_all[site_]+=boost::str(boost::format("mean_r:%.2f, mean_gr:%.2f, mean_gb:%.2f, mean_b:%.2f\n")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);

        //-----------------------OB Test-----------------------------
        int roi_w_ = manage.param_ob_darkshading_.OB_region_r_;
        int roi_h_ = manage.param_ob_darkshading_.OB_region_r_;
        int skip_ = manage.param_ob_darkshading_.skip_;
        double ob_ref_ = manage.param_ob_darkshading_.ref;
        double ob_Golden_dif = manage.param_ob_darkshading_.Golden_dif;
        double mean_r_roi_dif = fabs(mean_r_roi-ob_ref_);
        double mean_gr_roi_dif = fabs(mean_gr_roi-ob_ref_);
        double mean_gb_roi_dif = fabs(mean_gb_roi-ob_ref_);
        double mean_b_roi_dif = fabs(mean_b_roi-ob_ref_);


        double min_diff_spec_class1 = manage.param_ob_darkshading_.min_diff_ref_class1_;
        double max_diff_spec_class1 = manage.param_ob_darkshading_.max_diff_ref_class1_;

        double min_diff_spec_class2 = manage.param_ob_darkshading_.min_diff_ref_class2_;
        double max_diff_spec_class2 = manage.param_ob_darkshading_.max_diff_ref_class2_;

        QVector<cv::Rect> OB_ROI;
        QVector<double> roi_r_ob;
        QVector<double> roi_gr_ob;
        QVector<double> roi_gb_ob;
        QVector<double> roi_b_ob;

        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,skip_,roi_w_,roi_h_));//leftup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,skip_,roi_w_,roi_h_));//rightup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect((w/2-1)/2-roi_w_/2,(h/2-1)/2-roi_h_/2,roi_w_,roi_h_));//center
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//leftdown
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//rightdown

        TestItem::test_roi_ob_bychannel(w, h, ob_ref_, OB_ROI,
                                        roi_r_ob, roi_gr_ob, roi_gb_ob, roi_b_ob,
                                        Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                        Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);


        double max_roi_r_ob = *std::max_element(roi_r_ob.begin(),roi_r_ob.end());
        double max_roi_gr_ob = *std::max_element(roi_gr_ob.begin(),roi_gr_ob.end());
        double max_roi_gb_ob = *std::max_element(roi_gb_ob.begin(),roi_gb_ob.end());
        double max_roi_b_ob = *std::max_element(roi_b_ob.begin(),roi_b_ob.end());
        double max_roi_rgb_ob = std::max(std::max(max_roi_r_ob,max_roi_gr_ob),std::max(max_roi_gb_ob,max_roi_b_ob));

        double min_roi_r_ob = *std::min_element(roi_r_ob.begin(),roi_r_ob.end());
        double min_roi_gr_ob = *std::min_element(roi_gr_ob.begin(),roi_gr_ob.end());
        double min_roi_gb_ob = *std::min_element(roi_gb_ob.begin(),roi_gb_ob.end());
        double min_roi_b_ob = *std::min_element(roi_b_ob.begin(),roi_b_ob.end());
        double min_roi_rgb_ob = std::min(std::min(min_roi_r_ob,min_roi_gr_ob),std::min(min_roi_gb_ob,min_roi_b_ob));

        double rgb_ob_max_min_diff = max_roi_rgb_ob - min_roi_rgb_ob;

        if (max_roi_rgb_ob > max_diff_spec_class2 ||  min_roi_rgb_ob < min_diff_spec_class2)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class2_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class2_;
        }
        else if ((max_roi_rgb_ob>=max_diff_spec_class1 && max_roi_rgb_ob <= max_diff_spec_class2)||
                 (min_roi_rgb_ob>=min_diff_spec_class2 && min_roi_rgb_ob <= min_diff_spec_class1) )
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class1_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class1_;
        }
        else if(mean_r_roi_dif < ob_Golden_dif || mean_gr_roi_dif < ob_Golden_dif || mean_gb_roi_dif < ob_Golden_dif || mean_b_roi_dif < ob_Golden_dif)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_Golden_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_Golden_;
        }

        if (rgb_ob_max_min_diff > manage.param_ob_darkshading_.ob_max_min_diff_)
        {
            ob_ob_max_min_diff_result.is_passed = false;
            ob_ob_max_min_diff_result.bin = manage.param_ob_darkshading_.bin_ob_max_min_diff_;
            ob_ob_max_min_diff_result.grade = manage.param_ob_darkshading_.grade_ob_max_min_diff_;
        }


        for(int i=0; i<OB_ROI.size(); i++)
        {
            if(i==0)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==1)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==2)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Center ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==3)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==4)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
        }

        g_str_msg_all[site_]+=boost::str(boost::format("ALL_ROIs_RGB_OB_MAX:%.2f, ALL_ROIs_RGB_OB_MIN:%.2f,\n")
                               % max_roi_rgb_ob % min_roi_rgb_ob);

        g_str_msg_all[site_]+=boost::str(boost::format("ALL_ROIs_RGB_OB_MAX_MIN_DIFF:%.2f,\n")
                               % rgb_ob_max_min_diff );

        for(int i=0; i<OB_ROI.size(); i++)
        {
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
        }

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|")
                               % max_roi_rgb_ob % min_roi_rgb_ob);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|")
                               % rgb_ob_max_min_diff);


        //-----------------------DarkShading--------------------------
        double max_mean_r_DarkShading=0;
        double max_mean_gr_DarkShading=0;
        double max_mean_gb_DarkShading=0;
        double max_mean_b_DarkShading=0;

        double min_mean_r_DarkShading=0;
        double min_mean_gr_DarkShading=0;
        double min_mean_gb_DarkShading=0;
        double min_mean_b_DarkShading=0;

        int max_r_DarkShading_pos=0;
        int max_gr_DarkShading_pos=0;
        int max_gb_DarkShading_pos=0;
        int max_b_DarkShading_pos=0;

        int min_r_DarkShading_pos=0;
        int min_gr_DarkShading_pos=0;
        int min_gb_DarkShading_pos=0;
        int min_b_DarkShading_pos=0;


        TestItem::test_channel_darkshading(w,h,manage.param_ob_darkshading_.x_cnt_,manage.param_ob_darkshading_.y_cnt_,
                                           max_mean_r_DarkShading,max_mean_gr_DarkShading,max_mean_gb_DarkShading,max_mean_b_DarkShading,
                                           min_mean_r_DarkShading,min_mean_gr_DarkShading,min_mean_gb_DarkShading,min_mean_b_DarkShading,
                                           max_r_DarkShading_pos,max_gr_DarkShading_pos,max_gb_DarkShading_pos,max_b_DarkShading_pos,
                                           min_r_DarkShading_pos,min_gr_DarkShading_pos,min_gb_DarkShading_pos,min_b_DarkShading_pos,
                                           Algorithm_all[site_].g_buf_r_tmp_,Algorithm_all[site_].g_buf_gr_tmp_,Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_, g_str_msg_all[site_]);

        logMsg(std::string("After darkshading test"));





        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%d|%d|%d|%d|%d|%d|%d|%d|")
                               % max_mean_r_DarkShading % max_mean_gr_DarkShading % max_mean_gb_DarkShading % max_mean_b_DarkShading
                               % min_mean_r_DarkShading % min_mean_gr_DarkShading % min_mean_gb_DarkShading % min_mean_b_DarkShading
                               % max_r_DarkShading_pos % max_gr_DarkShading_pos % max_gb_DarkShading_pos % max_b_DarkShading_pos
                               % min_r_DarkShading_pos % min_gr_DarkShading_pos % min_gb_DarkShading_pos % min_b_DarkShading_pos);



        g_str_msg_all[site_]+=boost::str(boost::format("Darkshading:\n"
                                             " max_mean_r_DarkShading:%.2f ,max_mean_gr_DarkShading:%.2f , max_mean_gb_DarkShading:%.2f , max_mean_b_DarkShading:%.2f\n"
                                             " min_mean_r_DarkShading:%.2f ,min_mean_gr_DarkShading:%.2f , min_mean_gb_DarkShading:%.2f , min_mean_b_DarkShading:%.2f\n"
                                             " max_r_DarkShading_pos:%d ,max_gr_DarkShading_pos:%d , max_gb_DarkShading_pos:%d , max_b_DarkShading_pos:%d\n"
                                             " min_r_DarkShading_pos:%d ,min_gr_DarkShading_pos:%d , min_gb_DarkShading_pos:%d , min_b_DarkShading_pos:%d\n")
                               % max_mean_r_DarkShading % max_mean_gr_DarkShading % max_mean_gb_DarkShading % max_mean_b_DarkShading
                               % min_mean_r_DarkShading % min_mean_gr_DarkShading % min_mean_gb_DarkShading % min_mean_b_DarkShading
                               % max_r_DarkShading_pos % max_gr_DarkShading_pos % max_gb_DarkShading_pos % max_b_DarkShading_pos
                               % min_r_DarkShading_pos % min_gr_DarkShading_pos % min_gb_DarkShading_pos % min_b_DarkShading_pos
                               );

        if(max_mean_r_DarkShading>manage.param_ob_darkshading_.r_DarkShading_max_||
                max_mean_gr_DarkShading>manage.param_ob_darkshading_.gr_DarkShading_max_||
                max_mean_gb_DarkShading>manage.param_ob_darkshading_.gb_DarkShading_max_||
                max_mean_b_DarkShading>manage.param_ob_darkshading_.b_DarkShading_max_||
                min_mean_r_DarkShading<manage.param_ob_darkshading_.r_DarkShading_min_||
                min_mean_gr_DarkShading<manage.param_ob_darkshading_.gr_DarkShading_min_||
                min_mean_gb_DarkShading<manage.param_ob_darkshading_.gb_DarkShading_min_||
                min_mean_b_DarkShading<manage.param_ob_darkshading_.b_DarkShading_min_)
        {
            darkshading_result.is_passed=false;
            darkshading_result.bin = manage.param_ob_darkshading_.bin_DarkShading_;
            darkshading_result.grade = manage.param_ob_darkshading_.grade_DarkShading_;
        }
        else if(max_mean_r_DarkShading>manage.param_ob_darkshading_.r_DarkShading_mid_ ||
                max_mean_gr_DarkShading>manage.param_ob_darkshading_.gr_DarkShading_mid_||
                max_mean_gb_DarkShading>manage.param_ob_darkshading_.gb_DarkShading_mid_||
                max_mean_b_DarkShading>manage.param_ob_darkshading_.b_DarkShading_mid_)
        {
            darkshading_result.is_passed=false;
            darkshading_result.bin = manage.param_ob_darkshading_.bin_DarkShading_MA;
            darkshading_result.grade = manage.param_ob_darkshading_.grade_DarkShading_MA;
        }



        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                         "max_roi_rgb_ob=%.4f (SPEC MAX_Class2: %.4f) \n"
                                                         "min_roi_rgb_ob=%.4f (SPEC MIN_Class2: %.4f) \n"
                                                         "max_roi_rgb_ob=%.4f (SPEC MAX_Class1: %.4f ~ MAX_Class2: %.4f) \n"
                                                         "min_roi_rgb_ob=%.4f (SPEC MIN_Class1: %.4f ~ MIN_Class2: %.4f) \n"
                                                         "rgb_ob_max_min_diff=%.4f (MAX Diff: %.4f) \n"
                                                         )
                                                   % max_roi_rgb_ob % max_diff_spec_class2
                                                   % min_roi_rgb_ob % min_diff_spec_class2
                                                   % max_roi_rgb_ob % max_diff_spec_class1 % max_diff_spec_class2
                                                   % min_roi_rgb_ob % min_diff_spec_class1 % min_diff_spec_class2
                                                   % rgb_ob_max_min_diff % manage.param_ob_darkshading_.ob_max_min_diff_
                                                   );


        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                        "max_mean_r_DarkShading=%.4f (MAX: %.4f MID: %.4f) \n"
                                                        "max_mean_gr_DarkShading=%.4f (MAX: %.4f MID: %.4f) \n"
                                                        "max_mean_gb_DarkShading=%.4f (MAX: %.4f MID: %.4f) \n"
                                                        "max_mean_b_DarkShading=%.4f (MAX: %.4f MID: %.4f) \n"
                                                        "min_mean_r_DarkShading=%.4f (MIN: %.4f ) \n"
                                                        "min_mean_gr_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_gb_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_b_DarkShading=%.4f (MIN: %.4f) \n"
                                                         )
                                            % max_mean_r_DarkShading
                                            % manage.param_ob_darkshading_.r_DarkShading_max_
                                            % manage.param_ob_darkshading_.r_DarkShading_mid_
                                            % max_mean_gr_DarkShading
                                            % manage.param_ob_darkshading_.gr_DarkShading_max_
                                           % manage.param_ob_darkshading_.gr_DarkShading_mid_
                                            % max_mean_gb_DarkShading
                                            % manage.param_ob_darkshading_.gb_DarkShading_max_
                                           % manage.param_ob_darkshading_.gb_DarkShading_mid_
                                            % max_mean_b_DarkShading
                                            % manage.param_ob_darkshading_.b_DarkShading_max_
                                           % manage.param_ob_darkshading_.b_DarkShading_mid_
                                            % min_mean_r_DarkShading
                                            % manage.param_ob_darkshading_.r_DarkShading_min_
                                            % min_mean_gr_DarkShading
                                            % manage.param_ob_darkshading_.gr_DarkShading_min_
                                            % min_mean_gb_DarkShading
                                            % manage.param_ob_darkshading_.gb_DarkShading_min_
                                            % min_mean_b_DarkShading
                                            % manage.param_ob_darkshading_.b_DarkShading_min_
                                           );

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % darkshading_result.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
    {
        ob_result.g_msg = false;
        ob_ob_max_min_diff_result.g_msg = false;
        darkshading_result.g_msg = false;
    }
    vector<Subitem_Result> resluts;
    resluts.push_back(ob_result);
    resluts.push_back(ob_ob_max_min_diff_result);
    resluts.push_back(darkshading_result);


    return resluts;
}





vector<Subitem_Result> DarkTest::OB_and_DarkShading_V1_3(const ParamManage& manage)
{
    int w=manage.width_;
    int h=manage.height_;

    Subitem_Result ob_result("OB Test_V1.3",true);
    Subitem_Result ob_ob_max_min_diff_result("OB Max Min Diff_result_V1.3",true);
    Subitem_Result darkshading_result("DarkShading_V1.3",true);

    if(manage.param_ob_darkshading_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------OB and DarkShading V1.3-----------------------------\n"));

        //---------------------Whole Region Light Response------------------

        double mean_r_roi = -1;
        double mean_gr_roi = -1;
        double mean_gb_roi = -1;
        double mean_b_roi = -1;

        TestItem::test_roi_channel_mean( w,h,
                                         manage.param_ob_darkshading_.roi_x_,
                                         manage.param_ob_darkshading_.roi_y_,
                                         manage.param_ob_darkshading_.roi_w_,
                                         manage.param_ob_darkshading_.roi_h_,
                                         mean_r_roi,mean_gr_roi,mean_gb_roi,mean_b_roi,
                                         Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                         Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);
        g_str_msg_all[site_]+=boost::str(boost::format("mean_r:%.2f, mean_gr:%.2f, mean_gb:%.2f, mean_b:%.2f\n")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);

        //-----------------------OB Test-----------------------------
        int roi_w_ = manage.param_ob_darkshading_.OB_region_r_;
        int roi_h_ = manage.param_ob_darkshading_.OB_region_r_;
        int skip_ = manage.param_ob_darkshading_.skip_;
//        double ob_ref_ = manage.param_ob_darkshading_.ref;
        double ob_Golden_dif = manage.param_ob_darkshading_.Golden_dif;
        double center_mean_r,center_mean_gr,center_mean_gb,center_mean_b;


//        double mean_r_roi_dif = fabs(mean_r_roi-ob_ref_);
//        double mean_gr_roi_dif = fabs(mean_gr_roi-ob_ref_);
//        double mean_gb_roi_dif = fabs(mean_gb_roi-ob_ref_);
//        double mean_b_roi_dif = fabs(mean_b_roi-ob_ref_);


        double min_diff_spec_class1 = manage.param_ob_darkshading_.min_diff_ref_class1_;
        double max_diff_spec_class1 = manage.param_ob_darkshading_.max_diff_ref_class1_;

        double min_diff_spec_class2 = manage.param_ob_darkshading_.min_diff_ref_class2_;
        double max_diff_spec_class2 = manage.param_ob_darkshading_.max_diff_ref_class2_;

        QVector<cv::Rect> OB_ROI;
        QVector<double> roi_r_ob;
        QVector<double> roi_gr_ob;
        QVector<double> roi_gb_ob;
        QVector<double> roi_b_ob;

        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,skip_,roi_w_,roi_h_));//leftup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,skip_,roi_w_,roi_h_));//rightup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect((w/2-1)/2-roi_w_/2,(h/2-1)/2-roi_h_/2,roi_w_,roi_h_));//center
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//leftdown
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//rightdown

        TestItem::test_roi_shading_to_center_bychannel(w, h, OB_ROI,
                                        roi_r_ob, roi_gr_ob, roi_gb_ob, roi_b_ob, center_mean_r, center_mean_gr, center_mean_gb, center_mean_b,
                                        Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                        Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);


        double mean_r_roi_dif = fabs(mean_r_roi-center_mean_r);
        double mean_gr_roi_dif = fabs(mean_gr_roi-center_mean_gr);
        double mean_gb_roi_dif = fabs(mean_gb_roi-center_mean_gb);
        double mean_b_roi_dif = fabs(mean_b_roi-center_mean_b);


        double max_roi_r_ob = *std::max_element(roi_r_ob.begin(),roi_r_ob.end());
        double max_roi_gr_ob = *std::max_element(roi_gr_ob.begin(),roi_gr_ob.end());
        double max_roi_gb_ob = *std::max_element(roi_gb_ob.begin(),roi_gb_ob.end());
        double max_roi_b_ob = *std::max_element(roi_b_ob.begin(),roi_b_ob.end());
        double max_roi_rgb_ob = std::max(std::max(max_roi_r_ob,max_roi_gr_ob),std::max(max_roi_gb_ob,max_roi_b_ob));

        double min_roi_r_ob = *std::min_element(roi_r_ob.begin(),roi_r_ob.end());
        double min_roi_gr_ob = *std::min_element(roi_gr_ob.begin(),roi_gr_ob.end());
        double min_roi_gb_ob = *std::min_element(roi_gb_ob.begin(),roi_gb_ob.end());
        double min_roi_b_ob = *std::min_element(roi_b_ob.begin(),roi_b_ob.end());
        double min_roi_rgb_ob = std::min(std::min(min_roi_r_ob,min_roi_gr_ob),std::min(min_roi_gb_ob,min_roi_b_ob));

        double rgb_ob_max_min_diff = max_roi_rgb_ob - min_roi_rgb_ob;



        if (max_roi_rgb_ob > max_diff_spec_class2 ||  min_roi_rgb_ob < min_diff_spec_class2)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class2_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class2_;
        }
        else if ((max_roi_rgb_ob>=max_diff_spec_class1 && max_roi_rgb_ob <= max_diff_spec_class2)||
                 (min_roi_rgb_ob>=min_diff_spec_class2 && min_roi_rgb_ob <= min_diff_spec_class1) )
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_class1_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_class1_;
        }
        else if(mean_r_roi_dif < ob_Golden_dif || mean_gr_roi_dif < ob_Golden_dif || mean_gb_roi_dif < ob_Golden_dif || mean_b_roi_dif < ob_Golden_dif)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_OB_Golden_;
            ob_result.grade=manage.param_ob_darkshading_.grade_OB_Golden_;
        }

        if (rgb_ob_max_min_diff > manage.param_ob_darkshading_.ob_max_min_diff_)
        {
            ob_ob_max_min_diff_result.is_passed = false;
            ob_ob_max_min_diff_result.bin = manage.param_ob_darkshading_.bin_ob_max_min_diff_;
            ob_ob_max_min_diff_result.grade = manage.param_ob_darkshading_.grade_ob_max_min_diff_;
        }

        if (mean_r_roi < manage.param_ob_darkshading_.min_Mean_ ||
            mean_r_roi > manage.param_ob_darkshading_.max_Mean_ ||
            mean_gr_roi < manage.param_ob_darkshading_.min_Mean_ ||
            mean_gr_roi > manage.param_ob_darkshading_.max_Mean_ ||
            mean_gb_roi < manage.param_ob_darkshading_.min_Mean_ ||
            mean_gb_roi > manage.param_ob_darkshading_.max_Mean_ ||
            mean_b_roi < manage.param_ob_darkshading_.min_Mean_ ||
            mean_b_roi > manage.param_ob_darkshading_.max_Mean_)
        {
            ob_result.is_passed =false;
            ob_result.bin=manage.param_ob_darkshading_.bin_ob_mean;
            ob_result.grade=manage.param_ob_darkshading_.grade_ob_mean;
        }



        for(int i=0; i<OB_ROI.size(); i++)
        {
            if(i==0)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==1)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==2)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Center ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==3)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==4)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
        }

        g_str_msg_all[site_]+=boost::str(boost::format("ALL_ROIs_RGB_OB_MAX:%.2f, ALL_ROIs_RGB_OB_MIN:%.2f,\n")
                               % max_roi_rgb_ob % min_roi_rgb_ob);

        g_str_msg_all[site_]+=boost::str(boost::format("ALL_ROIs_RGB_OB_MAX_MIN_DIFF:%.2f,\n")
                               % rgb_ob_max_min_diff );

        for(int i=0; i<OB_ROI.size(); i++)
        {
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                                   % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
        }

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|")
                               % max_roi_rgb_ob % min_roi_rgb_ob);

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|")
                               % rgb_ob_max_min_diff);


        //-----------------------DarkShading--------------------------
        double max_mean_r_DarkShading=0;
        double max_mean_gr_DarkShading=0;
        double max_mean_gb_DarkShading=0;
        double max_mean_b_DarkShading=0;

        double min_mean_r_DarkShading=0;
        double min_mean_gr_DarkShading=0;
        double min_mean_gb_DarkShading=0;
        double min_mean_b_DarkShading=0;

        int max_r_DarkShading_pos=0;
        int max_gr_DarkShading_pos=0;
        int max_gb_DarkShading_pos=0;
        int max_b_DarkShading_pos=0;

        int min_r_DarkShading_pos=0;
        int min_gr_DarkShading_pos=0;
        int min_gb_DarkShading_pos=0;
        int min_b_DarkShading_pos=0;


        TestItem::test_channel_darkshading(w,h,manage.param_ob_darkshading_.x_cnt_,manage.param_ob_darkshading_.y_cnt_,
                                           max_mean_r_DarkShading,max_mean_gr_DarkShading,max_mean_gb_DarkShading,max_mean_b_DarkShading,
                                           min_mean_r_DarkShading,min_mean_gr_DarkShading,min_mean_gb_DarkShading,min_mean_b_DarkShading,
                                           max_r_DarkShading_pos,max_gr_DarkShading_pos,max_gb_DarkShading_pos,max_b_DarkShading_pos,
                                           min_r_DarkShading_pos,min_gr_DarkShading_pos,min_gb_DarkShading_pos,min_b_DarkShading_pos,
                                           Algorithm_all[site_].g_buf_r_tmp_,Algorithm_all[site_].g_buf_gr_tmp_,Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_, g_str_msg_all[site_]);

        logMsg(std::string("After darkshading test"));


        double max_rgb_darkshading = std::max(std::max(max_mean_r_DarkShading,max_mean_gr_DarkShading),std::max(max_mean_gb_DarkShading,max_mean_b_DarkShading));
        double min_rgb_darkshading = std::min(std::min(min_mean_r_DarkShading,min_mean_gr_DarkShading),std::min(min_mean_gb_DarkShading,min_mean_b_DarkShading));

        double max_min_diff_darkshading = max_rgb_darkshading - min_rgb_darkshading;


        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%d|%d|%d|%d|%d|%d|%d|%d|%.2f|")
                               % max_mean_r_DarkShading % max_mean_gr_DarkShading % max_mean_gb_DarkShading % max_mean_b_DarkShading
                               % min_mean_r_DarkShading % min_mean_gr_DarkShading % min_mean_gb_DarkShading % min_mean_b_DarkShading
                               % max_r_DarkShading_pos % max_gr_DarkShading_pos % max_gb_DarkShading_pos % max_b_DarkShading_pos
                               % min_r_DarkShading_pos % min_gr_DarkShading_pos % min_gb_DarkShading_pos % min_b_DarkShading_pos
                               % max_min_diff_darkshading);



        g_str_msg_all[site_]+=boost::str(boost::format("Darkshading:\n"
                                             " max_mean_r_DarkShading:%.2f ,max_mean_gr_DarkShading:%.2f , max_mean_gb_DarkShading:%.2f , max_mean_b_DarkShading:%.2f\n"
                                             " min_mean_r_DarkShading:%.2f ,min_mean_gr_DarkShading:%.2f , min_mean_gb_DarkShading:%.2f , min_mean_b_DarkShading:%.2f\n"
                                             " max_r_DarkShading_pos:%d ,max_gr_DarkShading_pos:%d , max_gb_DarkShading_pos:%d , max_b_DarkShading_pos:%d\n"
                                             " min_r_DarkShading_pos:%d ,min_gr_DarkShading_pos:%d , min_gb_DarkShading_pos:%d , min_b_DarkShading_pos:%d\n"
                                             " max_min_diff_Darkshading:%.2f\n")
                               % max_mean_r_DarkShading % max_mean_gr_DarkShading % max_mean_gb_DarkShading % max_mean_b_DarkShading
                               % min_mean_r_DarkShading % min_mean_gr_DarkShading % min_mean_gb_DarkShading % min_mean_b_DarkShading
                               % max_r_DarkShading_pos % max_gr_DarkShading_pos % max_gb_DarkShading_pos % max_b_DarkShading_pos
                               % min_r_DarkShading_pos % min_gr_DarkShading_pos % min_gb_DarkShading_pos % min_b_DarkShading_pos
                               % max_min_diff_darkshading);

        if(max_mean_r_DarkShading>manage.param_ob_darkshading_.r_DarkShading_max_||
                max_mean_gr_DarkShading>manage.param_ob_darkshading_.gr_DarkShading_max_||
                max_mean_gb_DarkShading>manage.param_ob_darkshading_.gb_DarkShading_max_||
                max_mean_b_DarkShading>manage.param_ob_darkshading_.b_DarkShading_max_||
                min_mean_r_DarkShading<manage.param_ob_darkshading_.r_DarkShading_min_||
                min_mean_gr_DarkShading<manage.param_ob_darkshading_.gr_DarkShading_min_||
                min_mean_gb_DarkShading<manage.param_ob_darkshading_.gb_DarkShading_min_||
                min_mean_b_DarkShading<manage.param_ob_darkshading_.b_DarkShading_min_)
        {
            darkshading_result.is_passed=false;
            darkshading_result.bin = manage.param_ob_darkshading_.bin_DarkShading_;
            darkshading_result.grade = manage.param_ob_darkshading_.grade_DarkShading_;
        }



        // 1. 输出所有通道值和 SPEC
        g_str_msg_all[site_] += boost::str(boost::format(
            "OB_Mean:\n"
            "mean_r=%.4f, mean_gr=%.4f, mean_gb=%.4f, mean_b=%.4f\n"
            "SPEC: MIN=%.4f, MAX=%.4f\n"
        ) % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi
          % manage.param_ob_darkshading_.min_Mean_
          % manage.param_ob_darkshading_.max_Mean_);


        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                         "max_roi_rgb_ob=%.4f (SPEC MAX_Class2: %.4f) \n"
                                                         "min_roi_rgb_ob=%.4f (SPEC MIN_Class2: %.4f) \n"
                                                         "max_roi_rgb_ob=%.4f (SPEC MAX_Class1: %.4f ~ MAX_Class2: %.4f) \n"
                                                         "min_roi_rgb_ob=%.4f (SPEC MIN_Class1: %.4f ~ MIN_Class2: %.4f) \n"
                                                         "rgb_ob_max_min_diff=%.4f (MAX Diff: %.4f) \n"
                                                         )
                                                   % max_roi_rgb_ob % max_diff_spec_class2
                                                   % min_roi_rgb_ob % min_diff_spec_class2
                                                   % max_roi_rgb_ob % max_diff_spec_class1 % max_diff_spec_class2
                                                   % min_roi_rgb_ob % min_diff_spec_class1 % min_diff_spec_class2
                                                   % rgb_ob_max_min_diff % manage.param_ob_darkshading_.ob_max_min_diff_
                                                   );


        g_str_msg_all[site_] += boost::str(boost::format("SPEC: \n"
                                                        "max_mean_r_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_gr_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_gb_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "max_mean_b_DarkShading=%.4f (MAX: %.4f) \n"
                                                        "min_mean_r_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_gr_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_gb_DarkShading=%.4f (MIN: %.4f) \n"
                                                        "min_mean_b_DarkShading=%.4f (MIN: %.4f) \n"
                                                         )
                                            % max_mean_r_DarkShading
                                            % manage.param_ob_darkshading_.r_DarkShading_max_
                                            % max_mean_gr_DarkShading
                                            % manage.param_ob_darkshading_.gr_DarkShading_max_
                                            % max_mean_gb_DarkShading
                                            % manage.param_ob_darkshading_.gb_DarkShading_max_
                                            % max_mean_b_DarkShading
                                            % manage.param_ob_darkshading_.b_DarkShading_max_
                                            % min_mean_r_DarkShading
                                            % manage.param_ob_darkshading_.r_DarkShading_min_
                                            % min_mean_gr_DarkShading
                                            % manage.param_ob_darkshading_.gr_DarkShading_min_
                                            % min_mean_gb_DarkShading
                                            % manage.param_ob_darkshading_.gb_DarkShading_min_
                                            % min_mean_b_DarkShading
                                            % manage.param_ob_darkshading_.b_DarkShading_min_
                                           );

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % darkshading_result.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";
    }
    else
    {
        ob_result.g_msg = false;
        ob_ob_max_min_diff_result.g_msg = false;
        darkshading_result.g_msg = false;
    }
    vector<Subitem_Result> resluts;
    resluts.push_back(ob_result);
    resluts.push_back(ob_ob_max_min_diff_result);
    resluts.push_back(darkshading_result);

    return resluts;
}





Subitem_Result DarkTest::OB_and_GRGB(const ParamManage& manage)
{
    if(manage.ob_grgb_version == "ob_grgb_V1.0")
        return OB_and_GRGB_V1_0(manage);

    Subitem_Result res("OB_and_GRGB", false);
    res.g_msg = false;
    return res;
}


Subitem_Result DarkTest::OB_and_GRGB_V1_0(const ParamManage& manage)
{
    Subitem_Result ob_gr_gb_diff_result("OB Gr Gb Diff_result",true);

    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_ob_grgb_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------OB and GrGb Dif V1.0-----------------------------\n"));

        //---------------------Whole Region Light Response------------------

        double mean_r_roi = -1;
        double mean_gr_roi = -1;
        double mean_gb_roi = -1;
        double mean_b_roi = -1;


        TestItem::test_roi_channel_mean( w,h,
                                         manage.param_light_response_.roi_x_,
                                         manage.param_light_response_.roi_y_,
                                         manage.param_light_response_.roi_w_,
                                         manage.param_light_response_.roi_h_,
                                         mean_r_roi,mean_gr_roi,mean_gb_roi,mean_b_roi,
                                         Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                         Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);


        g_str_log_all[site_]+=boost::str(boost::format("%.2f|%.2f|%.2f|%.2f|")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);
        g_str_msg_all[site_]+=boost::str(boost::format("mean_r:%.2f, mean_gr:%.2f, mean_gb:%.2f, mean_b:%.2f\n")
                               % mean_r_roi % mean_gr_roi % mean_gb_roi % mean_b_roi);
        double abs_gr_gb =qAbs(mean_gr_roi-mean_gb_roi); //|GR-GB| 绝对值

        g_str_log_all[site_]+=boost::str(boost::format("%.2f|")
                               % abs_gr_gb);
        g_str_msg_all[site_]+=boost::str(boost::format("mean_gr-mean_gb:%.2f\n")
                               % abs_gr_gb);



        //-----------------------OB Test-----------------------------
        int roi_w_ = manage.param_ob_grgb_.OB_region_r_;
        int roi_h_ = manage.param_ob_grgb_.OB_region_r_;
        int skip_ = manage.param_ob_grgb_.skip_;
        double ob_ref_ = manage.param_ob_grgb_.ref;



        QVector<cv::Rect> OB_ROI;
        QVector<double> roi_r_ob;
        QVector<double> roi_gr_ob;
        QVector<double> roi_gb_ob;
        QVector<double> roi_b_ob;
        QVector<double> roi_GrGb_ob;

        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,skip_,roi_w_,roi_h_));//leftup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,skip_,roi_w_,roi_h_));//rightup
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect((w/2-1)/2-roi_w_/2,(h/2-1)/2-roi_h_/2,roi_w_,roi_h_));//center
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(skip_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//leftdown
        OB_ROI.append(cv::Rect(0,0,w/2-1,h/2-1)&cv::Rect(w/2-1-skip_-roi_w_,h/2-1-skip_-roi_h_,roi_w_,roi_h_));//rightdown

        TestItem::test_roi_ob_bychannel(w, h, ob_ref_, OB_ROI,
                                        roi_r_ob, roi_gr_ob, roi_gb_ob, roi_b_ob,
                                        Algorithm_all[site_].g_buf_r_tmp_, Algorithm_all[site_].g_buf_gr_tmp_,
                                        Algorithm_all[site_].g_buf_gb_tmp_,Algorithm_all[site_].g_buf_b_tmp_);

        for(int i=0; i<OB_ROI.size(); i++)
        {

            if(i==0)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==1)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightup ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));

            }
            else if(i==2)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Center ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==3)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Leftdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
            else if(i==4)
            {
                g_str_msg_all[site_]+=boost::str(boost::format("Rightdown ROI OB_R:%.2f, OB_Gr:%.2f, OB_Gb:%.2f, OB_B:%.2f,\n")
                                       % roi_r_ob.at(i) % roi_gr_ob.at(i) % roi_gb_ob.at(i) % roi_b_ob.at(i));
            }
        }



        for(int i=0; i<OB_ROI.size(); i++)
        {
            roi_GrGb_ob.append(qAbs(roi_gr_ob.at(i) - roi_gb_ob.at(i)));
            g_str_log_all[site_]+=boost::str(boost::format("%.2f|")
                                   % (roi_gr_ob.at(i) - roi_gb_ob.at(i)) );
        }
        double Max_roi_GrGb_ob = *std::max_element(roi_GrGb_ob.begin(),roi_GrGb_ob.end());
        g_str_log_all[site_]+=boost::str(boost::format("%.2f|")
                               % Max_roi_GrGb_ob );
        g_str_msg_all[site_]+=boost::str(boost::format("ROI1 OB_GRGB_DIF:%.2f, ROI2 OB_GRGB_DIF:%.2f, ROI3 OB_GRGB_DIF:%.2f, ROI4 OB_GRGB_DIF:%.2f,ROI5 OB_GRGB_DIF:%.2f,\n")
                               % roi_GrGb_ob.at(0) % roi_GrGb_ob.at(1) % roi_GrGb_ob.at(2) % roi_GrGb_ob.at(3)  % roi_GrGb_ob.at(4));
        g_str_msg_all[site_]+=boost::str(boost::format("ROI OB_GRGB_DIF MAX:%.2f\n")
                               % Max_roi_GrGb_ob );



        if (Max_roi_GrGb_ob > manage.param_ob_grgb_.max_ob_Gr_Gb_diff_ )
        {
            ob_gr_gb_diff_result.is_passed =false;
            ob_gr_gb_diff_result.bin=manage.param_ob_grgb_.bin_OB_Gr_Gb_diff_class2_;
            ob_gr_gb_diff_result.grade=manage.param_ob_grgb_.grade_OB_Gr_Gb_diff_class2_;
        }
        else  if (Max_roi_GrGb_ob > manage.param_ob_grgb_.min_ob_Gr_Gb_diff_ )
        {
            ob_gr_gb_diff_result.is_passed =false;
            ob_gr_gb_diff_result.bin=manage.param_ob_grgb_.bin_OB_Gr_Gb_diff_class1_;
            ob_gr_gb_diff_result.grade=manage.param_ob_grgb_.grade_OB_Gr_Gb_diff_class1_;
        }

        g_str_msg_all[site_] += boost::str(boost::format(
            "SPEC:\n"
            "Max_roi_GrGb_ob: %.4f\n"
            "SPEC MAX: %.4f\n"
            "SPEC MIN: %.4f\n"
        ) % Max_roi_GrGb_ob
          % manage.param_ob_grgb_.max_ob_Gr_Gb_diff_
          % manage.param_ob_grgb_.min_ob_Gr_Gb_diff_);

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % ob_gr_gb_diff_result.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cast_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
    {
        ob_gr_gb_diff_result.g_msg = false;
    }

    return ob_gr_gb_diff_result;
}



//仅用于黑屏（mode == DN） Deadrow and Deadcol  注：需要使用g_binary_
Subitem_Result DarkTest::FPN_DL_Bright_Dark(const ParamManage& manage,bool simple_test_COM)
{
    if(manage.fpn_dl_bright_dark_version == "fpn_dl_bright_dark_V1.0")
       return FPN_DL_Bright_Dark_V1_0( manage,simple_test_COM);
    else if(manage.fpn_dl_bright_dark_version == "fpn_dl_bright_dark_V1.1")
       return FPN_DL_Bright_Dark_V1_1( manage,simple_test_COM);
    else if(manage.fpn_dl_bright_dark_version == "fpn_dl_bright_dark_V1.2")
       return FPN_DL_Bright_Dark_V1_2( manage,simple_test_COM);


    Subitem_Result res("Deadcol_Brightdark_lines", false);
    res.g_msg = false;
    return res;
}


Subitem_Result DarkTest::FPN_DL_Bright_Dark_V1_0(const ParamManage& manage,bool simple_test_COM)
{
    Subitem_Result resule_line("Deadcol_Brightdark_lines",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_fpn_dl_bright_dark.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadcol Brightdark lines only darktest V1.0-----------------------------\n"));

        int replace_DN = 0;
        if(Algorithm_all[site_].mean_gr_roi >0)
            replace_DN = Algorithm_all[site_].mean_gr_roi;
        else
            replace_DN = 16;

        //坏点抹掉后进行线条查找  简测时不需要补
        if(!simple_test_COM)
        {
            for(int i=0; i<h; i++)
                for(int j=0; j<w; j++)
                {
                    if(Algorithm_all[site_].g_binary_[i*w+j]>128)
                        Algorithm_all[site_].g_buf_raw_[i*w+j]=replace_DN;
                }

            TestItem::split_channel(manage.bayer_,w,h,
                                    Algorithm_all[site_].g_buf_raw_,Algorithm_all[site_].g_buf_r_,
                                    Algorithm_all[site_].g_buf_gr_,Algorithm_all[site_].g_buf_gb_,
                                    Algorithm_all[site_].g_buf_b_);
        }


        double max_col_diff[4]={0};
        int max_col_pos[4]={0};
        double max_bright_col_diff[4]={0};
        int max_bright_col_pos[4]={0};
        double max_dark_col_diff[4]={0};
        int max_dark_col_pos[4]={0};
        double col_max_min_diff[4]={0};

        int cnt_dead_col=0;
        int cnt_col_dol =0;
        double col_dol_std_max[4]={0};
        int col_dol_std_max_pos[4]={0};

        int cnt_breakline_col =0;
        double breakline_col_max_value[4]={0};
        int breakline_col_max_pos[4]={0};


        TestItem::test_col_defect_bychannel_bright_dark(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                                        Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                                        manage.param_fpn_dl_bright_dark.strip_col_,
                                                        manage.param_fpn_dl_bright_dark.r_area_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_spec_,
                                                        max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                                        cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                                        DN, g_str_msg_all[site_], manage.param_fpn_dl_bright_dark.log_print_,
                                                        max_bright_col_diff,max_bright_col_pos,max_dark_col_diff,max_dark_col_pos);//max_dark_col_diff为负值


//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
//                               % max_row_diff[0] % max_row_diff[1] % max_row_diff[2] % max_row_diff[3]
//                % max_row_pos[0] % max_row_pos[1] % max_row_pos[2] % max_row_pos[3]
//                % row_max_min_diff[0] % row_max_min_diff[1] % row_max_min_diff[2] % row_max_min_diff[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % row_dol_std_max[0] % row_dol_std_max[1] % row_dol_std_max[2] % row_dol_std_max[3]
//                % row_dol_std_max_pos[0] % row_dol_std_max_pos[1] % row_dol_std_max_pos[2] % row_dol_std_max_pos[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % breakline_row_max_value[0] % breakline_row_max_value[1] % breakline_row_max_value[2] % breakline_row_max_value[3]
//                % breakline_row_max_pos[0] % breakline_row_max_pos[1] % breakline_row_max_pos[2] % breakline_row_max_pos[3]);


        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
                               % max_col_diff[0] % max_col_diff[1] % max_col_diff[2] % max_col_diff[3]
                % max_col_pos[0] % max_col_pos[1] % max_col_pos[2] % max_col_pos[3]
                % col_max_min_diff[0] % col_max_min_diff[1] % col_max_min_diff[2] % col_max_min_diff[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                         % max_bright_col_diff[0] % max_bright_col_diff[1] % max_bright_col_diff[2] % max_bright_col_diff[3]
                % max_bright_col_pos[0] % max_bright_col_pos[1] % max_bright_col_pos[2] % max_bright_col_pos[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                         % max_dark_col_diff[0] % max_dark_col_diff[1] % max_dark_col_diff[2] % max_dark_col_diff[3]
                % max_dark_col_pos[0] % max_dark_col_pos[1] % max_dark_col_pos[2] % max_dark_col_pos[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                               % col_dol_std_max[0] % col_dol_std_max[1] % col_dol_std_max[2] % col_dol_std_max[3]
                % col_dol_std_max_pos[0] % col_dol_std_max_pos[1] % col_dol_std_max_pos[2] % col_dol_std_max_pos[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % breakline_col_max_value[0] % breakline_col_max_value[1] % breakline_col_max_value[2] % breakline_col_max_value[3]
//                % breakline_col_max_pos[0] % breakline_col_max_pos[1] % breakline_col_max_pos[2] % breakline_col_max_pos[3]);


//        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|")
//                               % cnt_dead_row % cnt_dead_col %cnt_row_dol %cnt_col_dol %cnt_breakline_row %cnt_breakline_col);




        logMsg (std::string("after dead line"));

        int cnt_dol = /*cnt_row_dol +*/ cnt_col_dol;
        int cnt_break_line = /*cnt_breakline_row +*/ cnt_breakline_col;
        if (cnt_dead_col>0 /*|| cnt_dead_row>0*/ || cnt_dol>0)
        {
            resule_line.is_passed = false;
        }

        int cnt_fpn_level_1=0;
        int cnt_fpn_level_2=0;
        int cnt_hard_dead_col=0;
        int cnt_hard_dead_col_bright=0;
        int cnt_hard_dead_col_dark=0;
        if(cnt_dead_col>0)
        {
            for (int i=0; i<4; i++)
            {
                if(max_col_diff[i]>manage.param_fpn_dl_bright_dark.deadcol_spec_)
                    cnt_hard_dead_col++;
                else if(max_col_diff[i]>manage.param_fpn_dl_bright_dark.fpn_level_2_spec_)
                    cnt_fpn_level_2++;
                else
                    cnt_fpn_level_1++;
            }
        }

        //新增亮暗线和spec对比
        for (int i=0; i<4; i++)
        {
            if(max_bright_col_diff[i]>manage.param_fpn_dl_bright_dark.deadcol_bright_spec_)
                cnt_hard_dead_col_bright++;
            if(max_dark_col_diff[i]<manage.param_fpn_dl_bright_dark.deadcol_dark_spec_)//max_dark_col_diff为负值
                cnt_hard_dead_col_dark++;
        }
        if (cnt_hard_dead_col_bright>0  || cnt_hard_dead_col_dark>0)
        {
            resule_line.is_passed = false;
        }




        if (cnt_hard_dead_col>0)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dead_col_;
            resule_line.grade= manage.param_fpn_dl_bright_dark.grade_dead_col_;
        }
        else if (cnt_fpn_level_2 >0)
        {
            resule_line.bin= manage.param_fpn_dl_bright_dark.bin_fpn_level_2_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_fpn_level_2_;
        }
        else if (cnt_fpn_level_1 >0)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_fpn_level_1_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_fpn_level_1_;
        }

//        if (cnt_dead_row>0 && manage.param_fpn_dl_bright_dark.grade_dead_row_>resule_line.grade)
//        {
//            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dead_row_;
//            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_dead_row_;
//        }
        //新增
        if (cnt_hard_dead_col_bright>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_bright_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_bright_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_bright_;
        }

        if (cnt_hard_dead_col_dark>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_dark_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_dark_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_dark_;
        }


        if (cnt_dol>0 && manage.param_fpn_dl_bright_dark.grade_dol_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dol_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_dol_;
        }

        if (cnt_break_line>0 && manage.param_fpn_dl_bright_dark.grade_break_line_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_break_line_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_break_line_;
        }

//        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|")
//                               % cnt_hard_dead_col %cnt_fpn_level_2 %cnt_fpn_level_1);

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % resule_line.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";
    }
    else
    {
        resule_line.g_msg = false;
    }


    return resule_line;
}
Subitem_Result DarkTest::FPN_DL_Bright_Dark_V1_1(const ParamManage& manage,bool simple_test_COM)
{
    //20C3 黑屏亮暗线：针对亮线进行分段
    Subitem_Result resule_line("Deadcol_Brightdark_lines_V1.1",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_fpn_dl_bright_dark.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadcol Brightdark lines only darktest V1.1-----------------------------\n"));

        int replace_DN = 0;
        if(Algorithm_all[site_].mean_gr_roi >0)
            replace_DN = Algorithm_all[site_].mean_gr_roi;
        else
            replace_DN = 16;

        //坏点抹掉后进行线条查找  简测时不需要补
        if(!simple_test_COM)
        {
            for(int i=0; i<h; i++)
                for(int j=0; j<w; j++)
                {
                    if(Algorithm_all[site_].g_binary_[i*w+j]>128)
                        Algorithm_all[site_].g_buf_raw_[i*w+j]=replace_DN;
                }

            TestItem::split_channel(manage.bayer_,w,h,
                                    Algorithm_all[site_].g_buf_raw_,Algorithm_all[site_].g_buf_r_,
                                    Algorithm_all[site_].g_buf_gr_,Algorithm_all[site_].g_buf_gb_,
                                    Algorithm_all[site_].g_buf_b_);
        }


        double max_col_diff[4]={0};
        int max_col_pos[4]={0};
        double max_bright_col_diff[4]={0};
        int max_bright_col_pos[4]={0};
        double max_dark_col_diff[4]={0};
        int max_dark_col_pos[4]={0};
        double col_max_min_diff[4]={0};

        int cnt_dead_col=0;
        int cnt_col_dol =0;
        double col_dol_std_max[4]={0};
        int col_dol_std_max_pos[4]={0};

        int cnt_breakline_col =0;
        double breakline_col_max_value[4]={0};
        int breakline_col_max_pos[4]={0};


        TestItem::test_col_defect_bychannel_bright_dark(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                                        Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                                        manage.param_fpn_dl_bright_dark.strip_col_,
                                                        manage.param_fpn_dl_bright_dark.r_area_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_spec_,
                                                        max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                                        cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                                        DN, g_str_msg_all[site_], manage.param_fpn_dl_bright_dark.log_print_,
                                                        max_bright_col_diff,max_bright_col_pos,max_dark_col_diff,max_dark_col_pos);//max_dark_col_diff为负值


//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
//                               % max_row_diff[0] % max_row_diff[1] % max_row_diff[2] % max_row_diff[3]
//                % max_row_pos[0] % max_row_pos[1] % max_row_pos[2] % max_row_pos[3]
//                % row_max_min_diff[0] % row_max_min_diff[1] % row_max_min_diff[2] % row_max_min_diff[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % row_dol_std_max[0] % row_dol_std_max[1] % row_dol_std_max[2] % row_dol_std_max[3]
//                % row_dol_std_max_pos[0] % row_dol_std_max_pos[1] % row_dol_std_max_pos[2] % row_dol_std_max_pos[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % breakline_row_max_value[0] % breakline_row_max_value[1] % breakline_row_max_value[2] % breakline_row_max_value[3]
//                % breakline_row_max_pos[0] % breakline_row_max_pos[1] % breakline_row_max_pos[2] % breakline_row_max_pos[3]);


//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
//                               % max_col_diff[0] % max_col_diff[1] % max_col_diff[2] % max_col_diff[3]
//                % max_col_pos[0] % max_col_pos[1] % max_col_pos[2] % max_col_pos[3]
//                % col_max_min_diff[0] % col_max_min_diff[1] % col_max_min_diff[2] % col_max_min_diff[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                         % max_bright_col_diff[0] % max_bright_col_diff[1] % max_bright_col_diff[2] % max_bright_col_diff[3]
                % max_bright_col_pos[0] % max_bright_col_pos[1] % max_bright_col_pos[2] % max_bright_col_pos[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                         % max_dark_col_diff[0] % max_dark_col_diff[1] % max_dark_col_diff[2] % max_dark_col_diff[3]
                % max_dark_col_pos[0] % max_dark_col_pos[1] % max_dark_col_pos[2] % max_dark_col_pos[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % col_dol_std_max[0] % col_dol_std_max[1] % col_dol_std_max[2] % col_dol_std_max[3]
//                % col_dol_std_max_pos[0] % col_dol_std_max_pos[1] % col_dol_std_max_pos[2] % col_dol_std_max_pos[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % breakline_col_max_value[0] % breakline_col_max_value[1] % breakline_col_max_value[2] % breakline_col_max_value[3]
//                % breakline_col_max_pos[0] % breakline_col_max_pos[1] % breakline_col_max_pos[2] % breakline_col_max_pos[3]);


//        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|")
//                               % cnt_dead_row % cnt_dead_col %cnt_row_dol %cnt_col_dol %cnt_breakline_row %cnt_breakline_col);





        logMsg (std::string("after dead line"));

        int cnt_dol = /*cnt_row_dol +*/ cnt_col_dol;
        int cnt_break_line = /*cnt_breakline_row +*/ cnt_breakline_col;
        if (cnt_dead_col>0 /*|| cnt_dead_row>0*/ || cnt_dol>0)
        {
            resule_line.is_passed = false;
        }

        int cnt_fpn_level_1=0;
        int cnt_fpn_level_2=0;
        int cnt_hard_dead_col=0;
        int cnt_hard_dead_col_bright=0;
        int cnt_hard_dead_col_bright_MA=0;
        int cnt_hard_dead_col_dark=0;
        if(cnt_dead_col>0)
        {
            for (int i=0; i<4; i++)
            {
                if(max_col_diff[i]>manage.param_fpn_dl_bright_dark.deadcol_spec_)
                    cnt_hard_dead_col++;
                else if(max_col_diff[i]>manage.param_fpn_dl_bright_dark.fpn_level_2_spec_)
                    cnt_fpn_level_2++;
                else
                    cnt_fpn_level_1++;
            }
        }

        //新增亮暗线和spec对比
        for (int i=0; i<4; i++)
        {
            if(max_bright_col_diff[i]>manage.param_fpn_dl_bright_dark.deadcol_bright_level_2_spec_)
                cnt_hard_dead_col_bright++;
            else if(max_bright_col_diff[i]>manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_)
                cnt_hard_dead_col_bright_MA++;
            if(max_dark_col_diff[i]<manage.param_fpn_dl_bright_dark.deadcol_dark_spec_)//max_dark_col_diff为负值
                cnt_hard_dead_col_dark++;
        }
        if (cnt_hard_dead_col_bright>0  || cnt_hard_dead_col_bright_MA>0 || cnt_hard_dead_col_dark>0)
        {
            resule_line.is_passed = false;
        }




        if (cnt_hard_dead_col>0)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dead_col_;
            resule_line.grade= manage.param_fpn_dl_bright_dark.grade_dead_col_;
        }
        else if (cnt_fpn_level_2 >0)
        {
            resule_line.bin= manage.param_fpn_dl_bright_dark.bin_fpn_level_2_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_fpn_level_2_;
        }
        else if (cnt_fpn_level_1 >0)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_fpn_level_1_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_fpn_level_1_;
        }

//        if (cnt_dead_row>0 && manage.param_fpn_dl_bright_dark.grade_dead_row_>resule_line.grade)
//        {
//            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dead_row_;
//            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_dead_row_;
//        }
        //新增
        if (cnt_hard_dead_col_bright>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_bright_level_2_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_bright_level_2_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_bright_level_2_;
        }
        else if (cnt_hard_dead_col_bright_MA>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_bright_level_1_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_bright_level_1_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_bright_level_1_;
        }

        if (cnt_hard_dead_col_dark>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_dark_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_dark_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_dark_;
        }


        if (cnt_dol>0 && manage.param_fpn_dl_bright_dark.grade_dol_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dol_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_dol_;
        }

        if (cnt_break_line>0 && manage.param_fpn_dl_bright_dark.grade_break_line_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_break_line_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_break_line_;
        }

        g_str_msg_all[site_]+=boost::str(boost::format("cnt_hard_dead_col_bright: %d, cnt_hard_dead_col_bright_level2: %d, deadcol_bright_level_1_spec_: %d，deadcol_bright_level_2_spec_: %d\n")
                                         % cnt_hard_dead_col_bright % cnt_hard_dead_col_bright_MA % manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_ % manage.param_fpn_dl_bright_dark.deadcol_bright_level_2_spec_ );

        g_str_msg_all[site_]+=boost::str(boost::format("cnt_hard_dead_col_dark: %d, deadcol_dark_spec_: %d\n")
                                         % cnt_hard_dead_col_dark %  manage.param_fpn_dl_bright_dark.deadcol_dark_spec_  );


//        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|")
//                               % cnt_hard_dead_col %cnt_fpn_level_2 %cnt_fpn_level_1);

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % resule_line.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";
    }
    else
    {
        resule_line.g_msg = false;
    }


    return resule_line;
}



Subitem_Result DarkTest::FPN_DL_Bright_Dark_V1_2(const ParamManage& manage,bool simple_test_COM)
{
    //20C3 黑屏亮暗线：针对亮线进行分段
    Subitem_Result resule_line("Deadcol_Brightdark_lines_V1.2",true);
    int w=manage.width_;
    int h=manage.height_;

    if(manage.param_fpn_dl_bright_dark.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();

        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Deadcol Brightdark lines only darktest V1.2-----------------------------\n"));

        int replace_DN = 0;
        if(Algorithm_all[site_].mean_gr_roi >0)
            replace_DN = Algorithm_all[site_].mean_gr_roi;
        else
            replace_DN = 16;

        //坏点抹掉后进行线条查找  简测时不需要补
        if(!simple_test_COM)
        {
            for(int i=0; i<h; i++)
                for(int j=0; j<w; j++)
                {
                    if(Algorithm_all[site_].g_binary_[i*w+j]>128)
                        Algorithm_all[site_].g_buf_raw_[i*w+j]=replace_DN;
                }

            TestItem::split_channel(manage.bayer_,w,h,
                                    Algorithm_all[site_].g_buf_raw_,Algorithm_all[site_].g_buf_r_,
                                    Algorithm_all[site_].g_buf_gr_,Algorithm_all[site_].g_buf_gb_,
                                    Algorithm_all[site_].g_buf_b_);
        }


        double max_col_diff[4]={0};
        int max_col_pos[4]={0};
        double max_bright_col_diff[4]={0};
        int max_bright_col_pos[4]={0};
        double max_dark_col_diff[4]={0};
        int max_dark_col_pos[4]={0};
        double col_max_min_diff[4]={0};

        int cnt_dead_col=0;
        int cnt_col_dol =0;
        double col_dol_std_max[4]={0};
        int col_dol_std_max_pos[4]={0};

        int cnt_breakline_col =0;
        double breakline_col_max_value[4]={0};
        int breakline_col_max_pos[4]={0};


        TestItem::test_col_defect_bychannel_bright_dark(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_,
                                                        Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,w, h,
                                                        manage.param_fpn_dl_bright_dark.strip_col_,
                                                        manage.param_fpn_dl_bright_dark.r_area_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.spec_dol_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.fpn_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_level_1_spec_,
                                                        manage.param_fpn_dl_bright_dark.deadcol_dark_level_1_spec_,
                                                        max_col_diff, max_col_pos, col_max_min_diff, cnt_dead_col,
                                                        cnt_col_dol, col_dol_std_max, col_dol_std_max_pos,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        manage.param_fpn_dl_bright_dark.break_col_spec_,
                                                        cnt_breakline_col, breakline_col_max_value, breakline_col_max_pos,
                                                        DN, g_str_msg_all[site_], manage.param_fpn_dl_bright_dark.log_print_,
                                                        max_bright_col_diff,max_bright_col_pos,max_dark_col_diff,max_dark_col_pos);//max_dark_col_diff为负值


//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
//                               % max_row_diff[0] % max_row_diff[1] % max_row_diff[2] % max_row_diff[3]
//                % max_row_pos[0] % max_row_pos[1] % max_row_pos[2] % max_row_pos[3]
//                % row_max_min_diff[0] % row_max_min_diff[1] % row_max_min_diff[2] % row_max_min_diff[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % row_dol_std_max[0] % row_dol_std_max[1] % row_dol_std_max[2] % row_dol_std_max[3]
//                % row_dol_std_max_pos[0] % row_dol_std_max_pos[1] % row_dol_std_max_pos[2] % row_dol_std_max_pos[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % breakline_row_max_value[0] % breakline_row_max_value[1] % breakline_row_max_value[2] % breakline_row_max_value[3]
//                % breakline_row_max_pos[0] % breakline_row_max_pos[1] % breakline_row_max_pos[2] % breakline_row_max_pos[3]);


//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|%.3f|%.3f|%.3f|%.3f|")
//                               % max_col_diff[0] % max_col_diff[1] % max_col_diff[2] % max_col_diff[3]
//                % max_col_pos[0] % max_col_pos[1] % max_col_pos[2] % max_col_pos[3]
//                % col_max_min_diff[0] % col_max_min_diff[1] % col_max_min_diff[2] % col_max_min_diff[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                         % max_bright_col_diff[0] % max_bright_col_diff[1] % max_bright_col_diff[2] % max_bright_col_diff[3]
                % max_bright_col_pos[0] % max_bright_col_pos[1] % max_bright_col_pos[2] % max_bright_col_pos[3]);

        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
                                         % max_dark_col_diff[0] % max_dark_col_diff[1] % max_dark_col_diff[2] % max_dark_col_diff[3]
                % max_dark_col_pos[0] % max_dark_col_pos[1] % max_dark_col_pos[2] % max_dark_col_pos[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % col_dol_std_max[0] % col_dol_std_max[1] % col_dol_std_max[2] % col_dol_std_max[3]
//                % col_dol_std_max_pos[0] % col_dol_std_max_pos[1] % col_dol_std_max_pos[2] % col_dol_std_max_pos[3]);

//        g_str_log_all[site_]+=boost::str(boost::format("%.3f|%.3f|%.3f|%.3f|%d|%d|%d|%d|")
//                               % breakline_col_max_value[0] % breakline_col_max_value[1] % breakline_col_max_value[2] % breakline_col_max_value[3]
//                % breakline_col_max_pos[0] % breakline_col_max_pos[1] % breakline_col_max_pos[2] % breakline_col_max_pos[3]);


//        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|%d|")
//                               % cnt_dead_row % cnt_dead_col %cnt_row_dol %cnt_col_dol %cnt_breakline_row %cnt_breakline_col);





        logMsg (std::string("after dead line"));

        int cnt_dol = /*cnt_row_dol +*/ cnt_col_dol;
        int cnt_break_line = /*cnt_breakline_row +*/ cnt_breakline_col;
        if (cnt_dead_col>0 /*|| cnt_dead_row>0*/ || cnt_dol>0)
        {
            resule_line.is_passed = false;
        }

        int cnt_fpn_level_1=0;
        int cnt_fpn_level_2=0;
        int cnt_hard_dead_col=0;
        int cnt_hard_dead_col_bright=0;
        int cnt_hard_dead_col_bright_MA=0;
        int cnt_hard_dead_col_dark=0;
        int cnt_hard_dead_col_dark_MA=0;
        if(cnt_dead_col>0)
        {
            for (int i=0; i<4; i++)
            {
                if(max_col_diff[i]>manage.param_fpn_dl_bright_dark.deadcol_spec_)
                    cnt_hard_dead_col++;
                else if(max_col_diff[i]>manage.param_fpn_dl_bright_dark.fpn_level_2_spec_)
                    cnt_fpn_level_2++;
                else
                    cnt_fpn_level_1++;
            }
        }

        //新增亮暗线和spec对比
        for (int i=0; i<4; i++)
        {
            if(max_bright_col_diff[i]>manage.param_fpn_dl_bright_dark.deadcol_bright_level_2_spec_)
                cnt_hard_dead_col_bright++;
            else if(max_bright_col_diff[i]>manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_)
                cnt_hard_dead_col_bright_MA++;
            if(max_dark_col_diff[i]<manage.param_fpn_dl_bright_dark.deadcol_dark_level_2_spec_)//max_dark_col_diff为负值
                cnt_hard_dead_col_dark++;
            else if(max_dark_col_diff[i]<manage.param_fpn_dl_bright_dark.deadcol_dark_level_1_spec_)//max_dark_col_diff为负值
                cnt_hard_dead_col_dark_MA++;
        }
        if (cnt_hard_dead_col_bright>0  || cnt_hard_dead_col_bright_MA>0 || cnt_hard_dead_col_dark>0 || cnt_hard_dead_col_dark_MA>0)
        {
            resule_line.is_passed = false;
        }

        if (cnt_hard_dead_col>0)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dead_col_;
            resule_line.grade= manage.param_fpn_dl_bright_dark.grade_dead_col_;
        }
        else if (cnt_fpn_level_2 >0)
        {
            resule_line.bin= manage.param_fpn_dl_bright_dark.bin_fpn_level_2_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_fpn_level_2_;
        }
        else if (cnt_fpn_level_1 >0)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_fpn_level_1_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_fpn_level_1_;
        }

//        if (cnt_dead_row>0 && manage.param_fpn_dl_bright_dark.grade_dead_row_>resule_line.grade)
//        {
//            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dead_row_;
//            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_dead_row_;
//        }
        //新增
        if (cnt_hard_dead_col_bright>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_bright_level_2_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_bright_level_2_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_bright_level_2_;
        }
        else if (cnt_hard_dead_col_bright_MA>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_bright_level_1_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_bright_level_1_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_bright_level_1_;
        }

        if (cnt_hard_dead_col_dark>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_dark_level_2_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_dark_level_2_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_dark_level_2_;
        }
        else  if (cnt_hard_dead_col_dark_MA>0 && manage.param_fpn_dl_bright_dark.grade_deadcol_dark_level_1_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_deadcol_dark_level_1_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_deadcol_dark_level_1_;
        }


        if (cnt_dol>0 && manage.param_fpn_dl_bright_dark.grade_dol_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_dol_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_dol_;
        }

        if (cnt_break_line>0 && manage.param_fpn_dl_bright_dark.grade_break_line_>resule_line.grade)
        {
            resule_line.bin = manage.param_fpn_dl_bright_dark.bin_break_line_;
            resule_line.grade = manage.param_fpn_dl_bright_dark.grade_break_line_;
        }

        g_str_msg_all[site_]+=boost::str(boost::format("cnt_hard_dead_col_bright: %d, cnt_hard_dead_col_bright_MA: %d, deadcol_bright_level_1_spec_: %d，deadcol_bright_level_2_spec_: %d\n")
                                         % cnt_hard_dead_col_bright % cnt_hard_dead_col_bright_MA % manage.param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_ % manage.param_fpn_dl_bright_dark.deadcol_bright_level_2_spec_ );

        g_str_msg_all[site_]+=boost::str(boost::format("cnt_hard_dead_col_dark: %d,cnt_hard_dead_col_dark_MA: %d, deadcol_dark_level_1_spec_: %d, deadcol_dark_level_2_spec_: %d\n")
                                         % cnt_hard_dead_col_dark   % cnt_hard_dead_col_dark_MA %  manage.param_fpn_dl_bright_dark.deadcol_dark_level_1_spec_ %  manage.param_fpn_dl_bright_dark.deadcol_dark_level_2_spec_ );


//        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|")
//                               % cnt_hard_dead_col %cnt_fpn_level_2 %cnt_fpn_level_1);

        g_str_log_all[site_]+=boost::str(boost::format("%s|") % resule_line.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";
    }
    else
    {
        resule_line.g_msg = false;
    }


    return resule_line;
}



Subitem_Result DarkTest::Histogram_Speical_DN_Count(const ParamManage& manage)
{
    if(manage.histogram_version == "histogram_V1.0")
        return Histogram_Speical_DN_Count_V1_0(manage);

    Subitem_Result res("Histogram_res", false);
    res.g_msg = false;
    return res;
}


Subitem_Result DarkTest::Histogram_Speical_DN_Count_V1_0(const ParamManage& manage)
{
    Subitem_Result Histogram_res("Histogram_res",true);
    int w = manage.width_;
    int h = manage.height_;

    if(manage.param_histogram_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Histogram Specail DN Count V1.0-----------------------------\n"));

        QVector<int> Dn_stat = manage.param_histogram_.Dn_stat;
        QVector<int> Dn_end = manage.param_histogram_.Dn_end;
        QVector<int> Cnt_spec_min = manage.param_histogram_.Cnt_spec_min_;
        QVector<int> Cnt_spec_max = manage.param_histogram_.Cnt_spec_max_;


        QVector<int> Dn_stat_cnt(Dn_stat.size(), 0);  // 根据 Dn_stat 大小动态初始化

        int Dn_histogram[256]={0};

        // 填充直方图
        for (int i = 0; i < w * h; ++i) {
            Dn_histogram[Algorithm_all[site_].g_buf_raw_[i]]++;
        }

        std::string msg_spec;
        // 统计符合区间条件的值
        int cnt_all = 0;
        for (size_t k = 0; k < Dn_stat.size(); k++) {
            int low = Dn_stat[k];
            int high = Dn_end[k];
            for (int i = low; i <= high; i++) {
                Dn_stat_cnt[k] += Dn_histogram[i];
            }

            // 检查 Dn_stat_cnt[k] 是否在 Cnt_spec_min[k] 和 Cnt_spec_max[k] 的区间内
            if (Dn_stat_cnt[k] < Cnt_spec_min[k] || Dn_stat_cnt[k] > Cnt_spec_max[k]) {
                Histogram_res.is_passed=false;
                Histogram_res.bin = manage.param_histogram_.bin_;
                Histogram_res.grade = manage.param_histogram_.grade_;
            }
            cnt_all += Dn_stat_cnt[k];


            msg_spec += boost::str(boost::format("Dn_stat_cnt=%d (SPEC: %d ~ %d) \n")
                                                 % Dn_stat_cnt[k]
                                                 % Cnt_spec_min[k]
                                                 % Cnt_spec_max[k]);

        }
        if (cnt_all>manage.param_histogram_.Cnt_spec_all_) {
            Histogram_res.is_passed=false;
            Histogram_res.bin = manage.param_histogram_.bin_;
            Histogram_res.grade = manage.param_histogram_.grade_;
        }


        // 构建格式化消息字符串
        std::string msg = "Counts: ";
        for (size_t k = 0; k < Dn_stat.size(); k++) {
            msg += boost::str(boost::format("Cnt_%d_%dDN: %d, ") % Dn_stat[k] % Dn_end[k] % Dn_stat_cnt[k]);
        }
        msg = msg.substr(0, msg.size() - 2);  // 去除最后一个多余的 ", "

        // 更新 g_str_msg_all
        g_str_msg_all[site_] += msg + "\n";
        g_str_msg_all[site_] += "SPEC:\n" + msg_spec + "\n";

        // 构建日志字符串
        std::string log = "";
        for (size_t k = 0; k < Dn_stat.size(); k++) {
            log += boost::str(boost::format("%d|") % Dn_stat_cnt[k]);
        }
        log = log.substr(0, log.size() - 1);  // 去除最后一个多余的 "|"

        // 更新 g_str_log_all
        g_str_log_all[site_] += log + "|";
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % Histogram_res.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";
    }
    else
    {
        Histogram_res.g_msg = false;
    }

    return Histogram_res;
}





Subitem_Result DarkTest::Global_LUM_DL_test_CP(const ParamManage& manage)
{
    if(manage.edge_roi_deadline_dark_version == "edge_roi_deadline_dark_V1.0")
        return Global_LUM_DL_test_CP_V1_0(manage);

    Subitem_Result res("DarkDL_CP", false);
    res.g_msg = false;
    return res;

}


Subitem_Result DarkTest::Global_LUM_DL_test_CP_V1_0(const ParamManage& manage)
{
    Subitem_Result DarkDL_CP("DarkDL_CP",true);

    if(manage.param_edge_ROI_DL_.open_mode_ == 1)
    {
        Algorithm_all[site_].time_begin=clock();
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------Global_LUM_DL_test_CP V1.0-----------------------------\n"));


        int w = manage.width_;
        int h = manage.height_;


        int skip = manage.param_edge_ROI_DL_.skip_;
        int center_size = manage.param_edge_ROI_DL_.center_size_;

        int edge_size = manage.param_edge_ROI_DL_.edge_size_;               //行列的间距
        int gride_size = manage.param_edge_ROI_DL_.gride_size_;              //这个就是划分的区域,后续可以外配
        int roi_w = ((w-skip-skip) / gride_size);//409
        int roi_h = ((h-skip-skip) / gride_size);//409


        QVector <cv::Rect> up_edge_rois,down_edge_rois,left_edge_rois,right_edge_rois;
        QVector <double>   up_edge_rois_mean,down_edge_rois_mean,left_edge_rois_mean,right_edge_rois_mean;
        QVector <double>   up_edge_rois_mean_dif,down_edge_rois_mean_dif,left_edge_rois_mean_dif,right_edge_rois_mean_dif;
        double mean_center  = AlgorithmFuncs::GetROIMean (Algorithm_all[site_].g_buf_raw_, w, h, (w-center_size)/2,  (h-center_size)/2, center_size, center_size);      //这个200就是卡控中心 ROI 区域的大小

        for(int i = 0;i!=gride_size;i++)
        {
           up_edge_rois.push_back   (cv::Rect(roi_w*i+skip,    0+skip,          roi_w,   edge_size));
           down_edge_rois.push_back (cv::Rect(roi_w*i+skip,    h-edge_size-skip,      roi_w,   edge_size));
           left_edge_rois.push_back (cv::Rect(0+skip,          roi_h*i+skip,    edge_size,roi_h));
           right_edge_rois.push_back(cv::Rect(w-edge_size-skip,roi_h*i+skip,    edge_size,roi_h));
        }
        for(int n = 0;n!=gride_size;n++)
        {
           up_edge_rois_mean.append(AlgorithmFuncs::GetROIMean (Algorithm_all[site_].g_buf_raw_, w, h, up_edge_rois.at(n).x, up_edge_rois.at(n).y, (up_edge_rois.at(n).width-1), (up_edge_rois.at(n).height-1)));
           down_edge_rois_mean.append(AlgorithmFuncs::GetROIMean (Algorithm_all[site_].g_buf_raw_, w, h, down_edge_rois.at(n).x, down_edge_rois.at(n).y, (down_edge_rois.at(n).width-1), (down_edge_rois.at(n).height-1)));
           left_edge_rois_mean.append(AlgorithmFuncs::GetROIMean (Algorithm_all[site_].g_buf_raw_, w, h, left_edge_rois.at(n).x, left_edge_rois.at(n).y, (left_edge_rois.at(n).width-1), (left_edge_rois.at(n).height-1)));
           right_edge_rois_mean.append(AlgorithmFuncs::GetROIMean (Algorithm_all[site_].g_buf_raw_, w, h, right_edge_rois.at(n).x, right_edge_rois.at(n).y, (right_edge_rois.at(n).width-1),(right_edge_rois.at(n).height-1)));
        }
        for(int j = 0;j!=gride_size;++j)
        {
            up_edge_rois_mean_dif.append(up_edge_rois_mean.at(j)-mean_center);
            down_edge_rois_mean_dif.append(down_edge_rois_mean.at(j)-mean_center);
            left_edge_rois_mean_dif.append(left_edge_rois_mean.at(j)-mean_center);
            right_edge_rois_mean_dif.append(right_edge_rois_mean.at(j)-mean_center);
        }

        for(int i = 0;i!=gride_size;++i)
        {
            if((up_edge_rois_mean_dif.at(i) >=manage.param_edge_ROI_DL_.spec_p_) || (up_edge_rois_mean_dif.at(i) <= manage.param_edge_ROI_DL_.spec_n_)
                 || (down_edge_rois_mean_dif.at(i) >=manage.param_edge_ROI_DL_.spec_p_) || (down_edge_rois_mean_dif.at(i) <= manage.param_edge_ROI_DL_.spec_n_)
                 || (left_edge_rois_mean_dif.at(i) >=manage.param_edge_ROI_DL_.spec_p_) || (left_edge_rois_mean_dif.at(i) <= manage.param_edge_ROI_DL_.spec_n_)
                 ||(right_edge_rois_mean_dif.at(i) >=manage.param_edge_ROI_DL_.spec_p_) || (right_edge_rois_mean_dif.at(i) <= manage.param_edge_ROI_DL_.spec_n_) )
            {
                DarkDL_CP.is_passed = false;
                DarkDL_CP.bin = manage.param_edge_ROI_DL_.bin_dl_;
                DarkDL_CP.grade = manage.param_edge_ROI_DL_.grade_dl_;
            }

        }
        g_str_msg_all[site_]+=boost::str(boost::format("Center_Mean:%.4f\n")
                                        % mean_center);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|")
                                        % mean_center);

        /*
        for(int i = 0;i!=gride_size;++i)
        {
            g_str_msg_all[site_]+=boost::str(boost::format("UP_Roi_%d_mean:%.4f\n")
                                            % i % up_edge_rois_mean.at(i));
            g_str_msg_all[site_]+=boost::str(boost::format("DOWN_Roi_%d_mean:%.4f\n")
                                            % i % down_edge_rois_mean.at(i));
            g_str_msg_all[site_]+=boost::str(boost::format("LEFT_Roi_%d_mean:%.4f\n")
                                            % i % left_edge_rois_mean.at(i));
            g_str_msg_all[site_]+=boost::str(boost::format("RIGHT_Roi_%d_mean:%.4f\n")
                                            % i % right_edge_rois_mean.at(i));

        }
        */

        for(int i = 0;i!=gride_size;++i)
        {
           g_str_msg_all[site_]+=boost::str(boost::format("UP_Roi_%d_center_mean_dif:%.4f\n")
                                            % i % up_edge_rois_mean_dif.at(i));

           g_str_log_all[site_]+=boost::str(boost::format("%.4f|")
                                            % up_edge_rois_mean_dif.at(i));

        }
        for(int i = 0;i!=gride_size;++i)
        {
           g_str_msg_all[site_]+=boost::str(boost::format("DOWN_Roi_%d_center_mean_dif:%.4f\n")
                                            % i % down_edge_rois_mean_dif.at(i));

           g_str_log_all[site_]+=boost::str(boost::format("%.4f|")
                                            % down_edge_rois_mean_dif.at(i));

        }
        for(int i = 0;i!=gride_size;++i)
        {
           g_str_msg_all[site_]+=boost::str(boost::format("LEFT_Roi_%d_center_mean_dif:%.4f\n")
                                            % i % left_edge_rois_mean_dif.at(i));

           g_str_log_all[site_]+=boost::str(boost::format("%.4f|")
                                            % left_edge_rois_mean_dif.at(i));

        }
        for(int i = 0;i!=gride_size;++i)
        {
           g_str_msg_all[site_]+=boost::str(boost::format("RIGHT_Roi_%d_center_mean_dif:%.4f\n")
                                            % i % right_edge_rois_mean_dif.at(i));

           g_str_log_all[site_]+=boost::str(boost::format("%.4f|")
                                            % right_edge_rois_mean_dif.at(i));

        }
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % DarkDL_CP.name.c_str());

        g_str_msg_all[site_]+=boost::str(boost::format("SPEC:\n "
                                                       "spec_n_:%.4f ~ spec_p_:%.4f\n")
                                         % manage.param_edge_ROI_DL_.spec_n_ % manage.param_edge_ROI_DL_.spec_p_ );

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
    {
        DarkDL_CP.g_msg = false;
    }


    return DarkDL_CP;
}

Subitem_Result DarkTest::PD_DataCollect(const ParamManage& manage)
{
    Subitem_Result res("PD_DataCollect",true);

    int w=manage.width_;
    int h=manage.height_;

    if(manage.pd_collect.open_mode_ == 1)
    {
        g_str_msg_all[site_]+=boost::str(boost::format("--------------------------PD 1X2 or 2X1 DataCollect-----------------------------\n"));

        std::string g_str_msg_;
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

        Spec_Mode spec_mode_=DN;

        diff_blemish_spec_r_= 0.5;
        diff_blemish_spec_gr_= 0.5;
        diff_blemish_spec_gb_ = 0.5;
        diff_blemish_spec_b_= 0.5;

        diff_spec_r_= manage.pd_collect.DN_diff_spec_level1_;
        diff_spec_gr_= manage.pd_collect.DN_diff_spec_level1_;
        diff_spec_gb_ = manage.pd_collect.DN_diff_spec_level1_;
        diff_spec_b_= manage.pd_collect.DN_diff_spec_level1_;


        TestItem::find_pixel_defect(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_, Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
                                    Algorithm_all[site_].g_buf_delta_r_, Algorithm_all[site_].g_buf_delta_gr_, Algorithm_all[site_].g_buf_delta_gb_, Algorithm_all[site_].g_buf_delta_b_, Algorithm_all[site_].g_buf_delta_raw_,
                                    Algorithm_all[site_].g_binary_r_, Algorithm_all[site_].g_binary_gr_, Algorithm_all[site_].g_binary_gb_, Algorithm_all[site_].g_binary_b_, Algorithm_all[site_].g_binary_,
                                    Algorithm_all[site_].g_buf_blemish_r_, Algorithm_all[site_].g_buf_blemish_gr_, Algorithm_all[site_].g_buf_blemish_gb_,Algorithm_all[site_]. g_buf_blemish_b_, Algorithm_all[site_].g_buf_blemish_raw_,
                                    w, h, manage.pd_collect.cnt_skip_, manage.pd_collect.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,1000,1000,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,Algorithm_all[site_].g_buf_tmp_,
                                    0, g_str_msg_);

        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;

        g_str_msg_all[site_]+=boost::str(boost::format("Spec:%d, "
                                             "cnt_1x2_ch:%d, "
                                             "cnt_2x1_ch:%d, "
                                             "cnt_1x2_2x1_pix:%d, "
                                             "cnt_white_pix:%d, cnt_dark_pix:%d\n")
                               % manage.pd_collect.DN_diff_spec_level1_
                               % cnt_1x2_ch
                               % cnt_2x1_ch
                               % cnt_1x2_2x1_pix
                               % cnt_white_pix
                               % cnt_dark_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|")
                                         % cnt_1x2_ch % cnt_2x1_ch  % cnt_1x2_2x1_pix% cnt_white_pix  % cnt_dark_pix);


        diff_spec_r_= manage.pd_collect.DN_diff_spec_level2_;
        diff_spec_gr_= manage.pd_collect.DN_diff_spec_level2_;
        diff_spec_gb_ = manage.pd_collect.DN_diff_spec_level2_;
        diff_spec_b_= manage.pd_collect.DN_diff_spec_level2_;


        TestItem::find_pixel_defect(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_, Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
                                    Algorithm_all[site_].g_buf_delta_r_, Algorithm_all[site_].g_buf_delta_gr_, Algorithm_all[site_].g_buf_delta_gb_, Algorithm_all[site_].g_buf_delta_b_, Algorithm_all[site_].g_buf_delta_raw_,
                                    Algorithm_all[site_].g_binary_r_, Algorithm_all[site_].g_binary_gr_, Algorithm_all[site_].g_binary_gb_, Algorithm_all[site_].g_binary_b_, Algorithm_all[site_].g_binary_,
                                    Algorithm_all[site_].g_buf_blemish_r_, Algorithm_all[site_].g_buf_blemish_gr_, Algorithm_all[site_].g_buf_blemish_gb_,Algorithm_all[site_]. g_buf_blemish_b_, Algorithm_all[site_].g_buf_blemish_raw_,
                                    w, h, manage.pd_collect.cnt_skip_, manage.pd_collect.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,1000,1000,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,Algorithm_all[site_].g_buf_tmp_,
                                    0, g_str_msg_);

        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;

        g_str_msg_all[site_]+=boost::str(boost::format("Spec:%d, "
                                             "cnt_1x2_ch:%d, "
                                             "cnt_2x1_ch:%d, "
                                             "cnt_1x2_2x1_pix:%d, "
                                             "cnt_white_pix:%d, cnt_dark_pix:%d\n")
                               % manage.pd_collect.DN_diff_spec_level2_
                               % cnt_1x2_ch
                               % cnt_2x1_ch
                               % cnt_1x2_2x1_pix
                               % cnt_white_pix
                               % cnt_dark_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|")
                                         % cnt_1x2_ch % cnt_2x1_ch  % cnt_1x2_2x1_pix% cnt_white_pix  % cnt_dark_pix);


        diff_spec_r_= manage.pd_collect.DN_diff_spec_level3_;
        diff_spec_gr_= manage.pd_collect.DN_diff_spec_level3_;
        diff_spec_gb_ = manage.pd_collect.DN_diff_spec_level3_;
        diff_spec_b_= manage.pd_collect.DN_diff_spec_level3_;


        TestItem::find_pixel_defect(Algorithm_all[site_].g_buf_r_, Algorithm_all[site_].g_buf_gr_, Algorithm_all[site_].g_buf_gb_, Algorithm_all[site_].g_buf_b_,
                                    Algorithm_all[site_].g_buf_delta_r_, Algorithm_all[site_].g_buf_delta_gr_, Algorithm_all[site_].g_buf_delta_gb_, Algorithm_all[site_].g_buf_delta_b_, Algorithm_all[site_].g_buf_delta_raw_,
                                    Algorithm_all[site_].g_binary_r_, Algorithm_all[site_].g_binary_gr_, Algorithm_all[site_].g_binary_gb_, Algorithm_all[site_].g_binary_b_, Algorithm_all[site_].g_binary_,
                                    Algorithm_all[site_].g_buf_blemish_r_, Algorithm_all[site_].g_buf_blemish_gr_, Algorithm_all[site_].g_buf_blemish_gb_,Algorithm_all[site_]. g_buf_blemish_b_, Algorithm_all[site_].g_buf_blemish_raw_,
                                    w, h, manage.pd_collect.cnt_skip_, manage.pd_collect.r_area_,
                                    diff_spec_r_,diff_spec_gr_,diff_spec_gb_,diff_spec_b_,
                                    diff_blemish_spec_r_,diff_blemish_spec_gr_,diff_blemish_spec_gb_,diff_blemish_spec_b_,
                                    spec_mode_, manage.bayer_,cnts_cluster, cnt_2x4_pix, cnt_2x2_pix,
                                    cnt_1x2_2x1_pix, cnt_2x2_diagonal_pix, cnt_1x2_ch,
                                    cnt_2x1_ch, cnt_2x2_leftdiagonal_ch, cnt_2x2_rightdiagonal_ch,
                                    cnt_white_pix, cnt_dark_pix, paticle, other_cluster,
                                    other_cluster_ch, total_cluster_cnt,1000,1000,
                                    blemish_cnt,cnt_white_pix_blemish,cnt_dark_pix_blemish,Algorithm_all[site_].g_buf_tmp_,
                                    0, g_str_msg_);

        cnt_1x2_2x1_pix = cnt_1x2_2x1_pix/2;

        g_str_msg_all[site_]+=boost::str(boost::format("Spec:%d, "
                                             "cnt_1x2_ch:%d, "
                                             "cnt_2x1_ch:%d, "
                                             "cnt_1x2_2x1_pix:%d, "
                                             "cnt_white_pix:%d, cnt_dark_pix:%d\n")
                               % manage.pd_collect.DN_diff_spec_level3_
                               % cnt_1x2_ch
                               % cnt_2x1_ch
                               % cnt_1x2_2x1_pix
                               % cnt_white_pix
                               % cnt_dark_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%d|%d|%d|%d|%d|")
                                         % cnt_1x2_ch % cnt_2x1_ch  % cnt_1x2_2x1_pix% cnt_white_pix  % cnt_dark_pix);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        Algorithm_all[site_].time_end=clock();
        g_str_msg_all[site_]+="\n cost_time:"+QString::number(Algorithm_all[site_].time_end-Algorithm_all[site_].time_begin).toStdString()+"\n";

    }
    else
        res.g_msg = false;


    return res;
}








