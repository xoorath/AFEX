#if !defined(NUKLEARBASE_H)

/*
* This is free and unencumbered software released into the public domain.
*
* Anyone is free to copy, modify, publish, use, compile, sell, or
* distribute this software, either in source code form or as a compiled
* binary, for any purpose, commercial or non-commercial, and by any
* means.
*
* In jurisdictions that recognize copyright laws, the author or authors
* of this software dedicate any and all copyright interest in the
* software to the public domain. We make this dedication for the benefit
* of the public at large and to the detriment of our heirs and
* successors. We intend this dedication to be an overt act of
* relinquishment in perpetuity of all present and future rights to this
* software under copyright law.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* For more information, please refer to <https://unlicense.org>
*/

/*
* To use this thing:
*
* 1) #include "nuklearbase.h"
*
* 2) Initialize with by calling nuklearbase_initialize(yourState, userData)
*    where yourState is a struct nuklearbase_state* to keep some necessary stuff
*    and userData is a pointer to anything that should be passed to user implemented functions.
*    (Also do custom initialization stuff, of course.)
*
* 3) Call nuklearbase_terminate(state) when it's time to end the program.
*
* 4) Implement nuklearbase_userDefinedRedrawing(struct nuklearbase_state* state, void* userData).
*    Might be good to call nuklearbase_updateWindowSize(state).
*    And call nuklearbase_loop(state) to redraw on the context.
*
* 5) Implement nuklearbase_userDefinedNuklearDeclarations(struct nuklearbase_state* state, void* userData)
*    which is called inside nuklearbase_loop(state). Declare all Nuklear layouts and widgets and such.
*
* A) By default, Nuklear gets all the input. To get a first shot at it, do:
*       #define NUKLEARBASE_USER_HANDLED_INPUT
*       #include "nuklearbase.h"
*    and implement nuklearbase_userDefinedInputHandling(struct nuklearbase_state* state, void* userData, struct nuklearbase_input* input).
*    Also, set state->inputMode = USER_GETS_INPUT; Then nuklearbase_userDefinedInputHandling is called with input.
*    If the input is unhandled, or for any reason really, send it to the GUI with nuklearbase_addInput(input).
*    Otherwise, free(input) might be good.
*
* B) By default, the Nuklear default font is used. To use custom fonts, do
*       #define NK_INCLUDE_STANDARD_IO                 // <-- Optional, for font file loading.
*       #define NUKLEARBASE_USER_DEFINED_FONTS
*       #define NUKLEARBASE_MAX_USER_DEFINED_FONTS 50  // <-- Optional, if needing more/fewer fonts.
*       #include "nuklearbase.h"
*    and implement nuklearbase_userDefinedFonts(struct nk_font** fonts, struct nk_font_atlas* fontAtlas)
*    with lines like:
*       fonts[n] = nk_font_atlas_add_from_memory(fontAtlas, pointerToFontInTTF, 380928, 14.5f, NULL);
*    or (with NK_INCLUDE_STANDARD_IO):
*       fonts[n] = nk_font_atlas_add_from_file(fontAtlas, "mono.ttf", 14.5f, NULL);
*    Set fonts wherever with:
*       nk_style_set_font(&state->nuklearContext, &state->userDefinedFonts[n]->handle);
*
* For now.
*/

// I use http://glad.dav1d.de/#profile=core&language=c&specification=gl&loader=on&api=gl%3D4.5
#include "glad.c"
#include <GLFW/glfw3.h>

//#define NK_IMPLEMENTATION
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_ZERO_COMMAND_MEMORY

#if !defined(NUKLEARBASE_USER_DEFINED_FONTS)
#define NK_INCLUDE_DEFAULT_FONT
#endif

#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4701)
#pragma warning(disable:4996)
// From https://github.com/vurtun/nuklear/
#include "nuklear.h"
#pragma warning(pop)

// TODO: Clipboard!
// TODO: Make code better probably!

#if defined(NUKLEARBASE_USER_DEFINED_FONTS)
#if !defined(NUKLEARBASE_MAX_USER_DEFINED_FONTS)
#define NUKLEARBASE_MAX_USER_DEFINED_FONTS 8
#endif
#endif

enum nuklearbase_input_mode;
struct nuklearbase_state;
struct nuklearbase_key_press;
struct nuklearbase_text_input;
struct nuklearbase_cursor_position;
struct nuklearbase_mouse_press;
struct nuklearbase_scroll_input;
struct nuklearbase_input;
struct _nuklearbase_input_node;
struct _nuklearbase_basic_vertex_layout;

#define NUKLEARBASE_UNREFERENCED(x) (x)

enum nuklearbase_input_mode
{
	NUKLEAR_GETS_INPUT,
	USER_GETS_INPUT
};

struct nuklearbase_state
{
	GLFWwindow* glfwWindowContext;
	char const* windowName;
	int windowWidthInPixels, windowHeightInPixels;
	int framebufferWidthtInPixels, framebufferHeightInPixels;
	GLuint vertexBufferObjectID;
	GLuint vertexArrayObjectID;
	GLuint elementArrayBufferObjectID;
	GLuint shaderProgramID;
	GLint textureCoordinateAttributeID;
	GLint colorValueAttributeID;
	GLint displaySizeAttributeID;
	GLint textureImageUniformID;
	GLuint fontTextureUnitID;
	struct nk_buffer nuklearDrawingCommands;
	struct nk_draw_null_texture nullTexture;
	struct nk_context nuklearContext;
	struct nk_font_atlas fontTextureAtlas;
	struct nk_vec2 framebufferPixelsPerWindowPixels;
	struct nk_convert_config nuklearVertexConversionConfig;
	enum nuklearbase_input_mode inputMode;
	struct _nuklearbase_input_node* firstUnhandledInput;
	struct _nuklearbase_input_node* lastUnhandledInput;
	double lastMouseX;
	double lastMouseY;
	double lastTime;
	int textReplace;
	void* userData;
#if defined(NUKLEARBASE_USER_DEFINED_FONTS)
	struct nk_font* userDefinedFonts[NUKLEARBASE_MAX_USER_DEFINED_FONTS];
#endif
};

