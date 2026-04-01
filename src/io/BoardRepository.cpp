#include "BoardRepository.h"

#include <algorithm>
#include <filesystem>

void BoardRepository::getAllBoardFileNames(std::vector<std::string>& vec_to_fill) 
{
	namespace fs = std::filesystem;

	for (const auto& entry : fs::directory_iterator(fs::current_path())) 
	{
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();

		if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".screen") 
			vec_to_fill.push_back(filenameStr);
	}

	std::sort(vec_to_fill.begin(), vec_to_fill.end());
}

