#ifndef TESTITEMS_H
#define TESTITEMS_H

#include "multithread.h"

typedef unsigned char BYTE;
typedef unsigned char uchar;

namespace TestItem {

void distribute_pointer_by_RGB_BAYER(RGB_BAYER bayer,uchar **pb1 ,uchar **pb2,uchar **pb3,uchar **pb4,
                                     uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_);

void split_channel(RGB_BAYER bayer, int w, int h, uchar *g_buf_raw_, uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_);

void split_channel_4C(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_, uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_);


void split_channel_raw16_4C(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                            uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                            int raw_bits = 10);

void split_4C_same_channel_1_to_4_raw8(int w, int h, uchar *g_buf_, uchar *g_buf_1_, uchar *g_buf_2_, uchar *g_buf_3_, uchar *g_buf_4_);

void split_4C_same_channel_1_to_4_raw16(int w, int h, uchar *g_buf_, uchar *g_buf_1_, uchar *g_buf_2_, uchar *g_buf_3_, uchar *g_buf_4_,
                                      int raw_bits = 10);


void split_channel_raw16(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                         uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                         int raw_bits = 10);

void split_channel_short(RGB_BAYER bayer,int w, int h, short *g_buf_raw_, short *g_buf_r_, short *g_buf_gr_, short *g_buf_gb_, short *g_buf_b_);


void split_channel_raw16_multiframe(RGB_BAYER bayer,int w, int h, int frame_cnt, uchar *g_buf_raw_16_,
                                    uchar *g_buf_r_16_multiframe_, uchar *g_buf_gr_16_multiframe_,
                                    uchar *g_buf_gb_16_multiframe_, uchar *g_buf_b_16_multiframe_,
                                    int raw_bits = 10);


void split_channel_raw10_to_raw16_multiframe_4C(RGB_BAYER bayer,int width, int height, int frame_cnt, uchar *g_buf_raw_10_,
                                                          uchar *g_buf_r_16_multiframe_, uchar *g_buf_gr_16_multiframe_,
                                                          uchar *g_buf_gb_16_multiframe_, uchar *g_buf_b_16_multiframe_);

void split_channel_raw10_to_raw16_multiframe(RGB_BAYER bayer,int width, int height, int frame_cnt, uchar *g_buf_raw_10_,
                                             uchar *g_buf_r_16_multiframe_, uchar *g_buf_gr_16_multiframe_,
                                             uchar *g_buf_gb_16_multiframe_, uchar *g_buf_b_16_multiframe_);

void split_channel_raw16_to_raw8(RGB_BAYER bayer, int w, int h, uchar *g_buf_raw_16_,
                                 uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                                 uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_,
                                 int raw_bits = 10);

void split_channel_raw16_to_low_raw8(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                                     uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_);

void split_channel_raw10_to_raw8(RGB_BAYER bayer, int w, int h, uchar *g_buf_raw_10_, uchar *g_buf_raw_10_buf_,
                                 uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_);

void split_channel_raw10_to_raw8_4C(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_10_, uchar *g_buf_raw_10_buf_,
                                    uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_);

void split_channel_raw10_to_raw8_low8(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_10_, uchar *g_buf_raw_10_buf_,
                                      uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_);

void split_channel_raw16_to_raw8_4C(RGB_BAYER bayer,int w, int h, uchar *g_buf_raw_16_,
                                    uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                                    uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, uchar *g_buf_raw_,
                                    int raw_bits = 10);

void test_channel_colmean_max_min(int width, int height,double &mean_r_max, double &mean_gr_max, double &mean_gb_max, double &mean_b_max,
                                            double &mean_r_min, double &mean_gr_min, double &mean_gb_min, double &mean_b_min,
                                            int &mean_r_max_pos, int &mean_gr_max_pos, int &mean_gb_max_pos, int &mean_b_max_pos,
                                            int &mean_r_min_pos, int &mean_gr_min_pos, int &mean_gb_min_pos, int &mean_b_min_pos,
                                            double &pixel_r_max,double &pixel_gr_max,double &pixel_gb_max,double &pixel_b_max,double &mean_center_left,double &mean_center_right,
                                            uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_);

void test_channel_mean(int w, int h, double &mean_r, double &mean_gr, double &mean_gb, double &mean_b,
                       uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_);

void test_channel_linearity_col_ushort(int width, int height, int win_x, int win_y, int win_w, int win_h, double* linearity, double max_dn,
                                       uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_);

void test_roi_channel_mean(int width, int height, int win_x, int win_y, int win_w, int win_h, double &mean_r, double &mean_gr, double &mean_gb, double &mean_b,
                           uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_);

void test_channel_mean_raw16(int w, int h, double &mean_r, double &mean_gr, double &mean_gb, double &mean_b,
                             uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_);

void test_roi_channel_mean_raw16(int width, int height, int win_x, int win_y, int win_w, int win_h,
                                 double &mean_r, double &mean_gr, double &mean_gb, double &mean_b,
                                 uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_);

void test_channel_prnu(int w, int h, int w_cell, int h_cell,
                       double &prnu_r, double &prnu_gr, double &prnu_gb, double &prnu_b,
                       uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_);


void test_real_roi_prnu(int width, int height, int win_x, int win_y, int win_w, int win_h,
                        double &prnu_r, double &prnu_gr, double &prnu_gb, double &prnu_b,
                        float *g_delta_r_, float *g_delta_gr_, float *g_delta_gb_, float *g_delta_b_,
                        uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_);


void test_channel_uniformity_max(int w, int h, int w_cell, int h_cell,
                                 double &prnu_r, double &prnu_gr, double &prnu_gb, double &prnu_b,
                                 double &prnu_r_max, double &prnu_gr_max, double &prnu_gb_max, double &prnu_b_max,
                                 int &r_max_pos, int &gr_max_pos, int &gb_max_pos, int &b_max_pos,
                                 uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, std::string &msg);


void test_non_uniformity_bychannel(int w, int h, int w_cell, int h_cell,
                                   double &n_uniform_r, double &n_uniform_gr, double &n_uniform_gb, double &n_uniform_b,
                                   double &n_uniform_r_max, double &n_uniform_gr_max,
                                   double &n_uniform_gb_max, double &n_uniform_b_max,
                                   int &r_max_pos, int &gr_max_pos, int &gb_max_pos, int &b_max_pos,
                                   double &n_uniform_mirror_diff_r_max, double &n_uniform_mirror_diff_gr_max,
                                   double &n_uniform_mirror_diff_gb_max, double &n_uniform_mirror_diff_b_max,
                                   int &mirror_diff_r_max_pos, int &mirror_diff_gr_max_pos,
                                   int &mirror_diff_gb_max_pos, int &mirror_diff_b_max_pos,
                                   double &n_uniform_updown_diff_r_max, double &n_uniform_updown_diff_gr_max,
                                   double &n_uniform_updown_diff_gb_max, double &n_uniform_updown_diff_b_max,
                                   int &updown_diff_r_max_pos, int &updown_diff_gr_max_pos,
                                   int &updown_diff_gb_max_pos, int &updown_diff_b_max_pos,
                                   uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                   std::string &msg, int flag_log_print);


void test_colorratio(double mean_gr, double mean_gb, int w, int h,
                     int pos_x, int pos_y, int w_rect, int h_rect,
                     uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                     QMap <QPair<QString,double>,double> &GG_ratio,
                     QMap <QPair<QString,double>,double> &RG_ratio,
                     QMap <QPair<QString,double>,double> &BG_ratio,
                     QMap <QString,double> &G_G_diff,
                     QMap <QString,double> &GG_Ratio_target,
                     QMap <QString,double> &RG_Ratio_target,
                     QMap <QString,double> &BG_Ratio_target);

void test_MxN_roi_std_mean(uchar *g_buf_raw_, int width, int height,
                           int skip_col, int skip_row, int Col_M, int Row_N,
                           QVector <double> &mean, QVector <double> &std);

void test_roi_channel_pixel_percent_DN_raw8(int width, int height, cv::Rect &roi,
                                            uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                            double r_x_percent, double gr_x_percent, double gb_x_percent, double b_x_percent,
                                            int &r_x_dn, int &gr_x_dn, int &gb_x_dn, int &b_x_dn,
                                            int log_print, std::string &msg);

void test_roi_channel_pixel_percent_DN_short(int width, int height, cv::Rect &roi,
                                             short *g_buf_r_, short *g_buf_gr_, short *g_buf_gb_, short *g_buf_b_,
                                             double r_x_percent, double gr_x_percent, double gb_x_percent, double b_x_percent,
                                             int &r_x_dn,int &gr_x_dn,int &gb_x_dn,int &b_x_dn,
                                             int log_print, std::string &msg);

void test_roi_channel_pixel_percent_DN_raw16(int width, int height, cv::Rect &roi,
                                             short *g_buf_r_, short *g_buf_gr_, short *g_buf_gb_, short *g_buf_b_,
                                             double r_x_percent, double gr_x_percent, double gb_x_percent, double b_x_percent,
                                             int &r_x_dn,int &gr_x_dn,int &gb_x_dn,int &b_x_dn, std::string &msg);

void test_deadrow_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                            int w, int h, int skip, int radius, double spec_dol,
                            double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                            double &max_row_diff_R, double &max_row_diff_Gr, double &max_row_diff_Gb, double &max_row_diff_B,
                            int &max_row_pos, double &row_max_min_diff, int &cnt_dead_row, int &cnt_dol, Spec_Mode mode, std::string &msg);

