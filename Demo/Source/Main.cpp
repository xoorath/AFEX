#include <Engine/Window.h>
#include <Engine/Log.h>
#include <Engine/Settings.h>
#include <Engine/DebugUI.h>

#include <Windows.h>

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	::afex::log::Init();
	::afex::settings::Load();

	afex::Window* window = afex::Window::Create(
		::afex::settings::window::GetWidth(), 
		::afex::settings::window::GetHeight(), 
		::afex::settings::window::GetTitle());
	
	while (window->Update()) {
	}
	::afex::settings::Save();
	delete window;
    return 0;
}