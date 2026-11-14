#include "multithread.h"


MultiThread::get_channel_mean::get_channel_mean(){}

void MultiThread::get_channel_mean::setparam(uchar *in_data, int width, int height)
{
    this->data = in_data;
    this->w = width;
    this->h = height;
}

void MultiThread::get_channel_mean::run()
{
    this->mean=AlgorithmFuncs::GetChannelMean(this->data, this->w, this->h);
}

MultiThread::get_roi_channel_col_mean_min::get_roi_channel_col_mean_min(){}
void MultiThread::get_roi_channel_col_mean_min::setparam(uchar *in_data, int width, int height)
{
    this->data = in_data;
    this->w = width;
    this->h = height;

}

void MultiThread::get_roi_channel_col_mean_min::run()
{
    AlgorithmFuncs::GetROIMean_col_min(this->data, this->w, this->h,this->mean_max,this->mean_min,this->pixel_max,this->mean_center_left,this->mean_center_right,this->max_pos,this->min_pos);
}



MultiThread::get_roi_channel_mean::get_roi_channel_mean(){}

void MultiThread::get_roi_channel_mean::setparam(uchar *in_data, int width, int height, int x0, int y0, int win_width, int win_height)
{
    this->data = in_data;
    this->w = width;
    this->h = height;
    this->win_x = x0;
    this->win_y = y0;
    this->win_w = win_width;
    this->win_h = win_height;
}

void MultiThread::get_roi_channel_mean::run()
{
    this->mean=AlgorithmFuncs::GetROIMean(this->data, this->w, this->h, this->win_x, this->win_y, this->win_w, this->win_h);
}


MultiThread::get_mutil_roi_linearity_col_ushort::get_mutil_roi_linearity_col_ushort(){}

void MultiThread::get_mutil_roi_linearity_col_ushort::setparam(ushort *in_data, int width, int height, int x0, int y0, int win_width, int win_height, double* linear_, double max_dn_)

{
    this->data = in_data;
    this->w = width;
    this->h = height;
    this->win_x = x0;
    this->win_y = y0;
    this->win_w = win_width;
    this->win_h = win_height;
    this->linear = linear_;
    this->max_dn = max_dn_;
}

void MultiThread::get_mutil_roi_linearity_col_ushort::run()
{
    bool result = AlgorithmFuncs::Get_ROILinearity_Col_Ushort (this->data, this->w, this->h, this->win_x, this->win_y,  this->win_w, this->win_h, this->linear, this->max_dn);
}


MultiThread::get_mutil_roi_mean_short::get_mutil_roi_mean_short(){}

void MultiThread::get_mutil_roi_mean_short::setparam(short *in_data_H, short *in_data_L, short *in_data_Diff, int width, int height, int x0, int y0, int win_width, int win_height)

{
    this->data_H = in_data_H;
    this->data_L = in_data_L;
    this->data_Diff = in_data_Diff;
    this->w = width;
    this->h = height;
    this->win_x = x0;
    this->win_y = y0;
    this->win_w = win_width;
    this->win_h = win_height;
    this->value_log.clear();
}

void MultiThread::get_mutil_roi_mean_short::run()
{
    bool result =AlgorithmFuncs::Get_Multi_ROI_Mean_short (this->data_H, this->data_L, this->data_Diff, this->w, this->h,
                                                         this->win_x, this->win_y,  this->win_w, this->win_h, this->value_log);
}



MultiThread::get_roi_channel_pixel_percent_DN_raw8::get_roi_channel_pixel_percent_DN_raw8(){}

void MultiThread::get_roi_channel_pixel_percent_DN_raw8::setparam(uchar *data_, int w_, int h_, double x_,
                                                                  int roi_x_, int roi_y_, int roi_w_, int roi_h_)
{
    this->data = data_;
    this->w = w_;
    this->h = h_;
    this->x = x_;
    this->msg = "NONE";
    this->x_pos_dn=-999;
    this->roi_x = roi_x_;
    this->roi_y = roi_y_;
    this->roi_w = roi_w_;
    this->roi_h = roi_h_;
    for(int i=0; i<256; i++)
    {
        this->histogram[i]=0;
    }

}

void MultiThread::get_roi_channel_pixel_percent_DN_raw8::run()
{
    AlgorithmFuncs::get_x_percent_corresponding_DN_number_raw8(this->data, this->w, this->h, this->x, this->x_pos_dn,
                                                               this->roi_x, this->roi_y, this->roi_w, this->roi_h,
                                                               this->histogram,this->msg);
}





MultiThread::get_roi_channel_pixel_percent_DN_short::get_roi_channel_pixel_percent_DN_short(){}

