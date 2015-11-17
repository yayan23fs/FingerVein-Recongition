//
//  iqaMatching.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/3.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "iqaMatching.hpp"

float iqaMatching(Mat dataMat, Mat refMat)
{
    int height = dataMat.rows;
    int width = dataMat.cols;
    int size = (dataMat.rows*dataMat.cols);


    int pixelDataInInt[size];
    int pixelDataRefInt[size];


    float mean_input         = 0, mean_reference     = 0;
    float var_input          = 0, var_reference      = 0;
    float covar              = 0;
    float k1                 = (float) 0.01, k2      = (float) 0.03;
    float c1                 = 0, c2                 = 0;
    float cnt_mean           = 0;
    float cnt_var            = 0;
    float cnt_covar          = 0;
    float SSIM = 0;

    uchar *ptr;

    c1 = (k1 * 256) * (k1 * 256);
    c2 = (k2 * 256) * (k2 * 256);

//    dataMat.get(0, 0, pixelDataIn);
//    refMat.get(0, 0, pixelDataRef);

    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelDataInInt[y*width+x] = ptr[x];
        }
    }

    for(int y = 0; y < height; y++){
        ptr = refMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelDataRefInt[y*width+x] = ptr[x];
        }
    }

    // Brightness
    for( int y = 0; y <height; y++ )
    {
        for( int x = 0; x < width; x++ )
        {
            if( pixelDataInInt[y*width+x] != 0 && pixelDataRefInt[y*width+x] != 0 )
            {
                mean_input     += pixelDataInInt[y*width+x];
                mean_reference += pixelDataRefInt[y*width+x];
                cnt_mean++;
            }
        }
    }
    mean_input     /= cnt_mean;
    mean_reference /= cnt_mean;

    // Contrast && Structure
    for( int y = 0; y < height; y++ )
    {
        for( int x = 0; x < width; x++ )
        {
            //  Variance
            if( pixelDataInInt[y*width+x] != 0  && pixelDataRefInt[y*width+x] != 0 )
            {
                var_input += ((pixelDataInInt[y*width+x] - mean_input) *
                              (pixelDataInInt[y*width+x] - mean_input));
                var_reference += ((pixelDataRefInt[y*width+x] - mean_reference) *
                                  (pixelDataRefInt[y*width+x] - mean_reference));
                cnt_var++;
            }
            // 	Covariance
            if( (pixelDataInInt[y*width+x] != 0 && pixelDataRefInt[y*width+x] != 0) &&
               (pixelDataInInt[y*width+x] <= mean_input || pixelDataRefInt[y+width+x] <= mean_reference) )
            {
                covar += ((pixelDataInInt[y*width+x] - mean_input) *
                          (pixelDataRefInt[y*width+x] - mean_reference));
                cnt_covar++;
            }
        }
    }
    //共變異數（Covariance）衡量 "兩個變量" 的總體誤差
    //變異數(variance)是共變異數的一種特殊情況，即當 "兩個變量是相同" 的情況

    var_input     /= cnt_var;
    var_reference /= cnt_var;
    covar /= cnt_covar;

    SSIM = ( (2 * mean_input * mean_reference + c1) * (2 * covar + c2) ) /
    ( ((mean_input * mean_input) + (mean_reference * mean_reference) + c1) *
     (var_input + var_reference + c2 ));

    return SSIM;
}