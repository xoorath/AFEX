#include <Engine/DebugUI.h>
#include <Engine/Window.h>
#include <Engine/Assets/FontAsset.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear/nuklear_glfw_gl4.h>

#include <GLFW/glfw3.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

namespace afex {
namespace internal {
class DebugUIImpl {
public:
	DebugUIImpl(Window* window) {
		m_Window = window;
		
		ctx = nk_glfw3_init(m_Window->GetGLFWWindow(), NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

		// Should be done before the debug UI tries to create or use any fonts.
		assets::FontAsset::BeginAtlas();
		assets::FontAsset::LoadDefaultFont();
		assets::FontAsset::EndAtlas();

		assets::FontAsset::BeginAtlas();
		droidAsset.LoadAsset("Droid", "./fonts/IBM-Plex-Sans/IBMPlexSans-Bold.ttf");
		assets::FontAsset::EndAtlas();

		nk_style_set_font(ctx, droidAsset.GetNkFontHandle());


		/* Create bindless texture.
		* The index returned is not the opengl resource id.
		* IF you need the GL resource id use: nk_glfw3_get_tex_ogl_id() */
		{
			int tex_index = 0;
			enum { tex_width = 256, tex_height = 256 };
			char pixels[tex_width * tex_height * 4];
			memset(pixels, 128, sizeof(pixels));
			tex_index = nk_glfw3_create_texture(pixels, tex_width, tex_height);
			
			img = nk_image_id(tex_index);
		}

		bg.r = 0.14f, bg.g = 0.71f, bg.b = 0.44f, bg.a = 1.0f;

		assets::FontAsset::ReportMemoryUsage();

	}

	~DebugUIImpl() {
		nk_glfw3_shutdown();
	}

	void Update() {
		nk_glfw3_new_frame();
		if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
			NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
			NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			enum { EASY, HARD };
			static int op = EASY;
			static int property = 20;
			nk_layout_row_static(ctx, 30, 80, 1);
			if (nk_button_label(ctx, "button"))
				fprintf(stdout, "button pressed\n");

			nk_layout_row_dynamic(ctx, 30, 2);
			if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
			if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;

			nk_layout_row_dynamic(ctx, 25, 1);
			nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

			nk_layout_row_dynamic(ctx, 20, 1);
			nk_label(ctx, "background:", NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 25, 1);
			if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
				nk_layout_row_dynamic(ctx, 120, 1);
				bg = nk_color_picker(ctx, bg, NK_RGBA);
				nk_layout_row_dynamic(ctx, 25, 1);
				bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
				bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
				bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
				bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
				nk_combo_end(ctx);
			}
		}
		nk_end(ctx);

		/* Bindless Texture */
		if (nk_begin(ctx, "Texture", nk_rect(250, 150, 230, 250),
			NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
			NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			struct nk_command_buffer *canvas = nk_window_get_canvas(ctx);
			struct nk_rect total_space = nk_window_get_content_region(ctx);
			nk_draw_image(canvas, total_space, &img, { 255,255,255,255 });
		}
		nk_end(ctx);

		/* ----------------------------------------- */

		/* Draw */
		int width, height;
		glfwGetWindowSize(m_Window->GetGLFWWindow(), &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(bg.r, bg.g, bg.b, bg.a);
		/* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
		* with blending, scissor, face culling, depth test and viewport and
		* defaults everything back into a default state.
		* Make sure to either a.) save and restore or b.) reset your own state after
		* rendering the UI. */
		nk_glfw3_render(NK_ANTI_ALIASING_ON);
	}

	assets::FontAsset droidAsset;
	Window* m_Window;
	struct nk_image img;
	struct nk_context *ctx;
	struct nk_colorf bg;
};
}

#define DEBUGUI_IMPL(x) reinterpret_cast<internal::DebugUIImpl*>(x->m_Pimpl)

DebugUI* DebugUI::Create(Window* window) {
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