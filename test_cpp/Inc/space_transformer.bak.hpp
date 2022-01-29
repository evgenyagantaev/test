#pragma once

#include "thermometry.hpp"



class space_transformer
{
private:

    // distances from cameras to object
    double d1, d2;

    // coordinates of four  points on distance d1 in rgb-camera frame

    cv::Mat x1_d1;
    cv::Mat x2_d1;
    cv::Mat x3_d1;
    cv::Mat x4_d1;

    // coordinates of those points in thermo-cam frame

    cv::Mat y1_d1;
    cv::Mat y2_d1;
    cv::Mat y3_d1;
    cv::Mat y4_d1;

    // coordinates of four  points on distance d2 in rgb-camera frame

    cv::Mat x1_d2;
    cv::Mat x2_d2;
    cv::Mat x3_d2;
    cv::Mat x4_d2;

    // coordinates of those points in thermo-cam frame

    cv::Mat y1_d2;
    cv::Mat y2_d2;
    cv::Mat y3_d2;
    cv::Mat y4_d2;

    // transformation matrices

    cv::Mat A;
    cv::Mat b;
    cv::Mat A0;
    cv::Mat K;
    cv::Mat b0;
    cv::Mat m;

    cv::Mat AA; 
    cv::Mat AA1;
    cv::Mat BB; 
    cv::Mat BB1;
    
public:

    space_transformer();
    void calibrate(void);
    void set_calibration_data(double distance1, double distance2, cv::Point *X1, cv::Point *Y1, cv::Point *X2, cv::Point *Y2);

    void manual_calibrate();
    
    // method transforms point from rgb-cam frame cpace into thermo-cam frame space
    cv::Point transform(cv::Point point, double distance);
    cv::Point transform(cv::Point point);

    void print_parameters(void);
};

