#include "nospread.h"
#include "weaponslist.h"
#include "utils.h"

#pragma warning(disable:4018)
#pragma warning(disable:4800)
#pragma warning(disable:4244)
#pragma warning(disable:4101)
#pragma warning(disable:4715)
#pragma warning(disable:4305)

static unsigned int glSeed = 0; 

#define M_PI		3.14159265358979323846

cNoSpread gNoSpread;

unsigned int seed_table[ 256 ] =
{
	28985, 27138, 26457, 9451, 17764, 10909, 28790, 8716, 6361, 4853, 17798, 21977, 19643, 20662, 10834, 20103,
	27067, 28634, 18623, 25849, 8576, 26234, 23887, 18228, 32587, 4836, 3306, 1811, 3035, 24559, 18399, 315,
	26766, 907, 24102, 12370, 9674, 2972, 10472, 16492, 22683, 11529, 27968, 30406, 13213, 2319, 23620, 16823,
	10013, 23772, 21567, 1251, 19579, 20313, 18241, 30130, 8402, 20807, 27354, 7169, 21211, 17293, 5410, 19223,
	10255, 22480, 27388, 9946, 15628, 24389, 17308, 2370, 9530, 31683, 25927, 23567, 11694, 26397, 32602, 15031,
	18255, 17582, 1422, 28835, 23607, 12597, 20602, 10138, 5212, 1252, 10074, 23166, 19823, 31667, 5902, 24630,
	18948, 14330, 14950, 8939, 23540, 21311, 22428, 22391, 3583, 29004, 30498, 18714, 4278, 2437, 22430, 3439,
	28313, 23161, 25396, 13471, 19324, 15287, 2563, 18901, 13103, 16867, 9714, 14322, 15197, 26889, 19372, 26241,
	31925, 14640, 11497, 8941, 10056, 6451, 28656, 10737, 13874, 17356, 8281, 25937, 1661, 4850, 7448, 12744,
	21826, 5477, 10167, 16705, 26897, 8839, 30947, 27978, 27283, 24685, 32298, 3525, 12398, 28726, 9475, 10208,
	617, 13467, 22287, 2376, 6097, 26312, 2974, 9114, 21787, 28010, 4725, 15387, 3274, 10762, 31695, 17320,
	18324, 12441, 16801, 27376, 22464, 7500, 5666, 18144, 15314, 31914, 31627, 6495, 5226, 31203, 2331, 4668,
	12650, 18275, 351, 7268, 31319, 30119, 7600, 2905, 13826, 11343, 13053, 15583, 30055, 31093, 5067, 761,
	9685, 11070, 21369, 27155, 3663, 26542, 20169, 12161, 15411, 30401, 7580, 31784, 8985, 29367, 20989, 14203,
	29694, 21167, 10337, 1706, 28578, 887, 3373, 19477, 14382, 675, 7033, 15111, 26138, 12252, 30996, 21409,
	25678, 18555, 13256, 23316, 22407, 16727, 991, 9236, 5373, 29402, 6117, 15241, 27715, 19291, 19888, 19847
};

unsigned int cNoSpread::U_Random( void ) 
{ 
	glSeed *= 69069; 
	glSeed += seed_table[ glSeed & 0xff ];

	return ( ++glSeed & 0x0fffffff ); 
} 

void cNoSpread::U_Srand( unsigned int seed )
{
	glSeed = seed_table[ seed & 0xff ];
}

int cNoSpread::UTIL_SharedRandomLong( unsigned int seed, int low, int high )
{
	unsigned int range;

	U_Srand( (int)seed + low + high );

	range = high - low + 1;
	if ( !(range - 1) )
	{
		return low;
	}
	else
	{
		int offset;
		int rnum;

		rnum = U_Random();

		offset = rnum % range;

		return (low + offset);
	}
}

float cNoSpread::UTIL_SharedRandomFloat( unsigned int seed, float low, float high )
{
	unsigned int range;

	U_Srand( (int)seed + *(int *)&low + *(int *)&high );

	U_Random();
	U_Random();

	range = (unsigned int)(high - low);
	if ( !range )
	{
		return low;
	}
	else
	{
		int tensixrand;
		float offset;

		tensixrand = U_Random() & 65535;

		offset = (float)tensixrand / 65536.0f;

		return (low + offset * range );
	}
}

