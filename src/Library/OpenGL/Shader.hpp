#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace GL
{
	class Program;

	class Shader
	{
	public:
		Shader();
		Shader( GLenum type, const std::string& fileName, const std::string& source );
		~Shader();

		Shader( Shader&& copy );
		Shader& operator=( Shader&& copy );

		void setSource( const std::string& source );
		void compile();

		void setProgram( Program& program );

		GLuint getHandle() const;

	private:
		std::string m_fileName;
		GLuint m_handle;
		Program* m_program;
	};

	class Program
	{
	public:
		Program();
		~Program();

		Program( Program&& copy );
		Program& operator=( Program&& copy );

		void attach( Shader& shader );
		void detach( Shader& shader );

		void link();
		void use() const;

		GLint getUniformLocation( const std::string& name ) const;

	private:
		GLuint m_handle;
	};
}

class Shader
{
public:
	Shader( std::vector< GL::Shader > shaders );

	Shader( Shader&& copy );
	Shader& operator=( Shader&& copy );

	void bind() const;

	void setUniform( const std::string& name, const glm::mat4& value ) const;
	void setUniform( const std::string& name, const glm::mat3& value ) const;
	void setUniform( const std::string& name, const glm::vec4& value ) const;
	void setUniform( const std::string& name, const glm::vec3& value ) const;
	void setUniform( const std::string& name, const glm::vec2& value ) const;
	void setUniform( const std::string& name, float value ) const;
	void setUniform( const std::string& name, int value ) const;
	void setUniform( const std::string& name, bool value ) const;

	void setUniform( const std::string& name, const std::vector< glm::vec3 >& values ) const;

private:
	GL::Program m_program;
};
#endif