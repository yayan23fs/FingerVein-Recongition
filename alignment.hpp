//
//  alignment.hpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#ifndef alignment_hpp
#define alignment_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "SkeletonNodes.h"
using namespace cv;

Mat alignment(Mat dataMat, Mat refMat,SkeletonNodes skelPoint_input[],SkeletonNodes skelPoint_reference[],
              int numberNodes_inputImg,int numberNodes_referenceImg,int block_searchRange);

#endif /* alignment_hpp */