void MultiThread::get_roi_channel_pixel_percent_DN_short::setparam(short *data_, int w_, int h_, double x_,
                                                                  int roi_x_, int roi_y_, int roi_w_, int roi_h_)
{
    this->data = data_;
    this->w = w_;
    this->h = h_;
    this->x = x_;
    this->msg = "NONE";
    this->x_pos_dn=-999;
    this->roi_x = roi_x_;
    this->roi_y = roi_y_;
    this->roi_w = roi_w_;
    this->roi_h = roi_h_;
    for(int i=0; i<2047; i++)
    {
        this->histogram[i]=0;
    }
}

void MultiThread::get_roi_channel_pixel_percent_DN_short::run()
{
    AlgorithmFuncs::get_x_percent_corresponding_DN_number_short(this->data, this->w, this->h, this->x, this->x_pos_dn,
                                                                this->roi_x, this->roi_y, this->roi_w, this->roi_h,
                                                                this->histogram, this->msg);
}





MultiThread::get_roi_channel_pixel_percent_DN_raw16::get_roi_channel_pixel_percent_DN_raw16(){}

void MultiThread::get_roi_channel_pixel_percent_DN_raw16::setparam(short *data_, int w_, int h_, double x_)
{
    this->data = data_;
    this->w = w_;
    this->h = h_;
    this->x = x_;
    this->msg = "NONE";
    this->x_pos_dn=-999;
}

void MultiThread::get_roi_channel_pixel_percent_DN_raw16::run()
{
    AlgorithmFuncs::get_x_percent_corresponding_DN_number_raw16(this->data, this->w, this->h, this->x, this->x_pos_dn, this->msg);
}



MultiThread::get_roi_mean_std_by_rect::get_roi_mean_std_by_rect(){}

void MultiThread::get_roi_mean_std_by_rect::setparam(uchar *in_data, cv::Rect &roi_rect, int width, int height)
{
    this->data = in_data;
    this->ROI_Rect = roi_rect;
    this->w = width;
    this->h = height;
}

void MultiThread::get_roi_mean_std_by_rect::run()
{
    AlgorithmFuncs::Get_ROI_MEAN_STD (this->data, this->ROI_Rect, this->w, this->h, this->mean, this->std);
}




MultiThread::get_double_roi_mean_std_by_rect::get_double_roi_mean_std_by_rect(){}

void MultiThread::get_double_roi_mean_std_by_rect::setparam(double *in_data, cv::Rect &roi_rect, int width, int height)
{
    this->data = in_data;
    this->ROI_Rect = roi_rect;
    this->w = width;
    this->h = height;
}

void MultiThread::get_double_roi_mean_std_by_rect::run()
{
    AlgorithmFuncs::Get_ROI_MEAN_Double (this->data, this->ROI_Rect, this->w, this->h, this->mean, this->std);
}


MultiThread::get_double_roi_mean_std_by_rect_chao::get_double_roi_mean_std_by_rect_chao(){}

void MultiThread::get_double_roi_mean_std_by_rect_chao::setparam(double *in_data, Point2i start_loca,
                                                                 cv::Rect &roi_rect, int width, int height)
{
    this->start_loca=start_loca;
    this->data = in_data;
    this->ROI_Rect = roi_rect;
    this->w = width;
    this->h = height;
}

void MultiThread::get_double_roi_mean_std_by_rect_chao::run()
{
    AlgorithmFuncs::Get_ROI_MEAN_Double_chao (this->data, this->start_loca,this->ROI_Rect, this->w, this->mean, this->std);
}






MultiThread::get_channel_mean_raw16::get_channel_mean_raw16(){}

void MultiThread::get_channel_mean_raw16::setparam(uchar *in_data, int width, int height)
{
    this->data = in_data;
    this->w = width;
    this->h = height;
}

void MultiThread::get_channel_mean_raw16::run()
{
    this->mean=AlgorithmFuncs::GetChannelMean_Doublebyte(this->data, this->w, this->h);
}



MultiThread::get_roi_channel_mean_raw16::get_roi_channel_mean_raw16(){}

void MultiThread::get_roi_channel_mean_raw16::setparam(unsigned short *in_data, int width, int height, int x0, int y0, int win_width, int win_height)
{
    this->data = in_data;
    this->w = width;
    this->h = height;
    this->win_x = x0;
    this->win_y = y0;
    this->win_w = win_width;
    this->win_h = win_height;
}

void MultiThread::get_roi_channel_mean_raw16::run()
{
    this->mean=AlgorithmFuncs::GetROIMean_Doublebyte(this->data, this->w, this->h, this->win_x, this->win_y, this->win_w, this->win_h);
}


MultiThread::get_grid_roi_mean_raw16::get_grid_roi_mean_raw16(){}

void MultiThread::get_grid_roi_mean_raw16::setparam(short *in_data, int width, int height, int cnt_col, int cnt_row)
{
    this->data = in_data;
    this->w = width;
    this->h = height;
    this->cnt_w = cnt_col;
    this->cnt_h = cnt_row;
    this->GridROI_Mean.clear();
}

