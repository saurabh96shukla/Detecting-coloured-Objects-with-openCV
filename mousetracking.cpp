#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap(0);
    
    if(!cap.isOpened())
    {
        cout<<"error..."<<endl;
        return -1;
    }
    
    namedWindow("Video", 1);
    
    Mat frame;
    Mat hsv;
    Mat thresholded;
    Mat flipped=Mat(frame.rows,frame.cols,CV_8UC3);
    
    int iLastX= -1;
    int iLastY = -1;
    
    while(1)
    {
        cap>>frame;
        flip(frame,flipped,1);
        cvtColor(flipped, hsv, COLOR_BGR2HSV);
        inRange(hsv, Scalar(0,100,100), Scalar(10, 255,255), thresholded);
        inRange(hsv, Scalar(160,100,100), Scalar(179,255,255), thresholded);
        
        //doing morphological opening
        erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
        dilate(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
        
        //doing morphological closing
        dilate(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
        erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
        
        Moments oMoments = moments(thresholded);
        
        double M01= oMoments.m01;
        double M10= oMoments.m10;
        double dArea=oMoments.m00;
        
        if(dArea>10000)
        {
            int posX = M10/dArea;
            int posY = M01/dArea;
            
            if(iLastX>=0 && iLastY>=0 && posX>=0 && posY>=0)
            {
                cout<<"X coordinate ="<<posX<<"\t";
                cout<<"Y coordinate = "<<posY<<endl;
            }
             iLastX=posX;
             iLastY=posY;
         }
        
        
        
        
        imshow("Video", thresholded);
        
        waitKey(33);
        
    }
    return 0;
}
    