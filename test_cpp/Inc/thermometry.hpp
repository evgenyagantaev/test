#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

#include "opencv_headers.hpp"

#include "thermo_pixel.hpp"
#include "thermo_pixel_vector.hpp"
#include "temperature_range.hpp"
#include "rect_thermal_spot.hpp"

#include "utilities.hpp"
#include "unistd.h"

#define TEMPERATURE_LOW_BOUND 33
#define TEMPERATURE_UP_BOUND 40

#define DEFAULT_ABB_TEMPERATURE 41
#define DEFAULT_BACKGROUND_TEMPERATURE 27

#define EYE_WIDTH_COEFF  (3.0 / 8.0)
#define EYE_HEIGHT_COEFF (3.0 / 8.0)
#define AUX_COEFF (1.0 / 8.0)

#define PRIMARY_RANDOM_POINTS_NUMBER_EYE 8
#define SECONDARY_RANDOM_POINTS_NUMBER_EYE 3
#define SECONDARY_BASES_NUMBER_EYE 3
#define PRIMARY_RANDOM_POINTS_NUMBER_FOREHEAD 23
#define SECONDARY_RANDOM_POINTS_NUMBER_FOREHEAD 7
#define SECONDARY_BASES_NUMBER_FOREHEAD 5

#define RANDOM_GENERATION_TRY_ATTEMPTS 50

#define HEAD_CUT_EYE 1
#define CLASTER_BODY_EYE 7
#define HEAD_CUT_FOREHEAD 2
#define CLASTER_BODY_FOREHEAD 9

#define DEFAULT_T0 19.91
#define DEFAULT_K 0.0897

struct FaceTemperature
{
    double left_eye;
    double right_eye;
    double forehead;

    FaceTemperature(double _left_eye, double _right_eye, double _forehead) :
        left_eye(_left_eye), right_eye(_right_eye), forehead(_forehead) {};

    FaceTemperature() :
        left_eye(0), right_eye(0), forehead(0) {};

    void print()
    {
        cout << "Left eye T=" << left_eye << endl
            << "Right eye T=" << right_eye << endl
            << "Forehead T=" << forehead << endl;
    }
};

typedef struct 
{
    cv::Rect faceRect;
    cv::Point leftEye;
    cv::Point rightEye;
    float yaw;
    float pitch;
    float roll;
} FacePositionInfo;


class thermometry
{
private: 

    cv::Point left_eye_center;
    cv::Point right_eye_center;
    thermo_pixel abb_base_pixel;
    thermo_pixel background_base_pixel;
    thermo_pixel_vector left_eye_claster;
    thermo_pixel_vector right_eye_claster;
    thermo_pixel_vector forehead_claster;
    thermo_pixel_vector secondary_layer_bases;
    rect_thermal_spot left_eye_spot;
    rect_thermal_spot right_eye_spot;
    rect_thermal_spot forehead_spot;

    temperature_range range;

    int manual_mark_counter;

    cv::Mat mark_canvas;
    cv::Mat thermal_field;

    int eyes_span;
    int eye_spot_width;
    int eye_spot_height;
    int eye_spot_base_horizontal_offset;
    int eye_spot_base_vertical_offset;
    int forehead_spot_width;
    int forehead_spot_height;
    int forehead_spot_base_horizontal_offset;
    int forehead_spot_base_vertical_offset;

    bool marked;

    double detect_spot_temperature(rect_thermal_spot *spot, thermo_pixel_vector *claster);
    void secondary_layer_detect(rect_thermal_spot *spot, thermo_pixel_vector *claster);
    void primary_layer_detect(rect_thermal_spot *spot, thermo_pixel_vector *claster);

    //***

    //void mark_frame_manually(current_thermo_frame frame_to_mark);
 
    
public:

    thermometry(void);

    //void mark_frame(current_thermo_frame frame_to_mark);
    void mark_frame(cv::Mat frame_to_mark, int x, int y);

    void set_mark_canvas(cv::Mat value);
    cv::Mat get_mark_canvas(void);
    
    bool is_marked(void);
    void unmark(void);

    double left_eye_detect_temperature();
    double right_eye_detect_temperature();
    double forehead_detect_temperature();

    void legacy_mark(FacePositionInfo facePos);
    
    FaceTemperature detect_temperature(cv::Mat frame, FacePositionInfo facePos);
        
};

