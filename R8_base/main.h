#ifndef __MAIN_H__
#define __MAIN_H__

#pragma once

#include <windows.h>
#include <vector>
#include <deque>

using namespace std;

//#include <gl/GL.h>
//#include <gl/GLU.h>
//#include <d3d9.h>
//#include <d3dx9.h>
#include <dwmapi.h>
#include <gl/GL.h>

//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "opengl32")

#include "detours.h"
#include "cFont.h"

#include "ValveSDK/engine/wrect.h"
#include "ValveSDK/engine/cl_dll.h"
#include "ValveSDK/engine/r_studioint.h"
#include "ValveSDK/engine/cl_entity.h"
#include "ValveSDK/misc/com_model.h"
#include "ValveSDK/engine/triangleapi.h"
#include "ValveSDK/engine/pmtrace.h"
#include "ValveSDK/engine/pm_defs.h"
#include "ValveSDK/engine/pm_info.h"
#include "ValveSDK/common/ref_params.h"
#include "ValveSDK/common/studio_event.h"
#include "ValveSDK/common/net_api.h"
#include "ValveSDK/common/r_efx.h"
#include "ValveSDK/engine/cvardef.h"
#include "ValveSDK/engine/util_vector.h"
#include "ValveSDK/misc/parsemsg.h"
#include "ValveSDK/engine/studio.h"
#include "ValveSDK/engine/event_args.h"
#include "ValveSDK/engine/event_flags.h"
#include "ValveSDK/common/event_api.h"
#include "ValveSDK/common/screenfade.h"
#include "ValveSDK/engine/keydefs.h"
#include "ValveSDK/common/engine_launcher_api.h"

