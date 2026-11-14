#ifndef PARAMMANAGE_H
#define PARAMMANAGE_H

#include <string>
#include <windows.h>
#include <fstream>
#include <QMap>
#include <QPair>
#include <QPoint>
#include <QString>
#include <QVector>
#include <QDomDocument>

void logMsg(const std::string & msg,size_t site_=0,int grade=2);

enum RGB_BAYER{RG,BG,GR,GB};



struct MRect{
    int x_;
    int y_;
    int w_;
    int h_;
};


struct Param_Binary
{
    int skip;
    double dark_binary_spec_r_;
    double dark_binary_spec_gr_;
    double dark_binary_spec_gb_;
    double dark_binary_spec_b_;
    int dark_binary_radius_;

    double bright_binary_spec_r_;
    double bright_binary_spec_gr_;
    double bright_binary_spec_gb_;
    double bright_binary_spec_b_;
    int bright_binary_radius_;

    double flicker_pix_binary_spec_;

    int burnable_typeNum;
    int type1_or_type11_;

    int skip_row_top_;
    int skip_row_bottom_;
    int skip_col_left_;
    int skip_col_right_;
};


struct Param_Dark_Ref
{
    double col_ref_MAX_max;
    double col_ref_MAX_min;
    double col_ref_MIN_max;
    double col_ref_MIN_min;

    double Ref_Split_screen_ROI_dif;

    int bin_ref_col;
    int grade_ref_col;

    int bin_ref_Split_screen;
    int grade_ref_Split_screen;

    int ref_open_mode_;

    int log_align_;

};


struct Param_Darkcurrent
{
    int skip_;
    int dark_area_line_total_cnt_;
    int dark_area_line_center_cnt_;
    int roi_w_;
    int roi_h_;

    double DC_dark_area_min_;
    double DC_dark_area_max_;

    double DC_leftup_min_;
    double DC_leftup_max_;

    double DC_rightup_min_;
    double DC_rightup_max_;

    double DC_leftdown_min_;
    double DC_leftdown_max_;

    double DC_rightdown_min_;
    double DC_rightdown_max_;

    double DC_center_min_;
    double DC_center_max_;

    double DC_whole_area_min_;
    double DC_whole_area_max_;

    int bin_darkcurrent_;
    int grade_darkcurrent_;
};


struct Param_Darkcurrent_Global
{
    int skip_x_;
    int skip_y_;
    int roi_w_;
    int roi_h_;
    int dark_height_;

    QVector <double> roi_dkc_spec_min_;
    QVector <double> roi_dkc_spec_max_;

    int bin_darkcurrent_;
    int grade_darkcurrent_;
};


struct Param_Colorbar
{
    int w;
    int h;
    double dif_min_pattern_;
    double dif_ratio_pattern_;
    int limit_cnt_pattern;
    std::vector<MRect> areas_pattern_;
    int bin_;
    int grade_;
};



struct Param_POG_Normal_Bright
{
    double spec_p_;
    double spec_n_;
    int r1_;
    int r2_;

    int crop_;
    double scratch_p_;
    double scratch_n_;
    int limit_size_;


    double lsc_ratio_;
    int resize_w_;
    int resize_h_;

    int miscount_;

    int bin_pog_;
    int grade_pog_;

    int blemish_skip_;
    int r_kernel_;
    int r_outer_;
    double blemish_spec_;

    int save_image_;
    int open_mode_;
    int log_align_;

};


struct Param_ImageDirtDetect_Bright
{
    int pre_blur_size_;
    int img_resize_;
    float lsc_ratio_;


    int gfda_blur_r1_;
    int gfda_blur_r2_;
    float gfda_pog_spec_p_;
    float gfda_pog_spec_n_;
    float gfda_scratch_spec_p_;
    float gfda_spot_spec_p_;
    float gfda_spot_spec_n_;
    int gfda_spot_limit_size_;


    float leda_ratio_p_;
    float leda_ratio_n_;
    float leda_spot_spec_p_;
    int leda_spot_limit_size_;

    int ldma_stain_skip_;
    int ldma_stain_r_kernel_;
    int ldma_stain_r_outer_;
    float ldma_stain_spec_;


    int pog_miscount_;
    int bin_pog_;
    int grade_pog_;

    int scratch_miscount_;
    int bin_scratch_;
    int grade_scratch_;

    int spot_miscount_;
    int bin_spot_;
    int grade_spot_;

    int stain_miscount_;
    int bin_stain_;
    int grade_stain_;

    int darkbar_miscount_;
    int bin_darkbar_;
    int grade_darkbar_;

    int save_image_;
    int open_mode_;
    int log_align_;
};


struct Param_POG
{
    double spec_p_;
    double spec_n_;
    int r1_;
    int r2_;
    int miscount_;

    int resize_w_;
    int resize_h_;

    int bin_pog_;
    int grade_pog_;

    int save_image_;
    int open_mode_;
    int log_align_;

};


struct MPkgParam
{
    double threshold_sobel_;
    int limit_size_;
    int skip_;
    int code_;
    int code_white_;
    struct mottle
    {
        int code;
        int count;
        mottle():code(0),count(999999){}
    }
    MTottleParam;
    int grade_;
    int open_mode_;

    //PKG中的pd 单点卡控
    double ratio_diff_spec_r_;
    double ratio_diff_spec_gr_;
    double ratio_diff_spec_gb_;
    double ratio_diff_spec_b_;


};


struct MBlemishParam
{
    int winSmall;
    int winBig;
    double threshold_center_;
    double threshold_margin_;
    int limit_size_;
    int max_size_;
    int skip_;
    int code_dark_;
    int grade_;

   // cv::Rect center;
    int x_;
    int y_;
    int w_;
    int h_;
    int skip_rect;