void test_deadcol_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                            int w, int h, int skip, int radius, double spec_dol,
                            double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                            double &max_col_diff_R, double &max_col_diff_Gr, double &max_col_diff_Gb, double &max_col_diff_B,
                            int &max_col_pos, double &col_max_min_diff, int &cnt_dead_col, int &cnt_dol, Spec_Mode mode, std::string &msg);


void test_edge_deadrow_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                 int w, int h, int skip, int radius, double spec_dol,
                                 double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                 double &max_row_diff_R, double &max_row_diff_Gr, double &max_row_diff_Gb, double &max_row_diff_B,
                                 int &max_row_pos, double &row_max_min_diff, int &cnt_dead_row, int &cnt_dol, Spec_Mode mode, std::string &msg);


void test_edge_deadcol_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                 int w, int h, int skip, int radius, double spec_dol,
                                 double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                 double &max_col_diff_R, double &max_col_diff_Gr, double &max_col_diff_Gb, double &max_col_diff_B,
                                 int &max_col_pos, double &col_max_min_diff,int &cnt_dead_col, int &cnt_dol, Spec_Mode mode, std::string &msg);


void find_cluster(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                  uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                  int w, int h, int skip, int radius,
                  double spec_r, double spec_gr, double spec_gb, double spec_b,
                  Spec_Mode mode, RGB_BAYER bayer,
                  QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                  int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                  int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                  int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                  int &other_cluster_ch, int &total_cluster_cnt, uchar *buf_map, std::string &g_str_msg_);

