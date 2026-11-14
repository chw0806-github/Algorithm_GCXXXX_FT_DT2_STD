#include "FindClusterType.h"

int FindClusterType:: Find_ClusterType_in_binarymap_Type0(ParamManage& test_manage, int &dd_total_cnt, int &big_dd_cnt,int limit)
{
    //pix_type以map形式返回坏点类型和信息,请在外部为其分配width*height个bytes的空间。
    //dd_info返回转换成烧录约定格式后的BYTE型数组，4023最多可烧录43个坏点，外部dd_info数组长度设定为43x4 bytes.
    //dd_total_cnt返回要烧录的坏点个数，外部初始设定值0.
    //big_dd_cnt返回尺寸超标的坏点个数，外部初始设定值0，若big_dd_cnt大于0，则不进行OTP DD烧录，出FAIL BIN,Sbin74.
    //otp_dd_type返回11种类型坏点的个数，请在外部为其分配11个int的空间.
    int width = test_manage.width_;
    int height = test_manage.height_;
    int type = test_manage.param_binary_.type1_or_type11_;
    bool flag = false;
    int l, m;
    int i, j, ii, jj;
    int cnt;
    bool bType5_1 = false;

    dd_total_cnt =0;
    big_dd_cnt =0;

    if(width<0 || height<0 || crop_x<0 || crop_y<0 || map_img==NULL || !(type==1||type==11))
    {
        g_str_msg_all[site_].append ("image size or spec is wrong!\n");
        return -2;
    }

    memset(pix_type, 255, width * height);
    memset(otp_dd_type, 0, 12);

    //type 5*1
    for (i = 0; i < height; ++i)
    {
        j = width-4;
        if(map_img[width*i+j] ==255 && map_img[width*i+j+1] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+3] ==255)
        {
            pix_type[i * width + j] = 99;
            bType5_1 = true;
            big_dd_cnt++;
        }

        j = 0;
        if(map_img[width*i+j] ==255 && map_img[width*i+j+1] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+3] ==255)
        {
            pix_type[i * width + j] = 99;
            bType5_1 = true;
            big_dd_cnt++;
        }
    }

    for (i = 0; i < height; ++i)
    {
        for (j = 4; j < width-4; ++j)

        {
            if(j+6>width-1) continue;

            if(map_img[width*i+j] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+4] ==255  && map_img[width*i+j+6] ==255)
            {
                pix_type[i * width + j] = 99;
                bType5_1 = true;
                big_dd_cnt++;
            }
        }
    }

    if(bType5_1 == false)
    {

        // type1 or 同通道三坏点抓(type1+type0)
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;

                    if((j + 4) < width)
                    {
                        if(map_img[i * width + j + 2] == 255)
                        {
                            flag = true;
                            map_img[i * width + j    ] = 2;
                            map_img[i * width + j + 2] = 2;

                            pix_type[i * width + j] = 2;
                            otp_dd_type[1]++;

                            if(map_img[i * width + j + 4] == 255)
                            {
                                flag = true;

                                map_img[i * width + j + 4] = 12;

                                pix_type[i * width + j+4] = 12;
                                otp_dd_type[11]++;

                            }

                            continue;
                        }
                    }
                }
            }
        }

        // other type
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;
                   if((j + 1) < width)
                    {
                        if(map_img[i * width + j + 1] == 255)
                        {
                            flag = true;
                            map_img[i * width + j    ] = 7;
                            map_img[i * width + j + 1] = 7;
                            pix_type[i * width + j] = 7;
                            otp_dd_type[6]++;
                            continue;
                        }
                    }

//                    if((j + 2) < width)
//                    {
//                        if(map_img[i * width + j + 2] == 255)
//                        {
//                            flag = true;
//                            map_img[i * width + j    ] = 2;
//                            map_img[i * width + j + 2] = 2;

