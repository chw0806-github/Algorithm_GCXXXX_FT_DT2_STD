#include "FTAlgorithm.h"
#include "Params/parammanage.h"
#include "git_version_data.h"

#include <QFile>

#include <vector>
#include <ctime>
#include <QStringList>

#define row_noise_test
#define column_noise_test
#define Diff_PRNU_test
#define Edge_Line
#define Real_ROI_PRNU

#define FUNC_LOG_ON

static bool load_Params_flag=false;
static bool Save_Func_LOG=false;
extern bool Save_Debug_LOG;

extern QString msg_;
std::string g_str_msg_="";
std::vector<std::string> g_str_logtmp(Parallel_num);
std::vector<std::string> g_str_msg_all(Parallel_num);
std::vector<std::string> g_str_log_all(Parallel_num);

AlgorithmItems Algorithm_all[Parallel_num];

std::string kyec_log;
clock_t cast_time_step1;
clock_t cast_time_step2;

using namespace FT_Algorithm;
using namespace std;

FT_Algorithm::Product_ID product_id_;

static QMap<QString, ParamManage*> str2Param_Map;

bool getitems_path(const std::string &fpath,QMap<QString, QStringList> &pathdir_items);

bool Load_Params_From_File(QString file_path, const std::string path_dir,
                           const QStringList &itemlist, QString &errorlog,
                           QMap<QString, ParamManage*> str2Param);

bool Load_Params_All(const std::string& spec_dir);

bool set_str2Param_pathFile(const std::string& fpath);

bool write_func_log(int site_num,QString func_name,bool start_)
{
    //创建log文件
    if(Save_Func_LOG && site_num==0)
    {
        QString path= "./func_log.txt";
        QFile file(path);
        if(file.open(QIODevice::Append | QIODevice::Text))
        {
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_log =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
            current_log+=" : "+func_name;
            if(start_)
                current_log+=" start! \n";
            else
                current_log+=" end! \n";

            file.write(current_log.toStdString().c_str());
            file.close();
        }
    }
    return true;
}


FTALGORITHMSHARED_EXPORT bool FT_Algorithm::init_cp_algorithm_parallel(Product_ID PRO_ID,int site_num, const char *path_,
                                                            uchar *data_start,int width_,int height_,
                                                            int framecnt_,int resultcnt_,int buf_type)
{
    g_str_msg_.clear();
    if(site_num>=Parallel_num)
    {
        g_str_msg_+="site_num > Parallel_num \n\n";
        return false;
    }

    if(!load_Params_flag)
    {
//        set_str2Param();
        set_str2Param_pathFile(path_);

        string path(path_);
        if(path.size()==0)
        {
            g_str_msg_+="path_.size()==0!!! \n";
            return false;
        }
        product_id_=PRO_ID;
        if(!Load_Params_All(path))
            return false;

        load_Params_flag=true;
    }

    bool res=false;
    int param_w=p_param_manage_binary_->width_;
    int param_h=p_param_manage_binary_->height_;

    if(param_w==width_&&param_h==height_)
    {
        res=Algorithm_all[site_num].init(data_start,width_,height_,
                                     framecnt_,resultcnt_,buf_type);
        if(res)
            g_str_msg_+="init succeed!";
        else
            g_str_msg_+="init failed!";
    }
    else
        g_str_msg_+="param_w & param_h  !=  width_ & height_";

    return res;


}

bool set_str2Param_pathFile(const std::string& fpath)
{
    static QMap<QString, ParamManage*>str2Param;

    QString path_file = QString::fromStdString(fpath) + "/params-path.xml";
    QFile file(path_file);

    // 打开文件并检查是否成功
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file!";
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qWarning() << "Failed to parse XML!";
        file.close(); // 文件解析失败后关闭文件
        return false;
    }

    // 解析完成后自动关闭文件流
    file.close();

    QDomElement root = doc.documentElement();  // 获取根元素

    if (root.isNull()) {
        qWarning() << "Empty XML document.";
        return false;
    }

    // 遍历所有子节点并处理
    QDomNodeList childNodes = root.childNodes();
    for (int i = 0; i < childNodes.count(); ++i) {
        QDomNode node = childNodes.at(i);
        if (node.isElement()) {
            QDomElement element = node.toElement();
            QString nodeName = element.tagName();  // 获取节点名称
//            qDebug() << "Node Name:" << nodeName;

            ParamManage* param_manage = new ParamManage();
            str2Param[nodeName] = param_manage;

            // 处理更多节点数据
        }
    }

    str2Param [QString("param_binarization")]=p_param_manage_binary_;

    // 更新全局变量
    str2Param_Map = str2Param;

//    return true;
}

