#include "rawfile.h"
#include <iostream>

//also based on xeniks gsc-tool's deserialize function

deserialization_result RawFile::deserialize(std::vector<uint8_t> const& data)
{
	size_t pos = 0;

	name = std::string{ reinterpret_cast<char const*>(data.data()) };
	pos += name.size() + 1;

	compressed_len = *reinterpret_cast<uint32_t const*>(data.data() + pos);
	pos += 4;

	len = *reinterpret_cast<uint32_t const*>(data.data() + pos);
	pos += 4;

	if (len == 0)
	{
		return deserialization_result::FILE_EMPTY;
	}

	if (compressed_len == 0 || data.at(pos) != 0x78)
	{
		if ((len + name.size() + 1 + sizeof(len) * 2) != data.size())
		{
			std::cout << "RawFile::deserialize - File size do not match up for uncompressed file" << std::endl;
			return deserialization_result::INVALID_FILE;
		}

		buffer.resize(len);
		std::memcpy(buffer.data(), data.data() + pos, len);
		return deserialization_result::FILE_NOT_COMPRESSED; //not compressed or not compressed with zlib specifically
	}

	switch (data.at(pos + 1))
	{
	case 0x01:
		//printf("RawFile::deserialize - File %s using: No Compression/low\n", name.data());
		break;
	case 0x9C:
		//printf("RawFile::deserialize - File %s using: Default Compression\n", name.data());
		break;
	case 0xDA:
		//printf("RawFile::deserialize - File %s using: Best Compression\n", name.data());
		break;
	default:
		std::cout << "RawFile::deserialize - Invalid zlib magic" << std::endl;
		return deserialization_result::INVALID_FILE;
		break;
	}

	if ((compressed_len + name.size() + 1 + sizeof(compressed_len) * 2) != data.size())
	{
		std::cout << "RawFile::deserialize - File size do not match up for compressed file" << std::endl;
		return deserialization_result::INVALID_FILE;
	}

	buffer.resize(compressed_len);
	std::memcpy(buffer.data(), data.data() + pos, compressed_len);

	return deserialization_result::SUCCESS;
}

std::vector<uint8_t> RawFile::inflate()
{
	std::vector<std::uint8_t> inflated;
	inflated.resize(this->len);

	auto status = uncompress(inflated.data(), (uLongf*)&this->len, (Bytef*)this->buffer.data(), this->compressed_len);

	return inflated;
}