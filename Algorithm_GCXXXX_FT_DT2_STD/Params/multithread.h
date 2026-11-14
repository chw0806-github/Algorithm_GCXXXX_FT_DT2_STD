#ifndef MULTITHREAD_H
#define MULTITHREAD_H


#include <QThread>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QString>

#define Channel_Num 4

#include "cpalgorithmraw.h"

enum Bayer_Channel {R,Gr,Gb,B};
typedef unsigned char uchar;

namespace MultiThread
{

class get_channel_mean : public QThread
{
public:
    get_channel_mean();
    void setparam(uchar *in_data, int width, int height);
    void run();
    double mean;
private:
    uchar *data;
    int w;
    int h;
};


class get_mutil_roi_linearity_col_ushort : public QThread
{
public:
    get_mutil_roi_linearity_col_ushort();
    void setparam(ushort *in_data, int width, int height, int x0, int y0, int win_width, int win_height, double* linear_, double max_dn_);
    void run();
private:
    ushort *data;
    int w;
    int h;
    int win_x;
    int win_y;
    int win_w;
    int win_h;
    double max_dn;
    double* linear;
};


class get_roi_channel_col_mean_min : public QThread
{
public:
    get_roi_channel_col_mean_min();
    void setparam(uchar *in_data, int width, int height);
    void run();
    double mean_max;
    double mean_min;
    double pixel_max;
    double mean_center_left;
    double mean_center_right;
    int max_pos;
    int min_pos;

private:
    uchar *data;
    int w;
    int h;

};

class get_roi_channel_mean : public QThread
{
public:
    get_roi_channel_mean();
    void setparam(uchar *in_data, int width, int height, int x0, int y0, int win_width, int win_height);
    void run();
    double mean;
private:
    uchar *data;
    int w;
    int h;
    int win_x;
    int win_y;
    int win_w;
    int win_h;
};



class get_mutil_roi_mean_short : public QThread
{
public:
    get_mutil_roi_mean_short();
    void setparam(short *in_data_H, short *in_data_L, short *in_data_Diff, int width, int height, int x0, int y0, int win_width, int win_height);
    void run();
    QVector<double> value_log;
private:
    short *data_H;
    short *data_L;
    short *data_Diff;
    int w;
    int h;
    int win_x;
    int win_y;
    int win_w;
    int win_h;
};

class get_roi_channel_pixel_percent_DN_raw8 : public QThread
{
public:
    get_roi_channel_pixel_percent_DN_raw8();
    void setparam(uchar *data_, int w_, int h_, double x_, int roi_x_, int roi_y_, int roi_w_, int roi_h_);
    void run();
    std::string msg;
    int x_pos_dn;
    int histogram[256];
private:
    uchar *data;
    int w;
    int h;
    double x;
    int roi_x;
    int roi_y;
    int roi_w;
    int roi_h;
};


class get_roi_channel_pixel_percent_DN_short : public QThread
{
public:
    get_roi_channel_pixel_percent_DN_short();
    void setparam(short *data_, int w_, int h_, double x_, int roi_x_, int roi_y_, int roi_w_, int roi_h_);
    void run();
    std::string msg;
    int x_pos_dn;
    int histogram[2047];
private:
    short *data;
    int w;
    int h;
    double x;
    int roi_x;
    int roi_y;
    int roi_w;
    int roi_h;
};


class get_roi_channel_pixel_percent_DN_raw16 : public QThread
{
public:
    get_roi_channel_pixel_percent_DN_raw16();
    void setparam(short *data_, int w_, int h_, double x_);
    void run();
    std::string msg;
    int x_pos_dn;

private:
    short *data;
    int w;
    int h;
    double x;
};


class get_roi_mean_std_by_rect : public QThread
{
public:
    get_roi_mean_std_by_rect();
    void setparam(uchar *in_data, cv::Rect &roi_rect, int width, int height);
    void run();
    double mean;
    double std;
private:
    uchar *data;
    cv::Rect ROI_Rect;
    int w;
    int h;
};



class get_double_roi_mean_std_by_rect : public QThread
{
public:
    get_double_roi_mean_std_by_rect();
    void setparam(double *in_data, cv::Rect &roi_rect, int width, int height);
    void run();
    double mean;
    double std;
private:
    double *data;
    cv::Rect ROI_Rect;
    int w;
    int h;
};


class get_double_roi_mean_std_by_rect_chao : public QThread
{
public:
    get_double_roi_mean_std_by_rect_chao();
    void setparam(double *in_data, Point2i start_loca_,cv::Rect &roi_rect, int width, int height);
    void run();
    double mean;
    double std;
private:
    double *data;
    Point2i start_loca;
    cv::Rect ROI_Rect;
    int w;
    int h;
};



class get_channel_mean_raw16 : public QThread
{
public:
    get_channel_mean_raw16();
    void setparam(uchar *in_data, int width, int height);
    void run();
    double mean;
private:
    uchar *data;
    int w;
    int h;
};



class get_roi_channel_mean_raw16 : public QThread
{
public:
    get_roi_channel_mean_raw16();
    void setparam(unsigned short *in_data, int width, int height, int x0, int y0, int win_width, int win_height);
    void run();
    double mean;
private:
    unsigned short *data;
    int w;
    int h;
    int win_x;
    int win_y;
    int win_w;
    int win_h;
};



class get_grid_roi_mean_raw16 : public QThread
{
public:
    get_grid_roi_mean_raw16();
    void setparam(short *in_data, int width, int height, int cnt_col, int cnt_row);
    void run();
    QVector<double> GridROI_Mean;
private:
    short *data;
    int w;
    int h;
    int cnt_w;
    int cnt_h;
};





class get_real_roi_prnu : public QThread
{
public:
    get_real_roi_prnu();
    void setparam(float *delta_data, uchar *data, int width, int height, int x0, int y0, int win_width, int win_height);
    void run();
    double prnu;
private:
    float *delta;
    uchar *in_data;
    int w;
    int h;
    int win_x;
    int win_y;
    int win_w;
    int win_h;
};



class get_channel_uniformity : public QThread
{
public:
    get_channel_uniformity();
    void setparam(uchar *in_data, int width, int height, int w_cell, int h_cell);
    void run();
    double prnu;
private:
    uchar *data;
    int w;
    int h;
    int w_cell_;
    int h_cell_;
};





class get_channel_uniformity_max : public QThread
{
public:
    get_channel_uniformity_max();
    void setparam(uchar *in_data, int width, int height, int w_cell, int h_cell);
    void run();
    double prnu;
    double prnu_eachcell_max;
    int max_cell_pos;
    QVector<double> prnu_eachcell;
private:
    uchar *data;
    int w;
    int h;
    int w_cell_;
    int h_cell_;
};





class get_non_uniformity_bychannel : public QThread
{
public:
    get_non_uniformity_bychannel();
    void setparam(uchar *in_data, int width, int height, int w_cell, int h_cell);
    void run();
    double n_uniform_;
    double n_uniform_eachcell_max_;
    int max_cell_pos_;
    double n_uniform_mirror_diff_eachcell_max_;
    int mirror_diff_max_cell_pos_;
    double n_uniform_updown_diff_eachcell_max_;
    int updown_diff_max_cell_pos_;

