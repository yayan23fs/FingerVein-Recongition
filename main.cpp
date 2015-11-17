
#include <iostream>
//#include <omp.h>
#include "Algorithm.h"
#define threads 4;
#define maxPic 9;

//Skeleton Generate
//#define DB1;
#define DB2;
//#define New_DB;

using namespace std;
using namespace cv;

static double matchingScore_DOWN = 0.00;
static double matchingScore_UP   = 1.00;


int main()
{
    //cout<<CV_VERSION<<endl;   //  Test if OpenCV is working


/* ================ Varable Declaration ======================*/
    Mat refMat ;
    Mat refIQAMat;
    Size size;  //Image size informaiton

    char str_node[50],str_iqa[50],str_saving[50];
    int counter = 0;    //計數器
    int numberNodes_inputImg     = 0;  //計算”輸入“影像結點數
    int numberNodes_referenceImg = 0;  //計算“比對”影像結點數

    SkeletonNodes skelPoint_i[250]; //紀錄輸入影像node資訊物件
    SkeletonNodes skelPoint_r[250]; //紀錄比對影像node資訊物件

    float maxScore = 0;
    float tempScore = 0;
    float iqaMaxScore = 0;
    float iqaTempScore = 0;

    clock_t start ,finish;  //Time estimate

    double iqa_score_cnt=0;
    double skeleton_comp_cnt=0;
    double Skeleton_success_cnt=0;

    int MaxPic=19;

/*=============================================================*/
#if 1
    Mat inputData = imread("/Users/leque/Desktop/DataBase1/cgalfinger_0.bmp",0);//0~3,13
    Mat inputIQAMat;
    imshow("InputIQAMat",inputData);

    findROI_CGAL(inputData, 2, 70, 70);
    inputData.copyTo(inputIQAMat);
    localContrastGlobal(inputData,20);
    binarization(inputData,90,30);
    PGF(inputData, 2);
    thinning(inputData);
    numberNodes_inputImg = findNodes(inputData, 2, skelPoint_i);
    ContrastEnhancement(inputIQAMat);

      imshow("InputIQAMat",inputData);

    //輸入DataBase
    for(int i=0;i<=MaxPic;i++)
    {

        sprintf(str_iqa,"/Users/leque/Desktop/DataBase1/cgalfinger_%d.bmp",i);//iqa
        // // /Users/leque/Desktop/DataBase1/cgalfinger_%d.bmp
        // /Users/leque/Desktop/DataBase2/cgaliqadatabase%d.bmp

        sprintf(str_node,"/Users/leque/Desktop/DataBase1/SK_DB1/DB1_SK_%d.bmp",i);//node
        // /Users/leque/Desktop/DataBase1/SK_DB1/DB1_SK_%d.bmp
        // /Users/leque/Desktop/DataBase2/cgalnodedatabase%d.bmp

        refMat = imread(str_node,0);
        refIQAMat = imread(str_iqa,0);

        numberNodes_referenceImg = findNodes(refMat, 2, skelPoint_r);
        
        // (int block_searchRange, int threshold_pointDist)
        tempScore = skeletonMatching(inputData, refMat, numberNodes_inputImg, numberNodes_referenceImg, 10, 10,  skelPoint_i , skelPoint_r );

                //若skeletonMatching分數介於0~1之間，將data base的圖alignment然後算出IQA分數
        if(tempScore >= matchingScore_DOWN && tempScore <= matchingScore_UP)
        {
            refIQAMat = alignment(inputIQAMat, refIQAMat, skelPoint_i, skelPoint_r,
                                  numberNodes_inputImg, numberNodes_referenceImg, 2);
            iqaTempScore = iqaMatching(inputIQAMat, refIQAMat); //算IQA
            Skeleton_success_cnt++;
        }


        //	skeletonMatching分數>0,比較最高分數，若此項為最高分數高於上一項
        if(tempScore > maxScore )
        {
            iqaMaxScore = iqaTempScore;
            maxScore = tempScore;
            skeleton_comp_cnt++;
            //	skeleton_matching最高分再見茶IQA matching
            if(iqaTempScore > 0.5)
            {
                imshow("refMat",refMat);
                imshow("refIQAMat",refIQAMat);
                cout << "Skeleton matching score is " << maxScore << endl;
                cout << "IQA matching score is " << iqaMaxScore << endl;
                iqa_score_cnt++;
            }
            else
            {
                cout << "Skeleton matching score is " << maxScore << endl;
                cout << "Please try to adjust your finger position." << endl;
            }
        }

    }
//    cout << "Skeleton_success_cnt =" << (Skeleton_success_cnt/(MaxPic+1))*100<<"%"<<endl;
//    cout << "int skeleton_comp_cnt =" << (skeleton_comp_cnt/(MaxPic+1))*100<<"%"<<endl;
//    cout << "iqa_score_cnt =" << (iqa_score_cnt/(MaxPic+1))*100<<"%"<<endl;

#endif





#if 0
//Skeleton Generate


    #ifdef DB1 //0~19
    Mat temp1;
    for(int i=0;i<=19;i++)
    {
        sprintf(str_iqa,"/Users/leque/Desktop/DataBase1/cgalfinger_%d.bmp",i);
        sprintf(str_saving,"/Users/leque/Desktop/DataBase1/SK_DB1/DB1_SK_%d.bmp",i);
        temp1=imread(str_iqa,0);
        findROI_CGAL(temp1, 2, 70, 70);
        localContrastGlobal(temp1,20);
        binarization(temp1,90,30);
        PGF(temp1, 2);
        thinning(temp1);
        imwrite(str_saving,temp1);

    }
    #endif

    #ifdef DB2 //輸入資料庫的圖片 DB2 1~10=10
    Mat temp2;
    for(int i=0;i<=9;i++)
    {
        sprintf(str_iqa,"/Users/leque/Desktop/DataBase2/cgaliqadatabase%d.bmp",i);
        sprintf(str_saving,"/Users/leque/Desktop/DataBase2/cgalnodedatabase%d.bmp",i);
        temp2=imread(str_iqa,0);
        findROI_CGAL(temp2, 2, 70, 70);
        localContrastGlobal(temp2,20);
        binarization(temp2,90,30);
        PGF(temp2, 2);
        thinning(temp2);
        imwrite(str_saving,temp2);

    }
    #endif



    //輸入資料庫的圖片 New_DB 0~295=296
    #ifdef New_DB
    int max_pic_num=295;
    Mat temp;
    for(int i=0;i<=max_pic_num;i++)
    {
        sprintf(str_iqa,"/Users/leque/Desktop/New_DB/vein_DB_%d.bmp",i);
        sprintf(str_saving,"/Users/Leque/Desktop/New_DB/Skeleton_DB/Skeleton%d.bmp",i);
        temp=imread(str_iqa,0);
        findROI_CGAL(temp, 2, 70, 70);
        localContrastGlobal(temp,20);
        binarization(temp,90,30);
        PGF(temp, 2);
        thinning(temp);
        imwrite(str_saving,temp);

    }
    #endif

    cout<<"Generate skeleton done!!!!";

#endif

    
    waitKey(0);
    return 0;
}


