#ifndef __LOADER_HPP__
#define __LOADER_HPP__

#include "OpenGL/Shader.hpp"
#include "OpenGL/Texture.hpp"
/*#include "VertexLayout.hpp"
#include "Mesh.hpp"*/
#include <vector>
#include <string>

namespace Loader
{
	Shader loadShader( const std::string& vertexFile, const std::string& fragmentFile );

	std::string getShaderSource( const std::string fileName );
	std::string includeToSource( const std::string& source, const std::string& includeFile );

	Texture2D loadTexture2D( const std::string& fileName, const Texture2DParameters& params );

	CubeMap loadCubeMap( const std::vector< std::string >& fileNames, const CubeMapParameters& params );
	/*Texture2D loadLinearTexture2D( const std::string& fileName, const Texture2DParameters& params );

	CubeMap loadCubeMap( const std::string& path, const std::string& ext );
	Texture2D loadHdrTexture( const std::string& fileName );*/
	std::vector< float > loadRawHeightmap( const std::string& fileName, int size );

	/*Mesh loadModel( const std::string& fileName, VertexLayout layout );
	InstancedMesh loadInstancedModel( const std::string& fileName, VertexLayout layout, const std::vector< Instance >& instances );
	InstancedMesh loadInstancedModel( const std::string& fileName, VertexLayout layout, const std::vector< Instance >& instances, int meshIndex );*/
}
#endif