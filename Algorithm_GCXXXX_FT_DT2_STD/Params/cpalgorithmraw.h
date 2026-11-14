#ifndef CPALGORITHMYUV_H
#define CPALGORITHMYUV_H

#include <QMap>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include <string>
#include <vector>

#include <algorithm>
#include <math.h>

#include <cstring>
#include <cmath>

#include <pthread.h>

#include <fstream>
#include <windows.h>
#include <numeric>
#include <stack>
#include <QDir>
#include <QSet>

#include "parammanage.h"
#include "frequencyfilter.h"

#undef max
#undef min

using namespace cv;
using namespace std;


//#define GC13C33
//#define GC8C54
#define GC16B2

#define PI 3.1415926


#define THREAD_NUM 2

#ifdef GC13C33
  #define FULLSIZE_W 4208
  #define FULLSIZE_H 3120
  #define BLOCK_H (FULLSIZE_H+32) // row padding=16
#endif

#ifdef GC8C54
  #define FULLSIZE_W 3264
  #define FULLSIZE_H 2448
  #define BLOCK_H (FULLSIZE_H+32) // row padding=16
#endif


#ifdef GC16B2
  #define FULLSIZE_W 4640
  #define FULLSIZE_H 3488
  #define BLOCK_H (FULLSIZE_H+32) // row padding=16
#endif

struct BlemishInfo {

    QString type;
    int lsc_x;
    int lsc_y;
    int lsc_w;
    int lsc_h;

    QString enhace_type;
    int enhace_x;
    int enhace_y;
    int enhace_w;
    int enhace_h;
};



typedef struct paramCrosstalk
{
    int ID;
    int col_start;
    int w;
    int h;
    unsigned char **gain;
}paramCrosstalk;

typedef struct paramThread
{
    int ID;                  // thread ID index;
    int col_start;           // column start index
    int w;                   // block width with col padding
    int h;                   // block height with row padding
    unsigned short *in_raw16; // fullsize MTK unpack raw16
    unsigned short *out_raw16;// fullsize MTK unpack raw16
    unsigned char **crosstalk_gain;
    int flat_th;
    int diff_th;
    int cfa;                 // 0:GRBG  1:RGGB  2:BGGR  3:GBRG
    int r_gain;              // AWB gain*256
    int gr_gain;
    int gb_gain;
    int b_gain;
    short black_level;
    int xtalk_ratio_r_h;
    int xtalk_ratio_r_v;
    int xtalk_ratio_b_h;
    int xtalk_ratio_b_v;
    int xtalk_en;   // crosstalk enable
    int xtalk_static_en; // static crosstalk enable
    int xtalk_dynamic_en; // dynamic crosstalk enable
    int otpdpc_en;  // OTP_DPC enable
    int dpc_en;     // dynamic DPC enable
    int fc_en;      // false color suppress enable
    int fc_level;   // false color suppress level, larger level indicates weaker strength
}paramThread;


typedef struct paramThreadScaler
{
    int ID;
    unsigned char *knee_in;
    unsigned char *gain_out;
    int width_out;
    int height_out;
    int width_in;
    int height_in;
}paramThreadScaler;


extern paramCrosstalk XTargs[THREAD_NUM];
extern unsigned int otpdpc_table[250];
extern int otpdpc_total_num;
extern clock_t cast_time_step1;
extern clock_t cast_time_step2;


typedef unsigned char uchar;

enum Spec_Mode{Ratio,DN};

