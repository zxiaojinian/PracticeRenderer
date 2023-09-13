#pragma once

namespace PR
{
	class SSBO
	{
		virtual uint32_t GetRendererID() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
	};
}

