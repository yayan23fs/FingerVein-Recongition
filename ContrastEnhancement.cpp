//
//  ContrastEnhancement.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "ContrastEnhancement.hpp"

void ContrastEnhancement(Mat dataMat){
    int width =dataMat.cols;
    int height=dataMat.rows;
    int   hist[256];
    float cdf[256];
    int   sum = 0;
    int   sz = 0;
    int   mapping[256];

    for(int i = 0; i < 256; ++i )
    {
        hist[i] = 0;
    }

    for(int y = 0; y < height; ++y )
    {

        for(int x = 0; x < width; ++x )
        {
            if(dataMat.data[y*width+x] != 0)
            {
                ++hist[dataMat.data[y*width+x]];
                sz++;
            }
        }
    }

    for( int i = 0; i < 256; ++i )
    {
        sum += hist[i];
        cdf[i] = sum / float(sz);
    }
    sz = 0;

    for( int i = 0; i < 256; ++i )
    {
        mapping[i] = 255 * cdf[i] + 0.5;
        if( mapping[i] > 255 )
            mapping[i] = 255;
    }

    for( int y = 0; y < height; y++ )
    {
        for( int x = 0; x < width; x++ )
        {
            int a;
            a=dataMat.data[y*width+x];
            if( dataMat.data[y*width+x] != 0 )
                dataMat.data[y*width+x] = mapping[a];
        }
    }
    
};
