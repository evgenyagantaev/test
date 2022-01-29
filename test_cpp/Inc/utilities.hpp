#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "opencv_headers.hpp"

#include "window_frame.hpp"

#define WHITE 0xff
#define BLACK 0x00



bool draw_cross(cv::Mat img, int i, int j, uint8_t brightness);

void onMouse( int event, int x, int y, int, void* userInput );