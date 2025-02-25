#include "rasterizer.h"
#include <algorithm>

void drawTriangle(Frame& frame, int x0, int y0, int x1, int y1, int x2, int y2, Color color) {
    int minX = std::min({x0, x1, x2});
    int minY = std::min({y0, y1, y2});
    int maxX = std::max({x0, x1, x2});
    int maxY = std::max({y0, y1, y2});

    float area = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
    if (std::abs(area) < 1e-2) return;
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float w0 = ((x1 - x) * (y2 - y) - (x2 - x) * (y1 - y)) / area;
            float w1 = ((x2 - x) * (y0 - y) - (x0 - x) * (y2 - y)) / area;
            float w2 = ((x0 - x) * (y1 - y) - (x1 - x) * (y0 - y)) / area;

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                frame.setPixel(x, y, color);
            }
        }
    }
}
