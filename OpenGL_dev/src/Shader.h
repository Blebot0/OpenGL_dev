#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "Renderer.h"
#include "glm/glm.hpp"
/***************** Structure to define string of Vertex and Fragment Shaders ***************/
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
public:
	
	Shader(const std::string& filepath);

	~Shader();


	void Bind() const;
	void Unbind() const;

	// Set Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int value);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
private:

	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	unsigned int m_RendererID;
	int GetUniformLocation(const std::string& name);

	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

