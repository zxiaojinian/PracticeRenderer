#pragma once

#include "Core/Common.h"

#include <string>
#include <initializer_list>
#include <vector>
#include <memory>

namespace PR
{
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;
		static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}

