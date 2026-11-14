#ifndef LARGEGAINTEST_H
#define LARGEGAINTEST_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];

class LargeGainTest : public CPBaseTestItem
{
public:
    LargeGainTest(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "LargeGainTest";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);

    }

    ~LargeGainTest(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;

public:

    int  TestFunc_Large_Gain_Bright_Bayer(ParamManage& manage,uchar *pData,uchar *pImage_save);

    int  TestFunc_Large_Gain_IIC_Bayer(ParamManage& manage,uchar *pData,uchar *pImage_save);

private:

    Subitem_Result PD_Test( const ParamManage& manage, Pixel_Defect_type type);

    Subitem_Result PD_Test_V1_0( const ParamManage& manage, Pixel_Defect_type type);

    Subitem_Result PD_Test_Channel( const ParamManage& manage);

    Subitem_Result PD_Test_Channel_V1_0( const ParamManage& manage);

};

#endif // LARGEGAINTEST_H
