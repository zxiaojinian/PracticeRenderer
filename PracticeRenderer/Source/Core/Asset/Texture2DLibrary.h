#pragma once

#include "Core/Render/Texture2D.h"

namespace PR
{
	class Texture2DLibrary
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<Texture2D>& texture2D);
		void Add(const std::shared_ptr<Texture2D>& texture2D);
		std::shared_ptr<Texture2D> Load(const std::string& filepath);
		std::shared_ptr<Texture2D> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Texture2D> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Texture2Ds;
		std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_LoadCache;
	};
}

