#include "main.h"
#include "menu.h"
#include "nospread.h"

//FULL VERSION



//                                                                                          ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//                                                                                          ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//                                                                                          ░░░░░░░░▄███████████████▄▄░░░░░░░░░░░░
//                                                                                          ░░░░░░██████████████████████░░░░░░░░░░
//                                                                                          ░░░░▄█████░░▄▄▄▄▄▄▄▄▄░░░▀████░░░░░░░░░
//                                                                                          ░░░▐█████░░░███████████░░░████░░░░░░░░
//                                                                                          ░░░█████░░░░██▌░░░░░████░░░████░░░░░░░
//                                                                                          ░░█████░░░░░███░░░░░░███░░░█████░░░░░░
//                                                                                          ░▐████▌░░░░░████▄░▄▄████░░░░█████░░░░░ 
//                                                                                          ░▐████░░░░░░▄█████████▀░░░░░▐████░░░░░
//                                                                                          ░▐███▌░░░░▄██████████▄░░░░░░░████▌░░░░
//                                                                                          ░░███▌░░░▐███▀░░░▀█████░░░░░░████▌░░░░
//                                                                                          ░░████░░░▐██▌░░░░░░░████░░░░░████░░░░░
//                                                                                          ░░▐███▌░░▐███▄░░░░░░░███░░░░░████░░░░░
//                                                                                          ░░░████░░░███████▄▄▄████░░░░░████░░░░░
//                                                                                          ░░░░████▄░░░▀▀█████████░░░░░████▌░░░░░
//                                                                                          ░░░░░█████▄░░░░░░░░░░░░░░░░█████░░░░░░
//                                                                                          ░░░░░░▀███████▄▄▄▄▄▄▄▄▄▄▄██████░░░░░░░
//                                                                                          ░░░░░░░░▀▀███████████████████░░░░░░░░░
//                                                                                          ░░░░░░░░░░░░░░▀▀▀▀▀▀▀▀▀▀▀▀▀░░░░░░░░░░░
//                                                                                          ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
//                                                                                          ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░

//                                                                                                              ░░╔╗░░░░░░░░░░░░░░╔╗╔╗░
//                                                                                                              ░░║║░░░░░░░░░░░░░░║║║║░
//                                                                                                              ╔═╝╠══╦╗╔╦═╗╔═╗╔╗╔╣║║║░
//                                                                                                              ║╔╗║║═╣╚╝║╔╝║╔╗╣║║║║║║░
//                                                                                                              ║╚╝║║═╬╗╔╣║░║║║║╚╝║╚╣╚╗
//                                                                                                              ╚══╩══╝╚╝╚╝░╚╝╚╩══╩═╩═╝



///////////////// STRUCTURES ////////////////
cheat_s g_Cheat;
local_s g_Local;
player_s g_Player[33];
settings_s g_Settings;
soundhack_s g_Sound[N];
PreS_DynamicSound_t PreS_DynamicSound_s;
antiscreen_s g_AntiScreen;
aim_mode2_s g_Aim2;
cNoFlash NoFlash;
///////////////// STRUCTURES ////////////////

/* SOND ESP*/

int* MSG_ReadCount = nullptr;
int MSG_SavedReadCount = 0;
sizebuf_t* MSG_Buffer = nullptr;

HL_MSG_ReadByte MSG_ReadByte = nullptr;
HL_MSG_ReadBitVec3Coord MSG_ReadBitVec3Coord = nullptr;
HL_MSG_ReadBits MSG_ReadBits = nullptr;
HL_MSG_StartBitReading MSG_StartBitReading = nullptr;
HL_MSG_EndBitReading MSG_EndBitReading = nullptr;

pfnEngineMessage pSVC_sound = 0;
PEngineMsg pEngineMsgBase = 0;

/* SOUND ESP */

#define PITCH 0
#define YAW 1

///////////////// TEMP VARS /////////////////

bool	bTriggerWorkInThisFrame = false,
		bAntiSSTemp = true,
		bDrawMenu = false,
		bTriggerWork = false;

DWORD	dwTriggerDrawStatusTime;

///////////////// TEMP VARS /////////////////

///////////////// AIM VARS //////////////////
int iTargetID = 0;
float fOldFOV = 999999.9f;
///////////////// AIM VARS //////////////////

template < class CData > CData convert_string_to(string data)
{
	CData temp;
	stringstream ss(data);
	ss >> temp;
	return temp;
}

void ReadBulletControl(string szGet, float& nNum1, float& nNum2)
{
	string szNums[2];
	int nNum = 0;

	for (int i = 0; i < szGet.size(); ++i)
	{
		if (szGet[i] == ',')
		{
			nNum++;
			continue;
		}

		szNums[nNum] += szGet[i];
	}

	nNum1 = convert_string_to< float >(szNums[0]);
	nNum2 = convert_string_to< float >(szNums[1]);
}

bool CheckFOV(Vector dest, int * iScreen, float fFov)
{
	float fScrPos[2] =
	{
		abs(dest.x - iScreen[0]),
		abs(dest.y - iScreen[1])
	};
	return (fScrPos[0] <= fFov && fScrPos[1] <= fFov);
}

bool CheckFOV(Vector dest, int * iScreen, float fFovX, float fFovY)
{
	float fScrPos[2] =
	{
		abs(dest.x - iScreen[0]),
		abs(dest.y - iScreen[1])
	};
	return (fScrPos[0] <= fFovX && fScrPos[1] <= fFovY);
}

void MyVectorAngles(const float * forward, float * angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);

		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);

		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;

	while (angles[0] < -89)		{ angles[0] += 180; angles[1] += 180; }
	while (angles[0] > 89)		{ angles[0] -= 180; angles[1] += 180; }
	while (angles[1] < -180)	{ angles[1] += 360; }
	while (angles[1] > 180)		{ angles[1] -= 360; }
}

void SmoothAimAngles(float *MyViewAngles, float *AimAngles, float *OutAngles, float Smoothing)
{
	if (Smoothing < 0.0000001f)
		return;

	VectorSubtract(AimAngles, MyViewAngles, OutAngles);

	if (OutAngles[YAW] > 180.0f)
		OutAngles[YAW] -= 360.0f;
	if (OutAngles[YAW] < -180.0f)
		OutAngles[YAW] += 360.0f;

	OutAngles[PITCH] = OutAngles[PITCH] / Smoothing + MyViewAngles[PITCH];
	OutAngles[YAW] = OutAngles[YAW] / Smoothing + MyViewAngles[YAW];

	if (OutAngles[YAW] > 360.0f)
		OutAngles[YAW] -= 360.0f;
	if (OutAngles[YAW] < 0.0f)
		OutAngles[YAW] += 360.0f;
}

bool bDataCompare(const UCHAR *pData, const UCHAR *bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;

	return (*szMask) == 0;
}

ULONG FindPattern(UCHAR *bMask, char *szMask, ULONG dwAddress, ULONG dwLen)
{
	for (ULONG i = 0; i < dwLen; i++)
		if (bDataCompare((UCHAR*)(dwAddress + i), bMask, szMask))
			return (ULONG)(dwAddress + i);

	return 0;
}

float GetRecoilVar()
{
	int iID = g_Local.iWpnID;
	float recoil = 0.0f;
	switch (iID)
	{
	case WEAPONLIST_USP:
		recoil = 1.0f;
		break;
	case WEAPONLIST_GLOCK18:
		recoil = 1.0f;
		break;
	case WEAPONLIST_DEAGLE:
		recoil = 0.2f;
		break;
	case WEAPONLIST_P228:
		recoil = 1.0f;
		break;
	case WEAPONLIST_ELITE:
		recoil = 1.2f;
		break;
	case WEAPONLIST_FIVESEVEN:
		recoil = 1.0f;
		break;
	case WEAPONLIST_M4A1:
		recoil = 1.73f;
		break;
	case WEAPONLIST_AK47:
		recoil = 1.83f;
		break;
	case WEAPONLIST_AUG:
		recoil = 1.6f;
		break;
	case WEAPONLIST_SG552:
		recoil = 1.74f;
		break;
	case WEAPONLIST_M249:
		recoil = 1.85f;
		break;
	case WEAPONLIST_UMP45:
		recoil = 1.1f;
		break;
	case WEAPONLIST_MP5:
		recoil = 1.2f;
		break;
	case WEAPONLIST_TMP:
		recoil = 1.1f;
		break;
	case WEAPONLIST_MAC10:
		recoil = 1.58f;
		break;
	case WEAPONLIST_P90:
		recoil = 1.6f;
		break;
	default:
		recoil = 1.5f;
		break;
	}
	return recoil;
}

