#include <Engine/Settings.h>
#include <json/json.hpp>
#include <fstream>
#include <iomanip>

namespace afex {
namespace settings {
using nlohmann::json;

struct Settings {
	struct {
		string title;
		u32 width, height;
	} Window;
};

void to_json(json& j, const Settings& s) {
	j = json 
	{ 
		{
			"window", {
				{ 
					"title", s.Window.title 
				},
				{ 
					"width", s.Window.width 
				},
				{ 
					"height", s.Window.height 
				}
			}
		} 
	};
}

void from_json(const json& j, Settings& s) {
	auto windowSettings = j.at("window");
	windowSettings.at("title").get_to(s.Window.title);
	windowSettings.at("width").get_to(s.Window.width);
	windowSettings.at("height").get_to(s.Window.height);
}

Settings g_Settings;

void Load() {
	::std::ifstream fs("./app-config.json");
	if (fs.is_open()) {
		json doc;
		fs >> doc;
		g_Settings = doc.get<Settings>();
	}
}
void Save() {
	::std::ofstream fs("./app-config.json");
	if (fs.is_open()) {
		json doc = g_Settings;
		fs << ::std::setw(4) << doc;
	}
}
namespace window {
string GetTitle() {
	return g_Settings.Window.title;
}

u32 GetWidth() {
	return g_Settings.Window.width;
}

u32 GetHeight() {
	return g_Settings.Window.height;
}

void SetTitle(string title) {
	g_Settings.Window.title = title;
}

void SetWidth(u32 width) {
	g_Settings.Window.width = width;
}

void SetHeight(u32 height) {
	g_Settings.Window.height = height;
}
}
}

}