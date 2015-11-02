//
//  skeletonMatching.cpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#include "skeletonMatching.hpp"

float skeletonMatching(Mat dataMat, Mat refMat, int numberNodes_inputImg, int numberNodes_referenceImg,int block_searchRange, int threshold_pointDist)
{
    int width =dataMat.cols;//320
    int height=dataMat.rows;//240
    int pixelDataIn[width*height];
    int pixelDataRef[width*height];
    SkeletonNodes skelPoint_i[100];
    SkeletonNodes skelPoint_r [100];


    uchar *ptr;
    int   pos1_x = 0, pos1_y = 0, pos2_x = 0, pos2_y = 0;
    float node_vectorScore_input = 0, node_vectorScore_reference = 0;
    int   number_connect_i = 0, number_connect_r = 0;
    int   cnt_match = 0;
    float count_matchedSkeletonNodes = 0;
    float score_matching = 0;

    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelDataIn[y*width+x] = ptr[x];
        }
    }
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelDataRef[y*width+x] = ptr[x];
        }
    }

    for( int cnt_i = 0; cnt_i < numberNodes_inputImg; cnt_i++ )
    {
        pos1_x = skelPoint_i[cnt_i].getX();
        pos1_y = skelPoint_i[cnt_i].getY();
        number_connect_i = skelPoint_i[cnt_i].getNumberConnection();

        for( int cnt_r = 0; cnt_r < numberNodes_referenceImg; cnt_r++ )
        {
            pos2_x = skelPoint_r[cnt_r].getX();
            pos2_y = skelPoint_r[cnt_r].getY();
            number_connect_r = skelPoint_r[cnt_r].getNumberConnection();

            if((sqrt((pos2_y-pos1_y)*(pos2_y-pos1_y)+(pos2_x-pos1_x)*(pos2_x-pos1_x))
                <= block_searchRange )
               && (number_connect_i == number_connect_r) )
            {
                node_vectorScore_input = vectorConnectNodes(pixelDataIn, height, width, pos1_x, pos1_y,
                                                            pos1_x, pos1_y, pos1_x, pos1_y, skelPoint_i, numberNodes_inputImg);
                node_vectorScore_reference = vectorConnectNodes(pixelDataRef, height, width, pos2_x, pos2_y,
                                                                pos2_x, pos2_y, pos2_x, pos2_y, skelPoint_r, numberNodes_referenceImg);

                if(abs(node_vectorScore_input - node_vectorScore_reference) < threshold_pointDist )
                    cnt_match = 1;
                if( cnt_match == 1 )
                {
                    count_matchedSkeletonNodes++;
                    cnt_match = 0;
                    break;
                }
            }
        }
    }

    score_matching = (float)(count_matchedSkeletonNodes / numberNodes_inputImg);
    
    
    return score_matching;
}