struct tempvars_s
{
	DWORD dwAimTime;
	bool bAimTime;

	DWORD dwRCSTime;
	bool bRCSTime;

	DWORD dwAimDelay;
	bool bAimDelay;

	int aimAttackID;
}
g_tempVars;

int GetBulletTime()
{
	if ( g_Local.bPistolWpn || g_Local.bSnipeWpn )
	{
		return 0;
	}
	else
	{
		return 2;
	}
}

int GetBulletDelay()
{
	if ( g_Local.bPistolWpn || g_Local.bSnipeWpn )
	{
		return 0;
	}
	else
	{
		return 3;
	}
}

void BulletConrtol_CreateMove( usercmd_s *cmd )
{
	static int SaveICLIP = 0;

	if ( cmd->buttons & IN_ATTACK )
	{
		g_Local.iShotsFired = abs( SaveICLIP - g_Local.iClip );
	}
	else
	{
		SaveICLIP = g_Local.iClip;

		if ( !g_Local.bIsNoShot )
			g_Local.iShotsFired = 0;
	}
}

void Aimbot(int i, usercmd_s * cmd)
{
	g_tempVars.aimAttackID = 0;

	if (!(cmd->buttons & IN_ATTACK) && !bTriggerWorkInThisFrame)
	{
		g_tempVars.dwAimDelay = GetTickCount(); // aim delay
		g_tempVars.bRCSTime = false;
		return;
	}

	// checks
	if (!g_Player[i].bIsValid)														return;
	if (!g_Player[i].bVisible && g_Settings.aim.pwalls)								return;
	if (g_Local.bInReload || g_Local.bBadWpn)										return;
	if (g_Local.iClip <= 0)															return;
	if (bTriggerWorkInThisFrame)													return;
	if (g_Player[i].iTeam == g_Local.iTeam && !(g_Settings.aim.dm))					return;

	g_tempVars.aimAttackID = i;

	if (g_Local.iShotsFired > GetBulletTime() 
		 && g_Player[i].fDistance >= g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance)
		return;

	if (g_Local.iShotsFired > GetBulletDelay() )
		g_tempVars.bRCSTime = true;
	else
		g_tempVars.bRCSTime = false;

	// aim delay
	if ( GetTickCount() - g_tempVars.dwAimDelay < g_Settings.aim.weapons[g_Local.iWpnID].predelay && g_Player[i].fFov > 3.0f )
	{
		g_Local.bIsNoShot = true;
		cmd->buttons &= ~IN_ATTACK;
	}
	else
	{
		g_Local.bIsNoShot = false;
	}

	float fSmooth = (g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance) ? g_Settings.aim.weapons[g_Local.iWpnID].norecoil_smooth : g_Settings.aim.weapons[g_Local.iWpnID].smooth;
	float fRecoilX = g_Local.vPunchAngle[0] * GetRecoilVar();
	float fRecoilY = g_Local.vPunchAngle[1] * GetRecoilVar();

	float MyAngles[3];
	float AimAngles[3];
	float Angles[3];

	g_pEngine->GetViewAngles(MyAngles);
	MyVectorAngles(g_Player[i].vAimOrigin - g_Local.vEye, AimAngles);

	// rcs
	if (g_tempVars.bRCSTime && g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance)
	{
		AimAngles[0] -= fRecoilX;
		AimAngles[1] -= fRecoilY;
	}

	// smooth
	if (fSmooth >= 1.0f && !g_Settings.aim.silent)	SmoothAimAngles(MyAngles, AimAngles, Angles, fSmooth);
	else											VectorCopy(AimAngles, Angles);

	// set aim

	if (g_Settings.aim.silent)	gNoSpread.ApplySilentAngles(Angles, cmd);
	else						g_pEngine->SetViewAngles(Angles);
}

void Aimbot2(int i, usercmd_s * cmd)
{
	g_tempVars.aimAttackID = 0;

	if (!(cmd->buttons & IN_ATTACK) && !bTriggerWorkInThisFrame)
	{
		g_tempVars.dwAimDelay = GetTickCount(); // aim delay
		g_tempVars.bRCSTime = false;
		return;
	}

	// checks
	if (!g_Player[i].bIsValid)														return;
	if (!g_Player[i].bVisible && g_Settings.aim.pwalls)								return;
	if (g_Local.bInReload || g_Local.bBadWpn)										return;
	if (g_Local.iClip <= 0)															return;
	if (bTriggerWorkInThisFrame)													return;
	if (g_Player[i].iTeam == g_Local.iTeam && !(g_Settings.aim.dm))					return;

	g_tempVars.aimAttackID = i;

	if (g_Local.iShotsFired > GetBulletTime() && 
		 g_Player[i].fDistance >= g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance)
		return;																							

	if ( g_Local.iShotsFired > GetBulletDelay() )
		g_tempVars.bRCSTime = true;
	else
		g_tempVars.bRCSTime = false;

	// aim delay
	if ( GetTickCount() - g_tempVars.dwAimDelay < g_Settings.aim.weapons[g_Local.iWpnID].predelay && g_Player[i].fFov > 3.0f )
	{
		g_Local.bIsNoShot = true;
		cmd->buttons &= ~IN_ATTACK;
	}
	else
	{
		g_Local.bIsNoShot = false;
	}

	float fFov = ((g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance) ? g_Settings.aim.weapons[g_Local.iWpnID].norecoil_fov : g_Settings.aim.weapons[g_Local.iWpnID].fov) * (90.0f / g_Local.iFOV);
	float fSmooth = (g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance) ? g_Settings.aim.weapons[g_Local.iWpnID].norecoil_smooth : g_Settings.aim.weapons[g_Local.iWpnID].smooth;
	float fRecoilX = g_Local.vPunchAngle[0] * GetRecoilVar();
	float fRecoilY = g_Local.vPunchAngle[1] * GetRecoilVar();

	Vector m_vecScreen;
	if (g_Cheat.WorldToScreen(g_Player[i].vAimOrigin, m_vecScreen))
	{
		float x = 0, y = 0, m_fPlayerCoord[2] = { m_vecScreen[0], m_vecScreen[1] };

		if (g_tempVars.bRCSTime && g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance)
		{
			float m_fRecoilFactor = ((g_Screen.iWidth * g_Screen.iHeight) * 20.0f) / 2073600.0f;
			x = g_Local.vPunchAngle[1] * m_fRecoilFactor;
			y = g_Local.vPunchAngle[0] * m_fRecoilFactor;
		}

		if (fSmooth >= 1.0f)
		{
			float a = (m_vecScreen[0] - g_Screen.iWidth / 2) / fSmooth;
			float b = (m_vecScreen[1] - g_Screen.iHeight / 2) / fSmooth;
			m_vecScreen[0] = g_Screen.iWidth / 2 + a;
			m_vecScreen[1] = g_Screen.iHeight / 2 + b;
		}

		if (g_tempVars.bRCSTime && g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance)
		{
			if ((int)x > m_vecScreen[0])
			{
				if (fSmooth >= 1.0f)
					g_pEngine->pfnSetMousePos((int)(m_vecScreen[0] - x / fSmooth), (int)(m_vecScreen[1] - y / fSmooth));
				else
					g_pEngine->pfnSetMousePos((int)(m_vecScreen[0] - x), (int)(m_vecScreen[1] - y));
			}
			else
			{
				if (fSmooth >= 1.0f)
					g_pEngine->pfnSetMousePos((int)(m_vecScreen[0] + x / fSmooth), (int)(m_vecScreen[1] - y / fSmooth));
				else
					g_pEngine->pfnSetMousePos((int)(m_vecScreen[0] + x), (int)(m_vecScreen[1] - y));
			}
		}
		else
			g_pEngine->pfnSetMousePos((int)m_vecScreen[0], (int)m_vecScreen[1]);
	}
}