    QVector<double> n_uniform_eachcell_;
    QVector<double> n_uniform_mirror_diff_eachcell_;
    QVector<double> n_uniform_updown_diff_eachcell_;

private:
    uchar *data;
    int w;
    int h;
    int w_cell_;
    int h_cell_;
};




class get_image_colorratio : public QThread
{
public:
    get_image_colorratio();
    void setparam(uchar *pR, uchar *pGr, uchar *pGb,
                  uchar *pB, int x0, int y0, int width, int height, int w_rect, int h_rect);
    void run();
    double g_g_ratio;
    double r_g_ratio;
    double b_g_ratio;
    double gr_gb_diff;
private:
    uchar *p_buf_r;
    uchar *p_buf_gr;
    uchar *p_buf_gb;
    uchar *p_buf_b;
    int w;
    int h;
    int pos_x;
    int pos_y;
    int w_rect_;
    int h_rect_;
};




class get_roi_colorratio : public QThread
{
public:
    get_roi_colorratio();
    void setparam(uchar *pR, uchar *pGr, uchar *pGb, uchar *pB,
                  int width, int height, int roi_x, int roi_y, int roi_w, int roi_h);
    void run();
    double g_g_ratio;
    double r_g_ratio;
    double b_g_ratio;
    double gr_gb_diff;
private:
    uchar *p_buf_r;
    uchar *p_buf_gr;
    uchar *p_buf_gb;
    uchar *p_buf_b;
    int w;
    int h;
    int roi_x_;
    int roi_y_;
    int roi_w_;
    int roi_h_;
};





class get_row_defect_bychannel : public QThread
{
public:
    get_row_defect_bychannel();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, double row_max_min_diff, int cnt_deadrow,
                  double dol_std_max, int dol_std_max_pos,
                  double breakline_row_spec, int breakline_row_cnt,
                  double breakline_row_max_value,int breakline_row_max_pos,
                  Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadrow_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;
    double row_max_min_diff_;

