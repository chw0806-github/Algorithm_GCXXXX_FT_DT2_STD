#ifndef DARKCURRENT_H
#define DARKCURRENT_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];

class DarkCurrent: public CPBaseTestItem
{
public:

    DarkCurrent(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "DarkCurrent";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);
    }

    ~DarkCurrent(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;

public:

    int TestFunc_Darkcurrent(ParamManage& manage,uchar *pData_long_exp,
                                      uchar *pData_short_exp);

    int w, h;

private:

    Subitem_Result  TestFunc_Darkcurrent_test(const ParamManage& manage,Darkcurrent_type type,
                              short * p_long_exp,short * p_short_exp);

    Subitem_Result Global_Darkcurrent_test(const ParamManage& manage,  short * p_long_exp,short * p_short_exp);

};

#endif // DARKCURRENT_H
