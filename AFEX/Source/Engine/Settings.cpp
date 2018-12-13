#include <Engine/Settings.h>
#include <json/json.hpp>
#include <fstream>
#include <iomanip>

afex::Settings settings;
namespace afex {

using nlohmann::json;

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

void Settings::Load() {
	::std::ifstream fs("./app-config.json");
	if (fs.is_open()) {
		json doc;
		fs >> doc;
		*this = doc.get<Settings>();
	}
}
void Settings::Save() {
	::std::ofstream fs("./app-config.json");
	if (fs.is_open()) {
		json doc = *this;
		fs << ::std::setw(4) << doc;
	}
}

}