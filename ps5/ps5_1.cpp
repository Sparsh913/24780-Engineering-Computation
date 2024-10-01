#include <iostream>
#include "fssimplewindow.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

// A sample particle simulator where particles interact with each other and the user click can add more particles

const double SG_PI = 3.14159265;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const double GRAVITY = 98.1;

class Particle {
    public:
        float x, y;
        float vx, vy;
        float r,g,b,a;
        float radius;
        bool isAlive;

        Particle(float startX, float startY) {
            x = startX;
            y = startY;
            vx = (rand() % 200 - 100) / 100.0f;  // Random velocity
            vy = (rand() % 200 - 100) / 100.0f;
            r = rand() % 100 / 100.0f;
            g = rand() % 100 / 100.0f;
            b = rand() % 100 / 100.0f;
            a = 1.0f;  // Initial full opacity
            radius = rand() % 5 + 1; 
        }

        void Update() {
            x += vx;
            y += vy;
            a -= 0.01f;  // Fade out
            if (a < 0.0f) {
                isAlive = false;
            }
        }

        void Draw() {
            glBegin(GL_POLYGON);
            // draw the particle as filled circles
            for (int i = 0; i < 360; i++) {
                float rad = i * SG_PI / 180.0;
                glVertex2f(x + cos(rad) * radius, y + sin(rad) * radius);
            }
        }

};

std::vector<Particle> particles;

void AddParticle(int mouseX, int mouseY) {
    particles.push_back(Particle(mouseX, mouseY));
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (auto &particle : particles) {
        particle.Update();
        particle.Draw();
    }

    FsSwapBuffers();
}

int main(void) {
    FsOpenWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);
    srand(time(NULL));

    while (true) {
        FsPollDevice();
        auto key = FsInkey();
        if (key == FSKEY_ESC) break;

        int lb, mb, rb, mx, my;
        auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);
        if (lb) {
            AddParticle(mx, my);
        }

        Display();
        FsSleep(10);
    }

    return 0;
}