//                            pix_type[i * width + j] = 2;
//                            otp_dd_type[1]++;
//                            continue;
//                        }
//                    }

                    if((i + 1) < height)
                    {
                        if(map_img[(i + 1) * width + j] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j] = 8;
                           // map_img[(i + 1) * width + j] = 8;

                            pix_type[i * width + j] = 8;
                            otp_dd_type[7]++;
                            continue;
                        }
                    }

                    if((i + 2) < height)
                    {
                        if(map_img[(i + 2) * width + j] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j] = 3;
                          //  map_img[(i + 2) * width + j] = 3;

                            pix_type[i * width + j] = 3;
                            otp_dd_type[2]++;
                            continue;
                        }
                    }

                    if((i + 1) < height && (j + 1) < width)
                    {
                        if(map_img[(i + 1) * width + j + 1] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 9;
                          //  map_img[(i + 1) * width + j + 1] = 9;

                            pix_type[i * width + j] = 9;
                            otp_dd_type[8]++;
                            continue;
                        }
                    }

                    if((i + 2) < height && (j + 2) < width)
                    {
                        if(map_img[(i + 2) * width + j + 2] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 4;
                          //  map_img[(i + 2) * width + j + 2] = 4;

                            pix_type[i * width + j] = 4;
                            otp_dd_type[3]++;
                            continue;
                        }
                    }

                    if((i + 1) < height && (j - 1) > 0)
                    {
                        if(map_img[(i + 1) * width + j - 1] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 6;
                         //   map_img[(i + 1) * width + j - 1] = 6;

                            pix_type[i * width + j] = 6;
                            otp_dd_type[5]++;
                            continue;
                        }
                    }

                    if((i + 2) < height && (j - 2) > 0)
                    {
                        if(map_img[(i + 2) * width + j - 2] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 5;
                           // map_img[(i + 2) * width + j - 2] = 5;

                            pix_type[i * width + j] = 5;
                            otp_dd_type[4]++;
                            continue;
                        }
                    }

                    map_img[i * width + j] = 10;
                    pix_type[i * width + j] = 10;
                    otp_dd_type[9]++;

                }
            }
        }

        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if(pix_type[i * width + j] == 2)
                {
                    dd_total_cnt++;
                    dd_info_xy.insert(QPair<int,int>(i,j),1);
                }
                else if(pix_type[i * width + j] == 7)
                {
                    dd_total_cnt++;
                    dd_info_xy.insert(QPair<int,int>(i,j),2);

                }
                else if(pix_type[i * width + j] != 255 && pix_type[i * width + j] != 10)
                {
                    dd_total_cnt++;
                    dd_info_xy.insert(QPair<int,int>(i,j),0);
                }
                else if(pix_type[i * width + j] == 12)
                {
                    dd_total_cnt++;
                    dd_info_xy.insert(QPair<int,int>(i,j),0);
                }
                if(dd_total_cnt >limit)
                    return 1;
            }


        }
    }
    return true;
}
//GC2607
int FindClusterType:: Find_ClusterType_in_binarymap_Type11(ParamManage& test_manage, int &dd_total_cnt, int &big_dd_cnt,int dd_totla_limit)
{
    //pix_type以map形式返回坏点类型和信息,请在外部为其分配width*height个bytes的空间。
    //dd_info返回转换成烧录约定格式后的BYTE型数组，2607最多可烧录64个坏点，外部dd_info数组长度设定为64x4 bytes.
    //dd_total_cnt返回要烧录的坏点个数，外部初始设定值0.
    //big_dd_cnt返回尺寸超标的坏点个数，外部初始设定值0，若big_dd_cnt大于0，则不进行OTP DD烧录，出FAIL BIN,Sbin74.
    //otp_dd_type返回11种类型坏点的个数，请在外部为其分配11个int的空间.

    int width = test_manage.width_;
    int height = test_manage.height_;
    int type = test_manage.param_binary_.type1_or_type11_;
    bool flag = false;
    int l, m;
    int i, j, ii, jj;
    int cnt;
    bool bType5_1 = false;

    dd_total_cnt =0;
    big_dd_cnt =0;

    if(width<0 || height<0 || crop_x<0 || crop_y<0 || map_img==NULL || !(type==1||type==11))
    {
        g_str_msg_all[site_].append ("image size or spec is wrong!\n");
        return -2;
    }

    memset(pix_type, 255, width * height);
    memset(otp_dd_type, 0, 11);

    //type 5*1
    for (i = 0; i < height; ++i)
    {
        j = width-4;
        if(map_img[width*i+j] ==255 && map_img[width*i+j+1] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+3] ==255)
        {
            pix_type[i * width + j] = 99;
            bType5_1 = true;
            big_dd_cnt++;
        }

        j = 0;
        if(map_img[width*i+j] ==255 && map_img[width*i+j+1] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+3] ==255)
        {
            pix_type[i * width + j] = 99;
            bType5_1 = true;
            big_dd_cnt++;
        }
    }

    for (i = 0; i < height; ++i)
    {
        for (j = 4; j < width-4; ++j)

        {
            if(j+6>width-1) continue;

            if(map_img[width*i+j] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+4] ==255 && map_img[width*i+j+6] ==255)
            {
                pix_type[i * width + j] = 99;
                bType5_1 = true;
                big_dd_cnt++;
            }
        }
    }

    if(bType5_1 == false)
    {

        // type1 or 同通道三坏点抓(type1+type0)
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;

                    if((j + 4) < width)
                    {
                        if(map_img[i * width + j + 2] == 255)
                        {
                            flag = true;
                            map_img[i * width + j    ] = 2;
                            map_img[i * width + j + 2] = 2;

                            pix_type[i * width + j] = 2;
                            otp_dd_type[1]++;

                            if(map_img[i * width + j + 4] == 255)
                            {
                                flag = true;

                                map_img[i * width + j + 4] = 12;

                                pix_type[i * width + j+4] = 12;
                                otp_dd_type[11]++;

                            }

                            continue;
                        }
                    }
                }
            }
        }
        switch(type)
        {
        case 1:
            for (i=0; i<height; i++)
            {
                for (j=0; j<width; j++)
                {
                    if(map_img[i * width + j] == 255)
                    {
                        l = (i + crop_y) / 4 * 4 - crop_y; //2*4
                        m = (j + crop_x) / 2 * 2 - crop_x; //2*4

                        if (l < 0 || m < 0) continue; //2*4

                        if (map_img[l * width + m] == 1) continue; //2*4

                        // 2*4 type1
                        cnt = 0;
                        for(ii = l; ii < l + 4; ii++)
                        {
                            for(jj = m; jj < m + 2; jj++)
                            {
                                if(ii < height && jj < width)
                                {
                                    if(map_img[ii * width + jj] == 255)
                                        cnt++;
                                }
                            }
                        }

                        if (cnt >= 2)
                        {
                            map_img[(l    ) * width + m    ] = 1;
                            map_img[(l    ) * width + m + 1] = 1;
                            map_img[(l + 1) * width + m    ] = 1;
                            map_img[(l + 1) * width + m + 1] = 1;
                            map_img[(l + 2) * width + m    ] = 1;
                            map_img[(l + 2) * width + m + 1] = 1;
                            map_img[(l + 3) * width + m    ] = 1;
                            map_img[(l + 3) * width + m + 1] = 1;
                            pix_type[l * width + m] = 1;
                            otp_dd_type[0]++;

                            continue;
                        }
                    }
                }
            }
            break;
        case 11:
            for (i = 0; i < height; ++i)
            {
                for (j = 0; j < width; ++j)
                {
                    if(map_img[i * width + j] == 255)
                    {
//                        l = (i + crop_y) / 2 * 2 - crop_y; //2*2
//                        m = (j + crop_x) / 2 * 2 - crop_x; //2*2

                        l = i; //2*2
                        m = j; //2*2

                        if (l < 0 || m < 0) continue; //2*2

                        if (map_img[l * width + m] == 1) continue; //2*2

                        // 2*2 type11
                        cnt = 0;
                        for(ii = l; ii < l + 2; ii++)
                        {
                            for(jj = m; jj < m + 2; jj++)
                            {
                                if(ii < height && jj < width)
                                {
                                    if(map_img[ii * width + jj] == 255) cnt++;
                                }
                            }
                        }

                        if (cnt > 2)
                        {

                            map_img[(l    ) * width + m    ] = 11;
                            map_img[(l    ) * width + m + 1] = 11;
                            map_img[(l + 1) * width + m    ] = 11;
                            map_img[(l + 1) * width + m + 1] = 11;
                            pix_type[l * width + m] = 11;
                            otp_dd_type[10]++;


                            continue;
                        }
                    }
                }
            }
            break;
        }
        // other type
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;


                    if((j + 1) < width)
                    {
                        if(map_img[i * width + j + 1] == 255)
                        {
                            flag = true;
                            map_img[i * width + j    ] = 7;
                            map_img[i * width + j + 1] = 7;

                            pix_type[i * width + j] = 7;
                            otp_dd_type[6]++;

                            continue;
                        }
                    }

