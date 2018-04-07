#pragma once

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class COneFile
{
public:
	COneFile();
	~COneFile();

public:
	bool ReadIt(const std::string &path);
		
	const vector<string> &getFileLines()const;
private:
	std::string mFilePath;
	std::vector<std::string> mFileLines;
	std::ifstream mInFileStream;
};

