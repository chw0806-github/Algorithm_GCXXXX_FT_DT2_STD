#ifndef FREQUENCYFILTER_H
#define FREQUENCYFILTER_H

#endif // FREQUENCYFILTER_H


//**************************************
//频率域滤波——以高斯低通为例
//**************************************
#include <opencv2/opencv.hpp>

#include <fstream>
#include <iostream>

#include <QString>
#include <string>
#include <QVector>
#include <vector>

using namespace std;
using namespace cv;

namespace frequencyfilter
{
Mat gaussianlbrf(Mat scr, float sigma, int saveimage);//高斯低通滤波器函数
Mat freqfilt(Mat scr, Mat blur, int saveimage);//频率域滤波函数

Mat self_adaptive_threshold_binary(Mat src, int block_x, int block_y, double ratio, int saveimage);


Mat FourierTransform(Mat I, int saveimage, QString name,
                     float &tl, float &tr, float &bl, float &br);


}