float cNoSpread::GetVecSpread(float speed)
{
	int id;
	float spread;

	id = g_Local.iWpnID;// GetCurWeaponId();
	if(g_Settings.NoSpread.calc == 1)
	{
		switch (id)
		{
		case WEAPONLIST_DEAGLE:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (/*speed &&*/ g_Local.spread.recoil == 0)
					spread = 0.2175 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.recoil<1)
					spread = 0.219f;
				else if (g_Local.spread.recoil<2)
					spread = 0.22f;
				else if (g_Local.spread.recoil<3)
					spread = 0.2205f;
				else if (g_Local.spread.recoil<4)
					spread = 0.2215f;
				else if (g_Local.spread.recoil<5)
					spread = 0.222f;
				else if (g_Local.spread.recoil<6)
					spread = 0.224f;
				else if (g_Local.spread.recoil<7)
					spread = 0.2245f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.115 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.13 * (1.0f - g_Local.spread.spreadvar);
			}		
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);
			break;
		case WEAPONLIST_ELITE:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.175 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.3 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_FIVESEVEN:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_GLOCK18:
			if (!(g_Local.spread.WeaponState & GLOCK18_BURST))
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.165 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.0f - g_Local.spread.spreadvar;
			}
			else
				spread = 0;

			break;
		case WEAPONLIST_P228:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_G3SG1:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.15f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.035 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.055 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		case WEAPONLIST_SG550:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.15f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.04 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.05 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		case WEAPONLIST_USP:
			if (!(g_Local.spread.WeaponState & USP_SILENCER))
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.225 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.2 * (1.0f - g_Local.spread.spreadvar);
			}
			else
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.125 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.3 * (1.0f - g_Local.spread.spreadvar);
			}

			break;
		case WEAPONLIST_AK47:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f && g_Local.spread.recoil == 0)
					spread = g_Local.spread.spreadvar*0.07 + 0.042955;
				else if (g_Local.spread.recoil<5)
					spread = g_Local.spread.spreadvar*0.07 + 0.043;
				else if (g_Local.spread.recoil<10)
					spread = g_Local.spread.spreadvar*0.07 + 0.043575;
				else if (g_Local.spread.recoil<15)
					spread = g_Local.spread.spreadvar*0.07 + 0.0440575;
				else if (g_Local.spread.recoil<20)
					spread = g_Local.spread.spreadvar*0.07 + 0.0447575;
				else if (g_Local.spread.recoil<25)
					spread = g_Local.spread.spreadvar*0.07 + 0.04558595;
				else if (g_Local.spread.recoil<30)
					spread = g_Local.spread.spreadvar*0.07 + 0.0462575285;

				if (g_Local.spread.recoil == 0)
					spread = g_Local.spread.spreadvar*0.0253;
				else if (g_Local.spread.recoil<5)
					spread = g_Local.spread.spreadvar*0.025425;
				else if (g_Local.spread.recoil<10)
					spread = g_Local.spread.spreadvar*0.025525;
				else if (g_Local.spread.recoil<15)
					spread = g_Local.spread.spreadvar*0.0255575;
				else if (g_Local.spread.recoil<20)
					spread = g_Local.spread.spreadvar*0.02565;
				else if (g_Local.spread.recoil<25)
					spread = g_Local.spread.spreadvar*0.025717;
				else if (g_Local.spread.recoil<30)
					spread = g_Local.spread.spreadvar*0.025895;
			}
			else if (g_Local.spread.recoil == 0)
				spread = g_Local.spread.spreadvar*0.43 + .0428;
			else if (g_Local.spread.recoil <5)
				spread = g_Local.spread.spreadvar*0.43 + .043;
			else if (g_Local.spread.recoil<10)
				spread = g_Local.spread.spreadvar*0.43 + .043275;
			else if (g_Local.spread.recoil<15)
				spread = g_Local.spread.spreadvar*0.43 + .04352;
			else if (g_Local.spread.recoil<20)
				spread = g_Local.spread.spreadvar*0.43 + .04372;
			else if (g_Local.spread.recoil<25)
				spread = g_Local.spread.spreadvar*0.43 + .043958;
			else if (g_Local.spread.recoil<30)
				spread = g_Local.spread.spreadvar*0.43 + .04405;

			break;
		case WEAPONLIST_SG552:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f && g_Local.spread.recoil == 0)
					spread = g_Local.spread.spreadvar*0.07 + 0.035;
				else if (g_Local.spread.recoil<5)
					spread = g_Local.spread.spreadvar*0.07 + 0.0351575;
				else if (g_Local.spread.recoil<10)
					spread = g_Local.spread.spreadvar*0.07 + 0.03525;
				else if (g_Local.spread.recoil<15)
					spread = g_Local.spread.spreadvar*0.07 + 0.03545;
				else if (g_Local.spread.recoil<20)
					spread = g_Local.spread.spreadvar*0.07 + 0.0362575;
				else if (g_Local.spread.recoil<25)
					spread = g_Local.spread.spreadvar*0.07 + 0.0363595;
				else if (g_Local.spread.recoil<30)
					spread = g_Local.spread.spreadvar*0.07 + 0.037058596;

				if (g_Local.spread.recoil == 0)
					spread = g_Local.spread.spreadvar*0.02;
				else if (g_Local.spread.recoil<5)
					spread = g_Local.spread.spreadvar*0.020125;
				else if (g_Local.spread.recoil<10)
					spread = g_Local.spread.spreadvar*0.020225;
				else if (g_Local.spread.recoil<15)
					spread = g_Local.spread.spreadvar*0.0203575;
				else if (g_Local.spread.recoil<20)
					spread = g_Local.spread.spreadvar*0.02045;
				else if (g_Local.spread.recoil<25)
					spread = g_Local.spread.spreadvar*0.020517;
				else if (g_Local.spread.recoil<30)
					spread = g_Local.spread.spreadvar*0.020595;
			}
			else if (g_Local.spread.recoil == 0)
				spread = g_Local.spread.spreadvar*0.43 + .0358;
			else if (g_Local.spread.recoil <5)
				spread = g_Local.spread.spreadvar*0.43 + .036;
			else if (g_Local.spread.recoil<10)
				spread = g_Local.spread.spreadvar*0.43 + .036275;
			else if (g_Local.spread.recoil<15)
				spread = g_Local.spread.spreadvar*0.43 + .03652;
			else if (g_Local.spread.recoil<20)
				spread = g_Local.spread.spreadvar*0.43 + .03672;
			else if (g_Local.spread.recoil<25)
				spread = g_Local.spread.spreadvar*0.43 + .036958;
			else if (g_Local.spread.recoil<30)
				spread = g_Local.spread.spreadvar*0.43 + .03705;

			break;
		case WEAPONLIST_AUG:
			if (g_Local.spread.prcflags & FL_ONGROUND)	
			{				
				if (speed > 140.0f)		
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;	
				else				
					spread = g_Local.spread.spreadvar * 0.02;	
			}			
			else			
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_M249:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed > 140.0f)		
				{
					if (g_Local.spread.recoil == 0)	
						spread = 0.064f;		
					else if (g_Local.spread.recoil< 6)		
						spread = 0.083f;	
					else					
						spread = 0.1305f;	
				}
				else
				{
					if (g_Local.spread.recoil == 0)			
						spread = 0.006f;		
					else if (g_Local.spread.recoil< 6)		
						spread = 0.012f;		
					else					
						spread = 0.027f;
				}
			}				
			else	
			{
				if (g_Local.spread.recoil == 0)		
					spread = 0.145f;			
				else if (g_Local.spread.recoil< 6)				
					spread = 0.245f;			
				else				
					spread = 0.495f;	
			}
			break;
		case WEAPONLIST_M4A1:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (!(g_Local.spread.WeaponState & M4A1_SILENCER))
				{
					if (speed > 140.0f)
						spread = g_Local.spread.spreadvar*0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar*0.02;
				}
				else
				{
					if (speed > 140.0f && g_Local.spread.recoil == 0)
						spread = g_Local.spread.spreadvar*0.07 + 0.03505;
					else if (g_Local.spread.recoil<5)
						spread = g_Local.spread.spreadvar*0.07 + 0.0351;
					else if (g_Local.spread.recoil<10)
						spread = g_Local.spread.spreadvar*0.07 + 0.03515;
					else if (g_Local.spread.recoil<15)
						spread = g_Local.spread.spreadvar*0.07 + 0.0351595;
					else if (g_Local.spread.recoil<20)
						spread = g_Local.spread.spreadvar*0.07 + 0.0352575;
					else if (g_Local.spread.recoil<25)
						spread = g_Local.spread.spreadvar*0.07 + 0.0353595;
					else if (g_Local.spread.recoil<30)
						spread = g_Local.spread.spreadvar*0.07 + 0.035358596;

					if (g_Local.spread.recoil == 0)
						spread = g_Local.spread.spreadvar*0.02;
					else if (g_Local.spread.recoil<5)
						spread = g_Local.spread.spreadvar*0.020105;
					else if (g_Local.spread.recoil<10)
						spread = g_Local.spread.spreadvar*0.020275;
					else if (g_Local.spread.recoil<15)
						spread = g_Local.spread.spreadvar*0.020325;
					else if (g_Local.spread.recoil<20)
						spread = g_Local.spread.spreadvar*0.020425;
					else if (g_Local.spread.recoil<25)
						spread = g_Local.spread.spreadvar*0.020515;
					else if (g_Local.spread.recoil<30)
						spread = g_Local.spread.spreadvar*0.0206175;
				}
			}
			else if (g_Local.spread.recoil == 0)
				spread = g_Local.spread.spreadvar*0.43 + .0368;
			else if (g_Local.spread.recoil <5)
				spread = g_Local.spread.spreadvar*0.43 + .0369;
			else if (g_Local.spread.recoil<10)
				spread = g_Local.spread.spreadvar*0.43 + .03715;
			else if (g_Local.spread.recoil<15)
				spread = g_Local.spread.spreadvar*0.43 + .03752;
			else if (g_Local.spread.recoil<20)
				spread = g_Local.spread.spreadvar*0.43 + .03772;
			else if (g_Local.spread.recoil<25)
				spread = g_Local.spread.spreadvar*0.43 + .0437958;
			else if (g_Local.spread.recoil<30)
				spread = g_Local.spread.spreadvar*0.43 + .03705;

			break;
		case WEAPONLIST_MP5:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{			
				if (speed > 120.0f)	
				{				
					if (g_Local.spread.recoil == 0)		
						spread = 0.0f;			
					else if (g_Local.spread.recoil < 2)	
						spread = 0.022543f;			
					else if (g_Local.spread.recoil < 3)		
						spread = 0.018727f;			
					else if (g_Local.spread.recoil < 4)		
						spread = 0.019636f;			
					else if (g_Local.spread.recoil < 5)		
						spread = 0.020908f;				
					else if (g_Local.spread.recoil < 6)		
						spread = 0.022543f;				
					else if (g_Local.spread.recoil < 7)		
						spread = 0.024542f;				
					else if (g_Local.spread.recoil < 8)		
						spread = 0.026905f;				
					else if (g_Local.spread.recoil < 9)		
						spread = 0.029631f;				
					else		
						spread = 0.03f;	
				}	
				else		
				{			
					if (g_Local.spread.recoil == 0)		
						spread = 0.0f;		
					else if (g_Local.spread.recoil < 2)		
						spread = 0.022543f;			
					else if (g_Local.spread.recoil < 3)		
						spread = 0.018727f;				
					else if (g_Local.spread.recoil < 4)		
						spread = 0.019636f;				
					else if (g_Local.spread.recoil < 5)		
						spread = 0.020908f;				
					else if (g_Local.spread.recoil < 6)			
						spread = 0.022543f;				
					else if (g_Local.spread.recoil < 7)		
						spread = 0.024542f;				
					else if (g_Local.spread.recoil < 8)		
						spread = 0.026905f;				
					else if (g_Local.spread.recoil < 9)		
						spread = 0.029631f;				
					else					
						spread = 0.03f;			
				}		
			}		
			else		
			{			
				if (g_Local.spread.recoil == 0)		
					spread = 0.0f;			
				else if (g_Local.spread.recoil < 2)		
					spread = 0.090909f;			
				else if (g_Local.spread.recoil < 3)		
					spread = 0.093635f;			
				else if (g_Local.spread.recoil < 4)			
					spread = 0.098178f;				
				else if (g_Local.spread.recoil < 5)		
					spread = 0.104539f;			
				else if (g_Local.spread.recoil < 6)		
					spread = 0.112717f;				
				else if (g_Local.spread.recoil < 7)			
					spread = 0.122712f;				
				else if (g_Local.spread.recoil < 8)			
					spread = 0.134525f;				
				else if (g_Local.spread.recoil < 9)			
					spread = 0.148155f;				
				else				
					spread = 0.15f;	
			}	
			break;
		case WEAPONLIST_MAC10:
			if (g_Local.spread.prcflags & FL_ONGROUND)	
				spread = 0.03 * g_Local.spread.spreadvar;	
			else			
				spread = 0.375 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_P90:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 170.0f)
					spread = g_Local.spread.spreadvar * 0.115;
				else
					spread = g_Local.spread.spreadvar * 0.045;
			}
			else
				spread = g_Local.spread.spreadvar * 0.3;

			break;
		case WEAPONLIST_TMP:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.25 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_UMP45:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.24 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_AWP:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 10.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.001f;
				}
				else if (speed < 140.0f)
				{
					spread = 0.1f;
				}
				else
					spread = 0.25f;
			}
			else
				spread = 0.85f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.08f;

			break;
		case WEAPONLIST_SCOUT:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 170.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.021f;
				}
				else
					spread = 0.087f;
			}
			else
				spread = 0.2104f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.028f;

			break;
		default:
			spread = 0;
			break;
		}
	}
	else if(g_Settings.NoSpread.calc == 2)
	{
		switch (id)
		{
		case WEAPONLIST_DEAGLE:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.115 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.13 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_ELITE:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.175 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.3 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_FIVESEVEN:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_GLOCK18:
			if (!(g_Local.spread.WeaponState & GLOCK18_BURST))
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.165 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.0f - g_Local.spread.spreadvar;
			}
			else
				spread = 0;

			break;
		case WEAPONLIST_P228:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_G3SG1:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.15f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.035 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.055 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		case WEAPONLIST_SG550:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.15f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.04 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.05 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		case WEAPONLIST_USP:
			if (!(g_Local.spread.WeaponState & USP_SILENCER))
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.225 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.2 * (1.0f - g_Local.spread.spreadvar);
			}
			else
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.125 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.3 * (1.0f - g_Local.spread.spreadvar);
			}

			break;
		case WEAPONLIST_AK47:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.04;
				else
					spread = g_Local.spread.spreadvar * 0.0275;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .04;

			break;
		case WEAPONLIST_GALIL:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.04;
				else
					spread = g_Local.spread.spreadvar * 0.0275;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .04;

			break;
		case WEAPONLIST_SG552:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;
				else
					spread = g_Local.spread.spreadvar * 0.02;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_AUG:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;
				else
					spread = g_Local.spread.spreadvar * 0.02;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_M249:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.095 + 0.045;
				else
					spread = g_Local.spread.spreadvar * 0.03;
			}
			else
				spread = g_Local.spread.spreadvar * 0.5 + .045;

			break;
		case WEAPONLIST_M4A1:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (!(g_Local.spread.WeaponState & M4A1_SILENCER))
				{
					if (speed > 140.0f)
						spread = g_Local.spread.spreadvar * 0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar * 0.02;
				}
				else
				{
					if (speed > 140.0f)
						spread = g_Local.spread.spreadvar * 0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar * 0.025;
				}
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_MP5:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.2 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_MAC10:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.375 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_P90:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 170.0f)
					spread = g_Local.spread.spreadvar * 0.115;
				else
					spread = g_Local.spread.spreadvar * 0.045;
			}
			else
				spread = g_Local.spread.spreadvar * 0.3;

			break;
		case WEAPONLIST_TMP:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.25 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_UMP45:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.24 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_AWP:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 10.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.001f;
				}
				else if (speed < 140.0f)
				{
					spread = 0.1f;
				}
				else
					spread = 0.25f;
			}
			else
				spread = 0.85f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.08f;

			break;
		case WEAPONLIST_SCOUT:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 170.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.007f;
				}
				else
					spread = 0.075f;
			}
			else
				spread = 0.2f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		default:
			spread = 0;
			break;
		}
	}
	else if(g_Settings.NoSpread.calc == 3)
	{
		switch (id)
		{
		case WEAPONLIST_DEAGLE:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.115 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.13 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_ELITE:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.175 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.3 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_GALIL:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.04;
				else
					spread = g_Local.spread.spreadvar * 0.0375;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + 0.03;

			break;
		case WEAPONLIST_FAMAS:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.03;
				else
					spread = g_Local.spread.spreadvar * 0.020;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .04;
			break;
		case WEAPONLIST_FIVESEVEN:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_GLOCK18:
			if (!IsCurWeaponInBurst())
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.165 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.0f - g_Local.spread.spreadvar;
			}
			else
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.185 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.095 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.3 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.2 * (1.0f - g_Local.spread.spreadvar);
			}

			break;
		case WEAPONLIST_P228:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_G3SG1:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 100.0f)
					spread = 0.15f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.04f * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.06f * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 0.45f * (1.0f - g_Local.spread.spreadvar);

			if (!(g_Local.iFOV < 90.0f))
				spread += .0016f;

			break;
		case WEAPONLIST_SG550:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.15f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.04 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.05 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		case WEAPONLIST_USP:
			if (!IsCurWeaponSilenced())
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.225 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.2 * (1.0f - g_Local.spread.spreadvar);
			}
			else
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.125 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.3 * (1.0f - g_Local.spread.spreadvar);
			}

			break;
		case WEAPONLIST_AK47:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.04;
				else
					spread = g_Local.spread.spreadvar * 0.0275;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .04;

			break;
		case WEAPONLIST_SG552:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;
				else
					spread = g_Local.spread.spreadvar * 0.02;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_AUG:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;
				else
					spread = g_Local.spread.spreadvar * 0.02;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_M249:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.095 + 0.045;
				else
					spread = g_Local.spread.spreadvar * 0.03;
			}
			else
				spread = g_Local.spread.spreadvar * 0.5 + .045;

			break;
		case WEAPONLIST_M4A1:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (!IsCurWeaponSilenced())
				{
					if (speed > 140.0f)
						spread = g_Local.spread.spreadvar * 0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar * 0.02;
				}
				else
				{
					if (speed > 140.0f)
						spread = g_Local.spread.spreadvar * 0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar * 0.025;
				}
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_MP5:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.2 * g_Local.spread.spreadvar;
			break;
		case WEAPONLIST_MAC10:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.375 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_P90:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 170.0f)
					spread = g_Local.spread.spreadvar * 0.115;
				else
					spread = g_Local.spread.spreadvar * 0.045;
			}
			else
				spread = g_Local.spread.spreadvar * 0.3;

			break;
		case WEAPONLIST_TMP:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.25 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_UMP45:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.24 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_AWP:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 10.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.001f;
				}
				else if (speed < 140.0f)
				{
					spread = 0.1f;
				}
				else
					spread = 0.25f;
			}
			else
				spread = 0.85f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.08f;

			break;
		case WEAPONLIST_SCOUT:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 170.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.007f;
				}
				else
					spread = 0.075f;
			}
			else
				spread = 0.2f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		default:
			spread = 0;
			break;
		}
	}
	else if(g_Settings.NoSpread.calc == 4)
	{
		float recoil = g_Local.spread.recoil;
		if (id==WEAPONLIST_DEAGLE)	
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed)
					spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.115 * (1.0f - g_Local.spread.spreadvar);
				else					
					spread = 0.13 * (1.0f - g_Local.spread.spreadvar);
			}			
			else				
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);
		}	
		else if (id==WEAPONLIST_ELITE)
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed)
					spread = 0.175 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
				else					
					spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
			}			
			else				
				spread = 1.3 * (1.0f - g_Local.spread.spreadvar);
		}	
		else if (id==WEAPONLIST_FIVESEVEN)
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed)					
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else					
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}			
			else				
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);
		}	
		else if (id==WEAPONLIST_GLOCK18)
		{			
			if (!(g_Local.spread.WeaponState & GLOCK18_BURST))
			{				
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{					
					if (speed)						
						spread = 0.165 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
					else						
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}				
				else					
					spread = 1.0f - g_Local.spread.spreadvar;
			}			
			else				
				spread = 0;
		}	
		else if (id==WEAPONLIST_P228)
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed)					
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else					
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}			
			else				
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);
		}	
		else if (id==WEAPONLIST_G3SG1)	
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)			
			{				
				if (speed)					
					spread = 0.15f;				
				else if (g_Local.spread.prcflags & FL_DUCKING)					
					spread = 0.035 * (1.0f - g_Local.spread.spreadvar);				
				else					
					spread = 0.055 * (1.0f - g_Local.spread.spreadvar);
			}			
			else				
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);
			if (!(g_Local.iFOV < 90.0f))				
				spread += 0.025f;	
		}	
		else if (id==WEAPONLIST_SG550)	
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed)	
					spread = 0.15f;	
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.04 * (1.0f - g_Local.spread.spreadvar);	
				else					
					spread = 0.05 * (1.0f - g_Local.spread.spreadvar);
			}			
			else				
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);	
			if (!(g_Local.iFOV < 90.0f))		
				spread += 0.025f;	
		}	
		else if (id==WEAPONLIST_USP)	
		{			
			if (!(g_Local.spread.WeaponState & USP_SILENCER))
			{				
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{				
					if (speed)				
						spread = 0.225 * (1.0f - g_Local.spread.spreadvar);	
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.08 * (1.0f - g_Local.spread.spreadvar);	
					else			
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}				
				else					
					spread = 1.2 * (1.0f - g_Local.spread.spreadvar);
			}			
			else			
			{				
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{					
					if (speed)						
						spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.125 * (1.0f - g_Local.spread.spreadvar);	
					else						
						spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
				}				
				else					
					spread = 1.3 * (1.0f - g_Local.spread.spreadvar);			
			}	
		}	
		else if (id==WEAPONLIST_AK47)	
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed > 140.0f)
					if (g_Local.spread.recoil == 0)
						spread = 0.054f;
					else if (g_Local.spread.recoil < 6)
						spread = 0.0645f;
					else						
						spread = 0.1275f;				
				else					
					if (g_Local.spread.recoil == 0)
						spread = 0.0055f;
					else if (g_Local.spread.recoil < 6)
						spread = 0.009625f;	
					else					
						spread = 0.034375f;		
			}			
			else				
				if (g_Local.spread.recoil == 0)	
					spread = 0.12f;			
				else if (g_Local.spread.recoil < 6)	
					spread = 0.18f;			
				else					
					spread = 0.54f;	
		}	
		else if (id==WEAPONLIST_P228)
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed > 140.0f)	
					if (g_Local.spread.recoil == 0)	
						spread = 0.05f;	
					else if (g_Local.spread.recoil < 7)	
						spread = 0.0575f;	
					else				
						spread = 0.11f;		
				else				
					if (g_Local.spread.recoil == 0)	
						spread = 0.004f;	
					else if (g_Local.spread.recoil < 7)	
						spread = 0.006f;	
					else					
						spread = 0.02f;		
			}			
			else			
				if (g_Local.spread.recoil == 0)	
					spread = 0.125f;		
				else if (g_Local.spread.recoil < 7)	
					spread = 0.17f;			
				else				
					spread = 0.485f;
		}
		else if (id==WEAPONLIST_AUG)	
		{		
			if (g_Local.spread.prcflags & FL_ONGROUND)	
			{				
				if (speed > 140.0f)		
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;	
				else				
					spread = g_Local.spread.spreadvar * 0.02;	
			}			
			else			
				spread = g_Local.spread.spreadvar * 0.4 + .035;
		}	
		else if (id==WEAPONLIST_M249)
		{		
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{				
				if (speed > 140.0f)		
					if (recoil == 0)	
						spread = 0.064f;		
					else if (recoil < 6)		
						spread = 0.083f;	
					else					
						spread = 0.1305f;		
				else				
					if (recoil == 0)			
						spread = 0.006f;		
					else if (recoil < 6)		
						spread = 0.012f;		
					else					
						spread = 0.027f;	
			}		
			else			
				if (recoil == 0)		
					spread = 0.145f;			
				else if (recoil < 6)				
					spread = 0.245f;			
				else				
					spread = 0.495f;	
		}
		else if (id==WEAPONLIST_M4A1)
		{		
			if (g_Local.spread.prcflags & FL_ONGROUND)	
			{			
				if (speed > 120.0f)		
				{				
					if (g_Local.spread.recoil == 0)	
						spread = 0.049f;			
					else if (g_Local.spread.recoil < 7)		
						spread = 0.056f;				
					else			
						spread = 0.105f;	
				}		
				else	
				{		
					if (g_Local.spread.recoil == 0)	
						spread = 0.004f;		
					else if (g_Local.spread.recoil < 7)	
						spread = 0.006f;		
					else					
						spread = 0.02f;			
				}		
			}		
			else	
			{		
				if (g_Local.spread.recoil == 0)	
					spread = 0.115f;	
				else if (g_Local.spread.recoil < 7)	
					spread = 0.155f;		
				else				
					spread = 0.435f;	
			}
		}	
		else if (id==WEAPONLIST_MP5)	
		{		
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{			
				if (speed > 120.0f)	
				{				
					if (g_Local.spread.recoil == 0)		
						spread = 0.0f;			
					else if (g_Local.spread.recoil < 2)	
						spread = 0.022543f;			
					else if (g_Local.spread.recoil < 3)		
						spread = 0.018727f;			
					else if (g_Local.spread.recoil < 4)		
						spread = 0.019636f;			
					else if (g_Local.spread.recoil < 5)		
						spread = 0.020908f;				
					else if (g_Local.spread.recoil < 6)		
						spread = 0.022543f;				
					else if (g_Local.spread.recoil < 7)		
						spread = 0.024542f;				
					else if (g_Local.spread.recoil < 8)		
						spread = 0.026905f;				
					else if (g_Local.spread.recoil < 9)		
						spread = 0.029631f;				
					else		
						spread = 0.03f;	
				}	
				else		
				{			
					if (g_Local.spread.recoil == 0)		
						spread = 0.0f;		
					else if (g_Local.spread.recoil < 2)		
						spread = 0.022543f;			
					else if (g_Local.spread.recoil < 3)		
						spread = 0.018727f;				
					else if (g_Local.spread.recoil < 4)		
						spread = 0.019636f;				
					else if (g_Local.spread.recoil < 5)		
						spread = 0.020908f;				
					else if (g_Local.spread.recoil < 6)			
						spread = 0.022543f;				
					else if (g_Local.spread.recoil < 7)		
						spread = 0.024542f;				
					else if (g_Local.spread.recoil < 8)		
						spread = 0.026905f;				
					else if (g_Local.spread.recoil < 9)		
						spread = 0.029631f;				
					else					
						spread = 0.03f;			
				}		
			}		
			else		
			{			
				if (g_Local.spread.recoil == 0)		
					spread = 0.0f;			
				else if (g_Local.spread.recoil < 2)		
					spread = 0.090909f;			
				else if (g_Local.spread.recoil < 3)		
					spread = 0.093635f;			
				else if (g_Local.spread.recoil < 4)			
					spread = 0.098178f;				
				else if (g_Local.spread.recoil < 5)		
					spread = 0.104539f;			
				else if (g_Local.spread.recoil < 6)		
					spread = 0.112717f;				
				else if (g_Local.spread.recoil < 7)			
					spread = 0.122712f;				
				else if (g_Local.spread.recoil < 8)			
					spread = 0.134525f;				
				else if (g_Local.spread.recoil < 9)			
					spread = 0.148155f;				
				else				
					spread = 0.15f;	
			}	
		}	
		else if (id==WEAPONLIST_MAC10)
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)	
				spread = 0.03 * g_Local.spread.spreadvar;	
			else			
				spread = 0.375 * g_Local.spread.spreadvar;
		}	
		else if (id==WEAPONLIST_P90)	
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)	
			{				
				if (speed > 170.0f)	
					spread = g_Local.spread.spreadvar * 0.115;	
				else				
					spread = g_Local.spread.spreadvar * 0.045;	
			}			
			else				
				spread = g_Local.spread.spreadvar * 0.3;
		}	
		else if (id==WEAPONLIST_TMP)	
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)	
				spread = 0.03 * g_Local.spread.spreadvar;
			else			
				spread = 0.25 * g_Local.spread.spreadvar;	
		}	
		else if (id==WEAPONLIST_UMP45)
		{			
			if (g_Local.spread.prcflags & FL_ONGROUND)	
				spread = 0.04 * g_Local.spread.spreadvar;
			else			
				spread = 0.24 * g_Local.spread.spreadvar;
		}
		else if (id==WEAPONLIST_AWP)
		{		
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 10.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.001f;
				}
				else if (speed < 140.0f)
				{
					spread = 0.1f;
				}
				else
					spread = 0.25f;
			}
			else
				spread = 0.85f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.08f;

		}
		else if (id==WEAPONLIST_SCOUT)	
		{	
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 170.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.012f;
				}
				else
					spread = 0.085f;
			}
			else
				spread = 0.2104f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

		}
	}
	else
	{
		switch (id)
		{
		case WEAPONLIST_DEAGLE:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.115 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.13 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_ELITE:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.175 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.3 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_GALIL:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.04;
				else
					spread = g_Local.spread.spreadvar * 0.0375;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + 0.03;

			break;
		case WEAPONLIST_FAMAS:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.03;
				else
					spread = g_Local.spread.spreadvar * 0.020;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .04;
			break;
		case WEAPONLIST_FIVESEVEN:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_GLOCK18:
			if (!IsCurWeaponInBurst())
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.165 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.0f - g_Local.spread.spreadvar;
			}
			else
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.185 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.095 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.3 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.2 * (1.0f - g_Local.spread.spreadvar);
			}

			break;
		case WEAPONLIST_P228:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.255 * (1.0f - g_Local.spread.spreadvar);
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 1.5 * (1.0f - g_Local.spread.spreadvar);

			break;
		case WEAPONLIST_G3SG1:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.15f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.035 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.055 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		case WEAPONLIST_SG550:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.15f;
				else if (g_Local.spread.prcflags & FL_DUCKING)
					spread = 0.04 * (1.0f - g_Local.spread.spreadvar);
				else
					spread = 0.05 * (1.0f - g_Local.spread.spreadvar);
			}
			else
				spread = 0.45 * (1.0f - g_Local.spread.spreadvar);

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		case WEAPONLIST_USP:
			if (!IsCurWeaponSilenced())
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.225 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.08 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.1 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.2 * (1.0f - g_Local.spread.spreadvar);
			}
			else
			{
				if (g_Local.spread.prcflags & FL_ONGROUND)
				{
					if (speed)
						spread = 0.25 * (1.0f - g_Local.spread.spreadvar);
					else if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0.125 * (1.0f - g_Local.spread.spreadvar);
					else
						spread = 0.15 * (1.0f - g_Local.spread.spreadvar);
				}
				else
					spread = 1.3 * (1.0f - g_Local.spread.spreadvar);
			}

			break;
		case WEAPONLIST_AK47:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.04;
				else
					spread = g_Local.spread.spreadvar * 0.0275;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .04;

			break;
		case WEAPONLIST_SG552:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;
				else
					spread = g_Local.spread.spreadvar * 0.02;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_AUG:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.07 + 0.035;
				else
					spread = g_Local.spread.spreadvar * 0.02;
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_M249:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 140.0f)
					spread = g_Local.spread.spreadvar * 0.095 + 0.045;
				else
					spread = g_Local.spread.spreadvar * 0.03;
			}
			else
				spread = g_Local.spread.spreadvar * 0.5 + .045;

			break;
		case WEAPONLIST_M4A1:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (!IsCurWeaponSilenced())
				{
					if (speed > 140.0f)
						spread = g_Local.spread.spreadvar * 0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar * 0.02;
				}
				else
				{
					if (speed > 140.0f)
						spread = g_Local.spread.spreadvar * 0.07 + 0.035;
					else
						spread = g_Local.spread.spreadvar * 0.025;
				}
			}
			else
				spread = g_Local.spread.spreadvar * 0.4 + .035;

			break;
		case WEAPONLIST_MP5:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.2 * g_Local.spread.spreadvar;
			break;
		case WEAPONLIST_MAC10:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.375 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_P90:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed > 170.0f)
					spread = g_Local.spread.spreadvar * 0.115;
				else
					spread = g_Local.spread.spreadvar * 0.045;
			}
			else
				spread = g_Local.spread.spreadvar * 0.3;

			break;
		case WEAPONLIST_TMP:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.03 * g_Local.spread.spreadvar;
			else
				spread = 0.25 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_UMP45:
			if (g_Local.spread.prcflags & FL_ONGROUND)
				spread = 0.04 * g_Local.spread.spreadvar;
			else
				spread = 0.24 * g_Local.spread.spreadvar;

			break;
		case WEAPONLIST_AWP:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 10.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.001f;
				}
				else if (speed < 140.0f)
				{
					spread = 0.1f;
				}
				else
					spread = 0.25f;
			}
			else
				spread = 0.85f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.08f;

			break;
		case WEAPONLIST_SCOUT:
			if (g_Local.spread.prcflags & FL_ONGROUND)
			{
				if (speed < 170.0f)
				{
					if (g_Local.spread.prcflags & FL_DUCKING)
						spread = 0;
					else
						spread = 0.007f;
				}
				else
					spread = 0.075f;
			}
			else
				spread = 0.2f;

			if (!(g_Local.iFOV < 90.0f))
				spread += 0.025f;

			break;
		default:
			spread = 0;
			break;
		}
	}

	return spread;
}