void find_cluster_delta(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                        double *g_buf_delta_r_,double *g_buf_delta_gr_,double *g_buf_delta_gb_,double *g_buf_delta_b_, double *g_buf_delta_raw_,
                        uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                        int w, int h, int skip, int radius,
                        double spec_r, double spec_gr, double spec_gb, double spec_b,
                        Spec_Mode mode, RGB_BAYER bayer,
                        QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                        int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                        int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                        int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                        int &other_cluster_ch, int &total_cluster_cnt, uchar *buf_map, std::string &g_str_msg_);

//void find_cluster_and_blemish(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
//                              uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
//                              uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
//                              int w, int h, int skip, int radius,
//                              double spec_r, double spec_gr, double spec_gb, double spec_b,
//                              double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
//                              Spec_Mode mode, RGB_BAYER bayer,
//                              QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
//                              int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
//                              int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
//                              int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
//                              int &other_cluster_ch, int &total_cluster_cnt, int &blemish, uchar *buf_map, std::string &g_str_msg_);


void find_all_single_pixel_defect(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                              float *g_buf_delta_r_,float *g_buf_delta_gr_,float *g_buf_delta_gb_,float *g_buf_delta_b_, float *g_buf_delta_raw_,
                              uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                              uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
                              int w, int h, int skip, int radius,
                              double spec_r, double spec_gr, double spec_gb, double spec_b,
                              double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
                              Spec_Mode mode, RGB_BAYER bayer, int save_imgae, std::string &g_str_msg_, std::vector<MClusterInf> &pd_all_single_particle);



