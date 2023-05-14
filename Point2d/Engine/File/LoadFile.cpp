#include "LoadFile.h"


std::vector<uint8_t> Engine::File::LoadFile(const std::string& i_FileName)
{
		std::vector<uint8_t> Contents;

		if (! i_FileName.empty())
		{
			FILE* pFile = nullptr;

			errno_t fopenError = fopen_s(&pFile, i_FileName.c_str(), "rb");
			if (fopenError == 0)
			{
				assert(pFile != nullptr);

				int FileIOError = fseek(pFile, 0, SEEK_END);
				assert(FileIOError == 0);

				long FileSize = ftell(pFile);
				assert(FileSize >= 0);

				FileIOError = fseek(pFile, 0, SEEK_SET);
				assert(FileIOError == 0);

				Contents.reserve(FileSize);
				Contents.resize(FileSize);

				size_t FileRead = fread(&Contents[0], 1, FileSize, pFile);
				assert(FileRead == FileSize);

				fclose(pFile);
			}
		}

		return Contents;
}