    int open_mode_;
    int log_align_;


    //V1_1
    double lsc_ratio_;
    int resize_w_;
    int resize_h_;

    //---------enhace------------------
    int magnification;
    int winSmall_enhace;
    int winBig_enhace;
    double threshold_center_enhace;
    double threshold_margin_enhace;
    int limit_size_enhace;
    int max_size_enhace;
    int skip_enhace;
    int skip_rect_enhace;

   // cv::Rect center;
    int x_enhace;
    int y_enhace;
    int w_enhace;
    int h_enhace;

    double lsc_ratio_enhace;
    int resize_w_enhace;
    int resize_h_enhace;


    //--------搜索区域----------
    int search_area_x;
    int search_area_y;

    int min_enhace_size;
    int max_enhace_size;

    //-------------------------
    //Blemish中的pd卡控
    double ratio_diff_spec_r_;
    double ratio_diff_spec_gr_;
    double ratio_diff_spec_gb_;
    double ratio_diff_spec_b_;


    double DN_diff_spec_r_;
    double DN_diff_spec_gr_;
    double DN_diff_spec_gb_;
    double DN_diff_spec_b_;

    int pd_area_x_;
    int pd_area_y_;

    int cnt_skip_;
    int r_area_;
    int save_image_;
    int pd_single_open_mode_;


};


struct MClusterInf
{
    int x_;
    int y_;
    int w_;
    int h_;
    int number;
    int code_;
    MClusterInf():x_(0),y_(0),w_(0),h_(0),number(999),code_(0){}
    MClusterInf(int a,int b,int c,int d):\
        x_(a),y_(b),w_(c),h_(d),number(999),code_(0){}
    MClusterInf(int a,int b,int c,int d,int e,int f):
        x_(a),y_(b),w_(c),h_(d),number(e),code_(f){}
};


struct ParamHistogram
{
//    int DN_spec_min_;
//    int DN_spec_max_;
//    int Cnt_spec_min_;
//    int Cnt_spec_max_;

    int bin_;
    int grade_;


    QVector <int> Dn_stat;
    QVector <int> Dn_end;
    QVector <int> Cnt_spec_min_;
    QVector <int> Cnt_spec_max_;

    int Cnt_spec_all_;

    int open_mode_;
};


struct ParamRowNoise
{
    double row_noise_spec;
    int bin_;
    int grade_;
    int open_mode_;
};


struct ParamColumnNoise
{
    double column_noise_spec;
    double column_noise_spec_level_1_;
    double column_noise_spec_level_2_;
    int bin_;
    int bin_level_1_;
    int bin_level_2_;
    int grade_;
    int grade_level_1_;
    int grade_level_2_;
    int open_mode_;
};


struct Param_Color_Shading
{
    int skip;
    int ROI_X_Cnt;
    int ROI_Y_Cnt;

    double factor_center_class1;      //V1_0
    double factor_center_class2;      //V1_0

    double factor_center_rg_class1;   //V1_1
    double factor_center_rg_class2;   //V1_1
    double factor_center_bg_class1;   //V1_1
    double factor_center_bg_class2;   //V1_1

    int bin_shanding;
    int grade_shanding;
    int bin_shanding_MA;
    int grade_shanding_MA;

    int open_mode_;
    int log_align_;
};


struct Param_Color_Shading_5x5
{
    int skip;
    int ROI_X_Cnt;
    int ROI_Y_Cnt;

    double rg1_min;
    double bg1_min;
    double rg2_min;
    double bg2_min;
    double rg3_min;
    double bg3_min;


    double rg1_max;
    double bg1_max;
    double rg2_max;
    double bg2_max;
    double rg3_max;
    double bg3_max;




    int bin_shanding;
    int grade_shanding;


    int open_mode_;
    int log_align_;
};

struct Param_Color_Shading_15x15
{
    int skip;
    int ROI_X_Cnt;
    int ROI_Y_Cnt;

    double max_diff_corner;
    double max_diff_edge;

    int bin_shanding;
    int grade_shanding;
    int bin_shanding_MA;
    int grade_shanding_MA;

    int open_mode_;
    int log_align_;
};


struct Param_RelativeIllum
{
    int w_win_;
    int h_win_;
    double limit_min_ratio_;
    double limit_max_ratio_;
    int bin_;
    int grade_;
    int open_mode_;
    int log_align_;

};


struct Param_Ratio_Fourier{

    int ROI_X_Cnt;
    int ROI_Y_Cnt;

    double rg_top_left_min_;
    double rg_top_left_max_;
    double rg_top_right_min_;
    double rg_top_right_max_;

    double rg_btm_left_min_;
    double rg_btm_left_max_;
    double rg_btm_right_min_;
    double rg_btm_right_max_;

    double bg_top_left_min_;
    double bg_top_left_max_;
    double bg_top_right_min_;
    double bg_top_right_max_;

    double bg_btm_left_min_;
    double bg_btm_left_max_;
    double bg_btm_right_min_;
    double bg_btm_right_max_;

    int bin_;
    int grade_;

    int save_image_;
    int open_mode_;
    int log_align_;

};


