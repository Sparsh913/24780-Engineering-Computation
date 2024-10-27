#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "fssimplewindow.h"

class CharBitmap {
public:
    int wid, hei;
    char* pix;
    CharBitmap() : wid(0), hei(0), pix(nullptr) {} // Default constructor
    ~CharBitmap() { // Destructor
        CleanUp();
    }

    void CleanUp() {
        if (pix != nullptr) {
            delete[] pix;
        }
        wid = 0;
        hei = 0;
        pix = nullptr;
    }

    void Create(int w, int h) {
        CleanUp();
        wid = w;
        hei = h;
        pix = new char[wid * hei];
        for (int i = 0; i < wid * hei; ++i) {
            pix[i] = 0;  // Initialize all pixels to 0
        }
    }

    void SetPixel(int x, int y, char p) {
        if (x >= 0 && x < wid && y >= 0 && y < hei) {
            pix[y * wid + x] = p;
        }
    }

    char GetPixel(int x, int y) const {
        if (x >= 0 && x < wid && y >= 0 && y < hei) {
            return pix[y * wid + x];
        }
        return 0;
    }

    void Draw() const {
        for (int y = 0; y < hei; ++y) {
            for (int x = 0; x < wid; ++x) {
                char pixelValue = GetPixel(x, y);
                switch (int(pixelValue)) {
                case 0: glColor3ub(0, 0, 0); break;       // Black
                case 1: glColor3ub(0, 0, 255); break;     // Blue
                case 2: glColor3ub(255, 0, 0); break;     // Red
                case 3: glColor3ub(255, 0, 255); break;   // Magenta
                case 4: glColor3ub(0, 255, 0); break;     // Green
                case 5: glColor3ub(0, 255, 255); break;   // Cyan
                case 6: glColor3ub(255, 255, 0); break;   // Yellow
                case 7: glColor3ub(255, 255, 255); break; // White
                }
                // Draw a square of 20x20 pixels
                glBegin(GL_QUADS);
                glVertex2i(x * 20, y * 20);
                glVertex2i((x + 1) * 20, y * 20);
                glVertex2i((x + 1) * 20, (y + 1) * 20);
                glVertex2i(x * 20, (y + 1) * 20);
                glEnd();
            }
        }
    }

    void SaveToFile(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open " << filename << " for writing.\n";
            return;
        }

        // Write width and height
        outFile << wid << " " << hei << std::endl;

        // Write the pixel values row by row
        for (int y = 0; y < hei; ++y) {
            for (int x = 0; x < wid; ++x) {
                outFile << int(GetPixel(x, y));
            }
            outFile << std::endl;  // Newline after each row
        }

        outFile.close();
        std::cout << "Bitmap saved to " << filename << std::endl;
    }

    void LoadBitmapFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (inFile.is_open()) {
            int width, height;
            inFile >> width >> height;
            Create(width, height);

            std::string line;
            for (int y = 0; y < height; ++y) {
                inFile >> line;
                for (int x = 0; x < width; ++x) {
                    SetPixel(x, y, line[x] - '0');
                }
            }
            inFile.close();
        } else {
            std::cerr << "Error: Unable to open file for loading bitmap.\n";
        }
    }
};

void DrawGrid(int wid, int hei) {
    glColor3ub(255, 255, 255);  // White lines for grid
    for (int i = 0; i <= wid; ++i) {
        glBegin(GL_LINES);
        glVertex2i(i * 20, 0);
        glVertex2i(i * 20, hei * 20);
        glEnd();
    }
    for (int i = 0; i <= hei; ++i) {
        glBegin(GL_LINES);
        glVertex2i(0, i * 20);
        glVertex2i(wid * 20, i * 20);
        glEnd();
    }
}

int main() {
    CharBitmap bitmap;
    int width = 0, height = 0;

    // Get the bitmap dimensions from the user
    while (true) {
        std::string input;
        std::cout << "Enter Dimension> ";
        std::getline(std::cin, input);
        sscanf(input.c_str(), "%d %d", &width, &height);

        if (width > 0 && height > 0 && width <= 64 && height <= 64) {
            break;
        } else {
            std::cout << "Invalid dimensions! Please enter again.\n";
        }
    }

    bitmap.Create(width, height);

    FsOpenWindow(0, 0, width * 20, height * 20, 1);

    while (true) {
        FsPollDevice();
        int key = FsInkey();
        if (key == FSKEY_ESC) {
            break;
        }

        // Save the bitmap if the "S" key is pressed
        if (key == FSKEY_S) {
            bitmap.SaveToFile("pattern.txt");
        }
        if (key == FSKEY_L) {
            bitmap.LoadBitmapFromFile("pattern.txt");
            std::cout << "Bitmap loaded from pattern.txt\n";
        }

        int lb, mb, rb, mx, my;
        auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);
        if (key >= FSKEY_0 && key <= FSKEY_7) {
            int colorCode = key - FSKEY_0;
            int x = mx / 20;
            int y = my / 20;
            bitmap.SetPixel(x, y, colorCode);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        bitmap.Draw();
        DrawGrid(width, height);
        FsSwapBuffers();
        FsSleep(10);
    }

    return 0;
}