//
//  main.cpp
//  AI_GridWorld
//
//  Created by cpsfaculty on 11/13/17.
//  Copyright (c) 2017 ___Ju Shen___. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "VisualDisplay.h"

using namespace std;
using namespace cv;


int main(int argc, const char * argv[]) {
    
    VisualDisplay display;
    display.loadImages();
    display.run();
    
    
    return 0;
}
