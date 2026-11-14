#ifndef RANGETEST_H
#define RANGETEST_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];

enum Range_Respose_type{Range_Respose_4channel,Range_Respose_16channel};

class RangeTest : public CPBaseTestItem
{
public:

    RangeTest(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "RangeTest";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);
    }

    ~RangeTest(){
        Algorithm_all[site_].Free_All_Buf();
    }
private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;

    int image_height;
    int image_width;
    int *OTP_DD;
    int total_number;
    int crop_x;
    int crop_y;

public:

    int  TestFunc_Range_Bayer(ParamManage& manage,uchar *pData,uchar *pImage_save);

private:

    Subitem_Result ROI_MEAN_and_STD(const ParamManage& manage);

    Subitem_Result ROI_MEAN_and_STD_V1_0(const ParamManage& manage);
};

#endif // RANGETEST_H
