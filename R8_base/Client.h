#ifndef __CLIENT_H__
#define __CLIENT_H__

#pragma once

#pragma warning(disable : 4996)
#pragma warning(disable : 4800)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)
#pragma warning(disable : 4430)

#include "main.h"

void HookUserMessages();
void HookStudio();

void HUD_Frame(double time);
void HUD_Redraw(float time, int intermission);
void HUD_PlayerMove(struct playermove_s *ppmove, int server);
void V_CalcRefdef(struct ref_params_s *pparams);
void CL_CreateMove(float frametime, struct usercmd_s *cmd, int active);
void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
int HUD_UpdateClientData(client_data_t *pcldata, float flTime);

void HookFunction();

typedef void(*PreS_DynamicSound_t)(int, DWORD, char *, float *, DWORD, DWORD, DWORD, DWORD);
extern PreS_DynamicSound_t PreS_DynamicSound_s;
void PreS_DynamicSound(int entid, DWORD unk0, char *szSoundFile, float *fOrigin, DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4);

extern deque < Vector > d_vSoundOrigin;
extern deque < DWORD > d_dwSoundTime;

#ifndef VectorScale
#define VectorScale(a,b) {(a)[0]*=b;(a)[1]*=b;(a)[2]*=b;}
#endif

struct entry_s 
{
	char sName[N], sValue[N];
	float *pfValue, fStep, fMax, fMin;
};
extern entry_s cEntry;

struct menu_s 
{
	float cOpen[10], cSave;
	int iCur[5], iLineHeight, iLineWidth, iDist, x[5], y[5], iIdx, maxOpen[10];
	vector < entry_s > vEntrys[2];
};
extern menu_s g_Menu;

struct settings_s 
{
	struct def_s
	{
		float fov, smooth, time, norecoil, predelay, hitbox, norecoil_delay, norecoil_target, tfov, thitbox, tdelay;
	} def;

	struct aim_s
	{
		float enable, dm, pwalls, dstmin, dstmax, type, fixz, mode, silent, fov_distbased, smooth_fov_based;
		struct weapon_s
		{
			float fov, smooth, norecoil, predelay, hitbox, norecoil_target, automatic, norecoil_fov, norecoil_smooth, norecoil_distance;
		} weapons[31];
	} aim;

	struct esp_s
	{
		float box, sound, drawtrigarea, drawaimspot, _far, drawfov, weapon, box_out, box_w, box_h;
	} esp;

	struct misc_s
	{
		float bhop, noflash, punch, crosshair, fastzoom, lambert;
	} misc;

	struct trigger_s
	{
		float enable, dm, pwalls, key, dstmin, dstmax, type, fixz, onlyzoom, keymode;
		struct weapon_s
		{
			float fov, hitbox, headonly;
		} weapons[31];
	} trigger;

	struct knifebot_s
	{
		float enable, distance;
	} kb;

	struct NoSpread_s
	{
		int calc;
	} NoSpread;

	char szBaseDir[MAX_PATH];
};
extern settings_s g_Settings;

struct player_s
{
	int iIndex, iTeam, iScrPos[2], iTrigScrPosHEAD[2], iESPScrPos[2], iSoundScrPos[2], iHitpoints, iTrigScrPosCHEST[2], iWeaponID;
	DWORD dwSoundTime;
	cl_entity_s *pEnt;
	Vector vOrigin, vBone[52], vHitbox[12], vTriggerHitbox[12], vSoundOrigin, vPreSoundOrigin, vAimOrigin, vTriggerOrigin;
	bool bIsValid, bIsAlive, bInReload, bVisible, bSoundValid, bAlive;
	float fDistance, fFov, fTriggerFov, fTrTestFov;
};
extern player_s g_Player[33];

struct soundhack_s 
{
	DWORD dwSoundTime;
	bool bSoundValid;
	Vector vSoundOrigin;
	int iSoundScrPos[2];
};
extern soundhack_s g_Sound[N];

struct aim_mode2_s 
{
	bool bAimFirstFrame = true; // AIM_TIME
	DWORD dwFirstFrameTime; // AIM_TIME

	bool bPreAimWork = true; // AIM_DELAY
	DWORD dwAimDelayTime; // AIM_DELAY
	bool bFirstFramePreAim = true; // AIM_DELAY

