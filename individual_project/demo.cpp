#include <iostream>
#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "ysglfontdata.h"
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float SG_PI = 3.14159265;

YsSoundPlayer soundPlayer;
YsSoundPlayer::SoundData backgroundMusic;

// Variables to control star lifecycle
float starSize = 50.0f;
float expansionRate = 0.15f;
float explosionRadius = 0.0f;
bool supernova = false;
int phase = -1;  // 0 = Nebula, 1 = Main Sequence, 2 = Red Giant, 3 = Supernova, 4 = Black Hole
float nebulaOffsetX = 0.0f;
float nebulaOffsetY = 0.0f;
int frameCounter = 0;  // Counter to slow down the floating effect
float accretionDiskAngle = 0.0f;  // Angle for rotating the accretion disk
const float blackHoleX = WINDOW_WIDTH / 2;
const float blackHoleY = WINDOW_HEIGHT / 2;

struct Planet {
    float x, y;
    float size;
    float red, green, blue;
};

struct Star {
    float x, y;
    float brightness;
};

std::vector<Planet> planets;
std::vector<Star> stars;

void InitSpaceBackground() {
    // Initialize planets
    planets.clear();
    for (int i = 0; i < 3; ++i) {  // Fewer, larger planets
        Planet p;
        p.x = rand() % WINDOW_WIDTH;
        p.y = rand() % WINDOW_HEIGHT;
        p.size = 40 + rand() % 40;  // Larger sizes for more impact
        p.red = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
        p.green = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
        p.blue = 1.0f;
        planets.push_back(p);
    }

    // Initialize stars
    stars.clear();
    for (int i = 0; i < 100; ++i) {  // Plenty of stars
        Star s;
        s.x = rand() % WINDOW_WIDTH;
        s.y = rand() % WINDOW_HEIGHT;
        s.brightness = 0.5f + static_cast<float>(rand() % 50) / 100.0f;  // Vary brightness
        stars.push_back(s);
    }
}


void InitPlanets() {
    for (int i = 0; i < 5; ++i) {  // Generate 5 planets
        Planet p;
        p.x = rand() % WINDOW_WIDTH;
        p.y = rand() % WINDOW_HEIGHT;
        p.size = 15 + rand() % 20;
        p.red = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
        p.green = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
        p.blue = 1.0f;

        planets.push_back(p);
    }
}

void DrawBackground() {
    // Draw dark space background
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);  // Dark blue-black space background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw stars (static or moving based on the phase)
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (auto &s : stars) {
        if (phase == 4) {
            // Move star towards the black hole center in the black hole phase
            s.x += (blackHoleX - s.x) * 0.0009f;
            s.y += (blackHoleY - s.y) * 0.0009f;
            s.brightness -= 0.0004f;  // Fade out slightly as it approaches the black hole
        }
        glColor3f(s.brightness, s.brightness, s.brightness);  // Bright white stars
        glVertex2f(s.x, s.y);
    }
    glEnd();

    // Draw planets with optional shrinking and pulling effect
    // for (auto &p : planets) {
    //     if (phase == 4) {
    //         // Move planet towards the black hole center in the black hole phase
    //         p.x += (blackHoleX - p.x) * 0.0005f;
    //         p.y += (blackHoleY - p.y) * 0.0005f;
    //         if (p.size > 1.0f) p.size *= 0.99f;  // Slowly decrease size
    //     }

    //     // Glow effect for planets
    //     for (int layer = 0; layer < 10; ++layer) {
    //         float layerSize = p.size * (1.0f + 0.1f * layer);
    //         float layerAlpha = 0.1f / (layer + 1);  // Gradually fade out

    //         glColor4f(p.red, p.green, p.blue, layerAlpha);
    //         glBegin(GL_TRIANGLE_FAN);
    //         for (int i = 0; i < 100; ++i) {
    //             float angle = 2.0f * SG_PI * i / 100.0f;
    //             float x = cos(angle) * layerSize;
    //             float y = sin(angle) * layerSize;
    //             glVertex2f(p.x + x, p.y + y);
    //         }
    //         glEnd();
    //     }
    // }
}

struct Particle {
    float x, y;            // Position
    float dx, dy;          // Velocity (direction and speed)
    float red, green, blue; // Color
    float life;            // Life span (to fade out particles)
};

std::vector<Particle> particles;
bool supernovaStarted = false;

