#include "shared.h"
#include "Commctrl.h"
#include "ShlObj.h"
#include "Shlwapi.h"
#include "Shellapi.h"


#include "libs/imgui/imgui.h"
#include "libs/imgui/imgui_impl_opengl2.h"
#include "libs/imgui/imgui_impl_win32.h"




static int(__stdcall *entryPoint)(HINSTANCE, HINSTANCE, LPSTR, int) = (int(__stdcall*)(HINSTANCE, HINSTANCE, LPSTR, int))0x4640B0;

char sys_cmdline[MAX_STRING_CHARS];

extern "C" bool bClosing = false;

extern HMODULE hModule;
HINSTANCE hInst;
#ifdef DEBUG
HMODULE hLogFile;
#endif

void Sys_Unload()
{
	bClosing = true;
	static bool unloaded = false;

	if (unloaded)
		return;
	unloaded = true;

#ifdef DEBUG
	_CrtDumpMemoryLeaks();
	CloseHandle(hLogFile);
#endif
}





// Forward declare message handler from imgui_impl_win32.cpp
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK h_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		//return true;


	switch (uMsg)
	{
	case WM_CREATE:
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_MINIMIZEBOX);
		break;
	}

	LRESULT(CALLBACK * o_WndProc)(HWND, UINT, WPARAM, LPARAM);
	*(int*)&o_WndProc = 0x466BE0;
	return o_WndProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	strncpy(sys_cmdline, lpCmdLine, sizeof(sys_cmdline) - 1);

	void MSS32_Hook();
	MSS32_Hook();

	extern bool mss32_original_loaded;
	if (!mss32_original_loaded)
		return 0;

	return entryPoint(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}