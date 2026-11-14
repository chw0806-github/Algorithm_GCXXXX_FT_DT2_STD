#ifndef ALGORITHMITEMS_H
#define ALGORITHMITEMS_H

#include "gbuf_manage.h"
#include "testitems.h"
#include <QDir>

static const int  Parallel_num=4;

enum POG_type{POG_Bright,POG_Normal};
enum PD_single_type{Blemish,PKG};
enum Pixel_Defect_type {Pixel_Defect_dark,Pixel_Defect_bright,External_Configuration};
enum Darkcurrent_type{Darkcurrent_normal,Darkcurrent_4Channel};
enum Split_Channel_type {Split_Bayer,Split_4C};

extern std::string g_str_msg_;
extern std::vector<std::string> g_str_logtmp;
extern std::vector<std::string> g_str_msg_all;
extern std::vector<std::string> g_str_log_all;

class Color_Ratio_Value
{
public:

    void reset_all(){
        gg_leftup = 0;
        rg_leftup = 0;
        bg_leftup = 0;
        gg_leftup_5X5 = 0;

        gg_rightup = 0;
        rg_rightup = 0;
        bg_rightup = 0;
        gg_rightup_5X5 = 0;

        gg_leftdown = 0;
        rg_leftdown = 0;
        bg_leftdown = 0;
        gg_leftdown_5X5 = 0;

        gg_rightdown = 0;
        rg_rightdown = 0;
        bg_rightdown = 0;
        gg_rightdown_5X5 = 0;

        gg_center = 0;
        rg_center = 0;
        bg_center = 0;
        gg_center_5X5 = 0;

        GG_ratio.clear();
        RG_ratio.clear();
        BG_ratio.clear();
        G_G_diff.clear();

        roi_rg_ratio.clear();
        roi_bg_ratio.clear();
        roi_gg_ratio.clear();
    }

    double gg_leftup;
    double rg_leftup;
    double bg_leftup;
    double gg_leftup_5X5;

    double gg_rightup;
    double rg_rightup;
    double bg_rightup;
    double gg_rightup_5X5;


    double gg_leftdown;
    double rg_leftdown ;
    double bg_leftdown ;
    double gg_leftdown_5X5;

    double gg_rightdown ;
    double rg_rightdown ;
    double bg_rightdown ;
    double gg_rightdown_5X5;

    double gg_center ;
    double rg_center ;
    double bg_center;
    double gg_center_5X5;

    QMap <QPair<QString,double>,double> GG_ratio;
    QMap <QPair<QString,double>,double> RG_ratio;
    QMap <QPair<QString,double>,double> BG_ratio;
    QMap <QString,double> G_G_diff;

    QVector<double> roi_rg_ratio;
    QVector<double> roi_bg_ratio;
    QVector<double> roi_gg_ratio;
};


class Subitem_Result
{
public:

    explicit Subitem_Result(string name_,bool result_,
                            int bin_ = 1,int grade_ = 0, bool g_msg_ = true):
        name(name_), is_passed( result_),
        bin( bin_),grade( grade_), g_msg(g_msg_){}

public:
    std::string name;      //测试分项名称
    bool is_passed;        //测试结果
    int bin;               //结果bin
    int grade;             //测试分项等级
    bool g_msg;
};

class AlgorithmItems
{
public:

    bool reset_gbuf_pointer();      //指针置空

    bool init(uchar *data_start,int width_,int height_,
              int framecnt_=16,int resultcnt_=8, int buf_type=16);

    bool New_Pointer_Buf(uchar** ptr,Memory_Area Area,
                         Buf_type type,int width_,
                         int height_,int famer_cnt);

    bool Free_Pointer_Buf(uchar** ptr);

    bool Free_All_Buf();


    //类型转换
    bool  Convert_Raw16_to_Ushort(const ParamManage& manage,uchar *pData,
                                  unsigned short *pshort,
                                  int frameCnt,int datatype);
    bool  Convert_Raw10_to_Raw16(const ParamManage& manage,uchar *pData_in,
                                 uchar *pData_out,int frameCnt);


