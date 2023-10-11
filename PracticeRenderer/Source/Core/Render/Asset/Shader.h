#pragma once

#include "ShaderDefine.h"
#include "Core/Render/Texture.h"
#include "Core/Render/Cubemap.h"
#include "Core/Render/Buffer.h"
#include "Core/Render/RenderState/RenderStateBlock.h"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <memory>
#include <any>

namespace PR
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		bool HaveProperty(const std::string& property);
		void UploadProperty(const std::unordered_map<std::string, PropertyValue>& materialValue);
		void SetRenderState();
		RenderStateBlock& GetRenderStateBlock() { return m_RenderStateBlock; }

		static void SetInt(const std::string& name, int value);
		static void SetFloat(const std::string& name, float value);
		static void SetFloat2(const std::string& name, const glm::vec2& value);
		static void SetFloat3(const std::string& name, const glm::vec3& value);
		static void SetFloat4(const std::string& name, const glm::vec4& value);
		static void SetMat4(const std::string& name, const glm::mat4& value);
		static void SetTexture(const std::string& name, Texture* value);
		static void SetCubemap(const std::string& name, Cubemap* value);
		static void SetBuffer(const std::string& name, Buffer* value);
		//static void SetIntArray(const std::string& name, int* values, uint32_t count);

		static std::shared_ptr<Shader> Create(const std::string& filepath);
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	protected:
		virtual void CollectProperty() = 0;
		virtual void CollectRenderState() = 0;

		virtual void UploadInt(const std::string& name, int value) = 0;
		virtual void UploadFloat(const std::string& name, float value) = 0;
		virtual void UploadFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void UploadFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void UploadFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void UploadMat4(const std::string& name, const glm::mat4& value) = 0;
		//virtual void UploadIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void UploadBuffer(const std::string& name, Buffer* value) = 0;

	protected:
		RenderStateBlock m_RenderStateBlock;
		std::vector<PropertyData> m_PropertyData;
		static std::unordered_map<std::string, PropertyValue> s_PropertyValue;

	};
}

