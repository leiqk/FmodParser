#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class CFileAnalysis
{
public:
	CFileAnalysis();
	~CFileAnalysis();

	void AddOneFileContent(const vector<string > &vecFileLines);

	bool AnalysisItem();
	
	bool WriteItemToCSharp(string sPath);

	const map<string, string> &GetGuidEvent()const
	{
		return mMapGuidEvent;
	}
	const map<string, string> &GetGuidBus()const
	{
		return mMapGuidBus;
	}
	const map<string, string> &GetGuidBank()const
	{
		return mMapGuidBank;
	}
private:
	//һ���ļ����е���������
	std::vector<std::string> mVecFileLines;

	//guid - event
	std::map<std::string, std::string> mMapGuidEvent;

	//guid - bus
	std::map<std::string, std::string> mMapGuidBus;

	//guid - bank
	std::map<std::string, std::string> mMapGuidBank;	
	
	std::ofstream mOutFileStream;
};