    //  分通道

    bool  Split_Channel(const ParamManage& manage, Split_Channel_type type,bool OB_flag,
                        uchar *pData_in,int framecnt,int datatype, uchar *pImage_save);

    bool  Split_Channel_TwoRaw16(const ParamManage& manage,uchar *pData_long_exp,
                                 uchar *pData_short_exp,int frameCnt,bool flag_short_null,
                                 int raw_bits = 10);

    //NEW

    void GetROIMean_Doublebyte_RGB(unsigned short *pData,
                              int width, int height,
                              int col, int line,
                              int win_w, int win_h,double &mean_r, double &mean_gr, double &mean_gb, double &mean_b);


    //通用功能函数
    Subitem_Result Light_Response(const ParamManage& manage);     //ok

    Subitem_Result Light_Response_V1_0(const ParamManage& manage);

    Subitem_Result Relative_Illumination(const ParamManage& manage);    //ok

    Subitem_Result Relative_Illumination_V1_0(const ParamManage& manage);

    Subitem_Result Mosaic(const ParamManage& manage, int datatype);

    Subitem_Result Non_Uniformity(const ParamManage& manage);       //ok

    Subitem_Result Non_Uniformity_V1_0(const ParamManage& manage);

    Subitem_Result ImageDirt(const ParamManage& manage);            //ok

    Subitem_Result ImageDirt_V1_0(const ParamManage& manage);

    Subitem_Result POG_Test(const ParamManage& manage,POG_type type);       //ok

    Subitem_Result POG_Test_V1_0(const ParamManage& manage,POG_type type);

    vector<Subitem_Result> Color_Ratio( ParamManage& manage);

    vector<Subitem_Result> Color_Ratio_V1_0( ParamManage& manage);
    vector<Subitem_Result> Color_Ratio_V1_1( ParamManage& manage);
     Color_Ratio_Value ratio_test_result;


    Subitem_Result PKG_Test(const ParamManage& manage);

    Subitem_Result PKG_Test_V1_0(const ParamManage& manage);

    Subitem_Result PKG_Test_V1_1(const ParamManage& manage);

    Subitem_Result PD_all_single_pix_find(const ParamManage& manage, PD_single_type type, std::vector<MClusterInf> &pd_all_single_particle);   //排除 dpc off 的干扰

