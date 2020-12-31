#include "main.h"
#include "nospread.h"

pfnUserMsgHook pResetHUD;
pfnUserMsgHook pSetFOV;
pfnUserMsgHook pTeamInfo;
pfnUserMsgHook pCurWeapon;
pfnUserMsgHook pDeathMsg;
pfnUserMsgHook pWeaponList;
pfnUserMsgHook pAmmoX;

int ResetHUD(const char *pszName, int iSize, void *pbuf)
{
	//g_Engine.Con_Printf("ResetHUD\n");
	d_vSoundOrigin.clear();
	d_dwSoundTime.clear();
	return pResetHUD(pszName, iSize, pbuf);
}

int SetFOV(const char *pszName, int iSize, void *pbuf)
{
	//g_Engine.Con_Printf("SetFOV\n");
	BEGIN_READ(pbuf, iSize);
	g_Local.iFOV = READ_BYTE();
	if (!g_Local.iFOV)
		g_Local.iFOV = 90;
	return pSetFOV(pszName, iSize, pbuf);
}

int TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int iIndex = READ_BYTE();
	char *szTeam = READ_STRING();
	_strlwr(szTeam);
	if (!strcmp(szTeam, "terrorist"))
	{
		g_Player[iIndex].iTeam = 1;
		if (iIndex == g_Local.iIndex) { g_Local.iTeam = 1; }
	}
	else if (!strcmp(szTeam, "ct"))
	{
		g_Player[iIndex].iTeam = 2;
		if (iIndex == g_Local.iIndex) { g_Local.iTeam = 2; }
	}
	else
	{
		g_Player[iIndex].iTeam = 0;
		if (iIndex == g_Local.iIndex) { g_Local.iTeam = 0; }
	}
	return pTeamInfo(pszName, iSize, pbuf);
}

int WeaponList(const char *pszName, int iSize, void *pbuf)
{
	char *weaponname;
	int ammo1type, max1, ammo2type, max2, slot, slotpos, id, flags;

	BEGIN_READ(pbuf, iSize);

	weaponname = READ_STRING();

	ammo1type = READ_CHAR();
	max1 = READ_BYTE();

	ammo2type = READ_CHAR();
	max2 = READ_BYTE();

	slot = READ_CHAR();
	slotpos = READ_CHAR();

	id = READ_CHAR();
	flags = READ_BYTE();

	WeaponListAdd(weaponname, ammo1type, max1, ammo2type, max2, slot, slotpos, id, flags);

	return (*pWeaponList)(pszName, iSize, pbuf);
}

int AmmoX(const char *pszName, int iSize, void *pbuf)
{
	int ID, Count;

	BEGIN_READ(pbuf, iSize);

	ID = READ_BYTE();
	Count = READ_BYTE();

	WeaponListAmmoX(ID, Count);

	return (*pAmmoX)(pszName, iSize, pbuf);
}

int CurWeapon(const char *pszName, int iSize, void *pbuf)
{
	/*BEGIN_READ(pbuf, iSize);
	int iState = READ_BYTE();
	int iWeaponID = READ_CHAR();
	int iClip = READ_CHAR();
	if (iState)
	{
		g_Local.iClip = iClip;
		g_Local.iWpnID = iWeaponID;
	}*/
	BEGIN_READ(pbuf, iSize);

	int iState = READ_BYTE();
	int iWeaponID = READ_CHAR();
	int iClip = READ_CHAR();
	static int iLastWeaponID = 0;

	if (iState)
	{
		g_Local.iClip = iClip;
		g_Local.iWpnID = iWeaponID;
	}

	WeaponListCurWeapon(iState, iWeaponID, iClip);

	//SHIT
	if (iWeaponID != iLastWeaponID)
	{
		iLastWeaponID = iWeaponID;
		g_Local.spread.recoil = 0;
		g_Local.spread.prevtime = 0;
		g_Local.spread.firing = false;
		gNoSpread.DefaultSpreadVar(iWeaponID);
	}


	return pCurWeapon(pszName, iSize, pbuf);
}

int DeathMsg(const char *pszName, int iSize, void *pbuf)
{
	//g_Engine.Con_Printf("DeathMsg\n");
	return pDeathMsg(pszName, iSize, pbuf);
}