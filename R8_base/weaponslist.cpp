#include <string.h>
#include "main.h"
#include "weaponslist.h"

#pragma warning(disable:4786)
#pragma warning(disable:4018)
#pragma warning(disable:4800)
#pragma warning(disable:4244)
#pragma warning(disable:4101)
#pragma warning(disable:4715)
#pragma warning(disable:4305)

vector<Weapon_List> WeaponListStruct;

bool IsCurWeaponSilenced(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
	{
		if (tmpWeapon->Id == WEAPONLIST_M4A1 && tmpWeapon->weapondata.m_iWeaponState & M4A1_SILENCER)
			return true;

		if (tmpWeapon->Id == WEAPONLIST_USP && tmpWeapon->weapondata.m_iWeaponState & USP_SILENCER)
			return true;
	}

	return false;
}


bool IsCurWeaponInBurst(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
	{
		if (tmpWeapon->Id == WEAPONLIST_GLOCK18 && tmpWeapon->weapondata.m_iWeaponState & GLOCK18_BURST)
			return true;
	}

	return false;
}


int GetCurPenetration(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
		return tmpWeapon->penetrate;

	return WALL_PEN0;
}


Weapon_List *GetCurWeapon(void)
{
	int i;

	for (i=0;i<WeaponListStruct.size();i++)
	{
		if (WeaponListStruct[i].Active && WeaponListStruct[i].CurWeapon)
			return &WeaponListStruct[i];
	}

	return NULL;
}


int GetCurWeaponAmmo(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
		return tmpWeapon->CAmmo;

	return 0;
}


int GetCurRecoil(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
		return tmpWeapon->weapondata.m_fInZoom;

	return 0;
}

bool IsLeftElite(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
	{
		if (tmpWeapon->Id == WEAPONLIST_M4A1 && tmpWeapon->weapondata.m_iWeaponState & ELITE_LEFT)
			return true;
	}

	return false;
}

bool IsReloading(void)
{
	Weapon_List *tmpweapon;

	tmpweapon = GetCurWeapon();
	if (tmpweapon && tmpweapon->weapondata.m_fInReload)
		return true;

	return false;
}


bool CanCurWeaponAttack(void)
{
	Weapon_List *tmpweapon;

	tmpweapon = GetCurWeapon();
	if (tmpweapon && tmpweapon->weapondata.m_flNextPrimaryAttack <= 0)
		return true;

	return false;
}


static Weapon_List *GetSecWeapon(void)
{
	int i;

	for (i=0;i<WeaponListStruct.size();i++)
	{
		if (WeaponListStruct[i].Active && WeaponListStruct[i].Slot == 1)
			return &WeaponListStruct[i];
	}

	return NULL;
}


static Weapon_List *GetPriWeapon(void)
{
	int i;

	for (i=0;i<WeaponListStruct.size();i++)
	{
		if (WeaponListStruct[i].Active && WeaponListStruct[i].Slot == 0)
			return &WeaponListStruct[i];
	}

	return NULL;
}


int GetWeaponIndexByID(int WeaponID)
{
	int i;

	for (i=0;i<WeaponListStruct.size();i++)
	{
		if (WeaponListStruct[i].Id == WeaponID)
			return i;
	}

	return -1;
}


int GetCurWeaponId(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
		return tmpWeapon->Id;

	return -1;
}


int GetPriWeaponId(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetPriWeapon();
	if (tmpWeapon)
		return tmpWeapon->Id;

	return -1;
}


int GetSecWeaponId(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetSecWeapon();
	if (tmpWeapon)
		return tmpWeapon->Id;

	return -1;
}


const char *GetPriWeaponCmd(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetPriWeapon();
	if (tmpWeapon)
		return tmpWeapon->command;

	return NULL;
}


const char *GetSecWeaponCmd(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetSecWeapon();
	if (tmpWeapon)
		return tmpWeapon->command;

	return NULL;
}


const char *GetPriWeaponName(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetPriWeapon();
	if (tmpWeapon)
		return tmpWeapon->name;

	return NULL;
}


const char *GetSecWeaponName(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetSecWeapon();
	if (tmpWeapon)
		return tmpWeapon->name;

	return NULL;
}


bool HasPriWeapon(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetPriWeapon();
	if (tmpWeapon)
		return true;

	return false;
}


bool HasSecWeapon(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetSecWeapon();
	if (tmpWeapon)
		return true;

	return false;
}


int GetPrimaryAmmoX(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetPriWeapon();
	if (tmpWeapon)
		return tmpWeapon->XAmmo;

	return -1;
}


int GetSecondaryAmmoX(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetSecWeapon();
	if (tmpWeapon)
		return tmpWeapon->XAmmo;

	return -1;
}


