#include "Window.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

Window::GLFWInit::GLFWInit()
{
	if( !glfwInit() )
		throw std::runtime_error( "glfwInit() failed" );
}

Window::GLFWInit::~GLFWInit()
{
	glfwTerminate();
}

Window::Window( int width, int height, const std::string& title, int samples ) :
	m_initializer()
{
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	glfwWindowHint( GLFW_SAMPLES, samples );

	m_handle = glfwCreateWindow( width, height, title.c_str(), nullptr, nullptr );
	if( !m_handle )
		throw std::runtime_error( "glfwCreateWindow() failed" );

	glfwMakeContextCurrent( m_handle );

	glewExperimental = GL_TRUE;

	if( glewInit() != GLEW_OK )
	{
		glfwDestroyWindow( m_handle ); // if we throw, the destructor won't be called, so we must encapsulate it into RAII class or destroy it now
		throw std::runtime_error( "glewInit() failed" );
	}

	glfwSetWindowUserPointer( m_handle, this );

	glfwSetCursorPosCallback( m_handle, Window::mouseMovedCallback );
	glfwSetKeyCallback( m_handle, Window::keyCallback );
	glfwSetMouseButtonCallback( m_handle, Window::mouseButtonCallback );
	glfwSetScrollCallback( m_handle, Window::scrollCallback );
	//glfwSetCharCallback( m_handle, Window::textInputCallback );
	glfwSetCharModsCallback( m_handle, Window::textInputCallback );
	glfwSetWindowSizeCallback( m_handle, Window::resizeCallback );
}

Window::~Window()
{
	if( m_handle )
		glfwDestroyWindow( m_handle );
}

void Window::run()
{
	float time = glfwGetTime();
	float oldTime = time;

	int fps = 0;
	float lastSecond = 0.0f;

	while( !glfwWindowShouldClose( m_handle ) )
	{
		glfwPollEvents();

		float t = glfwGetTime();
		float deltaTime = t - oldTime;
		oldTime = t;

		processEvents( deltaTime );
		update( deltaTime );
		render( deltaTime );

		glfwSwapBuffers( m_handle );

		fps++;
		lastSecond += deltaTime;

		if( lastSecond > 1.0f )
		{
			std::cout << fps << std::endl;
			fps = 0;
			lastSecond -= 1.0f;
		}
	}
}

void Window::close()
{
	glfwSetWindowShouldClose( m_handle, 1 );
}

bool Window::isKeyPressed( int key ) const
{
	return ( glfwGetKey( m_handle, key ) == GLFW_PRESS );
}

bool Window::isButtonPressed( int button ) const
{
	return ( glfwGetMouseButton( m_handle, button ) == GLFW_PRESS );
}

GLFWwindow* Window::getHandle() const
{
	return m_handle;
}

void Window::mouseMovedCallback( GLFWwindow* window, double x, double y )
{
	Window* app = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );
	app->mouseMoved( x, y );
}

void Window::keyCallback( GLFWwindow* window, int key, int, int action, int )
{
	Window* app = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );

	if( action == GLFW_PRESS )
		app->keyPressed( key );
	else
		app->keyReleased( key );
}

void Window::mouseButtonCallback( GLFWwindow* window, int button, int action, int )
{
	Window* app = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );

	if( action == GLFW_PRESS )
		app->mousePressed( button );
	else
		app->mouseReleased( button );
}

void Window::scrollCallback( GLFWwindow*, double, double )
{
	//Window* app = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );
}

void Window::textInputCallback( GLFWwindow*, unsigned int, int )
{
	//Window* app = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );*/
}

void Window::resizeCallback( GLFWwindow*, int, int )
{
	//Window* app = reinterpret_cast< Window* >( glfwGetWindowUserPointer( window ) );
}

void Window::processEvents( float )
{

}

void Window::update( float )
{

}

void Window::render( float )
{

}

void Window::mouseMoved( float, float )
{

}

void Window::keyPressed( int )
{

}

void Window::keyReleased( int )
{

}

void Window::mousePressed( int )
{

}

void Window::mouseReleased( int )
{

}