#include "main.h"

#define CompareMemory(Buff1, Buff2, Size) __comparemem((const UCHAR *)Buff1, (const UCHAR *)Buff2, (UINT)Size)
#define FindMemoryClone(Start, End, Clone, Size) __findmemoryclone((const ULONG)Start, (const ULONG)End, (const ULONG)Clone, (UINT)Size)
#define FindReference(Start, End, Address) __findreference((const ULONG)Start, (const ULONG)End, (const ULONG)Address)

AutoOffset c_Offset;

typedef unsigned Cardinal;
typedef unsigned* PCardinal;
typedef unsigned** PPCardinal;

bool AutoOffset::GetRendererInfo()
{
	DWORD GameUI = (DWORD)GetModuleHandle("GameUI.dll");
	DWORD vgui = (DWORD)GetModuleHandle("vgui.dll");
	DWORD vgui2 = (DWORD)GetModuleHandle("vgui2.dll");
	DWORD d3dim = (DWORD)GetModuleHandle("d3dim.dll");

	HwBase = (DWORD)GetModuleHandle("hw.dll"); // Hardware

	if (HwBase == NULL)
	{
		HwBase = (DWORD)GetModuleHandle("sw.dll"); // Software
		if (HwBase == NULL)
		{
			HwBase = (DWORD)GetModuleHandle(NULL); // Non-Steam?
			if (HwBase == NULL) // Invalid module handle.
			{
				Error("Invalid module handle.");
			}
			else
				HLType = RENDERTYPE_UNDEFINED;
		}
		else
			HLType = RENDERTYPE_SOFTWARE;
	}
	else
	{
		if (d3dim == NULL)
			HLType = RENDERTYPE_HARDWARE;
		else
			HLType = RENDERTYPE_D3D;
	}

	HwSize = (DWORD)GetModuleSize(HwBase);

	if (HwSize == NULL)
	{
		switch (HwSize)
		{
		case RENDERTYPE_HARDWARE: HwSize = 0x122A000; break;
		case RENDERTYPE_UNDEFINED: HwSize = 0x2116000; break;
		case RENDERTYPE_SOFTWARE: HwSize = 0xB53000; break;
		default:Error("Invalid renderer type.");
		}
	}

	HwEnd = HwBase + HwSize - 1;

	ClBase = (DWORD)GetModuleHandle("client.dll");
	
	if (ClBase != NULL) {
		ClSize = (DWORD)GetModuleSize(ClBase);
		ClEnd = ClBase + ClSize - 1;
	}
	else {
		ClBase = HwBase;
		ClEnd = HwEnd;
		ClSize = HwSize;
	}

	if (GameUI != NULL)
	{
		UiBase = GameUI;
		UiSize = (DWORD)GetModuleSize(UiBase);
		UiEnd = UiBase + UiSize - 1;
	}

	HlBase = (DWORD)GetModuleHandle(NULL);
	HlSize = (DWORD)GetModuleSize(HlBase);
	HlEnd = HlBase + HlSize - 1;

	return (HwBase && ClBase && GameUI && vgui && vgui2 && HlBase);
}

unsigned AutoOffset::Absolute(DWORD Addr)
{
	return Cardinal(Addr) + *(PCardinal)(Addr)+4;
}

void AutoOffset::Error(const PCHAR Msg)
{
	MessageBoxA(0, Msg, "Fatal Error", MB_OK | MB_ICONERROR);
	ExitProcess(0);
}

DWORD AutoOffset::GetModuleSize(const DWORD Address)
{
	return PIMAGE_NT_HEADERS(Address + (DWORD)PIMAGE_DOS_HEADER(Address)->e_lfanew)->OptionalHeader.SizeOfImage;
}

DWORD AutoOffset::FarProc(const DWORD Address, DWORD LB, DWORD HB)
{
	return ((Address < LB) || (Address > HB));
}

BOOL AutoOffset::__comparemem(const UCHAR *buff1, const UCHAR *buff2, UINT size)
{
	for (UINT i = 0; i < size; i++, buff1++, buff2++)
	{
		if ((*buff1 != *buff2) && (*buff2 != 0xFF))
			return FALSE;
	}
	return TRUE;
}

ULONG AutoOffset::__findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size)
{
	for (ULONG ul = start; (ul + size) < end; ul++)
	{
		if (CompareMemory(ul, clone, size))
			return ul;
	}
	return NULL;
}