struct Param_Pixel_Defect
{
    int cnt_skip_;
    int r_area_;
    double DN_diff_spec_r_;
    double DN_diff_spec_gr_;
    double DN_diff_spec_gb_;
    double DN_diff_spec_b_;
    double ratio_diff_spec_r_;
    double ratio_diff_spec_gr_;
    double ratio_diff_spec_gb_;
    double ratio_diff_spec_b_;
    int limit_size_;
    int bin1_;
    int grade1_;
    int limit_cnt1_;
    int bin2_;
    int grade2_;
    int limit_cnt2_;
    int bin3_;
    int grade3_;
    // --for 1x2 or 2x1 cluster in channel
    int bin1x2_2x1_ch_;
    int grade7_;
    // --for 2x2 diagonal cluster in channel
    int bin2x2_diagonal_ch_;
    int grade8_;
    // --for 2x4 cluster in global--
    int bin2x4_;
    int grade4_;
    int bin1x2_2x1_;
    int grade5_;
    int bin2x2_diagonal_;
    int grade6_;
    // -------------------
    // --for 2x2 cluster in global
    int bin2x2_global_;
    int grade9_;
    // -------------------
    int bin_single_pix_;
    int grade10_;
    int single_pix_limit_;
    //--------------------
    int limit_wp1_;
    int limit_wp2_;
    int limit_wp3_;
    int limit_dp1_;
    int limit_dp2_;
    int limit_dp3_;

    int bin_limit_wp1_;
    int bin_limit_wp2_;
    int bin_limit_wp3_;
    int bin_limit_dp1_;
    int bin_limit_dp2_;
    int bin_limit_dp3_;

    int grade_limit_wp1_;
    int grade_limit_wp2_;
    int grade_limit_wp3_;
    int grade_limit_dp1_;
    int grade_limit_dp2_;
    int grade_limit_dp3_;

    int all_1x2_2x1_spec;


    double blemish_spec_r_;
    double blemish_spec_gr_;
    double blemish_spec_gb_;
    double blemish_spec_b_;
    double blemish_dn_spec_;

    int blemish_global_size_limit_;
    int blemish_bychannel_size_limit_;
    int bin_blemish_;
    int grade_blemish_;
    int blemish_limit_wp1_;
    int blemish_limit_wp2_;
    int blemish_limit_wp3_;
    int blemish_limit_dp1_;
    int blemish_limit_dp2_;
    int blemish_limit_dp3_;


    int save_image_;
    int open_mode_;
    int log_align_;

    int type_;
    int single_pix_type_;

    //-------新增gray_PD 使用
    int single_pix_dark_limit_r;
    int single_pix_dark_limit_gr;
    int single_pix_dark_limit_gb;
    int single_pix_dark_limit_b;

    int bin_single_pix_dark_channel;
    int grade_single_pix_dark_channel;

    int  single_pix_bright_limit_r;
    int  single_pix_bright_limit_gr;
    int  single_pix_bright_limit_gb;
    int  single_pix_bright_limit_b;

    int bin_single_pix_bright_channel;
    int grade_single_pix_bright_channel;

    int ROI_Channel_X;
    int ROI_Channel_Y;
    int ROI_Channel_W;
    int ROI_Channel_H;

    int crop_single_pix_dark_limit_r;
    int crop_single_pix_dark_limit_gr;
    int crop_single_pix_dark_limit_gb;
    int crop_single_pix_dark_limit_b;

    int bin_crop_single_pix_dark_channel;
    int grade_crop_single_pix_dark_channel;


    int crop_single_pix_dark_limit_gr_gb;
    int bin_crop_single_pix_dark_channel_gr_gb;
    int grade_crop_single_pix_dark_channel_gr_gb;


    int crop_single_pix_bright_limit_r;
    int crop_single_pix_bright_limit_gr;
    int crop_single_pix_bright_limit_gb;
    int crop_single_pix_bright_limit_b;

    int bin_crop_single_pix_bright_channel;
    int grade_crop_single_pix_bright_channel;

    int crop_single_pix_bright_limit_gr_gb;
    int bin_crop_single_pix_bright_channel_gr_gb;
    int grade_crop_single_pix_bright_channel_gr_gb;

    int grade_single_pix;
    int single_pix_limit_Class1;
    int bin_single_pix_Class1;
    int single_pix_limit_Class2;
    int bin_single_pix_Class2;
    int single_pix_limit_Class3;
    int bin_single_pix_Class3;
    int single_pix_limit_Class4;
    int bin_single_pix_Class4;
    int single_pix_limit_Class5;
    int bin_single_pix_Class5;

    QString only_bright_dark_pix_mode;
    //--------


};

struct Param_PD_collect
{
    int cnt_skip_;
    int r_area_;
    double DN_diff_spec_level1_;
    double DN_diff_spec_level2_;
    double DN_diff_spec_level3_;

    int single_pix_limit_;
    int all_1x2_2x1_spec;

    int save_image_;
    int open_mode_;
    int log_align_;
    int type_;
    int single_pix_type_;
};

struct ParamBlemish
{
    double spec_r_;
    double spec_gr_;
    double spec_gb_;
    double spec_b_;
    int bin_;
    int grade_;
};


struct Param_Real_ROI_PRNU
{
    int X_ROI;
    int Y_ROI;
    int W_ROI;
    int H_ROI;
    double spec_prnu_r_;
    double spec_prnu_gr_;
    double spec_prnu_gb_;
    double spec_prnu_b_;
    int bin_;
    int grade_;
    int open_mode_;
    int log_align_;
};


struct Param_Non_Uniformity
{
    int w_cell_;
    int h_cell_;

    double spec_n_uniform_r_;
    double spec_n_uniform_gr_;
    double spec_n_uniform_gb_;
    double spec_n_uniform_b_;

    double max_n_uniform_r_;
    double max_n_uniform_gr_;
    double max_n_uniform_gb_;
    double max_n_uniform_b_;

    double max_mirror_diff_n_uniform_r_;
    double max_mirror_diff_n_uniform_gr_;
    double max_mirror_diff_n_uniform_gb_;
    double max_mirror_diff_n_uniform_b_;

    double max_updown_diff_n_uniform_r_;
    double max_updown_diff_n_uniform_gr_;
    double max_updown_diff_n_uniform_gb_;
    double max_updown_diff_n_uniform_b_;

    int bin_mean_;
    int grade_mean_;

