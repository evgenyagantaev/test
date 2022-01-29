#include "utilities.hpp"

using namespace cv;
using namespace std;


bool draw_cross(cv::Mat img, int j, int i, uint8_t brightness)
{
    bool success = false;

    if((i>2) && (j>2) && (i<(img.rows - 3)) && (j<(img.cols - 3)))
    {
        img.at<uchar>(i-1,j) = brightness;
        img.at<uchar>(i-2,j) = brightness;
        img.at<uchar>(i-3,j) = brightness;
        img.at<uchar>(i+1,j) = brightness;
        img.at<uchar>(i+2,j) = brightness;
        img.at<uchar>(i+3,j) = brightness;
        img.at<uchar>(i,j-1) = brightness;
        img.at<uchar>(i,j-2) = brightness;
        img.at<uchar>(i,j-3) = brightness;
        img.at<uchar>(i,j+1) = brightness;
        img.at<uchar>(i,j+2) = brightness;
        img.at<uchar>(i,j+3) = brightness;

        success = true;
    }

    return success;
    
}

//Mouse callback

void onMouse( int event, int x, int y, int, void* userInput )
{
    Window_frame *window_frame = (Window_frame *)userInput;


    if( event == EVENT_LBUTTONDOWN )
    {
        draw_cross(window_frame->frame, x, y, BLACK);
        cout << "pixel[" << y << "," << x << "] -> " << (int)(window_frame->frame).at<uchar>(y,x) << endl;
        // Show the result
        imshow(window_frame->window_name, window_frame->frame);

    }
 }
