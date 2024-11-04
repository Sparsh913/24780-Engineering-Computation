#include <iostream>
#include "fssimplewindow.h"
#include "yspng.h"
#include <vector>

// Function to binarize the image
void BinarizeImage(YsRawPngDecoder &png) {
    int pixelCount = png.wid * png.hei;
    for (int i = 0; i < pixelCount; ++i) {
        int index = i * 4;  // Each pixel has 4 components (RGBA)
        if (png.rgba[index] < 220 || png.rgba[index + 1] < 220 || png.rgba[index + 2] < 220) {
            // Set pixel to black (0, 0, 0, 255)
            png.rgba[index] = 0;
            png.rgba[index + 1] = 0;
            png.rgba[index + 2] = 0;
        } else {
            // Set pixel to white (255, 255, 255, 255)
            png.rgba[index] = 255;
            png.rgba[index + 1] = 255;
            png.rgba[index + 2] = 255;
        }
    }
}

// Function to extract contours from the binarized image
void ExtractContour(YsRawPngDecoder &png) {
    std::vector<unsigned char> tempChannel(png.wid * png.hei, 255);  // Temporary storage for marking

    // Iterate over each pixel (excluding edges)
    for (int y = 1; y < png.hei - 1; ++y) {
        for (int x = 1; x < png.wid - 1; ++x) {
            int index = (y * png.wid + x) * 4;  // Index for RGBA components
            if (png.rgba[index] == 255) {  // Check if pixel is white
                // Check neighboring pixels to see if one is black
                bool hasBlackNeighbor = false;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dy == 0 && dx == 0) continue;  // Skip the center pixel
                        int neighborIndex = ((y + dy) * png.wid + (x + dx)) * 4;
                        if (png.rgba[neighborIndex] == 0) {  // Check if neighbor is black
                            hasBlackNeighbor = true;
                            break;
                        }
                    }
                    if (hasBlackNeighbor) break;
                }
                if (!hasBlackNeighbor) {
                    // Mark pixel for changing to black
                    tempChannel[y * png.wid + x] = 0;
                }
            }
        }
    }

    // Copy the temp channel back to the image (apply changes)
    for (int y = 1; y < png.hei - 1; ++y) {
        for (int x = 1; x < png.wid - 1; ++x) {
            int index = (y * png.wid + x) * 4;
            if (tempChannel[y * png.wid + x] == 0) {
                png.rgba[index] = 0;
                png.rgba[index + 1] = 0;
                png.rgba[index + 2] = 0;
            }
        }
    }
}

int main() {
    FsChangeToProgramDir();  // Ensure program runs from the correct directory

    YsRawPngDecoder png;
    if (YSOK != png.Decode("ps9.png")) {
        std::cerr << "Error: Could not open or decode ps9.png" << std::endl;
        return 1;
    }
    png.Flip();  // Flip the image to align correctly for rendering

    FsOpenWindow(0, 0, png.wid, png.hei, 1);

    bool binarized = false;
    bool contourExtracted = false;

    while (true) {
        FsPollDevice();
        int key = FsInkey();
        if (key == FSKEY_ESC) {
            break;
        } else if (key == FSKEY_SPACE && !binarized) {
            BinarizeImage(png);
            binarized = true;
        } else if (key == FSKEY_ENTER && binarized && !contourExtracted) {
            ExtractContour(png);
            contourExtracted = true;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glRasterPos2i(0, png.hei - 1);
        glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
        FsSwapBuffers();
        FsSleep(10);
    }

    return 0;
}
