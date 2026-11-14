#ifndef DARKTEST_H
#define DARKTEST_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];

class DarkTest: public CPBaseTestItem
{
public:

    DarkTest(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "DarkTest";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);

    }

    ~DarkTest(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;

public:
    int TestFunc_IIC_LIGHT_Dark_(ParamManage& manage,uchar *pData,uchar *pImage_save);

    int TestFunc_IIC_Dark_(ParamManage& manage,uchar *pData,uchar *pImage_save);

    int TestFunc_DAG_(ParamManage& manage,uchar *pData,uchar *pImage_save);

    //标准
    int TestFunc_Normal_Dark_ALL(ParamManage &manage, uchar *pData, uchar *pImage_save);

private:

    vector<Subitem_Result> OB_and_DarkShading(const ParamManage& manage);

    vector<Subitem_Result> OB_and_DarkShading_V1_0(const ParamManage& manage);

    vector<Subitem_Result> OB_and_DarkShading_V1_1(const ParamManage& manage);

    vector<Subitem_Result> OB_and_DarkShading_V1_2(const ParamManage& manage);

    vector<Subitem_Result> OB_and_DarkShading_V1_3(const ParamManage& manage);

    Subitem_Result Deadrow_Deadcol(const ParamManage& manage,bool simple_test_COM);

    Subitem_Result Deadrow_Deadcol_V1_0(const ParamManage& manage,bool simple_test_COM);

    Subitem_Result Deadrow_Deadcol_V1_1(const ParamManage& manage,bool simple_test_COM);

    Subitem_Result Deadrow_Deadcol_IIC(const ParamManage& manage,bool simple_test_COM);

    Subitem_Result Deadrow_Deadcol_IIC_V1_0(const ParamManage& manage,bool simple_test_COM);

    Subitem_Result FPN_DL_Bright_Dark(const ParamManage &manage, bool simple_test_COM);

    Subitem_Result FPN_DL_Bright_Dark_V1_0(const ParamManage &manage, bool simple_test_COM);

    Subitem_Result FPN_DL_Bright_Dark_V1_1(const ParamManage &manage, bool simple_test_COM);

    Subitem_Result FPN_DL_Bright_Dark_V1_2(const ParamManage &manage, bool simple_test_COM);

    Subitem_Result Histogram_Speical_DN_Count(const ParamManage& manage);

    Subitem_Result Histogram_Speical_DN_Count_V1_0(const ParamManage& manage);

    Subitem_Result Global_LUM_DL_test_CP( const ParamManage& manage);

    Subitem_Result Global_LUM_DL_test_CP_V1_0( const ParamManage& manage);

    Subitem_Result PD_DataCollect( const ParamManage& manage);

    Subitem_Result OB_and_GRGB( const ParamManage& manage);

    Subitem_Result OB_and_GRGB_V1_0( const ParamManage& manage);

};

#endif // DARKTEST_H
