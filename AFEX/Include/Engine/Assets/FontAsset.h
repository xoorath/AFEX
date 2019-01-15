#pragma once
#include <Engine/Assets/Asset.h>
#include <Engine/BlockAlloc.h>
#include <Engine/Conversions/MemConversions.h>

struct nk_font;
struct nk_font_atlas;
struct nk_user_font;

namespace afex {
namespace assets {

class FontAsset : public Asset {
public:
	virtual ~FontAsset(){}

	virtual hash::HashName32 const& GetAssetTypeHashName() const override;
	virtual hash::HashName32 const& GetAssetHashName() const override;
	virtual Asset*                  GetDefault() const override;
	virtual bool                    Loaded() const override;

	nk_font* GetNkFont() const;
	nk_user_font* GetNkFontHandle() const;

	static void BeginAtlas();
	static void EndAtlas();

	struct PreLoadSettings {
		float m_Height;
	};

	// providing nullptr will clear the active settings (reverting to defaults.
	static void SetPreLoadSettings(PreLoadSettings* settings);

	// The application should do this once before using any fonts.
	static void LoadDefaultFont();

	// Logs memory usage using the engine log functionality.
	static void ReportMemoryUsage();
private:
	virtual void LoadAssetInternal(hash::HashName32 const& name, string const& path) override;
	virtual void UnloadAssetInternal() override;
	
	void LoadFromMemory(byte* data, usize size);

	static hash::HashName32       s_FontAssetHashName;
	static FontAsset              s_DefaultFont;
	static PreLoadSettings        s_DefaultSettings;
	static PreLoadSettings*       s_ActiveSettings;
	static nk_font_atlas*         s_ActiveAtlas;
	static BlockAllocator<16_MiB> s_FontBuffer;

	enum class LoadState {
		NotLoaded,
		LoadingError,
		SuccessfullyLoaded
	};

	LoadState        m_LoadState = LoadState::NotLoaded;
	hash::HashName32 m_FontHashName;
	nk_font*         m_NkFont;
};
}
}