    double dol_std_max_;
    int dol_std_max_pos_;

    int breakline_row_cnt_;
    double breakline_row_max_value_;
    int breakline_row_max_pos_;

    QVector<double> mean_each_;
    QVector<double> delta_each_;
    QVector<double> diff_each_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    double breakline_row_spec_;

    Spec_Mode mode_;
};



class get_col_defect_bychannel : public QThread
{
public:
    get_col_defect_bychannel();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, double col_max_min_diff, int cnt_deadcol,
                  double dol_std_max, int dol_std_max_pos,
                  double breakline_col_spec, int breakline_col_cnt,
                  double breakline_col_max_value,int breakline_col_max_pos,
                  Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadcol_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;
    double col_max_min_diff_;

    double dol_std_max_;
    int dol_std_max_pos_;

    int breakline_col_cnt_;
    double breakline_col_max_value_;
    int breakline_col_max_pos_;

    QVector<double> mean_each_;
    QVector<double> delta_each_;
    QVector<double> diff_each_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    double breakline_col_spec_;

    Spec_Mode mode_;
};

class get_col_defect_bychannel_bright_dark : public QThread
{
public:
    get_col_defect_bychannel_bright_dark();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, double spec_bright_line , double spec_dark_line,int cnt_dol, double max_diff,
                  int max_pos, double col_max_min_diff, int cnt_deadcol,
                  double dol_std_max, int dol_std_max_pos,
                  double breakline_col_spec, int breakline_col_cnt,
                  double breakline_col_max_value,int breakline_col_max_pos,
                  Spec_Mode mode, std::string msg,int cnt_bright_col,
                  double max_bright_col_diff,int max_bright_col_pos,double max_dark_col_diff,int max_dark_col_pos);
    void run();
    int cnt_deadcol_;
    int cnt_dol_;
    int cnt_bright_col_;
    double max_diff_;
    int max_pos_;
    double max_bright_col_diff_;
    int max_bright_col_pos_;
    double max_dark_col_diff_;
    int max_dark_col_pos_;
    double col_max_min_diff_;

    double dol_std_max_;
    int dol_std_max_pos_;

    int breakline_col_cnt_;
    double breakline_col_max_value_;
    int breakline_col_max_pos_;

    QVector<double> mean_each_;
    QVector<double> delta_each_;
    QVector<double> diff_each_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    double spec_bright_line_;
    double spec_dark_line_;
    double breakline_col_spec_;

    Spec_Mode mode_;
};


class get_edge_row_defect_bychannel : public QThread
{
public:
    get_edge_row_defect_bychannel();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos,  int cnt_deadrow, double dol_std_max, int dol_std_max_pos,
                  Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadrow_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;

    double dol_std_max_;
    int dol_std_max_pos_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    Spec_Mode mode_;
};



class get_edge_col_defect_bychannel : public QThread
{
public:
    get_edge_col_defect_bychannel();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, int cnt_deadcol,double dol_std_max, int dol_std_max_pos,
                  Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadcol_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;

