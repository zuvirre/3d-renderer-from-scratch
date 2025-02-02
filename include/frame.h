#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <string>
#include <cstdint>

const int WIDTH = 800;
const int HEIGHT = 600;

struct Color {
    uint8_t r, g, b;
};

class Frame {
private:
    std::vector<Color> pixels;
public:
    Frame();  
    void clear(Color color = {0, 0, 0});
    void setPixel(int x, int y, Color color);
    void savePPM(const std::string& filename) const;
};

#endif