	bool bPreRecoilFirstFrame = true; // PRE_RCS_DELAY
	DWORD dwPreRecoilTime; // PRE_RCS_DELAY
	bool bPreRecoilWork = true; // PRE_RCS_DELAY;
};
extern aim_mode2_s g_Aim2;

class cNoFlash
{
public:
	float Flashed;
	float FadeEnd;
	screenfade_t* pScreenFade;

}; extern cNoFlash NoFlash;

struct cheat_s 
{
	DWORD dwInGame;

	bool IsGoodWeapon(int id) {
		if (id == WEAPONLIST_KNIFE || id == WEAPONLIST_C4 || id == WEAPONLIST_FLASHBANG || id == WEAPONLIST_HEGRENADE || id == WEAPONLIST_SMOKEGRENADE)
			return false;
		return true;
	}

	string GetWeaponNameByID(int id) {
		if (id == WEAPONLIST_AWP)
			return "awp.";
		else if (id == WEAPONLIST_SCOUT)
			return "scout.";
		else if (id == WEAPONLIST_AK47)
			return "ak47.";
		else if (id == WEAPONLIST_M4A1)
			return "m4a1.";
		else if (id == WEAPONLIST_DEAGLE)
			return "deagle.";
		else if (id == WEAPONLIST_FAMAS)
			return "famas.";
		else if (id == WEAPONLIST_GALIL)
			return "galil.";
		else if (id == WEAPONLIST_GLOCK18)
			return "glock.";
		else if (id == WEAPONLIST_MP5)
			return "mp5.";
		else if (id == WEAPONLIST_USP)
			return "usp.";
		else if (id == WEAPONLIST_AUG)
			return "aug.";
		else if (id == WEAPONLIST_ELITE)
			return "elite.";
		else if (id == WEAPONLIST_FIVESEVEN)
			return "fiveseven.";
		else if (id == WEAPONLIST_G3SG1)
			return "g3sg1.";
		else if (id == WEAPONLIST_M249)
			return "m249.";
		else if (id == WEAPONLIST_MAC10)
			return "mac10.";
		else if (id == WEAPONLIST_MP5)
			return "mp5.";
		else if (id == WEAPONLIST_P228)
			return "p228.";
		else if (id == WEAPONLIST_P90)
			return "p90.";
		else if (id == WEAPONLIST_SG550)
			return "sg550.";
		else if (id == WEAPONLIST_SG552)
			return "sg552.";
		else if (id == WEAPONLIST_TMP)
			return "tmp.";
		else if (id == WEAPONLIST_UMP45)
			return "ump45.";
		return "NULL";
	}

	char* GetWeaponNameByID_ESP(int id) {
		if (id == WEAPONLIST_AWP)
			return "awp";
		else if (id == WEAPONLIST_SCOUT)
			return "scout";
		else if (id == WEAPONLIST_AK47)
			return "ak47";
		else if (id == WEAPONLIST_M4A1)
			return "m4a1";
		else if (id == WEAPONLIST_DEAGLE)
			return "deagle";
		else if (id == WEAPONLIST_FAMAS)
			return "famas";
		else if (id == WEAPONLIST_GALIL)
			return "galil";
		else if (id == WEAPONLIST_GLOCK18)
			return "glock";
		else if (id == WEAPONLIST_MP5)
			return "mp5";
		else if (id == WEAPONLIST_USP)
			return "usp";
		else if (id == WEAPONLIST_AUG)
			return "aug";
		else if (id == WEAPONLIST_ELITE)
			return "elite";
		else if (id == WEAPONLIST_FIVESEVEN)
			return "five7";
		else if (id == WEAPONLIST_G3SG1)
			return "g3sg1";
		else if (id == WEAPONLIST_M249)
			return "m249";
		else if (id == WEAPONLIST_MAC10)
			return "mac10";
		else if (id == WEAPONLIST_P228)
			return "p228";
		else if (id == WEAPONLIST_P90)
			return "p90";
		else if (id == WEAPONLIST_SG550)
			return "sg550";
		else if (id == WEAPONLIST_SG552)
			return "sg552";
		else if (id == WEAPONLIST_TMP)
			return "tmp";
		else if (id == WEAPONLIST_UMP45)
			return "ump45";
		else if (id == WEAPONLIST_KNIFE)
			return "knife";
		else if (id == WEAPONLIST_C4)
			return "c4";
		else if (id == WEAPONLIST_HEGRENADE)
			return "he";
		else if (id == WEAPONLIST_FLASHBANG)
			return "flash";
		else if (id == WEAPONLIST_SMOKEGRENADE) 
			return "smoke";
		else if (id == WEAPONLIST_M3)
			return "m3";
		else if (id == WEAPONLIST_XM1014) 
			return "xm1014";
		return "unknown";
	}

