// inflation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "inflation.h"

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


	printf("v%.2f - made by form\n", VERSION);
}

void print_arguments()
{
	std::cout << "usage: inflation <path> <write_empty_files>*" << std::endl;
	std::cout << "*: Optional argument, default is false, values are true or false" << std::endl;
}

int main(int argc, const char* argv[])
{
	do_ascii_print();

	if (argc < 3)
	{
		std::cout << "usage: inflation <path> <output_path>" << std::endl;
		return 1;
	}
	else if (argc > 4)
	{
		std::cerr << "ERROR: too many arguments (could be spaces in the paths, if so, use citations)" << std::endl;
		print_arguments();
		return 1;
	}

	bool b_write_empty_files = true;

	auto input = std::string(argv[1]);
	auto output = std::string(argv[2]);

	if (argc == 4)
	{
		auto write_empty = std::string(argv[3]);

		if (write_empty.find("false") != std::string::npos)
		{
			b_write_empty_files = false;
		}
		else if (write_empty.find("true") != std::string::npos)
		{
			//do nothing
		}
		else {
			std::cerr << "ERROR: Invalid third argument, value should be true or false or there could be spaces in the paths, if so, use citations" << std::endl;
			print_arguments();
			return 1;
		}
	}

	std::cout << "input path: " << input << std::endl;
	std::cout << "output path: " << output << std::endl;
	std::cout << "write empty files: " << (b_write_empty_files ? "true" : "false") << std::endl;

	auto i = new inflation(input, output, b_write_empty_files);
	i->run();
}