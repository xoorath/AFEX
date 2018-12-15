#pragma once

#include <Engine/Types.h>
#include <vector>

namespace afex {
static struct Settings{
	struct {
		string title;
		u32 width, height;
	} Window;
	struct {
		std::vector<string> mono;
		std::vector<string> sans;
		std::vector<string> serif;
	} Fonts;
	void Load();
	void Save();
} g_Settings;
}