void InitSupernovaParticles() {
    particles.clear();
    for (int i = 0; i < 300; ++i) {
        Particle p;
        p.x = WINDOW_WIDTH / 2;
        p.y = WINDOW_HEIGHT / 2;

        // Slower particle speed and randomized direction
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * SG_PI;
        float speed = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.0f;  // Slower speed
        p.dx = cos(angle) * speed;
        p.dy = sin(angle) * speed;

        // Bright colors
        p.red = 1.0f;
        p.green = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
        p.blue = 0.0f;

        // Increase lifespan
        p.life = 2.0f;  // Longer life span to extend fade-out time
        
        particles.push_back(p);
    }
    supernovaStarted = true;
}


void DrawSupernovaParticles() {
    glPointSize(3.0f);  // Set particle size
    glBegin(GL_POINTS);
    for (auto &p : particles) {
        // Update particle position
        p.x += p.dx;
        p.y += p.dy;

        // Slow down the fade-out speed
        p.life -= 0.005f;  // Slower fade-out
        if (p.life > 0) {
            glColor4f(p.red, p.green, p.blue, p.life);  // Use life as alpha for fade-out
            glVertex2f(p.x, p.y);
        }
    }
    glEnd();
    
    // Remove particles that have faded out
    particles.erase(std::remove_if(particles.begin(), particles.end(),
                                   [](Particle &p) { return p.life <= 0; }),
                    particles.end());

    // Transition to the next phase after explosion
    if (particles.empty()) {
        supernovaStarted = false;
        phase++;
    }
}


void DrawNebula() {
    srand(time(NULL));  // Seed random number generator
    // float rand_pos = rand() % 300 - 150; // Random position for nebula
    for (int i = 0; i < 30; ++i) {
        // Random position for each blob, with added offset for floating effect
        float x = WINDOW_WIDTH / 2 + rand() % 300 - 150 + nebulaOffsetX;
        float y = WINDOW_HEIGHT / 2 + rand() % 300 - 150 + nebulaOffsetY;

        // Random size for each blob
        float size = 20 + rand() % 30;

        // Set a bright, semi-transparent color with slight variation
        float red = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
        float green = 0.5f + static_cast<float>(rand() % 50) / 100.0f;
        float blue = 1.0f;

        glColor4f(red, green, blue, 0.6f);  // Adjust alpha for transparency

        glBegin(GL_TRIANGLE_FAN);
        for (int j = 0; j < 100; ++j) {
            float angle = 2.0f * SG_PI * j / 100.0f;
            float px = cos(angle) * size;
            float py = sin(angle) * size;
            glVertex2f(x + px, y + py);
        }

        glEnd();
    }

    // Slow down the offset updates using frame counter
    frameCounter++;
    // if (frameCounter >= 50) {  // Adjust this value to make the floating effect slower
    //     nebulaOffsetX += 0.01f;  // Smaller increments for extremely slow movement
    //     nebulaOffsetY += 0.005f;
        
    //     // Reset offsets periodically to prevent drifting too far
    //     if (nebulaOffsetX > 5.0f || nebulaOffsetX < -5.0f) nebulaOffsetX = 0.0f;
    //     if (nebulaOffsetY > 5.0f || nebulaOffsetY < -5.0f) nebulaOffsetY = 0.0f;

    //     frameCounter = 0;  // Reset counter after updating
    // }
}

void DrawStar(float size, int phase) {
    if (phase == 1) {
        glColor3f(1.0f, 1.0f, 0.0f);  // Yellow star
    } else if (phase == 2) {
        glColor3f(1.0f, 0.0f, 0.0f);  // Red giant
    }
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 100; ++i) {
        float angle = 2.0f * SG_PI * i / 100.0f;
        float x = cos(angle) * size;
        float y = sin(angle) * size;
        glVertex2f(WINDOW_WIDTH / 2 + x, WINDOW_HEIGHT / 2 + y);
    }
    glEnd();
    frameCounter++;
}

void DrawSupernova(float radius) {
    glColor3f(1.0f, 0.5f, 0.0f);  // Orange explosion
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 100; ++i) {
        float angle = 2.0f * SG_PI * i / 100.0f;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
        glVertex2f(WINDOW_WIDTH / 2 + x, WINDOW_HEIGHT / 2 + y);
    }
    glEnd();
    frameCounter++;
}

