#include <stdio.h>
#include <algorithm>
#include "fssimplewindow.h"
#include "yspng.h"


void MakeHistogram(unsigned int hist[256],const YsRawPngDecoder &png)
{
	for(int i=0; i<256; ++i)
	{
		hist[i]=0;
	}
	for(int i=0; i<png.wid*png.hei; ++i)
	{
		auto r=png.rgba[i*4];
		auto g=png.rgba[i*4+1];
		auto b=png.rgba[i*4+2];
		hist[r]=hist[r]+1;
		hist[g]=hist[g]+1;
		hist[b]=hist[b]+1;
	}
}

void PrintHistogram(const unsigned int hist[256])
{
	int maxNum=0;
	for(int i=0; i<256; ++i)
	{
		maxNum=std::max<unsigned int>(maxNum,hist[i]);
	}

	for(int i=0; i<256; ++i)
	{
		printf("%3d:",i);

		int len;
		len=hist[i]*80/maxNum;
		for(int j=0; j<len; ++j)
		{
			printf("#");
		}
		printf("\n");
	}
}

int main(void)
{
	FsChangeToProgramDir();

    YsRawPngDecoder png;
    if(YSOK==png.Decode("imagesample2.png"))
    {
		printf("Wid %d Hei %d\n",png.wid,png.hei);
	}
	else
	{
		printf("Failed to open file.\n");
		return 1;
	}
	png.Flip();

	unsigned int hist[256];
	MakeHistogram(hist,png);
	PrintHistogram(hist);

	FsOpenWindow(0,0,png.wid,png.hei,1);
	for(;;)
	{
		FsPollDevice();
		if(FSKEY_ESC==FsInkey())
		{
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glRasterPos2i(0,png.hei-1);
		glDrawPixels(png.wid,png.hei,GL_RGBA,GL_UNSIGNED_BYTE,png.rgba);
		FsSwapBuffers();
	}

	return 0;
}