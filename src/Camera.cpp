#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( int width, int height, const glm::dvec3& position, double speed, float near, float far ) :
	m_position( position ),
	m_speed( speed ),
	m_pitch( 0.0f ),
	m_yaw( 0.0f ),
	m_width( width ),
	m_height( height ),
	m_near( near ),
	m_far( far )
{
	updateOrientation();
	updateViewMatrix();
	updateProjectionMatrix();
}

void Camera::moveLeft( float deltaTime )
{
	m_position += -glm::dvec3( getRightVector() ) * (double)deltaTime * m_speed;
	updateViewMatrix();
}

void Camera::moveRight( float deltaTime )
{
	m_position += glm::dvec3( getRightVector() ) * (double)deltaTime * m_speed;
	updateViewMatrix();
}

void Camera::moveForward( float deltaTime )
{
	m_position += m_direction * (double)deltaTime * m_speed;
	
	updateViewMatrix();
}

void Camera::moveBackward( float deltaTime )
{
	m_position += -glm::dvec3( m_direction ) * (double)deltaTime * m_speed;
	updateViewMatrix();
}

void Camera::moveUp( float deltaTime )
{
	m_position += glm::dvec3( 0.0, 1.0, 0.0 ) * (double)deltaTime * m_speed;
	updateViewMatrix();
}

void Camera::moveDown( float deltaTime )
{
	m_position += -glm::dvec3( 0.0, 1.0, 0.0 ) * (double)deltaTime * m_speed;
	updateViewMatrix();
}

void Camera::orientate( float x, float y )
{
	m_pitch = glm::clamp( m_pitch + y, -89.0f, 89.0f );
	m_yaw += x;

	updateOrientation();
}
#include <iostream>
void Camera::updateOrientation()
{
	double pitch = glm::radians( (double)m_pitch );
	double yaw = glm::radians( (double)m_yaw );

	m_direction.x = glm::cos( pitch ) * glm::cos( yaw );
	m_direction.y = glm::sin( pitch );
	m_direction.z = glm::cos( pitch ) * glm::sin( yaw );

	m_direction = glm::normalize( m_direction );

	updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	m_viewMatrix = glm::lookAt( glm::vec3( 0.0f ), glm::vec3( m_direction ), getUpVector() );

	glm::dvec3 direction = m_position + m_direction;
	m_absViewMatrix = glm::lookAt( glm::vec3( m_position ), glm::vec3( direction ), getUpVector() );
}

void Camera::updateProjectionMatrix()
{
	float aspect = static_cast< float >( m_width ) / m_height;
	m_projectionMatrix = glm::perspective( glm::radians( 45.0f ), aspect, m_near, m_far );
}

void Camera::setSpeed( double speed )
{
	m_speed = speed;
}

glm::dvec3 Camera::getRightVector() const
{
	glm::dvec3 right = glm::cross( m_direction, glm::dvec3( 0.0, 1.0, 0.0 ) );
	return glm::normalize( right );
}

glm::vec3 Camera::getUpVector() const
{
	// TODO: fix it with cross( right, dir ) or cross( dir, right )
	return glm::vec3( 0.0f, 1.0f, 0.0f );
}

glm::mat4 Camera::getMatrix() const
{
	return m_projectionMatrix * m_viewMatrix;
}

glm::mat4 Camera::getAbsMatrix() const
{
	return m_projectionMatrix * m_absViewMatrix;
}

const glm::mat4& Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

const glm::dvec3& Camera::getPosition() const
{
	return m_position;
}

double Camera::getSpeed() const
{
	return m_speed;
}