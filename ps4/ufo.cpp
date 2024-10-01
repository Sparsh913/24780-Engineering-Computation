#include <fssimplewindow.h>

void DrawUFO(int x,int y)
{
	glColor3ub(0,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+-9,y+-8);
	glVertex2i(x+8,y+-20);
	glVertex2i(x+25,y+-10);
	glVertex2i(x+-9,y+-8);
	glVertex2i(x+-8,y+5);
	glVertex2i(x+25,y+3);
	glVertex2i(x+24,y+-11);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+5,y+-7);
	glVertex2i(x+2,y+-5);
	glVertex2i(x+2,y+0);
	glVertex2i(x+7,y+1);
	glVertex2i(x+11,y+-1);
	glVertex2i(x+11,y+-5);
	glVertex2i(x+8,y+-7);
	glVertex2i(x+5,y+-7);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+-8,y+4);
	glVertex2i(x+-19,y+12);
	glVertex2i(x+-4,y+17);
	glVertex2i(x+8,y+17);
	glVertex2i(x+25,y+13);
	glVertex2i(x+38,y+6);
	glVertex2i(x+24,y+3);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+0,y+17);
	glVertex2i(x+2,y+24);
	glVertex2i(x+9,y+25);
	glVertex2i(x+18,y+23);
	glVertex2i(x+20,y+15);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+-15,y+14);
	glVertex2i(x+-14,y+20);
	glVertex2i(x+-7,y+23);
	glVertex2i(x+0,y+20);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+35,y+8);
	glVertex2i(x+33,y+17);
	glVertex2i(x+27,y+21);
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
		DrawUFO(x,y);
		FsSwapBuffers();
	}

	return 0;
}
