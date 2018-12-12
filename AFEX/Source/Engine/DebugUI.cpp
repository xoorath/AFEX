#include <Engine/DebugUI.h>


#define NK_INCLUDE_STANDARD_IO
#define NUKLEARBASE_USER_HANDLED_INPUT
#include <nuklear/nuklearbase.h>

#include <GLFW/glfw3.h>

struct TestData
{
	int buttonPresses;
	struct nk_text_edit buffer;
};

static void nuklearbase_userDefinedRedrawing(struct nuklearbase_state* state, void* userData) {
	NUKLEARBASE_UNREFERENCED(userData);
	nuklearbase_updateWindowSize(state);
	nuklearbase_loop(state);
}
static void nuklearbase_userDefinedNuklearDeclarations(struct nuklearbase_state* state, void* userData) {
	struct TestData* testData = (struct TestData*)userData;
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
			fprintf(stdout, "Button pressed %d times\n", ++(testData->buttonPresses));
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
		nk_edit_buffer(&state->nuklearContext, NK_EDIT_BOX, &testData->buffer, nk_filter_default);
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
	fontArray[0] = nk_font_atlas_add_from_file(fontAtlas, "mono.ttf", 14, 0);
	fontArray[1] = nk_font_atlas_add_from_file(fontAtlas, "serif.ttf", 16, 0);
	if (fontArray[0] == NULL || fontArray[1] == NULL)
	{
		fprintf(stderr, "Put some fonts in the executable directory!\n");
		exit(-1);
	}
}
#endif

namespace afex {
namespace debugui {
struct TestData testData = { 0 };
struct nuklearbase_state* testState;
void Init(struct ::GLFWwindow* window) {
	testState = (struct nuklearbase_state*)calloc(1, sizeof(struct nuklearbase_state));
	testState->windowName = "Test Thing";
	testState->inputMode = USER_GETS_INPUT;

	
	nk_textedit_init_default(&testData.buffer);

	testState->windowWidthInPixels = 800;
	testState->windowHeightInPixels = 600;

	nuklearbase_initialize(testState, window, &testData);

//	while (!glfwWindowShouldClose(testState->glfwWindowContext))
	{
		
	}
	//(testState);
	//free(testState);
}

void Update() {
	nuklearbase_userDefinedRedrawing(testState, &testData);
}

void Shutdown() {
	nuklearbase_terminate(testState);
	free(testState);
}

}
}