ULONG AutoOffset::__findreference(const ULONG start, const ULONG end, const ULONG address)
{
	UCHAR Pattern[5];
	Pattern[0] = 0x68;
	*(ULONG*)&Pattern[1] = address;
	return FindMemoryClone(start, end, Pattern, sizeof(Pattern)-1);
}

PVOID AutoOffset::ClientFuncs()
{
	PCHAR String = "ScreenFade";
	DWORD Address = (DWORD)FindMemoryClone(HwBase, HwEnd, String, strlen(String));
	PVOID ClientPtr = (PVOID)*(PDWORD)(FindReference(HwBase, HwEnd, Address) + 0x13); // all patch

	if (FarProc((DWORD)ClientPtr, HwBase, HwEnd))
		Error("Couldn't find ClientPtr pointer.");

	return ClientPtr;
}

PVOID AutoOffset::EngineFuncs()
{
	PVOID EnginePtr = (cl_enginefunc_t*)*(DWORD*)((DWORD)g_pClient->Initialize + 0x22); // old patch
	if (FarProc((DWORD)EnginePtr, HwBase, HwEnd) && FarProc((DWORD)EnginePtr, HlBase, HlEnd))
	{
		EnginePtr = (cl_enginefunc_t*)*(DWORD*)((DWORD)g_pClient->Initialize + 0x1C); // new patch
		if (FarProc((DWORD)EnginePtr, ClBase, ClEnd))
		{
			EnginePtr = (cl_enginefunc_t*)*(DWORD*)((DWORD)g_pClient->Initialize + 0x1D); // steam
			if (FarProc((DWORD)EnginePtr, ClBase, ClEnd))
			{
				EnginePtr = (cl_enginefunc_t*)*(DWORD*)((DWORD)g_pClient->Initialize + 0x37); // hl-steam
				if (FarProc((DWORD)EnginePtr, ClBase, ClEnd))
				{
					Error("Couldn't find EnginePtr pointer.");
				}
			}
		}
	}
	return EnginePtr;
}

PVOID AutoOffset::StudioFuncs()
{
	PVOID StudioPtr = (engine_studio_api_t*)*(DWORD*)((DWORD)g_pClient->HUD_GetStudioModelInterface + 0x30); // old patch
	if (FarProc((DWORD)StudioPtr, HwBase, HwEnd) && FarProc((DWORD)StudioPtr, HlBase, HlEnd))
	{
		StudioPtr = (engine_studio_api_t*)*(DWORD*)((DWORD)g_pClient->HUD_GetStudioModelInterface + 0x1A); // new patch / steam	

		if (FarProc((DWORD)StudioPtr, ClBase, ClEnd))
			Error("Couldn't find StudioPtr pointer.");
	}
	return StudioPtr;
}

PUserMsg AutoOffset::FindUserMsgBase()
{
	BYTE Pattern_UserMsg[8] =
	{
		0x52, 0x50, 0xE8, 0xFF, 0xFF, 0xFF, 0xFF, 0x83
	};

	BYTE Pattern_UserMsg2[12] =
	{
		0xFF, 0xFF, 0xFF, 0x0C,
		0x56, 0x8B, 0x35, 0xFF, 0xFF, 0xFF, 0xFF, 0x57
	};

	Cardinal Address = Absolute((Cardinal)FindMemoryClone((DWORD)g_Engine.pfnHookUserMsg, (DWORD)g_Engine.pfnHookUserMsg + 0x32,
		&Pattern_UserMsg, sizeof(Pattern_UserMsg)) + 3);

	if (FarProc(Address, HwBase, HwEnd))
		Error("Couldn't find UserMsgBase #1 pointer.");

	Address = (Cardinal)FindMemoryClone((DWORD)Address, (DWORD)Address + 0x32,
		&Pattern_UserMsg2, sizeof(Pattern_UserMsg2)) + 7;

	if (FarProc(Address, HwBase, HwEnd))
		Error("Couldn't find UserMsgBase #2 pointer.");

	return PUserMsg(**PPCardinal(Address));
}

