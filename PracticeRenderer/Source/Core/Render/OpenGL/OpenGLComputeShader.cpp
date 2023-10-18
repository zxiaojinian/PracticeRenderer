#include "pch.h"
#include "OpenGLComputeShader.h"

#include "Core/Common.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <regex>
#include <filesystem>

namespace PR
{
	OpenGLComputeShader::OpenGLComputeShader(const std::string& filePath)
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

		auto shaderSources = ProcessShader(source, filePath);
		Compile(shaderSources);

		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);

		CollectProperty();
	}

	OpenGLComputeShader::OpenGLComputeShader(const std::string& name, const std::string& src)
		: m_Name(name)
	{
		Compile(src);
		CollectProperty();
	}

	OpenGLComputeShader::~OpenGLComputeShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLComputeShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLComputeShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLComputeShader::UploadInt(const std::string& name, int value)
	{
		auto location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLComputeShader::UploadFloat(const std::string& name, float value)
	{
		auto location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLComputeShader::UploadFloat2(const std::string& name, const glm::vec2& value)
	{
		auto location = GetUniformLocation(name);
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLComputeShader::UploadFloat3(const std::string& name, const glm::vec3& value)
	{
		auto location = GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLComputeShader::UploadFloat4(const std::string& name, const glm::vec4& value)
	{
		auto location = GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLComputeShader::UploadMat4(const std::string& name, const glm::mat4& value)
	{
		auto location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLComputeShader::UploadBuffer(const std::string& name, Buffer* value)
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

	void OpenGLComputeShader::CollectProperty()
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
			bool isImage = false;
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
				case GL_IMAGE_2D:
					propertyType = PropertyType::Texture;
					isImage = true;
					break;
				case GL_SAMPLER_CUBE:
					propertyType = PropertyType::Cubemap;
					break;
				case GL_IMAGE_CUBE:
					propertyType = PropertyType::Cubemap;
					isImage = true;
					break;
				default:
					propertyType = PropertyType::Unknown;
					break;
			}

			m_PropertyData.push_back({ propertyType, nameStr, isImage });
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

	std::string OpenGLComputeShader::ProcessShader(const std::string& source, const std::string& filePath)
	{
		std::string processedShaderCode;

		std::istringstream iss(source);
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

	std::string OpenGLComputeShader::GetIncludeFilePath(const std::string& includePath, const std::string& filePath)
	{
		std::filesystem::path currentPath(filePath);
		std::string includeFilePath = (currentPath.parent_path() / includePath).string();

		if (!std::filesystem::exists(includeFilePath))
		{
			includeFilePath = includePath;
		}

		return includeFilePath;
	}

	void OpenGLComputeShader::Compile(const std::string shaderSource)
	{
		GLuint shader = glCreateShader(GL_COMPUTE_SHADER);

		const GLchar* sourceCStr = shaderSource.c_str();
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
			PR_ASSERT(false, "Shader compilation failure:\n" + shaderSource);
			return;
		}

		GLuint program = glCreateProgram();
		glAttachShader(program, shader);
		glLinkProgram(program);
		m_RendererID = program;

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
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
				glDeleteShader(shader);

				PR_LOG_ERROR("{0}", infoLog.data());
			}
			PR_ASSERT(false, "Shader link failure!");
			return;
		}
		glDetachShader(program, shader);
		glDeleteShader(shader);
	}

	uint32_t OpenGLComputeShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocation.find(name) != m_UniformLocation.end())
			return m_UniformLocation[name];

		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			PR_LOG_WARN("ComputeShader " + m_Name + "doesn't have uniform" + name);
		m_UniformLocation[name] = location;
		return location;
	}

	uint32_t OpenGLComputeShader::GetBufferIndex(const std::string& name, BufferType bufferType)
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
			PR_LOG_WARN("ComputeShader " + m_Name + "doesn't have buffer" + name);
		m_BufferIndex[name] = index;
		return index;
	}
}
