#ifndef LAGTEST_H
#define LAGTEST_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];


class LagTest: public CPBaseTestItem
{
public:
    LagTest(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "LagTest";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);
    }

    ~LagTest(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;
public:


    int TestFunc_Lag_bayer(ParamManage& manage,uchar *pData_Lag_HL_first_doublebyte,
                                    uchar *pData_Lag_HL_end_doublebyte,int datatype);

private:

    short *p_diff_ ;
    short *p_diff_r ;
    short *p_diff_gr ;
    short *p_diff_gb ;
    short *p_diff_b ;

    Subitem_Result Lag_Mean_Test(const ParamManage& manage);

    Subitem_Result Lag_Mean_Test_V1_0(const ParamManage& manage);

    Subitem_Result ROI_Mean_Test(const ParamManage& manage);

    Subitem_Result ROI_Mean_Test_V1_0(const ParamManage& manage);

};

#endif // LAGTEST_H