void cNoSpread::GetSpreadXY(unsigned int seed, int future, float *velocity, float *vec)
{
	float vecspread, speed;

	vec[0] = UTIL_SharedRandomFloat(seed + future, -0.5, 0.5) + UTIL_SharedRandomFloat(seed + 1 + future, -0.5, 0.5);
	vec[1] = UTIL_SharedRandomFloat(seed + 2 + future, -0.5, 0.5) + UTIL_SharedRandomFloat(seed + 3 + future, -0.5, 0.5);

	speed = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]);

	vecspread = GetVecSpread(speed);

	vec[0] *= vecspread;
	vec[1] *= vecspread;
}

void cNoSpread::GetSpreadOffset(unsigned int seed, int future, float *inangles, float *velocity, float *outangles)
{
	float forward[3], right[3], up[3], vecDir[3];
	float view[3], dest[3], spread[2];

	g_Engine.pfnAngleVectors(inangles, forward, right, up);

	GetSpreadXY(seed, future, velocity, spread);

	vecDir[0] = forward[0] + spread[0] * right[0] + spread[1] * up[0];
	view[0] = 8192 * vecDir[0];

	vecDir[1] = forward[1] + spread[0] * right[1] + spread[1] * up[1];
	view[1] = 8192 * vecDir[1];

	vecDir[2] = forward[2] + spread[0] * right[2] + spread[1] * up[2];
	view[2] = 8192 * vecDir[2];

	VectorAngles(view, dest);
	dest[0] *= -1;

	outangles[0] = inangles[0] - dest[0];
	outangles[1] = inangles[1] - dest[1];
	outangles[2] = 0;
}

