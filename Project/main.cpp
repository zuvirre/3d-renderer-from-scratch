#include "../include/frame.h"
#include "../include/rasterizer.h"
#include <iostream>

int main() {
    Frame frame;
    frame.clear({0, 0, 0});

    drawTriangle(frame, 100, 100, 400, 50, 200, 300, {255, 0, 0});
    drawTriangle(frame, 300, 200, 600, 100, 500, 400, {0, 255, 0});
    drawTriangle(frame, 150, 400, 450, 350, 250, 550, {0, 0, 255}); 
    frame.savePPM("output.ppm");
    return 0;
}
