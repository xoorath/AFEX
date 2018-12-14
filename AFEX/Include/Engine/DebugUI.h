#pragma once

namespace afex {
class Window;
class DebugUI {
	static DebugUI* Create(::afex::Window* window);
	~DebugUI();

	void Update();
private:
	DebugUI() = default;
	void* m_Pimpl;
};
}