void cNoSpread::VectorAngles(const float *forward, float *angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		pitch = 0;
	}
	else
	{
		yaw = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);

		pitch = atan2(forward[2], tmp) * 180 / M_PI;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void cNoSpread::DefaultSpreadVar(int weaponid)
{
	if (weaponid == WEAPONLIST_DEAGLE)
		g_Local.spread.spreadvar = 0.9f;
	else if (weaponid == WEAPONLIST_MP5)
		g_Local.spread.spreadvar = 0.0f;
	else if (weaponid == WEAPONLIST_AK47)
		g_Local.spread.spreadvar = 0.2f;
	else if (weaponid == WEAPONLIST_SG552)
		g_Local.spread.spreadvar = 0.2f;
	else if (weaponid == WEAPONLIST_AUG)
		g_Local.spread.spreadvar = 0.3f;
	else if (weaponid == WEAPONLIST_M4A1)
		g_Local.spread.spreadvar = 0.2f;
	else if (weaponid == WEAPONLIST_M249)
		g_Local.spread.spreadvar = 0.2f;
	else if (weaponid == WEAPONLIST_MAC10)
		g_Local.spread.spreadvar = 0.15f;
	else if (weaponid == WEAPONLIST_UMP45)
		g_Local.spread.spreadvar = 0;
	else if (weaponid == WEAPONLIST_TMP)
		g_Local.spread.spreadvar = 0.2f;
	else if (weaponid == WEAPONLIST_P90)
		g_Local.spread.spreadvar = 0.15f;
	else if (weaponid == WEAPONLIST_P228)
		g_Local.spread.spreadvar = 0.9f;
	else if (weaponid == WEAPONLIST_FIVESEVEN)
		g_Local.spread.spreadvar = 0.92f;
	else if (weaponid == WEAPONLIST_ELITE)
		g_Local.spread.spreadvar = 0.88f;
	else if (weaponid == WEAPONLIST_GLOCK18)
		g_Local.spread.spreadvar = 0.9f;
	else if (weaponid == WEAPONLIST_USP)
		g_Local.spread.spreadvar = 0.92f;
	else if (weaponid == WEAPONLIST_G3SG1)
		g_Local.spread.spreadvar = 0.2f;
	else if (weaponid == WEAPONLIST_SG550)
		g_Local.spread.spreadvar = 0.2f;
	else
		g_Local.spread.spreadvar = 0;
}

