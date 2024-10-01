#include <stdio.h>
#include "fssimplewindow.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

const double PI = 3.14159265;
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const double GRAVITY = 9.8;
const double TIME_STEP = 0.025;

const int CANNON_BASE_X = 50;
const int CANNON_BASE_Y = WIN_HEIGHT - 50;
const int CANNON_SIZE = 20;
const int BARREL_LENGTH = 30;
const double PROJECTILE_SPEED = 400;  

const int MAX_OBSTACLES = 5;
const int TARGET_WIDTH = 100;
const int TARGET_HEIGHT = 100;
const int TAIL_LENGTH = 10;
std::vector<std::pair<float, float>> cannonballTrail;

struct Projectile {
    double x, y, vx, vy;
    double tailX[TAIL_LENGTH] = {0};
    double tailY[TAIL_LENGTH] = {0};
    int tailIndex = 0;
};

struct Obstacle {
    int x, y, width, height;
    int hits;
};

// Struct for the target
struct Target {
    double x, y, vy;
};

// Function to draw the cannon
void DrawCannon(double angle)
{
    glColor3ub(0, 0, 255);
    glBegin(GL_QUADS);
    glVertex2i(CANNON_BASE_X - CANNON_SIZE / 2, CANNON_BASE_Y + CANNON_SIZE / 2);
    glVertex2i(CANNON_BASE_X + CANNON_SIZE / 2, CANNON_BASE_Y + CANNON_SIZE / 2);
    glVertex2i(CANNON_BASE_X + CANNON_SIZE / 2, CANNON_BASE_Y - CANNON_SIZE / 2);
    glVertex2i(CANNON_BASE_X - CANNON_SIZE / 2, CANNON_BASE_Y - CANNON_SIZE / 2);
    glEnd();

    glBegin(GL_LINES);
    glVertex2i(CANNON_BASE_X, CANNON_BASE_Y);
    glVertex2i(CANNON_BASE_X + BARREL_LENGTH * cos(angle * PI / 180), 
               CANNON_BASE_Y - BARREL_LENGTH * sin(angle * PI / 180));
    glEnd();
}


void RenderProjectile(Projectile &proj, int shotCount)
{
    
    switch (shotCount)
    {
        case 1: glColor3f(0, 0, 1); break;    // Blue
        case 2: glColor3f(0, 1, 1); break;    // Cyan
        case 3: glColor3f(1, 1, 0); break;    // Yellow
        case 4: glColor3f(1, 0, 1); break;    // Magenta
        default: glColor3f(1, 0, 0); break;   // Red
    }

    // Draw projectile
    glBegin(GL_POLYGON);
    for (int i = 0; i < 32; i++)
    {
        double angle = (double)i * PI / 16.0;
        glVertex2d(proj.x + 5 * cos(angle), proj.y + 5 * sin(angle));
    }
    glEnd();

    // Draw tail
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < TAIL_LENGTH; i++)
    {
        if (proj.tailX[i] != 0 || proj.tailY[i] != 0)
        {
            glVertex2d(proj.tailX[i], proj.tailY[i]);
        }
    }
    glEnd();
}

// Runge-Kutta method for updating projectile position
void UpdateProjectilePosition(Projectile &proj)
{
    double k1x = TIME_STEP * proj.vx;
    double k1y = TIME_STEP * proj.vy;
    double k1vx = 0;
    double k1vy = TIME_STEP * GRAVITY;

    double k2x = TIME_STEP * (proj.vx + k1vx / 2);
    double k2y = TIME_STEP * (proj.vy + k1vy / 2);
    double k2vx = 0;
    double k2vy = TIME_STEP * GRAVITY;

    double k3x = TIME_STEP * (proj.vx + k2vx / 2);
    double k3y = TIME_STEP * (proj.vy + k2vy / 2);
    double k3vx = 0;
    double k3vy = TIME_STEP * GRAVITY;

    double k4x = TIME_STEP * (proj.vx + k3vx);
    double k4y = TIME_STEP * (proj.vy + k3vy);
    double k4vx = 0;
    double k4vy = TIME_STEP * GRAVITY;

    proj.x += (k1x + 2 * k2x + 2 * k3x + k4x) / 6;
    proj.y += (k1y + 2 * k2y + 2 * k3y + k4y) / 6;
    proj.vy += (k1vy + 2 * k2vy + 2 * k3vy + k4vy) / 6;

    proj.tailX[proj.tailIndex] = proj.x;
    proj.tailY[proj.tailIndex] = proj.y;
    proj.tailIndex = (proj.tailIndex + 1) % TAIL_LENGTH;
}


