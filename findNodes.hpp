//
//  findNodes.hpp
//  OpencvTest
//
//  Created by Leque on 2015/11/2.
//  Copyright © 2015年 LQ. All rights reserved.
//

#ifndef findNodes_hpp
#define findNodes_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "SkeletonNodes.h"

using namespace cv;

int findNodes(Mat dataMat, int block_showPoints, SkeletonNodes skelPoints[]);

#endif /* findNodes_hpp */