void cNoSpread::HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed)
{
	static int prevammo = 0, curammo;
	int i, index, Id;
	const char *command;

	if(runfuncs)
	{
		g_Local.spread.random_seed = random_seed;

		g_Local.spread.gtime = time;

		g_Local.spread.prcflags = to->client.flags;

		if (cmd->buttons & IN_ATTACK && CanCurWeaponAttack()) // catch case when pistol and IN_ATTACK is always on and not firing
		{
			PrimaryAttack();
		}
		else if (!(cmd->buttons & (IN_ATTACK | IN_ATTACK2)))
		{
			if (g_Local.spread.firing)
			{
				g_Local.spread.firing = false;

				if (g_Local.spread.recoil > 15)
					g_Local.spread.recoil = 15;

				g_Local.spread.recoiltime = time + 0.4;
			}

			if (IsCurWeaponSec())
			{
				g_Local.spread.recoil = 0;
			}
			else if (g_Local.spread.recoil > 0)
			{
				if (g_Local.spread.recoiltime <= time)
				{
					g_Local.spread.recoiltime = g_Local.spread.recoiltime + 0.0225;
					g_Local.spread.recoil--;
				}
			}
		}

		for (i = 0; i < WeaponListStruct.size(); i++)
		{
			if (WeaponListStruct[i].Id == to->client.m_iId)
			{
				if (!WeaponListStruct[i].CurWeapon) // FIX: This doesn't catch when you have a weapon and you buy the same weapon
				{
					prevammo = 0;

					g_Local.spread.recoil = 0;
					g_Local.spread.prevtime = 0;
					DefaultSpreadVar(WeaponListStruct[i].Id);
					g_Local.spread.recoiltime = time;
					g_Local.spread.firing = true;
				}

				WeaponListStruct[i].CurWeapon = true;
				g_Local.weapon = GetCurWeapon();
			}
			else
				WeaponListStruct[i].CurWeapon = false;
		}

		for (i=0;i<MAX_WEAPONS;i++)
		{
			// this assumes (i == wd[i].m_iId)
			index = GetWeaponIndexByID(i);
			if (index == -1)
				continue;

			if (to->weapondata[i].m_iId)
				memcpy(&WeaponListStruct[index].weapondata, &to->weapondata[i], sizeof(weapon_data_t));
			else
				memset(&WeaponListStruct[index].weapondata, 0, sizeof(weapon_data_t));
		}

		if (IsReloading())
		{
			Id = GetCurWeaponId();

			g_Local.spread.recoil = 0;
			g_Local.spread.prevtime = 0;
			DefaultSpreadVar(Id);
			g_Local.spread.recoiltime = time;
			g_Local.spread.firing = false;
		}

		curammo = GetCurWeaponAmmo();
		prevammo = curammo;
	}
}

