#pragma once
#include <vector>
#include <memory>

#include <zlib/deflate.h>
#ifdef _DEBUG
#pragma comment(lib, "zlib_debug.lib")
#else
#pragma comment(lib, "zlib.lib")
#endif

enum deserialization_result
{
	SUCCESS,
	FILE_NOT_COMPRESSED,
	INVALID_FILE,
	FILE_EMPTY
};

//also based on xeniks gsc-tool's asset struct

struct RawFile
{
	using ptr = std::unique_ptr<RawFile>;

	std::string name;
	uint32_t compressed_len;
	uint32_t len;
	std::vector<uint8_t> buffer;
	std::vector<uint8_t> bytecode;

	deserialization_result deserialize(std::vector<uint8_t> const& data);
	std::vector<uint8_t> inflate();
};
