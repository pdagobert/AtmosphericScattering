#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <string>

struct GLFWwindow;

class Window
{
public:
	Window( int width, int height, const std::string& title, int samples = 0 );
	virtual ~Window();

	void run();
	void close();

	bool isKeyPressed( int key ) const;
	bool isButtonPressed( int button ) const;

	GLFWwindow* getHandle() const;

protected:
	virtual void processEvents( float deltaTime );
	virtual void update( float deltaTime );
	virtual void render( float deltaTime );

	virtual void mouseMoved( float x, float y );
	virtual void keyPressed( int key );
	virtual void keyReleased( int key );
	virtual void mousePressed( int button );
	virtual void mouseReleased( int button );

private:
	static void mouseMovedCallback( GLFWwindow* window, double x, double y );
	static void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
	static void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
	static void scrollCallback( GLFWwindow* window, double x, double y );
	static void textInputCallback( GLFWwindow* window, unsigned int codepoint, int mods );
	static void resizeCallback( GLFWwindow* window, int width, int height );

private:
	struct GLFWInit
	{
		GLFWInit();
		~GLFWInit();
	};
	
	GLFWInit m_initializer;
	GLFWwindow* m_handle;
};
#endif