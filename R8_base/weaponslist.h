
#ifndef WEAPONSLIST_H
#define WEAPONSLIST_H

#include <vector>
#include "main.h"

#define WEAPONLIST_P228		1
#define	WEAPONLIST_UNKNOWN1	2
#define	WEAPONLIST_SCOUT	3
#define	WEAPONLIST_HEGRENADE 4
#define	WEAPONLIST_XM1014 5
#define	WEAPONLIST_C4 6
#define	WEAPONLIST_MAC10 7
#define	WEAPONLIST_AUG 8
#define	WEAPONLIST_SMOKEGRENADE 9
#define	WEAPONLIST_ELITE 10
#define	WEAPONLIST_FIVESEVEN 11
#define	WEAPONLIST_UMP45 12
#define	WEAPONLIST_SG550 13
#define	WEAPONLIST_GALIL 14
#define	WEAPONLIST_FAMAS 15
#define	WEAPONLIST_USP 16
#define	WEAPONLIST_GLOCK18 17
#define	WEAPONLIST_AWP 18
#define	WEAPONLIST_MP5 19
#define	WEAPONLIST_M249 20
#define	WEAPONLIST_M3 21
#define	WEAPONLIST_M4A1 22
#define	WEAPONLIST_TMP 23
#define	WEAPONLIST_G3SG1 24
#define	WEAPONLIST_FLASHBANG 25
#define	WEAPONLIST_DEAGLE 26
#define	WEAPONLIST_SG552 27
#define	WEAPONLIST_AK47 28
#define	WEAPONLIST_KNIFE 29
#define	WEAPONLIST_P90 30


#define CLIPCAP_AK47 30
#define CLIPCAP_AUG 30
#define CLIPCAP_AWP 10
#define CLIPCAP_DEAGLE 7
#define CLIPCAP_ELITE 15
#define CLIPCAP_FIVESEVEN 20
#define CLIPCAP_G3SG1 20
#define CLIPCAP_GLOCK18 20
#define CLIPCAP_M249 100
#define CLIPCAP_M3 8
#define CLIPCAP_M4A1 30
#define CLIPCAP_MAC10 30
#define CLIPCAP_MP5 30
#define CLIPCAP_P228 13
#define CLIPCAP_P90 50
#define CLIPCAP_SCOUT 10
#define CLIPCAP_SG550 30
#define CLIPCAP_SG552 30
#define CLIPCAP_TMP 30
#define CLIPCAP_UMP45 25
#define CLIPCAP_USP 12
#define CLIPCAP_XM1014 7
#define CLIPCAP_GALIL 35
#define CLIPCAP_FAMAS 25

enum 
{
	 WEAPON_NONE = 0
	,WEAPON_USP,WEAPON_GLOCK,WEAPON_DEAGLE,WEAPON_P228,WEAPON_ELITE,WEAPON_FIVE7
	,WEAPON_XM,WEAPON_M3,WEAPON_GALIL,WEAPON_FAMAS
	,WEAPON_MP5,WEAPON_TMP,WEAPON_P90,WEAPON_MAC,WEAPON_UMP
	,WEAPON_AK,WEAPON_SIG,WEAPON_COLT,WEAPON_AUG
	,WEAPON_SCOUT,WEAPON_AWP,WEAPON_G3SG1,WEAPON_SG550
	,WEAPON_PARA,WEAPON_KNIFE
	,WEAPON_FLASH,WEAPON_HE,WEAPON_SMOKE,WEAPON_C4
};

//enum
//{
//	BULLETTYPE_0=0,
//	BULLETTYPE_1=1,
//	BULLETTYPE_2=2,
//	BULLETTYPE_3=3,
//	BULLETTYPE_4=4,
//	BULLETTYPE_5=5,
//	BULLETTYPE_6=6,
//	BULLETTYPE_7=7,
//	BULLETTYPE_8=8,
//	BULLETTYPE_9=9,
//	BULLETTYPE_10=10,
//	BULLETTYPE_11=11,
//	BULLETTYPE_12=12,
//	BULLETTYPE_13=13,
//	BULLETTYPE_14=14,
//	BULLETTYPE_15=15
//};

Weapon_List *GetCurWeapon(void);

const char *GetPriWeaponName(void); // Returns the name of the primary weapon
const char *GetSecWeaponName(void); // Returns the name of the secondary weapon

const char *GetCurWeaponName(void); // Returns current weapon name

int GetCurWeaponAmmo(void);

int GetPrimaryAmmo(void); // Returns the amount of ammo in the primary weapon's clip
int GetSecondaryAmmo(void); // Returns the amount of ammo in the secondary weapon's clip

int GetPrimaryAmmoX(void); // Returns the amount of extra ammo for the primary weapon
int GetSecondaryAmmoX(void); // Returns the amount of extra ammo for the secondary weapon

int GetWeaponIndexByID(int WeaponID);
int GetCurWeaponID(void);

bool HasPriWeapon(void); // Returns true if we have a primary weapon
bool HasSecWeapon(void); // Returns true if we have a secondary weapon

bool IsCurWeaponPri(void); // Returns true if the current weapon is the primary
bool IsCurWeaponSec(void); // Returns true if the current weapon is the secondary

bool IsCurWeaponGun(void); // Returns true if the current weapon is either a primary or secondary weapon
bool IsCurWeaponKnife(void); // Returns true if the current weapon is a knife
bool IsCurWeaponNonAttack(void); // Returns true if the current weapon is not a gun or a knife
bool IsCurWeaponNade(void);
bool IsCurWeaponC4(void);

bool CurWeaponHasAmmo(void); // Returns true if the current weapon has ammo
bool SecWeaponHasAmmo(void); // Returns true if the secondary weapon has ammo
bool BackupWeaponHasAmmo(void); // Returns true if the backup weapon has ammo

bool CanCurWeaponAttack(void);

bool IsReloading(void);

bool HasWeaponName(const char *weaponname); // Returns true if we curently have this weapon

const char *GetWeaponNameByID(int Id);

const char *GetWeaponName(int weaponmodel);

int GetCurWeaponId(void);
int GetPriWeaponId(void);
int GetSecWeaponId(void);

const char *GetPriWeaponCmd(void);
const char *GetSecWeaponCmd(void);

int GetCurRecoil(void);

int GetClipCap(int ID);

int CurBulletType(void);
float CurWallPierce(void);
int CurDamage(void);
float CurDistance(void);
int GetCurPenetration(void);

bool IsCurWeaponInBurst(void);

// MsgHook Functions
void WeaponListCurWeapon(int CurWeapon, int WeaponID, int Ammo);
void WeaponListAmmoX(int ID, int Count);
void WeaponListUpdate(int WeaponBits);
void WeaponListAdd(const char *weaponname, int ammo1type, int max1, int ammo2type, int max2, int slot, int slotpos, int id, int flags);

#endif
