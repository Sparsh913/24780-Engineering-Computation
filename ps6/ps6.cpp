#include <iostream>
#include <math.h>
#include "fssimplewindow.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

const double tolerance=1e-6;

class Equation
{
public:
	double a,b,c;
	Equation() : a(0), b(0), c(0) {}

    void Plot(float R, float G, float B) const {
        glColor3f(R, G, B);

        // Ensure we don't divide by zero
        if (b != 0) {
            double x1 = -30.0;
            double y1 = (c - a * x1) / b;
            double x2 = 30.0;
            double y2 = (c - a * x2) / b;

            glBegin(GL_LINES);
            glVertex2f(300 + x1 * 10, 300 - y1 * 10);
            glVertex2f(300 + x2 * 10, 300 - y2 * 10);
            glEnd();
        }
    }
};

class SimultanousEquation
{
public:
	Equation eqn[2];
	SimultanousEquation() : eqn() {}

	bool Solve(double &x, double &y) const {
		double D = eqn[0].a * eqn[1].b - eqn[0].b * eqn[1].a;
		if (fabs(D) < tolerance) {
			return false;
		}
		x = (eqn[1].b * eqn[0].c - eqn[0].b * eqn[1].c) / D;
		y = (eqn[0].a * eqn[1].c - eqn[1].a * eqn[0].c) / D;
		return true;
	}

	void Plot() const {
		eqn[0].Plot(1.0, 0.0, 0.0);
		eqn[1].Plot(0.0, 0.0, 1.0);
	}
};

class Axes
{
	// Draw x and y axes in black and the gridlines in grey (0.7,0.7, 0.7)
public:
	void Draw(void) const {

		glColor3f(0.7,0.7,0.7);
		glBegin(GL_LINES);
		for(int i=0; i<30; ++i) {
			glVertex2i(300+i*10,0);
			glVertex2i(300+i*10,600);
			glVertex2i(300-i*10,0);
			glVertex2i(300-i*10,600);
			glVertex2i(0,300+i*10);
			glVertex2i(600,300+i*10);
			glVertex2i(0,300-i*10);
			glVertex2i(600,300-i*10);
		}

		glColor3f(0, 0, 0); // Color for axes
        
        // Draw X axis
        glVertex2i(0, 300); // From left to right
        glVertex2i(600, 300);

        // Draw Y axis
        glVertex2i(300, 0); // From top to bottom
        glVertex2i(300, 600);

        glEnd();
		
	}
};

int main(void)
{
	SimultanousEquation eqn;
	Axes axes;
	double x,y;

	std::cout << "ax+by=c\n";
	std::cout << "dx+ey=f\n";
	std::cout << "Enter a b c d e f:";
	std::cin >>
		eqn.eqn[0].a >> eqn.eqn[0].b >> eqn.eqn[0].c >>
		eqn.eqn[1].a >> eqn.eqn[1].b >> eqn.eqn[1].c;

	if(true==eqn.Solve(x,y))
	{
	std::cout << "x=" << x << " y=" << y << '\n';
	}
	else
	{
	std::cout << "No solution.\n";
	}
	FsOpenWindow(0,0,600,600,1);
	for(;;)
	{
		FsPollDevice();
		if(FSKEY_ESC==FsInkey())
		{
		break;
		}
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glColor3f(0,0,0);
		glBegin(GL_LINES);
		glVertex2i(300,0);
		glVertex2i(300,600);
		glVertex2i(0,300);
		glVertex2i(600,300);
		glEnd();
		axes.Draw();
		eqn.Plot();
		FsSwapBuffers();
	}
	return 0;
}
