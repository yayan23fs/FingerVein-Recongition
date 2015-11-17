//
//  findNodes.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "findNodes.hpp"

int findNodes(Mat dataMat, int block_showPoints, SkeletonNodes skelPoints[])
{

    int width =dataMat.cols;//320
    int height=dataMat.rows;//240
    int pixelData[width*height];
    uchar *ptr;
    int count_skelNodes = 0;
    int status_point = 0;
    int pos_x = 0, pos_y = 0;

    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelData[y*width+x] = ptr[x];
        }
    }

    for (int y = 1; y < height-1; y++)
    {
        for(int x = 1; x < width-1; x++)
        {
            if(pixelData[y*width+x] == 0)
            {
                if( pixelData[(y-1)*width+(x-1)] != pixelData[(y-1)*width+x] )
                    status_point++;
                if( pixelData[(y-1)*width+x]     != pixelData[(y-1)*width+(x+1)] )
                    status_point++;
                if( pixelData[(y-1)*width+(x+1)] != pixelData[y*width+(x+1)] )
                    status_point++;
                if( pixelData[y*width+(x+1)]     != pixelData[(y+1)*width+(x+1)] )
                    status_point++;
                if( pixelData[(y+1)*width+(x+1)] != pixelData[(y+1)*width+x] )
                    status_point++;
                if( pixelData[(y+1)*width+x]     != pixelData[(y+1)*width+(x-1)] )
                    status_point++;
                if( pixelData[(y+1)*width+(x-1)] != pixelData[y*width+(x-1)] )
                    status_point++;
                if( pixelData[y*width+(x-1)]     != pixelData[(y-1)*width+(x-1)] )
                    status_point++;

                if( status_point == 2 ) // Ending point
                {
                    skelPoints[count_skelNodes] = *new SkeletonNodes();
                    skelPoints[count_skelNodes].setNumberConnection(status_point / 2);
                    skelPoints[count_skelNodes].setNodeType(1);
                    skelPoints[count_skelNodes].setX(x);
                    skelPoints[count_skelNodes].setY(y);
                    count_skelNodes++;
                }
                else if( status_point >= 6 ) // Bifurcation point
                {
                    skelPoints[count_skelNodes] = *new SkeletonNodes();
                    skelPoints[count_skelNodes].setNumberConnection(status_point / 2);
                    skelPoints[count_skelNodes].setNodeType(2);
                    skelPoints[count_skelNodes].setX(x);
                    skelPoints[count_skelNodes].setY(y);
                    count_skelNodes++;
                }
                status_point = 0;
            }
        }
    }

    // Show skeleton nodes , 4x4 mask on it
#if 0
    for( int c = 0; c < count_skelNodes; c++ )
    {
        if( skelPoints[c].getNodeType() == 1 )
        {
            pos_x = skelPoints[c].getX();
            pos_y = skelPoints[c].getY();
            for( int m = -1*block_showPoints; m <= block_showPoints; m++ )
            {
                for( int n = -1*block_showPoints; n <= block_showPoints; n++ )
                {
                    if(((pos_y+m) >= 0) && ((pos_x+n) >= 0)
                       && ((pos_y+m) < height) && ((pos_x+n) < width))
                    {
                        pixelData[(pos_y+m)*width+pos_x+n] = 0;
                    }
                }
            }
        }
        if( skelPoints[c].getNodeType() == 2 )
        {
            pos_x = skelPoints[c].getX();
            pos_y = skelPoints[c].getY();
            for( int m = -1*block_showPoints; m <= block_showPoints; m++ )
            {
                for( int n = -1*block_showPoints; n <= block_showPoints; n++ )
                {
                    if(((pos_y+m) >= 0) && ((pos_x+n) >= 0)
                       && ((pos_y+m) < height) && ((pos_x+n) < width))
                    {
                        pixelData[(pos_y+m)*width+pos_x+n] = 160;//+ (-256)
                    }
                }
            }
        }
    }

    for(int y = 0; y < height; y++)
    {
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++)
        {
            ptr[x] = pixelData[y*width+x];
        }
    }
#endif
//    imshow("FindNode", dataMat);

    return count_skelNodes;
}
