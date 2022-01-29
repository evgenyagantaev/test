#include "thermometry.hpp"

//#define DEBUG_CONSOLE_OUTPUT       // console debug output toggle define
//#define VISUAL_DEBUG                 // visual debug animation toggle define
//#define MANUAL_MARK                  // manual markup visual animation toggle define
#define THREE_LAYERS                 // with this define we do 3 layers, without - 2 layers


using namespace cv;
using namespace std;

FaceTemperature thermometry::detect_temperature(cv::Mat frame, FacePositionInfo facePos)
{

    set_mark_canvas(frame);
    legacy_mark(facePos);     
    
    double left_eye_temp = left_eye_detect_temperature();
    double right_eye_temp = right_eye_detect_temperature();
    double forehead_temp = forehead_detect_temperature();

    return FaceTemperature(left_eye_temp, right_eye_temp, forehead_temp);
}

thermometry::thermometry(void)
{
    manual_mark_counter = 0;
    marked = false;
    left_eye_spot.set_type(SPOT_TYPE::EYE); // eye spot type
    right_eye_spot.set_type(SPOT_TYPE::EYE); // eye spot type
    forehead_spot.set_type(SPOT_TYPE::FOREHEAD); // forehead spot type

    #ifdef MANUAL_MARK
    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
    namedWindow("manual mark window", WINDOW_NORMAL);
    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
    #endif
    
}