void MultiThread::get_grid_roi_mean_raw16::run()
{
    int code=AlgorithmFuncs::Get_GridROI_Mean_Doublebyte (this->data, this->w, this->h, this->cnt_w, this->cnt_h, this->GridROI_Mean);
}




MultiThread::get_real_roi_prnu::get_real_roi_prnu(){}

void MultiThread::get_real_roi_prnu::setparam(float *delta_data, uchar *data, int width, int height, int x0, int y0, int win_width, int win_height)
{
    this->delta = delta_data;
    this->in_data = data;
    this->w = width;
    this->h = height;
    this->win_x = x0;
    this->win_y = y0;
    this->win_w = win_width;
    this->win_h = win_height;
}

void MultiThread::get_real_roi_prnu::run()
{
    this->prnu=AlgorithmFuncs::Get_Real_ROI_PRNU(this->delta, this->in_data, this->w, this->h, this->win_x, this->win_y, this->win_w, this->win_h);
}


MultiThread::get_channel_uniformity::get_channel_uniformity(){}

void MultiThread::get_channel_uniformity::setparam(uchar *in_data, int width, int height, int w_cell, int h_cell)
{
    this->data = in_data;
    this->w = width;
    this->h = height;
    this->w_cell_ = w_cell;
    this->h_cell_ = h_cell;
}

void MultiThread::get_channel_uniformity::run()
{
    this->prnu=AlgorithmFuncs::UniformityTest(this->data, this->w, this->h, this->w_cell_, this->h_cell_);
}



MultiThread::get_non_uniformity_bychannel::get_non_uniformity_bychannel(){}

void MultiThread::get_non_uniformity_bychannel::setparam(uchar *in_data, int width, int height, int w_cell, int h_cell)
{
    this->data = in_data;
    this->w = width;
    this->h = height;
    this->w_cell_ = w_cell;
    this->h_cell_ = h_cell;
    this->n_uniform_ = 0;
    this->n_uniform_eachcell_max_ = 0;
    this->max_cell_pos_ = 0;
    this->n_uniform_mirror_diff_eachcell_max_ =0;
    this->mirror_diff_max_cell_pos_ =0;
    this->n_uniform_updown_diff_eachcell_max_ =0;
    this->updown_diff_max_cell_pos_ =0;
    this->n_uniform_eachcell_.clear();
    this->n_uniform_mirror_diff_eachcell_.clear();
    this->n_uniform_updown_diff_eachcell_.clear();
}

void MultiThread::get_non_uniformity_bychannel::run()
{
    this->n_uniform_ = AlgorithmFuncs::Non_UniformityTest(this->data,this->w,this->h,
                                                          this->w_cell_,this->h_cell_,
                                                          this->n_uniform_eachcell_max_, this->max_cell_pos_,
                                                          this->n_uniform_mirror_diff_eachcell_max_, this->mirror_diff_max_cell_pos_,
                                                          this->n_uniform_updown_diff_eachcell_max_, this->updown_diff_max_cell_pos_,
                                                          this->n_uniform_eachcell_,
                                                          this->n_uniform_mirror_diff_eachcell_,
                                                          this->n_uniform_updown_diff_eachcell_);
}


MultiThread::get_channel_uniformity_max::get_channel_uniformity_max(){}

void MultiThread::get_channel_uniformity_max::setparam(uchar *in_data, int width, int height, int w_cell, int h_cell)
{
    this->data = in_data;
    this->w = width;
    this->h = height;
    this->w_cell_ = w_cell;
    this->h_cell_ = h_cell;
    this->prnu_eachcell_max = 0;
    this->max_cell_pos = 0;
    this->prnu_eachcell.clear();
}

void MultiThread::get_channel_uniformity_max::run()
{
    this->prnu=AlgorithmFuncs::UniformityTest_Max(this->data, this->w, this->h, this->w_cell_, this->h_cell_,
                                                  this->prnu_eachcell_max, this->max_cell_pos, this->prnu_eachcell);
}


MultiThread::get_image_colorratio::get_image_colorratio(){}

void MultiThread::get_image_colorratio::setparam(uchar *pR, uchar *pGr, uchar *pGb,uchar *pB,
                                                 int x0, int y0, int width, int height, int w_rect, int h_rect)
{
    this->p_buf_r= pR;
    this->p_buf_gr= pGr;
    this->p_buf_gb= pGb;
    this->p_buf_b= pB;
    this->pos_x=x0;
    this->pos_y=y0;
    this->w=width;
    this->h=height;
    this->w_rect_=w_rect;
    this->h_rect_=h_rect;
}

