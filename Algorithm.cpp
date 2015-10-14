//
//  Algorithm.cpp
//  OpencvTest
//
//  Created by Leque on 2015/7/6.
//  Copyright (c) 2015年 LQ. All rights reserved.
//

#include "Algorithm.h"
#include <math.h>
#include "SkeletonNodes.h"
//#include <omp.h>

//ROI
void findROI_CGAL(Mat dataMat, int block_remove_undesir,
                  int numberCutBlackLine, int numberFillBlackLine) {

    int width = dataMat.cols;
    int height = dataMat.rows;
    double  max_up = 0, max_down = 0;
    int    max_up_Position = 0, max_down_Position = 0;
    int    count_remove_undesir = 0;

    Mat pixelDataTmp;
    dataMat.copyTo(pixelDataTmp);

    // Find initial ROI mask by using biggest gradient
    for( int x = 0; x < width; x++ )
    {
        for( int y = height/2; y > 0; y-- )
        {
            if( dataMat.data[y*width+x] - dataMat.data[(y-1)*width+x] > max_up)
            {
                max_up = (dataMat.data[y*width+x] - dataMat.data[(y-1)*width+x]);
                max_up_Position = y;
            }
        }
        max_up = 0;

        for( int y = height/2; y < height-1; y++ )
        {
            if( dataMat.data[y*width+x] - dataMat.data[(y+1)*width+x] > max_down)
            {
                max_down = (dataMat.data[y*width+x] - dataMat.data[(y+1)*width+x]);
                max_down_Position = y;
            }
        }
        max_down = 0;

        for( int y = 0; y < height; y++ )
        {
            if( y < max_up_Position )
                dataMat.data[y*width+x] = 0;
            if( y > max_down_Position )
                dataMat.data[y*width+x] = 0;
        }

        max_up_Position = 0;
        max_down_Position = 0;
    }

    // Remove undesirable mask
    for( int y = block_remove_undesir; y < height-block_remove_undesir; y++ )
    {
        for( int x = block_remove_undesir; x < width-block_remove_undesir; x++ )
        {
            for( int un = (-1)*block_remove_undesir; un <= block_remove_undesir; un++ )
            {
                if( dataMat.data[(y+un)*width+(x+un)] == 0 )
                    count_remove_undesir++;
            }
            if( count_remove_undesir >= ((block_remove_undesir+1)*(block_remove_undesir+1))/2 )
                dataMat.data[y*width+x] = 0;
            count_remove_undesir = 0;
        }
    }

    // Remove unnecessary black line
    for( int y = 0; y < height; y++ )
    {
        for( int num_cut = 1; num_cut < numberCutBlackLine; num_cut++ )
            for( int x = 0; x < width; x++ )
            {
                if( dataMat.data[y*width+x] == 0 )
                {
                    if( (dataMat.data[y*width+x-1] != 0) && (dataMat.data[y*width+x+num_cut] != 0))
                        dataMat.data[y*width+x] = pixelDataTmp.data[y*width+x];
                }
            }
    }


    // Fill necessary line
    for( int y = 0; y < height; y++ )
    {
        for( int num_fill = 1; num_fill < numberFillBlackLine; num_fill++ )
            for( int x = 0; x < width; x++ )
            {
                if( dataMat.data[y*width+x] != 0 )
                {
                    if( (dataMat.data[y*width+x-1] == 0) && (dataMat.data[y*width+x+num_fill] == 0))
                        dataMat.data[y*width+x] = 0;
                }
            }
    }

    //    return dataMat;
}

//AverageBrintness
unsigned int AverageBrintness(Mat& image){
    int brintness=0;
    int width =image.cols;//320
    int height=image.rows;//240

    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            brintness=brintness+image.data[y*width+x];
        }
    }
    return brintness/(width*height);
}


//Salt Algorithm
void salt(Mat& image, int n)
{
    for(int k=0; k<n; k++)
    {
        int i = rand()%image.cols;
        int j = rand()%image.rows;

        if(image.channels() == 1)
        {
            image.at<uchar>(j,i) = 255;
        }
        else
        {
            image.at<Vec3b>(j,i)[0] = 255;
            image.at<Vec3b>(j,i)[1] = 255;
            image.at<Vec3b>(j,i)[2] = 255;
        }
    }
}

//show histogram , in put "hist"
Mat getHistImg(const MatND& hist)
{
    double maxVal=0;
    double minVal=0;

    //找到直方图中的最大值和最小值
    minMaxLoc(hist,&minVal,&maxVal,0,0);
    int histSize=hist.rows; //256

    Mat histImg(histSize,histSize,CV_8U,Scalar(255));
    // 设置最大峰值为图像高度的90%
    int hpt=static_cast<int>(0.9*histSize);

    for(int h=0;h<histSize;h++)
    {
        float binVal=hist.at<float>(h);
        int intensity=static_cast<int>(binVal*hpt/maxVal);
        line(histImg,Point(h,histSize),Point(h,histSize-intensity),Scalar::all(0));
    }

    return histImg;
}