bool getitems_path(const std::string &fpath, QMap<QString, QStringList> &pathdir_items)
{

    QString path_file =QString::fromStdString(fpath)+"/params-path.xml";
    QFile fp(path_file);

    if(!fp.exists()){
        g_str_msg_+="filepath of params not exists";
        return false;
    }
    if(!fp.open(QIODevice::ReadOnly)){
        g_str_msg_+="can not open file!";
        return false;
    }

    QDomDocument doc;
    if(!doc.setContent(&fp)){
        g_str_msg_+="QDomDocument can not open file!";
        return false;
    }
    QDomElement root = doc.documentElement();
    QDomNode n1 = root.firstChild();
    while(!n1.isNull()){

        QDomElement e = n1.toElement();
        QString upitem =e.tagName();
        bool ok;
        QString fcnt = e.attribute("fcnt");
        int itemscnt =  fcnt.toInt(&ok);
        QStringList itemlist;
        QDomNode subn = e.firstChild();

        while(!subn.isNull()){
            QDomElement sube = subn.toElement();
            itemlist.append(sube.text());
            subn = subn.nextSibling();
        }

        if(itemscnt != itemlist.length())
        {
            g_str_msg_+="itemscnt != itemlist.length() ";
            return false;
        }

        pathdir_items[upitem] = itemlist;
        n1 = n1.nextSibling();
    }
    return true;
}