//                    if((j + 2) < width)
//                    {
//                        if(map_img[i * width + j + 2] == 255)
//                        {
//                            flag = true;
//                            map_img[i * width + j    ] = 2;
//                            map_img[i * width + j + 2] = 2;

//                            pix_type[i * width + j] = 2;
//                            otp_dd_type[1]++;
//
//                            continue;
//                        }
//                    }

                    if((i + 1) < height)
                    {
                        if(map_img[(i + 1) * width + j] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j] = 8;
                            map_img[(i + 1) * width + j] = 8;

                            pix_type[i * width + j] = 8;
                            otp_dd_type[7]++;

                            continue;
                        }
                    }

                    if((i + 2) < height)
                    {
                        if(map_img[(i + 2) * width + j] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j] = 3;
                            map_img[(i + 2) * width + j] = 3;

                            pix_type[i * width + j] = 3;
                            otp_dd_type[2]++;

                            continue;
                        }
                    }

                    if((i + 1) < height && (j + 1) < width)
                    {
                        if(map_img[(i + 1) * width + j + 1] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 9;
                            map_img[(i + 1) * width + j + 1] = 9;

                            pix_type[i * width + j] = 9;
                            otp_dd_type[8]++;

                            continue;
                        }
                    }

                    if((i + 2) < height && (j + 2) < width)
                    {
                        if(map_img[(i + 2) * width + j + 2] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 4;
                            map_img[(i + 2) * width + j + 2] = 4;

                            pix_type[i * width + j] = 4;
                            otp_dd_type[3]++;

                            continue;
                        }
                    }

                    if((i + 1) < height && (j - 1) > 0)
                    {
                        if(map_img[(i + 1) * width + j - 1] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 6;
                            map_img[(i + 1) * width + j - 1] = 6;

                            pix_type[i * width + j - 1] = 6;
                            otp_dd_type[5]++;

                            continue;
                        }
                    }

                    if((i + 2) < height && (j - 2) > 0)
                    {
                        if(map_img[(i + 2) * width + j - 2] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 5;
                            map_img[(i + 2) * width + j - 2] = 5;

                            pix_type[i * width + j - 2] = 5;
                            otp_dd_type[4]++;

                            continue;
                        }
                    }

                    map_img[i * width + j] = 10;
                    pix_type[i * width + j] = 10;
                    otp_dd_type[9]++;

                }
            }
        }



        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if(pix_type[i * width + j] == 1)
                {


                    dd_info_xy.insert(QPair<int,int>(i,j),4);

                    dd_total_cnt++;

                }
                else if(pix_type[i * width + j] == 2)
                {


                     dd_info_xy.insert(QPair<int,int>(i,j),1);

                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 3)
                {



                     dd_info_xy.insert(QPair<int,int>(i,j),8);
                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 4)
                {


                     dd_info_xy.insert(QPair<int,int>(i,j),9);
                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 5)
                {


                     dd_info_xy.insert(QPair<int,int>(i,j),10);
                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 6)
                {


                     dd_info_xy.insert(QPair<int,int>(i,j),7);
                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 7)
                {


                     dd_info_xy.insert(QPair<int,int>(i,j),2);
                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 8)
                {

                     dd_info_xy.insert(QPair<int,int>(i,j),5);
                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 9)
                {


                     dd_info_xy.insert(QPair<int,int>(i,j),6);
                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 11)
                {

                    dd_info_xy.insert(QPair<int,int>(i,j),3);
                    dd_total_cnt++;
                }
                else if(pix_type[i * width + j] == 12)
                {

                    dd_info_xy.insert(QPair<int,int>(i,j),0);
                    dd_total_cnt++;
                }
                if(dd_total_cnt > dd_totla_limit)
                    return 1;
            }
        }

    }
    return true;
}