	string _GetWeaponType(int wid) 
	{
		if (wid == WEAPONLIST_AWP || wid == WEAPONLIST_SCOUT || wid == WEAPONLIST_SG550 || wid == WEAPONLIST_G3SG1)
			return "snipers";
		else if (wid == WEAPONLIST_DEAGLE || wid == WEAPONLIST_ELITE || wid == WEAPONLIST_FIVESEVEN ||
			wid == WEAPONLIST_GLOCK18 || wid == WEAPONLIST_USP || wid == WEAPONLIST_P228)
			return "pistols";
		else if (!(wid == WEAPONLIST_KNIFE || wid == WEAPONLIST_C4 || wid == WEAPONLIST_FLASHBANG ||
			wid == WEAPONLIST_HEGRENADE || wid == WEAPONLIST_SMOKEGRENADE))
			return "machine";
		return "other";
	}

	int GetWeaponType(int wid)
	{
		if (wid == WEAPONLIST_AWP || wid == WEAPONLIST_SCOUT || wid == WEAPONLIST_SG550 || wid == WEAPONLIST_G3SG1) 
			return 3;
		else if (wid == WEAPONLIST_DEAGLE || wid == WEAPONLIST_ELITE || wid == WEAPONLIST_FIVESEVEN ||
			wid == WEAPONLIST_GLOCK18 || wid == WEAPONLIST_USP || wid == WEAPONLIST_P228)
			return 2;
		else if (!(wid == WEAPONLIST_KNIFE || wid == WEAPONLIST_C4 || wid == WEAPONLIST_FLASHBANG ||
			wid == WEAPONLIST_HEGRENADE || wid == WEAPONLIST_SMOKEGRENADE))
			return 1;
		return 0;
	}

