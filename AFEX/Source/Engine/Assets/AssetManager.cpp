#include <Engine/Assets/AssetManager.h>

namespace afex {
namespace assets {

/*static*/ AssetManager& AssetManager::Get() {
	static AssetManager s_AssetManager;
	return s_AssetManager;
}

}
}