typedef struct cl_clientfuncs_s
{
	int(*Initialize) (cl_enginefunc_t *pEnginefuncs, int iVersion);
	int(*HUD_Init) (void);
	int(*HUD_VidInit) (void);
	void(*HUD_Redraw) (float time, int intermission);
	int(*HUD_UpdateClientData) (client_data_t *pcldata, float flTime);
	int(*HUD_Reset) (void);
	void(*HUD_PlayerMove) (struct playermove_s *ppmove, int server);
	void(*HUD_PlayerMoveInit) (struct playermove_s *ppmove);
	char(*HUD_PlayerMoveTexture) (char *name);
	void(*IN_ActivateMouse) (void);
	void(*IN_DeactivateMouse) (void);
	void(*IN_MouseEvent) (int mstate);
	void(*IN_ClearStates) (void);
	void(*IN_Accumulate) (void);
	void(*CL_CreateMove) (float frametime, struct usercmd_s *cmd, int active);
	int(*CL_IsThirdPerson) (void);
	void(*CL_CameraOffset) (float *ofs);
	struct kbutton_s *(*KB_Find) (const char *name);
	void(*CAM_Think) (void);
	void(*V_CalcRefdef) (struct ref_params_s *pparams);
	int(*HUD_AddEntity) (int type, struct cl_entity_s *ent, const char *modelname);
	void(*HUD_CreateEntities) (void);
	void(*HUD_DrawNormalTriangles) (void);
	void(*HUD_DrawTransparentTriangles) (void);
	void(*HUD_StudioEvent) (const struct mstudioevent_s *event, const struct cl_entity_s *entity);
	void(*HUD_PostRunCmd) (struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
	void(*HUD_Shutdown) (void);
	void(*HUD_TxferLocalOverrides) (struct entity_state_s *state, const struct clientdata_s *client);
	void(*HUD_ProcessPlayerState) (struct entity_state_s *dst, const struct entity_state_s *src);
	void(*HUD_TxferPredictionData) (struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd);
	void(*Demo_ReadBuffer) (int size, unsigned char *buffer);
	int(*HUD_ConnectionlessPacket) (struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
	int(*HUD_GetHullBounds) (int hullnumber, float *mins, float *maxs);
	void(*HUD_Frame) (double time);
	int(*HUD_Key_Event) (int down, int keynum, const char *pszCurrentBinding);
	void(*HUD_TempEntUpdate) (double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int(*Callback_AddVisibleEntity)(struct cl_entity_s *pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s *pTemp, float damp));
	struct cl_entity_s *(*HUD_GetUserEntity) (int index);
	int(*HUD_VoiceStatus) (int entindex, qboolean bTalking);
	int(*HUD_DirectorMessage) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int(*HUD_GetStudioModelInterface) (int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio);
	void(*HUD_CHATINPUTPOSITION_FUNCTION) (int *x, int *y);
	int(*HUD_GETPLAYERTEAM_FUNCTION) (int iplayer);
	void(*CLIENTFACTORY) (void);
} cl_clientfunc_t; 

#define M_PI 3.14159265358979323846
#define POW(x) ((x)*(x))
#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}
#define VectorLengthSquared(v) ((v)[0]*(v)[0]+(v)[1]*(v)[1]+(v)[2]*(v)[2])
#define VectorDistance(a,b) sqrt(POW((a)[0]-(b)[0])+POW((a)[1]-(b)[1])+POW((a)[2]-(b)[2]))
#define VectorLength(a) sqrt(POW((a)[0])+POW((a)[1])+POW((a)[2]))
#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#define VectorMul(vec,num,res){(res)[0]=(vec)[0]*(num);(res)[1]=(vec)[1]*(num);(res)[2]=(vec)[2]*(num);}
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}

#define WEAPONLIST_P228			1
#define	WEAPONLIST_UNKNOWN1		2
#define	WEAPONLIST_SCOUT		3
#define	WEAPONLIST_HEGRENADE	4
#define	WEAPONLIST_XM1014		5
#define	WEAPONLIST_C4			6
#define	WEAPONLIST_MAC10		7
#define	WEAPONLIST_AUG			8
#define	WEAPONLIST_SMOKEGRENADE	9
#define	WEAPONLIST_ELITE		10
#define	WEAPONLIST_FIVESEVEN	11
#define	WEAPONLIST_UMP45		12
#define	WEAPONLIST_SG550		13
#define	WEAPONLIST_GALIL		14
#define	WEAPONLIST_FAMAS		15
#define	WEAPONLIST_USP			16
#define	WEAPONLIST_GLOCK18		17
#define	WEAPONLIST_AWP			18
#define	WEAPONLIST_MP5			19
#define	WEAPONLIST_M249			20
#define	WEAPONLIST_M3			21
#define	WEAPONLIST_M4A1			22
#define	WEAPONLIST_TMP			23
#define	WEAPONLIST_G3SG1		24
#define	WEAPONLIST_FLASHBANG	25
#define	WEAPONLIST_DEAGLE		26
#define	WEAPONLIST_SG552		27
#define	WEAPONLIST_AK47			28 
#define	WEAPONLIST_KNIFE		29
#define	WEAPONLIST_P90			30
#define WEAPONLIST_ALL			31

#define BULLET_NONE BULLETTYPE_0
#define BULLET_AK47 BULLETTYPE_11
#define BULLET_AUG BULLETTYPE_12
#define BULLET_AWP BULLETTYPE_10
#define BULLET_DEAGLE BULLETTYPE_13
#define BULLET_ELITE BULLETTYPE_1
#define BULLET_FIVESEVEN BULLETTYPE_15
#define BULLET_G3SG1 BULLETTYPE_11
#define BULLET_GLOCK18 BULLETTYPE_1
#define BULLET_M249 BULLETTYPE_12
#define BULLET_M4A1 BULLETTYPE_12
#define BULLET_MAC10 BULLETTYPE_9
#define BULLET_MP5 BULLETTYPE_1
#define BULLET_P228 BULLETTYPE_15
#define BULLET_P90 BULLETTYPE_14
#define BULLET_SCOUT BULLETTYPE_11
#define BULLET_SG550 BULLETTYPE_12
#define BULLET_SG552 BULLETTYPE_12
#define BULLET_TMP BULLETTYPE_1
#define BULLET_UMP45 BULLETTYPE_9
#define BULLET_USP BULLETTYPE_9
#define BULLET_M3 BULLETTYPE_0
#define BULLET_XM1014 BULLETTYPE_0
#define BULLET_GALIL BULLETTYPE_12
#define BULLET_FAMAS BULLETTYPE_12

#define WALL_PEN0 0
#define WALL_PEN1 1
#define WALL_PEN2 2

#define USP_SILENCER (1 << 0)
#define GLOCK18_BURST (1 << 1)
#define M4A1_SILENCER (1 << 2)
#define ELITE_LEFT (1 << 4)

#define MAX_WEAPON_NAME 32

#define PUBLIC_VERSION	0
#define VOVAN_VERSION	0
#define AIM_VERSION		0

bool IsLeftElite(void);
bool IsCurWeaponSilenced(void);

enum
{
	BULLETTYPE_0 = 0,
	BULLETTYPE_1 = 1,
	BULLETTYPE_2 = 2,
	BULLETTYPE_3 = 3,
	BULLETTYPE_4 = 4,
	BULLETTYPE_5 = 5,
	BULLETTYPE_6 = 6,
	BULLETTYPE_7 = 7,
	BULLETTYPE_8 = 8,
	BULLETTYPE_9 = 9,
	BULLETTYPE_10 = 10,
	BULLETTYPE_11 = 11,
	BULLETTYPE_12 = 12,
	BULLETTYPE_13 = 13,
	BULLETTYPE_14 = 14,
	BULLETTYPE_15 = 15
};

const int N = 32;

typedef struct TUserMsg
{
	int number;
	int size;
	char name[16];
	struct TUserMsg* next;
	pfnUserMsgHook pfn;
} *PUserMsg;

struct spread_info
{
	unsigned int random_seed;

	float gtime;
	float prevtime;
	float brokentime;
	float spreadvar;
	float recoiltime;

	bool firing;

	int WeaponState;
	int prcflags;
	int recoil;
};

struct Weapon_List
{
	char name[MAX_WEAPON_NAME];
	int len;
	char command[MAX_WEAPON_NAME];
	int AmmoType;
	int Ammo2Type;
	int Max1;
	int Max2;
	int Slot;
	int SlotPos;
	int Id;
	int Flags;
	int ClipCap;	// Number of bullets the clip can hold
	int CAmmo;		// Current ammo in clip
	int XAmmo;		// Extra ammo
	bool Active;	// True if we have this weapon
	bool CurWeapon; // True if this is the current weapon
	int penetrate;
	weapon_data_t weapondata;
	float distance;
	int bullettype;
	float wallpierce1;
	float wallpierce2;
	int damage1;
	int damage2;
};
extern vector<Weapon_List> WeaponListStruct;

struct local_s 
{
	char *tmp_wpn;
	playermove_s* ppmove;
	float fPing, fHeight, fGroundAngle, m_flNextPrimaryAttack, m_flNextSecondaryAttack;
	string sWpnType;
	int iIndex, iTeam, iWpnID, iClip, iWpnType, iFOV, iFlags, iWaterLevel, iUseHull, iAmmo, iHealth, iShotsFired;
	cl_entity_s *pEnt;
	Vector vOrigin, vEye, vPunchAngle, vNoSpreadAng, vDrawSpr, cmdViewAngles;
	bool bIsAlive, bBadWpn, bSnipeWpn, bPistolWpn, bInReload, bOnLadder, bInAttack1, bInAttack2;
	bool bIsNoShot;
	Weapon_List *weapon;
	spread_info spread;
};
extern local_s g_Local;

typedef void(__stdcall *glReadPixels_t)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid*);
extern glReadPixels_t glReadPixels_s;
void __stdcall m_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);

