#include "pch.h"
#include "OpenGLBuffer.h"

#include "Core/Common.h"

namespace PR
{
	OpenGLBuffer::OpenGLBuffer(uint32_t count, uint32_t stride, BufferType bufferType, BufferUsage bufferUsage)
		: m_BufferType(bufferType), m_Count(count), m_Stride(stride)
	{
		m_OpenGLBufferType = GL_UNIFORM_BUFFER;
		if (m_BufferType == BufferType::StorageBuffer)
		{
			m_OpenGLBufferType = GL_SHADER_STORAGE_BUFFER;
		}

		GLenum usage;
		switch (bufferUsage)
		{
		case PR::BufferUsage::Static:
			usage = GL_STATIC_DRAW;
			break;
		case PR::BufferUsage::Dynamic:
			usage = GL_DYNAMIC_DRAW;
			break;
		default:
			usage = GL_DYNAMIC_DRAW;
			break;
		}
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(m_OpenGLBufferType, m_RendererID);
		glBufferData(m_OpenGLBufferType, static_cast<GLsizeiptr>(count) * stride, nullptr, usage);
		glBindBuffer(m_OpenGLBufferType, 0);
	}

	OpenGLBuffer::~OpenGLBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLBuffer::SetData(const void* data, uint32_t offset, uint32_t count, uint32_t elementSize)
	{
		PR_ASSERT((count * elementSize <= m_Count * m_Stride), "The data exceeds the maximum capacity.")
		glBindBuffer(m_OpenGLBufferType, m_RendererID);
		glBufferSubData(m_OpenGLBufferType, offset, static_cast<GLsizeiptr>(count) * elementSize, data);
		glBindBuffer(m_OpenGLBufferType, 0);
	}

	void OpenGLBuffer::Bind(uint32_t bindingPoint)
	{
		m_BindingPoint = bindingPoint;
		glBindBufferBase(m_OpenGLBufferType, bindingPoint, m_RendererID);
	}
}