Mat DFT(Mat I){
    Mat padded;                            //expand inputimage to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); //on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows,0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] ={Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another planewith zeros

    dft(complexI, complexI);            // this way the result may fit inthe source matrix

    // compute the magnitude and switch tologarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 +Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I),planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);//planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmicscale
    log(magI, magI);

    // crop the spectrum, if it has an oddnumber of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2,magI.rows & -2));

    // rearrange the quadrants of Fourierimage  so that the origin is at the imagecenter
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); //Bottom-Right

    Mat tmp;                           // swap quadrants(Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Rightwith Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX);// Transform the matrix with float values into a
    // viewable image form (float betweenvalues 0 and 1).

    //imshow("Input Image"       , I  );    // Show the result
    imshow("spectrum magnitude",magI);
    return magI;
}

//Mat feature_extraction(Mat img_1){
//
//    //-- Step 1: Detect the keypoints using Sift Detector
//    int minHessian = 400;
//    FastFeatureDetector detector(3,3,0.04,10,1.6);
//    //SiftFeatureDetector detector(4,130);
//    vector<KeyPoint> keypoints_1;
//    detector.detect( img_1, keypoints_1 );
//
//    //-- Draw keypoints
//    Mat img_keypoints_1;
//
//    drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//    //-- Show detected (drawn) keypoints
//    imshow("Keypoints 1", img_keypoints_1 );
//    return img_keypoints_1;
//}



void localContrastGlobal(Mat dataMat, int maskSize){
    int width =dataMat.cols;
    int height=dataMat.rows;
    int size=width*height;
    Mat pixelData ;
    Mat pixelDataTmp ;
    dataMat.copyTo(pixelData);
    //    //中心點位置
    for(int y = maskSize; y < height - maskSize; y++)
    {
        for(int x = maskSize; x < width - maskSize; x++)
        {
            int sum = 0;
            int hist [256]={0};
            int grayTmp;
            double grayNow;

            //統計mask中的直方圖
            for(int j = y - maskSize; j <= y + maskSize; j++)
            {
                for(int i = x - maskSize; i <= x + maskSize; i++)
                    hist[pixelData.data[j*width+i]]++;//累積直方圖
            }

            grayTmp = pixelData.data[y*width+x];

            for(int i = 0; i <= grayTmp; i++)
                sum += hist[i];
            grayNow = 255 * sum/((2*maskSize+1)*(2*maskSize+1));

            if((int)grayNow > 255)
                grayNow = 255;
            else if((int)grayNow < 0)
                grayNow = 0;
            dataMat.data[y*width+x] = (int)grayNow;
        }
    }


    imshow("localContrastGlobal", dataMat );

}


void ContrastEnhancement(Mat dataMat){
    int width =dataMat.cols;
    int height=dataMat.rows;
    int   hist[256];
    float cdf[256];
    int   sum = 0;
    int   sz = 0;
    int   mapping[256];

    for(int i = 0; i < 256; ++i )
    {
        hist[i] = 0;
    }

    for(int y = 0; y < height; ++y )
    {

        for(int x = 0; x < width; ++x )
        {
            if(dataMat.data[y*width+x] != 0)
            {
                ++hist[dataMat.data[y*width+x]];
                sz++;
            }
        }
    }

    for( int i = 0; i < 256; ++i )
    {
        sum += hist[i];
        cdf[i] = sum / float(sz);
    }
    sz = 0;

    for( int i = 0; i < 256; ++i )
    {
        mapping[i] = 255 * cdf[i] + 0.5;
        if( mapping[i] > 255 )
            mapping[i] = 255;
    }

    for( int y = 0; y < height; y++ )
    {
        for( int x = 0; x < width; x++ )
        {
            int a;
            a=dataMat.data[y*width+x];
            if( dataMat.data[y*width+x] != 0 )
                dataMat.data[y*width+x] = mapping[a];
        }
    }

};
void binarization(Mat dataMat,int T_Bin, int maskSize)
{
    int width =dataMat.cols;
    int height=dataMat.rows;

    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
        {
            if(dataMat.data[width*y+x]>T_Bin)
                dataMat.data[width*y+x]=255;
            else if((x < maskSize || x >= width - maskSize) || (y < maskSize || y >= height - maskSize))
                dataMat.data[y*width+x] = 255;
            else
                dataMat.data[width*y+x]=0;
        }
    }
    imshow("binarization", dataMat);

}
//void PGF(Mat dataMat, int blockSize)
//{
////    Mat dataMat = src.getMat(ACCESS_WRITE);
//    int width = dataMat.cols;
//    int height = dataMat.rows;
//    uchar *ptr;
//    int data[width*height];
//    int cntBlack = 0;
//    int cntWhite = 0;
//
//    for(int y = 0; y < height; y++){
//        ptr = dataMat.ptr<uchar>(y);
//        for(int x = 0; x < width; x++){
//            data[y*width+x] = ptr[x];
//        }
//    }
//
//    for( int y = blockSize; y < height-blockSize; y++ )
//    {
//        for( int x = blockSize; x < width-blockSize; x++ )
//        {
//            for( int m = -1*blockSize; m <= blockSize; m++ )
//            {
//                for( int n = -1*blockSize; n <= blockSize; n++ )
//                {
//                    if(data[(y+m)*width+(x+n)] == 0 )
//                        cntBlack++;
//                    else if(data[(y+m)*width+(x+n)] == 255)
//                        cntWhite++;
//                }
//            }
//            if( cntBlack > cntWhite )
//            {
//                data[y*width+x] = 0;
//            }
//            else if( cntBlack < cntWhite )
//            {
//                data[y*width+x] = 255;
//            }
//            cntBlack = cntWhite = 0;
//        }
//    }
//
//    for(int y = 0; y < height; y++){
//        ptr = dataMat.ptr<uchar>(y);
//        for(int x = 0; x < width; x++){
//            ptr[x] = data[y*width+x];
//        }
//    }
//
//    imshow("PFG", dataMat);
//}
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

