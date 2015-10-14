//
//  Algorithm.h
//  OpencvTest
//
//  Created by Leque on 2015/7/8.
//  Copyright (c) 2015年 LQ. All rights reserved.
//

#ifndef OpencvTest_Algorithm_h
#define OpencvTest_Algorithm_h

#include <opencv2/opencv.hpp>
#include "SkeletonNodes.h"

using namespace std;
using namespace cv;
/*------------------------------------------------------------*/
Mat getHistImg(const MatND& hist);
void findROI_CGAL(Mat dataMat, int block_remove_undesir,
                  int numberCutBlackLine, int numberFillBlackLine);
void ContrastEnhancement(Mat dataMat);
void localContrastGlobal(Mat dataMat, int maskSize);
void binarization(Mat dataMat,int T_Bin, int maskSize);
void PGF(Mat dataMat , int blockSize);
void thinning(Mat dataMat);
int findNodes(Mat dataMat, int block_showPoints, SkeletonNodes skelPoints[]);
/*------------------------------------------------------------*/

#endif
