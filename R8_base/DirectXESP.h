#include "main.h"

IDirect3D9Ex* p_Object = nullptr;
IDirect3DDevice9Ex* p_Device = nullptr;
D3DPRESENT_PARAMETERS p_Params;
ID3DXLine* p_Line = nullptr;
ID3DXFont* pFontSmall = nullptr, *pFontBig = nullptr;

int iWidth = 0;
int iHeight = 0;

int InitFrames = 0;
bool InitFlag = false, esp_work = true;

char WindowName[MAX_PATH] = "Counter-Strike";
char lWindowName[MAX_PATH] = "Vermillion Hack";

const MARGINS Margin = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
HWND tWnd;
RECT tSize;
HWND dxhWnd;
MSG Message;

inline int DrawString(char* String, int x, int y, DWORD color, ID3DXFont* ifont)
{
	RECT ShadowPos;
	ShadowPos.left = x + 1;
	ShadowPos.top = y + 1;
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	//ifont->DrawTextA(0, String, strlen(String), &ShadowPos, DT_NOCLIP, color);
	ifont->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, color);
	return 0;
}

inline int DrawShadowString(char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont)
{
	RECT Font;
	Font.left = x;
	Font.top = y;
	RECT Fonts;
	Fonts.left = x + 1;
	Fonts.top = y;
	RECT Fonts1;
	Fonts1.left = x - 1;
	Fonts1.top = y;
	RECT Fonts2;
	Fonts2.left = x;
	Fonts2.top = y + 1;
	RECT Fonts3;
	Fonts3.left = x;
	Fonts3.top = y - 1;
	ifont->DrawTextA(0, String, strlen(String), &Fonts3, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
	ifont->DrawTextA(0, String, strlen(String), &Fonts2, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
	ifont->DrawTextA(0, String, strlen(String), &Fonts1, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
	ifont->DrawTextA(0, String, strlen(String), &Fonts, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
	ifont->DrawTextA(0, String, strlen(String), &Font, DT_NOCLIP, D3DCOLOR_ARGB(255, r, g, b));
	return 0;
}

inline void DrawLine(float x, float y, float xx, float yy, D3DCOLOR color)
{
	D3DXVECTOR2 dLine[2];

	p_Line->SetWidth(1);

	dLine[0].x = x;
	dLine[0].y = y;

	dLine[1].x = xx;
	dLine[1].y = yy;

	p_Line->Draw(dLine, 2, color);
}

inline void FillRGB(float x, float y, float w, float h, D3DCOLOR color)
{
	D3DXVECTOR2 vLine[2];

	p_Line->SetWidth(w);

	vLine[0].x = x + w / 2;
	vLine[0].y = y;
	vLine[1].x = x + w / 2;
	vLine[1].y = y + h;

	p_Line->Begin();
	p_Line->Draw(vLine, 2, color);
	p_Line->End();
}

inline void DrawBox(float x, float y, float width, float height, int lw, D3DCOLOR color)
{
	FillRGB(x, y, width, lw, color);
	FillRGB(x, y, lw, height, color);
	FillRGB((x + width), y, lw, height, color);
	FillRGB(x, y + height, width + lw, lw, color);
}

inline void DrawShadowBox(float x, float y, float width, float height, int lw, D3DCOLOR color)
{
	DrawBox(x - 1, y - 1, width + 1, height + 1, lw + 1, D3DCOLOR_ARGB(255, 0, 0, 0));
	DrawBox(x, y, width - 1, height - 1, lw + 1, D3DCOLOR_ARGB(255, 0, 0, 0));
	DrawBox(x, y, width, height, lw, color);
}


/*bool _GetWorldToScreenMatrix(WorldToScreenMatrix_t *pWorldToScreenMatrix, float *flOrigin, float *flOut)
{
	flOut[0] = pWorldToScreenMatrix->flMatrix[0][0] * flOrigin[0] + pWorldToScreenMatrix->flMatrix[1][0] * flOrigin[1] + pWorldToScreenMatrix->flMatrix[2][0] * flOrigin[2] + pWorldToScreenMatrix->flMatrix[3][0];
	flOut[1] = pWorldToScreenMatrix->flMatrix[0][1] * flOrigin[0] + pWorldToScreenMatrix->flMatrix[1][1] * flOrigin[1] + pWorldToScreenMatrix->flMatrix[2][1] * flOrigin[2] + pWorldToScreenMatrix->flMatrix[3][1];
	float flZ = pWorldToScreenMatrix->flMatrix[0][3] * flOrigin[0] + pWorldToScreenMatrix->flMatrix[1][3] * flOrigin[1] + pWorldToScreenMatrix->flMatrix[2][3] * flOrigin[2] + pWorldToScreenMatrix->flMatrix[3][3];
	if (flZ != 0.0f)
	{
		float flTmp = 1.0f / flZ;
		flOut[0] *= flTmp;
		flOut[1] *= flTmp;
	}
	return (flZ <= 0.0f);
}

bool _WorldToScreen(float *flOrigin, float *pflVecScreen)
{
	// 43 PATCH - 0xE956A0
	// STEAM	- 0xEC6280
	DWORD WorldTSMarix = (DWORD)GetModuleHandle("hw.dll") + 0xEC6280;
	WorldToScreenMatrix_t* pWorldToScreenMatrix = (WorldToScreenMatrix_t*)(WorldTSMarix);
	if (!_GetWorldToScreenMatrix(pWorldToScreenMatrix, flOrigin, pflVecScreen))
	{
		if (pflVecScreen[0] < 1 && pflVecScreen[1] < 1 && pflVecScreen[0] > -1 && pflVecScreen[1] > -1)
		{
			pflVecScreen[0] = pflVecScreen[0] * (g_Screen.Width / 2) + (g_Screen.Width / 2);
			pflVecScreen[1] = -pflVecScreen[1] * (g_Screen.Height / 2) + (g_Screen.Height / 2);
		}
		return true;
	}
	return false;
}*/

/*void MakeESPLine(float center[3], float x1, float y1, float z1, float x2, float y2, float z2, DWORD color)
{
	float pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
	float pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
	float xy1[2], xy2[2];
	if (_WorldToScreen(pointPos1, xy1) && _WorldToScreen(pointPos2, xy2))
		DrawLine(xy1[0], xy1[1], xy2[0], xy2[1], color);
}*/

/*void Make3DBox(float center[3], float w, float h, DWORD color)
{
	//bottom
	MakeESPLine(center, -w, -w, 0, w, -w, 0, color);
	MakeESPLine(center, -w, -w, 0, -w, w, 0, color);
	MakeESPLine(center, w, w, 0, w, -w, 0, color);
	MakeESPLine(center, w, w, 0, -w, w, 0, color);

	//middle
	MakeESPLine(center, -w, -w, 0, -w, -w, h, color);
	MakeESPLine(center, -w, w, 0, -w, w, h, color);
	MakeESPLine(center, w, -w, 0, w, -w, h, color);
	MakeESPLine(center, w, w, 0, w, w, h, color);

	//top
	MakeESPLine(center, -w, -w, h, w, -w, h, color);
	MakeESPLine(center, -w, -w, h, -w, w, h, color);
	MakeESPLine(center, w, w, h, w, -w, h, color);
	MakeESPLine(center, w, w, h, -w, w, h, color);
}*/

int DirectXInit(HWND hWnd)
{
	p_Object = nullptr;
	p_Device = nullptr;
	p_Line = nullptr;
	pFontSmall = nullptr;

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(1);

	ZeroMemory(&p_Params, sizeof(p_Params));

	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.PresentationInterval = 1; //D3DPRESENT_INTERVAL_IMMEDIATE
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = iWidth;
	p_Params.BackBufferHeight = iHeight;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		exit(1);

	if (!p_Line)
		D3DXCreateLine(p_Device, &p_Line);

	D3DXCreateFont(p_Device, 18, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Calibri", &pFontSmall);
	D3DXCreateFont(p_Device, 24, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Consolas", &pFontBig);

	return 0;
}

int Render()
{
	p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	p_Device->BeginScene();


	//if (GetAsyncKeyState(VK_DELETE) & 1)
	//	g_pMenu.bVisible = !g_pMenu.bVisible;
	//if (g_pMenu.bVisible)
	//{ 
	//	g_pMenu.DrawMenu(100, 100, 200);

	//	if (GetAsyncKeyState(DOWN) & 1)
	//		g_pMenu.KeyEvent(DOWN);

	//	if (GetAsyncKeyState(UP) & 1)
	//		g_pMenu.KeyEvent(UP);

	//	if (GetAsyncKeyState(RIGHT) & 1)
	//		g_pMenu.KeyEvent(RIGHT);

	//	if (GetAsyncKeyState(LEFT) & 1)
	//		g_pMenu.KeyEvent(LEFT);
	//}

	char *drawStr = "Vermillion Hack";
	if (InitFrames < 255 && !InitFlag)
	{
		RECT d3drc;
		GetWindowRect(FindWindow(NULL, "Counter-Strike"), &d3drc);
		int w = d3drc.right - d3drc.left;
		int h = d3drc.bottom - d3drc.top;	
		InitFrames++;
		DrawString(drawStr, w / 2 - (strlen(drawStr) * 6), h / 6, D3DCOLOR_ARGB(InitFrames, 178, 34, 34), pFontBig);
	}

	if (InitFrames == 255)
		InitFlag = true;

	if (InitFlag && InitFrames > 0)
	{
		RECT d3drc;
		GetWindowRect(FindWindow(NULL, "Counter-Strike"), &d3drc);
		int w = d3drc.right - d3drc.left;
		int h = d3drc.bottom - d3drc.top;
		InitFrames -= 3;
		DrawString(drawStr, w / 2 - (strlen(drawStr) * 6), h / 6, D3DCOLOR_ARGB(InitFrames, 178, 34, 34), pFontBig);
	}

	/*if (g_Settings.crosshair && (g_Local.sniper || g_Settings.crosshair_sniper == 0))
	{
		FillRGB(iWidth / 2 - g_Settings.crosshair_w / 2, iHeight / 2 - 12, g_Settings.crosshair_w, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
		FillRGB(iWidth / 2, iHeight / 2 - g_Settings.crosshair_h / 2 - 12, 1, g_Settings.crosshair_h, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (g_Settings.aimbot.draw_fov && !g_Local.badweapon)
	{
		DrawBox(g_Screen.Width / 2 - g_Settings.weapons[g_Local.weaponid].aim_fov + 4, g_Screen.Height / 2 - g_Settings.weapons[g_Local.weaponid].aim_fov + 2,
			g_Settings.weapons[g_Local.weaponid].aim_fov * 2, g_Settings.weapons[g_Local.weaponid].aim_fov * 2, 1, D3DCOLOR_ARGB(255, 0, 0, 0));
	}
	if (g_Settings.aimbot.draw_punch)
	{
		float recoil = g_Settings.weapons[g_Local.weaponid].calc_recoil == 1 ? (g_Screen.Width * g_Screen.Height * 20.0f) / 2073600.0f :
			g_Settings.weapons[g_Local.weaponid].recoil;
		float x = recoil * g_Local.punch_x;
		float y = recoil * g_Local.punch_y;

		FillRGB(g_Screen.Width / 2 + 3 - x, g_Screen.Height / 2 + 1 + y, 3, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (GetAsyncKeyState(g_Settings.keys.esp_key))
		esp_work = !esp_work;

	for (int i = 0; i < 32; i++)
	{
		if (!g_Player[i].Updated)	continue;
		if (!g_Player[i].Alive)		continue;

		int h = (int)((g_Settings.esp.size_w * 500 / g_Player[i].distance));
		int w = (int)((g_Settings.esp.size_h * 500 / g_Player[i].distance));

		if (g_Player[i].esp_screen_x > 0 && g_Player[i].esp_screen_y > 0 && g_Settings.esp.mode == 1 && g_Settings.esp.enable && esp_work)
		{
			if (g_Player[i].team == 1)
			{
				if (g_Settings.esp.box)
					DrawShadowBox(g_Player[i].esp_screen_x - h + 4, g_Player[i].esp_screen_y - w + 3, h * 2, w * 2, 1, D3DCOLOR_ARGB(255, 255, 99, 71));
				if (g_Settings.esp.draw_spot)
				{ 
					FillRGB(g_Player[i].screen_x + 2, g_Player[i].screen_y - 1, 5, 5, D3DCOLOR_ARGB(255, 0, 0, 0));
					FillRGB(g_Player[i].screen_x + 3, g_Player[i].screen_y, 3, 3, D3DCOLOR_ARGB(255, 255, 255, 0));				
				}
			}
			if (g_Player[i].team == 2)
			{
				if (g_Settings.esp.box)
					DrawShadowBox(g_Player[i].esp_screen_x - h + 4, g_Player[i].esp_screen_y - w + 3, h * 2, w * 2, 1, D3DCOLOR_ARGB(255, 0, 191, 255));
				if (g_Settings.esp.draw_spot)
				{ 
					FillRGB(g_Player[i].screen_x + 2, g_Player[i].screen_y - 1, 5, 5, D3DCOLOR_ARGB(255, 0, 0, 0));
					FillRGB(g_Player[i].screen_x + 3, g_Player[i].screen_y, 3, 3, D3DCOLOR_ARGB(255, 0, 255, 255));
				}
			}

			//Vector pos = g_Player[i].esp_origin;
			//pos[2] = g_Player[i].esp_origin[2] - 35.0f;
			//Make3DBox(pos, 15, 62, D3DCOLOR_ARGB(255, 0, 255, 255));
		}
	}*/

	p_Device->EndScene();
	p_Device->PresentEx(0, 0, 0, 0, 0);
	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		Render();
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void InitD3DForm()
{
	//Sleep(3000);

	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));

	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = GetModuleHandle(0);
	wClass.lpfnWndProc = WinProc;
	wClass.lpszClassName = lWindowName;
	wClass.lpszMenuName = lWindowName;
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassExA(&wClass);

	tWnd = FindWindow(0, WindowName);

	GetWindowRect(tWnd, &tSize);
	iWidth = tSize.right - tSize.left;
	iHeight = tSize.bottom - tSize.top;
	dxhWnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, lWindowName, lWindowName, WS_POPUP, tSize.left, tSize.top, iWidth, iHeight, 0, 0, 0, 0);
	
	SetLayeredWindowAttributes(dxhWnd, 0, 1.0f, LWA_ALPHA);
	SetLayeredWindowAttributes(dxhWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
	ShowWindow(dxhWnd, SW_SHOWNOACTIVATE);

	DirectXInit(dxhWnd);

	while (true)
	{
		tWnd = FindWindow(NULL, "Counter-Strike");
		GetWindowRect(tWnd, &tSize);
		iWidth = tSize.right - tSize.left;
		iHeight = tSize.bottom - tSize.top;
		MoveWindow(dxhWnd, tSize.left, tSize.top + 23, iWidth, iHeight, true);

		if (!tWnd)
			ExitProcess(0);

		if (PeekMessage(&Message, dxhWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message);
			TranslateMessage(&Message);
		}
	}
}