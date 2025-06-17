#include <acpch.h>
#include "Util/util.h"
#include <iostream>
#include <fstream>
namespace ac
{
	namespace util
	{
        std::string ReadFile(const std::string& path)
        {
            std::ifstream file(path, std::ios::in | std::ios::binary);
            if (!file)
            {
                std::cerr << "Failed to open file: " << path << std::endl;
                return "";
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }
	}
}