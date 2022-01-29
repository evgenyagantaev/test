#include "space_transformer.hpp"

using namespace cv;


space_transformer::space_transformer()
{
    // transformation matrices

    A = Mat(2, 2, CV_64F);
    b = Mat(2, 1,  CV_64F);

    // set calibration data ***

    double coeff = 0.45;
    double rotation = 0.01;
    
    double y_offset = 4;

    A.at<double>(0,0) = coeff;
    A.at<double>(1,1) = coeff;
    A.at<double>(0,1) = rotation;
    A.at<double>(1,0) = -rotation;

    b.at<double>(1,0) = y_offset;


    alpha = -12.8467;
    beta = -32.9572;

    
}




cv::Point space_transformer::transform(cv::Point point, double d)
{
    double x_offset;

    Mat x(2, 1,  CV_64F);
    Mat y(2, 1,  CV_64F);

    x_offset = alpha + beta/d;

    b.at<double>(0,0) = (int)x_offset;

    x.at<double>(0,0) = point.x;
    x.at<double>(1,0) = point.y;

    y = A * x + b;

    Point transform_result(y.at<double>(0,0), y.at<double>(1,0));

    return transform_result;
}

