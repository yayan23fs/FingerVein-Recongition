//
//  PGF.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "PGF.hpp"

void PGF(Mat dataMat , int blockSize)
{
    int width =dataMat.cols;//320
    int height=dataMat.rows;//240
    int cntBlack = 0;
    int cntWhite = 0;

    for( int y = blockSize; y < height-blockSize; y++ )
    {
        for( int x = blockSize; x < width-blockSize; x++ )
        {
            for( int m = -1*blockSize; m <= blockSize; m++ )
            {
                for( int n = -1*blockSize; n <= blockSize; n++ )
                {
                    if(dataMat.data[(y+m)*width+(x+n)] == 0 )
                        cntBlack++;
                    else if(dataMat.data[(y+m)*width+(x+n)] == 255)
                        cntWhite++;
                }
            }
            if( cntBlack > cntWhite )
            {
                dataMat.data[y*width+x] = 0;
            }
            else if( cntBlack < cntWhite )
            {
                dataMat.data[y*width+x] = 255;
            }
            cntBlack = cntWhite = 0;
        }
    }

    imshow("PFG", dataMat);
}
