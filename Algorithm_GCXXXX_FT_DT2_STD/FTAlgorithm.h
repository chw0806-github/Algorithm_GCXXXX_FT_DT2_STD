#ifndef FTALGORITHM_H
#define FTALGORITHM_H

#if defined(FTALGORITHM_LIBRARY)
#  define FTALGORITHMSHARED_EXPORT extern "C" __declspec(dllexport)
#else
#  define FTALGORITHMSHARED_EXPORT extern "C" __declspec(dllexport)
#endif

#include "TestItems/BrightTest.h"
#include "TestItems/Darktest.h"
#include "TestItems/LargeGainTest.h"
#include "TestItems/ColorbarTest.h"
#include "TestItems/NoiseTest.h"
#include "TestItems/DarkCurrent.h"
#include "TestItems/MapBinarization.h"
#include "TestItems/FindClusterType.h"
#include "TestItems/SearchSensorDD.h"
#include "TestItems/RangeTest.h"
#include "TestItems/LagTest.h"

#include <string>

namespace FT_Algorithm{

enum Product_ID {Product_GC2607};

enum Image_Type {GC_2L_FullSize_Bayer};

//enum DadkCurrent_Type {GC_DadkCurrent_Bayer, GC_NegtiveSignal_Bayer};

// 标准 STD 测试项类型（QString，降低耦合，调用方无需包含枚举定义）
// 有效值: "Normal_Bright", "Normal_Dark", "Large_Gain_Bright", "Range", "Lag", "All_Noise",
//         "MapBinarization_Dark", "MapBinarization_Bright", "Flicker_Binarization", "Colorbar", "Darkcurrent", "Dag",
//         "Three_BinaryMap_Merge", "Two_BinaryMap_Merge", "Find_ClusterType_in_binarymap"

// 标准 STD 统一参数结构体
struct StdTestParams {
    int site_num;
    int datatype;
    int frameCnt;
    int width;
    int height;

    uchar* pData;                    // 主输入数据
    uchar* pData_out;                // 二值化输出（MapBinarization/Flicker 用）
    uchar* pData_aux;                // 辅助输入（Lag: end帧, Darkcurrent: short_exp）
    const uchar* pData_in3;          // 第三路输入（Three_BinaryMap_Merge 用，nullptr 表示 Two_BinaryMap_Merge）
    uchar* pImage_save;              // 图像保存缓冲（可为 nullptr）

