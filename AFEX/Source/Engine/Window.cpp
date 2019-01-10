#include <Engine/Window.h>
#include <Engine/DebugUI.h>

#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace afex {
namespace internal {

bool glfwInitialized = false;
class WindowImpl {

	GLFWwindow* m_GlfwWindow;
	DebugUI* m_DebugUI;
	string m_WindowTitle;
public:
	~WindowImpl() {
		delete m_DebugUI;
	}
	bool Init(Window* owner, u32 width, u32 height, string const& title) {
		m_WindowTitle = title;
		m_GlfwWindow = glfwCreateWindow(numeric_cast<int>(width), numeric_cast<int>(height), m_WindowTitle.c_str(), nullptr, nullptr);
		if (nullptr == m_GlfwWindow) {
			return false;
		}
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwMakeContextCurrent(m_GlfwWindow);

		if (false == gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			return false;
		}

		m_DebugUI = DebugUI::Create(owner);
		return true;
	}

	bool Update() {
		glClear(GL_COLOR_BUFFER_BIT);
		m_DebugUI->Update();
		glfwSwapBuffers(m_GlfwWindow);
		glfwPollEvents();
		return false == glfwWindowShouldClose(m_GlfwWindow);
	}

	GLFWwindow* GetGLFWWindow() const {
		return m_GlfwWindow;
	}

	u32 GetWidth() const {
		if (m_GlfwWindow == nullptr) return 0;
		int width, height;
		glfwGetWindowSize(m_GlfwWindow, &width, &height);
		return numeric_cast<u32>(width);
	}

	void SetWidth(u32 width) {
		if (m_GlfwWindow == nullptr) return;
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
		if (m_GlfwWindow == nullptr) return;
		int oldWidth, oldHeight;
		glfwGetWindowSize(m_GlfwWindow, &oldWidth, &oldHeight);
		glfwSetWindowSize(m_GlfwWindow, oldWidth, height);
	}

	string GetTitle() const {
		return m_WindowTitle;
	}

	void SetTitle(string const& title) {
		m_WindowTitle = title;
		glfwSetWindowTitle(m_GlfwWindow, m_WindowTitle.c_str());
	}
};

}

#define WINDOW_IMPL(x) reinterpret_cast<internal::WindowImpl*>(x->m_Pimpl)

// static
Window* Window::Create(u32 width, u32 height, string const& title) {
	if (false == internal::glfwInitialized) {
		if (false == (internal::glfwInitialized = glfwInit())) {
			return nullptr;
		}
	}
	Window* w = new Window();
	w->m_Pimpl = new internal::WindowImpl();
	if (false == WINDOW_IMPL(w)->Init(w, width, height, title)) {
		delete w;
		return nullptr;
	}
	return w;
}

Window::~Window() {
	if (m_Pimpl) {
		delete WINDOW_IMPL(this);
		m_Pimpl = nullptr;
		if (internal::glfwInitialized) {
			glfwTerminate();
			internal::glfwInitialized = false;
		}
	}
}

GLFWwindow* Window::GetGLFWWindow() const {
	return WINDOW_IMPL(this)->GetGLFWWindow();
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