#include "Syringe.h"

DEFINE_HOOK(0xc, CD_AlwaysFindYR, 6)
{
	R->EAX(2);
	return 0x4A8265;
}

DEFINE_HOOK(0x4790E0, CD_AlwaysAvailable, 7)
{
	R->AL(1);
	return 0x479109;
}

DEFINE_HOOK(0x479110, CD_NeverAsk, 5)
{
	R->AL(1);
	return 0x4791EA;
}

/*
Implemented in Launch_Impl.cpp

DEFINE_HOOK(0x49F5C0, CopyProtection_IsLauncherRunning, 8)
{
	R->AL(1);
	return 0x49F61A;
}

*/

/*
Implemented in Launch_Impl.cpp

DEFINE_HOOK(0x49F620, CopyProtection_NotifyLauncher, 5)
{
	R->AL(1);
	return 0x49F733;
}

*/

/*
Implemented in Launch_Impl.cpp

DEFINE_HOOK(0x49F7A0, CopyProtection_CheckProtectedData, 8)
{
	R->AL(1);
	return 0x49F8A7;
}

*/

// this douchebag blows your base up when it thinks you're cheating
DEFINE_HOOK(0x55CFDF, BlowMeUp, 0)
{
	return 0x55D059;
}