void DrawTarget(Target &target)
{
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(target.x, target.y);
    glVertex2i(target.x + TARGET_WIDTH, target.y);
    glVertex2i(target.x + TARGET_WIDTH, target.y + TARGET_HEIGHT);
    glVertex2i(target.x, target.y + TARGET_HEIGHT);
    glEnd();
}

// Move target up and down
void MoveTarget(Target &target)
{
    target.y += target.vy * TIME_STEP;
    if (target.y < 0 || target.y > WIN_HEIGHT - TARGET_HEIGHT)
    {
        target.vy = -target.vy;
    }
}

void DrawObstacle(Obstacle &obstacle)
{
    if (obstacle.hits == 0)
        glColor3f(0, 1, 0);  // Green
    else if (obstacle.hits == 1)
        glColor3f(1, 1, 0);  // Yellow

    glBegin(GL_QUADS);
    glVertex2i(obstacle.x, obstacle.y);
    glVertex2i(obstacle.x + obstacle.width, obstacle.y);
    glVertex2i(obstacle.x + obstacle.width, obstacle.y + obstacle.height);
    glVertex2i(obstacle.x, obstacle.y + obstacle.height);
    glEnd();
}

bool IsCollision(double x, double y, int obsX, int obsY, int obsWidth, int obsHeight)
{
    return (x >= obsX && x <= obsX + obsWidth && y >= obsY && y <= obsY + obsHeight);
}

int main(void)
{
    FsOpenWindow(16, 16, WIN_WIDTH, WIN_HEIGHT, 1);
    srand(time(NULL));

    double cannonAngle = 45.0;
    Projectile projectile;
    bool projectileActive = false;
    int cannonballCount = 0;

    Target target = {WIN_WIDTH - TARGET_WIDTH, WIN_HEIGHT - TARGET_HEIGHT, -100};

    // Array to store obstacles
    Obstacle obstacles[MAX_OBSTACLES];
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        obstacles[i].width = 80 + rand() % 71;   
        obstacles[i].height = 80 + rand() % 71;   

        int minX = CANNON_BASE_X + CANNON_SIZE + 10;
        int maxX = WIN_WIDTH - obstacles[i].width;
        obstacles[i].x = minX + rand() % (maxX - minX);

        int maxY = WIN_HEIGHT - obstacles[i].height;
        obstacles[i].y = rand() % maxY;

        obstacles[i].hits = 0;  
    }

    for (;;)
    {
        FsPollDevice();
        int key = FsInkey();
        if (key == FSKEY_ESC) break;

        if (key == FSKEY_UP && cannonAngle < 90) cannonAngle += 3;
        if (key == FSKEY_DOWN && cannonAngle > 0) cannonAngle -= 3;

        if (key == FSKEY_SPACE && !projectileActive)
        {
            projectileActive = true;
            cannonballCount++;
            projectile.x = CANNON_BASE_X + BARREL_LENGTH * cos(cannonAngle * PI / 180);
            projectile.y = CANNON_BASE_Y - BARREL_LENGTH * sin(cannonAngle * PI / 180);
            projectile.vx = PROJECTILE_SPEED * cos(cannonAngle * PI / 180);
            projectile.vy = -PROJECTILE_SPEED * sin(cannonAngle * PI / 180);

            // Reset tail when a new projectile is fired
            for (int i = 0; i < TAIL_LENGTH; i++)
            {
                projectile.tailX[i] = 0;
                projectile.tailY[i] = 0;
            }
            projectile.tailIndex = 0;
        }

        if (projectileActive)
        {
            UpdateProjectilePosition(projectile);
            if (projectile.x > WIN_WIDTH || projectile.y > WIN_HEIGHT) projectileActive = false;

            for (int i = 0; i < MAX_OBSTACLES; i++)
            {
                if (IsCollision(projectile.x, projectile.y, obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height))
                {
                    obstacles[i].hits++;
                    projectileActive = false;
                    if (obstacles[i].hits >= 2)
                    {
                        obstacles[i].width = obstacles[i].height = 0;  
                    }
                    break;
                }
            }


            if (IsCollision(projectile.x, projectile.y, target.x, target.y, TARGET_WIDTH, TARGET_HEIGHT))
            {
                printf("Target hit after %d shots!\n", cannonballCount);
                break;
            }
        }

        MoveTarget(target);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawCannon(cannonAngle);
        if (projectileActive) RenderProjectile(projectile, cannonballCount);

        for (int i = 0; i < MAX_OBSTACLES; i++)
        {
            DrawObstacle(obstacles[i]);
        }

        DrawTarget(target);

        FsSwapBuffers();
        FsSleep(20);
    }

    return 0;
}