/* START SOUND ESP BY _OR_75 - SVC_SOUND HOOK */

PEngineMsg EngineMsgByName( char* szMsgName )
{
	PEngineMsg Ptr = nullptr;
	Ptr = pEngineMsgBase;

	while ( lstrcmpA( Ptr->name , "End of List" ) )
	{
		if ( !lstrcmpA( Ptr->name , szMsgName ) )
			return Ptr;

		Ptr++;
	}

	Ptr->pfn = 0;
	return Ptr;
}

pfnEngineMessage HookEngineMsg( char *szMsgName , pfnEngineMessage pfn )
{
	PEngineMsg Ptr = nullptr;
	pfnEngineMessage Original = nullptr;

	Ptr = EngineMsgByName( szMsgName );

	if ( Ptr->pfn != 0 )
	{
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
	}
	else
	{
		MessageBoxA( 0 , szMsgName , 0 , MB_OK | MB_ICONERROR );
	}

	return Original;
}

deque < Vector > d_vSoundOrigin;
deque < DWORD > d_dwSoundTime;

void MSG_SaveReadCount()
{
	MSG_SavedReadCount = *MSG_ReadCount;
}

void MSG_RestoreReadCount()
{
	*MSG_ReadCount = MSG_SavedReadCount;
}

void SVC_Sound()
{
	MSG_SaveReadCount();

	int field_mask , volume;
	float attenuation;
	int channel , ent , sound_num;
	float origin[3] = { 0 };
	int pitch;

	MSG_StartBitReading( MSG_Buffer );

	field_mask = MSG_ReadBits( 9 );

	if ( field_mask & SND_FL_VOLUME )
		volume = MSG_ReadBits( 8 ) / 255;
	else
		volume = DEFAULT_SOUND_PACKET_VOLUME;

	if ( field_mask & SND_FL_ATTENUATION )
		attenuation = MSG_ReadBits( 8 ) / 64.0f;

	channel = MSG_ReadBits( 3 );
	ent = MSG_ReadBits( 11 );

	if ( field_mask & SND_FL_LARGE_INDEX )
		sound_num = MSG_ReadBits( 16 );
	else
		sound_num = MSG_ReadBits( 8 );

	MSG_ReadBitVec3Coord( origin );

	if ( field_mask & SND_FL_PITCH )
		pitch = MSG_ReadBits( 8 );

	MSG_EndBitReading( MSG_Buffer );

	// add_log( "SVC_Sound: %i , %i [%0.2f , %0.2f %0.2f]\n" , ent , sound_num , origin[0] , origin[1] , origin[2] );

	if ( g_AntiScreen.DrawVisuals && g_Settings.esp.sound && AIM_VERSION == 0 )
	{
		if ( !g_Cheat.VisibleTrace( g_Local.vOrigin , origin ) )
		{
			d_vSoundOrigin.push_back( origin );
			d_dwSoundTime.push_back( GetTickCount() );
		}
	}

	MSG_RestoreReadCount();
	pSVC_sound();
}


void HookEngineMessages()
{
	pEngineMsgBase = (PEngineMsg)c_Offset.FindSVCMessages();
	pSVC_sound = HookEngineMsg( "svc_sound" , SVC_Sound );
}

/* END SOUND ESP BY _OR_75 - SVC_SOUND HOOK */

/////////////////////// SOUND ESP ////////////////////////////

void PreS_DynamicSound(int entid, DWORD unk0, char *szSoundFile, float *fOrigin, DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4)
{
	/*
	if (fabs(fOrigin[0] - g_Local.vOrigin[0]) > 1.0f &&
		fabs(fOrigin[1] - g_Local.vOrigin[1]) > 1.0f &&
		fabs(fOrigin[2] - g_Local.vOrigin[2]) > 1.0f)
	{
		d_vSoundOrigin.push_back(fOrigin);
		d_dwSoundTime.push_back(GetTickCount());
	}
	*/

	if (g_Settings.esp._far)
	{
		if (entid > 0 && entid < 33 && entid != g_Local.iIndex)
		{
			g_Player[entid].vSoundOrigin = fOrigin;
			g_Player[entid].dwSoundTime = GetTickCount();
			g_Player[entid].bSoundValid = true;
		}
	}

	PreS_DynamicSound_s(entid, unk0, szSoundFile, fOrigin, unk1, unk2, unk3, unk4);
}

/////////////////////// SOUND ESP ////////////////////////////

void HookUserMessages()
{
#define HOOK_MSG(n) \
	p##n = HookUserMsg(#n, ##n);

	HOOK_MSG(ResetHUD);
	HOOK_MSG(SetFOV);
	HOOK_MSG(TeamInfo);
	HOOK_MSG(CurWeapon);
	HOOK_MSG(DeathMsg);
	HOOK_MSG(AmmoX);
	HOOK_MSG(WeaponList);
}

int StudioCheckBBox() { return 1; }

void StudioEntityLight(struct alight_s *plight)
{
	cl_entity_s *ent = g_Studio.GetCurrentEntity();

	g_Cheat.GetBoneOrigin(ent);
	g_Cheat.GetHitboxOrigin(ent);

	if ( g_Settings.misc.lambert && AIM_VERSION == 0 )
		if (g_AntiScreen.DrawVisuals && ent && ent->player)
			plight->ambientlight = 128;

	g_Studio.StudioEntityLight(plight);
}

void HookStudio()
{
	memcpy(&g_Studio, (LPVOID)g_pStudio, sizeof(engine_studio_api_t));
	g_pStudio->StudioCheckBBox = StudioCheckBBox;
	g_pStudio->StudioEntityLight = StudioEntityLight;
}

char *read(char *filename, char *section, char *key, char *def)
{
	char *out = new char[512];
	GetPrivateProfileString(section, key, def, out, 200, filename);
	return out;
}

void ReadSettingsForWeapons(char fname[])
{
	for (int i = 1; i <= 30; ++i) 
	{
		settings_s &s = g_Settings;
		string wpn = g_Cheat.GetWeaponNameByID(i);

		string	fov = wpn + "fov" ,
			smooth = wpn + "smooth" ,
			rcs = wpn + "recoil.control" ,
			rcs_delay = wpn + "recoil.delay" ,
			rcs_target = wpn + "recoil.target" ,
			rcs_fov = wpn + "recoil.fov" ,
			rcs_smooth = wpn + "recoil.smooth" ,
			rcs_dist = wpn + "recoil.distance" ,
			time = wpn + "time" ,
			hitbox = wpn + "target" ,
			headonly = wpn + "headonly" ,
			delay = wpn + "delay";
			//bullet_control = wpn + "bullet.control";
			//automatic = wpn + "auto";

		if (wpn != "NULL") 
		{
			s.aim.weapons[i].fov = atof(read(fname, "aimbot", (char*)fov.c_str(), "0"));
			s.aim.weapons[i].smooth = atof(read(fname, "aimbot", (char*)smooth.c_str(), "0"));
			s.aim.weapons[i].norecoil_target = atof(read(fname, "aimbot", (char*)rcs_target.c_str(), "0"));
			s.aim.weapons[i].norecoil_fov = atof(read(fname, "aimbot", (char*)rcs_fov.c_str(), "0"));
			s.aim.weapons[i].norecoil_smooth = atof(read(fname, "aimbot", (char*)rcs_smooth.c_str(), "0"));
			s.aim.weapons[i].norecoil_distance = atof(read(fname, "aimbot", (char*)rcs_dist.c_str(), "0"));			
			s.aim.weapons[i].hitbox = atof(read(fname, "aimbot", (char*)hitbox.c_str(), "0"));
			s.aim.weapons[i].predelay = atof(read(fname, "aimbot", (char*)delay.c_str(), "0"));
			//s.aim.weapons[i].automatic = atof(read(fname, "aimbot", (char*)automatic.c_str(), "0"));
			s.trigger.weapons[i].fov = atof(read(fname, "triggerbot", (char*)fov.c_str(), "0"));
			s.trigger.weapons[i].headonly = atof(read(fname, "triggerbot", (char*)headonly.c_str(), "0"));

			//ReadBulletControl((string)read(fname, "aimbot", (char*)bullet_control.c_str(), "0"), s.aim.weapons[i].norecoil_delay, s.aim.weapons[i].time);

			if (s.aim.weapons[i].norecoil_distance < 0.01f)
				s.aim.weapons[i].norecoil_distance = 0.01f; 
		}
		else 
		{
			s.aim.weapons[i].fov = 0;
			s.aim.weapons[i].smooth = 0;
			s.aim.weapons[i].norecoil = 0;
			//s.aim.weapons[i].norecoil_delay = 0;
			s.aim.weapons[i].norecoil_target = 0;
			s.aim.weapons[i].norecoil_fov = 0;
			s.aim.weapons[i].norecoil_smooth = 0;
			//s.aim.weapons[i].time = 0;
			s.aim.weapons[i].hitbox = 7;
			s.aim.weapons[i].predelay = 0;

			s.trigger.weapons[i].fov = 0;
			s.trigger.weapons[i].headonly = 0;
		}
	}
}