struct antiscreen_s {
	bool DrawVisuals, MyCall;
	BOOL ScreenFirst = TRUE;
	PBYTE BufferScreen;
	int temp;
	DWORD dwSize, time_scr;
};
extern antiscreen_s g_AntiScreen;

///////////////// LOGS //////////////////
#include <sstream>
#include <fstream>
extern ofstream ofile;
inline void __cdecl add_log(const char *fmt, ...) {
	if (ofile) {
		if (!fmt)
			return;
		va_list va_alist;
		char logbuf[256] = { 0 };
		va_start(va_alist, fmt);
		_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
		va_end(va_alist);
		ofile << logbuf << endl;
	}
}
///////////////// LOGS //////////////////

/* SVC START */

typedef unsigned __int16 uint16;

typedef void( *pfnEngineMessage )( );

typedef struct sizebuf_s
{
	const char *buffername;
	uint16 flags;
	byte *data;
	int maxsize;
	int cursize;
} sizebuf_t;

typedef struct EngineMsg
{
	int number;
	char* name;
	pfnEngineMessage pfn;
} *PEngineMsg;

#define BIT(n) (1<<(n))

#define DEFAULT_SOUND_PACKET_VOLUME			255

// Sound flags
#define SND_FL_VOLUME			BIT(0)	// send volume
#define SND_FL_ATTENUATION		BIT(1)	// send attenuation
#define SND_FL_LARGE_INDEX		BIT(2)	// send sound number as short instead of byte
#define SND_FL_PITCH			BIT(3)	// send pitch

extern int* MSG_ReadCount;
extern int MSG_SavedReadCount;
extern sizebuf_t* MSG_Buffer;

typedef byte( *HL_MSG_ReadByte )( );
typedef void( *HL_MSG_ReadBitVec3Coord )( float* fa );
typedef int( *HL_MSG_ReadBits )( int Count );
typedef void( *HL_MSG_StartBitReading )( sizebuf_t* buffer );
typedef void( *HL_MSG_EndBitReading )( sizebuf_t* buffer );

extern HL_MSG_ReadByte MSG_ReadByte;
extern HL_MSG_ReadBitVec3Coord MSG_ReadBitVec3Coord;
extern HL_MSG_ReadBits MSG_ReadBits;
extern HL_MSG_StartBitReading MSG_StartBitReading;
extern HL_MSG_EndBitReading MSG_EndBitReading;

extern pfnEngineMessage pSVC_sound;
extern PEngineMsg pEngineMsgBase;

/* SVC END */

#include "CvarUtils.h"
#include "Offset.h"
#include "Client.h"
#include "UserMsgs.h"
#include "Utils.h"
#include "weaponslist.h"

#endif