#pragma once

namespace afex {
class Window;
class DebugUI {
public:
	static DebugUI* Create(Window* window);
	~DebugUI();

	void Update();
private:
	DebugUI() = default;
	void* m_Pimpl;
};
}
