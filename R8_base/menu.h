#include "Client.h"

#define pb push_back
#define U VK_UP
#define D VK_DOWN
#define L VK_LEFT
#define R VK_RIGHT
#define B VK_BACK
#define E VK_RETURN
#define CKey GetAsyncKeyState

menu_s g_Menu;
//d3ddrawing_s g_d3dDrawing;
visuals_s g_Drawing;
bool bFirstFrame = true, b[107];
int nx = 15, ny = 35, saves = 0, lr_time = 0;
float STEP = 0.01f;

void write(char *filename, char *section, string key, float val) 
{
	if (key[0] == 'N' && key[1] == 'U')
		return;

	char dest[100];
	sprintf(dest, "%0.2f", val);
	
	WritePrivateProfileStringA(section, (char*)key.c_str(), dest, filename);
}

void writeBC(char *filename, char *section, string key, float val0, float val1)
{
	if (key[0] == 'N' && key[1] == 'U')
		return;

	char dest[100];
	sprintf(dest, "%0.2f,%0.2f", val0, val1);
	
	WritePrivateProfileStringA(section, (char*)key.c_str(), dest, filename);
}

void SaveSettingsForWeapons(char fname[]) 
{
	for (int i = 1; i <= 30; ++i) 
	{
		settings_s &s = g_Settings;
		string wpn = g_Cheat.GetWeaponNameByID(i);
		if (wpn == "NULL") continue;

		string	fov = wpn + "fov" ,
			smooth = wpn + "smooth" ,
			//rcs_delay = wpn + "recoil.delay" ,
			rcs_target = wpn + "recoil.target" ,
			rcs_fov = wpn + "recoil.fov" ,
			rcs_smooth = wpn + "recoil.smooth" ,
			rcs_dist = wpn + "recoil.distance" ,
			//time = wpn + "time" ,
			hitbox = wpn + "target" ,
			headonly = wpn + "headonly" ,
			delay = wpn + "delay";
				//automatic = wpn + "auto",
				//bullet_control = wpn + "bullet.control";

		write(fname, "aimbot", fov, s.aim.weapons[i].fov);
		write(fname, "aimbot", smooth, s.aim.weapons[i].smooth);
		//write(fname, "aimbot", rcs_delay, s.aim.weapons[i].norecoil_delay);
		write(fname, "aimbot", rcs_target, s.aim.weapons[i].norecoil_target);
		write(fname, "aimbot", rcs_fov, s.aim.weapons[i].norecoil_fov);
		write(fname, "aimbot", rcs_smooth, s.aim.weapons[i].norecoil_smooth);
		write(fname, "aimbot", rcs_dist, s.aim.weapons[i].norecoil_distance);
		//write(fname, "aimbot", time, s.aim.weapons[i].time);
		write(fname, "aimbot", hitbox, s.aim.weapons[i].hitbox);
		write(fname, "aimbot", delay, s.aim.weapons[i].predelay);
		//write(fname, "aimbot", automatic, s.aim.weapons[i].automatic);
		write(fname, "triggerbot", fov, s.trigger.weapons[i].fov);
		write(fname, "triggerbot", headonly, s.trigger.weapons[i].headonly);

		//writeBC(fname, "aimbot", bullet_control, s.aim.weapons[i].norecoil_delay, s.aim.weapons[i].time);
	}
}