void ReadSettings(char fname[])
{
	settings_s &s = g_Settings;
	s.aim.enable = atof(read(fname, "aimbot", "aimbot.enabled", "0"));
	s.aim.dm = atof(read(fname, "aimbot", "aimbot.deathmatch", "0"));
	s.aim.pwalls = atof(read(fname, "aimbot", "aimbot.visible.check", "0"));
	s.aim.type = atof(read(fname, "aimbot", "aimbot.type", "0"));
	s.aim.mode = atof(read(fname, "aimbot", "aimbot.mode", "0"));
	s.aim.silent = atof(read(fname, "aimbot", "aimbot.silent", "0"));
	s.aim.fov_distbased = atof(read(fname, "aimbot", "aimbot.fov.distance.based", "0"));

	s.trigger.enable = atof(read(fname, "triggerbot", "triggerbot.enabled", "0"));
	s.trigger.dm = atof(read(fname, "triggerbot", "triggerbot.deathmatch", "0"));
	s.trigger.dstmin = atof(read(fname, "triggerbot", "triggerbot.distance.min", "0"));
	s.trigger.dstmax = atof(read(fname, "triggerbot", "triggerbot.distance.max", "0"));
	s.trigger.pwalls = atof(read(fname, "triggerbot", "triggerbot.visible.check", "0"));
	s.trigger.key = (float)g_Cheat.CharToKey(read(fname, "triggerbot", "triggerbot.key", "0"));
	s.trigger.keymode = atof(read(fname, "triggerbot", "triggerbot.key.mode", "0"));
	s.trigger.onlyzoom = atof(read(fname, "triggerbot", "triggerbot.onlyzoom", "0"));

	s.esp.box = atof(read(fname, "esp", "esp.box", "0"));
	s.esp.box_out = atof(read(fname, "esp", "esp.box.outline", "0"));
	s.esp.box_w = atof(read(fname, "esp", "esp.box.width", "0"));
	s.esp.box_h = atof(read(fname, "esp", "esp.box.height", "0"));
	s.esp._far = atof(read(fname, "esp", "esp.far", "0"));
	s.esp.drawaimspot = atof(read(fname, "esp", "esp.aimbot.hitbox", "0"));
	s.esp.drawtrigarea = atof(read(fname, "esp", "esp.triggerbot.area", "0"));
	s.esp.drawfov = atof(read(fname, "esp", "esp.draw.fov", "0"));
	s.esp.weapon = atof(read(fname, "esp", "esp.weapon", "0"));
	s.esp.sound = atof(read(fname, "esp", "esp.sound", "0"));

	s.kb.enable = atof(read(fname, "knifebot", "knifebot.enabled", "0"));
	s.kb.distance = atof(read(fname, "knifebot", "knifebot.distance", "0"));

	s.misc.bhop = atof(read(fname, "misc", "misc.bhop", "0"));
	s.misc.noflash = atof(read(fname, "misc", "misc.noflash", "0"));
	s.misc.punch = atof(read(fname, "misc", "misc.draw.punchangle", "0"));
	s.misc.fastzoom = atof(read(fname, "misc", "misc.fastzoom", "0"));
	s.misc.lambert = atof(read(fname, "misc", "misc.lambert", "0"));

	ReadSettingsForWeapons(fname);
}

static int Timer = 6;
static int Timer2 = 6;
static bool AntiScreenshot;
static bool AntiScreenshot2;
typedef void(*con_func)();
con_func real_screen;
con_func real_screen2;
void fake_screen() { AntiScreenshot = true; }
void fake_screen2() { AntiScreenshot2 = true; }

typedef struct cmd_s
{
	struct cmd_s	*pNext;
	PCHAR			pszName;
	DWORD			pfnFunc;
	BYTE			bCrap[20];
}	cmd_t, *pcmd_t;

DWORD CopyCmd(char*oldName, char*newName, void(*oldFunc)())
{
	pcmd_t pCmd = g_pEngine->pfnGetCmdList();
	while (pCmd != NULL)
	{
		if (!strcmpi(pCmd->pszName, oldName))
		{
			pCmd->pszName = newName;
			g_pEngine->pfnAddCommand(oldName, oldFunc);
			return pCmd->pfnFunc;
		}
		pCmd = pCmd->pNext;
	}
	return 0;
}

void HUD_Frame(double time)
{
	static bool FirstFrame = true;

	if (PUBLIC_VERSION == 0)
	{
		if (bAntiSSTemp)
		{
			bAntiSSTemp = false;
			g_AntiScreen.time_scr = GetTickCount();
			g_AntiScreen.temp = 0;
		}

		if (GetTickCount() - g_AntiScreen.time_scr > 10000)
		{
			g_AntiScreen.DrawVisuals = false;
			g_AntiScreen.temp++;
			if (g_AntiScreen.temp > 10)
			{
				bAntiSSTemp = true;
				g_AntiScreen.ScreenFirst = TRUE;
				DWORD sz = ( g_Screen.iWidth * g_Screen.iHeight ) * 3;
				free( (PBYTE)g_AntiScreen.BufferScreen );
				PBYTE buf = (PBYTE)malloc( sz );
				glReadPixels( 0 , 0 , g_Screen.iWidth , g_Screen.iHeight , GL_RGB , GL_UNSIGNED_BYTE , buf );
				free( (PBYTE)buf );
			}
		}
	}

	if (FirstFrame)
	{
		g_Screen.iSize = sizeof(SCREENINFO);
		g_pEngine->pfnGetScreenInfo(&g_Screen);

		HookEngineMessages();

		NoFlash.pScreenFade = *(screenfade_t**)((DWORD)g_pEngine->pfnSetScreenFade + 0x17);

		if (IsBadReadPtr(NoFlash.pScreenFade, sizeof(screenfade_t)))
			NoFlash.pScreenFade = *(screenfade_t**)((DWORD)g_pEngine->pfnSetScreenFade + 0x18);
		
		pUserMsgBase = c_Offset.FindUserMsgBase();

		int len = strlen( BaseDir );

		BaseDir[len - 1] = 'i';
		BaseDir[len - 2] = 'n';
		BaseDir[len - 3] = 'i';

		strcpy(g_Settings.szBaseDir, BaseDir);
		
		ReadSettings(g_Settings.szBaseDir);

		HookUserMessages();
		HookStudio();

		g_Settings.NoSpread.calc = 3;

		if (PUBLIC_VERSION)
		{
			if ( AIM_VERSION == 0 )
			{
				real_screen = (con_func)CopyCmd( "snapshot" , "" , fake_screen );
				real_screen2 = (con_func)CopyCmd( "screenshot" , "" , fake_screen2 );
			}

			if (VOVAN_VERSION == 0)
			{
				g_pEngine->pfnClientCmd("toggleconsole");

				g_pEngine->pfnConsolePrint("  R8 by devr null \n");
				g_pEngine->pfnConsolePrint("  for CSX Community \n\n");
				g_pEngine->pfnConsolePrint("  © 2011-2021\n");
				g_pEngine->pfnConsolePrint("  getze.xyz/csx\n");

				
//                            ░█████╗░░██████╗██╗░░██╗  ░█████╗░░█████╗░███╗░░░███╗███╗░░░███╗██╗░░░██╗███╗░░██╗██╗████████╗██╗░░░██╗
//                            ██╔══██╗██╔════╝╚██╗██╔╝  ██╔══██╗██╔══██╗████╗░████║████╗░████║██║░░░██║████╗░██║██║╚══██╔══╝╚██╗░██╔╝
//                            ██║░░╚═╝╚█████╗░░╚███╔╝░  ██║░░╚═╝██║░░██║██╔████╔██║██╔████╔██║██║░░░██║██╔██╗██║██║░░░██║░░░░╚████╔╝░
//                            ██║░░██╗░╚═══██╗░██╔██╗░  ██║░░██╗██║░░██║██║╚██╔╝██║██║╚██╔╝██║██║░░░██║██║╚████║██║░░░██║░░░░░╚██╔╝░░
//                            ╚█████╔╝██████╔╝██╔╝╚██╗  ╚█████╔╝╚█████╔╝██║░╚═╝░██║██║░╚═╝░██║╚██████╔╝██║░╚███║██║░░░██║░░░░░░██║░░░
//                            ░╚════╝░╚═════╝░╚═╝░░╚═╝  ░╚════╝░░╚════╝░╚═╝░░░░░╚═╝╚═╝░░░░░╚═╝░╚═════╝░╚═╝░░╚══╝╚═╝░░░╚═╝░░░░░░╚═╝░░░
			}
		}
		else
		{
			if ( AIM_VERSION == 0 )
			{
				DWORD sz = g_Screen.iWidth * g_Screen.iHeight * 3;
				PBYTE buf = (PBYTE)malloc( sz );
				glReadPixels_s( 0 , 0 , g_Screen.iWidth , g_Screen.iHeight , GL_RGB , GL_UNSIGNED_BYTE , buf );
				free( (PBYTE)buf );
			}
		}

		FirstFrame = false;
	}

	if (GetAsyncKeyState(VK_F4) & 1)
		ReadSettings(g_Settings.szBaseDir);

	if (GetAsyncKeyState(VK_F7) & 1)
	{
		if (g_Settings.aim.enable)
			g_Settings.aim.enable = 0;
		else
			g_Settings.aim.enable = 1;
	}

	g_pEngine->pfnGetScreenInfo(&g_Screen);

	if (PUBLIC_VERSION)
	{
		if (AntiScreenshot )
		{
			Timer--;
			if (Timer <= 0)
			{
				if (real_screen)
					real_screen();
				Timer = 10;
				AntiScreenshot = false;
			}
		}

		if (AntiScreenshot2 )
		{
			Timer2--;
			if (Timer2 <= 0)
			{
				if (real_screen2)
					real_screen2();
				Timer2 = 10;
				AntiScreenshot2 = false;
			}
		}

		g_AntiScreen.DrawVisuals = !(AntiScreenshot || AntiScreenshot2);
	}

	return g_Client.HUD_Frame(time);
}