void MultiThread::get_image_colorratio::run()
{
    AlgorithmFuncs::getColorRatioByRect(this->p_buf_r, this->p_buf_gr, this->p_buf_gb, this->p_buf_b,
                                        this->w, this->h,
                                        this->pos_x, this->pos_y, this->w_rect_, this->h_rect_,
                                        this->g_g_ratio, this->r_g_ratio, this->b_g_ratio, this->gr_gb_diff);
}




MultiThread::get_roi_colorratio::get_roi_colorratio(){}

void MultiThread::get_roi_colorratio::setparam(uchar *pR, uchar *pGr, uchar *pGb, uchar *pB,
                                               int width, int height, int roi_x, int roi_y, int roi_w, int roi_h)
{
    this->p_buf_r= pR;
    this->p_buf_gr= pGr;
    this->p_buf_gb= pGb;
    this->p_buf_b= pB;
    this->w=width;
    this->h=height;
    this->roi_x_ =roi_x;
    this->roi_y_ =roi_y;
    this->roi_w_ =roi_w;
    this->roi_h_ =roi_h;
}

void MultiThread::get_roi_colorratio::run()
{
    AlgorithmFuncs::get_roi_colorratio(this->p_buf_r, this->p_buf_gr, this->p_buf_gb, this->p_buf_b,
                                        this->w, this->h, this->roi_x_, this->roi_y_, this->roi_w_, this->roi_h_,
                                        this->g_g_ratio, this->r_g_ratio, this->b_g_ratio, this->gr_gb_diff);
}




MultiThread::get_row_defect_bychannel::get_row_defect_bychannel(){}

void MultiThread::get_row_defect_bychannel::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                                     double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                                     int max_pos, double row_max_min_diff, int cnt_deadrow,
                                                     double dol_std_max, int dol_std_max_pos,
                                                     double breakline_row_spec, int breakline_row_cnt,
                                                     double breakline_row_max_value,int breakline_row_max_pos,
                                                     Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->row_max_min_diff_ = row_max_min_diff;
    this->cnt_deadrow_ = cnt_deadrow,
    this->dol_std_max_ = dol_std_max;
    this->dol_std_max_pos_ = dol_std_max_pos;
    this->breakline_row_spec_ = breakline_row_spec;
    this->breakline_row_cnt_ = breakline_row_cnt;
    this->breakline_row_max_value_ = breakline_row_max_value;
    this->breakline_row_max_pos_ = breakline_row_max_pos;
    this->mode_ = mode;
    this->msg_ = msg;
    this->mean_each_.clear();
    this->delta_each_.clear();
    this->diff_each_.clear();
}

void MultiThread::get_row_defect_bychannel::run()
{
    this->cnt_deadrow_ = AlgorithmFuncs::find_row_defect_bychannel_new(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                   this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                   this->max_pos_,this->row_max_min_diff_,
                                                                   this->dol_std_max_,this->dol_std_max_pos_,
                                                                   this->breakline_row_spec_,this->breakline_row_cnt_,
                                                                   this->breakline_row_max_value_,this->breakline_row_max_pos_,
                                                                   this->msg_,this->mean_each_,this->delta_each_,this->diff_each_);
}





MultiThread::get_col_defect_bychannel::get_col_defect_bychannel(){}

void MultiThread::get_col_defect_bychannel::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                                     double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                                     int max_pos, double col_max_min_diff, int cnt_deadcol,
                                                     double dol_std_max, int dol_std_max_pos,
                                                     double breakline_col_spec, int breakline_col_cnt,
                                                     double breakline_col_max_value,int breakline_col_max_pos,
                                                     Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->col_max_min_diff_ = col_max_min_diff;
    this->cnt_deadcol_ = cnt_deadcol,
    this->dol_std_max_ = dol_std_max;
    this->dol_std_max_pos_ = dol_std_max_pos;
    this->breakline_col_spec_ = breakline_col_spec;
    this->breakline_col_cnt_ = breakline_col_cnt;
    this->breakline_col_max_value_ = breakline_col_max_value;
    this->breakline_col_max_pos_ = breakline_col_max_pos;
    this->mode_ = mode;
    this->msg_ = msg;
    this->mean_each_.clear();
    this->delta_each_.clear();
    this->diff_each_.clear();
}

void MultiThread::get_col_defect_bychannel::run()
{
    this->cnt_deadcol_ = AlgorithmFuncs::find_col_defect_bychannel_new(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                   this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                   this->max_pos_,this->col_max_min_diff_,
                                                                   this->dol_std_max_,this->dol_std_max_pos_,
                                                                   this->breakline_col_spec_,this->breakline_col_cnt_,
                                                                   this->breakline_col_max_value_,this->breakline_col_max_pos_,
                                                                   this->msg_,this->mean_each_,this->delta_each_,this->diff_each_);
}


MultiThread::get_col_defect_bychannel_bright_dark::get_col_defect_bychannel_bright_dark(){}