	int CharToKey(char key[32])
	{
		if (!strcmp(key, "MOUSE1"))				return 1;
		if (!strcmp(key, "MOUSE2"))				return 2;
		if (!strcmp(key, "MOUSE3"))				return 4;
		if (!strcmp(key, "MOUSE4"))				return 5;
		if (!strcmp(key, "MOUSE5"))				return 6;

		if (!strcmp(key, "F1"))					return 112;
		if (!strcmp(key, "F2"))					return 113;
		if (!strcmp(key, "F3"))					return 114;
		if (!strcmp(key, "F4"))					return 115;
		if (!strcmp(key, "F5"))					return 116;
		if (!strcmp(key, "F6"))					return 117;
		if (!strcmp(key, "F7"))					return 118;
		if (!strcmp(key, "F8"))					return 119;
		if (!strcmp(key, "F9"))					return 120;
		if (!strcmp(key, "F10"))				return 121;

		if (!strcmp(key, "SPACE"))				return 32;
		if (!strcmp(key, "BACKSPACE"))			return 8;
		if (!strcmp(key, "TAB"))				return 9;
		if (!strcmp(key, "ENTER"))				return 13;
		if (!strcmp(key, "SHIFT"))				return 16;
		if (!strcmp(key, "CTRL"))				return 17;
		if (!strcmp(key, "ALT"))				return 18;
		if (!strcmp(key, "CAPSLOCK"))			return 20;
		if (!strcmp(key, "ESC"))				return 27;
		if (!strcmp(key, "INSERT"))				return 45;
		if (!strcmp(key, "PAGEUP"))				return 33;
		if (!strcmp(key, "PAGEDOWN"))			return 34;
		if (!strcmp(key, "END"))				return 35;
		if (!strcmp(key, "HOME"))				return 36;
		if (!strcmp(key, "LEFT"))				return 37;
		if (!strcmp(key, "UP"))					return 38;
		if (!strcmp(key, "RIGHT"))				return 39;
		if (!strcmp(key, "DOWN"))				return 40;
		if (!strcmp(key, "DELETE"))				return 46;
		if (!strcmp(key, "PRINTSCREEN"))		return 44;
		if (!strcmp(key, "SCROLLLOCK"))			return 145;

		if (!strcmp(key, "0"))					return NULL;
		if (!strcmp(key, "NULL"))				return NULL;

		if (!strcmp(key, "1"))					return 49;
		if (!strcmp(key, "2"))					return 50;
		if (!strcmp(key, "3"))					return 51;
		if (!strcmp(key, "4"))					return 52;
		if (!strcmp(key, "5"))					return 53;
		if (!strcmp(key, "6"))					return 54;
		if (!strcmp(key, "7"))					return 55;
		if (!strcmp(key, "8"))					return 56;
		if (!strcmp(key, "9"))					return 57;
		if (!strcmp(key, "-"))					return 189;
		if (!strcmp(key, "="))					return 187;
		if (!strcmp(key, "["))					return 219;
		if (!strcmp(key, "]"))					return 221;
		if (!strcmp(key, ";"))					return 186;
		if (!strcmp(key, "'"))					return 222;
		if (!strcmp(key, "\\"))					return 220;
		if (!strcmp(key, ","))					return 188;
		if (!strcmp(key, "."))					return 190;
		if (!strcmp(key, "/"))					return 191;

		if (!strcmp(key, "A"))					return 65;
		if (!strcmp(key, "B"))					return 66;
		if (!strcmp(key, "C"))					return 67;
		if (!strcmp(key, "D"))					return 68;
		if (!strcmp(key, "E"))					return 69;
		if (!strcmp(key, "F"))					return 70;
		if (!strcmp(key, "G"))					return 71;
		if (!strcmp(key, "H"))					return 72;
		if (!strcmp(key, "I"))					return 73;
		if (!strcmp(key, "J"))					return 74;
		if (!strcmp(key, "K"))					return 75;
		if (!strcmp(key, "L"))					return 76;
		if (!strcmp(key, "M"))					return 77;
		if (!strcmp(key, "N"))					return 78;
		if (!strcmp(key, "O"))					return 79;
		if (!strcmp(key, "P"))					return 80;
		if (!strcmp(key, "Q"))					return 81;
		if (!strcmp(key, "R"))					return 82;
		if (!strcmp(key, "S"))					return 83;
		if (!strcmp(key, "T"))					return 84;
		if (!strcmp(key, "U"))					return 85;
		if (!strcmp(key, "V"))					return 86;
		if (!strcmp(key, "W"))					return 87;
		if (!strcmp(key, "X"))					return 88;
		if (!strcmp(key, "Y"))					return 89;
		if (!strcmp(key, "Z"))					return 90;

		if (!strcmp(key, "NUM0"))				return 96;
		if (!strcmp(key, "NUM1"))				return 97;
		if (!strcmp(key, "NUM2"))				return 98;
		if (!strcmp(key, "NUM3"))				return 99;
		if (!strcmp(key, "NUM4"))				return 100;
		if (!strcmp(key, "NUM5"))				return 101;
		if (!strcmp(key, "NUM6"))				return 102;
		if (!strcmp(key, "NUM7"))				return 103;
		if (!strcmp(key, "NUM8"))				return 104;
		if (!strcmp(key, "NUM9"))				return 105;

		return 0;
	}

	bool WorldToScreen(float *pflOrigin, float *pflVecScreen)
	{
		int iResult = g_Engine.pTriAPI->WorldToScreen(pflOrigin, pflVecScreen);
		if (pflVecScreen[0] < 1 && pflVecScreen[1] < 1 && pflVecScreen[0] > -1 && pflVecScreen[1] > -1 && !iResult)
		{
			pflVecScreen[0] = pflVecScreen[0] * (g_Screen.iWidth / 2) + (g_Screen.iWidth / 2);
			pflVecScreen[1] = -pflVecScreen[1] * (g_Screen.iHeight / 2) + (g_Screen.iHeight / 2);
			return true;
		}
		return false;
	}

	bool VisibleTrace(float *pflFrom, float *pflTo)
	{
		if (!pflFrom || !pflTo) { return false; }
		pmtrace_t pTrace;
		g_Engine.pEventAPI->EV_SetTraceHull(2);
		g_Engine.pEventAPI->EV_PlayerTrace(pflFrom, pflTo, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_Local.iIndex, &pTrace);
		return ( pTrace.fraction == 1.0f );
	}