void HUD_Redraw(float time, int intermission)
{
	g_Client.HUD_Redraw(time, intermission);

	if (g_AntiScreen.DrawVisuals)
	{
		if (PUBLIC_VERSION && VOVAN_VERSION == 0 && AIM_VERSION == 0 )
			g_Drawing.DrawString(5, 130, 205, 0, 0, "R8");

		gNoSpread.DrawSpread();
	}

	g_Drawing.DrawString( g_Screen.iWidth / 2 , 10 , 255 , 0 , 0 , "%i" , g_Local.iClip );
	g_Drawing.DrawString( g_Screen.iWidth / 2 , 20 , 255 , 0 , 0 , "%i" , g_Local.iShotsFired );

	if (g_AntiScreen.DrawVisuals && g_Settings.esp.sound && AIM_VERSION == 0 )
	{
		for (int i = 0; i < d_vSoundOrigin.size(); ++i)
		{
			if (GetTickCount() - d_dwSoundTime[0] > 300)
			{
				d_dwSoundTime.pop_front();
				d_vSoundOrigin.pop_front();
			}
			else
				break;
		}

		for (int i = 0; i < d_vSoundOrigin.size(); ++i)
		{
			float tmp[3];
			int w = (1000000 / (VectorDistance(d_vSoundOrigin[i], g_Local.vEye) * g_Local.iFOV)) * (g_Screen.iWidth * g_Screen.iHeight) / 995328;

			if (g_Cheat.WorldToScreen(d_vSoundOrigin[i], tmp) && !g_Cheat.VisibleTrace(g_Local.vEye, d_vSoundOrigin[i]))
				g_Drawing.DrawShadowBox((int)tmp[0] - w, (int)tmp[1] - w, w * 2, w * 2, 255, 255, 255);
		}
	}

	g_Local.pEnt = g_pEngine->GetLocalPlayer();
	g_Local.iIndex = g_Local.pEnt->index;
	g_Local.bIsAlive = g_Local.pEnt && !(g_Local.pEnt->curstate.effects & EF_NODRAW) && g_Local.pEnt->player && g_Local.pEnt->curstate.movetype != 6 && g_Local.pEnt->curstate.movetype != 0;

	net_status_s st;
	g_pEngine->pNetAPI->Status(&st);
	g_Local.fPing = st.latency * 1000;

	for (int i = 0; i < 33; ++i)
	{
		float fTemp[2];
		player_s& p = g_Player[i];

		p.pEnt = g_pEngine->GetEntityByIndex(i);
		p.bIsAlive = p.pEnt && !(p.pEnt->curstate.effects & EF_NODRAW) && p.pEnt->player && p.pEnt->curstate.movetype != 6 && p.pEnt->curstate.movetype != 0;
		p.bIsValid = g_Cheat.CheckEntity(p.pEnt) && p.bIsAlive;

		if (g_Settings.esp._far && g_AntiScreen.DrawVisuals && AIM_VERSION == 0 )
		{
			if (GetTickCount() - p.dwSoundTime >= 300)
				p.bSoundValid = false;

			if (g_Cheat.VisibleTrace(g_Local.vEye, p.vSoundOrigin))
				p.bSoundValid = false;

			if (p.bSoundValid)
			{
				if (g_Cheat.WorldToScreen(p.vSoundOrigin, fTemp))
				{
					p.iSoundScrPos[0] = (int)fTemp[0];
					p.iSoundScrPos[1] = (int)fTemp[1];
				}
				else
				{
					p.iSoundScrPos[0] = -1;
					p.iSoundScrPos[1] = -1;
					p.bSoundValid = false;
				}
			}
			if (p.bSoundValid)
			{
				float DIST = VectorDistance(p.vSoundOrigin, g_Local.vEye);
				int w = (float)(8000.0f * 90.0f) / (DIST * g_Local.iFOV);
				int h = (float)(8000.0f * 90.0f) / (DIST * g_Local.iFOV);
				int x = p.iSoundScrPos[0], y = p.iSoundScrPos[1];
				int r = 255, g = 30, b = 255;
				p.iTeam == 1 ? b = 30 : r = 30;
				g_Drawing.DrawBox(x - (int)w, y - (int)h, (int)w * 2, (int)h * 2, 1, r, g, b, 255);
			}
		}

		if (!p.bIsValid)
			continue;

		if (g_Settings.aim.type == 1)
		{
			g_Settings.aim.weapons[g_Local.iWpnID].hitbox = min(11, g_Settings.aim.weapons[g_Local.iWpnID].hitbox);

			if (!(g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance))
				p.vAimOrigin = p.vHitbox[(int)g_Settings.aim.weapons[g_Local.iWpnID].hitbox];
			else
				p.vAimOrigin = p.vHitbox[(int)g_Settings.aim.weapons[g_Local.iWpnID].norecoil_target];

		}
		else if (g_Settings.aim.type == 2)
		{
			g_Settings.aim.weapons[g_Local.iWpnID].hitbox = min(51, g_Settings.aim.weapons[g_Local.iWpnID].hitbox);

			if (!(g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance))
				p.vAimOrigin = p.vBone[(int)g_Settings.aim.weapons[g_Local.iWpnID].hitbox];
			else
				p.vAimOrigin = p.vBone[(int)g_Settings.aim.weapons[g_Local.iWpnID].norecoil_target];

		}

		p.vOrigin = p.pEnt->origin;
		p.fDistance = VectorDistance(g_Local.vEye, p.vAimOrigin);
		p.bVisible = g_Cheat.VisibleTrace(g_Local.vEye, p.vAimOrigin);

		if (!p.bVisible && g_Cheat.VisibleTrace(g_Local.vEye, p.vHitbox[11]))
			p.vAimOrigin = p.vHitbox[11];
		if (!p.bVisible && g_Cheat.VisibleTrace(g_Local.vEye, p.vHitbox[7]))
			p.vAimOrigin = p.vHitbox[7];

		float cx1 = g_Screen.iWidth / 2, cy1 = g_Screen.iHeight / 2;
		p.fFov = (float)sqrt((cx1 - p.iScrPos[0]) * (cx1 - p.iScrPos[0]) + (cy1 - p.iScrPos[1]) * (cy1 - p.iScrPos[1]));

		p.vOrigin[2] -= 5;
		if (g_Cheat.WorldToScreen(p.vOrigin, fTemp))
			p.iESPScrPos[0] = (int)fTemp[0],
			p.iESPScrPos[1] = (int)fTemp[1];
		else
			p.iESPScrPos[0] = -1,
			p.iESPScrPos[1] = -1;

		if (g_Cheat.WorldToScreen(p.vAimOrigin, fTemp))
			p.iScrPos[0] = (int)fTemp[0],
			p.iScrPos[1] = (int)fTemp[1];
		else
			p.iScrPos[0] = -1,
			p.iScrPos[1] = -1;

		if (g_Cheat.WorldToScreen(p.vHitbox[11], fTemp))
			p.iTrigScrPosHEAD[0] = (int)fTemp[0],
			p.iTrigScrPosHEAD[1] = (int)fTemp[1];
		else
			p.iTrigScrPosHEAD[0] = -1,
			p.iTrigScrPosHEAD[1] = -1;

		if (g_Cheat.WorldToScreen(p.vHitbox[7], fTemp))
			p.iTrigScrPosCHEST[0] = (int)fTemp[0],
			p.iTrigScrPosCHEST[1] = (int)fTemp[1];
		else
			p.iTrigScrPosCHEST[0] = -1,
			p.iTrigScrPosCHEST[1] = -1;

		if (!p.bIsValid)
			continue;

		/*
		Vector pOrigin = p.vOrigin;
		pOrigin[2] += p.pEnt->curstate.mins.z;
		int box3dh = p.pEnt->curstate.maxs.z - p.pEnt->curstate.mins.z;
		int box3dw = 15;
		g_Drawing.Make3DBox(pOrigin, box3dw, box3dh, 255, 255, 255);
		*/

		/*
		Vector vViewAngles;
		g_pEngine->GetViewAngles(vViewAngles);
		float fCurrentFOV = fCalcFOV(g_Local.ppmove->forward, p.vAimOrigin - g_Local.vEye);
		g_pEngine->Con_NPrintf(i, "Calculated FOV: %.2f", fCurrentFOV);
		*/

		if (g_AntiScreen.DrawVisuals && AIM_VERSION == 0 )
		{
			if (g_Settings.esp.weapon) 
			{
				int w = ((8600 * 100) / (p.fDistance * g_Local.iFOV)) * (g_Screen.iWidth * g_Screen.iHeight) / 995328;
				int h = ((18000 * 100) / (p.fDistance * g_Local.iFOV)) * (g_Screen.iWidth * g_Screen.iHeight) / 995328;
				int x = p.iESPScrPos[0];
				int y = p.iESPScrPos[1];
				int r = 255, g = 30, b = 255;
				p.iTeam == 1 ? b = 30 : r = 30;
				model_s * pModel = g_Studio.GetModelByIndex(g_pEngine->GetEntityByIndex(i)->curstate.weaponmodel);
				if (pModel && pModel->name)
				{
					char *m = pModel->name, wpn[32];
					if (!strstr(m, "shield"))
					{
						int iLen = (int)strlen(m + 9) - 4;
						strncpy(wpn, m + 9, iLen);
						wpn[iLen] = '\0';
					}
					else
					{
						int iLen = (int)strlen(m + 16) - 4;
						strncpy(wpn, m + 16, iLen);
						wpn[iLen] = '\0';
					}
					g_Drawing.DrawString(x - (int)w, y - (int)h - 18, r, g, b, "%s", wpn);
				}
			}

			if (g_Settings.esp.box)
			{
				if (p.iESPScrPos[0] > 0 &&
					p.iESPScrPos[1] > 0 &&
					p.iESPScrPos[0] < g_Screen.iWidth &&
					p.iESPScrPos[1] < g_Screen.iHeight)
				{
					int w = ((g_Settings.esp.box_w * 46500) / (p.fDistance * g_Local.iFOV)) * (g_Screen.iWidth * g_Screen.iHeight) / 995328;
					int h = ((g_Settings.esp.box_h * 46500) / (p.fDistance * g_Local.iFOV)) * (g_Screen.iWidth * g_Screen.iHeight) / 995328;
					int x = p.iESPScrPos[0];
					int y = p.iESPScrPos[1];
					int r = 255, g = 30, b = 255;
					p.iTeam == 1 ? b = 30 : r = 30;
					if (p.bVisible) g = 255;
					if (g_Settings.esp.box_out)
						g_Drawing.DrawShadowBox(x - (int)w, y - (int)h, (int)w * 2, (int)h * 2, r, g, b);
					else
						g_Drawing.DrawBox(x - (int)w, y - (int)h, (int)w * 2, (int)h * 2, 1, r, g, b, 255);
				}
			}

			if (g_Settings.esp.drawtrigarea && g_Settings.trigger.weapons[g_Local.iWpnID].fov > 0)
			{
				if (p.iTrigScrPosHEAD[0] > 0 &&
					p.iTrigScrPosHEAD[1] > 0 &&
					p.iTrigScrPosHEAD[0] < g_Screen.iWidth &&
					p.iTrigScrPosHEAD[1] < g_Screen.iHeight)
				{
					int x = p.iTrigScrPosHEAD[0], y = p.iTrigScrPosHEAD[1];
					int x1 = p.iTrigScrPosCHEST[0], y1 = p.iTrigScrPosCHEST[1];
					float tfov = ((POW(g_Settings.trigger.weapons[g_Local.iWpnID].fov) * 90) / (p.fDistance * g_Local.iFOV));
					if (g_Settings.trigger.weapons[g_Local.iWpnID].headonly)
						g_Drawing.DrawBox(x - tfov + 1, y - tfov + 1, tfov * 2 + 1, tfov * 2 + 1, 1, 30, 255, 30, 255); //HEAD
					else 
					{
						float tfovX = ((POW(g_Settings.trigger.weapons[g_Local.iWpnID].fov + 20) * 90) / (g_Player[i].fDistance * g_Local.iFOV));
						float tfovY = ((POW((g_Settings.trigger.weapons[g_Local.iWpnID].fov + 20) * 1.25) * 90) / (g_Player[i].fDistance * g_Local.iFOV));
						g_Drawing.DrawBox(x - tfov + 1, y - tfov + 1, tfov * 2 + 1, tfov * 2 + 1, 1, 255, 30, 255, 255); //HEAD						
						g_Drawing.DrawBox(x1 - tfovX + 1, y1 - tfovY + 1, tfovX * 2 + 1, tfovY * 2 + 1, 1, 255, 30, 255, 255); //CHEST
					}
				}
			}

			if (g_Settings.esp.drawfov && g_Settings.aim.weapons[g_Local.iWpnID].fov > 0)
			{
				if (g_Cheat.WorldToScreen(p.vAimOrigin, fTemp))
				{
					if (fTemp[0] > 0 && fTemp[1] > 0)
					{
						int x = (int)fTemp[0];
						int y = (int)fTemp[1];

						float tempFOV;
						if (p.fDistance <= g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance)
							tempFOV = g_Settings.aim.weapons[g_Local.iWpnID].norecoil_fov;
						else
							tempFOV = g_Settings.aim.weapons[g_Local.iWpnID].fov;

						float afov = ((POW(tempFOV) * (g_Settings.aim.fov_distbased * 10.0f)) / (p.fDistance * g_Local.iFOV));
						float ffov = tempFOV * (90.0f / g_Local.iFOV);

						if (!g_Settings.aim.fov_distbased)
							g_Drawing.DrawBox(x - ffov + 1, y - ffov + 1, ffov * 2 + 1, ffov * 2 + 1, 1, 255, 255, 255, 255);
						else
							g_Drawing.DrawBox(x - afov + 1, y - afov + 1, afov * 2 + 1, afov * 2 + 1, 1, 255, 255, 255, 255);
					}
				}
			}

			if (g_Settings.esp.drawaimspot)
			{
				if (p.iScrPos[0] > 0 &&
					p.iScrPos[1] > 0 &&
					p.iScrPos[0] < g_Screen.iWidth &&
					p.iScrPos[1] < g_Screen.iHeight)
				{
					int x = p.iScrPos[0];
					int y = p.iScrPos[1];
					int r = 255, g = 255, b = 255;
					p.iTeam == 1 ? b = 30 : r = 30;
					g_Drawing.FillRGB(x - 1, y - 1, 2, 2, r, g, b, 255);
				}
			}
		}

		if (!p.bIsValid)
		{
			p.fFov = 0.0f;
			p.fTriggerFov = 0.0f;
		}
	}

	if (g_Settings.misc.punch && g_AntiScreen.DrawVisuals && AIM_VERSION == 0 )
	{
		float recoil = ((g_Screen.iWidth * g_Screen.iHeight) * 20.0f) / 2073600.0f;
		float y = g_Local.vPunchAngle[0] * recoil;
		float x = g_Local.vPunchAngle[1] * recoil;
		g_Drawing.FillRGB(g_Screen.iWidth / 2 - x - 1, g_Screen.iHeight / 2 + y - 1, 3, 3, 255, 1, 1, 255);
	}

	if (g_Settings.misc.noflash && g_AntiScreen.DrawVisuals && AIM_VERSION == 0 )
	{
		if (NoFlash.pScreenFade->fadeEnd > g_pEngine->GetClientTime())
		{
			NoFlash.FadeEnd = NoFlash.pScreenFade->fadeEnd;
			if ((int)g_Settings.misc.noflash >= 255)
				NoFlash.pScreenFade->fadeEnd = 0.0f;
			else if (NoFlash.pScreenFade->fadealpha > (int)g_Settings.misc.noflash)
				NoFlash.pScreenFade->fadealpha = (int)g_Settings.misc.noflash;
		}
		if (NoFlash.FadeEnd > g_pEngine->GetClientTime())
			NoFlash.Flashed = (NoFlash.FadeEnd - g_pEngine->GetClientTime()) * NoFlash.pScreenFade->fadeSpeed;
		else
			NoFlash.Flashed = 0.0f;

		if (NoFlash.Flashed > 255.0f) NoFlash.Flashed = 255.0f;
		if (NoFlash.Flashed < 0.0f) NoFlash.Flashed = 0.0f;
		float Percentage = (NoFlash.Flashed / 255.0f) * 100.0f;
		if (Percentage > 0)
		{
			g_Drawing.FillRGB(0, 0, g_Screen.iWidth, g_Screen.iHeight * 0.15, NoFlash.pScreenFade->fader, NoFlash.pScreenFade->fadeg, NoFlash.pScreenFade->fadeb, NoFlash.Flashed);
			g_Drawing.FillRGB(0, g_Screen.iHeight * 0.85, g_Screen.iWidth, g_Screen.iHeight, NoFlash.pScreenFade->fader, NoFlash.pScreenFade->fadeg, NoFlash.pScreenFade->fadeb, NoFlash.Flashed);
		}
	}
	else if (g_Settings.misc.noflash && !g_AntiScreen.DrawVisuals && NoFlash.Flashed > 0)
		NoFlash.pScreenFade->fadealpha = NoFlash.Flashed;

	if (g_AntiScreen.DrawVisuals && AIM_VERSION == 0 )
	{
		if (g_Settings.trigger.keymode == 2)
		{
			if (g_Settings.trigger.enable && GetTickCount() - dwTriggerDrawStatusTime < 1000) 
			{
				int iWidth, iHeight;
				g_pEngine->pfnDrawConsoleStringLen("TRIGGERBOT: ENABLED", &iWidth, &iHeight);

				if (bTriggerWork)
					g_Drawing.DrawString(g_Screen.iWidth - iWidth - 10, 5, 205, 0, 0, "TRIGGERBOT: ENABLED");
				else
					g_Drawing.DrawString(g_Screen.iWidth - iWidth - 10, 5, 205, 0, 0, "TRIGGERBOT: DISABLED");
			}
		}
		else if (GetAsyncKeyState(g_Settings.trigger.key))
			g_Drawing.DrawString(g_Screen.iWidth - 170, 5, 205, 0, 0, "TRIGGERBOT: ENABLED");
	}

	if ( GetAsyncKeyState( VK_F6 ) & 1 )
	{
		bDrawMenu = !bDrawMenu;
	}

	if ( bDrawMenu && g_AntiScreen.DrawVisuals || ( bDrawMenu && AIM_VERSION ) )
		DrawMenu();

	if (g_Local.iWpnID == WEAPONLIST_KNIFE || g_Local.iWpnID == WEAPONLIST_C4 || g_Local.iWpnID == WEAPONLIST_FLASHBANG ||
		g_Local.iWpnID == WEAPONLIST_HEGRENADE || g_Local.iWpnID == WEAPONLIST_SMOKEGRENADE)
		g_Local.bBadWpn = true;
	else
		g_Local.bBadWpn = false;

	if (g_Local.iWpnID == WEAPONLIST_AWP || g_Local.iWpnID == WEAPONLIST_SCOUT || g_Local.iWpnID == WEAPONLIST_SG550 ||
		 g_Local.iWpnID == WEAPONLIST_G3SG1 )
		g_Local.bSnipeWpn = true;
	else
		g_Local.bSnipeWpn = false;

	if (g_Local.iWpnID == WEAPONLIST_DEAGLE || g_Local.iWpnID == WEAPONLIST_ELITE || g_Local.iWpnID == WEAPONLIST_FIVESEVEN ||
		g_Local.iWpnID == WEAPONLIST_GLOCK18 || g_Local.iWpnID == WEAPONLIST_USP || g_Local.iWpnID == WEAPONLIST_P228)
		g_Local.bPistolWpn = true;
	else
		g_Local.bPistolWpn = false;
}