void MultiThread::get_col_defect_bychannel_bright_dark::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                                                 double spec_line, double spec_dol, double spec_bright_line , double spec_dark_line,int cnt_dol, double max_diff,
                                                                 int max_pos, double col_max_min_diff, int cnt_deadcol,
                                                                 double dol_std_max, int dol_std_max_pos,
                                                                 double breakline_col_spec, int breakline_col_cnt,
                                                                 double breakline_col_max_value,int breakline_col_max_pos,
                                                                 Spec_Mode mode, std::string msg,int cnt_bright_col,
                                                                 double max_bright_col_diff,int max_bright_col_pos,double max_dark_col_diff,int max_dark_col_pos)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->spec_bright_line_ = spec_bright_line;
    this->spec_dark_line_ = spec_dark_line;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->col_max_min_diff_ = col_max_min_diff;
    this->cnt_deadcol_ = cnt_deadcol,
    this->dol_std_max_ = dol_std_max;
    this->dol_std_max_pos_ = dol_std_max_pos;
    this->breakline_col_spec_ = breakline_col_spec;
    this->breakline_col_cnt_ = breakline_col_cnt;
    this->breakline_col_max_value_ = breakline_col_max_value;
    this->breakline_col_max_pos_ = breakline_col_max_pos;
    this->mode_ = mode;
    this->msg_ = msg;
    this->mean_each_.clear();
    this->delta_each_.clear();
    this->diff_each_.clear();
    this->cnt_bright_col_ = cnt_bright_col;
    this->max_bright_col_diff_ = max_bright_col_diff;
    this->max_bright_col_pos_ = max_bright_col_pos;
    this->max_dark_col_diff_ = max_dark_col_diff;
    this->max_dark_col_pos_ = max_dark_col_pos;
}

void MultiThread::get_col_defect_bychannel_bright_dark::run()
{
    this->cnt_deadcol_ = AlgorithmFuncs::find_txoff_col_defect_bychannel_new(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                             this->spec_line_,this->spec_dol_,this->spec_bright_line_,this->spec_dark_line_,this->cnt_dol_,this->max_diff_,
                                                                             this->max_pos_,this->col_max_min_diff_,
                                                                             this->dol_std_max_,this->dol_std_max_pos_,
                                                                             this->breakline_col_spec_,this->breakline_col_cnt_,
                                                                             this->breakline_col_max_value_,this->breakline_col_max_pos_,
                                                                             this->msg_,this->mean_each_,this->delta_each_,this->diff_each_,this->cnt_bright_col_,
                                                                             this->max_bright_col_diff_,this->max_bright_col_pos_,this->max_dark_col_diff_,this->max_dark_col_pos_);
}



MultiThread::get_edge_row_defect_bychannel::get_edge_row_defect_bychannel(){}

void MultiThread::get_edge_row_defect_bychannel::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                                          double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                                          int max_pos, int cnt_deadrow,double dol_std_max, int dol_std_max_pos,
                                                          Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;

    this->cnt_deadrow_ = cnt_deadrow;
    this->dol_std_max_ = dol_std_max;
    this->dol_std_max_pos_ = dol_std_max_pos;
    this->mode_ = mode;
    this->msg_ = msg;
}

void MultiThread::get_edge_row_defect_bychannel::run()
{
    this->cnt_deadrow_ = AlgorithmFuncs::find_edge_row_defect_bychannel_new(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                        this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                        this->max_pos_,this->dol_std_max_,this->dol_std_max_pos_,this->msg_);
}






MultiThread::get_edge_col_defect_bychannel::get_edge_col_defect_bychannel(){}

void MultiThread::get_edge_col_defect_bychannel::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                                          double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                                          int max_pos, int cnt_deadcol,double dol_std_max, int dol_std_max_pos,
                                                          Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;

    this->cnt_deadcol_ = cnt_deadcol;
    this->dol_std_max_ = dol_std_max;
    this->dol_std_max_pos_ = dol_std_max_pos;
    this->mode_ = mode;
    this->msg_ = msg;
}

void MultiThread::get_edge_col_defect_bychannel::run()
{
    this->cnt_deadcol_ = AlgorithmFuncs::find_edge_col_defect_bychannel_new(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                        this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                        this->max_pos_,this->dol_std_max_,this->dol_std_max_pos_,this->msg_);
}



MultiThread::get_channel_deadrow::get_channel_deadrow(){}

void MultiThread::get_channel_deadrow::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                           double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                           int max_pos, double row_max_min_diff, int cnt_deadrow, Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->row_max_min_diff_ = row_max_min_diff;
    this->cnt_deadrow_ = cnt_deadrow,
    this->mode_ = mode;
    this->msg_ = msg;
}

void MultiThread::get_channel_deadrow::run()
{
    this->cnt_deadrow_ = AlgorithmFuncs::FindDeadRow_by_channel(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                this->max_pos_,this->row_max_min_diff_,this->msg_);
}



