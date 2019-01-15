#pragma once

#include <Engine/HashName.h>

namespace afex {
namespace assets {

class Asset {
public:
	virtual ~Asset() {}
	virtual hash::HashName32 const& GetAssetTypeHashName() const = 0;
	virtual hash::HashName32 const& GetAssetHashName() const = 0;

	// A default asset to use as a placeholder in cases where having an asset fail to load isn't a critical error.
	virtual Asset* GetDefault() const = 0;

	// Returns true if the asset is loaded without critical errors
	virtual bool Loaded() const = 0;

	// Returns this asset if this asset has loaded, otherwise default. 
	// Note: Check default for null on asset types where the default relies on an asset from disk
	Asset* GetThisOrDefault() {
		return Loaded() ? this : GetDefault();
	}

	// Begins loading on this asset, and registers the asset with the asset manager.
	void LoadAsset(hash::HashName32 const& name, string const& path);
	
	// Unloads this asset, unregistering it with the asset manager.
	void UnloadAsset();

protected:
	virtual void LoadAssetInternal(hash::HashName32 const& name, string const& path) = 0;
	virtual void UnloadAssetInternal() = 0;
};
}
}