void SaveSettings() 
{
	settings_s &s = g_Settings;
	write(g_Settings.szBaseDir, "aimbot", "aimbot.enabled", s.aim.enable);
	write(g_Settings.szBaseDir, "aimbot", "aimbot.deathmatch", s.aim.dm);
	write(g_Settings.szBaseDir, "aimbot", "aimbot.visible.check", s.aim.pwalls);
	write(g_Settings.szBaseDir, "aimbot", "aimbot.type", s.aim.type);
	write(g_Settings.szBaseDir, "aimbot", "aimbot.mode", s.aim.mode);
	write(g_Settings.szBaseDir, "aimbot", "aimbot.silent", s.aim.silent);
	write(g_Settings.szBaseDir, "aimbot", "aimbot.fov.distance.based", s.aim.fov_distbased);
	//write(g_Settings.szBaseDir, "aimbot", "aimbot.smooth.fov.based", s.aim.smooth_fov_based);

	write(g_Settings.szBaseDir, "triggerbot", "triggerbot.enabled", s.trigger.enable);
	write(g_Settings.szBaseDir, "triggerbot", "triggerbot.deathmatch", s.trigger.dm);
	write(g_Settings.szBaseDir, "triggerbot", "triggerbot.distance.min", s.trigger.dstmin);
	write(g_Settings.szBaseDir, "triggerbot", "triggerbot.distance.max", s.trigger.dstmax);
	write(g_Settings.szBaseDir, "triggerbot", "triggerbot.visible.check", s.trigger.pwalls);
	write(g_Settings.szBaseDir, "triggerbot", "triggerbot.key.mode", s.trigger.keymode);
	write(g_Settings.szBaseDir, "triggerbot", "triggerbot.onlyzoom", s.trigger.onlyzoom );


	write(g_Settings.szBaseDir, "esp", "esp.box", s.esp.box);
	write(g_Settings.szBaseDir, "esp", "esp.box.outline", s.esp.box_out);
	write(g_Settings.szBaseDir, "esp", "esp.box.width", s.esp.box_w);
	write(g_Settings.szBaseDir, "esp", "esp.box.height", s.esp.box_h);
	write(g_Settings.szBaseDir, "esp", "esp.far", s.esp._far);
	write(g_Settings.szBaseDir, "esp", "esp.aimbot.hitbox", s.esp.drawaimspot);
	write(g_Settings.szBaseDir, "esp", "esp.triggerbot.area", s.esp.drawtrigarea);
	write(g_Settings.szBaseDir, "esp", "esp.draw.fov", s.esp.drawfov);
	write(g_Settings.szBaseDir, "esp", "esp.weapon", s.esp.weapon);
	write(g_Settings.szBaseDir, "esp", "esp.sound", s.esp.sound);

	write(g_Settings.szBaseDir, "knifebot", "knifebot.enabled", s.kb.enable);
	write(g_Settings.szBaseDir, "knifebot", "knifebot.distance", s.kb.distance);

	write(g_Settings.szBaseDir, "misc", "misc.bhop", s.misc.bhop);
	write(g_Settings.szBaseDir, "misc", "misc.noflash", s.misc.noflash);
	write(g_Settings.szBaseDir, "misc", "misc.draw.punchangle", s.misc.punch);
	write(g_Settings.szBaseDir, "misc", "misc.fastzoom", s.misc.fastzoom);
	write(g_Settings.szBaseDir, "misc", "misc.lambert", s.misc.fastzoom);

	SaveSettingsForWeapons(g_Settings.szBaseDir);
}

void ResetValues() 
{
	for (int i = 0; i < 10; ++i)
		g_Menu.cOpen[i] = 0;
	ZeroMemory(g_Menu.iCur, sizeof(g_Menu.iCur));
	ZeroMemory(g_Menu.x, sizeof(g_Menu.x));
	ZeroMemory(g_Menu.y, sizeof(g_Menu.y));
	g_Menu.iLineHeight = 20;
	g_Menu.iLineWidth = 200;
	g_Menu.iDist = 3;
	g_Menu.iIdx = 0;
}

void ResetOpens(int k)
{
	for (int i = 0; i < 10; ++i)
		if (i != k)
			g_Menu.cOpen[i] = 0;
	g_Menu.iCur[1] = 0;
}

void EntryPush(const char *sName, float *pfValue, float fStep, float fMin, float fMax) 
{
	entry_s temp = { 0 };
	strcpy(temp.sName, sName);
	temp.pfValue = pfValue;
	temp.fMin = fMin;
	temp.fMax = fMax;
	temp.fStep = fStep;
	g_Menu.vEntrys[0].pb(temp);
}

void Entry2Push(const char *sName, float *pfValue, float fStep, float fMin, float fMax) 
{
	entry_s temp = { 0 };
	char dest[32] = { 0 };
	strcpy(temp.sName, sName);
	temp.pfValue = pfValue;
	temp.fMin = fMin;
	temp.fMax = fMax;
	temp.fStep = fStep;
	sprintf(dest, " %0.2f", *pfValue);
	strcpy(temp.sValue, dest);
	g_Menu.vEntrys[1].pb(temp);
}

