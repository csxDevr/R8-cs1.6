#include "main.h"
#include "PEB.h"

glReadPixels_t glReadPixels_s;

ofstream ofile;

void __stdcall m_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	if (g_AntiScreen.ScreenFirst )
	{
		g_AntiScreen.ScreenFirst = FALSE;
		g_AntiScreen.dwSize = ( width * height ) * 3;
		g_AntiScreen.BufferScreen = (PBYTE)malloc(g_AntiScreen.dwSize);
		glReadPixels_s(x, y, width, height, format, type, pixels);
		__try
		{
			if( AIM_VERSION == 0 )
			memcpy(g_AntiScreen.BufferScreen, pixels, g_AntiScreen.dwSize);
		}
		__except (EXCEPTION_EXECUTE_HANDLER){};
		g_AntiScreen.DrawVisuals = true;
		return;
	}
	__try
	{
		if ( AIM_VERSION == 0 )
		memcpy(pixels, g_AntiScreen.BufferScreen, g_AntiScreen.dwSize);
	}
	__except (EXCEPTION_EXECUTE_HANDLER){};
}

DWORD EATHook( HMODULE Module , PCHAR ExportedFunctionName , PVOID HookFunction , PVOID* OriginalAddress )
{
	DWORD EATA , OP;

	PIMAGE_DOS_HEADER DOSH = (PIMAGE_DOS_HEADER)Module;
	PIMAGE_NT_HEADERS NTH = NULL;

	if ( DOSH->e_magic != IMAGE_DOS_SIGNATURE ) return NULL;

	NTH = ( (PIMAGE_NT_HEADERS)( (DWORD)(DOSH)+(DWORD)( DOSH->e_lfanew ) ) );

	if ( NTH->Signature != IMAGE_NT_SIGNATURE ) return NULL;

	EATA = NTH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	PIMAGE_EXPORT_DIRECTORY EATP = (PIMAGE_EXPORT_DIRECTORY)( (DWORD)EATA + (DWORD)Module );

	for ( DWORD i = 0; i < EATP->NumberOfFunctions; i++ )
	{

		PDWORD ENTP = (PDWORD)( (DWORD)Module + ( (DWORD)EATP->AddressOfNames + ( sizeof( DWORD )*i ) ) );

		if ( strcmp( (PCHAR)( (DWORD)Module + *ENTP ) , ExportedFunctionName ) == 0 )
		{
			PWORD AONP = (PWORD)( (DWORD)Module + ( (DWORD)EATP->AddressOfNameOrdinals + ( i*sizeof( WORD ) ) ) );
			PDWORD AOF = (PDWORD)( (DWORD)Module + ( (DWORD)EATP->AddressOfFunctions + ( sizeof( DWORD )**AONP ) ) );

			if ( !VirtualProtect( AOF , sizeof( DWORD ) , PAGE_READWRITE , &OP ) ) return NULL;
			*OriginalAddress = (PVOID)( *AOF + DWORD( Module ) );
			*AOF = ( ( (DWORD)HookFunction ) - DWORD( Module ) );
			if ( !VirtualProtect( AOF , sizeof( DWORD ) , OP , &OP ) ) return NULL;
			return 1;
		}
	}
	return NULL;
}

void SetupHook()
{
	while ( !c_Offset.GetRendererInfo() )
		Sleep(100);

	g_pClient = (cl_clientfunc_t*)c_Offset.ClientFuncs();
	g_pEngine = (cl_enginefunc_t*)c_Offset.EngineFuncs();
	g_pStudio = (engine_studio_api_t*)c_Offset.StudioFuncs();

	if (PUBLIC_VERSION == 0)
	{
		HMODULE hmOpenGL = GetModuleHandleA("opengl32.dll");
		PreS_DynamicSound_s = (PreS_DynamicSound_t)DetourFunction((PBYTE)(DWORD)c_Offset.Sound(), (PBYTE)(DWORD)PreS_DynamicSound);
		
		//EATHook( hmOpenGL , "glReadPixels" , m_glReadPixels , ( (PVOID*)( &glReadPixels_s ) ) );
		
		glReadPixels_s = (glReadPixels_t)DetourFunction( (PBYTE)GetProcAddress( hmOpenGL , "glReadPixels" ) , (PBYTE)m_glReadPixels );
	}

	Hook:
	
	RtlCopyMemory(&g_Client, g_pClient, sizeof(cl_clientfunc_t));
	RtlCopyMemory(&g_Engine, g_pEngine, sizeof(cl_enginefunc_t));
	RtlCopyMemory(&g_Studio, g_pStudio, sizeof(engine_studio_api_t));

	if (!g_Client.V_CalcRefdef || !g_Engine.V_CalcShake || !g_Studio.StudioSetupSkin)
		goto Hook;

	HookFunction();
}

BOOL __declspec(dllexport) WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason) 
	{
	case DLL_PROCESS_ATTACH:
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			DisableThreadLibraryCalls(hinstDLL);

			GetModuleFileNameA(hinstDLL, BaseDir, sizeof(BaseDir));

			//char* pos = BaseDir + strlen(BaseDir);
			//while (pos >= BaseDir && *pos != '\\') --pos; pos[1] = 0;

			UnlinkModuleFromPEB(hinstDLL);

			if ( PUBLIC_VERSION )
			{
				ofile.open( "C:\\vermillion\\VermLOG.txt" , ios::app );
			}

			if (RemoveHeader(hinstDLL)) 
			{
				HANDLE hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetupHook, 0, 0, 0);
				HideThread(hThread);
			}
		}
		break;
	}
	return TRUE;
}