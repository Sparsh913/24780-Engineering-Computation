#include <iostream>
#include <math.h>
#include "fssimplewindow.h"
using namespace std;

const double SG_pi = 3.14159;
void DrawCircle(double cx, double cy, double r, bool col)
{
    if (col)
    {
        glColor3ub(255, 255, 0); // yellow color
    }
    else
    {
        glColor3ub(0, 0, 0); // black color
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(cx, cy); // center of circle
    for (int i = 0; i <= 360; i += 5)
    {
        double angle = (double)i * SG_pi / 180; // convert degrees to radians
        double x = cx + r * cos(angle);
        double y = cy + r * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}

void DrawTongue(double cx, double cy, double radius)
{
    glColor3ub(255, 128, 128); // pink color for tongue
    // make tongue U shaped - left line - right line - and a curve to join those two
    glBegin(GL_POLYGON);
    glVertex2d(cx - radius/2.5, cy); // left bottom
    glVertex2d(cx + radius/2.5, cy); // right bottom
    glVertex2d(cx + radius / 2.5, cy - radius); // right top
    glVertex2d(cx - radius / 2.5, cy - radius); // left top
    glEnd();
    // Draw a curve for the tongue
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(cx, cy); // center of curve
    for (int i = 0; i <= 180; i += 5)
    {
        double angle = (double)i * SG_pi / 180; // convert degrees to radians
        double x = cx + (radius / 2.5) * cos(angle);
        double y = cy + (radius / 2.5) * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}

int main (void)
{
    FsOpenWindow(0, 0, 800, 600, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(;;)
    {
        FsPollDevice();
        auto key = FsInkey();
        if (key == FSKEY_ESC) break;

        // create a smiley face with gl primitives
        // draw a circle for the face
        DrawCircle(400, 300, 100, true); // face
        DrawCircle(370, 250, 10, false); // left eye
        DrawCircle(430, 250, 10, false); // right eye
        // DrawCircle(400, 320, 20, false); // mouth
        // for mouth, we can use a triangle strip to create a smile
        glColor3ub(0, 0, 0); // black color for mouth
        // make smiley curve thicker
        glLineWidth(5);
        glBegin(GL_LINE_STRIP);
        for (int i = 20; i <= 160; i += 5)
        {
            double angle = (double)i * SG_pi / 180; // convert degrees to radians
            double x = 400 + 70 * cos(angle);
            double y = 300 + 70 * sin(angle) - 20; // move down for mouth
            glVertex2d(x, y);
        }
        glEnd();

        DrawTongue(400, 380, 30); // tongue

        glFlush();
    }
    return 0;
}