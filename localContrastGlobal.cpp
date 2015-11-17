//
//  localContrastGlobal.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "localContrastGlobal.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;

void localContrastGlobal(Mat dataMat, int maskSize){
    int width =dataMat.cols;
    int height=dataMat.rows;
    Size size=dataMat.size();
    Mat pixelData ;
    Mat pixelDataTmp ;
    dataMat.copyTo(pixelData);
    // 中心點位置
    for(int y = maskSize; y < height - maskSize; y++)
    {
        for(int x = maskSize; x < width - maskSize; x++)
        {
            int sum = 0;
            int hist [256]={0};
            int grayTmp;
            double grayNow;

            //統計mask中的直方圖
            for(int j = y - maskSize; j <= y + maskSize; j++)
            {
                for(int i = x - maskSize; i <= x + maskSize; i++)
                    hist[pixelData.data[j*width+i]]++;//累積直方圖(pdf)
            }

            grayTmp = pixelData.data[y*width+x];

            for(int i = 0; i <= grayTmp; i++)
                sum += hist[i];//cdf
            grayNow = 255 * sum/((2*maskSize+1)*(2*maskSize+1));//255*(總和/mask_size)

            if((int)grayNow > 255)
                grayNow = 255;
            else if((int)grayNow < 0)
                grayNow = 0;
            dataMat.data[y*width+x] = (int)grayNow;
        }
    }



}
