//
//  thinning.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "thinning.hpp"

bool condition1 (int pixelData[], int width,int x, int y )
{
    int NumOfNonzero = 0;

    for( int j = -1; j < 2; j++ )
    {
        for( int i = -1; i < 2; i++ )
        {
            if( i != 0 && j != 0 && pixelData[(y+j)*width+(x+i)] == 1 )     //neighbor 8 pixels
                NumOfNonzero++;
        }
    }

    if( NumOfNonzero >= 2 && NumOfNonzero <= 6 )
        return true;
    else
        return false;
}

bool condition2 (int pixelData[], int width,int x, int y )
{
    int NumOfChange = 0;

    for( int m = -1; m < 1; m++ )
    {
        if( (pixelData[(y-1)*width+(x+m+1)] - pixelData[(y-1)*width+(x+m)]) == 1 )
            NumOfChange++;
        if( (pixelData[(y+m+1)*width+(x+1)] - pixelData[(y+m)*width+(x+1)]) == 1 )
            NumOfChange++;
        if( (pixelData[(y+1)*width+(x+m)] - pixelData[(y+1)*width+(x+m+1)]) == 1 )
            NumOfChange++;
        if( (pixelData[(y+m)*width+(x-1)] - pixelData[(y+m+1)*width+(x-1)]) == 1 )      //clock-wise down-up==1
            NumOfChange++;
    }

    if( NumOfChange == 1 )
        return true;
    else
        return false;
}

bool condition3_a (int pixelData[], int width, int x, int y)
{
    if( (pixelData[(y-1)*width+x])*(pixelData[y*width+(x+1)])*(pixelData[(y+1)*width+x]) == 0//right
       &&(pixelData[(y-1)*width+x])*(pixelData[y*width+(x-1)])*(pixelData[y*width+(x+1)]) == 0 )//top
        return true;
    else
        return false;
}
bool condition3_b (int pixelData[], int width, int x, int y)
{
    if( (pixelData[y*width+(x-1)])*(pixelData[y*width+(x+1)])*(pixelData[(y+1)*width+x]) == 0//button
       &&(pixelData[(y-1)*width+x])*(pixelData[y*width+(x-1)])*(pixelData[(y+1)*width+x]) == 0 )//left
        return true;
    else
        return false;
}

void thinning(Mat dataMat)
{
    bool run;
    uchar *ptr;
    int width =dataMat.cols;//320
    int height=dataMat.rows;//240
    int pixelData[width*height];
    int thinTemp1[width*height];
    int thinTemp2[width*height];

    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelData[y*width+x] = ptr[x];
        }
    }


    for( int i = 0; i < width*height; i++)
        pixelData[i]  = ((pixelData[i]==0)?1:0);

    do{
        run = false;
        for( int y = 1; y < height-1; y++ )
        {
            for( int x = 1; x < width-1; x++ )
            {
                if( pixelData[y*width+x] == 1 && condition1(pixelData, width, x, y)
                   && condition2(pixelData , width, x, y) && condition3_a(pixelData, width, x, y))
                {
                    thinTemp1[y*width+x] = 0;
                    run = true;
                }
                else
                {
                    thinTemp1[y*width+x] = pixelData[y*width+x];
                }
                thinTemp2[y*width+x] = thinTemp1[y*width+x];
            }
        }

        for( int y = 1; y < height-1; y++ )
        {
            for( int x = 1; x < width-1; x++ )
            {
                if( thinTemp1[y*width+x] == 1 && condition1(thinTemp1, width, x, y)
                   && condition2(thinTemp1, width, x, y) && condition3_b(thinTemp1, width, x, y) )
                {
                    thinTemp2[y*width+x] = 0;
                    run = true;
                }
            }
        }
        for( int y = 1; y < height-1; y++ )
            for( int x = 1; x < width-1; x++ )
                pixelData[y*width+x] = thinTemp2[y*width+x];

    }while(run == true);

    for( int i = 0; i < width*height; i++ )
        pixelData[i] = ((pixelData[i]==1) ? 0 : 255);

    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            ptr[x] = pixelData[y*width+x];
        }
    }
    
//    imshow("Thinning", dataMat);
}