//GC10V0
int FindClusterType:: Find_ClusterType_in_binarymap_Type15(ParamManage& test_manage, int &dd_total_cnt, int &big_dd_cnt,int dd_totla_limit)
{

    //______________________

    logMsg("step_1",0,0);
    int width = test_manage.width_;
    int height = test_manage.height_;
    int zwidth = width>>2;

    //GC10V0
    if(width<=0 || height<=0 /*|| crop_x!=0 || crop_y!=0 ||  map_src==nullptr*/)
    {
        g_str_msg_all[site_].append ("image size or spec is wrong!\n");
        return -1;
    }

    dd_total_cnt=0;
    big_dd_cnt=0;
//    /*int */otp_dd_total_cnt =0;
//    int otp_dd_limit_cnt = 500;
//    uchar *map_img=nullptr;
//    if(!Algorithm_all[site_].New_Pointer_Buf (&map_img,Result_area,Buf_uchar_8,width,height,1))
//    {
//        g_str_msg_all[site_]+="Algorithm init failed!";
//        return -1;
//    }
//    memcpy(map_img,map_src,width*height*sizeof(uchar));
    for(int i=0;i!=15;++i)
        otp_dd_type[i]=0;


    //type big DD
    bool bType_bigDD = false;
    logMsg("start find big dd 1",0,0);
    for (int i = 2; i < height-2; ++i)
    {
        for (int j = 2; j < width-2; ++j)
        {
            if(map_img[i * width + j] == 255)
            {
                int cnt =0;
                for(int ii = i-2; ii <= i+2; ii++)
                    for(int jj = j-2; jj <= j+2; jj++)
                        if(map_img[width*ii+jj] ==255) cnt++;

                if(cnt >= 16)
                {
                    big_dd_cnt++;
                    bType_bigDD = true;
                }
            }
        }
    }

    // 同通道4连点
    logMsg("start find big dd 2",0,0);
    for(int i=0;i!=height;i++)
    {
        for(int j=6;j!=width;j++)
        {
            if(map_img[i * width + j]==255  && map_img[i * width + j-2]==255 &&
               map_img[i * width + j-4]==255 && map_img[i * width + j-6]==255)
            {
                big_dd_cnt++;
                bType_bigDD = true;
                if(big_dd_cnt<=10)
                {
                    QString msg=QString("Find big dd x:%1,y%2 \n").arg(j).arg(i);
                    g_str_msg_all[site_].append (msg.toStdString());
                }
            }
        }
    }

    if(bType_bigDD )
        return 1;

    logMsg("start find cluster",0,0);


    // 使用 QPair 列表初始化 QMap
    // 创建一个空的 QMap
    QMap<unsigned char, int> cluster_type;

    // 手动插入元素
    cluster_type.insert(0b1000, 1);
    cluster_type.insert(0b0100, 2);
    cluster_type.insert(0b1100, 3);
    cluster_type.insert(0b0010, 4);
    cluster_type.insert(0b1010, 5);
    cluster_type.insert(0b0110, 6);
    cluster_type.insert(0b1110, 7);
    cluster_type.insert(0b0001, 8);
    cluster_type.insert(0b1001, 9);
    cluster_type.insert(0b0101, 10);
    cluster_type.insert(0b1101, 11);
    cluster_type.insert(0b0011, 12);
    cluster_type.insert(0b1011, 13);
    cluster_type.insert(0b0111, 14);
    cluster_type.insert(0b1111, 15);



    //双坏点及以上
    int cluster_cnt=0;
    QVector<uchar> type_map(width*height/4,0);
    for(int i=0,j=0;i!=width*height;i=i+4,j++)
    {
        int sum = map_img[i]+map_img[i+1]+map_img[i+2]+map_img[i+3];
        if(sum>255)
        {
            if( (map_img[i]==255 && map_img[i+1]==255) || (map_img[i+1]==255 && map_img[i+2]==255) || (map_img[i+2]==255 && map_img[i+3]==255) )
            {
                //g_str_msg_all[site_].append (boost::str(boost::format("Find cluster dd excexpt x = %d,y = %d  \n")%(i%width)%(i/width)));
                continue;
            }

            cluster_cnt++;
            type_map[j]= (map_img[i]&0b1000) | (map_img[i+1]&0b0100) | (map_img[i+2]&0b0010) | (map_img[i+3]&0b0001);
            map_img[i] = map_img[i+1] = map_img[i+2] = map_img[i+3]=0;
            if(cluster_cnt<10)
            {
                int type_=cluster_type[type_map[j]];
                g_str_msg_all[site_].append (boost::str(boost::format("Find cluster dd x = %d,y = %d ,type= %x \n")%(i%width)%(i/width)%(type_)));


            }


        }
    }



    // 创建一个空的 QSet<unsigned char>
    QSet<unsigned char> type_1000;

    // 使用 insert() 插入元素
    type_1000.insert(0b1010);
    type_1000.insert(0b1110);
    type_1000.insert(0b1011);
    type_1000.insert(0b1111);


    //----------------------------
    QSet<uchar> type_0100;
    type_0100.insert(0b0101);
    type_0100.insert(0b1101);
    type_0100.insert(0b0111);
    type_0100.insert(0b1111);

    //----------------------------
    QSet<uchar> type_0010;
    type_0010.insert(0b1100);
    type_0010.insert(0b1010);
    type_0010.insert(0b1001);
    type_0010.insert(0b1011);
    type_0010.insert(0b1101);
    type_0010.insert(0b1110);
    type_0010.insert(0b1111);

    //___________________________
    QSet<uchar> type_0001;
    type_0001.insert(0b1100);
    type_0001.insert(0b0110);
    type_0001.insert(0b0101);
    type_0001.insert(0b0111);
    type_0001.insert(0b1101);
    type_0001.insert(0b1110);
    type_0001.insert(0b1111);


    //水平单坏点
    int k=0;
    int single_cnt=0;
    for(int i=0;i!=height;++i)
    {
        for(int j=0;j!=width;j=j+4,k++)
        {
            int loca=i*width+j;
            int sum = map_img[loca]+map_img[loca+1]+map_img[loca+2]+map_img[loca+3];

            if(sum==255)
            {
                if(map_img[loca]==255 && j>=4)
                {
                    if(type_1000.contains(type_map[k-1]))
                        type_map[k]=0b1000;
                }
                else if(map_img[loca+1]==255 && j>=4)
                {
                    if(type_0100.contains(type_map[k-1]))
                        type_map[k]=0b0100;
                }
                else if(map_img[loca+2]==255 && j+4<width)
                {
                    if(map_img[loca+4]==255 || type_0010.contains(type_map[k+1]))
                        type_map[k]=0b0010;
                }
                else if(map_img[loca+3]==255 && j+4<width)
                {
                    if(/*map_img[loca+4]==255 || */map_img[loca+5]==255 || type_0001.contains(type_map[k+1]))
                        type_map[k]=0b0001;
                }

                if(type_map[k]!=0)
                {
                    single_cnt++;
//                    map_img[i] = map_img[i+1] = map_img[i+2] = map_img[i+3]=0;
                    map_img[loca] = map_img[loca+1] = map_img[loca+2] = map_img[loca+3] = 0;
                    if(single_cnt<10)
                    {
                        int type_=cluster_type[type_map[k]];
                        g_str_msg_all[site_].append (boost::str(boost::format("Find single_1 dd x = %d,y = %d ,type= %x \n")%(j)%(i)%(type_)));

                    }

                }
            }
        }
    }


    //垂直单坏点
    QMap<int,uchar>single_map;
    single_map.insert(0,0b1000);
    single_map.insert(1,0b0100);
    single_map.insert(2,0b0010);
    single_map.insert(3,0b0001);

    for(int i=0;i!=height;++i)
    {
        for(int j=0;j!=width;++j)
        {
            if(map_img[i * width + j]==255)
            {
                bool need_burn=false;
                if(i+1!=height)
                {
                    if(/*map_img[ (i+1) * width + j]==255 || */(j-1>=0 && map_img[ (i+1) * width + j-1]==255) || (j+1!=width && map_img[ (i+1) * width + j+1]==255) )
                        need_burn=true;
                }
                if(!need_burn && i+2!=height)
                {
                    if(map_img[ (i+2) * width + j]==255 || (j-2>=0 && map_img[ (i+2) * width + j-2]==255) || (j+2!=width && map_img[ (i+2) * width + j+2]==255) )
                        need_burn=true;
                }

                if(need_burn)
                {
                    single_cnt++;
                    int loca=i * width + j;
                    map_img[loca]=0;
                    type_map[loca/4]=single_map[loca%4];
                    if(single_cnt<10)
                    {
                        int type_=cluster_type[type_map[loca/4]];

                        g_str_msg_all[site_].append (boost::str(boost::format("Find single_2 dd x = %d,y = %d,type =%x \n")%(4*j)%(i)%(type_)));
                    }
                }
            }
        }
    }

    //write dd info
//    int dd_info_cnt=0;
//    dd_total_cnt=cluster_cnt+single_cnt;
    if (1/*dd_total_cnt <= dd_totla_limit*/)
    {
        for(int i =0;i<height;i++)
        {
            for(int j =0;j< zwidth;j++)
            {
                uchar type_ = type_map[i*zwidth+j];
                if(cluster_type.contains(type_))
                {
                    int otp_type = cluster_type[type_];
                    otp_dd_type[otp_type-1]++;

//                    int col = j+0+1;
//                    int row = i+0+1;

//                    dd_info[dd_info_cnt++] = (col & 0x00ff);
//                    dd_info[dd_info_cnt++] = (row & 0x000f) << 4 | ((col >>8) &0x0007);
//                    dd_info[dd_info_cnt++] = (row >>4) & 0x00ff;
//                    dd_info[dd_info_cnt++] = otp_type&0x0f;

//                    dd_info_xy.insert(QPair<int,int>(i,j*4),otp_type&0x0f);
                    dd_info_xy.insert(QPair<int,int>(i,j),otp_type&0x0f);
                }
            }
        }
    }

    // Fix: dd_total_cnt 直接使用 dd_info_xy 的数量，保证两者一致
    dd_total_cnt = dd_info_xy.size();

    return 1;



}





