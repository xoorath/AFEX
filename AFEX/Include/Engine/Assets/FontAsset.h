#pragma once
#include <Engine/Assets/Asset.h>
#include <Engine/BlockAlloc.h>
#include <Engine/Conversions/MemConversions.h>
namespace afex {
namespace assets {
static BlockAllocator<16_MiB> fontBuffer;

class FontAsset : public Asset {
public:
	virtual u32 GetAssetTypeHash() {
		constexpr u32 typeHash = afex::hash::GetHash32("FontAsset");
		return typeHash;
	}

private:
	u32 m_AssetPathHash;
};
}
}