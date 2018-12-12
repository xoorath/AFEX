#pragma once

#include <Engine/Types.h>

namespace afex {
namespace settings {
void Load();
void Save();
namespace window {
string GetTitle();
u32 GetWidth();
u32 GetHeight();

void SetTitle(string title);
void SetWidth(u32 width);
void SetHeight(u32 height);
}
}
}
