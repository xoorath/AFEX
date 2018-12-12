#include <Engine/Window.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <GLFW/glfw3.h>

namespace afex {
namespace internal {

bool glfwInitialized = false;
struct WindowImpl {
	GLFWwindow* m_GlfwWindow;
	string windowTitle;
	bool Init(u32 width, u32 height, string const& title) {
		windowTitle = title;
		m_GlfwWindow = glfwCreateWindow(numeric_cast<int>(width), numeric_cast<int>(height), windowTitle.c_str(), nullptr, nullptr);
		if (nullptr == m_GlfwWindow) {
			return false;
		}
		glfwMakeContextCurrent(m_GlfwWindow);
		return true;
	}

	bool Update() {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_GlfwWindow);
		glfwPollEvents();
		return false == glfwWindowShouldClose(m_GlfwWindow);
	}

	u32 GetWidth() const {
		if (m_GlfwWindow == nullptr) return 0;
		int width, height;
		glfwGetWindowSize(m_GlfwWindow, &width, &height);
		return numeric_cast<u32>(width);
	}

	void SetWidth(u32 width) {
		if (m_GlfwWindow == nullptr) return 0;
		int oldWidth, oldHeight;
		glfwGetWindowSize(m_GlfwWindow, &oldWidth, &oldHeight);
		glfwSetWindowSize(m_GlfwWindow, width, oldHeight);
	}

	u32 GetHeight() const {
		if (m_GlfwWindow == nullptr) return 0;
		int width, height;
		glfwGetWindowSize(m_GlfwWindow, &width, &height);
		return numeric_cast<u32>(height);
	}

	void SetHeight(u32 height) {
		if (m_GlfwWindow == nullptr) return 0;
		int oldWidth, oldHeight;
		glfwGetWindowSize(m_GlfwWindow, &oldWidth, &oldHeight);
		glfwSetWindowSize(m_GlfwWindow, oldWidth, height);
	}

	string GetTitle() const {
		return windowTitle;
	}

	void SetTitle(string const& title) {
		windowTitle = title;
		glfwSetWindowTitle(m_GlfwWindow, windowTitle.c_str());
	}
};

}

#define WINDOW_IMPL(x) reinterpret_cast<internal::WindowImpl*>(x->pimpl)

// static
Window* Window::Create(u32 width, u32 height, string const& title) {
	if (false == internal::glfwInitialized) {
		if (false == (internal::glfwInitialized = glfwInit())) {
			return nullptr;
		}
	}
	Window* w = new Window();
	w->pimpl = new internal::WindowImpl();
	if (false == WINDOW_IMPL(w)->Init(width, height, title)) {
		delete w;
		return nullptr;
	}
	return w;
}

Window::~Window() {
	if (pimpl) {
		delete WINDOW_IMPL(this);
		pimpl = nullptr;
		if (internal::glfwInitialized) {
			glfwTerminate();
			internal::glfwInitialized = false;
		}
	}
}

u32 Window::GetWidth() const {
	return WINDOW_IMPL(this)->GetWidth();
}

void Window::SetWidth(u32 width) {
	return WINDOW_IMPL(this)->SetWidth(width);
}

u32 Window::GetHeight() const {
	return WINDOW_IMPL(this)->GetHeight();
}

void Window::SetHeight(u32 height) {
	WINDOW_IMPL(this)->SetHeight(height);
}

string Window::GetTitle() const {
	return WINDOW_IMPL(this)->GetTitle();
}

void Window::SetTitle(const string& title) {
	WINDOW_IMPL(this)->SetTitle(title);
}

bool Window::Update() {
	return WINDOW_IMPL(this)->Update();
}

#undef WINDOW_IMPL
}