bool Load_Params_From_File(QString file_path, const std::string path_dir,
                           const QStringList &itemlist, QString &errorlog,
                           QMap<QString, ParamManage*> str2Param)
{
    bool ok;
    QString item;
    QStringList item_actual;
    QString path = QString::fromStdString(path_dir);

    ParamManage *tmp = str2Param[path];

    QString path_txt=file_path+"/"+QString::fromStdString(path_dir);

    //Basic
    item =QString("Basic.data");
    ok = tmp->loadBasicParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //binarization
    item =QString("binary.data");
    ok = tmp->loadBinaryParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("ROI_PRNU.data");
    ok = tmp->load_Real_ROI_PRNU_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //darkcurrent
    item =QString("darkcurrent.data");
    ok = tmp->loadDarkcurrentParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //global_darkcurrent
    item =QString("global_darkcurrent.data");
    ok = tmp->load_Darkcurrent_Global_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //histogram
    item =QString("histogram.data");
    ok = tmp->loadHistogramParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //Lag
    item =QString("Lag.data");
    ok = tmp->load_Lag_Grid_ROI_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //Lagre gain
    item =QString("EdgeLineDefect.data");
    ok = tmp->load_edge_line_defect_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("LightResponse.data");
    ok = tmp->load_light_response_param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("LineDefect.data");
    ok = tmp->load_line_defect_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("LineDefect_V1.1.data");
    ok = tmp->load_line_defect_Param_V1_1(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("LineDefect_IIC.data");
    ok = tmp->load_line_defect_iic_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("NonUniformity.data");
    ok = tmp->load_NonUniformity_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("POG.data");
    ok = tmp->load_POG_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("ImageDirt.data");
    ok = tmp->load_ImageDirttDetect_Bright_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("RelativeIllumination.data");
    ok = tmp->load_RI_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //DAG
    item =QString("Dag.data");
    ok = tmp->load_dag_param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("LightLinearity.data");
    ok = tmp->load_light_linearity_param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("DagMeanStd.data");
    ok = tmp->load_dag_mean_std_param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //Bright
    item =QString("ColorRatio.data");
    ok = tmp->load_Color_Ratio_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //Bright
    item =QString("ColorRatio_V1.1.data");
    ok = tmp->load_Color_Ratio_Param_V1_1 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("ColorShading.data");
    ok = tmp->load_Color_Shading_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("ColorShading_V1.1.data");
    ok = tmp->load_Color_Shading_Param_V1_1 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("ColorShading_5x5.data");
    ok = tmp->load_Color_Shading_5x5_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("ColorShading_15x15.data");
    ok = tmp->load_Color_Shading_15_15_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("ColorFourier.data");
    ok = tmp->load_Color_Fourier_Param(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("GI.data");
    ok = tmp->load_Color_GI_Param(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("GI_V1.1.data");
    ok = tmp->load_Color_GI_Param_V1_1(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("GI_5x5.data");
    ok = tmp->load_Color_GI_5x5_Param(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("Opticalcenter.data");
    ok = tmp->load_Optical_Center_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("PDCollect.data");
    ok = tmp->load_PD_Collect_Param(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("PixelDefect.data");
    ok = tmp->load_Pixel_Defect_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("PixelDefect_V1.1.data");
    ok = tmp->load_Pixel_Defect_Param_V1_1 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("PixelDefect_V1.2.data");
    ok = tmp->load_Pixel_Defect_Param_V1_2 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("PixelDefect_V1.3.data");
    ok = tmp->load_Pixel_Defect_Param_V1_3 (path_txt.toStdString());
    if (ok) item_actual.append(item);


    item =QString("PixelDefect_V1.4.data");
    ok = tmp->load_Pixel_Defect_Param_V1_4 (path_txt.toStdString());
    if (ok) item_actual.append(item);




    item =QString("PD.data");
    ok = tmp->load_Pixel_Defect_Single_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("PD_Channel_Single.data");
    ok = tmp->load_Pixel_Defect_Channel_single_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("POG.data");        //白屏pog 特殊录入
    ok = tmp->load_POG_Normal_Bright_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("MPKG.data");
    ok = tmp->loadMPkgParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("MPKG_V1.1.data");
    ok = tmp->loadMPkgParam_V1_1(path_txt.toStdString());
    if (ok) item_actual.append(item);


    item =QString("Blemish_V1.1.data");
    ok = tmp->loadMBlemishParam_V1_1 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("Blemish.data");
    ok = tmp->loadMBlemishParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //LGB
    item =QString("Mosaic.data");
    ok = tmp->loadMosaicParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //Dark
    item =QString("Channel_Noise.data");
    ok = tmp->loadChannelNoiseParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("Channel_Noise_V1.1.data");
    ok = tmp->loadChannelNoiseParam_V1_1 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("Column_Noise.data");
    ok = tmp->loadColumnNoiseParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("FPN_DL.data");
    ok = tmp->load_FPN_DL_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("FPN_DL_V1.1.data");
    ok = tmp->load_FPN_DL_Param_V1_1(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("FPN_DL_IIC.data");
    ok = tmp->load_FPN_DL_IIC_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("FPN_DL_Bright_Dark.data");
    ok = tmp->load_FPN_DL_bright_dark_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("FPN_DL_Bright_Dark_V1.1.data");
    ok = tmp->load_FPN_DL_bright_dark_Param_V1_1(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("FPN_DL_Bright_Dark_V1.2.data");
    ok = tmp->load_FPN_DL_bright_dark_Param_V1_2(path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("OB_DarkShading.data");
    ok = tmp->load_OB_DarkShading_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("OB_DarkShading_V1.1.data");
    ok = tmp->load_OB_DarkShading_Param_V1_1 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("OB_DarkShading_V1.2.data");
    ok = tmp->load_OB_DarkShading_Param_V1_2 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("OB_DarkShading_V1.3.data");
    ok = tmp->load_OB_DarkShading_Param_V1_3 (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("OB_GrGb.data");
    ok = tmp->load_OB_GrGb_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("Read_Noise.data");
    ok = tmp->load_Read_Noise_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("Row_Noise.data");
    ok = tmp->loadRowNoiseParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("LightRef.data");
    ok = tmp->load_DarkRef_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //Range
    item =QString("Roi_std.data");
    ok = tmp->load_ROI_STD_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    //colorbar
    item =QString("Colorbar.xml");
    ok = tmp->loadColorbarParam (path_txt.toStdString());
    if (ok) item_actual.append(item);

    item =QString("Edge_ROI_DL.data");
    ok = tmp->load_Edge_ROI_DeadLine_Dark_Param (path_txt.toStdString());
    if (ok) item_actual.append(item);

    bool ok_load=true;
    foreach(const QString item_tmp, itemlist)
    {
        if(item_tmp.endsWith(".tiff") || item_tmp.endsWith(".bmp"))
        {
            path_txt+="/"+item_tmp;
            tmp->colorbar_images[item_tmp]=path_txt;
            item_actual.append(item_tmp);
        }
        if(!item_actual.contains(item_tmp))
        {
            errorlog += msg_;
            errorlog += QString("%1 in xml was not load!!\n").arg(item_tmp);
            ok_load = false;
        }
    }
    msg_.clear();
    return ok_load;
}

bool Load_Params_All(const std::string& spec_dir)
{
    QMap<QString,QStringList> pathdir_items; //get all .data path and count

    if(!getitems_path(spec_dir, pathdir_items))
    {
        g_str_msg_+="load  params-path.xml error";
        return false;
    }

    QMap<QString, ParamManage*> str2Param_;
    switch (product_id_) {
    case Product_GC2607:
        str2Param_=str2Param_Map;  break;
    }

    QString errorlog;
    QMap<QString,QStringList>::iterator iter = pathdir_items.begin();
    while(iter!=pathdir_items.end())
    {
        QString path_dir = iter.key();
        QStringList itemlist = iter.value();
        QString load_log;
        iter ++;

        if(str2Param_.find(path_dir)==str2Param_.end())
        {
            g_str_msg_+="Error: str2Param no have: "+path_dir.toStdString()+"\n";
            continue;
        }

        load_log+="**** Begin load "+path_dir+" **** \n";
        bool load_flag=Load_Params_From_File( QString::fromStdString(spec_dir),
                                    path_dir.toStdString(),itemlist,errorlog,
                                    str2Param_);

        load_log+=errorlog;
        load_log+="**** End load "+path_dir+" **** \n\n";
        g_str_msg_+=load_log.toStdString();

        if(!load_flag)
            return false;

    }
    return true;
}


/*-------------   对外 功能函数     ---------------*/

//二值化图 3张 raw8合并
FTALGORITHMSHARED_EXPORT bool FT_Algorithm::Three_BinaryMap_Merge (const uchar *in1_raw8,const uchar *in2_raw8,
                            const uchar *in3_raw8, uchar *out_raw8,Image_Type type_)
{
    ParamManage mange_=g_param_manage_binary_;

    int w = mange_.width_;
    int h = mange_.height_;
    // ===== 参数校验 =====
    if (w<=0 || h<=0 || in1_raw8==NULL || in2_raw8==NULL || in3_raw8==NULL)
        return false;
    // ===== Step1：三图合并 → 输出到 out_raw8 =====
    for (int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            if(in1_raw8[i*w+j]==0 && in2_raw8[i*w+j]==0 && in3_raw8[i*w+j]==0)
                out_raw8[i*w+j]=0;
            else
                out_raw8[i*w+j]=255;
        }

    // ===== Step2：对 out_raw8 边缘清零 =====
    const int elem_size       = sizeof(out_raw8[0]);  // uchar = 1字节
    const int skip_row_top    = mange_.param_binary_.skip_row_top_;
    const int skip_row_bottom = mange_.param_binary_.skip_row_bottom_;
    const int skip_col_left   = mange_.param_binary_.skip_col_left_;
    const int skip_col_right  = mange_.param_binary_.skip_col_right_;

    // 极端情况保护
    if (skip_row_top + skip_row_bottom >= h) return false;
    if (skip_col_left + skip_col_right >= w) return false;

    for (int i = 0; i < h; ++i)
    {
        uchar* row = &out_raw8[i * w];  // ✅ 操作 out_raw8，类型匹配

        if ((skip_row_top    > 0 && i < skip_row_top) ||
            (skip_row_bottom > 0 && i >= h - skip_row_bottom))
        {
            // 前N行 / 最后N行：整行清零
            memset(row, 0, w * elem_size);
        }
        else
        {
            // 中间行：只清左右边缘
            if (skip_col_left  > 0)
                memset(row,                      0, skip_col_left  * elem_size);

            if (skip_col_right > 0)
                memset(row + w - skip_col_right, 0, skip_col_right * elem_size);
        }
    }



    return true;
}

//二值化图 2张 raw8合并
FTALGORITHMSHARED_EXPORT bool FT_Algorithm::Two_BinaryMap_Merge (const uchar *in1_raw8,const uchar *in2_raw8,
                          uchar *out_raw8,Image_Type type_)
{
    ParamManage mange_=g_param_manage_binary_;
    int w = mange_.width_;
    int h = mange_.height_;
    // ===== 参数校验 =====
    if (w<=0 || h<=0 || in1_raw8==NULL || in2_raw8==NULL )
        return false;
    // ===== Step1：三图合并 → 输出到 out_raw8 =====
    for (int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            if(in1_raw8[i*w+j]==0 && in2_raw8[i*w+j]==0)
                out_raw8[i*w+j]=0;
            else
                out_raw8[i*w+j]=255;
        }


    // ===== Step2：对 out_raw8 边缘清零 =====
    const int elem_size       = sizeof(out_raw8[0]);  // uchar = 1字节
    const int skip_row_top    = mange_.param_binary_.skip_row_top_;
    const int skip_row_bottom = mange_.param_binary_.skip_row_bottom_;
    const int skip_col_left   = mange_.param_binary_.skip_col_left_;
    const int skip_col_right  = mange_.param_binary_.skip_col_right_;

    // 极端情况保护
    if (skip_row_top + skip_row_bottom >= h) return false;
    if (skip_col_left + skip_col_right >= w) return false;

    for (int i = 0; i < h; ++i)
    {
        uchar* row = &out_raw8[i * w];  // ✅ 操作 out_raw8，类型匹配

        if ((skip_row_top    > 0 && i < skip_row_top) ||
            (skip_row_bottom > 0 && i >= h - skip_row_bottom))
        {
            // 前N行 / 最后N行：整行清零
            memset(row, 0, w * elem_size);
        }
        else
        {
            // 中间行：只清左右边缘
            if (skip_col_left  > 0)
                memset(row,                      0, skip_col_left  * elem_size);

            if (skip_col_right > 0)
                memset(row + w - skip_col_right, 0, skip_col_right * elem_size);
        }
    }

    return true;
}


FTALGORITHMSHARED_EXPORT int FT_Algorithm::Find_ClusterType_in_binarymap(int site_num, uchar *map_img, int crop_x, int crop_y,
                                                         uchar *pix_type, QMap<QPair<int,int>, int> &dd_info_xy,int &dd_total_cnt,
                                                         int &big_dd_cnt,int *otp_dd_type,int dd_totla_limit,int &dd_special_cnt)
{
    int  res=-1;
    write_func_log(site_num,"Find_ClusterType_in_binarymap ",true );
    if(site_num<Parallel_num)
    {
        FindClusterType test(site_num,crop_x,crop_y,map_img,pix_type,
                             dd_info_xy,otp_dd_type,dd_totla_limit);
        ParamManage mange_=g_param_manage_binary_;
       int type = mange_.param_binary_.burnable_typeNum;//获取配置的可烧录type数量
        switch (type) {
        case 3://只能烧录type0/1/2三种类型，大部分数码型号
            res=test.Find_ClusterType_in_binarymap_Type0(mange_,dd_total_cnt,big_dd_cnt,dd_totla_limit);
            break;
        case 11://能烧录11种type类型，如GC2607、手机型号
            res=test.Find_ClusterType_in_binarymap_Type11(mange_,dd_total_cnt,big_dd_cnt,dd_totla_limit);
            break;
        case 15://能烧录15种type类型，如GC10V0
            res=test.Find_ClusterType_in_binarymap_Type15(mange_,dd_total_cnt,big_dd_cnt,dd_totla_limit);
            break;
        case 152://GC4602 , dd_special_cnt
            res=test.Find_ClusterType_in_binarymap_Type15_Ex(mange_,dd_total_cnt,big_dd_cnt,dd_totla_limit,dd_special_cnt);
            break;
        }
    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }
    write_func_log(site_num,"Find_ClusterType_in_binarymap ",false);
    return res;
}


/*-------------  标准化  相关      ---------------*/
static int check_std_param_size(const ParamManage& mange_, int width, int height)
{
    if (mange_.width_ != width || mange_.height_ != height)
        return -3;
    return 0;
}

//标准 Standard   白屏
FTALGORITHMSHARED_EXPORT int  FT_Algorithm::TestFunc_Normal_Bright_Standard(int site_num,uchar *pData,int frameCnt,
                                                  int datatype,uchar *pImage_save,QString paramNameBayer, QString testContentFlag, int width, int height)
{
    int  res=-1;
    if(site_num<Parallel_num)
    {
        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return res;
        }
        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;
        Split_Channel_type channel_type_=Split_Bayer;
        BrightTest test_(site_num,channel_type_,frameCnt,datatype);

        if(testContentFlag == "All")
        {
            res= test_.TestFunc_Normal_Bright_ALL(mange_,pData,pImage_save);
        }
        if(testContentFlag == "allExceptPog")
        {
            res= test_.TestFunc_Normal_Bright_AllExceptPog(mange_,pData,pImage_save);
        }
        else if(testContentFlag == "IIC")
        {
            res= test_.TestFunc_IIC_Bright(mange_,pData,pImage_save);
        }
        else if(testContentFlag == "Pog")
        {
            res= test_.TestFunc_Normal_Bright_POG(mange_,pData,pImage_save);
        }

    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }
    return res;
}


//标准 Standard   黑屏
FTALGORITHMSHARED_EXPORT int  FT_Algorithm::TestFunc_Normal_Dark_Standard(int site_num,uchar *pData,int frameCnt,
                                                    int datatype,uchar *pImage_save,QString paramNameBayer, QString testContentFlag, int width, int height)
{
    int  res=-1;
    if(site_num<Parallel_num)
    {
        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return res;
        }
        ParamManage mange_ = * param;   //解引用
        Split_Channel_type channel_type_=Split_Bayer;
        DarkTest test_(site_num,channel_type_,frameCnt,datatype);

        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        if(testContentFlag == "All")
        {
            res= test_.TestFunc_Normal_Dark_ALL(mange_,pData,pImage_save);
        }
        else if(testContentFlag == "IIC")
        {
            res= test_.TestFunc_IIC_Dark_(mange_,pData,pImage_save);
        }
        else if(testContentFlag == "IIC_LIGHT")
        {
            res= test_.TestFunc_IIC_LIGHT_Dark_(mange_,pData,pImage_save);
        }
    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }
    return res;

}


//标准 Standard   灰屏
FTALGORITHMSHARED_EXPORT int  FT_Algorithm::TestFunc_Large_Gain_Bright_Standard(int site_num,uchar *pData,int frameCnt,
                                                    int datatype,uchar *pImage_save,QString paramNameBayer, int width, int height)
{
    int  res=-1;
    if(site_num<Parallel_num)
    {
        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return res;
        }

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_Bayer;

        LargeGainTest test_(site_num,channel_type_,frameCnt,datatype);

        res= test_.TestFunc_Large_Gain_Bright_Bayer(mange_,pData,pImage_save);
    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }
    return res;

}

//标准 Standard   高亮
FTALGORITHMSHARED_EXPORT int  FT_Algorithm::TestFunc_Range_Standard(int site_num,uchar *pData,int frameCnt,
                                                    int datatype,uchar *pImage_save,QString paramNameBayer, int width, int height)
{
    int  res=-1;
    if(site_num<Parallel_num)
    {
        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return res;
        }

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_Bayer;

        RangeTest test_(site_num,channel_type_,frameCnt,datatype);

        res= test_.TestFunc_Range_Bayer(mange_,pData,pImage_save);

    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }

    return res;
}


//标准 Standard   Lag
FTALGORITHMSHARED_EXPORT int  FT_Algorithm::TestFunc_Lag_Standard(int site_num,uchar *pData_Lag_HL_first_doublebyte,
                  uchar *pData_Lag_HL_end_doublebyte,int datatype, QString paramNameBayer, int width, int height)
{
    int  res=-1;
    if(site_num<Parallel_num)
    {
        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return res;
        }

        ParamManage mange_ = *param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_Bayer;

        LagTest test(site_num,channel_type_,2,datatype);

        res = test.TestFunc_Lag_bayer(mange_,pData_Lag_HL_first_doublebyte,
                                       pData_Lag_HL_end_doublebyte,datatype);
    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }
    return res;
}


//标准 Standard   Noise
FTALGORITHMSHARED_EXPORT int  FT_Algorithm::TestFunc_ALL_Noise_Standard(int site_num,uchar *pData,int frameCnt,
                             int datatype,uchar *pImage_save,QString paramNameBayer, int width, int height)
{
    int  res=-1;
    write_func_log(site_num,"TestFunc_ALL_Noise ",true );
    if(site_num<Parallel_num)
    {

        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return res;
        }

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_Bayer;

        NoiseTest test_(site_num,channel_type_,frameCnt,datatype);

        res= test_.TestFunc_ALL_Noise_Dark(mange_,pData,pImage_save);

    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }

    write_func_log(site_num,"TestFunc_ALL_Noise ", false );
    return res;
}


//黑屏分通道坏点查找
FTALGORITHMSHARED_EXPORT int FT_Algorithm::MapBinarization_dark_Standard(int site_num,  uchar *pData_in, uchar *pData_out, int framecnt,
                                            int datatype, uchar *pImage_save,QString paramNameBayer, int width, int height)
{
    if(site_num<Parallel_num)
    {
        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return -1;
        }

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_Bayer;

        MapBinarization test(site_num,channel_type_,framecnt,datatype);
        return test.FindDeadPixel_dark(mange_,pData_in,pData_out,pImage_save);

    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
        return -1;
    }
}


//分通道坏点查找  闪点
FTALGORITHMSHARED_EXPORT int FT_Algorithm::Flicker_pix_STD_Binarization_Standard(int site_num, uchar *pData_in, uchar *pData_out,
                                                        int frame_cnt, int datatype,QString paramNameBayer, int width, int height)
{
    if(site_num<Parallel_num)
    {
        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return -1;
        }

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_4C;

        MapBinarization test(site_num,channel_type_,frame_cnt,datatype);
        return test.Flicker_pix_STD_Binarization(mange_,pData_in,pData_out);
    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
        return -1;
    }
}


//白屏分通道坏点查找
FTALGORITHMSHARED_EXPORT int FT_Algorithm::MapBinarization_bright_Standard(int site_num, uchar *pData_in, uchar *pData_out, int framecnt,
                                                   int datatype, uchar *pImage_save,QString paramNameBayer, int width, int height)
{
    if(site_num<Parallel_num)
    {
        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return -1;
        }

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_Bayer;

        MapBinarization test(site_num,channel_type_,framecnt,datatype);
        return test.FindDeadPixel_bright(mange_,pData_in,pData_out,pImage_save);
    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
        return -1;
    }
}


FTALGORITHMSHARED_EXPORT int FT_Algorithm::Testfunc_Colorbar_Standard(int site_num,void *data_raw, const char *sample_path,
                                             int datatype, uchar *pImage_save,QString paramNameBayer, QString testContentFlag, int width, int height)
{
    int  res=-1;
    if(site_num<Parallel_num)
    {
        QString item=sample_path;

        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return res;
        }

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        if(mange_.colorbar_images.find(item)==mange_.colorbar_images.end())
        {
            g_str_msg_all[site_num].append(item.toStdString());
            g_str_msg_all[site_num].append(" is not in params-path xml list!!! \n");
            write_func_log(site_num,"Testfunc_Colorbar ",false );
            return res;
        }
        std::string image_path=mange_.colorbar_images[item].toStdString();
        char path_char[200];
        strcpy(path_char,image_path.c_str());

        Split_Channel_type channel_type_=Split_Bayer;
        ColorbarTest test(site_num,channel_type_,1,datatype);


        if(testContentFlag == "RAW8")
        {
            uchar* uchar_data = static_cast<uchar*>(data_raw);
            res=test.Testfunc_Colorbar(mange_,uchar_data,path_char,pImage_save);
        }
        else if(testContentFlag == "RAW16")
        {
            short* short_data = static_cast<short*>(data_raw);
            res=test.Testfunc_Colorbar_RAW16(mange_,short_data,path_char,pImage_save);
        }


    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }
    return res;
}

