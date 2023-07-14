#include "inflation.h"

//from https://github.com/xensik/gsc-tool/blob/dev/src/utils/file.cpp
std::vector<uint8_t> read_file(std::filesystem::path const& file)
{
	auto data = std::vector<uint8_t>{};

	auto stream = std::ifstream{ file, std::ios::binary };

	if (!stream.good() && !stream.is_open())
	{
		throw std::runtime_error(std::string("Could not open file: ") + file.string());
	}

	stream.seekg(0, std::ios::end);
	std::streamsize size = stream.tellg();
	stream.seekg(0, std::ios::beg);

	if (size > -1)
	{
		data.resize(static_cast<uint32_t>(size));
		stream.read(reinterpret_cast<char*>(data.data()), size);
	}

	stream.close();

	return data;
}

//<-->
auto write_data(std::filesystem::path const& file, uint8_t const* data, size_t size) -> void
{
	auto path = file;

	std::filesystem::create_directories(path.remove_filename());

	auto stream = std::ofstream{ file, std::ios::binary | std::ofstream::out };

	if (stream.is_open())
	{
		stream.write(reinterpret_cast<char const*>(data), size);
		stream.close();
	}
}


inflation::inflation(const std::string& in, const std::string& out, const bool write_empty_files)
{
	this->in = in;
	this->out = out;
	this->write_empty_files = write_empty_files;
}

bool inflation::handle_file(const fs::path& file)
{
	auto data = read_file(file);

	RawFile rawfile;
	auto result = rawfile.deserialize(data);

	if (result == deserialization_result::SUCCESS)
	{
		auto output_data = rawfile.inflate();

		//there's probably a better way of doing this
		auto file_path = file.generic_string();
		auto output_path = fs::path(file_path.replace(0, this->in.size(), this->out));

		write_data(output_path, output_data.data(), output_data.size());
		
		std::cout << "inflation::handle_file: Inflated file: " << rawfile.name << std::endl;

		return true;
	}
	else if (result == deserialization_result::FILE_NOT_COMPRESSED)
	{
		auto file_path = file.generic_string();
		auto output_path = fs::path(file_path.replace(0, this->in.size(), this->out));

		write_data(output_path, rawfile.buffer.data(), rawfile.buffer.size());

		printf("RawFile::deserialize - File %s is not compressed (?)\n", rawfile.name.data());

		return true;
	}
	else if (result == deserialization_result::FILE_EMPTY)
	{
		auto file_path = file.generic_string();
		auto output_path = fs::path(file_path.replace(0, this->in.size(), this->out));

		if (write_empty_files) write_data(output_path, (const uint8_t*)"", 0);
		

		printf("RawFile::deserialize - File %s is empty\n", rawfile.name.data());

		return true;
	}

	std::cerr << "inflation::handle_file - Invalid file: " << file.filename() << " qutting..." << std::endl;
	return false;
}

bool inflation::iterate_files()
{
	std::cout << "inflation::iterate_files - iterating path for files" << std::endl << std::endl;

	if (!std::filesystem::exists(out)) {
		std::filesystem::create_directory(out); //this should not be needed i think (too much ps4 coding lately), cba to test but i was arsed to write this comment?
		std::cout << "inflation::iterate_files - Output dir does not exist, creating directory" << std::endl;
	}

	//basically copied from https://github.com/xensik/gsc-tool/blob/dev/src/tool/main.cpp so credit to xensik

	if (fs::is_directory(this->in))
	{
		for (auto const& entry : fs::recursive_directory_iterator(this->in))
		{
			if (entry.is_regular_file())
			{
				auto file = entry.path().generic_string();

				if (!handle_file(file))
				{
					return false;
				}
			}
		}
	}
	else if (fs::is_regular_file(this->in))
	{
		auto file = fs::path(this->in).generic_string();

		if (!handle_file(file))
		{
			return false;
		}
	}
	else
	{
		std::cerr << "inflation::iterate_files - Input directory does not exist, exiting" << std::endl;
		return false;
	}

	return true;
}

bool inflation::run()
{
	return iterate_files();
}