MultiThread::get_channel_deadcol::get_channel_deadcol(){}

void MultiThread::get_channel_deadcol::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                           double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                           int max_pos, double col_max_min_diff, int cnt_deadcol, Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->col_max_min_diff_ = col_max_min_diff;
    this->cnt_deadcol_ = cnt_deadcol,
    this->mode_ = mode;
    this->msg_ = msg;
}

void MultiThread::get_channel_deadcol::run()
{
    this->cnt_deadcol_ = AlgorithmFuncs::FindDeadCol_by_channel(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                this->max_pos_,this->col_max_min_diff_,this->msg_);
}





MultiThread::get_channel_deadrow_add_breakline::get_channel_deadrow_add_breakline(){}

void MultiThread::get_channel_deadrow_add_breakline::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                                              double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                                              int max_pos, double row_max_min_diff, int cnt_deadrow,
                                                              double breakline_row_spec, int breakline_row_cnt, double breakline_row_max_value,
                                                              int breakline_row_max_pos, Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->row_max_min_diff_ = row_max_min_diff;
    this->cnt_deadrow_ = cnt_deadrow,
    this->mode_ = mode;
    this->breakline_row_spec_ = breakline_row_spec;
    this->breakline_row_cnt_ = breakline_row_cnt;
    this->breakline_row_max_value_ = breakline_row_max_value;
    this->breakline_row_max_pos_ = breakline_row_max_pos;
    this->msg_ = msg;
    this->delta_each_.clear();
}

void MultiThread::get_channel_deadrow_add_breakline::run()
{
    this->cnt_deadrow_ = AlgorithmFuncs::FindDeadRow_by_channel_add_breakline(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                              this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                              this->max_pos_,this->row_max_min_diff_,this->breakline_row_spec_,
                                                                              this->breakline_row_cnt_,this->breakline_row_max_value_,
                                                                              this->breakline_row_max_pos_,this->msg_,this->delta_each_);
}




MultiThread::get_channel_deadcol_add_breakline::get_channel_deadcol_add_breakline(){}

void MultiThread::get_channel_deadcol_add_breakline::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                                              double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                                              int max_pos, double col_max_min_diff, int cnt_deadcol,
                                                              double breakline_col_spec, int breakline_col_cnt, double breakline_col_max_value,
                                                              int breakline_col_max_pos, Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->col_max_min_diff_ = col_max_min_diff;
    this->cnt_deadcol_ = cnt_deadcol,
    this->mode_ = mode;
    this->breakline_col_spec_ = breakline_col_spec;
    this->breakline_col_cnt_ = breakline_col_cnt;
    this->breakline_col_max_value_ = breakline_col_max_value;
    this->breakline_col_max_pos_ = breakline_col_max_pos;
    this->msg_ = msg;
    this->delta_each_.clear();
}

void MultiThread::get_channel_deadcol_add_breakline::run()
{
    this->cnt_deadcol_ = AlgorithmFuncs::FindDeadCol_by_channel_add_breakline(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                              this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                              this->max_pos_,this->col_max_min_diff_,this->breakline_col_spec_,
                                                                              this->breakline_col_cnt_,this->breakline_col_max_value_,
                                                                              this->breakline_col_max_pos_,this->msg_,this->delta_each_);
}





MultiThread::get_edge_channel_deadrow::get_edge_channel_deadrow(){}

void MultiThread::get_edge_channel_deadrow::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                           double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                           int max_pos, double row_max_min_diff, int cnt_deadrow, Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->row_max_min_diff_ = row_max_min_diff;
    this->cnt_deadrow_ = cnt_deadrow,
    this->mode_ = mode;
    this->msg_ = msg;
}

void MultiThread::get_edge_channel_deadrow::run()
{
    this->cnt_deadrow_ = AlgorithmFuncs::Find_Edge_DeadRow_by_channel(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                      this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                      this->max_pos_,this->row_max_min_diff_,this->msg_);
}






MultiThread::get_edge_channel_deadcol::get_edge_channel_deadcol(){}

void MultiThread::get_edge_channel_deadcol::setparam(uchar *in_data, int width, int height, int radius, int skip_cnt,
                                           double spec_line, double spec_dol, int cnt_dol, double max_diff,
                                           int max_pos, double col_max_min_diff, int cnt_deadcol, Spec_Mode mode, std::string msg)
{
    this->p_data = in_data;
    this->w = width;
    this->h = height;
    this->r = radius;
    this->skip = skip_cnt;
    this->spec_line_ =spec_line;
    this->spec_dol_ = spec_dol;
    this->cnt_dol_ = cnt_dol;
    this->max_diff_ = max_diff;
    this->max_pos_ = max_pos;
    this->col_max_min_diff_ = col_max_min_diff;
    this->cnt_deadcol_ = cnt_deadcol,
    this->mode_ = mode;
    this->msg_ = msg;
}

