#pragma once

#include <iostream>
#include <filesystem>
#include "rawfile.h"
#include <fstream>

namespace fs = std::filesystem;

class inflation
{
public:
	inflation(const std::string& in, const std::string& out, const bool write_empty_files = true);
	bool run();
private:
	bool iterate_files();
	bool handle_file(const fs::path& file);
	std::string in;
	std::string out;
	bool write_empty_files;
};