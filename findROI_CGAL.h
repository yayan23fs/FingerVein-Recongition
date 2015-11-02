//
//  Algorithm.h
//  OpencvTest
//
//  Created by Leque on 2015/7/8.
//  Copyright (c) 2015年 LQ. All rights reserved.
//

#ifndef findROI_CGAL_h
#define findROI_CGAL_h

#include <opencv2/opencv.hpp>
#include "SkeletonNodes.h"

using namespace std;
using namespace cv;
/*------------------------------------------------------------*/

void findROI_CGAL(Mat dataMat, int block_remove_undesir,
                  int numberCutBlackLine, int numberFillBlackLine);


/*------------------------------------------------------------*/

#endif
