#include "Shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <stdexcept>
#include <iostream>

Shader::Shader( std::vector< GL::Shader > shaders ) :
	m_program()
{
	for( GL::Shader& s : shaders )
	{
		s.compile();
		m_program.attach( s );
	}

	m_program.link();
}

Shader::Shader( Shader&& copy ) :
	m_program( std::move( copy.m_program ) )
{

}

Shader& Shader::operator=( Shader&& copy )
{
	m_program = std::move( copy.m_program );
	return *this;
}

void Shader::bind() const
{
	m_program.use();
}

void Shader::setUniform( const std::string& name, const glm::mat4& value ) const
{
	glUniformMatrix4fv( m_program.getUniformLocation( name ), 1, GL_FALSE, glm::value_ptr( value ) );
}

void Shader::setUniform( const std::string& name, const glm::mat3& value ) const
{
	glUniformMatrix3fv( m_program.getUniformLocation( name ), 1, GL_FALSE, glm::value_ptr( value ) );
}

void Shader::setUniform( const std::string& name, const glm::vec4& value ) const
{
	glUniform4fv( m_program.getUniformLocation( name ), 1, glm::value_ptr( value ) );
}

void Shader::setUniform( const std::string& name, const glm::vec3& value ) const
{
	glUniform3fv( m_program.getUniformLocation( name ), 1, glm::value_ptr( value ) );
}

void Shader::setUniform( const std::string& name, const glm::vec2& value ) const
{
	glUniform2fv( m_program.getUniformLocation( name ), 1, glm::value_ptr( value ) );
}

void Shader::setUniform( const std::string& name, float value ) const
{
	glUniform1f( m_program.getUniformLocation( name ), value );
}

void Shader::setUniform( const std::string& name, int value ) const
{
	glUniform1i( m_program.getUniformLocation( name ), value );
}

void Shader::setUniform( const std::string& name, bool value ) const
{
	setUniform( name, (int)value );
}

void Shader::setUniform( const std::string& name, const std::vector< glm::vec3 >& values ) const
{
	for( unsigned int i = 0; i < values.size(); i++ )
	{
		std::string fullName = name + "[" + std::to_string( i ) + "]";
		setUniform( fullName, values[ i ] );
	}
}

namespace GL
{
	Shader::Shader() :
		m_fileName(),
		m_handle( 0 ),
		m_program( nullptr )
	{

	}

	Shader::Shader( GLenum type, const std::string& fileName, const std::string& source ) :
		m_fileName( fileName ),
		m_handle( glCreateShader( type ) ),
		m_program( nullptr )
	{
		setSource( source );
	}

	Shader::~Shader()
	{
		if( m_handle != 0 )
		{
			if( m_program )
				m_program->detach( *this );

			glDeleteShader( m_handle );
		}
	}

	Shader::Shader( Shader&& copy ) :
		m_fileName( std::move( copy.m_fileName ) ),
		m_handle( copy.m_handle ),
		m_program( copy.m_program )
	{
		copy.m_handle = 0;
	}

	Shader& Shader::operator=( Shader&& copy )
	{
		m_fileName = std::move( copy.m_fileName );
		m_handle = copy.m_handle;
		m_program = copy.m_program;

		copy.m_handle = 0;

		return *this;
	}

	void Shader::setSource( const std::string& source )
	{
		const char* src = source.c_str();
		glShaderSource( m_handle, 1, &src, nullptr );
	}

	void Shader::compile()
	{
		glCompileShader( m_handle );

		GLint status;
		glGetShaderiv( m_handle, GL_COMPILE_STATUS, &status );

		if( !status )
		{
			int size;
			glGetShaderiv( m_handle, GL_INFO_LOG_LENGTH, &size );

			std::vector< char > error( size );
			glGetShaderInfoLog( m_handle, size, nullptr, &error[ 0 ] );

			throw std::runtime_error( m_fileName + " compilation failed :" + std::string( &error[ 0 ] ) );
		}
	}

	void Shader::setProgram( Program& program )
	{
		m_program = &program;
	}

	GLuint Shader::getHandle() const
	{
		return m_handle;
	}

	Program::Program() :
		m_handle( glCreateProgram() )
	{
		
	}

	Program::Program( Program&& copy ) :
		m_handle ( copy.m_handle )
	{
		copy.m_handle = 0;
	}

	Program& Program::operator=( Program&& copy )
	{
		m_handle = copy.m_handle;
		copy.m_handle = 0;

		return *this;
	}

	Program::~Program()
	{
		if( m_handle != 0 )
			glDeleteProgram( m_handle );
	}

	void Program::attach( Shader& shader )
	{
		glAttachShader( m_handle, shader.getHandle() );
		shader.setProgram( *this );
	}

	void Program::detach( Shader& shader )
	{
		glDetachShader( m_handle, shader.getHandle() );
	}

	void Program::link()
	{
		glLinkProgram( m_handle );

		int status;
		glGetProgramiv( m_handle, GL_LINK_STATUS, &status );

		if( !status )
		{
			int size;
			glGetProgramiv( m_handle, GL_INFO_LOG_LENGTH, &size );

			std::vector< char > error( size );
			glGetProgramInfoLog( m_handle, size, nullptr, &error[ 0 ] );

			throw std::runtime_error( "Program linking failed : + " + std::string( &error[ 0 ] ) );
		}
	}

	void Program::use() const
	{
		glUseProgram( m_handle );
	}

	GLint Program::getUniformLocation( const std::string& name ) const
	{
		GLint location = glGetUniformLocation( m_handle, name.c_str() );
		if( location == -1 )
			std::cout << "location not found : " << name << std::endl;

		return location;
	}
}