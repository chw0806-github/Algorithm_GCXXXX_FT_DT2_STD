#include "cpalgorithmraw.h"
#include <QElapsedTimer>

using namespace std;
using namespace cv;

#define BOOST_NO_EXCEPTIONS
#include <boost/throw_exception.hpp>
//void boost::throw_exception(std::exception const & e){
////do nothing
//}

template<typename T>
double AlgorithmFuncs::GetDatasStd(std::vector<T> &datas)
{
    if (datas.size ()==0)
        return 0;
    double mean_t = 0;
    for (int i=0; i<datas.size (); ++i)
        mean_t+=(double)datas.at (i);
    mean_t/=datas.size ();
    double std_t = 0.0;
    for (int i=0; i<datas.size (); ++i)
    {
        std_t+=(double)((mean_t-datas.at (i))*(mean_t-datas.at (i)));
    }
    std_t/=datas.size ()-1;
    return std::sqrt (std_t);
}

template<typename T>
double  AlgorithmFuncs::GetDatasStd(std::vector<T> &datas,double mean_t)
{
    double std_t = 0.0;
    for (int i=0; i<datas.size (); ++i)
    {
        std_t+=(double)((mean_t-datas.at (i))*(mean_t-datas.at (i)));
    }
    std_t/=datas.size ()-1;
    return std::sqrt (std_t);
}


void AlgorithmFuncs::raw10_to_raw16_several_frame(uchar *pData, short *p_raw16, int w, int h, int frameCnt )
{
        short *pshort_tmp = p_raw16;
        for(int cnt = 0; cnt< frameCnt; cnt++)
        {
            uchar *p_tmp = pData + w*h*5/4*cnt;
            for (int i=0; i<w*h*5/4; ++i)
            {
                if ((i+1)%5 != 0)
                {
                    uchar val = p_tmp[i];
                    int grp = (i+1)/5;
                    uchar val_2L = p_tmp[grp*5+4];
                    int offset = (i+1)%5;

                    if (offset == 4)
                        *pshort_tmp++ = (val<<2)|((val_2L&0xc0)>>6);
                    else if (offset == 3)
                        *pshort_tmp++ = (val<<2)|((val_2L&0x30)>>4);
                    else if (offset == 2)
                        *pshort_tmp++ = (val<<2)|((val_2L&0x0c)>>2);
                    else if (offset == 1)
                        *pshort_tmp++ = (val<<2)|(val_2L&0x03);
                }
            }
        }
}


void AlgorithmFuncs::two_short_images_minus(short *pData1, short *pData2, short *pDiff, int w, int h)
{
    short *pshort1 = pData1;
    short *pshort2 = pData2;
    short *pshort_diff = pDiff;

    for(int i=0; i<w*h; i++)
        *pshort_diff++ =*pshort1++ - *pshort2++;
}

int AlgorithmFuncs::find_specific_area_in_binary_map(cv::Mat binaryImage, double area_spec)
{
//    IplImage* src = &IplImage(binaryImage);
//    IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);

//    CvMemStorage* storage = cvCreateMemStorage(0);
//    CvSeq* contour = NULL;
//    int contour_num = cvFindContours(src, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
//    cvZero(dst);

    int m = 0;
//    for( ; contour != 0; contour = contour->h_next )
//    {
//        double tmparea = fabs(cvContourArea(contour));
//        if(tmparea > area_spec)
//        {
//            m++;
//            CvScalar color = CV_RGB( 0, 255, 255 );
//            cvDrawContours(dst, contour, color, color, -1, 1, 8);   //绘制外部和内部的轮廓
//        }
//        else
//            cvSeqRemove(contour, 0); // 删除面积小于设定值的轮廓

//    }
//    cvSaveImage("./counter.bmp",dst);
//    cvReleaseImage(&dst);
//    cvReleaseMemStorage(&storage);

    return m;
}



bool ACE_Enhance(cv::Mat& src_img, cv::Mat& dst_img, unsigned int half_winSize, double Max_Q, double threshold)
{
    int rows(src_img.rows);
    int cols(src_img.cols);
    float* pdata = NULL;
    float* pdata1 = NULL;
    cv::Mat tmpDstImg(rows, cols, CV_32FC1);
    for (int i = 2*half_winSize; i < (rows - 2*half_winSize); i++)
    {
        pdata = tmpDstImg.ptr<float>(i);
        pdata1 = src_img.ptr<float>(i);
        for (int j = 2*half_winSize; j < (cols - 2*half_winSize); j++)
        {
            cv::Mat tempArea_outer = src_img(cv::Rect(j - half_winSize, i - half_winSize, half_winSize * 2 + 1, half_winSize * 2 + 1));   //截取窗口图像

            cv::Scalar mean_value_outer = cv::mean(tempArea_outer);

            double MeanVlaue_outer = mean_value_outer[0];


            cv::Mat tempArea_inner = src_img(cv::Rect(j - 1, i - 1, 1 * 2+1 , 1 * 2+1 ));

            cv::Scalar mean_value_inner = cv::mean(tempArea_inner);

            double MeanVlaue_inner = mean_value_inner[0];



            double delta = MeanVlaue_inner - MeanVlaue_outer;

            if (std::abs(delta)>MeanVlaue_outer*threshold)
            {
                double pixelvalue = Max_Q*delta;
                double tempValue = pdata1[j] + pixelvalue;
                pdata[j] = tempValue;
            }
            else
            {
                pdata[j] = pdata1[j];

            }
        }

    }

    dst_img = tmpDstImg;

    return true;
}



int AlgorithmFuncs::Bright_ImageDirttDetect(cv::Mat srcImage, int src_width,int src_height,int pre_blur_size_, int img_resize,float lsc_ratio,
                            int blur_r1, int blur_r2, float pog_spec_p,float pog_spec_n, double &max_pog, int& max_pos,
                            double &min_pog,int& min_pos,float gfda_scratch_spec_p, float gfda_spot_spec_p,float gfda_spot_spec_n,
                            int gfda_spot_limit_size,float leda_ratio_n, float leda_ratio_p, float leda_spot_spec_p ,int leda_spot_limit_size,
                            int stain_skip, int stain_r_kernel,int stain_r_outer,float stain_spec,int saveimage,
                            int &dark_bar_cnt,int &GFDA_pog_cnt,int &GFDA_scratch_cnt,int &GFDA_spot_cnt,int &LEDA_spot_cnt,
                            int &LDMA_stain_cnt,std::string & msg)


{




    if(saveimage)
    {
        QDir dir;
        dir.mkpath("./Preprocess_Image");
        dir.mkpath("./DarkBar_Image");
        dir.mkpath("./GFDA_Image");
        dir.mkpath("./LEDA_Image");
        dir.mkpath("./LDMA_Image");
    }

    int all_count=0;
    GFDA_pog_cnt =GFDA_scratch_cnt =GFDA_spot_cnt=0;
    dark_bar_cnt =LEDA_spot_cnt =LDMA_stain_cnt=0;


    /* algorithm */

    //============================= Raw 预处理 =============================
    //=========================    Preprocess    =============================

    //----------读取原始图像----------------
    cv::Mat srcImage_32;
    srcImage.convertTo(srcImage_32,CV_32FC1);
    if(saveimage)
           cv::imwrite(QString("./Preprocess_Image/SrcImage.bmp").toStdString().c_str(),srcImage_32);

    //算法剔除单坏点
    float signal_pixel_DN_spec=35;
    float signal_pixel_ratio_spec=0.4;
    for (int y = 0; y < srcImage_32.rows; y++)
    {
        for (int x = 2; x < srcImage_32.cols-2; x++)
        {
            float pixel_diff=srcImage_32.at<float>(y, x)-srcImage_32.at<float>(y, x-2);
            float pixel_diff_=srcImage_32.at<float>(y, x)-srcImage_32.at<float>(y, x+2);
            if(pixel_diff*pixel_diff>0 && fabs(pixel_diff)>signal_pixel_DN_spec &&fabs(pixel_diff_)>signal_pixel_DN_spec)
                srcImage_32.at<float>(y, x)=(srcImage_32.at<float>(y, x-2)+srcImage_32.at<float>(y, x+2))/2;

            float pixel_diff_ratio=pixel_diff/srcImage_32.at<float>(y, x);
            float pixel_diff_ratio_=pixel_diff_/srcImage_32.at<float>(y, x);
            if(pixel_diff*pixel_diff>0 && fabs(pixel_diff_ratio)>signal_pixel_ratio_spec &&fabs(pixel_diff_ratio_)>signal_pixel_ratio_spec)
                srcImage_32.at<float>(y, x)=(srcImage_32.at<float>(y, x-2)+srcImage_32.at<float>(y, x+2))/2;
        }
    }
    if(saveimage)
        cv::imwrite(QString("./Preprocess_Image/Small_image_dpc.bmp").toStdString().c_str(),srcImage_32);

    //----------压缩1/2 ，去除Bayer结构----------------
    cv::Mat src_small;
    cv::resize(srcImage_32,src_small,cv::Size(src_width/2,src_height/2),0,0,cv::INTER_LINEAR);
    if(saveimage)
        cv::imwrite(QString("./Preprocess_Image/Small_image.bmp").toStdString().c_str(),src_small);

    //----------均值滤波----------------
    cv::Mat src_blur_img;
    cv::blur(src_small,src_blur_img,cv::Size(pre_blur_size_,pre_blur_size_));
    if(saveimage)
        cv::imwrite(QString("./Preprocess_Image/Blur_image.bmp").toStdString().c_str(),src_blur_img);

    //-----------中值去噪--------------
    cv::Mat src_medianBlur_image;
    cv::medianBlur(src_blur_img,src_medianBlur_image,3);
    if(saveimage)
        cv::imwrite(QString("./Preprocess_Image/MedianBlur_image.bmp").toStdString().c_str(),src_medianBlur_image);

    //----------压缩为固定像素----------------
    double resize_ratio=sqrt( img_resize / static_cast<double>(src_width*src_height));          //width & height 的压缩比例
    int resize_w=static_cast<int>(src_width*resize_ratio) ;
    int resize_h=static_cast<int>(src_height*resize_ratio);
    cv::Mat src_smallx2;
    cv::resize(src_medianBlur_image,src_smallx2,cv::Size(resize_w,resize_h),0,0,cv::INTER_LINEAR);
    if(saveimage)
        cv::imwrite(QString("./Preprocess_Image/Smallx2_image.bmp").toStdString().c_str(),src_smallx2);
    cv::Mat ldea_image = src_smallx2.clone();

    //----------LSC----------------
    cv::Mat  lsc_img = src_smallx2;
    double mean_corner =0;
    for(int i=0; i<30; i++)
    {
        float *p_row_small = src_smallx2.ptr<float>(i);
        for(int j=0; j<40; j++)
            mean_corner += p_row_small[j];
    }
    double mean_center =0;
    for(int i=src_smallx2.rows/2-15; i<src_smallx2.rows/2+15; i++)
    {
        float *p_row_small = src_smallx2.ptr<float>(i);
        for(int j=src_smallx2.cols/2-20; j<src_smallx2.cols/2+20; j++)
            mean_center += p_row_small[j];
    }

    mean_corner=fabs(mean_corner)<=1e-6? 1:mean_corner;
    double delta = (mean_center/mean_corner -1)*lsc_ratio;
    double factor = delta/((resize_w/2)*(resize_w/2) +(resize_h/2)*(resize_h/2));
    for(int i=0; i<src_smallx2.rows; i++)
    {
        float *p_row_small = src_smallx2.ptr<float>(i);
        float *p_row_lsc = lsc_img.ptr<float>(i);
        int y = std::abs(i-resize_h/2);
        for(int j=0; j<src_smallx2.cols; j++)
        {
            int x = std::abs(j-resize_w/2);
            p_row_lsc[j] = p_row_small[j] * ((x*x + y*y) * factor + 1);
        }
    }
    if(saveimage)
        cv::imwrite(QString("./Preprocess_Image/LSC_image.bmp").toStdString().c_str(),lsc_img);

    //----------对比度增强----------------
    cv::Mat  enhace;
    double  minV, maxV;
    int  minId[2] = {}, maxId[2] = {};	// minnimum Index, maximum Index
    cv::minMaxIdx(lsc_img, &minV, &maxV, minId, maxId);
    double max_ds = 250;
    double min_ds = 50;
    double max_min_diff=maxV-minV>1? maxV-minV:1;
    enhace = (lsc_img - minV)*(max_ds-min_ds)/max_min_diff+min_ds;
    if(saveimage)
        cv::imwrite(QString("./Preprocess_Image/Enhace_image.bmp").toStdString().c_str(),enhace);


    //============================= 全局暗带捕捉算法 =============================

    int grid_size=11;
    cv::Mat dark_bar_re=enhace.clone();
    int grid_w=dark_bar_re.cols/grid_size;
    int grid_h=dark_bar_re.rows/grid_size;
    cv::Mat dark_bar_map= cv::Mat::zeros(grid_h,grid_w,CV_32FC1);

    for(int i=0;i!=grid_h;++i)
    {
        QVector<float> dark_bar_row;
        for(int j=0;j!=grid_w;++j)
        {
            float mean=0;
            for(int m=0;m!=grid_size;m++)
            {
                for(int n=0;n!=grid_size;n++)
                {
                    int x_=grid_size*j+m;
                    int y_=grid_size*i+n;
                    mean+=dark_bar_re.at<float>(y_, x_);
                }
            }
            mean/=grid_size*grid_size;
            dark_bar_map.at<float>(i, j)=mean;
        }
    }
    if(saveimage)
        cv::imwrite(QString("./DarkBar_Image/dark_bar_grid.bmp").toStdString().c_str(),dark_bar_map);

    //计算4个方向的 diff_1-diff_2
    int darkbar_cnt=0;
    float darkbar_spec=140;

    //row
    cv::Mat diff_map_0(dark_bar_map.size(),CV_32FC1);
    cv::Mat kern = (cv::Mat_<double>(3,3) << 0,0,0,1,-2,1,0,0,0);
    cv::filter2D(dark_bar_map,diff_map_0,dark_bar_map.depth(),kern);
    dark_bar_cnt+=Darkbar_Binary(diff_map_0,darkbar_spec,saveimage,0,msg);

   //col
    cv::Mat diff_map_90(dark_bar_map.size(),CV_32FC1);
    kern = (cv::Mat_<double>(3,3) << 0,1,0,0,-2,0,0,1,0);
    cv::filter2D(dark_bar_map,diff_map_90,dark_bar_map.depth(),kern);
    dark_bar_cnt+=Darkbar_Binary(diff_map_90,darkbar_spec,saveimage,90,msg);

    //right_up————left_down
    cv::Mat diff_map_45(dark_bar_map.size(),CV_32FC1);
    kern = (cv::Mat_<double>(3,3) << 0,0,1,0,-2,0,1,0,0);
    cv::filter2D(dark_bar_map,diff_map_45,dark_bar_map.depth(),kern);
    dark_bar_cnt+=Darkbar_Binary(diff_map_45,darkbar_spec,saveimage,45,msg);

    //left_up————right_down
    cv::Mat diff_map_135(dark_bar_map.size(),CV_32FC1);
    kern = (cv::Mat_<double>(3,3) << 1,0,0,0,-2,0,0,0,1);
    cv::filter2D(dark_bar_map,diff_map_135,dark_bar_map.depth(),kern);
    dark_bar_cnt+=Darkbar_Binary(diff_map_135,darkbar_spec,saveimage,135,msg);


    //============================= 全局滤波差值异常捕捉算法 =============================
    //================= G-F-D-A (global filter differential abnormal )=============================

    cv::Mat blur_img = enhace.clone();

    //----------均值滤波A----------------
    cv::Mat blur_A,small_tmp_A,blur_A_tmp;
    cv::medianBlur(blur_img, small_tmp_A,5);
    cv::copyMakeBorder(small_tmp_A, small_tmp_A, blur_r1/2, blur_r1/2, blur_r1/2, blur_r1/2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_A,blur_A_tmp,cv::Size(blur_r1,blur_r1));
    blur_A = blur_A_tmp(cv::Rect(blur_r1/2,blur_r1/2,blur_img.cols,blur_img.rows));
    if(saveimage)
        cv::imwrite(QString("./GFDA_Image/blur_A_image.bmp").toStdString().c_str(),blur_A);

    cv::Mat blur_A2,small_tmp_A2,blur_A2_tmp;
    cv::medianBlur(blur_A,small_tmp_A2,5);
    cv::copyMakeBorder(small_tmp_A2, small_tmp_A2, 2, 2, 2, 2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_A2,blur_A2_tmp,cv::Size(5,5));
    blur_A2 = blur_A2_tmp(cv::Rect(2,2,blur_img.cols,blur_img.rows));
    if(saveimage)
        cv::imwrite(QString("./GFDA_Image/blur_A2_image.bmp").toStdString().c_str(),blur_A2);

    //----------均值滤波B----------------
    cv::Mat blur_B,small_tmp_B,blur_B_tmp;
    cv::copyMakeBorder(blur_img, small_tmp_B, blur_r2/2, blur_r2/2, blur_r2/2, blur_r2/2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_B,blur_B_tmp,cv::Size(blur_r2,blur_r2));
    blur_B = blur_B_tmp(cv::Rect(blur_r2/2,blur_r2/2,blur_img.cols,blur_img.rows));
    if(saveimage)
        cv::imwrite(QString("./GFDA_Image/blur_B_image.bmp").toStdString().c_str(),blur_B);

    //--------均值滤波AB差--------------
    cv::Rect pog_crop_size=cv::Rect(blur_r1,blur_r1,blur_img.cols-2*blur_r1,blur_img.rows-2*blur_r1);
    cv::Mat blur_diff_ = blur_A2 -blur_B;
    cv::Mat blur_diff_crop = blur_diff_(pog_crop_size);
    cv::Mat image_blur_diff;
    cv::medianBlur(blur_diff_crop,image_blur_diff,1);

    cv::Mat blur_diff_dark= cv::Mat::zeros(image_blur_diff.rows,image_blur_diff.cols,CV_32FC1);
    cv::Mat blur_diff_bright= cv::Mat::zeros(image_blur_diff.rows,image_blur_diff.cols,CV_32FC1);
    for (int y = 0; y < image_blur_diff.rows; y++)
    {
        for (int x = 0; x < image_blur_diff.cols; x++)
        {
            if(image_blur_diff.at<float>(y, x)>0)
                blur_diff_dark.at<float>(y, x)=image_blur_diff.at<float>(y, x);
            else
                blur_diff_bright.at<float>(y, x)=image_blur_diff.at<float>(y, x);
        }
    }
    if(saveimage)
    {
        cv::Mat diff_pog_16u=image_blur_diff.clone();
        diff_pog_16u=abs(diff_pog_16u)*100.0;
        diff_pog_16u.convertTo(diff_pog_16u,CV_16U);
        cv::imwrite(QString("./GFDA_Image/blur_diff_image.png").toStdString().c_str(),diff_pog_16u);

        QString file_name_=QString("./GFDA_Image/blur_Diff_Data.csv");
        FILE* fp = fopen(file_name_.toStdString().c_str(),"wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
        for (int y = 0; y < image_blur_diff.rows; y++)
        {
            for (int x = 0; x < image_blur_diff.cols; x++)
            {
                float point = image_blur_diff.at<float>(y, x);
                fprintf(fp, "%4.2f,", point);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);

        cv::Mat blur_diff_dark_16u=blur_diff_dark.clone()*100.0;
        blur_diff_dark_16u.convertTo(blur_diff_dark_16u,CV_16U);
        cv::imwrite(QString("./GFDA_Image/blur_dark_diff_image.png").toStdString().c_str(),blur_diff_dark_16u);

        cv::Mat blur_diff_bright_16u=abs(blur_diff_bright.clone())*100.0;
        blur_diff_bright_16u.convertTo(blur_diff_bright_16u,CV_16U);
        cv::imwrite(QString("./GFDA_Image/blur_bright_diff_image.png").toStdString().c_str(),blur_diff_bright_16u);
    }


    //-------- 连通区域检测  (POG) --------------
    int max_pos_index[2]={0};
    int min_pos_index[2]={0};
    cv::minMaxIdx(image_blur_diff,&min_pog,&max_pog,min_pos_index,max_pos_index);
    max_pos = max_pos_index[1]*image_blur_diff.cols +max_pos_index[0];
    min_pos = min_pos_index[1]*image_blur_diff.cols +min_pos_index[0];
    cv::Mat binary_pog = cv::Mat::zeros(image_blur_diff.rows,image_blur_diff.cols,CV_8U);
    for(int i=0; i<binary_pog.rows; i++)
    {
        uchar *p_row_binary_pog = binary_pog.ptr<uchar>(i);
        float *p_row_diff= image_blur_diff.ptr<float>(i);

        for(int j=0; j<binary_pog.cols; j++)
        {

            if(p_row_diff[j]>pog_spec_p || p_row_diff[j]<pog_spec_n)
            {
                p_row_binary_pog[j] =255;
                GFDA_pog_cnt++;
            }
            else
                p_row_binary_pog[j] =0;
        }
    }
    if(saveimage)
        cv::imwrite(QString("./GFDA_Image/pog_binary_image.bmp").toStdString().c_str(),binary_pog);


    //-------- 霍夫直线检测  (Scratch) --------------
    //暗线
    cv::Mat gfba_hougu_binary=cv::Mat::zeros(blur_diff_dark.rows,blur_diff_dark.cols,CV_8U);

    for (int y = 0; y < blur_diff_dark.rows; y++)
    {
        for (int x = 0; x < blur_diff_dark.cols; x++)
        {
            if(blur_diff_dark.at<float>(y, x)>gfda_scratch_spec_p)
                gfba_hougu_binary.at<uchar>(y, x)=255;
        }
    }
    vector<Vec4i> gfda_lines;
    HoughLinesP(gfba_hougu_binary, gfda_lines, 1, CV_PI/180, 30, 35, 2);   //连续线条
    vector<Vec4i> gfda_lines_2;
    HoughLinesP(gfba_hougu_binary, gfda_lines_2, 1, CV_PI/180, 25, 50, 4);  //断续线条 gfda_lines.insert(gfda_lines.end(), gfda_lines_2.begin(), gfda_lines_2.end());

    GFDA_scratch_cnt+=gfda_lines.size();
    if(saveimage)
    {
        cv::Mat houghline=cv::Mat::zeros(blur_diff_dark.rows,blur_diff_dark.cols,CV_8U);
        for( size_t i = 0; i < gfda_lines.size(); i++ )
        {
            Vec4i l = gfda_lines[i];
            line( houghline, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 1);
        }
        cv::imwrite(QString("./GFDA_Image/hougt_dark_binary_image.bmp").toStdString().c_str(),gfba_hougu_binary);
        cv::imwrite(QString("./GFDA_Image/hougt_dark_line_image.bmp").toStdString().c_str(),houghline);
    }


    //亮线
    float gfda_scratch_spec_n=-1.2;
    cv::Mat gfba_hougu_bright_binary=cv::Mat::zeros(blur_diff_bright.rows,blur_diff_bright.cols,CV_8U);
    for (int y = 0; y < blur_diff_bright.rows; y++)
    {
        for (int x = 0; x < blur_diff_bright.cols; x++)
        {
            if(blur_diff_bright.at<float>(y, x)<gfda_scratch_spec_n)
                gfba_hougu_bright_binary.at<uchar>(y, x)=255;
        }
    }

    vector<Vec4i> gfda_light_lines;
    HoughLinesP(gfba_hougu_bright_binary, gfda_light_lines, 1, CV_PI/180, 30, 35, 2);   //连续线条
    vector<Vec4i> gfda_light_lines_2;
    HoughLinesP(gfba_hougu_bright_binary, gfda_light_lines_2, 1, CV_PI/180, 25, 50, 4);  //断续线条 gfda_lines.insert(gfda_lines.end(), gfda_lines_2.begin(), gfda_lines_2.end());
    gfda_light_lines.insert(gfda_light_lines.end(), gfda_light_lines_2.begin(), gfda_light_lines_2.end());

    GFDA_scratch_cnt+=gfda_light_lines.size();
    if(saveimage)
    {
        cv::Mat houghline=cv::Mat::zeros(blur_diff_bright.rows,blur_diff_bright.cols,CV_8U);
        for( size_t i = 0; i < gfda_light_lines.size(); i++ )
        {
            Vec4i l = gfda_light_lines[i];
            line( houghline, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 1);
        }
        cv::imwrite(QString("./GFDA_Image/hougt_bright_binary_image.bmp").toStdString().c_str(),gfba_hougu_bright_binary);
        cv::imwrite(QString("./GFDA_Image/hougt_bright_line_image.bmp").toStdString().c_str(),houghline);
    }


     //------- 连通区域检测  (Spot) --------------
    int gfda_cluster_r=1;
    QVector<QSet<int> > gfda_spot_group;
    cv::Mat spot_dark_binary=cv::Mat::zeros(blur_diff_dark.rows,blur_diff_dark.cols,CV_8U);
//    int max_pos_index1[2]={0};
//    int min_pos_index1[2]={0};
//    double max_scratch = 0.00;
//    double min_scratch = 0.00;
//    cv::minMaxIdx(blur_diff_dark,&min_scratch,&max_scratch,min_pos_index1,max_pos_index1);

//    msg+=boost::str(boost::format("GFDA_max_spot : %f\n")
//                           % max_scratch);
//    msg+=boost::str(boost::format("GFDA_min_spot : %f\n")
//                           % min_scratch);
    for (int y = 0; y < blur_diff_dark.rows; y++)
    {
        for (int x = 0; x < blur_diff_dark.cols; x++)
        {
            if(blur_diff_dark.at<float>(y, x) >gfda_spot_spec_p )
                spot_dark_binary.at<uchar>(y, x)=255;
        }
    }
    GFDA_spot_cnt+=ConnectedDomainDetect(spot_dark_binary,gfda_cluster_r,gfda_spot_limit_size,gfda_spot_group);
    if(saveimage)
        cv::imwrite(QString("./GFDA_Image/spot_dark_binary_image.bmp").toStdString().c_str(),spot_dark_binary);

    cv::Mat spot_bright_binary=cv::Mat::zeros(blur_diff_bright.rows,blur_diff_bright.cols,CV_8U);
    for (int y = 0; y < blur_diff_bright.rows; y++)
    {
        for (int x = 0; x < blur_diff_bright.cols; x++)
        {
            if(blur_diff_bright.at<float>(y, x) <gfda_spot_spec_n )
                spot_bright_binary.at<uchar>(y, x)=255;
        }
    }
    GFDA_spot_cnt+=ConnectedDomainDetect(spot_bright_binary,gfda_cluster_r,gfda_spot_limit_size,gfda_spot_group);
    if(saveimage)
        cv::imwrite(QString("./GFDA_Image/spot_bright_binary_image.bmp").toStdString().c_str(),spot_bright_binary);


    //============================= 局域做差极值异常捕捉法 =============================
    //================= L-E-D-A (local extreme differential abnormal)=============================

    int skip=2;
    int ldea_w=ldea_image.cols;
    int ldea_h=ldea_image.rows;
    QVector<float> diff_max_ratio(ldea_w*ldea_h,0);
    QVector<float> diff_min_ratio(ldea_w*ldea_h,0);
    QVector<float> diff_ratio(ldea_w*ldea_h,0);
    for(int i=skip; i!=ldea_h-skip; i++)
     {
        for(int j=skip; j!=ldea_w-skip; j++)
        {
            QVector<QPoint> outer_start{ QPoint(j-skip ,i-skip),
                                         QPoint(j-skip ,i     ),
                                         QPoint(j      ,i-skip),
                                         QPoint(j      ,i     )};
            QVector<float> outer_mean;
            for(int index=0;index!=outer_start.size();++index)
            {
                float outer_mean_=0;
                for(int m=0;m<=skip;++m)
                {
                    for(int n=0;n<=skip;++n)
                    {
                        int x_=outer_start[index].x()+n;
                        int y_=outer_start[index].y()+m;
                        outer_mean_+=ldea_image.at<float>(y_, x_);
                    }
                }
                int out_count=(skip+1)*(skip+1)-1;
                outer_mean_=(outer_mean_-ldea_image.at<float>(i, j))/out_count;
                outer_mean.push_back(outer_mean_);
            }

            float max_ratio=-10000;
            float min_ratio=10000;
            for(int index=0;index!=outer_mean.size();++index)
            {
                float ratio_=(ldea_image.at<float>(i, j)-outer_mean[index])/ldea_image.at<float>(i, j)*100;
                max_ratio=std::max(max_ratio,ratio_);
                min_ratio=std::min(min_ratio,ratio_);
            }
            diff_max_ratio[ldea_w*i+j]=max_ratio;
            diff_min_ratio[ldea_w*i+j]=min_ratio;
        }
    }

    //比周围四象限都暗
    cv::Mat max_ratio_map = cv::Mat(ldea_h,ldea_w,CV_32FC1);
    for(int i=0; i<ldea_h; i++)
    {
        float *p_row_pog = max_ratio_map.ptr<float>(i);
        for(int j=0; j<ldea_w; j++)
        {
            if(diff_max_ratio[ldea_w*i+j]<leda_ratio_n)
                p_row_pog[j] =-diff_max_ratio[ldea_w*i+j]*100;
            else
                p_row_pog[j] =0;
        }
    }

    //比周围四象限都亮
    cv::Mat min_ratio_map = cv::Mat(ldea_h,ldea_w,CV_32FC1);
    for(int i=0; i<ldea_h; i++)
    {
        float *p_row_pog = min_ratio_map.ptr<float>(i);
        for(int j=0; j<ldea_w; j++)
        {
            if(diff_min_ratio[ldea_w*i+j]>leda_ratio_p)
                p_row_pog[j] =diff_min_ratio[ldea_w*i+j]*100;
            else
                p_row_pog[j] =0;
        }
    }

    //merger
    cv::Mat merger_ratio_map = cv::Mat(ldea_h,ldea_w,CV_32FC1);
    cv::Mat ldea_binary = cv::Mat::zeros(ldea_h,ldea_w,CV_8U);
    int spot_skip=4;
    for(int i=spot_skip; i<ldea_h-spot_skip; i++)
    {
        float *p_row_pog = merger_ratio_map.ptr<float>(i);
        float *p_bright_pog = min_ratio_map.ptr<float>(i);
        float *p_dark_pog = max_ratio_map.ptr<float>(i);
        uchar *p_spot = ldea_binary.ptr<uchar>(i);
        for(int j=spot_skip; j<ldea_w-spot_skip; j++)
        {
            p_row_pog[j] =p_bright_pog[j]+p_dark_pog[j];
            if( i<ldea_h/4 || i>ldea_h/4*3  || j<ldea_w/4 || j>ldea_w/4*3 )
                p_spot[j] =p_row_pog[j]>0? 255:0;
        }
    }
    if(saveimage)
    {
        cv::Mat max_ratio_map_16u=max_ratio_map.clone();
        max_ratio_map_16u.convertTo(max_ratio_map_16u,CV_16U);
        cv::imwrite(QString("./LEDA_Image/ratio_dark.png").toStdString().c_str(),max_ratio_map_16u);

        cv::Mat min_ratio_map_16u=min_ratio_map.clone();
        min_ratio_map_16u.convertTo(min_ratio_map_16u,CV_16U);
        cv::imwrite(QString("./LEDA_Image/ratio_bright.png").toStdString().c_str(),min_ratio_map_16u);

        cv::Mat merger_ratio_map_16u=merger_ratio_map.clone();
        merger_ratio_map_16u.convertTo(merger_ratio_map_16u,CV_16U);
        cv::imwrite(QString("./LEDA_Image/ratio_merger.png").toStdString().c_str(),merger_ratio_map_16u);
        cv::imwrite(QString("./LEDA_Image/ratio_merger_binary.bmp").toStdString().c_str(),ldea_binary);
    }

    //------- 连通区域检测  (Spot) --------------
    leda_spot_spec_p*=100;
    for(int i=spot_skip; i<ldea_h-spot_skip; i++)
    {
        float *p_row_ratio = merger_ratio_map.ptr<float>(i);
        for(int j=spot_skip; j<ldea_w-spot_skip; j++)
        {
            if( i<ldea_h/4 || i>ldea_h/4*3  || j<ldea_w/4 || j>ldea_w/4*3 )
            {
                if(p_row_ratio[j]>leda_spot_spec_p)
                    LEDA_spot_cnt++;
            }
        }
    }


    //============================= 局域做差均值异常捕捉法 =============================
    //================= L-D-M-A (local differential mean abnormal)=============================

    //------- 坏点检测  (Stain) --------------
    int ldma_w = enhace.cols;
    int ldma_h = enhace.rows;
    QVector<float> ratio_blemish(ldma_w*ldma_h,0);
    cv::Mat ldma_stain_binary = cv::Mat::zeros(ldma_h,ldma_w,CV_8U);
    for (int i=stain_skip; i<ldma_h-stain_skip; i++)
    {
        for (int j=stain_skip; j<ldma_w-stain_skip; j++)
        {
            // Kernel
            int cnt = 0;
            float mean_t = 0.0;
            for (int l=i-stain_r_kernel; l<=i+stain_r_kernel; l++)
            {
                for (int m=j-stain_r_kernel; m<=j+stain_r_kernel; m++)
                {
                    cnt++;
                    mean_t+= enhace.at<float>(l, m);
                }
            }
            float mean_ex = mean_t-enhace.at<float>(i, j);
            float mean_kernel =mean_ex/(cnt-1);

            //OUTER
            cnt = 0;
            mean_t = 0.0;
            for (int l=i-stain_r_outer; l<= i+stain_r_outer; l++)
            {
                for (int m=j-stain_r_outer; m<=j+stain_r_outer; m++)
                {
                    cnt+=1;
                    mean_t+=enhace.at<float>(l, m);
                }
            }
            mean_ex = mean_t-enhace.at<float>(i, j);
            float mean_outer =mean_ex/(cnt-1);

            ratio_blemish[i*ldma_w+j]=std::abs(mean_outer-mean_kernel)/mean_outer;
            if(ratio_blemish[i*ldma_w+j]>stain_spec)
            {
                LDMA_stain_cnt++;
                ldma_stain_binary.at<uchar>(i, j) = 255;
            }
        }
    }
    if(saveimage)
    {
        cv::imwrite(QString("./LDMA_Image/stain_binary.bmp").toStdString().c_str(),ldma_stain_binary);

        cv::Mat ratio_stain_map = cv::Mat(ldma_h,ldma_w,CV_32FC1);
        for(int i=0; i<ldma_h; i++)
        {
            float *p_row_pog = ratio_stain_map.ptr<float>(i);
            for(int j=0; j<ldma_w; j++)
                p_row_pog[j] =ratio_blemish[ldma_w*i+j]*1000;
        }
        ratio_stain_map.convertTo(ratio_stain_map,CV_16U);
        cv::imwrite(QString("./LDMA_Image/stain_ratio_map.png").toStdString().c_str(),ratio_stain_map);
    }

    all_count= dark_bar_cnt + GFDA_pog_cnt + GFDA_scratch_cnt + GFDA_spot_cnt  + LEDA_spot_cnt + LDMA_stain_cnt;
    return all_count;
}






//int AlgorithmFuncs::SaveTestMat(QString file_name,QString image_name,cv::Mat mat_)
//{
//    QString path=QString("./CPAlgorithmImages/%1").arg(file_name);
//    QDir dir(path);
//    if(!dir.exists())
//    {
//        QDir dir_;
//        dir_.mkpath(path);
//    }
//    path+="/"+image_name;

//    if(path.endsWith("csv"))
//    {
//        FILE* fp = fopen(path.toStdString().c_str(),"wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
//        for (int y = 0; y < mat_.rows; y++)
//        {
//            for (int x = 0; x < mat_.cols; x++)
//            {
//                float point = mat_.at<float>(y, x);
//                fprintf(fp, "%4.2f,", point);
//            }
//            fprintf(fp, "\n");
//        }
//        fclose(fp);
//    }
//    else if(path.endsWith("bmp"))
//    {
//        cv::Mat mat_8u=mat_.clone();
//        mat_8u.convertTo(mat_8u,CV_8UC1);

//        static QVector<QRgb>  sColorTable;
//        if ( sColorTable.isEmpty() )
//        {
//            sColorTable.resize( 256 );
//            for ( int i = 0; i < 256; ++i )
//                sColorTable[i] = qRgb( i, i, i );
//        }
//        QImage bmp_save( mat_8u.data,mat_8u.cols, mat_8u.rows,static_cast<int>(mat_8u.step),QImage::Format_Indexed8 );
//        bmp_save.setColorTable( sColorTable );
//        bmp_save.save(path,"BMP",100);
//    }
////    else if(path.endsWith("png"))
////    {
////        QImage img_(mat_.data,mat_.cols,mat_.rows,mat_.cols * 2,QImage::Format_Grayscale16);
////        img_.save(path,"PNG",100);
////    }

//    return 1;
//}




int AlgorithmFuncs::Darkbar_Binary(cv::Mat diff_map, float darkbar_spec,int saveimage, int angle,std::string& msg)
{
    int darkbar_cnt=0;
    cv::Mat diff_diff_png = cv::Mat::zeros(diff_map.rows,diff_map.cols,CV_32FC1);
    for (int y = 0; y < diff_map.rows; y++)
    {
        for (int x = 0; x < diff_map.cols; x++)
            diff_diff_png.at<float>(y, x)=fabs(diff_map.at<float>(y, x)*100) ;
    }

    int skip_size= (angle==0||angle==90)? 3:2;
    cv::Mat diff_diff_binary=cv::Mat::zeros(diff_map.rows,diff_map.cols,CV_8U);
    for (int y = skip_size; y < diff_map.rows-skip_size; y++)
    {
        for (int x = skip_size; x < diff_map.cols-skip_size; x++)
        {
            if(diff_diff_png.at<float>(y, x)>darkbar_spec)
                diff_diff_binary.at<uchar>(y, x)=255;
        }
    }

    vector<Vec4i> darkbar_lines;
    HoughLinesP(diff_diff_binary, darkbar_lines, 1, CV_PI/180, 25, 30, 2);
    vector<Vec4i> darkbar_lines_2;
    HoughLinesP(diff_diff_binary, darkbar_lines_2, 1, CV_PI/90, 35, 40, 4);
    darkbar_lines.insert(darkbar_lines.end(), darkbar_lines_2.begin(), darkbar_lines_2.end());

    int angle_temp=30;
    int right_angle=angle+90;
    if(right_angle>=180)
        right_angle-=180;
    double radian_max=cos((right_angle-angle_temp)*CV_PI/180);
    double radian_min=cos((right_angle+angle_temp)*CV_PI/180);
    if(CV_PI/(right_angle-angle_temp)<0)
        radian_max=1;

//    QString msg+=QString("angle:%1 right_angle:%2 radian_max:%3 radian_min:%4 \n")
//            .arg(angle).arg(right_angle).arg(radian_max).arg(radian_min);

    vector<Vec4i> darkbar_lines_;
    for(int i=0;i!=darkbar_lines.size();++i)
    {
        Vec4i line_ = darkbar_lines[i];
        int w=abs(line_[0]-line_[2]);
        int h=abs(line_[1]-line_[3]);
        double len=sqrt(h*h+w*w);

        double line_cos=0;
        if(line_[1]-line_[3]<0)
            line_cos=(line_[0]-line_[2])/len;
        else
            line_cos=(line_[2]-line_[0])/len;

//        msg+=QString("line_0:%1 line_1:%2 line_2:%3 line_3:%4 line_cos:%5\n")
//                .arg(line_[0]).arg(line_[1]).arg(line_[2]).arg(line_[3]).arg(line_cos);

        if(line_cos>=radian_min && line_cos<=radian_max)
            darkbar_lines_.push_back(line_);
    }
    darkbar_cnt=darkbar_lines_.size();


    if(saveimage)
    {
        cv::imwrite(QString("./DarkBar_Image/%1_diff_diff_binary.bmp").arg(angle).toStdString().c_str(),diff_diff_binary);
        diff_diff_png.convertTo(diff_diff_png,CV_16U);
        cv::imwrite(QString("./DarkBar_Image/%1_diff_diff.png").arg(angle).toStdString().c_str(),diff_diff_png);

        cv::Mat houghline=cv::Mat::zeros(diff_diff_binary.rows,diff_diff_binary.cols,CV_8U);
        for( size_t i = 0; i < darkbar_lines.size(); i++ )
        {
            Vec4i l = darkbar_lines[i];
            line( houghline, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 1);
        }
        cv::imwrite(QString("./DarkBar_Image/%1_hougt_line.bmp").arg(angle).toStdString().c_str(),houghline);

        cv::Mat houghline_=cv::Mat::zeros(diff_diff_binary.rows,diff_diff_binary.cols,CV_8U);
        for( size_t i = 0; i < darkbar_lines_.size(); i++ )
        {
            Vec4i l = darkbar_lines_[i];
            line( houghline_, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 1);
        }
        cv::imwrite(QString("./DarkBar_Image/%1_hougt_line_.bmp").arg(angle).toStdString().c_str(),houghline_);
    }

    return darkbar_cnt;
}


int AlgorithmFuncs::ConnectedDomainDetect(cv::Mat src,int cluster_r, int limit_size, QVector<QSet<int> >& cluster_group )
{
    int clus_cnt=0;
    int w_s = src.cols;
    int h_s = src.rows;
    cluster_group.clear();

    QVector<int> ClusterMap_flag(w_s*h_s,0);
    for(int i=cluster_r; i<h_s-cluster_r; i++)
    {
        for(int j=cluster_r; j<w_s-cluster_r; j++)
        {
            if( src.at<uchar>(i, j)==255 && ClusterMap_flag[w_s*i+j]==0 )
            {
                QSet<int> blemish_group;
                std::stack<QPoint> posList;

                blemish_group.insert(w_s*i+j);
                posList.push(QPoint(j,i));
                ClusterMap_flag[w_s*i+j]=1;

                while(posList.size() > 0)
                {
                    int x_=posList.top().x();
                    int y_=posList.top().y();
                    posList.pop();

                    for(int m=-cluster_r;m<=cluster_r;++m)
                    {
                        for(int n=-cluster_r;n<=cluster_r;++n)
                        {
                            int p_x=x_+n;
                            int p_y=y_+m;
                            if( p_x <0 || p_y <0 || p_x >=w_s  || p_y >=h_s)
                                continue;
                           if( src.at<uchar>(p_y,p_x)==255 && ClusterMap_flag[w_s*p_y+p_x]==0)
                            {
                                blemish_group.insert(w_s*p_y+p_x);
                                posList.push(QPoint(p_x,p_y));
                                ClusterMap_flag[w_s*p_y+p_x]=1;
                            }
                        }
                    }

                }
                cluster_group.push_back(blemish_group);
            }
        }
    }
    for(int i=0;i!=cluster_group.size();++i)
    {
        if(cluster_group[i].size()>=limit_size)
            clus_cnt++;
    }
    return clus_cnt;

}


int AlgorithmFuncs::find_pog_normal_bright(cv::Mat srcImage, int crop, int r1, int r2, double spec_p, double spec_n,
                                           double scratch_p, double scratch_n, int resize_w, int resize_h, double limit_size,
                                           double lsc_ratio, double *max, int *max_pos_index, double *min, int *min_pos_index,
                                           int blemish_skip, int r_kernel, int r_outer, double blemish_spec, int saveimage)
{
    //----------读取原始图像----------------
    cv::Mat srcImage_32;
    srcImage.convertTo(srcImage_32,CV_32FC1);


    //----------亮度提取----------------
    cv::Mat grayImage(srcImage_32.size(),CV_32FC1);
    double s=1.0/16;
    cv::Mat kern = (cv::Mat_<double>(3,3) << s,2*s,s,2*s,4*s,2*s,s,2*s,s);
    cv::filter2D(srcImage_32,grayImage,srcImage_32.depth(),kern);

    if(saveimage)
        cv::imwrite(QString("./grayImage_image.bmp").toStdString().c_str(),grayImage);

    //----------均值滤波----------------
    cv::Mat blur_img;
    cv::blur(grayImage,blur_img,cv::Size(5,5));


    //-----------中值去噪--------------
    cv::Mat image_mblur;
    cv::medianBlur(blur_img,image_mblur,3);


    //----------图像压缩----------------
    cv::Mat src_small;
    cv::resize(image_mblur,src_small,cv::Size(resize_w,resize_h),0,0,cv::INTER_LINEAR);

    if(saveimage)
        cv::imwrite(QString("./small_image.bmp").toStdString().c_str(),src_small);


    //----------LSC----------------
    cv::Mat  lsc_img = src_small;


    double mean_corner =0;
    for(int i=0; i<30; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        for(int j=0; j<40; j++)
        {
            mean_corner += p_row_small[j];

        }
    }
    double mean_center =0;
    for(int i=src_small.rows/2-15; i<src_small.rows/2+15; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        for(int j=src_small.cols/2-20; j<src_small.cols/2+20; j++)
        {
            mean_center += p_row_small[j];

        }
    }

    double delta = (mean_center/mean_corner -1)*lsc_ratio;

    double factor = delta/((resize_w/2)*(resize_w/2) + (resize_h/2)*(resize_h/2));

    for(int i=0; i<src_small.rows; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        float *p_row_lsc = lsc_img.ptr<float>(i);
        int y = std::abs(i-resize_h/2);
        for(int j=0; j<src_small.cols; j++)
        {
            int x = std::abs(j-resize_w/2);
            p_row_lsc[j] = p_row_small[j] * ((x*x + y*y) * factor + 1);

        }
    }


    if(saveimage)
        cv::imwrite(QString("./lsc_img_image.bmp").toStdString().c_str(),lsc_img);


    //----------对比度增强----------------
    cv::Mat  enhace;

    double  minV, maxV;
    int  minId[2] = {}, maxId[2] = {};	// minnimum Index, maximum Index

    cv::minMaxIdx(lsc_img, &minV, &maxV, minId, maxId);

    double max_ds = 250;
    double min_ds = 50;

    enhace = (lsc_img - minV)*(max_ds-min_ds)/(maxV-minV)+min_ds;

    if(saveimage)
        cv::imwrite(QString("./enhace_image.bmp").toStdString().c_str(),enhace);


    int counter =0;

    //=================POG 斑缺陷检测=============================

    cv::Mat pog_img = enhace;
    //----------均值滤波A----------------
    cv::Mat blur_A,small_tmp_A,blur_A_tmp;
    cv::copyMakeBorder(pog_img, small_tmp_A, r1/2, r1/2, r1/2, r1/2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_A,blur_A_tmp,cv::Size(r1,r1));
    blur_A = blur_A_tmp(cv::Rect(r1/2,r1/2,pog_img.cols,pog_img.rows));

    //----------均值滤波B----------------
    cv::Mat blur_B,small_tmp_B,blur_B_tmp;
    cv::copyMakeBorder(pog_img, small_tmp_B, r2/2, r2/2, r2/2, r2/2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_B,blur_B_tmp,cv::Size(r2,r2));
    blur_B = blur_B_tmp(cv::Rect(r2/2,r2/2,pog_img.cols,pog_img.rows));

    //--------均值滤波AB差--------------
    cv::Mat blur_diff_pog = blur_A -blur_B;
    cv::Mat blur_diff_crop_pog = blur_diff_pog(cv::Rect(r1*1.5,r1*1.5,pog_img.cols-r1*3,pog_img.rows-r1*3));

    //-----------中值去噪--------------
    cv::Mat image_blur_pog;
    cv::medianBlur(blur_diff_crop_pog,image_blur_pog,3);

    cv::minMaxIdx(image_blur_pog,min,max,min_pos_index,max_pos_index);

    if(saveimage)
    {
        FILE* fp = fopen("POG_Diff_Data.csv","wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
        for (int y = 0; y < image_blur_pog.rows; y++)
        {
            for (int x = 0; x < image_blur_pog.cols; x++)
            {
                float point = image_blur_pog.at<float>(y, x);
                fprintf(fp, "%4.2f,", point);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
    }


    //------------二值化---------------
    cv::Mat binary_pog = cv::Mat(image_blur_pog.rows,image_blur_pog.cols,CV_8U);
    for(int i=0; i<image_blur_pog.rows; i++)
    {
        uchar *p_row_binary_pog = binary_pog.ptr<uchar>(i);
        float *p_row_blur = image_blur_pog.ptr<float>(i);

        for(int j=0; j<image_blur_pog.cols; j++)
        {
            if(p_row_blur[j]>spec_p || p_row_blur[j]<spec_n)
                p_row_binary_pog[j] =255;
            else
                p_row_binary_pog[j] =0;
        }
    }

    if(saveimage)
        cv::imwrite(QString("./pog_binary_image.bmp").toStdString().c_str(),binary_pog);

    cv::Mat_<uchar>::iterator it =binary_pog.begin<uchar>();
    cv::Mat_<uchar>::iterator itend = binary_pog.end<uchar>();

    for (;it!=itend;it++)
    {
        if((*it)>0) counter++;
    }

    //======================Scratch 划痕检测=============================

    cv::Mat scratch_img = enhace;
    //----------滤波A----------------
    cv::Mat blur_s_A,blur_s_A_tmp;
    cv::blur(scratch_img,blur_s_A_tmp,cv::Size(9,9));
    cv::medianBlur(blur_s_A_tmp,blur_s_A,5);


    //----------均值滤波B----------------
    cv::Mat blur_s_B,blur_s_B_tmp;
    cv::blur(scratch_img,blur_s_B_tmp,cv::Size(3,3));
    cv::medianBlur(blur_s_B_tmp,blur_s_B,3);

    //--------均值滤波AB差--------------
    cv::Mat blur_s_diff_scratch = blur_s_A -blur_s_B;
    cv::Mat blur_s_diff_crop_scratch = blur_s_diff_scratch(cv::Rect(crop,crop,scratch_img.cols-2*crop,scratch_img.rows-2*crop));

    //-----------中值去噪--------------
    cv::Mat image_blur_s_scratch;
    cv::medianBlur(blur_s_diff_crop_scratch,image_blur_s_scratch,1);

    if(saveimage)
    {
        FILE* fp = fopen("scratch_Diff_Data.csv","wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
        for (int y = 0; y < image_blur_s_scratch.rows; y++)
        {
            for (int x = 0; x < image_blur_s_scratch.cols; x++)
            {
                float point = image_blur_s_scratch.at<float>(y, x);
                fprintf(fp, "%4.2f,", point);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
    }


    //------------二值化---------------
    cv::Mat binary_scratch = cv::Mat(image_blur_s_scratch.rows,image_blur_s_scratch.cols,CV_8U);
    for(int i=0; i<image_blur_s_scratch.rows; i++)
    {
        uchar *p_row_binary_scratch = binary_scratch.ptr<uchar>(i);
        float *p_row_blur = image_blur_s_scratch.ptr<float>(i);

        for(int j=0; j<image_blur_s_scratch.cols; j++)
        {
            if(p_row_blur[j]>scratch_p || p_row_blur[j]<scratch_n)
                p_row_binary_scratch[j] =255;
            else
                p_row_binary_scratch[j] =0;
        }
    }

    if(saveimage)
        cv::imwrite(QString("./scratch_binary_image.bmp").toStdString().c_str(),binary_scratch);


    int w_s= binary_scratch.cols;
    int h_s =binary_scratch.rows;

    uchar *g_binary_map = new uchar[w_s*h_s];

    for(int i=0; i<h_s; i++)
    {
        uchar *p_row_binary_scratch = binary_scratch.ptr<uchar>(i);

        for(int j=0; j<w_s; j++)
        {
            g_binary_map[i*w_s + j] = p_row_binary_scratch[j];
        }
    }

    std::vector<AlgorithmFuncs::MPDInf> clusters;
    defectCounter (g_binary_map,binary_scratch.cols,binary_scratch.rows,128,clusters);
    BOOST_FOREACH(const AlgorithmFuncs::MPDInf &cluster, clusters)
    {
        if (cluster.w_>limit_size ||
                cluster.h_>limit_size)
        {
            counter++;
        }
    }


    //================Blemish 污点检测======================

    int skip=blemish_skip;
    double spec_b=blemish_spec;
    double mean_kernel;
    double mean_outer;
    int row_start_inner =0;
    int row_end_inner =0;
    int col_start_inner=0;
    int col_end_inner =0;
    int row_start_outer =0;
    int row_end_outer =0;
    int col_start_outer=0;
    int col_end_outer =0;

    int w = enhace.cols;
    int h = enhace.rows;
    float *data= new float[w*h];

    for(int i=0; i<h; i++)
    {
        float *p_row = enhace.ptr<float>(i);

        for(int j=0; j<w; j++)
        {
            data[i*w+j] = p_row[j];
        }
    }


    uchar *binary_map = new uchar [w*h];
    memset(binary_map,0,w*h);

    for (int i=skip; i<h-skip; i++)
    {
        for (int j=skip; j<w-skip; j++)
        {
            mean_kernel=0;
            mean_outer=0;
//-------------------Kernel---------------------
            if(true)
            {
                double mean_t = 0.0;
                int cnt = 0;

                row_start_inner = i-r_kernel;
                row_end_inner = i+r_kernel;
                col_start_inner = j-r_kernel;
                col_end_inner = j+r_kernel;

                for (int l=row_start_inner; l<=row_end_inner; l++)
                {
                    for (int m=col_start_inner; m<=col_end_inner; m++)
                    {
                            cnt+=1;
                            mean_t+=(double)data[w*l+m];
                    }
                }
                double mean_ex = mean_t-data[i*w+j];
                mean_ex/=(cnt-1);
                mean_kernel =mean_ex;
            }

//-------------------OUTER---------------------
            if(true)
            {
                double mean_t = 0.0;
                int cnt = 0;

                row_start_outer = i-r_outer;
                row_end_outer = i+r_outer;

                col_start_outer = j-r_outer;
                col_end_outer = j+r_outer;

                for (int l=row_start_outer; l<=row_end_outer; l++)
                {
                    for (int m=col_start_outer; m<=col_end_outer; m++)
                    {
                            cnt+=1;
                            mean_t+=(double)data[w*l+m];
                    }
                }
                double mean_ex = mean_t-data[i*w+j];
                mean_ex/=(cnt-1);
                mean_outer =mean_ex;
            }

//----------------------------------------------------
            if(std::abs(mean_outer-mean_kernel)>mean_outer*spec_b)
                binary_map[i*w+j] = 255;
            else
                binary_map[i*w+j] = 0;

        }
    }

    //------------二值化---------------
    cv::Mat binary_blemish = cv::Mat(h,w,CV_8U,binary_map);

    it =binary_blemish.begin<uchar>();
    itend = binary_blemish.end<uchar>();
    for (;it!=itend;it++)
    {
        if((*it)>0) counter++;
    }
    if(saveimage)
        cv::imwrite(QString("./blemish_binary_image.bmp").toStdString().c_str(),binary_blemish);


    delete []g_binary_map;
    delete []binary_map;
    delete []data;
    return counter;

}




int AlgorithmFuncs::find_pog(cv::Mat srcImage, int r1, int r2, int resize_w, int resize_h, double spec_p, double spec_n,
                             int saveimage, double *max, int *max_pos_index, double *min, int *min_pos_index, Spec_Mode mode)
{
    //----------读取原始图像----------------
    cv::Mat srcImage_32;
    srcImage.convertTo(srcImage_32,CV_32FC1);

    //-----------中值去噪--------------
    cv::Mat image_mblur;
    cv::medianBlur(srcImage_32,image_mblur,3);

    //----------亮度提取----------------
    cv::Mat grayImage(image_mblur.size(),CV_32FC1);
    double s=1.0/16;
    cv::Mat kern = (cv::Mat_<double>(3,3) << s,2*s,s,2*s,4*s,2*s,s,2*s,s);
    cv::filter2D(image_mblur,grayImage,image_mblur.depth(),kern);

    //----------均值滤波----------------
    cv::Mat blur_mat;
    cv::blur(grayImage,blur_mat,cv::Size(7,7));

    //----------对比度增强----------------
    cv::Mat  enhance;
    if (mode ==Ratio)
    {
        double  minV, maxV;
        int  minId[2] = {}, maxId[2] = {};	// minnimum Index, maximum Index

        cv::minMaxIdx(blur_mat, &minV, &maxV, minId, maxId);

        double max_ds = 250;
        double min_ds = 5;

        enhance = (blur_mat - minV)*(max_ds-min_ds)/(maxV-minV)+min_ds;
    }
    else
    {
      enhance=grayImage;
    }
    if(saveimage)
        cv::imwrite(QString("./pog_enhance_image.bmp").toStdString().c_str(),enhance);

    //----------图像压缩----------------
    cv::Mat src_small;
    cv::resize(enhance,src_small,cv::Size(resize_w,resize_h),0,0,cv::INTER_LINEAR);

    //----------均值滤波A----------------
    cv::Mat blur_A,small_tmp_A,blur_A_tmp;
    cv::copyMakeBorder(src_small, small_tmp_A, r1/2, r1/2, r1/2, r1/2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_A,blur_A_tmp,cv::Size(r1,r1));
    blur_A = blur_A_tmp(cv::Rect(r1/2,r1/2,src_small.cols,src_small.rows));

    //----------均值滤波B----------------
    cv::Mat blur_B,small_tmp_B,blur_B_tmp;
    cv::copyMakeBorder(src_small, small_tmp_B, r2/2, r2/2, r2/2, r2/2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_B,blur_B_tmp,cv::Size(r2,r2));
    blur_B = blur_B_tmp(cv::Rect(r2/2,r2/2,src_small.cols,src_small.rows));

    //--------均值滤波AB差--------------
    cv::Mat blur_diff = blur_A -blur_B;
    cv::Mat blur_diff_crop = blur_diff(cv::Rect(r1/2,r1/2,src_small.cols-r1,src_small.rows-r1));

    //-----------中值去噪--------------
    cv::Mat image_blur;
    cv::medianBlur(blur_diff_crop,image_blur,3);

    cv::minMaxIdx(image_blur,min,max,min_pos_index,max_pos_index);

    //------------二值化---------------
    cv::Mat binary = cv::Mat(image_blur.rows,image_blur.cols,CV_8U);
    for(int i=0; i<image_blur.rows; i++)
    {
        uchar *p_row_binary = binary.ptr<uchar>(i);
        float *p_row_blur = image_blur.ptr<float>(i);

        for(int j=0; j<image_blur.cols; j++)
        {
            if(p_row_blur[j]>spec_p || p_row_blur[j]<spec_n)
                p_row_binary[j] =255;
            else
                p_row_binary[j] =0;
        }
    }
    //--------------------------------

    cv::Mat_<uchar>::iterator it =binary.begin<uchar>();
    cv::Mat_<uchar>::iterator itend = binary.end<uchar>();
    int counter =0;
    for (;it!=itend;it++)
    {
        if((*it)>0) counter++;
    }
    if(saveimage)
        cv::imwrite(QString("./pog_binary_image.bmp").toStdString().c_str(),binary);
    return counter;

}



int AlgorithmFuncs::blur_diff_binary(cv::Mat srcImage, int r1, int r2, double spec, int saveimage)
{
    //----------读取原始图像----------------
    cv::Mat srcImage_32;
    srcImage.convertTo(srcImage_32,CV_32FC1);

    //-----------中值去噪--------------
    cv::Mat image_mblur;
    cv::medianBlur(srcImage_32,image_mblur,3);

    //----------亮度提取----------------
    cv::Mat grayImage(image_mblur.size(),CV_32FC1);
    double s=1.0/16;
    cv::Mat kern = (cv::Mat_<double>(3,3) << s,2*s,s,2*s,4*s,2*s,s,2*s,s);
    cv::filter2D(image_mblur,grayImage,image_mblur.depth(),kern);

    //----------均值滤波----------------
    cv::Mat blur_mat;
    cv::blur(grayImage,blur_mat,cv::Size(7,7));

    //----------对比度增强----------------
    cv::Mat  enhance;
    double  minV, maxV;
    int  minId[2] = {}, maxId[2] = {};	// minnimum Index, maximum Index

    cv::minMaxIdx(blur_mat, &minV, &maxV, minId, maxId);

    double max_ds = 250;
    double min_ds = 5;

    enhance = (blur_mat - minV)*(max_ds-min_ds)/(maxV-minV)+min_ds;

    //----------图像压缩----------------
    cv::Mat src_small;
    cv::resize(enhance,src_small,cv::Size(400,300),0,0,cv::INTER_LINEAR);

    //----------均值滤波A----------------
    cv::Mat blur_A,small_tmp_A,blur_A_tmp;
    cv::copyMakeBorder(src_small, small_tmp_A, r1/2, r1/2, r1/2, r1/2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_A,blur_A_tmp,cv::Size(r1,r1));
    blur_A = blur_A_tmp(cv::Rect(r1/2,r1/2,src_small.cols,src_small.rows));

    //----------均值滤波B----------------
    cv::Mat blur_B,small_tmp_B,blur_B_tmp;
    cv::copyMakeBorder(src_small, small_tmp_B, r2/2, r2/2, r2/2, r2/2, cv::BORDER_REFLECT);
    cv::blur(small_tmp_B,blur_B_tmp,cv::Size(r2,r2));
    blur_B = blur_B_tmp(cv::Rect(r2/2,r2/2,src_small.cols,src_small.rows));

    //--------均值滤波AB差--------------
    cv::Mat blur_diff = blur_A -blur_B;
    cv::Mat blur_diff_crop = blur_diff(cv::Rect(r1/2,r1/2,src_small.cols-r1,src_small.rows-r1));

    //-----------中值去噪--------------
    cv::Mat image_blur;
    cv::medianBlur(blur_diff_crop,image_blur,3);

    //------------二值化---------------
    cv::Mat binary;
    cv::threshold(image_blur,binary,spec,255,cv::THRESH_BINARY);
    cv::Mat tmp;
    binary.convertTo(tmp,CV_8U);
    cv::Mat_<uchar>::iterator it =tmp.begin<uchar>();
    cv::Mat_<uchar>::iterator itend = tmp.end<uchar>();
    int counter =0;
    for (;it!=itend;it++)
    {
        if((*it)>0) counter++;
    }
    if(saveimage)
        cv::imwrite(QString("./binary_image.bmp").toStdString().c_str(),tmp);
    return counter;

}

double AlgorithmFuncs::GetDatasStd_Qvector(QVector<double> &datas)
{
    if (datas.size ()==0)
        return 0;
    double mean_t = 0;
    for (int i=0; i<datas.size (); ++i)
        mean_t+=(double)datas.at (i);
    mean_t/=datas.size ();
    double std_t = 0.0;
    for (int i=0; i<datas.size (); ++i)
    {
        std_t+=(double)((mean_t-datas.at (i))*(mean_t-datas.at (i)));
    }
    std_t/=datas.size () -1;
    return std::sqrt (std_t);
}

double AlgorithmFuncs::GetDatasStdandMean(QVector <int> &datas, double &mean)
{
    if (datas.size ()==0)
        return 0;
    double mean_t = 0;
    for (int i=0; i<datas.size (); ++i)
        mean_t+=(double)datas.at (i);
    mean_t/=datas.size ();
    mean=mean_t;
    double std_t = 0.0;
    for (int i=0; i<datas.size (); ++i)
    {
        std_t+=(double)((mean_t-datas.at (i))*(mean_t-datas.at (i)));
    }
    std_t/=datas.size ()-1;
    return std::sqrt (std_t);
}


double AlgorithmFuncs::Get_Double_DatasStdandMean(QVector <double> &datas, double &mean)
{
    if (datas.size ()==0)
        return 0;
    double mean_t = 0;
    for (int i=0; i<datas.size (); ++i)
        mean_t+=(double)datas.at (i);
    mean_t/=datas.size ();
    mean=mean_t;
    double std_t = 0.0;
    for (int i=0; i<datas.size (); ++i)
    {
        std_t+=(double)((mean_t-datas.at (i))*(mean_t-datas.at (i)));
    }
    std_t/=datas.size ()-1;
    return std::sqrt (std_t);
}


double AlgorithmFuncs::TestRelativeIlumination (uchar *pData, int w,
                                                int h, int w_win,
                                                int h_win)
{
    double ROI_2 = 0.0;
    double ROI_3 = 0.0;
    double ROI_4 = 0.0;
    double ROI_5 = 0.0;
    double ROI_6 = 0.0;
    int cnt = 0;

    for (int i=0; i<h_win; ++i)
    {
        for (int j=0; j<w_win; ++j)
        {
            ROI_3+=pData[w*i+j];
            cnt+=1;
        }
    }
    ROI_3/=cnt;

    cnt = 0;
    for (int i=h-h_win; i<h; ++i)
    {
        for (int j=0; j<w_win; ++j)
        {
            ROI_4+=pData[w*i+j];
            cnt+=1;
        }
    }
    ROI_4/=cnt;

    cnt = 0;
    for (int i=0; i<h_win; ++i)
    {
        for (int j=w-w_win; j<w; ++j)
        {
            ROI_5+=pData[w*i+j];
            cnt+=1;
        }
    }
    ROI_5/=cnt;

    cnt = 0;
    for (int i=h-h_win; i<h; ++i)
    {
        for (int j=w-w_win; j<w; ++j)
        {
            ROI_6+=pData[w*i+j];
            cnt+=1;
        }
    }
    ROI_6/=cnt;

    cnt = 0;
    for (int i=h/2-h_win/2; i<h/2-h_win/2+h_win; ++i)
    {
        for (int j=w/2-w_win/2; j<w/2-w_win/2+w_win; ++j)
        {
            ROI_2+=pData[w*i+j];
            cnt+=1;
        }
    }
    ROI_2/=cnt;
    if (ROI_2<0.001)
        ROI_2 = 0.001;

    double darkest = (ROI_3 + ROI_4 + ROI_5 + ROI_6)/4;
    if (ROI_2<darkest/10)
        return 1000;
    else if (ROI_2>darkest*10)
        return 10;
    else
        return 100*darkest/ROI_2;
}

void AlgorithmFuncs::TestColorShading (uchar *pR, uchar *pG, uchar *pB,
                                       int w, int h, int w_win,
                                       int h_win,
                                       std::vector<double> &ratios)
{
    double R_2 = 0.0;
    double G_2 = 0.0;
    double B_2 = 0.0;
    double R_3 = 0.0;
    double G_3 = 0.0;
    double B_3 = 0.0;
    double R_4 = 0.0;
    double G_4 = 0.0;
    double B_4 = 0.0;
    double R_5 = 0.0;
    double G_5 = 0.0;
    double B_5 = 0.0;
    double R_6 = 0.0;
    double G_6 = 0.0;
    double B_6 = 0.0;
    int cnt = 0;
    for (int i=0; i<h_win; ++i)
    {
        for (int j=0; j<w_win; ++j)
        {
            R_3+=pR[w*i+j];
            G_3+=pG[w*i+j];
            B_3+=pB[w*i+j];
            cnt+=1;
        }
    }
    R_3/=cnt;G_3/=cnt;B_3/=cnt;cnt = 0;
    //---------------------------------
    for (int i=h-h_win; i<h; ++i)
    {
        for (int j=0; j<w_win; ++j)
        {
            R_4+=pR[w*i+j];
            G_4+=pG[w*i+j];
            B_4+=pB[w*i+j];
            cnt+=1;
        }
    }
    R_4/=cnt;G_4/=cnt;B_4/=cnt;cnt = 0;
    //----------------------------------
    for (int i=0; i<h_win; ++i)
    {
        for (int j=w-w_win; j<w; ++j)
        {
            R_5+=pR[w*i+j];
            G_5+=pG[w*i+j];
            B_5+=pB[w*i+j];
            cnt+=1;
        }
    }
    R_5/=cnt;G_5/=cnt;B_5/=cnt;cnt = 0;
    //---------------------------------
    for (int i=h-h_win; i<h; ++i)
    {
        for (int j=w-w_win; j<w; ++j)
        {
            R_6+=pR[w*i+j];
            G_6+=pG[w*i+j];
            B_6+=pB[w*i+j];
            cnt+=1;
        }
    }
    R_6/=cnt;G_6/=cnt;B_6/=cnt;cnt = 0;
    //----------------------------------
    for (int i=h/2-h_win/2; i<h/2-h_win/2+h_win; ++i)
    {
        for (int j=w/2-w_win/2; j<w/2-w_win/2+w_win; ++j)
        {
            R_2+=pR[w*i+j];
            G_2+=pG[w*i+j];
            B_2+=pB[w*i+j];
            cnt+=1;
        }
    }
    R_2/=cnt;G_2/=cnt;B_2/=cnt;cnt = 0;
    //-----------------------------------
    G_3+=0.01;
    R_6+=0.01;
    G_6+=0.01;
    B_6+=0.01;
    G_6+=0.01;
    R_4+=0.01;
    G_4+=0.01;
    B_4+=0.01;
    G_5+=0.01;
    G_2+=0.01;

    double r_g_2 = R_2/G_2*100;
    double b_g_2 = B_2/G_2*100;

    double R_G_1 = R_3/G_3*G_6/R_6*100;
    double B_G_1 = B_3/G_3*G_6/B_6*100;
    double R_G_2 = R_5/G_5*G_4/R_4*100;
    double B_G_2 = B_5/G_5*G_4/B_4*100;
    double ave_R_G = (R_3/G_3+R_4/G_4+R_5/G_5+R_6/G_6)/4;
    double ave_B_G = (B_3/G_3+B_4/G_4+B_5/G_5+B_6/G_6)/4;
    double R_G_3 = R_2/G_2/ave_R_G*100;
    double B_G_3 = B_2/G_2/ave_B_G*100;
    ratios.push_back (R_G_1);
    ratios.push_back (B_G_1);
    ratios.push_back (R_G_2);
    ratios.push_back (B_G_2);
    ratios.push_back (R_G_3);
    ratios.push_back (B_G_3);
    ratios.push_back (r_g_2);
    ratios.push_back (b_g_2);
}



int AlgorithmFuncs::getClusterInf(cv::Mat&src,std::vector<MClusterInf>&result,\
                                    const int &sizeLimit,const int & sizeMax)
{
    for (int i(0); i<src.rows; ++i)
    {
        uchar*data(src.ptr<uchar>(i));
        for (int j(0); j<src.cols; ++j)
        {
            if(*(data+j)==255)
            {
                std::vector<std::pair<int,int> > list_pos;
                list_pos.push_back (std::pair<int,int>(i,j));
                *(data+j)=0;
                int row_end (i);
                int col_start(j);
                int col_end(j);
                int cnt_pix(0);
                while(list_pos.size ()>0)
                {
                    std::pair<int,int> pos(list_pos.back ());
                    list_pos.pop_back ();
                    cnt_pix+=1;
                    row_end = (pos.first>row_end)?pos.first:row_end;
                    col_start = (pos.second<col_start)?pos.second:col_start;
                    col_end = (pos.second>col_end)?pos.second:col_end;
                    for (int l(pos.first-1); l<=pos.first+1; ++l)
                    {
                        uchar*d(src.ptr<uchar>(l));
                        for (int m(pos.second-1); m<=pos.second+1; ++m)
                        {
                            if (l>=0 && l<src.rows && \
                                    m>=0 && m<src.cols&& \
                                    *(d+m)==255)
                            {
                                list_pos.push_back (std::pair<int,int>(l,m));//存入周围为255的点
                                *(d+m)=0;
                            }
                        }
                    }
                }
                const int w(col_end-col_start+1);
                const int h(row_end-i+1);
                if((max(w,h)>sizeLimit)&&(max(w,h)<= sizeMax))//pd sizelimit==1
                {
                    int code(9);
                    if (w == 1 && h== 1)
                    {
                        code = 1;
                    }
                    else if (w*h == 2)
                    {
                        code = 2;
                    }
                    else if (w == 2 && h== 2)
                    {
                        code= cnt_pix+1;
                    }
                    result.push_back (MClusterInf(col_start,i,w,h,cnt_pix,code));
                }
            }
        }
    }
    return result.size();
}


int AlgorithmFuncs::FindPkG_WF(cv::Mat &image, const MPkgParam &param,std::string &msg)

{
    QElapsedTimer timer_blemish;
    timer_blemish.start();
    const char sobel1[9]={-1, 0, 1,
                          -2, 0, 2,
                          -1, 0, 1};
    const char sobel2[9]={1, 2, 1,
                          0, 0, 0,
                          -1,-2,-1};
    const char sobel3[9]={0, 1, 2,
                          -1, 0, 1,
                          -2,-1, 0};
    const char sobel4[9]={-2,-1, 0,
                          -1, 0, 1,
                          0, 1, 2};
    const int &width(image.cols);
    const int &height(image.rows);
    cv::Mat map(cv::Mat::zeros(image.size(),CV_8UC1));

    cv::Mat data_map(cv::Mat::zeros(image.size(),CV_8UC1));

    for (int i(param.skip_); i<height-param.skip_; ++i)
    {
        const uchar*previous(image.ptr<uchar>(i-1)),
                *current(image.ptr<uchar>(i)),
                *next(image.ptr<uchar>(i+1));
        uchar *d(map.ptr<uchar>(i));
        uchar *data(data_map.ptr<uchar>(i));
        for (int j(param.skip_); j<width-param.skip_; ++j)
        {
            if((i<=0)||(i>=height-1)||(j<=0)||(j>=width-1))
                continue;
            const int temp_1(*(sobel1+0)* *(previous+j-1)+
                             *(sobel1+2)* *(previous+j+1)+
                             *(sobel1+3)* *(current+j-1)+
                             *(sobel1+5)* *(current+j+1)+
                             *(sobel1+6)* *(next+j-1)+
                             *(sobel1+8)* *(next+j+1));
            const int temp_2(*(sobel2+0)* *(previous+j-1)+
                             *(sobel2+1)* *(previous+j)+
                             *(sobel2+2)* *(previous+j+1)+
                             *(sobel2+6)* *(next+j-1)+
                             *(sobel2+7)* *(next+j)+
                             *(sobel2+8)* *(next+j+1));
            const int temp_3(*(sobel3+1)* *(previous+j)+
                             *(sobel3+2)* *(previous+j+1)+
                             *(sobel3+3)* *(current+j-1)+
                             *(sobel3+5)* *(current+j+1)+
                             *(sobel3+6)* *(next+j-1)+
                             *(sobel3+7)* *(next+j));
            const int temp_4(*(sobel4+0)* *(previous+j-1)+
                             *(sobel4+1)* *(previous+j)+
                             *(sobel4+3)* *(current+j-1)+
                             *(sobel4+5)* *(current+j+1)+
                             *(sobel4+7)* *(next+j)+
                             *(sobel4+8)* *(next+j+1));
            const double temp_grads(std::max(std::max(abs(temp_1),abs(temp_2)),
                                             std::max(abs(temp_3),abs(temp_4))
                                             )
                                    );
            if (temp_grads > param.threshold_sobel_)
            {
                *(d+j)=255;
            }
            if (temp_grads >param.threshold_sobel_)
                *(data+j)=255;
            else
                *(data+j) = 0;
        }
    }
    double mean = cv::mean(data_map).val[0];
    msg+=boost::str(boost::format("Sobel Rawdata Mean:%.3f\n") % mean);
   // cv::imwrite(QString("./sobel_raw_data_map.bmp").toStdString().c_str(),data_map);

    std::vector<MClusterInf>pkgs;
    const int mottle_count=getClusterInf(map,pkgs,0,1000);

    msg+=boost::str(boost::format("find mottle, total:%d\n") % pkgs.size());
    if(mottle_count>param.MTottleParam.count){
        return param.MTottleParam.code;
    }
    if(pkgs.size()>1000){
        msg+=boost::str(boost::format("pkg size out 1000,skip detail msg test\n"));
        return param.code_;
    }

    MClusterInf pkg_max;
    for(std::vector<MClusterInf>::iterator iter=pkgs.begin();iter!=pkgs.end();)
    {
        if(std::max(iter->w_,iter->h_)<=param.limit_size_){
            iter =pkgs.erase(iter);
            continue;
        }
        if (iter->w_*iter->h_ >= pkg_max.w_*pkg_max.h_){
            pkg_max=*iter;
        }
        iter++;
    }
    msg+=boost::str(boost::format("find pkgs, total:%d\n") % pkgs.size());
    if (pkgs.size()==0){
        return 0;
    }
    msg+=boost::str(boost::format("pkg, x:%d y:%d w:%d h:%d\n")
                    % pkg_max.x_ % pkg_max.y_ % pkg_max.w_ % pkg_max.h_);
    const int &x_center(pkg_max.x_+pkg_max.w_/2);
    const int &y_center(pkg_max.y_+pkg_max.h_/2);
    const cv::Rect roi_in=cv::Rect(0,0,image.cols,image.rows)&cv::Rect(x_center-4,y_center-4,9,9);
    const cv::Rect roi_out=cv::Rect(0,0,image.cols,image.rows)&cv::Rect(x_center-7,y_center-7,15,15);
    const double mean_in=cv::mean(image(roi_in)).val[0];
    const double mean_out=cv::mean(image(roi_out)).val[0];
    if(mean_in>mean_out) {
        return param.code_white_;
    }
    return param.code_;

}

int AlgorithmFuncs::FindPkG_WF_1_1(cv::Mat &image, const MPkgParam &param,std::string &msg, std::vector<MClusterInf> &pd_all_single_particle)

{
    //抹掉单点
    QElapsedTimer timer_blemish;
    timer_blemish.start();

    const char sobel1[9]={-1, 0, 1,
                          -2, 0, 2,
                          -1, 0, 1};
    const char sobel2[9]={1, 2, 1,
                          0, 0, 0,
                          -1,-2,-1};
    const char sobel3[9]={0, 1, 2,
                          -1, 0, 1,
                          -2,-1, 0};
    const char sobel4[9]={-2,-1, 0,
                          -1, 0, 1,
                          0, 1, 2};
    const int &width(image.cols);
    const int &height(image.rows);
    cv::Mat map(cv::Mat::zeros(image.size(),CV_8UC1));

    cv::Mat data_map(cv::Mat::zeros(image.size(),CV_8UC1));

    for (int i(param.skip_); i<height-param.skip_; ++i)
    {
        const uchar*previous(image.ptr<uchar>(i-1)),
                *current(image.ptr<uchar>(i)),
                *next(image.ptr<uchar>(i+1));
        uchar *d(map.ptr<uchar>(i));
        uchar *data(data_map.ptr<uchar>(i));
        for (int j(param.skip_); j<width-param.skip_; ++j)
        {
            if((i<=0)||(i>=height-1)||(j<=0)||(j>=width-1))
                continue;
            const int temp_1(*(sobel1+0)* *(previous+j-1)+
                             *(sobel1+2)* *(previous+j+1)+
                             *(sobel1+3)* *(current+j-1)+
                             *(sobel1+5)* *(current+j+1)+
                             *(sobel1+6)* *(next+j-1)+
                             *(sobel1+8)* *(next+j+1));
            const int temp_2(*(sobel2+0)* *(previous+j-1)+
                             *(sobel2+1)* *(previous+j)+
                             *(sobel2+2)* *(previous+j+1)+
                             *(sobel2+6)* *(next+j-1)+
                             *(sobel2+7)* *(next+j)+
                             *(sobel2+8)* *(next+j+1));
            const int temp_3(*(sobel3+1)* *(previous+j)+
                             *(sobel3+2)* *(previous+j+1)+
                             *(sobel3+3)* *(current+j-1)+
                             *(sobel3+5)* *(current+j+1)+
                             *(sobel3+6)* *(next+j-1)+
                             *(sobel3+7)* *(next+j));
            const int temp_4(*(sobel4+0)* *(previous+j-1)+
                             *(sobel4+1)* *(previous+j)+
                             *(sobel4+3)* *(current+j-1)+
                             *(sobel4+5)* *(current+j+1)+
                             *(sobel4+7)* *(next+j)+
                             *(sobel4+8)* *(next+j+1));
            const double temp_grads(std::max(std::max(abs(temp_1),abs(temp_2)),
                                             std::max(abs(temp_3),abs(temp_4))
                                             )
                                    );
            if (temp_grads > param.threshold_sobel_)
            {
                *(d+j)=255;
            }
            if (temp_grads >param.threshold_sobel_)
                *(data+j)=255;
            else
                *(data+j) = 0;
        }
    }
    double mean = cv::mean(data_map).val[0];
    msg+=boost::str(boost::format("Sobel Rawdata Mean:%.3f\n") % mean);
    //cv::imwrite(QString("./sobel_raw_data_map.bmp").toStdString().c_str(),data_map);

    std::vector<MClusterInf>pkgs;
    const int mottle_count=getClusterInf(map,pkgs,0,1000);

    msg+=boost::str(boost::format("find mottle, total:%d\n") % pkgs.size());
    if(mottle_count>param.MTottleParam.count){
        return param.MTottleParam.code;
    }
  MClusterInf pkg_max;
 msg+=boost::str(boost::format("find pkgs, total:%d\n") % pkgs.size());
 //去除单点single的影响
 std::vector<MClusterInf> pkgs_remove_signle_pd;


 if (pkgs.size()>0)
 {
     int counter(0);
     BOOST_FOREACH (const MClusterInf &p, pkgs)
     {
         bool flag = false;
         BOOST_FOREACH (const MClusterInf &pd_particle, pd_all_single_particle)
         {

             if(qAbs(p.x_ - pd_particle.x_) <= 3 && qAbs(p.y_ - pd_particle.y_) <= 3)
             {
                 counter+=1;
                 if (counter<6)
                 {

                     msg+=boost::str(boost::format("remove signle pd in particle, x:%d y:%d w:%d h:%d\n")
                                     % pd_particle.x_% pd_particle.y_%pd_particle.w_% pd_particle.h_);
//                        flag = true;
                 }
                 flag = true;
                 if(timer_blemish.elapsed() > 300)//超过1s直接返回code183
                 {
                     msg+=boost::str(boost::format("remove signle pd in particle time out\n"));
                     return  param.code_;
                 }
             }
         }
         if(flag == false)
         {
             pkgs_remove_signle_pd.push_back (MClusterInf(p.x_ ,p.y_, p.w_, p.h_, p.number, p.code_));
         }
     }
 }



 msg+=boost::str(boost::format("Skip single PD,find pkgs, total:%d\n") % pkgs_remove_signle_pd.size());



 if(pkgs_remove_signle_pd.size()>1000){
     msg+=boost::str(boost::format("pkg size out 1000,skip detail msg test\n"));
     return param.code_;
 }
 int n = 0;
for(std::vector<MClusterInf>::iterator iter=pkgs_remove_signle_pd.begin();iter!=pkgs_remove_signle_pd.end();)
{
    if(std::max(iter->w_,iter->h_)<=param.limit_size_){
        iter =pkgs_remove_signle_pd.erase(iter);
        continue;
    }

   if (iter->w_*iter->h_ >= pkg_max.w_*pkg_max.h_){
       pkg_max=*iter;
}
n++;
    iter++;
    if(n>20)
    {
        msg+=boost::str(boost::format("pkg num > 50,ship find max PKG \n"));
        msg+=boost::str(boost::format("pkg, x:%d y:%d w:%d h:%d\n")
                        % pkg_max.x_ % pkg_max.y_ % pkg_max.w_ % pkg_max.h_);
        return param.code_;
    }
        if(timer_blemish.elapsed() > 300)//超过1s直接返回code123
        {
            msg+=boost::str(boost::format("sum pkg time out\n"));

            return param.code_;


        }
}
// msg+=boost::str(boost::format("find pkgs, total:%d\n") % pkgs.size());
if (pkgs_remove_signle_pd.size()==0){
    return 0;
}
    msg+=boost::str(boost::format("pkg, x:%d y:%d w:%d h:%d\n")
                    % pkg_max.x_ % pkg_max.y_ % pkg_max.w_ % pkg_max.h_);
    const int &x_center(pkg_max.x_+pkg_max.w_/2);
    const int &y_center(pkg_max.y_+pkg_max.h_/2);
    const cv::Rect roi_in=cv::Rect(0,0,image.cols,image.rows)&cv::Rect(x_center-4,y_center-4,9,9);
    const cv::Rect roi_out=cv::Rect(0,0,image.cols,image.rows)&cv::Rect(x_center-7,y_center-7,15,15);
    const double mean_in=cv::mean(image(roi_in)).val[0];
    const double mean_out=cv::mean(image(roi_out)).val[0];
    if(mean_in>mean_out) {
        return param.code_white_;
    }
    return param.code_;

}


int AlgorithmFuncs::FindBlemsh_WF(cv::Mat &image, const MBlemishParam &param,std::string &msg)

{
    QElapsedTimer timer_blemish;
    timer_blemish.start();
    cv::Mat mean2,mean1,dif;
//    cv::Mat dif=cv::Mat::eye(this->image.cols, this->image.rows, CV_32FC1);
    cv::blur(image,mean1,cv::Size(param.winSmall/2*2+1,param.winSmall/2*2+1),cv::Point(-1,-1),cv::BORDER_REFLECT101);
    cv::blur(image,mean2,cv::Size(param.winBig/2*2+1,param.winBig/2*2+1));
    //----------只取黑色坏点(大-小为黑，小-大为白)
    cv::subtract(mean2,mean1,dif);

    dif.convertTo(dif,CV_32FC1);
    mean2.convertTo(mean2,CV_32FC1);
    cv::Mat result;
    cv::divide(dif,mean2,result);

    cv::Mat center(result,cv::Rect(param.x_,param.y_, param.w_ ,param.h_ ));
    cv::threshold(center,center,param.threshold_center_,255,CV_THRESH_BINARY);
    cv::threshold(result,result,param.threshold_margin_,255,CV_THRESH_BINARY);
    result.rowRange(0,param.skip_).setTo(cv::Scalar(0));
    result.rowRange(result.rows-param.skip_,result.rows).setTo(cv::Scalar(0));
    result.colRange(0,param.skip_).setTo(cv::Scalar(0));
    result.colRange(result.cols-param.skip_,result.cols).setTo(cv::Scalar(0));

    result.convertTo(result,CV_8UC1);
    const int &length(param.skip_rect);
    result(cv::Range(0,length)/*row*/, cv::Range(0,length)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(0,length), cv::Range(result.cols-length,result.cols)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(result.rows-length,result.rows), cv::Range(0,length)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(result.rows-length,result.rows), cv::Range(result.cols-length,result.cols)/*col*/).setTo(cv::Scalar(0));

    std::vector<MClusterInf>particle;
    getClusterInf(result,particle,param.limit_size_,param.max_size_);
    msg+=boost::str(boost::format("find blemish, total:%d\n") % particle.size());
    if(particle.size()>1000)
         return  param.code_dark_;

    if (particle.size()>0)
    {
        int counter(0);
        BOOST_FOREACH (const MClusterInf &p, particle)
        {
            counter+=1;
            if (counter<6)
            {
                msg+=boost::str(boost::format("blemish, x:%d y:%d w:%d h:%d\n")
                                % p.x_% p.y_%p.w_% p.h_);
                if(timer_blemish.elapsed() > 300)//超过1s直接返回code183
                {
                    msg+=boost::str(boost::format("particle not in enahce time out\n"));
                    return  param.code_dark_;
                }
                continue;
            }
            break;
        }
        return param.code_dark_;
    }
    return 0;

}




int AlgorithmFuncs::FindBlemsh_WF_NEW(cv::Mat &image, const MBlemishParam &param,std::string &msg, std::vector<MClusterInf> &pd_all_single_particle)
{
    QElapsedTimer timer_blemish;
    timer_blemish.start();
    std::vector<MClusterInf>particle;
    getimage_ClusterInf(image, param, particle, msg);  //没有图像增强

    std::vector<MClusterInf>particle_enhace;
    getimage_ClusterInf_enhace(image, param, particle_enhace, msg);  //有图像增强

    if(particle.size()>1000)
         return  param.code_dark_;


    //去除单点single的影响
    std::vector<MClusterInf> particle_remove_signle_pd;
    if (particle.size()>0)
    {
        int counter(0);
        BOOST_FOREACH (const MClusterInf &p, particle)
        {
            bool flag = false;
            BOOST_FOREACH (const MClusterInf &pd_particle, pd_all_single_particle)
            {

                if(qAbs(p.x_ - pd_particle.x_) <= param.pd_area_x_ && qAbs(p.y_ - pd_particle.y_) <= param.pd_area_y_)
                {
                    counter+=1;
                    if (counter<6)
                    {

                        msg+=boost::str(boost::format("remove signle pd in particle, x:%d y:%d w:%d h:%d\n")
                                        % pd_particle.x_% pd_particle.y_%pd_particle.w_% pd_particle.h_);
//                        flag = true;

                    }
                    if(timer_blemish.elapsed() > 300)//超过1s直接返回code183
                    {
                        msg+=boost::str(boost::format("remove signle pd in particle time out\n"));
                        return  param.code_dark_;
                    }
                    flag = true;
                }
            }
            if(flag == false)
            {
                particle_remove_signle_pd.push_back (MClusterInf(p.x_ ,p.y_, p.w_, p.h_, p.number, p.code_));
            }
        }
    }







    //区域搜索
    std::vector<MClusterInf> particle_not_in_enahce;
    QVector<QVector<BlemishInfo> > categorizedBlemishes;
    QVector<BlemishInfo> enhace_blemishList;
    if (particle_remove_signle_pd.size()>0)
    {
        int counter(0);
        BOOST_FOREACH (const MClusterInf &p, particle_remove_signle_pd)   //particle是LSC出图的结果
        {
//            counter+=1;
//            if (counter<6)
//            {
//                msg+=boost::str(boost::format("blemish, x:%d y:%d w:%d h:%d\n")
//                                % p.x_% p.y_%p.w_% p.h_);
//            }

            enhace_blemishList.clear();
            bool categorized = false;
            BOOST_FOREACH (const MClusterInf &p_enhace, particle_enhace)
            {
                if (qAbs(p.x_ - p_enhace.x_*param.magnification) <= param.search_area_x && qAbs(p.y_ - p_enhace.y_*param.magnification) <= param.search_area_y)
                {
                    // 添加示例数据到列表中
                    enhace_blemishList.push_back({"lsc_blemish", p.x_, p.y_, p.w_,p.h_,
                                                  "enhace_blemish", p_enhace.x_*param.magnification, p_enhace.y_*param.magnification, p_enhace.w_*param.magnification, p_enhace.h_*param.magnification});
                    categorized = true;
                }
                if(timer_blemish.elapsed() > 300)//超过1s直接返回code183
                {
                    msg+=boost::str(boost::format("particle not in enahce time out\n"));
                    return  param.code_dark_;
                }
            }
            if (categorized) {
                categorizedBlemishes.push_back({enhace_blemishList});
            }
            if(!categorized)
            {
                particle_not_in_enahce.push_back (MClusterInf(p.x_ ,p.y_, p.w_, p.h_, 0, 0));
            }
        }
    }




    //增强图像连通大区域
    int counter(0);
    std::vector<MClusterInf> particle_enhace_all_size;

    for (int i = 0; i < categorizedBlemishes.size(); ++i)
    {

        const QVector<BlemishInfo>& blemishList = categorizedBlemishes.at(i);

        int max_p_enhace_x_plus_w = INT_MIN;
        int max_p_enhace_y_plus_h = INT_MIN;
        int min_p_enhace_x = INT_MAX;
        int min_p_enhace_y = INT_MAX;

        int current_x;
        int current_y;


        // 遍历当前分类中的每个 BlemishInfo
        for (int j = 0; j < blemishList.size(); ++j)
        {
            const BlemishInfo& info = blemishList.at(j);

            int current_x_plus_w = info.enhace_x + info.enhace_w;
            int current_y_plus_h = info.enhace_y + info.enhace_h;
            int current_enhace_x = info.enhace_x;
            int current_enhace_y = info.enhace_y;
            current_x = info.lsc_x;
            current_y = info.lsc_y;

            // 更新最大值
            if (current_x_plus_w > max_p_enhace_x_plus_w) {
                max_p_enhace_x_plus_w = current_x_plus_w;
            }

            if (current_y_plus_h > max_p_enhace_y_plus_h) {
                max_p_enhace_y_plus_h = current_y_plus_h;
            }

            // 更新最小值
            if (current_enhace_x < min_p_enhace_x) {
                min_p_enhace_x = current_enhace_x;
            }

            if (current_enhace_y < min_p_enhace_y) {
                min_p_enhace_y = current_enhace_y;
            }
        }

//        counter+=1;
//        if (counter<6)
//        {

//            msg+=boost::str(boost::format("blemish enhace roi total size, x:%d y:%d w:%d h:%d\n")
//                                %current_x  % current_y % (max_p_enhace_x_plus_w - min_p_enhace_x)  % (max_p_enhace_y_plus_h - min_p_enhace_y));
//        }

        particle_enhace_all_size.push_back (MClusterInf(current_x ,current_y, max_p_enhace_x_plus_w - min_p_enhace_x, max_p_enhace_y_plus_h - min_p_enhace_y, 0, 0));
    }





    //判断真实的True Blemish

//    if( particle.size() > particle_enhace_all_size.size())
//    {
//        return param.code_dark_;
//    }

    int counter_(0);
    bool flag = false;
    if (particle_remove_signle_pd.size()>0)
    {

        BOOST_FOREACH (const MClusterInf &p, particle_remove_signle_pd)
        {

            BOOST_FOREACH (const MClusterInf &p_enhace, particle_enhace_all_size)
            {
                if(p.x_ == p_enhace.x_ && p.y_ == p_enhace.y_)
                {
                    if((max(p_enhace.w_, p_enhace.h_) > param.min_enhace_size)  && (max(p_enhace.w_, p_enhace.h_) <= param.max_enhace_size ))   //真正的blemish
                    {
                        flag = true;

                        counter_+=1;
                        if (counter_<6)
                        {
                            msg+=boost::str(boost::format("blemish, x:%d y:%d w:%d h:%d\n")
                                            % p.x_% p.y_%p.w_% p.h_);
                        }
                        if(timer_blemish.elapsed() > 300)//超过1s直接返回code183
                        {
                            msg+=boost::str(boost::format("particle not in enahce time out\n"));
                            return  param.code_dark_;
                        }
                    }
                }
            }

        }

    }


    int code = 0;
    if(flag)
    {
        code = param.code_dark_;
    }

    if (particle_not_in_enahce.size()>0)
    {
        int counter(0);
        BOOST_FOREACH (const MClusterInf &p, particle_not_in_enahce)
        {
            counter+=1;
            if (counter<6)
            {

                msg+=boost::str(boost::format("blemish, x:%d y:%d w:%d h:%d\n")
                                % p.x_% p.y_%p.w_% p.h_);
                continue;
            }
            if(timer_blemish.elapsed() > 300)//超过1s直接返回code183
            {
                msg+=boost::str(boost::format("particle not in enahce time out\n"));
                return  param.code_dark_;
            }
            break;
        }
        code = param.code_dark_;
    }

    return code;

}



int AlgorithmFuncs::getimage_ClusterInf(cv::Mat &image, const MBlemishParam &param, std::vector<MClusterInf> &particle ,std::string &msg)
{


    //----------读取原始图像----------------
    cv::Mat srcImage_32;
    image.convertTo(srcImage_32,CV_32FC1);


    //----------亮度提取----------------
    cv::Mat grayImage(srcImage_32.size(),CV_32FC1);
    double s=1.0/16;
    cv::Mat kern = (cv::Mat_<double>(3,3) << s,2*s,s,2*s,4*s,2*s,s,2*s,s);
    cv::filter2D(srcImage_32,grayImage,srcImage_32.depth(),kern);



    //----------均值滤波----------------
    cv::Mat blur_img;
    cv::blur(grayImage,blur_img,cv::Size(5,5));


    //-----------中值去噪--------------
    cv::Mat image_mblur;
    cv::medianBlur(blur_img,image_mblur,3);




    //----------图像压缩----------------

    cv::Mat src_small;
    cv::resize(image_mblur,src_small,cv::Size(param.resize_w_,param.resize_h_),0,0,cv::INTER_LINEAR);


    //----------LSC----------------
    cv::Mat  lsc_img = src_small;


    double mean_corner =0;
    for(int i=0; i<30; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        for(int j=0; j<40; j++)
        {
            mean_corner += p_row_small[j];

        }
    }
    double mean_center =0;
    for(int i=src_small.rows/2-15; i<src_small.rows/2+15; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        for(int j=src_small.cols/2-20; j<src_small.cols/2+20; j++)
        {
            mean_center += p_row_small[j];

        }
    }


    double delta = (mean_center/mean_corner -1)*param.lsc_ratio_;

    double factor = delta/((param.resize_w_/2)*(param.resize_w_/2) + (param.resize_h_/2)*(param.resize_h_/2));

    for(int i=0; i<src_small.rows; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        float *p_row_lsc = lsc_img.ptr<float>(i);
        int y = std::abs(i-param.resize_h_/2);
        for(int j=0; j<src_small.cols; j++)
        {
            int x = std::abs(j-param.resize_w_/2);
            p_row_lsc[j] = p_row_small[j] * ((x*x + y*y) * factor + 1);

        }
    }



    cv::Mat mean2,mean1,dif;

    //这里就是经过 lsc 算法的
    cv::blur(lsc_img,mean1,cv::Size(param.winSmall/2*2+1,param.winSmall/2*2+1),cv::Point(-1,-1),cv::BORDER_REFLECT101);
    cv::blur(lsc_img,mean2,cv::Size(param.winBig/2*2+1,param.winBig/2*2+1));


    //----------只取黑色坏点(大-小为黑，小-大为白)
    cv::subtract(mean2,mean1,dif);

    dif.convertTo(dif,CV_32FC1);

    mean2.convertTo(mean2,CV_32FC1);
    cv::Mat result;
    cv::divide(dif,mean2,result);

    cv::Mat center(result,cv::Rect(param.x_,param.y_, param.w_ ,param.h_ ));
    cv::threshold(center,center,param.threshold_center_,255,CV_THRESH_BINARY);
    cv::threshold(result,result,param.threshold_margin_,255,CV_THRESH_BINARY);

    result.rowRange(0,param.skip_).setTo(cv::Scalar(0));
    result.rowRange(result.rows-param.skip_,result.rows).setTo(cv::Scalar(0));
    result.colRange(0,param.skip_).setTo(cv::Scalar(0));
    result.colRange(result.cols-param.skip_,result.cols).setTo(cv::Scalar(0));

    result.convertTo(result,CV_8UC1);
    const int &length(param.skip_rect);
    result(cv::Range(0,length)/*row*/, cv::Range(0,length)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(0,length), cv::Range(result.cols-length,result.cols)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(result.rows-length,result.rows), cv::Range(0,length)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(result.rows-length,result.rows), cv::Range(result.cols-length,result.cols)/*col*/).setTo(cv::Scalar(0));


    getClusterInf(result,particle,param.limit_size_,param.max_size_);
    msg+=boost::str(boost::format("find blemish, total:%d\n") % particle.size());

}




int AlgorithmFuncs::getimage_ClusterInf_enhace(cv::Mat &image, const MBlemishParam &param, std::vector<MClusterInf> &particle ,std::string &msg)
{


    //----------读取原始图像----------------
    cv::Mat srcImage_32;
    image.convertTo(srcImage_32,CV_32FC1);


    //----------亮度提取----------------
    cv::Mat grayImage(srcImage_32.size(),CV_32FC1);
    double s=1.0/16;
    cv::Mat kern = (cv::Mat_<double>(3,3) << s,2*s,s,2*s,4*s,2*s,s,2*s,s);
    cv::filter2D(srcImage_32,grayImage,srcImage_32.depth(),kern);



    //----------均值滤波----------------
    cv::Mat blur_img;
    cv::blur(grayImage,blur_img,cv::Size(5,5));


    //-----------中值去噪--------------
    cv::Mat image_mblur;
    cv::medianBlur(blur_img,image_mblur,3);


    //----------图像压缩----------------

    cv::Mat src_small;
    cv::resize(image_mblur,src_small,cv::Size(param.resize_w_enhace,param.resize_h_enhace),0,0,cv::INTER_LINEAR);




    //----------LSC----------------
    cv::Mat  lsc_img = src_small;


    double mean_corner =0;
    for(int i=0; i<30; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        for(int j=0; j<40; j++)
        {
            mean_corner += p_row_small[j];

        }
    }
    double mean_center =0;
    for(int i=src_small.rows/2-15; i<src_small.rows/2+15; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        for(int j=src_small.cols/2-20; j<src_small.cols/2+20; j++)
        {
            mean_center += p_row_small[j];

        }
    }


    double delta = (mean_center/mean_corner -1)*param.lsc_ratio_enhace;

    double factor = delta/((param.resize_w_enhace/2)*(param.resize_w_enhace/2) + (param.resize_h_enhace/2)*(param.resize_h_enhace/2));

    for(int i=0; i<src_small.rows; i++)
    {
        float *p_row_small = src_small.ptr<float>(i);
        float *p_row_lsc = lsc_img.ptr<float>(i);
        int y = std::abs(i-param.resize_h_enhace/2);
        for(int j=0; j<src_small.cols; j++)
        {
            int x = std::abs(j-param.resize_w_enhace/2);
            p_row_lsc[j] = p_row_small[j] * ((x*x + y*y) * factor + 1);

        }
    }


    //----------对比度增强----------------
    cv::Mat  enhace;

    double  minV, maxV;
    int  minId[2] = {}, maxId[2] = {};	// minnimum Index, maximum Index

    cv::minMaxIdx(lsc_img, &minV, &maxV, minId, maxId);

    double max_ds = 250;
    double min_ds = 50;

    enhace = (lsc_img - minV)*(max_ds-min_ds)/(maxV-minV)+min_ds;


    cv::Mat mean2,mean1,dif;

    //这里就是经过 lsc 算法的
    cv::blur(enhace,mean1,cv::Size(param.winSmall_enhace/2*2+1,param.winSmall_enhace/2*2+1),cv::Point(-1,-1),cv::BORDER_REFLECT101);
    cv::blur(enhace,mean2,cv::Size(param.winBig_enhace/2*2+1,param.winBig_enhace/2*2+1));


    //----------只取黑色坏点(大-小为黑，小-大为白)
    cv::subtract(mean2,mean1,dif);

    dif.convertTo(dif,CV_32FC1);

    mean2.convertTo(mean2,CV_32FC1);
    cv::Mat result;
    cv::divide(dif,mean2,result);

    cv::Mat center(result,cv::Rect(param.x_enhace,param.y_enhace, param.w_enhace ,param.h_enhace ));
    cv::threshold(center,center,param.threshold_center_enhace,255,CV_THRESH_BINARY);
    cv::threshold(result,result,param.threshold_margin_enhace,255,CV_THRESH_BINARY);


    result.rowRange(0,param.skip_enhace).setTo(cv::Scalar(0));
    result.rowRange(result.rows-param.skip_enhace,result.rows).setTo(cv::Scalar(0));
    result.colRange(0,param.skip_enhace).setTo(cv::Scalar(0));
    result.colRange(result.cols-param.skip_enhace,result.cols).setTo(cv::Scalar(0));

    result.convertTo(result,CV_8UC1);
    const int &length(param.skip_rect_enhace);
    result(cv::Range(0,length)/*row*/, cv::Range(0,length)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(0,length), cv::Range(result.cols-length,result.cols)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(result.rows-length,result.rows), cv::Range(0,length)/*col*/).setTo(cv::Scalar(0));
    result(cv::Range(result.rows-length,result.rows), cv::Range(result.cols-length,result.cols)/*col*/).setTo(cv::Scalar(0));


//    std::vector<MClusterInf>particle;
    getClusterInf(result,particle,param.limit_size_enhace,param.max_size_enhace);
    msg+=boost::str(boost::format("find blemish_enhace, total:%d\n") % particle.size());

}




int AlgorithmFuncs::FindALLClusterBayer_in_binary(uchar *binary_map, uchar *buf_map,
                                                  int w, int h,
                                                  QMap<int,int> &cnts_cluster,
                                                  int &cnt_2x4_pix,
                                                  int &cnt_2x2_pix,
                                                  int &cnt_1x2_2x1_pix,
                                                  int &cnt_2x2_diagonal_pix,
                                                  int &cnt_1x2_ch,
                                                  int &cnt_2x1_ch,
                                                  int &cnt_2x2_leftdiagonal,
                                                  int &cnt_2x2_rightdiagonal,
                                                  int &cnt_white_pix,
                                                  int &cnt_dark_pix,
                                                  int &paticle,
                                                  int &other_cluster,
                                                  int &other_cluster_ch,
                                                  std::string &g_str_msg_)
{
    int n1=0,n2=0,n3=0,n4=0,n5=0,n6=0;
    uchar *binary_map_tmp = buf_map;
    memcpy(binary_map_tmp,binary_map,w*h*sizeof(uchar));
    //counter cluster
    std::vector<MPDInf> clusters;
    defectCounter (binary_map,w,h,128,clusters);
    int cnt_cluster = 0;
    g_str_msg_+=boost::str(boost::format("global cluster cnt : %d\n")
                           % (clusters.size()));
    BOOST_FOREACH(const MPDInf &cluster, clusters)
    {
        if (cluster.w_>3 ||
                cluster.h_>5)
        {
            paticle+=1;
            if (paticle<=5)
            {
                g_str_msg_+=boost::str(boost::format("paticle or peeling x:%d y:%d w:%d h:%d\n")
                                % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
        }
        else if (cluster.w_>=2 ||
                cluster.h_>=2)
        {
//            g_str_msg_+=boost::str(boost::format("cluster in \n"));

            if (true)
            {
                if (cluster.w_ == 2 &&
                        cluster.h_ == 4)
                {
                    if (cnt_2x4_pix < 5)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x4 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 4);
                    }
                    cnt_2x4_pix+=1;
                    cnts_cluster.insert(0,cnts_cluster.value(0)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 1)
                {
                    if (cnt_1x2_2x1_pix < 7)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x1 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % cluster.w_  % cluster.h_);
                    }
                    cnt_1x2_2x1_pix+=1;
                    cnts_cluster.insert(1,cnts_cluster.value(1)+1);
                }
                else if (cluster.w_ == 1 &&
                         cluster.h_ == 2 &&
                         binary_map_tmp[cluster.y_*w+cluster.x_]==255 &&
                         binary_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if (cnt_1x2_2x1_pix < 7)
                    {
                        g_str_msg_+=boost::str(boost::format("global 1x2 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) %  cluster.w_  % cluster.h_);
                    }
                    cnt_1x2_2x1_pix+=1;
                    cnts_cluster.insert(3,cnts_cluster.value(3)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         binary_map_tmp[cluster.y_*w+cluster.x_]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && binary_map_tmp[cluster.y_*w+cluster.x_+1]==0
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_]==0)
                {
                    if (cnt_2x2_diagonal_pix < 3)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x2 diagonal cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    }
                    cnt_2x2_diagonal_pix+=1;

                    cnts_cluster.insert(5,cnts_cluster.value(5)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         binary_map_tmp[cluster.y_*w+cluster.x_]==0
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==0
                         && binary_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if (cnt_2x2_diagonal_pix < 3)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x2 diagonal cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    }
                    cnt_2x2_diagonal_pix+=1;

                    cnts_cluster.insert(6,cnts_cluster.value(6)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         binary_map_tmp[cluster.y_*w+cluster.x_]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && binary_map_tmp[cluster.y_*w+cluster.x_+1]==0
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n1<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n1++;
                    cnts_cluster.insert(9,cnts_cluster.value(9)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         binary_map_tmp[cluster.y_*w+cluster.x_]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==0
                         && binary_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n2<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n2++;
                    cnts_cluster.insert(10,cnts_cluster.value(10)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         binary_map_tmp[cluster.y_*w+cluster.x_]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && binary_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_]==0)
                {
                    if(n3<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n3++;
                    cnts_cluster.insert(11,cnts_cluster.value(11)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         binary_map_tmp[cluster.y_*w+cluster.x_]==0
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && binary_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n4<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n4++;
                    cnts_cluster.insert(12,cnts_cluster.value(12)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         binary_map_tmp[cluster.y_*w+cluster.x_]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && binary_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && binary_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n5<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 4WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n5++;
                    cnts_cluster.insert(13,cnts_cluster.value(13)+1);
                }
                else
                {
                    if(n6<3)
                        g_str_msg_+=boost::str(boost::format("global other cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % cluster.w_  % cluster.h_ );
                    n6++;
                    other_cluster++;
                    cnts_cluster.insert(14,cnts_cluster.value(14)+1);
                }
            }
            cnt_cluster+=1;
        }
    }


    uchar *binary_map_tmp_ch = buf_map + w*h;
    memcpy(binary_map_tmp_ch,binary_map,w*h*sizeof(uchar));

    std::vector<MPDInf> clusters_ch;
    defectCounter_channel (binary_map,w,h,128,clusters_ch);

    g_str_msg_+=boost::str(boost::format("bychannel cluster cnt : %d\n")
                           % (clusters_ch.size()));

    BOOST_FOREACH(const MPDInf &cluster, clusters_ch)
    {
        if(cluster.w_ >=3 ||
                cluster.h_ >=3 )
        {
            if (cluster.w_ == 3 &&
                    cluster.h_ == 1 &&
                    binary_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                    binary_map_tmp_ch[cluster.y_*w+cluster.x_+1]==0 &&
                    binary_map_tmp_ch[cluster.y_*w+cluster.x_+2]==255)
            {
                cnt_2x1_ch+=1;

                if (cnt_2x1_ch<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x1_ch  x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(2,cnts_cluster.value(2)+1);
            }

            else if (cluster.w_ == 1 &&
                     cluster.h_ == 3 &&
                     binary_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                     binary_map_tmp_ch[(cluster.y_+1)*w+cluster.x_]==0 &&
                     binary_map_tmp_ch[(cluster.y_+2)*w+cluster.x_]==255)
            {
                cnt_1x2_ch+=1;

                if (cnt_1x2_ch<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_1x2_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(4,cnts_cluster.value(4)+1);
            }

            else if (cluster.w_ == 3 &&
                     cluster.h_ == 3 &&
                     binary_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                     binary_map_tmp_ch[(cluster.y_+1)*w+cluster.x_+1]==0 &&
                     binary_map_tmp_ch[(cluster.y_+2)*w+cluster.x_+2]==255)
            {
                cnt_2x2_leftdiagonal+=1;

                if (cnt_2x2_leftdiagonal<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x2_leftdiagonal_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(7,cnts_cluster.value(7)+1);
            }

            else if (cluster.w_ == 3 &&
                     cluster.h_ == 3 &&
                     binary_map_tmp_ch[cluster.y_*w+cluster.x_+2]==255 &&
                     binary_map_tmp_ch[(cluster.y_+1)*w+cluster.x_+1]==0 &&
                     binary_map_tmp_ch[(cluster.y_+2)*w+cluster.x_]==255)
            {
                cnt_2x2_rightdiagonal+=1;

                if (cnt_2x2_rightdiagonal<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x2_rightdiagonal_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(8,cnts_cluster.value(8)+1);
            }

            else
            {
                other_cluster_ch +=1;
                if (other_cluster_ch <=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_other_cluster_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

            }
            cnt_cluster+=1;
        }
        else
        {
            other_cluster_ch +=1;
            if (other_cluster_ch <=3)
            {
                g_str_msg_+=boost::str(boost::format("cnt_other_cluster_ch x:%d y:%d w:%d h:%d\n")
                                       % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
            cnt_cluster+=1;
        }
    }

    g_str_msg_+=boost::str(boost::format("cluster_ch Cnt:%d    other_cluster_ch Cnt:%d\n")
                           % (cnt_2x1_ch + cnt_1x2_ch + cnt_2x2_leftdiagonal +cnt_2x2_rightdiagonal) % other_cluster_ch) ;

    int cnt =clusters.size() +clusters_ch.size();
    return cnt;
}





int AlgorithmFuncs::Findblemish_in_blemish_binary(uchar *binary_map, int w, int h,
                                                  int blemish_global_size_limit,
                                                  int blemish_bychannel_size_limit,
                                                  std::string &g_str_msg_)
{
    int blemish = 0;
    //counter cluster
    std::vector<MPDInf> clusters;
    defectCounter (binary_map,w,h,128,clusters);

    BOOST_FOREACH(const MPDInf &cluster, clusters)
    {
        if (cluster.w_>blemish_global_size_limit ||
                cluster.h_>blemish_global_size_limit)
        {
            blemish+=1;
            if (blemish<=5)
            {
                g_str_msg_+=boost::str(boost::format("blemish x:%d y:%d w:%d h:%d\n")
                                       % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
        }
    }

    std::vector<MPDInf> clusters_ch;
    defectCounter_channel (binary_map,w,h,128,clusters_ch);


    BOOST_FOREACH(const MPDInf &cluster, clusters_ch)
    {
        if(cluster.w_ >=blemish_bychannel_size_limit ||
                cluster.h_ >=blemish_bychannel_size_limit )
        {
            blemish+=1;
            if (blemish<=5)
            {
                g_str_msg_+=boost::str(boost::format("blemish x:%d y:%d w:%d h:%d\n")
                                       % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
        }
    }

    return blemish;
}


int AlgorithmFuncs::FindALLClusterBayer_WF(uchar *data, int w, int h,
                                           QMap<int,int> &cnts_cluster,
                                           int &cnt_2x4_pix,
                                           int &cnt_2x2_pix,
                                           int &cnt_1x2_2x1_pix,
                                           int &cnt_2x2_diagonal_pix,
                                           int &cnt_1x2_ch,
                                           int &cnt_2x1_ch,
                                           int &cnt_2x2_leftdiagonal,
                                           int &cnt_2x2_rightdiagonal,
                                           int &cnt_white_pix,
                                           int &cnt_dark_pix,
                                           int &paticle,
                                           int &other_cluster,
                                           int &other_cluster_ch,
                                           int r, double spec, int skipCnt,
                                           uchar *buf_map, std::string &g_str_msg_)
{
    cnt_2x4_pix = 0;
    cnt_1x2_2x1_pix = 0;
    cnt_2x2_pix = 0;
    cnt_2x2_diagonal_pix=0;
    cnt_1x2_ch=0;
    cnt_2x1_ch=0;
    cnt_2x2_leftdiagonal=0;
    cnt_2x2_rightdiagonal=0;
    cnt_white_pix=0;
    cnt_dark_pix=0;
    paticle=0;
    other_cluster =0;
    other_cluster_ch =0;

    int n1=0,n2=0,n3=0,n4=0,n5=0,n6=0;
    memset(buf_map,0,w*h);

    g_str_msg_+=boost::str(boost::format("spec : %f\n")
                    % spec);
    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        for (int j=skipCnt; j<w-skipCnt; ++j)
        {
            double mean_t = 0.0;
            int cnt = 0;

            int row_start = i-2*r;
            int row_end = i+2*r;
            if (row_start<0)
            {
                if(std::abs(row_start)%2==0)
                    row_start = 0;
                else
                    row_start =1;
                row_end = row_start+4*r;
            }
            else if (row_end>(h-1))
            {
                if(std::abs(row_end)%2==0)
                    row_end = h-2;
                else
                    row_end = h-1;
                row_start = row_end-4*r;
            }
            int col_start = j-2*r;
            int col_end = j+2*r;
            if (col_start<0)
            {
                if(std::abs(col_start)%2==0)
                    col_start = 0;
                else
                    col_start = 1;
                col_end = col_start+4*r;
            }
            else if (col_end>(w-1))
            {
                if(std::abs(col_end)%2==0)
                    col_end = w-2;
                else
                    col_end = w-1;
                col_start = col_end-4*r;
            }

            for (int l=row_start; l<=row_end; l+=2)
            {
                for (int m=col_start; m<=col_end; m+=2)
                {
                    if (l>=skipCnt && l<h-skipCnt &&
                            m>=skipCnt && m<w-skipCnt)
                    {
                        cnt+=1;
                        mean_t+=(double)data[w*l+m];
                    }
                }
            }
            if (cnt <= 1)
            {
                // this condition: impossible
                continue;
            }
            double mean_ex = mean_t-data[i*w+j];
            mean_ex/=(cnt-1);
            mean_t/=cnt;
            double compareValue = std::fabs((double)data[w*i+j]-mean_ex)/mean_t;
            if (compareValue>=spec)
            {
                buf_map[w*i+j] = 255;
            }
        }
    }
    uchar *buf_map_tmp = new uchar[w*h];
    memcpy(buf_map_tmp,buf_map,w*h*sizeof(uchar));
    //counter cluster
    std::vector<MPDInf> clusters;
    defectCounter (buf_map,w,h,128,clusters);
    int cnt_cluster = 0;
    g_str_msg_+=boost::str(boost::format("cluster cnt : %d\n")
                           % (clusters.size()));
    BOOST_FOREACH(const MPDInf &cluster, clusters)
    {
        if (cluster.w_>3 ||
                cluster.h_>5)
        {
            paticle+=1;
            if (paticle<=5)
            {
                g_str_msg_+=boost::str(boost::format("paticle or peeling x:%d y:%d w:%d h:%d\n")
                                % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
        }
        else if (cluster.w_>=2 ||
                cluster.h_>=2)
        {
//            g_str_msg_+=boost::str(boost::format("cluster in \n"));

            if (true)
            {
                if (cluster.w_ == 2 &&
                        cluster.h_ == 4)
                {
                    if (cnt_2x4_pix < 5)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x4 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 4);
                    }
                    cnt_2x4_pix+=1;
                    cnts_cluster.insert(0,cnts_cluster.value(0)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 1)
                {
                    if (cnt_1x2_2x1_pix < 7)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x1 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % cluster.w_  % cluster.h_);
                    }
                    cnt_1x2_2x1_pix+=1;
                    cnts_cluster.insert(1,cnts_cluster.value(1)+1);
                }
                else if (cluster.w_ == 1 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255 &&
                         buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if (cnt_1x2_2x1_pix < 7)
                    {
                        g_str_msg_+=boost::str(boost::format("global 1x2 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) %  cluster.w_  % cluster.h_);
                    }
                    cnt_1x2_2x1_pix+=1;
                    cnts_cluster.insert(3,cnts_cluster.value(3)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==0
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==0)
                {
                    if (cnt_2x2_diagonal_pix < 3)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x2 diagonal cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    }
                    cnt_2x2_diagonal_pix+=1;

                    cnts_cluster.insert(5,cnts_cluster.value(5)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==0
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==0
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if (cnt_2x2_diagonal_pix < 3)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x2 diagonal cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    }
                    cnt_2x2_diagonal_pix+=1;

                    cnts_cluster.insert(6,cnts_cluster.value(6)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==0
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n1<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n1++;
                    cnts_cluster.insert(9,cnts_cluster.value(9)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==0
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n2<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n2++;
                    cnts_cluster.insert(10,cnts_cluster.value(10)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==0)
                {
                    if(n3<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n3++;
                    cnts_cluster.insert(11,cnts_cluster.value(11)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==0
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n4<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n4++;
                    cnts_cluster.insert(12,cnts_cluster.value(12)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n5<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 4WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n5++;
                    cnts_cluster.insert(13,cnts_cluster.value(13)+1);
                }
                else
                {
                    if(n6<3)
                        g_str_msg_+=boost::str(boost::format("global other cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % cluster.w_  % cluster.h_ );
                    n6++;
                    other_cluster++;
                    cnts_cluster.insert(14,cnts_cluster.value(14)+1);
                }
            }
            cnt_cluster+=1;
        }
    }


    uchar *buf_map_tmp_ch = new uchar[w*h];
    memcpy(buf_map_tmp_ch,buf_map,w*h*sizeof(uchar));

    std::vector<MPDInf> clusters_ch;
    defectCounter_channel (buf_map,w,h,128,clusters_ch);

    g_str_msg_+=boost::str(boost::format("cluster in channel cnt : %d\n")
                           % (clusters_ch.size()));

    BOOST_FOREACH(const MPDInf &cluster, clusters_ch)
    {
        if(cluster.w_ >=3 ||
                cluster.h_ >=3 )
        {
            if (cluster.w_ == 3 &&
                    cluster.h_ == 1 &&
                    buf_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                    buf_map_tmp_ch[cluster.y_*w+cluster.x_+1]==0 &&
                    buf_map_tmp_ch[cluster.y_*w+cluster.x_+2]==255)
            {
                cnt_2x1_ch+=1;

                if (cnt_2x1_ch<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x1_ch  x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(2,cnts_cluster.value(2)+1);
            }

            else if (cluster.w_ == 1 &&
                     cluster.h_ == 3 &&
                     buf_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                     buf_map_tmp_ch[(cluster.y_+1)*w+cluster.x_]==0 &&
                     buf_map_tmp_ch[(cluster.y_+2)*w+cluster.x_]==255)
            {
                cnt_1x2_ch+=1;

                if (cnt_1x2_ch<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_1x2_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(4,cnts_cluster.value(4)+1);
            }

            else if (cluster.w_ == 3 &&
                     cluster.h_ == 3 &&
                     buf_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                     buf_map_tmp_ch[(cluster.y_+1)*w+cluster.x_+1]==0 &&
                     buf_map_tmp_ch[(cluster.y_+2)*w+cluster.x_+2]==255)
            {
                cnt_2x2_leftdiagonal+=1;

                if (cnt_2x2_leftdiagonal<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x2_leftdiagonal_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(7,cnts_cluster.value(7)+1);
            }

            else if (cluster.w_ == 3 &&
                     cluster.h_ == 3 &&
                     buf_map_tmp_ch[cluster.y_*w+cluster.x_+2]==255 &&
                     buf_map_tmp_ch[(cluster.y_+1)*w+cluster.x_+1]==0 &&
                     buf_map_tmp_ch[(cluster.y_+2)*w+cluster.x_]==255)
            {
                cnt_2x2_rightdiagonal+=1;

                if (cnt_2x2_rightdiagonal<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x2_rightdiagonal_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(8,cnts_cluster.value(8)+1);
            }

            else
            {
                other_cluster_ch +=1;
                if (other_cluster_ch <=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_other_cluster_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

            }
            cnt_cluster+=1;
        }
        else
        {
            other_cluster_ch +=1;
            if (other_cluster_ch <=3)
            {
                g_str_msg_+=boost::str(boost::format("cnt_other_cluster_ch x:%d y:%d w:%d h:%d\n")
                                       % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
            cnt_cluster+=1;
        }
    }

    g_str_msg_+=boost::str(boost::format("cluster_ch Cnt:%d    other_cluster_ch Cnt:%d\n")
                           % (cnt_2x1_ch + cnt_1x2_ch + cnt_2x2_leftdiagonal +cnt_2x2_rightdiagonal) % other_cluster_ch) ;

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (buf_map[w*i+j] > 128)
            {cnt_white_pix++;
                cnt_dark_pix++;}
        }
    }
    g_str_msg_+=boost::str(boost::format("single pix %d :%d\n")
                    % (cnt_white_pix) % (cnt_dark_pix) );
    delete [] buf_map_tmp;
    delete [] buf_map_tmp_ch;
    int cnt =clusters.size() +clusters_ch.size();
    return cnt;
}


int AlgorithmFuncs::FindALLClusterBayer_DF(uchar *data, int w, int h,
                                           QMap<int,int> &cnts_cluster,
                                           int &cnt_2x4_pix,
                                           int &cnt_2x2_pix,
                                           int &cnt_1x2_2x1_pix,
                                           int &cnt_2x2_diagonal_pix,
                                           int &cnt_1x2_ch,
                                           int &cnt_2x1_ch,
                                           int &cnt_2x2_leftdiagonal,
                                           int &cnt_2x2_rightdiagonal,
                                           int &cnt_white_pix,
                                           int &cnt_dark_pix,
                                           int &paticle,
                                           int &other_cluster,
                                           int &other_cluster_ch,
                                           int r, double spec, int skipCnt,
                                           uchar *buf_map, std::string &g_str_msg_)
{
    cnt_2x4_pix = 0;
    cnt_1x2_2x1_pix = 0;
    cnt_2x2_pix = 0;
    cnt_2x2_diagonal_pix=0;
    cnt_1x2_ch=0;
    cnt_2x1_ch=0;
    cnt_2x2_leftdiagonal=0;
    cnt_2x2_rightdiagonal=0;
    cnt_white_pix=0;
    cnt_dark_pix=0;
    paticle=0;
    other_cluster =0;
    other_cluster_ch =0;

    int n1=0,n2=0,n3=0,n4=0,n5=0,n6=0;
    memset(buf_map,0,w*h);

    g_str_msg_+=boost::str(boost::format("spec : %f\n")
                    % spec);
    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        for (int j=skipCnt; j<w-skipCnt; ++j)
        {
            double mean_t = 0.0;
            int cnt = 0;

            int row_start = i-2*r;
            int row_end = i+2*r;
            if (row_start<0)
            {
                if(std::abs(row_start)%2==0)
                    row_start = 0;
                else
                    row_start =1;
                row_end = row_start+4*r;
            }
            else if (row_end>(h-1))
            {
                if(std::abs(row_end)%2==0)
                    row_end = h-2;
                else
                    row_end = h-1;
                row_start = row_end-4*r;
            }
            int col_start = j-2*r;
            int col_end = j+2*r;
            if (col_start<0)
            {
                if(std::abs(col_start)%2==0)
                    col_start = 0;
                else
                    col_start = 1;
                col_end = col_start+4*r;
            }
            else if (col_end>(w-1))
            {
                if(std::abs(col_end)%2==0)
                    col_end = w-2;
                else
                    col_end = w-1;
                col_start = col_end-4*r;
            }

            for (int l=row_start; l<=row_end; l+=2)
            {
                for (int m=col_start; m<=col_end; m+=2)
                {
                    if (l>=skipCnt && l<h-skipCnt &&
                            m>=skipCnt && m<w-skipCnt)
                    {
                        cnt+=1;
                        mean_t+=(double)data[w*l+m];
                    }
                }
            }
            if (cnt <= 1)
            {
                // this condition: impossible
                continue;
            }
            double mean_ex = mean_t-data[i*w+j];
            mean_ex/=(cnt-1);
            mean_t/=cnt;
            double compareValue = std::fabs((double)data[w*i+j]-mean_ex);
            if (compareValue>=spec)
            {
                buf_map[w*i+j] = 255;
            }
        }
    }
    uchar *buf_map_tmp = new uchar[w*h];
    memcpy(buf_map_tmp,buf_map,w*h*sizeof(uchar));
    //counter cluster
    std::vector<MPDInf> clusters;
    defectCounter (buf_map,w,h,128,clusters);
    int cnt_cluster = 0;
    g_str_msg_+=boost::str(boost::format("cluster cnt : %d\n")
                           % (clusters.size()));
    BOOST_FOREACH(const MPDInf &cluster, clusters)
    {
        if (cluster.w_>3 ||
                cluster.h_>5)
        {
            paticle+=1;
            if (paticle<=5)
            {
                g_str_msg_+=boost::str(boost::format("paticle or peeling x:%d y:%d w:%d h:%d\n")
                                % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
        }
        else if (cluster.w_>=2 ||
                cluster.h_>=2)
        {
//            g_str_msg_+=boost::str(boost::format("cluster in \n"));

            if (true)
            {
                if (cluster.w_ == 2 &&
                        cluster.h_ == 4)
                {
                    if (cnt_2x4_pix < 5)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x4 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 4);
                    }
                    cnt_2x4_pix+=1;
                    cnts_cluster.insert(0,cnts_cluster.value(0)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 1)
                {
                    if (cnt_1x2_2x1_pix < 7)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x1 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % cluster.w_  % cluster.h_);
                    }
                    cnt_1x2_2x1_pix+=1;
                    cnts_cluster.insert(1,cnts_cluster.value(1)+1);
                }
                else if (cluster.w_ == 1 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255 &&
                         buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if (cnt_1x2_2x1_pix < 7)
                    {
                        g_str_msg_+=boost::str(boost::format("global 1x2 cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) %  cluster.w_  % cluster.h_);
                    }
                    cnt_1x2_2x1_pix+=1;
                    cnts_cluster.insert(3,cnts_cluster.value(3)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==0
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==0)
                {
                    if (cnt_2x2_diagonal_pix < 3)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x2 diagonal cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    }
                    cnt_2x2_diagonal_pix+=1;

                    cnts_cluster.insert(5,cnts_cluster.value(5)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==0
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==0
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if (cnt_2x2_diagonal_pix < 3)
                    {
                        g_str_msg_+=boost::str(boost::format("global 2x2 diagonal cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    }
                    cnt_2x2_diagonal_pix+=1;

                    cnts_cluster.insert(6,cnts_cluster.value(6)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==0
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n1<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n1++;
                    cnts_cluster.insert(9,cnts_cluster.value(9)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==0
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n2<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n2++;
                    cnts_cluster.insert(10,cnts_cluster.value(10)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==0)
                {
                    if(n3<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n3++;
                    cnts_cluster.insert(11,cnts_cluster.value(11)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==0
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n4<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 3WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n4++;
                    cnts_cluster.insert(12,cnts_cluster.value(12)+1);
                }
                else if (cluster.w_ == 2 &&
                         cluster.h_ == 2 &&
                         buf_map_tmp[cluster.y_*w+cluster.x_]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_+1]==255
                         && buf_map_tmp[cluster.y_*w+cluster.x_+1]==255
                         && buf_map_tmp[(cluster.y_+1)*w+cluster.x_]==255)
                {
                    if(n5<3)
                        g_str_msg_+=boost::str(boost::format("global 2x2 4WP cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % 2 % 2);
                    cnt_2x2_pix ++;
                    n5++;
                    cnts_cluster.insert(13,cnts_cluster.value(13)+1);
                }
                else
                {
                    if(n6<3)
                        g_str_msg_+=boost::str(boost::format("global other cluster x:%d y:%d w:%d h:%d\n")
                                        % (cluster.x_) % (cluster.y_) % cluster.w_  % cluster.h_ );
                    n6++;
                    other_cluster++;
                    cnts_cluster.insert(14,cnts_cluster.value(14)+1);
                }
            }
            cnt_cluster+=1;
        }
    }


    uchar *buf_map_tmp_ch = new uchar[w*h];
    memcpy(buf_map_tmp_ch,buf_map,w*h*sizeof(uchar));

    std::vector<MPDInf> clusters_ch;
    defectCounter_channel (buf_map,w,h,128,clusters_ch);

    g_str_msg_+=boost::str(boost::format("cluster in channel cnt : %d\n")
                           % (clusters_ch.size()));

    BOOST_FOREACH(const MPDInf &cluster, clusters_ch)
    {
        if(cluster.w_ >=3 ||
                cluster.h_ >=3 )
        {
            if (cluster.w_ == 3 &&
                    cluster.h_ == 1 &&
                    buf_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                    buf_map_tmp_ch[cluster.y_*w+cluster.x_+1]==0 &&
                    buf_map_tmp_ch[cluster.y_*w+cluster.x_+2]==255)
            {
                cnt_2x1_ch+=1;

                if (cnt_2x1_ch<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x1_ch  x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(2,cnts_cluster.value(2)+1);
            }

            else if (cluster.w_ == 1 &&
                     cluster.h_ == 3 &&
                     buf_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                     buf_map_tmp_ch[(cluster.y_+1)*w+cluster.x_]==0 &&
                     buf_map_tmp_ch[(cluster.y_+2)*w+cluster.x_]==255)
            {
                cnt_1x2_ch+=1;

                if (cnt_1x2_ch<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_1x2_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(4,cnts_cluster.value(4)+1);
            }

            else if (cluster.w_ == 3 &&
                     cluster.h_ == 3 &&
                     buf_map_tmp_ch[cluster.y_*w+cluster.x_]==255 &&
                     buf_map_tmp_ch[(cluster.y_+1)*w+cluster.x_+1]==0 &&
                     buf_map_tmp_ch[(cluster.y_+2)*w+cluster.x_+2]==255)
            {
                cnt_2x2_leftdiagonal+=1;

                if (cnt_2x2_leftdiagonal<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x2_leftdiagonal_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(7,cnts_cluster.value(7)+1);
            }

            else if (cluster.w_ == 3 &&
                     cluster.h_ == 3 &&
                     buf_map_tmp_ch[cluster.y_*w+cluster.x_+2]==255 &&
                     buf_map_tmp_ch[(cluster.y_+1)*w+cluster.x_+1]==0 &&
                     buf_map_tmp_ch[(cluster.y_+2)*w+cluster.x_]==255)
            {
                cnt_2x2_rightdiagonal+=1;

                if (cnt_2x2_rightdiagonal<=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_2x2_rightdiagonal_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

                cnts_cluster.insert(8,cnts_cluster.value(8)+1);
            }

            else
            {
                other_cluster_ch +=1;
                if (other_cluster_ch <=3)
                {
                    g_str_msg_+=boost::str(boost::format("cnt_other_cluster_ch x:%d y:%d w:%d h:%d\n")
                                    % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
                }

            }
            cnt_cluster+=1;
        }
        else
        {
            other_cluster_ch +=1;
            if (other_cluster_ch <=3)
            {
                g_str_msg_+=boost::str(boost::format("cnt_other_cluster_ch x:%d y:%d w:%d h:%d\n")
                                       % (cluster.x_) % (cluster.y_) % cluster.w_ % cluster.h_);
            }
            cnt_cluster+=1;
        }
    }

    g_str_msg_+=boost::str(boost::format("cluster_ch Cnt:%d    other_cluster_ch Cnt:%d\n")
                           % (cnt_2x1_ch + cnt_1x2_ch + cnt_2x2_leftdiagonal +cnt_2x2_rightdiagonal) % other_cluster_ch) ;

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            if (buf_map[w*i+j] > 128)
            {cnt_white_pix++;
                cnt_dark_pix++;}
        }
    }
    cnt_white_pix = cnt_white_pix - cnt_1x2_2x1_pix/2 - cnt_2x2_diagonal_pix/2;
    cnt_dark_pix = cnt_white_pix;
    g_str_msg_+=boost::str(boost::format("single pix %d :%d\n")
                    % (cnt_white_pix) % (cnt_dark_pix) );
    delete [] buf_map_tmp;
    delete [] buf_map_tmp_ch;
    int cnt =clusters.size() +clusters_ch.size();
    return cnt;
}




bool AlgorithmFuncs::FindPOG(uchar *data, int skip, int w, int h,
                                QMap<int,int> &POG,
                                int r_kernel, int r_outer, double spec,
                                std::string &g_str_msg_)
{
    g_str_msg_+=boost::str(boost::format("POG spec : %f\n")
                           % spec);
    double mean_kernel;
    double mean_outer;
    int row_start_inner =0;
    int row_end_inner =0;
    int col_start_inner=0;
    int col_end_inner =0;
    int row_start_outer =0;
    int row_end_outer =0;
    int col_start_outer=0;
    int col_end_outer =0;
    POG.clear();
    int print_cnt =0;
    bool result = true;
    for (int i=skip+r_kernel; i<h-skip-r_kernel; i+=2*r_kernel)
    {
        for (int j=skip+r_outer; j<w-skip-r_outer; j+=2*r_kernel)
        {
            mean_kernel=0;
            mean_outer=0;
//-------------------Kernel---------------------
            if(true)
            {
                double mean_t = 0.0;
                int cnt = 0;

                row_start_inner = i-r_kernel;
                row_end_inner = i+r_kernel;
                if (row_start_inner<skip)
                {
                    if(std::abs(row_start_inner)%2==0)
                        row_start_inner = skip;
                    else
                        row_start_inner = skip+1;
                    row_end_inner = row_start_inner+2*r_kernel;
                }
                else if (row_end_inner>(h-1-skip))
                {
                    if(std::abs(row_end_inner)%2==0)
                        row_end_inner = h-2-skip;
                    else
                        row_end_inner = h-1-skip;
                    row_start_inner = row_end_inner-2*r_kernel;
                }
                col_start_inner = j-r_kernel;
                col_end_inner = j+r_kernel;
                if (col_start_inner<skip)
                {
                    if(std::abs(col_start_inner)%2==0)
                        col_start_inner = skip;
                    else
                        col_start_inner = skip+1;
                    col_end_inner = col_start_inner+2*r_kernel;
                }
                else if (col_end_inner>(w-1-skip))
                {
                    if(std::abs(col_end_inner)%2==0)
                        col_end_inner = w-2-skip;
                    else
                        col_end_inner = w-1-skip;
                    col_start_inner = col_end_inner-2*r_kernel;
                }

                for (int l=row_start_inner; l<=row_end_inner; l+=1)
                {
                    for (int m=col_start_inner; m<=col_end_inner; m+=1)
                    {
                        if (l>=0 && l<h &&
                                m>=0 && m<w)
                        {
                            cnt+=1;
                            mean_t+=(double)data[w*l+m];
                        }
                    }
                }
                if (cnt <= 1)
                {
                    // this condition: impossible
                    continue;
                }
                double mean_ex = mean_t-data[i*w+j];
                mean_ex/=(cnt-1);
                mean_kernel =mean_ex;
            }

//-------------------OUTER---------------------
            if(true)
            {
                double mean_t = 0.0;
                int cnt = 0;

                row_start_outer = i-r_kernel;
                row_end_outer = i+r_kernel;
                if (row_start_outer<skip)
                {
                    if(std::abs(row_start_outer)%2==0)
                        row_start_outer = skip;
                    else
                        row_start_outer = skip+1;
                    row_end_outer = row_start_outer+2*r_kernel;
                }
                else if (row_end_outer>(h-1-skip))
                {
                    if(std::abs(row_end_outer)%2==0)
                        row_end_outer = h-2-skip;
                    else
                        row_end_outer = h-1-skip;
                    row_start_outer = row_end_outer-2*r_kernel;
                }
                col_start_outer = j-r_outer;
                col_end_outer = j+r_outer;
                if (col_start_outer<skip)
                {
                    if(std::abs(col_start_outer)%2==0)
                        col_start_outer = skip;
                    else
                        col_start_outer = skip+1;
                    col_end_outer = col_start_outer+2*r_outer;
                }
                else if (col_end_outer>(w-1-skip))
                {
                    if(std::abs(col_end_outer)%2==0)
                        col_end_outer = w-2-skip;
                    else
                        col_end_outer = w-1-skip;
                    col_start_outer = col_end_outer-2*r_outer;
                }

                for (int l=row_start_outer; l<=row_end_outer; l+=1)
                {
                    for (int m=col_start_outer; m<=col_end_outer; m+=1)
                    {
                        if (l>=0 && l<h &&
                                m>=0 && m<w)
                        {
                            if(m>=col_start_inner && m<=col_end_inner)
                                continue;
                            cnt+=1;
                            mean_t+=(double)data[w*l+m];
                        }
                    }
                }
                if (cnt <= 1)
                {
                    // this condition: impossible
                    continue;
                }
                double mean_ex = mean_t-data[i*w+j];
                mean_ex/=(cnt-1);
                mean_outer =mean_ex;
            }

//----------------------------------------------------
            if(std::abs(mean_outer-mean_kernel)>mean_outer*spec)
            {
                result = false;
                if(print_cnt <=5)
                {
//                    POG.insert(j,i);
                    g_str_msg_+=boost::str(boost::format("find POG, position, X: %d Y: %d\n")
                                           % j % i);
                    print_cnt++;
                }
            }
        }
    }
    return result;
}



//bool AlgorithmFuncs::FindBlemish(uchar *data, int skip, int w, int h,
//                                QMap<int,int> &blemish,
//                                int r_kernel, int r_outer, double spec,
//                                std::string &g_str_msg_)
//{
//    g_str_msg_+=boost::str(boost::format("Blemish spec : %f\n")
//                           % spec);
//    double mean_kernel;
//    double mean_outer;
//    int row_start_inner =0;
//    int row_end_inner =0;
//    int col_start_inner=0;
//    int col_end_inner =0;
//    int row_start_outer =0;
//    int row_end_outer =0;
//    int col_start_outer=0;
//    int col_end_outer =0;
//    blemish.clear();
//    int print_cnt =0;
//    bool result = true;
//    for (int i=skip+r_kernel; i<h-skip-r_kernel; i++)
//    {
//        for (int j=skip+r_outer; j<w-skip-r_outer; j++)
//        {
//            mean_kernel=0;
//            mean_outer=0;
////-------------------Kernel---------------------
//            if(true)
//            {
//                double mean_t = 0.0;
//                int cnt = 0;

//                row_start_inner = i-r_kernel;
//                row_end_inner = i+r_kernel;
//                if (row_start_inner<skip)
//                {
//                    if(std::abs(row_start_inner)%2==0)
//                        row_start_inner = skip;
//                    else
//                        row_start_inner = skip+1;
//                    row_end_inner = row_start_inner+2*r_kernel;
//                }
//                else if (row_end_inner>(h-1-skip))
//                {
//                    if(std::abs(row_end_inner)%2==0)
//                        row_end_inner = h-2-skip;
//                    else
//                        row_end_inner = h-1-skip;
//                    row_start_inner = row_end_inner-2*r_kernel;
//                }
//                col_start_inner = j-r_kernel;
//                col_end_inner = j+r_kernel;
//                if (col_start_inner<skip)
//                {
//                    if(std::abs(col_start_inner)%2==0)
//                        col_start_inner = skip;
//                    else
//                        col_start_inner = skip+1;
//                    col_end_inner = col_start_inner+2*r_kernel;
//                }
//                else if (col_end_inner>(w-1-skip))
//                {
//                    if(std::abs(col_end_inner)%2==0)
//                        col_end_inner = w-2-skip;
//                    else
//                        col_end_inner = w-1-skip;
//                    col_start_inner = col_end_inner-2*r_kernel;
//                }

//                for (int l=row_start_inner; l<=row_end_inner; l+=1)
//                {
//                    for (int m=col_start_inner; m<=col_end_inner; m+=1)
//                    {
//                        if (l>=0 && l<h &&
//                                m>=0 && m<w)
//                        {
//                            cnt+=1;
//                            mean_t+=(double)data[w*l+m];
//                        }
//                    }
//                }
//                if (cnt <= 1)
//                {
//                    // this condition: impossible
//                    continue;
//                }
//                double mean_ex = mean_t-data[i*w+j];
//                mean_ex/=(cnt-1);
//                mean_kernel =mean_ex;
//            }

////-------------------OUTER---------------------
//            if(true)
//            {
//                double mean_t = 0.0;
//                int cnt = 0;

//                row_start_outer = i-r_kernel;
//                row_end_outer = i+r_kernel;
//                if (row_start_outer<skip)
//                {
//                    if(std::abs(row_start_outer)%2==0)
//                        row_start_outer = skip;
//                    else
//                        row_start_outer = skip+1;
//                    row_end_outer = row_start_outer+2*r_kernel;
//                }
//                else if (row_end_outer>(h-1-skip))
//                {
//                    if(std::abs(row_end_outer)%2==0)
//                        row_end_outer = h-2-skip;
//                    else
//                        row_end_outer = h-1-skip;
//                    row_start_outer = row_end_outer-2*r_kernel;
//                }
//                col_start_outer = j-r_outer;
//                col_end_outer = j+r_outer;
//                if (col_start_outer<skip)
//                {
//                    if(std::abs(col_start_outer)%2==0)
//                        col_start_outer = skip;
//                    else
//                        col_start_outer = skip+1;
//                    col_end_outer = col_start_outer+2*r_outer;
//                }
//                else if (col_end_outer>(w-1-skip))
//                {
//                    if(std::abs(col_end_outer)%2==0)
//                        col_end_outer = w-2-skip;
//                    else
//                        col_end_outer = w-1-skip;
//                    col_start_outer = col_end_outer-2*r_outer;
//                }

//                for (int l=row_start_outer; l<=row_end_outer; l+=1)
//                {
//                    for (int m=col_start_outer; m<=col_end_outer; m+=1)
//                    {
//                        if (l>=0 && l<h &&
//                                m>=0 && m<w)
//                        {
//                            if(m>=col_start_inner && m<=col_end_inner)
//                                continue;
//                            cnt+=1;
//                            mean_t+=(double)data[w*l+m];
//                        }
//                    }
//                }
//                if (cnt <= 1)
//                {
//                    // this condition: impossible
//                    continue;
//                }
//                double mean_ex = mean_t-data[i*w+j];
//                mean_ex/=(cnt-1);
//                mean_outer =mean_ex;
//            }

////----------------------------------------------------
//            if(std::abs(mean_outer-mean_kernel)>mean_outer*spec)
//            {
//                result = false;
//                if(print_cnt <=5)
//                {
////                    blemish.insert(j,i);
//                    g_str_msg_+=boost::str(boost::format("find Blemish, position, X: %d Y: %d\n")
//                                           % j % i);
//                    print_cnt++;
//                }
//            }
//        }
//    }
//    return result;
//}




bool AlgorithmFuncs::Find_wihtepix(uchar *data, int skip, int w, int h,
                                int wp_class1_spec, int wp_class2_spec,
                                int &wp_class1_cnt, int &wp_class2_cnt,
                                std::string &g_str_msg_)
{
    g_str_msg_+=boost::str(boost::format("wp class1 spec : %d, wp class2 spec: %d\n")
                           % wp_class1_spec % wp_class2_spec);
    for (int i=skip; i<h-skip; ++i)
    {
        for (int j=skip; j<w-skip; ++j)
        {
            if(data[i*w+j]>wp_class2_spec)
            {
                wp_class2_cnt ++;
                if(wp_class2_cnt<5)
                g_str_msg_+=boost::str(boost::format("find class2 whitepix, position, X: %d Y: %d\n")
                                       % j % i);
            }
            else if(data[i*w+j]>wp_class1_spec)
            {
                wp_class1_cnt ++;
                if(wp_class1_cnt<5)
                g_str_msg_+=boost::str(boost::format("find class1 whitepix, position, X: %d Y: %d\n")
                                       % j % i);
            }

        }
    }
    return true;
}



int AlgorithmFuncs::defectCounter_chao(uchar *map, int width, int height,
                                  int spec, std::vector<MPDInf> &clusters,
                                       std::vector<MPDInf> &clusters_ch)
{
    //寻找相邻坏点
    QVector<QPoint> map_pos_;
    for (int i=0; i<height; ++i)
    {
        for (int j=0; j<width; ++j)
        {
            if (map[width*i+j] > spec)
            {
                std::vector<std::pair<int,int> > posList; //QPair<i,j>
                int x_start = j;
                int x_end = j;
                int y_start = i;
                int y_end = i;
                int n=0;
                int neighbor_row =0;
                int neighbor_col =0;

                posList.push_back(std::pair<int,int>(i,j));
                map[width*i+j] = 0;

                while(posList.size() > 0)
                {
                    std::pair<int,int> pos(posList.back ());
                    posList.pop_back();
                    int ii = pos.first;
                    int jj = pos.second;
                    if (ii > y_end)
                    {
                        y_end = ii;
                    }
                    if (ii < y_start)
                    {
                        y_start = ii;
                    }
                    if (jj > x_end)
                    {
                        x_end = jj;
                    }
                    if (jj < x_start)
                    {
                        x_start = jj;
                    }

                    for (int l=ii-1; l<=ii+1; ++l)
                    {
                        for (int m=jj-1; m<=jj+1; ++m)
                        {
                            if (l>=0 && l<height &&
                                m>=0 && m<width &&
                                    map[width*l+m] > spec)
                            {
                                n++;
                                posList.push_back(std::pair<int,int>(l,m));
                                map[width*l+m] = 0;
                                neighbor_row = l;
                                neighbor_col = m;
                            }
                        }
                    }
                }
                if (n==0)
                {
                    map[width*i+j] = 255;
                    map_pos_.push_back(QPoint(i,j));
                }
                else if (n==1)
                {
                    map[width*i+j] = 255;
                    map[width*neighbor_row+neighbor_col] = 255;
                    map_pos_.push_back(QPoint(i,j));
                    map_pos_.push_back(QPoint(neighbor_row,neighbor_col));
                    clusters.push_back(MPDInf(x_start,y_start,
                                              x_end-x_start+1,y_end-y_start+1));
                }
                else
                    clusters.push_back(MPDInf(x_start,y_start,
                                              x_end-x_start+1,y_end-y_start+1));

            }
        }
    }

    //寻找同通道相邻坏点
    for(int count_=0;count_!=map_pos_.size();++count_)
    {
        std::vector<std::pair<int,int> > posList; //QPair<i,j>
        int i=map_pos_[count_].x();
        int j=map_pos_[count_].y();
        if (map[width*i+j] > spec)
        {
            int x_start =j;
            int x_end = j;
            int y_start = i;
            int y_end = i;
            int n=0;
            posList.push_back(std::pair<int,int>(i,j));
            //------入栈后就将非零标记清空，以免再次被计入
            map[width*i+j] = 0;

            while(posList.size() > 0)
            {
                std::pair<int,int> pos(posList.back ());
                posList.pop_back();
                int ii = pos.first;
                int jj = pos.second;
                if (ii > y_end)
                    y_end = ii;
                if (ii < y_start)
                    y_start = ii;
                if (jj > x_end)
                    x_end = jj;
                if (jj < x_start)
                    x_start = jj;

                for (int l=ii-2; l<=ii+2;l=l+2 )
                {
                    for (int m=jj-2; m<=jj+2; m=m+2)
                    {
                        if (l>=0 && l<height &&
                                m>=0 && m<width &&
                                map[width*l+m] > spec)
                        {
                            n++;
                            posList.push_back(std::pair<int,int>(l,m));
                            map[width*l+m] = 0;
                        }
                    }
                }
            }
            if(n==0)
                map[width*i+j] = 255;
            else
                clusters_ch.push_back(MPDInf(x_start,y_start,
                                             x_end-x_start+1,y_end-y_start+1));
        }
    }
    return clusters.size();
}




int AlgorithmFuncs::defectCounter(uchar *map, int width, int height,
                                  int spec, std::vector<MPDInf> &clusters)
{
    for (int i=0; i<height; ++i)
    {
        for (int j=0; j<width; ++j)
        {
            if (map[width*i+j] > spec)
            {
                std::vector<std::pair<int,int> > posList; //QPair<i,j>
                int x_start = j;
                int x_end = j;
                int y_start = i;
                int y_end = i;
                int n=0;
                int neighbor_row =0;
                int neighbor_col =0;

                posList.push_back(std::pair<int,int>(i,j));
                map[width*i+j] = 0;

                while(posList.size() > 0)
                {
                    std::pair<int,int> pos(posList.back ());
                    posList.pop_back();
                    int ii = pos.first;
                    int jj = pos.second;
                    if (ii > y_end)
                    {
                        y_end = ii;
                    }
                    if (ii < y_start)
                    {
                        y_start = ii;
                    }
                    if (jj > x_end)
                    {
                        x_end = jj;
                    }
                    if (jj < x_start)
                    {
                        x_start = jj;
                    }

                    for (int l=ii-1; l<=ii+1; ++l)
                    {
                        for (int m=jj-1; m<=jj+1; ++m)
                        {
                            if (l>=0 && l<height &&
                                m>=0 && m<width &&
                                    map[width*l+m] > spec)
                            {
                                n++;
                                posList.push_back(std::pair<int,int>(l,m));
                                map[width*l+m] = 0;
                                neighbor_row = l;
                                neighbor_col = m;
                            }
                        }
                    }
                }
                if (n==0)
                    map[width*i+j] = 255;
                else if (n==1)
                {
                    map[width*i+j] = 255;
                    map[width*neighbor_row+neighbor_col] = 255;
                    clusters.push_back(MPDInf(x_start,y_start,
                                              x_end-x_start+1,y_end-y_start+1));
                }
                else
                    clusters.push_back(MPDInf(x_start,y_start,
                                              x_end-x_start+1,y_end-y_start+1));
            }
        }
    }
    return clusters.size();
}



int AlgorithmFuncs::defectCounter_channel(uchar *map, int width, int height,
                                  int spec, std::vector<MPDInf> &clusters)
{
    for (int i=0; i<height; ++i)
    {
        for (int j=0; j<width; ++j)
        {
            if (map[width*i+j] > spec)
            {
                std::vector<std::pair<int,int> > posList; /*QPair<i,j>*/
                int x_start = j;
                int x_end = j;
                int y_start = i;
                int y_end = i;
                int n=0;
                posList.push_back(std::pair<int,int>(i,j));
                //------入栈后就将非零标记清空，以免再次被计入
                map[width*i+j] = 0;

                while(posList.size() > 0)
                {
                    std::pair<int,int> pos(posList.back ());
                    posList.pop_back();
                    int ii = pos.first;
                    int jj = pos.second;
                    if (ii > y_end)
                    {
                        y_end = ii;
                    }
                    if (ii < y_start)
                    {
                        y_start = ii;
                    }
                    if (jj > x_end)
                    {
                        x_end = jj;
                    }
                    if (jj < x_start)
                    {
                        x_start = jj;
                    }

                    for (int l=ii-2; l<=ii+2;l=l+2 )
                    {
                        for (int m=jj-2; m<=jj+2; m=m+2)
                        {
                            if (l>=0 && l<height &&
                                    m>=0 && m<width &&
                                    map[width*l+m] > spec)
                            {
                                n++;
                                posList.push_back(std::pair<int,int>(l,m));
                                map[width*l+m] = 0;
                            }
                        }
                    }
                }
                if(n==0)
                    map[width*i+j] = 255;
                else
                    clusters.push_back(MPDInf(x_start,y_start,
                                              x_end-x_start+1,y_end-y_start+1));
            }
        }
    }
    return clusters.size();
}




int AlgorithmFuncs::Find_Edge_DeadRow_by_channel (Spec_Mode mode,
                                                  uchar *pData, int w, int h,
                                                  int r, int stripCnt,
                                                  double spec_line,
                                                  double spec_dol,
                                                  int &cnt_dol,
                                                  double &max_diff,
                                                  int &max_pos,
                                                  double &row_max_min_diff,
                                                  std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_row_;
    int N = w/2;
    for (int i=0; i<h; ++i)
    {
        double mean_t = 0.0;
        for (int j=0; j<N; ++j)
        {
            mean_t+=(double)(pData[w*i+2*j]);
            mean_t+=(double)(pData[w*i+2*j+1]);
        }
        each_row_.push_back (mean_t/(2*N));
    }

    double max_row = *std::max_element(each_row_.begin(),each_row_.end());
    double min_row = *std::min_element(each_row_.begin(),each_row_.end());
    row_max_min_diff = max_row-min_row;
    msg+=boost::str(boost::format("Max row:%.4f Min row:%.4f Max-Min:%.4f\n")
                    % max_row % min_row % row_max_min_diff);

    for (int i=0; i<stripCnt/2; ++i)
    {
        double mean_down = each_row_.at (i);

        if (i+r<each_row_.size())
        {
            int cnt_t1 = 0;
            mean_down = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_down+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_down/=cnt_t1;
        }
        double dif_down= std::abs (each_row_.at (i)-mean_down);

        double diff=0;

        if(mean_down==0)
        {
            dif_down =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                diff  = dif_down/mean_down;
            }
            else if(mode==DN)
            {
                diff = dif_down;

            }
        }

        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d   dif_down:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        std::vector<int> row_data;
        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(row_data);
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }



    for (int i=each_row_.size()-stripCnt/2; i<each_row_.size(); ++i)
    {
        double mean_top = each_row_.at (i);

        if (i-r>0)
        {
            int cnt_t1 = 0;
            mean_top = 0.0;
            for (int index = i-r; index<i; ++index)
            {
                mean_top+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_top/=cnt_t1;
        }
        double dif_top= std::abs (each_row_.at (i)-mean_top);

        double diff=0;
        if(mode==Ratio)
        {
            diff  = dif_top/mean_top;
        }
        else if(mode==DN)
        {
            diff = dif_top;

        }
        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d  dif_top:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        std::vector<int> row_data;
        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(row_data);
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }

    return cnt_lines;
}






int AlgorithmFuncs::Find_Edge_DeadCol_by_channel (Spec_Mode mode,
                                                  uchar *pData, int w, int h,
                                                  int r, int stripCnt,
                                                  double spec_line,
                                                  double spec_dol,
                                                  int &cnt_dol,
                                                  double &max_diff,
                                                  int &max_pos,
                                                  double &col_max_min_diff,
                                                  std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_col_;
    int N = h/2;
    for (int j=0; j<w; ++j)
    {
        double mean_t = 0.0;
        for (int i=0; i<N; ++i)
        {
            mean_t+=(double)(pData[w*2*i+j]);
            mean_t+=(double)(pData[w*(2*i+1)+j]);
        }

        each_col_.push_back (mean_t/(2*N));
    }


    double max_col = *std::max_element(each_col_.begin(),each_col_.end());
    double min_col = *std::min_element(each_col_.begin(),each_col_.end());
    col_max_min_diff = max_col-min_col;
    msg+=boost::str(boost::format("Max col:%.4f Min col:%.4f Max-Min:%.4f\n")
                    % max_col % min_col % col_max_min_diff);


    for (int i=0; i<stripCnt/2; ++i)
    {
        double mean_right = each_col_.at (i);

        if (i+r<each_col_.size())
        {
            int cnt_t1 = 0;
            mean_right = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_right+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_right/=cnt_t1;
        }
        double dif_right= std::abs (each_col_.at (i)-mean_right);

        double diff=0;

        if(mean_right==0)
        {
            dif_right =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                diff = dif_right/mean_right;
            }
            else if(mode==DN)
            {
                diff = dif_right;
            }
        }

        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d   dif_right:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        std::vector<int> col_data;
        int index_col = i;
        for (int index_row=0; index_row<h; ++index_row)
            col_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(col_data);
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }



    for (int i=each_col_.size()-stripCnt/2; i<each_col_.size(); ++i)
    {
        double mean_left = each_col_.at (i);

        if (i-r>0)
        {
            int cnt_t1 = 0;
            mean_left = 0.0;
            for (int index = i-r; index<i; ++index)
            {
                mean_left+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_left/=cnt_t1;
        }
        double dif_left= std::abs (each_col_.at (i)-mean_left);

        double diff=0;
        if(mode==Ratio)
        {
            diff  = dif_left/mean_left;
        }
        else if(mode==DN)
        {
            diff = dif_left;

        }
        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d  dif_left:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        std::vector<int> col_data;
        int index_col = i;
        for (int index_row=0; index_row<h; ++index_row)
            col_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(col_data);
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }

    return cnt_lines;
}



int AlgorithmFuncs::FindDeadRow_by_channel_add_breakline (Spec_Mode mode,
                                                          uchar *pData, int w, int h,
                                                          int r, int stripCnt,
                                                          double spec_line,
                                                          double spec_dol,
                                                          int &cnt_dol,
                                                          double &max_diff,
                                                          int &max_pos,
                                                          double &row_max_min_diff,
                                                          double break_line_spec,
                                                          int &break_line_cnt,
                                                          double &break_line_max_value,
                                                          int &break_line_max_pos,
                                                          std::string &msg,
                                                          QVector<double> &delta_each_row)
{
    int cnt_lines = 0;
    std::vector<double> each_row_;
    int N = w/2;

    std::vector<double> each_row_half1_;
    std::vector<double> each_row_half2_;


    for (int i=0; i<h; ++i)
    {
        double mean_t = 0.0;
        double mean_t_half1 =0.0;
        double mean_t_half2 =0.0;

        for (int j=0; j<N; ++j)
        {
            mean_t+=(double)(pData[w*i+2*j]);
            mean_t+=(double)(pData[w*i+2*j+1]);
            if(j<(N>>1))
                mean_t_half1 = mean_t_half1 + (double)(pData[w*i+2*j]) + (double)(pData[w*i+2*j+1]);
            else
                mean_t_half2 = mean_t_half2 + (double)(pData[w*i+2*j]) + (double)(pData[w*i+2*j+1]);
        }
        each_row_.push_back (mean_t/(2*N));
        each_row_half1_.push_back (mean_t_half1/N);
        each_row_half2_.push_back (mean_t_half2/N);
    }


    QVector<double> delta_each_row_half1;
    QVector<double> delta_each_row_half2;

    for(int i=0; i<each_row_half1_.size()-2; i++)
    {
        double delta1 =each_row_half1_.at(i+1)-each_row_half1_.at(i);
        double delta2 =each_row_half1_.at(i+2)-each_row_half1_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_row_half1.push_back(delta);
    }

    for(int i=0; i<delta_each_row_half1.size(); i++)
    {
        if(delta_each_row_half1.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half1 = *std::max_element(delta_each_row_half1.begin(),delta_each_row_half1.end());
    double break_line_max_pos_half1 = delta_each_row_half1.indexOf(break_line_max_value);



    for(int i=0; i<each_row_half2_.size()-2; i++)
    {
        double delta1 =each_row_half2_.at(i+1)-each_row_half2_.at(i);
        double delta2 =each_row_half2_.at(i+2)-each_row_half2_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_row_half2.push_back(delta);
    }

    for(int i=0; i<delta_each_row_half2.size(); i++)
    {
        if(delta_each_row_half2.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half2 = *std::max_element(delta_each_row_half2.begin(),delta_each_row_half2.end());
    double break_line_max_pos_half2 = delta_each_row_half2.indexOf(break_line_max_value);


    if (break_line_max_value_half1 >break_line_max_value_half2)
    {
        break_line_max_value = break_line_max_value_half1;
        break_line_max_pos = break_line_max_pos_half1;
        delta_each_row=delta_each_row_half1;
    }
    else
    {
        break_line_max_value = break_line_max_value_half2;
        break_line_max_pos = break_line_max_pos_half2;
        delta_each_row=delta_each_row_half2;
    }


    double max_row = *std::max_element(each_row_.begin(),each_row_.end());
    double min_row = *std::min_element(each_row_.begin(),each_row_.end());
    row_max_min_diff = max_row-min_row;
    msg+=boost::str(boost::format("Max row:%.4f Min row:%.4f Max-Min:%.4f\n")
                    % max_row % min_row % row_max_min_diff);

    for (int i=stripCnt/2; i<each_row_.size ()-stripCnt/2; ++i)
    {
        double mean_top = each_row_.at (i);
        double mean_down = each_row_.at (i);
        if (i-r>=stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_top = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_top+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_top/=cnt_t1;
        }
        if (i+r<each_row_.size ()-stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_down = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_down+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_down/=cnt_t1;
        }
        double dif_top = std::abs (each_row_.at (i)-mean_top);
        double dif_down= std::abs (each_row_.at (i)-mean_down);

        double dif_top_ratio=0;
        double dif_down_ratio=0;
        double diff=0;

        if(mean_top==0||mean_down==0)
        {
            dif_top_ratio =-1;
            dif_down_ratio =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_top_ratio = dif_top/mean_top;
                dif_down_ratio = dif_down/mean_down;
                diff = std::max(dif_top_ratio,dif_down_ratio);
            }
            else if(mode==DN)
            {
                dif_top_ratio = dif_top;
                dif_down_ratio = dif_down;
                diff = std::max(dif_top_ratio,dif_down_ratio);
            }
        }
        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d dif_top:%.2f dif_down:%.2f\n")
                                % (i*2) % dif_top_ratio % dif_down_ratio);
            }
        }

        std::vector<int> row_data;
        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(row_data);
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }
    return cnt_lines;
}




int AlgorithmFuncs::FindDeadCol_by_channel_add_breakline  (Spec_Mode mode,
                                                           uchar *pData, int w, int h,
                                                           int r, int stripCnt,
                                                           double spec_line,
                                                           double spec_dol,
                                                           int &cnt_dol,
                                                           double &max_diff,
                                                           int &max_pos,
                                                           double &col_max_min_diff,
                                                           double break_line_spec,
                                                           int &break_line_cnt,
                                                           double &break_line_max_value,
                                                           int &break_line_max_pos,
                                                           std::string &msg,
                                                           QVector<double> &delta_each_col)
{
    int cnt_lines = 0;
    std::vector<double> each_col_;
    int N = h/2;

    std::vector<double> each_col_half1_;
    std::vector<double> each_col_half2_;


    for (int j=0; j<w; ++j)
    {
        double mean_t = 0.0;
        double mean_t_half1 =0.0;
        double mean_t_half2 =0.0;

        for (int i=0; i<N; ++i)
        {
            mean_t+=(double)(pData[w*2*i+j]);
            mean_t+=(double)(pData[w*(2*i+1)+j]);
            if(i<(N>>1))
                mean_t_half1 = mean_t_half1 + (double)(pData[w*2*i+j]) + (double)(pData[w*(2*i+1)+j]);
            else
                mean_t_half2 = mean_t_half2 + (double)(pData[w*2*i+j]) + (double)(pData[w*(2*i+1)+j]);
        }
        each_col_.push_back (mean_t/(2*N));
        each_col_half1_.push_back (mean_t_half1/N);
        each_col_half2_.push_back (mean_t_half2/N);
    }


    QVector<double> delta_each_col_half1;
    QVector<double> delta_each_col_half2;

    for(int i=0; i<each_col_half1_.size()-2; i++)
    {
        double delta1 =each_col_half1_.at(i+1)-each_col_half1_.at(i);
        double delta2 =each_col_half1_.at(i+2)-each_col_half1_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_col_half1.push_back(delta);
    }

    for(int i=0; i<delta_each_col_half1.size(); i++)
    {
        if(delta_each_col_half1.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half1 = *std::max_element(delta_each_col_half1.begin(),delta_each_col_half1.end());
    double break_line_max_pos_half1 = delta_each_col_half1.indexOf(break_line_max_value);



    for(int i=0; i<each_col_half2_.size()-2; i++)
    {
        double delta1 =each_col_half2_.at(i+1)-each_col_half2_.at(i);
        double delta2 =each_col_half2_.at(i+2)-each_col_half2_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_col_half2.push_back(delta);
    }

    for(int i=0; i<delta_each_col_half2.size(); i++)
    {
        if(delta_each_col_half2.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half2 = *std::max_element(delta_each_col_half2.begin(),delta_each_col_half2.end());
    double break_line_max_pos_half2 = delta_each_col_half2.indexOf(break_line_max_value);


    if (break_line_max_value_half1 >break_line_max_value_half2)
    {
        break_line_max_value = break_line_max_value_half1;
        break_line_max_pos = break_line_max_pos_half1;
        delta_each_col=delta_each_col_half1;
    }
    else
    {
        break_line_max_value = break_line_max_value_half2;
        break_line_max_pos = break_line_max_pos_half2;
        delta_each_col=delta_each_col_half2;
    }


    double max_col = *std::max_element(each_col_.begin(),each_col_.end());
    double min_col = *std::min_element(each_col_.begin(),each_col_.end());
    col_max_min_diff = max_col-min_col;
    msg+=boost::str(boost::format("Max col:%.4f Min col:%.4f Max-Min:%.4f\n")
                    % max_col % min_col % col_max_min_diff);


    for (int i=stripCnt/2; i<each_col_.size ()-stripCnt/2; ++i)
    {
        double mean_left = each_col_.at (i);
        double mean_right = each_col_.at (i);
        if (i-r>=stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_left = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_left+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_left/=cnt_t1;
        }
        if (i+r<each_col_.size ()-stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_right = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_right+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_right/=cnt_t1;
        }
        double dif_left = std::abs (each_col_.at (i)-mean_left);
        double dif_right= std::abs (each_col_.at (i)-mean_right);


        double dif_left_ratio=0;
        double dif_right_ratio=0;
        double diff=0;

        if(mean_left==0||mean_right==0)
        {
            dif_left_ratio =-1;
            dif_right_ratio =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_left_ratio = dif_left/mean_left;
                dif_right_ratio = dif_right/mean_right;
                diff = std::max(dif_left_ratio,dif_right_ratio);
            }
            else if (mode==DN)
            {
                dif_left_ratio = dif_left;
                dif_right_ratio = dif_right;
                diff = std::max(dif_left_ratio,dif_right_ratio);
            }
        }

        if( diff > max_diff)
        {
                max_diff = diff;
                max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;
            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d dif_left:%.2f dif_right:%.2f\n")
                                % (i*2) % dif_left_ratio % dif_right_ratio);
            }
        }
        std::vector<int> col_data;
        int index_col = i;
        for (int index_row=0; index_row<h; ++index_row)
            col_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(col_data);
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }
    return cnt_lines;
}







int AlgorithmFuncs::FindDeadRow_by_channel (Spec_Mode mode,
                                            uchar *pData, int w, int h,
                                            int r, int stripCnt,
                                            double spec_line,
                                            double spec_dol,
                                            int &cnt_dol,
                                            double &max_diff,
                                            int &max_pos,
                                            double &row_max_min_diff,
                                            std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_row_;
    int N = w/2;
    for (int i=0; i<h; ++i)
    {
        double mean_t = 0.0;
        for (int j=0; j<N; ++j)
        {
            mean_t+=(double)(pData[w*i+2*j]);
            mean_t+=(double)(pData[w*i+2*j+1]);
        }
        each_row_.push_back (mean_t/(2*N));
    }

    double max_row = *std::max_element(each_row_.begin(),each_row_.end());
    double min_row = *std::min_element(each_row_.begin(),each_row_.end());
    row_max_min_diff = max_row-min_row;
    msg+=boost::str(boost::format("Max row:%.4f Min row:%.4f Max-Min:%.4f\n")
                    % max_row % min_row % row_max_min_diff);

    for (int i=stripCnt/2; i<each_row_.size ()-stripCnt/2; ++i)
    {
        double mean_top = each_row_.at (i);
        double mean_down = each_row_.at (i);
        if (i-r>=stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_top = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_top+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_top/=cnt_t1;
        }
        if (i+r<each_row_.size ()-stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_down = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_down+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_down/=cnt_t1;
        }
        double dif_top = std::abs (each_row_.at (i)-mean_top);
        double dif_down= std::abs (each_row_.at (i)-mean_down);

        double dif_top_ratio=0;
        double dif_down_ratio=0;
        double diff=0;

        if(mean_top==0||mean_down==0)
        {
            dif_top_ratio =-1;
            dif_down_ratio =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_top_ratio = dif_top/mean_top;
                dif_down_ratio = dif_down/mean_down;
                diff = std::max(dif_top_ratio,dif_down_ratio);
            }
            else if(mode==DN)
            {
                dif_top_ratio = dif_top;
                dif_down_ratio = dif_down;
                diff = std::max(dif_top_ratio,dif_down_ratio);
            }
        }

        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d dif_top:%.2f dif_down:%.2f\n")
                                % (i*2) % dif_top_ratio % dif_down_ratio);
            }
        }

        std::vector<int> row_data;
        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(row_data);
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }
    return cnt_lines;
}


int AlgorithmFuncs::FindDeadCol_by_channel (Spec_Mode mode,
                                            uchar *pData, int w, int h,
                                            int r, int stripCnt,
                                            double spec_line,
                                            double spec_dol,
                                            int &cnt_dol,
                                            double &max_diff,
                                            int &max_pos,
                                            double &col_max_min_diff,
                                            std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_col_;
    int N = h/2;
    for (int j=0; j<w; ++j)
    {
        double mean_t = 0.0;
        for (int i=0; i<N; ++i)
        {
            mean_t+=(double)(pData[w*2*i+j]);
            mean_t+=(double)(pData[w*(2*i+1)+j]);
        }

        each_col_.push_back (mean_t/(2*N));
    }


    double max_col = *std::max_element(each_col_.begin(),each_col_.end());
    double min_col = *std::min_element(each_col_.begin(),each_col_.end());
    col_max_min_diff = max_col-min_col;
    msg+=boost::str(boost::format("Max col:%.4f Min col:%.4f Max-Min:%.4f\n")
                    % max_col % min_col % col_max_min_diff);


    for (int i=stripCnt/2; i<each_col_.size ()-stripCnt/2; ++i)
    {
        double mean_left = each_col_.at (i);
        double mean_right = each_col_.at (i);
        if (i-r>=stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_left = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_left+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_left/=cnt_t1;
        }
        if (i+r<each_col_.size ()-stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_right = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_right+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_right/=cnt_t1;
        }
        double dif_left = std::abs (each_col_.at (i)-mean_left);
        double dif_right= std::abs (each_col_.at (i)-mean_right);


        double dif_left_ratio=0;
        double dif_right_ratio=0;
        double diff=0;

        if(mean_left==0||mean_right==0)
        {
            dif_left_ratio =-1;
            dif_right_ratio =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_left_ratio = dif_left/mean_left;
                dif_right_ratio = dif_right/mean_right;
                diff = std::max(dif_left_ratio,dif_right_ratio);
            }
            else if (mode==DN)
            {
                dif_left_ratio = dif_left;
                dif_right_ratio = dif_right;
                diff = std::max(dif_left_ratio,dif_right_ratio);
            }
        }

        if( diff > max_diff)
        {
                max_diff = diff;
                max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;
            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d dif_left:%.2f dif_right:%.2f\n")
                                % (i*2) % dif_left_ratio % dif_right_ratio);
            }
        }
        std::vector<int> col_data;
        int index_col = i;
        for (int index_row=0; index_row<h; ++index_row)
            col_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(col_data);
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }
    return cnt_lines;
}





int AlgorithmFuncs::FindDeadCol_bright_global_byDN (uchar *pData, int w, int h,
                                        int r, int stripCnt,
                                        double spec_line,
                                        double spec_dol,
                                        int &cnt_dol,
                                        double &max_diff,
                                        int &max_pos,
                                        std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_col_;
    for (int j=0; j<w; ++j)
    {
        double mean_t = 0.0;
        for (int i=0; i<h; ++i)
        {
            mean_t+=(double)(pData[w*i+j]);
        }

        each_col_.push_back (mean_t/h);
    }
    for (int i=stripCnt; i<each_col_.size ()-stripCnt; ++i)
    {
        double mean_left = each_col_.at (i);
        double mean_right = each_col_.at (i);
        if (i-r>=stripCnt)
        {
            int cnt_t1 = 0;
            mean_left = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_left+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_left/=cnt_t1;
        }
        if (i+r<each_col_.size ()-stripCnt)
        {
            int cnt_t1 = 0;
            mean_right = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_right+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_right/=cnt_t1;
        }
        double dif_left = std::abs (each_col_.at (i)-mean_left);
        double dif_right= std::abs (each_col_.at (i)-mean_right);


        double diff = std::max(dif_left,dif_right);
        if( diff > max_diff)
        {
                max_diff = diff;
                max_pos = i;
        }

        if (dif_left>spec_line||
                dif_right>spec_line)
        {
            std::vector<int> col_data;
            int index_col = i;
            for (int index_row=0; index_row<h; ++index_row)
                col_data.push_back (pData[w*index_row+index_col]);
            double std_dol = GetDatasStd<int>(col_data);
            if (std_dol>spec_dol)
            {//dol
                cnt_dol+=1;
                if (cnt_dol<5)
                    msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                    % i % std_dol);
            }
            else
            {
                cnt_lines+=1;
                if (cnt_lines<11)
                {
                    msg+=boost::str(boost::format("dead col,col:%d dif_left:%.3f DN dif_right:%.3f DN\n")
                                    % i % dif_left % dif_right);
                }
            }
        }
    }
    return cnt_lines;
}





int AlgorithmFuncs::FindDeadRow_bright_global_byDN (uchar *pData, int w, int h,
                                                int r, int stripCnt,
                                                double spec,
                                                double &max_diff,
                                                int &max_pos,
                                                std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_row_;
    for (int i=0; i<h; ++i)
    {
        double mean_t = 0.0;
        for (int j=0; j<w; ++j)
        {
            mean_t+=(double)(pData[w*i+j]);
        }
            each_row_.push_back (mean_t/w);
    }
    for (int i=stripCnt; i<each_row_.size ()-stripCnt; ++i)
    {
        double mean_top = each_row_.at (i);
        double mean_down = each_row_.at (i);
        if (i-r>=stripCnt)
        {
            int cnt_t1 = 0;
            mean_top = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_top+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_top/=cnt_t1;
        }
        if (i+r<each_row_.size ()-stripCnt)
        {
            int cnt_t1 = 0;
            mean_down = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_down+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_down/=cnt_t1;
        }
        double dif_top = std::abs (each_row_.at (i)-mean_top);
        double dif_down= std::abs (each_row_.at (i)-mean_down);

        double diff = std::max(dif_top,dif_down);
        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i;
        }

        if (dif_top>spec ||
                dif_down>spec)
        {
            cnt_lines+=1;

            if (cnt_lines<11)
            {
                msg+=boost::str(boost::format("dead row,row:%d dif_top:%.3f DN dif_down:%.3f DN\n")
                                % i % dif_top % dif_down);
            }
        }
    }
    return cnt_lines;
}



int AlgorithmFuncs::find_row_defect_bychannel (Spec_Mode mode,
                                               uchar *pData, int w, int h,
                                               int r, int stripCnt,
                                               double spec_line,
                                               double spec_dol,
                                               int &cnt_dol,
                                               double &max_diff,
                                               int &max_pos,
                                               double &row_max_min_diff,
                                               double &dol_std_max,
                                               int &dol_std_max_pos,
                                               double break_line_spec,
                                               int &break_line_cnt,
                                               double &break_line_max_value,
                                               int &break_line_max_pos,
                                               std::string &msg,
                                               QVector<double> &mean_each_row,
                                               QVector<double> &delta_each_row,
                                               QVector<double> &diff_each_row)
{
    int cnt_lines = 0;
    std::vector<double> each_row_;
    int N = w/2;

    std::vector<double> each_row_half1_;
    std::vector<double> each_row_half2_;


    for (int i=0; i<h; ++i)
    {
        double mean_t = 0.0;
        double mean_t_half1 =0.0;
        double mean_t_half2 =0.0;

        for (int j=0; j<N; ++j)
        {
            mean_t+=(double)(pData[w*i+2*j]);
            mean_t+=(double)(pData[w*i+2*j+1]);
            if(j<(N>>1))
                mean_t_half1 = mean_t_half1 + (double)(pData[w*i+2*j]) + (double)(pData[w*i+2*j+1]);
            else
                mean_t_half2 = mean_t_half2 + (double)(pData[w*i+2*j]) + (double)(pData[w*i+2*j+1]);
        }
        double tmp = mean_t/(2*N);
        mean_each_row.push_back(tmp);
        each_row_.push_back (tmp);
        each_row_half1_.push_back (mean_t_half1/N);
        each_row_half2_.push_back (mean_t_half2/N);
    }

    QVector<double> delta_each_row_half1;
    QVector<double> delta_each_row_half2;

    for(int i=0; i<each_row_half1_.size()-2; i++)
    {
        double delta1 =each_row_half1_.at(i+1)-each_row_half1_.at(i);
        double delta2 =each_row_half1_.at(i+2)-each_row_half1_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_row_half1.push_back(delta);
    }

    for(int i=0; i<delta_each_row_half1.size(); i++)
    {
        if(delta_each_row_half1.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half1 = *std::max_element(delta_each_row_half1.begin(),delta_each_row_half1.end());
    double break_line_max_pos_half1 = delta_each_row_half1.indexOf(break_line_max_value_half1);



    for(int i=0; i<each_row_half2_.size()-2; i++)
    {
        double delta1 =each_row_half2_.at(i+1)-each_row_half2_.at(i);
        double delta2 =each_row_half2_.at(i+2)-each_row_half2_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_row_half2.push_back(delta);
    }

    for(int i=0; i<delta_each_row_half2.size(); i++)
    {
        if(delta_each_row_half2.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half2 = *std::max_element(delta_each_row_half2.begin(),delta_each_row_half2.end());
    double break_line_max_pos_half2 = delta_each_row_half2.indexOf(break_line_max_value_half2);


    if (break_line_max_value_half1 >break_line_max_value_half2)
    {
        break_line_max_value = break_line_max_value_half1;
        break_line_max_pos = 2*break_line_max_pos_half1;
        delta_each_row=delta_each_row_half1;
    }
    else
    {
        break_line_max_value = break_line_max_value_half2;
        break_line_max_pos = 2*break_line_max_pos_half2;
        delta_each_row=delta_each_row_half2;
    }


    double max_row = *std::max_element(each_row_.begin(),each_row_.end());
    double min_row = *std::min_element(each_row_.begin(),each_row_.end());
    row_max_min_diff = max_row-min_row;
    msg+=boost::str(boost::format("Max row:%.4f Min row:%.4f Max-Min:%.4f\n")
                    % max_row % min_row % row_max_min_diff);

    for (int i=stripCnt/2; i<each_row_.size ()-stripCnt/2; ++i)
    {
        double mean_top = each_row_.at (i);
        double mean_down = each_row_.at (i);
        if (i-r>=stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_top = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_top+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_top/=cnt_t1;
        }
        if (i+r<each_row_.size ()-stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_down = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_down+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_down/=cnt_t1;
        }
        double dif_top = std::abs (each_row_.at (i)-mean_top);
        double dif_down= std::abs (each_row_.at (i)-mean_down);

        double dif_top_ratio=0;
        double dif_down_ratio=0;
        double diff=0;

        if(mean_top==0||mean_down==0)
        {
            dif_top_ratio =-1;
            dif_down_ratio =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_top_ratio = dif_top/mean_top;
                dif_down_ratio = dif_down/mean_down;
                diff = std::max(dif_top_ratio,dif_down_ratio);
            }
            else if(mode==DN)
            {
                dif_top_ratio = dif_top;
                dif_down_ratio = dif_down;
                diff = std::max(dif_top_ratio,dif_down_ratio);
            }
        }

        diff_each_row.append(diff);

        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d dif_top:%.2f dif_down:%.2f\n")
                                % (i*2) % dif_top_ratio % dif_down_ratio);
            }
        }

        std::vector<int> row_data;
        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(row_data);
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_row*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (index_row*2) % std_dol);
        }
    }
    return cnt_lines;
}





int AlgorithmFuncs::find_col_defect_bychannel  (Spec_Mode mode,
                                                uchar *pData, int w, int h,
                                                int r, int stripCnt,
                                                double spec_line,
                                                double spec_dol,
                                                int &cnt_dol,
                                                double &max_diff,
                                                int &max_pos,
                                                double &col_max_min_diff,
                                                double &dol_std_max,
                                                int &dol_std_max_pos,
                                                double break_line_spec,
                                                int &break_line_cnt,
                                                double &break_line_max_value,
                                                int &break_line_max_pos,
                                                std::string &msg,
                                                QVector<double> &mean_each_col,
                                                QVector<double> &delta_each_col,
                                                QVector<double> &diff_each_col)
{
    int cnt_lines = 0;
    std::vector<double> each_col_;
    int N = h/2;

    std::vector<double> each_col_half1_;
    std::vector<double> each_col_half2_;


    for (int j=0; j<w; ++j)
    {
        double mean_t = 0.0;
        double mean_t_half1 =0.0;
        double mean_t_half2 =0.0;

        for (int i=0; i<N; ++i)
        {
            mean_t+=(double)(pData[w*2*i+j]);
            mean_t+=(double)(pData[w*(2*i+1)+j]);
            if(i<(N>>1))
                mean_t_half1 = mean_t_half1 + (double)(pData[w*2*i+j]) + (double)(pData[w*(2*i+1)+j]);
            else
                mean_t_half2 = mean_t_half2 + (double)(pData[w*2*i+j]) + (double)(pData[w*(2*i+1)+j]);
        }
        double tmp=mean_t/(2*N);
        mean_each_col.push_back(tmp);
        each_col_.push_back (tmp);
        each_col_half1_.push_back (mean_t_half1/N);
        each_col_half2_.push_back (mean_t_half2/N);
    }

    QVector<double> delta_each_col_half1;
    QVector<double> delta_each_col_half2;

    for(int i=0; i<each_col_half1_.size()-2; i++)
    {
        double delta1 =each_col_half1_.at(i+1)-each_col_half1_.at(i);
        double delta2 =each_col_half1_.at(i+2)-each_col_half1_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_col_half1.push_back(delta);
    }

    for(int i=0; i<delta_each_col_half1.size(); i++)
    {
        if(delta_each_col_half1.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half1 = *std::max_element(delta_each_col_half1.begin(),delta_each_col_half1.end());
    double break_line_max_pos_half1 = delta_each_col_half1.indexOf(break_line_max_value_half1);



    for(int i=0; i<each_col_half2_.size()-2; i++)
    {
        double delta1 =each_col_half2_.at(i+1)-each_col_half2_.at(i);
        double delta2 =each_col_half2_.at(i+2)-each_col_half2_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_col_half2.push_back(delta);
    }

    for(int i=0; i<delta_each_col_half2.size(); i++)
    {
        if(delta_each_col_half2.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half2 = *std::max_element(delta_each_col_half2.begin(),delta_each_col_half2.end());
    double break_line_max_pos_half2 = delta_each_col_half2.indexOf(break_line_max_value_half2);


    if (break_line_max_value_half1 >break_line_max_value_half2)
    {
        break_line_max_value = break_line_max_value_half1;
        break_line_max_pos = 2*break_line_max_pos_half1;
        delta_each_col=delta_each_col_half1;
    }
    else
    {
        break_line_max_value = break_line_max_value_half2;
        break_line_max_pos = 2*break_line_max_pos_half2;
        delta_each_col=delta_each_col_half2;
    }


    double max_col = *std::max_element(each_col_.begin(),each_col_.end());
    double min_col = *std::min_element(each_col_.begin(),each_col_.end());
    col_max_min_diff = max_col-min_col;
    msg+=boost::str(boost::format("Max col:%.4f Min col:%.4f Max-Min:%.4f\n")
                    % max_col % min_col % col_max_min_diff);


    for (int i=stripCnt/2; i<each_col_.size ()-stripCnt/2; ++i)
    {
        double mean_left = each_col_.at (i);
        double mean_right = each_col_.at (i);
        if (i-r>=stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_left = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_left+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_left/=cnt_t1;
        }
        if (i+r<each_col_.size ()-stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_right = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_right+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_right/=cnt_t1;
        }
        double dif_left = std::abs (each_col_.at (i)-mean_left);
        double dif_right= std::abs (each_col_.at (i)-mean_right);


        double dif_left_ratio=0;
        double dif_right_ratio=0;
        double diff=0;

        if(mean_left==0||mean_right==0)
        {
            dif_left_ratio =-1;
            dif_right_ratio =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_left_ratio = dif_left/mean_left;
                dif_right_ratio = dif_right/mean_right;
                diff = std::max(dif_left_ratio,dif_right_ratio);
            }
            else if (mode==DN)
            {
                dif_left_ratio = dif_left;
                dif_right_ratio = dif_right;
                diff = std::max(dif_left_ratio,dif_right_ratio);
            }
        }

        diff_each_col.append(diff);

        if( diff > max_diff)
        {
                max_diff = diff;
                max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;
            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d dif_left:%.2f dif_right:%.2f\n")
                                % (i*2) % dif_left_ratio % dif_right_ratio);
            }
        }
        std::vector<int> col_data;
        int index_col = i;
        for (int index_row=0; index_row<h; ++index_row)
            col_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(col_data);
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_col*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (index_col*2) % std_dol);
        }
    }
    return cnt_lines;
}




int AlgorithmFuncs::find_edge_row_defect_bychannel (Spec_Mode mode,
                                                    uchar *pData, int w, int h,
                                                    int r, int stripCnt,
                                                    double spec_line,
                                                    double spec_dol,
                                                    int &cnt_dol,
                                                    double &max_diff,
                                                    int &max_pos,
                                                    double &dol_std_max,
                                                    int &dol_std_max_pos,
                                                    std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_row_;
    int N = w/2;
    for (int i=0; i<h; ++i)
    {
        double mean_t = 0.0;
        for (int j=0; j<N; ++j)
        {
            mean_t+=(double)(pData[w*i+2*j]);
            mean_t+=(double)(pData[w*i+2*j+1]);
        }
        each_row_.push_back (mean_t/(2*N));
    }

    for (int i=0; i<stripCnt/2; ++i)
    {
        double mean_down = each_row_.at (i);

        if (i+r<each_row_.size())
        {
            int cnt_t1 = 0;
            mean_down = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_down+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_down/=cnt_t1;
        }
        double dif_down= std::abs (each_row_.at (i)-mean_down);

        double diff=0;
        if(mode==Ratio)
        {
            diff  = dif_down/mean_down;
        }
        else if(mode==DN)
        {
            diff = dif_down;

        }
        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d   dif_down:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        std::vector<int> row_data;
        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(row_data);
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_row*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }



    for (int i=each_row_.size()-stripCnt/2; i<each_row_.size(); ++i)
    {
        double mean_top = each_row_.at (i);

        if (i-r>0)
        {
            int cnt_t1 = 0;
            mean_top = 0.0;
            for (int index = i-r; index<i; ++index)
            {
                mean_top+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_top/=cnt_t1;
        }
        double dif_top= std::abs (each_row_.at (i)-mean_top);

        double diff=0;
        if(mode==Ratio)
        {
            diff  = dif_top/mean_top;
        }
        else if(mode==DN)
        {
            diff = dif_top;

        }
        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d  dif_top:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        std::vector<int> row_data;
        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(row_data);
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_row*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }

    return cnt_lines;
}






int AlgorithmFuncs::find_edge_col_defect_bychannel (Spec_Mode mode,
                                                    uchar *pData, int w, int h,
                                                    int r, int stripCnt,
                                                    double spec_line,
                                                    double spec_dol,
                                                    int &cnt_dol,
                                                    double &max_diff,
                                                    int &max_pos,
                                                    double &dol_std_max,
                                                    int &dol_std_max_pos,
                                                    std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_col_;
    int N = h/2;
    for (int j=0; j<w; ++j)
    {
        double mean_t = 0.0;
        for (int i=0; i<N; ++i)
        {
            mean_t+=(double)(pData[w*2*i+j]);
            mean_t+=(double)(pData[w*(2*i+1)+j]);
        }

        each_col_.push_back (mean_t/(2*N));
    }


    for (int i=0; i<stripCnt/2; ++i)
    {
        double mean_right = each_col_.at (i);

        if (i+r<each_col_.size())
        {
            int cnt_t1 = 0;
            mean_right = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_right+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_right/=cnt_t1;
        }
        double dif_right= std::abs (each_col_.at (i)-mean_right);

        double diff=0;
        if(mode==Ratio)
        {
            diff  = dif_right/mean_right;
        }
        else if(mode==DN)
        {
            diff = dif_right;

        }
        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d   dif_right:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        std::vector<int> col_data;
        int index_col = i;
        for (int index_row=0; index_row<h; ++index_row)
            col_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(col_data);
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_col*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }



    for (int i=each_col_.size()-stripCnt/2; i<each_col_.size(); ++i)
    {
        double mean_left = each_col_.at (i);

        if (i-r>0)
        {
            int cnt_t1 = 0;
            mean_left = 0.0;
            for (int index = i-r; index<i; ++index)
            {
                mean_left+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_left/=cnt_t1;
        }
        double dif_left= std::abs (each_col_.at (i)-mean_left);

        double diff=0;
        if(mode==Ratio)
        {
            diff  = dif_left/mean_left;
        }
        else if(mode==DN)
        {
            diff = dif_left;

        }
        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d  dif_left:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        std::vector<int> col_data;
        int index_col = i;
        for (int index_row=0; index_row<h; ++index_row)
            col_data.push_back (pData[w*index_row+index_col]);
        double std_dol = GetDatasStd<int>(col_data);
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_col*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }

    return cnt_lines;
}







int AlgorithmFuncs::find_row_defect_bychannel_new (Spec_Mode mode,
                                               uchar *pData, int w, int h,
                                               int r, int stripCnt,
                                               double spec_line,
                                               double spec_dol,
                                               int &cnt_dol,
                                               double &max_diff,
                                               int &max_pos,
                                               double &row_max_min_diff,
                                               double &dol_std_max,
                                               int &dol_std_max_pos,
                                               double break_line_spec,
                                               int &break_line_cnt,
                                               double &break_line_max_value,
                                               int &break_line_max_pos,
                                               std::string &msg,
                                               QVector<double> &mean_each_row,
                                               QVector<double> &delta_each_row,
                                               QVector<double> &diff_each_row)
{
    int cnt_lines = 0;
    std::vector<double> each_row_;
    int N = w/2;

    std::vector<double> each_row_half1_(h);
    std::vector<double> each_row_half2_(h);

    for (size_t i=0; i<h; ++i)
    {
        double mean_t_half1 =0.0;
        double mean_t_half2 =0.0;

        for (int j=0; j<N; ++j)
        {
            if(j<(N>>1))
                mean_t_half1 = mean_t_half1 + (double)(pData[w*i+2*j]) + (double)(pData[w*i+2*j+1]);
            else
                mean_t_half2 = mean_t_half2 + (double)(pData[w*i+2*j]) + (double)(pData[w*i+2*j+1]);
        }
        each_row_half1_[i]=mean_t_half1/N;
        each_row_half2_[i]=mean_t_half2/N;
        double mean_t = (each_row_half1_[i]+each_row_half2_[i])/2;
        each_row_.push_back (mean_t);
        mean_each_row.push_back(mean_t);
    }

    QVector<double> delta_each_row_half1;
    QVector<double> delta_each_row_half2;

    double delta_half1_first =each_row_half1_.at(1)-each_row_half1_.at(0);
    double break_line_max_value_half1=0;
    double break_line_max_pos_half1=0;
    for(int i=0; i<each_row_half1_.size()-2; i++)
    {
        double delta2 =each_row_half1_.at(i+2)-each_row_half1_.at(i+1);
        double delta_diff=std::abs(delta2-delta_half1_first);
        delta_each_row_half1.push_back(std::abs(delta_diff));

        if(delta_diff>break_line_spec)
            break_line_cnt++;

        if(delta_diff>break_line_max_value_half1)
        {
            break_line_max_value_half1 = delta_diff;
            break_line_max_pos_half1 = i;
        }
        delta_half1_first = delta2;
    }

    double delta_half2_2 =each_row_half2_.at(1)-each_row_half2_.at(0);
    double break_line_max_value_half2=0;
    double break_line_max_pos_half2=0;
    for(int i=0; i<each_row_half2_.size()-2; i++)
    {
        double delta2 =each_row_half2_.at(i+2)-each_row_half2_.at(i+1);
        double delta_diff=std::abs(delta2-delta_half2_2);
        delta_each_row_half2.push_back(std::abs(delta_diff));

        if(delta_diff>break_line_spec)
            break_line_cnt++;

        if(delta_diff>break_line_max_value_half2)
        {
            break_line_max_value_half2 = delta_diff;
            break_line_max_pos_half2 = i;
        }
        delta_half2_2 = delta2;
    }

    if (break_line_max_value_half1 >break_line_max_value_half2)
    {
        break_line_max_value = break_line_max_value_half1;
        break_line_max_pos = 2*break_line_max_pos_half1;
        delta_each_row=delta_each_row_half1;
    }
    else
    {
        break_line_max_value = break_line_max_value_half2;
        break_line_max_pos = 2*break_line_max_pos_half2;
        delta_each_row=delta_each_row_half2;
    }

    double max_row = *std::max_element(each_row_.begin(),each_row_.end());
    double min_row = *std::min_element(each_row_.begin(),each_row_.end());
    row_max_min_diff = max_row-min_row;
    msg+=boost::str(boost::format("Max row:%.4f Min row:%.4f Max-Min:%.4f\n")
                    % max_row % min_row % row_max_min_diff);

    std::vector<int> row_data(w,0);
    for (int i=stripCnt/2; i<each_row_.size ()-stripCnt/2; ++i)
    {
        double mean_top = each_row_.at (i);
        double mean_down = each_row_.at (i);
        if (i-r>=stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_top = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_top+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_top/=cnt_t1;
        }
        if (i+r<each_row_.size ()-stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_down = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_down+=each_row_.at(index);
                cnt_t1+=1;
            }
            mean_down/=cnt_t1;
        }
        double dif_top = std::abs (each_row_.at (i)-mean_top);
        double dif_down= std::abs (each_row_.at (i)-mean_down);

        double dif_top_ratio=0;
        double dif_down_ratio=0;
        double diff=0;

        if(mean_top==0||mean_down==0)
        {
            dif_top_ratio =-1;
            dif_down_ratio =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_top_ratio = dif_top/mean_top;
                dif_down_ratio = dif_down/mean_down;
                diff = std::max(dif_top_ratio,dif_down_ratio);
            }
            else if(mode==DN)
            {
                dif_top_ratio = dif_top;
                dif_down_ratio = dif_down;
                diff = std::max(dif_top_ratio,dif_down_ratio);
            }
        }

        diff_each_row.append(diff);

        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d dif_top:%.2f dif_down:%.2f\n")
                                % (i*2) % dif_top_ratio % dif_down_ratio);
            }
        }

        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data[index_col]=pData[w*index_row+index_col];
        double std_dol = GetDatasStd<int>(row_data,each_row_.at (i));
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_row*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (index_row*2) % std_dol);
        }
    }

    return cnt_lines;
}





int AlgorithmFuncs::find_col_defect_bychannel_new  (Spec_Mode mode,
                                                uchar *pData, int w, int h,
                                                int r, int stripCnt,
                                                double spec_line,
                                                double spec_dol,
                                                int &cnt_dol,
                                                double &max_diff,
                                                int &max_pos,
                                                double &col_max_min_diff,
                                                double &dol_std_max,
                                                int &dol_std_max_pos,
                                                double break_line_spec,
                                                int &break_line_cnt,
                                                double &break_line_max_value,
                                                int &break_line_max_pos,
                                                std::string &msg,
                                                QVector<double> &mean_each_col,
                                                QVector<double> &delta_each_col,
                                                QVector<double> &diff_each_col)
{
    int cnt_lines = 0;
    std::vector<double> each_col_;
    int N = h/2;

    std::vector<double> each_col_half1_(w);
    std::vector<double> each_col_half2_(w);

    for (int j=0; j<w; ++j)
    {
        double mean_t_half1 =0.0;
        double mean_t_half2 =0.0;
        for (int i=0; i<N; ++i)
        {
            if(i<(N>>1))
                mean_t_half1 = mean_t_half1 + (double)(pData[w*2*i+j]) + (double)(pData[w*(2*i+1)+j]);
            else
                mean_t_half2 = mean_t_half2 + (double)(pData[w*2*i+j]) + (double)(pData[w*(2*i+1)+j]);
        }

        each_col_half1_[j]=mean_t_half1/N;
        each_col_half2_[j]=mean_t_half2/N;
        double tmp=(each_col_half1_[j]+each_col_half2_[j])/2;
        mean_each_col.push_back(tmp);
        each_col_.push_back (tmp);
    }

    QVector<double> delta_each_col_half1;
    QVector<double> delta_each_col_half2;

    for(int i=0; i<each_col_half1_.size()-2; i++)
    {
        double delta1 =each_col_half1_.at(i+1)-each_col_half1_.at(i);
        double delta2 =each_col_half1_.at(i+2)-each_col_half1_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_col_half1.push_back(delta);
    }

    for(int i=0; i<delta_each_col_half1.size(); i++)
    {
        if(delta_each_col_half1.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half1 = *std::max_element(delta_each_col_half1.begin(),delta_each_col_half1.end());
    double break_line_max_pos_half1 = delta_each_col_half1.indexOf(break_line_max_value_half1);



    for(int i=0; i<each_col_half2_.size()-2; i++)
    {
        double delta1 =each_col_half2_.at(i+1)-each_col_half2_.at(i);
        double delta2 =each_col_half2_.at(i+2)-each_col_half2_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_col_half2.push_back(delta);
    }

    for(int i=0; i<delta_each_col_half2.size(); i++)
    {
        if(delta_each_col_half2.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half2 = *std::max_element(delta_each_col_half2.begin(),delta_each_col_half2.end());
    double break_line_max_pos_half2 = delta_each_col_half2.indexOf(break_line_max_value_half2);


    if (break_line_max_value_half1 >break_line_max_value_half2)
    {
        break_line_max_value = break_line_max_value_half1;
        break_line_max_pos = 2*break_line_max_pos_half1;
        delta_each_col=delta_each_col_half1;
    }
    else
    {
        break_line_max_value = break_line_max_value_half2;
        break_line_max_pos = 2*break_line_max_pos_half2;
        delta_each_col=delta_each_col_half2;
    }


    double max_col = *std::max_element(each_col_.begin(),each_col_.end());
    double min_col = *std::min_element(each_col_.begin(),each_col_.end());
    col_max_min_diff = max_col-min_col;
    msg+=boost::str(boost::format("Max col:%.4f Min col:%.4f Max-Min:%.4f\n")
                    % max_col % min_col % col_max_min_diff);

    std::vector<int> col_data(h,0);
    for (int i=stripCnt/2; i<each_col_.size ()-stripCnt/2; ++i)
    {
        double mean_left = each_col_.at (i);
        double mean_right = each_col_.at (i);
        if (i-r>=stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_left = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {
                mean_left+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_left/=cnt_t1;
        }
        if (i+r<each_col_.size ()-stripCnt/2)
        {
            int cnt_t1 = 0;
            mean_right = 0.0;
            for (int index = i+1; index<=i+r; ++index)
            {
                mean_right+=each_col_.at(index);
                cnt_t1+=1;
            }
            mean_right/=cnt_t1;
        }
        double dif_left = std::abs (each_col_.at (i)-mean_left);
        double dif_right= std::abs (each_col_.at (i)-mean_right);


        double dif_left_ratio=0;
        double dif_right_ratio=0;
        double diff=0;

        if(mean_left==0||mean_right==0)
        {
            dif_left_ratio =-1;
            dif_right_ratio =-1;
            diff =-1;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_left_ratio = dif_left/mean_left;
                dif_right_ratio = dif_right/mean_right;
                diff = std::max(dif_left_ratio,dif_right_ratio);
            }
            else if (mode==DN)
            {
                dif_left_ratio = dif_left;
                dif_right_ratio = dif_right;
                diff = std::max(dif_left_ratio,dif_right_ratio);
            }
        }

        diff_each_col.append(diff);

        if( diff > max_diff)
        {
                max_diff = diff;
                max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;
            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d dif_left:%.2f dif_right:%.2f\n")
                                % (i*2) % dif_left_ratio % dif_right_ratio);
            }
        }

        int index_col = i;

        for (int index_row=0; index_row<h; ++index_row)
            col_data[index_row]= pData[w*index_row+index_col];
        double std_dol = GetDatasStd<int>(col_data,each_col_.at (i));
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_col*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (index_col*2) % std_dol);
        }
    }
    return cnt_lines;
}


int AlgorithmFuncs::find_txoff_col_defect_bychannel_new  (Spec_Mode mode,
                                                          uchar *pData, int w, int h,
                                                          int r, int stripCnt,
                                                          double spec_line,
                                                          double spec_dol,
                                                          double spec_bright_line,
                                                          double spec_dark_line,
                                                          int &cnt_dol,
                                                          double &max_diff,
                                                          int &max_pos,
                                                          double &col_max_min_diff,
                                                          double &dol_std_max,
                                                          int &dol_std_max_pos,
                                                          double break_line_spec,
                                                          int &break_line_cnt,
                                                          double &break_line_max_value,
                                                          int &break_line_max_pos,
                                                          std::string &msg,
                                                          QVector<double> &mean_each_col,
                                                          QVector<double> &delta_each_col,
                                                          QVector<double> &diff_each_col,
                                                          // 新增参数
                                                          int &cnt_bright_col,
                                                          double &max_bright_col_diff,
                                                          int &max_bright_col_pos,
                                                          double &max_dark_col_diff,
                                                          int &max_dark_col_pos)
//该函数中判断亮暗线，不会统计到cnt_lines中，也未统计亮暗线数量，超过这个spec的值不参与均值计算
//该函数最终输出亮暗线的max值，可在外部添加判断fail出code，函数内没有亮暗线数量统计
//函数中return的cnt_lines是与spec：spec_line对比后的，返回的cnt_bright_col数量是在cnt_lines中的亮线数量
{
    int cnt_lines = 0;
    int cnt_edge = 0;
    double mean_edge = 0.0;
    std::vector<double> each_col_;
    int N = h/2;
    std::vector<int> each_col_index;

    std::vector<double> each_col_half1_(w);
    std::vector<double> each_col_half2_(w);

    for (int j=0; j<w; ++j)
    {
        double mean_t_half1 =0.0;
        double mean_t_half2 =0.0;
        for (int i=0; i<N; ++i)
        {
            if(i<(N>>1))
                mean_t_half1 = mean_t_half1 + (double)(pData[w*2*i+j]) + (double)(pData[w*(2*i+1)+j]);
            else
                mean_t_half2 = mean_t_half2 + (double)(pData[w*2*i+j]) + (double)(pData[w*(2*i+1)+j]);
        }

        each_col_half1_[j]=mean_t_half1/N;
        each_col_half2_[j]=mean_t_half2/N;
        double tmp=(each_col_half1_[j]+each_col_half2_[j])/2;
        mean_each_col.push_back(tmp);
        each_col_.push_back (tmp);
    }

    QVector<double> delta_each_col_half1;
    QVector<double> delta_each_col_half2;

    for(int i=0; i<each_col_half1_.size()-2; i++)
    {
        double delta1 =each_col_half1_.at(i+1)-each_col_half1_.at(i);
        double delta2 =each_col_half1_.at(i+2)-each_col_half1_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_col_half1.push_back(delta);
    }

    for(int i=0; i<delta_each_col_half1.size(); i++)
    {
        if(delta_each_col_half1.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half1 = *std::max_element(delta_each_col_half1.begin(),delta_each_col_half1.end());
    double break_line_max_pos_half1 = delta_each_col_half1.indexOf(break_line_max_value_half1);



    for(int i=0; i<each_col_half2_.size()-2; i++)
    {
        double delta1 =each_col_half2_.at(i+1)-each_col_half2_.at(i);
        double delta2 =each_col_half2_.at(i+2)-each_col_half2_.at(i+1);
        double delta = std::abs(delta2-delta1);
        delta_each_col_half2.push_back(delta);
    }

    for(int i=0; i<delta_each_col_half2.size(); i++)
    {
        if(delta_each_col_half2.at(i)>break_line_spec)
        {
            break_line_cnt++;
        }
    }

    double break_line_max_value_half2 = *std::max_element(delta_each_col_half2.begin(),delta_each_col_half2.end());
    double break_line_max_pos_half2 = delta_each_col_half2.indexOf(break_line_max_value_half2);


    if (break_line_max_value_half1 >break_line_max_value_half2)
    {
        break_line_max_value = break_line_max_value_half1;
        break_line_max_pos = 2*break_line_max_pos_half1;
        delta_each_col=delta_each_col_half1;
    }
    else
    {
        break_line_max_value = break_line_max_value_half2;
        break_line_max_pos = 2*break_line_max_pos_half2;
        delta_each_col=delta_each_col_half2;
    }

    double max_col = *std::max_element(each_col_.begin(),each_col_.end());
    double min_col = *std::min_element(each_col_.begin(),each_col_.end());
    col_max_min_diff = max_col-min_col;
    msg+=boost::str(boost::format("Max col:%.4f Min col:%.4f Max-Min:%.4f\n")
                    % max_col % min_col % col_max_min_diff);

    std::vector<double> each_col_edge_;
    for (int index = 0; index != 15; ++index) {
        each_col_edge_.push_back(each_col_.at(index));
    }
    std::sort(each_col_edge_.begin(), each_col_edge_.end());
    double mean_edge_median=each_col_edge_[7];

    std::vector<int> col_data(h,0);
    for (int i=stripCnt/2; i<each_col_.size ()-stripCnt/2; ++i)
    {
        double mean_left = each_col_.at (i);
        double dif_left = 0.0;
        double diff_left_signed = 0.0;
        double mean_edge_diff=0.00;
        double current_mean = each_col_.at(i);
        if (i<15)
        {
            mean_edge_diff=each_col_.at(i)- mean_edge_median;
            dif_left = std::abs(mean_edge_diff);
            // 计算带符号的差值（新增）
            diff_left_signed = mean_edge_diff;
        }

        else
        {
            int cnt_t1 = 0;
            mean_left = 0.0;
            for (int index = i-r; index<=i-1; ++index)
            {

                if (std::find(each_col_index.begin(), each_col_index.end(), index) == each_col_index.end())
                {
                    mean_left+=each_col_.at(index);
                    cnt_t1+=1;
                }
                else
                {
//                    if((each_col_index.size()) < 5)
//                    {
//                        msg+=boost::str(boost::format("left col position:%d \n") %index);//会输出多遍
//                    }
                }
            }
            if (cnt_t1!=0)
                mean_left/=cnt_t1;
            else
                mean_left=each_col_.at(i);

            dif_left = std::abs (current_mean-mean_left);
            // 计算带符号的差值（新增）
            diff_left_signed = current_mean - mean_left;
        }

        double dif_left_ratio=0;
        double diff=0;

        // 带符号的差值（仅用于DN模式）
        double signed_diff_dn = 0;

        if(mean_left==0){
            dif_left_ratio =-1;
            diff =-1;
            signed_diff_dn = 0;
        }
        else
        {
            if(mode==Ratio)
            {
                dif_left_ratio = dif_left/mean_left;
                diff = dif_left_ratio;
                // 比值模式不计算带符号差值
                signed_diff_dn = 0;
            }
            else if (mode==DN)
            {
                dif_left_ratio = dif_left;
                diff = dif_left_ratio;
                // 仅在DN模式下计算带符号的DN差值
                signed_diff_dn =diff_left_signed;
            }
        }

        diff_each_col.append(diff);

        if( diff > max_diff)
        {
                max_diff = diff;
                max_pos = i*2;
        }

        // 新增：仅在DN分别更新亮线和暗线的最大差值
        if(mode == DN && signed_diff_dn != 0) // 仅DN模式且有效差值
        {
            if(signed_diff_dn > 0) // 亮线（正差值）
            {
                if(signed_diff_dn > max_bright_col_diff)
                {
                    max_bright_col_diff = signed_diff_dn;
                    max_bright_col_pos = i*2;
                }
                if (signed_diff_dn > spec_bright_line)
                {
                    each_col_index.push_back (i);
                    if(each_col_index.size()<5)
                        msg+=boost::str(boost::format("bright col:each_col_index:%d \n")% i);
                }
            }
            else if(signed_diff_dn < 0) // 暗线（负差值）
            {
                if(signed_diff_dn < max_dark_col_diff) //
                {
                    max_dark_col_diff = signed_diff_dn;
                    max_dark_col_pos = i*2;
                }
                if (signed_diff_dn < spec_dark_line)
                {
                    each_col_index.push_back (i);
                    if(each_col_index.size()<5)
                        msg+=boost::str(boost::format("dark col:each_col_index:%d \n")% i);
                }
            }
        }

        if (diff>spec_line)//diff > FPN_LEVEL1的线判断亮暗
        {
            cnt_lines+=1; // 总缺陷计数（亮线+暗线）

            // 判断是亮线还是暗线（仅在DN模式下使用新逻辑）
            if(mode == DN && signed_diff_dn != 0)
            {
                bool is_bright_line = (signed_diff_dn > 0);

                if (is_bright_line)
                {
                    cnt_bright_col++; // 亮线计数
                    if (cnt_bright_col < 5) {
                        msg+=boost::str(boost::format("bright col,col:%d current:%.2f left:%.2f signed_diff:%.2f\n")
                                        % (i*2) % current_mean % mean_left % signed_diff_dn);
                    }
                }
                else
                {
                    // 暗线处理
                    if ((cnt_lines - cnt_bright_col) < 5) {
                        msg+=boost::str(boost::format("dead col,col:%d current:%.2f left:%.2f signed_diff:%.2f\n")
                                        % (i*2) % current_mean % mean_left % signed_diff_dn);
                    }
                }
            }
            else
            {
                // 保持原有的判断逻辑（比值模式或无效差值）
                double diff_left_signed = current_mean - mean_left;
                bool is_bright_line = (diff_left_signed > 0);

                if (is_bright_line) {
                    cnt_bright_col++; // 亮线计数
                    if (cnt_bright_col < 5) {
                        msg+=boost::str(boost::format("bright col,col:%d current:%.2f left:%.2f diff_l:%.2f\n")
                                        % (i*2) % current_mean % mean_left % diff_left_signed );
                    }
                }
                else {
                    // 暗线处理
                    if ((cnt_lines - cnt_bright_col) < 5) {
                        msg+=boost::str(boost::format("dead col,col:%d dif_left:%.2f\n")
                                        % (i*2) % dif_left_ratio);
                    }
                }
            }
        }
        int index_col = i;

        for (int index_row=0; index_row<h; ++index_row)
            col_data[index_row]= pData[w*index_row+index_col];
        double std_dol = GetDatasStd<int>(col_data,each_col_.at (i));
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_col*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (index_col*2) % std_dol);
        }
    }
    return cnt_lines;
}




int AlgorithmFuncs::find_edge_row_defect_bychannel_new (Spec_Mode mode,
                                                    uchar *pData, int w, int h,
                                                    int r, int stripCnt,
                                                    double spec_line,
                                                    double spec_dol,
                                                    int &cnt_dol,
                                                    double &max_diff,
                                                    int &max_pos,
                                                    double &dol_std_max,
                                                    int &dol_std_max_pos,
                                                    std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_row_(h,0);
    int N = w/2;
    for (int i=0; i<h; ++i)
    {
        if(i>=stripCnt/2+r && i<h-stripCnt/2-r)
            continue;
        double mean_t = 0.0;
        for (int j=0; j<N; ++j)
        {
            mean_t+=(double)(pData[w*i+2*j]);
            mean_t+=(double)(pData[w*i+2*j+1]);
        }
        each_row_[i]=mean_t/(2*N);
    }

    std::vector<int> row_data(w,0);
    for (int i=0; i<each_row_.size(); ++i)
    {
        double diff=0;
        if(i<stripCnt/2)
        {
            double mean_down = each_row_.at (i);
            if (i+r<each_row_.size())
            {
                int cnt_t1 = 0;
                mean_down = 0.0;
                for (int index = i+1; index<=i+r; ++index)
                {
                    mean_down+=each_row_.at(index);
                    cnt_t1+=1;
                }
                mean_down/=cnt_t1;
            }
            double dif_down= std::abs (each_row_.at (i)-mean_down);
            if(mode==Ratio)
                diff  = dif_down/mean_down;
            else if(mode==DN)
                diff = dif_down;
        }
        else if(i>=each_row_.size()-stripCnt/2)
        {
            double mean_top = each_row_.at (i);
            if (i-r>0)
            {
                int cnt_t1 = 0;
                mean_top = 0.0;
                for (int index = i-r; index<i; ++index)
                {
                    mean_top+=each_row_.at(index);
                    cnt_t1+=1;
                }
                mean_top/=cnt_t1;
            }
            double dif_top= std::abs (each_row_.at (i)-mean_top);

            if(mode==Ratio)
                diff  = dif_top/mean_top;
            else if(mode==DN)
                diff = dif_top;
        }
        else
            continue;

        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead row,row:%d  dif_top:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        int index_row = i;
        for (int index_col=0; index_col<w; ++index_col)
            row_data[index_col]=pData[w*index_row+index_col];
        double std_dol = GetDatasStd<int>(row_data,each_row_.at (i));
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_row*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,row:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }

    return cnt_lines;
}






int AlgorithmFuncs::find_edge_col_defect_bychannel_new (Spec_Mode mode,
                                                    uchar *pData, int w, int h,
                                                    int r, int stripCnt,
                                                    double spec_line,
                                                    double spec_dol,
                                                    int &cnt_dol,
                                                    double &max_diff,
                                                    int &max_pos,
                                                    double &dol_std_max,
                                                    int &dol_std_max_pos,
                                                    std::string &msg)
{
    int cnt_lines = 0;
    std::vector<double> each_col_(w,0);
    int N = h/2;
    for (int j=0; j<w; ++j)
    {
        if(j>stripCnt/2+r && j<w-stripCnt/2-r)
            continue;
        double mean_t = 0.0;
        for (int i=0; i<N; ++i)
        {
            mean_t+=(double)(pData[w*2*i+j]);
            mean_t+=(double)(pData[w*(2*i+1)+j]);
        }

        each_col_[j]=mean_t/(2*N);
    }

    std::vector<int> col_data(h);
    for (int i=0; i<each_col_.size(); ++i)
    {
        double diff=0;
        if(i<stripCnt/2)
        {
            double mean_right = each_col_.at (i);

            if (i+r<each_col_.size())
            {
                int cnt_t1 = 0;
                mean_right = 0.0;
                for (int index = i+1; index<=i+r; ++index)
                {
                    mean_right+=each_col_.at(index);
                    cnt_t1+=1;
                }
                mean_right/=cnt_t1;
            }
            double dif_right= std::abs (each_col_.at (i)-mean_right);
            if(mode==Ratio)
                diff  = dif_right/mean_right;
            else if(mode==DN)
                diff = dif_right;
        }
        else if(i>=each_col_.size()-stripCnt/2)
        {
            double mean_left = each_col_.at (i);

            if (i-r>0)
            {
                int cnt_t1 = 0;
                mean_left = 0.0;
                for (int index = i-r; index<i; ++index)
                {
                    mean_left+=each_col_.at(index);
                    cnt_t1+=1;
                }
                mean_left/=cnt_t1;
            }
            double dif_left= std::abs (each_col_.at (i)-mean_left);


            if(mode==Ratio)
                diff  = dif_left/mean_left;
            else if(mode==DN)
                diff = dif_left;
        }
        else
            continue;

        if( diff > max_diff)
        {
            max_diff = diff;
            max_pos = i*2;
        }

        if (diff>spec_line)
        {
            cnt_lines+=1;

            if (cnt_lines<5)
            {
                msg+=boost::str(boost::format("dead col,col:%d  dif_left:%.2f\n")
                                % (i*2)  % diff);
            }
        }

        int index_col = i;
        for (int index_row=0; index_row<h; ++index_row)
            col_data[index_row]=pData[w*index_row+index_col];
        double std_dol = GetDatasStd<int>(col_data,each_col_.at (i));
        if(std_dol>dol_std_max)
        {
            dol_std_max = std_dol;
            dol_std_max_pos = index_col*2;
        }
        if (std_dol>spec_dol)
        {//dol
            cnt_dol+=1;
            if (cnt_dol<3)
                msg+=boost::str(boost::format("DOL,col:%d std:%.2f\n")
                                % (i*2) % std_dol);
        }
    }

    return cnt_lines;
}




double AlgorithmFuncs::UniformityTest (uchar *pData, int w, int h,
                                     int w_cell, int h_cell)
{
    std::vector<double> mean_each_cell;
    std::vector<double> stdv_each_cell;
    int cnt_vertical = h/h_cell;
    int cnt_horizontal = w/w_cell;
    for (int i=0; i<cnt_vertical; ++i)
    {
        for (int j=0; j<cnt_horizontal; ++j)
        {
            int x_start = w_cell*j;
            int y_start = h_cell*i;
            double mean_tmp = 0.0;
            int cnt_tmp = 0;
            for (int index_row=y_start; index_row<y_start+h_cell; index_row++)
            {
                for (int index_col=x_start; index_col<x_start+w_cell; index_col+=2)
                {
                    cnt_tmp+=2;
                    mean_tmp+=(double)(pData[w*index_row+index_col]);
                    mean_tmp+=(double)(pData[w*index_row+index_col+1]);
                }
            }
            mean_tmp/=cnt_tmp;
            mean_each_cell.push_back (mean_tmp);
            double stdv_tmp = 0.0;
            for (int index_row=y_start; index_row<y_start+h_cell; ++index_row)
            {
                for (int index_col=x_start; index_col<x_start+w_cell; ++index_col)
                {
                    stdv_tmp+=(mean_tmp-(double)(pData[w*index_row+index_col]))*
                            (mean_tmp-(double)(pData[w*index_row+index_col]));
                }
            }
            stdv_tmp/=cnt_tmp;
            stdv_tmp = sqrt (stdv_tmp);
            stdv_each_cell.push_back (stdv_tmp);
        }
    }
    double prnu_tmp = 0.0;
    for (std::vector<double>::size_type i=0;
         i<mean_each_cell.size ();
         ++i)
    {
        double mean_tmp = mean_each_cell.at (i);
        if (mean_tmp <= 0.001)
                mean_tmp = 0.001;
        prnu_tmp+=(stdv_each_cell.at (i)/mean_tmp*100);
    }
    prnu_tmp/=mean_each_cell.size ();
    return prnu_tmp;
}




double AlgorithmFuncs::UniformityTest_Max (uchar *pData, int w, int h,
                                           int w_cell, int h_cell,
                                           double &prnu_eachcell_max, int &max_pos,
                                           QVector<double> &prnu_eachcell)
{
    QVector<double> mean_each_cell;
    QVector<double> stdv_each_cell;
    int cnt_vertical = h/h_cell;
    int cnt_horizontal = w/w_cell;
    for (int i=0; i<cnt_vertical; ++i)
    {
        for (int j=0; j<cnt_horizontal; ++j)
        {
            int x_start = w_cell*j;
            int y_start = h_cell*i;
            double mean_tmp = 0.0;
            int cnt_tmp = 0;
            for (int index_row=y_start; index_row<y_start+h_cell; index_row++)
            {
                for (int index_col=x_start; index_col<x_start+w_cell; index_col+=2)
                {
                    cnt_tmp+=2;
                    mean_tmp+=(double)(pData[w*index_row+index_col]);
                    mean_tmp+=(double)(pData[w*index_row+index_col+1]);
                }
            }
            mean_tmp/=cnt_tmp;
            mean_each_cell.push_back (mean_tmp);
            double stdv_tmp = 0.0;
            for (int index_row=y_start; index_row<y_start+h_cell; ++index_row)
            {
                for (int index_col=x_start; index_col<x_start+w_cell; ++index_col)
                {
                    stdv_tmp+=(mean_tmp-(double)(pData[w*index_row+index_col]))*
                            (mean_tmp-(double)(pData[w*index_row+index_col]));
                }
            }
            stdv_tmp/=cnt_tmp;
            stdv_tmp = sqrt (stdv_tmp);
            stdv_each_cell.push_back (stdv_tmp);
        }
    }
    double prnu_tmp = 0.0;
    for (QVector<double>::size_type i=0;
         i<mean_each_cell.size ();
         ++i)
    {
        double mean_tmp = mean_each_cell.at (i);
        if (mean_tmp <= 0.001)
                mean_tmp = 0.001;
        double tmp = stdv_each_cell.at (i)/mean_tmp*100;
        prnu_tmp+= tmp;
        prnu_eachcell.push_back(tmp);
    }
    prnu_tmp/=mean_each_cell.size ();

    prnu_eachcell_max = * max_element(prnu_eachcell.begin(),prnu_eachcell.end());
    max_pos = prnu_eachcell.indexOf(prnu_eachcell_max);

    return prnu_tmp;
}





double AlgorithmFuncs::Non_UniformityTest(uchar *pData, int w, int h,
                                          int w_cell, int h_cell,
                                          double &n_uniform_eachcell_max, int &max_pos,
                                          double &n_uniform_mirror_diff_eachcell_max, int &max_pos_mirror_diff,
                                          double &n_uniform_updown_diff_eachcell_max, int &max_pos_updown_diff,
                                          QVector<double> &n_uniform_eachcell,
                                          QVector<double> &n_uniform_mirror_diff_eachcell,
                                          QVector<double> &n_uniform_updown_diff_eachcell)
{
    QVector<double> mean_each_cell;
    QVector<double> stdv_each_cell;
    int cnt_vertical = h/h_cell;
    int cnt_horizontal = w/w_cell;
    for (int i=0; i<cnt_vertical; ++i)
    {
        for (int j=0; j<cnt_horizontal; ++j)
        {
            int x_start = w_cell*j;
            int y_start = h_cell*i;
            double mean_tmp = 0.0;
            int cnt_tmp = 0;
            for (int index_row=y_start; index_row<y_start+h_cell; index_row++)
            {
                for (int index_col=x_start; index_col<x_start+w_cell; index_col+=2)
                {
                    cnt_tmp+=2;
                    mean_tmp+=(double)(pData[w*index_row+index_col]);
                    mean_tmp+=(double)(pData[w*index_row+index_col+1]);
                }
            }
            mean_tmp/=cnt_tmp;
            mean_each_cell.push_back (mean_tmp);
            double stdv_tmp = 0.0;
            for (int index_row=y_start; index_row<y_start+h_cell; ++index_row)
            {
                for (int index_col=x_start; index_col<x_start+w_cell; ++index_col)
                {
                    stdv_tmp+=(mean_tmp-(double)(pData[w*index_row+index_col]))*
                            (mean_tmp-(double)(pData[w*index_row+index_col]));
                }
            }
            stdv_tmp/=cnt_tmp;
            stdv_tmp = sqrt (stdv_tmp);
            stdv_each_cell.push_back (stdv_tmp);
        }
    }
    double n_uniform_tmp = 0.0;

    for (QVector<double>::size_type i=0;
         i<mean_each_cell.size ();
         ++i)
    {
        double mean_tmp = mean_each_cell.at (i);
        if (mean_tmp <= 0.001)
                mean_tmp = 0.001;
        double tmp = stdv_each_cell.at (i)/mean_tmp*100;
        n_uniform_tmp+= tmp;
        n_uniform_eachcell.push_back(tmp);
    }
    n_uniform_tmp/=mean_each_cell.size ();

    n_uniform_eachcell_max = * max_element(n_uniform_eachcell.begin(),n_uniform_eachcell.end());
    max_pos = n_uniform_eachcell.indexOf(n_uniform_eachcell_max);

    QVector<double> n_uniform_mirror_eachcell = n_uniform_eachcell;
    QVector<double> n_uniform_updown_eachcell = n_uniform_eachcell;
    for (int i=0; i<cnt_vertical; ++i)
    {
        for (int j=0; j<cnt_horizontal; ++j)
        {
            double tmp = n_uniform_eachcell.at(i*cnt_horizontal+j);
            n_uniform_mirror_eachcell.replace(i*cnt_horizontal+cnt_horizontal-j-1,tmp);
            n_uniform_updown_eachcell.replace((cnt_vertical-i-1)*cnt_horizontal+j,tmp);
        }
    }

    for (int i=0; i<cnt_vertical; ++i)
    {
        for (int j=0; j<cnt_horizontal; ++j)
        {
            double mirror = std::abs(n_uniform_eachcell.at(i*cnt_horizontal+j) - n_uniform_mirror_eachcell.at(i*cnt_horizontal+j));
            double updown = std::abs(n_uniform_eachcell.at(i*cnt_horizontal+j) - n_uniform_updown_eachcell.at(i*cnt_horizontal+j));
            n_uniform_mirror_diff_eachcell.append(mirror);
            n_uniform_updown_diff_eachcell.append(updown);
        }
    }
    n_uniform_mirror_diff_eachcell_max = * max_element(n_uniform_mirror_diff_eachcell.begin(),n_uniform_mirror_diff_eachcell.end());
    max_pos_mirror_diff = n_uniform_mirror_diff_eachcell.indexOf(n_uniform_mirror_diff_eachcell_max);

    n_uniform_updown_diff_eachcell_max = * max_element(n_uniform_updown_diff_eachcell.begin(),n_uniform_updown_diff_eachcell.end());
    max_pos_updown_diff = n_uniform_updown_diff_eachcell.indexOf(n_uniform_updown_diff_eachcell_max);

    return n_uniform_tmp;
}



void AlgorithmFuncs::Uniformity_Detail_Test (int *pData, int w, int h,
                                int w_cell, int h_cell,
                                QVector<double> &stdv_each_cell)
{
    QVector<double> mean_each_cell;
    int cnt_vertical = h/h_cell;
    int cnt_horizontal = w/w_cell;
    for (int i=0; i<cnt_vertical; ++i)
    {
        for (int j=0; j<cnt_horizontal; ++j)
        {
            int x_start = w_cell*j;
            int y_start = h_cell*i;
            double mean_tmp = 0.0;
            int cnt_tmp = 0;
            for (int index_row=y_start; index_row<y_start+h_cell; index_row++)
            {
                for (int index_col=x_start; index_col<x_start+w_cell; index_col+=2)
                {
                    cnt_tmp+=2;
                    mean_tmp+=(double)(pData[w*index_row+index_col]);
                    mean_tmp+=(double)(pData[w*index_row+index_col+1]);
                }
            }
            mean_tmp/=cnt_tmp;
            mean_each_cell.push_back (mean_tmp);
            double stdv_tmp = 0.0;
            for (int index_row=y_start; index_row<y_start+h_cell; ++index_row)
            {
                for (int index_col=x_start; index_col<x_start+w_cell; ++index_col)
                {
                    stdv_tmp+=(mean_tmp-(double)(pData[w*index_row+index_col]))*
                            (mean_tmp-(double)(pData[w*index_row+index_col]));
                }
            }
            stdv_tmp/=cnt_tmp;
            stdv_tmp = sqrt (stdv_tmp);
            if(mean_tmp<=0)
                stdv_each_cell.push_back(0);
            else
                stdv_each_cell.push_back (stdv_tmp/mean_tmp);
        }
    }
}


void AlgorithmFuncs::DarkShadingTest(uchar *pData, int w, int h,
                                     int x_cnt, int y_cnt,
                                     double &ob_eachcell_max, double &ob_eachcell_min,
                                     int &max_pos, int &min_pos, QVector<double> &mean_ob_eachcell,
                                     std::string &msg)
{

    int cnt_vertical=y_cnt;
    int cnt_horizontal=x_cnt;

    int h_cell = h/cnt_vertical;
    int w_cell = w/cnt_horizontal;

    msg+=boost::str(boost::format("w:%d,h:%d,,w_cell:%d,h_cell:%d,cnt_vertical:%d,cnt_horizontal:%d\n")
                        %w %h %w_cell %h_cell %cnt_vertical % cnt_horizontal);

    for(int i=0; i<cnt_horizontal; ++i)
    {
        for(int j=0; j<cnt_vertical;++j)
        {
            int x_start=w_cell*i;
            int y_start=h_cell*j;
            double mean_tmp=0.0;
            int cnt_tmp=0;
            for(int index_row=y_start; index_row<y_start+h_cell; index_row++)
            {
                for(int index_col=x_start;index_col<x_start+w_cell;index_col+=2)
                {
                    mean_tmp+=pData[w*index_row+index_col];
                    mean_tmp+=pData[w*index_row+index_col+1];
                    cnt_tmp+=2;

                }

            }

            mean_tmp/= cnt_tmp;
            mean_ob_eachcell.push_back(mean_tmp);
        }
    }

    ob_eachcell_max =* max_element(mean_ob_eachcell.begin(),mean_ob_eachcell.end());
    ob_eachcell_min =* min_element(mean_ob_eachcell.begin(),mean_ob_eachcell.end());
    max_pos = mean_ob_eachcell.indexOf(ob_eachcell_max);
    min_pos = mean_ob_eachcell.indexOf(ob_eachcell_min);

    msg+=boost::str(boost::format("ob_eachcell_max:%.2f,ob_eachcell_min%.2f"
                                  "max_pos:%d,min_pos:%d\n")
                        % ob_eachcell_max % ob_eachcell_min %max_pos %min_pos);

//    double mean_ob_sum= std::accumulate(mean_ob_eachcell.begin(),mean_ob_eachcell.end(),0.0);
//    double mean_ob_average=mean_ob_sum/mean_ob_eachcell.size();
//    return mean_ob_average;

}

void AlgorithmFuncs::getColorRatioByRect (uchar *pR, uchar *pG1,
                                          uchar *pG2,
                                          uchar *pB, int w, int h,
                                          int x, int y,
                                          int w_rect, int h_rect,
                                          double &g_g,
                                          double &r_g, double &b_g,
                                          double &gr_gb_diff)
{
    double sum_r = 0.1;
    double sum_g1 = 0.1;
    double sum_g2 = 0.1;
    double sum_b = 0.1;
    int cnt_t = 0;
    for (int i=y; i<y+h_rect; ++i)
    {
        for (int j=x; j<x+w_rect; ++j)
        {
            if (i>=0 && i<h && j>=0 && j<w)
            {
                sum_r+=(double)pR[w*i+j];
                sum_g1+=(double)pG1[w*i+j];
                sum_g2+=(double)pG2[w*i+j];
                sum_b+=(double)pB[w*i+j];
                cnt_t+=1;
            }
        }
    }

    double sum_g_average = (sum_g1 +sum_g2)/2;

    if (sum_g1>0 && sum_g2>0)
    {
        if (sum_g1*100<sum_r)
            r_g = 100;
        else
            r_g = sum_r/sum_g_average;
        if (sum_g2*100<sum_b)
            b_g = 100;
        else
            b_g = sum_b/sum_g_average;
        if (sum_g2*100<sum_g1)
            g_g = 100;
        else
            g_g = sum_g1/sum_g2;
    }

    gr_gb_diff = std::abs((sum_g1-sum_g2)/(w_rect*h_rect));
}




void AlgorithmFuncs::get_roi_colorratio (uchar *pR, uchar *pG1, uchar *pG2, uchar *pB,
                                         int w, int h, int x, int y,int w_rect, int h_rect,
                                         double &g_g, double &r_g, double &b_g, double &gr_gb_diff)
{
    double sum_r = 0.1;
    double sum_g1 = 0.1;
    double sum_g2 = 0.1;
    double sum_b = 0.1;
    int cnt_t = 0;
    for (int i=y; i<y+h_rect; ++i)
    {
        for (int j=x; j<x+w_rect; ++j)
        {
            if (i>=0 && i<h && j>=0 && j<w)
            {
                sum_r+=(double)pR[w*i+j];
                sum_g1+=(double)pG1[w*i+j];
                sum_g2+=(double)pG2[w*i+j];
                sum_b+=(double)pB[w*i+j];
                cnt_t+=1;
            }
        }
    }

    double sum_g_average = (sum_g1 +sum_g2)/2;

    if (sum_g1>0 && sum_g2>0)
    {
        if (sum_g1*100<sum_r)
            r_g = 100;
        else
            r_g = sum_r/sum_g_average;
        if (sum_g2*100<sum_b)
            b_g = 100;
        else
            b_g = sum_b/sum_g_average;
        if (sum_g2*100<sum_g1)
            g_g = 100;
        else
            g_g = std::abs((sum_g1/sum_g2));
    }

    gr_gb_diff = std::abs((sum_g1-sum_g2)/(w_rect*h_rect));
}


void AlgorithmFuncs::get_roi_colorratio_5X5 (uchar *pR, uchar *pG1, uchar *pG2, uchar *pB,
                                             int w, int h, int x, int y,int w_rect, int h_rect,
                                             double &g_g, double &r_g, double &b_g, double &gr_gb_diff)
{
    double sum_r = 0.1;
    double sum_g1 = 0.1;
    double sum_g2 = 0.1;
    double sum_b = 0.1;
    int cnt_t = 0;
    for (int i=y; i<y+h_rect; ++i)
    {
        for (int j=x; j<x+w_rect; ++j)
        {
            if (i>=0 && i<h && j>=0 && j<w)
            {
                sum_r+=(double)pR[w*i+j];
                sum_g1+=(double)pG1[w*i+j];
                sum_g2+=(double)pG2[w*i+j];
                sum_b+=(double)pB[w*i+j];
                cnt_t+=1;
            }
        }
    }

    double sum_g_average = (sum_g1 +sum_g2)/2;

    if (sum_g1>0 && sum_g2>0)
    {
        if (sum_g1*100<sum_r)
            r_g = 100;
        else
            r_g = sum_r/sum_g_average;
        if (sum_g2*100<sum_b)
            b_g = 100;
        else
            b_g = sum_b/sum_g_average;
        if (sum_g2*100<sum_g1)
            g_g = 100;
        else
            g_g = std::abs((sum_g1-sum_g2)/sum_g_average);
    }

    gr_gb_diff = std::abs((sum_g1-sum_g2)/(w_rect*h_rect));
}


double AlgorithmFuncs::GetChannelMean (uchar *pData, int w, int h)
{
    int cnt = 0;
    double mean_t = 0;
    for (int i=0; i<h; i++)
    {
        for (int j=0; j<w; j+=2)
        {
            mean_t+=pData[w*i+j];
            mean_t+=pData[w*i+j+1];
            cnt+=2;
        }
    }
    return mean_t/cnt;
}



double AlgorithmFuncs::GetChannelMean_Doublebyte (uchar *pData, int w, int h)
{
    int cnt = 0;
    double mean_t = 0;
    for (int i=0; i<2*w*h; i+=4)
    {
        mean_t+=(pData[i+1]&0x03)<<8 | pData[i];
        mean_t+=(pData[i+3]&0x03)<<8 | pData[i+2];
        cnt+=2;
    }
    return mean_t/cnt;
}


double AlgorithmFuncs::GetROIMean (uchar *pData, int width, int height, int col, int line,  int win_w, int win_h)
{
    int cnt = 0;
    double mean_t = 0;
    if(line+win_h>=height || col+win_w>=width)
        return -1;
    for (int i=line; i<=line+win_h; ++i)
    {
        for (int j=col; j<=col+win_w; ++j)
        {
            mean_t+=pData[width*i+j];
            cnt+=1;
        }
    }
    return mean_t/cnt;
}



double AlgorithmFuncs::GetROIMean_short (short *pData, int width, int height, int col, int line,  int win_w, int win_h)
{
    int cnt = 0;
    double mean_t = 0;
    if(line+win_h>=height || col+win_w>=width)
        return -1;
    for (int i=line; i<=line+win_h; ++i)
    {
        for (int j=col; j<=col+win_w; ++j)
        {
            mean_t+=pData[width*i+j];
            cnt+=1;
        }
    }
    return mean_t/cnt;
}



bool AlgorithmFuncs::Get_ROILinearity_Col_Ushort (ushort *pData, int width, int height, int win_x0, int win_y0,  int win_w, int win_h, double* linear, double max_dn)
{
    int cnt = 0;
    int index = -1;
    //double max_dn = 4095;
    double temp = 0;
    double mean_x = 0;//横坐标均值
    double mean_y = 0;//纵坐标均值，代指DN
    double diffxxbar = 0;
    double diffyybar = 0;
    double cov_x_x = 0;
    double cov_x_y = 0;
    double cov_y_y = 0;
    double slope = 0;
    double intercept = 0;
    double r= 0.0;
    double r_2 = 0.0;
    double m_dn = 0.0;
    QVector<double> mean_each_col;
    QVector<double> mean_each_num;
//    if(win_y0+win_h>height || win_x0+win_w != width)//防呆
    if(win_y0+win_h>height/* || win_x0+win_w != width*/)//防呆
        return false;
    if(win_x0+win_w>width)//防呆
        return false;
//    if(win_h<2 || win_w <10)//防呆
//        return false;

    for (int j=win_x0; j<win_x0+win_w; ++j)
    {
        cnt = 0;
        temp = 0;
        m_dn = 0.0;
        for (int i=win_y0; i<win_y0+win_h; ++i)
        {
            temp += (double)pData[width*i+j];
            cnt += 1;
            if(m_dn <= (double)pData[width*i+j])
            {
                m_dn = (double)pData[width*i+j];
            }
        }
        temp = temp/cnt;
        mean_y += temp;
        mean_x += j;
        mean_each_num.push_back(j);
        mean_each_col.push_back(temp);

        if(m_dn >= max_dn)
        {
            index = j - win_x0;
            break;
        }
//        if(fabs(m_dn - max_dn) < 0.000001)
//        {
//            index = j - win_x0;
//            break;
//        }
    }

    if(index == -1)//index = -1 说明未饱和
    {
        index = win_w - 1;
    }
    else if(index == 1 || index == 0)//index =  0 / 1 说明前几列就饱和
    {
        return false;
    }

    mean_x /= index;
    mean_y /= index;

    for(int j=0; j<=index; ++j)
    {
        diffxxbar = mean_each_num.at(j) - mean_x;
        diffyybar = mean_each_col.at(j) - mean_y;
        cov_x_y += diffxxbar * diffyybar;
        cov_x_x += diffxxbar * diffxxbar;
        cov_y_y += diffyybar * diffyybar;
    }
    cov_x_y /= index - 1;
    cov_x_x /= index - 1;
    cov_y_y /= index - 1;
    if(cov_x_x<0.000001 || cov_y_y <0.000001)//防呆
    {
        return false;
    }
    r = cov_x_y / std::sqrt(cov_x_x*cov_y_y);
    r_2 = r * r;
    slope = cov_x_y / cov_x_x;
    intercept = mean_y - slope * mean_x;
    linear[0] = slope;
    linear[1] = intercept;
    linear[2] = r_2 * 100;
    linear[3] = m_dn;
    return true;
}


bool AlgorithmFuncs::Get_Multi_ROI_Mean_short (short *pData_H, short *pData_L, short *pData_Diff, int width, int height,
                                                 int col, int line,  int win_w, int win_h, QVector<double> &str_log)
{
    int cnt = 0;
    double mean_H = 0;
    double mean_L = 0;
    double mean_Diff = 0;
    if(line+win_h>=height || col+win_w>=width)
        return false;
    for (int i=line; i<=line+win_h; ++i)
    {
        for (int j=col; j<=col+win_w; ++j)
        {
            mean_H+=pData_H[width*i+j];
            mean_L+=pData_L[width*i+j];
            mean_Diff+=pData_Diff[width*i+j];
            cnt+=1;
        }
    }
    str_log.append(mean_H/cnt);
    str_log.append(mean_L/cnt);
    str_log.append(mean_Diff/cnt);
    return true;
}


void AlgorithmFuncs::Get_ROI_MEAN_STD (uchar *pData, cv::Rect &roi, int width, int height, double &mean, double &std)
{
    int w = width;
    int h = height;
    int cnt = 0;
    double mean_t = 0;
    double std_t = 0;

    for(int i=roi.y; i<roi.br().y; ++i)
        for(int j=roi.x; j<roi.br().x; ++j)
        {
            mean_t+= pData[i*w+j];
            cnt+=1;
        }
    mean = mean_t/cnt;

    for(int i=roi.y; i<roi.br().y; ++i)
        for(int j=roi.x; j<roi.br().x; ++j)
        {
            std_t+=(double)((mean-pData[i*w+j])*(mean-pData[i*w+j]));
        }

    std = std::sqrt (std_t/(cnt-1));
}



void AlgorithmFuncs::Get_ROI_MEAN_Double_chao ( double *pData, Point2i start_loca, cv::Rect &roi, int width,
                                               double& split_mean, double& split_std_v)
{
    int w = width;
    int cnt = 0;
    double split_mean_=0;
    double split_std_v_=0;

    //求解各通道均值
    for(int i=roi.y; i<roi.br().y; ++i)
    {
        for(int j=roi.x; j<roi.br().x; ++j)
        {
            int loca_y= 2*i + start_loca.y;
            int loca_x= 2*j + start_loca.x;
            split_mean_+= pData[ loca_y*w + loca_x ];
            cnt++;
        }
    }
    split_mean_/=cnt;

    //求解各通道方差
    for(int i=roi.y; i<roi.br().y; ++i)
    {
        for(int j=roi.x; j<roi.br().x; ++j)
        {
            int loca_y= 2*i + start_loca.y;
            int loca_x= 2*j + start_loca.x;
            double data_val= pData[ loca_y*w + loca_x ];
            split_std_v_+=(split_mean_-data_val)*(split_mean_-data_val);
        }
    }
    split_std_v_ = std::sqrt (split_std_v_/(cnt-1));

    split_mean=split_mean_;
    split_std_v=split_std_v_;
}



double AlgorithmFuncs::GetROIMean_col_min (uchar *pData, int width, int height,double &mean_max,double &mean_min,double &pixel_max,double &mean_left,double &mean_right,int &pos_max,int &pos_min)
{

    int avg = 0;
    QVector <double>  mean_column;
    QVector <double>  mean;
    FILE* fp = fopen("mean_column.csv","wt");
    for(int i = 0; i < width; ++i)
    {
        avg=0;
        for (int j=0; j<height; j++)
        {
            avg += pData[j*width+i];
            mean.append(pData[j*width+i]);
        }
        avg = avg/height;
        mean_column.append(avg);
        fprintf(fp, "%4.2f,", avg);
    }
    fclose(fp);
    double mean_left_ = 0;
    double mean_right_ = 0;
    int sum = 0;
    int ROI_W = width/10;
    int ROI_H = height/5;
    for(int i = (ROI_W*2); i < (ROI_W*3); ++i)
    {
        for (int j=ROI_H; j<ROI_H*2; j++)
        {
            mean_left_ += pData[j*width+i];
            sum++;
        }
    }
    mean_left_ = mean_left_/sum;
    sum = 0;
    for(int i = (ROI_W*7); i < ROI_W*8; ++i)
    {
        for (int j=ROI_H; j<ROI_H*2; j++)
        {
            mean_right_ += pData[j*width+i];
            sum++;
        }
    }
    mean_right_ = mean_right_/sum;
    QVector <double>::const_iterator max_mean_col = std::max_element(mean_column.begin(),mean_column.end());
    QVector <double>::const_iterator min_mean_col = std::min_element(mean_column.begin(),mean_column.end());
    QVector <double>::const_iterator max_pixel = std::max_element(mean.begin(),mean.end());

    int max_pos = mean_column.indexOf(*max_mean_col);
    int min_pos = mean_column.indexOf(*min_mean_col);

    mean_max = *max_mean_col;
    mean_min = *min_mean_col;
    pos_max = max_pos;
    pos_min = min_pos;
    pixel_max = *max_pixel;
    mean_right = mean_right_;
    mean_left = mean_left_;

    return mean_max;
}


void AlgorithmFuncs::Get_ROI_MEAN_Double (double *pData, cv::Rect &roi, int width, int height, double &mean, double &std)
{
    int w = width;
    int h = height;
    int cnt = 0;
    double mean_t = 0;
    double std_t = 0;

    for(int i=roi.y; i<roi.br().y; ++i)
        for(int j=roi.x; j<roi.br().x; ++j)
        {
            mean_t+= pData[i*w+j];
            cnt+=1;
        }
    mean = mean_t/cnt;

    for(int i=roi.y; i<roi.br().y; ++i)
        for(int j=roi.x; j<roi.br().x; ++j)
        {
            std_t+=(double)((mean-pData[i*w+j])*(mean-pData[i*w+j]));
        }

    std = std::sqrt (std_t/(cnt-1));
}



double AlgorithmFuncs::GetROIMean_Doublebyte (unsigned short *pData, int width, int height, int col, int line,  int win_w, int win_h)
{
    int cnt = 0;
    double mean_t = 0;
    if(line+win_h>=height || col+win_w>=width)
        return -1;
    for (int i=line; i<line+win_h; ++i)
    {
        for (int j=col; j<col+win_w; ++j)
        {
            mean_t+= pData[width*i+j];
            cnt+=1;
        }
    }
    return mean_t/cnt;
}


int  AlgorithmFuncs::Get_GridROI_Mean_Doublebyte (short *pData, int width, int height, int cnt_col, int cnt_row, QVector<double> &GridROI_Mean)
{

    if(cnt_col<=0 || cnt_row<=0)
        return -1;
    int col_step = width/cnt_col;
    int row_step = height/cnt_row;


    for (int m=0; m<cnt_row; ++m)
        for(int n=0; n<cnt_col; ++n)
        {
            double mean_t = 0;
            int cnt = 0;

            int line = 0 + row_step*m;
            int col  = 0  + col_step*n;
            for (int i=line; i<line+row_step; ++i)
            {
                for (int j=col; j<col+col_step; ++j)
                {
                    mean_t+= pData[width*i+j];
                    cnt+=1;
                }
            }
            GridROI_Mean.push_back(mean_t/cnt);
        }
    return 1;
}




double AlgorithmFuncs::Get_Real_ROI_PRNU (float *Delta, uchar *raw_data, int width, int height, int col,
                                          int line,  int win_w, int win_h)
{
    int cnt = 0;
    double mean_t = 0;
    if(line+win_h>=height || col+win_w>=width)
        return -1;
    for (int i=line; i<line+win_h; ++i)
    {
        for (int j=col; j<col+win_w; ++j)
        {
            mean_t+=Delta[width*i+j] * Delta[width*i+j];
            cnt+=1;
        }
    }
    double sigma =std::sqrt(mean_t/(cnt-1));


    cnt = 0;
    mean_t = 0;
    for (int i=line; i<line+win_h; ++i)
    {
        for (int j=col; j<col+win_w; ++j)
        {
            mean_t+=raw_data[width*i+j];
            cnt+=1;
        }
    }

    double mean = mean_t/cnt;

    return sigma/mean*100;
}




//合并多张图片
void AlgorithmFuncs::SeveralFrameToOne (uchar *pIn, uchar *pOut,
                                     int width, int height,
                                     int frameCnt, int Datatype)
{
    if(Datatype==8)
    {
        int w = 2*width;
        int h = height;
        if (frameCnt == 1)
            memcpy (pOut,pIn,w*h);      // 只有 1 帧，直接复制，不需要平均
        else if (frameCnt>1)
        {
            for (int i=0; i<h; i++)     // 遍历每一行
            {
                for (int j=0; j<w; j+=4)        // 每次处理 4 字节（= 2 个像素）
                {
                    int val_tmp1 = 0;
                    int val_tmp2 = 0;

                    for (int index=0; index<frameCnt; ++index)
                    {
                        val_tmp1+= (*(pIn+index*(w*h)+(w*i+j+1))&0xff)<<8 | *(pIn+index*(w*h)+(w*i+j)) ;     // 高字节地址,低字节地址
                        val_tmp2+= (*(pIn+index*(w*h)+(w*i+j+3))&0xff)<<8 | *(pIn+index*(w*h)+(w*i+j+2)) ;
                    }
                    int tmp1 = val_tmp1/frameCnt;           // 像素 A 的平均值
                    int tmp2 = val_tmp2/frameCnt;           // 像素 B 的平均值

                    pOut[w*i+j] = tmp1&0x00ff;              // 像素 A 低 8 位
                    pOut[w*i+j+1] = (tmp1&0xff00)>>8;       // 像素 A 高 2 位
                    pOut[w*i+j+2] = tmp2&0x00ff;            // 像素 B 低 8 位
                    pOut[w*i+j+3] = (tmp2&0xff00)>>8;       // 像素 B 高 2 位
                }
            }
        }
    }
    else if(Datatype==10)
    {
        int w = 2*width;
        int h = height;
        if (frameCnt == 1)
            memcpy (pOut,pIn,w*h);      // 只有 1 帧，直接复制，不需要平均
        else if (frameCnt>1)
        {
            for (int i=0; i<h; i++)     // 遍历每一行
            {
                for (int j=0; j<w; j+=4)        // 每次处理 4 字节（= 2 个像素）
                {
                    int val_tmp1 = 0;
                    int val_tmp2 = 0;

                    for (int index=0; index<frameCnt; ++index)
                    {
                        val_tmp1+= (*(pIn+index*(w*h)+(w*i+j+1))&0x03)<<8 | *(pIn+index*(w*h)+(w*i+j)) ;     // 高字节地址,低字节地址
                        val_tmp2+= (*(pIn+index*(w*h)+(w*i+j+3))&0x03)<<8 | *(pIn+index*(w*h)+(w*i+j+2)) ;
                    }
                    int tmp1 = val_tmp1/frameCnt;           // 像素 A 的平均值
                    int tmp2 = val_tmp2/frameCnt;           // 像素 B 的平均值

                    pOut[w*i+j] = tmp1&0x00ff;              // 像素 A 低 8 位
                    pOut[w*i+j+1] = (tmp1&0x0300)>>8;       // 像素 A 高 2 位
                    pOut[w*i+j+2] = tmp2&0x00ff;            // 像素 B 低 8 位
                    pOut[w*i+j+3] = (tmp2&0x0300)>>8;       // 像素 B 高 2 位
                }
            }
        }
    }

    else if(Datatype == 12)
    {
        int w = 2 * width;
        int h = height;
        if (frameCnt == 1)
            memcpy(pOut, pIn, w * h);
        else if (frameCnt > 1)
        {
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j += 4)
                {
                    int val_tmp1 = 0;
                    int val_tmp2 = 0;

                    for (int index = 0; index < frameCnt; ++index)
                    {
                        val_tmp1 += (*(pIn + index*(w*h) + (w*i+j+1)) & 0x0F) << 8
                                  |  *(pIn + index*(w*h) + (w*i+j));
                        val_tmp2 += (*(pIn + index*(w*h) + (w*i+j+3)) & 0x0F) << 8
                                  |  *(pIn + index*(w*h) + (w*i+j+2));
                    }
                    int tmp1 = val_tmp1 / frameCnt;     // 像素 A 平均值
                    int tmp2 = val_tmp2 / frameCnt;     // 像素 B 平均值

                    pOut[w*i+j]   =  tmp1 & 0x00FF;          // 像素 A 低 8 位
                    pOut[w*i+j+1] = (tmp1 & 0x0F00) >> 8;    // 像素 A 高 4 位
                    pOut[w*i+j+2] =  tmp2 & 0x00FF;          // 像素 B 低 8 位
                    pOut[w*i+j+3] = (tmp2 & 0x0F00) >> 8;    // 像素 B 高 4 位
                }
            }
        }
    }

    else if(Datatype==14)
    {
        int w = 2*width;
        int h = height;
        if (frameCnt == 1)
            memcpy (pOut,pIn,w*h);
        else if (frameCnt>1)
        {
            for (int i=0; i<h; i++)
            {
                for (int j=0; j<w; j+=4)
                {
                    int val_tmp1 = 0;
                    int val_tmp2 = 0;

                    for (int index=0; index<frameCnt; ++index)
                    {
                        val_tmp1+= (*(pIn+index*(w*h)+(w*i+j+1))&0x3f)<<8 | *(pIn+index*(w*h)+(w*i+j)) ;
                        val_tmp2+= (*(pIn+index*(w*h)+(w*i+j+3))&0x3f)<<8 | *(pIn+index*(w*h)+(w*i+j+2)) ;
                    }
                    int tmp1 = val_tmp1/frameCnt;
                    int tmp2 = val_tmp2/frameCnt;

                    pOut[w*i+j] = tmp1&0x00ff;
                    pOut[w*i+j+1] = (tmp1&0x3f00)>>8;
                    pOut[w*i+j+2] = tmp2&0x00ff;
                    pOut[w*i+j+3] = (tmp2&0x3f00)>>8;
                }
            }
        }
    }

    else if(Datatype == 16)
    {
        int w = 2 * width;
        int h = height;
        if (frameCnt == 1)
            memcpy(pOut, pIn, w * h);
        else if (frameCnt > 1)
        {
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j += 4)
                {
                    int val_tmp1 = 0;
                    int val_tmp2 = 0;

                    for (int index = 0; index < frameCnt; ++index)
                    {
                        val_tmp1 += (*(pIn + index*(w*h) + (w*i+j+1)) & 0xFF) << 8
                                  |  *(pIn + index*(w*h) + (w*i+j));
                        val_tmp2 += (*(pIn + index*(w*h) + (w*i+j+3)) & 0xFF) << 8
                                  |  *(pIn + index*(w*h) + (w*i+j+2));
                    }
                    int tmp1 = val_tmp1 / frameCnt;     // 像素 A 平均值
                    int tmp2 = val_tmp2 / frameCnt;     // 像素 B 平均值

                    pOut[w*i+j]   =  tmp1 & 0x00FF;          // 像素 A 低 8 位
                    pOut[w*i+j+1] = (tmp1 & 0xFF00) >> 8;    // 像素 A 高 8 位
                    pOut[w*i+j+2] =  tmp2 & 0x00FF;          // 像素 B 低 8 位
                    pOut[w*i+j+3] = (tmp2 & 0xFF00) >> 8;    // 像素 B 高 8 位
                }
            }
        }
    }
    else
        return;

    /*
    if(Datatype == 8)
    {
        int w = width;      // 注意：8-bit 时每像素 1 字节，w = width（不乘2）
        int h = height;

        if (frameCnt == 1)
            memcpy(pOut, pIn, w * h);
        else if (frameCnt > 1)
        {
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)     // 每次处理 1 字节 = 1 个像素
                {
                    int val_tmp = 0;

                    for (int index = 0; index < frameCnt; ++index)
                    {
                        val_tmp += *(pIn + index*(w*h) + (w*i + j));
                    }

                    pOut[w*i + j] = val_tmp / frameCnt;     // 直接存储平均值
                }
            }
        }
    }

    */


}



void AlgorithmFuncs::from_doublebyte_to_raw8 (uchar *pIn, uchar *pOut,int w, int h, int raw_bits)
{
    if (raw_bits == 8)
    {
        for (int i=0; i<h; i++)
        {
            for (int j=0; j<2*w; j+=4)
            {
                int m = j >> 1;
                pOut[w*i + m]     = pIn[2*w*i + j + 1];
                pOut[w*i + m + 1] = pIn[2*w*i + j + 3];

            }
        }
        return;
    }
    if (raw_bits == 10)
    {
        for (int i=0; i<h; i++)
        {
            for (int j=0; j<2*w; j+=4)
            {
                int m = j >> 1;
                pOut[w*i + m]     = static_cast<uchar>((pIn[2*w*i + j]     >> 2) | (((pIn[2*w*i + j + 1]) & 0x03) << 6));
                pOut[w*i + m + 1] = static_cast<uchar>((pIn[2*w*i + j + 2] >> 2) | (((pIn[2*w*i + j + 3]) & 0x03) << 6));
            }
        }
        return;
    }
    if (raw_bits == 12)
    {
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < 2*w; j += 4)
            {
                int m = j >> 1;
                pOut[w*i + m]     = static_cast<uchar>((pIn[2*w*i + j]     >> 4) | (((pIn[2*w*i + j + 1]) & 0x0F) << 4));
                pOut[w*i + m + 1] = static_cast<uchar>((pIn[2*w*i + j + 2] >> 4) | (((pIn[2*w*i + j + 3]) & 0x0F) << 4));
            }
        }
        return;
    }
    if (raw_bits == 14)
    {
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < 2*w; j += 4)
            {
                int m = j >> 1;
                pOut[w*i + m]     = static_cast<uchar>((pIn[2*w*i + j]     >> 6) | (((pIn[2*w*i + j + 1]) & 0x3F) << 2));
                pOut[w*i + m + 1] = static_cast<uchar>((pIn[2*w*i + j + 2] >> 6) | (((pIn[2*w*i + j + 3]) & 0x3F) << 2));
            }
        }
        return;
    }
    if (raw_bits == 16)
    {
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < 2*w; j += 4)
            {
                int m = j >> 1;
                pOut[w*i + m]     = pIn[2*w*i + j + 1];
                pOut[w*i + m + 1] = pIn[2*w*i + j + 3];
            }
        }
        return;
    }
    /* 其它位深：双字节 LE，取有效值的高 8 位 */
    {
        unsigned int low_bits = static_cast<unsigned int>(raw_bits - 8);
        unsigned int mask = (1u << low_bits) - 1u;
        int rshift = raw_bits - 8;
        for (int i=0; i<h; i++)
        {
            for (int j=0; j<2*w; j+=4)
            {
                int m=j>>1;
                unsigned int b0 = pIn[2*w*i+j];
                unsigned int b1 = pIn[2*w*i+j+1] & mask;
                unsigned int b2 = pIn[2*w*i+j+2];
                unsigned int b3 = pIn[2*w*i+j+3] & mask;
                unsigned int v0 = b0 | (b1 << 8);
                unsigned int v1 = b2 | (b3 << 8);
                pOut[w*i+m] = static_cast<uchar>(v0 >> rshift);
                pOut[w*i+m+1] = static_cast<uchar>(v1 >> rshift);
            }
        }
    }
}


void AlgorithmFuncs:: RGGB_Map_Integrate_Raw16 (uchar *g_buf_r_16_, uchar *g_buf_gr_16_, uchar *g_buf_gb_16_, uchar *g_buf_b_16_,
                                                uchar *g_buf_raw_16_, int w, int h, RGB_BAYER bayer)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    if (bayer == RG)
    {
        pb1 = g_buf_r_16_;
        pb2 = g_buf_gr_16_;
        pb3 = g_buf_gb_16_;
        pb4 = g_buf_b_16_;
    }
    else if (bayer == BG)
    {
        pb1 = g_buf_b_16_;
        pb2 = g_buf_gb_16_;
        pb3 = g_buf_gr_16_;
        pb4 = g_buf_r_16_;
    }
    else if (bayer == GR)
    {
        pb1 = g_buf_gr_16_;
        pb2 = g_buf_r_16_;
        pb3 = g_buf_b_16_;
        pb4 = g_buf_gb_16_;
    }
    else if (bayer == GB)
    {
        pb1 = g_buf_gb_16_;
        pb2 = g_buf_b_16_;
        pb3 = g_buf_r_16_;
        pb4 = g_buf_gr_16_;
    }

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<2*w; j+=2)
        {
            int m=j/2;
            if (i%2 == 0 && m%2 == 0)
            {
               g_buf_raw_16_[2*w*i+j] = *pb1++;
               g_buf_raw_16_[2*w*i+j+1] = *pb1++;
            }
            else if(i%2 == 0 && m%2 == 1)
            {
               g_buf_raw_16_[2*w*i+j] = *pb2++;
               g_buf_raw_16_[2*w*i+j+1] = *pb2++;
            }
            else if(i%2 == 1 && m%2 == 0)
            {
               g_buf_raw_16_[2*w*i+j] = *pb3++;
               g_buf_raw_16_[2*w*i+j+1] = *pb3++;
            }
            else if(i%2 == 1 && m%2 == 1)
            {
               g_buf_raw_16_[2*w*i+j] = *pb4++;
               g_buf_raw_16_[2*w*i+j+1] = *pb4++;
            }
        }
    }
}





template<typename T>
void AlgorithmFuncs::RGGB_Map_Integrate_chao(T *g_binary_r_, T *g_binary_gr_, T *g_binary_gb_,
                                             T *g_binary_b_, T *g_binary_, int w, int h,
                                             RGB_BAYER bayer)
{
    T *pb1 = NULL;
    T *pb2 = NULL;
    T *pb3 = NULL;
    T *pb4 = NULL;
    if (bayer == RG)
    {
        pb1 = g_binary_r_;
        pb2 = g_binary_gr_;
        pb3 = g_binary_gb_;
        pb4 = g_binary_b_;
    }
    else if (bayer == BG)
    {
        pb1 = g_binary_b_;
        pb2 = g_binary_gb_;
        pb3 = g_binary_gr_;
        pb4 = g_binary_r_;
    }
    else if (bayer == GR)
    {
        pb1 = g_binary_gr_;
        pb2 = g_binary_r_;
        pb3 = g_binary_b_;
        pb4 = g_binary_gb_;
    }
    else if (bayer == GB)
    {
        pb1 = g_binary_gb_;
        pb2 = g_binary_b_;
        pb3 = g_binary_r_;
        pb4 = g_binary_gr_;
    }

    for (int i=0; i<h; i=i+2)
    {
        for (int j=0; j<w; j=j+2)
        {
            g_binary_[w*i+j] =  *pb1++;
            g_binary_[w*i+j+1] = *pb2++;
        }
        for (int j=0; j<w; j=j+2)
        {
            g_binary_[w*(i+1)+j] = *pb3++;
            g_binary_[w*(i+1)+j+1] = *pb4++;
        }
    }
}

template void
AlgorithmFuncs::RGGB_Map_Integrate_chao<float>(float *, float *, float *,float*,float*,int,
int, RGB_BAYER );

template void
AlgorithmFuncs::RGGB_Map_Integrate_chao<uchar>(uchar *, uchar *, uchar *,uchar*,uchar*,int,
int, RGB_BAYER );

template void
AlgorithmFuncs::RGGB_Map_Integrate_chao<double>(double *, double *, double *,double*,double*,int,
int, RGB_BAYER );

void AlgorithmFuncs:: RGGB_Map_Integrate_4C (uchar *g_binary_r_, uchar *g_binary_gr_, uchar *g_binary_gb_,
                                             uchar *g_binary_b_, uchar *g_binary_, int w, int h, RGB_BAYER bayer)
{
    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    if (bayer == RG)
    {
        pb1 = g_binary_r_;
        pb2 = g_binary_gr_;
        pb3 = g_binary_gb_;
        pb4 = g_binary_b_;
    }
    else if (bayer == BG)
    {
        pb1 = g_binary_b_;
        pb2 = g_binary_gb_;
        pb3 = g_binary_gr_;
        pb4 = g_binary_r_;
    }
    else if (bayer == GR)
    {
        pb1 = g_binary_gr_;
        pb2 = g_binary_r_;
        pb3 = g_binary_b_;
        pb4 = g_binary_gb_;
    }
    else if (bayer == GB)
    {
        pb1 = g_binary_gb_;
        pb2 = g_binary_b_;
        pb3 = g_binary_r_;
        pb4 = g_binary_gr_;
    }

    for (int i=0; i<h; i=i+4)
    {
        for(int k=0;k!=2;++k)
        {
            for (int j=0; j<w; j=j+4)
            {
                int loca=w*(i+k)+j;
                g_binary_[loca]=  *pb1++;
                g_binary_[loca+1]=  *pb1++;
                g_binary_[loca+2]=  *pb2++;
                g_binary_[loca+3]=  *pb2++;
            }
        }
        for(int k=2;k!=4;++k)
        {
            for (int j=0; j<w; j=j+4)
            {
                int loca=w*(i+k)+j;
                g_binary_[loca]=  *pb3++;
                g_binary_[loca+1]=  *pb3++;
                g_binary_[loca+2]=  *pb4++;
                g_binary_[loca+3]=  *pb4++;
            }
        }
    }
}


void AlgorithmFuncs:: bychannel_MapBinarization (const uchar *in_raw8, uchar *out_raw8, int width, int height, int skip_cnt, int radius, double binary_spec, Spec_Mode mode)
{
    int skipCnt=0;
    const int w = width;
    const int h = height;
    int r = radius;
    double spec = binary_spec;
    vector<double> vec_up , vec_done;

    for(int i=0;i!=w;++i)
    {
        double r_num=0;
        for(int j=0;j!=r;++j)
            r_num+= (double)in_raw8[w*j+i];
        vec_up.push_back(r_num);

        r_num=0;
        for(int j=r+1;j!=r*2+1;++j)
            r_num+=(double)in_raw8[w*j+i];
        vec_done.push_back(r_num);
    }

    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        //当 i>r &&i<h-r   更新vec_up vec_done   sum_t
        if( i>r && i< h-r )
        {
            for(int j=0;j!=w;++j)
            {
                vec_up[j]   = vec_up[j]   - (double)in_raw8[w* (i-r-1) + j] + (double)in_raw8[w* (i - 1) + j] ;
                vec_done[j] = vec_done[j] - (double)in_raw8[w*i        + j] + (double)in_raw8[w* (i + r) + j] ;
            }
        }

        vector<double> sum_t(4,0.0);
        for(int x=0; x<r; x++)
        {
            sum_t[0]+=vec_up[x];
            sum_t[1]+=vec_done[x];
            sum_t[2]+=vec_up[x+r+1];
            sum_t[3]+=vec_done[x+r+1];
        }

        for (int j=skipCnt; j<w-skipCnt; ++j)
        {

            if( j>r && j< w-r)
            {
                sum_t[0] = sum_t[0] - vec_up[j-r-1]   + vec_up[j-1] ;
                sum_t[1] = sum_t[1] - vec_done[j-r-1] + vec_done[j-1] ;
                sum_t[2] = sum_t[2] - vec_up[j]       + vec_up[j+r] ;
                sum_t[3] = sum_t[3] - vec_done[j]     + vec_done[j+r] ;
            }

            int cnt = 4*r*r;
            double mean_t=0;
            for(int n=0; n<4; n++)
                mean_t+= sum_t[n];
            mean_t=mean_t/cnt;

            double compareValue=0;
            if(mode==Ratio)
                compareValue= std::fabs ((double)in_raw8[w*i+j]-mean_t)/mean_t;
            else if(mode==DN)
                compareValue = std::fabs ((double)in_raw8[w*i+j]-mean_t);

            if (compareValue>=spec)
                out_raw8[w*i+j] = 255;
            else
                out_raw8[w*i+j] = 0;
        }
    }
}





void AlgorithmFuncs:: bychannel_MapBinarization_delta (const uchar *in_raw8, uchar *out_raw8, int width, int height, int skip_cnt, int radius,
                                                       double binary_spec, Spec_Mode mode, double *delta_map)
{

    int skipCnt=0;
    int w = width;
    int h = height;
    int r = radius;
    double spec = binary_spec;

    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        for (int j=skipCnt; j<w-skipCnt; ++j)
        {
            double mean_t = 0.0;
            int cnt = 0;
            int row_start = i-r;
            int row_end = i+r;

            if (row_start<0)
            {
                row_start = 0;
                row_end = row_start+2*r;
            }
            else if (row_end>(h-1))
            {
                row_end = h-1;
                row_start = row_end-2*r;
            }
            int col_start = j-r;
            int col_end = j+r;
            if (col_start<0)
            {
                col_start = 0;
                col_end = col_start+2*r;
            }
            else if (col_end>(w-1))
            {
                col_end = w-1;
                col_start = col_end-2*r;
            }

            for(int y=0; y<r; y++)
                for(int x=0; x<r; x++)
                {
                    mean_t+=(double)in_raw8[w*(row_start+y)+(col_start+x)];
                    mean_t+=(double)in_raw8[w*(row_start+y)+(col_start+x+r+1)];
                    mean_t+=(double)in_raw8[w*(row_start+y+r+1)+(col_start+x)];
                    mean_t+=(double)in_raw8[w*(row_start+y+r+1)+(col_start+x+r+1)];
                    cnt+=4;
                }

            if (cnt <= 1)
            {
                // this condition: impossible
                continue;
            }

            mean_t/=cnt;
            double mean_ex = mean_t;
            double compareValue =0;
            if(mode==Ratio)
                compareValue = std::fabs ((double)in_raw8[w*i+j]-mean_ex)/mean_t;
            else if(mode==DN)
                compareValue = std::fabs ((double)in_raw8[w*i+j]-mean_ex);

            delta_map[w*i+j]=(double)(in_raw8[w*i+j]-mean_ex);

            if (compareValue>=spec)
            {
                out_raw8[w*i+j] = 255;
            }
            else
            {
                out_raw8[w*i+j] = 0;
            }
        }
    }
}





void AlgorithmFuncs:: bychannel_MapBinarization_add_blemish (const uchar *in_raw8, uchar *out_raw8, uchar *out_blemish, int width, int height, int skip_cnt,
                                                             int radius, double binary_spec, double blemish_spec, Spec_Mode mode)
{
    int skipCnt=0;
    int w = width;
    int h = height;
    int r = radius;
    double spec = binary_spec;
    double spec_b = blemish_spec;

    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        for (int j=skipCnt; j<w-skipCnt; ++j)
        {
            double mean_t[4] = {0.0};
            int cnt = 0;
            int row_start = i-r;
            int row_end = i+r;

            if (row_start<0)
            {
                row_start = 0;
                row_end = row_start+2*r;
            }
            else if (row_end>(h-1))
            {
                row_end = h-1;
                row_start = row_end-2*r;
            }
            int col_start = j-r;
            int col_end = j+r;
            if (col_start<0)
            {
                col_start = 0;
                col_end = col_start+2*r;
            }
            else if (col_end>(w-1))
            {
                col_end = w-1;
                col_start = col_end-2*r;
            }


            for(int y=0; y<r; y++)
                for(int x=0; x<r; x++)
                {
                    mean_t[0]+=(double)in_raw8[w*(row_start+y)+(col_start+x)];
                    mean_t[1]+=(double)in_raw8[w*(row_start+y)+(col_start+x+r+1)];
                    mean_t[2]+=(double)in_raw8[w*(row_start+y+r+1)+(col_start+x)];
                    mean_t[3]+=(double)in_raw8[w*(row_start+y+r+1)+(col_start+x+r+1)];
                    cnt++;
                }

            if (cnt <= 1)
            {
                continue;
            }

            for(int n=0; n<4; n++)
                mean_t[n]/=cnt;

            double compareValue[4] = {0.0};

            if(mode==Ratio)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)in_raw8[w*i+j]-mean_t[n])/mean_t[n];
            }
            else if(mode==DN)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)in_raw8[w*i+j]-mean_t[n]);
            }

            int flag=0;
            for(int n=0; n<4; n++)
                if(compareValue[n]>spec)
                    flag++;


            if (flag>0)
            {
                out_raw8[w*i+j] = 255;
                out_blemish[w*i+j] = 0;
            }
            else
            {
                out_raw8[w*i+j] = 0;

                int flag_b=0;
                for(int n=0; n<4; n++)
                    if(compareValue[n]>spec_b)
                        flag_b++;
                if(flag_b>0)
                    out_blemish[w*i+j] = 255;
                else
                    out_blemish[w*i+j] = 0;
            }
        }
    }
}




void AlgorithmFuncs:: bychannel_pixel_MapBinarization (const uchar *in_raw8, float *delta_data, uchar *out_raw8,
                                                       uchar *out_blemish, int width, int height,int skip_cnt,
                                                       int radius, double binary_spec,double blemish_spec, Spec_Mode mode)
{
    int skipCnt=0;
    const int w = width;
    const int h = height;
    int r = radius;
    double spec = binary_spec;
    double spec_b = blemish_spec;
    vector<double> vec_up , vec_done;

    for(int i=0;i!=w;++i)
    {
        double r_num=0;
        for(int j=0;j!=r;++j)
            r_num+= (double)in_raw8[w*j+i];
        vec_up.push_back(r_num);

        r_num=0;
        for(int j=r+1;j!=r*2+1;++j)
            r_num+=(double)in_raw8[w*j+i];
        vec_done.push_back(r_num);
    }

    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        //当 i>r &&i<h-r   更新vec_up vec_done   sum_t
        if( i>r && i< h-r )
        {
            for(int j=0;j!=w;++j)
            {
                vec_up[j]   = vec_up[j]   - (double)in_raw8[w* (i-r-1) + j] + (double)in_raw8[w* (i - 1) + j] ;
                vec_done[j] = vec_done[j] - (double)in_raw8[w*i        + j] + (double)in_raw8[w* (i + r) + j] ;
            }
        }

        vector<double> sum_t(4,0.0);
        for(int x=0; x<r; x++)
        {
            sum_t[0]+=vec_up[x];
            sum_t[1]+=vec_done[x];
            sum_t[2]+=vec_up[x+r+1];
            sum_t[3]+=vec_done[x+r+1];
        }

        for (int j=skipCnt; j<w-skipCnt; ++j)
        {

            if( j>r && j< w-r)
            {
                sum_t[0] = sum_t[0] - vec_up[j-r-1]   + vec_up[j-1] ;
                sum_t[1] = sum_t[1] - vec_done[j-r-1] + vec_done[j-1] ;
                sum_t[2] = sum_t[2] - vec_up[j]       + vec_up[j+r] ;
                sum_t[3] = sum_t[3] - vec_done[j]     + vec_done[j+r] ;
            }

            int cnt = r*r;
            double mean_t[4] = {0.0};
            for(int n=0; n<4; n++)
                mean_t[n]= sum_t[n]/cnt;

            double compareValue[4] = {0.0};
            if(mode==Ratio)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)in_raw8[w*i+j]-mean_t[n])/mean_t[n];
            }
            else if(mode==DN)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)in_raw8[w*i+j]-mean_t[n]);
            }

            double mean_ref = (mean_t[0]+mean_t[1]+mean_t[2]+mean_t[3])/4;
            delta_data[w*i+j] = (float)in_raw8[w*i+j] -(float)(mean_ref);

            int flag=0;
            for(int n=0; n<4; n++)
                if(compareValue[n]>spec)
                    flag++;


            if (flag>0)
            {
                out_raw8[w*i+j] = 255;
                out_blemish[w*i+j] = 0;
            }
            else
            {
                out_raw8[w*i+j] = 0;

                int flag_b=0;
                for(int n=0; n<4; n++)
                    if(compareValue[n]>spec_b)
                        flag_b++;
                if(flag_b>0)
                    out_blemish[w*i+j] = 255;
                else
                    out_blemish[w*i+j] = 0;
            }
        }
    }


}



void AlgorithmFuncs:: bychannel_pixel_chao (const uchar *in_raw8, float *delta_data, uchar *out_raw8,
                                            uchar *out_blemish, int width, int height,int skip_cnt,
                                            int radius, double binary_spec,double blemish_spec, Spec_Mode mode)
{
    QVector<QPoint> pos_vec_binary;
    QVector<QPoint> pos_vec_blemish;
    int skipCnt=0;
    const int w = width;
    const int h = height;
    int r = radius;
    double spec = binary_spec;
    double spec_b = blemish_spec;
    vector<double> vec_up , vec_done;

    vector<double> delta_data_new,delta_data_old;
    vector<int> blemish_data_new,blemish_data_old;

    for(int i=0;i!=w;++i)
    {
        double r_num=0;
        for(int j=0;j!=r;++j)
            r_num+= (double)in_raw8[w*j+i];
        vec_up.push_back(r_num);

        r_num=0;
        for(int j=r+1;j!=r*2+1;++j)
            r_num+=(double)in_raw8[w*j+i];
        vec_done.push_back(r_num);
    }

    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        //当 i>r &&i<h-r   更新vec_up vec_done   sum_t
        if( i>r && i< h-r )
        {
            for(int j=0;j!=w;++j)
            {
                vec_up[j]   = vec_up[j]   - (double)in_raw8[w* (i-r-1) + j] + (double)in_raw8[w* (i - 1) + j] ;
                vec_done[j] = vec_done[j] - (double)in_raw8[w*i        + j] + (double)in_raw8[w* (i + r) + j] ;
            }
        }

        vector<double> sum_t(4,0.0);
        for(int x=0; x<r; x++)
        {
            sum_t[0]+=vec_up[x];
            sum_t[1]+=vec_done[x];
            sum_t[2]+=vec_up[x+r+1];
            sum_t[3]+=vec_done[x+r+1];
        }

        for (int j=skipCnt; j<w-skipCnt; ++j)
        {

            if( j>r && j< w-r)
            {
                sum_t[0] = sum_t[0] - vec_up[j-r-1]   + vec_up[j-1] ;
                sum_t[1] = sum_t[1] - vec_done[j-r-1] + vec_done[j-1] ;
                sum_t[2] = sum_t[2] - vec_up[j]       + vec_up[j+r] ;
                sum_t[3] = sum_t[3] - vec_done[j]     + vec_done[j+r] ;
            }

            int cnt = r*r;
            double mean_t[4] = {0.0};
            for(int n=0; n<4; n++)
                mean_t[n]= sum_t[n]/cnt;

            double compareValue[4] = {0.0};
            if(mode==Ratio)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)in_raw8[w*i+j]-mean_t[n])/mean_t[n];
            }
            else if(mode==DN)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)in_raw8[w*i+j]-mean_t[n]);
            }

            double mean_ref = (mean_t[0]+mean_t[1]+mean_t[2]+mean_t[3])/4;
            delta_data[w*i+j] = (float)in_raw8[w*i+j] -(float)(mean_ref);

            int flag=0;
            for(int n=0; n<4; n++)
                if(compareValue[n]>spec)
                    flag++;


            if (flag>0)
            {
                out_raw8[w*i+j] = 255;
                pos_vec_binary.push_back(QPoint(i,j));
                out_blemish[w*i+j] = 0;
            }
            else
            {
                out_raw8[w*i+j] = 0;

                int flag_b=0;
                for(int n=0; n<4; n++)
                    if(compareValue[n]>spec_b)
                        flag_b++;
                if(flag_b>0)
                {
                    out_blemish[w*i+j] = 255;
                    pos_vec_blemish.push_back(QPoint(i,j));
                }
                else
                    out_blemish[w*i+j] = 0;
            }
        }
    }

    /*

    unique_ptr<double[]> data_avg(new double[w*h]);

    for(int i=0;i!=w;++i)
    {
        double r_num=0;
        for(int j=0;j!=r;++j)
            r_num+= (double)in_raw8[w*j+i];;
        vec_up.push_back(r_num);
    }

    float r_r=r*r;
    for(int i=0;i!=h;++i)
    {
        //更新vec_up 数组
        if( i>0 && i+r<=h )
        {
            for(int j=0;j!=w;++j)
                vec_up[j]  = vec_up[j] - (double)in_raw8[w * (i-1)+ j] + (double)in_raw8[w* (i+r-1) + j];
        }
        double sumt=0;
        for(int x=0; x<r; x++)
            sumt+=vec_up[x];

        data_avg[i*w]=sumt/r_r;
        for(int j=1;j!= w-r+1 ;++j)
        {
            sumt= sumt - vec_up[j-1]   + vec_up[j+ r-1] ;
            data_avg[i*w+j]=sumt/r_r;
        }
    }

    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        for (int j=skipCnt; j<w-skipCnt; ++j)
        {
            int row_start = i-r;
            if ( row_start < 0 )
                row_start = 0;
            else if ( i+r >= h )
                row_start = h-2*r-1;

            int col_start = j-r;
            if ( col_start < 0)
                col_start = 0;
            else if ( j+r >= w )
                col_start = w-2*r-1;

            double mean_t[4] = {0.0};

            mean_t[0] =  data_avg[row_start*w + col_start];
            mean_t[1] =  data_avg[row_start*w + col_start+r+1];
            mean_t[2] =  data_avg[(row_start+r+1)*w + col_start];
            mean_t[3] =  data_avg[(row_start+r+1)*w + col_start+r+1];

            double compareValue[4] = {0.0};
            if(mode==Ratio)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)in_raw8[w*i+j]-mean_t[n])/mean_t[n];
            }
            else if(mode==DN)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = (double)in_raw8[w*i+j]-mean_t[n];
            }

            double mean_ref = (mean_t[0]+mean_t[1]+mean_t[2]+mean_t[3])/4;
            delta_data[w*i+j] = (float)in_raw8[w*i+j] -(float)(mean_ref);
            //delta_data_new.push_back( delta_data[w*i+j]);

            int flag=0;
            for(int n=0; n<4; n++)
                if(compareValue[n]>spec)
                    flag++;


            if (flag>0)
            {
                out_raw8[w*i+j] = 255;
                out_blemish[w*i+j] = 0;
            }
            else
            {
                out_raw8[w*i+j] = 0;

                int flag_b=0;
                for(int n=0; n<4; n++)
                    if(compareValue[n]>spec_b)
                        flag_b++;
                if(flag_b>0)
                    out_blemish[w*i+j] = 255;
                else
                    out_blemish[w*i+j] = 0;
            }
            //blemish_data_new.push_back(out_blemish[w*i+j]);
        }
    }


    */


    /*
    for (int i=skipCnt; i<h-skipCnt; ++i)
    {
        for (int j=skipCnt; j<w-skipCnt; ++j)
        {
            double mean_t[4] = {0.0};
            int cnt = 0;
            int row_start = i-r;
            int row_end = i+r;

            if (row_start<0)
            {
                row_start = 0;
                row_end = row_start+2*r;
            }
            else if (row_end>(h-1))
            {
                row_end = h-1;
                row_start = row_end-2*r;
            }
            int col_start = j-r;
            int col_end = j+r;
            if (col_start<0)
            {
                col_start = 0;
                col_end = col_start+2*r;
            }
            else if (col_end>(w-1))
            {
                col_end = w-1;
                col_start = col_end-2*r;
            }

            for(int y=0; y<r; y++)
                for(int x=0; x<r; x++)
                {
                    mean_t[0]+=(double)in_raw8[w*(row_start+y)+(col_start+x)];
                    mean_t[1]+=(double)in_raw8[w*(row_start+y)+(col_start+x+r+1)];
                    mean_t[2]+=(double)in_raw8[w*(row_start+y+r+1)+(col_start+x)];
                    mean_t[3]+=(double)in_raw8[w*(row_start+y+r+1)+(col_start+x+r+1)];
                    cnt++;
                }

            if (cnt <= 1)
            {
                continue;
            }

            for(int n=0; n<4; n++)
                mean_t[n]/=cnt;

            double compareValue[4] = {0.0};

            if(mode==Ratio)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = std::fabs ((double)in_raw8[w*i+j]-mean_t[n])/mean_t[n];
            }
            else if(mode==DN)
            {
                for(int n=0; n<4; n++)
                    compareValue[n] = (double)in_raw8[w*i+j]-mean_t[n];
            }

            double mean_ref = (mean_t[0]+mean_t[1]+mean_t[2]+mean_t[3])/4;
            delta_data[w*i+j] = (float)in_raw8[w*i+j] -(float)(mean_ref);
            delta_data_old.push_back( delta_data[w*i+j]);

            int flag=0;
            for(int n=0; n<4; n++)
                if(compareValue[n]>spec)
                    flag++;


            if (flag>0)
            {
                out_raw8[w*i+j] = 255;
                out_blemish[w*i+j] = 0;
            }
            else
            {
                out_raw8[w*i+j] = 0;

                int flag_b=0;
                for(int n=0; n<4; n++)
                    if(compareValue[n]>spec_b)
                        flag_b++;
                if(flag_b>0)
                    out_blemish[w*i+j] = 255;
                else
                    out_blemish[w*i+j] = 0;
            }
            blemish_data_old.push_back(out_blemish[w*i+j]);
        }
    }

*/

/*
    vector<double> delta_data_diff;
    vector<int> delta_num_count;
    vector<int> blemish_data_diff;
    int delta_diff_count=0;
    int blemish_diff_count=0;
    for(int i=0;i!=delta_data_new.size();++i)
    {
        auto data=delta_data_new[i]-delta_data_old[i];
        if( fabs( data) >0.000001)
        {
            delta_diff_count++;
            if(delta_data_diff.size()<300)
            {
                delta_data_diff.push_back(data);
                delta_num_count.push_back(i);
            }
        }
    }
    for(int i=0;i!=blemish_data_new.size();++i)
    {
        auto data=blemish_data_new[i]-blemish_data_old[i];
        if( data != 0)
        {
            blemish_diff_count++;
            if(blemish_data_diff.size()<300)
                blemish_data_diff.push_back(data);
        }
    }

    int z=0;
    z=1;
    z=1;
    z=1;
    z=1;
    z=1;
    */


}



void AlgorithmFuncs:: Flicker_Pix_Binarization_Doublebyte (uchar *pRaw16_in, uchar *binar_out_raw8, int framecnt, int width, int height, double binary_spec)
{
    int w = width;
    int h = height;
    int std_spec = binary_spec;
    int frame_cnt=framecnt;
    vector<unsigned short> two_byte_array_(frame_cnt);

    for (int i=0, j=0; i<w*h*2; )
    {
            for(int cnt=0; cnt<frame_cnt; cnt++)
            {
                uchar *pRaw16 = pRaw16_in + cnt*w*h*2;
                two_byte_array_[cnt]=(unsigned short)pRaw16[i] | (((unsigned short)(pRaw16[i+1]&0x03))<<8);
            }

            double mean_t = 0;
            for (int i=0; i<frame_cnt; ++i)
                mean_t+=(double)two_byte_array_[i];
            mean_t/=frame_cnt;
            double std_t = 0.0;
            for (int i=0; i<frame_cnt; ++i)
            {
                std_t+=(double)((mean_t-two_byte_array_[i])*(mean_t-two_byte_array_[i]));
            }
            std_t/=frame_cnt;
            std_t =sqrt(std_t);

            if(std_t >= std_spec)
                binar_out_raw8[j] =255;
            else
                binar_out_raw8[j] =0;
            i+=2;
            j++;
    }
}

void AlgorithmFuncs:: Flicker_Pix_Binarization_Doublebyte_chao (uchar *pRaw16_in, uchar *binar_out_raw8, int framecnt,
                                                                int width,int height,int y_start,  double binary_spec)
{
    int w = width;
    int h = height;
    int all_size=w*(h/4);
    int start_loca=y_start*w;

    int std_spec = binary_spec;
    int frame_cnt=framecnt;
    vector<unsigned short> two_byte_array_(frame_cnt);

    for(int i=start_loca;i!=start_loca+all_size;++i)
    {
        int Raw16_loca=i*2;
        for(int cnt=0; cnt<frame_cnt; cnt++)
        {
            uchar *pRaw16 = pRaw16_in + cnt*w*h*2;
            two_byte_array_[cnt]=(unsigned short)pRaw16[Raw16_loca] | (((unsigned short)(pRaw16[Raw16_loca+1]&0x03))<<8);
        }

        double mean_t = 0;
        for (int size_i=0; size_i<frame_cnt; ++size_i)
            mean_t+=(double)two_byte_array_[size_i];
        mean_t/=frame_cnt;
        double std_t = 0.0;
        for (int size_i=0; size_i<frame_cnt; ++size_i)
        {
            std_t+=(double)((mean_t-two_byte_array_[size_i])*(mean_t-two_byte_array_[size_i]));
        }
        std_t/=frame_cnt;
        std_t =sqrt(std_t);

        if(std_t >= std_spec)
            binar_out_raw8[i] =255;
        else
            binar_out_raw8[i] =0;
    }
}


void AlgorithmFuncs:: Get_STD_MAP_Doublebyte (uchar *pRaw16_in, double *std_out, int framecnt, int width, int height)
{
    int w = width;
    int h = height;
    int frame_cnt=framecnt;
    vector<unsigned short> two_byte_array_(frame_cnt);
//    unsigned short two_byte_array_[frame_cnt];


    for (int i=0, j=0; i<w*h*2; )
    {
        for(int cnt=0; cnt<frame_cnt; cnt++)
        {
            uchar *pRaw16 = pRaw16_in + cnt*w*h*2;
            two_byte_array_[cnt]=(unsigned short)pRaw16[i] | (((unsigned short)(pRaw16[i+1]&0x03))<<8);
        }

        double mean_t = 0;
        for (int k=0; k<frame_cnt; ++k)
            mean_t+=(double)two_byte_array_[k];
        mean_t/=frame_cnt;
        double std_t = 0.0;
        for (int k=0; k<frame_cnt; ++k)
        {
            std_t+=(double)((mean_t-two_byte_array_[k])*(mean_t-two_byte_array_[k]));
        }
        std_t/=frame_cnt-1;
        std_t =sqrt(std_t);

        std_out[j] =std_t;

        i+=2;
        j++;
    }
}

void AlgorithmFuncs:: Get_STD_MAP_Doublebyte_chao (uchar *pRaw16_in, double *std_out, int framecnt, int width,
                                                   int height ,int y_start)
{
    int w = width;
    int h = height;
    int all_size=w*(h/4);
    int start_loca=y_start*w;
    int frame_cnt=framecnt;
    vector<unsigned short> two_byte_array_(frame_cnt);

    for(int i=start_loca;i!=start_loca+all_size;++i)
    {
        int Raw16_loca=i*2;
        for(int cnt=0; cnt<frame_cnt; cnt++)
        {
            uchar *pRaw16 = pRaw16_in + cnt*w*h*2;
            two_byte_array_[cnt]=(unsigned short)pRaw16[Raw16_loca] | (((unsigned short)(pRaw16[Raw16_loca+1]&0x03))<<8);
        }

        double mean_t = 0;
        for (int size_i=0; size_i<frame_cnt; ++size_i)
            mean_t+=(double)two_byte_array_[size_i];
        mean_t/=frame_cnt;
        double std_t = 0.0;
        for (int size_i=0; size_i<frame_cnt; ++size_i)
        {
            std_t+=(double)((mean_t-two_byte_array_[size_i])*(mean_t-two_byte_array_[size_i]));
        }
        std_t/=frame_cnt-1;
        std_t =sqrt(std_t);

        std_out[i] =std_t;
    }
}



bool AlgorithmFuncs::Get_channel_mean_from_image_doublebyte(uchar *pData, RGB_BAYER bayer, int width, int height,
                                            double &mean_r, double &mean_gr, double &mean_gb, double &mean_b)
{
    int w=width;
    int h=height;
    if(bayer <0 || w<0 || h<0 || pData==NULL)
        return false;

    uchar *buf_r = new uchar [w*h/4 * 2];
    uchar *buf_gr = new uchar [w*h/4 * 2];
    uchar *buf_gb = new uchar [w*h/4 * 2];
    uchar *buf_b = new uchar [w*h/4 * 2];

    uchar *pb1 = NULL;
    uchar *pb2 = NULL;
    uchar *pb3 = NULL;
    uchar *pb4 = NULL;
    if (bayer == RG)
    {
        pb1 = buf_r;
        pb2 = buf_gr;
        pb3 = buf_gb;
        pb4 = buf_b;
    }
    else if (bayer == BG)
    {
        pb1 = buf_b;
        pb2 = buf_gb;
        pb3 = buf_gr;
        pb4 = buf_r;
    }
    else if (bayer == GR)
    {
        pb1 = buf_gr;
        pb2 = buf_r;
        pb3 = buf_b;
        pb4 = buf_gb;
    }
    else if (bayer == GB)
    {
        pb1 = buf_gb;
        pb2 = buf_b;
        pb3 = buf_r;
        pb4 = buf_gr;
    }

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<2*w; j+=2)
        {
            int m=j/2;
            if (i%2 == 0 && m%2 == 0)
            {
                *pb1++ = pData[2*w*i+j];
                *pb1++ = pData[2*w*i+j+1];
            }
            else if(i%2 == 0 && m%2 == 1)
            {
                *pb2++ = pData[2*w*i+j];
                *pb2++ = pData[2*w*i+j+1];
            }
            else if(i%2 == 1 && m%2 == 0)
            {
                *pb3++ = pData[2*w*i+j];
                *pb3++ = pData[2*w*i+j+1];
            }
            else if(i%2 == 1 && m%2 == 1)
            {
                *pb4++ = pData[2*w*i+j];
                *pb4++ = pData[2*w*i+j+1];
            }
        }
    }

    mean_r =GetChannelMean_Doublebyte(buf_r,w/2,h/2);
    mean_gr =GetChannelMean_Doublebyte(buf_gr,w/2,h/2);
    mean_gb =GetChannelMean_Doublebyte(buf_gb,w/2,h/2);
    mean_b =GetChannelMean_Doublebyte(buf_b,w/2,h/2);

    delete [] buf_r;
    delete [] buf_gr;
    delete [] buf_gb;
    delete [] buf_b;

    return true;
}


int AlgorithmFuncs::Find_2X4Cluster_in_diff_image_WF(uchar *in1_raw8,uchar *in2_raw8,
                                                        int w,int h,
                                                        double spec,
                                                        std::string &g_str_msg_)
{
    int l,m,i, j, ii, jj;
    int cnt;
    int cnt2_4 = 0;
    uchar *out_tmp = new uchar[w*h];
    uchar *map_img = new uchar[w*h];
    if (w<=0 || h<=0 || in1_raw8==NULL || in2_raw8==NULL )
    {
        g_str_msg_.append ("image size or spec is wrong!\n");
        return -2;
    }
    for (int i=0; i<h; i++)
        for(int j=0; j<w; j++)
        {
            out_tmp[i*w+j]=std::abs(in1_raw8[i*w+j]-in2_raw8[i*w+j]);

            if (out_tmp[i*w+j]>spec)
                map_img[i*w+j]=255;
            else
                map_img[i*w+j]=0;
        }



    for (i = 0; i < h; ++i)
    {
        for (j = 0; j < w; ++j)
        {
            if(i < h && j < w)
            {
                if(map_img[i * w + j] == 255)
                {
                    l = i ; //2*4
                    m = j ; //2*4

                    // 2*4 type1
                    cnt = 0;
                    for(ii = l; ii < l + 4; ii++)
                    {
                        for(jj = m; jj < m + 2; jj++)
                        {
                            if(ii < h && jj < w)
                            {
                                if(map_img[ii * w + jj] == 255) cnt++;
                            }
                        }
                    }

                    if (cnt >= 6)
                    {
                        cnt2_4++;

                        g_str_msg_+=boost::str(boost::format("Find autoload with and without 2X4_cluster x:%d y:%d \n")
                                               % j % i );
                        i = i+4;
                        j = j+2;
                        continue;
                    }
                }
            }
        }
    }

    g_str_msg_+=boost::str(boost::format("Cnt2_4:%d\n")
                           % cnt2_4 );
    delete []out_tmp;
    delete []map_img;
    return cnt2_4;

}


void AlgorithmFuncs::get_roi_otp_channel_gain(double mean_r_roi, double mean_gr_roi, double mean_gb_roi, double mean_b_roi,
                                              double R_G_typical_roi, double B_G_typical_roi,
                                              double &R_channel_gain_final, double &Gr_channel_gain_final,
                                              double &Gb_channel_gain_final, double &B_channel_gain_final,
                                              int roi_no, std::string &msg)
{

    double R_G_typical = R_G_typical_roi;
    double B_G_typical = B_G_typical_roi;
    double R_  = mean_r_roi;//480;
    double Gr_ = mean_gr_roi;//710;
    double Gb_ = mean_gb_roi;//700;
    double B_  = mean_b_roi;//520;
    double R_channel_gain_1 =0;
    double Gr_channel_gain_1 =0;
    double Gb_channel_gain_1 =0;
    double B_channel_gain_1 =0;

    logMsg ("Mark11!",0);

    QString roi;

    switch (roi_no)
    {
    case 0:
        roi = "leftup";
        break;
    case 1:
        roi = "rightup";
        break;
    case 2:
        roi = "center";
        break;
    case 3:
        roi = "leftdown";
        break;
    case 4:
        roi = "rightdown";
        break;
    default:
        roi = "None";
        return;
    }

        logMsg ("Mark12!",0);
    if(Gr_>=Gb_)
    {
        Gb_channel_gain_1 = Gr_/Gb_;
        Gr_channel_gain_1 = 1;
    }
    else
    {
        Gr_channel_gain_1 = Gb_/Gr_;
        Gb_channel_gain_1 = 1;
    }

    double G_= std::max(Gr_,Gb_);

    double R_G_test = R_/G_;
    double B_G_test = B_/G_;

    R_channel_gain_1 = R_G_typical/R_G_test;
    B_channel_gain_1 = B_G_typical/B_G_test;

        logMsg ("Mark13!",0);
    double min_val = std::min(R_channel_gain_1,B_channel_gain_1);
    if(min_val<1)
    {
        if(R_channel_gain_1>=B_channel_gain_1)
        {
            double tmp = 1/B_channel_gain_1;
            B_channel_gain_final = 1;
            R_channel_gain_final = R_channel_gain_1*tmp;
            Gr_channel_gain_final = Gr_channel_gain_1*tmp;
            Gb_channel_gain_final = Gb_channel_gain_1*tmp;
        }
        else
        {
            double tmp = 1/R_channel_gain_1;
            R_channel_gain_final = 1;
            B_channel_gain_final = B_channel_gain_1*tmp;
            Gr_channel_gain_final = Gr_channel_gain_1*tmp;
            Gb_channel_gain_final = Gb_channel_gain_1*tmp;
        }
    }
    else
    {
        R_channel_gain_final = R_channel_gain_1;
        B_channel_gain_final = B_channel_gain_1;
        Gr_channel_gain_final = Gr_channel_gain_1;
        Gb_channel_gain_final = Gb_channel_gain_1;
    }



    logMsg ("Mark14!",0);

    msg+=boost::str(boost::format("ROI:%s otp trim channel result:\n")
                    % roi.toStdString().c_str() );

    msg+=boost::str(boost::format("R_G_typical=%.4f,B_G_typical=%.4f\n"
                                         "R_channel_gain_final=%.4f,Gr_channel_gain_final=%.4f\n"
                                         "Gb_channel_gain_final=%.4f,B_channel_gain_final=%.4f\n")
                           % R_G_typical
                           % B_G_typical
                           % R_channel_gain_final
                           % Gr_channel_gain_final
                           % Gb_channel_gain_final
                           % B_channel_gain_final
                           );

    logMsg ("Mark15!",0);

}


void AlgorithmFuncs::get_x_percent_corresponding_DN_number_raw16(short *data, int w, int h, double x, int &x_pos_dn, std::string &msg)
{

    int forward_pix[1024] = {0};
    int backward_pix[1024] = {0};

    int i=0;
    while(i<w*h)
    {
        if (data[i]>=0)
            forward_pix[data[i]]++;
        else
            backward_pix[data[i]*(-1)]++;
        i++;
    }

    int full_pix[2047]={0};
    for(int i=1; i<1024; i++)
        full_pix[1023-i]=backward_pix[i];
    for(int i=0; i<1024; i++)
        full_pix[1023+i]=forward_pix[i];

    double sum =0;
    double total =w*h;
    for(int i=0; i<2047; i++)
    {
        sum+=full_pix[i];
        double ratio=sum/total*100;

        if(ratio<= x)
            x_pos_dn=i-1023;

    }
    if(msg!="NONE")
        msg+=boost::str(boost::format("pos_%0.2f_corresponding_dn:%d\n")
                        %x % x_pos_dn );
}




void AlgorithmFuncs::get_x_percent_corresponding_DN_number_raw8(uchar *data, int w, int h, double x, int &x_pos_dn,
                                                                int roi_x, int roi_y, int roi_w, int roi_h,
                                                                int *histogram, std::string &msg)
{
    for(int i=roi_y; i<roi_y+roi_h; i++)
        for(int j=roi_x; j<roi_x+roi_w; j++)
        {
            histogram[data[i*w+j]]++;
        }

    double sum =0;
    double total =roi_w*roi_h;
    for(int i=0; i<256; i++)
    {
        sum+=histogram[i];
        double ratio=sum/total*100;
        if(ratio<= x)
            x_pos_dn=i;
        else
            break;
    }

    if(msg!="NONE")
        msg+=boost::str(boost::format("pos_%0.2f_corresponding_dn:%d\n")
                        %x % x_pos_dn );
}



void AlgorithmFuncs::get_x_percent_corresponding_DN_number_short(short *data, int w, int h, double x, int &x_pos_dn,
                                                                int roi_x, int roi_y, int roi_w, int roi_h,
                                                                int *histogram, std::string &msg)
{

    int forward_pix[1024] = {0};
    int backward_pix[1024] = {0};

    for(int i=roi_y; i<roi_y+roi_h; i++)
        for(int j=roi_x; j<roi_x+roi_w; j++)
        {
            if (data[i*w+j]>=0)
                forward_pix[data[i*w+j]]++;
            else
                backward_pix[data[i*w+j]*(-1)]++;
        }

    for(int i=1; i<1024; i++)
        histogram[1023-i]=backward_pix[i];
    for(int i=0; i<1024; i++)
        histogram[1023+i]=forward_pix[i];

    double sum =0;
    double total =roi_w*roi_h;
    for(int i=0; i<2047; i++)
    {
        sum+=histogram[i];
        double ratio=sum/total*100;

        if(ratio<= x)
            x_pos_dn=i-1023;
        else
            break;
    }
    if(msg!="NONE")
        msg+=boost::str(boost::format("pos_%0.2f_corresponding_dn:%d\n")
                        %x % x_pos_dn );
}



void RGBshading::getROI(const cv::Rect&r)
{
    roi.insert(roi.end(),r&cv::Rect(0,0,width,height));
}
void RGBshading::process(const uchar*data,double &LCU_R,double&LCU_B)const
{
    assert(roi.size()==4);
    const int R=0,Gr=1,Gb=2,B=3;
    double ratio[2][4];
    for(std::vector<cv::Rect>::size_type k=0;k<roi.size();++k)  //size_type相当于unsigned int
    {
        std::vector<cv::Rect>::const_reference r=roi.at(k);    //const_reference为指向常量的引用&
        double sum[4]={0.};
        int count[4]={0};
        for(int i=r.y;i<r.br().y;++i)
        {
            for(int j=r.x;j<r.br().x;++j)
            {
                const int &c=i%2*2+j%2;
                *(sum+c)+=*(data+i*width+j);
                ++(*(count+c));
            }
        }
        const double &R_mean=*(sum+R)/(*(count+R));
        const double &G_mean=(*(sum+Gr)+*(sum+Gb))/(*(count+Gr)+*(count+Gb));
        const double &B_mean=*(sum+B)/(*(count+B));
        *(*ratio+k)=R_mean/G_mean;
        *(*(ratio+1)+k)=B_mean/G_mean ;
    }
    double *const &MAX_RG=std::max_element(*ratio,*ratio+4);
    double *const &MIN_RG=std::min_element(*ratio,*ratio+4);
    double *const &MAX_BG=std::max_element(*(ratio+1),*(ratio+1)+4);
    double *const &MIN_BG=std::min_element(*(ratio+1),*(ratio+1)+4);
    LCU_R=4000000.*(*MAX_RG-*MIN_RG)/std::accumulate(*ratio,*ratio+4,0.);
    LCU_B=4000000.*(*MAX_BG-*MIN_BG)/std::accumulate(*(ratio+1),*(ratio+1)+4,0.);
    //迭代函数：accumulate(第一个迭代变量,最后一个迭代变量,迭代初始值,迭代的方式即迭代函数/默认是加法)
    // qDebug()<<LCU_R<<LCU_B;
}



#define cg(i,j) (in[i][j] - grad[i][j])

paramCrosstalk XTargs[THREAD_NUM];
unsigned int otpdpc_table[250];
int otpdpc_total_num;

#pragma comment(lib,"pthreadVC2.lib")


void AlgorithmFuncs::bbb1(unsigned short *in, int image_width, unsigned int *OTP_DD, int total_number,int col_length,int row_length)
{
    int number_offset = 0;
    int i;
    int valid_flag;
    int height, width;//row col
    int type;
    unsigned int *DD_info;
    int position_true;
    //int position_shift,position_shift_1;
    int four_cell_width, four_cell_height;
    int four_cell_height_side1 = 0;
    int four_cell_height_side2 = 0;
    int DD_pixel_0, DD_pixel_1;
    int DD_pixel_01, DD_pixel_11;
    int DD_pixel_02, DD_pixel_12;
    int DD_pixel_03, DD_pixel_13;
    int replace_offset[4] = { -3 * image_width,3 * image_width,-image_width,image_width };//up 3 ,down 3,up 1,down 1;
                                                                                          //int replace_offset[4]={2,2,2,2};
    int up_limit, down_limit;


    for (i = 0;i<total_number;i++)
    {
        DD_info = OTP_DD + (i);//type[1:0] row[11:0] col[12:0]
        valid_flag = ((*DD_info) >> 31) & 0x01;
        if (true) {
            height = (((*DD_info) >> col_length) & ((2<<row_length)-1)) - 1;
            width = ((*DD_info) & ((2<<col_length)-1)) - 1;
            //height=MAX(MIN(height,3000),5);
            //width=MAX(MIN(width,4000),5);
            position_true = (width + height * image_width);
            four_cell_height = height & 1;
            four_cell_width = width & 1;
            four_cell_height_side1 = height < 4;
            four_cell_height_side2 = height > 3115;
            up_limit = four_cell_height_side1;
            down_limit = 1 - four_cell_height_side2;
            type = ((*DD_info) >> (col_length+row_length)) & 63;
            DD_pixel_0 = position_true;
            DD_pixel_01 = DD_pixel_0 + 1;
            DD_pixel_02 = DD_pixel_0 + 2;
            DD_pixel_03 = DD_pixel_0 + 3;
            DD_pixel_1 = DD_pixel_0 + replace_offset[3];
            DD_pixel_11 = DD_pixel_1 + 1;
            DD_pixel_12 = DD_pixel_1 + 2;
            DD_pixel_13 = DD_pixel_1 + 3;
            switch ((int)(type))
            {
            case 11:
            case 3:
                in[DD_pixel_0] = in[(DD_pixel_0 + replace_offset[up_limit])];//
                in[DD_pixel_01] = in[(DD_pixel_01 + replace_offset[up_limit])];//+ replace_offset[0]
                in[DD_pixel_1] = in[(DD_pixel_1 + replace_offset[down_limit])];//+ replace_offset[1]
                in[DD_pixel_11] = in[(DD_pixel_11 + replace_offset[down_limit])];//+ replace_offset[1]
                break;
            case 2:
                in[DD_pixel_0] = in[(DD_pixel_0 + replace_offset[3 - four_cell_height])];
                in[DD_pixel_01] = in[(DD_pixel_01 + replace_offset[3 - four_cell_height])];
                break;
            case 1:
                in[DD_pixel_0] = in[(DD_pixel_0 + replace_offset[3 - four_cell_height])];
                in[DD_pixel_02] = in[(DD_pixel_02 + replace_offset[3 - four_cell_height])];
                break;
            case 8:
            case 0:
                in[DD_pixel_0] = in[(DD_pixel_0 + replace_offset[3 - four_cell_height])];
                break;
            case 9:
                in[DD_pixel_0] = in[(DD_pixel_0 + replace_offset[3 - four_cell_height])];
                in[DD_pixel_03] = in[(DD_pixel_03 + replace_offset[3 - four_cell_height])];
                break;
            case 24:
                in[DD_pixel_0] = in[(DD_pixel_0 + replace_offset[up_limit])];
                in[DD_pixel_1] = in[(DD_pixel_1 + replace_offset[down_limit])];

                break;
            case 40:
                in[DD_pixel_0] = in[(DD_pixel_0 + replace_offset[3 - four_cell_height])];
                in[DD_pixel_11] = in[(DD_pixel_11 + replace_offset[2 + four_cell_height])];
                break;
            case 56:
                in[DD_pixel_0] = in[(DD_pixel_0 + replace_offset[3 - four_cell_height])];
                in[DD_pixel_1 - 1] = in[(DD_pixel_1 - 1 + replace_offset[2 + four_cell_height])];
                break;
            default:
                break;
            }
        }
    }
    return;
}

void AlgorithmFuncs::bbb2(unsigned int *OTP_DD, int image_height, int image_width, int total_number, int mirror_flag, int updown_flag, int crop_height, int crop_width,int col_length,int row_length)
{
    int i, height, width;
    unsigned int OTP;
    int type;
    for (i = 0;i<total_number;i++)
    {
        OTP = OTP_DD[i]; //type[1:0] row[11:0] col[12:0]
        height = ((OTP >> col_length) & ((2<<row_length)-1)) - crop_height;
        width = (OTP & ((2<<col_length)-1)) - crop_width;
        type = (OTP >> (col_length+row_length)) & 0x03f;
        if (mirror_flag)
        {
            width = image_width  - width + 1;
        }
        if (updown_flag)
        {
            height = image_height - height + 1;
        }

        switch ((int)(type))
        {
        case 11:
        case 3:
            width = width - mirror_flag;
            height = height - updown_flag;
            break;
        case 2:
            width = width - mirror_flag;
            break;
        case 1:
            width = width - 2 * mirror_flag;
            break;
        case 9:
            width = width - 3 * mirror_flag;
            break;
        case 24:
            height = height - updown_flag;
            break;
        case 40:
            width = width - mirror_flag;
            height = height - updown_flag;
            break;
        case 56:
            width = width + mirror_flag;
            height = height - updown_flag;
            break;
        default:
            break;
        }
        OTP = OTP & 0x80000000;
        OTP = OTP + (type << (col_length+row_length)) + (height << col_length) + width;
        OTP_DD[i] = OTP;
    }
}

int AlgorithmFuncs::bbb7(int *first_max, int *first_min, int *second_max, int *second_min, int *mat4_flag, int op[12])
{
    //int mat4[4] = { op[0],op[1],op[2],op[3] };

    int f1, f2, f3, f4, f5, f6;
    int tmp;
    f1 = op[0]>op[1];
    f2 = op[0]>op[2];
    f3 = op[0]>op[3];
    f4 = op[1]>op[2];
    f5 = op[1]>op[3];
    f6 = op[2]>op[3];
    tmp = f1 + (f2<<1) + (f3<<2) + (f4<<3) + (f5<<4) + (f6<<5);
    switch (tmp)
    {
    case 0:
        *first_max = op[3];
        *second_max = op[2];
        *second_min = op[1];
        *first_min = op[0];
        //mat4_flag[0] = 0;
        //mat4_flag[1] = 1;
        //mat4_flag[2] = 2;
        //mat4_flag[3] = 3;
        break;
    case 1:
        *first_max = op[3];
        *second_max = op[2];
        *second_min = op[0];
        *first_min = op[1];
        mat4_flag[0] = 1;
        mat4_flag[1] = 0;
        //mat4_flag[2] = 2;
        //mat4_flag[3] = 3;
        break;
    case 3:
        *first_max = op[3];
        *second_max = op[0];
        *second_min = op[2];
        *first_min = op[1];
        mat4_flag[0] = 1;
        mat4_flag[1] = 2;
        mat4_flag[2] = 0;
        //mat4_flag[3] = 3;
        break;
    case 7:
        *first_max = op[0];
        *second_max = op[3];
        *second_min = op[2];
        *first_min = op[1];
        mat4_flag[0] = 1;
        mat4_flag[1] = 2;
        mat4_flag[2] = 3;
        mat4_flag[3] = 0;
        break;
    case 8:
        *first_max = op[3];
        *second_max = op[1];
        *second_min = op[2];
        *first_min = op[0];
        //mat4_flag[0] = 0;
        mat4_flag[1] = 2;
        mat4_flag[2] = 1;
        //mat4_flag[3] = 3;
        break;
    case 9:
        break;
    case 10:
        *first_max = op[3];
        *second_max = op[1];
        *second_min = op[0];
        *first_min = op[2];
        mat4_flag[0] = 2;
        mat4_flag[1] = 0;
        mat4_flag[2] = 1;
        //mat4_flag[3] = 3;
        break;
    case 11:
        *first_max = op[3];
        *second_max = op[0];
        *second_min = op[1];
        *first_min = op[2];
        mat4_flag[0] = 2;
        //mat4_flag[1] = 1;
        mat4_flag[2] = 0;
        //mat4_flag[3] = 3;
        break;
    case 15:
        *first_max = op[0];
        *second_max = op[3];
        *second_min = op[1];
        *first_min = op[2];
        mat4_flag[0] = 2;
        //mat4_flag[1] = 1;
        mat4_flag[2] = 3;
        mat4_flag[3] = 0;
        break;
    case 24:
        *first_max = op[1];
        *second_max = op[3];
        *second_min = op[2];
        *first_min = op[0];
        //mat4_flag[0] = 0;
        mat4_flag[1] = 2;
        mat4_flag[2] = 3;
        mat4_flag[3] = 1;
        break;
    case 26:
        *first_max = op[1];
        *second_max = op[3];
        *second_min = op[0];
        *first_min = op[2];
        mat4_flag[0] = 2;
        mat4_flag[1] = 0;
        mat4_flag[2] = 3;
        mat4_flag[3] = 1;
        break;
    case 30:
        *first_max = op[1];
        *second_max = op[0];
        *second_min = op[3];
        *first_min = op[2];
        mat4_flag[0] = 2;
        mat4_flag[1] = 3;
        mat4_flag[2] = 0;
        mat4_flag[3] = 1;
        break;
    case 31:
        *first_max = op[0];
        *second_max = op[1];
        *second_min = op[3];
        *first_min = op[2];
        mat4_flag[0] = 2;
        mat4_flag[1] = 3;
        mat4_flag[2] = 1;
        mat4_flag[3] = 0;
        break;
    case 32:
        *first_max = op[2];
        *second_max = op[3];
        *second_min = op[1];
        *first_min = op[0];
        //mat4_flag[0] = 0;
        //mat4_flag[1] = 1;
        mat4_flag[2] = 3;
        mat4_flag[3] = 2;
        break;
    case 33:
        *first_max = op[2];
        *second_max = op[3];
        *second_min = op[0];
        *first_min = op[1];
        mat4_flag[0] = 1;
        mat4_flag[1] = 0;
        mat4_flag[2] = 3;
        mat4_flag[3] = 2;
        break;
    case 37:
        *first_max = op[2];
        *second_max = op[0];
        *second_min = op[3];
        *first_min = op[1];
        mat4_flag[0] = 1;
        mat4_flag[1] = 3;
        mat4_flag[2] = 0;
        mat4_flag[3] = 2;
        break;
    case 39:
        *first_max = op[0];
        *second_max = op[2];
        *second_min = op[3];
        *first_min = op[1];
        mat4_flag[0] = 1;
        mat4_flag[1] = 3;
        //mat4_flag[2] = 2;
        mat4_flag[3] = 0;
        break;
    case 48:
        *first_max = op[2];
        *second_max = op[1];
        *second_min = op[3];
        *first_min = op[0];
        //mat4_flag[0] = 0;
        mat4_flag[1] = 3;
        mat4_flag[2] = 1;
        mat4_flag[3] = 2;
        break;
    case 52:
        *first_max = op[2];
        *second_max = op[1];
        *second_min = op[0];
        *first_min = op[3];
        mat4_flag[0] = 3;
        mat4_flag[1] = 0;
        mat4_flag[2] = 1;
        mat4_flag[3] = 2;
        break;
    case 53:
        *first_max = op[2];
        *second_max = op[0];
        *second_min = op[1];
        *first_min = op[3];
        mat4_flag[0] = 3;
        //mat4_flag[1] = 1;
        mat4_flag[2] = 0;
        mat4_flag[3] = 2;
        break;
    case 55:
        *first_max = op[0];
        *second_max = op[2];
        *second_min = op[1];
        *first_min = op[3];
        mat4_flag[0] = 3;
        //mat4_flag[1] = 1;
        //mat4_flag[2] = 2;
        mat4_flag[3] = 0;
        break;
    case 56:
        *first_max = op[1];
        *second_max = op[2];
        *second_min = op[3];
        *first_min = op[0];
        //mat4_flag[0] = 0;
        mat4_flag[1] = 3;
        //mat4_flag[2] = 2;
        mat4_flag[3] = 1;
        break;
    case 60:
        *first_max = op[1];
        *second_max = op[2];
        *second_min = op[0];
        *first_min = op[3];
        mat4_flag[0] = 3;
        mat4_flag[1] = 0;
        //mat4_flag[2] = 2;
        mat4_flag[3] = 1;
        break;
    case 62:
        *first_max = op[1];
        *second_max = op[0];
        *second_min = op[2];
        *first_min = op[3];
        mat4_flag[0] = 3;
        mat4_flag[1] = 2;
        mat4_flag[2] = 0;
        mat4_flag[3] = 1;
        break;
    case 63:
        *first_max = op[0];
        *second_max = op[1];
        *second_min = op[2];
        *first_min = op[3];
        mat4_flag[0] = 3;
        mat4_flag[1] = 2;
        mat4_flag[2] = 1;
        mat4_flag[3] = 0;
        break;
    default:
        break;
    }
    return 0;
}

void AlgorithmFuncs::bbb8(int *first_max, int op[12])
{
    int i;
    *first_max = op[4];
    for (i = 5;i<12;i++)
    {
        if (op[i]>*first_max)
        {
            *first_max = op[i];
        }
    }
}

void AlgorithmFuncs::bbb9(int *first_min, int op[12])
{
    int i;
    *first_min = op[4];
    for (i = 5;i<12;i++)
    {
        if (op[i]<*first_min)
        {
            *first_min = op[i];
        }
    }
}

void AlgorithmFuncs::bbb10(short**raw, int cfa_in, int image_height, int image_width)//
{
    int cfa_G_first = 0;
    int DPC_4C_mode = 3;
    int dd_th_bright = 4;
    int dd_th_bright_double = 3;
    int dd_th_dark = 10; //20//80
    int dd_th_dark_double = 20; //80
    int coef_sg = 32;
    int coef_do = 32;
    int output_mode = 0;

    int op[12];
    int replace[4], replace_reverse[4];
    int mat4_flag[4];
    int first_max, second_max, first_min, second_min;//max,max_sec,min,min_sec;
    int first_max_around, first_min_around;//max,max_sec,min,min_sec;
    int Y_comparer;

    int strong_dir;
    int center_is_G;

    int is_bright_dark;
    int dd_th_sg, dd_th_do;
    int judge_defect_sg, judge_defect_do;
    int is_single_light, is_single_dark, is_double_light, is_double_dark;
    int is_very_bright;

    //		int a1,b1,a2,b2,a3,b3,a4,b4;

    int max_min_limit;
    int DPC_4C_mode_sg;

    int i, j;
    int jd1, jd2, jd3, jd4, ja1, ja2, ja3, ja4, ja5;
    int id1, id2, id3, id4, ia1, ia2, ia3, ia4, ia5;
    int i_pixel, j_pixel;
    int replace_mode;

    //		for (k=0; k<16; k++)
    //		window_G_C[k] = gBuf[gBuffLineIdx[k]];

    if (cfa_in == 1 || cfa_in == 2)
    {
        cfa_G_first = 2;
    }
    for (i_pixel = 8;i_pixel<image_height - 8;i_pixel += 2)//
    {
        i = i_pixel;
        ia1 = i + 1; ia2 = i + 2; ia3 = i + 3;  ia4 = i + 4; ia5 = i + 5;// ia6 = i+6;
        id1 = i - 1; id2 = i - 2; id3 = i - 3;  id4 = i - 4;// id5 = i-5; id6 = i-6;id7 = i-7;
        center_is_G = ((i_pixel >> 1) + 1) & 1;
        for (j_pixel = 8 - cfa_G_first;j_pixel<image_width - 8;j_pixel += 2)//(+0)
        {
            j = j_pixel;
            ja1 = j + 1; ja2 = j + 2; ja3 = j + 3;  ja4 = j + 4; ja5 = j + 5;// ja6 = j+6;
            jd1 = j - 1; jd2 = j - 2; jd3 = j - 3;  jd4 = j - 4;// jd5 = j-5; jd6 = j-6;jd7 = j-7;
            /*排序*/ center_is_G = 1 - center_is_G;
            if (center_is_G)
            {
                op[0] = raw[i][j];
                op[1] = raw[i][ja1];
                op[2] = raw[ia1][ja1];
                op[3] = raw[ia1][j];
                op[4] = raw[id1][jd1];
                op[5] = raw[id1][ja2];
                op[6] = raw[ia2][ja2];
                op[7] = raw[ia2][jd1];
                op[8] = raw[ia2][jd2];
                op[9] = raw[id1][jd2];
                op[10] = raw[id1][ja3];
                op[11] = raw[ia2][ja3];
            }
            else
            {
                op[0] = raw[i][j];
                op[1] = raw[i][ja1];
                op[2] = raw[ia1][ja1];
                op[3] = raw[ia1][j];
                op[4] = raw[id3][j];
                op[5] = raw[id3][ja1];
                op[6] = raw[i][ja4];
                op[7] = raw[ia1][ja4];
                op[8] = raw[ia4][ja1];
                op[9] = raw[ia4][j];
                op[10] = raw[ia1][jd3];
                op[11] = raw[i][jd3];
            }
            Y_comparer = (op[0] + op[1] + op[2] + op[3]) >> 2;
            mat4_flag[0] = 0;mat4_flag[1] = 1;mat4_flag[2] = 2;mat4_flag[3] = 3;
            AlgorithmFuncs::bbb7(&first_max, &first_min, &second_max, &second_min, mat4_flag, op);
            /*亮度计算控制基础阈值*/
            is_bright_dark = Y_comparer & 768;
            is_very_bright = 0;
            max_min_limit = (is_bright_dark + 1) << 5;
            dd_th_sg = dd_th_dark + (Y_comparer >> dd_th_bright);
            dd_th_do = dd_th_sg;//dd_th_dark_double + (Y_comparer>>dd_th_bright_double);

                                /*方向判断*/
            strong_dir = 0;
            /*单坏点与双坏点阈值计算*/
            judge_defect_do = dd_th_sg;//(coef_do) *(max_min_limit + judge_defect_multi3*dd_th_do)>>5;
            judge_defect_sg = dd_th_do;//(coef_sg) *(max_min_limit + judge_defect_multi3*dd_th_sg)>>5;
                                       /*单坏点与双坏点判断*/
            is_single_light = (first_max - second_max)>judge_defect_sg;
            is_single_dark = (second_min - first_min)>judge_defect_sg;
            if (is_single_light)
            {
                AlgorithmFuncs::bbb8(&first_max_around, op);
                is_single_light = (first_max - first_max_around)>judge_defect_sg;
            }
            if (is_single_dark)
            {
                AlgorithmFuncs::bbb9(&first_min_around, op);
                is_single_dark = (first_min_around - first_min)>judge_defect_sg;
            }
            is_double_light = 0;//double_defect && lighter_than_4cell;
            is_double_dark = 0;//double_defect && !lighter_than_4cell;
                               /*单坏点替换值 blending值计算*/
                               /*双坏点替换值 blending值计算*/
            DPC_4C_mode_sg = DPC_4C_mode & 1;
            replace_mode = DPC_4C_mode_sg * is_single_light + (DPC_4C_mode_sg*is_single_dark * 4);// + is_XT_flag*16;//+2*DPC_4C_mode_do*is_double_light+8*DPC_4C_mode_do*is_double_dark;

            switch ((int)(replace_mode))
            {
            case 0:
                continue;
                break;
            case 1:
                replace[3] = second_max;
                replace[2] = second_max;
                replace[1] = second_min;
                replace[0] = first_min;
                break;
            case 4:
                replace[3] = first_max;
                replace[2] = second_max;
                replace[1] = second_min;
                replace[0] = second_min;
                break;
            case 5:
                replace[3] = second_max;
                replace[2] = second_max;
                replace[1] = second_min;
                replace[0] = second_min;
                break;
            default:
                replace[3] = first_max;
                replace[2] = second_max;
                replace[1] = second_min;
                replace[0] = first_min;
                break;
            }
            /*替换*/
            replace_reverse[mat4_flag[0]] = replace[0];
            replace_reverse[mat4_flag[1]] = replace[1];
            replace_reverse[mat4_flag[2]] = replace[2];
            replace_reverse[mat4_flag[3]] = replace[3];
            //replace_reverse[0] = MIN(1023, replace_reverse[0]);
            //replace_reverse[1] = MIN(1023, replace_reverse[1]);
            //replace_reverse[2] = MIN(1023, replace_reverse[2]);
            //replace_reverse[3] = MIN(1023, replace_reverse[3]);
            raw[i][j] = replace_reverse[0];
            raw[i][ja1] = replace_reverse[1];
            raw[ia1][ja1] = replace_reverse[2];
            raw[ia1][j] = replace_reverse[3];

        }
    }
    return;
}

void AlgorithmFuncs::bbb11(unsigned short *in, short **out, int col_start, int block_w, int width, int height, short black_level, int padding_flag)
{
    int i, j, t, ii, k1, k2;
    int iii, jjj;
    t = width;

    if (padding_flag == 0) { // leftmost block
        for (i = 0; i<height; i++) {
            ii = i*t+ col_start;
            iii = i + 16;
            for (j = 0; j<block_w - 16; j ++) {
                jjj = j + 16;
                //out[iii][jjj] = (short)(in[k1] + ((in[k2] & 0x03) << 8))- black_level;
                out[iii][jjj] = ((short)in[ii + j]) - black_level;
            }
        }
        // top padding size=16
        k2 = block_w * 2 - 35;
        for (i = 0; i < 16; i++) {
            k1 = 33 - i;
            for (j = 0; j < 16; j++) {
                out[i][j] = out[k1][33 - j];
            }
            for (j = 16; j < block_w; j++) {
                out[i][j] = out[k1][j];
            }
        }
        // bottom padding size=16
        for (i = height + 16; i < height + 32; i++) {
            k1 = height * 2 - i + 29;
            for (j = 0; j < 16; j++) {
                out[i][j] = out[k1][33 - j];
            }
            for (j = 16; j < block_w; j++) {
                out[i][j] = out[k1][j];
            }
        }
        // left padding size=16
        for (i = 16; i < height + 16; i++) {
            for (j = 0; j < 16; j++) {
                out[i][j] = out[i][33 - j];
            }
        }
    }
    else if (padding_flag==THREAD_NUM-1){ // rightmost block
        for (i = 0; i<height; i++) {
            ii = i*t+ col_start-16;
            iii = i + 16;
            for (j = 0; j<block_w - 16; j ++) {
                jjj = j;
                out[iii][jjj] = ((short)in[ii+j])  - black_level;
            }
        }
        // top padding size=16
        k2 = block_w * 2 - 35;
        for (i = 0; i < 16; i++) {
            k1 = 33 - i;
            for (j = 0; j < block_w - 16; j++) {
                out[i][j] = out[k1][j];
            }
            for (j = block_w - 16; j < block_w; j++) {
                out[i][j] = out[k1][k2 - j];
            }
        }
        // bottom padding size=16
        for (i = height + 16; i < height + 32; i++) {
            k1 = height * 2 - i + 29;
            for (j = 0; j < block_w - 16; j++) {
                out[i][j] = out[k1][j];
            }
            for (j = block_w - 16; j < block_w; j++) {
                out[i][j] = out[k1][k2 - j];
            }
        }
        // right padding size=16
        for (i = 16; i < height + 16; i++) {
            for (j = block_w - 16; j < block_w; j++) {
                out[i][j] = out[i][k2 - j];
            }
        }
    }
    else { // middle blocks
        for (i = 0; i<height; i++) {
            ii = i*t+ col_start - 16;
            iii = i + 16;
            for (j = 0; j<block_w; j ++) {
                jjj = j;
                out[iii][jjj] = ((short)in[ii+j])- black_level;
            }
        }
        // top padding size=16
        k2 = block_w * 2 - 35;
        for (i = 0; i < 16; i++) {
            k1 = 33 - i;
            for (j = 0; j < block_w; j++) {
                out[i][j] = out[k1][j];
            }
        }
        // bottom padding size=16
        for (i = height + 16; i < height + 32; i++) {
            k1 = height * 2 - i + 29;
            for (j = 0; j < block_w; j++) {
                out[i][j] = out[k1][j];
            }
        }
    }
}

void AlgorithmFuncs::bbb12(short **in, unsigned short *out, int block_w,int block_h,int col_start, int width, int height, short black_level)
{
    int i, j, t, ii, k1, i16;
    t = width;
    for (i = 0;i < block_h -32;i++) {
        ii = i*t+col_start;
        i16 = i + 16;
        for (j = 0;j < block_w - 32;j++) {
            k1 = in[i16][j+16]+ black_level;
            out[ii + j] = (unsigned short)(k1 < 0 ? 0 : k1>1023 ? 1023 : k1);
        }
    }
}

void AlgorithmFuncs::bbb13(short **in, short ** grad, int offset_x, int offset_y, int width, int height, int flat_th, int cdiff_grad_level)
{
    int i,j,k;
    int id1,id2,ia1,ia2,ia3,ia4,ia5;
    int jd1,jd2,ja1,ja2,ja3,ja4,ja5;
    short grad1_max,grad1_min, grad2_max, grad2_min;
    short dir1_min, dir2_min;
    short gradh1,gradv1,grads1,grads2,gradt1,gradt2;
    short gradh2, gradv2, grads4, gradt3, gradt4;
    short grad1[4],grad2[4];
    short gradh11, gradh12, gradv11, gradv12, grads11, grads12, gradt11, gradt12;
    short gradh21, gradh22, gradv21, gradv22, grads21, grads22, gradt21, gradt22;
    short g_mean_hor_1, g_mean_ver_1, g_mean_hor_2, g_mean_ver_2;
    //short **out_tmp = (short **)malloc(sizeof(short*)*height);
    //for (i = 0;i < height;i++) {
    //	out_tmp[i] = (short *)malloc(sizeof(short)*width);
    //	memset(out_tmp[i], 0, sizeof(short)*width);
    //}
    short flat_th_1,flat_th_2;
    for(i=offset_y;i<height-5;i+=4){
        id1=i-1;id2=i-2;
        ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;ia4 = i + 4;ia5 = i + 5;
        for (j = offset_x;j < width - 5;j += 4) {
            jd1 = j - 1;jd2 = j - 2;
            ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;ja4 = j + 4;ja5 = j + 5;
            gradh1 = (abs(in[i][jd1] - in[i][ja2]) + abs(in[ia1][jd1] - in[ia1][ja2])) >> 1;
            gradv1 = (abs(in[id1][j] - in[ia2][j]) + abs(in[id1][ja1] - in[ia2][ja1])) >> 1;
            grads1 = abs(in[i][jd1] - in[id1][j]);
            grads2 = abs(in[ia2][ja1] - in[ia1][ja2]);
            gradt1 = abs(in[id1][ja1] - in[i][ja2]);
            gradt2 = abs(in[ia1][jd1] - in[ia2][j]);

            gradh11 = (abs(in[i][jd1] - in[i][j]) + abs(in[ia1][jd1] - in[ia1][j])) >> cdiff_grad_level;
            gradh12 = (abs(in[i][ja2] - in[i][ja1]) + abs(in[ia1][ja2] - in[ia1][ja1])) >> cdiff_grad_level;
            gradv11 = (abs(in[id1][j] - in[i][j]) + abs(in[id1][ja1] - in[i][ja1])) >> cdiff_grad_level;
            gradv12 = (abs(in[ia2][j] - in[ia1][j]) + abs(in[ia2][ja1] - in[ia1][ja1])) >> cdiff_grad_level;
            grads11 = (abs(in[ia1][jd1] - in[i][j]) + abs(in[ia2][j] - in[ia1][ja1])) >> cdiff_grad_level;
            grads12 = (abs(in[i][j] - in[id1][ja1]) + abs(in[ia1][ja1] - in[i][ja2])) >> cdiff_grad_level;
            gradt11 = (abs(in[i][jd1] - in[ia1][j]) + abs(in[id1][j] - in[i][ja1])) >> cdiff_grad_level;
            gradt12 = (abs(in[ia2][ja1] - in[ia1][j]) + abs(in[ia1][ja2] - in[i][ja1])) >> cdiff_grad_level;
            /*g_mean_1 = (in[i][jd1] + in[i][ja2] + in[ia1][jd1] + in[ia1][ja2] + in[id1][j]  + in[ia2][j] + in[id1][ja1]  + in[ia2][ja1])>>4;
            flat_th_1 = g_mean_1+flat_th;*/
            g_mean_hor_1 = (in[i][jd1] + in[i][ja2] + in[ia1][jd1] + in[ia1][ja2]) >> 4;
            g_mean_ver_1 = (in[id1][j] + in[ia2][j] + in[id1][ja1] + in[ia2][ja1]) >> 4;
            flat_th_1 = std::min(g_mean_hor_1, g_mean_ver_1) + flat_th;


            gradh2 = (abs(in[ia2][ja1] - in[ia2][ja4]) + abs(in[ia3][ja1] - in[ia3][ja4])) >> 1;
            gradv2 = (abs(in[ia1][ja2] - in[ia4][ja2]) + abs(in[ia1][ja3] - in[ia4][ja3])) >> 1;
            //grads3 = abs(in[ia2][ja1] - in[ia1][ja2]);
            grads4 = abs(in[ia4][ja3] - in[ia3][ja4]);
            gradt3 = abs(in[ia1][ja3] - in[ia2][ja4]);
            gradt4 = abs(in[ia3][ja1] - in[ia4][ja2]);

            gradh21 = (abs(in[ia2][ja1] - in[ia2][ja2]) + abs(in[ia3][ja1] - in[ia3][ja2])) >> cdiff_grad_level;
            gradh22 = (abs(in[ia2][ja4] - in[ia2][ja3]) + abs(in[ia3][ja4] - in[ia3][ja3])) >> cdiff_grad_level;
            gradv21 = (abs(in[ia1][ja2] - in[ia2][ja2]) + abs(in[ia1][ja3] - in[ia2][ja3])) >> cdiff_grad_level;
            gradv22 = (abs(in[ia4][ja2] - in[ia3][ja2]) + abs(in[ia4][ja3] - in[ia3][ja3])) >> cdiff_grad_level;
            grads21 = (abs(in[ia3][ja1] - in[ia2][ja2]) + abs(in[ia4][ja2] - in[ia3][ja3])) >> cdiff_grad_level;
            grads22 = (abs(in[ia2][ja2] - in[ia1][ja3]) + abs(in[ia3][ja3] - in[ia2][ja4])) >> cdiff_grad_level;
            gradt21 = (abs(in[ia2][ja1] - in[ia3][ja2]) + abs(in[ia1][ja2] - in[ia2][ja3])) >> cdiff_grad_level;
            gradt22 = (abs(in[ia4][ja3] - in[ia3][ja2]) + abs(in[ia3][ja4] - in[ia2][ja3])) >> cdiff_grad_level;
            /*g_mean_2 = (in[ia2][ja1] + in[ia2][ja4] + in[ia3][ja1] + in[ia3][ja4] + in[ia1][ja2] + in[ia4][ja2] + in[ia1][ja3] + in[ia4][ja3]) >> 4;
            flat_th_2 = g_mean_2+flat_th;*/
            g_mean_hor_2 = (in[ia2][ja1] + in[ia2][ja4] + in[ia3][ja1] + in[ia3][ja4]) >> 4;
            g_mean_ver_2 = (in[ia1][ja2] + in[ia4][ja2] + in[ia1][ja3] + in[ia4][ja3]) >> 4;
            flat_th_2 = min(g_mean_hor_2, g_mean_ver_2) + flat_th;


            /*grad1[0] = (grad[id2][j] + grad[i][jd2] + grad[i][j] + grad[i][ja2] + grad[ia2][j] + gradh1+ gradh11+ gradh12);
            grad1[1] = (grad[id2][ja1] + grad[i][jd1] + grad[i][ja1] + grad[i][ja3] + grad[ia2][ja1] + gradv1 + gradv11 + gradv12);
            grad1[2] = (grad[id1][j] + grad[ia1][jd2] + grad[ia1][j] * 2 + grad[ia1][ja2] + grad[ia3][j] + grads1 + grads2+ grads11+ grads12);
            grad1[3] = (grad[id1][ja1] + grad[ia1][jd1] + grad[ia1][ja1] * 2 + grad[ia1][ja3] + grad[ia3][ja1] + gradt1 + gradt2+ gradt11+ gradt12);

            grad2[0] = (grad[i][ja2] + grad[ia2][j] + grad[ia2][ja2] + grad[ia2][ja4] + grad[ia4][ja2] + gradh2 + gradh21 + gradh22);
            grad2[1] = (grad[i][ja3] + grad[ia2][ja1] + grad[ia2][ja3] + grad[ia2][ja5] + grad[ia4][ja3] + gradv2 + gradv21 + gradv22);
            grad2[2] = (grad[ia1][ja2] + grad[ia3][j] + grad[ia3][ja2] * 2 + grad[ia3][ja4] + grad[ia5][ja2] + grads2 + grads4 + grads21 + grads22);
            grad2[3] = (grad[ia1][ja3] + grad[ia3][ja1] + grad[ia3][ja3] * 2 + grad[ia3][ja5] + grad[ia5][ja3] + gradt3 + gradt4 + gradt21 + gradt22);*/

            /*grad1[0] = (grad[id2][j]*2 + grad[i][jd2] + grad[i][j] * 2 + grad[i][ja2] + grad[ia2][j]*2);
            grad1[1] = (grad[id2][ja1]*2 + grad[i][jd1] + grad[i][ja1] * 2 + grad[i][ja3] + grad[ia2][ja1]*2);
            grad1[2] = (grad[id1][j] + grad[ia1][jd2] + grad[ia1][j] * 2 + grad[ia1][ja2] + grad[ia3][j] + grads1 + grads2);
            grad1[3] = (grad[id1][ja1] + grad[ia1][jd1] + grad[ia1][ja1] * 2 + grad[ia1][ja3] + grad[ia3][ja1] + gradt1 + gradt2);

            grad2[0] = (grad[i][ja2]*2 + grad[ia2][j] + grad[ia2][ja2] * 2 + grad[ia2][ja4] + grad[ia4][ja2]*2);
            grad2[1] = (grad[i][ja3]*2 + grad[ia2][ja1] + grad[ia2][ja3] * 2 + grad[ia2][ja5] + grad[ia4][ja3]*2);
            grad2[2] = (grad[ia1][ja2] + grad[ia3][j] + grad[ia3][ja2] * 2 + grad[ia3][ja4] + grad[ia5][ja2] + grads2 + grads4);
            grad2[3] = (grad[ia1][ja3] + grad[ia3][ja1] + grad[ia3][ja3] * 2 + grad[ia3][ja5] + grad[ia5][ja3] + gradt3 + gradt4);*/

            grad1[0] = (grad[id2][j] + grad[i][jd2] + grad[i][j] * 2 + grad[i][ja2] + grad[ia2][j] + gradh1 + gradh11 + gradh12);
            grad1[1] = (grad[id2][ja1] + grad[i][jd1] + grad[i][ja1] * 2 + grad[i][ja3] + grad[ia2][ja1] + gradv1 + gradv11 + gradv12);
            grad1[2] = (grad[id1][j] + grad[ia1][jd2] + grad[ia1][j] * 2 + grad[ia1][ja2] + grad[ia3][j] + grads1 + grads2 + grads11 + grads12);
            grad1[3] = (grad[id1][ja1] + grad[ia1][jd1] + grad[ia1][ja1] * 2 + grad[ia1][ja3] + grad[ia3][ja1] + gradt1 + gradt2 + gradt11 + gradt12);

            grad2[0] = (grad[i][ja2] + grad[ia2][j] + grad[ia2][ja2] * 2 + grad[ia2][ja4] + grad[ia4][ja2] + gradh2 + gradh21 + gradh22);
            grad2[1] = (grad[i][ja3] + grad[ia2][ja1] + grad[ia2][ja3] * 2 + grad[ia2][ja5] + grad[ia4][ja3] + gradv2 + gradv21 + gradv22);
            grad2[2] = (grad[ia1][ja2] + grad[ia3][j] + grad[ia3][ja2] * 2 + grad[ia3][ja4] + grad[ia5][ja2] + grads2 + grads4 + grads21 + grads22);
            grad2[3] = (grad[ia1][ja3] + grad[ia3][ja1] + grad[ia3][ja3] * 2 + grad[ia3][ja5] + grad[ia5][ja3] + gradt3 + gradt4 + gradt21 + gradt22);


            //out_tmp[i][j] = grad1[0];
            //out_tmp[i][ja1] = grad1[1];
            //out_tmp[ia1][j] = grad1[2];
            //out_tmp[ia1][ja1] = grad1[3];
            //out_tmp[ia2][ja2] = grad2[0];
            //out_tmp[ia2][ja3] = grad2[1];
            //out_tmp[ia3][ja2] = grad2[2];
            //out_tmp[ia3][ja3] = grad2[3];

            grad1_min = grad1[0];
            grad1_max = grad1[0];
            grad2_min = grad2[0];
            grad2_max = grad2[0];
            dir1_min = 0;
            dir2_min = 0;
            for (k = 1;k < 4;k++) {
                if (grad1[k]<grad1_min) {
                    grad1_min = grad1[k];
                    dir1_min = k;
                }
                else if (grad1[k]>grad1_max) {
                    grad1_max = grad1[k];
                }
                if (grad2[k]<grad2_min) {
                    grad2_min = grad2[k];
                    dir2_min = k;
                }
                else if (grad2[k]>grad2_max) {
                    grad2_max = grad2[k];
                }
            }
            if((grad1_max-grad1_min)< flat_th_1)//  && grad1_min < 3*flat_th_1/4)
            { // || ((grad1_min2 - grad1_min)<diff_th && ((dir1_min==0 && dir1_min2==1)|| (dir1_min == 1 && dir1_min2 == 0)|| (dir1_min == 2 && dir1_min2 == 3)|| (dir1_min == 3 && dir1_min2 == 2)))){
                grad[id2][j]=4;
            } else{
                grad[id2][j]=dir1_min;
            }
            //grad[id2][ja1]=grad1_min;
            //grad[id1][j]=2;

            if((grad2_max-grad2_min)< flat_th_2)//  && grad2_min < 3*flat_th_2/4)
            { // || ((grad2_min2 - grad2_min)<diff_th && ((dir2_min == 0 && dir2_min2 == 1) || (dir2_min == 1 && dir2_min2 == 0) || (dir2_min == 2 && dir2_min2 == 3) || (dir2_min == 3 && dir2_min2 == 2)))) {
                grad[i][jd2] = 4;
            }
            else {
                grad[i][jd2] = dir2_min;
            }
        }
    }
}

void AlgorithmFuncs::bbb14(short **in, short ** grad, int offset_x, int offset_y, int width, int height)
{
    int i, j;
    int id1, id2,id3, id4, id5,id6, ia1,ia2,ia3,ia4;
    int jd1, jd2, jd3, jd4, jd5,jd6, ja1, ja2, ja3, ja4, ja5;
    short dir_cur;
    short t1, t2, t3, t4;
    short k, dir_vote[5], dir_vote_max, dir_use;

    for (i = offset_y;i<height - 4;i += 4) {
        id1 = i - 1;id2 = i - 2;id3 = i - 3;id4 = i - 4; id5 = i - 5;id6 = i - 6;
        ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;ia4 = i + 4;
        for (j = offset_x;j<width - 6;j += 4) {
            jd1 = j - 1;jd2 = j - 2;jd3 = j - 3;jd4 = j - 4;jd5 = j - 5;jd6 = j - 6;
            ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;ja4 = j + 4;ja5 = j + 5;
            dir_cur = grad[id2][j];

            //if (dir_cur == 4) {
            //	dir_left = grad[id2][jd4]; dir_right = grad[id2][ja4]; dir_up = grad[id6][j];
            //	grad_left = grad[id2][jd3]; grad_right = grad[id2][ja5]; grad_up = grad[id6][ja1];
            //	inherit_left = grad[id1][jd4]; inherit_right = grad[id1][ja4]; inherit_up = grad[id5][j];
            //	t1 = grad_left<grad_right;
            //	t2 = grad_left<grad_up;
            //	t3 = grad_right<grad_up;
            //	min_dir = t1&&t2 ? dir_left : (t3&&!t1 ? dir_right : dir_up);
            //	min_inherit = t1&&t2 ? inherit_left : (t3&&!t1 ? inherit_right : inherit_up);
            //	if (min_inherit>0) {
            //		dir_cur = min_dir;
            //		grad[id1][j] = min_inherit - 1;
            //	}
            //}
            dir_vote[0] = 0;
            dir_vote[1] = 0;
            dir_vote[2] = 0;
            dir_vote[3] = 0;
            dir_vote[4] = 0;
            dir_vote_max = 0;
            dir_use = 0;
            for (k = 0;k <= 4;k++) {
                if (grad[id4][jd6] == k) dir_vote[k] += 1;
                if (grad[id4][jd2] == k) dir_vote[k] += 1;
                if (grad[i][jd6] == k) dir_vote[k] += 1;
                if (grad[i][jd2] == k) dir_vote[k] += 1;
                if (dir_cur == k) dir_vote[k] += 1;
                if (dir_vote[k] > dir_vote_max) {
                    dir_vote_max = dir_vote[k];
                    dir_use = k;
                }
            }
            dir_cur = dir_use;

            if (dir_cur == 0) { // -
                grad[i][j] = (in[i][jd1] * 2 + in[i][ja2]) / 3;
                grad[i][ja1] = (in[i][jd1] + in[i][ja2] * 2) / 3;
                grad[ia1][j] = (in[ia1][jd1] * 2 + in[ia1][ja2]) / 3;
                grad[ia1][ja1] = (in[ia1][jd1] + in[ia1][ja2] * 2) / 3;
            }
            else if (dir_cur == 1) { // |
                grad[i][j] = (in[id1][j] * 2 + in[ia2][j]) / 3;
                grad[i][ja1] = (in[id1][ja1] * 2 + in[ia2][ja1]) / 3;
                grad[ia1][j] = (in[id1][j] + in[ia2][j] * 2) / 3;
                grad[ia1][ja1] = (in[id1][ja1] + in[ia2][ja1] * 2) / 3;
            }
            else if (dir_cur == 2) { // /
                t1 = (in[ia1][jd1] + in[id1][ja1]) >> 1;
                t2 = (in[ia2][j] + in[i][ja2]) >> 1;
                t3 = (in[i][j] - t1 + in[ia1][ja1] - t2) / 2;
                grad[i][ja1] = in[i][ja1] - t3;
                grad[ia1][j] = in[ia1][j] - t3;
                grad[i][j] = t1;
                grad[ia1][ja1] = t2;
            }
            else if (dir_cur == 3) { //
                t1 = (in[id1][j] + in[ia1][ja2]) >> 1;
                t2 = (in[i][jd1] + in[ia2][ja1]) >> 1;
                t3 = (in[i][ja1] - t1 + in[ia1][j] - t2) / 2;
                grad[i][j] = in[i][j] - t3;
                grad[ia1][ja1] = in[ia1][ja1] - t3;
                grad[i][ja1] = t1;
                grad[ia1][j] = t2;
            }
            else {

                t1 = in[i][jd1] + in[ia1][jd1];
                t2 = in[id1][j] + in[id1][ja1];
                t3 = in[i][ja2] + in[ia1][ja2];
                t4 = in[ia2][j] + in[ia2][ja1];
                grad[i][j] = (t1 + t2) >> 2;
                grad[i][ja1] = (t2 + t3) >> 2;
                grad[ia1][j] = (t1 + t4) >> 2;
                grad[ia1][ja1] = (t3 + t4) >> 2;

                /*grad[i][j] = (in[i][jd1] + in[i][ja2] + in[id1][j] + in[ia2][j]) >> 2;
                grad[i][ja1] = (in[i][jd1] + in[i][ja2] + in[id1][ja1] + in[ia2][ja1]) >> 2;
                grad[ia1][j] = (in[ia1][jd1] + in[ia1][ja2] + in[id1][j] + in[ia2][j]) >> 2;
                grad[ia1][ja1] = (in[ia1][jd1] + in[ia1][ja2] + in[id1][ja1] + in[ia2][ja1]) >> 2;*/
            }

            dir_cur = grad[i][jd2];

            //if (dir_cur == 4) {
            //	dir_left = grad[i][jd6]; dir_right = grad[i][ja2]; dir_up = grad[id4][jd2];
            //	grad_left = grad[i][jd5]; grad_right = grad[i][ja3]; grad_up = grad[id4][jd1];
            //	inherit_left = grad[ia1][jd6]; inherit_right = grad[ia1][ja2]; inherit_up = grad[id3][jd2];
            //	t1 = grad_left<grad_right;
            //	t2 = grad_left<grad_up;
            //	t3 = grad_right<grad_up;
            //	min_dir = t1&&t2 ? dir_left : (t3&&!t1 ? dir_right : dir_up);
            //	min_inherit = t1&&t2 ? inherit_left : (t3&&!t1 ? inherit_right : inherit_up);
            //	if (min_inherit>0) {
            //		dir_cur = min_dir;
            //		grad[ia1][jd2] = min_inherit - 1;
            //	}
            //}

            dir_vote[0] = 0;
            dir_vote[1] = 0;
            dir_vote[2] = 0;
            dir_vote[3] = 0;
            dir_vote[4] = 0;
            dir_vote_max = 0;
            dir_use = 0;
            for (k = 0;k <= 4;k++) {
                if (grad[id2][j] == k) dir_vote[k] += 1;
                if (grad[id2][ja4] == k) dir_vote[k] += 1;
                if (grad[ia2][j] == k) dir_vote[k] += 1;
                if (grad[ia2][ja4] == k) dir_vote[k] += 1;
                if (dir_cur == k) dir_vote[k] += 1;
                if (dir_vote[k] > dir_vote_max) {
                    dir_vote_max = dir_vote[k];
                    dir_use = k;
                }
            }
            dir_cur = dir_use;

            if (dir_cur == 0) {
                grad[ia2][ja2] = (in[ia2][ja1] * 2 + in[ia2][ja4]) / 3;
                grad[ia2][ja3] = (in[ia2][ja1] + in[ia2][ja4] * 2) / 3;
                grad[ia3][ja2] = (in[ia3][ja1] * 2 + in[ia3][ja4]) / 3;
                grad[ia3][ja3] = (in[ia3][ja1] + in[ia3][ja4] * 2) / 3;
            }
            else if (dir_cur == 1) {
                grad[ia2][ja2] = (in[ia1][ja2] * 2 + in[ia4][ja2]) / 3;
                grad[ia2][ja3] = (in[ia1][ja3] * 2 + in[ia4][ja3]) / 3;
                grad[ia3][ja2] = (in[ia1][ja2] + in[ia4][ja2] * 2) / 3;
                grad[ia3][ja3] = (in[ia1][ja3] + in[ia4][ja3] * 2) / 3;
            }
            else if (dir_cur == 2) {
                t1 = (in[ia3][ja1] + in[ia1][ja3]) >> 1;
                t2 = (in[ia4][ja2] + in[ia2][ja4]) >> 1;
                t3 = (in[ia2][ja2] - t1 + in[ia3][ja3] - t2) / 2;
                grad[ia2][ja3] = in[ia2][ja3] - t3;
                grad[ia3][ja2] = in[ia3][ja2] - t3;
                grad[ia2][ja2] = t1;
                grad[ia3][ja3] = t2;
            }
            else if (dir_cur == 3) {
                t1 = (in[ia1][ja2] + in[ia3][ja4]) >> 1;
                t2 = (in[ia2][ja1] + in[ia4][ja3]) >> 1;
                t3 = (in[ia2][ja3] - t1 + in[ia3][ja2] - t2) / 2;
                grad[ia2][ja2] = in[ia2][ja2] - t3;
                grad[ia3][ja3] = in[ia3][ja3] - t3;
                grad[ia2][ja3] = t1;
                grad[ia3][ja2] = t2;
            }
            else {
                t1 = in[ia2][ja1] + in[ia3][ja1];
                t2 = in[ia1][ja2] + in[ia1][ja3];
                t3 = in[ia2][ja4] + in[ia3][ja4];
                t4 = in[ia4][ja2] + in[ia4][ja3];
                grad[ia2][ja2] = (t1 + t2) >> 2;
                grad[ia2][ja3] = (t2 + t3) >> 2;
                grad[ia3][ja2] = (t1 + t4) >> 2;
                grad[ia3][ja3] = (t3 + t4) >> 2;
                //grad[ia2][ja2] = (in[ia2][ja1] + in[ia2][ja4] + in[ia1][ja2] + in[ia4][ja2]) >> 2;
                //grad[ia2][ja3] = (in[ia2][ja1] + in[ia2][ja4] + in[ia1][ja3] + in[ia4][ja3]) >> 2;
                //grad[ia3][ja2] = (in[ia3][ja1] + in[ia3][ja4] + in[ia1][ja2] + in[ia4][ja2]) >> 2;
                //grad[ia3][ja3] = (in[ia3][ja1] + in[ia3][ja4] + in[ia1][ja3] + in[ia4][ja3]) >> 2;
            }
        }
    }
}

void AlgorithmFuncs::bbb15(short **in, short **grad, short **out, int offset_x, int offset_y, int width, int height, int cfa, int fc_en, int fc_level)
{
    int id3,id1, i, ia1, ia2, ia3, ia4;
    int jd3,jd1, j, ja1, ja2, ja3, ja4,ja6;
    int hp1, hp2,hp3,hp4;
    if (cfa == 0 || cfa == 3) {
        if (fc_en) {
            for (i = offset_y - 4;i < height - 4;i += 4) {
                ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;
                for (j = offset_x - 4;j < width - 4;j += 4) {
                    ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;
                    grad[i][j] = in[i][j] + in[i][ja1] + in[ia1][j] + in[ia1][ja1];
                    grad[i][ja1] = grad[i][ja2] + grad[i][ja3] + grad[ia1][ja2] + grad[ia1][ja3];
                    grad[ia1][j] = grad[ia2][j] + grad[ia2][ja1] + grad[ia3][j] + grad[ia3][ja1];
                    grad[ia1][ja1] = in[ia2][ja2] + in[ia2][ja3] + in[ia3][ja2] + in[ia3][ja3];
                }
            }
            for (i = offset_y;i < height - 8;i += 4) {
                id3 = i - 3;id1 = i - 1;ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;ia4 = i + 4;
                for (j = offset_x;j < width - 8;j += 4) {
                    jd3 = j - 3;jd1 = j - 1;ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;ja4 = j + 4;

                    hp1 = abs((grad[i][j] << 2) - grad[id3][j] - grad[ia1][j] - grad[i][jd3] - grad[i][ja1]) >> fc_level;
                    hp2 = abs((grad[i][ja1] << 2) - grad[id3][ja1] - grad[ia1][ja1] - grad[i][j] - grad[i][ja4]) >> fc_level;
                    hp3 = abs((grad[ia1][j] << 2) - grad[i][j] - grad[ia4][j] - grad[ia1][jd3] - grad[ia1][ja1]) >> fc_level;
                    hp4 = abs((grad[ia1][ja1] << 2) - grad[i][ja1] - grad[ia4][ja1] - grad[ia1][j] - grad[ia1][ja4]) >> fc_level;
                    hp1 = 256 - hp1; if (hp1 < 0) hp1 = 0;
                    hp2 = 256 - hp2; if (hp2 < 0) hp2 = 0;
                    hp3 = 256 - hp3; if (hp3 < 0) hp3 = 0;
                    hp4 = 256 - hp4; if (hp4 < 0) hp4 = 0;


                    out[i][j] = in[i][j];
                    out[i][ja1] = in[i][ja1] + ((((cg(i, jd1) + cg(i, ja2) * 2) / 3)*hp1) >> 8);
                    out[i][ja2] = grad[i][ja2];
                    out[i][ja3] = grad[i][ja3] + (((cg(i, ja3))*hp2) >> 8);
                    out[ia1][j] = in[ia1][j] + ((((cg(id1, j) + cg(ia2, j) * 2) / 3)*hp1) >> 8);
                    out[ia1][ja1] = in[ia1][ja1];
                    out[ia1][ja2] = grad[ia1][ja2] + ((((cg(id1, ja1) * 2 + cg(ia2, ja1) * 4 + cg(id1, ja4) + cg(ia2, ja4) * 2) / 9)*hp2) >> 8);
                    out[ia1][ja3] = grad[ia1][ja3];
                    out[ia2][j] = grad[ia2][j];
                    out[ia2][ja1] = grad[ia2][ja1] + ((((cg(ia1, jd1) * 2 + cg(ia1, ja2) * 4 + cg(ia4, jd1) + cg(ia4, ja2) * 2) / 9)*hp3) >> 8);
                    out[ia2][ja2] = in[ia2][ja2];
                    out[ia2][ja3] = in[ia2][ja3] + ((((cg(ia1, ja3) * 2 + cg(ia4, ja3)) / 3)*hp4) >> 8);
                    out[ia3][j] = grad[ia3][j] + (((cg(ia3, j))*hp3) >> 8);
                    out[ia3][ja1] = grad[ia3][ja1];
                    out[ia3][ja2] = in[ia3][ja2] + ((((cg(ia3, ja1) * 2 + cg(ia3, ja4)) / 3)*hp4) >> 8);
                    out[ia3][ja3] = in[ia3][ja3];
                }
            }

        }
        else
        {
            for (i = offset_y;i < height - 8;i += 4) {
                id1 = i - 1;ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;ia4 = i + 4;
                for (j = offset_x;j < width - 8;j += 4) {
                    jd1 = j - 1;ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;ja4 = j + 4;
                    out[i][j] = in[i][j];
                    out[i][ja1] = in[i][ja1] + (cg(i, jd1) + cg(i, ja2) * 2) / 3;
                    out[i][ja2] = grad[i][ja2];
                    out[i][ja3] = in[i][ja3];
                    out[ia1][j] = in[ia1][j] + (cg(id1, j) + cg(ia2, j) * 2) / 3;
                    out[ia1][ja1] = in[ia1][ja1];
                    out[ia1][ja2] = grad[ia1][ja2] + (cg(id1, ja1) * 2 + cg(ia2, ja1) * 4 + cg(id1, ja4) + cg(ia2, ja4) * 2) / 9;
                    out[ia1][ja3] = grad[ia1][ja3];
                    out[ia2][j] = grad[ia2][j];
                    out[ia2][ja1] = grad[ia2][ja1] + (cg(ia1, jd1) * 2 + cg(ia1, ja2) * 4 + cg(ia4, jd1) + cg(ia4, ja2) * 2) / 9;
                    out[ia2][ja2] = in[ia2][ja2];
                    out[ia2][ja3] = in[ia2][ja3] + (cg(ia1, ja3) * 2 + cg(ia4, ja3)) / 3;
                    out[ia3][j] = in[ia3][j];
                    out[ia3][ja1] = grad[ia3][ja1];
                    out[ia3][ja2] = in[ia3][ja2] + (cg(ia3, ja1) * 2 + cg(ia3, ja4)) / 3;
                    out[ia3][ja3] = in[ia3][ja3];
                }
            }
        }
    }
    else
    {
        if (fc_en) {
            for (i = offset_y - 4;i < height - 4;i += 4) {
                ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;
                for (j = offset_x - 4;j < width - 4;j += 4) {
                    ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;
                    grad[i][ja2] = grad[i][j] + grad[i][ja1] + grad[ia1][j] + grad[ia1][ja1];
                    grad[i][ja3] = in[i][ja2] + in[i][ja3] + in[ia1][ja2] + in[ia1][ja3];
                    grad[ia1][ja2] = in[ia2][j] + in[ia2][ja1] + in[ia3][j] + in[ia3][ja1];
                    grad[ia1][ja3] = grad[ia2][ja2] + grad[ia2][ja3] + grad[ia3][ja2] + grad[ia3][ja3];
                }
            }
            for (i = offset_y;i < height - 8;i += 4) {
                id3 = i - 3;id1 = i - 1;ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;ia4 = i + 4;
                for (j = offset_x;j < width - 8;j += 4) {
                    ja6 = j + 3;jd1 = j - 1;ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;ja4 = j + 4;

                    hp1 = abs((grad[i][ja2] << 2) - grad[id3][ja2] - grad[ia1][ja2] - grad[i][jd1] - grad[i][ja3]) >> fc_level;
                    hp2 = abs((grad[i][ja3] << 2) - grad[id3][ja3] - grad[ia1][ja3] - grad[i][ja2] - grad[i][ja6]) >> fc_level;
                    hp3 = abs((grad[ia1][ja2] << 2) - grad[i][ja2] - grad[ia4][ja2] - grad[ia1][jd1] - grad[ia1][ja3]) >> fc_level;
                    hp4 = abs((grad[ia1][ja3] << 2) - grad[i][ja3] - grad[ia4][ja3] - grad[ia1][ja2] - grad[ia1][ja6]) >> fc_level;
                    hp1 = 256 - hp1; if (hp1 < 0) hp1 = 0;
                    hp2 = 256 - hp2; if (hp2 < 0) hp2 = 0;
                    hp3 = 256 - hp3; if (hp3 < 0) hp3 = 0;
                    hp4 = 256 - hp4; if (hp4 < 0) hp4 = 0;

                    out[i][j] = grad[i][j]+(((cg(i,j))*hp1)>>8);
                    out[i][ja1] = grad[i][ja1];
                    out[i][ja2] = in[i][ja2] + ((((cg(i, ja1) * 2 + cg(i, ja4)) / 3)*hp2)>>8);
                    out[i][ja3] = in[i][ja3];
                    out[ia1][j] = grad[ia1][j];
                    out[ia1][ja1] = grad[ia1][ja1] + ((((cg(id1, jd1) + cg(id1, ja2) * 2 + cg(ia2, jd1) * 2 + cg(ia2, ja2) * 4) / 9)*hp1)>>8);
                    out[ia1][ja2] = in[ia1][ja2];
                    out[ia1][ja3] = in[ia1][ja3] + ((((cg(id1, ja3) + cg(ia2, ja3) * 2) / 3)*hp2)>>8);
                    out[ia2][j] = in[ia2][j] + ((((cg(ia1, j) * 2 + cg(ia4, j)) / 3)*hp3)>>8);
                    out[ia2][ja1] = in[ia2][ja1];
                    out[ia2][ja2] = grad[ia2][ja2] + ((((cg(ia1, ja1) * 4 + cg(ia1, ja4) * 2 + cg(ia4, ja1) * 2 + cg(ia4, ja4)) / 9)*hp4)>>8);
                    out[ia2][ja3] = grad[ia2][ja3];
                    out[ia3][j] = in[ia3][j];
                    out[ia3][ja1] = in[ia3][ja1] + ((((cg(ia3, jd1) + cg(ia3, ja2) * 2) / 3)*hp3)>>8);
                    out[ia3][ja2] = grad[ia3][ja2];
                    out[ia3][ja3] = grad[ia3][ja3]+(((cg(ia3,ja3))*hp4)>>8);
                }
            }
        }
        else
        {
            for (i = offset_y;i < height - 8;i += 4) {
                id1 = i - 1;ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;ia4 = i + 4;
                for (j = offset_x;j < width - 8;j += 4) {
                    jd1 = j - 1;ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;ja4 = j + 4;
                    out[i][j] = in[i][j];
                    out[i][ja1] = grad[i][ja1];
                    out[i][ja2] = in[i][ja2] + (cg(i, ja1) * 2 + cg(i, ja4)) / 3;
                    out[i][ja3] = in[i][ja3];
                    out[ia1][j] = grad[ia1][j];
                    out[ia1][ja1] = grad[ia1][ja1] + (cg(id1, jd1) + cg(id1, ja2) * 2 + cg(ia2, jd1) * 2 + cg(ia2, ja2) * 4) / 9;
                    out[ia1][ja2] = in[ia1][ja2];
                    out[ia1][ja3] = in[ia1][ja3] + (cg(id1, ja3) + cg(ia2, ja3) * 2) / 3;
                    out[ia2][j] = in[ia2][j] + (cg(ia1, j) * 2 + cg(ia4, j)) / 3;
                    out[ia2][ja1] = in[ia2][ja1];
                    out[ia2][ja2] = grad[ia2][ja2] + (cg(ia1, ja1) * 4 + cg(ia1, ja4) * 2 + cg(ia4, ja1) * 2 + cg(ia4, ja4)) / 9;
                    out[ia2][ja3] = grad[ia2][ja3];
                    out[ia3][j] = in[ia3][j];
                    out[ia3][ja1] = in[ia3][ja1] + (cg(ia3, jd1) + cg(ia3, ja2) * 2) / 3;
                    out[ia3][ja2] = grad[ia3][ja2];
                    out[ia3][ja3] = in[ia3][ja3];
                }
            }
        }
    }
    // for median only: in = G plane
    /*
    for (i = offset_y - 4;i < height - 4;i += 4) {
        ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;
        for (j = offset_x - 4;j < width - 4;j += 4) {
            ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;
            in[i][j] = grad[i][j];
            in[i][ja1] = grad[i][ja1];
            in[ia1][j] = grad[ia1][j];
            in[ia1][ja1] = grad[ia1][ja1];
            in[ia2][ja2] = grad[ia2][ja2];
            in[ia2][ja3] = grad[ia2][ja3];
            in[ia3][ja2] = grad[ia3][ja2];
            in[ia3][ja3] = grad[ia3][ja3];
        }
    }
    */
}

short AlgorithmFuncs::bbb16(short a1,short a2,short a3,short a4,short a5)
{
    short out, min1, min2, max1, max2;
    if (a1 < a2) {
        min1 = a1; max1 = a2;
    }
    else {
        min1 = a2; max1 = a1;
    }
    if (a3 < a4) {
        min2 = a3; max2 = a4;
    }
    else {
        min2 = a4; max2 = a3;
    }
    if (min1 < min2) {
        if (a5 < max1) {
            min1 = a5;
        }
        else {
            min1 = max1;
            max1 = a5;
        }
    }
    else {
        if (a5 < max2) {
            min2 = a5;
        }
        else {
            min2 = max2;
            max2 = a5;
        }
    }
    if (min1 < min2) {
        if (max1 < min2) {
            out = max1;
        }
        else {
            out = min2;
        }
    }
    else {
        if (max2 < min1) {
            out = max2;
        }
        else {
            out = min1;
        }
    }
    return(out);
}

void AlgorithmFuncs::bbb17(short **in, short **g, short **cd, int offset_x, int offset_y, int width, int height)
{
    int id4, id2, i, ia1, ia2, ia4;
    int jd4, jd2, jd1, j, ja1, ja2, ja4;
    short med;
    short a1, a2, a3, a4, a5;
    for (i = offset_y-4;i < height-4;i+=2){
        ia1 = i + 1;
        for (j = offset_x-6;j < width-4;j+=2){
            ja1 = j + 1;
            cd[i][ja1] = in[i][j]-g[i][j];
            cd[ia1][j] = in[ia1][ja1]-g[ia1][ja1];
        }
    }
    // vertical
    for (i = offset_y;i < height - 8;i += 2) {
        id4 = i - 4;id2 = i - 2;ia2 = i + 2;ia4 = i + 4;
        for (j = offset_x;j < width - 8;j += 2) {
            ja1 = j + 1;
            // 1x5 median filter
            a1 = cd[id4][ja1];//in[id4][j] - g[id4][j];//cd[id4][j];
            a2 = cd[id2][ja1];//in[id2][j] - g[id2][j];//cd[id2][j];
            a3 = cd[i][ja1];//in[i][j] - g[i][j];//cd[i][j];
            a4 = cd[ia2][ja1];//in[ia2][j] - g[ia2][j];//cd[ia2][j];
            a5 = cd[ia4][ja1];//in[ia4][j] - g[ia4][j];//cd[ia4][j];
            cd[i][j] = bbb16(a1, a2, a3, a4, a5);
        }
    }
    for (i = offset_y+1;i < height - 7;i += 2) {
        id4 = i - 4;id2 = i - 2;ia2 = i + 2;ia4 = i + 4;
        for (j = offset_x+1;j < width - 7;j += 2) {
            jd1 = j - 1;
            // 1x5 median filter
            a1 = cd[id4][jd1];//in[id4][j] - g[id4][j];//cd[id4][j];
            a2 = cd[id2][jd1];//in[id2][j] - g[id2][j];//cd[id2][j];
            a3 = cd[i][jd1];//in[i][j] - g[i][j];//cd[i][j];
            a4 = cd[ia2][jd1];//in[ia2][j] - g[ia2][j];//cd[ia2][j];
            a5 = cd[ia4][jd1];//in[ia4][j] - g[ia4][j];//cd[ia4][j];
            cd[i][j] = bbb16(a1, a2, a3, a4, a5);
        }
    }

    //horizontal
    for (i = offset_y;i < height - 8;i += 2) {
        for (j = offset_x;j < width - 8;j += 2) {
            jd4 = j - 4;jd2 = j - 2;ja2 = j + 2;ja4 = j + 4;
            // 1x5 median filter
            a1 = cd[i][jd4];
            a2 = cd[i][jd2];
            a3 = cd[i][j];
            a4 = cd[i][ja2];
            a5 = cd[i][ja4];
            med = bbb16(a1, a2, a3, a4, a5);
            in[i][j] = g[i][j] + med;
        }
    }
    for (i = offset_y + 1;i < height - 7;i += 2) {
        for (j = offset_x + 1;j < width - 7;j += 2) {
            jd4 = j - 4;jd2 = j - 2;ja2 = j + 2;ja4 = j + 4;
            // 1x5 median filter
            a1 = cd[i][jd4];
            a2 = cd[i][jd2];
            a3 = cd[i][j];
            a4 = cd[i][ja2];
            a5 = cd[i][ja4];
            med = bbb16(a1, a2, a3, a4, a5);
            in[i][j] = g[i][j] + med;
        }
    }

}

void AlgorithmFuncs::bbb18(short **in, short **grad, int width, int height)
{
    int i,j,ia1,ja1;
    for(i=0;i<height-1;i+=2){
        ia1=i+1;
        for(j=0;j<width-1;j+=2){
            ja1=j+1;
            grad[i][j]=(abs(in[i][j]-in[i][ja1])+abs(in[ia1][j]-in[ia1][ja1]))>>1;
            grad[i][ja1]=(abs(in[i][j]-in[ia1][j])+abs(in[i][ja1]-in[ia1][ja1]))>>1;
            grad[ia1][j]=abs(in[ia1][j]-in[i][ja1]);
            grad[ia1][ja1]=abs(in[i][j]-in[ia1][ja1]);
        }
    }
}

void AlgorithmFuncs::bbb19(short **in, int cfa, int width, int height, int r_gain, int gr_gain, int gb_gain, int b_gain)
{
    int i, j, ia1, ja1,ia2,ja2,ia3,ja3;
    int gain11,gain12,gain21,gain22;
    if(cfa==0){
        gain11=gr_gain; gain12=r_gain;
        gain21=b_gain; gain22=gb_gain;
    }else if(cfa==1){
        gain11=r_gain; gain12=gr_gain;
        gain21=gb_gain; gain22=b_gain;
    }else if(cfa==2){
        gain11=b_gain; gain12=gb_gain;
        gain21=gr_gain; gain22=r_gain;
    }else{
        gain11=gb_gain; gain12=b_gain;
        gain21=r_gain; gain22=gr_gain;
    }
    for (i = 0;i<height - 3;i += 4) {
        ia1 = i + 1;ia2 = i + 2;ia3 = i + 3;
        for (j = 0;j<width - 3;j += 4) {
            ja1 = j + 1;ja2 = j + 2;ja3 = j + 3;
            in[i][j] = (short)((in[i][j]*gain11)>>8);
            in[i][ja1] = (short)((in[i][ja1]*gain11)>>8);
            in[i][ja2] = (short)((in[i][ja2]*gain12)>>8);
            in[i][ja3] = (short)((in[i][ja3]*gain12)>>8);
            in[ia1][j] = (short)((in[ia1][j]*gain11)>>8);
            in[ia1][ja1] = (short)((in[ia1][ja1]*gain11)>>8);
            in[ia1][ja2] = (short)((in[ia1][ja2]*gain12)>>8);
            in[ia1][ja3] = (short)((in[ia1][ja3]*gain12)>>8);
            in[ia2][j] = (short)((in[ia2][j]*gain21)>>8);
            in[ia2][ja1] = (short)((in[ia2][ja1]*gain21)>>8);
            in[ia2][ja2] = (short)((in[ia2][ja2]*gain22)>>8);
            in[ia2][ja3] = (short)((in[ia2][ja3]*gain22)>>8);
            in[ia3][j] = (short)((in[ia3][j]*gain21)>>8);
            in[ia3][ja1] = (short)((in[ia3][ja1]*gain21)>>8);
            in[ia3][ja2] = (short)((in[ia3][ja2]*gain22)>>8);
            in[ia3][ja3] = (short)((in[ia3][ja3]*gain22)>>8);
        }
    }
}

void AlgorithmFuncs::bbb20(paramThreadScaler *para)
{
    unsigned char *in;
    unsigned char *out = para->gain_out;
    int width_out = para->width_out;
    int height_out = para->height_out;
    int width_in = para->width_in;
    int height_in=para->height_in;
    int k = para->ID;

    float a11, a12, a13;
    float a21, a22, a23;
    float a31, a32, a33;
    float b11, b12, b13;
    float b21, b22, b23;
    float b31, b32, b33;
    float c1, c2, c3, t;
    float x, y, y2;
    int ox, oy;
    int iii;
    int i, j, ii, jj;
    int x1, y1;
    int grid_w = width_in - 2;
    int grid_h = height_in - 2;
    int width_grid = (int)ceil((float)(width_out) / grid_w);
    int height_grid = (int)ceil((float)(height_out) / grid_h);
    int i_width_off0, i_width_off1, i_width_off2;
    int koff = (k >> 2)*width_out*4+ (k % 4);

    in = para->knee_in + k*width_in*height_in;

    //FILE *fp1;
    //unsigned char *out2 = (unsigned char*)malloc(width_out*height_out*sizeof(unsigned char));
    //if (k == 0) {
    //	fp1 = fopen("E:\\matlab\\4cell\\1009_code_500ms\\gaindown.raw", "wb");
    //	if (fp1) {
    //		fwrite(in, 48, 1, fp1);
    //		fclose(fp1);
    //	}
    //}

    for (i = 0;i < grid_h;i++) {
        y1 = i*height_out / grid_h;
        i_width_off0 = i*width_in;
        i_width_off1 = (i + 1)*width_in;
        i_width_off2 = (i + 2)*width_in;
        for (j = 0;j < grid_w;j++) {
            a11 = in[i_width_off0 + j]; a12 = in[i_width_off0 + j + 1]; a13 = in[i_width_off0 + j + 2];
            a21 = in[i_width_off1 + j]; a22 = in[i_width_off1 + j + 1]; a23 = in[i_width_off1 + j + 2];
            a31 = in[i_width_off2 + j]; a32 = in[i_width_off2 + j + 1]; a33 = in[i_width_off2 + j + 2];
            b11 = a11 / 4 - a12 / 2 + a13 / 4 - a21 / 2 + a22 - a23 / 2 + a31 / 4 - a32 / 2 + a33 / 4;
            b12 = a12 / 2 - a11 / 2 + a21 - a22 - a31 / 2 + a32 / 2;
            b13 = a11 / 4 + a12 / 4 - a21 / 2 - a22 / 2 + a31 / 4 + a32 / 4;
            b21 = a12 - a11 / 2 - a13 / 2 + a21 / 2 - a22 + a23 / 2;
            b22 = a11 - a12 - a21 + a22;
            b23 = a21 / 2 - a12 / 2 - a11 / 2 + a22 / 2;
            b31 = a11 / 4 - a12 / 2 + a13 / 4 + a21 / 4 - a22 / 2 + a23 / 4;
            b32 = a12 / 2 - a11 / 2 - a21 / 2 + a22 / 2;
            b33 = a11 / 4 + a12 / 4 + a21 / 4 + a22 / 4;
            x1 = j*width_out / grid_w;
            for (ii = 0;ii < height_grid;ii++) {
                oy = y1 + ii;
                iii = oy* width_out*16+ koff;
                y = (float)(ii*grid_h) / (float)height_out;
                y2 = y*y;
                c1 = b33 + b23*y + b13*y2;
                c2 = b12*y2 + b22*y + b32;
                c3 = b11*y2 + b21*y + b31;
                for (jj = 0;jj < width_grid;jj++) {
                    ox = x1 + jj;
                    x = (float)(jj*grid_w) / (float)width_out;
                    t = (c1 + x*c2 + x*x*c3);
                    if (t<0)
                    {
                        t = 0;
                    }
                    else if (t>255)
                    {
                        t = 255;
                    }
                    //if (k == 0) {
                    //	out2[(iii - koff) / 16 + ox] = (unsigned char)(t);
                    //}
                    out[iii + ox*4] = (unsigned char)(t);
                }
            }
        }
    }
    //if (k == 0) {
    //	fp1 = fopen("E:\\matlab\\4cell\\1009_code_500ms\\gainup.raw", "wb");
    //	if (fp1) {
    //		fwrite(out2, width_out*height_out*sizeof(unsigned char), 1, fp1);
    //		fclose(fp1);
    //	}
    //}
    //free(out2);
}

void * AlgorithmFuncs::bbb21(void *args)
{
//    pthread_t threadID = pthread_self();
    paramThreadScaler *para = (paramThreadScaler *)args;
    AlgorithmFuncs::bbb20(para);
    pthread_exit(NULL);
    return NULL;
}

void AlgorithmFuncs::bbb3(unsigned char *gainmap_down, unsigned char *I_out, int width_out, int height_out, int width_in, int height_in)
{
    pthread_t pt[16];
    paramThreadScaler args[16];
    int width = width_out>>2;
    int height = height_out>>2;
    int t;

    for (t = 0;t < 16;t++)
    {
        args[t].knee_in =gainmap_down;
        args[t].gain_out =I_out;
        args[t].ID=t;
        args[t].width_out =width;
        args[t].height_out =height;
        args[t].width_in=width_in;
        args[t].height_in=height_in;

        /*threadProcessScaler((void *)(&args[t]));*/
        pthread_create(&pt[t], NULL, &bbb21, (void *)(&args[t]));
    }
    // join thread
    for (t = 0;t < 16;t++) {
        pthread_join(pt[t], NULL);
    }
}

void AlgorithmFuncs::bbb4(unsigned char *in, unsigned char **out, int col_start, int block_w, int width, int height, int padding_flag, int crosstalk_otp_en)
{
    int i, j, t, ii, k1, k2, k3, k4;
    int iii, jjj;
    t = width;
    if (crosstalk_otp_en) {
        if (padding_flag == 0) { // leftmost block
            for (i = 0; i < height; i++) {
                ii = i*t;
                iii = i + 16;
                for (j = 0; j < block_w - 16; j += 4) {
                    jjj = j + 16;
                    k1 = ii + (j + col_start); k2 = k1 + 1; k3 = k1 + 2; k4 = k1 + 3;
                    out[iii][jjj] = in[k1];
                    out[iii][jjj + 1] = in[k2];
                    out[iii][jjj + 2] = in[k3];
                    out[iii][jjj + 3] = in[k4];
                }
            }
            // top padding size=16
            k2 = block_w * 2 - 35;
            for (i = 0; i < 16; i++) {
                k1 = 33 - i;
                for (j = 0; j < 16; j++) {
                    out[i][j] = out[k1][33 - j];
                }
                for (j = 16; j < block_w; j++) {
                    out[i][j] = out[k1][j];
                }
            }
            // bottom padding size=16
            for (i = height + 16; i < height + 32; i++) {
                k1 = height * 2 - i + 29;
                for (j = 0; j < 16; j++) {
                    out[i][j] = out[k1][33 - j];
                }
                for (j = 16; j < block_w; j++) {
                    out[i][j] = out[k1][j];
                }
            }
            // left padding size=16
            for (i = 16; i < height + 16; i++) {
                for (j = 0; j < 16; j++) {
                    out[i][j] = out[i][33 - j];
                }
            }
            // right padding size=16//fix by sue 2020417
            for (i = 16; i < height + 16; i++) {
                for (j = block_w - 16; j < block_w; j++) {
                    out[i][j] = out[i][k2 - j];
                }
            }
        }
        else if (padding_flag == THREAD_NUM - 1) { // rightmost block
            for (i = 0; i < height; i++) {
                ii = i*t;
                iii = i + 16;
                for (j = 0; j < block_w - 16; j += 4) {
                    jjj = j;
                    k1 = ii + (j + col_start - 16); k2 = k1 + 1; k3 = k1 + 2; k4 = k1 + 3;
                    out[iii][jjj] = in[k1];
                    out[iii][jjj + 1] = in[k2];
                    out[iii][jjj + 2] = in[k3];
                    out[iii][jjj + 3] = in[k4];
                }
            }
            // top padding size=16
            k2 = block_w * 2 - 35;
            for (i = 0; i < 16; i++) {
                k1 = 33 - i;
                for (j = 0; j < block_w - 16; j++) {
                    out[i][j] = out[k1][j];
                }
                for (j = block_w - 16; j < block_w; j++) {
                    out[i][j] = out[k1][k2 - j];
                }
            }
            // bottom padding size=16
            for (i = height + 16; i < height + 32; i++) {
                k1 = height * 2 - i + 29;
                for (j = 0; j < block_w - 16; j++) {
                    out[i][j] = out[k1][j];
                }
                for (j = block_w - 16; j < block_w; j++) {
                    out[i][j] = out[k1][k2 - j];
                }
            }
            // right padding size=16
            for (i = 16; i < height + 16; i++) {
                for (j = block_w - 16; j < block_w; j++) {
                    out[i][j] = out[i][k2 - j];
                }
            }
        }
        else { // middle blocks
            for (i = 0; i < height; i++) {
                ii = i*t;
                iii = i + 16;
                for (j = 0; j < block_w; j += 4) {
                    jjj = j;
                    k1 = ii + (j + col_start - 16); k2 = k1 + 1; k3 = k1 + 2; k4 = k1 + 3;
                    out[iii][jjj] = in[k1];
                    out[iii][jjj + 1] = in[k2];
                    out[iii][jjj + 2] = in[k3];
                    out[iii][jjj + 3] = in[k4];
                }
            }
            // top padding size=16
            k2 = block_w * 2 - 35;
            for (i = 0; i < 16; i++) {
                k1 = 33 - i;
                for (j = 0; j < block_w; j++) {
                    out[i][j] = out[k1][j];
                }
            }
            // bottom padding size=16
            for (i = height + 16; i < height + 32; i++) {
                k1 = height * 2 - i + 29;
                for (j = 0; j < block_w; j++) {
                    out[i][j] = out[k1][j];
                }
            }
        }
    }
    else {
        for (i = 0; i < BLOCK_H; i++) {
             memset(out[i],0,sizeof(unsigned char)*block_w);
        }
    }
}

void AlgorithmFuncs::bbb5(unsigned char *gain_down, unsigned char *gain_down_temp, int Mirror, int upsidedown, int CFAin, int CFAout, int samplerate_height, int samplerate_width)
{
    int i, j, k;
    int CFA_upsidedown, CFA_mirror, CFA_change;
    int allnum = samplerate_height * samplerate_width;

    for (k = 0; k < 16; k++) {
        if (Mirror == 1 && upsidedown == 1) {
            for (i = k * samplerate_height; i >(k - 1) * samplerate_height; i--) {
                for (j = (samplerate_width - 1); j > 0; j--) {
                    gain_down_temp[samplerate_width*(samplerate_height - 1 - i) + (samplerate_width - 1 - j)] = gain_down[samplerate_width*i + j];
                }
            }
        }
        else if (Mirror == 0 && upsidedown == 1) {
            for (i = k * samplerate_height; i > (k - 1) * samplerate_height; i--) {
                for (j = 0; j < samplerate_width; j++) {
                    gain_down_temp[samplerate_width*(samplerate_height - 1 - i) + j] = gain_down[samplerate_width*i + j];
                }
            }
        }
        else if (Mirror == 1 && upsidedown == 0) {
            for (i = k * samplerate_height; i < (k + 1) * samplerate_height; i++) {
                for (j = (samplerate_width - 1); j > 0; j--) {
                    gain_down_temp[samplerate_width*i + (samplerate_width - 1 - j)] = gain_down[samplerate_width*i + j];
                }
            }
        }
        else {
            memcpy(gain_down_temp, gain_down, 16 * allnum * sizeof(unsigned char));
        }
    }

    CFA_upsidedown = (((CFAin & 0x00000002) >> 1) != ((CFAout & 0x00000002) >> 1)); //MIrror Upsidedown CFA_mirror CFA_upsidedown
    CFA_mirror = (((CFAin & 0x00000001) >> 1) != ((CFAout & 0x00000001) >> 1));

    CFA_change = (Mirror << 3) + (upsidedown << 2) + (CFA_mirror << 1) + CFA_upsidedown;



    switch (CFA_change) {
    case 0:break;
    case 1: {
        memcpy(gain_down, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 2: {
        memcpy(gain_down, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 1 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 3: {
        memcpy(gain_down, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 0 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 1 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 4: {
        memcpy(gain_down, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 0 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 1 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 5: {
        memcpy(gain_down, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 0 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 1 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 6: {
        memcpy(gain_down, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 0 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 1 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 7: {
        memcpy(gain_down, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 0 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 1 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 8: {
        memcpy(gain_down, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 9: {
        memcpy(gain_down, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 10: {
        memcpy(gain_down, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 11: {
        memcpy(gain_down, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 12: {
        memcpy(gain_down, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 13: {
        memcpy(gain_down, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 14: {
        memcpy(gain_down, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        break; }
    case 15: {
        memcpy(gain_down, gain_down_temp + 15 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + allnum, gain_down_temp + 14 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 2 * allnum, gain_down_temp + 13 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 3 * allnum, gain_down_temp + 12 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 4 * allnum, gain_down_temp + 11 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 5 * allnum, gain_down_temp + 10 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 6 * allnum, gain_down_temp + 9 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 7 * allnum, gain_down_temp + 8 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 8 * allnum, gain_down_temp + 7 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 9 * allnum, gain_down_temp + 6 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 10 * allnum, gain_down_temp + 5 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 11 * allnum, gain_down_temp + 4 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 12 * allnum, gain_down_temp + 3 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 13 * allnum, gain_down_temp + 2 * allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 14 * allnum, gain_down_temp + allnum, allnum * sizeof(unsigned char));
        memcpy(gain_down + 15 * allnum, gain_down_temp, allnum * sizeof(unsigned char));
        break; }
    default:break;
    }
}

void AlgorithmFuncs::bbb22(short **in, short **grad, unsigned char **crosstalk_gain, int cfa, int width, int height, int xtalk_static_en, int xtalk_dynamic_en, int ge_th, int MIX_coeff, int ge_slope, int ge_str)
{
    int id2, i, ia1, ia2, ia3, ia4, ia6;
    int jd2, j, ja1, ja2, ja3, ja4, ja6;
    int id4, jd4;
    short in1, in2, in3, in4;
    int around, around_2;
    int ginterp;
    int dff_GE;
    int Alhfa;
    //int ge_th = 170 * 64;
    //int MIX_coeff = 128;
    //int ge_slope = 229;


    if (xtalk_static_en) {
        for (i = 0;i < height - 1;i += 2) {
            ia1 = i + 1;
            for (j = 0;j < width - 1;j += 2) {
                ja1 = j + 1;
                in1 = in[i][j];
                in2 = in[i][ja1];
                in3 = in[ia1][j];
                in4 = in[ia1][ja1];
                in1 = (short)((((int)in1) * ((int)crosstalk_gain[i][j])) >> 10) + in1;
                in2 = (short)((((int)in2) * ((int)crosstalk_gain[i][ja1])) >> 10) + in2;
                in3 = (short)((((int)in3) * ((int)crosstalk_gain[ia1][j])) >> 10) + in3;
                in4 = (short)((((int)in4) * ((int)crosstalk_gain[ia1][ja1])) >> 10) + in4;
                grad[i][j] = (in1 + in2 + in3 + in4);
                in[i][j] = in1;
                in[i][ja1] = in2;
                in[ia1][j] = in3;
                in[ia1][ja1] = in4;
            }
        }
    }
    else {
        for (i = 0;i < height - 1;i += 2) {
            ia1 = i + 1;
            for (j = 0;j < width - 1;j += 2) {
                ja1 = j + 1;
                in1 = in[i][j];
                in2 = in[i][ja1];
                in3 = in[ia1][j];
                in4 = in[ia1][ja1];
                grad[i][j] = (in1 + in2 + in3 + in4);
            }
        }
    }

    if (xtalk_dynamic_en) {
        if (cfa == 0 || cfa == 3) {
            for (i = 4; i < height - 4; i += 4) {
                id2 = i - 2; ia1 = i + 1; ia2 = i + 2; ia3 = i + 3; ia4 = i + 4; id4 = i - 4; ia6 = i + 6;
                for (j = 4; j < width - 4; j += 4) {
                    jd2 = j - 2; ja1 = j + 1; ja2 = j + 2; ja3 = j + 3; ja4 = j + 4; jd4 = j - 4; ja6 = j + 6;
                    if (((grad[i][j] > grad[id2][jd2]) && (grad[i][j] > grad[ia2][ja2]) || (grad[i][j] < grad[id2][jd2]) && (grad[i][j] < grad[ia2][ja2])) &&
                        ((grad[i][j] > grad[ia2][jd2]) && (grad[i][j] > grad[id2][ja2]) || (grad[i][j] < grad[ia2][jd2]) && (grad[i][j] < grad[id2][ja2])))
                    {
                        around = ((grad[id2][jd2] + grad[ia2][ja2] + grad[ia2][jd2] + grad[id2][ja2]) >> 2);
                        if (abs(around - grad[i][j]) < (grad[i][j] >> ge_str)) {
                            if ((abs(around - grad[i][j]) < ge_th)) {
                                around_2 = ((grad[id4][jd4] + grad[ia4][ja4] + grad[ia4][jd4] + grad[id4][ja4]) >> 2);
                                ginterp = around + ((MIX_coeff*(grad[i][j] - around_2)) >> 8);

                                ////减法做法
                                dff_GE = ((ginterp - grad[i][j]) >> 1);
                                dff_GE = (dff_GE >> 2) + ((dff_GE & 0x00000002) >> 1);
                                in[i][j] = dff_GE + in[i][j];
                                in[ia1][j] = dff_GE + in[ia1][j];
                                in[i][ja1] = dff_GE + in[i][ja1];
                                in[ia1][ja1] = dff_GE + in[ia1][ja1];

                                //除法做法
                                //gain_GE = (float)((grad[i][j] + ginterp) >> 1) / grad[i][j];
                                //in[i][j] = gain_GE * in[i][j];
                                //in[ia1][j] = gain_GE * in[ia1][j];
                                //in[i][ja1] = gain_GE * in[i][ja1];
                                //in[ia1][ja1] = gain_GE * in[ia1][ja1];
                            }
                            else {
                                Alhfa = 128 + ((ge_slope * (ge_th - abs(grad[i][j] - around))) >> 10);// Alhfa 0.8
                                if (Alhfa > 0) {
                                    around_2 = ((grad[id4][jd4] + grad[ia4][ja4] + grad[ia4][jd4] + grad[id4][ja4]) >> 2);
                                    ginterp = around + ((MIX_coeff*(grad[i][j] - around_2)) >> 8);

                                    //减法做法
                                    dff_GE = ((Alhfa * (ginterp - grad[i][j])) >> 8);
                                    dff_GE = (dff_GE >> 2) + ((dff_GE & 0x00000002) >> 1);
                                    in[i][j] = dff_GE + in[i][j];
                                    in[ia1][j] = dff_GE + in[ia1][j];
                                    in[i][ja1] = dff_GE + in[i][ja1];
                                    in[ia1][ja1] = dff_GE + in[ia1][ja1];

                                    //除法做法
                                    //gain_GE = (float)(((256 - Alhfa) * grad[i][j] + Alhfa * ginterp) >> 8) / grad[i][j];
                                    //in[i][j] = gain_GE * in[i][j];
                                    //in[ia1][j] = gain_GE * in[ia1][j];
                                    //in[i][ja1] = gain_GE * in[i][ja1];
                                    //in[ia1][ja1] = gain_GE * in[ia1][ja1];
                                }
                            }
                        }
                    }
                    if (((grad[ia2][ja2] > grad[i][j]) && (grad[ia2][ja2] > grad[ia4][ja4]) || (grad[ia2][ja2] < grad[i][j]) && (grad[ia2][ja2] < grad[ia4][ja4])) &&
                        ((grad[ia2][ja2] > grad[ia4][j]) && (grad[ia2][ja2] > grad[i][ja4]) || (grad[ia2][ja2] < grad[ia4][j]) && (grad[ia2][ja2] < grad[i][ja4])))
                    {
                        around = ((grad[i][j] + grad[ia4][ja4] + grad[ia4][j] + grad[i][ja4]) >> 2);
                        if (abs(around - grad[ia2][ja2]) < (grad[ia2][ja2] >> ge_str)) {
                            if ((abs(around - grad[ia2][ja2]) < ge_th)) {
                                around_2 = ((grad[id2][jd2] + grad[ia6][ja6] + grad[ia6][jd2] + grad[id2][ja6]) >> 2);
                                ginterp = around + ((MIX_coeff*(grad[ia2][ja2] - around_2)) >> 8);

                                ////减法做法
                                dff_GE = ((ginterp - grad[ia2][ja2]) >> 1);
                                dff_GE = (dff_GE >> 2) + ((dff_GE & 0x00000002) >> 1);
                                in[ia2][ja2] = dff_GE + in[ia2][ja2];
                                in[ia3][ja2] = dff_GE + in[ia3][ja2];
                                in[ia2][ja3] = dff_GE + in[ia2][ja3];
                                in[ia3][ja3] = dff_GE + in[ia3][ja3];

                                //除法做法
                                //gain_GE = (float)((grad[ia2][ja2] + ginterp) >> 1) / grad[ia2][ja2];
                                //in[ia2][ja2] = gain_GE * in[ia2][ja2];
                                //in[ia3][ja2] = gain_GE * in[ia3][ja2];
                                //in[ia2][ja3] = gain_GE * in[ia2][ja3];
                                //in[ia3][ja3] = gain_GE * in[ia3][ja3];
                            }
                            else {
                                Alhfa = 128 + ((ge_slope * (ge_th - abs(grad[ia2][ja2] - around))) >> 10);// Alhfa 0.8
                                if (Alhfa > 0) {
                                    around_2 = ((grad[id2][jd2] + grad[ia6][ja6] + grad[ia6][jd2] + grad[id2][ja6]) >> 2);
                                    ginterp = around + ((MIX_coeff*(grad[ia2][ja2] - around_2)) >> 8);

                                    //减法做法
                                    dff_GE = ((Alhfa * (ginterp - grad[ia2][ja2])) >> 8);
                                    dff_GE = (dff_GE >> 2) + ((dff_GE & 0x00000002) >> 1);
                                    in[ia2][ja2] = dff_GE + in[ia2][ja2];
                                    in[ia3][ja2] = dff_GE + in[ia3][ja2];
                                    in[ia2][ja3] = dff_GE + in[ia2][ja3];
                                    in[ia3][ja3] = dff_GE + in[ia3][ja3];

                                    //除法做法
                                    //gain_GE = (float)(((256 - Alhfa) * grad[ia2][ja2] + Alhfa * ginterp) >> 8) / grad[ia2][ja2];
                                    //in[ia2][ja2] = gain_GE * in[ia2][ja2];
                                    //in[ia3][ja2] = gain_GE * in[ia3][ja2];
                                    //in[ia2][ja3] = gain_GE * in[ia2][ja3];
                                    //in[ia3][ja3] = gain_GE * in[ia3][ja3];
                                }
                            }
                        }
                    }

                }
            }
        }
        else {
            for (i = 4; i < height - 4; i += 4) {
                id2 = i - 2; ia1 = i + 1; ia2 = i + 2; ia3 = i + 3; ia4 = i + 4; id4 = i - 4; ia6 = i + 6;
                for (j = 4; j < width - 4; j += 4) {
                    jd2 = j - 2; ja1 = j + 1; ja2 = j + 2; ja3 = j + 3; ja4 = j + 4; jd4 = j - 4; ja6 = j + 6;
                    if (((grad[i][ja2] > grad[id2][j]) && (grad[i][ja2] > grad[ia2][ja4]) || (grad[i][ja2] < grad[id2][j]) && (grad[i][ja2] < grad[ia2][ja4])) &&
                        ((grad[i][ja2] > grad[ia2][j]) && (grad[i][ja2] > grad[id2][ja4]) || (grad[i][ja2] < grad[ia2][j]) && (grad[i][ja2] < grad[id2][ja4])))
                    {
                        around = ((grad[id2][j] + grad[ia2][ja4] + grad[ia2][j] + grad[id2][ja4]) >> 2);
                        if (abs(around - grad[i][ja2]) < (grad[i][ja2] >> ge_str)) {
                            if ((abs(around - grad[i][ja2]) < ge_th)) {
                                around_2 = ((grad[id4][jd2] + grad[ia4][ja6] + grad[ia4][jd2] + grad[id4][ja6]) >> 2);
                                ginterp = around + ((MIX_coeff*(grad[i][ja2] - around_2)) >> 8);

                                ////减法做法
                                dff_GE = ((ginterp - grad[i][ja2]) >> 1);
                                dff_GE = (dff_GE >> 2) + ((dff_GE & 0x00000002) >> 1);
                                in[i][ja2] = dff_GE + in[i][ja2];
                                in[ia1][ja2] = dff_GE + in[ia1][ja2];
                                in[i][ja3] = dff_GE + in[i][ja3];
                                in[ia1][ja3] = dff_GE + in[ia1][ja3];

                                //除法做法
                                //gain_GE = (float)((grad[i][ja2] + ginterp) >> 1) / grad[i][ja2];
                                //in[i][ja2] = gain_GE * in[i][ja2];
                                //in[ia1][ja2] = gain_GE * in[ia1][ja2];
                                //in[i][ja3] = gain_GE * in[i][ja3];
                                //in[ia1][ja3] = gain_GE * in[ia1][ja3];
                            }
                            else {
                                Alhfa = 128 + ((ge_slope * (ge_th - abs(grad[i][ja2] - around))) >> 10);// Alhfa 0.8
                                if (Alhfa > 0) {
                                    around_2 = ((grad[id4][jd2] + grad[ia4][ja6] + grad[ia4][jd2] + grad[id4][ja6]) >> 2);
                                    ginterp = around + ((MIX_coeff*(grad[i][ja2] - around_2)) >> 8);

                                    //减法做法
                                    dff_GE = ((Alhfa * (ginterp - grad[i][ja2])) >> 8);
                                    dff_GE = (dff_GE >> 2) + ((dff_GE & 0x00000002) >> 1);
                                    in[i][ja2] = dff_GE + in[i][ja2];
                                    in[ia1][ja2] = dff_GE + in[ia1][ja2];
                                    in[i][ja3] = dff_GE + in[i][ja3];
                                    in[ia1][ja3] = dff_GE + in[ia1][ja3];

                                    //除法做法
                                    //gain_GE = (float)(((256 - Alhfa) * grad[i][ja2] + Alhfa * ginterp) >> 8) / grad[i][ja2];
                                    //in[i][ja2] = gain_GE * in[i][ja2];
                                    //in[ia1][ja2] = gain_GE * in[ia1][ja2];
                                    //in[i][ja3] = gain_GE * in[i][ja3];
                                    //in[ia1][ja3] = gain_GE * in[ia1][ja3];
                                }
                            }
                        }
                    }
                    if (((grad[ia2][j] > grad[i][jd2]) && (grad[ia2][j] > grad[ia4][ja2]) || (grad[ia2][j] < grad[i][jd2]) && (grad[ia2][j] < grad[ia4][ja2])) &&
                        ((grad[ia2][j] > grad[ia4][jd2]) && (grad[ia2][j] > grad[i][ja2]) || (grad[ia2][j] < grad[ia4][jd2]) && (grad[ia2][j] < grad[i][ja2])))
                    {
                        around = ((grad[i][jd2] + grad[ia4][ja2] + grad[ia4][jd2] + grad[i][ja2]) >> 2);
                        if (abs(around - grad[ia2][j]) < (grad[ia2][j] >> ge_str)) {
                            if ((abs(around - grad[ia2][j]) < ge_th)) {
                                around_2 = ((grad[id2][jd4] + grad[ia6][ja4] + grad[ia6][jd4] + grad[id2][ja4]) >> 2);
                                ginterp = around + ((MIX_coeff*(grad[ia2][j] - around_2)) >> 8);

                                ////减法做法
                                dff_GE = ((ginterp - grad[ia2][j]) >> 1);
                                dff_GE = (dff_GE >> 2) + ((dff_GE & 0x00000002) >> 1);
                                in[ia2][j] = dff_GE + in[ia2][j];
                                in[ia3][j] = dff_GE + in[ia3][j];
                                in[ia2][ja1] = dff_GE + in[ia2][ja1];
                                in[ia3][ja1] = dff_GE + in[ia3][ja1];

                                //除法做法
                                //gain_GE = (float)((grad[ia2][j] + ginterp) >> 1) / grad[ia2][j];
                                //in[ia2][j] = gain_GE * in[ia2][j];
                                //in[ia3][j] = gain_GE * in[ia3][j];
                                //in[ia2][ja1] = gain_GE * in[ia2][ja1];
                                //in[ia3][ja1] = gain_GE * in[ia3][ja1];
                            }
                            else {
                                Alhfa = 128 + ((ge_slope * (ge_th - abs(grad[ia2][j] - around))) >> 10);// Alhfa 0.8
                                if (Alhfa > 0) {
                                    around_2 = ((grad[id2][jd4] + grad[ia6][ja4] + grad[ia6][jd4] + grad[id2][ja4]) >> 2);
                                    ginterp = around + ((MIX_coeff*(grad[ia2][j] - around_2)) >> 8);

                                    //减法做法
                                    dff_GE = ((Alhfa * (ginterp - grad[ia2][j])) >> 8);
                                    dff_GE = (dff_GE >> 2) + ((dff_GE & 0x00000002) >> 1);
                                    in[ia2][j] = dff_GE + in[ia2][j];
                                    in[ia3][j] = dff_GE + in[ia3][j];
                                    in[ia2][ja1] = dff_GE + in[ia2][ja1];
                                    in[ia3][ja1] = dff_GE + in[ia3][ja1];

                                    //除法做法
                                    //gain_GE = (float)(((256 - Alhfa) * grad[ia2][j] + Alhfa * ginterp) >> 8) / grad[ia2][j];
                                    //in[ia2][j] = gain_GE * in[ia2][j];
                                    //in[ia3][j] = gain_GE * in[ia3][j];
                                    //in[ia2][ja1] = gain_GE * in[ia2][ja1];
                                    //in[ia3][ja1] = gain_GE * in[ia3][ja1];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void AlgorithmFuncs::bbb23(short **in, int cfa, int offset_x, int offset_y, int width, int height, int r_gain, int gr_gain, int gb_gain, int b_gain)
{
    int i, j, ia1, ja1;
    int gain11,gain12,gain21,gain22;
    int r_gain_use,gr_gain_use,gb_gain_use,b_gain_use;
    r_gain_use=16777216/r_gain;
    gr_gain_use=16777216/gr_gain;
    gb_gain_use = 16777216 /gb_gain;
    b_gain_use=16777216/b_gain;
    if(cfa==0){
        gain11=gr_gain_use; gain12=r_gain_use;
        gain21=b_gain_use; gain22=gb_gain_use;
    }else if(cfa==1){
        gain11=r_gain_use; gain12=gr_gain_use;
        gain21=gb_gain_use; gain22=b_gain_use;
    }else if(cfa==2){
        gain11=b_gain_use; gain12=gb_gain_use;
        gain21=gr_gain_use; gain22=r_gain_use;
    }else{
        gain11=gb_gain_use; gain12=b_gain_use;
        gain21=r_gain_use; gain22=gr_gain_use;
    }
    for (i = offset_y;i<height - 16;i += 2) {
        ia1 = i + 1;
        for (j = offset_x;j<width - 16;j += 2) {
            ja1 = j + 1;
            in[i][j] = (short)((in[i][j]*gain11)>>16);
            in[i][ja1] = (short)((in[i][ja1]*gain12)>>16);
            in[ia1][j] = (short)((in[ia1][j]*gain21)>>16);
            in[ia1][ja1] = (short)((in[ia1][ja1]*gain22)>>16);
        }
    }
}

void AlgorithmFuncs::bbb24(paramThread *para)
{
    //char fn[30];
    int threadID = para->ID;
    unsigned short *in_raw16 = para->in_raw16;
    unsigned short *out_raw16 = para->out_raw16;
    unsigned char** crosstalk_gain = para->crosstalk_gain;
    int col_start = para->col_start;
    int width = para->w;
    int height = para->h;
    int flat_th = para->flat_th;
    int cfa = para->cfa;
    int r_gain = para->r_gain;
    int gr_gain = para->gr_gain;
    int gb_gain = para->gb_gain;
    int b_gain = para->b_gain;
    short black_level = para->black_level;
    int xtalk_static_en = para->xtalk_static_en;
    int xtalk_dynamic_en = para->xtalk_dynamic_en;

    int xtalk_ge_th=para->xtalk_ratio_r_h;
    int xtalk_MIX_coeff=para->xtalk_ratio_r_v;
    int xtalk_ge_slope=para->xtalk_ratio_b_h;
    int xtalk_ge_str=para->xtalk_ratio_b_v;

    int cdiff_grad_level = para->diff_th;

    int xtalk_en = para->xtalk_en;
    int dpc_en=para->dpc_en;

    int fc_en = para->fc_en;
    int fc_level = para->fc_level;

    int i;
    int offx1, offx2, offx3, offx4;
    int offy1, offy2, offy3, offy4;
    //short raw16[height][width], grad[height][width], out16[height][width];
    short *raw16[BLOCK_H], *grad[BLOCK_H], *out16[BLOCK_H];
    short *raw16_0, *grad_0, *out16_0;
    //short **raw16,**grad,**out16;
    raw16_0 = (short *)malloc(sizeof(short*)*height*width);
    grad_0 = (short *)malloc(sizeof(short*)*height*width);
    out16_0 = (short *)malloc(sizeof(short*)*height*width);

    raw16[0] = raw16_0;
    grad[0] = grad_0;
    out16[0] = out16_0;
    for (i = 1; i < height; i++) {
        raw16[i] = raw16[i - 1] + width;
        grad[i] = grad[i - 1] + width;
        out16[i] = out16[i - 1] + width;
    }

    if (cfa == 0 || cfa == 3) {
        offx1 = 4; offy1 = 2;
        offx2 = 8; offy2 = 6;
        offx3 = 12; offy3 = 12;
        offx4 = 16; offy4 = 16;
    }
    else {
        offx1 = 2; offy1 = 2;
        offx2 = 6; offy2 = 6;
        offx3 = 12; offy3 = 12;
        offx4 = 16; offy4 = 16;
    }
    AlgorithmFuncs::bbb11(in_raw16, raw16, col_start, width, FULLSIZE_W, FULLSIZE_H, black_level, threadID);
    if (dpc_en) bbb10(raw16, cfa, height, width);
    AlgorithmFuncs::bbb19(raw16, cfa, width, height, r_gain, gr_gain, gb_gain, b_gain);
    if (xtalk_en) bbb22(raw16, grad, crosstalk_gain, cfa, width, height, xtalk_static_en, xtalk_dynamic_en, xtalk_ge_th, xtalk_MIX_coeff, xtalk_ge_slope, xtalk_ge_str);
    /*FILE *fp1 = fopen("E:\\matlab\\4cell\\1009_code_500ms\\xtalk_out.raw", "wb");
    if (fp1) {
        for (i = 0;i < height;i++)
            fwrite(raw16[i], width, 2, fp1);
        fclose(fp1);
    }*/

    AlgorithmFuncs::bbb18(raw16, grad, width, height);
    AlgorithmFuncs::bbb13(raw16, grad, offx1, offy1, width, height, flat_th, cdiff_grad_level);
    AlgorithmFuncs::bbb14(raw16, grad, offx2, offy2, width, height);
    AlgorithmFuncs::bbb15(raw16, grad, out16, offx3, offy3, width, height, cfa, fc_en, fc_level);
    AlgorithmFuncs::bbb23(out16, cfa, offx4, offy4, width, height, r_gain, gr_gain, gb_gain, b_gain);
    AlgorithmFuncs::bbb12(out16, out_raw16, width, height, col_start, FULLSIZE_W, FULLSIZE_H, black_level);

    free(grad_0);
    free(raw16_0);
    free(out16_0);
}

void * AlgorithmFuncs::bbb6(void *args)
{
    //pthread_t threadID = pthread_self();
    paramThread *para = (paramThread *)args;
    AlgorithmFuncs::bbb24(para);
    pthread_exit(NULL);
    return NULL;
}


#define data(i,j) (*(data+(j)+cols*(i)))
#define dat(i,j) (*(dat+(j)+cols*(i)))

void AlgorithmFuncs::aaa1(double **dat, double *d, int count)
{
    int i;
    free(d);
    for (i = 0; i<count; i++)
        free(*(dat + i));
    free(dat);
}
int AlgorithmFuncs::aaa2(double *data, int count, double *Answer)
{
    int j, m, n;
    double tmp, **dat, *d = data;
    dat = (double**)malloc(count * sizeof(double*));
    for (m = 0; m<count; m++, d += (count + 1))
    {
        dat[m] = (double*)malloc((count + 1) * sizeof(double));
        memcpy(dat[m], d, (count + 1) * sizeof(double));
    }
    d = (double*)malloc((count + 1) * sizeof(double));
    for (m = 0; m<count - 1; m++)
    {
        //如果主对角线元素为0，行交换
        for (n = m + 1; n<count&&dat[m][m] == 0.0; n++)
        {
            if (dat[n][m] != 0.0)
            {
                memcpy(d, dat[m], (count + 1) * sizeof(double));
                memcpy(dat[m], dat[n], (count + 1) * sizeof(double));
                memcpy(dat[n], d, (count + 1) * sizeof(double));
            }
        }
        //行交换后，主对角线元素仍然为0，无解，返回-1
        if (dat[m][m] == 0.0)
        {
            aaa1(dat, d, count);
            return-1;
        }
        //消元
        for (n = m + 1; n<count; n++)
        {
            tmp = dat[n][m] / dat[m][m];
            for (j = m; j <= count; j++)
                dat[n][j] -= tmp*dat[m][j];
        }
    }
    for (j = 0; j<count; j++)
        d[j] = 0.0;
    //求得count-1的元
    Answer[count - 1] = dat[count - 1][count] / dat[count - 1][count - 1];
    //逐行代入求各元
    for (m = count - 2; m >= 0; m--)
    {
        for (j = count - 1; j>m; j--)
            d[m] += Answer[j] * dat[m][j];
        Answer[m] = (dat[m][count] - d[m]) / dat[m][m];
    }
    aaa1(dat, d, count);
    return 0;
}
int AlgorithmFuncs::aaa3(double*data, int rows, int cols, int med_w, int xy_density, double*Answer)
{
    int m, n, i, count = cols - 1;
    double *dat, *p, a, b;
    if (data == 0 || Answer == 0 || rows<2 || cols<2)
        return -1;
    b = count*cols * sizeof(double);
    dat = (double*)malloc(b);
    memset(dat, 0.0, b);
    for (m = 0; m<count; m++)//n=0tocols-2
    {
        for (n = m; n<count; n++)//i=n+1tocols-2
        {
            if ((abs(n%med_w - m%med_w)<3) && (abs(n / med_w - m / med_w)<3))
            {
                for (a = 0, i = ((n + 1)>med_w * 2 + 3) ? ((n + 1 - (med_w * 2 + 3)) - (n + 1 - (med_w * 2 + 3)) / (med_w - 2) * 2)*xy_density : 0; i < rows && i<(m + 1 - (m + 1) / med_w * 2)*xy_density; i++)
                {
                    a += (data(i, m)*data(i, n));
                }
                dat(m, n) = a; //dat[n+1,i+1]=Sum(Xn*Xi)
                dat(n, m) = a; //dat[i+1,n+1]=Sum(Xn*Xi)
            }
        }
    }

    for (n = 0; n<count; n++)
    {
        for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
        {
            a += (p[n] * p[count]);
        }
        dat[(n + 1)*cols - 1] = a;//dat[n+1,cols]=Sum(Xn*Y)
    }
    n = aaa2(dat, cols - 1, Answer);
    free(dat);
    return 0;
}
double ***AlgorithmFuncs::aaa4(int height, int width, int num, int *malloc_flag)
{
    double ***matrix = NULL;
    double **pp = NULL;
    double *p = NULL;
    if ((height > 0) && (width > 0) && (num > 0))
    {
        matrix = (double***)malloc(height * sizeof(double**));
        pp = (double**)malloc(height * width * sizeof(double*));
        p = (double*)malloc(height * width * num * sizeof(double));
        if ((matrix != NULL) && (pp != NULL) && (p != NULL)) {
            for (int i = 0; i < height; i++)
            {
                matrix[i] = pp + i * width;
                for (int j = 0; j < width; j++)
                {
                    matrix[i][j] = p + j * num;
                    for (int k = 0; k < num; k++)
                        matrix[i][j][k] = 0;
                }
                p = p + width * num;
            }
        }
        else
            *malloc_flag = 1;
    }
    else
        *malloc_flag = 1;
    return matrix;
}
double **AlgorithmFuncs::aaa5(int height, int width, int *malloc_flag)
{
    double **matrix = NULL;
    double *p = NULL;
    if ((height > 0) && (width > 0))
    {
        matrix = (double**)malloc(height * sizeof(double*));
        p = (double*)malloc(height * width * sizeof(double));
        if ((matrix != NULL) && (p != NULL)) {
            for (int i = 0; i < height; i++)
            {
                matrix[i] = p + i * width;
                for (int j = 0; j < width; j++)
                    matrix[i][j] = 0;
            }
        }
        else
        {
            logMsg (std::string("51!"));
            *malloc_flag = 1;
        }
    }
    else
    {
        logMsg (std::string("52!"));
        *malloc_flag = 1;
    }
    return matrix;
}
int AlgorithmFuncs::aaa6(int *a)
{
    int middle=0;
    int temp;
    int b[3];
    for (int i = 0; i < 3; i++)
    {
        if (*(a + i * 3) > *(a + i * 3 + 1))
        {
            if (*(a + i * 3) > *(a + i * 3 + 2))
            {
                if (*(a + i * 3 + 2) > *(a + i * 3 + 1))
                    b[i] = *(a + i * 3 + 2);
                else
                    b[i] = *(a + i * 3 + 1);
            }
            else
                b[i] = *(a + i * 3);
        }
        else
        {
            if (*(a + i * 3) < *(a + i * 3 + 2))
            {
                if (*(a + i * 3 + 2) > *(a + i * 3 + 1))
                    b[i] = *(a + i * 3 + 1);
                else
                    b[i] = *(a + i * 3 + 2);
            }
            else
                b[i] = *(a + i * 3);
        }
    }
    if (b[0] > b[1])
    {
        if (b[0] > b[2])
        {
            if (b[2] > b[1])
                middle = b[2];
            else
                middle = b[1];
        }
        else
            middle = b[0];
    }
    else
    {
        if (b[0] < b[2])
        {
            if (b[2] > b[1])
                middle = b[1];
            else
                middle = b[2];
        }
        else
            middle = b[0];
    }
    return middle;
}
double **AlgorithmFuncs::aaa7(int *standard_pic, int height, int width, int *malloc_flag)
{
    double **middle_pic = NULL;
    middle_pic = aaa5(height, width, malloc_flag);
    if (*malloc_flag == 1)
        return middle_pic;
    int mean_sequence[9];

    //create the 3-D map,the final D means the pixel around (8+1).
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                int position_x = (k / 6) * 4;
                int position_y = ((k % 3) == 2) * 4;
                *(mean_sequence + k) = *(standard_pic + (i + position_x) * width + (j + position_y));
            }
            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
        for (int j = 4; j < width - 4; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                int position_x = (k / 6) * 4;
                int position_y = (k % 3 - 1) * 4;
                *(mean_sequence + k) = *(standard_pic + (i + position_x) * width + (j + position_y));
            }
            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
        for (int j = width - 4; j < width; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                int position_x = (k / 6) * 4;
                int position_y = ((k % 3) == 0) * (-4);
                *(mean_sequence + k) = *(standard_pic + (i + position_x) * width + (j + position_y));
            }
            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
    }
    for (int i = 4; i < height - 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                int position_x = (k / 3 - 1) * 4;
                int position_y = ((k % 3) == 2) * 4;
                *(mean_sequence + k) = *(standard_pic + (i + position_x) * width + (j + position_y));
            }
            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
        for (int j = 4; j < width - 4; j++)
        {
            *(mean_sequence + 0) = *(standard_pic + (i - 4)*width + (j - 4));
            *(mean_sequence + 1) = *(standard_pic + (i - 4)*width + j);
            *(mean_sequence + 2) = *(standard_pic + (i - 4)*width + (j + 4));
            *(mean_sequence + 3) = *(standard_pic + i*width + (j - 4));
            *(mean_sequence + 4) = *(standard_pic + i*width + j);
            *(mean_sequence + 5) = *(standard_pic + i*width + (j + 4));
            *(mean_sequence + 6) = *(standard_pic + (i + 4)*width + (j - 4));
            *(mean_sequence + 7) = *(standard_pic + (i + 4)*width + j);
            *(mean_sequence + 8) = *(standard_pic + (i + 4)*width + (j + 4));

            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
        for (int j = width - 4; j < width; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                int position_x = (k / 3 - 1) * 4;
                int position_y = ((k % 3) == 0) * (-4);
                *(mean_sequence + k) = *(standard_pic + (i + position_x) * width + (j + position_y));
            }
            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
    }
    for (int i = height - 4; i < height; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                int position_x = ((k > 2) - 1) * 4;
                int position_y = ((k % 3) == 2) * 4;
                *(mean_sequence + k) = *(standard_pic + (i + position_x) * width + (j + position_y));
            }
            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
        for (int j = 4; j < width - 4; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                int position_x = ((k > 2) - 1) * 4;
                int position_y = (k % 3 - 1) * 4;
                *(mean_sequence + k) = *(standard_pic + (i + position_x) * width + (j + position_y));
            }
            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
        for (int j = width - 4; j < width; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                int position_x = ((k > 2) - 1) * 4;
                int position_y = ((k % 3) == 0) * (-4);
                *(mean_sequence + k) = *(standard_pic + (i + position_x) * width + (j + position_y));
            }
            *(*(middle_pic + i) + j) = aaa6(mean_sequence);
        }
    }

    free(standard_pic);
    return middle_pic;
}
double **AlgorithmFuncs::aaa8(double **middle_pic, int height, int width, int size_of_mean_window, int *malloc_flag, std::string &msg)
{
    double **final_pic = NULL;
    final_pic = aaa5(height, width, malloc_flag);
    if (*malloc_flag == 1)
    {
        msg+=boost::str(boost::format("Mark 81\n"));
        return final_pic;
    }

    int point_start = 2 * (size_of_mean_window - 1);
    int point_final_h = 2 * point_start + height;
    int point_middle_h = point_start + height;
    int point_reg_h = height - point_start;
    int point_final_w = 2 * point_start + width;
    int point_middle_w = point_start + width;
    int point_reg_w = width - point_start;
    //get full pic, where full pic means original pic + pixel around.
    double **full_pic = NULL;
    full_pic = aaa5(height + 2 * point_start, width + 2 * point_start, malloc_flag);
    if (*malloc_flag == 1)
    {
        msg+=boost::str(boost::format("Mark 82\n"));
        return final_pic;
    }
    for (int i = 0; i < point_start; i++)
    {
        for (int j = 0; j < point_start; j++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i) + j);
        for (int j = point_start, j_reg = 0; j < point_middle_w; j++, j_reg++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i) + j_reg);
        for (int j = point_middle_w, j_reg = point_reg_w; j < point_final_w; j++, j_reg++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i) + j_reg);
    }
    for (int i = point_start, i_reg = 0; i < point_middle_h; i++, i_reg++)
    {
        for (int j = 0; j < point_start; j++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i_reg) + j);
        for (int j = point_start, j_reg = 0; j < point_middle_w; j++, j_reg++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i_reg) + j_reg);
        for (int j = point_middle_w, j_reg = point_reg_w; j < point_final_w; j++, j_reg++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i_reg) + j_reg);
    }
    for (int i = point_middle_h, i_reg = point_reg_h; i < point_final_h; i++, i_reg++)
    {
        for (int j = 0; j < point_start; j++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i_reg) + j);
        for (int j = point_start, j_reg = 0; j < point_middle_w; j++, j_reg++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i_reg) + j_reg);
        for (int j = point_middle_w, j_reg = point_reg_w; j < point_final_w; j++, j_reg++)
            *(*(full_pic + i) + j) = *(*(middle_pic + i_reg) + j_reg);
    }

    msg+=boost::str(boost::format("Mark 83\n"));

    //get mean pic
    int h_quarter = height >> 2;
    int w_quarter = width >> 2;
    int number_of_point = size_of_mean_window*size_of_mean_window;
    int window_start = (1 - size_of_mean_window) / 2;
    int window_final = (size_of_mean_window - 1) / 2 + 1;
    for (int i = point_start, i_reg = 0; i < point_middle_h; i++, i_reg++)
    {
        for (int j = point_start, j_reg = 0; j < point_middle_w; j++, j_reg++)
        {
            double sum = 0;
            for (int k = window_start; k < window_final; k++)
                for (int l = window_start; l < window_final; l++)
                    sum += *(*(full_pic + i + 4 * k) + j + 4 * l);
            *(*(final_pic + i_reg) + j_reg) = sum / number_of_point;
        }
    }

    msg+=boost::str(boost::format("Mark 84\n"));
    free(middle_pic[0]);
    free(middle_pic);
    free(full_pic[0]);
    free(full_pic);
    return final_pic;
}
double ***AlgorithmFuncs::aaa9(double **final_pic, int height, int width, char *pattern, int *malloc_flag,std::string &msg)
{
    int h_quarter = height / 4;
    int w_quarter = width / 4;
    double ***gain_map_original = NULL;
    gain_map_original = aaa4(h_quarter, w_quarter, 16, malloc_flag);
    if (*malloc_flag == 1)
    {
        msg+=boost::str(boost::format("Mark 91\n"));
        return gain_map_original;
    }
//calculate the hole max map (mean in 2x2)
    int h_half = height / 2;
    int w_half = width / 2;
    double **maxmap_reg = NULL;
    maxmap_reg = aaa5(h_half, w_half, malloc_flag);
    if (*malloc_flag == 1)
    {
        msg+=boost::str(boost::format("Mark 92\n"));
        return gain_map_original;
    }
    for (int i = 0; i < h_half; i++)
        for (int j = 0; j < w_half; j++)
        {
            double max_in_2x2 = *(*(final_pic + 2 * i) + 2 * j);
            if (*(*(final_pic + 2 * i) + 2 * j + 1) > max_in_2x2)
                max_in_2x2 = *(*(final_pic + 2 * i) + 2 * j + 1);
            if (*(*(final_pic + 2 * i + 1) + 2 * j) > max_in_2x2)
                max_in_2x2 = *(*(final_pic + 2 * i + 1) + 2 * j);
            if (*(*(final_pic + 2 * i + 1) + 2 * j + 1) > max_in_2x2)
                max_in_2x2 = *(*(final_pic + 2 * i + 1) + 2 * j + 1);
            *(*(maxmap_reg + i) + j) = max_in_2x2;
        }
//find 9x9 max
    double **maxmap = NULL;
    maxmap = aaa5(h_half, w_half, malloc_flag);
    if (*malloc_flag == 1)
    {
        msg+=boost::str(boost::format("Mark 93\n"));
        return gain_map_original;
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j + 2);
            if (*(*(maxmap_reg + i + 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j);
            if (*(*(maxmap_reg + i + 2) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j + 2);
            *(*(maxmap + i) + j) = max_in_9x9;
        }
        for (int j = 2; j < w_half - 2; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j - 2);
            if (*(*(maxmap_reg + i) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j + 2);
            if (*(*(maxmap_reg + i + 2) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j - 2);
            if (*(*(maxmap_reg + i + 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j);
            if (*(*(maxmap_reg + i + 2) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j + 2);
            *(*(maxmap + i) + j) = max_in_9x9;
        }
        for (int j = w_half - 2; j < w_half; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j - 2);
            if (*(*(maxmap_reg + i + 2) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j - 2);
            if (*(*(maxmap_reg + i + 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j);
            *(*(maxmap + i) + j) = max_in_9x9;
        }
    }
    msg+=boost::str(boost::format("Mark 94\n"));
    for (int i = 2; i < h_half - 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i - 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j);
            if (*(*(maxmap_reg + i - 2) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j + 2);
            if (*(*(maxmap_reg + i) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j + 2);
            if (*(*(maxmap_reg + i + 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j);
            if (*(*(maxmap_reg + i + 2) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j + 2);
            *(*(maxmap + i) + j) = max_in_9x9;
        }
        for (int j = 2; j < w_half - 2; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i - 2) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j - 2);
            if (*(*(maxmap_reg + i - 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j);
            if (*(*(maxmap_reg + i - 2) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j + 2);

            if (*(*(maxmap_reg + i) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j - 2);
            if (*(*(maxmap_reg + i) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j + 2);

            if (*(*(maxmap_reg + i + 2) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j - 2);
            if (*(*(maxmap_reg + i + 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j);
            if (*(*(maxmap_reg + i + 2) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j + 2);

            *(*(maxmap + i) + j) = max_in_9x9;
        }
        for (int j = w_half - 2; j < w_half; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i - 2) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j - 2);
            if (*(*(maxmap_reg + i - 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j);
            if (*(*(maxmap_reg + i) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j - 2);
            if (*(*(maxmap_reg + i + 2) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j - 2);
            if (*(*(maxmap_reg + i + 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i + 2) + j);
            *(*(maxmap + i) + j) = max_in_9x9;
        }
    }

    msg+=boost::str(boost::format("Mark 95\n"));

    for (int i = h_half - 2; i < h_half; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i - 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j);
            if (*(*(maxmap_reg + i - 2) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j + 2);
            if (*(*(maxmap_reg + i) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j + 2);
            *(*(maxmap + i) + j) = max_in_9x9;

        }
        for (int j = 2; j < w_half - 2; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i - 2) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j - 2);
            if (*(*(maxmap_reg + i - 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j);
            if (*(*(maxmap_reg + i - 2) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j + 2);
            if (*(*(maxmap_reg + i) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j - 2);
            if (*(*(maxmap_reg + i) + j + 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j + 2);
            *(*(maxmap + i) + j) = max_in_9x9;
        }
        for (int j = w_half - 2; j < w_half; j++)
        {
            double max_in_9x9 = *(*(maxmap_reg + i) + j);
            if (*(*(maxmap_reg + i - 2) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j - 2);
            if (*(*(maxmap_reg + i - 2) + j) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i - 2) + j);
            if (*(*(maxmap_reg + i) + j - 2) > max_in_9x9)
                max_in_9x9 = *(*(maxmap_reg + i) + j - 2);
            *(*(maxmap + i) + j) = max_in_9x9;
        }
    }
    msg+=boost::str(boost::format("Mark 96\n"));

    free(maxmap_reg[0]);
    free(maxmap_reg);
//correct the max map (make Gmax the same)
    int mode = 0;
    if ((*pattern == 'r') || (*pattern == 'R'))
        mode = 1;
    else if (((*pattern == 'g') || (*pattern == 'G')) && ((*(pattern + 1) == 'r') || (*(pattern + 1) == 'R')))
        mode = 2;
    else if (((*pattern == 'g') || (*pattern == 'G')) && ((*(pattern + 1) == 'b') || (*(pattern + 1) == 'B')))
        mode = 3;
    else if ((*pattern == 'b') || (*pattern == 'B'))
        mode = 4;
    if ((mode == 1) || (mode == 4))
        for (int i = 0; i < h_quarter; i++)
            for (int j = 0; j < w_quarter; j++)
                if (*(*(maxmap + 2 * i) + 2 * j + 1) > *(*(maxmap + 2 * i + 1) + 2 * j))
                    *(*(maxmap + 2 * i + 1) + 2 * j) = *(*(maxmap + 2 * i) + 2 * j + 1);
                else
                    *(*(maxmap + 2 * i) + 2 * j + 1) = *(*(maxmap + 2 * i + 1) + 2 * j);
    else if ((mode == 2) || (mode == 3))
        for (int i = 0; i < h_quarter; i++)
            for (int j = 0; j < w_quarter; j++)
                if (*(*(maxmap + 2 * i) + 2 * j) > *(*(maxmap + 2 * i + 1) + 2 * j + 1))
                    *(*(maxmap + 2 * i + 1) + 2 * j + 1) = *(*(maxmap + 2 * i) + 2 * j);
                else
                    *(*(maxmap + 2 * i) + 2 * j) = *(*(maxmap + 2 * i + 1) + 2 * j + 1);
//calculate the original gain map (noisy)
    for (int i = 0; i < h_quarter; i++)
        for (int j = 0; j < w_quarter; j++)
            for (int m = 0; m < 16; m++)
            {
                int map_position_x = m / 8;
                int map_position_y = (m % 4) / 2;
                int mean_position_x = m / 4;
                int mean_position_y = m % 4;
                *(*(*(gain_map_original + i) + j) + m) = ((double) *(*(maxmap + 2 * i + map_position_x) + 2 * j + map_position_y))/ (double) *(*(final_pic + 4 * i + mean_position_x) + 4 * j + mean_position_y);
            }


    msg+=boost::str(boost::format("Mark 97\n"));

    free(maxmap[0]);
    free(maxmap);
    free(final_pic[0]);
    free(final_pic);
    return gain_map_original;
}
void AlgorithmFuncs::aaa10(double *in_map_flow, double *out_map_flow, int num_one_time, int interval, double *b, double B, int *malloc_flag)
{
    double *w1 = NULL;
    double *w2 = NULL;
    /* forward pass */
    w1 = (double *)malloc((num_one_time + 3) * sizeof(double));
    if (w1 == NULL)
    {
        *malloc_flag = 1;
        return ;
    }
    w2 = (double *)malloc((num_one_time + 3) * sizeof(double));
    if (w2 == NULL)
    {
        *malloc_flag = 1;
        return ;
    }
    *w1= *in_map_flow;
    *(w1 + 1) = *in_map_flow;
    *(w1 + 2) = *in_map_flow;
    for (int i = 0; i < num_one_time; i++)
        *(w1 + i + 3) = B * *(in_map_flow + i * interval) + (*(b + 1) * *(w1 + i + 2) + *(b + 2) * *(w1 + i + 1) + *(b + 3) * *(w1 + i))/ *b;
    /* backward pass */
    *(w2 + num_one_time) = *(w1 + num_one_time + 2);
    *(w2 + num_one_time + 1) = *(w1 + num_one_time + 2);
    *(w2 + num_one_time + 2) = *(w1 + num_one_time + 2);
    for (int i = num_one_time - 1; i >= 0; i--)
    {
        *(w2 + i) = B * *(w1 + i) + (*(b + 1) * *(w2 + i + 1) + *(b + 2) * *(w2 + i + 2) + *(b + 3) * *(w2 + i + 3))/ *b;
        *(out_map_flow + i * interval) = *(w2 + i);
    }
    free(w1);
    free(w2);
}
double ***AlgorithmFuncs::aaa11(double ***gain_map, int height, int width, int *malloc_flag)
{

    double ***low_pass_map = NULL;
    low_pass_map = aaa4(height, width, 16, malloc_flag);
    if (*malloc_flag == 1)
        return low_pass_map;
    //get gaussian core
    int sigma = 3;
    //the code in if is the original gaussian filter

    //gaussian parameter
    double q = 0.98711 * sigma - 0.96330;
    double qq = q * q;
    double qqq = qq * q;
    double *b;
    b = (double*)malloc(4 * sizeof(double));
    if (b == NULL)
    {
        *malloc_flag = 1;
        return low_pass_map;
    }
    *b = 1.57825 + 2.44413 * q + 1.4281 * qq + 0.422205 * qqq;
    *(b + 1) = 2.44413 * q + 2.85619 * qq + 1.26661 * qqq;
    *(b + 2) = -(1.4281 * qq + 1.26661 * qqq);
    *(b + 3) = 0.422205 * qqq;
    double B = 1 - (*(b + 1) + *(b + 2) + *(b + 3))/ *b;
    //filtering
    for (int k = 0; k < 16; k++)
    {
        double *in_map_flow = NULL;
        in_map_flow = (double*)malloc(height * width * sizeof(double));
        if (in_map_flow == NULL)
        {
                *malloc_flag = 1;
                return low_pass_map;
        }
        double *out_map_flow = NULL;
        out_map_flow = (double*)malloc(height * width * sizeof(double));
        if (out_map_flow == NULL)
        {
                *malloc_flag = 1;
                return low_pass_map;
        }

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                *(in_map_flow + i * width + j) = *(*(*(gain_map + i) + j) + k);
        for (int i = 0; i < height; i++)
            aaa10(in_map_flow + i * width, out_map_flow + i * width, width, 1, b, B, malloc_flag);
        free(in_map_flow);

        for (int j = 0; j < width; j++)
            aaa10(out_map_flow + j, out_map_flow + j, height, width, b, B, malloc_flag);
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                *(*(*(low_pass_map + i) + j) + k) = *(out_map_flow + i * width + j);
        free(out_map_flow);
    }

    free(b);
    free(gain_map[0][0]);
    free(gain_map[0]);
    free(gain_map);
    return low_pass_map;
}
double ***AlgorithmFuncs::aaa12(double ***gain_map, int height, int width, int point_height, int point_width, int number_of_point_in_one_area, int *malloc_flag)
{
    int samplerate_height = (point_height - 2) * (number_of_point_in_one_area - 1) + 1;
    int samplerate_width = (point_width - 2) * (number_of_point_in_one_area - 1) + 1;
    double ***downsampled_map = NULL;
    downsampled_map = aaa4(samplerate_height, samplerate_width, 16, malloc_flag);
    if (*malloc_flag == 1)
        return downsampled_map;
    int delta_x = (height - samplerate_height) / (samplerate_height - 1);
    int delta_y = (width - samplerate_width) / (samplerate_width - 1);
    for (int i = 0; i < samplerate_height - 1; i++)
        for (int j = 0; j < samplerate_width - 1; j++)
            for (int k = 0; k < 16; k++)
                *(*(*(downsampled_map + i) + j) + k) = *(*(*(gain_map + (delta_x + 1) * i) + (delta_y + 1) * j) + k);
    for (int i = 0; i < samplerate_height; i++)
        for (int k = 0; k < 16; k++)
            *(*(*(downsampled_map + i) + samplerate_width - 1) + k) = *(*(*(gain_map + (delta_x + 1) * i) + width - 1) + k);
    for (int j = 0; j < samplerate_width; j++)
        for (int k = 0; k < 16; k++)
            *(*(*(downsampled_map + samplerate_height - 1) + j) + k) = *(*(*(gain_map + height - 1) + (delta_y + 1) * j) + k);

    free(gain_map[0][0]);
    free(gain_map[0]);
    free(gain_map);
    return downsampled_map;
}
double ***AlgorithmFuncs::aaa13(double ***downsampled_map, int point_height, int point_width, int number_of_point_in_one_area, int *malloc_flag)
{
    double ***bezier_point_map = NULL;
    bezier_point_map = aaa4(point_height, point_width, 16, malloc_flag);
    if (*malloc_flag == 1)
        return downsampled_map;
    double x, y;
    double x2, y2;
    double *p11=NULL;
    double *p12 = NULL;
    double *p13 = NULL;
    double *p21 = NULL;
    double *p22 = NULL;
    double *p23 = NULL;
    double *p31 = NULL;
    double *p32 = NULL;
    double *p33 = NULL;
    p11 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p11 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }
    p12 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p12 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }
    p13 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p13 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }
    p21 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p21 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }
    p22 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p22 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }
    p23 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p23 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }
    p31 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p31 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }
    p32 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p32 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }
    p33 = (double*)malloc(number_of_point_in_one_area*number_of_point_in_one_area * sizeof(double));
    if (p33 == NULL)
    {
        *malloc_flag = 1;
        return bezier_point_map;
    }

    int height_in_Xarray = number_of_point_in_one_area*number_of_point_in_one_area*(point_height - 2)*(point_width - 2);
    int width_in_Xarray = point_height*point_width + 1;
    double **Xarray_reg = NULL;
    Xarray_reg = aaa5(height_in_Xarray,width_in_Xarray, malloc_flag);
    if (*malloc_flag == 1)
        return bezier_point_map;


    for (int i = 0; i < number_of_point_in_one_area; i++)
    {
        x = ((double)i) / (number_of_point_in_one_area - 1);
        x2 = x*x;
        for (int j = 0; j < number_of_point_in_one_area; j++)
        {
            y = ((double)j) / (number_of_point_in_one_area - 1);
            y2 = y*y;
            *(p11 + i*number_of_point_in_one_area + j) = (y2*(x2 *0.25 - x *0.5 + 0.25) - y*(x2 *0.5 - x + 0.5) - x *0.5 + x2 *0.25 + 0.25);
            *(p12 + i*number_of_point_in_one_area + j) = (y*(x2 *0.5 - x + 0.5) - x *0.5 - y2*(x2 *0.5 - x + 0.5) + x2 *0.25 + 0.25);
            *(p13 + i*number_of_point_in_one_area + j) = (y2*(x2 *0.25 - x *0.5 + 0.25));
            *(p21 + i*number_of_point_in_one_area + j) = (x *0.5 + y2*(-x2 *0.5 + x *0.5 + 0.25) - x2 *0.5 - y*(-x2 + x + 0.5) + 0.25);
            *(p22 + i*number_of_point_in_one_area + j) = (x *0.5 - y2*(-x2 + x + 0.5) - x2 *0.5 + y*(-x2 + x + 0.5) + 0.25);
            *(p23 + i*number_of_point_in_one_area + j) = (y2*(-x2 *0.5 + x *0.5 + 0.25));
            *(p31 + i*number_of_point_in_one_area + j) = ((x2*y2) *0.25 - (x2*y) *0.5 + x2 *0.25);
            *(p32 + i*number_of_point_in_one_area + j) = ((x2*y) *0.5 - (x2*y2) *0.5 + x2 *0.25);
            *(p33 + i*number_of_point_in_one_area + j) = ((x2*y2) *0.25);
        }
    }

    for (int i = 0; i<point_height - 2; i++)
        for (int j = 0; j<point_width - 2; j++)
            for (int k = 0; k<number_of_point_in_one_area; k++)
                for (int l = 0; l < number_of_point_in_one_area; l++)
                {
                    int position;//row position in Xarray_reg
                    position = number_of_point_in_one_area*number_of_point_in_one_area*(point_width - 2)*i + number_of_point_in_one_area*number_of_point_in_one_area*j + number_of_point_in_one_area*k + l;
                    for (int m = 0; m < width_in_Xarray; m++)
                        *(*(Xarray_reg + position) + m) = 0;
                    *(*(Xarray_reg + position) + i*point_width + j)= *(p11 + k*number_of_point_in_one_area + l);
                    *(*(Xarray_reg + position) + i*point_width + j + 1) = *(p12 + k*number_of_point_in_one_area + l);
                    *(*(Xarray_reg + position) + i*point_width + j + 2) = *(p13 + k*number_of_point_in_one_area + l);
                    *(*(Xarray_reg + position) + (i + 1)*point_width + j) = *(p21 + k*number_of_point_in_one_area + l);
                    *(*(Xarray_reg + position) + (i + 1)*point_width + j + 1) = *(p22 + k*number_of_point_in_one_area + l);
                    *(*(Xarray_reg + position) + (i + 1)*point_width + j + 2) = *(p23 + k*number_of_point_in_one_area + l);
                    *(*(Xarray_reg + position) + (i + 2)*point_width + j) = *(p31 + k*number_of_point_in_one_area + l);
                    *(*(Xarray_reg + position) + (i + 2)*point_width + j + 1) = *(p32 + k*number_of_point_in_one_area + l);
                    *(*(Xarray_reg + position) + (i + 2)*point_width + j + 2) = *(p33 + k*number_of_point_in_one_area + l);
                }

    free(p11); free(p12); free(p13); free(p21); free(p22); free(p23); free(p31); free(p32); free(p33);
    double *Answer = NULL;
    Answer = (double*)malloc(point_height*point_width * sizeof(double));
    if (*malloc_flag == 1)
        return bezier_point_map;

    double *Xarray_trans = NULL;
    Xarray_trans = (double*)malloc(height_in_Xarray*width_in_Xarray * sizeof(double));
    if (*malloc_flag == 1)
        return bezier_point_map;

    for (int m = 0; m < 16; m++)
    {
        //合成数据矩阵
        memset(Answer, 0.0, point_height*point_width);
        for (int i = 0; i<point_height - 2; i++)
            for (int j = 0; j<point_width - 2; j++)
                for (int k = 0; k<number_of_point_in_one_area; k++)
                    for (int l = 0; l < number_of_point_in_one_area; l++)
                    {
                        int position;
                        position = number_of_point_in_one_area*number_of_point_in_one_area*(point_width - 2)*i + number_of_point_in_one_area*number_of_point_in_one_area*j + number_of_point_in_one_area*k + l;
                        int position_x; int position_y;
                        position_x = i*(number_of_point_in_one_area - 1) + k; position_y = j*(number_of_point_in_one_area - 1) + l;
                        *(*(Xarray_reg + position) + width_in_Xarray - 1) = *(*(*(downsampled_map + position_x) + position_y) + m);
                    }
        for (int i = 0; i < height_in_Xarray; i++)
            for (int j = 0; j < width_in_Xarray; j++)
                *(Xarray_trans + i*width_in_Xarray + j) = *(*(Xarray_reg + i) + j);

        //拟合函数
        aaa3(Xarray_trans, height_in_Xarray, width_in_Xarray, point_width, number_of_point_in_one_area*number_of_point_in_one_area, Answer);
        //output
        for (int i = 0; i < point_height; i++)
            for (int j = 0; j < point_width; j++)
                *(*(*(bezier_point_map + i) + j) + m) = *(Answer + i*point_width + j);
        //printf("%f", *(*(*(bezier_point_map))+k));
    }

    free(downsampled_map[0][0]);
    free(downsampled_map[0]);
    free(downsampled_map);
    free(Xarray_reg[0]);
    free(Xarray_reg);
    free(Answer);
    free(Xarray_trans);
    return bezier_point_map;
}
void AlgorithmFuncs::aaa14(double ***bezier_point_map, int height, int width, int rate_of_quantization, uint8_t* result)
{
    double max_of_control_point = 1 + (double) 255 / (double) rate_of_quantization;
    double min_of_control_point = 1;
    for (int k = 0; k < 16; k++)
    {
        int khw = k*height*width;
        for (int i = 0; i < height; i++)
        {
            int iw = i*width;
            for (int j = 0; j < width; j++)
            {
                if (*(*(*(bezier_point_map + i) + j) + k) < min_of_control_point)
                    *(*(*(bezier_point_map + i) + j) + k) = min_of_control_point;
                else if (*(*(*(bezier_point_map + i) + j) + k) > max_of_control_point)
                    *(*(*(bezier_point_map + i) + j) + k) = max_of_control_point;
                *(result + j + iw + khw) = (uint8_t)((*(*(*(bezier_point_map + i) + j) + k) - 1) * rate_of_quantization);
            }
        }
    }
    free(bezier_point_map[0][0]);
    free(bezier_point_map[0]);
    free(bezier_point_map);
}

void AlgorithmFuncs::aaa15(uint8_t *in, float *out, int height_in, int width_in, int height_out, int width_out)
{
    uint8_t *in_reg;
    float a11, a12, a13;
    float a21, a22, a23;
    float a31, a32, a33;
    float b11, b12, b13;
    float b21, b22, b23;
    float b31, b32, b33;
    float c1, c2, c3, t;
    float x, y, y2;
    int ox, oy;
    int iii;
    int i, j, ii, jj;
    int x1, y1;
    int grid_w = width_in - 2;
    int grid_h = height_in - 2;
    int width_grid = (int)ceil((float)(width_out) / grid_w);
    int height_grid = (int)ceil((float)(height_out) / grid_h);
    int i_width_off0, i_width_off1, i_width_off2;

    for (int k = 0; k < 16; k++)
    {
        int koff = (k >> 2)*width_out * 4 + (k % 4);
        in_reg = in + k*width_in*height_in;

        for (i = 0; i < grid_h; i++) {
            y1 = i*height_out / grid_h;
            i_width_off0 = i*width_in;
            i_width_off1 = (i + 1)*width_in;
            i_width_off2 = (i + 2)*width_in;
            for (j = 0; j < grid_w; j++) {
                a11 = in_reg[i_width_off0 + j]; a12 = in_reg[i_width_off0 + j + 1]; a13 = in_reg[i_width_off0 + j + 2];
                a21 = in_reg[i_width_off1 + j]; a22 = in_reg[i_width_off1 + j + 1]; a23 = in_reg[i_width_off1 + j + 2];
                a31 = in_reg[i_width_off2 + j]; a32 = in_reg[i_width_off2 + j + 1]; a33 = in_reg[i_width_off2 + j + 2];
                b11 = a11 / 4 - a12 / 2 + a13 / 4 - a21 / 2 + a22 - a23 / 2 + a31 / 4 - a32 / 2 + a33 / 4;
                b12 = a12 / 2 - a11 / 2 + a21 - a22 - a31 / 2 + a32 / 2;
                b13 = a11 / 4 + a12 / 4 - a21 / 2 - a22 / 2 + a31 / 4 + a32 / 4;
                b21 = a12 - a11 / 2 - a13 / 2 + a21 / 2 - a22 + a23 / 2;
                b22 = a11 - a12 - a21 + a22;
                b23 = a21 / 2 - a12 / 2 - a11 / 2 + a22 / 2;
                b31 = a11 / 4 - a12 / 2 + a13 / 4 + a21 / 4 - a22 / 2 + a23 / 4;
                b32 = a12 / 2 - a11 / 2 - a21 / 2 + a22 / 2;
                b33 = a11 / 4 + a12 / 4 + a21 / 4 + a22 / 4;
                x1 = j*width_out / grid_w;
                for (ii = 0; ii < height_grid; ii++) {
                    oy = y1 + ii;
                    iii = oy* width_out * 16 + koff;
                    y = (float)(ii*grid_h) / (float)height_out;
                    y2 = y*y;
                    c1 = b33 + b23*y + b13*y2;
                    c2 = b12*y2 + b22*y + b32;
                    c3 = b11*y2 + b21*y + b31;
                    for (jj = 0; jj < width_grid; jj++) {
                        ox = x1 + jj;
                        x = (float)(jj*grid_w) / (float)width_out;
                        t = (c1 + x*c2 + x*x*c3);
                        if (t < 0)
                        {
                            t = 0;
                        }
                        else if (t > 255)
                        {
                            t = 255;
                        }
                        //if (k == 0) {
                        //	out2[(iii - koff) / 16 + ox] = (unsigned char)(t);
                        //}
                        out[iii + ox * 4] = t;
                    }
                }
            }
        }
    }
}



double AlgorithmFuncs::GetROIChannelMean(unsigned char *pData, int width, int height, int roi_x, int roi_y, int roi_w, int roi_h)
{
    int cnt = 0;
    double mean_t = 0;

    if (roi_y + roi_h > height || roi_x + roi_w > width)
        return -1;

    for (int i = roi_y; i < roi_y + roi_h; i++)
    {
        for (int j = roi_x; j < roi_x + roi_w; j++)
        {
            mean_t += pData[width * i + j];
            cnt++;
        }
    }

    return mean_t / cnt;
}


