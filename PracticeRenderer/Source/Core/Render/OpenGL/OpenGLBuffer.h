#pragma once

#include "Core/Render/Buffer.h"

#include <glad/glad.h>

namespace PR
{
	class OpenGLBuffer : public Buffer
	{
	public:
		OpenGLBuffer(uint32_t count, uint32_t stride, BufferType bufferType, BufferUsage bufferUsage);
		virtual ~OpenGLBuffer() override;

		virtual void SetData(const void* data, uint32_t offset, uint32_t count, uint32_t elementSize) override;
		virtual void Bind(uint32_t bindingPoint = 0) override;
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual uint32_t GetCount() const override { return m_Count; };
		virtual BufferType GetBufferType() const override { return m_BufferType; }
		virtual uint32_t GetBindingPoint() const override { return m_BindingPoint; }

	private:
		uint32_t m_Count, m_Stride;
		uint32_t m_RendererID = -1;
		BufferType m_BufferType;
		GLenum m_OpenGLBufferType;
		uint32_t m_BindingPoint = -1;
	};
}


