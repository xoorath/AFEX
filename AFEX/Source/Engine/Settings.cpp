#include <Engine/Settings.h>
#include <json/json.hpp>
#include <fstream>
#include <iomanip>


namespace afex {
Settings settings;

using nlohmann::json;
using std::ifstream;
using std::ofstream;

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
		},
		{
			"fonts", {
				{
					"mono", s.Fonts.mono
				},
				{
					"sans", s.Fonts.sans
				},
				{
					"serif", s.Fonts.serif
				},
			}
		}
	};
}

void from_json(const json& j, Settings& s) {
	auto windowSettings = j.at("window");
	windowSettings.at("title").get_to(s.Window.title);
	windowSettings.at("width").get_to(s.Window.width);
	windowSettings.at("height").get_to(s.Window.height);
	auto fontSettings = j.at("fonts");
	fontSettings.at("mono").get_to(s.Fonts.mono);
	fontSettings.at("sans").get_to(s.Fonts.sans);
	fontSettings.at("serif").get_to(s.Fonts.serif);
}

void Settings::Load() {
	ifstream fs("./app-config.json");
	if (fs.is_open()) {
		json doc;
		fs >> doc;
		*this = doc.get<Settings>();
	}
}
void Settings::Save() {
	ofstream fs("./app-config.json");
	if (fs.is_open()) {
		json doc = *this;
		fs << std::setw(4) << doc;
	}
}

}