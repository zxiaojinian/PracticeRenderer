#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <memory>
#include <any>

namespace PR
{
	enum class PropertyType
	{
		Unknown,
		Property_Int,
		Property_Float,
		Property_Float2,
		Property_Float3,
		Property_Float4,
		Property_Mat4,
		Property_Texture2D,
	};

	struct PropertyData
	{
		PropertyType Type;
		std::string Name;
	};

	struct PropertyValue
	{
		PropertyType Type;
		std::any Value;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		bool HaveProperty(const std::string& property);

		virtual const std::string& GetName() const = 0;

		void UploadProperty(std::unordered_map<std::string, PropertyValue>& materialValue);

		static void SetInt(const std::string& name, int value);
		static void SetFloat(const std::string& name, float value);
		static void SetFloat2(const std::string& name, const glm::vec2& value);
		static void SetFloat3(const std::string& name, const glm::vec3& value);
		static void SetFloat4(const std::string& name, const glm::vec4& value);
		static void SetMat4(const std::string& name, const glm::mat4& value);
		//static void SetIntArray(const std::string& name, int* values, uint32_t count);

		static std::shared_ptr<Shader> Create(const std::string& filepath);
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	protected:
		virtual void CollectProperty() = 0;

		virtual void UploadInt(const std::string& name, int value) = 0;
		virtual void UploadFloat(const std::string& name, float value) = 0;
		virtual void UploadFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void UploadFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void UploadFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void UploadMat4(const std::string& name, const glm::mat4& value) = 0;
		//virtual void UploadIntArray(const std::string& name, int* values, uint32_t count) = 0;

	protected:
		std::vector<PropertyData> m_PropertyData;
		static std::unordered_map<std::string, PropertyValue> s_PropertyValue;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		void Add(const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> Load(const std::string& filepath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}

