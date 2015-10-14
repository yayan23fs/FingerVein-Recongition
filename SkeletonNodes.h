//
//  SkeletonNodes.h
//  OpencvTest
//
//  Created by Leque on 2015/7/8.
//  Copyright (c) 2015年 LQ. All rights reserved.
//

#ifndef OpencvTest_SkeletonNodes_h
#define OpencvTest_SkeletonNodes_h

class SkeletonNodes{
    private:
        int lengthConnection = 0;
        int numberConnection = 0;
        int nodeType = 0;
        int x = 0;
        int y = 0;
    
    void init()
    {
        setLengthConnection(0);
        setNumberConnection(0);
        setNodeType(0);
        setX(0);
        setY(0);
    }
    public:
     int getLengthConnection() {
        return lengthConnection;
    }
     void setLengthConnection(int lengthConnection) {
        this->lengthConnection = lengthConnection;
    }

     int getNumberConnection() {
        return numberConnection;
    }

     void setNumberConnection(int numberConnection) {
        this->numberConnection = numberConnection;
    }

     int getNodeType() {
        return nodeType;
    }

     void setNodeType(int nodeType) {
        this->nodeType = nodeType;
    }

     int getX() {
        return x;
    }

     void setX(int x) {
        this->x = x;
    }
    
     int getY() {
        return y;
    }
    
     void setY(int y) {
        this->y = y;
    }
};

#endif
