#ifndef __TWEAK_BAR_hpp
#define __TWEAK_BAR_HPP

#include <glm/glm.hpp>

class Window;
struct nk_context;

struct Parameters
{
	int wireframe = 1;
	int normalsFilter = 2;
	//int showAABB = 1;

	int currentDebugTexture = 0;
	float debugTextureScale = 1.0f;

	float sunAngle = 0.0f;

	int applyToneMapping = 1;
	int applyGamma = 1;

	glm::vec3 betaR = glm::vec3( 5.8e-8, 13.5e-8, 33.1e-8 );
	float betaM = 40e-8;
};

class TweakBar
{
public:
	TweakBar( const Window& window );
	~TweakBar();

	void newFrame();
	void update( int numNodes, double altitude, double speed );
	void render();

	bool isActive();

private:
	nk_context* m_context;

public:
	Parameters parameters;
};
#endif