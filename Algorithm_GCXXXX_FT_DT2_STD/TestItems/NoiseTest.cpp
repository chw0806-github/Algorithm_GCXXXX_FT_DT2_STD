#include "NoiseTest.h"

//ALL Noise
int  NoiseTest::TestFunc_ALL_Noise_Dark(ParamManage& manage,uchar *pData,uchar *pImage_save)
{
    if (test_frameCnt<=1)
    {
        g_str_msg_all[site_].append ("frameCnt<=1!!!\n");
        return -1;
    }
    logMsg (std::string("into row noise_pixel test, check params first!"));
    if (manage.width_<=0 ||manage.height_<=0 ||
        (test_datatype !=8 && test_datatype !=10 && test_datatype !=12 && test_datatype !=14 && test_datatype !=16))
    {
        g_str_msg_all[site_].append ("image size or cell size of some algorithm is wrong!\n");
        logMsg ("image size or cell size of some algorithm is wrong!",0);
        return -2;
    }

    if(!Algorithm_all[site_]. Convert_Raw16_to_Ushort(manage,pData,(unsigned short *)pData,
                                                     test_frameCnt,test_datatype))
        return -2;

    logMsg ("Start Get_STD_Map_by_4channel!");
    if(!Get_STD_Map_by_4channel(manage,pData,test_frameCnt))
        return -2;

    std::vector<Subitem_Result> items_results;   //各测试子项 测试结果集合
    items_results.push_back(Noise_Dark_by_pixel_chao(manage,Noise_Dark_Pixel_read));
    items_results.push_back(Noise_Dark_by_pixel_chao(manage,Noise_Dark_Pixel_row));
    items_results.push_back(Noise_Dark_by_pixel_chao(manage,Noise_Dark_Pixel_ColRTS));
    items_results.push_back(Noise_Dark_by_pixel_chao(manage,Noise_Dark_Pixel_Col_chanel));
    Subitem_Result noise_result=Algorithm_all[site_].Calculate_The_Final_Bin(items_results);

    return noise_result.bin;

}

bool NoiseTest::Get_STD_Map_by_4channel(const ParamManage& manage,uchar *pData,int frame_cnt)
{
    int w = manage.width_;
    int h = manage.height_;

    bool flag = Algorithm_all[site_].New_Pointer_Buf( (uchar**) &g_buf_delta_raw_,
                                            Result_area,Buf_double,w,h,1);
    if(!flag)
        return false;

    MultiThread::get_bychannel_std_map_raw16_chao *p_delta[Channel_Num];
    for (int i=0; i < Channel_Num; i++)
        p_delta[i] = new MultiThread::get_bychannel_std_map_raw16_chao();

    p_delta[0]->setparam(pData, g_buf_delta_raw_, w,h,0, frame_cnt);
    p_delta[1]->setparam(pData, g_buf_delta_raw_, w,h,h/4, frame_cnt);
    p_delta[2]->setparam(pData, g_buf_delta_raw_, w,h,h*2/4, frame_cnt);
    p_delta[3]->setparam(pData, g_buf_delta_raw_, w,h,h*3/4, frame_cnt);

    for (int i=0; i < Channel_Num; i++)
        p_delta[i]->start();
    for (int i=0; i < Channel_Num; i++)
        p_delta[i]->wait();

    for (int i=0; i < Channel_Num; i++)
        delete p_delta[i];

    return true;
}



