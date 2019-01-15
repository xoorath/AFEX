#include <Engine/Assets/FontAsset.h>
#include <fstream>

#define NK_INCLUDE_FONT_BAKING
#include <nuklear/nuklear_glfw_gl4.h>

using std::ifstream;
using std::ios;

#define DEFAULT_FONT     IBMPlexMono_Medium_ttf
#define DEFAULT_FONT_LEN IBMPlexMono_Medium_ttf_len

extern unsigned char DEFAULT_FONT[];
extern unsigned int DEFAULT_FONT_LEN;

namespace afex {
namespace assets {
/*static*/ BlockAllocator<16_MiB> FontAsset::s_FontBuffer;
/*static*/ hash::HashName32 FontAsset::s_FontAssetHashName("FontAsset");
/*static*/ FontAsset FontAsset::s_DefaultFont;
/*static*/ nk_font_atlas* FontAsset::s_ActiveAtlas = nullptr;
/*static*/ FontAsset::PreLoadSettings FontAsset::s_DefaultSettings = {
	s_DefaultSettings.m_Height = 16.f
};
/*static*/ FontAsset::PreLoadSettings* FontAsset::s_ActiveSettings = &FontAsset::s_DefaultSettings;

/*virtual*/ hash::HashName32 const& FontAsset::GetAssetTypeHashName() const /*override*/ {
	return s_FontAssetHashName;
}

/*virtual*/ hash::HashName32 const& FontAsset::GetAssetHashName() const /*override*/ {
	return m_FontHashName;
}

/*virtual*/ Asset* FontAsset::GetDefault() const /*override*/ {
	return &s_DefaultFont;
}

/*virtual*/ bool FontAsset::Loaded() const /*override*/ {
	return m_LoadState == LoadState::SuccessfullyLoaded;
}

/*virtual*/ void FontAsset::LoadAssetInternal(hash::HashName32 const& name, string const& path) /*override*/ {
	m_FontHashName = name;
	ifstream fs(path);
	if (AFEX_ENSURE_TRUE(fs.is_open())) {
		fs.seekg(0, ios::end);
		ifstream::pos_type pos = fs.tellg();
		usize length = static_cast<usize>(pos);
		byte* data = reinterpret_cast<byte*>(s_FontBuffer.Malloc(length));
		fs.seekg(0, ios::beg);
		fs.read(data, length);
		fs.close();
		LoadFromMemory(data, length);
		ReportMemoryUsage();
		s_FontBuffer.Free(reinterpret_cast<void*>(data));
	}
	else {
		LOG(ERROR) << "FontAsset: Could not load file at path: " << path;
		m_LoadState = LoadState::LoadingError;
	}
}

/*virtual*/ void FontAsset::UnloadAssetInternal() /*override*/ {

}

nk_font* FontAsset::GetNkFont() const {
	return Loaded() ? m_NkFont : s_DefaultFont.m_NkFont;
}

nk_user_font* FontAsset::GetNkFontHandle() const {
	nk_font* font = GetNkFont();
	AFEX_ASSERT_TRUE(nullptr != font);
	return &font->handle;
}

/*static*/ void FontAsset::BeginAtlas() {
	nk_glfw3_font_stash_begin(&s_ActiveAtlas);
	
}
/*static*/ void FontAsset::EndAtlas() {
	nk_glfw3_font_stash_end();
}

/*static*/ void FontAsset::SetPreLoadSettings(PreLoadSettings* settings) {
	s_ActiveSettings = nullptr == settings ? &s_DefaultSettings : settings;
}

/*static*/ void FontAsset::LoadDefaultFont() {
	s_DefaultFont.LoadFromMemory(reinterpret_cast<byte*>(DEFAULT_FONT), numeric_cast<usize>(DEFAULT_FONT_LEN));
}

void FontAsset::LoadFromMemory(byte* data, usize size) {
	if (AFEX_ENSURE_FALSE(nullptr == s_ActiveSettings)) {
		LOG(ERROR) << "FontAsset: No active settings. This shouldn't be possible...";
		s_ActiveSettings = &s_DefaultSettings;
	}
	m_NkFont = nk_font_atlas_add_from_memory(s_ActiveAtlas, data, numeric_cast<nk_size>(size), s_ActiveSettings->m_Height, nullptr);
	if (AFEX_ENSURE_FALSE(nullptr == m_NkFont)) {
		m_LoadState = LoadState::LoadingError;
		return;
	}
	m_LoadState = LoadState::SuccessfullyLoaded;
}

/*static*/ void FontAsset::ReportMemoryUsage() {
	auto allocatorInfo = s_FontBuffer.ReportInfo();
	char buffSize[32];
	char buffUsed[32];
	char buffFree[32];
	char buffWasted[32];
	BytesToReadableString(buffSize, sizeof(buffSize), numeric_cast<u64>(allocatorInfo.size));
	BytesToReadableString(buffUsed, sizeof(buffUsed), numeric_cast<u64>(allocatorInfo.bytesUsed));
	BytesToReadableString(buffFree, sizeof(buffFree), numeric_cast<u64>(allocatorInfo.bytesFree));
	BytesToReadableString(buffWasted, sizeof(buffWasted), numeric_cast<u64>(allocatorInfo.bytesWasted));

	LOG(INFO) << 
		"FontAsset: Buffer size: (" << buffSize 
		<< ") used: (" << buffUsed 
		<< ") free: (" << buffFree 
		<< ") wasted: (" << buffWasted 
		<< ") blocks: " << allocatorInfo.blockCount;
}
}
}