int GetPrimaryAmmo(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetPriWeapon();
	if (tmpWeapon)
		return tmpWeapon->CAmmo;

	return -1;
}


int GetSecondaryAmmo(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetSecWeapon();
	if (tmpWeapon)
		return tmpWeapon->CAmmo;

	return -1;
}


bool IsCurWeaponPri(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetPriWeapon();
	if (tmpWeapon && tmpWeapon->CurWeapon)
		return true;

	return false;
}


bool IsCurWeaponSec(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetSecWeapon();
	if (tmpWeapon && tmpWeapon->CurWeapon)
		return true;

	return false;
}


bool IsCurWeaponGun(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon && (tmpWeapon->Slot == 0 || tmpWeapon->Slot == 1))
		return true;

	return false;
}


bool CurWeaponHasAmmo(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon && tmpWeapon->CAmmo > 0)
		return true;

	return false;
}


bool IsCurWeaponKnife(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon && tmpWeapon->Id == WEAPONLIST_KNIFE)
		return true;

	return false;
}


bool IsCurWeaponNonAttack(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon && (tmpWeapon->Id == WEAPONLIST_HEGRENADE || tmpWeapon->Id == WEAPONLIST_FLASHBANG || tmpWeapon->Id == WEAPONLIST_C4 || tmpWeapon->Id == WEAPONLIST_SMOKEGRENADE))
		return true;

	return false;
}


bool IsCurWeaponNade(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon && (tmpWeapon->Id == WEAPONLIST_HEGRENADE || tmpWeapon->Id == WEAPONLIST_FLASHBANG || tmpWeapon->Id == WEAPONLIST_SMOKEGRENADE))
		return true;

	return false;
}


bool IsCurWeaponC4(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon && tmpWeapon->Id == WEAPONLIST_C4)
		return true;

	return false;
}

int CurBulletType(void)
{
	Weapon_List *tmpWeapon = GetCurWeapon();

	if (tmpWeapon)
		return tmpWeapon->bullettype;

	return BULLET_NONE;	
}


float CurWallPierce(void)
{
	Weapon_List *tmpWeapon = GetCurWeapon();

	if (tmpWeapon)	
	{
		if (IsCurWeaponSilenced())
			return tmpWeapon->wallpierce2;
		else
			return tmpWeapon->wallpierce1;

		return tmpWeapon->wallpierce1;
	}

	return 0.0f;
}


int CurDamage(void)
{
	Weapon_List *tmpWeapon = GetCurWeapon();

	if (tmpWeapon)
	{
		if (IsCurWeaponSilenced())
			return tmpWeapon->damage2;
		else
			return tmpWeapon->damage1;

		if (tmpWeapon->Id == WEAPONLIST_ELITE)
		{
			if (g_Local.spread.WeaponState & ELITE_LEFT)
				return tmpWeapon->damage1;
			else
				return tmpWeapon->damage2;
		}

		return tmpWeapon->damage1;
	}

	return 0;
}

float CurDistance(void)
{
	Weapon_List *tmpWeapon = GetCurWeapon();

	if (tmpWeapon)
		return tmpWeapon->distance;

	return 0.0f;
}

const char *GetCurWeaponName(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
		return tmpWeapon->name;

	return NULL;
}


int GetCurWeaponID(void)
{
	Weapon_List *tmpWeapon;

	tmpWeapon = GetCurWeapon();
	if (tmpWeapon)
		return tmpWeapon->Id;

	return 0;
}


bool HasWeaponName(const char *weaponname)
{
	int i, len;

	len = strlen(weaponname);

	for (i=0;i<WeaponListStruct.size();i++)
	{
		if (WeaponListStruct[i].Active && len == WeaponListStruct[i].len && !memcmp(WeaponListStruct[i].name, weaponname, len))
			return true;
	}

	return false;
}


void WeaponListUpdate(int WeaponBits)
{
	int i;
	static int OldWeaponBits = 0;

	if (WeaponBits != OldWeaponBits)
	{
		OldWeaponBits = WeaponBits;

		for (i = 0; i<WeaponListStruct.size(); i++)
		{
			if (WeaponBits & (1 << WeaponListStruct[i].Id))
			{
				WeaponListStruct[i].Active = true;
			}
			else
			{
				WeaponListStruct[i].Active = false;
				WeaponListStruct[i].CAmmo = 0;
			}
		}
	}
}


void WeaponListCurWeapon(int CurWeapon, int WeaponID, int Ammo)
{
	int index;

	index = GetWeaponIndexByID(WeaponID);
	if (index != -1)
		WeaponListStruct[index].CAmmo = Ammo;
}