void cNoSpread::PrimaryAttack(void)
{
	int id;

	id = GetCurWeaponId();

	switch (id)
	{
	case WEAPONLIST_DEAGLE:
		if (g_Local.spread.prevtime)
		{
			g_Local.spread.spreadvar = g_Local.spread.spreadvar - (0.35 * (0.4 - (g_Local.spread.gtime - g_Local.spread.prevtime)));

			if (g_Local.spread.spreadvar > 0.9)
				g_Local.spread.spreadvar = 0.9f;
			else if (g_Local.spread.spreadvar < 0.55)
				g_Local.spread.spreadvar = 0.55f;
		}

		g_Local.spread.recoil++;
		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_ELITE:
		if (g_Local.spread.prevtime)
		{
			g_Local.spread.spreadvar = g_Local.spread.spreadvar - (0.275 * (0.325 - (g_Local.spread.gtime - g_Local.spread.prevtime)));

			if (g_Local.spread.spreadvar > 0.88)
				g_Local.spread.spreadvar = 0.88f;
			else if (g_Local.spread.spreadvar < 0.55)
				g_Local.spread.spreadvar = 0.55f;
		}

		g_Local.spread.recoil++;
		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_GALIL:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = ((float)g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil) / 200.0f + 0.35f;

		if (g_Local.spread.spreadvar > 1.25)
			g_Local.spread.spreadvar = 1.25f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_FAMAS:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = ((float)g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil) / 215.0f + 0.3f;

		if (g_Local.spread.spreadvar > 1.0)
			g_Local.spread.spreadvar = 1.0f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_FIVESEVEN:
		if (g_Local.spread.prevtime)
		{
			g_Local.spread.spreadvar = g_Local.spread.spreadvar - (0.25 * (0.275 - (g_Local.spread.gtime - g_Local.spread.prevtime)));

			if (g_Local.spread.spreadvar > 0.92)
				g_Local.spread.spreadvar = 0.92f;
			else if (g_Local.spread.spreadvar < 0.725)
				g_Local.spread.spreadvar = 0.725f;
		}

		g_Local.spread.recoil++;
		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_GLOCK18:
		if (!IsCurWeaponInBurst())
			g_Local.spread.recoil++;

		if (g_Local.spread.prevtime)
		{
			g_Local.spread.spreadvar = g_Local.spread.spreadvar - (0.275 * (0.325 - (g_Local.spread.gtime - g_Local.spread.prevtime)));

			if (g_Local.spread.spreadvar > 0.9)
				g_Local.spread.spreadvar = 0.9f;
			else if (g_Local.spread.spreadvar < 0.6)
				g_Local.spread.spreadvar = 0.6f;
		}

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_P228:
		if (g_Local.spread.prevtime)
		{
			g_Local.spread.spreadvar = g_Local.spread.spreadvar - (0.3 * (0.325 - (g_Local.spread.gtime - g_Local.spread.prevtime)));

			if (g_Local.spread.spreadvar > 0.9)
				g_Local.spread.spreadvar = 0.9f;
			else if (g_Local.spread.spreadvar < 0.6)
				g_Local.spread.spreadvar = 0.6f;
		}

		g_Local.spread.recoil++;
		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_G3SG1:
		if (g_Local.spread.brokentime)
		{
			g_Local.spread.spreadvar = 0.55 + (0.3 * (g_Local.spread.gtime - g_Local.spread.brokentime));

			if (g_Local.spread.spreadvar > 0.98)
				g_Local.spread.spreadvar = 0.98f;
		}

		g_Local.spread.recoil++;
		g_Local.spread.brokentime = g_Local.spread.gtime;
		g_Local.spread.firing = true;

		break;
	case WEAPONLIST_SG550:
		if (g_Local.spread.brokentime)
		{
			g_Local.spread.spreadvar = 0.65 + (0.35 * (g_Local.spread.gtime - g_Local.spread.brokentime));

			if (g_Local.spread.spreadvar > 0.98)
				g_Local.spread.spreadvar = 0.98f;
		}

		g_Local.spread.recoil++;
		g_Local.spread.brokentime = g_Local.spread.gtime;
		g_Local.spread.firing = true;

		break;
	case WEAPONLIST_USP:
		if (g_Local.spread.prevtime)
		{
			g_Local.spread.spreadvar = g_Local.spread.spreadvar - (0.275 * (0.3 - (g_Local.spread.gtime - g_Local.spread.prevtime)));

			if (g_Local.spread.spreadvar > 0.92)
				g_Local.spread.spreadvar = 0.92f;
			else if (g_Local.spread.spreadvar < 0.6)
				g_Local.spread.spreadvar = 0.6f;
		}

		g_Local.spread.recoil++;
		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_AK47:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 200.0f + 0.35;

		if (g_Local.spread.spreadvar > 1.25)
			g_Local.spread.spreadvar = 1.25f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_SG552:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 220.0f + 0.3;

		if (g_Local.spread.spreadvar > 1)
			g_Local.spread.spreadvar = 1.0f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_AUG:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 215.0f + 0.3;

		if (g_Local.spread.spreadvar > 1.0)
			g_Local.spread.spreadvar = 1.0f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_M249:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 175.0f + 0.4;

		if (g_Local.spread.spreadvar > 0.9)
			g_Local.spread.spreadvar = 0.9f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_M4A1:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 220.0f + 0.3;

		if (g_Local.spread.spreadvar > 1)
			g_Local.spread.spreadvar = 1.0f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_MP5:
		g_Local.spread.recoil++;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 220.1f + 0.45; // CS 1.6 FIX

		if (g_Local.spread.spreadvar > 0.75)
			g_Local.spread.spreadvar = 0.75f;

		g_Local.spread.prevtime = g_Local.spread.gtime;
		g_Local.spread.firing = true;

		break;
	case WEAPONLIST_MAC10:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 200.0f + 0.6;

		if (g_Local.spread.spreadvar > 1.65)
			g_Local.spread.spreadvar = 1.65f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_P90:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil / 175.0f + 0.45;

		if (g_Local.spread.spreadvar > 1)
			g_Local.spread.spreadvar = 1.0f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_TMP:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil * g_Local.spread.recoil / 200.0f + 0.55;

		if (g_Local.spread.spreadvar > 1.4)
			g_Local.spread.spreadvar = 1.4f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_UMP45:
		g_Local.spread.recoil++;
		g_Local.spread.firing = true;

		g_Local.spread.spreadvar = g_Local.spread.recoil * g_Local.spread.recoil / 210.0f + 0.5;

		if (g_Local.spread.spreadvar > 1.0)
			g_Local.spread.spreadvar = 1.0f;

		g_Local.spread.prevtime = g_Local.spread.gtime;

		break;
	case WEAPONLIST_AWP:
		g_Local.spread.recoil++;
		g_Local.spread.prevtime = g_Local.spread.gtime;
		g_Local.spread.firing = true;

		break;
	case WEAPONLIST_SCOUT:
		g_Local.spread.recoil++;
		g_Local.spread.prevtime = g_Local.spread.gtime;
		g_Local.spread.firing = true;

		break;
	default:
		break;
	}

	return;
}

