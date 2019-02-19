// Convert_VB3_Program.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


struct GSiProgramStruct
{
	int GSiID;
	int Version;
	char ProgramName[128];
	char ProgramDesc[128];
	char CategoryName[32];
	struct ParameterStruct
	{
		char ParamID[16];
		int ParamValueI;
		float ParamValueF;
	} Parameter[256]; // Max number of parameters that a Program can store
	unsigned char reserved[256];
} Temporary_Program{}, Clipboard_Program{}, VB3_Program[128] = {};


namespace pcl
{
	// These two functions are for parsing the command line 
	// (src: http://stackoverflow.com/questions/865668/parse-command-line-arguments)
	char* getCmdOption(char ** begin, char ** end, const std::string & option)
	{
		char ** itr = std::find(begin, end, option);
		if (itr != end && ++itr != end) { return *itr; }
		return 0;
	}
	bool cmdOptionExists(char** begin, char** end, const std::string& option)
	{
		return std::find(begin, end, option) != end;
	}
};

void printUsage(char *execname)
{
	printf("\n%s by Guido Scognamiglio\n", execname);
	printf("Extract single Programs from Bank file\n\n");
	printf("Usage:\n%s -e {bank_file}\n", execname);
	printf("\n");
}

bool Convert(char *BankFileName)
{
	FILE *inFile(NULL);
	size_t fileSize(0);

	inFile = fopen(BankFileName, "rb");
	if (inFile)
	{
		// Get the total file size
		fseek(inFile, 0, SEEK_END);
		fileSize = ftell(inFile);
		rewind(inFile);

		// Get bank file into memory
		if (fileSize)
		{
			fread(&VB3_Program, sizeof(VB3_Program), 1, inFile);
		}
		fclose(inFile);

		for (int i=0; i<128; ++i)
		{
			char ProgramFileName[260];
			sprintf(ProgramFileName, "%03d - %s.VB3Program", i, VB3_Program[i].ProgramName);

			FILE *outFile = fopen(ProgramFileName, "wb");
			if (outFile)
			{
				fwrite(&VB3_Program[i], sizeof(GSiProgramStruct), 1, outFile);
				fclose(outFile);
			}
		}
	}
	
	return true;
}


int main(int argc, char** argv)
{
	// Check that a file name has been provided
	if (argc < 2)
	{
		printUsage(argv[0]);
		return 0; //goto quit;
	}

	char BankFile[260];

	if (pcl::cmdOptionExists(argv, argv + argc, "-e"))
	{
		strcpy(BankFile, pcl::getCmdOption(argv, argv + argc, "-e"));

		// Check that a file has been specified
		if (!strlen(BankFile))
		{
			printf("ERROR: Please specify the name of a valid .VB3Bank file!\n");
			printUsage(argv[0]);
			return 0; //goto quit;
		}

		// Check file extension
		if (!strstr(BankFile, ".VB3Bank"))
		{
			printf("ERROR: A valid VB3 Bank file must have the .VB3Bank extension!\n");
			printUsage(argv[0]);
			return 0; //goto quit;
		}

		// Start conversion
		if (Convert(BankFile)) return 0;
	}


    return 0;
}