void thermometry::set_mark_canvas(Mat value)
{
    thermal_field = value;
    

    #ifdef MANUAL_MARK
    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
    mark_canvas = thermal_field.clone();
    putText(mark_canvas, "mark left eye", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
    #endif
}

Mat thermometry::get_mark_canvas(void)
{
    return mark_canvas;
}

void thermometry::legacy_mark(FacePositionInfo facePos)
{
    left_eye_center = facePos.leftEye; 
    right_eye_center = facePos.rightEye;

    double dx = left_eye_center.x - right_eye_center.x;
    double dy = left_eye_center.y - right_eye_center.y;
    // calculate eyes span without accounting of yaw angle
    eyes_span = sqrt(dx*dx + dy*dy);
    double yaw_co = cos(facePos.yaw);
    if(yaw_co != 0)
        yaw_co = 1 / yaw_co;
    else
        yaw_co = 1;   // exception: division by zero

    eyes_span *= yaw_co; // now we are accounting a yaw angle
    
    eye_spot_width = (int)(eyes_span * EYE_WIDTH_COEFF);
    eye_spot_height = eye_spot_width;
    eye_spot_base_horizontal_offset = eye_spot_width;
    eye_spot_base_vertical_offset = (int)(eye_spot_width/2);

    marked = true;
}



void thermometry::mark_frame(cv::Mat frame_to_mark, int x, int y)
{
    

    if(manual_mark_counter == 0) 
    {
        left_eye_center.x = x;
        left_eye_center.y = y;
        manual_mark_counter++;

        #ifdef DEBUG_CONSOLE_OUTPUT
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        cout << "mark_frame >> left eye position = " << left_eye_center.getx() << "   " << left_eye_center.gety() << endl;
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        #ifdef MANUAL_MARK
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        mark_canvas = thermal_field.clone();
        putText(mark_canvas, "mark right eye", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
        draw_cross(mark_canvas, y, x, BLACK);
        imshow("manual mark window", mark_canvas);
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif
    }
    else if(manual_mark_counter == 1) 
    {
        right_eye_center.x = x;
        right_eye_center.y = y;
        manual_mark_counter++;

        #ifdef DEBUG_CONSOLE_OUTPUT
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        cout << "mark_frame >> right eye position = " << right_eye_center.getx() << "   " << right_eye_center.gety() << endl;
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        #ifdef MANUAL_MARK
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        mark_canvas = thermal_field.clone();
        putText(mark_canvas, "mark ABB", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
        draw_cross(mark_canvas, y, x, BLACK);
        imshow("manual mark window", mark_canvas);
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif
    }
    else if(manual_mark_counter == 2) 
    {
        abb_base_pixel.set_position(x, y);
        manual_mark_counter++;

        abb_base_pixel.set_brightness((int)frame_to_mark.at<uchar>(y,x));
        abb_base_pixel.set_temperature(DEFAULT_ABB_TEMPERATURE);

        #ifdef DEBUG_CONSOLE_OUTPUT
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        cout << "mark_frame >> abb position = " << abb_base_pixel.get_position().getx() << "   " << abb_base_pixel.get_position().gety() << endl;
        cout << "mark_frame >> abb brightness = " << abb_base_pixel.get_brightness() << endl;
        cout << "mark_frame >> abb temperature = " << abb_base_pixel.get_temperature() << endl;
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        #ifdef MANUAL_MARK
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        mark_canvas = thermal_field.clone();
        putText(mark_canvas, "mark background", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
        draw_cross(mark_canvas, y, x, BLACK);
        imshow("manual mark window", mark_canvas);
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif
    }
    else if(manual_mark_counter == 3) 
    {
        background_base_pixel.set_position(x, y);
        manual_mark_counter = 0;
        
        background_base_pixel.set_brightness((int)frame_to_mark.at<uchar>(y,x));
        background_base_pixel.set_temperature(DEFAULT_BACKGROUND_TEMPERATURE);

        #ifdef DEBUG_CONSOLE_OUTPUT
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        cout << "mark_frame >> abb position = " << background_base_pixel.get_position().getx() << "   " << background_base_pixel.get_position().gety() << endl;
        cout << "mark_frame >> background brightness = " << background_base_pixel.get_brightness() << endl;
        cout << "mark_frame >> background temperature = " << background_base_pixel.get_temperature() << endl;
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        #ifdef MANUAL_MARK
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        draw_cross(mark_canvas, y, x, BLACK);
        imshow("manual mark window", mark_canvas);
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif


        
        eyes_span = left_eye_center.x - right_eye_center.x;
        eye_spot_width = (int)(eyes_span * EYE_WIDTH_COEFF);
        eye_spot_height = eye_spot_width;
        eye_spot_base_horizontal_offset = eye_spot_width;
        eye_spot_base_vertical_offset = (int)(eye_spot_width/2);

        marked = true;


    }
}

bool thermometry::is_marked(void)
{
    return marked;
}

void thermometry::unmark(void)
{
    marked = false;
    manual_mark_counter = 0;
    mark_canvas = thermal_field.clone();
}


double thermometry::left_eye_detect_temperature()
{
    left_eye_claster.clean();
    left_eye_spot.set_base(left_eye_center.x - eye_spot_base_horizontal_offset, left_eye_center.y - eye_spot_base_vertical_offset);
    left_eye_spot.set_width(eye_spot_width);
    left_eye_spot.set_height(eye_spot_height);

    return detect_spot_temperature(&left_eye_spot, &left_eye_claster);
}

double thermometry::right_eye_detect_temperature()
{
    right_eye_claster.clean();
    right_eye_spot.set_base(right_eye_center.x, right_eye_center.y - eye_spot_base_vertical_offset);
    right_eye_spot.set_width(eye_spot_width);
    right_eye_spot.set_height(eye_spot_height);

    return detect_spot_temperature(&right_eye_spot, &right_eye_claster);
}

double thermometry::forehead_detect_temperature()
{
    forehead_claster.clean();
    forehead_spot.set_base(right_eye_center.x - eyes_span * AUX_COEFF, right_eye_center.y - eyes_span * (1+AUX_COEFF));
    forehead_spot.set_width(eyes_span * (1 + 2*AUX_COEFF));
    forehead_spot.set_height((int)(eyes_span * 5 / 8));

    return detect_spot_temperature(&forehead_spot, &forehead_claster);
}


double thermometry::detect_spot_temperature(rect_thermal_spot *spot, thermo_pixel_vector *claster)
{
    
    double temperature = 0;

    int secondary_bases_number;
    if((*spot).get_type() == SPOT_TYPE::EYE)      // eye spot
        secondary_bases_number = SECONDARY_BASES_NUMBER_EYE;
    else
        secondary_bases_number = SECONDARY_BASES_NUMBER_FOREHEAD;


    bool try_random_generation = true;
    int try_counter = 0;

    while(try_random_generation && (try_counter < RANDOM_GENERATION_TRY_ATTEMPTS))
    {
        #ifdef VISUAL_DEBUG
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        mark_canvas = thermal_field.clone();
        putText(mark_canvas, "primary layer random points", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
        imshow("manual mark window", mark_canvas);
        waitKey(100);
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        primary_layer_detect(spot, claster);
        if((*claster).get_current_length() >= secondary_bases_number)
        {
            // enough points;
            try_random_generation = false;
        }
        else
        {
            try_counter++;
            #ifdef DEBUG_CONSOLE_OUTPUT
            //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
            cout << " not enough points; try  " << try_counter << " << " << endl;
            //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
            #endif
            
        }
    }

    

    if((*claster).get_current_length() >= secondary_bases_number)  // enough points
    {

        #ifdef VISUAL_DEBUG
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        waitKey(500);
        mark_canvas = thermal_field.clone();
        imshow("manual mark window", mark_canvas);
        waitKey(100);
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        int width = (int)((*spot).get_width() / 2);
        int height = (int)((*spot).get_height() / 2);
        (*spot).set_width(width);
        (*spot).set_height(height);

        
        secondary_layer_detect(spot, claster); // second layer

        #ifdef THREE_LAYERS
        secondary_layer_detect(spot, claster); // third layer
        #endif

        int head_cut;
        int cluster_body;
        if((*spot).get_type() == SPOT_TYPE::EYE)      // eye spot
        {
            head_cut = HEAD_CUT_EYE;
            cluster_body = CLASTER_BODY_EYE;
        }
        else
        {
            head_cut = HEAD_CUT_FOREHEAD;
            cluster_body = CLASTER_BODY_FOREHEAD;
        }
            
        int index = head_cut;
        while (index < (head_cut + cluster_body) && (index < (*claster).get_current_length()))
        {
            temperature += (*claster).get_pixel(index).get_temperature();
            index++;

            
        }
        temperature /= cluster_body;
        

    }
    else // not enough points in max number of attempts
    {
        temperature = -273;
    }
    
   
    return temperature;
    
}


void thermometry::primary_layer_detect(rect_thermal_spot *spot, thermo_pixel_vector *claster)
{
    int i;
    int primary_random_points_number;
    if((*spot).get_type() == SPOT_TYPE::EYE)      // eye spot
    {
        primary_random_points_number = PRIMARY_RANDOM_POINTS_NUMBER_EYE;
    }
    else
    {
        primary_random_points_number = PRIMARY_RANDOM_POINTS_NUMBER_FOREHEAD;
    }

    for(i=0; i<primary_random_points_number; i++)
    {
        
        Point random_point = (*spot).generate();
        
        #ifdef VISUAL_DEBUG
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        draw_cross(mark_canvas, random_point.y, random_point.x, BLACK);
        imshow("manual mark window", mark_canvas);
        waitKey(200);
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        thermo_pixel random_pixel;
        random_pixel.set_position(random_point.x, random_point.y);
        int pixel_brightness = (int)thermal_field.at<uchar>(random_point.y,random_point.x);
        random_pixel.set_brightness(pixel_brightness);
        double pixel_temperature = pixel_brightness * DEFAULT_K + DEFAULT_T0;
        random_pixel.set_temperature(pixel_temperature);

        #ifdef DEBUG_CONSOLE_OUTPUT
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        cout << " >> " << random_pixel.get_temperature() << " << " << endl;
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        if(range.check(random_pixel.get_temperature()))
        {
            (*claster).add_pixel(random_pixel);
        }   
    }
}


void thermometry::secondary_layer_detect(rect_thermal_spot *spot, thermo_pixel_vector *claster)
{
    
    int secondary_bases_number;
    if((*spot).get_type() == SPOT_TYPE::EYE)      // eye spot
        secondary_bases_number = SECONDARY_BASES_NUMBER_EYE;
    else
        secondary_bases_number = SECONDARY_BASES_NUMBER_FOREHEAD;

    if((*claster).get_current_length() >= secondary_bases_number)
    {
        int width = (*spot).get_width();
        int height = (*spot).get_height();
        int horizontal_offset = (int)(width / 2);
        int vertical_offset = (int)(height / 2);

        secondary_layer_bases.clean();
        int i;
        for(i=0; i<secondary_bases_number; i++)
        {
            secondary_layer_bases.add_pixel((*claster).get_pixel(i));
        }

        #ifdef VISUAL_DEBUG
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        mark_canvas = thermal_field.clone();
        imshow("manual mark window", mark_canvas);
        waitKey(500);
        for(i=0; i<secondary_bases_number; i++)
        {
            Point p = secondary_layer_bases.get_pixel(i).get_position();
            draw_cross(mark_canvas, p.y, p.x, BLACK);
        }
        putText(mark_canvas, "secondary layer base points", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
        imshow("manual mark window", mark_canvas);
        waitKey(500);
         mark_canvas = thermal_field.clone();
        imshow("manual mark window", mark_canvas);
        waitKey(500);
        for(i=0; i<secondary_bases_number; i++)
        {
            Point p = secondary_layer_bases.get_pixel(i).get_position();
            draw_cross(mark_canvas, p.y, p.x, BLACK);
        }
        putText(mark_canvas, "secondary layer base points", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
        imshow("manual mark window", mark_canvas);
        waitKey(500);
         mark_canvas = thermal_field.clone();
        imshow("manual mark window", mark_canvas);
        waitKey(500);
        for(i=0; i<secondary_bases_number; i++)
        {
            Point p = secondary_layer_bases.get_pixel(i).get_position();
            draw_cross(mark_canvas, p.y, p.x, BLACK);
        }
        putText(mark_canvas, "secondary layer base points", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
        imshow("manual mark window", mark_canvas);
        waitKey(500);
        mark_canvas = thermal_field.clone();
        putText(mark_canvas, "secondary layer random points", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
        imshow("manual mark window", mark_canvas);
        waitKey(100);
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif

        for(i=0; i<secondary_bases_number; i++)
        {
            int x = secondary_layer_bases.get_pixel(i).get_position().x;
            int y = secondary_layer_bases.get_pixel(i).get_position().y;
            (*spot).set_base(x - horizontal_offset, y - vertical_offset);

            #ifdef DEBUG_CONSOLE_OUTPUT
            //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
            cout << i << " >> secondary base " << endl;
            //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
            #endif

            int secondary_random_points_number;
            if((*spot).get_type() == SPOT_TYPE::EYE)      // eye spot
                secondary_random_points_number = SECONDARY_RANDOM_POINTS_NUMBER_EYE;
            else
                secondary_random_points_number = SECONDARY_RANDOM_POINTS_NUMBER_FOREHEAD;
            
            int j;
            for(j=0; j<secondary_random_points_number; j++)
            {
                
                Point random_point = (*spot).generate();

                #ifdef VISUAL_DEBUG
                //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
                draw_cross(mark_canvas, random_point.y, random_point.x, BLACK);
                imshow("manual mark window", mark_canvas);
                waitKey(200);
                //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
                #endif

                thermo_pixel random_pixel;
                random_pixel.set_position(random_point.x, random_point.y);
                int pixel_brightness = (int)thermal_field.at<uchar>(random_point.y,random_point.x);
                random_pixel.set_brightness(pixel_brightness);
                double pixel_temperature = pixel_brightness * DEFAULT_K + DEFAULT_T0;
                random_pixel.set_temperature(pixel_temperature);

                #ifdef DEBUG_CONSOLE_OUTPUT
                //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
                cout << " >> secondary random " << random_pixel.get_temperature() << " << " << endl;
                //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
                #endif

                if(range.check(random_pixel.get_temperature()))
                {
                    (*claster).add_pixel(random_pixel);

                    #ifdef DEBUG_CONSOLE_OUTPUT
                    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
                    cout << "claster length = " << (*claster).get_current_length() << endl;
                    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
                    #endif
                } 
                 
            }

            #ifdef VISUAL_DEBUG
            //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
            waitKey(500);
            mark_canvas = thermal_field.clone();
            putText(mark_canvas, "secondary layer random points", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);
            imshow("manual mark window", mark_canvas);
            waitKey(100);
            //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
            #endif
            

        }//end   for(i=0; i<SECONDARY_BASES_NUMBER; i++)

        #ifdef VISUAL_DEBUG
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        mark_canvas = thermal_field.clone();
        putText(mark_canvas, "secondary layer claster", Point(30,30), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0,0,0), 2);

        int head_cut;
        int cluster_body;
        if((*spot).get_type() == SPOT_TYPE::EYE)      // eye spot
        {
            head_cut = HEAD_CUT_EYE;
            cluster_body = CLASTER_BODY_EYE;
        }
        else
        {
            head_cut = HEAD_CUT_FOREHEAD;
            cluster_body = CLASTER_BODY_FOREHEAD;
        }

        int k;
        for(k=0; k<(head_cut +cluster_body); k++)
        {
            Point cross_position = (*claster).get_pixel(k).get_position();
            draw_cross(mark_canvas, cross_position.y, cross_position.x, BLACK);
            imshow("manual mark window", mark_canvas);
            waitKey(200);
        }
        //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
        #endif
    }
}