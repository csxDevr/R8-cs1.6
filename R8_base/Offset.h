#ifndef __OFFSET_H__
#define __OFFSET_H__

#define OFF_SVC_MSG_PATTERN "\xBF\xFF\xFF\xFF\xFF\x8B\x04\xB5\xFF\xFF\xFF\xFF\x85\xC0\x74\xFF\x81\xFF\xFF\xFF\xFF\xFF\x7F\x04\x8B\x0F\xEB\x05"
#define OFF_SVC_MSG_MASK	"x????xxx????xxx?xx????xxxxxx"

class AutoOffset
{
private:
	DWORD HwBase, HwSize, HwEnd;
	DWORD ClBase, ClSize, ClEnd;
	DWORD UiBase, UiSize, UiEnd;
	DWORD HlBase, HlSize, HlEnd;

	BOOL __comparemem(const UCHAR *buff1, const UCHAR *buff2, UINT size);
	ULONG __findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size);
	ULONG __findreference(const ULONG start, const ULONG end, const ULONG address);

	DWORD FarProc(const DWORD Address, DWORD LB, DWORD HB);

public:
	BYTE HLType;

	bool GetRendererInfo();

	unsigned Absolute(DWORD Addr);
	void Error(const PCHAR Msg);
	DWORD GetModuleSize(const DWORD Address);
	DWORD MessagePtr(char Message[], BYTE size);

	bool CheckByte(DWORD Address, BYTE Value, int Offset);

	PVOID ClientFuncs();
	PVOID EngineFuncs();
	PVOID StudioFuncs();
	DWORD FindSVCMessages();
	void *Sound();

	DWORD FireBullets;

	PUserMsg FindUserMsgBase();	
};

extern AutoOffset c_Offset;

#endif