void CreateEntrys() 
{
	g_Menu.vEntrys[0].clear();
	g_Menu.vEntrys[1].clear();

	int ind = 0;

	EntryPush("AimBot", &g_Menu.cOpen[ind], 1, 0, 1);
	if (g_Menu.cOpen[ind])
	{
		Entry2Push("aim.enabled", &g_Settings.aim.enable, 1, 0, 1);
		Entry2Push("aim.silent", &g_Settings.aim.silent, 1, 0, 1);
		Entry2Push("aim.type", &g_Settings.aim.type, 1, 1, 2);
		Entry2Push("aim.mode", &g_Settings.aim.mode, 1, 1, 2);
		Entry2Push("aim.deathmatch", &g_Settings.aim.dm, 1, 0, 1);
		Entry2Push("aim.visible.check", &g_Settings.aim.pwalls, 1, 0, 1);
		Entry2Push( "aim.fov.distance.based" , &g_Settings.aim.fov_distbased , STEP , 0 , 20 );
	}

	ind++;

	EntryPush("AimBot.Weapons", &g_Menu.cOpen[ind], 1, 0, 1);
	if (g_Menu.cOpen[ind])
	{
		Entry2Push("aim.fov", &g_Settings.aim.weapons[g_Local.iWpnID].fov, STEP, 0, 1000);
		Entry2Push("aim.smooth", &g_Settings.aim.weapons[g_Local.iWpnID].smooth, STEP, 0, 20);

		if (g_Settings.aim.type == 1)
			Entry2Push("aim.target", &g_Settings.aim.weapons[g_Local.iWpnID].hitbox, 1, 1, 11);
		else
			Entry2Push("aim.target", &g_Settings.aim.weapons[g_Local.iWpnID].hitbox, 1, 0, 51);

		//Entry2Push("aim.bullets", &g_Settings.aim.weapons[g_Local.iWpnID].time, 1, 0, 100);
		//Entry2Push("aim.recoil.bullets", &g_Settings.aim.weapons[g_Local.iWpnID].norecoil_delay, 1, 0, 100);

		Entry2Push("aim.recoil.distance", &g_Settings.aim.weapons[g_Local.iWpnID].norecoil_distance, STEP, 0.01f, 3000);	
		if (g_Settings.aim.type == 1)
			Entry2Push("aim.recoil.target", &g_Settings.aim.weapons[g_Local.iWpnID].norecoil_target, 1, 1, 11);
		else
			Entry2Push("aim.recoil.target", &g_Settings.aim.weapons[g_Local.iWpnID].norecoil_target, 1, 0, 51);

		Entry2Push("aim.recoil.fov", &g_Settings.aim.weapons[g_Local.iWpnID].norecoil_fov, STEP, 0, 1000);
		Entry2Push("aim.recoil.smooth", &g_Settings.aim.weapons[g_Local.iWpnID].norecoil_smooth, STEP, 0, 20);
		Entry2Push("aim.delay", &g_Settings.aim.weapons[g_Local.iWpnID].predelay, 1, 0, 1000);
		//Entry2Push("aim.auto", &g_Settings.aim.weapons[g_Local.iWpnID].automatic, 1, 0, 1);
	}

	ind++;

	EntryPush("TriggerBot", &g_Menu.cOpen[ind], 1, 0, 1);
	if (g_Menu.cOpen[ind])
	{
		Entry2Push("trigger.enabled", &g_Settings.trigger.enable, 1, 0, 1);
		Entry2Push("trigger.deathmatch", &g_Settings.trigger.dm, 1, 0, 1);
		Entry2Push("trigger.distance.min", &g_Settings.trigger.dstmin, 5, 0, 5000);
		Entry2Push("trigger.distance.max", &g_Settings.trigger.dstmax, 5, 0, 5000);
		Entry2Push("trigger.visible.check", &g_Settings.trigger.pwalls, 1, 0, 1);
		Entry2Push("trigger.key.mode", &g_Settings.trigger.keymode, 1, 1, 2);
		Entry2Push("trigger.onlyzoom", &g_Settings.trigger.onlyzoom , 1, 0, 1);
	}

	ind++;

	EntryPush("TriggerBot.Weapons", &g_Menu.cOpen[ind], 1, 0, 1);
	if (g_Menu.cOpen[ind])
	{
		Entry2Push("trigger.fov", &g_Settings.trigger.weapons[g_Local.iWpnID].fov, STEP, 0, 360);
		Entry2Push("trigger.headonly", &g_Settings.trigger.weapons[g_Local.iWpnID].headonly, 1, 0, 1);
	}

	ind++;

	if ( AIM_VERSION == 0 )
	{
		EntryPush( "Visuals" , &g_Menu.cOpen[ind] , 1 , 0 , 1 );
		if ( g_Menu.cOpen[ind] )
		{
			Entry2Push( "esp.box" , &g_Settings.esp.box , 1 , 0 , 1 );
			Entry2Push( "esp.box.outline" , &g_Settings.esp.box_out , 1 , 0 , 1 );
			Entry2Push( "esp.box.width" , &g_Settings.esp.box_w , 1 , 1 , 50 );
			Entry2Push( "esp.box.height" , &g_Settings.esp.box_h , 1 , 1 , 50 );
			Entry2Push( "esp.weapon" , &g_Settings.esp.weapon , 1 , 0 , 1 );
			Entry2Push( "esp.far" , &g_Settings.esp._far , 1 , 0 , 1 );
			Entry2Push( "esp.sound" , &g_Settings.esp.sound , 1 , 0 , 1 );
			Entry2Push( "esp.aimbot.hitbox" , &g_Settings.esp.drawaimspot , 1 , 0 , 1 );
			Entry2Push( "esp.triggerbot.area" , &g_Settings.esp.drawtrigarea , 1 , 0 , 1 );
			Entry2Push( "esp.draw.fov" , &g_Settings.esp.drawfov , 1 , 0 , 1 );
		}

		ind++;
	}

	EntryPush("KnifeBot", &g_Menu.cOpen[ind], 1, 0, 1);
	if (g_Menu.cOpen[ind])
	{
		Entry2Push( "knifebot.enabled" , &g_Settings.kb.enable , 1 , 0 , 1 );
		Entry2Push( "knifebot.distance" , &g_Settings.kb.distance , STEP , 0 , 100 );
	}

	ind++;

	EntryPush("Misc", &g_Menu.cOpen[ind], 1, 0, 1);
	if (g_Menu.cOpen[ind])
	{
		Entry2Push( "misc.bhop" , &g_Settings.misc.bhop , 1 , 0 , 1 );
		Entry2Push( "misc.fastzoom" , &g_Settings.misc.fastzoom , 1 , 0 , 1 );

		if ( AIM_VERSION == 0 )
		{
			Entry2Push( "misc.noflash" , &g_Settings.misc.noflash , 1 , 0 , 255 );
			Entry2Push( "misc.draw.punchangle" , &g_Settings.misc.punch , 1 , 0 , 1 );
			Entry2Push( "misc.lambert" , &g_Settings.misc.lambert , 1 , 0 , 1 );
		}
	}

	ind++;

	EntryPush("Save Settings", &g_Menu.cSave, 1, 0, 1);
}