void find_pixel_defect(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                       float *g_buf_delta_r_,float *g_buf_delta_gr_,float *g_buf_delta_gb_,float *g_buf_delta_b_, float *g_buf_delta_raw_,
                       uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                       uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
                       int w, int h, int skip, int radius,
                       double spec_r, double spec_gr, double spec_gb, double spec_b,
                       double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
                       Spec_Mode mode, RGB_BAYER bayer,
                       QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                       int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                       int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                       int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                       int &other_cluster_ch, int &total_cluster_cnt,
                       int blemish_global_size_limit, int blemish_bychannel_size_limit,
                       int &blemish,int &cnt_white_pix_blemish, int &cnt_dark_pix_blemish,
                       uchar *buf_map, int save_imgae, std::string &g_str_msg_);


void find_channel_single_white_dark_pixel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                       float *g_buf_delta_r_,float *g_buf_delta_gr_,float *g_buf_delta_gb_,float *g_buf_delta_b_, float *g_buf_delta_raw_,
                                       uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                                       uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
                                       int w, int h, int skip, int radius,
                                       double spec_r, double spec_gr, double spec_gb, double spec_b,
                                       double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
                                       Spec_Mode mode, RGB_BAYER bayer,
                                       QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                                       int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                                       int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                                       int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                                       int &other_cluster_ch, int &total_cluster_cnt,
                                       int blemish_global_size_limit, int blemish_bychannel_size_limit,
                                       int &blemish,int &cnt_white_pix_blemish, int &cnt_dark_pix_blemish,
                                       uchar *buf_map, int save_imgae, std::string &g_str_msg_,
                                       int ROI_Channel_X,int ROI_Channel_Y,int ROI_Channel_W,int ROI_Channel_H,
                                       int &cnt_dark_pix_r, int &cnt_dark_pix_gr, int &cnt_dark_pix_gb, int &cnt_dark_pix_b,
                                       int &cnt_white_pix_r, int &cnt_white_pix_gr, int &cnt_white_pix_gb, int &cnt_white_pix_b,
                                       int &crop_cnt_dark_pix_r, int &crop_cnt_dark_pix_gr, int &crop_cnt_dark_pix_gb, int &crop_cnt_dark_pix_b,
                                       int &crop_cnt_white_pix_r, int &crop_cnt_white_pix_gr, int &crop_cnt_white_pix_gb, int &crop_cnt_white_pix_b);


void find_dark_and_white_pixel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                               float *g_buf_delta_r_,float *g_buf_delta_gr_,float *g_buf_delta_gb_,float *g_buf_delta_b_, float *g_buf_delta_raw_,
                               uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_, uchar *g_binary_b_, uchar *g_binary_,
                               uchar *g_blemish_r_, uchar *g_blemish_gr_, uchar *g_blemish_gb_, uchar *g_blemish_b_, uchar *g_blemish_,
                               int w, int h, int skip, int radius,
                               double spec_r, double spec_gr, double spec_gb, double spec_b,
                               double spec_blemish_r, double spec_blemish_gr, double spec_blemish_gb, double spec_blemish_b,
                               Spec_Mode mode, RGB_BAYER bayer,
                               QMap<int,int> &cnts_cluster, int &cnt_2x4_pix, int &cnt_2x2_pix,
                               int &cnt_1x2_2x1_pix, int &cnt_2x2_diagonal_pix, int &cnt_1x2_ch,
                               int &cnt_2x1_ch, int &cnt_2x2_leftdiagonal, int &cnt_2x2_rightdiagonal,
                               int &cnt_white_pix, int &cnt_dark_pix, int &paticle, int &other_cluster,
                               int &other_cluster_ch, int &total_cluster_cnt,
                               int blemish_global_size_limit, int blemish_bychannel_size_limit,
                               int &blemish,int &cnt_white_pix_blemish, int &cnt_dark_pix_blemish,
                               uchar *buf_map, int save_imgae, std::string &g_str_msg_);