	bool PerfectWall(float *pflFrom, float *pflTo)
	{
		if (!pflFrom || !pflTo) { return false; }
		pmtrace_t pTrace;
		g_Engine.pEventAPI->EV_SetTraceHull(2);
		g_Engine.pEventAPI->EV_PlayerTrace(pflFrom, pflTo, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_Local.iIndex, &pTrace);
		return (pTrace.fraction > 0.0f);
	}

	bool CheckEntity(cl_entity_s *ent)
	{
		if ((ent->index != g_Local.iIndex) && !(ent->curstate.effects & EF_NODRAW) && ent->player && (ent->model->name != 0 || ent->model->name != "") && !(ent->curstate.messagenum < g_Engine.GetLocalPlayer()->curstate.messagenum))
			return true;
		else
			return false;
	}

	bool InTargetDiap(int x, int y, float distance)
	{
		float w = (1.0f / (distance)) * 50000;
		if (g_Screen.iWidth / 2  < (x - w) || g_Screen.iWidth / 2 > (x + w))
			return false;
		if (g_Screen.iHeight / 2 < (y - w) || g_Screen.iHeight / 2 > (y + w))
			return false;
		return true;
	}

	void GetBoneOrigin(cl_entity_s *ent)
	{
		if (ent && ent->player && ent->index != -1 && ent->index != g_Local.iIndex && this->CheckEntity(ent))
		{
			model_t* pModel = g_Studio.SetupPlayerModel(ent->index);
			studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(pModel);
			typedef float TransformMatrix[MAXSTUDIOBONES][3][4];
			TransformMatrix* pbonetransform = (TransformMatrix*)g_Studio.StudioGetBoneTransform();
			for (int i = 0; i < 52; ++i)
			{
				g_Player[ent->index].vBone[i][0] = (*pbonetransform)[i][0][3];
				g_Player[ent->index].vBone[i][1] = (*pbonetransform)[i][1][3];
				g_Player[ent->index].vBone[i][2] = (*pbonetransform)[i][2][3];
			}
		}
	}

	void GetHitboxOrigin(cl_entity_s *ent)
	{
		if (ent && ent->player && ent->index != -1 && ent->index != g_Local.iIndex && this->CheckEntity(ent))
		{
			for (int i = 1; i <= 11; ++i)
			{
				Vector vBBMin, vBBMax, vVel;

				typedef float BoneMatrix_t[MAXSTUDIOBONES][3][4];

				model_t *pModel;
				studiohdr_t *pStudioHeader;
				BoneMatrix_t *pBoneMatrix;
				mstudiobbox_t *pHitbox;

				pModel = g_Studio.SetupPlayerModel(ent->index);
				pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(pModel);
				pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();
				pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);

				VectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vBBMin);
				VectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vBBMax);

				g_Player[ent->index].vHitbox[i] = (vBBMax + vBBMin) * 0.5f;
				g_Player[ent->index].vTriggerHitbox[i] = (vBBMax + vBBMin) * 0.5f;
			}
		}
	}
};
extern cheat_s g_Cheat;

struct visuals_s 
{
	bool DrawVisuals;

	void FillRGB(int x, int y, int w, int h, int r, int g, int b, int a)
	{
		g_Engine.pfnTintRGBA(x, y, w, h, r, g, b, a);
	}

	void DrawBox(int x, int y, int w, int h, int linewidth, int r, int g, int b, int a)
	{
		FillRGB(x, y, w, linewidth, r, g, b, a);												// top
		FillRGB(x + w - linewidth, y + linewidth, linewidth, h - linewidth, r, g, b, a);		// right
		FillRGB(x, y + linewidth, linewidth, h - linewidth, r, g, b, a);						// left
		FillRGB(x + linewidth, y + h - linewidth, w - linewidth * 2, linewidth, r, g, b, a);	// bottom
	}

	void DrawString(int x, int y, int r, int g, int b, const char *fmt, ...)
	{
		va_list va_alist;
		char buf[256];
		va_start(va_alist, fmt);
		_vsnprintf(buf, sizeof(buf), fmt, va_alist);
		va_end(va_alist);
		g_Engine.pfnDrawSetTextColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
		g_Engine.pfnDrawConsoleString(x, y, buf);
	}

	void DrawShadowBox(int x, int y, int w, int h, int r, int g, int b) 
	{
		DrawBox(x, y, w, h, 1, r, g, b, 255);
		DrawBox(x - 1, y - 1, w + 2, h + 2, 1, 0, 0, 0, 255);
		DrawBox(x + 1, y + 1, w - 2, h - 2, 1, 0, 0, 0, 255);
	}

