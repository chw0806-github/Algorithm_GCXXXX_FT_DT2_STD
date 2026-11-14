#ifndef FINDCLUSTERTYPE_H
#define FINDCLUSTERTYPE_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];

class FindClusterType: public CPBaseTestItem
{
public:
    FindClusterType(int site_num,int crop_x_,int crop_y_,uchar *map_img_,
    uchar *pix_type_, QMap<QPair<int,int>, int> &dd_info_xy_,int *otp_dd_type_,int dd_totla_limit_):
    crop_x(crop_x_),crop_y(crop_y_),map_img(map_img_),
      pix_type(pix_type_),dd_info_xy(dd_info_xy_),otp_dd_type(otp_dd_type_),
      dd_totla_limit(dd_totla_limit_)
    {
        name = "FindClusterType";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);
    }

    ~FindClusterType()
    {
        Algorithm_all[site_].Free_All_Buf();
    }
public:

    int crop_x;
    int crop_y;

    int dd_totla_limit;

    uchar *map_img;
    uchar *pix_type;
//    uchar *dd_info;
    QMap<QPair<int,int>, int> &dd_info_xy;
    int *otp_dd_type;


    int Find_ClusterType_in_binarymap_Type0(ParamManage& test_manage,int &dd_total_cnt, int &big_dd_cnt,int limt);
    int Find_ClusterType_in_binarymap_Type11(ParamManage& test_manage,int &dd_total_cnt, int &big_dd_cnt,int limt);
    int Find_ClusterType_in_binarymap_Type15(ParamManage& test_manage,int &dd_total_cnt, int &big_dd_cnt,int limt);
    int Find_ClusterType_in_binarymap_Type15_Ex(ParamManage &test_manage, int &dd_total_cnt, int &big_dd_cnt, int limit, int &dd_special_cnt);   //GC4602
};

#endif // FINDCLUSTERTYPE_H