void HUD_PlayerMove(struct playermove_s *ppmove, int server)
{
	g_pEngine->pEventAPI->EV_LocalPlayerViewheight(g_Local.vEye);
	g_Local.vEye = ppmove->origin + g_Local.vEye;
	g_Local.vOrigin = ppmove->origin;
	g_Local.iFlags = ppmove->flags;
	g_Local.iWaterLevel = ppmove->waterlevel;
	g_Local.bOnLadder = ppmove->movetype == 5;
	g_Local.iUseHull = ppmove->usehull;
	g_Local.ppmove = ppmove;

	return g_Client.HUD_PlayerMove(ppmove, server);
}

void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed)
{
	g_Client.HUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);

	gNoSpread.HUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);

	if (runfuncs)
	{
		g_Local.bInReload = (to->weapondata[g_Local.iWpnID].m_fInReload != 0);
		g_Local.iAmmo = to->weapondata->m_iClip;
		g_Local.m_flNextPrimaryAttack = to->weapondata[g_Local.iWpnID].m_flNextPrimaryAttack;
		g_Local.m_flNextSecondaryAttack = to->weapondata[g_Local.iWpnID].m_flNextSecondaryAttack;
		g_Local.iClip = to->weapondata[g_Local.iWpnID].m_iClip;
	}
}

