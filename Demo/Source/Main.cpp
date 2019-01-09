#include <Engine/Window.h>
#include <Engine/Log.h>
#include <Engine/Settings.h>
#include <Engine/DebugUI.h>
#include <Engine/HashName.h>
#include <Windows.h>

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	::afex::log::Init();
	::afex::g_Settings.Load();

	auto const& windowSettings = ::afex::g_Settings.Window;
	::afex::Window* window = ::afex::Window::Create(windowSettings.width, windowSettings.height, windowSettings.title);

	while (window->Update()) {
	}
	::afex::g_Settings.Save();
	delete window;
    return 0;
}