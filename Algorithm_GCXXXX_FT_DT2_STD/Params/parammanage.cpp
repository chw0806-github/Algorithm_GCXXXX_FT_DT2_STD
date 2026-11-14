#include "parammanage.h"


#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>

QString msg_;
bool Save_Debug_LOG=false;



void logMsg(const std::string &msg,size_t site_, int grade)
{

    if(Save_Debug_LOG&&site_==0)
    {
        //grade, 0:error, 1:warnning, 2:message
        QDateTime current_date = QDateTime::currentDateTime();
        QString date_temp = current_date.toString("yyyy-MM-dd");
        std::string path_log = date_temp.toStdString();
        path_log.append (".log");
        std::ofstream f(path_log.c_str (),std::ios_base::app);
        if (f)
        {
            QDateTime current_time = QDateTime::currentDateTime();
            QString time_temp = current_time.toString("yyyy-MM-dd hh:mm:ss");
            std::string str_time = time_temp.toStdString();
            f<<grade<<" ## "<<str_time<<" ## "<<msg.c_str ();
            f<<"\n";
            f.flush ();
            f.close ();
        }
    }

}



bool ParamManage::loadColorbarParam (const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Colorbar.xml");
    QFile file(QString::fromStdString(path_file));
    if (!file.open (QFile::ReadOnly))
        return 0;

    QDomDocument xmlDoc("ColorbarXml");
    if (!xmlDoc.setContent (&file))
    {
        msg_.append ("Colorbar.xml format error\n");
        file.close();
        return 0;
    }

    file.close();
    this->param_colorbar_.areas_pattern_.clear();
    QDomElement docElem = xmlDoc.documentElement();
    QDomNode n = docElem.firstChild();

    while (!n.isNull())
    {
        QDomElement e = n.toElement();
        if (!e.isNull())
        {
            if (e.tagName() == QString("width"))
            {
                bool ok;
                this->param_colorbar_.w = e.text().toInt (&ok);
                if (!ok)
                {
                    msg_.append ("<width> error\n");
                    return 0;
                }
            }

            else if (e.tagName() == QString("height"))
            {
                bool ok;
                this->param_colorbar_.h = e.text().toInt(&ok);
                if (!ok)
                {
                    msg_.append ("<height> error\n");
                    return 0;
                }
            }
            else if (e.tagName() == QString("dif_pix_min"))
            {
                bool ok;
                this->param_colorbar_.dif_min_pattern_ = e.text().toInt(&ok);
                if (!ok)
                {
                    msg_.append("<dif_pix_min> error\n");
                    return 0;
                }
            }
            else if (e.tagName() == QString("dif_pix_ratio"))
            {
                bool ok;
                this->param_colorbar_.dif_ratio_pattern_ = e.text().toInt(&ok);
                if (!ok)
                {
                    msg_.append("<dif_pix_ratio> error\n");
                    return 0;
                }
            }
            else if (e.tagName() == QString("limit_cnt"))
            {
                bool ok;
                this->param_colorbar_.limit_cnt_pattern = e.text().toInt(&ok);
                if (!ok)
                {
                    msg_.append("<limit_cnt> error\n");
                    return 0;
                }
            }
            else if (e.tagName() == QString("area"))
            {
                MRect rect_t;
                bool ok1,ok2,ok3,ok4;
                rect_t.x_ = e.attribute("x").toInt(&ok1);
                rect_t.y_ = e.attribute("y").toInt(&ok2);
                rect_t.w_ = e.attribute("w").toInt(&ok3);
                rect_t.h_ = e.attribute("h").toInt(&ok4);
                this->param_colorbar_.areas_pattern_.push_back(rect_t);
                if (!(ok1 && ok2 && ok3 && ok4))
                {
                    msg_.append ("<area> error\n");
                    return 0;
                }
            }

            else if (e.tagName() == QString("bin"))
            {
                bool ok;
                this->param_colorbar_.bin_=e.text().toInt(&ok);
                if (!ok)
                {
                    msg_.append("<bin> error\n");
                    return 0;
                }
            }

            else if (e.tagName() == QString("grade"))
            {
                bool ok;
                this->param_colorbar_.grade_=e.text().toInt(&ok);
                if (!ok)
                {
                    msg_.append("<grade> error\n");
                    return 0;
                }
            }
        }
        n = n.nextSibling();
    }
    return 1;

}



