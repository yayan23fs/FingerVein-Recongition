//
//  alignment.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "alignment.hpp"
int bitmap_mask=0;

Mat alignment(Mat dataMat, Mat refMat, SkeletonNodes skelPoint_input[],SkeletonNodes skelPoint_reference[],
              int numberNodes_inputImg, int numberNodes_referenceImg,int block_searchRange)
{
    int height = dataMat.rows;
    int width = dataMat.cols;

    int  pixelDataIn [width*height];
    int  pixelDataRef [width*height];
    int  pixelDataRefBackup [width*height];
    
    float meanPointX_input = 0, meanPointY_input = 0;
    float meanPointX_reference = 0, meanPointY_reference = 0;
    int   cnt_matchedBifurcation_i = 0, cnt_matchedBifurcation_r = 0;
    int   meanPointX = 0, meanPointY = 0;

    uchar *ptr;


    //  Mat -> array
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelDataIn[y*width+x] = ptr[x];
        }
    }

    for(int y = 0; y < height; y++){
        ptr = refMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelDataRef[y*width+x] = ptr[x];
            pixelDataRefBackup[y*width+x] = ptr[x];
        }
    }
    
    //Calculate the coordinates of the mean point
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if( pixelDataIn[y*width+x] != bitmap_mask )//若現在這個點不等於0(黑色)
            {
                meanPointX_input += x;
                meanPointY_input += y;
                cnt_matchedBifurcation_i++;
            }
            if( pixelDataRef[y*width+x] != bitmap_mask )
            {
                meanPointX_reference += x;
                meanPointY_reference += y;
                cnt_matchedBifurcation_r++;
            }
        }
    }
    meanPointX_input     /= (float)cnt_matchedBifurcation_i;
    meanPointY_input     /= (float)cnt_matchedBifurcation_i;
    meanPointX_reference /= (float)cnt_matchedBifurcation_r;
    meanPointY_reference /= (float)cnt_matchedBifurcation_r;
    meanPointX = (int)(meanPointX_input - meanPointX_reference);
    meanPointY = (int)(meanPointY_input - meanPointY_reference);

    for( int y = 0; y < height; y++ )
    {
        for( int x = 0; x < width; x++ )
        {
            if( ((y+meanPointY) >= 0) && ((y+meanPointY) < height) &&
               ((x+meanPointX) >= 0) && ((x+meanPointX) < width) )
                pixelDataRef[y*width+x] = pixelDataRefBackup[(y+meanPointY)*width+(x+meanPointX)];
            else
                pixelDataRef[y*width+x] = bitmap_mask;
        }
    }
    for(int y = 0; y < height; y++)
    {
        ptr = refMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++)
        {
            ptr[x] = pixelDataRef[y*width+x];
        }
    }

    //    for(int y = 0; y < height; y++){
    //        ptr = dataMat.ptr<uchar>(y);
    //        for(int x = 0; x < width; x++){
    //            ptr[x] = pixelData[y*width+x];
    //        }
    //    }
//    refMat.put(0, 0, pixelDataRef);
//    imshow("Alignmemt", refMat);

    return refMat;
}