bool condition1 (int pixelData[], int width,int x, int y )
{
    int NumOfNonzero = 0;

    for( int j = -1; j < 2; j++ )
    {
        for( int i = -1; i < 2; i++ )
        {
            if( i != 0 && j != 0 && pixelData[(y+j)*width+(x+i)] == 1 )     //neighbor 8 pixels
                NumOfNonzero++;
        }
    }

    if( NumOfNonzero >= 2 && NumOfNonzero <= 6 )
        return true;
    else
        return false;
}

bool condition2 (int pixelData[], int width,int x, int y )
{
    int NumOfChange = 0;

    for( int m = -1; m < 1; m++ )
    {
        if( (pixelData[(y-1)*width+(x+m+1)] - pixelData[(y-1)*width+(x+m)]) == 1 )
            NumOfChange++;
        if( (pixelData[(y+m+1)*width+(x+1)] - pixelData[(y+m)*width+(x+1)]) == 1 )
            NumOfChange++;
        if( (pixelData[(y+1)*width+(x+m)] - pixelData[(y+1)*width+(x+m+1)]) == 1 )
            NumOfChange++;
        if( (pixelData[(y+m)*width+(x-1)] - pixelData[(y+m+1)*width+(x-1)]) == 1 )      //clock-wise down-up==1
            NumOfChange++;
    }

    if( NumOfChange == 1 )
        return true;
    else
        return false;
}

bool condition3_a (int pixelData[], int width, int x, int y)
{
    if( (pixelData[(y-1)*width+x])*(pixelData[y*width+(x+1)])*(pixelData[(y+1)*width+x]) == 0//right
       &&(pixelData[(y-1)*width+x])*(pixelData[y*width+(x-1)])*(pixelData[y*width+(x+1)]) == 0 )//top
        return true;
    else
        return false;
}
bool condition3_b (int pixelData[], int width, int x, int y)
{
    if( (pixelData[y*width+(x-1)])*(pixelData[y*width+(x+1)])*(pixelData[(y+1)*width+x]) == 0//button
       &&(pixelData[(y-1)*width+x])*(pixelData[y*width+(x-1)])*(pixelData[(y+1)*width+x]) == 0 )//left
        return true;
    else
        return false;
}

void thinning(Mat dataMat)
{
    bool run;
    uchar *ptr;
    int width =dataMat.cols;//320
    int height=dataMat.rows;//240
    int pixelData[width*height];
    int thinTemp1[width*height];
    int thinTemp2[width*height];

    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelData[y*width+x] = ptr[x];
        }
    }

    for( int i = 0; i < width*height; i++)
        pixelData[i]  = ((pixelData[i]==0)?1:0);

    do{
        run = false;
        for( int y = 1; y < height-1; y++ )
        {
            for( int x = 1; x < width-1; x++ )
            {
                if( pixelData[y*width+x] == 1 && condition1(pixelData, width, x, y)
                   && condition2(pixelData , width, x, y) && condition3_a(pixelData, width, x, y))
                {
                    thinTemp1[y*width+x] = 0;
                    run = true;
                }
                else
                {
                    thinTemp1[y*width+x] = pixelData[y*width+x];
                }
                thinTemp2[y*width+x] = thinTemp1[y*width+x];
            }
        }

        for( int y = 1; y < height-1; y++ )
        {
            for( int x = 1; x < width-1; x++ )
            {
                if( thinTemp1[y*width+x] == 1 && condition1(thinTemp1, width, x, y)
                   && condition2(thinTemp1, width, x, y) && condition3_b(thinTemp1, width, x, y) )
                {
                    thinTemp2[y*width+x] = 0;
                    run = true;
                }
            }
        }
        for( int y = 1; y < height-1; y++ )
            for( int x = 1; x < width-1; x++ )
                pixelData[y*width+x] = thinTemp2[y*width+x];

    }while(run == true);

    for( int i = 0; i < width*height; i++ )
        pixelData[i] = ((pixelData[i]==1) ? 0 : 255);
    
    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            ptr[x] = pixelData[y*width+x];
        }
    }
    
    imshow("Thinning", dataMat);
}