bool ParamManage::loadBinaryParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/binary.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_binary_spec_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.dark_binary_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[dark_binary_spec_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_binary_spec_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.dark_binary_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[dark_binary_spec_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_binary_spec_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.dark_binary_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[dark_binary_spec_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_binary_spec_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.dark_binary_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[dark_binary_spec_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_binary_radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.dark_binary_radius_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[dark_binary_radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bright_binary_spec_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.bright_binary_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[bright_binary_spec_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bright_binary_spec_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.bright_binary_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[bright_binary_spec_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bright_binary_spec_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.bright_binary_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[bright_binary_spec_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bright_binary_spec_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.bright_binary_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[bright_binary_spec_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bright_binary_radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.bright_binary_radius_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bright_binary_radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[flicker_pix_binary_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.flicker_pix_binary_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[flicker_pix_binary_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[burnable_typeNum]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.burnable_typeNum = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[burnable_typeNum] tag error!!!\n");
            }
        }
        else if(line.startsWith("[type1_or_type11]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.type1_or_type11_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[type1_or_type11] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_row_top]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.skip_row_top_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_row_top] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_row_bottom]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.skip_row_bottom_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_row_bottom] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_col_left]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.skip_col_left_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_col_left] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_col_right]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_binary_.skip_col_right_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_col_right] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Binary Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 18)
    {
        msg_.append("Binary Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::loadMPkgParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/MPKG.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pkg_version = "mpkg_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[threshold]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.threshold_sobel_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[threshold] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.skip_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[code]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.code_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[code] tag error!!!\n");
            }
        }
        else if(line.startsWith("[code_white]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.code_white_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[code_white] tag error!!!\n");
            }
        }
        else if(line.startsWith("[mottle_cnt_threshold]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.MTottleParam.count = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[mottle_cnt_threshold] tag error!!!\n");
            }
        }
        else if(line.startsWith("[mottle_code]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.MTottleParam.code = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[mottle_code] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("PKG Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 9)
    {
        msg_.append("PKG Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}

bool ParamManage::loadMPkgParam_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/MPKG_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pkg_version = "mpkg_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[threshold]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.threshold_sobel_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[threshold] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.skip_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[code]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.code_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[code] tag error!!!\n");
            }
        }
        else if(line.startsWith("[code_white]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.code_white_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[code_white] tag error!!!\n");
            }
        }
        else if(line.startsWith("[mottle_cnt_threshold]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.MTottleParam.count = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[mottle_cnt_threshold] tag error!!!\n");
            }
        }
        else if(line.startsWith("[mottle_code]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.MTottleParam.code = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[mottle_code] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }

        //排除dpc off的影响
        else if(line.startsWith("[ratio_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pkg_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("PKG Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 13)
    {
        msg_.append("PKG Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::loadMBlemishParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Blemish.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    blemish_version = "blemish_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[winSmall]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.winSmall = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[winSmall] tag error!!!\n");
            }
        }
        else if(line.startsWith("[winBig]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.winBig = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[winBig] tag error!!!\n");
            }
        }
        else if(line.startsWith("[threshold_center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.threshold_center_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[threshold_center] tag error!!!\n");
            }
        }
        else if(line.startsWith("[threshold_margin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.threshold_margin_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[threshold_margin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[max_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.max_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[max_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_rect]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.skip_rect = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_rect] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X] tag error!!!\n");
            }
        }

        else if(line.startsWith("[ROI_Y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_W]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_W] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_H]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_H] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.code_dark_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Blemish Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 16)
    {
        msg_.append("Blemish Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}




bool ParamManage::loadMBlemishParam_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Blemish_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    blemish_version = "blemish_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[winSmall]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.winSmall = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[winSmall] tag error!!!\n");
            }
        }
        else if(line.startsWith("[winBig]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.winBig = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[winBig] tag error!!!\n");
            }
        }
        else if(line.startsWith("[threshold_center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.threshold_center_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[threshold_center] tag error!!!\n");
            }
        }
        else if(line.startsWith("[threshold_margin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.threshold_margin_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[threshold_margin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[max_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.max_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[max_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_rect]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.skip_rect = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_rect] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X] tag error!!!\n");
            }
        }

        else if(line.startsWith("[ROI_Y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_W]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_W] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_H]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_H] tag error!!!\n");
            }
        }
        else if(line.startsWith("[lsc_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.lsc_ratio_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[lsc_ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[resize_w]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.resize_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[resize_w_] tag error!!!\n");
            }
        }
        else if(line.startsWith("[resize_h]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.resize_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[resize_h_] tag error!!!\n");
            }
        }

        //---------------------------------------------------------------------
        else if(line.startsWith("[magnification]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.magnification = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[magnification] tag error!!!\n");
            }
        }
        else if(line.startsWith("[winSmall_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.winSmall_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[winSmall_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[winBig_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.winBig_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[winBig_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[threshold_center_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.threshold_center_enhace = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[threshold_center_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[threshold_margin_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.threshold_margin_enhace = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[threshold_margin_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.limit_size_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[max_size_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.max_size_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[max_size_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.skip_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_rect_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.skip_rect_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_rect_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.x_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_enhace] tag error!!!\n");
            }
        }

        else if(line.startsWith("[ROI_Y_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.y_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_W_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.w_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_W_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_H_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.h_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_H_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[lsc_ratio_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.lsc_ratio_enhace = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[lsc_ratio_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[resize_w_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.resize_w_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[resize_w_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[resize_h_enhace]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.resize_h_enhace = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[resize_h_enhace] tag error!!!\n");
            }
        }
        else if(line.startsWith("[search_area_x]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.search_area_x = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[search_area_x] tag error!!!\n");
            }
        }
        else if(line.startsWith("[search_area_y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.search_area_y = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[search_area_y] tag error!!!\n");
            }
        }

        else if(line.startsWith("[min_enhace_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.min_enhace_size = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[min_enhace_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[max_enhace_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.max_enhace_size = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[max_enhace_size] tag error!!!\n");
            }
        }


        //--------------------------

        else if(line.startsWith("[bin_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.code_dark_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
        //排除dpc off的影响
        else if(line.startsWith("[ratio_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.DN_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.DN_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.DN_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.DN_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_b] tag error!!!\n");
            }
        }

        else if(line.startsWith("[pd_area_x]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.pd_area_x_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[pd_area_x] tag error!!!\n");
            }
        }
        else if(line.startsWith("[pd_area_y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.pd_area_y_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[pd_area_y] tag error!!!\n");
            }
        }

        else if(line.startsWith("[pd_skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[pd_single_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_blemish_old_.pd_single_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[pd_single_open_mode] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Blemish Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 53)
    {
        msg_.append("Blemish Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}
bool ParamManage::load_PD_Collect_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/PDCollect.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            pd_collect.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_spec_level1_]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            pd_collect.DN_diff_spec_level1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_spec_level1_] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_spec_level2_]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            pd_collect.DN_diff_spec_level2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_spec_level2_] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_spec_level3_]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            pd_collect.DN_diff_spec_level3_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_spec_level3_] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            pd_collect.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }

        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            pd_collect.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }

       }
    file.close();

    if(!ok)
    {
        msg_.append("PDCollect Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 6)
    {
        msg_.append("PDCollect Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Pixel_Defect_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/PixelDefect.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pixel_defect_version = "pixel_defect_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade5_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade6]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade6_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade6] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade7]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade7_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade7] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade8]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade8_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade8] tag error!!!\n");
            }
        }

        // ---- add special defect pixel:2x2 global ----
        else if(line.startsWith("[bin_2x2_global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_global_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade9]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade9_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade9] tag error!!!\n");
            }
        }

        // ---- add single pix cnt control ----
        else if(line.startsWith("[bin_single_pix]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_single_pix_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade10]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade10_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade10] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit] tag error!!!\n");
            }
        }

        // ---------------------------
        else if(line.startsWith("[limit_cnt1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_cnt1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_cnt1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_cnt2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_cnt2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_cnt2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_DN_Spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_dn_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_DN_Spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_global_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_global_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_global_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_bychannel_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_bychannel_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_bychannel_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
        // 双坏点阈值卡控
        else if(line.startsWith("[all_1x2_2x1_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.all_1x2_2x1_spec = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[all_1x2_2x1_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[type] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_type] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("PixelDefect Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 61)
    {
        msg_.append("PixelDefect Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_Pixel_Defect_Param_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/PixelDefect_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pixel_defect_version = "pixel_defect_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade5_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade6]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade6_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade6] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade7]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade7_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade7] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade8]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade8_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade8] tag error!!!\n");
            }
        }

        // ---- add special defect pixel:2x2 global ----
        else if(line.startsWith("[bin_2x2_global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_global_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade9]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade9_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade9] tag error!!!\n");
            }
        }

        // ---- add single pix cnt control ----
        else if(line.startsWith("[bin_single_pix]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_single_pix_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade10]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade10_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade10] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit] tag error!!!\n");
            }
        }

        // ---------------------------
        else if(line.startsWith("[limit_cnt1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_cnt1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_cnt1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_cnt2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_cnt2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_cnt2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_DN_Spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_dn_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_DN_Spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_global_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_global_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_global_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_bychannel_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_bychannel_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_bychannel_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
        // 双坏点阈值卡控
        else if(line.startsWith("[all_1x2_2x1_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.all_1x2_2x1_spec = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[all_1x2_2x1_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[type] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_type] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("PixelDefect Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 61)
    {
        msg_.append("PixelDefect Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_Pixel_Defect_Param_V1_2(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/PixelDefect_V1.2.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pixel_defect_version = "pixel_defect_V1.2";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade5_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade6]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade6_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade6] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade7]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade7_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade7] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade8]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade8_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade8] tag error!!!\n");
            }
        }

        // ---- add special defect pixel:2x2 global ----
        else if(line.startsWith("[bin_2x2_global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_global_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade9]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade9_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade9] tag error!!!\n");
            }
        }

        // ---- add single pix cnt control ----
        else if(line.startsWith("[bin_single_pix]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_single_pix_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade10]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade10_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade10] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit] tag error!!!\n");
            }
        }

        // ---------------------------
        else if(line.startsWith("[limit_cnt1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_cnt1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_cnt1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_cnt2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_cnt2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_cnt2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_DN_Spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_dn_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_DN_Spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_global_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_global_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_global_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_bychannel_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_bychannel_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_bychannel_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
        // 双坏点阈值卡控
        else if(line.startsWith("[all_1x2_2x1_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.all_1x2_2x1_spec = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[all_1x2_2x1_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[type] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_type] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("PixelDefect Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 61)
    {
        msg_.append("PixelDefect Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_Pixel_Defect_Param_V1_3(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/PixelDefect_V1.3.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pixel_defect_version = "pixel_defect_V1.3";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade5_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade6]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade6_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade6] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade7]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade7_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade7] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade8]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade8_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade8] tag error!!!\n");
            }
        }

        // ---- add special defect pixel:2x2 global ----
        else if(line.startsWith("[bin_2x2_global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_global_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade9]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade9_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade9] tag error!!!\n");
            }
        }

        // ---- add single pix cnt control ----
        else if(line.startsWith("[bin_single_pix]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_single_pix_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade10]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade10_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade10] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit] tag error!!!\n");
            }
        }

        // ---------------------------
        else if(line.startsWith("[limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp3] tag error!!!\n");
            }
        }

        else if(line.startsWith("[blemish_SpecR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_DN_Spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_dn_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_DN_Spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_global_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_global_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_global_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_bychannel_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_bychannel_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_bychannel_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
        // 双坏点阈值卡控
        else if(line.startsWith("[all_1x2_2x1_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.all_1x2_2x1_spec = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[all_1x2_2x1_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[type] tag error!!!\n");
            }
        }

    }
    file.close();

    if(!ok)
    {
        msg_.append("PixelDefect Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 70)
    {
        msg_.append("PixelDefect Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}
bool ParamManage::load_Pixel_Defect_Param_V1_4(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/PixelDefect_V1.4.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pixel_defect_version = "pixel_defect_V1.4";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.DN_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade4_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade5_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade6]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade6_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade6] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_1x2_2x1_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin1x2_2x1_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_1x2_2x1_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade7]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade7_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade7] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_2x2_diagonal_ch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_diagonal_ch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_diagonal_ch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade8]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade8_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade8] tag error!!!\n");
            }
        }

        // ---- add special defect pixel:2x2 global ----
        else if(line.startsWith("[bin_2x2_global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin2x2_global_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_2x2_global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade9]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade9_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade9] tag error!!!\n");
            }
        }

        // ---- add single pix cnt control ----
        else if(line.startsWith("[bin_single_pix]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_single_pix_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade10]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade10_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade10] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit] tag error!!!\n");
            }
        }

        // ---------------------------
        else if(line.startsWith("[limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp3] tag error!!!\n");
            }
        }

        else if(line.startsWith("[blemish_SpecR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_DN_Spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_dn_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_DN_Spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_global_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_global_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_global_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_bychannel_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_bychannel_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_bychannel_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.bin_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.grade_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.blemish_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
        // 双坏点阈值卡控
        else if(line.startsWith("[all_1x2_2x1_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.all_1x2_2x1_spec = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[all_1x2_2x1_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[type] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_.single_pix_type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_type] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("PixelDefect Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 71)
    {
        msg_.append("PixelDefect Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}
bool ParamManage::load_Pixel_Defect_Single_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/PD.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    single_pixel_defect_version = "PD_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.DN_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.DN_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.DN_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.DN_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ratio_diff_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }


        // ---- add single pix cnt control ----
        else if(line.startsWith("[bin_single_pix]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.bin_single_pix_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade10]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.grade10_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade10] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.single_pix_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit] tag error!!!\n");
            }
        }

        // ---------------------------
        else if(line.startsWith("[limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.bin_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.bin_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.bin_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.bin_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.bin_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.bin_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.grade_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.grade_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.grade_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.grade_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.grade_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.grade_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_limit_dp3] tag error!!!\n");
            }
        }


        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }

        else if(line.startsWith("[type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[type] tag error!!!\n");
            }
        }

        else if(line.startsWith("[single_pix_type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_single_pixel_defect_.single_pix_type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_type] tag error!!!\n");
            }
        }


    }
    file.close();

    if(!ok)
    {
        msg_.append("PixelDefect Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 37)
    {
        msg_.append("PixelDefect Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Pixel_Defect_Channel_single_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/PD_Channel_Single.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pixel_defect_channel_single_version = "pixel_defect_channel_single_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.cnt_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.DN_diff_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.DN_diff_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.DN_diff_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DN_diff_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.DN_diff_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DN_diff_b] tag error!!!\n");
            }
        }

        else if(line.startsWith("[ratio_diff_r]="))
        {
          cnt_param += 1;
          QStringList line_t = line.split("=");
          param_pixel_defect_channel_single_.ratio_diff_spec_r_ = line_t[1].toDouble(&ok);
          if(!ok)
          {
              msg_.append("[ratio_diff_r] tag error!!!\n");
          }
        }
        else if(line.startsWith("[ratio_diff_gr]="))
        {
          cnt_param += 1;
          QStringList line_t = line.split("=");
          param_pixel_defect_channel_single_.ratio_diff_spec_gr_ = line_t[1].toDouble(&ok);
          if(!ok)
          {
              msg_.append("[ratio_diff_gr] tag error!!!\n");
          }
        }
        else if(line.startsWith("[ratio_diff_gb]="))
        {
          cnt_param += 1;
          QStringList line_t = line.split("=");
          param_pixel_defect_channel_single_.ratio_diff_spec_gb_ = line_t[1].toDouble(&ok);
          if(!ok)
          {
              msg_.append("[ratio_diff_gb] tag error!!!\n");
          }
        }
        else if(line.startsWith("[ratio_diff_b]="))
        {
          cnt_param += 1;
          QStringList line_t = line.split("=");
          param_pixel_defect_channel_single_.ratio_diff_spec_b_ = line_t[1].toDouble(&ok);
          if(!ok)
          {
              msg_.append("[ratio_diff_b] tag error!!!\n");
          }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }

        // ---------------------------
        else if(line.startsWith("[grade_single_pix]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.grade_single_pix = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_single_pix] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_limit_Class1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_single_pix_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_single_pix_Class1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_limit_Class2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_single_pix_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_single_pix_Class2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit_Class3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_limit_Class3 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit_Class3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_single_pix_Class3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_single_pix_Class3 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix_Class3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit_Class4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_limit_Class4 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit_Class4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_single_pix_Class4]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_single_pix_Class4 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix_Class4] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_limit_Class5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_limit_Class5 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_limit_Class5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_single_pix_Class5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_single_pix_Class5 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix_Class5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[only_bright_dark_pix_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.only_bright_dark_pix_mode = line_t[1];
            if(!ok)
            {
                msg_.append("[only_bright_dark_pix_mode] tag error!!!\n");
            }
        }

        else if(line.startsWith("[blemish_SpecR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_SpecB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_SpecB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_DN_Spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_dn_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_DN_Spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_global_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_global_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_global_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_bychannel_size_limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_bychannel_size_limit_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_bychannel_size_limit] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_blemish]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.grade_blemish_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_blemish] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_limit_wp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_limit_wp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_wp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_limit_wp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_wp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_limit_dp1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_limit_dp2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_limit_dp3]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.blemish_limit_dp3_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_limit_dp3] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
        // 双坏点阈值卡控
        else if(line.startsWith("[all_1x2_2x1_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.all_1x2_2x1_spec = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[all_1x2_2x1_spec] tag error!!!\n");
            }
        }
        //新增
        else if(line.startsWith("[single_pix_dark_limit_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_dark_limit_r = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_dark_limit_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_dark_limit_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_dark_limit_gr = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_dark_limit_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_dark_limit_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_dark_limit_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_dark_limit_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_dark_limit_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_dark_limit_b = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_dark_limit_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_single_pix_dark_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_single_pix_dark_channel = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix_dark_channel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_single_pix_dark_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.grade_single_pix_dark_channel = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_single_pix_dark_channel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_bright_limit_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_bright_limit_r = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_bright_limit_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_bright_limit_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_bright_limit_gr = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_bright_limit_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_bright_limit_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_bright_limit_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_bright_limit_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[single_pix_bright_limit_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.single_pix_bright_limit_b = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[single_pix_bright_limit_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_single_pix_bright_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_single_pix_bright_channel = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_single_pix_bright_channel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_single_pix_bright_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.grade_single_pix_bright_channel = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_single_pix_bright_channel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Channel_X]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.ROI_Channel_X = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Channel_X] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Channel_Y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.ROI_Channel_Y = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Channel_Y] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Channel_W]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.ROI_Channel_W = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Channel_W] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Channel_H]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.ROI_Channel_H = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Channel_H] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_dark_limit_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_dark_limit_r = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_dark_limit_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_dark_limit_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_dark_limit_gr = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_dark_limit_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_dark_limit_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_dark_limit_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_dark_limit_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_dark_limit_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_dark_limit_b = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_dark_limit_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_crop_single_pix_dark_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_crop_single_pix_dark_channel = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_crop_single_pix_dark_channel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_crop_single_pix_dark_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.grade_crop_single_pix_dark_channel = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_crop_single_pix_dark_channel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_dark_limit_gr_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_dark_limit_gr_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_dark_limit_gr_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_crop_single_pix_dark_channel_gr_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_crop_single_pix_dark_channel_gr_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_crop_single_pix_dark_channel_gr_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_crop_single_pix_dark_channel_gr_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.grade_crop_single_pix_dark_channel_gr_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_crop_single_pix_dark_channel_gr_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_bright_limit_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_bright_limit_r = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_bright_limit_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_bright_limit_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_bright_limit_gr = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_bright_limit_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_bright_limit_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_bright_limit_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_bright_limit_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop_single_pix_bright_limit_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_bright_limit_b = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_bright_limit_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_crop_single_pix_bright_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_crop_single_pix_bright_channel = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_crop_single_pix_bright_channel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_crop_single_pix_bright_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.grade_crop_single_pix_bright_channel = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_crop_single_pix_bright_channel] tag error!!!\n");
            }
        }


        else if(line.startsWith("[crop_single_pix_bright_limit_gr_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.crop_single_pix_bright_limit_gr_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop_single_pix_bright_limit_gr_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_crop_single_pix_bright_channel_gr_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.bin_crop_single_pix_bright_channel_gr_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_crop_single_pix_bright_channel_gr_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_crop_single_pix_bright_channel_gr_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.grade_crop_single_pix_bright_channel_gr_gb = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_crop_single_pix_bright_channel_gr_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[type]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pixel_defect_channel_single_.type_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[type] tag error!!!\n");
            }
        }

    }
    file.close();

    if(!ok)
    {
        msg_.append("PixelDefect_Gray_Single Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 77)
    {
        msg_.append("PixelDefect_Gray_Single Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}




bool ParamManage::load_Color_GI_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/GI.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    gi_ratio_version = "gi_ratio_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[GI_Ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.GI_Ratio = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[GI_Ratio] tag error!!!\n");
            }
        }

        else if(line.startsWith("[ratio_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.ratio_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ratio_open_mode] tag error!!!\n");
            }
        }

        else if(line.startsWith("[bin_GI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.bin_GI_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_GI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_GI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.grade_GI_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_GI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("GI Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 8)
    {
        msg_.append("GI Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_Color_GI_Param_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/GI_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    gi_ratio_version = "gi_ratio_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[GI_Ratio_class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.GI_Ratio_class1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[GI_Ratio_class1] tag error!!!\n");
            }
        }

        else if(line.startsWith("[GI_Ratio_class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.GI_Ratio_class2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[GI_Ratio_class2] tag error!!!\n");
            }
        }

        else if(line.startsWith("[ratio_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.ratio_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ratio_open_mode] tag error!!!\n");
            }
        }

        else if(line.startsWith("[bin_GI_class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.bin_GI_class1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_GI_class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_GI_class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.grade_GI_class1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_GI_class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_GI_class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.bin_GI_class2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_GI_class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_GI_class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.grade_GI_class2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_GI_class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("GI Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 11)
    {
        msg_.append("GI Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Color_GI_5x5_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/GI_5x5.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    gi_5_5_ratio_version = "gi_5_5_ratio_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetGI_Leftup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.TargetGI_Leftup = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetGI_Leftup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetGI_Rightup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.TargetGI_Rightup = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetGI_Rightup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetGI_Leftdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.TargetGI_Leftdown = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetGI_Leftdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetGI_Rightdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.TargetGI_Rightdown = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetGI_Rightdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetGI_Center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.TargetGI_Center = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetGI_Center] tag error!!!\n");
            }
        }

        else if(line.startsWith("[ratio_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.ratio_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ratio_open_mode] tag error!!!\n");
            }
        }

        else if(line.startsWith("[bin_GI_5x5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.bin_GI_5x5 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_GI_5x5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_GI_5x5]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.grade_GI_5x5 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_GI_5x5] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_GI_5x5.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("GI Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 12)
    {
        msg_.append("GI Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_Color_Ratio_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/ColorRatio.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    color_ratio_version = "color_ratio_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_leftup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_leftup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_leftup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_leftup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_leftup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_leftup] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_rightup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_rightup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_rightup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_rightup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_rightup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_rightup] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_center] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_center] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_center] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_leftdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_leftdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_leftdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_leftdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_leftdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_leftdown] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_rightdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_rightdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_rightdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_rightdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_rightdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_rightdown] tag error!!!\n");
            }
        }



        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_leftup_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target_offset["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_leftup_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_leftup_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target_offset["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_leftup_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_leftup_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target_offset["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_leftup_offset] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_rightup_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target_offset["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_rightup_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_rightup_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target_offset["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_rightup_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_rightup_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target_offset["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_rightup_offset] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_center_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target_offset["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_center_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_center_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target_offset["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_center_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_center_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target_offset["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_center_offset] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_leftdown_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target_offset["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_leftdown_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_leftdown_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target_offset["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_leftdown_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_leftdown_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target_offset["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_leftdown_offset] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_rightdown_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target_offset["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_rightdown_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_rightdown_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target_offset["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_rightdown_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_rightdown_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target_offset["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_rightdown_offset] tag error!!!\n");
            }
        }




        //---------------------------------------------------
        else if(line.startsWith("[Factor_Center_Gr_Gb_Ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_center_gr_gb_ratio = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_Gr_Gb_Ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Center_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_center_class1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Center_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_center_class2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Corner_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_corner_class1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Corner_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Corner_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_corner_class2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Corner_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_golden = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_limit = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Limit] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[bin_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Corner_RG_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_corner_rg_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Corner_RG_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Corner_RG_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_corner_rg_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Corner_RG_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Corner_RG_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_corner_rg_min_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Corner_RG_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Corner_RG_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_corner_rg_min_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Corner_RG_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Corner_BG_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_corner_bg_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Corner_BG_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Corner_BG_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_corner_bg_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Corner_BG_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Corner_BG_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_corner_bg_min_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Corner_BG_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Corner_BG_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_corner_bg_min_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Corner_BG_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.ratio_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ratio_open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Gr_Gb_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.gr_gb_diff_spec = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Gr_Gb_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_GI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_GI_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_GI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_GI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_GI_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_GI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("ColorRatio Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 59)
    {
        msg_.append("ColorRatio Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_Color_Ratio_Param_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/ColorRatio_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    color_ratio_version = "color_ratio_V1.1";


    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_leftup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_leftup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_leftup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_leftup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_leftup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["leftup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_leftup] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_rightup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_rightup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_rightup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_rightup] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_rightup]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["rightup"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_rightup] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_center] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_center] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_center]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["center"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_center] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_leftdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_leftdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_leftdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_leftdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_leftdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["leftdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_leftdown] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[TargetRatioGG_rightdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_rightdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_rightdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_rightdown] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_rightdown]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["rightdown"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_rightdown] tag error!!!\n");
            }
        }
        //---------新增center补偿值------
        else if(line.startsWith("[TargetRatioGG_center_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["offset"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_center_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_center_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["offset"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_center_offset] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_center_offset]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["offset"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_center_offset] tag error!!!\n");
            }
        }
        //---------新增rate，按比例补偿------------
        else if(line.startsWith("[TargetRatioGG_center_rate]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.GG_Ratio_target["rate"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioGG_center_rate] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioRG_center_rate]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.RG_Ratio_target["rate"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioRG_center_rate] tag error!!!\n");
            }
        }
        else if(line.startsWith("[TargetRatioBG_center_rate]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.BG_Ratio_target["rate"] = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[TargetRatioBG_center_rate] tag error!!!\n");
            }
        }
        //---------------------------------------------------
        else if(line.startsWith("[Factor_Center_Gr_Gb_Ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_center_gr_gb_ratio = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_Gr_Gb_Ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Center_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_center_class1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Center_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_center_class2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Corner_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_corner_class1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Corner_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Corner_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_corner_class2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Corner_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_golden = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Limit]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.factor_limit = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Limit] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[bin_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Corner_RG_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_corner_rg_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Corner_RG_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Corner_RG_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_corner_rg_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Corner_RG_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Corner_RG_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_corner_rg_min_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Corner_RG_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Corner_RG_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_corner_rg_min_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Corner_RG_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Corner_BG_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_corner_bg_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Corner_BG_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Corner_BG_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_corner_bg_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Corner_BG_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_ratio_Corner_BG_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_color_ratio_corner_bg_min_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ratio_Corner_BG_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ratio_Corner_BG_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_color_ratio_corner_bg_min_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ratio_Corner_BG_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ratio_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.ratio_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ratio_open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Gr_Gb_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.gr_gb_diff_spec = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Gr_Gb_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_GI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.bin_GI_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_GI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_GI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.grade_GI_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_GI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_ratio_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("ColorRatio Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 50)
    {
        msg_.append("ColorRatio Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;


}






bool ParamManage::load_Color_Fourier_Param(const std::string &dir)
{

    std::string path_file = dir;
    path_file.append("/ColorFourier.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }

        else if(line.startsWith("[RG_TopLeft_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.rg_top_left_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG_TopLeft_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.rg_top_left_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG_TopRight_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.rg_top_right_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG_TopRight_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.rg_top_right_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }

        else if(line.startsWith("[RG_BtmLeft_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.rg_btm_left_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG_BtmLeft_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG_BtmLeft_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.rg_btm_left_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG_BtmLeft_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG_BtmRight_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.rg_btm_right_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG_BtmRight_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG_BtmRight_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.rg_btm_right_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG_BtmRight_Max] tag error!!!\n");
            }
        }

        else if(line.startsWith("[BG_TopLeft_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bg_top_left_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG_TopLeft_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG_TopLeft_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bg_top_left_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG_TopLeft_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG_TopRight_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bg_top_right_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG_TopRight_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG_TopRight_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bg_top_right_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG_TopRight_Max] tag error!!!\n");
            }
        }

        else if(line.startsWith("[BG_BtmLeft_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bg_btm_left_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG_BtmLeft_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG_BtmLeft_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bg_btm_left_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG_BtmLeft_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG_BtmRight_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bg_btm_right_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG_BtmRight_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG_BtmRight_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bg_btm_right_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG_BtmRight_Max] tag error!!!\n");
            }
        }


        else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode]  tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ratio_fourier_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }

    }
    file.close();

    if(!ok)
    {
        msg_.append("ColorRatio Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 23)
    {
        msg_.append("ColorRatio Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;

}



bool ParamManage::load_dag_param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Dag.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
    {
        path_file.clear();
        path_file = dir;
        path_file.append("/Dag_V1.1.data");
        file.setFileName(QString::fromStdString(path_file));
        if(!file.open(QFile::Text | QFile::ReadOnly))
            return false;
        dag_version = "DAG_V1.1";
    }
    else
        dag_version = "DAG_V1.0";

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[ROI_COL]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag.roi_col_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ROI_COL] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_ROW]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag.roi_row_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ROI_ROW] tag error!!!\n");
            }
        }
        else if(line.startsWith("[VAL]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag.val_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[VAL] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dag]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag.bin_dag_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[bin_dag] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dag]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag.grade_dag_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[grade_dag] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dag_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag.dag_open_mode_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[dag_open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag.log_align_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }

    }
    file.close();

    if(!ok)
    {
        msg_.append("LightResponse Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 7)
    {
        msg_.append("LightResponse Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;

}



bool ParamManage::load_light_linearity_param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/LightLinearity.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    light_linearity_version = "LightLinearity_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[LINEAR_SPEC]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.linear_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[LINEAR_SPEC] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MAX_DN]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.max_dn = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MAX_DN] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.roi_x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.roi_y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_W]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.roi_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_W] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_H]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.roi_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_H] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_linear]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.bin_linearity_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_linear] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_linear]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.grade_linearity_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_linear] tag error!!!\n");
            }
        }
        else if(line.startsWith("[linear_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.linearity_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[linear_open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_linearity_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("LightLinearity Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 10)
    {
        msg_.append("LightLinearity Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_dag_mean_std_param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/DagMeanStd.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    dag_mean_std_version = "DAG_StdMean_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[std]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag_std_mean.std_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[std] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dag_mean]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag_std_mean.bin_dag_mean_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[bin_dag_mean] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dag_mean]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag_std_mean.grade_dag_mean_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[grade_dag_mean] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag_std_mean.open_mode_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }

        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dag_std_mean.log_align_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }

    }
    file.close();

    if(!ok)
    {
        msg_.append("DagMeanStd Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 5)
    {
        msg_.append("DagMeanStd Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;

}





bool ParamManage::load_light_response_param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/LightResponse.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    param_light_response_.error_min_open = false;
    param_light_response_.error_max_open = false;

    light_response_version = "LightResponse_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[MinMeanR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.limit_r_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinMeanR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxMeanR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.limit_r_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxMeanR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinMeanGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.limit_gr_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinMeanGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxMeanGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.limit_gr_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxMeanGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinMeanGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.limit_gb_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinMeanGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxMeanGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.limit_gb_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxMeanGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinMeanB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.limit_b_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinMeanB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxMeanB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.limit_b_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxMeanB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.roi_x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.roi_y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_W]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.roi_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_W] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_H]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.roi_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_H] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[bin_response]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.bin_response_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_response] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_response]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.grade_response_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_response] tag error!!!\n");
            }
        }
        else if(line.startsWith("[response_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.response_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[response_open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_response_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
        else if(line.startsWith("[error_MIN]="))
        {
            QStringList line_t = line.split("=");
            param_light_response_.error_min = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[error_MIN] tag error!!!\n");
            }
            else
                param_light_response_.error_min_open = true;
        }
        else if(line.startsWith("[error_MAX]="))
        {
            QStringList line_t = line.split("=");
            param_light_response_.error_max = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[error_MAX] tag error!!!\n");
            }
            else
                param_light_response_.error_max_open = true;
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("LightResponse Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 16)
    {
        msg_.append("LightResponse Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}





bool ParamManage::load_NonUniformity_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/NonUniformity.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    non_uniformity_version = "NonUniformity_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[w_cell]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.w_cell_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[w_cell] tag error!!!\n");
            }
        }
        else if(line.startsWith("[h_cell]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.h_cell_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[h_cell] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.spec_n_uniform_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.spec_n_uniform_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.spec_n_uniform_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.spec_n_uniform_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_n_uniform_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxGr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_n_uniform_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxGr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxGb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_n_uniform_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxGb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_n_uniform_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Mirror_Diff_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_mirror_diff_n_uniform_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Mirror_Diff_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Mirror_Diff_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_mirror_diff_n_uniform_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Mirror_Diff_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Mirror_Diff_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_mirror_diff_n_uniform_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Mirror_Diff_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Mirror_Diff_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_mirror_diff_n_uniform_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Mirror_Diff_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Updown_Diff_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_updown_diff_n_uniform_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Updown_Diff_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Updown_Diff_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_updown_diff_n_uniform_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Updown_Diff_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Updown_Diff_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_updown_diff_n_uniform_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Updown_Diff_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Updown_Diff_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.max_updown_diff_n_uniform_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Updown_Diff_B] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[bin_mean]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.bin_mean_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_mean] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_mean]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.grade_mean_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_mean] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.bin_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.grade_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_mirror_diff_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.bin_mirror_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_mirror_diff_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_mirror_diff_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.grade_mirror_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_mirror_diff_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_updown_diff_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.bin_updown_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_updown_diff_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_updown_diff_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.grade_updown_max_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_updown_diff_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_non_uniformity_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("NonUniformity Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 29)
    {
        msg_.append("NonUniformity Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_DarkRef_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/LightRef.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    light_ref_version = "lightRef_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[ColRefMax_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.col_ref_MAX_max = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ColRefMax_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ColRefMax_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.col_ref_MAX_min = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ColRefMax_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ColRefMin_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.col_ref_MIN_max = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ColRefMin_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ColRefMin_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.col_ref_MIN_min = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ColRefMin_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Split_screen_Left_Right_Dif]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.Ref_Split_screen_ROI_dif = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Split_screen_Left_Right_Dif] tag error!!!\n");
            }
        }


        //---------------------------------------------------
        else if(line.startsWith("[bin_ref_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.bin_ref_col = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ref_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ref_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.grade_ref_col = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ref_col] tag error!!!\n");
            }
        }

        else if(line.startsWith("[bin_ref_split_screen]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.bin_ref_Split_screen = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_ref_split_screen] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_ref_split_screen]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.grade_ref_Split_screen = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_ref_split_screen] tag error!!!\n");
            }
        }


        else if(line.startsWith("[ref_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.ref_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ref_open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_light_ref_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("LightRef Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 11)
    {
        msg_.append("LightRef Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Darkcurrent_Global_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/global_darkcurrent.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip_x]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.skip_x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_x] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.skip_y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_y] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[roi_w]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[roi_w] tag error!!!\n");
            }
        }
        else if(line.startsWith("[roi_h]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[roi_h] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_height]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.dark_height_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[dark_height] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_roi1_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[dark_roi1_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[dark_roi1_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[dark_roi1_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[dark_roi2_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[dark_roi2_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[dark_roi2_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[dark_roi2_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi1_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi1_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi1_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi1_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi2_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi2_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi2_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi2_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi3_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi3_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi3_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi3_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi4_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi4_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi4_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi4_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi5_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi5_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi5_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi5_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi6_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi6_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi6_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi6_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi7_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi7_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi7_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi7_max]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi8_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi8_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi8_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi8_max]tag error!!!\n");
            }
        }
        else if(line.startsWith("[active_roi9_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_min_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi9_min]tag error!!!\n");
            }
        }

        else if(line.startsWith("[active_roi9_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.roi_dkc_spec_max_.append(line_t[1].toDouble(&ok));
            if(!ok)
            {
                msg_.append("[active_roi9_max]tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_darkcurrent]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.bin_darkcurrent_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_darkcurrent] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_darkcurrent]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_dakrcurrent_global_.grade_darkcurrent_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_darkcurrent] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("darkcurrent Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 29)
    {
        msg_.append("darkcurrent Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::loadDarkcurrentParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/darkcurrent.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_area_line_total_cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.dark_area_line_total_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[dark_area_line_total_cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[dark_area_line_center_cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.dark_area_line_center_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[dark_area_line_center_cnt] tag error!!!\n");
            }
        }

        //---------------------------------------------------
        else if(line.startsWith("[roi_w]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.roi_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[roi_w] tag error!!!\n");
            }
        }
        else if(line.startsWith("[roi_h]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.roi_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[roi_h] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_dark_area_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_dark_area_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_dark_area_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_dark_area_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_dark_area_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_dark_area_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_leftup_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_leftup_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_leftup_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_leftup_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_leftup_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_leftup_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_rightup_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_rightup_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_rightup_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_rightup_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_rightup_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_rightup_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_leftdown_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_leftdown_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_leftdown_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_leftdown_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_leftdown_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_leftdown_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_rightdown_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_rightdown_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_rightdown_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_rightdown_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_rightdown_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_rightdown_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_center_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_center_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_center_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_center_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_center_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_center_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_whole_area_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_whole_area_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_whole_area_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[DC_whole_area_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.DC_whole_area_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[DC_whole_area_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_darkcurrent]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.bin_darkcurrent_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_darkcurrent] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_darkcurrent]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_darkcurrent_spec_.grade_darkcurrent_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_darkcurrent] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("darkcurrent Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 21)
    {
        msg_.append("darkcurrent Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_line_defect_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/LineDefect.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    deadrow_deadcol_bright_version = "deadrow_deadcol_bright_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_R_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_r_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_R_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gr_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_gr_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gr_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gb_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_gb_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gb_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_B_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_b_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_B_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_R_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_r_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_R_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gr_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_gr_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gr_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gb_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_gb_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gb_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_B_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_b_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_B_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_R_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_r_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_R_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gr_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_gr_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gr_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gb_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_gb_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gb_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_B_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_b_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_B_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_R_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_r_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_R_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gr_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_gr_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gr_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gb_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_gb_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gb_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_B_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_b_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_B_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_row_dol_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_row_dol_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_row_dol_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_row_dol_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_col_dol_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_col_dol_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_col_dol_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_col_dol_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dead_row_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dead_row_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dead_col_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dead_col_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Line Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 50)
    {
        msg_.append("Line Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_line_defect_Param_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/LineDefect_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    deadrow_deadcol_bright_version = "deadrow_deadcol_bright_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_R_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_r_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_R_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gr_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_gr_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gr_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gb_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_gb_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gb_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_B_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_b_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_B_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_R_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_r_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_R_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gr_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_gr_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gr_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gb_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_gb_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gb_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_B_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadrow_spec_b_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_B_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_R_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_r_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_R_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gr_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_gr_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gr_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gb_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_gb_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gb_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_B_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_b_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_B_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_R_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_r_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_R_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gr_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_gr_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gr_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gb_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_gb_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gb_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_B_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.deadcol_spec_b_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_B_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_row_dol_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_row_dol_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_row_dol_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_row_dol_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_col_dol_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_col_dol_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_col_dol_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.spec_col_dol_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dead_row_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dead_row_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dead_col_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dead_col_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_r_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_r_level_1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_r_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_gr_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_gr_level_1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_gr_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_gb_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_gb_level_1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_gb_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_b_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_b_level_1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_b_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_r_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_r_level_1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_r_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_gr_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_gr_level_1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_gr_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_gb_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_gb_level_1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_gb_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_b_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_b_level_1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_b_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_break_line_level1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line_level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_break_line_level1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line_level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_r_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_r_level_2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_r_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_gr_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_gr_level_2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_gr_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_gb_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_gb_level_2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_gb_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_b_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakrow_spec_b_level_2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_b_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_r_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_r_level_2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_r_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_gr_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_gr_level_2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_gr_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_gb_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_gb_level_2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_gb_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_b_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.breakcol_spec_b_level_2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_b_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.bin_break_line_level2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line_level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.grade_break_line_level2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line_level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Line Param V1.1 error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 60)
    {
        msg_.append("Line Param V1.1 not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_line_defect_iic_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/LineDefect_IIC.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    line_defect_bright_iic_version = "line_defect_bright_iic_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_R_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadrow_spec_r_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_R_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gr_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadrow_spec_gr_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gr_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gb_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadrow_spec_gb_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gb_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_B_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadrow_spec_b_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_B_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_R_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadrow_spec_r_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_R_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gr_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadrow_spec_gr_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gr_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gb_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadrow_spec_gb_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gb_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_B_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadrow_spec_b_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_B_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_R_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadcol_spec_r_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_R_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gr_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadcol_spec_gr_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gr_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gb_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadcol_spec_gb_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gb_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_B_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadcol_spec_b_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_B_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_R_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadcol_spec_r_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_R_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gr_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadcol_spec_gr_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gr_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gb_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadcol_spec_gb_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gb_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_B_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.deadcol_spec_b_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_B_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.spec_row_dol_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.spec_row_dol_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.spec_row_dol_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.spec_row_dol_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.spec_col_dol_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.spec_col_dol_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.spec_col_dol_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.spec_col_dol_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.bin_dead_row_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.grade_dead_row_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.bin_dead_col_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.grade_dead_col_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.breakrow_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.breakrow_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.breakrow_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakrow_spec_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.breakrow_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakrow_spec_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_r]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.breakcol_spec_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_r] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.breakcol_spec_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.breakcol_spec_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[breakcol_spec_b]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.breakcol_spec_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[breakcol_spec_b] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.bin_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.grade_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_line_defect_bright_iic_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Line Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 50)
    {
        msg_.append("Line Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_edge_line_defect_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/EdgeLineDefect.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    edge_line_defect_bright_version = "edge_line_defect_bright_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_R_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadrow_spec_r_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_R_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gr_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadrow_spec_gr_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gr_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gb_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadrow_spec_gb_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gb_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_B_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadrow_spec_b_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_B_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_R_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadrow_spec_r_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_R_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gr_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadrow_spec_gr_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gr_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_Gb_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadrow_spec_gb_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_Gb_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadRow_B_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadrow_spec_b_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadRow_B_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_R_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadcol_spec_r_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_R_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gr_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadcol_spec_gr_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gr_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gb_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadcol_spec_gb_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gb_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_B_TA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadcol_spec_b_TA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_B_TA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_R_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadcol_spec_r_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_R_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gr_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadcol_spec_gr_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gr_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_Gb_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadcol_spec_gb_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_Gb_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Spec_DeadCol_B_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.deadcol_spec_b_MA_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Spec_DeadCol_B_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.spec_row_dol_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.spec_row_dol_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.spec_row_dol_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Row_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.spec_row_dol_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Row_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.spec_col_dol_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.spec_col_dol_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.spec_col_dol_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL_Col_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.spec_col_dol_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL_Col_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.bin_dead_row_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.grade_dead_row_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.bin_dead_col_MA_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.grade_dead_col_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_line_defect_bright_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Edge Line Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 39)
    {
        msg_.append("Edge Line Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Optical_Center_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Opticalcenter.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    optical_center_version = "Opticalcenter_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[x_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_optical_center_.spec_x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[x_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[y_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_optical_center_.spec_y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[y_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_optical_center_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_optical_center_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_optical_center_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_optical_center_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Optical Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 6)
    {
        msg_.append("Optical Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Lag_Grid_ROI_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/lag.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    lag_grid_roi_version = "lag_grid_roi_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Lag_R]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.lag_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Lag_R] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Lag_Gr]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.lag_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Lag_Gr] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Lag_Gb]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.lag_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Lag_Gb] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Lag_B]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.lag_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Lag_B] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_x]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.skip_x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_x] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.skip_y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_y] tag error!!!\n");
            }
        }
        else if(line.startsWith("[X_Grid_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.X_Grid_Cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[X_Grid_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Y_Grid_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Y_Grid_Cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Y_Grid_Cnt] tag error!!!\n");
            }
        }

        else if(line.startsWith("[Row_Up]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.row_up_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Row_Up] tag error!!!\n");
            }
        }

        else if(line.startsWith("[Row_Down]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.row_down_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Row_Down] tag error!!!\n");
            }
        }

        else if(line.startsWith("[Col_Left]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.col_left_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Col_Left] tag error!!!\n");
            }
        }

        else if(line.startsWith("[Col_Right]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.col_right_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Col_Right] tag error!!!\n");
            }
        }

        else if(line.startsWith("[Roi_Lag_R_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Roi_Lag_R_Max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Roi_Lag_R_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Roi_Lag_Gr_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Roi_Lag_Gr_Max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Roi_Lag_Gr_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Roi_Lag_Gb_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Roi_Lag_Gb_Max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Roi_Lag_Gb_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Roi_Lag_B_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Roi_Lag_B_Max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Roi_Lag_B_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Roi_Lag_R_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Roi_Lag_R_Min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Roi_Lag_R_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Roi_Lag_Gr_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Roi_Lag_Gr_Min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Roi_Lag_Gr_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Roi_Lag_Gb_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Roi_Lag_Gb_Min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Roi_Lag_Gb_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Roi_Lag_B_Min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.Roi_Lag_B_Min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Roi_Lag_B_Min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_Lag]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.bin_lag_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_Lag] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_Lag]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.grade_lag_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_Lag] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_Roi_Lag]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.bin_roi_lag_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_Roi_Lag] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_Roi_Lag]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.grade_roi_lag_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_Roi_Lag] tag error!!!\n");
            }
        }
        else if(line.startsWith("[mean_lag_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.mean_lag_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[mean_lag_open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[mean_lag_log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.mean_lag_log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[mean_lag_log_align] tag error!!!\n");
            }
        }
        else if(line.startsWith("[roi_lag_open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.roi_lag_open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[roi_lag_open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[roi_lag_log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_lag_grid_roi_.roi_lag_log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[roi_lag_log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Lag Grid ROI Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 28)
    {
        msg_.append("Lag Grid ROI Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::loadHistogramParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/histogram.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    histogram_version = "histogram_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line  =rd.readLine ();
        if (line.startsWith ("[Dn_stat]="))
        {
            cnt_param += 1;
            bool ok;
            QString sub = line.split("=")[1];
            QStringList list_t = sub.split (",");
            foreach (const QString &str_bin, list_t){
                int bin = str_bin.toInt (&ok);
                if (!ok)
                {
                    msg_.append("[Dn_stat] tag error!!!\n");
                }
                else
                {
                    param_histogram_.Dn_stat.push_back(bin);
                }
            }
        }
        else if (line.startsWith ("[Dn_end]="))
        {
            cnt_param += 1;
            bool ok;
            QString sub = line.split("=")[1];
            QStringList list_t = sub.split (",");
            foreach (const QString &str_bin, list_t){
                int bin = str_bin.toInt (&ok);
                if (!ok)
                {
                    msg_.append("[Dn_end] tag error!!!\n");
                }
                else
                {
                    param_histogram_.Dn_end.push_back(bin);
                }
            }
        }
        else if(line.startsWith("[Cnt_Spec_min]="))
        {
            cnt_param += 1;
            bool ok;
            QString sub = line.split("=")[1];
            QStringList list_t = sub.split (",");
            foreach (const QString &str_bin, list_t){
                int bin = str_bin.toInt (&ok);
                if (!ok)
                {
                    msg_.append("[Cnt_Spec_min] tag error!!!\n");
                }
                else
                {
                    param_histogram_.Cnt_spec_min_.push_back(bin);

                }
            }
        }
        else if(line.startsWith("[Cnt_Spec_max]="))
        {
            cnt_param += 1;
            bool ok;
            QString sub = line.split("=")[1];
            QStringList list_t = sub.split (",");
            foreach (const QString &str_bin, list_t){
                int bin = str_bin.toInt (&ok);
                if (!ok)
                {
                    msg_.append("[Cnt_Spec_max] tag error!!!\n");
                }
                else
                {
                    param_histogram_.Cnt_spec_max_.push_back(bin);

                }
            }
        }
        else if(line.startsWith("[Cnt_Spec_all]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_histogram_.Cnt_spec_all_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Cnt_Spec_all] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_histogram]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_histogram_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_histogram] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_histogram]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_histogram_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_histogram] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_histogram_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Histogram Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 8)
    {
        msg_.append("Histogram Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_ROI_STD_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Roi_std.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    lag_roi_std_version = "lag_roi_std_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[skip_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[col_roi_cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.col_roi_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[col_roi_cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[row_roi_cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.row_roi_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[row_roi_cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r_mean_min_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.r_mean_min_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[r_mean_min_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r_mean_max_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.r_mean_max_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[r_mean_max_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r_std_min_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.r_std_min_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[r_std_min_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r_std_max_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.r_std_max_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[r_std_max_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gr_mean_min_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.gr_mean_min_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gr_mean_min_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gr_mean_max_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.gr_mean_max_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gr_mean_max_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gr_std_min_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.gr_std_min_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gr_std_min_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gr_std_max_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.gr_std_max_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gr_std_max_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gb_mean_min_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.gb_mean_min_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gb_mean_min_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gb_mean_max_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.gb_mean_max_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gb_mean_max_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gb_std_min_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.gb_std_min_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gb_std_min_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gb_std_max_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.gb_std_max_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gb_std_max_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[b_mean_min_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.b_mean_min_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[b_mean_min_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[b_mean_max_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.b_mean_max_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[b_mean_max_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[b_std_min_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.b_std_min_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[b_std_min_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[b_std_max_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.b_std_max_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[b_std_max_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_roi_std_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Range Roi_std Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 24)
    {
        msg_.append("Range Roi_std Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_FPN_DL_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/FPN_DL.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;
    fpn_dl_version = "fpn_dl_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.fpn_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.fpn_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.deadcol_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.deadrow_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.spec_dol_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_row_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.break_row_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_row_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_col_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.break_col_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_col_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Line Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 25)
    {
        msg_.append("Line Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}
bool ParamManage::load_FPN_DL_Param_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/FPN_DL_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;
    fpn_dl_version = "fpn_dl_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.fpn_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.fpn_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadCol_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.deadcol_spec_level1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadCol_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadCol_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.deadcol_spec_level2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadCol_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadRow_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.deadrow_spec_level1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadRow_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadRow_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.deadrow_spec_level2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadRow_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.spec_dol_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_dead_row_level1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_dead_row_level1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_dead_row_level2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_dead_row_level2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_dead_col_level1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_dead_col_level1 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_dead_col_level2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_dead_col_level2 = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_row_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.break_row_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_row_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_col_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.break_col_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_col_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.bin_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.grade_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Line Param V1.1 error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 31)
    {
        msg_.append("Line Param V1.1 not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_FPN_DL_IIC_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/FPN_DL_IIC.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    fpn_dl_iic_version = "fpn_dl_iic_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.fpn_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.fpn_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.deadcol_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.deadrow_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.spec_dol_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.bin_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.grade_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.bin_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.grade_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_row_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.break_row_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_row_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_col_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.break_col_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_col_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.bin_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.grade_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_iic_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Line Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 25)
    {
        msg_.append("Line Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_FPN_DL_bright_dark_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/FPN_DL_Bright_Dark.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    fpn_dl_bright_dark_version = "fpn_dl_bright_dark_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.fpn_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.fpn_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadCol] tag error!!!\n");
            }
        }
        //新增
        else if(line.startsWith("[SpecDeadColBright]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_bright_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColBright] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadColDark]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_dark_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColDark] tag error!!!\n");
            }
        }

        else if(line.startsWith("[SpecDeadRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadrow_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.spec_dol_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        //新增
        else if(line.startsWith("[bin_dead_col_bright]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_bright_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_bright] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_bright]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_bright_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_bright] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_dark]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_dark_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_dark] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_dark]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_dark_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_dark] tag error!!!\n");
            }
        }



        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_row_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.break_row_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_row_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_col_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.break_col_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_col_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Line Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 31)
    {
        msg_.append("Line Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}
bool ParamManage::load_FPN_DL_bright_dark_Param_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/FPN_DL_Bright_Dark_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    fpn_dl_bright_dark_version = "fpn_dl_bright_dark_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.fpn_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.fpn_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadCol] tag error!!!\n");
            }
        }
        //新增
        else if(line.startsWith("[SpecDeadColBright_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColBright_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadColBright_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_bright_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColBright_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadColDark]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_dark_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColDark] tag error!!!\n");
            }
        }

        else if(line.startsWith("[SpecDeadRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadrow_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.spec_dol_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        //新增
        else if(line.startsWith("[bin_dead_col_bright_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_bright_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_bright_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_bright_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_bright_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_bright_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_bright_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_bright_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_bright_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_bright_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_bright_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_bright_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_dark]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_dark_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_dark] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_dark]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_dark_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_dark] tag error!!!\n");
            }
        }



        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_row_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.break_row_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_row_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_col_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.break_col_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_col_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("FPN_DL_Bright_Dark V1.1 Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 34)
    {
        msg_.append("FPN_DL_Bright_Dark V1.1 Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}

bool ParamManage::load_FPN_DL_bright_dark_Param_V1_2(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/FPN_DL_Bright_Dark_V1.2.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    fpn_dl_bright_dark_version = "fpn_dl_bright_dark_V1.2";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[StripRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.strip_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[StripCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.strip_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[StripCol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.fpn_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecFPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.fpn_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecFPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadCol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadCol] tag error!!!\n");
            }
        }
        //新增
        else if(line.startsWith("[SpecDeadColBright_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_bright_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColBright_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadColBright_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_bright_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColBright_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadColDark_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_dark_level_1_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColDark_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDeadColDark_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadcol_dark_level_2_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadColDark_Level_2] tag error!!!\n");
            }
        }

        else if(line.startsWith("[SpecDeadRow]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.deadrow_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDeadRow] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecDOL]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.spec_dol_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecDOL] tag error!!!\n");
            }
        }
        else if(line.startsWith("[radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.r_area_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_row]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dead_row_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_row] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_fpn_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_FPN_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_fpn_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_FPN_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dead_col_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col] tag error!!!\n");
            }
        }
        //新增
        else if(line.startsWith("[bin_dead_col_bright_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_bright_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_bright_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_bright_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_bright_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_bright_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_bright_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_bright_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_bright_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_bright_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_bright_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_bright_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_dark_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_dark_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_dark_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dead_col_dark_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_deadcol_dark_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dead_col_dark_Level_2] tag error!!!\n");
            }
        }

        else if(line.startsWith("[grade_dead_col_dark_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_dark_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_dark_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dead_col_dark_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_deadcol_dark_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dead_col_dark_Level_2] tag error!!!\n");
            }
        }



        else if(line.startsWith("[bin_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dol]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_dol_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dol] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_row_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.break_row_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_row_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[break_col_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.break_col_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[break_col_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.bin_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_break_line]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.grade_break_line_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_break_line] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_print]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.log_print_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_print] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_fpn_dl_bright_dark.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("FPN_DL_Bright_Dark V1.2 Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 37)
    {
        msg_.append("FPN_DL_Bright_Dark V1.2 Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_OB_DarkShading_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/OB_DarkShading.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    ob_darkshading_version = "ob_darkShading_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_region_radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.OB_region_r_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[OB_region_radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Target]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.ref = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Target] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Golden_Global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.Golden_dif = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Golden_Global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_Golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_Golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Min_diff_ref_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_ref_class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_ref_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_ref_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_ref_class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_ref_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Min_diff_ref_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_ref_class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_ref_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_ref_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_ref_class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_ref_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_Max_Min_diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.ob_max_min_diff_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[OB_Max_Min_diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Max_Min_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_ob_max_min_diff_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Max_Min_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Max_Min_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_ob_max_min_diff_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Max_Min_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.x_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.y_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Y_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_B_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_B_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_DarkShading_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_DarkShading_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("OB_DarkShading Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 31)
    {
        msg_.append("OB_DarkShading Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_OB_DarkShading_Param_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/OB_DarkShading_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    ob_darkshading_version = "ob_darkShading_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_region_radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.OB_region_r_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[OB_region_radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Target]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.ref = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Target] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Golden_Global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.Golden_dif = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Golden_Global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_Golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_Golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Min_diff_center_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_center_Class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_center_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_center_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_center_Class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_center_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Min_diff_ref_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_ref_class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_ref_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_ref_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_ref_class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_ref_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Min_diff_ref_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_ref_class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_ref_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_ref_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_ref_class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_ref_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_Max_Min_diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.ob_max_min_diff_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[OB_Max_Min_diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Max_Min_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_ob_max_min_diff_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Max_Min_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Max_Min_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_ob_max_min_diff_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Max_Min_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.x_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.y_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Y_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_B_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_B_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_DarkShading_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_DarkShading_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("OB_DarkShading Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 33)
    {
        msg_.append("OB_DarkShading Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}

bool ParamManage::load_OB_DarkShading_Param_V1_2(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/OB_DarkShading_V1.2.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    ob_darkshading_version = "ob_darkShading_V1.2";


    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_region_radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.OB_region_r_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[OB_region_radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Target]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.ref = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Target] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Golden_Global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.Golden_dif = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Golden_Global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_Golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_Golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Min_diff_ref_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_ref_class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_ref_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_ref_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_ref_class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_ref_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Min_diff_ref_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_ref_class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_ref_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_ref_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_ref_class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_ref_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_Max_Min_diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.ob_max_min_diff_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[OB_Max_Min_diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Max_Min_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_ob_max_min_diff_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Max_Min_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Max_Min_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_ob_max_min_diff_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Max_Min_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.x_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.y_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Y_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MidOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_mid_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MidOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MidOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_mid_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MidOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MidOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_mid_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MidOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_B_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MidOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_mid_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MidOB_B_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_B_DarkShading] tag error!!!\n");
            }
        }

        else if(line.startsWith("[bin_OB_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_DarkShading_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_DarkShading_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_DarkShading_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_DarkShading_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_DarkShading_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_DarkShading_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_DarkShading_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_DarkShading_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("OB_DarkShading V1.2 Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 37)
    {
        msg_.append("OB_DarkShading V1.2 Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_OB_DarkShading_Param_V1_3(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/OB_DarkShading_V1.3.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    ob_darkshading_version = "ob_darkShading_V1.3";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_region_radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.OB_region_r_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[OB_region_radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Target]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.ref = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Target] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Golden_Global]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.Golden_dif = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Golden_Global] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_Golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Golden] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Golden]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_Golden_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Golden] tag error!!!\n");
            }
        }
        //----
        else if(line.startsWith("[ROI_X]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.roi_x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.roi_y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_W]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.roi_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_W] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_H]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.roi_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_H] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinMean]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_Mean_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[MinMean] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxMean]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_Mean_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[MaxMean] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Mean]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_ob_mean = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Mean] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Mean]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_ob_mean = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Mean] tag error!!!\n");
            }
        }
        //----
        else if(line.startsWith("[Min_diff_ref_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_ref_class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_ref_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_ref_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_ref_class1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_ref_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Min_diff_ref_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.min_diff_ref_class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Min_diff_ref_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_diff_ref_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.max_diff_ref_class2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_diff_ref_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_Max_Min_diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.ob_max_min_diff_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[OB_Max_Min_diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_OB_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_OB_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Max_Min_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_ob_max_min_diff_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Max_Min_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Max_Min_Diff]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_ob_max_min_diff_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Max_Min_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.x_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.y_cnt_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Y_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_R_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.r_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_R_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_Gr_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gr_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gr_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_Gb_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.gb_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gb_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MinOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_min_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MinOB_B_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[MaxOB_B_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.b_DarkShading_max_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_B_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.bin_DarkShading_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_DarkShading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.grade_DarkShading_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_DarkShading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_darkshading_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("OB_DarkShading Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 39)
    {
        msg_.append("OB_DarkShading Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}





bool ParamManage::load_OB_GrGb_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/OB_GrGb.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    ob_grgb_version = "ob_grgb_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[OB_region_radius]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.OB_region_r_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[OB_region_radius] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_GRGB_OB_DIF_MAX_Level1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.min_ob_Gr_Gb_diff_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[ROI_GRGB_OB_DIF_MAX_Level1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_GRGB_OB_DIF_MAX_Level2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.max_ob_Gr_Gb_diff_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[MaxOB_Gr_Gb_Diff] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Gr_Gb_Diff_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.bin_OB_Gr_Gb_diff_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Gr_Gb_Diff_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Gr_Gb_Diff_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.grade_OB_Gr_Gb_diff_class1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Gr_Gb_Diff_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_OB_Gr_Gb_Diff_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.bin_OB_Gr_Gb_diff_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_OB_Gr_Gb_Diff_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_OB_Gr_Gb_Diff_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.grade_OB_Gr_Gb_diff_class2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_OB_Gr_Gb_Diff_Class2] tag error!!!\n");
            }
        }//
        else if(line.startsWith("[Target]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.ref = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Target] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ob_grgb_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("OB_GrGb Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 11)
    {
        msg_.append("OB_GrGb Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::loadBasicParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Basic.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[bayer]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            QString sub = line_t[1];
            if (sub == "RG")
                bayer_ = RG;
            else if (sub == "BG")
                bayer_ = BG;
            else if (sub == "GR")
                bayer_ = GR;
            else if (sub == "GB")
                bayer_ = GB;
            else
            {
                ok = false;
                msg_.append("unkown bayer ...\n");
            }
        }
        else if(line.startsWith("[width]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            width_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[width] tag error!!!\n");
            }
        }
        else if(line.startsWith("[height]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            height_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[height] tag error!!!\n");
            }
        }
        else if(line.startsWith("[flag_raw8_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            flag_raw8_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[flag_raw8_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[flag_split_channel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            flag_split_channel_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[flag_split_channel] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Basic Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 5)
    {
        msg_.append("Basic Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::loadRowNoiseParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Row_Noise.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;
    row_Noise_version = "row_Noise_version_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Row_Noise_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_row_noise_.row_noise_spec = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Row_Noise_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_row_noise_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_row_noise_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_row_noise_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Row Noise Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 4)
    {
        msg_.append("Row Noise Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::loadChannelNoiseParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Channel_Noise.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;
    col_channel_noise_version = "col_channel_noise_version_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Column_Noise_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.column_noise_spec = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Column_Noise_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Column Noise Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 4)
    {
        msg_.append("Column Noise Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}

bool ParamManage::loadChannelNoiseParam_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Channel_Noise_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;
    col_channel_noise_version = "col_channel_noise_version_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Column_Noise_Max_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.column_noise_spec_level_1_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Column_Noise_Max_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Column_Noise_Max_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.column_noise_spec_level_2_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Column_Noise_Max_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.bin_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.bin_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_Level_2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_Level_1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.grade_level_1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_Level_1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_Level_2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_channel_noise_.grade_level_2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_Level_2] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Column Noise Param V1.1 error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 6)
    {
        msg_.append("Column Noise Param V1.1 not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}




bool ParamManage::loadColumnNoiseParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Column_Noise.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    column_Noise_version = "column_Noise_version_V1.0";


    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Column_Noise_Max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_column_noise_.column_noise_spec = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Column_Noise_Max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_column_noise_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_column_noise_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_column_noise_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Column Noise Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 4)
    {
        msg_.append("Column Noise Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::load_Read_Noise_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Read_Noise.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;
    read_Noise_version = "read_Noise_version_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[X_ROI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_read_noise_.roi_x_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[X_ROI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Y_ROI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_read_noise_.roi_y_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Y_ROI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[W_ROI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_read_noise_.roi_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[W_ROI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[H_ROI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_read_noise_.roi_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[H_ROI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_read_noise_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Read Noise Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 5)
    {
        msg_.append("Read Noise Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Real_ROI_PRNU_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/ROI_PRNU.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    roi_prnu_version = "ROI_PRNU_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[X_ROI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.X_ROI = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[X_ROI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Y_ROI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.Y_ROI = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Y_ROI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[W_ROI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.W_ROI = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[W_ROI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[H_ROI]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.H_ROI = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[H_ROI] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecR]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.spec_prnu_r_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecR] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecG1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.spec_prnu_gr_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecG1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecG2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.spec_prnu_gb_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecG2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[SpecB]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.spec_prnu_b_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[SpecB] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_real_roi_prnu_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Real_ROI_PRNU Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 12)
    {
        msg_.append("Real_ROI_PRNU Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_RI_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/RelativeIllumination.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    relative_illu_version = "relativeIllumination_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[w_win]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_relative_illu_.w_win_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[w_win] tag error!!!\n");
            }
        }
        else if(line.startsWith("[h_win]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_relative_illu_.h_win_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[h_win] tag error!!!\n");
            }
        }
        else if(line.startsWith("[min_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_relative_illu_.limit_min_ratio_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[min_ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[max_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_relative_illu_.limit_max_ratio_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[max_ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_relative_illu_.bin_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_relative_illu_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_relative_illu_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_relative_illu_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("RelativeIlluminaction Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 8)
    {
        msg_.append("RelativeIlluminaction Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Color_Shading_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/ColorShading.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    color_shading_version = "color_shading_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }

        else if(line.startsWith("[Factor_Center_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.factor_center_class1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Center_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.factor_center_class2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_Class2] tag error!!!\n");
            }
        }

        else if(line.startsWith("[bin_shading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.bin_shanding = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_shading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_shading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.grade_shanding = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_shading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_shading_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.bin_shanding_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_shading_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_shading_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.grade_shanding_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_shading_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("ColorShading V1.0 Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 11)
    {
        msg_.append("ColorShading V1.0 Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Color_Shading_Param_V1_1(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/ColorShading_V1.1.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    color_shading_version = "color_shading_V1.1";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }

        else if(line.startsWith("[Factor_Center_RG_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.factor_center_rg_class1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_RG_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Center_RG_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.factor_center_rg_class2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_RG_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Center_BG_Class1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.factor_center_bg_class1 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_BG_Class1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Factor_Center_BG_Class2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.factor_center_bg_class2 = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Factor_Center_BG_Class2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_shading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.bin_shanding = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_shading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_shading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.grade_shanding = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_shading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_shading_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.bin_shanding_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_shading_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_shading_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.grade_shanding_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_shading_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("ColorShading Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 13)
    {
        msg_.append("ColorShading Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}





bool ParamManage::load_Color_Shading_5x5_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/ColorShading_5x5.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    color_shading_5x5_version = "color_shading_5_5_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }

        else if(line.startsWith("[RG1_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.rg1_min = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG1_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG2_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.rg2_min = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG2_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG3_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.rg3_min = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG3_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG1_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.rg1_max = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG1_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG2_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.rg2_max = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG2_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[RG3_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.rg3_max = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[RG3_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG1_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.bg1_min = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG1_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG2_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.bg2_min = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG2_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG3_min]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.bg3_min = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG3_min] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG1_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.bg1_max = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG1_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG2_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.bg2_max = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG2_max] tag error!!!\n");
            }
        }
        else if(line.startsWith("[BG3_max]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.bg3_max = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[BG3_max] tag error!!!\n");
            }
        }

        else if(line.startsWith("[bin_shading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.bin_shanding = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_shading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_shading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.grade_shanding = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_shading] tag error!!!\n");
            }
        }

        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_5x5.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("ColorShading_5x5 Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 19)
    {
        msg_.append("ColorShading_5x5 Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_Color_Shading_15_15_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/ColorShading_15x15.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    color_shading_15x15_version = "color_shading_15x15_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.skip = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[Skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_X_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.ROI_X_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_X_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ROI_Y_Cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.ROI_Y_Cnt = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ROI_Y_Cnt] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Diff_Corner]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.max_diff_corner = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Diff_Corner] tag error!!!\n");
            }
        }
        else if(line.startsWith("[Max_Diff_Edge]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.max_diff_edge = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Max_Diff_Edge] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_shading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.bin_shanding = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_shading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_shading]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.grade_shanding = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_shading] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_shading_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.bin_shanding_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_shading_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_shading_MA]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.grade_shanding_MA = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_shading_MA] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_color_shading_15x15.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("ColorShading_15x15 Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 11)
    {
        msg_.append("ColorShading_15x15 Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}





bool ParamManage::load_ImageDirttDetect_Bright_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/ImageDirt.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    imageDirt_normal_bright_version = "ImageDirt_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[pre_blur_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.pre_blur_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[pre_blur_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[img_resize]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.img_resize_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[img_resize] tag error!!!\n");
            }
        }
        else if(line.startsWith("[lsc_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.lsc_ratio_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[lsc_ratio] tag error!!!\n");
            }
        }

        else if(line.startsWith("[gfda_blur_r1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.gfda_blur_r1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[gfda_blur_r1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gfda_blur_r2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.gfda_blur_r2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[gfda_blur_r2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gfda_pog_spec_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.gfda_pog_spec_p_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[gfda_pog_spec_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gfda_pog_spec_n]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.gfda_pog_spec_n_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[gfda_pog_spec_n] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gfda_scratch_spec_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.gfda_scratch_spec_p_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[gfda_scratch_spec_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gfda_spot_spec_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.gfda_spot_spec_p_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[gfda_spot_spec_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gfda_spot_spec_n]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.gfda_spot_spec_n_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[gfda_spot_spec_n] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gfda_spot_limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.gfda_spot_limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[gfda_spot_limit_size] tag error!!!\n");
            }
        }

        else if(line.startsWith("[leda_ratio_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.leda_ratio_p_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[leda_ratio_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[leda_ratio_n]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.leda_ratio_n_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[leda_ratio_n] tag error!!!\n");
            }
        }
        else if(line.startsWith("[leda_spot_spec_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.leda_spot_spec_p_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[leda_spot_spec_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[leda_spot_limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.leda_spot_limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[leda_spot_limit_size] tag error!!!\n");
            }
        }

        else if(line.startsWith("[ldma_stain_skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.ldma_stain_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ldma_stain_skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ldma_stain_r_kernel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.ldma_stain_r_kernel_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ldma_stain_r_kernel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ldma_stain_r_outer]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.ldma_stain_r_outer_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[ldma_stain_r_outer] tag error!!!\n");
            }
        }
        else if(line.startsWith("[ldma_stain_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.ldma_stain_spec_ = line_t[1].toFloat(&ok);
            if(!ok)
            {
                msg_.append("[ldma_stain_spec] tag error!!!\n");
            }
        }

        else if(line.startsWith("[pog_miscount]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.pog_miscount_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[pog_miscount] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_pog]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.bin_pog_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_pog] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_pog]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.grade_pog_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_pog] tag error!!!\n");
            }
        }

        else if(line.startsWith("[scratch_miscount]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.scratch_miscount_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[scratch_miscount] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_scratch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.bin_scratch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_scratch] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_scratch]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.grade_scratch_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_scratch] tag error!!!\n");
            }
        }

        else if(line.startsWith("[spot_miscount]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.spot_miscount_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[spot_miscount] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_spot]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.bin_spot_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_spot] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_spot]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.grade_spot_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_spot] tag error!!!\n");
            }
        }

        else if(line.startsWith("[stain_miscount]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.stain_miscount_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[stain_miscount] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_stain]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.bin_stain_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_stain] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_stain]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.grade_stain_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_stain] tag error!!!\n");
            }
        }
        else if(line.startsWith("[darkbar_miscount]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.darkbar_miscount_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[darkbar_miscount] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_darkbar]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.bin_darkbar_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_darkbar] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_darkbar]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.grade_darkbar_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_darkbar] tag error!!!\n");
            }
        }


        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_ImageDirt_normal_bright_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("POG Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 37)
    {
        msg_.append("POG Param not enough, loaded "+QString::number(cnt_param)+" params\n");

        return false;
    }
    else
        return true;
}



bool ParamManage::load_POG_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/POG.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[spec_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.spec_p_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[spec_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[spec_n]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.spec_n_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[spec_n] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.r1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[r1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.r2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[r2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[miscount]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.miscount_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[miscount] tag error!!!\n");
            }
        }
        else if(line.startsWith("[resize_w]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.resize_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[resize_w] tag error!!!\n");
            }
        }
        else if(line.startsWith("[resize_h]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.resize_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[resize_h] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_pog]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.bin_pog_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_pog] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_pog]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.grade_pog_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_pog] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("POG Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 12)
    {
        msg_.append("POG Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



bool ParamManage::load_POG_Normal_Bright_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/POG.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    pog_version = "pog_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[spec_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.spec_p_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[spec_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[spec_n]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.spec_n_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[spec_n] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r1]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.r1_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[r1] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r2]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.r2_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[r2] tag error!!!\n");
            }
        }
        else if(line.startsWith("[crop]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.crop_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[crop] tag error!!!\n");
            }
        }
        else if(line.startsWith("[scratch_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.scratch_p_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[scratch_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[scratch_n]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.scratch_n_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[scratch_n] tag error!!!\n");
            }
        }
        else if(line.startsWith("[miscount]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.miscount_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[miscount] tag error!!!\n");
            }
        }
        else if(line.startsWith("[resize_w]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.resize_w_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[resize_w] tag error!!!\n");
            }
        }
        else if(line.startsWith("[resize_h]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.resize_h_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[resize_h] tag error!!!\n");
            }
        }
        else if(line.startsWith("[limit_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.limit_size_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[limit_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_pog]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.bin_pog_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_pog] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_pog]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.grade_pog_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_pog] tag error!!!\n");
            }
        }
        else if(line.startsWith("[lsc_ratio]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.lsc_ratio_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[lsc_ratio] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.blemish_skip_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[blemish_skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r_kernel]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.r_kernel_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[r_kernel] tag error!!!\n");
            }
        }
        else if(line.startsWith("[r_outer]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.r_outer_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[r_outer] tag error!!!\n");
            }
        }
        else if(line.startsWith("[blemish_spec]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.blemish_spec_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[blemish_spec] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_pog_normal_bright_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("POG Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 21)
    {
        msg_.append("POG Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}




bool ParamManage::load_Edge_ROI_DeadLine_Dark_Param(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Edge_ROI_DL.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    edge_roi_deadline_dark_version = "edge_roi_deadline_dark_V1.0";

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();

        if(line.startsWith("[skip]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.skip_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[skip] tag error!!!\n");
            }
        }
        else if(line.startsWith("[center_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.center_size_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[center_size_] tag error!!!\n");
            }
        }
        else if(line.startsWith("[edge_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.edge_size_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[edge_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[gride_size]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.gride_size_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[gride_size] tag error!!!\n");
            }
        }
        else if(line.startsWith("[spec_p]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.spec_p_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[spec_p] tag error!!!\n");
            }
        }
        else if(line.startsWith("[spec_n]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.spec_n_ = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[spec_n] tag error!!!\n");
            }
        }
        else if(line.startsWith("[bin_dl]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.bin_dl_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin_dl] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade_dl]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.grade_dl_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade_dl] tag error!!!\n");
            }
        }
        else if(line.startsWith("[save_image]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.save_image_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[save_image] tag error!!!\n");
            }
        }
        else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }
        else if(line.startsWith("[log_align]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_edge_ROI_DL_.log_align_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[log_align] tag error!!!\n");
            }
        }
    }
    file.close();

    if(!ok)
    {
        msg_.append("Edge_ROI_DL Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 11)
    {
        msg_.append("Edge_ROI_DL Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}


bool ParamManage::loadMosaicParam(const std::string &dir)
{
    std::string path_file = dir;
    path_file.append("/Mosaic.data");
    QFile file(QString::fromStdString(path_file));
    if(!file.open(QFile::Text | QFile::ReadOnly))
        return false;

    QTextStream rd(&file);
    bool ok = true;
    int cnt_param = 0;

    while(ok && !rd.atEnd())
    {
        QString line = rd.readLine();
        if(line.startsWith("[Mosaic_lines_cnt]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_Mosaic_.Mosaic_lines_cnt = line_t[1].toDouble(&ok);
            if(!ok)
            {
                msg_.append("[Mosaic_lines_cnt] tag error!!!\n");
            }
        }
       else if(line.startsWith("[bin]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            this->param_Mosaic_.bin_= line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[bin] tag error!!!\n");
            }
        }
        else if(line.startsWith("[grade]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_Mosaic_.grade_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[grade] tag error!!!\n");
            }
        }
      else if(line.startsWith("[open_mode]="))
        {
            cnt_param += 1;
            QStringList line_t = line.split("=");
            param_Mosaic_.open_mode_ = line_t[1].toInt(&ok);
            if(!ok)
            {
                msg_.append("[open_mode] tag error!!!\n");
            }
        }

    }
    file.close();

    if(!ok)
    {
        msg_.append("Mosaic Param error, please check!!!\n");
        return false;
    }
    else if(cnt_param != 4)
    {
        msg_.append("Mosaic Param not enough, loaded "+QString::number(cnt_param)+" params\n");
        return false;
    }
    else
        return true;
}