FTALGORITHMSHARED_EXPORT int  FT_Algorithm::TestFunc_Darkcurrent_Standard(int site_num,uchar *pData_long_exp,uchar *pData_short_exp,
                                   int datatype,QString paramNameBayer, int width, int height)
{
    int  res=-1;
    write_func_log(site_num,"TestFunc_Darkcurrent ",true );
    if(site_num<Parallel_num)
    {

        ParamManage* param = nullptr;
        if (str2Param_Map.contains(paramNameBayer))
            param = str2Param_Map[paramNameBayer];
         else
        {
            return res;
        }

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_Bayer;

        DarkCurrent test_(site_num,channel_type_,2,datatype);

        res=test_.TestFunc_Darkcurrent(mange_,pData_long_exp,pData_short_exp);

    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }
    write_func_log(site_num,"TestFunc_Darkcurrent ",false );

    return res;
}

//标准 Standard   Dag
FTALGORITHMSHARED_EXPORT int  FT_Algorithm::TestFunc_Dag_Standard(int site_num,uchar *pData,int frameCnt,
                                  int datatype,uchar *pImage_save,QString paramNameBayer, int width, int height)
{
    int  res=-1;
    if(site_num<Parallel_num)
    {
        ParamManage* param = str2Param_Map.value(paramNameBayer);

        ParamManage mange_ = * param;   //解引用
        int wh_chk = check_std_param_size(mange_, width, height);
        if (wh_chk != 0)
            return wh_chk;

        Split_Channel_type channel_type_=Split_Bayer;

        DarkTest test_(site_num,channel_type_,frameCnt,datatype);

        res= test_.TestFunc_DAG_(mange_,pData,pImage_save);
    }
    else
    {
        g_str_msg_.clear();
        g_str_msg_+="site_num is out of range.";
    }
    return res;
}