void MultiThread::get_edge_channel_deadcol::run()
{
    this->cnt_deadcol_ = AlgorithmFuncs::Find_Edge_DeadCol_by_channel(this->mode_,this->p_data,this->w,this->h,this->r,this->skip,
                                                                      this->spec_line_,this->spec_dol_,this->cnt_dol_,this->max_diff_,
                                                                      this->max_pos_,this->col_max_min_diff_,this->msg_);
}



MultiThread::bychannel_binarization::bychannel_binarization(){}

void MultiThread::bychannel_binarization::setparam(uchar *in_data, uchar *out_data, int width, int height, int skip_cnt,
                                                   int radius, double binary_spec, Spec_Mode mode)
{
    this->p_data = in_data;
    this->binary_data = out_data;
    this->w = width;
    this->h = height;
    this->skip = skip_cnt;
    this->r = radius;
    this->spec = binary_spec;
    this->mode_ = mode;
}

void MultiThread::bychannel_binarization::run()
{
   AlgorithmFuncs::bychannel_MapBinarization (this->p_data, this->binary_data, this->w, this->h, this->skip, this->r, this->spec, this->mode_);
}



MultiThread::bychannel_binarization_add_blemish::bychannel_binarization_add_blemish(){}

void MultiThread::bychannel_binarization_add_blemish::setparam(uchar *in_data, uchar *out_data, uchar *out_blemish, int width, int height, int skip_cnt,
                                                               int radius, double binary_spec, double blemish_Spec, Spec_Mode mode)
{
    this->p_data = in_data;
    this->binary_data = out_data;
    this->blemish_binary = out_blemish;
    this->w = width;
    this->h = height;
    this->skip = skip_cnt;
    this->r = radius;
    this->spec = binary_spec;
    this->spec_b = blemish_Spec;
    this->mode_ = mode;
}

void MultiThread::bychannel_binarization_add_blemish::run()
{
    AlgorithmFuncs::bychannel_MapBinarization_add_blemish (this->p_data, this->binary_data,this->blemish_binary,
                                                           this->w, this->h, this->skip, this->r, this->spec, this->spec_b, this->mode_);
}



MultiThread::bychannel_binarization_delta::bychannel_binarization_delta(){}

void MultiThread::bychannel_binarization_delta::setparam(uchar *in_data, uchar *out_data, int width, int height, int skip_cnt,
                                                   int radius, double binary_spec, Spec_Mode mode, double *delta_matrix)
{
    this->p_data = in_data;
    this->binary_data = out_data;
    this->w = width;
    this->h = height;
    this->skip = skip_cnt;
    this->r = radius;
    this->spec = binary_spec;
    this->mode_ = mode;
    this->delta_map = delta_matrix;
}

void MultiThread::bychannel_binarization_delta::run()
{
   AlgorithmFuncs::bychannel_MapBinarization_delta (this->p_data, this->binary_data, this->w, this->h, this->skip, this->r, this->spec, this->mode_, this->delta_map);
}




MultiThread::bychannel_pixel_defect_binarization::bychannel_pixel_defect_binarization(){}

void MultiThread::bychannel_pixel_defect_binarization::setparam(uchar *in_data, float *out_delta, uchar *out_data, uchar *out_blemish, int width, int height, int skip_cnt,
                                                                int radius, double binary_spec, double blemish_Spec, Spec_Mode mode)
{
    this->p_data = in_data;
    this->delta_data = out_delta;
    this->binary_data = out_data;
    this->blemish_binary = out_blemish;
    this->w = width;
    this->h = height;
    this->skip = skip_cnt;
    this->r = radius;
    this->spec = binary_spec;
    this->spec_b = blemish_Spec;
    this->mode_ = mode;
}

void MultiThread::bychannel_pixel_defect_binarization::run()
{
    AlgorithmFuncs::bychannel_pixel_MapBinarization (this->p_data, this->delta_data, this->binary_data,this->blemish_binary,
                                                     this->w, this->h, this->skip, this->r, this->spec, this->spec_b, this->mode_);
}




MultiThread::bychannel_flicker_binarization_raw16::bychannel_flicker_binarization_raw16(){}

void MultiThread::bychannel_flicker_binarization_raw16::setparam(uchar *in_data, uchar *out_data, int width, int height, int frame_cnt, double binary_spec)
{
    this->p_data = in_data;
    this->binary_data = out_data;
    this->w = width;
    this->h = height;
    this->framecnt = frame_cnt;
    this->spec = binary_spec;
}

void MultiThread::bychannel_flicker_binarization_raw16::run()
{
    AlgorithmFuncs:: Flicker_Pix_Binarization_Doublebyte (this->p_data, this->binary_data, this->framecnt, this->w, this->h, this->spec);
}


