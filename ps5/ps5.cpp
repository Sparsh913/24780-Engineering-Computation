#include <iostream>
#include "fssimplewindow.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;

const double SG_PI = 3.14159265;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int INITIAL_PARTICLE_COUNT = 20;
int updateCount = 0;

// Grass class to handle individual blades of grass
class GrassBlade {
public:
    float baseX, baseY;
    float height;
    float width;
    float windFactor;
    float bendAmount;
    float bendSpeed;

    GrassBlade(float baseX, float baseY, float height, float width, float windFactor, float bendSpeed)
        : baseX(baseX), baseY(baseY), height(height), width(width), windFactor(windFactor), bendSpeed(bendSpeed), bendAmount(0.0f) {}

    void Update(float globalBend) {
        // Uniform bending controlled by global sine wave
        bendAmount = globalBend * windFactor;
    }

    // Draw the grass blade with thickness (polygon)
    void Draw() {
        float tipX = baseX + bendAmount; 
        float tipY = baseY - height;

        // Thickness for both sides of the grass blade
        float leftBaseX = baseX - width / 2;
        float rightBaseX = baseX + width / 2;

        glBegin(GL_QUADS);
        glColor3f(0.13f, 0.55f, 0.13f);  // Grass color

        glVertex2f(leftBaseX, baseY);   // Bottom-left
        glVertex2f(leftBaseX + bendAmount, tipY);  // Top-left

        glVertex2f(rightBaseX + bendAmount, tipY);  // Top-right
        glVertex2f(rightBaseX, baseY);  // Bottom-right

        glEnd();
    }
};

// Particle class for kite-like particles
class Particle {
public:
    float x, y;
    float vx, vy;
    float r, g, b, a;
    float size;
    bool isAlive;

    Particle(float startX, float startY, float velocityX = 0, float velocityY = 0) {
        x = startX;
        y = startY;
        vx = (velocityX == 0) ? (rand() % 200 - 100) / 100.0f : velocityX;
        vy = (velocityY == 0) ? (rand() % 200 - 100) / 100.0f : velocityY;
        r = rand() % 100 / 100.0f;
        g = rand() % 100 / 100.0f;
        b = rand() % 100 / 100.0f;
        a = 1.0f;  
        size = rand() % 20 + 5;
        isAlive = true;
    }

    void Update() {
        x += vx;
        y += vy;
        a -= 0.002f;
        if (a <= 0.0f) {
            isAlive = false;
        }
        if (x < 0 || x > WINDOW_WIDTH) vx = -vx;
        if (y < 0 || y > WINDOW_HEIGHT) vy = -vy;
        updateCount++;
    }

    void Draw() {
        glColor4f(r, g, b, a);
        glBegin(GL_QUADS);
        glVertex2f(x - size / 2, y - size / 2);
        glVertex2f(x + size / 2, y - size / 2);
        glVertex2f(x + size / 2, y + size / 2);
        glVertex2f(x - size / 2, y + size / 2);
        glEnd();
    }

    bool IsCollidingWith(const Particle &other) {
        float dx = x - other.x;
        float dy = y - other.y;
        float distance = sqrt(dx * dx + dy * dy);
        return distance < (size + other.size) / 2;
    }

    void ResolveCollision(Particle &other) {
        float tempVx = vx;
        float tempVy = vy;
        vx = other.vx;
        vy = other.vy;
        other.vx = tempVx;
        other.vy = tempVy;
    }
};

std::vector<Particle> particles;
std::vector<GrassBlade> grassBlades;

// Add a new particle at the mouse position
void AddParticle(int mouseX, int mouseY) {
    particles.push_back(Particle(mouseX, mouseY));
}

// Check for collisions between particles
void CheckCollisions() {
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            if (particles[i].IsCollidingWith(particles[j])) {
                particles[i].ResolveCollision(particles[j]);
            }
        }
    }
}

