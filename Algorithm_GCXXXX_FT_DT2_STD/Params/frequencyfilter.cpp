//**************************************
//频率域滤波——以高斯低通为例
//**************************************
#include "Params/frequencyfilter.h"


//*****************高斯低通滤波器***********************
Mat frequencyfilter::gaussianlbrf(Mat scr,float sigma, int saveimage)
{
  Mat gaussianBlur(scr.size(),CV_32FC1); //，CV_32FC1
  float d0=2*sigma*sigma;//高斯函数参数，越小，频率高斯滤波器越窄，滤除高频成分越多，图像就越平滑
  for(int i=0;i<scr.rows ; i++ )
  {
      for(int j=0; j<scr.cols ; j++ )
      {
          float d=pow(float(i-scr.rows/2),2)+pow(float(j-scr.cols/2),2);//分子,计算pow必须为float型
          gaussianBlur.at<float>(i,j)=expf(-d/d0);//expf为以e为底求幂（必须为float型）
      }
  }

  if(saveimage)
  {
      FILE* fp = fopen("gaussianBlur_Data.csv","wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
      for (int y = 0; y < gaussianBlur.rows; y++)
      {
          for (int x = 0; x < gaussianBlur.cols; x++)
          {
              float point = gaussianBlur.at<float>(y, x);
              fprintf(fp, "%12.10f,", point);
          }
          fprintf(fp, "\n");
      }
      fclose(fp);
  }


 return gaussianBlur;
}


//*****************频率域滤波*******************
Mat frequencyfilter::freqfilt(Mat scr,Mat blur, int saveimage)
{
    //***********************DFT*******************
    Mat plane[]={scr, Mat::zeros(scr.size() , CV_32FC1)}; //创建通道，存储dft后的实部与虚部（CV_32F，必须为单通道数）
    Mat complexIm;
    merge(plane,2,complexIm);//合并通道 （把两个矩阵合并为一个2通道的Mat类容器）
    dft(complexIm,complexIm);//进行傅立叶变换，结果保存在自身

    //***************中心化********************
    split(complexIm,plane);//分离通道（数组分离）

    int cx=plane[0].cols/2;int cy=plane[0].rows/2;//以下的操作是移动图像  (零频移到中心)
    Mat part1_r(plane[0],Rect(0,0,cx,cy));  //元素坐标表示为(cx,cy)
    Mat part2_r(plane[0],Rect(cx,0,cx,cy));
    Mat part3_r(plane[0],Rect(0,cy,cx,cy));
    Mat part4_r(plane[0],Rect(cx,cy,cx,cy));

    Mat temp;
    part1_r.copyTo(temp);  //左上与右下交换位置(实部)
    part4_r.copyTo(part1_r);
    temp.copyTo(part4_r);

    part2_r.copyTo(temp);  //右上与左下交换位置(实部)
    part3_r.copyTo(part2_r);
    temp.copyTo(part3_r);

    Mat part1_i(plane[1],Rect(0,0,cx,cy));  //元素坐标(cx,cy)
    Mat part2_i(plane[1],Rect(cx,0,cx,cy));
    Mat part3_i(plane[1],Rect(0,cy,cx,cy));
    Mat part4_i(plane[1],Rect(cx,cy,cx,cy));

    part1_i.copyTo(temp);  //左上与右下交换位置(虚部)
    part4_i.copyTo(part1_i);
    temp.copyTo(part4_i);

    part2_i.copyTo(temp);  //右上与左下交换位置(虚部)
    part3_i.copyTo(part2_i);
    temp.copyTo(part3_i);

    //*****************滤波器函数与DFT结果的乘积****************
    Mat blur_r,blur_i,BLUR;
    multiply(plane[0], blur, blur_r); //滤波（实部与滤波器模板对应元素相乘）
    multiply(plane[1], blur, blur_i);//滤波（虚部与滤波器模板对应元素相乘）
    Mat plane1[]={blur_r, blur_i};
    merge(plane1,2,BLUR);//实部与虚部合并

      //*********************得到原图频谱图***********************************
    magnitude(plane[0],plane[1],plane[0]);//获取幅度图像，0通道为实部通道，1为虚部，因为二维傅立叶变换结果是复数
    plane[0]+=Scalar::all(1);  //傅立叶变o换后的图片不好分析，进行对数处理，结果比较好看
    log(plane[0],plane[0]);    // float型的灰度空间为[0，1])
    normalize(plane[0],plane[0],1,0,CV_MINMAX);  //归一化便于显示

    cv::imwrite(QString("./frequence_image.bmp").toStdString().c_str(),plane[0]);


    if(saveimage)
    {
        FILE* fp = fopen("frequence_Data.csv","wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
        for (int y = 0; y < plane[0].rows; y++)
        {
            for (int x = 0; x < plane[0].cols; x++)
            {
                float point = plane[0].at<float>(y, x);
                fprintf(fp, "%12.10f,", point);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
    }



    //******************IDFT*******************************
    /*
    Mat part111(BLUR,Rect(0,0,cx,cy));  //元素坐标(cx,cy)
    Mat part222(BLUR,Rect(cx,0,cx,cy));
    Mat part333(BLUR,Rect(0,cy,cx,cy));
    Mat part444(BLUR,Rect(cx,cy,cx,cy));
     part111.copyTo(temp);  //左上与右下交换位置(虚部)
    part444.copyTo(part111);
    temp.copyTo(part444);

    part222.copyTo(temp);  //右上与左下交换位置
    part333.copyTo(part222);
    temp.copyTo(part333);
    */

    idft( BLUR, BLUR);    //idft结果也为复数
    split(BLUR,plane);//分离通道，主要获取通道
    magnitude(plane[0],plane[1],plane[0]);  //求幅值(模)
    normalize(plane[0],plane[0],250,50,CV_MINMAX);  //归一化便于显示
    return plane[0];//返回参数
}


Mat frequencyfilter::self_adaptive_threshold_binary(Mat src, int block_x, int block_y, double ratio, int saveimage)
{
    int W=src.cols;
    int H=src.rows;
    int win_w=W/block_x;
    int win_h=H/block_y;

    vector<vector<float> >threshod_map_p;
    threshod_map_p.resize(block_y, vector<float>(block_x));

    vector<vector<float> >threshod_map_n(block_y);
    threshod_map_n.resize(block_y, vector<float>(block_x));

//    float threshod_map_p[block_y][block_x];
//    float threshod_map_n[block_y][block_x];

    for(int y=0; y<block_y; y++)
    {
        int y_start=y*win_h;
        for(int x=0; x<block_x; x++)
        {
            int x_start=x*win_w;
            Mat window;
            if(x_start+win_w>=W || y_start+win_h>=H)
            {
                window = src(Rect(x_start,y_start,W-x_start-1,H-y_start-1));
            }
            else
                window = src(Rect(x_start,y_start,win_w,win_h));

            double max=0;
            double min=0;
            int max_pos_index[2]={0};
            int min_pos_index[2]={0};

            minMaxIdx(window,&min,&max,min_pos_index,max_pos_index);

            threshod_map_p[y][x]=max;
            threshod_map_n[y][x]=min;
        }
    }

    if(saveimage)
    {
        FILE* fp_p = fopen("Threshold_p.csv","wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
        for (int y = 0; y < block_y; y++)
        {
            for (int x = 0; x < block_x; x++)
            {
                float point = threshod_map_p[y][x];
                fprintf(fp_p, "%4.2f,", point);
            }
            fprintf(fp_p, "\n");
        }
        fclose(fp_p);

        FILE* fp_n = fopen("Threshold_n.csv","wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
        for (int y = 0; y < block_y; y++)
        {
            for (int x = 0; x < block_x; x++)
            {
                float point = threshod_map_n[y][x];
                fprintf(fp_n, "%4.2f,", point);
            }
            fprintf(fp_n, "\n");
        }
        fclose(fp_n);
    }


    Mat binary = Mat(src.rows, src.cols, CV_8U);

    for(int i=0; i<H; i++)
    {
        int pos_y = i/win_h;
        float *ptr_src = src.ptr<float>(i);
        uchar *ptr_b = binary.ptr<uchar>(i);
        for(int j=0; j<W; j++)
        {
            int pos_x = j/win_w;
            QVector<float> threshold_p;
            QVector<float> threshold_n;
            if(pos_x -1>=0 && pos_y>=0)
            {
                threshold_p.append(threshod_map_p[pos_y][pos_x -1]);
                threshold_n.append(threshod_map_n[pos_y][pos_x -1]);
            }


            if(pos_x +1<block_x && pos_y>=0)
            {
                threshold_p.append(threshod_map_p[pos_y][pos_x +1]);
                threshold_n.append(threshod_map_n[pos_y][pos_x +1]);
            }


            double p_thr_v = *max_element(threshold_p.begin(),threshold_p.end());
//            double n_thr_v = *min_element(threshold_n.begin(),threshold_n.end());

            if(ptr_src[j]>p_thr_v*ratio)
                ptr_b[j] = 255;
            else
                ptr_b[j] = 0;
        }
    }
    Mat result =binary(Rect(0,0,W-17,H-13));
    return result;
}


Mat frequencyfilter::FourierTransform(Mat I, int saveimage, QString name,
                                      float &tl, float &tr, float &bl, float &br)
{
    Mat padded;
    copyMakeBorder(I, padded, 0, 0, 0,  0, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);

    dft(complexI, complexI);

    // complexI 分成planes[0]/planes[1] 存储实部和虚部
    split(complexI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magI = planes[0];

    // 图像增强
    magI += Scalar::all(1);
    log(magI, magI);

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    tl = magI.at<float>(0, 0);
    tr = magI.at<float>(0, magI.cols-1);
    bl = magI.at<float>(magI.rows-1, 0);
    br = magI.at<float>(magI.rows-1, magI.cols-1);

    if(saveimage)
    {
        FILE* fp = fopen(QString("./%1_frequence.csv").arg(name).toStdString().c_str(),"wt"); //"wt" 只写打开或建立一个文本文件，只读写数据 （t为文本文件)
        for (int y = 0; y < magI.rows; y++)
        {
            for (int x = 0; x < magI.cols; x++)
            {
                float point = magI.at<float>(y, x);
                fprintf(fp, "%12.10f,", point);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
    }

    normalize(magI, magI, 0, 255, NORM_MINMAX); // Transform the matrix with float values into a

    if(saveimage)
    {
        cv::imwrite(QString("./%1_frequence_image.bmp").arg(name).toStdString().c_str(),magI);
    }
    return magI;
}