DWORD AutoOffset::MessagePtr(char Message[], BYTE size)
{
	BYTE bPushAddrPattern[5] = { 0x68, 0x90, 0x90, 0x90, 0x90 };
	char* String = new char[size];
	strcpy(String, Message);
	DWORD Address = FindMemoryClone(HwBase, HwBase + HwSize, (PBYTE)String, size - 1);
	*(PDWORD)(bPushAddrPattern + 1) = Address;
	Address = FindMemoryClone(HwBase, HwBase + HwSize, (PBYTE)bPushAddrPattern, 5);
	return Address;
}

bool AutoOffset::CheckByte(DWORD Address, BYTE Value, int Offset)
{
	if (*(PBYTE)((DWORD)Address + Offset) == Value)
		return true;
	else
		return false;
}

DWORD FindPattern( PCHAR pattern , PCHAR mask , DWORD start , DWORD end , DWORD offset )
{
	int patternLength = lstrlenA( pattern );
	bool found = false;
	for ( DWORD i = start; i < end - patternLength; i++ )
	{
		found = true;
		for ( int idx = 0; idx < patternLength; idx++ )
		{
			if ( mask[idx] == 'x' && pattern[idx] != *(PCHAR)( i + idx ) )
			{
				found = false;
				break;
			}
		}
		if ( found )
		{
			return i + offset;
		}
	}
	return 0;
}

DWORD AutoOffset::FindSVCMessages()
{
	DWORD EngineMsgBase = FindPattern( OFF_SVC_MSG_PATTERN , OFF_SVC_MSG_MASK , HwBase , HwEnd , 1 );

	PEngineMsg pEngineMsgBase = (PEngineMsg)( *(PDWORD)EngineMsgBase - sizeof( DWORD ) );

	if ( pEngineMsgBase )
	{
		MSG_ReadByte = (HL_MSG_ReadByte)Absolute( ( (DWORD)pEngineMsgBase[32].pfn ) + 1 );
		MSG_ReadCount = *(PINT*)( (INT)(MSG_ReadByte)+1 );

		DWORD SVC_SoundBase = (DWORD)pEngineMsgBase[6].pfn;

		if ( *(PBYTE)( SVC_SoundBase + 0x0E ) == 0xE8 )
		{
			MSG_Buffer = (sizebuf_t *)( *(PDWORD)( SVC_SoundBase + 0x0A ) );
			MSG_StartBitReading = (HL_MSG_StartBitReading)Absolute( SVC_SoundBase + 0x0F );
			MSG_ReadBits = (HL_MSG_ReadBits)Absolute( SVC_SoundBase + 0x16 );
		}
		else if ( *(PBYTE)( SVC_SoundBase + 0x0C ) == 0xE8 )
		{
			MSG_Buffer = (sizebuf_t *)( *(PDWORD)( SVC_SoundBase + 0x08 ) );
			MSG_StartBitReading = (HL_MSG_StartBitReading)Absolute( SVC_SoundBase + 0x0D );
			MSG_ReadBits = (HL_MSG_ReadBits)Absolute( SVC_SoundBase + 0x14 );
		}

		if ( *(PBYTE)( SVC_SoundBase + 0xD6 ) == 0xE8 )
		{
			MSG_EndBitReading = (HL_MSG_EndBitReading)Absolute( SVC_SoundBase + 0xD7 );
			MSG_ReadBitVec3Coord = (HL_MSG_ReadBitVec3Coord)Absolute( SVC_SoundBase + 0xAF );
		}
		else if ( *(PBYTE)( SVC_SoundBase + 0xE2 ) == 0xE8 )
		{
			MSG_EndBitReading = (HL_MSG_EndBitReading)Absolute( SVC_SoundBase + 0xE3 );
			MSG_ReadBitVec3Coord = (HL_MSG_ReadBitVec3Coord)Absolute( SVC_SoundBase + 0xBE );
		}
	}

	return (DWORD)pEngineMsgBase;
}

void *AutoOffset::Sound()
{
	DWORD Addres = MessagePtr("S_StartDynamicSound: %s volume > 255", 37);

	for (int i = 1; i < 200; i++)
	{
		Addres--;
		if (CheckByte(Addres, 0x90, 0))
		{
			Addres++;
			if (CheckByte(Addres, 0x55, 0) || CheckByte(Addres, 0x83, 0))
			{
				return (void*)Addres;
			}
			else
				Error("Couldn't find PreS_DynamicSound #1 pointer.");
		}
	}
	Error("Couldn't find PreS_DynamicSound #2 pointer.");
	return 0;
}