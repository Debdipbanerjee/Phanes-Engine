#pragma once
#include <vector>
#include <string>
#include <stdint.h>
#include <assert.h>

namespace Engine
{
	namespace File
	{
		std::vector<uint8_t> LoadFile(const std::string& i_FileName);
	}
	
}