void test_channel_darkshading(int w, int h, int x_cnt, int y_cnt,
                              double &mean_ob_r_max, double &mean_ob_gr_max, double &mean_ob_gb_max, double &mean_ob_b_max,
                              double &mean_ob_r_min, double &mean_ob_gr_min, double &mean_ob_gb_min, double &mean_ob_b_min,
                              int &r_max_ob_pos, int &gr_max_ob_pos, int &gb_max_ob_pos, int &b_max_ob_pos,
                              int &r_min_ob_pos, int &gr_min_ob_pos, int &gb_min_ob_pos, int &b_min_ob_pos,
                              uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_, std::string &msg);

void test_deadrow_bychannel_breakline(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                      int w, int h, int skip, int radius, double spec_dol,
                                      double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                      double &max_row_diff_R, double &max_row_diff_Gr, double &max_row_diff_Gb, double &max_row_diff_B,
                                      int &max_row_pos, double &row_max_min_diff, int &cnt_dead_row, int &cnt_dol,
                                      double breakline_row_spec_r, double breakline_row_spec_gr,
                                      double breakline_row_spec_gb, double breakline_row_spec_b,
                                      int &breakline_row_cnt, double &breakline_row_max_value,
                                      int &breakline_row_max_pos, Spec_Mode mode, std::string &msg);

void test_deadcol_bychannel_breakline(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                      int w, int h, int skip, int radius, double spec_dol,
                                      double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                      double &max_col_diff_R, double &max_col_diff_Gr, double &max_col_diff_Gb, double &max_col_diff_B,
                                      int &max_col_pos, double &col_max_min_diff, int &cnt_dead_col, int &cnt_dol,
                                      double breakline_col_spec_r, double breakline_col_spec_gr,
                                      double breakline_col_spec_gb, double breakline_col_spec_b,
                                      int &breakline_col_cnt, double &breakline_col_max_value,
                                      int &breakline_col_max_pos, Spec_Mode mode, std::string &msg);

void test_row_defect_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                               int w, int h, int skip, int radius,
                               double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                               double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                               double *max_row_diff, int *max_row_pos, double *row_max_min_diff,
                               int &cnt_dead_row, int &cnt_dol, double *dol_std_max, int *dol_std_max_pos,
                               double breakline_row_spec_r, double breakline_row_spec_gr,
                               double breakline_row_spec_gb, double breakline_row_spec_b,
                               int &breakline_row_cnt, double *breakline_row_max_value,
                               int *breakline_row_max_pos, Spec_Mode mode, std::string &msg,
                               int log_print_flag);

void test_col_defect_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                               int w, int h, int skip, int radius,
                               double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                               double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                               double *max_col_diff, int *max_col_pos, double *col_max_min_diff,
                               int &cnt_dead_col, int &cnt_dol, double *dol_std_max, int *dol_std_max_pos,
                               double breakline_col_spec_r, double breakline_col_spec_gr,
                               double breakline_col_spec_gb, double breakline_col_spec_b,
                               int &breakline_col_cnt, double *breakline_col_max_value,
                               int *breakline_col_max_pos, Spec_Mode mode, std::string &msg,
                               int log_print_flag);

void test_col_defect_bychannel_bright_dark(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                           int w, int h, int skip, int radius,
                                           double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                                           double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                           double spec_bright_R, double spec_bright_Gr, double spec_bright_Gb, double spec_bright_B,
                                           double spec_dark_R, double spec_dark_Gr, double spec_dark_Gb, double spec_dark_B,
                                           double *max_col_diff, int *max_col_pos, double *col_max_min_diff,
                                           int &cnt_dead_col, int &cnt_dol, double *dol_std_max, int *dol_std_max_pos,
                                           double breakline_col_spec_r, double breakline_col_spec_gr,
                                           double breakline_col_spec_gb, double breakline_col_spec_b,
                                           int &breakline_col_cnt, double *breakline_col_max_value,
                                           int *breakline_col_max_pos, Spec_Mode mode, std::string &msg,
                                           int log_print_flag,double *max_bright_col_diff, int *max_bright_col_pos,double *max_dark_col_diff, int *max_dark_col_pos);

