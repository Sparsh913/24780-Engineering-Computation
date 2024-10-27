#include <iostream>
#include <vector>
#include <stdlib.h>
#include "fssimplewindow.h"
#include "yspng.h"

class Histogram {
protected:
    int hist[256];  // Array to hold pixel intensity frequencies
    bool histogramCreated;  // Flag to track if Make() has been called

public:
    Histogram() : histogramCreated(false) {
        Clear();
    }

    void Clear() {
        for (int &value : hist) {
            value = 0;
        }
    }

    void Make(const YsRawPngDecoder &png) {
        Clear();  // Reset histogram data
        int pixelCount = png.wid * png.hei * 4;
        for (int i = 0; i < pixelCount; ++i) {
            hist[png.rgba[i]]++;  // Count pixel intensities
        }
        histogramCreated = true;
    }

    void Print() const {
        if (!histogramCreated) {
            std::cout << "Error: Histogram not created. Call Make() first." << std::endl;
            return;
        }
        std::cout << "Histogram (Console Bar Graph):" << std::endl;
        for (int i = 0; i < 256; ++i) {
            std::cout << i << ": ";
            for (int j = 0; j < hist[i] / 100; ++j) {  // Adjust scaling for visibility
                std::cout << "|";
            }
            std::cout << std::endl;
        }
    }

    void Draw() const {
        if (!histogramCreated) {
            std::cerr << "Error: Histogram not created. Call Make() first." << std::endl;
            return;
        }

        int maxVal = 0;
        for (int value : hist) {
            if (value > maxVal) {
                maxVal = value;
            }
        }

        float scale = 80.0f / maxVal;  // Scale so the highest bar is 80 pixels

        glColor3f(1.0f, 0.0f, 0.0f);  // Set color for histogram bars
        for (int i = 0; i < 256; ++i) {
            int barHeight = static_cast<int>(hist[i] * scale);  // Scale bar height
            glBegin(GL_LINES);
            glVertex2i(i, 599);  // Bottom of the window
            glVertex2i(i, 599 - barHeight);  // Draw up based on bar height
            glEnd();
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <PNG filename>" << std::endl;
        return 1;
    }

    YsRawPngDecoder png;
    if (YSOK != png.Decode(argv[1])) {
        std::cerr << "Error: Could not open or decode " << argv[1] << std::endl;
        return 1;
    }
    png.Flip();

    FsOpenWindow(0, 0, png.wid, png.hei + 100, 1);  // Extra space for histogram display

    Histogram histogram;
    histogram.Make(png);  // Generate histogram based on the PNG image

    bool terminate = false;
    while (!terminate) {
        FsPollDevice();

        if (FSKEY_ESC == FsInkey()) {
            terminate = true;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glRasterPos2i(0, png.hei);
        glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);

        histogram.Draw();  // Draw histogram over the image

        FsSwapBuffers();
        FsSleep(10);
    }

    return 0;
}
