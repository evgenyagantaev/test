#pragma once

#include "thermometry.hpp"



class space_transformer
{
private:

    // transformation matrices

    cv::Mat A;
    cv::Mat b;

    // x offset function parameters
    double alpha;
    double beta;
    // y = alpha + beta/x
    
public:

    space_transformer();
    
    cv::Point transform(cv::Point point, double distance);
    
};

