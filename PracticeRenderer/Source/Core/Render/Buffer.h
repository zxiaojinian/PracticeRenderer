#pragma once

namespace PR
{
	enum class BufferType
	{
		UniformBuffer,
		StorageBuffer
	};

	//TEMP
	enum class BufferUsage
	{
		Static,
		Dynamic,
	};

	class Buffer
	{
	public:
		virtual ~Buffer() = default;
		virtual void SetData(const void* data, uint32_t offset, uint32_t count, uint32_t elementSize) = 0;
		virtual void Bind(uint32_t bindingPoint = 0) = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetCount() const = 0;
		virtual BufferType GetBufferType() const = 0;
		virtual uint32_t GetBindingPoint() const = 0;

		static std::shared_ptr<Buffer> Create(uint32_t count, uint32_t stride, BufferType bufferType, BufferUsage bufferUsage);
	};
}