    int bin_max_;
    int grade_max_;

    int bin_mirror_max_;
    int grade_mirror_max_;

    int bin_updown_max_;
    int grade_updown_max_;

    int open_mode_;
    int log_print_;
    int log_align_;
};

struct ParamMosaic{
    double Mosaic_lines_cnt;
    int bin_;
    int grade_;
    int open_mode_;
};
struct Param_OB_DarkShading
{
    int skip_;
    int OB_region_r_;

    double ref;
    double Golden_dif;
    int bin_OB_Golden_;
    int grade_OB_Golden_;

    double min_diff_ref_class1_;
    double max_diff_ref_class1_;
    double min_diff_ref_class2_;
    double max_diff_ref_class2_;

    int bin_OB_class1_;
    int grade_OB_class1_;
    int bin_OB_class2_;
    int grade_OB_class2_;
    double ob_max_min_diff_;
    int bin_ob_max_min_diff_;
    int grade_ob_max_min_diff_;


    int x_cnt_;
    int y_cnt_;
    int x_cnt_diff_;
    int y_cnt_diff_;

    double r_DarkShading_min_;
    double r_DarkShading_mid_;
    double r_DarkShading_max_;
    double gr_DarkShading_min_;
    double gr_DarkShading_mid_;
    double gr_DarkShading_max_;
    double gb_DarkShading_min_;
    double gb_DarkShading_mid_;
    double gb_DarkShading_max_;
    double b_DarkShading_min_;
    double b_DarkShading_mid_;
    double b_DarkShading_max_;
    double max_min_DarkShading_diff_;


    int bin_DarkShading_MA;
    int grade_DarkShading_MA;

    double ROI_GRGB_OB_DIF_MAX_Level1;
    double ROI_GRGB_OB_DIF_MAX_Level2;
    int bin_OB_Gr_Gb_Diff_Class1;
    int bin_OB_Gr_Gb_Diff_Class2;
    int grade_OB_Gr_Gb_Diff_Class1;
    int grade_OB_Gr_Gb_Diff_Class2;

    int bin_DarkShading_;
    int grade_DarkShading_;

    int open_mode_;
    int log_align_;

    //V1_1
    double min_diff_center_Class1_;
    double max_diff_center_Class2_;

    //V1_4
    int roi_x_;
    int roi_y_;
    int roi_w_;
    int roi_h_;

    int min_Mean_;
    int max_Mean_;

    int bin_ob_mean;
    int grade_ob_mean;
};


struct Param_OB_GR_GB
{
    int skip_;
    int OB_region_r_;
    double min_ob_Gr_Gb_diff_;
    double max_ob_Gr_Gb_diff_;
    int bin_OB_Gr_Gb_diff_class1_;
    int grade_OB_Gr_Gb_diff_class1_;
    int bin_OB_Gr_Gb_diff_class2_;
    int grade_OB_Gr_Gb_diff_class2_;
    int ref;
    int open_mode_;
    int log_align_;
};



struct Param_DAG
{
    int roi_col_;
    int roi_row_;
    int val_;

    int bin_dag_;
    int grade_dag_;

    int dag_open_mode_;
    int log_align_;
};


struct Param_Light_Linearity
{
    double linear_spec_;
    double max_dn;
    int roi_x_;
    int roi_y_;
    int roi_w_;
    int roi_h_;

    int bin_linearity_;
    int grade_linearity_;

    int linearity_open_mode_;
    int log_align_;
};

struct Param_DAG_StdMean
{
    double std_;
    int bin_dag_mean_;
    int grade_dag_mean_;
    int open_mode_;
    int log_align_;
};


struct Param_Light_Response
{
    double limit_r_min_;
    double limit_r_max_;
    double limit_gr_min_;
    double limit_gr_max_;
    double limit_gb_min_;
    double limit_gb_max_;
    double limit_b_min_;
    double limit_b_max_;

    int roi_x_;
    int roi_y_;
    int roi_w_;
    int roi_h_;

    int bin_response_;
    int grade_response_;

    int response_open_mode_;

    int log_align_;

    double error_min;
    double error_max;
    bool error_min_open;
    bool error_max_open;
};


struct Param_Read_Noise
{
    int roi_x_;
    int roi_y_;
    int roi_w_;
    int roi_h_;
    int open_mode_;

};


struct Param_GI_Ratio
{
    int skip;
    int ROI_X_Cnt;
    int ROI_Y_Cnt;
    double GI_Ratio;
    int bin_GI_;
    int grade_GI_;
    int ratio_open_mode_;
    int log_align_;

    //V1_1
    double GI_Ratio_class1;
    double GI_Ratio_class2;
    int bin_GI_class1;
    int grade_GI_class1;
    int bin_GI_class2;
    int grade_GI_class2;

};

struct Param_GI_Ratio_5x5
{
    int skip;
    int ROI_X_Cnt;
    int ROI_Y_Cnt;
    double TargetGI_Leftup;
    double TargetGI_Rightup;
    double TargetGI_Leftdown;
    double TargetGI_Rightdown;
    double TargetGI_Center;
    int bin_GI_5x5;
    int grade_GI_5x5;
    int ratio_open_mode_;
    int log_align_;
};


struct Param_GI_Ratio_V1_1
{
    int skip;
    int ROI_X_Cnt;
    int ROI_Y_Cnt;

    double GI_Ratio_class1;
    double GI_Ratio_class2;
    int bin_GI_class1;
    int grade_GI_class1;
    int bin_GI_class2;
    int grade_GI_class2;

    int ratio_open_mode_;
    int log_align_;
};


struct Param_Color_Ratio
{
    int skip;
    int ROI_X_Cnt;
    int ROI_Y_Cnt;

    double factor_center_gr_gb_ratio;

