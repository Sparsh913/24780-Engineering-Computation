#include <iostream>
#include <math.h>
#include "fssimplewindow.h"
#include <vector>

const int SCALE = 100;  // 1m = 100px
const int ORIGIN_X = 100;
const int ORIGIN_Y = 100;
const float SHAFT1_LENGTH = 1.0f;  // Length of Shaft 1 (in meters)
const float SHAFT2_LENGTH = 4.0f;  // Length of Shaft 2 (in meters)
const float SHAFT3_LENGTH = 2.0f;  // Length of Shaft 3 (in meters)
const float P3_X = 4.0f;           // Position of P3 in meters
const float P3_Y = 0.0f;           // Position of P3 in meters
const float ROTATION_INCREMENT = 0.01f;  // Rotation angle increment (in radians)

void DrawCircle(int x, int y, int radius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; ++i) {
        double angle = 2.0 * M_PI * i / 100.0;
        int px = x + (int)(radius * cos(angle));
        int py = y + (int)(radius * sin(angle));
        glVertex2i(px, py);
    }
    glEnd();
}

void DrawPath(const std::vector<std::pair<int, int>> &path) {
    glColor3f(0.0f, 1.0f, 0.0f);  // Green for the path
    glBegin(GL_LINE_LOOP);
    for (const auto &point : path) {
        glVertex2i(point.first, point.second);
    }
    glEnd();
}

// Function to find P2 position given P1
bool findP2Position(float p1x, float p1y, float x3, float y3, float len2, float len3, 
                   float& p2x, float& p2y, bool usePositiveSolution) {
    float d = sqrt((x3-p1x)*(x3-p1x) + (y3-p1y)*(y3-p1y));
    
    if (d > len2 + len3 || d < fabs(len2 - len3)) {
        return false;
    }
    
    float a = (len2*len2 - len3*len3 + d*d)/(2*d);
    float h = sqrt(len2*len2 - a*a);
    
    float x3_p1x = x3 - p1x;
    float y3_p1y = y3 - p1y;
    
    float xm = p1x + a*(x3_p1x)/d;
    float ym = p1y + a*(y3_p1y)/d;
    
    if (usePositiveSolution) {
        p2x = xm + h*(y3_p1y)/d;
        p2y = ym - h*(x3_p1x)/d;
    } else {
        p2x = xm - h*(y3_p1y)/d;
        p2y = ym + h*(x3_p1x)/d;
    }
    
    return true;
}

int main() {
    FsOpenWindow(0, 0, 800, 600, 1);  // Open a window

    float angle = 0.0f;  // Initial angle of the first shaft
    std::vector<std::pair<int, int>> midpointPath;
    bool usePositiveSolution = true;  // Keep track of which solution to use

    while (true) {
        FsPollDevice();
        if (FSKEY_ESC == FsInkey()) {
            break;
        }

        // Calculate P1 (end of Shaft 1)
        float p1x = SHAFT1_LENGTH * cos(angle);
        float p1y = SHAFT1_LENGTH * sin(angle);

        // Find P2 position based on P1 and P3
        float p2x, p2y;
        bool validPosition = findP2Position(p1x, p1y, P3_X, P3_Y, SHAFT2_LENGTH, SHAFT3_LENGTH, 
                                         p2x, p2y, usePositiveSolution);
        
        if (!validPosition) {
            angle += ROTATION_INCREMENT;
            continue;
        }

        // Calculate the midpoint of P1 and P2
        float midX = (p1x + p2x) / 2.0f;
        float midY = (p1y + p2y) / 2.0f;

        // Transform coordinates to pixels
        int p1ScreenX = ORIGIN_X + p1x * SCALE;
        int p1ScreenY = ORIGIN_Y - p1y * SCALE;
        int p2ScreenX = ORIGIN_X + p2x * SCALE;
        int p2ScreenY = ORIGIN_Y - p2y * SCALE;
        int p3ScreenX = ORIGIN_X + P3_X * SCALE;
        int p3ScreenY = ORIGIN_Y - P3_Y * SCALE;
        int midScreenX = ORIGIN_X + midX * SCALE;
        int midScreenY = ORIGIN_Y - midY * SCALE;

        // Add midpoint to the path
        midpointPath.emplace_back(midScreenX, midScreenY);

        // Clear the screen and render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw shafts
        glColor3f(1.0f, 0.0f, 0.0f);  // Red for the shafts
        glBegin(GL_LINES);
        glVertex2i(ORIGIN_X, ORIGIN_Y);      // P0 to P1
        glVertex2i(p1ScreenX, p1ScreenY);
        glVertex2i(p1ScreenX, p1ScreenY);    // P1 to P2
        glVertex2i(p2ScreenX, p2ScreenY);
        glVertex2i(p2ScreenX, p2ScreenY);    // P2 to P3
        glVertex2i(p3ScreenX, p3ScreenY);
        glEnd();

        // Draw joints
        DrawCircle(p1ScreenX, p1ScreenY, 5);  // P1
        DrawCircle(p2ScreenX, p2ScreenY, 5);  // P2
        DrawCircle(p3ScreenX, p3ScreenY, 5);  // P3

        // Draw the path of the midpoint
        DrawPath(midpointPath);

        FsSwapBuffers();
        FsSleep(10);

        // Increment angle for next frame
        angle += ROTATION_INCREMENT;
        if (angle >= 2.0 * M_PI) {
            angle -= 2.0 * M_PI;  // Keep angle within 0 to 2π
        }
    }

    return 0;
}