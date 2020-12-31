#include "cFont.h"
#include <gl\gl.h>
//#include <gl\glaux.h>
#include <gl\glu.h>

#pragma warning( disable: 4244 )

cFont::cFont() 
{ 
	strcpy(name, "Lucida Console"); 
	width = 13;
	height = 1;
} 

cFont::cFont(char *i_name, int i_width, int i_height) 
{ 
	strcpy(name, i_name); 
	width = i_width; 
	height = i_height;
}

void cFont::InitText() 
{ 
	HDC hDC; 
	HFONT hFont; 
	HFONT hOldFont; 

	hDC = wglGetCurrentDC(); 
	g_FontListID = glGenLists(256); 

	hFont=CreateFont(height, width, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 
      		OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
      		DEFAULT_PITCH|FF_DONTCARE, name); 

	hOldFont = (HFONT)SelectObject(hDC, hFont); 
    
	wglUseFontBitmapsA (hDC, 0, 255, g_FontListID);
	for(int i=0; i < 255; i++) 
	{ 
		SIZE s; 
		char line[2] = { (char)i, 0 }; 
		GetTextExtentPoint(hDC, line, 1, &s); 
		cwidth[i] = s.cx; 
		cheight = s.cy; 
	} 
	SelectObject(hDC, hOldFont); 
	DeleteObject(hFont); 
} 

void cFont::Print(int x, int y, int r, int g, int b, int a, BYTE flags, char *string, ...) 
{ 
	glDisable(GL_TEXTURE_2D); 

	char		strText[256];							 
	va_list		argumentPtr; 

	va_start(argumentPtr, string);				 
	vsprintf(strText, string, argumentPtr);			 
	va_end(argumentPtr); 

	int drawlen = 0; 
	for(char *p = strText; *p; p++) { drawlen += cwidth[*p]; } 
	 
	if(flags & FL_CENTER) { x -= (drawlen / 2); } 
	if(flags & FL_OUTLINE)
	{
		//Render(x,	y-1,	0, 0, 0, 200, strText); //comment these 4 if you experience slowness
		//Render(x,	y+1,	0, 0, 0, 200, strText); //"""""""
		//Render(x-1, y,		0, 0, 0, 200, strText); //"""""""
		//Render(x+1, y,		0, 0, 0, 200, strText); //"""""""

		Render(x-1, y-1,	0, 0, 0, 200, strText);
		Render(x+1, y-1,	0, 0, 0, 200, strText);
		Render(x-1, y+1,	0, 0, 0, 200, strText);
		Render(x+1, y+1,	0, 0, 0, 200, strText);

	} 
	if(flags & FL_BACKDROP)
	{
		Render(x,y-1,	0, 0, 0, 255, strText); //comment these 4 if you experience slowness
		Render(x,y-1,	0, 0, 0, 255, strText); //"""""""
		Render(x-1, y,		0, 0, 0, 255, strText); //"""""""
		Render(x-1, y,		0, 0, 0, 255, strText); //"""""""

		Render(x-1,y-1,	0, 0, 0, 255, strText);
		Render(x,y-1,	0, 0, 0, 255, strText);
		Render(x-1,y-1,	0, 0, 0, 255, strText);
		Render(x,y-1,	0, 0, 0, 255, strText);
	}
	if(flags & FL_BOLD)
	{
		Render(x-1, y-1,	r, g, b, a, strText);
		Render(x+1, y-1,	r, g, b, a, strText);
		Render(x-1, y+1,	r, g, b, a, strText);
		Render(x+1, y+1,	r, g, b, a, strText);
	}
	Render(x,	y,	r,	g,	b, a,	strText);		//draw normally 
	 
	glEnable(GL_TEXTURE_2D); 
}  

void cFont::Render(int x, int y, int r, int g, int b, int a, char *string) 
{ 
	int i=0; 
	while(x < 0)
	{ 
		x += cwidth[string[i]]; 
		i++; if(!string[i]) { return; } 
	} 

	glColor4ub(r, g, b, a); 
	glRasterPos2i(x, y); 

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glPushAttrib(GL_LIST_BIT);							 
	glListBase(g_FontListID);						 
	glCallLists(strlen(string) - i, GL_UNSIGNED_BYTE, string + i); 
	glPopAttrib();	 
} 

cFont g_Mini("mini",5,10); // 7 12
cFont g_Info("Lucida Console",10,13);