// 标准 STD 统一接口实现（QString 类型，降低耦合）
FTALGORITHMSHARED_EXPORT int FT_Algorithm::TestFunc_Standard_Execute(const QString& testType,
                                                                     const StdTestParams* params)
{
    if (params == nullptr)
    {
        g_str_msg_.clear();
        g_str_msg_ += "params is nullptr!!!\n";
        return -1;
    }
    if (params->site_num >= Parallel_num)
    {
        g_str_msg_.clear();
        g_str_msg_ += "site_num is out of range.\n";
        return -1;
    }

    const StdTestParams* p = params;

    if (testType == QLatin1String("Normal_Bright"))
        return TestFunc_Normal_Bright_Standard(p->site_num, p->pData, p->frameCnt,
            p->datatype, p->pImage_save, p->paramName, p->testContentFlag, p->width, p->height);
    if (testType == QLatin1String("Normal_Dark"))
        return TestFunc_Normal_Dark_Standard(p->site_num, p->pData, p->frameCnt,
            p->datatype, p->pImage_save, p->paramName, p->testContentFlag, p->width, p->height);
    if (testType == QLatin1String("Large_Gain_Bright"))
        return TestFunc_Large_Gain_Bright_Standard(p->site_num, p->pData, p->frameCnt,
            p->datatype, p->pImage_save, p->paramName, p->width, p->height);
    if (testType == QLatin1String("Range"))
        return TestFunc_Range_Standard(p->site_num, p->pData, p->frameCnt,
            p->datatype, p->pImage_save, p->paramName, p->width, p->height);
    if (testType == QLatin1String("Lag"))
        return TestFunc_Lag_Standard(p->site_num, p->pData, p->pData_aux,
            p->datatype, p->paramName, p->width, p->height);
    if (testType == QLatin1String("All_Noise"))
        return TestFunc_ALL_Noise_Standard(p->site_num, p->pData, p->frameCnt,
            p->datatype, p->pImage_save, p->paramName, p->width, p->height);
    if (testType == QLatin1String("MapBinarization_Dark"))
        return MapBinarization_dark_Standard(p->site_num, p->pData, p->pData_out,
            p->frameCnt, p->datatype, p->pImage_save, p->paramName, p->width, p->height);
    if (testType == QLatin1String("MapBinarization_Bright"))
        return MapBinarization_bright_Standard(p->site_num, p->pData, p->pData_out,
            p->frameCnt, p->datatype, p->pImage_save, p->paramName, p->width, p->height);
    if (testType == QLatin1String("Flicker_Binarization"))
        return Flicker_pix_STD_Binarization_Standard(p->site_num, p->pData, p->pData_out,
            p->frameCnt, p->datatype, p->paramName, p->width, p->height);
    if (testType == QLatin1String("Colorbar"))
        return Testfunc_Colorbar_Standard(p->site_num, p->data_raw, p->sample_path,
            p->datatype, p->pImage_save, p->paramName, p->testContentFlag, p->width, p->height);
    if (testType == QLatin1String("Darkcurrent"))
        return TestFunc_Darkcurrent_Standard(p->site_num, p->pData, p->pData_aux,
            p->datatype, p->paramName, p->width, p->height);
    if (testType == QLatin1String("Dag"))
        return TestFunc_Dag_Standard(p->site_num, p->pData, p->frameCnt,
            p->datatype, p->pImage_save, p->paramName, p->width, p->height);
    if (testType == QLatin1String("Three_BinaryMap_Merge")) {
        if (p->pData == nullptr || p->pData_aux == nullptr || p->pData_in3 == nullptr || p->pData_out == nullptr) {
            g_str_msg_.clear();
            g_str_msg_ += "Three_BinaryMap_Merge: pData/pData_aux/pData_in3/pData_out required.\n";
            return 0;
        }
        bool ok = Three_BinaryMap_Merge(static_cast<const uchar*>(p->pData), static_cast<const uchar*>(p->pData_aux),
            static_cast<const uchar*>(p->pData_in3), p->pData_out, GC_2L_FullSize_Bayer);
        return ok ? 1 : 0;
    }
    if (testType == QLatin1String("Two_BinaryMap_Merge")) {
        if (p->pData == nullptr || p->pData_aux == nullptr || p->pData_out == nullptr) {
            g_str_msg_.clear();
            g_str_msg_ += "Two_BinaryMap_Merge: pData/pData_aux/pData_out required.\n";
            return 0;
        }
        bool ok = Two_BinaryMap_Merge(static_cast<const uchar*>(p->pData), static_cast<const uchar*>(p->pData_aux),
            p->pData_out, GC_2L_FullSize_Bayer);
        return ok ? 1 : 0;
    }
    if (testType == QLatin1String("Find_ClusterType_in_binarymap")) {
        StdFindClusterParams* fp = static_cast<StdFindClusterParams*>(p->data_raw);
        if (fp == nullptr || fp->pMapImg == nullptr || fp->pPixType == nullptr || fp->pDdInfoXy == nullptr
            || fp->pDdTotalCnt == nullptr || fp->pBigDdCnt == nullptr || fp->pOtpDdType == nullptr || fp->ddSpecialCnt == nullptr) {
            g_str_msg_.clear();
            g_str_msg_ += "Find_ClusterType_in_binarymap: StdFindClusterParams required.\n";
            return -1;
        }
        return Find_ClusterType_in_binarymap(p->site_num, fp->pMapImg, fp->crop_x, fp->crop_y,
            fp->pPixType, *fp->pDdInfoXy, *fp->pDdTotalCnt, *fp->pBigDdCnt, fp->pOtpDdType, fp->ddTotalLimit, *fp->ddSpecialCnt);
    }

    g_str_msg_.clear();
    g_str_msg_ += "unknown testType: " + testType.toStdString() + "\n";
    return -1;
}