    Subitem_Result Blemish_Test_V1_1(const ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Blemish_Test(const ParamManage& manage);

    Subitem_Result Blemish_Test_V1_0(const ParamManage& manage);

    Subitem_Result Single_Pixel_Defect_Test( ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Single_Pixel_Defect_Test_V1_0( ParamManage& manage,Pixel_Defect_type type);


    Subitem_Result Pixel_Defect_Test( ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Pixel_Defect_Test_V1_0( ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Pixel_Defect_Test_V1_1( ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Pixel_Defect_Test_V1_2( ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Pixel_Defect_Test_V1_3( ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Pixel_Defect_Test_V1_4( ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Pixel_Defect_Test_NO_Test_Info( ParamManage& manage,Pixel_Defect_type type);

    Subitem_Result Deadrow_Deadcol_bright(const ParamManage& manage,bool log_out);

    Subitem_Result Deadrow_Deadcol_bright_V1_0(const ParamManage& manage,bool log_out);

    Subitem_Result Deadrow_Deadcol_bright_V1_1(const ParamManage& manage,bool log_out);

    Subitem_Result Deadrow_Deadcol_bright_IIC(const ParamManage& manage);

    Subitem_Result Deadrow_Deadcol_bright_IIC_V1_0(const ParamManage& manage);

    Subitem_Result Edge_Deadrow_Deadcol_bright(const ParamManage& manage);

    Subitem_Result Edge_Deadrow_Deadcol_bright_V1_0(const ParamManage& manage);

    Subitem_Result ROI_PRNU(const ParamManage& manage);

    Subitem_Result ROI_PRNU_V1_0(const ParamManage& manage);

    Subitem_Result ROI_DSNU(const ParamManage& manage);

    Subitem_Result ROI_DSNU_V1_0(const ParamManage& manage);

    Subitem_Result Ref_Col_Light(const ParamManage& manage);

    Subitem_Result Ref_Col_Light_V1_0(const ParamManage& manage);

    Subitem_Result DAG(const ParamManage& manage, uchar* pData);

    Subitem_Result DAG_V1_0(const ParamManage& manage, uchar* pData);

    Subitem_Result DAG_V1_1(const ParamManage& manage, uchar* pData);

    Subitem_Result Light_Linearity_Ushort(const ParamManage& manage);

    Subitem_Result Light_Linearity_Ushort_V1_0(const ParamManage& manage);

    Subitem_Result DAG_CycleStd(const ParamManage& manage, uchar* pData, int test_frameCnt);

    Subitem_Result DAG_CycleStd_V1_0(const ParamManage& manage, uchar* pData, int test_frameCnt);

    Subitem_Result Calculate_The_Final_Bin(const vector<Subitem_Result>& items_results);

public:

    bool New_PRNU(int w,int h);
    bool Free_PRNU();

    bool New_result_buf(int w,int h);
    bool Free_result_buf();

    void set_site(size_t site_num){site_=site_num;}

private:
    bool New_result_buf_two_raw16(int w,int h);

private:

    size_t site_;
    Gbuf_Manage Pointer_manage;
    bool flag_init;                   //初始化成功标志

public:

    cv::Mat img_tmp;                 //POG

    double mean_gr_roi;
    double mean_gb_roi;

    clock_t time_begin;
    clock_t time_end;

    /***************         结果区指针             **********/

    //单byte 指针
    uchar *g_buf_raw_;             //w*h   半张图
    uchar *g_buf_r_ ;              //w*h/4
    uchar *g_buf_gr_;              //w*h/4
    uchar *g_buf_gb_;              //w*h/4
    uchar *g_buf_b_;               //w*h/4

    uchar *g_buf_raw_tmp_;         //w*h   半张图
    uchar *g_buf_r_tmp_ ;          //w*h/4
    uchar *g_buf_gr_tmp_;          //w*h/4
    uchar *g_buf_gb_tmp_;          //w*h/4
    uchar *g_buf_b_tmp_;           //w*h/4

    //双byte 指针

    uchar *g_buf_raw_16_ ;          //w*h*2   一张图
    uchar *g_buf_r_16_ ;            //w*h/2
    uchar *g_buf_gr_16_ ;           //w*h/2
    uchar *g_buf_gb_16_ ;           //w*h/2
    uchar *g_buf_b_16_ ;            //w*h/2

    uchar *g_buf_raw_16_tmp_ ;          //w*h*2   一张图
    uchar *g_buf_r_16_tmp_ ;            //w*h/2
    uchar *g_buf_gr_16_tmp_ ;           //w*h/2
    uchar *g_buf_gb_16_tmp_ ;           //w*h/2
    uchar *g_buf_b_16_tmp_;            //w*h/2


    /***************         计算区指针             **********/

    //二值化图指针
    uchar *g_binary_r_ ;
    uchar *g_binary_gr_ ;
    uchar *g_binary_gb_ ;
    uchar *g_binary_b_ ;
    uchar *g_binary_ ;

    uchar *g_buf_tmp_ ;            // w*h  一张图  存储坏点类型

    float *g_buf_delta_raw_;        //w*h   两张图
    float *g_buf_delta_r_ ;
    float *g_buf_delta_gr_ ;
    float *g_buf_delta_gb_ ;
    float *g_buf_delta_b_ ;

    //单byte 指针
    uchar *g_buf_blemish_raw_ ;    //w*h   半张图
    uchar *g_buf_blemish_r_ ;
    uchar *g_buf_blemish_gr_ ;
    uchar *g_buf_blemish_gb_ ;
    uchar *g_buf_blemish_b_ ;

};


#endif // ALGORITHMITEMS_H