    double factor_center_class1;
    double factor_center_class2;

    double factor_corner_class1;
    double factor_corner_class2;

    double factor_golden;

    double factor_limit;

    QMap <QString,double> GG_Ratio_target;
    QMap <QString,double> RG_Ratio_target;
    QMap <QString,double> BG_Ratio_target;

    QMap <QString,double> GG_Ratio_target_offset;
    QMap <QString,double> RG_Ratio_target_offset;
    QMap <QString,double> BG_Ratio_target_offset;

    QMap <QString,double> GG_Ratio_target_offset_ratio;
    QMap <QString,double> RG_Ratio_target_offset_ratio;
    QMap <QString,double> BG_Ratio_target_offset_ratio;

    int bin_color_ratio_;
    int grade_color_ratio_;
    int bin_color_ratio_MA_;
    int grade_color_ratio_MA_;
    int bin_color_ratio_golden_;
    int grade_color_ratio_golden_;
    int bin_color_ratio_corner_rg_max_;
    int grade_color_ratio_corner_rg_max_;
    int bin_color_ratio_corner_rg_min_;
    int grade_color_ratio_corner_rg_min_;
    int bin_color_ratio_corner_bg_max_;
    int grade_color_ratio_corner_bg_max_;
    int bin_color_ratio_corner_bg_min_;
    int grade_color_ratio_corner_bg_min_;

    double gr_gb_diff_spec;
    int bin_GI_;
    int grade_GI_;

    int ratio_open_mode_;
    int log_align_;
};


struct Param_Line_Defect_bright
{
    int strip_row_;
    int strip_col_;

    double deadrow_spec_r_TA_;
    double deadrow_spec_gr_TA_;
    double deadrow_spec_gb_TA_;
    double deadrow_spec_b_TA_;

    double deadrow_spec_r_MA_;
    double deadrow_spec_gr_MA_;
    double deadrow_spec_gb_MA_;
    double deadrow_spec_b_MA_;


    double deadcol_spec_r_TA_;
    double deadcol_spec_gr_TA_;
    double deadcol_spec_gb_TA_;
    double deadcol_spec_b_TA_;

    double deadcol_spec_r_MA_;
    double deadcol_spec_gr_MA_;
    double deadcol_spec_gb_MA_;
    double deadcol_spec_b_MA_;


    double spec_row_dol_r_;
    double spec_row_dol_gr_;
    double spec_row_dol_gb_;
    double spec_row_dol_b_;

    double spec_col_dol_r_;
    double spec_col_dol_gr_;
    double spec_col_dol_gb_;
    double spec_col_dol_b_;


    int r_area_;
    int bin_dead_row_;
    int bin_dead_col_;
    int bin_dead_row_MA_;
    int bin_dead_col_MA_;
    int bin_dol_;
    int grade_dead_row_;
    int grade_dead_col_;
    int grade_dead_row_MA;
    int grade_dead_col_MA;
    int grade_dol_;

    double breakrow_spec_r_;
    double breakrow_spec_gr_;
    double breakrow_spec_gb_;
    double breakrow_spec_b_;

    double breakcol_spec_r_;
    double breakcol_spec_gr_;
    double breakcol_spec_gb_;
    double breakcol_spec_b_;


    double breakrow_spec_r_level_1;
    double breakrow_spec_gr_level_1;
    double breakrow_spec_gb_level_1;
    double breakrow_spec_b_level_1;

    double breakcol_spec_r_level_1;
    double breakcol_spec_gr_level_1;
    double breakcol_spec_gb_level_1;
    double breakcol_spec_b_level_1;

    double breakrow_spec_r_level_2;
    double breakrow_spec_gr_level_2;
    double breakrow_spec_gb_level_2;
    double breakrow_spec_b_level_2;

    double breakcol_spec_r_level_2;
    double breakcol_spec_gr_level_2;
    double breakcol_spec_gb_level_2;
    double breakcol_spec_b_level_2;

    int bin_break_line_;
    int grade_break_line_;

    int bin_break_line_level1;
    int grade_break_line_level1;
    int bin_break_line_level2;
    int grade_break_line_level2;

    int log_print_;
    int open_mode_;
    int log_align_;
};


struct Param_Line_Defect_bright_iic
{
    int strip_row_;
    int strip_col_;

    double deadrow_spec_r_TA_;
    double deadrow_spec_gr_TA_;
    double deadrow_spec_gb_TA_;
    double deadrow_spec_b_TA_;

    double deadrow_spec_r_MA_;
    double deadrow_spec_gr_MA_;
    double deadrow_spec_gb_MA_;
    double deadrow_spec_b_MA_;


    double deadcol_spec_r_TA_;
    double deadcol_spec_gr_TA_;
    double deadcol_spec_gb_TA_;
    double deadcol_spec_b_TA_;

    double deadcol_spec_r_MA_;
    double deadcol_spec_gr_MA_;
    double deadcol_spec_gb_MA_;
    double deadcol_spec_b_MA_;


    double spec_row_dol_r_;
    double spec_row_dol_gr_;
    double spec_row_dol_gb_;
    double spec_row_dol_b_;

    double spec_col_dol_r_;
    double spec_col_dol_gr_;
    double spec_col_dol_gb_;
    double spec_col_dol_b_;


    int r_area_;
    int bin_dead_row_;
    int bin_dead_col_;
    int bin_dead_row_MA_;
    int bin_dead_col_MA_;
    int bin_dol_;
    int grade_dead_row_;
    int grade_dead_col_;
    int grade_dead_row_MA;
    int grade_dead_col_MA;
    int grade_dol_;

    double breakrow_spec_r_;
    double breakrow_spec_gr_;
    double breakrow_spec_gb_;
    double breakrow_spec_b_;