bool CheckForPress(int x, int y, int x1, int y1, int w, int h) 
{
	if (CKey(VK_LBUTTON)) 
		if (x >= x1 && x <= x1 + w && y >= y1 && y <= y1 + h)
			return true;
	return false;
}

void KeyEvent() 
{
	int idx = g_Menu.iIdx;

	if (CKey(U) & 1) 
	{
		saves = 0;
		if (g_Menu.iCur[idx] > 0)
			g_Menu.iCur[idx]--;
		else
			g_Menu.iCur[idx] = g_Menu.vEntrys[idx].size() - 1;
		return;
	}

	if (CKey(D) & 1) 
	{
		saves = 0;
		if (g_Menu.iCur[idx] < g_Menu.vEntrys[idx].size() - 1)
			g_Menu.iCur[idx]++;
		else
			g_Menu.iCur[idx] = 0;		
		return;
	}	

	int i = g_Menu.iCur[idx];
	if (CKey(E) & 1) 
	{		
		if (g_Menu.iCur[0] == g_Menu.vEntrys[0].size() - 1) 	
			SaveSettings();
		else {
			ResetOpens(g_Menu.iCur[idx]);
			g_Menu.iIdx = 1 - g_Menu.iIdx;
			g_Menu.cOpen[g_Menu.iCur[0]] = (float)g_Menu.iIdx;
		}
		return;
	}

	if ((double)lr_time / 10 == 1.0f) 
	{
		STEP *= 2;
		if (STEP > 10.0f)
			STEP = 10.0f;
		lr_time = 1;
	}

	bool LR = false;

	if (CKey(R) & 1 && g_Menu.iIdx > 0) 
	{
		lr_time++;
		if (g_Menu.vEntrys[idx][i].pfValue != NULL) 
		{
			*g_Menu.vEntrys[idx][i].pfValue += g_Menu.vEntrys[idx][i].fStep;
			if (*g_Menu.vEntrys[idx][i].pfValue > g_Menu.vEntrys[idx][i].fMax)
				*g_Menu.vEntrys[idx][i].pfValue = g_Menu.vEntrys[idx][i].fMin;
			return;
		}
		LR = true;
	}

	if (CKey(L) & 1 && g_Menu.iIdx > 0) 
	{
		lr_time++;
		if (g_Menu.vEntrys[idx][i].pfValue != NULL) 
		{
			*g_Menu.vEntrys[idx][i].pfValue -= g_Menu.vEntrys[idx][i].fStep;
			if (*g_Menu.vEntrys[idx][i].pfValue < g_Menu.vEntrys[idx][i].fMin)
				*g_Menu.vEntrys[idx][i].pfValue = g_Menu.vEntrys[idx][i].fMax;
			return;
		}
		LR = true;
	}

	if (!GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT))
		STEP = 0.01f;
}