struct nuklearbase_key_press
{
	int token;
	int scancode;
	int action;
	int modifiers;
};

struct nuklearbase_text_input
{
	unsigned int codepoint;
	int modifiers;
};

struct nuklearbase_cursor_position
{
	double xPosition;
	double yPosition;
};

struct nuklearbase_mouse_press
{
	int button;
	int action;
	int modifiers;
};

struct nuklearbase_scroll_input
{
	double x;
	double y;
};

struct nuklearbase_input
{
	struct nuklearbase_key_press* key;
	struct nuklearbase_text_input* text;
	struct nuklearbase_cursor_position* cursor;
	struct nuklearbase_mouse_press* mouse;
	struct nuklearbase_scroll_input* scroll;
};

struct _nuklearbase_input_node
{
	struct nuklearbase_input* input;
	struct _nuklearbase_input_node* next;
};

struct _nuklearbase_basic_vertex_layout
{
	float vertexCoordinates[2];
	float textureCoordinates[2];
	nk_byte colorValues[4];
};

static void nuklearbase_userDefinedRedrawing(struct nuklearbase_state*, void*);
static void nuklearbase_userDefinedNuklearDeclarations(struct nuklearbase_state*, void*);
#if defined(NUKLEARBASE_USER_HANDLED_INPUT)
static void nuklearbase_userDefinedInputHandling(struct nuklearbase_state*, void*, struct nuklearbase_input*);
#endif
#if defined(NUKLEARBASE_USER_DEFINED_FONTS)
static void nuklearbase_userDefinedFonts(struct nk_font*[], struct nk_font_atlas*);
#endif

static void nuklearbase_initialize(struct nuklearbase_state*, GLFWwindow*, void* userData);
static void nuklearbase_updateWindowSize(struct nuklearbase_state*);
static void nuklearbase_loop(struct nuklearbase_state*);
static void nuklearbase_addInput(struct nuklearbase_input*);
static void nuklearbase_terminate(struct nuklearbase_state*);

static void _nuklearbase_handleError(int, const char*);
static void _nuklearbase_attachShader(GLuint, GLuint*, GLenum, const GLchar**);
static void _nuklearbase_deleteShader(GLuint, GLuint);
static void _nuklearbase_glfwWindowChangeCallback(GLFWwindow*, int, int);
static void _nuklearbase_keyCallback(GLFWwindow*, int, int, int, int);
static void _nuklearbase_textCallback(GLFWwindow*, unsigned int, int);
static void _nuklearbase_cursorCallback(GLFWwindow*, double, double);
static void _nuklearbase_mouseCallback(GLFWwindow*, int, int, int);
static void _nuklearbase_scrollCallback(GLFWwindow*, double, double);
static void _nuklearbase_handleInput(struct nuklearbase_state*);

static struct nuklearbase_state* _nuklearbase_state_global = NULL;
static struct _nuklearbase_input_node* _unhandled_input = NULL;

#define MAX_VERTEX_BUFFER_SIZE 512 * 1024
#define MAX_ELEMENT_BUFFER_SIZE 128 * 1024

#define INFORMATION_LOG_LENGTH 512
#define INFORMATION_LOG_ERROR_MESSAGE_LENGTH 544

static void
nuklearbase_updateWindowSize(struct nuklearbase_state* state)
{
	glfwGetWindowSize(state->glfwWindowContext, &state->windowWidthInPixels, &state->windowHeightInPixels);
	glfwGetFramebufferSize(state->glfwWindowContext, &state->framebufferWidthtInPixels, &state->framebufferHeightInPixels);
	state->framebufferPixelsPerWindowPixels.x = (float)state->framebufferWidthtInPixels / (float)state->windowWidthInPixels;
	state->framebufferPixelsPerWindowPixels.y = (float)state->framebufferHeightInPixels / (float)state->windowHeightInPixels;
	glViewport(0, 0, state->windowWidthInPixels, state->windowHeightInPixels);
}

