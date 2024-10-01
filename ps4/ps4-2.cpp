#include <iostream>
#include "fssimplewindow.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

const double SG_PI = 3.14159265;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const double GRAVITY = 98.1;
const double INITIAL_VELOCITY = 400.0;
const int TAIL_LENGTH = 10;
std::vector<std::pair<float, float>> cannonballTrail;

struct Obstacle {
    int x, y, width, height;
    int state = 0; // 0 for green, 1 for yellow
};

Obstacle obstacles[5];
int targetX = WINDOW_WIDTH - 50, targetY = 50, targetW = 100, targetH = 100;
float cannonballX, cannonballY, cannonballVelX, cannonballVelY;
bool cannonballInMotion = false;
int shotCount = 0;
float cannonAngle = 45.0 * SG_PI / 180.0;
bool gameRunning = true;

void DrawCannon(int x, int y, float angle)
{
    glColor3ub(0, 0, 255);  // Blue cannon
    glBegin(GL_QUADS);
    glVertex2i(x - 10, y - 10);
    glVertex2i(x + 10, y - 10);
    glVertex2i(x + 10, y + 10);
    glVertex2i(x - 10, y + 10);
    glEnd();
 
    // Draw the cannon barrel
    glBegin(GL_LINES);
    glVertex2i(x, y);
    glVertex2i(x + cos(angle) * 30, y - sin(angle) * 30);
    glEnd();
}

void DrawCannonball(float x, float y)
{
    // Ensure the cannonball color is fixed after five shots
    if (shotCount < 5) {
        switch (shotCount % 5) {
            case 1: glColor3ub(0, 0, 255); break;  // Blue
            case 2: glColor3ub(0, 255, 255); break;  // Cyan
            case 3: glColor3ub(255, 255, 0); break;  // Yellow
            case 4: glColor3ub(255, 0, 255); break;  // Magenta
            default: glColor3ub(255, 0, 0); break;  // Red
        }
    } else {
        glColor3ub(255, 0, 0);  // Permanently Red after five shots
    }

    // Draw the cannonball as a filled circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float rad = i * SG_PI / 180.0;
        glVertex2f(x + cos(rad) * 5, y + sin(rad) * 5);  // Radius = 5
    }
    glEnd();
}

void DrawObstacle(int x, int y, int w, int h, int state)
{
    if (state == 0) glColor3ub(0, 255, 0);  // Green obstacle
    else if (state == 1) glColor3ub(255, 255, 0);  // Yellow obstacle

    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
}

void DrawTarget(int x, int y)
{
    glColor3ub(255, 0, 0);  // Red target
    glBegin(GL_QUADS);
    glVertex2i(x - targetW / 2, y - targetH / 2);
    glVertex2i(x + targetW / 2, y - targetH / 2);
    glVertex2i(x + targetW / 2, y + targetH / 2);
    glVertex2i(x - targetW / 2, y + targetH / 2);
    glEnd();
}

bool CheckCollision(float bx, float by, Obstacle o)
{
    return (bx > o.x && bx < o.x + o.width && by > o.y && by < o.y + o.height);
}

bool CheckTargetCollision(float bx, float by)
{
    return (bx > targetX - targetW / 2 && bx < targetX + targetW / 2 && by > targetY - targetH / 2 && by < targetY + targetH / 2);
}

void FireCannon(int cannonX, int cannonY)
{
    if (!cannonballInMotion) {
        cannonballInMotion = true;
        cannonballX = cannonX + cos(cannonAngle) * 30; // 30 pixels barrrel length
        cannonballY = cannonY - sin(cannonAngle) * 30;
        cannonballVelX = INITIAL_VELOCITY * cos(cannonAngle);
        cannonballVelY = INITIAL_VELOCITY * -sin(cannonAngle);
        shotCount++;

        cannonballTrail.clear();  // Clear the trail for a new shot
    }
}

void MoveTarget()
{
    static int direction = 1;  // 1 = down, -1 = up
    targetY += direction * 10;  // Move the target at pixel rate
    if (targetY + targetH / 2 >= WINDOW_HEIGHT || targetY - targetH / 2 <= 0) {
        direction = -direction;  // Reverse direction at the boundaries
    }
}

