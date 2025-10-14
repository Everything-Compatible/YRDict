#include "Syringe.h"
#include "Version.h"

const char* DisplayStr = "YRDict " MARK_VERSION;
const char* InternalStr = "YRDict " PRODUCT_VERSION;


DEFINE_HOOK(0x74FAE0, GetModuleInternalVersion, 5)
{
	R->EAX<const char*>(DisplayStr);
	return 0x74FC7B;
}

DEFINE_HOOK(0x74FDC0, GetModuleVersion, 5)
{
	R->EAX<const char*>(InternalStr);
	return 0x74FEEF;
}