    double breakcol_spec_r_;
    double breakcol_spec_gr_;
    double breakcol_spec_gb_;
    double breakcol_spec_b_;

    int bin_break_line_;
    int grade_break_line_;

    int log_print_;
    int open_mode_;
    int log_align_;

};


struct Param_Edge_Line_Defect_bright
{
    int strip_row_;
    int strip_col_;

    double deadrow_spec_r_TA_;
    double deadrow_spec_gr_TA_;
    double deadrow_spec_gb_TA_;
    double deadrow_spec_b_TA_;

    double deadrow_spec_r_MA_;
    double deadrow_spec_gr_MA_;
    double deadrow_spec_gb_MA_;
    double deadrow_spec_b_MA_;


    double deadcol_spec_r_TA_;
    double deadcol_spec_gr_TA_;
    double deadcol_spec_gb_TA_;
    double deadcol_spec_b_TA_;

    double deadcol_spec_r_MA_;
    double deadcol_spec_gr_MA_;
    double deadcol_spec_gb_MA_;
    double deadcol_spec_b_MA_;


    double spec_row_dol_r_;
    double spec_row_dol_gr_;
    double spec_row_dol_gb_;
    double spec_row_dol_b_;

    double spec_col_dol_r_;
    double spec_col_dol_gr_;
    double spec_col_dol_gb_;
    double spec_col_dol_b_;


    int r_area_;
    int bin_dead_row_;
    int bin_dead_col_;
    int bin_dead_row_MA_;
    int bin_dead_col_MA_;
    int bin_dol_;
    int grade_dead_row_;
    int grade_dead_col_;
    int grade_dead_row_MA;
    int grade_dead_col_MA;
    int grade_dol_;

    int open_mode_;
    int log_align_;
};


struct Param_Optical_Center
{
    int spec_x_;
    int spec_y_;

    int bin_;
    int grade_;

    int open_mode_;
    int log_align_;

};


struct Param_ROI_STD
{
    int strip_col_;
    int strip_row_;
    int col_roi_cnt_;
    int row_roi_cnt_;

    double r_mean_min_spec_;
    double r_mean_max_spec_;
    double r_std_min_spec_;
    double r_std_max_spec_;

    double gr_mean_min_spec_;
    double gr_mean_max_spec_;
    double gr_std_min_spec_;
    double gr_std_max_spec_;

    double gb_mean_min_spec_;
    double gb_mean_max_spec_;
    double gb_std_min_spec_;
    double gb_std_max_spec_;

    double b_mean_min_spec_;
    double b_mean_max_spec_;
    double b_std_min_spec_;
    double b_std_max_spec_;

    int bin_;
    int grade_;

    int open_mode_;
    int log_align_;
};


struct Param_FPN_DL
{
    int strip_row_;
    int strip_col_;
    double fpn_level_1_spec_;
    double fpn_level_2_spec_;
    double deadcol_spec_;
    double deadrow_spec_;
    double spec_dol_;
    int r_area_;
    int bin_fpn_level_1_;
    int bin_fpn_level_2_;
    int bin_dead_row_;
    int bin_dead_col_;
    int bin_dol_;
    int grade_fpn_level_1_;
    int grade_fpn_level_2_;
    int grade_dead_row_;
    int grade_dead_col_;
    int grade_dol_;

    double break_row_spec_;
    double break_col_spec_;
    int bin_break_line_;
    int grade_break_line_;

    int log_print_;
    int open_mode_;
    int log_align_;


    //V1_1
    double deadcol_spec_level1;
    double deadrow_spec_level1;
    double deadcol_spec_level2;
    double deadrow_spec_level2;

    int bin_dead_row_level1;
    int bin_dead_col_level1;
    int bin_dead_row_level2;
    int bin_dead_col_level2;

    int grade_dead_row_level1;
    int grade_dead_col_level1;
    int grade_dead_row_level2;
    int grade_dead_col_level2;

};
//struct param_fpn_dl_
//{
//    int strip_row_;
//    int strip_col_;
//    double fpn_level_1_spec_;
//    double fpn_level_2_spec_;
//    double deadcol_spec_level1;
//    double deadrow_spec_level1;
//    double deadcol_spec_level2;
//    double deadrow_spec_level2;
//    double spec_dol_;
//    int r_area_;
//    int bin_fpn_level_1_;
//    int bin_fpn_level_2_;
//    int bin_dead_row_level1;
//    int bin_dead_col_level1;
//    int bin_dead_row_level2;
//    int bin_dead_col_level2;
//    int bin_dol_;
//    int grade_fpn_level_1_;
//    int grade_fpn_level_2_;
//    int grade_dead_row_level1;
//    int grade_dead_col_level1;
//    int grade_dead_row_level2;
//    int grade_dead_col_level2;
//    int grade_dol_;

//    double break_row_spec_;
//    double break_col_spec_;
//    int bin_break_line_;
//    int grade_break_line_;

//    int log_print_;
//    int open_mode_;
//    int log_align_;

//};


struct Param_FPN_DL_IIC
{
    int strip_row_;
    int strip_col_;
    double fpn_level_1_spec_;
    double fpn_level_2_spec_;
    double deadcol_spec_;
    double deadrow_spec_;
    double spec_dol_;
    int r_area_;
    int bin_fpn_level_1_;
    int bin_fpn_level_2_;
    int bin_dead_row_;
    int bin_dead_col_;
    int bin_dol_;
    int grade_fpn_level_1_;
    int grade_fpn_level_2_;
    int grade_dead_row_;
    int grade_dead_col_;
    int grade_dol_;

    double break_row_spec_;
    double break_col_spec_;
    int bin_break_line_;
    int grade_break_line_;

    int log_print_;
    int open_mode_;
    int log_align_;

};