/*-------------  日志  相关      ---------------*/

FTALGORITHMSHARED_EXPORT void FT_Algorithm::Enable_Debuglog(bool flag)
{
    Save_Func_LOG=flag;
    Save_Debug_LOG=flag;
}

FTALGORITHMSHARED_EXPORT int FT_Algorithm::GetMsgString(char *pBufOfMsg)
{
    memset(pBufOfMsg,'\0',g_str_msg_.length ()+1);
    memcpy (pBufOfMsg,g_str_msg_.c_str (),g_str_msg_.length ());

    return static_cast<int>(g_str_msg_.length());
}

FTALGORITHMSHARED_EXPORT int FT_Algorithm::GetMsgString_size(int site_num)
{
    size_t site_num_=static_cast<size_t>(site_num);
    if(site_num_<Parallel_num)
        return static_cast<int>(g_str_msg_all[site_num_].length())+10;
    else
        return -1;
}

FTALGORITHMSHARED_EXPORT int FT_Algorithm::GetLogString_size(int site_num)
{
    size_t site_num_=static_cast<size_t>(site_num);
    if(site_num_<Parallel_num)
        return static_cast<int>(g_str_log_all[site_num_].length())+10;
    else
        return -1;
}

FTALGORITHMSHARED_EXPORT int FT_Algorithm::GetLogString_Parallel(int site_num,char *pBufOfLog)
{
    size_t site_num_=static_cast<size_t>(site_num);
    if(site_num<Parallel_num)
    {
        memset(pBufOfLog,'\0',g_str_log_all[site_num_].length ()+1);
        memcpy (pBufOfLog,g_str_log_all[site_num_].c_str (),g_str_log_all[site_num_].length ());
        g_str_log_all[site_num_].clear();
        return 1;
    }
    else
        return -1;
}

FTALGORITHMSHARED_EXPORT int FT_Algorithm::GetMsgString_Parallel(int site_num,char *pBufOfMsg)
{
    size_t site_num_=static_cast<size_t>(site_num);
    if(site_num<Parallel_num)
    {
        memset(pBufOfMsg,'\0',g_str_msg_all[site_num_].length ()+1);
        memcpy (pBufOfMsg,g_str_msg_all[site_num_].c_str (),g_str_msg_all[site_num_].length ());
        g_str_msg_all[site_num_].clear();
        return 1;
    }
    return -1;
}




/*-------------  算法库版本  相关      ---------------*/

FTALGORITHMSHARED_EXPORT QString FT_Algorithm::FTAlgorithm_Version()
{
    // 使用从 .pro 文件中传递的版本号宏
    return QString(APP_VERSION);
}

FTALGORITHMSHARED_EXPORT QString FT_Algorithm::FTAlgorithm_GitHash()
{
    return QString(GIT_COMMIT_HASH);
}

FTALGORITHMSHARED_EXPORT QString FT_Algorithm::FTAlgorithm_GitDiff()
{
    return QString(GIT_DIFF_STATUS);
}

