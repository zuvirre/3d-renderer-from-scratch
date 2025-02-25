#include "frame.h"
#include <fstream>
#include <iostream>

Frame::Frame() : pixels(WIDTH * HEIGHT, {0, 0, 0}) {}

void Frame::clear(Color color) {
    std::fill(pixels.begin(), pixels.end(), color);
}

void Frame::setPixel(int x, int y, Color color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        pixels[y * WIDTH + x] = color;
    }
}

void Frame::savePPM(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to save image!\n";
        return;
    }
    file << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
    for (const auto& pixel : pixels) {
        file << pixel.r << pixel.g << pixel.b;
    }
    file.close();
}