    double dol_std_max_;
    int dol_std_max_pos_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    Spec_Mode mode_;
};



class get_channel_deadrow : public QThread
{
public:
    get_channel_deadrow();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, double row_max_min_diff, int cnt_deadrow, Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadrow_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;
    double row_max_min_diff_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    Spec_Mode mode_;
};



class get_channel_deadrow_add_breakline : public QThread
{
public:
    get_channel_deadrow_add_breakline();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, double row_max_min_diff, int cnt_deadrow,
                  double breakline_row_spec, int breakline_row_cnt, double breakline_row_max_value,
                  int breakline_row_max_pos, Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadrow_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;
    double row_max_min_diff_;

    double breakline_row_spec_;
    int breakline_row_cnt_;
    double breakline_row_max_value_;
    int breakline_row_max_pos_;

    QVector<double> delta_each_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    Spec_Mode mode_;
};




class get_channel_deadcol_add_breakline : public QThread
{
public:
    get_channel_deadcol_add_breakline();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, double col_max_min_diff, int cnt_deadcol,
                  double breakline_col_spec, int breakline_col_cnt, double breakline_col_max_value,
                  int breakline_col_max_pos, Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadcol_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;
    double col_max_min_diff_;

    double breakline_col_spec_;
    int breakline_col_cnt_;
    double breakline_col_max_value_;
    int breakline_col_max_pos_;

    QVector<double> delta_each_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    Spec_Mode mode_;
};


class get_channel_deadcol : public QThread
{
public:
    get_channel_deadcol();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, double col_max_min_diff, int cnt_deadcol, Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadcol_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;
    double col_max_min_diff_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    Spec_Mode mode_;
};


//class get_channel_deadcol_add_breakline : public QThread
//{
//public:
//    get_channel_deadcol_add_breakline();
//    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
//                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
//                  int max_pos, double col_max_min_diff, int cnt_deadcol, Spec_Mode mode, std::string msg);
//    void run();
//    int cnt_deadcol_;
//    int cnt_dol_;
//    double max_diff_;
//    int max_pos_;
//    double col_max_min_diff_;
//    std::string msg_;
//private:
//    uchar *p_data;
//    int w;
//    int h;
//    int r;
//    int skip;
//    double spec_line_;
//    double spec_dol_;
//    Spec_Mode mode_;
//};




class get_edge_channel_deadrow : public QThread
{
public:
    get_edge_channel_deadrow();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, double row_max_min_diff, int cnt_deadrow, Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadrow_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;
    double row_max_min_diff_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    Spec_Mode mode_;
};



class get_edge_channel_deadcol : public QThread
{
public:
    get_edge_channel_deadcol();
    void setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                  double spec_line, double spec_dol, int cnt_dol, double max_diff,
                  int max_pos, double col_max_min_diff, int cnt_deadcol, Spec_Mode mode, std::string msg);
    void run();
    int cnt_deadcol_;
    int cnt_dol_;
    double max_diff_;
    int max_pos_;
    double col_max_min_diff_;
    std::string msg_;
private:
    uchar *p_data;
    int w;
    int h;
    int r;
    int skip;
    double spec_line_;
    double spec_dol_;
    Spec_Mode mode_;
};


class bychannel_binarization : public QThread
{
public:
    bychannel_binarization();
    void setparam(uchar *in_data, uchar *out_data, int width, int height, int skip_cnt,
                   int radius, double binary_spec, Spec_Mode mode);
    void run();
    uchar *p_data;
    uchar *binary_data;
private:
    int w;
    int h;
    int skip;
    int r;
    double spec;
    Spec_Mode mode_;
};


class bychannel_binarization_add_blemish : public QThread
{
public:
    bychannel_binarization_add_blemish();
    void setparam(uchar *in_data, uchar *out_data, uchar *out_blemish, int width, int height, int skip_cnt,
                   int radius, double binary_spec, double blemish_Spec, Spec_Mode mode);
    void run();
    uchar *p_data;
    uchar *binary_data;
    uchar *blemish_binary;
private:
    int w;
    int h;
    int skip;
    int r;
    double spec;
    double spec_b;
    Spec_Mode mode_;
};


