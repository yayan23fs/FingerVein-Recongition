//
//  vectorConnectNodes.hpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#ifndef vectorConnectNodes_hpp
#define vectorConnectNodes_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "SkeletonNodes.h"

using namespace cv;

float vectorConnectNodes(int pixelData[], int height, int width, int pos_x_in, int pos_y_in,int pos_x_original,
                         int pos_y_original, int pos_x_last, int pos_y_last,SkeletonNodes skelPoint[], int numberNodes);
#endif /* vectorConnectNodes_hpp */
