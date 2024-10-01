#include <iostream>
#include <math.h>
#include "fssimplewindow.h"
using namespace std;

const double SG_pi = 3.14159;

void DrawRectangle(int x1, int y1, int x2, int y2)
{
    glBegin(GL_QUADS);
    glVertex2i(x1, y1);
    glVertex2i(x2, y1);
    glVertex2i(x2, y2);
    glVertex2i(x1, y2);
    glEnd();
}

void DrawCircle(int cx, int cy, int radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(cx, cy);
    for (int i = 0; i <= segments; i++)
    {
        double angle = 2.0 * SG_pi * i / segments;
        int x = cx + radius * cos(angle);
        int y = cy + radius * sin(angle);
        glVertex2i(x, y);
    }
    glEnd();
}

void DrawCar(int x, int y)
{
    // Flip Y axis (OpenGL origin is at the bottom left, we want top-left)
    y = 600 - y;

    // Car Body
    glColor3ub(100, 100, 255);  // purple color for body
    DrawRectangle(x, y, x + 200, y - 50);     // Lower body
    DrawRectangle(x + 50, y - 50, x + 150, y - 100);  // Upper body (roof)

    // Windows
    glColor3ub(150, 200, 255);  // Light blue for windows
    DrawRectangle(x + 60, y - 60, x + 90, y - 90);  // Left window
    DrawRectangle(x + 110, y - 60, x + 140, y - 90); // Right window

    // Wheels
    glColor3ub(50, 50, 50);  // Dark gray for wheels
    DrawCircle(x + 50, y, 25, 50);   // Front wheel
    DrawCircle(x + 150, y, 25, 50);  // Rear wheel

    // Wheel details (hubcaps)
    glColor3ub(200, 200, 200);  // Light gray for hubcaps
    DrawCircle(x + 50, y, 10, 50);   // Front hubcap
    DrawCircle(x + 150, y, 10, 50);  // Rear hubcap

    // Car Lights
    glColor3ub(255, 255, 0);  // Yellow for headlights
    DrawRectangle(x + 190, y - 20, x + 200, y - 30); // Right headlight
    DrawRectangle(x + 190, y - 40, x + 200, y - 50); // Left headlight

    glColor3ub(255, 0, 0);  // Red for taillights
    DrawRectangle(x, y - 20, x + 10, y - 30);  // Left taillight
    DrawRectangle(x, y - 40, x + 10, y - 50);  // Right taillight
}

int main(void)
{
    FsOpenWindow(0, 0, 800, 600, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(;;)
    {
        FsPollDevice();
        auto key = FsInkey();
        if (key == FSKEY_ESC) break;
        
        DrawCar(300, 300);

        glFlush();
        
    }

    return 0;
}
