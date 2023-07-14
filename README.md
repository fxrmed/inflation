# inflation
A small tool that uncompresses / inflates RawFile assets from Call of Duty games using the IW8 engine (and others?). 

These files are compressed using zlib even though more or less all of the RawFile assets are smaller than 10mb (iirc there's like two files over 1mb).
<sup><sub>I find this hilarious because the game is still enormous in file size</sub></sup>

This tool was very hastily written and alot of the code is borrowed from [*xensik's* gsc-tool](https://github.com/xensik/gsc-tool).

## Usage
``./inflation.exe <input_path> <output_path> <write_empty_files>``

**input_path**: `file` or `directory` (recursive process all files inside the directory)

**output_path**: `file` or `directory` (recursive process all files inside the directory)

**write_empty_files**: `true`, `false`
- *note:* Optional argument, default is `true`

## File Format
The tool expects files with any file extension (or none) in the input directory to follow this file format:

- A serialized RawFile struct: <br/>
***name***: null-terminated string <br/>
***compressed_len***: 4 byte uint <br/>
***len***: 4 byte uint <br/>
***buffer***: byte array[compressed_len or len if the file is not compressed] <br/>

## Build
- Install Visual Studio (I use VS2017 but anything newer should work without issue)
- Open project
- Build the project

## Credits
This project is somewhat based on [*xensik's* gsc-tool](https://github.com/xensik/gsc-tool). So much of the credit goes to *xensik*.
