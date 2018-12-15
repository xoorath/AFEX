#include <Engine/DebugUI.h>
#include <Engine/Window.h>

//#define NUKLEARBASE_USER_DEFINED_FONTS
#define NK_INCLUDE_STANDARD_IO
#define NUKLEARBASE_USER_HANDLED_INPUT
//#define NUKLEARBASE_MAX_USER_DEFINED_FONTS 3
#include <nuklear/nuklearbase.h>

#include <GLFW/glfw3.h>

namespace afex {
namespace internal {
class DebugUIImpl {
public:
	DebugUIImpl(::afex::Window* window) {
		m_Window = window;
		memset(&m_State, 0, sizeof(m_State));
		memset(&m_EditBuffer, 0, sizeof(m_EditBuffer));
		m_State.inputMode = USER_GETS_INPUT;
		nk_textedit_init_default(&m_EditBuffer);

		m_State.windowWidthInPixels = m_Window->GetWidth();
		m_State.windowHeightInPixels = m_Window->GetHeight();

		nuklearbase_initialize(&m_State, m_Window->GetGLFWWindow(), this);
	}

	~DebugUIImpl() {
		nuklearbase_terminate(&m_State);
	}

	void Update() {
		nuklearbase_userDefinedRedrawing(&m_State, this);
	}

	::afex::Window* m_Window;
	nuklearbase_state m_State;
	// ui data
	nk_text_edit m_EditBuffer;
	int m_ButtonPress;
};
}

#define DEBUGUI_IMPL(x) reinterpret_cast<::afex::internal::DebugUIImpl*>(x->m_Pimpl)

DebugUI* DebugUI::Create(::afex::Window* window) {
	DebugUI* ui = new DebugUI();
	ui->m_Pimpl = (void*)new internal::DebugUIImpl(window);
	return ui;
}

DebugUI::~DebugUI() {
	if (m_Pimpl) {
		delete DEBUGUI_IMPL(this);
		m_Pimpl = nullptr;
	}
}

void DebugUI::Update() {
	DEBUGUI_IMPL(this)->Update();
}

#undef DEBUGUI_IMPL
}

static void nuklearbase_userDefinedRedrawing(struct nuklearbase_state* state, void* userData) {
	NUKLEARBASE_UNREFERENCED(userData);
	nuklearbase_updateWindowSize(state);
	nuklearbase_loop(state);
}
static void nuklearbase_userDefinedNuklearDeclarations(struct nuklearbase_state* state, void* userData) {
	::afex::internal::DebugUIImpl* testData = reinterpret_cast<::afex::internal::DebugUIImpl*>(userData);
	float width = 360.0f, height = 240.0f;
	float x = (state->windowWidthInPixels - width) / 2.0f,
		y = (state->windowHeightInPixels - height) / 2.0f;
	if (nk_begin_titled(&state->nuklearContext, "testWindow1", "Window Title Goes Here",
		nk_rect(x, y, width, height),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE |
		NK_WINDOW_SCROLL_AUTO_HIDE))
	{
#ifdef NUKLEARBASE_USER_DEFINED_FONTS
		nk_style_set_font(&state->nuklearContext, &state->userDefinedFonts[1]->handle);
#endif
		nk_layout_row_dynamic(&state->nuklearContext, 0, 2);
		if (nk_button_label(&state->nuklearContext, "Press Me"))
			fprintf(stdout, "Button pressed %d times\n", ++(testData->m_ButtonPress));
		if (nk_button_label(&state->nuklearContext, "Quit"))
			glfwSetWindowShouldClose(state->glfwWindowContext, GLFW_TRUE);

		nk_layout_row_dynamic(&state->nuklearContext, 0, 1);
		const char *labeltext = "Stupid text";
		nk_label(&state->nuklearContext, labeltext, NK_TEXT_LEFT);
#ifdef NUKLEARBASE_USER_DEFINED_FONTS
		nk_style_set_font(&state->nuklearContext, &state->userDefinedFonts[0]->handle);
#endif
		const char *labeltext2 = "Stupid text in other font";
		nk_text(&state->nuklearContext, labeltext2, nk_strlen(labeltext2), NK_TEXT_CENTERED);

		nk_layout_row_dynamic(&state->nuklearContext, 75, 1);
		nk_edit_buffer(&state->nuklearContext, NK_EDIT_BOX, &testData->m_EditBuffer, nk_filter_default);
	}
	nk_end(&state->nuklearContext);
}
#if defined(NUKLEARBASE_USER_HANDLED_INPUT)
static void nuklearbase_userDefinedInputHandling(struct nuklearbase_state* state, void* userData, struct nuklearbase_input* input) {
	NUKLEARBASE_UNREFERENCED(userData);
	if (input && input->key && input->key->token == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(state->glfwWindowContext, GLFW_TRUE);
	}
	else
	{
		nuklearbase_addInput(input);
	}
}
#endif
#if defined(NUKLEARBASE_USER_DEFINED_FONTS)
static void nuklearbase_userDefinedFonts(struct nk_font* fontArray[], struct nk_font_atlas* fontAtlas) {
	// Google's Noto fonts are fine. https://github.com/googlei18n/noto-fonts/tree/master/hinted
	fontArray[0] = nk_font_atlas_add_from_file(fontAtlas, "./fonts/NotoSans-Regular.ttf", 18, 0);
	fontArray[1] = nk_font_atlas_add_from_file(fontAtlas, "./fonts/NotoSansSymbols-Regular.ttf", 22, 0);
	if (fontArray[0] == NULL || fontArray[1] == NULL)
	{
		fprintf(stderr, "Put some fonts in the executable directory!\n");
		exit(-1);
	}
}
#endif