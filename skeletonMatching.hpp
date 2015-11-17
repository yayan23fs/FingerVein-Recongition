//
//  skeletonMatching.hpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#ifndef skeletonMatching_hpp
#define skeletonMatching_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "SkeletonNodes.h"
#include "vectorConnectNodes.hpp"
//#include "main.cpp"

using namespace cv;


float skeletonMatching(Mat dataMat, Mat refMat, int numberNodes_inputImg, int numberNodes_referenceImg,int block_searchRange, int threshold_pointDist);

float skeletonMatching(Mat dataMat, Mat refMat, int numberNodes_inputImg, int numberNodes_referenceImg,int block_searchRange, int threshold_pointDist, SkeletonNodes skelPoint_i[] , SkeletonNodes skelPoint_r[] );
#endif /* skeletonMatching_hpp */