    QString paramName;               // paramNameBayer / testName 统一用此字段
    QString testContentFlag;         // "All", "IIC", "RAW8" 等
    const char* sample_path;         // Colorbar 采样路径
    void* data_raw;                  // Colorbar 原始数据 / Find_ClusterType 扩展参数（StdFindClusterParams*）
};

// Find_ClusterType_in_binarymap 扩展参数（通过 StdTestParams.data_raw 传递）
struct StdFindClusterParams {
    uchar* pMapImg;
    int crop_x, crop_y;
    uchar* pPixType;
    QMap<QPair<int,int>, int>* pDdInfoXy;
    int* pDdTotalCnt;
    int* pBigDdCnt;
    int* pOtpDdType;
    int ddTotalLimit;
    int* ddSpecialCnt;  //GC4602 专用
};




typedef unsigned char uchar;

FTALGORITHMSHARED_EXPORT bool init_cp_algorithm_parallel( Product_ID PRO_ID,int site_num,const char* path_,
uchar *data_start,int width_,int height_,int framecnt_=12,int resultcnt_=4,int buf_type=16);


/*-------------   对外 功能函数     ---------------*/


FTALGORITHMSHARED_EXPORT bool Three_BinaryMap_Merge (const uchar *in1_raw8,const uchar *in2_raw8,
                            const uchar *in3_raw8, uchar *out_raw8,Image_Type type_);

FTALGORITHMSHARED_EXPORT bool Two_BinaryMap_Merge (const uchar *in1_raw8,const uchar *in2_raw8,
                          uchar *out_raw8,Image_Type type_);

/*-------------   坏点查找、分类 功能函数    ---------------*/
FTALGORITHMSHARED_EXPORT int  Find_ClusterType_in_binarymap(int site_num, uchar *map_img, int crop_x, int crop_y,
                                                         uchar *pix_type,QMap<QPair<int,int>, int> &dd_info_xy, int &dd_total_cnt,
                                                         int &big_dd_cnt,int *otp_dd_type,int dd_totla_limit,int &dd_special_cnt);

///*-------------   黑屏测试 相关     ---------------*/
//FTALGORITHMSHARED_EXPORT int  TestFunc_Darkcurrent(int site_num,uchar *pData_long_exp,uchar *pData_short_exp,
//                                   int datatype,DadkCurrent_Type type_);

/*-------------   标准 std     ---------------*/
FTALGORITHMSHARED_EXPORT int TestFunc_Normal_Bright_Standard(int site_num,uchar *pData,int frameCnt,
                            int datatype,uchar *pImage_save,QString testName, QString testContentFlag, int width, int height);

FTALGORITHMSHARED_EXPORT int TestFunc_Normal_Dark_Standard(int site_num,uchar *pData,int frameCnt,
                            int datatype,uchar *pImage_save,QString testName, QString testContentFlag, int width, int height);

FTALGORITHMSHARED_EXPORT int TestFunc_Large_Gain_Bright_Standard(int site_num,uchar *pData,int frameCnt,
                            int datatype,uchar *pImage_save,QString testName, int width, int height);

FTALGORITHMSHARED_EXPORT int TestFunc_Range_Standard(int site_num,uchar *pData,int frameCnt,
                            int datatype,uchar *pImage_save,QString paramNameBayer, int width, int height);

FTALGORITHMSHARED_EXPORT int TestFunc_Lag_Standard(int site_num,uchar *pData_Lag_HL_first_doublebyte,
                            uchar *pData_Lag_HL_end_doublebyte,int datatype, QString paramNameBayer, int width, int height);

FTALGORITHMSHARED_EXPORT int TestFunc_ALL_Noise_Standard(int site_num,uchar *pData,int frameCnt,
                             int datatype,uchar *pImage_save,QString paramNameBayer, int width, int height);

FTALGORITHMSHARED_EXPORT int MapBinarization_dark_Standard(int site_num,  uchar *pData_in, uchar *pData_out, int framecnt,
                             int datatype, uchar *pImage_save,QString paramNameBayer, int width, int height);

FTALGORITHMSHARED_EXPORT int Flicker_pix_STD_Binarization_Standard(int site_num,  uchar *pData_in, uchar *pData_out, int framecnt,
                             int datatype, QString paramNameBayer, int width, int height);

FTALGORITHMSHARED_EXPORT int MapBinarization_bright_Standard(int site_num,  uchar *pData_in, uchar *pData_out, int framecnt,
                             int datatype, uchar *pImage_save,QString paramNameBayer, int width, int height);

FTALGORITHMSHARED_EXPORT int Testfunc_Colorbar_Standard(int site_num,void *data_raw, const char *sample_path,
                                             int datatype, uchar *pImage_save,QString paramNameBayer, QString testContentFlag, int width, int height);

FTALGORITHMSHARED_EXPORT int TestFunc_Darkcurrent_Standard(int site_num,uchar *pData_long_exp,uchar *pData_short_exp,
                                   int datatype,QString paramNameBayer, int width, int height);

FTALGORITHMSHARED_EXPORT int TestFunc_Dag_Standard(int site_num,uchar *pData,int frameCnt,
                                   int datatype,uchar *pImage_save,QString testName, int width, int height);

// 标准 STD 统一接口：根据 testType(QString) 分发到对应的测试函数
FTALGORITHMSHARED_EXPORT int TestFunc_Standard_Execute(const QString& testType,
                                                       const StdTestParams* params);

/*-------------  日志  相关      ---------------*/

FTALGORITHMSHARED_EXPORT void Enable_Debuglog(bool flag);

FTALGORITHMSHARED_EXPORT int GetMsgString(char *pBufOfMsg);

FTALGORITHMSHARED_EXPORT int GetLogString_Parallel(int site_num,char *pBufOfLog);

FTALGORITHMSHARED_EXPORT int GetMsgString_Parallel(int site_num,char *pBufOfMsg);

FTALGORITHMSHARED_EXPORT int GetMsgString_size(int site_num);

FTALGORITHMSHARED_EXPORT int GetLogString_size(int site_num);

FTALGORITHMSHARED_EXPORT QString FTAlgorithm_Version();

FTALGORITHMSHARED_EXPORT QString FTAlgorithm_GitHash();

FTALGORITHMSHARED_EXPORT QString FTAlgorithm_GitDiff();

}


#endif // FTALGORITHM_H