void DrawBlackHole(float size) {
    // Rotate the accretion disk by increasing the angle over time
    accretionDiskAngle += 0.01f;  // Adjust speed as needed

    for (int layer = 0; layer < 10; ++layer) {
        float layerSize = size * (1.5f + 0.1f * layer);
        float layerAlpha = 0.2f / (layer + 1);  // Fading transparency for each layer

        glColor4f(0.3f, 0.0f, 0.0f, layerAlpha);  // Reddish dark glow

        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < 100; ++i) {
            float angle = 2.0f * SG_PI * i / 100.0f + accretionDiskAngle;
            float x = cos(angle) * layerSize;
            float y = sin(angle) * layerSize;
            glVertex2f(blackHoleX + x, blackHoleY + y);
        }
        glEnd();
    }

    // Draw black hole core
    glColor3f(0.0f, 0.0f, 0.0f);  // Pure black core
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 100; ++i) {
        float angle = 2.0f * SG_PI * i / 100.0f;
        float x = cos(angle) * size;
        float y = sin(angle) * size;
        glVertex2f(blackHoleX + x, blackHoleY + y);
    }
    glEnd();

    // Draw a glowing halo for gravitational lensing
    for (int layer = 0; layer < 5; ++layer) {
        float haloSize = size * (2.0f + 0.1f * layer);
        float haloAlpha = 0.15f / (layer + 1);  // Fading glow effect

        glColor4f(1.0f, 1.0f, 1.0f, haloAlpha);  // White halo glow
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 100; ++i) {
            float angle = 2.0f * SG_PI * i / 100.0f;
            float x = cos(angle) * haloSize;
            float y = sin(angle) * haloSize;
            glVertex2f(blackHoleX + x, blackHoleY + y);
        }
        glEnd();
    }
}

void DrawText(const char *text, float x, float y, const char* phaseText) {
    glColor3f(1.0f, 1.0f, 1.0f);  // White text color
    glRasterPos2f(x, y);
    YsGlDrawFontBitmap20x32(text);

    // Render phase-specific text at the bottom of the screen
    glRasterPos2f(10, WINDOW_HEIGHT - 30);  // Position at the bottom
    YsGlDrawFontBitmap16x20(phaseText);
}

void Animate() {
    FsPollDevice();
    DrawBackground();  // Draw stars and planets in the background

    switch (phase) {
        case -1:  // Title screen phase
            DrawText("Lifecycle of a Star", WINDOW_WIDTH / 2 -170, WINDOW_HEIGHT / 2, "");
            frameCounter++;
            if (frameCounter > 200) {  // Show title for a short time
                phase = 0;
                frameCounter = 0;
            }
            break;

        case 0:  // Nebula phase
            DrawNebula();
            DrawText("", 0, 0, "Stellar Nebula");
            if (frameCounter > 300) {
                phase++;
            }
            break;

        case 1:  // Main Sequence phase
            DrawStar(starSize, phase);
            DrawText("", 0, 0, "Massive Star");
            starSize += expansionRate;
            if (starSize > 200.0f) phase++;
            break;

        case 2:  // Red Giant phase
            DrawStar(starSize, phase);
            DrawText("", 0, 0, "Red Supergiant");
            starSize += expansionRate * 2;
            if (starSize > 300.0f) {
                InitSupernovaParticles();
                phase++;
            }
            break;

        case 3:  // Supernova phase (particle explosion)
            if (supernovaStarted) {
                DrawSupernovaParticles();
                DrawText("", 0, 0, "Supernova Explosion");
            }
            break;

        case 4:  // Black Hole phase
            DrawBlackHole(50.0f);
            DrawText("", 0, 0, "Black Hole");
            break;

        default:
            break;
    }

    FsSwapBuffers();
}


int main() {
    FsChangeToProgramDir();
    FsOpenWindow(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);

    soundPlayer.Start();
    if (YSOK == backgroundMusic.LoadWav("bg2.wav")) {
        soundPlayer.PlayOneShot(backgroundMusic);
    }

    InitSpaceBackground();  // Initialize space background with stars and planets

    while (true) {
        if (FSKEY_ESC == FsInkey()) break;
        if (!soundPlayer.IsPlaying(backgroundMusic)) break;  // Stop if music ends
        Animate();
        if (phase == 0) FsSleep(80); // Slow down the nebula phase
        else FsSleep(10);
    }

    soundPlayer.End();
    return 0;
}