struct Param_FPN_DL_Bright_Dark
{
    int strip_row_;
    int strip_col_;
    double fpn_level_1_spec_;
    double fpn_level_2_spec_;
    double deadcol_spec_;
    double deadrow_spec_;
    double deadcol_bright_spec_;
    double deadcol_bright_level_1_spec_;
    double deadcol_bright_level_2_spec_;
    double deadcol_dark_spec_;
    double deadcol_dark_level_1_spec_;
    double deadcol_dark_level_2_spec_;
    double spec_dol_;
    int r_area_;
    int bin_fpn_level_1_;
    int bin_fpn_level_2_;
    int bin_dead_row_;
    int bin_dead_col_;
    int bin_deadcol_bright_;
    int bin_deadcol_bright_level_1_;
    int bin_deadcol_bright_level_2_;
    int bin_deadcol_dark_;
    int bin_deadcol_dark_level_1_;
    int bin_deadcol_dark_level_2_;
    int bin_dol_;
    int grade_fpn_level_1_;
    int grade_fpn_level_2_;
    int grade_dead_row_;

    int grade_dead_col_;
    int grade_deadcol_bright_;
    int grade_deadcol_bright_level_1_;
    int grade_deadcol_bright_level_2_;
    int grade_deadcol_dark_;
    int grade_deadcol_dark_level_1_;
    int grade_deadcol_dark_level_2_;
    int grade_dol_;

    double break_row_spec_;
    double break_col_spec_;
    int bin_break_line_;
    int grade_break_line_;

    int log_print_;
    int open_mode_;
    int log_align_;

};


struct Param_Lag_Grid_Roi
{
    double lag_r_;
    double lag_gr_;
    double lag_gb_;
    double lag_b_;

    int skip_x_;
    int skip_y_;
    int X_Grid_Cnt_;
    int Y_Grid_Cnt_;

    int row_up_;
    int row_down_;
    int col_left_;
    int col_right_;

    double Roi_Lag_R_Max_;
    double Roi_Lag_Gr_Max_;
    double Roi_Lag_Gb_Max_;
    double Roi_Lag_B_Max_;
    double Roi_Lag_R_Min_;
    double Roi_Lag_Gr_Min_;
    double Roi_Lag_Gb_Min_;
    double Roi_Lag_B_Min_;

    int bin_lag_;
    int grade_lag_;

    int bin_roi_lag_;
    int grade_roi_lag_;

    int mean_lag_open_mode_;
    int mean_lag_log_align_;

    int roi_lag_open_mode_;
    int roi_lag_log_align_;
};

struct Param_edge_ROI_DL
{
    int skip_;
    int center_size_;
    int edge_size_;
    int gride_size_;

    double spec_p_;
    double spec_n_;
    int bin_dl_;
    int grade_dl_;
    int save_image_;
    int open_mode_;
    int log_align_;

};

class ParamManage
{
public:

    //---------- basic --------------

    RGB_BAYER bayer_;
    int width_;
    int height_;
    int flag_raw8_image_;
    int flag_split_channel_;

    //---------colorbar-----------------
    QMap<QString,QString> colorbar_images;

    //-----------------------------------

    bool loadHistogramParam (const std::string &dir);

    bool loadChannelNoiseParam (const std::string &dir);

    bool loadChannelNoiseParam_V1_1 (const std::string &dir);

    bool loadRowNoiseParam(const std::string &dir);

    bool loadColumnNoiseParam(const std::string &dir);

    bool loadMPkgParam(const std::string &dir);

    bool loadMPkgParam_V1_1(const std::string &dir);

    bool loadMBlemishParam(const std::string &dir);

    bool loadMBlemishParam_V1_1(const std::string &dir);

    bool loadColorbarParam (const std::string &dir);

    bool loadBinaryParam(const std::string &dir);

    bool load_DarkRef_Param(const std::string &dir);

    bool loadDarkcurrentParam (const std::string &dir);

    bool load_Darkcurrent_Global_Param(const std::string &dir);

    bool loadBasicParam(const std::string &dir);

    bool load_dag_param(const std::string &dir);

    bool load_light_linearity_param(const std::string &dir);

    bool load_dag_mean_std_param(const std::string &dir);

    bool load_light_response_param (const std::string &dir);

    bool load_RI_Param(const std::string &dir);

    bool load_NonUniformity_Param (const std::string &dir);

    bool load_POG_Normal_Bright_Param(const std::string &dir);

    bool load_ImageDirttDetect_Bright_Param(const std::string &dir);

    bool load_POG_Param(const std::string &dir);

    bool load_line_defect_Param (const std::string &dir);

    bool load_line_defect_Param_V1_1 (const std::string &dir);

    bool load_line_defect_iic_Param (const std::string &dir);

    bool load_edge_line_defect_Param (const std::string &dir);

    bool load_Color_GI_Param(const std::string &dir);

    bool load_Color_GI_Param_V1_1(const std::string &dir);

    bool load_Color_GI_5x5_Param(const std::string &dir);

    bool load_Color_Ratio_Param (const std::string &dir);

    bool load_Color_Ratio_Param_V1_1(const std::string &dir);

    bool load_Color_Fourier_Param(const std::string &dir);

    bool load_Color_Shading_Param(const std::string &dir);

    bool load_Color_Shading_Param_V1_1(const std::string &dir);

    bool load_Color_Shading_5x5_Param(const std::string &dir);

    bool load_Color_Shading_15_15_Param(const std::string &dir);

    bool load_PD_Collect_Param(const std::string &dir);

    bool load_Pixel_Defect_Param(const std::string &dir);

    bool load_Pixel_Defect_Param_V1_1(const std::string &dir);

    bool load_Pixel_Defect_Param_V1_2(const std::string &dir);