static void
nuklearbase_initialize(struct nuklearbase_state* state, GLFWwindow* window, void* userData)
{
	_nuklearbase_state_global = state;
	state->userData = userData;
	{// Initialize GLFW and GLAD.
		{// Set error callback; when GLFW encounters an error, it calls this function.
			glfwSetErrorCallback(_nuklearbase_handleError);
		}
		{// Initialize GLFW.
			//if (glfwInit() != GLFW_TRUE)
				//_nuklearbase_handleError(28927, "couldn't initialize GLFW");
		}
		{// Define which OpenGL version to use: 3.3 core.
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}
		{// Create an OpenGL context and make it the current one.
			state->glfwWindowContext = window;
				//glfwCreateWindow(
				//state->windowWidthInPixels, state->windowHeightInPixels,
				//state->windowName, NULL, NULL);
			glfwMakeContextCurrent(state->glfwWindowContext);
		}
		{// Update the program state with the current window size.
			glfwGetWindowSize(state->glfwWindowContext, &state->windowWidthInPixels, &state->windowHeightInPixels);
		}
		{// Load OpenGL functions.
			if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
				_nuklearbase_handleError(7109, "couldn't load with GLAD");
		}
		{// Tell OpenGL how to convert normalized X-Y coordinates to window pixels relative to its lower left.
			glViewport(0, 0, state->windowWidthInPixels, state->windowHeightInPixels);
		}
		{// Keep the window redrawn even when changing size.
			glfwSetFramebufferSizeCallback(state->glfwWindowContext, _nuklearbase_glfwWindowChangeCallback);
		}
	}
	{// Define and create a shader program.
		const GLchar* vertexShaderSource =
			"#version 330 core\n"
			"uniform vec2 displaySize;"
			"in vec2 vertexCoordinates;"
			"in vec2 textureCoordinates;"
			"in vec4 colorValues;"
			"out vec2 passedTextureCoordinates;"
			"out vec4 passedColorValues;"
			"mat4 orthogonalProjectionMatrix = mat4(2.0f, 0, 0, 0,   0, -2.0, 0, 0,   0, 0, 0, 0,   -1.0f, 1.0f, 0, 1.0f);"
			"void main()"
			"{"
			"   passedTextureCoordinates = textureCoordinates;"
			"   passedColorValues = colorValues;"
			"   gl_Position = orthogonalProjectionMatrix * vec4(vertexCoordinates.x/displaySize.x, vertexCoordinates.y/displaySize.y, 0, 1);"
			"}";

		const GLchar* fragmentShaderSource =
			"#version 330 core\n"
			"precision mediump float;"
			"uniform sampler2D textureSamplerIn2D;"
			"in vec2 passedTextureCoordinates;"
			"in vec4 passedColorValues;"
			"out vec4 outputColorValues;"
			"void main()"
			"{"
			"   outputColorValues = passedColorValues * texture(textureSamplerIn2D, passedTextureCoordinates.st);"
			"}";
		state->shaderProgramID = glCreateProgram();
		GLuint vertexShaderID;
		GLuint fragmentShaderID;
		_nuklearbase_attachShader(state->shaderProgramID, &vertexShaderID, GL_VERTEX_SHADER, &vertexShaderSource);
		_nuklearbase_attachShader(state->shaderProgramID, &fragmentShaderID, GL_FRAGMENT_SHADER, &fragmentShaderSource);
		glLinkProgram(state->shaderProgramID);
		int shaderLinkingSucceeded;
		glGetProgramiv(state->shaderProgramID, GL_LINK_STATUS, &shaderLinkingSucceeded);
		if (shaderLinkingSucceeded != GL_TRUE)
		{
			char informationLog[INFORMATION_LOG_LENGTH];
			glGetProgramInfoLog(state->shaderProgramID, INFORMATION_LOG_LENGTH, NULL, informationLog);
			char errorMessage[INFORMATION_LOG_ERROR_MESSAGE_LENGTH];
			sprintf(errorMessage, "failed to link shaders: %s", informationLog);
			_nuklearbase_handleError(4798, errorMessage);
		}
		{// Remove shaders from memory; they are now instead part of the program.
			_nuklearbase_deleteShader(state->shaderProgramID, vertexShaderID);
			_nuklearbase_deleteShader(state->shaderProgramID, fragmentShaderID);
		}
	}
	{// Prepare objects for vertex transferring.
		GLuint vertexCoordinateAttributeID;
		GLuint textureCoordinateAttributeID;
		GLuint colorValueAttributeID;
		{// Create and make current a vertex array object.
			glGenVertexArrays(1, &state->vertexArrayObjectID);
			glBindVertexArray(state->vertexArrayObjectID);
		}
		{// Specify what information is to be contained with the vertex data.
			state->displaySizeAttributeID = glGetUniformLocation(state->shaderProgramID, "displaySize");
			state->textureImageUniformID = glGetUniformLocation(state->shaderProgramID, "textureSamplerIn2D");
			vertexCoordinateAttributeID = glGetAttribLocation(state->shaderProgramID, "vertexCoordinates");
			textureCoordinateAttributeID = glGetAttribLocation(state->shaderProgramID, "textureCoordinates");
			colorValueAttributeID = glGetAttribLocation(state->shaderProgramID, "colorValues");
			glEnableVertexAttribArray(vertexCoordinateAttributeID);
			glEnableVertexAttribArray(textureCoordinateAttributeID);
			glEnableVertexAttribArray(colorValueAttributeID);
		}
		{// Prepare buffers to contain the vertex data.
			{// Prepare vertex buffer; it becomes bound directly to the OpenGL state and is therefore unbound.
				glGenBuffers(1, &state->vertexBufferObjectID);
				glBindBuffer(GL_ARRAY_BUFFER, state->vertexBufferObjectID);
				glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_BUFFER_SIZE, NULL, GL_STREAM_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			{// Prepare element buffer; it becomes bound to the vertex _array_ object.
				glGenBuffers(1, &state->elementArrayBufferObjectID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->elementArrayBufferObjectID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_ELEMENT_BUFFER_SIZE, NULL, GL_STREAM_DRAW);
			}
		}
		{// Specify the binary format of one vertex.
			glBindBuffer(GL_ARRAY_BUFFER, state->vertexBufferObjectID);
			GLsizei bytesToStoreOneVertexDataPoint = sizeof(struct _nuklearbase_basic_vertex_layout);
			size_t bytesFrom0ToVertexCoordinatesAttribute = offsetof(struct _nuklearbase_basic_vertex_layout, vertexCoordinates);
			size_t bytesFrom0ToTextureCoordinatesAttribute = offsetof(struct _nuklearbase_basic_vertex_layout, textureCoordinates);
			size_t bytesFrom0ToColorValuesAttribute = offsetof(struct _nuklearbase_basic_vertex_layout, colorValues);
			glVertexAttribPointer(vertexCoordinateAttributeID, 2, GL_FLOAT, GL_FALSE, bytesToStoreOneVertexDataPoint, (void*)bytesFrom0ToVertexCoordinatesAttribute);
			glVertexAttribPointer(textureCoordinateAttributeID, 2, GL_FLOAT, GL_FALSE, bytesToStoreOneVertexDataPoint, (void*)bytesFrom0ToTextureCoordinatesAttribute);
			glVertexAttribPointer(colorValueAttributeID, 4, GL_UNSIGNED_BYTE, GL_TRUE, bytesToStoreOneVertexDataPoint, (void*)bytesFrom0ToColorValuesAttribute);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	{// Finished with the vertex array object for not so unbind to keep it untouched until needed.
		glBindVertexArray(0);
	}
	{// Make room for a Nuklear context.
		nk_init_default(&state->nuklearContext, 0);
		nk_buffer_init_default(&state->nuklearDrawingCommands);
	}
	{// Prepare Nuklear's fonts.
		GLuint fontTextureID;
		{// Prepare an OpenGL texture to contain fonts.
			glGenTextures(1, &fontTextureID);
			state->fontTextureUnitID = GL_TEXTURE0;
			glActiveTexture(state->fontTextureUnitID);
			glBindTexture(GL_TEXTURE_2D, fontTextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		{// Render fonts into the above texture.
			nk_font_atlas_init_default(&state->fontTextureAtlas);
			nk_font_atlas_begin(&state->fontTextureAtlas);
#if defined(NUKLEARBASE_USER_DEFINED_FONTS)
			for (int i = 0; i < NUKLEARBASE_MAX_USER_DEFINED_FONTS; i++)
			{
				state->userDefinedFonts[i] = NULL;
			}
			nuklearbase_userDefinedFonts(state->userDefinedFonts, &state->fontTextureAtlas);
			if (state->userDefinedFonts[0] == NULL)
			{
				_nuklearbase_handleError(23997, "failed to load user-defined fonts");
			}
#endif
			GLsizei fontAtlasTextureImageWidth;
			GLsizei fontAtlasTextureImageHeight;
			const void* allFontsAsOneAtlasTextureImage = nk_font_atlas_bake(&state->fontTextureAtlas,
				&fontAtlasTextureImageWidth,
				&fontAtlasTextureImageHeight,
				NK_FONT_ATLAS_RGBA32);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				fontAtlasTextureImageWidth, fontAtlasTextureImageHeight,
				0, GL_RGBA, GL_UNSIGNED_BYTE, allFontsAsOneAtlasTextureImage);
			nk_font_atlas_end(&state->fontTextureAtlas,
				nk_handle_id((int)fontTextureID),
				&state->nullTexture);
#if defined(NUKLEARBASE_USER_DEFINED_FONTS)
			for (int i = 0; state->userDefinedFonts[i] > 0; i++)
			{
				if (state->userDefinedFonts[i] == NULL)
				{
					break;
				}
				else
				{
					nk_init_default(&state->nuklearContext, &state->userDefinedFonts[i]->handle);
				}
			}
#endif
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		{// Set default font.
#if defined(NUKLEARBASE_USER_DEFINED_FONTS)
			nk_style_set_font(&state->nuklearContext, &state->userDefinedFonts[0]->handle);
#else
			nk_style_set_font(&state->nuklearContext, &state->fontTextureAtlas.default_font->handle);
#endif
		}
	}
	{// Let Nuklear know how to make vertices.
		static const struct nk_draw_vertex_layout_element vertex_layout[] = {
			{ NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct _nuklearbase_basic_vertex_layout, vertexCoordinates) },
		{ NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct _nuklearbase_basic_vertex_layout, textureCoordinates) },
		{ NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct _nuklearbase_basic_vertex_layout, colorValues) },
		{ NK_VERTEX_LAYOUT_END }
		};
		memset(&state->nuklearVertexConversionConfig, 0, sizeof(state->nuklearVertexConversionConfig));
		state->nuklearVertexConversionConfig.vertex_layout = vertex_layout;
		state->nuklearVertexConversionConfig.vertex_size = sizeof(struct _nuklearbase_basic_vertex_layout);
		state->nuklearVertexConversionConfig.vertex_alignment = NK_ALIGNOF(struct _nuklearbase_basic_vertex_layout);
		state->nuklearVertexConversionConfig.null = state->nullTexture;
		state->nuklearVertexConversionConfig.circle_segment_count = 22;
		state->nuklearVertexConversionConfig.curve_segment_count = 22;
		state->nuklearVertexConversionConfig.arc_segment_count = 22;
		state->nuklearVertexConversionConfig.global_alpha = 1.0f;
		state->nuklearVertexConversionConfig.shape_AA = NK_ANTI_ALIASING_ON;
		state->nuklearVertexConversionConfig.line_AA = NK_ANTI_ALIASING_ON;
	}
	{// Set GLFW input callbacks.
		glfwSetKeyCallback(state->glfwWindowContext, _nuklearbase_keyCallback);
		glfwSetCharModsCallback(state->glfwWindowContext, _nuklearbase_textCallback);
		glfwSetCursorPosCallback(state->glfwWindowContext, _nuklearbase_cursorCallback);
		glfwSetMouseButtonCallback(state->glfwWindowContext, _nuklearbase_mouseCallback);
		glfwSetScrollCallback(state->glfwWindowContext, _nuklearbase_scrollCallback);
		state->lastMouseX = 0.0;
		state->lastMouseY = 0.0;
		state->nuklearContext.delta_time_seconds = 0.0f;
		state->lastTime = glfwGetTime();
	}
}

static void
nuklearbase_loop(struct nuklearbase_state* state)
{
	state->nuklearContext.delta_time_seconds = (float)(glfwGetTime() - state->lastTime);
	state->lastTime = glfwGetTime();
	{// Handle collected input.
		nk_input_begin(&state->nuklearContext);
		_nuklearbase_handleInput(state);
		nk_input_end(&state->nuklearContext);
	}
	{
		nuklearbase_userDefinedNuklearDeclarations(state, state->userData);
	}
	GLboolean preLoop_GL_BLEND;
	GLint preLoop_GL_SRC_ALPHA;
	GLboolean preLoop_GL_CULL_FACE;
	GLboolean preLoop_GL_DEPTH_TEST;
	GLboolean preLoop_GL_SCISSOR_TEST;
	{// Prepare the OpenGL context for drawing what Nuklear wants drawn.
		glGetBooleanv(GL_BLEND, &preLoop_GL_BLEND);
		glGetIntegerv(GL_SRC_ALPHA, &preLoop_GL_SRC_ALPHA);
		glGetBooleanv(GL_CULL_FACE, &preLoop_GL_CULL_FACE);
		glGetBooleanv(GL_DEPTH_TEST, &preLoop_GL_DEPTH_TEST);
		glGetBooleanv(GL_SCISSOR_TEST, &preLoop_GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glBindVertexArray(state->vertexArrayObjectID);
		glActiveTexture(state->fontTextureUnitID);
		glBindBuffer(GL_ARRAY_BUFFER, state->vertexBufferObjectID);
	}
	{// Prepare the shader program and populate its uniforms.
		glUseProgram(state->shaderProgramID);
		glUniform1i(state->textureImageUniformID,
			GL_TEXTURE0 - state->fontTextureUnitID);
		glUniform2f(state->displaySizeAttributeID,
			(GLfloat)state->windowWidthInPixels,
			(GLfloat)state->windowHeightInPixels);
	}
	{// Tell Nuklear to convert its internal drawing commands into vertices, and where to put them.
		void* verticesToDraw = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		void* elementsToDraw = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		struct nk_buffer nuklearHandledVertixBuffer;
		struct nk_buffer nuklearHandledElementBuffer;
		nk_buffer_init_fixed(&nuklearHandledVertixBuffer, verticesToDraw, (size_t)MAX_VERTEX_BUFFER_SIZE);
		nk_buffer_init_fixed(&nuklearHandledElementBuffer, elementsToDraw, (size_t)MAX_ELEMENT_BUFFER_SIZE);
		nk_convert(&state->nuklearContext,
			&state->nuklearDrawingCommands,
			&nuklearHandledVertixBuffer,
			&nuklearHandledElementBuffer,
			&state->nuklearVertexConversionConfig);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
	{// Finally, draw what Nuklear wants drawn.
		const struct nk_draw_command* currentDrawingCommand = NULL;
		const nk_draw_index* currentElementIndexOffsetInBuffer = NULL;
		nk_draw_foreach(currentDrawingCommand,
			&state->nuklearContext,
			&state->nuklearDrawingCommands)
		{
			if (currentDrawingCommand->elem_count == 0)
				continue;
			glBindTexture(GL_TEXTURE_2D,
				(GLuint)currentDrawingCommand->texture.id);
			glScissor(
				(GLint)(currentDrawingCommand->clip_rect.x * state->framebufferPixelsPerWindowPixels.x),
				(GLint)((state->windowHeightInPixels - (GLint)(currentDrawingCommand->clip_rect.y + currentDrawingCommand->clip_rect.h)) * state->framebufferPixelsPerWindowPixels.y),
				(GLint)(currentDrawingCommand->clip_rect.w * state->framebufferPixelsPerWindowPixels.x),
				(GLint)(currentDrawingCommand->clip_rect.h * state->framebufferPixelsPerWindowPixels.y));
			glDrawElements(GL_TRIANGLES,
				(GLsizei)currentDrawingCommand->elem_count,
				GL_UNSIGNED_SHORT,
				currentElementIndexOffsetInBuffer);
			currentElementIndexOffsetInBuffer += currentDrawingCommand->elem_count;
		}
		nk_clear(&state->nuklearContext);
	}
	{// Clean up the OpenGL context for use by others.
		glUseProgram(0);
		glActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		if (preLoop_GL_SCISSOR_TEST == GL_FALSE)
			glDisable(GL_SCISSOR_TEST);
		if (preLoop_GL_DEPTH_TEST == GL_TRUE)
			glEnable(GL_DEPTH_TEST);
		if (preLoop_GL_CULL_FACE == GL_TRUE)
			glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, preLoop_GL_SRC_ALPHA);
		if (preLoop_GL_BLEND == GL_FALSE)
			glDisable(GL_BLEND);
	}
}

static void
nuklearbase_addInput(struct nuklearbase_input* newInput)
{
	struct _nuklearbase_input_node* newNode = (struct _nuklearbase_input_node*)calloc(1, sizeof(struct _nuklearbase_input_node));
	newNode->input = newInput;
	if (_nuklearbase_state_global->firstUnhandledInput == NULL)
	{
		_nuklearbase_state_global->firstUnhandledInput = newNode;
		_nuklearbase_state_global->lastUnhandledInput = newNode;
	}
	else
	{
		_nuklearbase_state_global->lastUnhandledInput->next = newNode;
		_nuklearbase_state_global->lastUnhandledInput = newNode;
	}
}

static void
nuklearbase_terminate(struct nuklearbase_state* state)
{
	nk_font_atlas_clear(&state->fontTextureAtlas);
	nk_free(&state->nuklearContext);
	nk_buffer_free(&state->nuklearDrawingCommands);
	glDeleteProgram(state->shaderProgramID);
	glDeleteBuffers(1, &state->vertexBufferObjectID);
	glDeleteBuffers(1, &state->elementArrayBufferObjectID);
	memset(&state, 0, sizeof(state));
	glfwTerminate();
}

static void
_nuklearbase_handleError(int errorCode, const char* errorMessage)
{
	fprintf(stderr, "Error %d: %s\n", errorCode, errorMessage);
	glfwTerminate();
	exit(EXIT_FAILURE);
}

static void
_nuklearbase_attachShader(GLuint programID, GLuint* shaderID, GLenum shaderType, const GLchar** shaderSource)
{
	*shaderID = glCreateShader(shaderType);
	glShaderSource(*shaderID, 1, shaderSource, NULL);
	glCompileShader(*shaderID);
	int shaderCompilationOrLinkingSucceeded;
	glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &shaderCompilationOrLinkingSucceeded);
	if (shaderCompilationOrLinkingSucceeded != GL_TRUE)
	{
		char informationLog[INFORMATION_LOG_LENGTH];
		glGetShaderInfoLog(*shaderID, INFORMATION_LOG_LENGTH, NULL, informationLog);
		char errorMessage[INFORMATION_LOG_ERROR_MESSAGE_LENGTH];
		sprintf(errorMessage, "failed to compile shader: %s", informationLog);
		_nuklearbase_handleError(58240, errorMessage);
	}
	glAttachShader(programID, *shaderID);
}

static void
_nuklearbase_deleteShader(GLuint programID, GLuint shaderID)
{
	glDetachShader(programID, shaderID);
	glDeleteShader(shaderID);
}

static void
_nuklearbase_glfwWindowChangeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
	NUKLEARBASE_UNREFERENCED(window);
	NUKLEARBASE_UNREFERENCED(newWidth);
	NUKLEARBASE_UNREFERENCED(newHeight);
	nuklearbase_userDefinedRedrawing(_nuklearbase_state_global, _nuklearbase_state_global->userData);
}

static void
_nuklearbase_keyCallback(GLFWwindow* windowContext, int token, int scancode, int action, int modifiers)
{
	NUKLEARBASE_UNREFERENCED(windowContext);
	struct nuklearbase_key_press* key = (struct nuklearbase_key_press*)calloc(1, sizeof(struct nuklearbase_key_press));
	key->token = token;
	key->scancode = scancode;
	key->action = action;
	key->modifiers = modifiers;
	struct nuklearbase_input* input = (struct nuklearbase_input*)calloc(1, sizeof(struct nuklearbase_input));
	input->key = key;
#if defined(NUKLEARBASE_USER_HANDLED_INPUT)
	if (_nuklearbase_state_global->inputMode == USER_GETS_INPUT)
	{
		nuklearbase_userDefinedInputHandling(_nuklearbase_state_global, _nuklearbase_state_global->userData, input);
		return;
	}
#else
	nuklearbase_addInput(input);
#endif
}

static void
_nuklearbase_textCallback(GLFWwindow* windowContext, unsigned int codepoint, int modifiers)
{
	NUKLEARBASE_UNREFERENCED(windowContext);
	struct nuklearbase_text_input* text = (struct nuklearbase_text_input*)calloc(1, sizeof(struct nuklearbase_text_input));
	text->codepoint = codepoint;
	text->modifiers = modifiers;
	struct nuklearbase_input* input = (struct nuklearbase_input*)calloc(1, sizeof(struct nuklearbase_input));
	input->text = text;
#if defined(NUKLEARBASE_USER_HANDLED_INPUT)
	if (_nuklearbase_state_global->inputMode == USER_GETS_INPUT)
	{
		nuklearbase_userDefinedInputHandling(_nuklearbase_state_global, _nuklearbase_state_global->userData, input);
		return;
	}
#else
	nuklearbase_addInput(input);
#endif
}

static void
_nuklearbase_cursorCallback(GLFWwindow* windowContext, double xPosition, double yPosition)
{
	NUKLEARBASE_UNREFERENCED(windowContext);
	struct nuklearbase_cursor_position* cursor = (struct nuklearbase_cursor_position*)calloc(1, sizeof(struct nuklearbase_cursor_position));
	cursor->xPosition = xPosition;
	cursor->yPosition = yPosition;
	struct nuklearbase_input* input = (struct nuklearbase_input*)calloc(1, sizeof(struct nuklearbase_input));
	input->cursor = cursor;
#if defined(NUKLEARBASE_USER_HANDLED_INPUT)
	if (_nuklearbase_state_global->inputMode == USER_GETS_INPUT)
	{
		nuklearbase_userDefinedInputHandling(_nuklearbase_state_global, _nuklearbase_state_global->userData, input);
		return;
	}
#else
	nuklearbase_addInput(input);
#endif
}

static void
_nuklearbase_mouseCallback(GLFWwindow* windowContext, int mouseButton, int action, int modifiers)
{
	NUKLEARBASE_UNREFERENCED(windowContext);
	struct nuklearbase_mouse_press* mouse = (struct nuklearbase_mouse_press*)calloc(1, sizeof(struct nuklearbase_mouse_press));
	mouse->button = mouseButton;
	mouse->action = action;
	mouse->modifiers = modifiers;
	struct nuklearbase_input* input = (struct nuklearbase_input*)calloc(1, sizeof(struct nuklearbase_input));
	input->mouse = mouse;
#if defined(NUKLEARBASE_USER_HANDLED_INPUT)
	if (_nuklearbase_state_global->inputMode == USER_GETS_INPUT)
	{
		nuklearbase_userDefinedInputHandling(_nuklearbase_state_global, _nuklearbase_state_global->userData, input);
		return;
	}
#else
	nuklearbase_addInput(input);
#endif
}

static void
_nuklearbase_scrollCallback(GLFWwindow* windowContext, double xScroll, double yScroll)
{
	NUKLEARBASE_UNREFERENCED(windowContext);
	struct nuklearbase_scroll_input* scroll = (struct nuklearbase_scroll_input*)calloc(1, sizeof(struct nuklearbase_scroll_input));
	scroll->x = xScroll;
	scroll->y = yScroll;
	struct nuklearbase_input* input = (struct nuklearbase_input*)calloc(1, sizeof(struct nuklearbase_input));
	input->scroll = scroll;
#if defined(NUKLEARBASE_USER_HANDLED_INPUT)
	if (_nuklearbase_state_global->inputMode == USER_GETS_INPUT)
	{
		nuklearbase_userDefinedInputHandling(_nuklearbase_state_global, _nuklearbase_state_global->userData, input);
		return;
	}
#else
	nuklearbase_addInput(input);
#endif
}

static void
_nuklearbase_handleInput(struct nuklearbase_state* state)
{
	struct _nuklearbase_input_node* currentNode;
	while (state->firstUnhandledInput != NULL)
	{
		currentNode = state->firstUnhandledInput;

		if (currentNode->input->key != NULL)
		{
			int pressedKey = -1;
			if (currentNode->input->key->modifiers == GLFW_MOD_SHIFT)
			{
				switch (currentNode->input->key->token)
				{
				case GLFW_KEY_LEFT_SHIFT:
				case GLFW_KEY_RIGHT_SHIFT:
					pressedKey = NK_KEY_SHIFT; break;
				case GLFW_KEY_DELETE:
					pressedKey = NK_KEY_CUT; break;
				case GLFW_KEY_INSERT:
					pressedKey = NK_KEY_PASTE; break;
				default:
				{
					// Unhandled SHIFT modified key presses are re-sent as non-modified key presses.
					_nuklearbase_keyCallback(state->glfwWindowContext, currentNode->input->key->token,
						currentNode->input->key->scancode,
						(currentNode->input->key->action == GLFW_RELEASE ? 0 : 1), 0);
				}
				}
			}

			else if (currentNode->input->key->modifiers == GLFW_MOD_CONTROL)
			{
				switch (currentNode->input->key->token)
				{
				case GLFW_KEY_LEFT_CONTROL:
				case GLFW_KEY_RIGHT_CONTROL:
					pressedKey = NK_KEY_CTRL; break;
				case GLFW_KEY_C:
					pressedKey = NK_KEY_COPY; break;
				case GLFW_KEY_INSERT:
					pressedKey = NK_KEY_COPY; break;
				case GLFW_KEY_X:
					pressedKey = NK_KEY_CUT; break;
				case GLFW_KEY_V:
					pressedKey = NK_KEY_PASTE; break;
				case GLFW_KEY_LEFT:
					pressedKey = NK_KEY_TEXT_WORD_LEFT; break;
				case GLFW_KEY_RIGHT:
					pressedKey = NK_KEY_TEXT_WORD_RIGHT; break;
				case GLFW_KEY_UP:
					pressedKey = NK_KEY_TEXT_LINE_START; break;
				case GLFW_KEY_DOWN:
					pressedKey = NK_KEY_TEXT_LINE_END; break;
				case GLFW_KEY_HOME:
					pressedKey = NK_KEY_TEXT_START; break;
				case GLFW_KEY_END:
					pressedKey = NK_KEY_TEXT_END; break;
				case GLFW_KEY_Z:
					pressedKey = NK_KEY_TEXT_UNDO; break;
				case GLFW_KEY_Y:
					pressedKey = NK_KEY_TEXT_REDO; break;
				case GLFW_KEY_A:
					pressedKey = NK_KEY_TEXT_SELECT_ALL; break;
				default:
				{
					// Unhandled CTRL modified key presses are re-sent as non-modified key presses.
					_nuklearbase_keyCallback(state->glfwWindowContext, currentNode->input->key->token,
						currentNode->input->key->scancode,
						(currentNode->input->key->action == GLFW_RELEASE ? 0 : 1), 0);
				}
				}
			}

			else if (currentNode->input->key->modifiers == 0)
			{
				switch (currentNode->input->key->token)
				{
				case GLFW_KEY_UNKNOWN:
					pressedKey = NK_KEY_NONE; break;
				case GLFW_KEY_DELETE:
					pressedKey = NK_KEY_DEL; break;
				case GLFW_KEY_ENTER:
					pressedKey = NK_KEY_ENTER; break;
				case GLFW_KEY_TAB:
					pressedKey = NK_KEY_TAB; break;
				case GLFW_KEY_BACKSPACE:
					pressedKey = NK_KEY_BACKSPACE; break;
				case GLFW_KEY_INSERT:
				{
					if (state->textReplace == 0)
					{
						state->textReplace = 1;
						pressedKey = NK_KEY_TEXT_REPLACE_MODE;
					}
					else
					{
						state->textReplace = 0;
						pressedKey = NK_KEY_TEXT_INSERT_MODE;
					}
				} break;
				case GLFW_KEY_UP:
					pressedKey = NK_KEY_UP; break;
				case GLFW_KEY_DOWN:
					pressedKey = NK_KEY_DOWN; break;
				case GLFW_KEY_LEFT:
					pressedKey = NK_KEY_LEFT; break;
				case GLFW_KEY_RIGHT:
					pressedKey = NK_KEY_RIGHT; break;
				case GLFW_KEY_HOME:
					pressedKey = NK_KEY_SCROLL_START; break;
				case GLFW_KEY_END:
					pressedKey = NK_KEY_SCROLL_END; break;
				case GLFW_KEY_PAGE_DOWN:
					pressedKey = NK_KEY_SCROLL_DOWN; break;
				case GLFW_KEY_PAGE_UP:
					pressedKey = NK_KEY_SCROLL_UP; break;
				default:
				{
					// Unhandled non-modified key presses are ignored.
				}
				}
			}

			else if (currentNode->input->key->modifiers == (GLFW_MOD_SHIFT & GLFW_MOD_CONTROL))
			{
				switch (currentNode->input->key->token)
				{
				case GLFW_KEY_INSERT:
					pressedKey = NK_KEY_TEXT_RESET_MODE; break;
				default:
				{
					// Unhandled SHIFT+CTRL modified key presses are re-sent as non-modified key presses.
					_nuklearbase_keyCallback(state->glfwWindowContext, currentNode->input->key->token,
						currentNode->input->key->scancode,
						(currentNode->input->key->action == GLFW_RELEASE ? 0 : 1), 0);
				}
				}
			}

			else
			{
				// Unhandled modified key of all sorts presses are re-sent as non-modified key presses.
				_nuklearbase_keyCallback(state->glfwWindowContext, currentNode->input->key->token,
					currentNode->input->key->scancode,
					(currentNode->input->key->action == GLFW_RELEASE ? 0 : 1), 0);
			}

			if (pressedKey >= 0)
			{
				if (currentNode->input->key->action == GLFW_REPEAT)
				{
					nk_input_key(&state->nuklearContext, (nk_keys)pressedKey, 0);
					nk_input_key(&state->nuklearContext, (nk_keys)pressedKey, 1);
				}
				else
				{
					nk_input_key(&state->nuklearContext, (nk_keys)pressedKey, (currentNode->input->key->action == GLFW_RELEASE ? 0 : 1));
				}
			}
			else
			{
				// This is for key presses not handled.
			}
			free(currentNode->input->key);
		}

		else if (currentNode->input->text != NULL)
		{
			nk_input_unicode(&state->nuklearContext, currentNode->input->text->codepoint);
			free(currentNode->input->text);
		}

		else if (currentNode->input->cursor != NULL)
		{
			state->lastMouseX = currentNode->input->cursor->xPosition;
			state->lastMouseY = currentNode->input->cursor->yPosition;
			nk_input_motion(&state->nuklearContext, (int)state->lastMouseX, (int)state->lastMouseY);
			free(currentNode->input->cursor);
		}

		else if (currentNode->input->mouse != NULL)
		{
			int mouseButton = -1;
			switch (currentNode->input->mouse->button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
				mouseButton = NK_BUTTON_LEFT; break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				mouseButton = NK_BUTTON_MIDDLE; break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				mouseButton = NK_BUTTON_RIGHT; break;
			case GLFW_MOUSE_BUTTON_5:
			{
				nk_input_key(&state->nuklearContext, NK_KEY_SCROLL_UP, 1);
				nk_input_key(&state->nuklearContext, NK_KEY_SCROLL_UP, 0);
			} break;
			case GLFW_MOUSE_BUTTON_4:
			{
				nk_input_key(&state->nuklearContext, NK_KEY_SCROLL_DOWN, 1);
				nk_input_key(&state->nuklearContext, NK_KEY_SCROLL_DOWN, 0);
			} break;
			default:
			{
				// Unhandled mouse button.
			} break;
			}
			if (mouseButton != -1)
			{
				int action = currentNode->input->mouse->action == GLFW_PRESS ? 1 : 0;
				nk_input_button(&state->nuklearContext, NK_BUTTON_LEFT, (int)state->lastMouseX, (int)state->lastMouseY, action);
			}
			free(currentNode->input->mouse);
		}

		else if (currentNode->input->scroll != NULL)
		{
			struct nk_vec2 scroll;
			scroll.x = (float)currentNode->input->scroll->x;
			scroll.y = (float)currentNode->input->scroll->y;
			nk_input_scroll(&state->nuklearContext, scroll);
			free(currentNode->input->scroll);
		}

		state->firstUnhandledInput = state->firstUnhandledInput->next;
		free(currentNode->input);
		free(currentNode);
		currentNode = NULL;
	}
	state->lastUnhandledInput = NULL;
}

#define NUKLEARBASE_H
#endif