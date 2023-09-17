#include "pch.h"
#include "OpenGLShader.h"

#include "Core/Common.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <regex>
#include <filesystem>

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

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source;
		std::ifstream in(filePath);
		if (in)
		{
			std::string result((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			in.close();
			source = result;
		}
		else
		{
			PR_LOG_ERROR("Could not open file '{0}'", filePath);
		}

		auto shaderSources = PreProcess(source, filePath);
		Compile(shaderSources);

		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);

		CollectProperty();
		CollectRenderState();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);

		CollectProperty();
		CollectRenderState();
	}


	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}


	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source, const std::string& filePath)
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

			std::string shaderSource = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);

			shaderSource = ProcessShader(shaderSource, filePath);

			shaderSources[ShaderTypeFromString(type)] = shaderSource;
		}

		return shaderSources;
	}

	std::string OpenGLShader::ProcessShader(const std::string& shaderCode, const std::string& filePath)
	{
		std::string processedShaderCode;

		std::istringstream iss(shaderCode);
		std::string line;
		while (std::getline(iss, line))
		{
			if (line.find("#include") != std::string::npos)
			{
				// 提取路径
				std::string includePath = line.substr(line.find("\"") + 1);
				includePath = includePath.substr(0, includePath.rfind("\""));

				std::string includeFilePath = GetIncludeFilePath(includePath, filePath);
				// 读取引用文件的内容
				std::ifstream includeFile(includeFilePath);
				if (includeFile)
				{
					std::string includeFileContents((std::istreambuf_iterator<char>(includeFile)), std::istreambuf_iterator<char>());
					includeFile.close();

					// 递归处理引用文件中的引用
					std::string processedIncludeFileContents = ProcessShader(includeFileContents, includeFilePath);

					processedShaderCode += processedIncludeFileContents;
				}
				else
				{
					PR_LOG_ERROR("Could not open file '{0}'", includeFilePath);
				}
				continue;
			}

			// 将非引用行直接添加到最终的着色器代码中
			processedShaderCode += line;
			processedShaderCode += "\n";
		}

		return processedShaderCode;
	}

	std::string OpenGLShader::GetIncludeFilePath(const std::string& includePath, const std::string& filePath)
	{
		std::filesystem::path currentPath(filePath);
		std::string includeFilePath = (currentPath.parent_path() / includePath).string();

		if (!std::filesystem::exists(includeFilePath))
		{
			includeFilePath = includePath;
		}

		return includeFilePath;
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


	void OpenGLShader::CollectProperty()
	{
		// Uniform
		GLint numUniforms;
		GLint maxUniformLen;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &numUniforms);
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
		std::vector<GLchar> uniformName(maxUniformLen);

		for (GLint i = 0; i < numUniforms; ++i)
		{
			GLint size;
			GLenum type;
			glGetActiveUniform(m_RendererID, i, maxUniformLen, NULL, &size, &type, uniformName.data());
			std::string nameStr(uniformName.data());

			auto location = glGetUniformLocation(m_RendererID, uniformName.data());
			if (location == -1)
			{
				continue;
			}
			m_UniformLocation[uniformName.data()] = location;

			PropertyType propertyType;
			switch (type)
			{
			case GL_INT:
				propertyType = PropertyType::Int;
				break;
			case GL_FLOAT:
				propertyType = PropertyType::Float;
				break;
			case GL_FLOAT_VEC2:
				propertyType = PropertyType::Float2;
				break;
			case GL_FLOAT_VEC3:
				propertyType = PropertyType::Float3;
				break;
			case GL_FLOAT_VEC4:
				propertyType = PropertyType::Float4;
				break;
			case GL_FLOAT_MAT4:
				propertyType = PropertyType::Mat4;
				break;
			case GL_SAMPLER_2D:
				propertyType = PropertyType::Texture;
				break;
			default:
				propertyType = PropertyType::Unknown;
				break;
			}

			m_PropertyData.push_back({ propertyType, nameStr });
		}

		//UBO
		GLint numUniformBlocks;
		GLint maxUniformBlockLen;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxUniformBlockLen);
		std::vector<GLchar> uniformBlockName(maxUniformBlockLen);

		for (GLint i = 0; i < numUniformBlocks; ++i)
		{
			glGetActiveUniformBlockName(m_RendererID, i, maxUniformBlockLen, NULL, uniformBlockName.data());
			std::string nameStr(uniformBlockName.data());
			m_PropertyData.push_back({ PropertyType::UBO, nameStr });
		}

		//SSBO
		GLint numSSBOs;
		glGetProgramInterfaceiv(m_RendererID, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numSSBOs);

		std::vector<GLenum> props = { GL_NAME_LENGTH };

		for (int i = 0; i < numSSBOs; ++i)
		{
			std::vector<GLint> params(props.size());
			glGetProgramResourceiv(m_RendererID, GL_SHADER_STORAGE_BLOCK, i, static_cast<GLsizei>(props.size()), props.data(), static_cast<GLsizei>(props.size()), NULL, params.data());

			GLsizei nameLen = params[0];
			std::vector<GLchar> nameData(nameLen);
			glGetProgramResourceName(m_RendererID, GL_SHADER_STORAGE_BLOCK, i, nameLen, NULL, nameData.data());
			std::string nameStr(nameData.data());

			m_PropertyData.push_back({ PropertyType::SSBO, nameStr });
		}
	}

	void OpenGLShader::CollectRenderState()
	{
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

	uint32_t OpenGLShader::GetBufferIndex(const std::string& name, BufferType bufferType)
	{
		if (m_BufferIndex.find(name) != m_BufferIndex.end())
			return m_BufferIndex[name];

		uint32_t index = -1;
		switch (bufferType)
		{
		case BufferType::UniformBuffer:
			index = glGetUniformBlockIndex(m_RendererID, name.c_str());
			break;
		case BufferType::StorageBuffer:
			index = glGetProgramResourceIndex(m_RendererID, GL_SHADER_STORAGE_BLOCK, name.c_str());
			break;
		}
		if (index == -1)
			PR_LOG_WARN("Shader " + m_Name + "doesn't have buffer" + name);
		m_BufferIndex[name] = index;
		return index;
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

	void OpenGLShader::UploadBuffer(const std::string& name, Buffer* value)
	{
		if (value)
		{
			if (value->GetBindingPoint() == -1)
			{
				PR_LOG_ERROR("buffer bindingpoint is error!");
				return;
			}
			GLuint index = GetBufferIndex(name, value->GetBufferType());
			if (value->GetBufferType() == BufferType::UniformBuffer)
			{
				glUniformBlockBinding(m_RendererID, index, value->GetBindingPoint());
			}
			else if (value->GetBufferType() == BufferType::StorageBuffer)
			{
				glShaderStorageBlockBinding(m_RendererID, index, value->GetBindingPoint());
			}			
		}
	}

	//void OpenGLShader::UploadIntArray(const std::string& name, int* values, uint32_t count)
	//{
	//	auto location = GetUniformLocation(name);
	//	glUniform1iv(location, count, values);
	//}
}
