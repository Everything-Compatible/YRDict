#include "Syringe.h"
#include "Version.h"
#include <Helpers/Macro.h>

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


DEFINE_HOOK(0x532017, DlgProc_MainMenu_Version, 5)
{
	GET(HWND, hWnd, ESI);

	// account for longer version numbers
	const int MinimumWidth = 168;

	RECT Rect;
	if (GetWindowRect(hWnd, &Rect)) {
		int width = Rect.right - Rect.left;

		if (width < MinimumWidth) {
			// extend to the left by the difference
			Rect.left -= (MinimumWidth - width);

			// if moved out of screen, move right by this amount
			if (Rect.left < 0) {
				Rect.right += -Rect.left;
				Rect.left = 0;
			}

			MoveWindow(hWnd, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, FALSE);
		}
	}

	return 0;
}

DEFINE_HOOK(0x52CA37, InitGame_Delay, 5)
{
	return 0x52CA65;
}
