#ifndef MAPBINARIZATION_H
#define MAPBINARIZATION_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];

enum MapBinarization_type {Binarization_4C,Binarization_bayer};

class MapBinarization :public CPBaseTestItem
{
public:

    MapBinarization(int site_num,Split_Channel_type type_,
               int frameCnt,int datatype):
    test_channel_type(type_),
    test_frameCnt(frameCnt),
    test_datatype(datatype)
    {
        name = "MapBinarization";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);

    }

    ~MapBinarization(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:

    Split_Channel_type test_channel_type;
    int test_frameCnt;
    int test_datatype;

public:

    int FindDeadPixel_bright( ParamManage& manage,uchar *pData_in, uchar *pData_out, uchar *pImage_save);

    int FindDeadPixel_dark(ParamManage& manage,uchar *pData_in, uchar *pData_out, uchar *pImage_save);

    int Flicker_pix_STD_Binarization( ParamManage& manage,uchar *pData_in, uchar *pData_out);

    int w, h;

private:

    bool Static_MapBinarization_bright(const ParamManage& manage,
                            MapBinarization_type type, uchar *pData_out);

    bool Static_MapBinarization_dark(const ParamManage& manage,
                            MapBinarization_type type, uchar *pData_out);

    bool Flicker_MapBinarization(const ParamManage& manage,
                                 int frame_cnt, uchar *pData_out);

    bool Flicker_MapBinarization_chao(const ParamManage& manage,uchar *pData,
                                 int frame_cnt, uchar *pData_out);
};

#endif // MAPBINARIZATION_H