void cNoSpread::ApplySilentAngles(float *angles, usercmd_s *cmd, bool add_angle)
{
	cl_entity_t *pLocal;
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;
	float newforward, newright, newup;
	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;

	pLocal = g_Engine.GetLocalPlayer();
	if (!pLocal) return;

	if (pLocal->curstate.movetype == MOVETYPE_WALK){ g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), viewforward, viewright, viewup); }
	else{ g_Engine.pfnAngleVectors(cmd->viewangles, viewforward, viewright, viewup); }

	int iHasShiftHeld = GetAsyncKeyState(VK_LSHIFT);
	if (pLocal->curstate.movetype == MOVETYPE_WALK && !iHasShiftHeld && !(g_Local.ppmove->movetype == 5))
	{
		if (add_angle == false) {
			cmd->viewangles.x = angles[0];
			cmd->viewangles.y = angles[1];
		}
		else {
			cmd->viewangles.x += angles[0];
			cmd->viewangles.y += angles[1];
		}
	}

	if (pLocal->curstate.movetype == MOVETYPE_WALK){ g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), aimforward, aimright, aimup); }
	else { g_Engine.pfnAngleVectors(cmd->viewangles, aimforward, aimright, aimup); }

	newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
	newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
	newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);

	if (angles[0]>81){ cmd->forwardmove = -newforward; }
	else { cmd->forwardmove = newforward; }

	cmd->sidemove = newright;
	cmd->upmove = newup;
}

