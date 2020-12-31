#pragma comment(lib,"OpenGL32.lib") 
#pragma comment(lib,"GLu32.lib") 
//#pragma comment(lib,"GLaux.lib") 

#include <windows.h> 
#include <gl/gl.h> 
#include <gl/glu.h> 
//#include <gl/glaux.h> 
#include "stdio.h"

#pragma warning (disable : 4996)

#ifndef _CFONT_H 
#define _CFONT_H 
#define FL_NONE	0 
#define FL_OUTLINE	2 
#define FL_BACKDROP	4 
#define FL_CENTER	8
#define FL_BOLD 16
#define FL_ALL	255 

class cFont 
{ 
public: 
	cFont(); 
	cFont(char*, int, int); 
	void Print(int x, int y, int r, int g, int b, int a, BYTE flags, char *szbuf, ...); 
	void InitText();    
private: 
	void	Render(int x, int y, int r, int g, int b, int a, char *string); 
	short	cheight; 
	short	cwidth[255]; 
	char   name[20]; 
	int   height;
	int   width;
	UINT   g_FontListID; 
}; 

extern cFont g_Mini;
extern cFont g_Info;
#endif 
