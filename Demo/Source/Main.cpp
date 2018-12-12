#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <Engine/Window.h>
#include <iostream>

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	afex::Window* window = afex::Window::Create(1280, 720, "test");
	while (window->Update()) {
		// do things
	}
	delete window;
    return 0;
}