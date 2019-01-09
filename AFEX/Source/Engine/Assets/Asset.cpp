#include <Engine/Assets/Asset.h>
#include <Engine/Assets/AssetManager.h>

namespace afex {
namespace assets {
void Asset::LoadAsset(hash::HashName32 const& name, string const& path) {
	LoadAssetInternal(name, path);
	AssetManager::Get().RegisterAsset(GetAssetHashName(), Loaded() ? this : GetDefault());
}

void Asset::UnloadAsset() {
	if (Loaded()) {
		UnloadAsset();
		AssetManager::Get().UnregisterAsset(GetAssetHashName());
	}
}
}
}