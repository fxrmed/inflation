// inflation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>

#include <zlib/deflate.h>

#ifdef _DEBUG
#pragma comment(lib, "zlib_debug.lib")
#else
#pragma comment(lib, "zlib.lib")
#endif

namespace fs = std::filesystem;

#define VERSION 1.0

void do_ascii_print()
{
	std::cout <<
		R"(                                          
.__        _____.__          __  .__               
|__| _____/ ____\  | _____ _/  |_|__| ____   ____  
|  |/    \   __\|  | \__  \\   __\  |/  _ \ /    \ 
|  |   |  \  |  |  |__/ __ \|  | |  (  <_> )   |  \
|__|___|  /__|  |____(____  /__| |__|\____/|___|  /
        \/                \/                    \/ )" << '\n';


	printf("v%.2f - debug\n", VERSION);
}

int main(int argc, const char* argv[])
{
	do_ascii_print();

	if (argc < 3)
	{
		std::cout << "usage: inflation <path> <output_path>" << std::endl;
		return 1;
	}
	else if (argc > 3)
	{
		std::cerr << "ERROR: too many arguments (could be spaces in the paths, if so, use citations)" << std::endl;
		std::cout << "usage: inflation <path>" << std::endl;
		return 1;
	}

	auto path = std::string(argv[1]);
	auto output = std::string(argv[2]);

	std::cout << "got path: " << path << std::endl;
	std::cout << "iterating path for files" << std::endl;
}

//basically copied from https://github.com/xensik/gsc-tool/blob/dev/src/tool/main.cpp so credit to xensik

void iterate_files(const std::string& path)
{
	if (fs::is_directory(path))
	{
		for (auto const& entry : fs::recursive_directory_iterator(path))
		{
			if (entry.is_regular_file() && entry.path().extension() != ".stack")
			{
				auto rel = fs::relative(entry, path).remove_filename();

				//do something
			}
		}
	}
	else if (fs::is_regular_file(path))
	{

		//do something
	}
	else
	{
		std::cerr << "ERROR: bad path: " << path << std::endl;
	}
}