int FindClusterType:: Find_ClusterType_in_binarymap_Type15_Ex(ParamManage& test_manage, int &dd_total_cnt, int &big_dd_cnt,int limit, int &dd_special_cnt)
{
    //pix_type以map形式返回坏点类型和信息,请在外部为其分配width*height个bytes的空间。
    //dd_info返回转换成烧录约定格式后的BYTE型数组，4023最多可烧录43个坏点，外部dd_info数组长度设定为43x4 bytes.
    //dd_total_cnt返回要烧录的坏点个数，外部初始设定值0.
    //big_dd_cnt返回尺寸超标的坏点个数，外部初始设定值0，若big_dd_cnt大于0，则不进行OTP DD烧录，出FAIL BIN,Sbin74.
    //otp_dd_type返回11种类型坏点的个数，请在外部为其分配11个int的空间.
    int width = test_manage.width_;
    int height = test_manage.height_;
    int type = test_manage.param_binary_.type1_or_type11_;
    bool flag = false;
    int l, m;
    int i, j, ii, jj;
    int cnt;
    bool bType5_1 = false;
    bool bTypespecial = false;

    dd_total_cnt =0;
    big_dd_cnt =0;
    dd_special_cnt =0;

    if(width<0 || height<0 || crop_x<0 || crop_y<0 || map_img==NULL || !(type==1||type==11))
    {
        g_str_msg_all[site_].append ("image size or spec is wrong!\n");
        return -2;
    }

    memset(pix_type, 255, width * height);
    memset(otp_dd_type, 0, 15);

    //type 5*1
    for (i = 0; i < height; ++i)
    {
        j = width-4;
        if(map_img[width*i+j] ==255 && map_img[width*i+j+1] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+3] ==255)
        {
            pix_type[i * width + j] = 99;
            bType5_1 = true;
            big_dd_cnt++;
        }

        j = 0;
        if(map_img[width*i+j] ==255 && map_img[width*i+j+1] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+3] ==255)
        {
            pix_type[i * width + j] = 99;
            bType5_1 = true;
            big_dd_cnt++;
        }
    }

    for (i = 0; i < height; ++i)
    {
        for (j = 4; j < width-4; ++j)

        {
            if(j+6>width-1) continue;

            if(map_img[width*i+j] ==255 && map_img[width*i+j+2] ==255 && map_img[width*i+j+4] ==255  && map_img[width*i+j+6] ==255)
            {
                pix_type[i * width + j] = 99;
                bType5_1 = true;
                big_dd_cnt++;
            }
        }
    }


    if(bType5_1 == false)
    {

        // type1 or 同通道三坏点抓(type1+type0)
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;

                    if((j + 4) < width)
                    {
                        if(map_img[i * width + j + 2] == 255)
                        {
                            flag = true;
                            map_img[i * width + j    ] = 2;
                            map_img[i * width + j + 2] = 2;

                            pix_type[i * width + j] = 2;
                            otp_dd_type[1]++;

                            if(map_img[i * width + j + 4] == 255)
                            {
                                flag = true;

                                map_img[i * width + j + 4] = 12;

                                pix_type[i * width + j+4] = 12;
                                otp_dd_type[11]++;

                            }

                            continue;
                        }
                    }
                }
            }
        }

        for (i = 1; i < height - 1; ++i)
        {
            for (j = 1; j < width - 1; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;

                    if((i + 1) < height - 1 && (j + 1) < width - 1)
                    {
                        if(map_img[i * width + (j - 1)] == 255 && map_img[i * width + (j + 1)] == 255 && map_img[(i - 1) * width + j] == 255 && map_img[(i + 1) * width + j] == 255)
                        {
                            flag = true;
                            map_img[i * width + (j - 1)] = 2;
                            map_img[i * width + (j + 1)] = 2;

                            map_img[(i - 1) * width + j] = 3;
                            map_img[(i + 1) * width + j] = 3;

                            pix_type[i * width + (j - 1)] = 2;
                            pix_type[(i - 1) * width + j] = 3;
                            pix_type[i       * width + j] = 15;
                            dd_special_cnt++;
                            otp_dd_type[14]++;

                            continue;
                        }
                    }
                }
            }
        }

        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;

                    if((j + 2) < width)
                    {
                        if(map_img[i * width + j + 2] == 255)
                        {
                            flag = true;
                            map_img[i * width + j    ] = 2;
                            map_img[i * width + j + 2] = 2;

                            pix_type[i * width + j] = 2;
                            otp_dd_type[1]++;

                            if(map_img[i * width + j + 1] == 255)
                            {
                                flag = true;

                                map_img[i * width + j + 1] = 13;

                                pix_type[i * width + j + 1] = 13;
                                dd_special_cnt++;
                                otp_dd_type[12]++;

                            }

                            continue;
                        }
                    }
                }
            }
        }
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;

                    if((i + 2) < height)
                    {
                        if(map_img[(i + 2) * width + j] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j] = 3;
                            map_img[(i + 2) * width + j] = 3;

                            pix_type[i * width + j] = 3;
                            otp_dd_type[2]++;

                            if(map_img[(i + 1) * width + j] == 255)
                            {
                                flag = true;

                                map_img[(i + 1) * width + j] = 14;

                                pix_type[(i + 1) * width + j] = 14;
                                dd_special_cnt++;
                                otp_dd_type[13]++;

                            }

                            continue;
                        }
                    }
                }
            }
        }

        // other type
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if (map_img[i * width + j] == 255)
                {
                    flag = false;
                   if((j + 1) < width)
                    {
                        if(map_img[i * width + j + 1] == 255)
                        {
                            flag = true;
                            map_img[i * width + j    ] = 7;
                            map_img[i * width + j + 1] = 7;
                            pix_type[i * width + j] = 7;
                            otp_dd_type[6]++;
                            continue;
                        }
                    }

//                    if((j + 2) < width)
//                    {
//                        if(map_img[i * width + j + 2] == 255)
//                        {
//                            flag = true;
//                            map_img[i * width + j    ] = 2;
//                            map_img[i * width + j + 2] = 2;

//                            pix_type[i * width + j] = 2;
//                            otp_dd_type[1]++;
//                            continue;
//                        }
//                    }

                    if((i + 1) < height)
                    {
                        if(map_img[(i + 1) * width + j] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j] = 8;
                           // map_img[(i + 1) * width + j] = 8;

                            pix_type[i * width + j] = 8;
                            otp_dd_type[7]++;
                            continue;
                        }
                    }

                    if((i + 2) < height)
                    {
                        if(map_img[(i + 2) * width + j] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j] = 3;
                          //  map_img[(i + 2) * width + j] = 3;

                            pix_type[i * width + j] = 3;
                            otp_dd_type[2]++;
                            continue;
                        }
                    }

                    if((i + 1) < height && (j + 1) < width)
                    {
                        if(map_img[(i + 1) * width + j + 1] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 9;
                          //  map_img[(i + 1) * width + j + 1] = 9;

                            pix_type[i * width + j] = 9;
                            otp_dd_type[8]++;
                            continue;
                        }
                    }

                    if((i + 2) < height && (j + 2) < width)
                    {
                        if(map_img[(i + 2) * width + j + 2] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 4;
                          //  map_img[(i + 2) * width + j + 2] = 4;

                            pix_type[i * width + j] = 4;
                            otp_dd_type[3]++;
                            continue;
                        }
                    }

                    if((i + 1) < height && (j - 1) > 0)
                    {
                        if(map_img[(i + 1) * width + j - 1] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 6;
                         //   map_img[(i + 1) * width + j - 1] = 6;

                            pix_type[i * width + j] = 6;
                            otp_dd_type[5]++;
                            continue;
                        }
                    }

                    if((i + 2) < height && (j - 2) > 0)
                    {
                        if(map_img[(i + 2) * width + j - 2] == 255)
                        {
                            flag = true;
                            map_img[i       * width + j    ] = 5;
                           // map_img[(i + 2) * width + j - 2] = 5;

                            pix_type[i * width + j] = 5;
                            otp_dd_type[4]++;
                            continue;
                        }
                    }

                    map_img[i * width + j] = 10;
                    pix_type[i * width + j] = 10;
                    otp_dd_type[9]++;

                }
            }
        }

        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < width; ++j)
            {
                if(pix_type[i * width + j] == 2)
                {
                    dd_total_cnt++;
                    dd_info_xy.insert(QPair<int,int>(i,j),1);
                }
                else if(pix_type[i * width + j] == 7)
                {
                    dd_total_cnt++;
                    dd_info_xy.insert(QPair<int,int>(i,j),2);

                }
                else if(pix_type[i * width + j] != 255 && pix_type[i * width + j] != 10)
                {
                    dd_total_cnt++;
                    dd_info_xy.insert(QPair<int,int>(i,j),0);
                }
                else if(pix_type[i * width + j] == 12 || pix_type[i * width + j] == 13 || pix_type[i * width + j] == 14 || pix_type[i * width + j] == 15)
                {
                    dd_total_cnt++;
                    dd_info_xy.insert(QPair<int,int>(i,j),0);
                }
                if(dd_total_cnt >limit)
                    return 1;
            }


        }
    }
    return true;
}