Subitem_Result NoiseTest::Noise_Dark_Row(const ParamManage& manage)
{
    int w = manage.width_;
    int h = manage.height_;
    Subitem_Result res("Noise_Row",true);


    if(manage.param_row_noise_.open_mode_ ==1)
    {

//        res.name="Noise_Row";
        QVector <double>  mean_row_std;
        double sum=0;
        for(int i=0; i<h; ++i)
        {
            sum=0;
            for (int j=0; j<w; j++)
            {
                sum += g_buf_delta_raw_[i*w+j];
            }
            sum = sum/w;
            mean_row_std.append(sum);
        }

        QVector <double>::const_iterator max_std = std::max_element(mean_row_std.begin(),mean_row_std.end());
        int max_pos = mean_row_std.indexOf(*max_std);

        float row_noise_max =1;
        float row_noise_max0 = 0;
        if(*max_std >0 )
            row_noise_max0 = std::log10((*max_std)/1023);
        else
            row_noise_max0 = 0;
        row_noise_max = 20* row_noise_max0;
        g_str_msg_all[site_]+=boost::str(boost::format("row_mean_std_Max:%.4f, row_noise_max:%.4f, row_noise_max pos:%d\n")
                                         % (*max_std) % row_noise_max  % max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%d|")
                                         % (*max_std) % row_noise_max  % max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        if(row_noise_max >  manage.param_row_noise_.row_noise_spec)
        {
            res.is_passed=false;
            res.bin=manage.param_row_noise_.bin_;
            res.grade=manage.param_row_noise_.grade_;
        }

    }
    else
    {
        res.g_msg=false;
    }

    return res;
}

Subitem_Result NoiseTest::Noise_Dark_ColRTS(const ParamManage& manage)
{
    int w = manage.width_;
    int h = manage.height_;
    Subitem_Result res("Noise_Col",true);

    if(manage.param_column_noise_.open_mode_ ==1)
    {
//        res.name="Noise_Col";
        QVector <double>  mean_column_std;
        double sum=0;
        double std_all_pixel=0;
        for(int i = 0; i < w; ++i)
        {
            sum=0;
            for (int j=0; j<h; j++)
                sum += g_buf_delta_raw_[j*w+i];
            std_all_pixel+=sum;
            sum = sum/h;
            mean_column_std.append(sum);
        }

        QVector <double>::const_iterator max_std = std::max_element(mean_column_std.begin(),mean_column_std.end());
        int max_pos = mean_column_std.indexOf(*max_std);

        float column_noise_max =1;
        float column_noise_max0 = 0;
        if(*max_std >0 )
            column_noise_max0 = std::log10((*max_std)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
        g_str_msg_all[site_]+=boost::str(boost::format("column_mean_std_Max:%.4f, column_noise_max:%.4f, column_noise_max pos:%d\n")
                                         % (*max_std) % column_noise_max % max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%d|")
                                         % (*max_std) % column_noise_max % max_pos);

        if(column_noise_max> manage.param_column_noise_.column_noise_spec)
        {
            res.is_passed=false;
            res.bin=manage.param_column_noise_.bin_;
            res.grade=manage.param_column_noise_.grade_;
        }


        //RTS
        std_all_pixel = std_all_pixel/(h*w);
        double mean_std_sigma = 0.0;
        for(int i = 0; i < h; ++i)
        {
            for (int j=0; j<w; j++)
            {
                double va_de=g_buf_delta_raw_[i*w+j];
                double val=std_all_pixel- va_de;
                mean_std_sigma+= val*val ;
            }
        }
        double doul1=mean_std_sigma;
        double doul2=doul1/(h*w-1);
        mean_std_sigma =sqrt ( doul2);

        double mean_std_spec = std_all_pixel+6*mean_std_sigma;
        int cnt_std =0;
        for(int i = 0; i < h; ++i)
        {
            for (int j=0; j<w; j++)
            {
                if(g_buf_delta_raw_[i*w+j]>mean_std_spec)
                    cnt_std++;
            }
        }

        g_str_msg_all[site_]+=boost::str(boost::format("RTS nosie mean_std:% .4f,RTS nosie mean_std_sigma:% .4f,"
                                             "RTS noise count greater than mean_std+6 sigma :%d\n")
                               % std_all_pixel % mean_std_sigma % cnt_std);

        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%d|")
                               % std_all_pixel % mean_std_sigma % cnt_std);
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

    }
    else
    {
        res.g_msg=false;
    }

    return res;
}


Subitem_Result NoiseTest::Noise_Dark_Col_Channel(const ParamManage& manage)
{
    int w = manage.width_;
    int h = manage.height_;
    Subitem_Result res("Noise_Col_Channel",true);

    if(manage.param_channel_noise_.open_mode_ == 1)
    {

//        res.name="Noise_Col_Channel";
        QVector <double>  mean_column_std;
        QVector <double>  mean_column_std1;
        QVector <double>  mean_column_std2;
        QVector <double>  mean_column_std3;
        QVector <double>  column_noise_channel;
        QVector <double>  column_noise_channel_dif;
        QVector <double>  column_std_channel_dif;
        double sum=0;
        double sum1=0;
        double sum2=0;
        double sum3=0;
        double std_all_pixel=0;
        double std_all_pixel1=0;
        double std_all_pixel2=0;
        double std_all_pixel3=0;
        for(int i = 0; i < w; i+=2)
        {
            sum=0;
            sum1=0;
            sum2=0;
            sum3=0;
            for (int j=0; j<h; j+=2)
            {
                sum += g_buf_delta_raw_[j*w+i];
                sum1 += g_buf_delta_raw_[j*w+i+1];
                sum2 += g_buf_delta_raw_[(j+1)*w+i];
                sum3 += g_buf_delta_raw_[(j+1)*w+i+1];
            }
            std_all_pixel+=sum;
            std_all_pixel1+=sum1;
            std_all_pixel2+=sum2;
            std_all_pixel3+=sum3;
            sum = 2*sum/h;
            sum1 = 2*sum1/h;
            sum2 = 2*sum2/h;
            sum3 = 2*sum3/h;
            mean_column_std.append(sum);
            mean_column_std1.append(sum1);
            mean_column_std2.append(sum2);
            mean_column_std3.append(sum3);
        }
        float column_noise_max =1;
        float column_noise_max0 = 0;
        int max_pos = 0;

        QVector <double>::const_iterator max_std = std::max_element(mean_column_std.begin(),mean_column_std.end());
        max_pos = (mean_column_std.indexOf(*max_std)*2);


        //均值
        double sumavg = std::accumulate(mean_column_std.begin(),mean_column_std.end(),0.0);
        double avg_std = sumavg/mean_column_std.size();
        float column_noise_avg =1;
        float column_noise_avg0 = 0;
        if(avg_std >0 )
            column_noise_avg0 = std::log10((avg_std)/1023);
        else
            column_noise_avg0 = 0;
        column_noise_avg = 20*column_noise_avg0;
        //均值

        column_noise_max =1;
        column_noise_max0 = 0;
        if(*max_std >0 )
            column_noise_max0 = std::log10((*max_std)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
        column_noise_channel.append(column_noise_max);
        double dif_max_avg = (column_noise_max-column_noise_avg);
        column_noise_channel_dif.append(dif_max_avg);
        double dif_max_avg_std = (*max_std-avg_std);
        column_std_channel_dif.append(dif_max_avg_std);
        g_str_msg_all[site_]+=boost::str(boost::format("R Channel column_mean_std_Max:%.4f, std_avg:%.4f, noise_max:%.4f, noise_avg:%.4f,dif_std:%.4f,dif_noise:%.4f, noise_max pos:%d\n")
                                         % (*max_std)  % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|")
                                         % (*max_std)  % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);

        QVector <double>::const_iterator max_std1 = std::max_element(mean_column_std1.begin(),mean_column_std1.end());
        max_pos = (mean_column_std1.indexOf(*max_std1)*2);

        //均值
        sumavg = std::accumulate(mean_column_std1.begin(),mean_column_std1.end(),0.0);
        avg_std = sumavg/mean_column_std1.size();
        column_noise_avg =1;
        column_noise_avg0 = 0;
        if(avg_std >0 )
            column_noise_avg0 = std::log10((avg_std)/1023);
        else
            column_noise_avg0 = 0;
        column_noise_avg = 20*column_noise_avg0;
        //均值

        column_noise_max =1;
        column_noise_max0 = 0;
        if(*max_std1 >0 )
            column_noise_max0 = std::log10((*max_std1)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
         column_noise_channel.append(column_noise_max);
        dif_max_avg = (column_noise_max-column_noise_avg);
        column_noise_channel_dif.append(dif_max_avg);
        dif_max_avg_std = (*max_std1-avg_std);
        column_std_channel_dif.append(dif_max_avg_std);
        g_str_msg_all[site_]+=boost::str(boost::format("Gr Channel column_mean_std_Max:%.4f,mean_std_avg:%.4f, noise_max:%.4f,noise_avg:%.4f, dif_std:%.4f,dif_noise:%.4f,noise_max pos:%d\n")
                                         % (*max_std1) % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg %  max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|")
                                         % (*max_std1) % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);

        QVector <double>::const_iterator max_std2 = std::max_element(mean_column_std2.begin(),mean_column_std2.end());
        max_pos = (mean_column_std2.indexOf(*max_std2)*2);

        //均值
        sumavg = std::accumulate(mean_column_std2.begin(),mean_column_std2.end(),0.0);
        avg_std = sumavg/mean_column_std2.size();
        column_noise_avg =1;
        column_noise_avg0 = 0;
        if(avg_std >0 )
            column_noise_avg0 = std::log10((avg_std)/1023);
        else
            column_noise_avg0 = 0;
        column_noise_avg = 20*column_noise_avg0;
        //均值

        column_noise_max =1;
        column_noise_max0 = 0;
        if(*max_std2 >0 )
            column_noise_max0 = std::log10((*max_std2)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
        column_noise_channel.append(column_noise_max);
        dif_max_avg = (column_noise_max-column_noise_avg);
        column_noise_channel_dif.append(dif_max_avg);
        dif_max_avg_std = (*max_std2-avg_std);
        column_std_channel_dif.append(dif_max_avg_std);
        g_str_msg_all[site_]+=boost::str(boost::format("Gb Channel column_mean_std_Max:%.4f, mean_std_avg:%.4f,noise_max:%.4f,noise_avg:%.4f,dif_std:%.4f,dif_noise:%.4f,noise_max pos:%d\n")
                                         % (*max_std2) % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg %  max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|")
                                         % (*max_std2) % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);

        QVector <double>::const_iterator max_std3 = std::max_element(mean_column_std3.begin(),mean_column_std3.end());
        max_pos = (mean_column_std3.indexOf(*max_std3)*2);

        //均值
        sumavg = std::accumulate(mean_column_std3.begin(),mean_column_std3.end(),0.0);
        avg_std = sumavg/mean_column_std3.size();
        column_noise_avg =1;
        column_noise_avg0 = 0;
        if(avg_std >0 )
            column_noise_avg0 = std::log10((avg_std)/1023);
        else
            column_noise_avg0 = 0;
        column_noise_avg = 20*column_noise_avg0;
        //均值

        column_noise_max =1;
        column_noise_max0 = 0;
        if(*max_std3 >0 )
            column_noise_max0 = std::log10((*max_std3)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
        column_noise_channel.append(column_noise_max);
        dif_max_avg = (column_noise_max-column_noise_avg);
        column_noise_channel_dif.append(dif_max_avg);
        dif_max_avg_std = (*max_std3-avg_std);
        column_std_channel_dif.append(dif_max_avg_std);
        g_str_msg_all[site_]+=boost::str(boost::format("B Channel column_mean_std_Max:%.4f, mean_std_avg:%.4f,noise_max:%.4f,noise_avg:%.4f,dif_std:%.4f,dif_noise:%.4f,noise_max pos:%d\n")
                                         % (*max_std3) % avg_std % column_noise_max %column_noise_avg % dif_max_avg_std % dif_max_avg %  max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|")
                                         % (*max_std) %  avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);
        double max_noise_channel = *std::max_element(column_noise_channel.begin(),column_noise_channel.end());
        double min_noise_channel = *std::min_element(column_noise_channel.begin(),column_noise_channel.end());
        double max_noise_channel_dif = *std::max_element(column_noise_channel_dif.begin(),column_noise_channel_dif.end());
        double max_std_channel_dif = *std::max_element(column_std_channel_dif.begin(),column_std_channel_dif.end());



        double noise_channel_dif = max_noise_channel-min_noise_channel;
        g_str_msg_all[site_]+=boost::str(boost::format("Channel Max:%.4f, Channel Min:%.4f, Channel dif:%.4f ,Channel std max-avg dif max:%.4f,Channel noise max-avg dif max:%.4f\n")
                                         % (max_noise_channel) % min_noise_channel % noise_channel_dif % max_std_channel_dif % max_noise_channel_dif);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|")
                                        % (max_noise_channel) % (noise_channel_dif) % (max_std_channel_dif) % (max_noise_channel_dif));
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());


        if(max_std_channel_dif > manage.param_channel_noise_.column_noise_spec && max_std_channel_dif > 0)
        {
            res.is_passed=false;
            res.bin=manage.param_channel_noise_.bin_;
            res.grade=manage.param_channel_noise_.grade_;
        }


    }
    else
    {
        res.g_msg=false;
    }

    return res;
}



Subitem_Result NoiseTest::Noise_Dark_Col_Channel_V1_1(const ParamManage& manage)
{
    int w = manage.width_;
    int h = manage.height_;
    Subitem_Result res("Noise_Col_Channel_V1.1",true);

    if(manage.param_channel_noise_.open_mode_ == 1)
    {

//        res.name="Noise_Col_Channel_V1.1";
        QVector <double>  mean_column_std;
        QVector <double>  mean_column_std1;
        QVector <double>  mean_column_std2;
        QVector <double>  mean_column_std3;
        QVector <double>  column_noise_channel;
        QVector <double>  column_noise_channel_dif;
        QVector <double>  column_std_channel_dif;
        double sum=0;
        double sum1=0;
        double sum2=0;
        double sum3=0;
        double std_all_pixel=0;
        double std_all_pixel1=0;
        double std_all_pixel2=0;
        double std_all_pixel3=0;
        for(int i = 0; i < w; i+=2)
        {
            sum=0;
            sum1=0;
            sum2=0;
            sum3=0;
            for (int j=0; j<h; j+=2)
            {
                sum += g_buf_delta_raw_[j*w+i];
                sum1 += g_buf_delta_raw_[j*w+i+1];
                sum2 += g_buf_delta_raw_[(j+1)*w+i];
                sum3 += g_buf_delta_raw_[(j+1)*w+i+1];
            }
            std_all_pixel+=sum;
            std_all_pixel1+=sum1;
            std_all_pixel2+=sum2;
            std_all_pixel3+=sum3;
            sum = 2*sum/h;
            sum1 = 2*sum1/h;
            sum2 = 2*sum2/h;
            sum3 = 2*sum3/h;
            mean_column_std.append(sum);
            mean_column_std1.append(sum1);
            mean_column_std2.append(sum2);
            mean_column_std3.append(sum3);
        }
        float column_noise_max =1;
        float column_noise_max0 = 0;
        int max_pos = 0;

        QVector <double>::const_iterator max_std = std::max_element(mean_column_std.begin(),mean_column_std.end());
        max_pos = (mean_column_std.indexOf(*max_std)*2);


        //均值
        double sumavg = std::accumulate(mean_column_std.begin(),mean_column_std.end(),0.0);
        double avg_std = sumavg/mean_column_std.size();
        float column_noise_avg =1;
        float column_noise_avg0 = 0;
        if(avg_std >0 )
            column_noise_avg0 = std::log10((avg_std)/1023);
        else
            column_noise_avg0 = 0;
        column_noise_avg = 20*column_noise_avg0;
        //均值

        column_noise_max =1;
        column_noise_max0 = 0;
        if(*max_std >0 )
            column_noise_max0 = std::log10((*max_std)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
        column_noise_channel.append(column_noise_max);
        double dif_max_avg = (column_noise_max-column_noise_avg);
        column_noise_channel_dif.append(dif_max_avg);
        double dif_max_avg_std = (*max_std-avg_std);
        column_std_channel_dif.append(dif_max_avg_std);
        g_str_msg_all[site_]+=boost::str(boost::format("R Channel column_mean_std_Max:%.4f, std_avg:%.4f, noise_max:%.4f, noise_avg:%.4f,dif_std:%.4f,dif_noise:%.4f, noise_max pos:%d\n")
                                         % (*max_std)  % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|")
                                         % (*max_std)  % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);

        QVector <double>::const_iterator max_std1 = std::max_element(mean_column_std1.begin(),mean_column_std1.end());
        max_pos = (mean_column_std1.indexOf(*max_std1)*2);

        //均值
        sumavg = std::accumulate(mean_column_std1.begin(),mean_column_std1.end(),0.0);
        avg_std = sumavg/mean_column_std1.size();
        column_noise_avg =1;
        column_noise_avg0 = 0;
        if(avg_std >0 )
            column_noise_avg0 = std::log10((avg_std)/1023);
        else
            column_noise_avg0 = 0;
        column_noise_avg = 20*column_noise_avg0;
        //均值

        column_noise_max =1;
        column_noise_max0 = 0;
        if(*max_std1 >0 )
            column_noise_max0 = std::log10((*max_std1)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
         column_noise_channel.append(column_noise_max);
        dif_max_avg = (column_noise_max-column_noise_avg);
        column_noise_channel_dif.append(dif_max_avg);
        dif_max_avg_std = (*max_std1-avg_std);
        column_std_channel_dif.append(dif_max_avg_std);
        g_str_msg_all[site_]+=boost::str(boost::format("Gr Channel column_mean_std_Max:%.4f,mean_std_avg:%.4f, noise_max:%.4f,noise_avg:%.4f, dif_std:%.4f,dif_noise:%.4f,noise_max pos:%d\n")
                                         % (*max_std1) % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg %  max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|")
                                         % (*max_std1) % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);

        QVector <double>::const_iterator max_std2 = std::max_element(mean_column_std2.begin(),mean_column_std2.end());
        max_pos = (mean_column_std2.indexOf(*max_std2)*2);

        //均值
        sumavg = std::accumulate(mean_column_std2.begin(),mean_column_std2.end(),0.0);
        avg_std = sumavg/mean_column_std2.size();
        column_noise_avg =1;
        column_noise_avg0 = 0;
        if(avg_std >0 )
            column_noise_avg0 = std::log10((avg_std)/1023);
        else
            column_noise_avg0 = 0;
        column_noise_avg = 20*column_noise_avg0;
        //均值

        column_noise_max =1;
        column_noise_max0 = 0;
        if(*max_std2 >0 )
            column_noise_max0 = std::log10((*max_std2)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
        column_noise_channel.append(column_noise_max);
        dif_max_avg = (column_noise_max-column_noise_avg);
        column_noise_channel_dif.append(dif_max_avg);
        dif_max_avg_std = (*max_std2-avg_std);
        column_std_channel_dif.append(dif_max_avg_std);
        g_str_msg_all[site_]+=boost::str(boost::format("Gb Channel column_mean_std_Max:%.4f, mean_std_avg:%.4f,noise_max:%.4f,noise_avg:%.4f,dif_std:%.4f,dif_noise:%.4f,noise_max pos:%d\n")
                                         % (*max_std2) % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg %  max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|")
                                         % (*max_std2) % avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);

        QVector <double>::const_iterator max_std3 = std::max_element(mean_column_std3.begin(),mean_column_std3.end());
        max_pos = (mean_column_std3.indexOf(*max_std3)*2);

        //均值
        sumavg = std::accumulate(mean_column_std3.begin(),mean_column_std3.end(),0.0);
        avg_std = sumavg/mean_column_std3.size();
        column_noise_avg =1;
        column_noise_avg0 = 0;
        if(avg_std >0 )
            column_noise_avg0 = std::log10((avg_std)/1023);
        else
            column_noise_avg0 = 0;
        column_noise_avg = 20*column_noise_avg0;
        //均值

        column_noise_max =1;
        column_noise_max0 = 0;
        if(*max_std3 >0 )
            column_noise_max0 = std::log10((*max_std3)/1023);
        else
            column_noise_max0 = 0;
        column_noise_max = 20*column_noise_max0;
        column_noise_channel.append(column_noise_max);
        dif_max_avg = (column_noise_max-column_noise_avg);
        column_noise_channel_dif.append(dif_max_avg);
        dif_max_avg_std = (*max_std3-avg_std);
        column_std_channel_dif.append(dif_max_avg_std);
        g_str_msg_all[site_]+=boost::str(boost::format("B Channel column_mean_std_Max:%.4f, mean_std_avg:%.4f,noise_max:%.4f,noise_avg:%.4f,dif_std:%.4f,dif_noise:%.4f,noise_max pos:%d\n")
                                         % (*max_std3) % avg_std % column_noise_max %column_noise_avg % dif_max_avg_std % dif_max_avg %  max_pos);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%d|")
                                         % (*max_std) %  avg_std % column_noise_max % column_noise_avg % dif_max_avg_std % dif_max_avg % max_pos);
        double max_noise_channel = *std::max_element(column_noise_channel.begin(),column_noise_channel.end());
        double min_noise_channel = *std::min_element(column_noise_channel.begin(),column_noise_channel.end());
        double max_noise_channel_dif = *std::max_element(column_noise_channel_dif.begin(),column_noise_channel_dif.end());
        double max_std_channel_dif = *std::max_element(column_std_channel_dif.begin(),column_std_channel_dif.end());



        double noise_channel_dif = max_noise_channel-min_noise_channel;
        g_str_msg_all[site_]+=boost::str(boost::format("Channel Max:%.4f, Channel Min:%.4f, Channel dif:%.4f ,Channel std max-avg dif max:%.4f,Channel noise max-avg dif max:%.4f\n")
                                         % (max_noise_channel) % min_noise_channel % noise_channel_dif % max_std_channel_dif % max_noise_channel_dif);
        g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|%.4f|%.4f|")
                                        % (max_noise_channel) % (noise_channel_dif) % (max_std_channel_dif) % (max_noise_channel_dif));
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());


        if(max_std_channel_dif > manage.param_channel_noise_.column_noise_spec_level_2_ && max_std_channel_dif > 0)
        {
            res.is_passed=false;
            res.bin=manage.param_channel_noise_.bin_level_2_;
            res.grade=manage.param_channel_noise_.grade_level_2_;
        }
        else if(max_std_channel_dif > manage.param_channel_noise_.column_noise_spec_level_1_ && max_std_channel_dif > 0)
        {
            res.is_passed=false;
            res.bin=manage.param_channel_noise_.bin_level_1_;
            res.grade=manage.param_channel_noise_.grade_level_1_;
        }


    }
    else
    {
        res.g_msg=false;
    }

    return res;
}




Subitem_Result NoiseTest::Noise_Dark_Read(const ParamManage& manage)
{
    int w = manage.width_;
    int h = manage.height_;
    Subitem_Result res("Noise_Read",true);

    if(manage.param_read_noise_.open_mode_ == 1)
    {

//        res.name="Noise_Read";
        int roi_x = manage.param_read_noise_.roi_x_;
        int roi_y = manage.param_read_noise_.roi_y_;
        int roi_w = manage.param_read_noise_.roi_w_;
        int roi_h = manage.param_read_noise_.roi_h_;

        cv::Rect roi = cv::Rect(0,0,w/2-1,h/2-1) & cv::Rect(roi_x, roi_y, roi_w, roi_h);
        vector<double> split_mean(Channel_Num,0),split_std_v(Channel_Num,0);

        MultiThread::get_double_roi_mean_std_by_rect_chao *p_roi[Channel_Num];
        for (int i=0; i < Channel_Num; i++)
            p_roi[i] = new MultiThread::get_double_roi_mean_std_by_rect_chao();

        p_roi[R]->setparam(g_buf_delta_raw_,Point2i(0,0),roi,w,h);
        p_roi[Gr]->setparam(g_buf_delta_raw_,Point2i(1,0),roi,w,h);
        p_roi[Gb]->setparam(g_buf_delta_raw_,Point2i(0,1),roi,w,h);
        p_roi[B]->setparam(g_buf_delta_raw_,Point2i(1,1),roi,w,h);

        for (int i=0; i < Channel_Num; i++)
            p_roi[i]->start();
        for (int i=0; i < Channel_Num; i++)
            p_roi[i]->wait();

        for (int i=0; i < Channel_Num; i++)
        {
            split_mean[i]= p_roi[i]->mean;
            split_std_v[i]= p_roi[i]->std;
        }

        switch (manage.bayer_) {
           case RG:
               break;
           case BG:
               swap(split_mean[0],split_mean[3]);
               swap(split_std_v[0],split_std_v[3]);
               swap(split_mean[1],split_mean[2]);
               swap(split_std_v[1],split_std_v[2]);
               break;
           case GR:
               swap(split_mean[0],split_mean[1]);
               swap(split_std_v[0],split_std_v[1]);
               swap(split_mean[2],split_mean[3]);
               swap(split_std_v[2],split_std_v[3]);
               break;
           case GB:
               swap(split_mean[0],split_mean[2]);
               swap(split_std_v[0],split_std_v[2]);
               swap(split_mean[1],split_mean[3]);
               swap(split_std_v[1],split_std_v[3]);
               break;
           default:
               break;
           }

        for (int i=0; i < Channel_Num; i++)
        {
            if(i==0)
                g_str_msg_all[site_]+=boost::str(boost::format("R Channel Read Noise Mean: %.4f, Read Noise Std: %.4f\n")
                                       % split_mean[i] % split_std_v[i]);
            else if(i==1)
                g_str_msg_all[site_]+=boost::str(boost::format("Gr Channel Read Noise Mean: %.4f, Read Noise Std: %.4f\n")
                                        % split_mean[i] % split_std_v[i]);
            else if(i==2)
                g_str_msg_all[site_]+=boost::str(boost::format("Gb Channel Read Noise Mean: %.4f, Read Noise Std: %.4f\n")
                                        % split_mean[i] % split_std_v[i]);
            else if(i==3)
                g_str_msg_all[site_]+=boost::str(boost::format("B Channel Read Noise Mean: %.4f, Read Noise Std: %.4f\n")
                                        % split_mean[i] % split_std_v[i]);
            g_str_log_all[site_]+=boost::str(boost::format("%.4f|%.4f|")
                                    % split_mean[i] % split_std_v[i]);
        }
        g_str_log_all[site_]+=boost::str(boost::format("%s|") % res.name.c_str());

        for(int i=0; i< Channel_Num; i++)
            delete p_roi[i];

    }
    else
    {
        res.g_msg=false;
    }

    return res;
}


//
Subitem_Result NoiseTest::Noise_Dark_by_pixel_chao(const ParamManage& manage,Noise_Dark_Pixel_type type)
{

    if(type == Noise_Dark_Pixel_row)
    {
        if(manage.row_Noise_version == "row_Noise_version_V1.0")
            return Noise_Dark_Row(manage);
    }

    else if(type == Noise_Dark_Pixel_ColRTS)
    {
        if(manage.column_Noise_version == "column_Noise_version_V1.0")
            return Noise_Dark_ColRTS(manage);
    }

    else if(type == Noise_Dark_Pixel_Col_chanel)
    {
        if(manage.col_channel_noise_version == "col_channel_noise_version_V1.0")
            return Noise_Dark_Col_Channel(manage);
        else if(manage.col_channel_noise_version == "col_channel_noise_version_V1.1")
            return Noise_Dark_Col_Channel_V1_1(manage);
    }

    else if(type == Noise_Dark_Pixel_read)
    {
        if(manage.read_Noise_version == "read_Noise_version_V1.0")
            return Noise_Dark_Read(manage);
    }

    Subitem_Result res("Noise", false);
    res.g_msg = false;
    return res;
}







