#pragma once
#include <string>
#include<unordered_map>
#include <QMatrix4x4>
#include "GLErrorMessage.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string  FragmentSource;
};

class Shader:public QOpenGLFunctions_3_3_Core
{
private:
	const std::string m_FilePath;
	unsigned int m_RendererID;
	//TODO::cachig for uniforms
	std::unordered_map<std::string, int>m_UniformLocationCache;
private:
	 int GetUniformLocation(const std::string &name);
	unsigned int ComplieShader(unsigned int type, const std::string &source);
	ShaderProgramSource  ParseShader(const std::string &filepath);
	unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);

public:
	Shader(const std::string &filepath);
	~Shader();
	void Bind() ;
	void Unbind() ;

	//set uniforms
	void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string &name, float value);
	void SetUniform1i(const std::string &name, int value);

	void SetUniformMat4f(const std::string &name, const QMatrix4x4& matrix);

};