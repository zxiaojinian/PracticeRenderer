#pragma once

#include "ShaderDefine.h"
#include "Core/Render/Texture.h"
#include "Core/Render/Buffer.h"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <memory>
#include <any>

namespace PR
{
	class ComputeShader
	{
	public:
		virtual ~ComputeShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		bool HaveProperty(const std::string & property);
		void UploadProperty();

		void SetInt(const std::string & name, int value);
		void SetFloat(const std::string & name, float value);
		void SetFloat2(const std::string & name, const glm::vec2 & value);
		void SetFloat3(const std::string & name, const glm::vec3 & value);
		void SetFloat4(const std::string & name, const glm::vec4 & value);
		void SetMat4(const std::string & name, const glm::mat4 & value);
		void SetTexture(const std::string & name, Texture * value);
		void SetBuffer(const std::string & name, Buffer * value);

		static void SetGlobalInt(const std::string& name, int value);
		static void SetGlobalFloat(const std::string& name, float value);
		static void SetGlobalFloat2(const std::string& name, const glm::vec2& value);
		static void SetGlobalFloat3(const std::string& name, const glm::vec3& value);
		static void SetGlobalFloat4(const std::string& name, const glm::vec4& value);
		static void SetGlobalMat4(const std::string& name, const glm::mat4& value);
		static void SetGlobalTexture(const std::string& name, Texture* value);
		static void SetGlobalBuffer(const std::string& name, Buffer* value);

		static std::shared_ptr<ComputeShader> Create(const std::string & filepath);
		static std::shared_ptr<ComputeShader> Create(const std::string & name, const std::string & src);

	protected:
		virtual void CollectProperty() = 0;

		virtual void UploadInt(const std::string & name, int value) = 0;
		virtual void UploadFloat(const std::string & name, float value) = 0;
		virtual void UploadFloat2(const std::string & name, const glm::vec2 & value) = 0;
		virtual void UploadFloat3(const std::string & name, const glm::vec3 & value) = 0;
		virtual void UploadFloat4(const std::string & name, const glm::vec4 & value) = 0;
		virtual void UploadMat4(const std::string & name, const glm::mat4 & value) = 0;
		virtual void UploadBuffer(const std::string & name, Buffer * value) = 0;

	protected:
		std::vector<PropertyData> m_PropertyData;
		std::unordered_map<std::string, PropertyValue> m_PropertyValue;
		static std::unordered_map<std::string, PropertyValue> s_PropertyValue;
	};
}

