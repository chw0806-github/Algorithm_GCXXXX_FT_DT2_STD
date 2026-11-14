#ifndef NOISETEST_H
#define NOISETEST_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];


enum Noise_Dark_type {Noise_Dark_row,Noise_Dark_col};
enum Noise_Dark_Pixel_type {Noise_Dark_Pixel_read, Noise_Dark_RTS, Noise_Dark_Pixel_row,
                            Noise_Dark_Pixel_ColRTS,Noise_Dark_Pixel_Col_chanel};

class NoiseTest: public CPBaseTestItem
{
public:

    NoiseTest(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "NoiseTest";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);
    }

    ~NoiseTest(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;

public:

    int TestFunc_ALL_Noise_Dark(ParamManage& manage,uchar *pData,uchar *pImage_save);

private:

    double *g_buf_delta_ ;
    double *g_buf_delta_raw_ ;
    double *g_buf_delta_r_ ;
    double *g_buf_delta_gr_ ;
    double *g_buf_delta_gb_ ;
    double *g_buf_delta_b_ ;

    bool Get_STD_Map_by_4channel(const ParamManage& manage,uchar *pData,int frame_cnt);

    Subitem_Result Noise_Dark_by_pixel_chao(const ParamManage& manage,Noise_Dark_Pixel_type type);

    Subitem_Result Noise_Dark_Row(const ParamManage& manage);
    Subitem_Result Noise_Dark_ColRTS(const ParamManage& manage);
    Subitem_Result Noise_Dark_Col_Channel(const ParamManage& manage);
    Subitem_Result Noise_Dark_Col_Channel_V1_1(const ParamManage& manage);
    Subitem_Result Noise_Dark_Read(const ParamManage& manage);

};

#endif // NOISETEST_H