void DrawMenu() 
{
	if (bFirstFrame)
		ResetValues(),
		bFirstFrame = false;

	CreateEntrys();
	KeyEvent();

	g_Menu.iLineHeight = 15;
	g_Menu.iLineWidth = 200;
	g_Menu.iDist = 1;
	g_Menu.x[0] = nx;
	g_Menu.y[0] = ny;
	g_Menu.x[1] = g_Menu.x[0] + g_Menu.iLineWidth + g_Menu.iDist;
	g_Menu.y[1] = g_Menu.y[0];
	int w = 210;

	int len = 70, i, fill_len = g_Menu.iLineWidth * (g_Menu.iIdx + 1) + g_Menu.iDist * g_Menu.iIdx;
	int v_x = (fill_len - len) / 2;
	g_Drawing.FillRGB(g_Menu.x[0], g_Menu.y[0] - (g_Menu.iLineHeight + 1), fill_len, g_Menu.iLineHeight, 0, 0, 0, 155);
	g_Drawing.DrawString(v_x + 1, g_Menu.y[0] - (g_Menu.iLineHeight + 1), w , 0, 0, "R8 coded by csxDevr");

//	       █████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████
//         ████████████████████████████████████████████████████████████████devr/null/██████████████████████god/mode/████████████████████████████████████
//         █░░░░░░░░░░░░░░░░███░░░░░░░░░░░░░░████░░░░░░░░░░░░░░███░░░░░░░░██░░░░░░░░████░░░░░░░░░░░░███░░░░░░░░░░░░░░█░░░░░░██░░░░░░█░░░░░░░░░░░░░░░░███
//         █░░▄▀▄▀▄▀▄▀▄▀▄▀░░███░░▄▀▄▀▄▀▄▀▄▀░░████░░▄▀▄▀▄▀▄▀▄▀░░███░░▄▀▄▀░░██░░▄▀▄▀░░████░░▄▀▄▀▄▀▄▀░░░░█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀░░██░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀▄▀░░███
//         █░░▄▀░░░░░░░░▄▀░░███░░▄▀░░░░░░▄▀░░████░░▄▀░░░░░░▄▀░░███░░░░▄▀░░██░░▄▀░░░░████░░▄▀░░░░▄▀▄▀░░█░░▄▀░░░░░░░░░░█░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░░░▄▀░░███
//         █░░▄▀░░████░░▄▀░░███░░▄▀░░██░░▄▀░░████░░▄▀░░██░░▄▀░░█████░░▄▀▄▀░░▄▀▄▀░░██████░░▄▀░░██░░▄▀░░█░░▄▀░░█████████░░▄▀░░██░░▄▀░░█░░▄▀░░████░░▄▀░░███
//         █░░▄▀░░░░░░░░▄▀░░███░░▄▀░░░░░░▄▀░░████░░▄▀░░░░░░▄▀░░░░███░░░░▄▀▄▀▄▀░░░░██████░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░░░░░█░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░░░▄▀░░███
//         █░░▄▀▄▀▄▀▄▀▄▀▄▀░░███░░▄▀▄▀▄▀▄▀▄▀░░████░░▄▀▄▀▄▀▄▀▄▀▄▀░░█████░░░░▄▀░░░░████████░░▄▀░░██░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀░░██░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀▄▀░░███
//         █░░▄▀░░░░░░▄▀░░░░███░░▄▀░░░░░░▄▀░░████░░▄▀░░░░░░░░▄▀░░███████░░▄▀░░██████████░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░░░░░█░░▄▀░░██░░▄▀░░█░░▄▀░░░░░░▄▀░░░░███
//         █░░▄▀░░██░░▄▀░░█████░░▄▀░░██░░▄▀░░████░░▄▀░░████░░▄▀░░███████░░▄▀░░██████████░░▄▀░░██░░▄▀░░█░░▄▀░░█████████░░▄▀▄▀░░▄▀▄▀░░█░░▄▀░░██░░▄▀░░█████
//         █░░▄▀░░██░░▄▀░░░░░░█░░▄▀░░░░░░▄▀░░████░░▄▀░░░░░░░░▄▀░░███████░░▄▀░░██████████░░▄▀░░░░▄▀▄▀░░█░░▄▀░░░░░░░░░░█░░░░▄▀▄▀▄▀░░░░█░░▄▀░░██░░▄▀░░░░░░█
//         █░░▄▀░░██░░▄▀▄▀▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░████░░▄▀▄▀▄▀▄▀▄▀▄▀░░███████░░▄▀░░██████████░░▄▀▄▀▄▀▄▀░░░░█░░▄▀▄▀▄▀▄▀▄▀░░███░░░░▄▀░░░░███░░▄▀░░██░░▄▀▄▀▄▀░░█
//         █░░░░░░██░░░░░░░░░░█░░░░░░░░░░░░░░████░░░░░░░░░░░░░░░░███████░░░░░░██████████░░░░░░░░░░░░███░░░░░░░░░░░░░░█████░░░░░░█████░░░░░░██░░░░░░░░░░█
//         █████████████████████████████████████████████getze.xyz/csx████████████supported█by█csxGodMode████████████████████████████████████████████████
//         █████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████

	for (int vec = 0; vec < 2; ++vec)
		for (i = 0; i < g_Menu.vEntrys[vec].size(); ++i) 
		{
			int x = g_Menu.x[vec], y = g_Menu.y[vec];
			g_Drawing.FillRGB(x, y + g_Menu.iLineHeight * i + g_Menu.iDist * i, g_Menu.iLineWidth, g_Menu.iLineHeight, 0, 0, 0, 155);
			if (i == g_Menu.iCur[vec])
				g_Drawing.FillRGB(x, y + g_Menu.iLineHeight * i + g_Menu.iDist * i, g_Menu.iLineWidth, g_Menu.iLineHeight, 255, 255, 255, 59);
			g_Drawing.DrawString(x + 5, y + g_Menu.iLineHeight * i + g_Menu.iDist * i, w , 0, 0, "%s",g_Menu.vEntrys[vec][i].sName);
			g_Drawing.DrawString(x + 140, y + g_Menu.iLineHeight * i + g_Menu.iDist * i, w , 0, 0,"%s", g_Menu.vEntrys[vec][i].sValue);
		}
}
