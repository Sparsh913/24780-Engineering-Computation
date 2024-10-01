#include <fssimplewindow.h>

void DrawSpaceShip(int x,int y)
{
	glColor3ub(0,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+-3,y+-15);
	glVertex2i(x+-7,y+12);
	glVertex2i(x+-7,y+43);
	glVertex2i(x+-2,y+46);
	glVertex2i(x+0,y+42);
	glVertex2i(x+8,y+42);
	glVertex2i(x+11,y+46);
	glVertex2i(x+14,y+42);
	glVertex2i(x+14,y+8);
	glVertex2i(x+11,y+-15);
	glVertex2i(x+8,y+-2);
	glVertex2i(x+-1,y+-2);
	glVertex2i(x+-3,y+-15);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+-7,y+19);
	glVertex2i(x+-19,y+24);
	glVertex2i(x+-18,y+41);
	glVertex2i(x+-7,y+41);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+14,y+18);
	glVertex2i(x+24,y+22);
	glVertex2i(x+24,y+39);
	glVertex2i(x+14,y+39);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+-4,y+25);
	glVertex2i(x+-4,y+39);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+10,y+25);
	glVertex2i(x+10,y+37);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+0,y+3);
	glVertex2i(x+-1,y+12);
	glVertex2i(x+0,y+19);
	glVertex2i(x+2,y+21);
	glVertex2i(x+5,y+21);
	glVertex2i(x+7,y+18);
	glVertex2i(x+8,y+11);
	glVertex2i(x+6,y+3);
	glVertex2i(x+4,y+1);
	glVertex2i(x+2,y+1);
	glVertex2i(x+0,y+3);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+-1,y+12);
	glVertex2i(x+1,y+15);
	glVertex2i(x+4,y+15);
	glVertex2i(x+7,y+13);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+-15,y+22);
	glVertex2i(x+-15,y+17);
	glVertex2i(x+-14,y+22);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+18,y+19);
	glVertex2i(x+19,y+15);
	glVertex2i(x+20,y+20);
	glEnd();
}

int main(void)
{
	int x=400,y=300;
	FsOpenWindow(0,0,800,600,1);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			break;
		}
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		DrawSpaceShip(x,y);
		FsSwapBuffers();
	}

	return 0;
}
