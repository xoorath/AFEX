#pragma once
#include <unordered_map>
#include <Engine/Assets/Asset.h>
#include <Engine/HashName.h>
#include <Engine/Types.h>

#if defined(_DEBUG)
#include <Engine/Log.h>
#endif

namespace afex {
namespace assets {
class AssetManager {
public:
	static AssetManager& Get();

	void RegisterAsset(hash::HashName32 const& hashName, Asset* asset) {
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
		auto found = m_AssetMap.find(hashName);
		if (AFEX_ENSURE_FALSE(found != m_AssetMap.end() && !"You're probably registering an asset twice.")) {
			AFEX_ASSERT_TRUE((*found).first.GetName() == hashName.GetName() && "We've detected a hash collision in asset names.");
			return;
		}
#endif
		m_AssetMap[hashName] = asset;
	}

	void UnregisterAsset(hash::HashName32 const& hashName) {
		auto found = m_AssetMap.find(hashName);
		if (found != m_AssetMap.end()) {
			m_AssetMap.erase(found);
		}
	}

	Asset* FindAsset(hash::HashName32 const& hashName) {
#if defined(_DEBUG) && !HASHNAME_NO_DETAILS
		auto found = m_AssetMap.find(hashName);
		if (false == AFEX_ENSURE_TRUE(found != m_AssetMap.end() && "Asset not found.")) {
			AFEX_ASSERT_TRUE((*found).first.GetName() == hashName.GetName() && "We've detected a hash collision in asset names.");
		}
#endif
		return m_AssetMap[hashName];
	}

private:

	std::unordered_map<hash::HashName32, Asset*> m_AssetMap;
};
}
}