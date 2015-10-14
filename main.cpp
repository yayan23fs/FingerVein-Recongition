//#include <cv.h>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/features2d.hpp>
//#include <iostream>
//#include <omp.h>
//#include <opencv2/opencv.hpp>
#include "Algorithm.h"

using namespace std;
using namespace cv;

#define threads 4;


int main()
{
    cout<<CV_VERSION;   //test opencv is working

/* ================ Declare variable ======================*/
    char str[50];
    Mat *pics = new Mat[100];

    int counter = 0;    //  計數器

    int numberNodes_inputImg     = 0;   //計算結點數
    int numberNodes_referenceImg = 0;


    SkeletonNodes skelPoint_i[100];
    SkeletonNodes skelPoint_r[100];


    clock_t start ,finish;
    
/*=========================================================*/

    //連續讀圖
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<10;j++)
        {
            sprintf(str,"/Users/Leque/Desktop/photo/cgalfinger%d%d.bmp",i,j);//產生連續圖片
            pics[counter]=imread(str,0);
            cout<<str<<endl;
            counter++;
            if(i==2 & j==0)break;
        }
    }
    imshow("Original",pics[1]);

    //算每張圖的ROI
//    #pragma omp parallel for num_threads(8)
    for(int i=1;i<=counter;i++){
        findROI_CGAL(pics[i], 2, 70, 70);
    }
    imshow("ROI",pics[1]);

//    ContrastEnhancement(pics[2]);
    localContrastGlobal(pics[1],20);
    binarization(pics[1],90,30);
    PGF(pics[1], 2);
    thinning(pics[1]);
    numberNodes_inputImg = findNodes(pics[1], 2, skelPoint_i);
    cout<<numberNodes_inputImg;




    waitKey(0);
    delete [] pics;
    return 0;
}


