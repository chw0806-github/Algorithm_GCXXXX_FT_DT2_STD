#ifndef TESTFUNCWHITECP2_H
#define TESTFUNCWHITECP2_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"

extern AlgorithmItems Algorithm_all[Parallel_num];


class BrightTest : public CPBaseTestItem
{
public:

    BrightTest(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "BrightTest";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);
    }

    ~BrightTest(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;

public:
    int TestFunc_Normal_Bright_POG(ParamManage& test_manage,uchar * pData,uchar *pImage_save);
    int TestFunc_IIC_Bright(ParamManage& test_manage,uchar * pData,uchar *pImage_save);
    int TestFunc_Normal_Bright_AllExceptPog(ParamManage &test_manage, uchar *pData, uchar *pImage_save);   //除了pog其他都有
    int TestFunc_Normal_Bright_ALL(ParamManage &test_manage, uchar *pData, uchar *pImage_save);            //最全

private:



    vector<Subitem_Result> Color_GI( ParamManage& manage);

    vector<Subitem_Result> Color_GI_V1_0( ParamManage& manage);

    vector<Subitem_Result> Color_GI_V1_1( ParamManage& manage);

    vector<Subitem_Result> Color_GI_5_5( ParamManage& manage);

    vector<Subitem_Result> Color_GI_5_5_V1_0( ParamManage& manage);

    vector<Subitem_Result> Color_Shading(ParamManage &manage);

    vector<Subitem_Result> Color_Shading_V1_0( ParamManage& manage);

    vector<Subitem_Result> Color_Shading_V1_1( ParamManage& manage);

    vector<Subitem_Result> Color_Shading_5_5( ParamManage& manage);

    vector<Subitem_Result> Color_Shading_5_5_V1_0( ParamManage& manage);

    vector<Subitem_Result> Color_Shading_15_15( ParamManage& manage);

    vector<Subitem_Result> Color_Shading_15_15_V1_0( ParamManage& manage);

    Subitem_Result Optical_Center( ParamManage& manage);

    Subitem_Result Optical_Center_V1_0( ParamManage& manage);





};

#endif // TESTFUNCWHITECP2_H
