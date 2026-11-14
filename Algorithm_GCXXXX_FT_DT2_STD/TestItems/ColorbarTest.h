#ifndef COLORBARTEST_H
#define COLORBARTEST_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];

enum Colorbar_type {Colorbar_normal,Colorbar_special};

class ColorbarTest : public CPBaseTestItem
{
public:

    ColorbarTest(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "ColorbarTest";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);
    }

    ~ColorbarTest(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;

public:

    int Testfunc_Colorbar(ParamManage& test_manage,uchar *data_raw,
                          const char *sample_path,uchar *pImage_save);

    int Testfunc_Colorbar_RAW16(ParamManage& test_manage,short *data_raw/*uchar *data_raw*/,
                                const char *sample_path,uchar *pImage_save);

    int w, h;


private:

    uchar* g_buf_raw_;

    short* g_buf_raw_16_;

    int Colorbar_Test(ParamManage& manage,const char *sample_path,
                      Colorbar_type type);

    int Colorbar_Test_RAW16(ParamManage& manage,const char *sample_path,
                            Colorbar_type type);
};

#endif // COLORBARTEST_H
