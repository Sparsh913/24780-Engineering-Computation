#include "fssimplewindow.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Design a cannonball game
const double SG_PI = 3.14159265;

void DrawCannon(int x, int y, int angle)
{
    glColor3ub(0, 0, 255);
    glBegin(GL_QUADS);
    glVertex2i(x - 10, y - 10);
    glVertex2i(x + 10, y - 10);
    glVertex2i(x + 10, y + 10);
    glVertex2i(x - 10, y + 10);
    glEnd();
    // barrel of the cannon is 30 pixels long - just a line
    glBegin(GL_LINES);
    glVertex2i(x, y);
    glVertex2i(x + 30 * cos(angle * SG_PI / 180.0), y + 30 * sin(angle * SG_PI / 180.0));
    glEnd();
}

void DrawCannonBall(int cx, int cy, int r, int num)
{
    switch (num)
    {
    case 0:
        glColor3ub(0, 0, 255);
        break;
    case 1:
        glColor3ub(0, 255, 255);
        break;
    case 2:
        glColor3ub(255, 255, 0);
        break;
    case 3:
        glColor3ub(255, 0, 255);
        break;
    default:
        glColor3ub(255, 0, 0);
        break;
    }
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 360; i++)
    {
        double angle = (double)i * SG_PI / 180.0;
        double x = cx + r * cos(angle);
        double y = cy + r * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}

void DrawObstacle(int x, int y, int w, int h, int state)
{
    if (state == 0)
    {
        glColor3ub(0, 255, 0);
    }
    else if (state == 1)
    {
        // yellow
        glColor3ub(255, 255, 0);
    }
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
}

void DrawTarget(int x, int y)
{
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(x - 5, y - 5);
    glVertex2i(x + 5, y - 5);
    glVertex2i(x + 5, y + 5);
    glVertex2i(x - 5, y + 5);
    glEnd();
}

bool CheckCollision(int bx, int by, int x, int y, int w, int h)
{
    return (bx > x && bx < x + w && by > y && by < y + h);
}

int main(void)
{
    srand((int)time(nullptr));

    int cannonX = 50, cannonY = 500; // cannon at lower left
    int cannonAngle = 45; // initial angle
    // cannonball position must be initialized to the cannon barrel position
    int cannonBallX = cannonX + 30 * cos(cannonAngle * SG_PI / 180.0);
    int cannonBallY = cannonY + 30 * sin(cannonAngle * SG_PI / 180.0);
    int cannonBallR = 5; // cannonball radius
    int cannonBallState = 0; // 0 - not fired, 1 - fired
    int num_balls = 0; // keep track of the number of balls fired and change colors based on this
    double cannonBallVx = 0.0, cannonBallVy = 0.0; // cannonball velocity
    // absolute initial velocity of the cannonball is 40 pix/s in the direction of the cannon barrel
    double cannonBallV = 40.0;
    // obstacles - 5 of them
    // each obstacle has x, y, width, height, and state - height and width random between 8 and 15
    double obsX[5], obsY[5], obsW[5], obsH[5], obsState[5];

    for (int i = 0; i < 5; i++)
    {
        obsW[i] = 8 + rand() % 8;
        obsH[i] = 8 + rand() % 8;
        // x position is random between 100 and window size - width/2
        obsX[i] = 100 + rand() % (800 - obsW[i]/2 - 100);
        // y position is random between 0 and window size - height/2
        obsY[i] = obsH[i]/2 + rand() % (600 - obsH[i]/2);
        obsState[i] = 0;
    } 

    FsOpenWindow(0, 0, 800, 600, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (;;)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto key = FsInkey();
        if (key == FSKEY_ESC) break;
        

        FsSwapBuffers();
    }

    return 0;
}