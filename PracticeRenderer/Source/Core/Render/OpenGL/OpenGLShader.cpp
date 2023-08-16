#include "pch.h"
#include "OpenGLShader.h"

#include "Core/Common.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace PR
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		PR_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);

		CollectProperty();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);

		CollectProperty();
	}


	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}


	void OpenGLShader::CollectProperty()
	{
		GLint numUniforms;
		GLint maxUniformLen;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &numUniforms);
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
		std::vector<GLchar> name(maxUniformLen);
		for (int i = 0; i < numUniforms; ++i)
		{
			GLint size;
			GLenum type;
			GLint location;
			glGetActiveUniform(m_RendererID, i, maxUniformLen, NULL, &size, &type, &name[0]);
			std::string name(name.data());

			PropertyType propertytype;
			switch (type)
			{
				case GL_INT:
				{
					propertytype = PropertyType::Property_Int;
					break;
				}
				case GL_FLOAT:
				{
					propertytype = PropertyType::Property_Float;
					break;
				}
				case GL_FLOAT_VEC2:
				{
					propertytype = PropertyType::Property_Float2;
					break;
				}

				case GL_FLOAT_VEC3:
				{
					propertytype = PropertyType::Property_Float3;
					break;
				}
				case GL_FLOAT_VEC4:
				{
					propertytype = PropertyType::Property_Float4;
					break;
				}

				case GL_FLOAT_MAT4:
				{
					propertytype = PropertyType::Property_Mat4;
					break;
				}					
				default:
					propertytype = PropertyType::Unknown;
					break;
			}

			m_PropertyData.push_back
			({
				propertytype,
				name,
			});
		}
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				PR_LOG_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			PR_LOG_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			PR_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			PR_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			PR_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{

		GLuint program = glCreateProgram();
		PR_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				PR_LOG_ERROR("{0}", infoLog.data());
				PR_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

				// We don't need the program anymore.
				glDeleteProgram(program);

				for (auto id : glShaderIDs)
					glDeleteShader(id);

				PR_LOG_ERROR("{0}", infoLog.data());
			}
			PR_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}

	uint32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocation.find(name) != m_UniformLocation.end())
			return m_UniformLocation[name];

		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			PR_LOG_WARN("Shader "+ m_Name + "doesn't have uniform" + name);
		m_UniformLocation[name] = location;
		return location;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadInt(const std::string& name, int value)
	{
		auto location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadFloat(const std::string& name, float value)
	{
		auto location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadFloat2(const std::string& name, const glm::vec2& value)
	{
		auto location = GetUniformLocation(name);
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadFloat3(const std::string& name, const glm::vec3& value)
	{
		auto location = GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadFloat4(const std::string& name, const glm::vec4& value)
	{
		auto location = GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadMat4(const std::string& name, const glm::mat4& value)
	{
		auto location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	//void OpenGLShader::UploadIntArray(const std::string& name, int* values, uint32_t count)
	//{
	//	auto location = GetUniformLocation(name);
	//	glUniform1iv(location, count, values);
	//}
}
