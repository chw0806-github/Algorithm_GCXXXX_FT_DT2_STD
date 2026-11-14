#include "ColorbarTest.h"

int ColorbarTest::Testfunc_Colorbar(ParamManage& test_manage,uchar *data_raw,
                      const char *sample_path,uchar *pImage_save)
{
    if (test_manage.param_colorbar_.w<=0 ||
        test_manage.param_colorbar_.h<=0)
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    if (!QFile::exists(QString::fromStdString(sample_path)))
    {
        g_str_msg_all[site_].append(sample_path);
        g_str_msg_all[site_].append("\n");
        g_str_msg_all[site_].append("colorbar sample image not exist!!!  Colorbar not test...\n");
        return -1;
    }

    //分配空间并分通道

    bool result_Split=Algorithm_all[site_].Split_Channel(test_manage, Split_Bayer,false,
                                               data_raw,1,test_datatype,pImage_save);
    if(!result_Split)
    {
        g_str_msg_all[site_].append ("Split_Channel failed!\n");
        return -1;
    }
    else
        g_buf_raw_=Algorithm_all[site_].g_buf_raw_;

    int code=Colorbar_Test(test_manage,sample_path,Colorbar_normal);
    return code;
}




int ColorbarTest::Testfunc_Colorbar_RAW16(ParamManage& test_manage,short *data_raw/*uchar *data_raw*/,
                      const char *sample_path,uchar *pImage_save)
{
    if (test_manage.param_colorbar_.w<=0 ||
        test_manage.param_colorbar_.h<=0)
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    if (!QFile::exists(QString::fromStdString(sample_path)))
    {
        g_str_msg_all[site_].append(sample_path);
        g_str_msg_all[site_].append("\n");
        g_str_msg_all[site_].append("colorbar sample image not exist!!!  Colorbar not test...\n");
        return -1;
    }

    g_buf_raw_16_ = data_raw;
    int code=Colorbar_Test_RAW16(test_manage,sample_path,Colorbar_normal);
    return code;


}




int ColorbarTest::Colorbar_Test(ParamManage& manage,const char *sample_path,
                                  Colorbar_type type)
{
    int cnt_dif = 0;
    int code=1;

    int width=0;
    int height=0;
    double dif_min_pattern_=0;
    double dif_ratio_pattern_=0;
    std::vector<MRect> areas_pattern_;

    uchar * pattern_sample_ =NULL;
    uchar * binary=NULL;

    if(type == Colorbar_normal)
    {
        width= manage.param_colorbar_.w;
        height= manage.param_colorbar_.h;
        dif_min_pattern_= manage.param_colorbar_.dif_min_pattern_;
        dif_ratio_pattern_= manage.param_colorbar_.dif_ratio_pattern_;
        areas_pattern_= manage.param_colorbar_.areas_pattern_;

    }

    if (pattern_sample_ == NULL)
    {
        ////load pattern sample/////
        cv::Mat img= cv::imread(sample_path,0);
        int w = img.cols;
        int h = img.rows;


        if(!Algorithm_all[site_].New_Pointer_Buf (&pattern_sample_,Alculate_area,
                                        Buf_uchar_8,w,h,1))
            return -1;

        if (pattern_sample_ == NULL)
        {
            g_str_msg_all[site_].append("allocate memory for pattern fail!!!");
            return -1;
        }

        for (int i=0; i<h; ++i)
        {
            uchar *p = img.ptr<uchar>(i);
            for(int j=0; j<w; ++j)
                pattern_sample_[i*w+j] = p[j];
        }
    }

    g_str_msg_all[site_].append (QString("dif_pix:%1 dif_ratio:%2\n").
                       arg (QString::number (dif_min_pattern_),
                            QString::number (dif_ratio_pattern_)).
                       toStdString().c_str());

    foreach (const MRect &area, areas_pattern_)
    {
        for (int i=area.y_; i<area.y_+area.h_; ++i)
        {
            for (int j=area.x_; j<area.x_+area.w_; ++j)
            {
                if (qAbs((int)pattern_sample_[width*i+j]-(int)g_buf_raw_[width*i+j])>
                        std::max(dif_min_pattern_,
                                 (int)pattern_sample_[width*i+j]*dif_ratio_pattern_/100)
                        )
                {
                    if(type==Colorbar_special)
                        binary[i*width+j]=255;
                    cnt_dif+=1;
                    if(cnt_dif<10)
                    {
                        g_str_msg_all[site_].append(QString::number(pattern_sample_[width*i+j]).toStdString().c_str());
                        g_str_msg_all[site_].append("---------");
                        g_str_msg_all[site_].append(QString::number(g_buf_raw_[width*i+j]).toStdString().c_str());
                        g_str_msg_all[site_].append("   X Pos:   ");
                        g_str_msg_all[site_].append(QString::number(j).toStdString().c_str());
                        g_str_msg_all[site_].append("   Y Pos:   ");
                        g_str_msg_all[site_].append(QString::number(i).toStdString().c_str());
                        g_str_msg_all[site_].append("\n");
                    }
                }
                else if(type==Colorbar_special)
                    binary[i*width+j]=0;
            }
        }
    }

    if(type==Colorbar_normal)
    {
        if (cnt_dif>manage.param_colorbar_.limit_cnt_pattern)
            code = manage.param_colorbar_.bin_;
        else
            code = 1;
    }

    Algorithm_all[site_].Free_Pointer_Buf(&pattern_sample_);
    g_str_log_all[site_]+=boost::str(boost::format("%d|")% cnt_dif);
    g_str_log_all[site_]+=boost::str(boost::format("%s|") % "Colorbar");
    g_str_msg_all[site_]+=boost::str(boost::format("Mismatch_Cnt:%d;\n")% cnt_dif);

    return code;
}