void WeaponListAmmoX(int ID, int Count)
{
	int i;

	for (i = 0; i<WeaponListStruct.size(); i++)
	{
		if (WeaponListStruct[i].AmmoType == ID)
			WeaponListStruct[i].XAmmo = Count;
	}
}


void SetWeaponData(struct Weapon_List *weapon)
{
	switch (weapon->Id)
	{
		case WEAPONLIST_P228:
			weapon->ClipCap = CLIPCAP_P228;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 4096.0f;
			weapon->wallpierce1 = 0.8f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 32;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_P228;
			break;
		case WEAPONLIST_SCOUT:
			weapon->ClipCap = CLIPCAP_SCOUT;
			weapon->penetrate = WALL_PEN2;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.98f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 75;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_SCOUT;
			break;
		case WEAPONLIST_HEGRENADE:
			weapon->ClipCap = 0;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 0.0f;
			weapon->wallpierce1 = 0.0f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 0;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_NONE;
			break;
		case WEAPONLIST_XM1014:
			weapon->ClipCap = CLIPCAP_XM1014;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 0.0f;
			weapon->wallpierce1 = 0.0f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 0;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_XM1014;
			break;
		case WEAPONLIST_C4:
			weapon->ClipCap = 0;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 0.0f;
			weapon->wallpierce1 = 0.0f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 0;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_NONE;
			break;
		case WEAPONLIST_MAC10:
			weapon->ClipCap = CLIPCAP_MAC10;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.82f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 29;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_MAC10;
			break;
		case WEAPONLIST_AUG:
			weapon->ClipCap = CLIPCAP_AUG;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.96f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 32;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_AUG;
			break;
		case WEAPONLIST_SMOKEGRENADE:
			weapon->ClipCap = 0;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 0.0f;
			weapon->wallpierce1 = 0.0f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 0;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_NONE;
			break;
		case WEAPONLIST_ELITE:
			weapon->ClipCap = CLIPCAP_ELITE;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.75f; // left
			weapon->wallpierce2 = 0.75f; // right
			weapon->damage1 = 20; // left
			weapon->damage2 = 27; // right
			weapon->bullettype = BULLET_ELITE;
			break;
		case WEAPONLIST_FIVESEVEN:
			weapon->ClipCap = CLIPCAP_FIVESEVEN;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 4096.0f;
			weapon->wallpierce1 = 0.885f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 14;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_FIVESEVEN;
			break;
		case WEAPONLIST_UMP45:
			weapon->ClipCap = CLIPCAP_UMP45;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.82f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 30;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_UMP45;
			break;
		case WEAPONLIST_SG550:
			weapon->ClipCap = CLIPCAP_SG550;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.98f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 40;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_SG550;
			break;
		case WEAPONLIST_GALIL:
			weapon->ClipCap = CLIPCAP_GALIL;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.96f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 33;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_GALIL;
			break;
		case WEAPONLIST_FAMAS:
			weapon->ClipCap = CLIPCAP_FAMAS;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.96f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 33;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_FAMAS;
			break;
		case WEAPONLIST_USP:
			weapon->ClipCap = CLIPCAP_USP;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 4096.0f;
			weapon->wallpierce1 = 0.79f; // unsilenced
			weapon->wallpierce2 = 0.79f; // silenced
			weapon->damage1 = 34; // unsilenced
			weapon->damage2 = 30; // silenced
			weapon->bullettype = BULLET_USP;
			break;
		case WEAPONLIST_GLOCK18:
			weapon->ClipCap = CLIPCAP_GLOCK18;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.75f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 20;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_GLOCK18;
			break;
		case WEAPONLIST_AWP:
			weapon->ClipCap = CLIPCAP_AWP;
			weapon->penetrate = WALL_PEN2;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.99f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 115;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_AWP;
			break;
		case WEAPONLIST_MP5:
			weapon->ClipCap = CLIPCAP_MP5;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.84f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 26;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_MP5;
			break;
		case WEAPONLIST_M249:
			weapon->ClipCap = CLIPCAP_M249;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.97f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 32;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_M249;
			break;
		case WEAPONLIST_M3:
			weapon->ClipCap = CLIPCAP_M3;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 0.0f;
			weapon->wallpierce1 = 0.0f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 0;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_M3;
			break;
		case WEAPONLIST_M4A1:
			weapon->ClipCap = CLIPCAP_M4A1;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.97f; // unsilenced
			weapon->wallpierce2 = 0.95f; // silenced
			weapon->damage1 = 32; // unsilenced
			weapon->damage2 = 33; // silenced
			weapon->bullettype = BULLET_M4A1;
			break;
		case WEAPONLIST_TMP:
			weapon->ClipCap = CLIPCAP_TMP;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.85f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 20;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_TMP;
			break;
		case WEAPONLIST_G3SG1:
			weapon->ClipCap = CLIPCAP_G3SG1;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.98f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 60;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_G3SG1;
			break;
		case WEAPONLIST_FLASHBANG:
			weapon->ClipCap = 0;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 0.0f;
			weapon->wallpierce1 = 0.0f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 0;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_NONE;
			break;
		case WEAPONLIST_DEAGLE:
			weapon->ClipCap = CLIPCAP_DEAGLE;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 4096.0f;
			weapon->wallpierce1 = 0.81f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 54;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_DEAGLE;
			break;
		case WEAPONLIST_SG552:
			weapon->ClipCap = CLIPCAP_SG552;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.955f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 33;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_SG552;
			break;
		case WEAPONLIST_AK47:
			weapon->ClipCap = CLIPCAP_AK47;
			weapon->penetrate = WALL_PEN1;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.98f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 36;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_AK47;
			break;
		case WEAPONLIST_KNIFE:
			weapon->ClipCap = 0;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 0.0f;
			weapon->wallpierce1 = 0.0f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 0;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_NONE;
			break;
		case WEAPONLIST_P90:
			weapon->ClipCap = CLIPCAP_P90;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 8192.0f;
			weapon->wallpierce1 = 0.885f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 21;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_P90;
			break;
		default:
			weapon->ClipCap = 0;
			weapon->penetrate = WALL_PEN0;
			weapon->distance = 0.0f;
			weapon->wallpierce1 = 0.0f;
			weapon->wallpierce2 = 0.0f;
			weapon->damage1 = 0;
			weapon->damage2 = 0;
			weapon->bullettype = BULLET_NONE;
			break;
	}
}