    bool load_Pixel_Defect_Param_V1_3(const std::string &dir);

    bool load_Pixel_Defect_Param_V1_4(const std::string &dir);

    bool load_Pixel_Defect_Single_Param(const std::string &dir);

    bool load_Pixel_Defect_Channel_single_Param(const std::string &dir);

    bool load_Real_ROI_PRNU_Param (const std::string &dir);

    bool load_Optical_Center_Param (const std::string &dir);

    bool load_FPN_DL_Param(const std::string &dir);

    bool load_FPN_DL_Param_V1_1(const std::string &dir);

    bool load_FPN_DL_IIC_Param(const std::string &dir);

    bool load_FPN_DL_bright_dark_Param(const std::string &dir);

    bool load_FPN_DL_bright_dark_Param_V1_1(const std::string &dir);

    bool load_FPN_DL_bright_dark_Param_V1_2(const std::string &dir);

    bool load_OB_DarkShading_Param(const std::string &dir);

    bool load_OB_DarkShading_Param_V1_1(const std::string &dir);

    bool load_OB_DarkShading_Param_V1_2(const std::string &dir);

    bool load_OB_DarkShading_Param_V1_3(const std::string &dir);

    bool load_OB_GrGb_Param(const std::string &dir);

    bool load_ROI_STD_Param (const std::string &dir);

    bool load_Lag_Grid_ROI_Param(const std::string &dir);

    bool load_Read_Noise_Param (const std::string &dir);

    //
    bool load_Edge_ROI_DeadLine_Dark_Param (const std::string &dir);

    bool loadMosaicParam(const std::string &dir);


public:
    //-----------------------------------

    Param_Binary param_binary_;

    Param_Light_Response param_light_response_;
    Param_RelativeIllum param_relative_illu_;
    Param_Non_Uniformity param_non_uniformity_;
    Param_Line_Defect_bright param_line_defect_bright_;

    ParamMosaic param_Mosaic_;

    Param_Edge_Line_Defect_bright param_edge_line_defect_bright_;
    Param_Line_Defect_bright_iic param_line_defect_bright_iic_;
    Param_GI_Ratio param_GI;
    Param_GI_Ratio_5x5 param_GI_5x5;
    Param_Color_Ratio param_color_ratio_;

    Param_Color_Shading param_color_shading;
    Param_Color_Shading_5x5 param_color_shading_5x5;
    Param_Color_Shading_15x15 param_color_shading_15x15;
    Param_Pixel_Defect param_pixel_defect_;
    Param_Pixel_Defect param_single_pixel_defect_;
    Param_Pixel_Defect param_pixel_defect_channel_single_;
    Param_PD_collect pd_collect;
    Param_Real_ROI_PRNU param_real_roi_prnu_;
    Param_Optical_Center param_optical_center_;

    Param_FPN_DL param_fpn_dl_;
    Param_FPN_DL_IIC param_fpn_dl_iic_;
    Param_FPN_DL_Bright_Dark param_fpn_dl_bright_dark;         //亮暗线
    Param_OB_DarkShading param_ob_darkshading_;
    Param_OB_GR_GB param_ob_grgb_;

    Param_ROI_STD param_roi_std_;
    Param_Lag_Grid_Roi param_lag_grid_roi_;
    Param_Read_Noise param_read_noise_;
    ParamRowNoise param_row_noise_;
    ParamColumnNoise param_column_noise_;
    ParamColumnNoise param_channel_noise_;
    Param_Colorbar param_colorbar_;
    MPkgParam param_pkg_;
    MBlemishParam param_blemish_old_;

    Param_ImageDirtDetect_Bright param_ImageDirt_normal_bright_;
    Param_POG_Normal_Bright param_pog_normal_bright_;
    Param_POG param_pog_;
    Param_Dark_Ref param_light_ref_;

    ParamHistogram param_histogram_;
    Param_Ratio_Fourier param_ratio_fourier_;
    Param_Darkcurrent param_darkcurrent_spec_;   //暂时没用到
    Param_Darkcurrent_Global param_dakrcurrent_global_;

    Param_DAG param_dag;
    Param_Light_Linearity param_light_linearity_;
    Param_DAG_StdMean param_dag_std_mean;



    Param_edge_ROI_DL param_edge_ROI_DL_;

    QString fpn_dl_version;
    QString color_shading_version;
    QString gi_ratio_version;
    QString light_response_version;
    QString deadrow_deadcol_bright_version;
    QString pkg_version;
    QString non_uniformity_version;
    QString optical_center_version;
    QString pixel_defect_version;
    QString single_pixel_defect_version;
    QString pog_version;
    QString relative_illu_version;
    QString roi_prnu_version;
    QString blemish_version;
    QString edge_line_defect_bright_version;
    QString color_shading_5x5_version;
    QString color_shading_15x15_version;
    QString color_ratio_version;
    QString line_defect_bright_iic_version;
    QString fpn_dl_bright_dark_version;
    QString ob_darkshading_version;
    QString fpn_dl_iic_version;
    QString lag_grid_roi_version;
    QString lag_roi_std_version;
    QString imageDirt_normal_bright_version;
    QString light_ref_version;
    QString histogram_version;
    QString dag_version;
    QString light_linearity_version;
    QString dag_mean_std_version;
    QString edge_roi_deadline_dark_version;
    QString pixel_defect_channel_single_version;
    QString gi_5_5_ratio_version;

    QString col_channel_noise_version;
    QString row_Noise_version;
    QString column_Noise_version;
    QString read_Noise_version;

    QString ob_grgb_version;

    //-------------------------------------------
};


static ParamManage g_param_manage_binary_, *p_param_manage_binary_=&g_param_manage_binary_ ;    //


#endif // PARAMMANAGE_H
