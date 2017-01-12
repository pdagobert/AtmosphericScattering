#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>

class Camera
{
public:
	Camera( int width, int height, const glm::dvec3& position = glm::dvec3( 0.0 ), double speed = 1.0, float near = 0.1f, float far = 1000.0f );

	void moveLeft( float deltaTime );
	void moveRight( float deltaTime );
	void moveForward( float deltaTime );
	void moveBackward( float deltaTime );
	void moveUp( float deltaTime );
	void moveDown( float deltaTime );

	void orientate( float x, float y );

	void setSpeed( double speed );

	//void setNear( float near );
	//void setFar( float far );

	glm::mat4 getMatrix() const;
	glm::mat4 getAbsMatrix() const;
	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;

	const glm::dvec3& getPosition() const;
	double getSpeed() const;

private:
	void updateViewMatrix();
	void updateProjectionMatrix();

	void updateOrientation();

	glm::dvec3 getRightVector() const;
	glm::vec3 getUpVector() const;

private:
	glm::dvec3 m_position;
	glm::dvec3 m_direction;
	
	double m_speed;

	float m_pitch, m_yaw;

	int m_width, m_height;
	float m_near, m_far;

	glm::mat4 m_viewMatrix, m_absViewMatrix, m_projectionMatrix;
};
#endif