void cNoSpread::CL_CreateMove(usercmd_s *cmd)
{
	float offset[3]={};
	
	if(!g_Local.bBadWpn)
		gNoSpread.GetSpreadOffset(g_Local.spread.random_seed, 2, cmd->viewangles, g_Local.ppmove->velocity, offset);
	
	VectorCopy(offset, g_Local.vNoSpreadAng);

	/*if(g_Settings.NoSpread.active > 0 && (g_Settings.NoSpread.in_attack ? cmd->buttons&IN_ATTACK : true))
	{
		cmd->viewangles[0] += offset[0];
		cmd->viewangles[1] += offset[1];
		cmd->viewangles[2] += offset[2];
	}*/

}

void cNoSpread::DrawSpread()
{
	Vector spreadangles,forward,eyepos,screen;

	if(!g_Local.bBadWpn && g_Local.bIsAlive)
	{
		pmtrace_t tr;

		VectorAdd(g_Local.cmdViewAngles, g_Local.vNoSpreadAng, spreadangles);

		g_Engine.pfnAngleVectors(spreadangles, forward, NULL, NULL);

		g_Engine.pEventAPI->EV_SetTraceHull(2);
		eyepos = g_Local.vEye;
		g_Engine.pEventAPI->EV_PlayerTrace(eyepos, eyepos + (forward * /*playerItems.CurDistance()*/8192),PM_GLASS_IGNORE, -1, &tr);
	
		if (g_Cheat.WorldToScreen(tr.endpos, g_Local.vDrawSpr))
		{
			g_Local.vDrawSpr.x = g_Screen.iWidth - g_Local.vDrawSpr.x;
			g_Local.vDrawSpr.y = g_Screen.iHeight - g_Local.vDrawSpr.y;
			//g_Drawing.FillArea(g_Local.vDrawSpr.x - 1, g_Local.vDrawSpr.y - 1, 3, 3, 0, 255, 0, 255);
		}
	}
}