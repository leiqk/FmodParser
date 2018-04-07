#include "stdafx.h"
#include "COneFile.h"
#include <codecvt>

COneFile::COneFile()
{
}


COneFile::~COneFile()
{
}

bool COneFile::ReadIt(const std::string & path)
{
	if (path.empty())
	{
		return false;
	}

	mInFileStream.open(path, std::ios::in);
	if (!mInFileStream.is_open())
	{
		return false;
	}

	string sLine;
	while (!mInFileStream.eof())
	{
		sLine.clear();
		std::getline(mInFileStream, sLine);
		mFileLines.push_back(sLine);
	}
	mInFileStream.close();

	return true;
}

const vector<string>& COneFile::getFileLines() const
{
	// TODO: 在此处插入 return 语句
	return mFileLines;
}
