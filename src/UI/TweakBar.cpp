#include "TweakBar.hpp"
#include "Window.hpp"
#include <GL/glew.h>
#include <glm/gtc/constants.hpp>
#include <cstring>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "Nuklear/nuklear.h"
#include "Nuklear/nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

TweakBar::TweakBar( const Window& window )
{
	m_context = nk_glfw3_init( window.getHandle(), NK_GLFW3_INSTALL_CALLBACKS );

	nk_font_atlas* atlas;
    nk_glfw3_font_stash_begin( &atlas );
	nk_glfw3_font_stash_end();
}

TweakBar::~TweakBar()
{
	nk_glfw3_shutdown();
}

void TweakBar::newFrame()
{
	nk_glfw3_new_frame();
}

void TweakBar::update( int numNodes, double altitude, double speed )
{
	nk_panel layout;

	if( nk_begin( m_context, &layout, "TweakBar", nk_rect( 50, 50, 250, 450 ), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE ) )
	{
		nk_layout_row_dynamic( m_context, 30, 1 );
		nk_label( m_context, "Global States", NK_TEXT_LEFT );

		nk_layout_row_dynamic( m_context, 30, 1 );
		nk_checkbox_label( m_context, "wireframe", &parameters.wireframe );

		nk_layout_row_dynamic( m_context, 30, 1 );
		nk_checkbox_label( m_context, "ToneMapping enabled", &parameters.applyToneMapping );

		nk_layout_row_dynamic( m_context, 30, 1 );
		nk_checkbox_label( m_context, "Gamma Correction enabled", &parameters.applyGamma );

		nk_layout_row_dynamic( m_context, 30, 2 );
		nk_label( m_context, "Sun Angle :", NK_TEXT_LEFT );
		nk_slider_float( m_context, 0.0f, &parameters.sunAngle, glm::pi< float >() * 2.0f, 0.01f );

		nk_layout_row_dynamic( m_context, 30, 2 );
		nk_label( m_context, "betaR.x :", NK_TEXT_LEFT );
		nk_slider_float( m_context, 1e-8, &parameters.betaR.x, 50e-8, 0.1e-8 );

		nk_layout_row_dynamic( m_context, 30, 2 );
		nk_label( m_context, "betaR.y :", NK_TEXT_LEFT );
		nk_slider_float( m_context, 1e-8, &parameters.betaR.y, 50e-8, 0.1e-8 );

		nk_layout_row_dynamic( m_context, 30, 2 );
		nk_label( m_context, "betaR.z :", NK_TEXT_LEFT );
		nk_slider_float( m_context, 1e-8, &parameters.betaR.z, 50e-8, 0.1e-8 );

		nk_layout_row_dynamic( m_context, 30, 2 );
		nk_label( m_context, "betaM :", NK_TEXT_LEFT );
		nk_slider_float( m_context, 1e-8, &parameters.betaM, 200e-8, 0.1e-8 );

		nk_layout_row_dynamic( m_context, 30, 1 );

		const char* NormalsFilters[] = { "Nearest", "Bilinear", "Bicubic" };
		parameters.normalsFilter = nk_combo( m_context, NormalsFilters, 3, parameters.normalsFilter, 25, nk_vec2( 200, 200 ) );

		const char* DebugTextureNames[] = { "None", "HeightMap", "NormalMap", "TangentMap" };
		parameters.currentDebugTexture = nk_combo( m_context, DebugTextureNames, 4, parameters.currentDebugTexture, 25, nk_vec2( 200, 200 ) );

		nk_layout_row_dynamic( m_context, 30, 2 );
		nk_label( m_context, "DebugTextureScale :", NK_TEXT_LEFT );
		nk_slider_float( m_context, 1.0f, &parameters.debugTextureScale, 100.0f, 1.0f );

		if( nk_tree_push( m_context, NK_TREE_TAB, "Info", NK_MAXIMIZED ) )
		{
			nk_layout_row_dynamic( m_context, 30, 1 );

			std::string str = "Nodes Count : " + std::to_string( numNodes );
			nk_label( m_context, str.c_str(), NK_TEXT_LEFT );

			nk_tree_pop( m_context );

			str = "Altitude(km) : " + std::to_string( altitude / 1e5 );
			nk_label( m_context, str.c_str(), NK_TEXT_LEFT );

			str = "Speed(km/s) : " + std::to_string( speed / 1e5 );
			nk_label( m_context, str.c_str(), NK_TEXT_LEFT ); 
		}
	}

	nk_end( m_context );
}

void TweakBar::render()
{
	nk_glfw3_render( NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER );
}

bool TweakBar::isActive()
{
	return nk_item_is_any_active( m_context );
}