	void DrawLine(float *fStart, float *fEnd, int lw, int r, int g, int b)
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(r, g, b, 255);
		glLineWidth((float)lw);
		glBegin(GL_LINES);
		glVertex2i(fStart[0], fStart[1]);
		glVertex2i(fEnd[0], fEnd[1]);
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	void MakeESPLine(float center[3], float x1, float y1, float z1, float x2, float y2, float z2, int r, int g, int b)
	{
		float pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
		float pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
		float xy1[2], xy2[2];
		if (g_Cheat.WorldToScreen(pointPos1, xy1) && g_Cheat.WorldToScreen(pointPos2, xy2))
			DrawLine(xy1, xy2, 1, r, g, b);
	}

	void Make3DBox(float center[3], float w, float h, int r, int g, int b)
	{
		//bottom
		MakeESPLine(center, -w, -w, 0, w, -w, 0, r, g, b);
		MakeESPLine(center, -w, -w, 0, -w, w, 0, r, g, b);
		MakeESPLine(center, w, w, 0, w, -w, 0, r, g, b);
		MakeESPLine(center, w, w, 0, -w, w, 0, r, g, b);

		//middle
		MakeESPLine(center, -w, -w, 0, -w, -w, h, r, g, b);
		MakeESPLine(center, -w, w, 0, -w, w, h, r, g, b);
		MakeESPLine(center, w, -w, 0, w, -w, h, r, g, b);
		MakeESPLine(center, w, w, 0, w, w, h, r, g, b);

		//top
		MakeESPLine(center, -w, -w, h, w, -w, h, r, g, b);
		MakeESPLine(center, -w, -w, h, -w, w, h, r, g, b);
		MakeESPLine(center, w, w, h, w, -w, h, r, g, b);
		MakeESPLine(center, w, w, h, -w, w, h, r, g, b);
	}
};
extern visuals_s g_Drawing;
/*
struct d3ddrawing_s 
{
	IDirect3D9Ex *p_Object;
	IDirect3DDevice9Ex *p_Device;
	D3DPRESENT_PARAMETERS p_Params;
	ID3DXLine *p_Line;
	ID3DXFont *pFontSmall, *pFontBig;

	int DrawString(char* String, int x, int y, DWORD color, ID3DXFont* ifont)
	{
		RECT ShadowPos;
		ShadowPos.left = x + 1;
		ShadowPos.top = y + 1;
		RECT FontPos;
		FontPos.left = x;
		FontPos.top = y;
		ifont->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, color);
		return 0;
	}

	int DrawShadowString(char* String, int x, int y, int r, int g, int b, ID3DXFont* ifont)
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

	void DrawLine(float x, float y, float xx, float yy, D3DCOLOR color)
	{
		D3DXVECTOR2 dLine[2];

		this->p_Line->SetWidth(1);

		dLine[0].x = x;
		dLine[0].y = y;

		dLine[1].x = xx;
		dLine[1].y = yy;

		this->p_Line->Draw(dLine, 2, color);
	}

	void FillRGB(float x, float y, float w, float h, D3DCOLOR color)
	{
		D3DXVECTOR2 vLine[2];

		this->p_Line->SetWidth(w);

		vLine[0].x = x + w / 2;
		vLine[0].y = y;
		vLine[1].x = x + w / 2;
		vLine[1].y = y + h;

		this->p_Line->Begin();
		this->p_Line->Draw(vLine, 2, color);
		this->p_Line->End();
	}

	void DrawBox(float x, float y, float width, float height, int lw, D3DCOLOR color)
	{
		FillRGB(x, y, width, lw, color);
		FillRGB(x, y, lw, height, color);
		FillRGB((x + width), y, lw, height, color);
		FillRGB(x, y + height, width + lw, lw, color);
	}

	void DrawShadowBox(float x, float y, float width, float height, int lw, D3DCOLOR color)
	{
		DrawBox(x - 1, y - 1, width + 1, height + 1, lw + 1, D3DCOLOR_ARGB(255, 0, 0, 0));
		DrawBox(x, y, width - 1, height - 1, lw + 1, D3DCOLOR_ARGB(255, 0, 0, 0));
		DrawBox(x, y, width, height, lw, color);
	}
};
extern d3ddrawing_s g_d3dDrawing;
*/

#endif