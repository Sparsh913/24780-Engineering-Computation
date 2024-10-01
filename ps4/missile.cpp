#include <fssimplewindow.h>

void DrawMissile(int x,int y)
{
	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+11,y+-9);
	glVertex2i(x+10,y+19);
	glVertex2i(x+20,y+19);
	glVertex2i(x+20,y+-9);
	glVertex2i(x+17,y+-16);
	glVertex2i(x+15,y+-17);
	glVertex2i(x+13,y+-16);
	glVertex2i(x+11,y+-9);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+11,y+-7);
	glVertex2i(x+6,y+-6);
	glVertex2i(x+6,y+-4);
	glVertex2i(x+10,y+-4);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+20,y+-8);
	glVertex2i(x+23,y+-7);
	glVertex2i(x+24,y+-4);
	glVertex2i(x+20,y+-4);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+10,y+9);
	glVertex2i(x+6,y+12);
	glVertex2i(x+6,y+16);
	glVertex2i(x+10,y+18);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+20,y+9);
	glVertex2i(x+25,y+10);
	glVertex2i(x+25,y+16);
	glVertex2i(x+20,y+18);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+15,y+9);
	glVertex2i(x+15,y+16);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+15,y+-8);
	glVertex2i(x+15,y+-5);
	glEnd();
	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex2i(x+10,y+19);
	glVertex2i(x+11,y+29);
	glVertex2i(x+14,y+27);
	glVertex2i(x+16,y+29);
	glVertex2i(x+18,y+26);
	glVertex2i(x+20,y+28);
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
		DrawMissile(x,y);
		FsSwapBuffers();
	}

	return 0;
}