// Draw sky background
void DrawSkyBackground() {
    glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);

    glColor3f(0.53f, 0.81f, 0.98f);  // Light blue at the top
    glVertex2i(0, 0);
    glVertex2i(WINDOW_WIDTH, 0);

    glColor3f(1, 1, 1);  // White at the bottom
    glVertex2i(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2i(0, WINDOW_HEIGHT);

    glEnd();
}

// Draw sun with rays
void DrawSunWithRays() {
    glColor3f(1.0f, 1.0f, 0.0f);
    int numSegments = 100;
    float cx = 700, cy = 100, r = 50;

    // Draw sun
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * SG_PI * i / numSegments;
        float dx = cos(angle) * r;
        float dy = sin(angle) * r;
        glVertex2f(cx + dx, cy + dy);
    }
    glEnd();

    // Draw rays around the sun
    glColor4f(1.0f, 1.0f, 0.0f, 0.5f);  // Semi-transparent rays
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < numSegments; i += 10) {  // 10 rays around the sun
        float angle1 = 2.0f * SG_PI * i / numSegments;
        float angle2 = 2.0f * SG_PI * (i + 1) / numSegments;
        float outerRadius = r + 30;  // Length of rays

        float dx1 = cos(angle1) * outerRadius;
        float dy1 = sin(angle1) * outerRadius;
        float dx2 = cos(angle2) * outerRadius;
        float dy2 = sin(angle2) * outerRadius;

        glVertex2f(cx, cy);
        glVertex2f(cx + dx1, cy + dy1);
        glVertex2f(cx + dx2, cy + dy2);
    }
    glEnd();
}

// Update and draw all grass blades
void DrawGrass(float globalBend) {
    for (auto &blade : grassBlades) {
        blade.Update(globalBend);
        blade.Draw();
    }
}

// Update and draw all particles
void UpdateAndDrawParticles() {
    for (auto &particle : particles) {
        particle.Update();
        particle.Draw();
    }

    // Remove particles that are no longer alive (faded out)
    particles.erase(
        std::remove_if(particles.begin(), particles.end(), [](const Particle &p) { return !p.isAlive; }),
        particles.end());

    // Check for collisions between particles
    if (updateCount % 10 == 0) {
        CheckCollisions();
    }
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    DrawSkyBackground();      // Draw sky first
    DrawSunWithRays();        // Draw the sun with rays

    // Create global bending factor using a very slow sine wave for all grass blades
    float globalBend = sin(updateCount * 0.002f) * 30.0f;  // Very slow bending for more realistic effect

    DrawGrass(globalBend);    // Draw the grass at the bottom

    UpdateAndDrawParticles(); // Update and draw all particles

    FsSwapBuffers();
}

// Create initial burst of particles from the center
void CreateInitialParticles() {
    for (int i = 0; i < INITIAL_PARTICLE_COUNT; ++i) {
        float angle = (rand() % 360) * (SG_PI / 180.0);  // Random angle in radians
        float speed = (rand() % 50 + 50) / 10.0f;        // Random speed
        float vx = cos(angle) * speed;
        float vy = sin(angle) * speed;
        particles.push_back(Particle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, vx, vy));
    }
}

// Initialize grass blades
void InitializeGrass() {
    for (int i = 0; i < WINDOW_WIDTH; i += 2) {  // dense grass
        float height = rand() % 40 + 80;  // Taller grass
        float width = 4;  // Thicker grass
        float windFactor = (rand() % 5 + 3) / 10.0f;  // Uniform bend intensity
        grassBlades.push_back(GrassBlade(i, WINDOW_HEIGHT, height, width, windFactor, 0.001f));
    }
}

int main(void) {
    FsOpenWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);
    srand(time(NULL));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InitializeGrass();      // Initialize grass
    CreateInitialParticles();  // Initial burst of particles

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
        updateCount++;
        FsSleep(10);  // Frame rate control
    }

    return 0;
}
