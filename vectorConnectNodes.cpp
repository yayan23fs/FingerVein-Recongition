//
//  vectorConnectNodes.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "vectorConnectNodes.hpp"

float vectorConnectNodes(int pixelData[], int height, int width, int pos_x_in, int pos_y_in,int pos_x_original,
                         int pos_y_original, int pos_x_last, int pos_y_last,SkeletonNodes skelPoint[], int numberNodes)
{
    float score = 0;
    int   cntStatus = 0;
    for( int m = -1; m <= 1; m++ )
        for( int n = -1; n <= 1; n++ )
            if( (m != 0) && (n != 0) )
            {
                if( (pixelData[(pos_y_last+m)*width+(pos_x_last+n)] != 255) &&
                   (((pos_y_last+m) != pos_y_original) && ((pos_x_last+n) != pos_x_original)) )
                {
                    for( int c = 0; c < numberNodes; c++ )
                    {
                        if( ((pos_y_last+m) == skelPoint[c].getY()) && ((pos_x_last+n) == skelPoint[c].getX()) )
                        {
                            cntStatus = 1;
                            break;
                        }
                    }
                    if( cntStatus == 1 )
                    {

                        score += sqrt( ((((pos_y_last+m)-pos_y_in)*((pos_y_last+m)-pos_y_in)) +
                                        (((pos_x_last+n)-pos_x_in)*((pos_x_last+n)-pos_x_in))) );
                    }
                    else if( cntStatus == 0 )
                    {
                        // === Length ===
                        //score++;
                        score += vectorConnectNodes(pixelData, height, width, pos_x_in, pos_y_in, pos_x_last,
                                                    pos_y_last, (pos_x_last+n), (pos_y_last+m), skelPoint, numberNodes);
                    }
                    cntStatus = 0;
                }
            }

    return score;
}
