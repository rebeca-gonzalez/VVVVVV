#include <fstream>
#include "Shader.h"


using namespace std;



Shader::Shader()
{
	shaderId = 0;
	compiled = false;
	g_vs =
		"   varying vec2 v_vTexcoord;"
		"	varying vec4 v_vColour;"
		"	varying vec2 v_vPosition;"
		"	uniform float u_vParams;"
		"	float vecfromxy(int xx, int yy) {"
		"		vec2 temp = v_vPosition - vec2(xx, yy);"
		"		return temp.x*temp.x + temp.y*temp.y;"
		"	}"
		"	void main() {"
		"		gl_FragColor = v_vColour * texture2D(gm_BaseTexture, v_vTexcoord);"
		"		if (vecfromxy(/*CENTER OF THE SCREEN*/) < u_vParams) {"
		"			gl_FragColor.rgb = vec3(1) - gl_FragColor.rgb;"
		"		}"
		"	}		";
}


void Shader::initFromSource(const ShaderType type, const string &source)
{
	const char *sourcePtr = source.c_str();
	GLint status;
	char buffer[512];

	switch(type)
	{
	case VERTEX_SHADER:
		shaderId = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT_SHADER:
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}
	if(shaderId == 0)
		return;
	glShaderSource(shaderId, 1, &sourcePtr, NULL);
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	compiled = (status == GL_TRUE);
	glGetShaderInfoLog(shaderId, 512, NULL, buffer);
	errorLog.assign(buffer);
}

bool Shader::initFromFile(const ShaderType type, const string &filename)
{
	string shaderSource;

	if(!loadShaderSource(filename, shaderSource))
		return false;
	initFromSource(type, shaderSource);

	return true;
}

void Shader::free()
{
	glDeleteShader(shaderId);
	shaderId = 0;
	compiled = false;
}

GLuint Shader::getId() const
{
	return shaderId;
}

bool Shader::isCompiled() const
{
	return compiled;
}

const string &Shader::log() const
{
	return errorLog;
}

bool Shader::loadShaderSource(const string &filename, string &shaderSource)
{
	ifstream fin;

	fin.open(filename.c_str());
	if(!fin.is_open())
		return false;
	shaderSource.assign(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());

	return true;
}