void UpdateCannonballPosition(float dt)
{
    if (cannonballInMotion) {
        // Update horizontal position (constant velocity)
        cannonballX += cannonballVelX * dt;
        
        // Update vertical velocity (gravity affects vertical velocity)
        cannonballVelY += GRAVITY * dt;  // Gravity increases downward velocity

        // Update vertical position
        cannonballY += cannonballVelY * dt + 0.5 * GRAVITY * dt * dt;

        // Add current position to the trail
        cannonballTrail.push_back({cannonballX, cannonballY});
        if (cannonballTrail.size() > TAIL_LENGTH) {
            cannonballTrail.erase(cannonballTrail.begin());  // Remove the oldest position if tail exceeds length
        }
        
        // Check if the cannonball hits any obstacles
        for (int i = 0; i < 5; i++) {
            if (obstacles[i].state < 2 && CheckCollision(cannonballX, cannonballY, obstacles[i])) {
                cannonballInMotion = false;
                obstacles[i].state++;

                cannonballTrail.clear();  // Clear the trail after hitting an obstacle
            }
        }

        // Check if the cannonball hits the target
        if (CheckTargetCollision(cannonballX, cannonballY)) {
            cannonballInMotion = false;
            gameRunning = false;  // Game ends when target is hit
            cannonballTrail.clear();
        }

        // If the cannonball goes out of bounds
        if (cannonballX > WINDOW_WIDTH || cannonballY > WINDOW_HEIGHT) {
            cannonballInMotion = false;
            cannonballTrail.clear();  // Clear the trail if the cannonball goes out of bounds
        }
    }
}

void DrawCannonballTrail()
{
    // Ensure the trail color is the same as the cannonball color
    if (shotCount < 5) {
        switch (shotCount % 5) {
            case 1: glColor3ub(0, 0, 255); break;  // Blue
            case 2: glColor3ub(0, 255, 255); break;  // Cyan
            case 3: glColor3ub(255, 255, 0); break;  // Yellow
            case 4: glColor3ub(255, 0, 255); break;  // Magenta
            default: glColor3ub(255, 0, 0); break;  // Red
        }
    } else {
        glColor3ub(255, 0, 0);  // Permanently Red after five shots
    }

    // Draw the trail as a line strip
    glBegin(GL_LINE_STRIP);
    for (auto &pos : cannonballTrail) {
        glVertex2f(pos.first, pos.second);
    }
    glEnd();
}


void GenerateObstacles()
{
    for (int i = 0; i < 5; ++i) {
        // Random width and height between 8m and 15m
        int width = rand() % 70 + 80;
        int height = rand() % 70 + 80;

        // Random x and y positions ensuring the obstacle stays within the window
        int x = rand() % (WINDOW_WIDTH - width - 1);  // Ensures that the obstacle doesn't extend beyond the right edge
        int y = rand() % (WINDOW_HEIGHT - height - 1);  // Ensures that the obstacle doesn't extend beyond the bottom edge

        // Assign values to the obstacle
        obstacles[i].x = x;
        obstacles[i].y = y;
        obstacles[i].width = width;
        obstacles[i].height = height;
    }
}

int main(void)
{
    FsOpenWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);
    GenerateObstacles();

    int cannonX = 50, cannonY = 580;  // Cannon at the bottom-left corner
    const float dt = 0.1;

    while (gameRunning) {
    FsPollDevice();
    auto key = FsInkey();

    if (key == FSKEY_ESC) break;
    if (key == FSKEY_SPACE) FireCannon(cannonX, cannonY);
    if (key == FSKEY_UP) cannonAngle = std::min(cannonAngle + 3 * SG_PI / 180.0, SG_PI / 2);
    if (key == FSKEY_DOWN) cannonAngle = std::max(cannonAngle - 3 * SG_PI / 180.0, 0.0);

    MoveTarget();
    UpdateCannonballPosition(dt);

    // Rendering
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    DrawCannon(cannonX, cannonY, cannonAngle);

    // Draw obstacles first
    for (int i = 0; i < 5; i++) {
        if (obstacles[i].state < 2) {  // Obstacle disappears after two hits
            DrawObstacle(obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height, obstacles[i].state);
        }
    }

    // Draw the target after obstacles to ensure it's on top
    DrawTarget(targetX, targetY);

    // Draw the cannonball
    if (cannonballInMotion) {
        DrawCannonballTrail();
        DrawCannonball(cannonballX, cannonballY);
    }

    FsSwapBuffers();
    FsSleep(25);
}

    // Number of shots taken to hit the target
    cout << "Target hit after " << shotCount << " shots!" << endl;

    return 0;
}