int ColorbarTest::Colorbar_Test_RAW16(ParamManage& manage,const char *sample_path,
                                  Colorbar_type type)
{
    int cnt_dif = 0;
    int code=1;

    int width=0;
    int height=0;
    double dif_min_pattern_=0;
    double dif_ratio_pattern_=0;
    std::vector<MRect> areas_pattern_;

    uint16_t *pattern_sample_ = NULL;
    uchar * binary=NULL;

    if(type == Colorbar_normal)
    {
        width= manage.param_colorbar_.w;
        height= manage.param_colorbar_.h;
        dif_min_pattern_= manage.param_colorbar_.dif_min_pattern_;
        dif_ratio_pattern_= manage.param_colorbar_.dif_ratio_pattern_;
        areas_pattern_= manage.param_colorbar_.areas_pattern_;

    }



    if (pattern_sample_ == NULL) {
        ////load 16-bit RAW pattern sample/////
        cv::Mat img = cv::imread(sample_path, cv::IMREAD_ANYDEPTH | cv::IMREAD_GRAYSCALE);

        if (img.empty()) {
            g_str_msg_all[site_].append("Failed to load 16-bit RAW image!!!");
            return -1;
        }

        int w = img.cols;
        int h = img.rows;


        // 显式计算缓冲区大小
        size_t data_size = w * h * sizeof(uint16_t);

        // 验证数据范围
        uint16_t min_val = *std::min_element(img.begin<uint16_t>(), img.end<uint16_t>());
        uint16_t max_val = *std::max_element(img.begin<uint16_t>(), img.end<uint16_t>());
        if (min_val < 0 || max_val > 65535) {
            g_str_msg_all[site_].append("Invalid pixel range in image!!!");
            return -1;
        }


        // 创建一个临时缓冲区来保存 p_algo_data_ 的副本
        pattern_sample_ = new uint16_t[w * h];  // 使用 uint16_t 来匹配图像的数据类型

        // 使用 memcpy 提升效率
        memcpy(pattern_sample_, img.data, data_size);

    }


    g_str_msg_all[site_].append (QString("dif_pix:%1 dif_ratio:%2\n").
                       arg (QString::number (dif_min_pattern_),
                            QString::number (dif_ratio_pattern_)).
                       toStdString().c_str());

    foreach (const MRect &area, areas_pattern_)
    {
        for (int i=area.y_; i<area.y_+area.h_; ++i)
        {
            for (int j=area.x_; j<area.x_+area.w_; ++j)
            {
                if (qAbs((int)pattern_sample_[width*i+j]-(int)g_buf_raw_16_[width*i+j])>
                        std::max(dif_min_pattern_,
                                 (int)pattern_sample_[width*i+j]*dif_ratio_pattern_/100)
                        )
                {
                    if(type==Colorbar_special)
                        binary[i*width+j]=255;
                    cnt_dif+=1;
                    if(cnt_dif<10)
                    {
                        g_str_msg_all[site_].append(QString::number(pattern_sample_[width*i+j]).toStdString().c_str());
                        g_str_msg_all[site_].append("---------");
                        g_str_msg_all[site_].append(QString::number(g_buf_raw_16_[width*i+j]).toStdString().c_str());
                        g_str_msg_all[site_].append("   X Pos:   ");
                        g_str_msg_all[site_].append(QString::number(j).toStdString().c_str());
                        g_str_msg_all[site_].append("   Y Pos:   ");
                        g_str_msg_all[site_].append(QString::number(i).toStdString().c_str());
                        g_str_msg_all[site_].append("\n");
                    }
                }
                else if(type==Colorbar_special)
                    binary[i*width+j]=0;
            }
        }
    }

    if(type==Colorbar_normal)
    {
        if (cnt_dif>manage.param_colorbar_.limit_cnt_pattern)
            code = manage.param_colorbar_.bin_;
        else
            code = 1;
    }

    // 使用完后不要忘记释放内存
    delete[] pattern_sample_;

    g_str_log_all[site_]+=boost::str(boost::format("%d|")% cnt_dif);
    g_str_log_all[site_]+=boost::str(boost::format("%s|") % "Colorbar_RAW16");
    g_str_msg_all[site_]+=boost::str(boost::format("Mismatch_Cnt:%d;\n")% cnt_dif);

    return code;
}



