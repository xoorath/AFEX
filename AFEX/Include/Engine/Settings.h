#pragma once

#include <Engine/Types.h>

namespace afex {
static struct Settings{
	struct {
		string title;
		u32 width, height;
	} Window;

	void Load();
	void Save();
} g_Settings;
}
