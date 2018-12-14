#pragma once

#include <Engine/Types.h>

class GLFWwindow;
namespace afex {
class Window {
public:
	static Window * Create(u32 width, u32 height, string const& title);

	GLFWwindow* GetGLFWWindow() const;

	u32 GetWidth() const;
	void SetWidth(u32 width);

	u32 GetHeight() const;
	void SetHeight(u32 height);

	string GetTitle() const;
	void SetTitle(string const& title);

	// returns false when the window should close.
	bool Update();

	~Window();
private:
	Window() = default;
	void* m_Pimpl;
};
}