int findNodes(Mat dataMat, int block_showPoints, SkeletonNodes skelPoints[])
{

    int width =dataMat.cols;//320
    int height=dataMat.rows;//240
    int pixelData[width*height];
    uchar *ptr;
    int count_skelNodes = 0;
    int status_point = 0;
    int pos_x = 0, pos_y = 0;

    for(int y = 0; y < height; y++){
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++){
            pixelData[y*width+x] = ptr[x];
        }
    }

    for (int y = 1; y < height-1; y++)
    {
        for(int x = 1; x < width-1; x++)
        {
            if(pixelData[y*width+x] == 0)
            {
                if( pixelData[(y-1)*width+(x-1)] != pixelData[(y-1)*width+x] )
                status_point++;
                if( pixelData[(y-1)*width+x]     != pixelData[(y-1)*width+(x+1)] )
                status_point++;
                if( pixelData[(y-1)*width+(x+1)] != pixelData[y*width+(x+1)] )
                status_point++;
                if( pixelData[y*width+(x+1)]     != pixelData[(y+1)*width+(x+1)] )
                status_point++;
                if( pixelData[(y+1)*width+(x+1)] != pixelData[(y+1)*width+x] )
                status_point++;
                if( pixelData[(y+1)*width+x]     != pixelData[(y+1)*width+(x-1)] )
                status_point++;
                if( pixelData[(y+1)*width+(x-1)] != pixelData[y*width+(x-1)] )
                status_point++;
                if( pixelData[y*width+(x-1)]     != pixelData[(y-1)*width+(x-1)] )
                status_point++;

                if( status_point == 2 ) // Ending point
                {
                    skelPoints[count_skelNodes] = *new SkeletonNodes();
                    skelPoints[count_skelNodes].setNumberConnection(status_point / 2);
                    skelPoints[count_skelNodes].setNodeType(1);
                    skelPoints[count_skelNodes].setX(x);
                    skelPoints[count_skelNodes].setY(y);
                    count_skelNodes++;
                }
                else if( status_point >= 6 ) // Bifurcation point
                {
                    skelPoints[count_skelNodes] = *new SkeletonNodes();
                    skelPoints[count_skelNodes].setNumberConnection(status_point / 2);
                    skelPoints[count_skelNodes].setNodeType(2);
                    skelPoints[count_skelNodes].setX(x);
                    skelPoints[count_skelNodes].setY(y);
                    count_skelNodes++;
                }
                status_point = 0;
            }
        }
    }

    // Show skeleton nodes , 4x4 mask on it
    for( int c = 0; c < count_skelNodes; c++ )
    {
        if( skelPoints[c].getNodeType() == 1 )
        {
            pos_x = skelPoints[c].getX();
            pos_y = skelPoints[c].getY();
            for( int m = -1*block_showPoints; m <= block_showPoints; m++ )
            {
                for( int n = -1*block_showPoints; n <= block_showPoints; n++ )
                {
                    if(((pos_y+m) >= 0) && ((pos_x+n) >= 0)
                       && ((pos_y+m) < height) && ((pos_x+n) < width))
                    {
                        pixelData[(pos_y+m)*width+pos_x+n] = 0;
                    }
                }
            }
        }
        if( skelPoints[c].getNodeType() == 2 )
        {
            pos_x = skelPoints[c].getX();
            pos_y = skelPoints[c].getY();
            for( int m = -1*block_showPoints; m <= block_showPoints; m++ )
            {
                for( int n = -1*block_showPoints; n <= block_showPoints; n++ )
                {
                    if(((pos_y+m) >= 0) && ((pos_x+n) >= 0)
                       && ((pos_y+m) < height) && ((pos_x+n) < width))
                    {
                        pixelData[(pos_y+m)*width+pos_x+n] = 160;//+ (-256)
                    }
                }
            }
        }
    }

    for(int y = 0; y < height; y++)
    {
        ptr = dataMat.ptr<uchar>(y);
        for(int x = 0; x < width; x++)
        {
            ptr[x] = pixelData[y*width+x];
        }
    }
    imshow("FindNode", dataMat);

    return count_skelNodes;
}

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

