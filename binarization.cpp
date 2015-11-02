//
//  binarization.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "binarization.hpp"

void binarization(Mat dataMat,int T_Bin, int maskSize)
{
    int width =dataMat.cols;
    int height=dataMat.rows;

    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
        {
            if(dataMat.data[width*y+x]>T_Bin)
                dataMat.data[width*y+x]=255;
            else if((x < maskSize || x >= width - maskSize) || (y < maskSize || y >= height - maskSize))
                dataMat.data[y*width+x] = 255;
            else
                dataMat.data[width*y+x]=0;
        }
    }
    imshow("binarization", dataMat);
    
}