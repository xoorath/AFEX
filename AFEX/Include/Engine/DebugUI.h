#pragma once

struct GLFWwindow;

namespace afex {
namespace debugui {
void Init(GLFWwindow* window);
void Update();
void Shutdown();
}
}