int HUD_UpdateClientData(client_data_t *pcldata, float flTime)
{
	if (pcldata)
		WeaponListUpdate(pcldata->iWeaponBits);

	return g_Client.HUD_UpdateClientData(pcldata, flTime);
}

void V_CalcRefdef(struct ref_params_s *pparams)
{
	VectorCopy(pparams->punchangle, g_Local.vPunchAngle);
	g_Client.V_CalcRefdef(pparams);
}

void Shoot(usercmd_s *cmd) 
{
	if (g_Local.m_flNextPrimaryAttack >= 0.0f)
		cmd->buttons &= ~IN_ATTACK;
	else
		cmd->buttons |= IN_ATTACK;
}

void Shoot2(usercmd_s *cmd) 
{
	if (g_Local.m_flNextSecondaryAttack >= 0.0f)
		cmd->buttons &= ~IN_ATTACK2;
	else
		cmd->buttons |= IN_ATTACK2;
}

void CL_CreateMove(float frametime, struct usercmd_s * cmd, int active)
{
	g_Client.CL_CreateMove(frametime, cmd, active);

	BulletConrtol_CreateMove(cmd);

	bool snipernzoom = ( g_Local.bSnipeWpn && g_Local.iFOV == 90 );

	// fastzoom
	if (cmd->buttons & IN_ATTACK && snipernzoom && g_Settings.misc.fastzoom)
	{
		cmd->buttons &= ~IN_ATTACK;
		Shoot2(cmd);
		Shoot(cmd);
	}

	// get time
	DWORD t = GetTickCount();

	VectorCopy(cmd->viewangles, g_Local.cmdViewAngles);

	if (g_Local.bIsAlive)
		gNoSpread.CL_CreateMove(cmd);

	// check trigger key
	if (g_Settings.trigger.keymode == 2)
	{
		if (GetAsyncKeyState(g_Settings.trigger.key) & 1)
		{
			bTriggerWork = !bTriggerWork;
			dwTriggerDrawStatusTime = t;
		}
	}
	else
	{
		if (GetAsyncKeyState(g_Settings.trigger.key))
			bTriggerWork = true;
		else
			bTriggerWork = false;
	}

	// bhop
	if (g_Settings.misc.bhop)
		if (cmd->buttons & IN_JUMP && !((g_Local.iFlags & FL_ONGROUND) || g_Local.ppmove->velocity[2] == 0.0f))
			cmd->buttons &= ~IN_JUMP;

	g_Local.bInAttack1 = cmd->buttons & IN_ATTACK && !g_Local.bInReload;
	g_Local.bInAttack2 = cmd->buttons & IN_ATTACK2;

	static bool m_bAimPanic = true;
	if (GetAsyncKeyState(VK_F7) & 1)
		m_bAimPanic = !m_bAimPanic;

	bTriggerWorkInThisFrame = false;

	float recoil = ((g_Screen.iWidth * g_Screen.iHeight) * 20.0f) / 2073600.0f;
	float y = g_Local.vPunchAngle[0] * recoil;
	float x = g_Local.vPunchAngle[1] * recoil;
	float minFOV = 999999.0f;
	int pID = -1;

	for (int i = 0; i < 33; ++i)
	{
		if (!g_Player[i].bIsValid) continue;
		if (!g_Player[i].iTeam) continue;

		if (g_Settings.kb.enable && g_Player[i].iTeam != g_Local.iTeam && g_Player[i].fFov <= 120)
			if (g_Player[i].fDistance <= g_Settings.kb.distance && g_Local.iWpnID == WEAPONLIST_KNIFE)
				Shoot2(cmd);

		if ( g_Settings.trigger.enable )
			if ( bTriggerWork &&
				 g_Player[i].iTrigScrPosHEAD[0] > 0 &&
				 g_Player[i].iTrigScrPosHEAD[1] > 0 &&
				 g_Player[i].iTrigScrPosHEAD[0] < g_Screen.iWidth &&
				 g_Player[i].iTrigScrPosHEAD[1] < g_Screen.iHeight &&
				 g_Player[i].fDistance >= g_Settings.trigger.dstmin &&
				 g_Player[i].fDistance <= g_Settings.trigger.dstmax &&
				 ( g_Player[i].bVisible || !g_Settings.trigger.pwalls ) &&
				 ( g_Player[i].iTeam != g_Local.iTeam || g_Settings.trigger.dm ) )
			{
				float tfov = ((POW(g_Settings.trigger.weapons[g_Local.iWpnID].fov) * 90.0f) / (g_Player[i].fDistance * g_Local.iFOV));

				if (g_Settings.trigger.weapons[g_Local.iWpnID].headonly)
				{
					if (CheckFOV(g_Local.vDrawSpr, g_Player[i].iTrigScrPosHEAD, tfov))
					{
						if ( !g_Local.bSnipeWpn )
						{
							Shoot( cmd );
						}
						else if ( g_Local.bSnipeWpn && g_Local.iFOV != 90 && g_Settings.trigger.onlyzoom )
						{
							Shoot( cmd );
						}
						else if ( g_Local.bSnipeWpn && !g_Settings.trigger.onlyzoom )
						{
							Shoot( cmd );
						}

						bTriggerWorkInThisFrame = true;
					}
				}
				else
				{
					float tfovX = ((POW(g_Settings.trigger.weapons[g_Local.iWpnID].fov + 20.0f) * 90.0f) / (g_Player[i].fDistance * g_Local.iFOV));
					float tfovY = ((POW((g_Settings.trigger.weapons[g_Local.iWpnID].fov + 20.0f) * 1.25f) * 90.0f) / (g_Player[i].fDistance * g_Local.iFOV));

					if (CheckFOV(g_Local.vDrawSpr, g_Player[i].iTrigScrPosCHEST, tfovX, tfovY) || CheckFOV(g_Local.vDrawSpr, g_Player[i].iTrigScrPosHEAD, tfov))
					{
						if ( !g_Local.bSnipeWpn )
						{
							Shoot( cmd );
						}
						else if ( g_Local.bSnipeWpn && g_Local.iFOV != 90 && g_Settings.trigger.onlyzoom )
						{
							Shoot( cmd );
						}
						else if ( g_Local.bSnipeWpn && !g_Settings.trigger.onlyzoom )
						{
							Shoot( cmd );
						}

						bTriggerWorkInThisFrame = true;
					}
				}
			}

		if (g_Settings.aim.enable)
		{
			Vector dst((float)g_Screen.iWidth / 2, (float)g_Screen.iHeight / 2, 0.0f);

			float tempFOV;

			if (g_Player[i].fDistance < g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance)
				tempFOV = g_Settings.aim.weapons[g_Local.iWpnID].norecoil_fov;
			else
				tempFOV = g_Settings.aim.weapons[g_Local.iWpnID].fov;

			float FOV = tempFOV * (90.0f / g_Local.iFOV);

			if (g_Settings.aim.fov_distbased > 0.0f)
				FOV = ((POW(tempFOV) * (g_Settings.aim.fov_distbased * 10.0f)) / (g_Player[i].fDistance * g_Local.iFOV));

			float playerFOV = g_Player[i].fFov;

			if (minFOV >= playerFOV && CheckFOV(dst, g_Player[i].iScrPos, FOV))
			{
				minFOV = playerFOV;
				pID = i;
			}
		}
	}

	if (g_Settings.aim.enable && m_bAimPanic && pID != -1)
	{
		if (g_Settings.aim.mode == 2)
			Aimbot(pID, cmd);
		else
			Aimbot2(pID, cmd);
	}
}

int HUD_Key_Event( int down , int keynum , const char *pszCurrentBinding )
{
	int ret = g_Client.HUD_Key_Event( down , keynum , pszCurrentBinding );

	if ( bDrawMenu )
	{
		if ( keynum == K_ENTER || keynum == K_LEFTARROW || keynum == K_RIGHTARROW ||
			 keynum == K_UPARROW || keynum == K_DOWNARROW )
			return 0;
	}

	return ret;
}

void HookFunction()
{
	g_pClient->HUD_Frame = HUD_Frame;
	g_pClient->HUD_Redraw = HUD_Redraw;
	g_pClient->CL_CreateMove = CL_CreateMove;
	g_pClient->HUD_PlayerMove = HUD_PlayerMove;
	g_pClient->V_CalcRefdef = V_CalcRefdef;
	g_pClient->HUD_PostRunCmd = HUD_PostRunCmd;
	g_pClient->HUD_UpdateClientData = HUD_UpdateClientData;
	g_pClient->HUD_Key_Event = HUD_Key_Event;
}
