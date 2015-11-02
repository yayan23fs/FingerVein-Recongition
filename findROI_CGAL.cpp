//
//  Algorithm.cpp
//  OpencvTest
//
//  Created by Leque on 2015/7/6.
//  Copyright (c) 2015年 LQ. All rights reserved.
//

#include "Algorithm.h"
#include <math.h>
#include "SkeletonNodes.h"
//#include <omp.h>

//ROI
void findROI_CGAL(Mat dataMat, int block_remove_undesir,
                  int numberCutBlackLine, int numberFillBlackLine) {

    int width = dataMat.cols;
    int height = dataMat.rows;
    double  max_up = 0, max_down = 0;
    int    max_up_Position = 0, max_down_Position = 0;
    int    count_remove_undesir = 0;

    Mat pixelDataTmp;
    dataMat.copyTo(pixelDataTmp);

    // Find initial ROI mask by using biggest gradient
    for( int x = 0; x < width; x++ )
    {
        for( int y = height/2; y > 0; y-- )
        {
            if( dataMat.data[y*width+x] - dataMat.data[(y-1)*width+x] > max_up)
            {
                max_up = (dataMat.data[y*width+x] - dataMat.data[(y-1)*width+x]);
                max_up_Position = y;
            }
        }
        max_up = 0;

        for( int y = height/2; y < height-1; y++ )
        {
            if( dataMat.data[y*width+x] - dataMat.data[(y+1)*width+x] > max_down)
            {
                max_down = (dataMat.data[y*width+x] - dataMat.data[(y+1)*width+x]);
                max_down_Position = y;
            }
        }
        max_down = 0;

        for( int y = 0; y < height; y++ )
        {
            if( y < max_up_Position )
                dataMat.data[y*width+x] = 0;
            if( y > max_down_Position )
                dataMat.data[y*width+x] = 0;
        }

        max_up_Position = 0;
        max_down_Position = 0;
    }

    // Remove undesirable mask
    for( int y = block_remove_undesir; y < height-block_remove_undesir; y++ )
    {
        for( int x = block_remove_undesir; x < width-block_remove_undesir; x++ )
        {
            for( int un = (-1)*block_remove_undesir; un <= block_remove_undesir; un++ )
            {
                if( dataMat.data[(y+un)*width+(x+un)] == 0 )
                    count_remove_undesir++;
            }
            if( count_remove_undesir >= ((block_remove_undesir+1)*(block_remove_undesir+1))/2 )
                dataMat.data[y*width+x] = 0;
            count_remove_undesir = 0;
        }
    }

    // Remove unnecessary black line
    for( int y = 0; y < height; y++ )
    {
        for( int num_cut = 1; num_cut < numberCutBlackLine; num_cut++ )
            for( int x = 0; x < width; x++ )
            {
                if( dataMat.data[y*width+x] == 0 )
                {
                    if( (dataMat.data[y*width+x-1] != 0) && (dataMat.data[y*width+x+num_cut] != 0))
                        dataMat.data[y*width+x] = pixelDataTmp.data[y*width+x];
                }
            }
    }


    // Fill necessary line
    for( int y = 0; y < height; y++ )
    {
        for( int num_fill = 1; num_fill < numberFillBlackLine; num_fill++ )
            for( int x = 0; x < width; x++ )
            {
                if( dataMat.data[y*width+x] != 0 )
                {
                    if( (dataMat.data[y*width+x-1] == 0) && (dataMat.data[y*width+x+num_fill] == 0))
                        dataMat.data[y*width+x] = 0;
                }
            }
    }

    //    return dataMat;
}