namespace AlgorithmFuncs {
struct MPDInf{
    int x_;
    int y_;
    int w_;
    int h_;
    MPDInf(int x, int y, int w, int h) :
        x_(x),
        y_(y),
        w_(w),
        h_(h)
    {}
    MPDInf() :
        x_(0),
        y_(0),
        w_(0),
        h_(0)
    {}
};



int find_specific_area_in_binary_map(cv::Mat binaryImage, double area_spec);
int Darkbar_Binary(cv::Mat diff_map, float darkbar_spec,int saveimage, int angle,std::string& msg);
int ConnectedDomainDetect(cv::Mat src,int cluster_r, int limit_size, QVector<QSet<int> >& cluster_group );
int Bright_ImageDirttDetect(cv::Mat srcImage, int src_width,int src_height,int pre_blur_size_, int img_resize,float lsc_ratio,
                            int blur_r1, int blur_r2, float pog_spec_p,float pog_spec_n, double &max_pog, int& max_pos,
                            double &min_pog,int& min_pos,float gfda_scratch_spec_p, float gfda_spot_spec_p,float gfda_spot_spec_n,
                            int gfda_spot_limit_size,float leda_ratio_n, float leda_ratio_p, float leda_spot_spec_p ,int leda_spot_limit_size,
                            int stain_skip, int stain_r_kernel,int stain_r_outer,float stain_spec,int saveimage, int &dark_bar_cnt,
                            int &GFDA_pog_cnt,int &GFDA_scratch_cnt,int &GFDA_spot_cnt,int &LEDA_spot_cnt,
                            int &LDMA_stain_cnt,std::string & msg);

int find_pog_normal_bright(cv::Mat srcImage, int crop, int r1, int r2, double spec_p, double spec_n,
                           double sigma_1, double sigma_2, int resize_w, int resize_h, double adaptive_ratio,
                           double lsc_ratio, double *max, int *max_pos_index, double *min, int *min_pos_index,
                           int blemish_skip, int r_kernel, int r_outer, double blemish_spec, int saveimage);

int find_pog(cv::Mat srcImage, int r1, int r2, int resize_w, int resize_h, double spec_p, double spec_n,
             int saveimage, double *max, int *max_pos_index, double *min, int *min_pos_index, Spec_Mode mode);

int blur_diff_binary(cv::Mat srcImage, int r1, int r2, double spec, int saveimage);

void SeveralFrameToOne (uchar *pIn, uchar *pOut,int width, int height,int frameCnt, int Datatype);

void raw10_to_raw16_several_frame(uchar *pData, short *p_raw16, int w, int h, int frameCnt );

void two_short_images_minus(short *pData1, short *pData2, short *pDiff, int w, int h);

bool Get_channel_mean_from_image_doublebyte(uchar *pData, RGB_BAYER bayer, int width, int height,
                                            double &mean_r, double &mean_gr, double &mean_gb, double &mean_b);

void from_doublebyte_to_raw8 (uchar *pIn, uchar *pOut,int w, int h, int raw_bits);

void Uniformity_Detail_Test (int *pData, int w, int h,
                             int w_cell, int h_cell,
                             QVector<double> &stdv_each_cell);

double GetChannelMean(uchar *pData, int w, int h);

double GetChannelMean_Doublebyte (uchar *pData, int w, int h);

double GetROIMean (uchar *pData, int width, int height, int col, int line,  int win_w, int win_h);

double GetROIMean_short (short *pData, int width, int height, int col, int line,  int win_w, int win_h);

double GetROIChannelMean(unsigned char* pData, int width, int height, int roi_x, int roi_y, int roi_w, int roi_h);

bool Get_ROILinearity_Col_Ushort (ushort *pData, int width, int height, int win_x0, int win_y0,  int win_w, int win_h, double* linear, double max_dn);

bool Get_Multi_ROI_Mean_short (short *pData_H, short *pData_L, short *pData_Diff, int width, int height,
                               int col, int line,  int win_w, int win_h, QVector<double> &str_log);

void Get_ROI_MEAN_STD (uchar *pData, cv::Rect &roi, int width, int height, double &mean, double &std);

double GetROIMean_col_min (uchar *pData, int width, int height,double &mean_max,double &mean_min,double &pixel_max,double &mean_left,double &mean_right,int &pos_max,int &pos_min);

void Get_ROI_MEAN_Double_chao ( double *pData, Point2i start_loca,cv::Rect &roi, int width,
                                               double& split_mean, double& split_std_v);

void Get_ROI_MEAN_Double (double *pData, cv::Rect &roi, int width, int height, double &mean, double &std);

double GetROIMean_Doublebyte (unsigned short *pData, int width, int height, int col, int line,  int win_w, int win_h);

int  Get_GridROI_Mean_Doublebyte (short *pData, int width, int height, int cnt_col, int cnt_row, QVector<double> &GridROI_Mean);

double UniformityTest(uchar *pData, int w, int h, int w_cell, int h_cell);

double UniformityTest_Max (uchar *pData, int w, int h,
                           int w_cell, int h_cell,
                           double &prnu_eachcell_max, int &max_pos,
                           QVector<double> &prnu_eachcell);


double Non_UniformityTest(uchar *pData, int w, int h,
                          int w_cell, int h_cell,
                          double &n_uniform_eachcell_max, int &max_pos,
                          double &n_uniform_mirror_diff_eachcell_max, int &max_pos_mirror_diff,
                          double &n_uniform_updown_diff_eachcell_max, int &max_pos_updown_diff,
                          QVector<double> &n_uniform_eachcell,
                          QVector<double> &n_uniform_mirror_diff_eachcell,
                          QVector<double> &n_uniform_updown_diff_eachcell);


double Get_Real_ROI_PRNU (float *Delta, uchar *raw_data, int width, int height, int col, int line,  int win_w, int win_h);


int Find_2X4Cluster_in_diff_image_WF(uchar *in1_raw8,uchar *in2_raw8,
                                     int w,int h,
                                     double spec,
                                     std::string &g_str_msg_);


int find_row_defect_bychannel (Spec_Mode mode,
                               uchar *pData, int w, int h,
                               int r, int stripCnt,
                               double spec_line,
                               double spec_dol,
                               int &cnt_dol,
                               double &max_diff,
                               int &max_pos,
                               double &row_max_min_diff,
                               double &dol_std_max,
                               int &dol_std_max_pos,
                               double break_line_spec,
                               int &break_line_cnt,
                               double &break_line_max_value,
                               int &break_line_max_pos,
                               std::string &msg,
                               QVector<double> &mean_each_row,
                               QVector<double> &delta_each_row,
                               QVector<double> &diff_each_row);

int find_col_defect_bychannel  (Spec_Mode mode,
                                uchar *pData, int w, int h,
                                int r, int stripCnt,
                                double spec_line,
                                double spec_dol,
                                int &cnt_dol,
                                double &max_diff,
                                int &max_pos,
                                double &col_max_min_diff,
                                double &dol_std_max,
                                int &dol_std_max_pos,
                                double break_line_spec,
                                int &break_line_cnt,
                                double &break_line_max_value,
                                int &break_line_max_pos,
                                std::string &msg,
                                QVector<double> &mean_each_col,
                                QVector<double> &delta_each_col,
                                QVector<double> &diff_each_col);


int find_edge_row_defect_bychannel (Spec_Mode mode,
                                    uchar *pData, int w, int h,
                                    int r, int stripCnt,
                                    double spec_line,
                                    double spec_dol,
                                    int &cnt_dol,
                                    double &max_diff,
                                    int &max_pos,
                                    double &dol_std_max,
                                    int &dol_std_max_pos,
                                    std::string &msg);



int find_edge_col_defect_bychannel (Spec_Mode mode,
                                    uchar *pData, int w, int h,
                                    int r, int stripCnt,
                                    double spec_line,
                                    double spec_dol,
                                    int &cnt_dol,
                                    double &max_diff,
                                    int &max_pos,
                                    double &dol_std_max,
                                    int &dol_std_max_pos,
                                    std::string &msg);


int find_row_defect_bychannel_new (Spec_Mode mode,
                               uchar *pData, int w, int h,
                               int r, int stripCnt,
                               double spec_line,
                               double spec_dol,
                               int &cnt_dol,
                               double &max_diff,
                               int &max_pos,
                               double &row_max_min_diff,
                               double &dol_std_max,
                               int &dol_std_max_pos,
                               double break_line_spec,
                               int &break_line_cnt,
                               double &break_line_max_value,
                               int &break_line_max_pos,
                               std::string &msg,
                               QVector<double> &mean_each_row,
                               QVector<double> &delta_each_row,
                               QVector<double> &diff_each_row);

int find_col_defect_bychannel_new  (Spec_Mode mode,
                                uchar *pData, int w, int h,
                                int r, int stripCnt,
                                double spec_line,
                                double spec_dol,
                                int &cnt_dol,
                                double &max_diff,
                                int &max_pos,
                                double &col_max_min_diff,
                                double &dol_std_max,
                                int &dol_std_max_pos,
                                double break_line_spec,
                                int &break_line_cnt,
                                double &break_line_max_value,
                                int &break_line_max_pos,
                                std::string &msg,
                                QVector<double> &mean_each_col,
                                QVector<double> &delta_each_col,
                                QVector<double> &diff_each_col);

int find_txoff_col_defect_bychannel_new  (Spec_Mode mode,
                                          uchar *pData, int w, int h,
                                          int r, int stripCnt,
                                          double spec_line,
                                          double spec_dol,
                                          double spec_bright_line,
                                          double spec_dark_line,
                                          int &cnt_dol,
                                          double &max_diff,
                                          int &max_pos,
                                          double &col_max_min_diff,
                                          double &dol_std_max,
                                          int &dol_std_max_pos,
                                          double break_line_spec,
                                          int &break_line_cnt,
                                          double &break_line_max_value,
                                          int &break_line_max_pos,
                                          std::string &msg,
                                          QVector<double> &mean_each_col,
                                          QVector<double> &delta_each_col,
                                          QVector<double> &diff_each_col,
                                          int &cnt_bright_col,
                                          double &max_bright_col_diff,
                                          int &max_bright_col_pos,
                                          double &max_dark_col_diff,
                                          int &max_dark_col_pos);


int find_edge_row_defect_bychannel_new (Spec_Mode mode,
                                    uchar *pData, int w, int h,
                                    int r, int stripCnt,
                                    double spec_line,
                                    double spec_dol,
                                    int &cnt_dol,
                                    double &max_diff,
                                    int &max_pos,
                                    double &dol_std_max,
                                    int &dol_std_max_pos,
                                    std::string &msg);



int find_edge_col_defect_bychannel_new (Spec_Mode mode,
                                    uchar *pData, int w, int h,
                                    int r, int stripCnt,
                                    double spec_line,
                                    double spec_dol,
                                    int &cnt_dol,
                                    double &max_diff,
                                    int &max_pos,
                                    double &dol_std_max,
                                    int &dol_std_max_pos,
                                    std::string &msg);


int FindDeadRow_by_channel_add_breakline (Spec_Mode mode,
                                          uchar *pData, int w, int h,
                                          int r, int stripCnt,
                                          double spec_line,
                                          double spec_dol,
                                          int &cnt_dol,
                                          double &max_diff,
                                          int &max_pos,
                                          double &row_max_min_diff,
                                          double break_line_spec,
                                          int &break_line_cnt,
                                          double &break_line_max_value,
                                          int &break_line_max_pos,
                                          std::string &msg,
                                          QVector<double> &delta_each_row);


int FindDeadCol_by_channel_add_breakline  (Spec_Mode mode,
                                           uchar *pData, int w, int h,
                                           int r, int stripCnt,
                                           double spec_line,
                                           double spec_dol,
                                           int &cnt_dol,
                                           double &max_diff,
                                           int &max_pos,
                                           double &col_max_min_diff,
                                           double break_line_spec,
                                           int &break_line_cnt,
                                           double &break_line_max_value,
                                           int &break_line_max_pos,
                                           std::string &msg,
                                           QVector<double> &delta_each_col);

int FindDeadRow_by_channel (Spec_Mode mode,
                            uchar *pData, int w, int h,
                            int r, int stripCnt,
                            double spec_line,
                            double spec_dol,
                            int &cnt_dol,
                            double &max_diff,
                            int &max_pos,
                            double &row_max_min_diff,
                            std::string &msg);


int FindDeadCol_by_channel (Spec_Mode mode,
                            uchar *pData, int w, int h,
                            int r, int stripCnt,
                            double spec_line,
                            double spec_dol,
                            int &cnt_dol,
                            double &max_diff,
                            int &max_pos,
                            double &col_max_min_diff,
                            std::string &msg);


int Find_Edge_DeadRow_by_channel (Spec_Mode mode,
                                  uchar *pData, int w, int h,
                                  int r, int stripCnt,
                                  double spec_line,
                                  double spec_dol,
                                  int &cnt_dol,
                                  double &max_diff,
                                  int &max_pos,
                                  double &row_max_min_diff,
                                  std::string &msg);


int Find_Edge_DeadCol_by_channel (Spec_Mode mode,
                                  uchar *pData, int w, int h,
                                  int r, int stripCnt,
                                  double spec_line,
                                  double spec_dol,
                                  int &cnt_dol,
                                  double &max_diff,
                                  int &max_pos,
                                  double &col_max_min_diff,
                                  std::string &msg);


int FindDeadCol_bright_global_byDN (uchar *pData, int w, int h,
                                    int r, int stripCnt,
                                    double spec_line,
                                    double spec_dol,
                                    int &cnt_dol,
                                    double &max_diff,
                                    int &max_pos,
                                    std::string &msg);

int FindDeadRow_bright_global_byDN (uchar *pData, int w, int h,
                                    int r, int stripCnt,
                                    double spec,
                                    double &max_diff,
                                    int &max_pos,
                                    std::string &msg);

int getClusterInf(cv::Mat&src,std::vector<MClusterInf>&result,
                                  const int &sizeLimit,const int &sizeMax);

int FindPkG_WF(cv::Mat &image, const MPkgParam &param,std::string &msg);

int FindPkG_WF_1_1(cv::Mat &image, const MPkgParam &param,std::string &msg, std::vector<MClusterInf> &pd_all_single_particle);


int FindBlemsh_WF(cv::Mat &image, const MBlemishParam &param,std::string &msg);

int FindBlemsh_WF_NEW(cv::Mat &image, const MBlemishParam &param,std::string &msg, std::vector<MClusterInf> &pd_all_single_particle);

int getimage_ClusterInf(cv::Mat &image, const MBlemishParam &param, std::vector<MClusterInf> &particle ,std::string &msg);

int getimage_ClusterInf_enhace(cv::Mat &image, const MBlemishParam &param, std::vector<MClusterInf> &particle ,std::string &msg);


int FindALLClusterBayer_in_binary(uchar *binary_map, uchar *buf_map, int w, int h,
                                  QMap<int,int> &cnts_cluster,
                                  int &cnt_2x4_pix,
                                  int &cnt_2x2_pix,
                                  int &cnt_1x2_2x1_pix,
                                  int &cnt_2x2_diagonal_pix,
                                  int &cnt_1x2_ch,
                                  int &cnt_2x1_ch,
                                  int &cnt_2x2_leftdiagonal,
                                  int &cnt_2x2_rightdiagonal,
                                  int &cnt_white_pix,
                                  int &cnt_dark_pix,
                                  int &paticle,
                                  int &other_cluster,
                                  int &other_cluster_ch, std::string &g_str_msg_);


int Findblemish_in_blemish_binary(uchar *binary_map, int w, int h,
                                  int blemish_global_size_limit,
                                  int blemish_bychannel_size_limit,
                                  std::string &g_str_msg_);


int FindALLClusterBayer_WF(uchar *data, int w, int h,
                           QMap<int,int> &cnts_cluster,
                           int &cnt_2x4_pix,
                           int &cnt_2x2_pix,
                           int &cnt_1x2_2x1_pix,
                           int &cnt_2x2_diagonal_pix,
                           int &cnt_1x2_ch,
                           int &cnt_2x1_ch,
                           int &cnt_2x2_leftdiagonal,
                           int &cnt_2x2_rightdiagonal,
                           int &cnt_white_pix,
                           int &cnt_dark_pix,
                           int &paticle,
                           int &other_cluster, int &other_cluster_ch,
                           int r, double spec, int skipCnt,
                           uchar *buf_map, std::string &msg);


int FindALLClusterBayer_DF(uchar *data, int w, int h,
                           QMap<int,int> &cnts_cluster,
                           int &cnt_2x4_pix,
                           int &cnt_2x2_pix,
                           int &cnt_1x2_2x1_pix,
                           int &cnt_2x2_diagonal_pix,
                           int &cnt_1x2_ch,
                           int &cnt_2x1_ch,
                           int &cnt_2x2_leftdiagonal,
                           int &cnt_2x2_rightdiagonal,
                           int &cnt_white_pix,
                           int &cnt_dark_pix,
                           int &paticle,
                           int &other_cluster, int &other_cluster_ch,
                           int r, double spec, int skipCnt,
                           uchar *buf_map, std::string &g_str_msg_);



bool FindPOG(uchar *data, int skip, int w, int h,
             QMap<int,int> &POG,
             int r_kernel, int r_outer, double spec,
             std::string &g_str_msg_);

bool Find_wihtepix(uchar *data, int skip, int w, int h,
                   int wp_class1_spec, int wp_class2_spec,
                   int &wp_class1_cnt, int &wp_class2_cnt,
                   std::string &g_str_msg_);

int defectCounter_chao(uchar *map, int width, int height,
                   int spec, std::vector<MPDInf> &clusters,
                       std::vector<MPDInf> &clusters_ch);

int defectCounter(uchar *map, int width, int height,
                   int spec, std::vector<MPDInf> &clusters);

int defectCounter_channel(uchar *map, int width, int height,
                          int spec, std::vector<MPDInf> &clusters);

double TestRelativeIlumination(uchar *pData, int w, int h, int w_win,
                               int h_win);
void TestColorShading(uchar *pR, uchar *pG, uchar *pB, int w, int h,
                      int w_win, int h_win, std::vector<double> &ratios);

template<typename T>
double GetDatasStd(std::vector<T> &datas);

template<typename T>
double GetDatasStd(std::vector<T> &datas,double mean_t);

template<typename T>
void RGGB_Map_Integrate_chao(T*g_binary_r_, T*g_binary_gr_, T*g_binary_gb_,
                             T*g_binary_b_, T*g_binary_, int w, int h, RGB_BAYER bayer);

double GetDatasStd_Qvector(QVector<double> &datas);

double GetDatasStdandMean(QVector <int> &datas, double &mean);

double Get_Double_DatasStdandMean(QVector <double> &datas, double &mean);

void bychannel_MapBinarization (const uchar *in_raw8, uchar *out_raw8, int width, int height, int skip_cnt, int radius, double binary_spec, Spec_Mode mode);

void bychannel_MapBinarization_delta (const uchar *in_raw8, uchar *out_raw8, int width, int height, int skip_cnt, int radius,
                                      double binary_spec, Spec_Mode mode, double *delta_map);


void bychannel_MapBinarization_add_blemish (const uchar *in_raw8, uchar *out_raw8, uchar *out_blemish, int width, int height, int skip_cnt,
                                            int radius, double binary_spec, double blemish_spec, Spec_Mode mode);


void bychannel_pixel_MapBinarization (const uchar *in_raw8, float *delta_data, uchar *out_raw8, uchar *out_blemish, int width, int height,
                                      int skip_cnt, int radius, double binary_spec, double blemish_spec, Spec_Mode mode);

void Flicker_Pix_Binarization_Doublebyte (uchar *pRaw16_in, uchar *binar_out_raw8, int framecnt, int width, int height, double binary_spec);

void Flicker_Pix_Binarization_Doublebyte_chao (uchar *pRaw16_in, uchar *binar_out_raw8, int framecnt, int width,int height, int y_start, double binary_spec);

void Get_STD_MAP_Doublebyte (uchar *pRaw16_in, double *std_out, int framecnt, int width, int height);

void Get_STD_MAP_Doublebyte_chao (uchar *pRaw16_in, double *std_out, int framecnt, int width, int height, int y_start);

void RGGB_Map_Integrate_4C (uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_, int w, int h, RGB_BAYER bayer);

void RGGB_Map_Integrate_Raw16 (uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                               uchar *g_buf_raw_16_, int w, int h, RGB_BAYER bayer);


void DarkShadingTest(uchar *pData, int w, int h,
                     int x_cnt, int y_cnt,
                     double &ob_eachcell_max, double &ob_eachcell_min,
                     int &max_pos, int &min_pos, QVector<double> &mean_ob_eachcell,
                     std::string &msg);

void getColorRatioByRect(uchar *pR, uchar *pG1, uchar *pG2,
                         uchar *pB, int w, int h,
                         int x, int y, int w_rect, int h_rect,
                         double &g_g, double &r_g, double &b_g, double &gr_gb_diff);

void get_roi_colorratio (uchar *pR, uchar *pG1, uchar *pG2, uchar *pB,
                         int w, int h, int x, int y,int w_rect, int h_rect,
                         double &g_g, double &r_g, double &b_g, double &gr_gb_diff);

void get_roi_colorratio_5X5 (uchar *pR, uchar *pG1, uchar *pG2, uchar *pB,
                         int w, int h, int x, int y,int w_rect, int h_rect,
                         double &g_g, double &r_g, double &b_g, double &gr_gb_diff);

void get_roi_otp_channel_gain(double mean_r_roi, double mean_gr_roi, double mean_gb_roi, double mean_b_roi,
                                              double R_G_typical_roi, double B_G_typical_roi,
                                              double &R_channel_gain_final, double &Gr_channel_gain_final,
                                              double &Gb_channel_gain_final, double &B_channel_gain_final,
                                              int roi_no, std::string &msg);

void get_x_percent_corresponding_DN_number_raw16(short *data, int w, int h, double x, int &x_pos_dn, std::string &msg);

void get_x_percent_corresponding_DN_number_raw8(uchar *data, int w, int h, double x, int &x_pos_dn,
                                                int roi_x, int roi_y, int roi_w, int roi_h,
                                                int *histogram, std::string &msg);

void get_x_percent_corresponding_DN_number_short(short *data, int w, int h, double x, int &x_pos_dn,
                                                 int roi_x, int roi_y, int roi_w, int roi_h,
                                                 int *histogram, std::string &msg);

//------------------------------------------
void bbb1(unsigned short *in, int image_width, unsigned int *OTP_DD, int total_number,int col_length,int row_length);
void bbb2(unsigned int *OTP_DD, int image_height, int image_width, int total_number,
                                        int mirror_flag, int updown_flag, int crop_height, int crop_width,int col_length,int row_length);

int bbb7(int *first_max, int *first_min, int *second_max, int *second_min, int *mat4_flag, int op[12]);

void bbb8(int *first_max, int op[12]);

void bbb9(int *first_min, int op[12]);

void bbb10(short**raw, int cfa_in, int image_height, int image_width);

void bbb11(unsigned short *in, short **out, int col_start, int block_w, int width, int height, short black_level, int padding_flag);

void bbb12(short **in, unsigned short *out, int block_w,int block_h,int col_start, int width, int height, short black_level);

void bbb13(short **in, short ** grad, int offset_x, int offset_y, int width, int height, int flat_th, int cdiff_grad_level);

void bbb14(short **in, short ** grad, int offset_x, int offset_y, int width, int height);

void bbb15(short **in, short **grad, short **out, int offset_x, int offset_y, int width, int height, int cfa, int fc_en, int fc_level);

short bbb16(short a1,short a2,short a3,short a4,short a5);

void bbb17(short **in, short **g, short **cd, int offset_x, int offset_y, int width, int height);

void bbb18(short **in, short **grad, int width, int height);

void bbb19(short **in, int cfa, int width, int height, int r_gain, int gr_gain, int gb_gain, int b_gain);

void bbb20(paramThreadScaler *para);

void *bbb21(void *args);

void bbb3(unsigned char *gainmap_down, unsigned char *I_out, int width_out, int height_out, int width_in, int height_in);

void bbb4(unsigned char *in, unsigned char **out, int col_start, int block_w, int width, int height, int padding_flag, int crosstalk_otp_en);

void bbb5(unsigned char *gain_down, unsigned char *gain_down_temp, int Mirror, int upsidedown, int CFAin, int CFAout, int samplerate_height, int samplerate_width);

void bbb22(short **in, short **grad, unsigned char **crosstalk_gain, int cfa, int width, int height, int xtalk_static_en, int xtalk_dynamic_en, int ge_th, int MIX_coeff, int ge_slope, int ge_str);

void bbb23(short **in, int cfa, int offset_x, int offset_y, int width, int height, int r_gain, int gr_gain, int gb_gain, int b_gain);

void bbb24(paramThread *para);

void *bbb6(void *args);

//------------4C Cell Xtalk-------------------

void aaa1(double **dat, double *d, int count);

int aaa2(double *data, int count, double *Answer);

int aaa3(double*data, int rows, int cols, int med_w, int xy_density, double*Answer);

double ***aaa4(int height, int width, int num, int *malloc_flag);

double **aaa5(int height, int width, int *malloc_flag);

int aaa6(int *a);

double **aaa7(int *standard_pic, int height, int width, int *malloc_flag);

double **aaa8(double **middle_pic, int height, int width, int size_of_mean_window, int *malloc_flag, std::string &msg);

double ***aaa9(double **final_pic, int height, int width, char *pattern, int *malloc_flag, std::string &msg);

void aaa10(double *in_map_flow, double *out_map_flow, int num_one_time, int interval, double *b, double B, int *malloc_flag);

double ***aaa11(double ***gain_map, int height, int width, int *malloc_flag);

double ***aaa12(double ***gain_map, int height, int width, int point_height, int point_width, int number_of_point_in_one_area, int *malloc_flag);

double ***aaa13(double ***downsampled_map, int point_height, int point_width, int number_of_point_in_one_area, int *malloc_flag);

void aaa14(double ***bezier_point_map, int height, int width, int rate_of_quantization, uint8_t* result);

void aaa15(uint8_t *in, float *out, int height_in, int width_in, int height_out, int width_out);


void bychannel_pixel_chao (const uchar *in_raw8, float *delta_data, uchar *out_raw8,
                           uchar *out_blemish, int width, int height,int skip_cnt,
                           int radius, double binary_spec,double blemish_spec, Spec_Mode mode);

}


//1.构造函数RGBshading(const int &w,const int &h):width(w),height(h){}图像的长宽
//2.成员函数void getROI（const cv::Rect&r）获取四个矩形框
//3.void process(const uchar*data,double &LCU_R,double&LCU_B)const输入数据，输出LCU_R，LCU_B
class RGBshading
{
private:
    std::vector<cv::Rect>roi;
    const int &width;
    const int &height;
public:
    RGBshading(const int &w,const int &h):width(w),height(h){}
    void getROI(const cv::Rect&r);
    void process(const uchar*data,double &LCU_R,double&LCU_B)const;
};

#endif // CPALGORITHMYUV_H