MultiThread::bychannel_flicker_binarization_raw16_chao::bychannel_flicker_binarization_raw16_chao(){}

void MultiThread::bychannel_flicker_binarization_raw16_chao::setparam(uchar *in_data, uchar *out_data, int width, int height,  int y_start, int frame_cnt, double binary_spec)
{
    this->p_data = in_data;
    this->binary_data = out_data;
    this->w = width;
    this->h = height;
    this->y_start_ = y_start;
    this->framecnt = frame_cnt;
    this->spec = binary_spec;
}

void MultiThread::bychannel_flicker_binarization_raw16_chao::run()
{
    AlgorithmFuncs:: Flicker_Pix_Binarization_Doublebyte_chao (this->p_data, this->binary_data, this->framecnt, this->w,
                                                               this->h,this->y_start_, this->spec);
}





MultiThread::get_bychannel_std_map_raw16::get_bychannel_std_map_raw16(){}

void MultiThread::get_bychannel_std_map_raw16::setparam(uchar *in_data, double *out_data, int width, int height, int frame_cnt)
{
    this->p_data = in_data;
    this->std_map = out_data;
    this->w = width;
    this->h = height;
    this->framecnt = frame_cnt;
}

void MultiThread::get_bychannel_std_map_raw16::run()
{
    AlgorithmFuncs::Get_STD_MAP_Doublebyte (this->p_data, this->std_map, this->framecnt, this->w, this->h);
}



MultiThread::get_bychannel_std_map_raw16_chao::get_bychannel_std_map_raw16_chao(){}

void MultiThread::get_bychannel_std_map_raw16_chao::setparam(uchar *in_data, double *out_data, int width, int height,  int y_start,  int frame_cnt)
{
    this->p_data = in_data;
    this->std_map = out_data;
    this->w = width;
    this->h = height;
    this->y_start_ = y_start;
    this->framecnt = frame_cnt;
}

void MultiThread::get_bychannel_std_map_raw16_chao::run()
{
    AlgorithmFuncs::Get_STD_MAP_Doublebyte_chao (this->p_data, this->std_map, this->framecnt, this->w, this->h,this->y_start_);
}




MultiThread::raw16_to_raw8::raw16_to_raw8()
    : raw_bits_(10)
{}

void MultiThread::raw16_to_raw8::setparam(uchar *in_raw16, uchar *out_raw8, int width, int height, int raw_bits)
{
     this->w=width;
     this->h=height;
     this->in_data=in_raw16;
     this->out_data=out_raw8;
     this->raw_bits_=raw_bits;
}

void MultiThread::raw16_to_raw8::run()
{
   AlgorithmFuncs::from_doublebyte_to_raw8 (this->in_data, this->out_data,this->w, this->h, this->raw_bits_);
}

MultiThread::get_darkshading::get_darkshading(){}

void MultiThread::get_darkshading::setparam(uchar *in_data, int width, int height, int x_cnt, int y_cnt, std::string msg)
{
    this->data=in_data;
    this->w=width;
    this->h=height;
    this->x_cnt_ =x_cnt;
    this->y_cnt_ =y_cnt;
    this->mean_ob_eachcell_max=0;
    this->mean_ob_eachcell_min=0;
    this->max_ob_pos=0;
    this->min_ob_pos=0;
    this->mean_ob_eachcell.clear();
    this->msg_=msg;
}

void MultiThread::get_darkshading ::run()
{
    AlgorithmFuncs::DarkShadingTest(this->data,this->w,this->h,this->x_cnt_,this->y_cnt_,
                                    this->mean_ob_eachcell_max,this->mean_ob_eachcell_min,
                                    this->max_ob_pos,this->min_ob_pos,this->mean_ob_eachcell,this->msg_);
}



MultiThread::bychannel_pixel_defect_chao::bychannel_pixel_defect_chao(){}

void MultiThread::bychannel_pixel_defect_chao::setparam(uchar *in_data, float *out_delta, uchar *out_data, uchar *out_blemish, int width, int height, int skip_cnt,
                                                                int radius, double binary_spec, double blemish_Spec, Spec_Mode mode)
{
    this->p_data = in_data;
    this->delta_data = out_delta;
    this->binary_data = out_data;
    this->blemish_binary = out_blemish;
    this->w = width;
    this->h = height;
    this->skip = skip_cnt;
    this->r = radius;
    this->spec = binary_spec;
    this->spec_b = blemish_Spec;
    this->mode_ = mode;
}

void MultiThread::bychannel_pixel_defect_chao::run()
{
    AlgorithmFuncs::bychannel_pixel_chao (this->p_data, this->delta_data, this->binary_data,
                                          this->blemish_binary,this->w, this->h, this->skip, this->r,
                                          this->spec, this->spec_b, this->mode_);
}