void test_edge_row_defect_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                    int w, int h, int skip, int radius,
                                    double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                                    double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                    double *max_row_diff, int *max_row_pos, int &cnt_dead_row, int &cnt_dol,
                                    double *dol_std_max, int *dol_std_max_pos, Spec_Mode mode, std::string &msg);


void test_edge_col_defect_bychannel(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                                    int w, int h, int skip, int radius,
                                    double spec_dol_R, double spec_dol_Gr, double spec_dol_Gb, double spec_dol_B,
                                    double spec_R, double spec_Gr, double spec_Gb, double spec_B,
                                    double *max_col_diff, int *max_col_pos, int &cnt_dead_col, int &cnt_dol,
                                    double *dol_std_max, int *dol_std_max_pos, Spec_Mode mode, std::string &msg);

void test_roi_colorratio(double mean_gr, double mean_gb, int w, int h,
                         int skip, int roi_x_cnt, int roi_y_cnt,
                         uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                         QVector<double> &roi_rg_ratio,
                         QVector<double> &roi_bg_ratio,
                         QVector<double> &roi_gg_ratio,
                         QMap <QPair<QString,double>,double> &GG_ratio,
                         QMap <QPair<QString,double>,double> &RG_ratio,
                         QMap <QPair<QString,double>,double> &BG_ratio,
                         QMap <QString,double> &G_G_diff,
                         QMap <QString,double> &GG_Ratio_target,
                         QMap <QString,double> &RG_Ratio_target,
                         QMap <QString,double> &BG_Ratio_target);

void test_roi_colorratio_5X5(double mean_gr, double mean_gb, int w, int h,
                         int skip, int roi_x_cnt, int roi_y_cnt,
                         uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,
                         QVector<double> &roi_rg_ratio,
                         QVector<double> &roi_bg_ratio,
                         QVector<double> &roi_gg_ratio,
                         QMap <QPair<QString,double>,double> &GG_ratio,
                         QMap <QPair<QString,double>,double> &RG_ratio,
                         QMap <QPair<QString,double>,double> &BG_ratio,
                         QMap <QString,double> &G_G_diff,
                         QMap <QString,double> &GG_Ratio_target,
                         QMap <QString,double> &RG_Ratio_target,
                         QMap <QString,double> &BG_Ratio_target);

void test_roi_ob_bychannel(int width, int height, int REF,
                           QVector<cv::Rect> &roi,
                           QVector<double> &ob_r_roi,
                           QVector<double> &ob_gr_roi,
                           QVector<double> &ob_gb_roi,
                           QVector<double> &ob_b_roi,
                           uchar *g_buf_r_, uchar *g_buf_gr_,
                           uchar *g_buf_gb_, uchar *g_buf_b_);

void test_roi_shading_to_center_bychannel(int width, int height,
                                          QVector<cv::Rect> &roi,
                                          QVector<double> &ob_r_roi,
                                          QVector<double> &ob_gr_roi,
                                          QVector<double> &ob_gb_roi,
                                          QVector<double> &ob_b_roi,
                                          double &center_mean_r,    // ★ 新增
                                          double &center_mean_gr,   // ★ 新增
                                          double &center_mean_gb,   // ★ 新增
                                          double &center_mean_b,    // ★ 新增
                                          uchar *g_buf_r_, uchar *g_buf_gr_,
                                          uchar *g_buf_gb_, uchar *g_buf_b_);



int test_pixel_cluster_in_smooth_image(cv::Mat srcImage, int resize_w, int resize_h, double lsc_ratio,
                                       int r, int skip, double spec, int defect_size_limit,
                                       Spec_Mode mode, std::string &msg, int save_image);


int TestFunc_LSC(uchar *g_buf_r_, uchar *g_buf_gr_, uchar *g_buf_gb_, uchar *g_buf_b_,int w, int h, int skip,
    int roi_x_cnt, int roi_y_cnt,
    int set_x_index, int set_y_index,
    int PixelBitCnt, bool debugmode,
    double &out_DiffCorner, double &out_Maxdiff,
    std::string &msg);


}
#endif // TESTITEMS_H
