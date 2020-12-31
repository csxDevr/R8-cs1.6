#pragma once
#include "Main.h"

class cNoSpread
{
	public:
		void GetSpreadOffset(unsigned int seed, int future, float *inangles, float *velocity, float *outangles);
		void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
		void GetRecoilOffset(unsigned int seed, int future, float *inangles, float *velocity, float *outangles);
		void CL_CreateMove(usercmd_s *cmd);
		void DefaultSpreadVar(int weaponid);
		void DrawSpread();
		void ApplySilentAngles(float *angles, usercmd_s *cmd, bool add_angle = false);
	private:
		unsigned int U_Random( void );
		float GetVecSpread(float speed);
		void PrimaryAttack(void);
		void VectorAngles(const float *forward, float *angles);
		void U_Srand( unsigned int seed );
		void GetSpreadXY(unsigned int seed, int future, float *velocity, float *vec);
		int UTIL_SharedRandomLong( unsigned int seed, int low, int high );
		float UTIL_SharedRandomFloat( unsigned int seed, float low, float high );
};
extern cNoSpread gNoSpread;
