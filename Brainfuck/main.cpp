//https://en.wikipedia.org/wiki/Brainfuck
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "Brainfuck.h"
#include "MindBlown.h"

//cut off the .mb extension, append .bf, return
std::string Rename(std::string name)
{
	name = name.substr(0, name.length() - 3);
	name += ".bf";
	return name;
}

bool EndsWith(std::string const &fullString, std::string const &ending) 
{
	if (fullString.length() >= ending.length()) 
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else 
	{
		return false;
	}
}

void PrintUsage()
{
	std::cout << "Usage: brainfuck [-b] filename" << std::endl;
	std::cout << "If filename ends with .b or .bf, it will be run as Brainfuck source." << std::endl;
	std::cout << "If filename ends with .mb, it will be compiled as Mind Blown source, then run." << std::endl;
	std::cout << "If the -b flag is given, it will dump the compiled Brainfuck source from a Mind Blown file before running it" << std::endl;
	std::cout << "-b means nothing if the original source was brainfuck." << std::endl;
	std::cout << "For documentation, please view readme at https://github.com/CodyIsAwesome/Brainfuck" << std::endl;
}
//adfbjcl
std::string GetOpt(int argc, char** argv)
{
	//scans argv for any arguments starting with '-'
	//assemble letters that follow into one string, and return that
	//super string for the caller to scan more easily

	//start at 1 to skip the filename in argv[0]
	std::string suppliedOptions = "";
	for (int i = 1; i < argc; i++)
	{
		std::string thisArg = argv[i];
		if (thisArg[0] != '-')
		{
			continue;
		}
		//start at 1 to skip the '-'
		for (int j = 1; j < thisArg.length(); j++)
		{
			suppliedOptions += thisArg[j];
		}
	}
	return suppliedOptions;
}

int main(int argc, char** argv)
{	
	if (argc == 1) //no options, print usage
	{
		PrintUsage();
		exit(0);
	}
	else //parse check for flag, run arg as filename
	{
		std::string options = GetOpt(argc, argv);
		if (options.size() > 1)
		{
			Crash("Unknown options were given.  Only the -b option is supported.");
		}
		int filenamePos = 1;
		bool dumpCompiledBrainfuck = false;
		int bPos = options.find("b");
		if (bPos != std::string::npos)
		{
			filenamePos = 2;
			dumpCompiledBrainfuck = true;
		}
		std::string filename(argv[filenamePos]);
		std::ifstream file;
		file.open(filename, std::ifstream::in);
		if (file.is_open())
		{
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::cout << "Now running " << filename << ":\n";
			if (EndsWith(filename, ".b") || EndsWith(filename, ".bf"))
			{
				Run(buffer.str());
			}
			else if (EndsWith(filename, ".mb"))
			{
				if (dumpCompiledBrainfuck)
				{
					std::string bf = Compile(buffer.str());
					std::ofstream outfile;
					outfile.open(Rename(filename));
					outfile << bf;
					outfile.close();
					Run(bf);
				}
				else
				{
					Run(Compile(buffer.str()));
				}
			}
			else
			{
				std::cout << "Unrecognized file format: " + filename << std::endl;
				std::cout << "Expecting filenames that end in .b, .bf, or .mb" << std::endl;
				file.close();
				exit(1);
			}
		}
		else
		{
			std::cout << std::endl << "Error opening file " << filename << std::endl << std::endl;
		}
		file.close();
	}
}