void WeaponListAdd(const char *weaponname, int ammo1type, int max1, int ammo2type, int max2, int slot, int slotpos, int id, int flags)
{
	struct Weapon_List dummy;
	int index, len;
	
	WeaponListStruct.push_back(dummy);

	index = WeaponListStruct.size() - 1;

	len = strlen(weaponname);
	if (len > MAX_WEAPON_NAME)
		return;

	memcpy(WeaponListStruct[index].command, weaponname, len + 1);

	if (len > 7 && !memcmp(weaponname, "weapon_", 7))
	{
		weaponname = weaponname + 7;
		WeaponListStruct[index].len = len - 7;
	}
	else
		WeaponListStruct[index].len = len;

	// Catch the special case of mp5navy where the msghook name differs from model name mp5
	if (WeaponListStruct[index].len == 7 && !memcmp(weaponname, "mp5navy", 7))
		WeaponListStruct[index].len = 3;

	memcpy(WeaponListStruct[index].name, weaponname, WeaponListStruct[index].len);
	WeaponListStruct[index].name[WeaponListStruct[index].len] = 0;

	WeaponListStruct[index].AmmoType = ammo1type;
	if (max1 == 255)
		WeaponListStruct[index].Max1 = -1;
	else
		WeaponListStruct[index].Max1 = max1;

	WeaponListStruct[index].Ammo2Type = ammo2type;
	if (max2 == 255)
		WeaponListStruct[index].Max2 = -1;
	else
		WeaponListStruct[index].Max2 = max2;

	WeaponListStruct[index].Slot = slot;
	WeaponListStruct[index].SlotPos = slotpos;

	WeaponListStruct[index].Id = id;
	WeaponListStruct[index].Flags = flags;

	WeaponListStruct[index].CAmmo = 0;
	WeaponListStruct[index].XAmmo = 0;

	WeaponListStruct[index].Active = false;
	WeaponListStruct[index].CurWeapon = false;

	SetWeaponData(&WeaponListStruct[index]);

	memset(&WeaponListStruct[index].weapondata, 0, sizeof(weapon_data_t));
}


const char *GetWeaponNameByID(int Id)
{
	int index;

	index = GetWeaponIndexByID(Id);
	if (index != -1)
			return WeaponListStruct[index].name;

	return NULL;
}


const char *GetWeaponName(int weaponmodel)
{
	int len, i;
	char *weaponname;
	model_s* mdl;

	mdl = g_Studio.GetModelByIndex(weaponmodel);
	if(!mdl || !mdl->name)
		return "unknown";

	weaponname = mdl->name;
	
	len = strlen(weaponname);

	if (len < 14)
		return "unknown";

	weaponname = weaponname + 9;
	len -= 13;

	for (i=0;i<WeaponListStruct.size();i++)
	{
		if (WeaponListStruct[i].len == len && !memcmp(WeaponListStruct[i].name, weaponname, len))
			return WeaponListStruct[i].name;
	}
	
	return "unknown";
}