class bychannel_binarization_delta : public QThread
{
public:
    bychannel_binarization_delta();
    void setparam(uchar *in_data, uchar *out_data, int width, int height, int skip_cnt,
                   int radius, double binary_spec, Spec_Mode mode, double *delta_matrix);
    void run();
    uchar *p_data;
    uchar *binary_data;
    double *delta_map;
private:
    int w;
    int h;
    int skip;
    int r;
    double spec;
    Spec_Mode mode_;
};



class bychannel_pixel_defect_binarization : public QThread
{
public:
    bychannel_pixel_defect_binarization();
    void setparam(uchar *in_data, float *out_delta, uchar *out_data, uchar *out_blemish, int width, int height, int skip_cnt,
                   int radius, double binary_spec, double blemish_Spec, Spec_Mode mode);
    void run();
    uchar *p_data;
    float *delta_data;
    uchar *binary_data;
    uchar *blemish_binary;
private:
    int w;
    int h;
    int skip;
    int r;
    double spec;
    double spec_b;
    Spec_Mode mode_;
};



class bychannel_pixel_defect_chao : public QThread
{
public:
    bychannel_pixel_defect_chao();
    void setparam(uchar *in_data, float *out_delta, uchar *out_data, uchar *out_blemish, int width, int height, int skip_cnt,
                   int radius, double binary_spec, double blemish_Spec, Spec_Mode mode);
    void run();
    uchar *p_data;
    float *delta_data;
    uchar *binary_data;
    uchar *blemish_binary;
private:
    int w;
    int h;
    int skip;
    int r;
    double spec;
    double spec_b;
    Spec_Mode mode_;

    int x_start_ ;
    int y_start_ ;
};


class bychannel_flicker_binarization_raw16 : public QThread
{
public:
    bychannel_flicker_binarization_raw16();
    void setparam(uchar *in_data, uchar *out_data, int width, int height, int frame_cnt, double binary_spec);
    void run();
    uchar *p_data;
    uchar *binary_data;
private:
    int w;
    int h;
    int framecnt;
    double spec;
};


class bychannel_flicker_binarization_raw16_chao : public QThread
{
public:
    bychannel_flicker_binarization_raw16_chao();
    void setparam(uchar *in_data, uchar *out_data, int width, int height, int y_start, int frame_cnt, double binary_spec);
    void run();
    uchar *p_data;
    uchar *binary_data;
private:
    int w;
    int h;
    int y_start_;
    int framecnt;
    double spec;
};

class get_bychannel_std_map_raw16 : public QThread
{
public:
    get_bychannel_std_map_raw16();
    void setparam(uchar *in_data, double *out_data, int width, int height, int frame_cnt);
    void run();
    uchar *p_data;
    double *std_map;
private:
    int w;
    int h;
    int framecnt;
};

class get_bychannel_std_map_raw16_chao : public QThread
{
public:
    get_bychannel_std_map_raw16_chao();
    void setparam(uchar *in_data, double *out_data, int width, int height, int y_start, int frame_cnt);
    void run();
    uchar *p_data;
    double *std_map;
private:
    int w;
    int h;
    int y_start_;
    int framecnt;
};



class raw16_to_raw8: public QThread
{
public:
    raw16_to_raw8();
    void setparam(uchar *in_raw16, uchar *out_raw8, int width, int height, int raw_bits = 10);
    void run();
    uchar *in_data;
    uchar *out_data;
private:
    int w;
    int h;
    int raw_bits_;
};


class get_darkshading : public QThread
{
public:
    get_darkshading();
    void setparam(uchar *in_data, int width, int height, int x_cnt, int y_cnt,std::string msg);
    void run();
    double mean_ob_eachcell_max;
    double mean_ob_eachcell_min;
    int max_ob_pos;
    int min_ob_pos;
    QVector<double> mean_ob_eachcell;
    std::string msg_;
private:
    uchar *data;
    int w;
    int h;
    int x_cnt_;
    int y_cnt_;

};

}

#endif // MULTITHREAD_H
