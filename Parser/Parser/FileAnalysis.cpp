#include "stdafx.h"
#include "FileAnalysis.h"
#include <algorithm>

CFileAnalysis::CFileAnalysis()
{
}

CFileAnalysis::~CFileAnalysis()
{
}

void CFileAnalysis::AddOneFileContent(const vector<string>& vecFileLines)
{
	mVecFileLines = vecFileLines;
}

bool CFileAnalysis::AnalysisItem()
{
	if (mVecFileLines.size() <= 0)
	{
		std::cout << "error guid file is Empty";
		return false;
	}

	vector<string> vecFileLines(mVecFileLines);
	for (int nI = 0; nI < vecFileLines.size(); nI++)
	{
		auto itemLine(vecFileLines.at(nI));		
		if (itemLine.empty())
		{
			continue;
		}
		int nRightBracket = itemLine.find_first_of("}");
		if (nRightBracket != -1)
		{
			string sGuid = itemLine.substr(0, 38);
			itemLine.replace(0, 39, "");

			int nFindTitle = itemLine.find("event:");
			if (-1 != nFindTitle)
			{
				mMapGuidEvent[sGuid] = itemLine;
				continue;
			}

			nFindTitle = itemLine.find("bus:");
			if (-1 != nFindTitle)
			{
				mMapGuidBus[sGuid] = itemLine;
				continue;
			}

			nFindTitle = itemLine.find("bank:");
			if (-1 != nFindTitle)
			{
				mMapGuidBank[sGuid] = itemLine;
			}
		}
	}
	return true;
}

bool CFileAnalysis::WriteItemToCSharp(string sPath)
{
	mOutFileStream.open(sPath, std::ios::out | std::ios::trunc);
	if (!mOutFileStream.is_open())
	{
		cout << "error cs file open failed";
		return false;
	}

	mOutFileStream << "/* file name:  FmodData.cs" << "\n";
	mOutFileStream << "*   descript:  fmod��Ч����ͨ����\n";
	mOutFileStream << "*     author:  leiqk 314332613@qq.com\n";
	mOutFileStream << "*       date:  2017��12��16��\n";
	mOutFileStream << "*      other:  ���ļ��������ļ�ͨ�������Զ����ɶ����������ֶ��޸�\n";
	mOutFileStream << "*/\n";
	mOutFileStream << "\n";
	mOutFileStream << "using System;\n";
	mOutFileStream << "using System.Collections.Generic;\n";
	mOutFileStream << "using System.Linq;\n";
	mOutFileStream << "using System.Text;\n";
	
	//����ö��
	mOutFileStream << "namespace FmodSounds\n";
	mOutFileStream << "{\n";
	mOutFileStream << "\tpublic enum EnumFmodEvent\n";
	mOutFileStream << "\t{\n";

	for (auto item : mMapGuidEvent)
	{
		string sItem = item.second;
		sItem.replace(0, 7, "");
		int nPos = sItem.find(" ");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "");
			nPos = sItem.find(" ");
		}
		nPos = sItem.find("/");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "_");
			nPos = sItem.find("/");
		}
		nPos = sItem.find("-");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "_");
			nPos = sItem.find("-");
		}
		sItem.insert(0, "\t\tEvent_");
		sItem.append(",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t\tEvent_Size_Max,\n";
	mOutFileStream << "\t}\n";

	mOutFileStream << "\tpublic enum EnumFmodBus\n";
	mOutFileStream << "\t{\n";
	for (auto item : mMapGuidBus)
	{
		string sItem = item.second;
		sItem.replace(0, 5, "");
		int nPos = sItem.find(" ");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "");
			nPos = sItem.find(" ");
		}
		nPos = sItem.find("/");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "_");
			nPos = sItem.find("/");
		}
		nPos = sItem.find("-");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "_");
			nPos = sItem.find("-");
		}
		sItem.insert(0, "\t\tBus_");
		sItem.append(",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t\tBus_Size_Max,\n";
	mOutFileStream << "\t}\n";

	mOutFileStream << "\tpublic enum EnumFmodBank\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tBank_Master_Bank_Strings,\n";
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");
		int nPos = sItem.find(" ");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "");
			nPos = sItem.find(" ");
		}
		nPos = sItem.find("/");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "_");
			nPos = sItem.find("/");
		}
		nPos = sItem.find("-");
		while (nPos != -1)
		{
			sItem.replace(nPos, 1, "_");
			nPos = sItem.find("-");
		}
		sItem.insert(0, "\t\tBank_");
		sItem.append(",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t\tBank_Size_Max,\n";
	mOutFileStream << "\t}\n";


	//����list
	mOutFileStream << "\n";
	mOutFileStream << "\tclass FmodData\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tprivate static List<string> mEventNames = new List<string>\n";
	mOutFileStream << "\t\t{\n";

	for (auto item : mMapGuidEvent)
	{
		string sItem = item.second;
		sItem.insert(0, "\t\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t\t};\n";

	mOutFileStream << "\t\tprivate static List<string> mBusNames = new List<string>\n";
	mOutFileStream << "\t\t{\n";
	for (auto item : mMapGuidBus)
	{
		string sItem = item.second;
		sItem.insert(0, "\t\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t\t};\n";
	
	mOutFileStream << "\t\tprivate static List<string> mBankNames = new List<string>\n";
	mOutFileStream << "\t\t{\n";

	mOutFileStream << "\t\t\t\"Master_Bank.strings\",\n";
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");
		sItem.insert(0, "\t\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t\t};\n";
	

	mOutFileStream << "\t\tprivate static List<string> mBankAssetBudleNames = new List<string>\n";
	mOutFileStream << "\t\t{\n";

	mOutFileStream << "\t\t\t\"sound/master_bank.strings\",\n";
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");
		sItem.insert(0, "sound/");
		sItem.insert(0, "\t\t\t\"");
		sItem.append("\",\n");		
		transform(sItem.begin(), sItem.end(), sItem.begin(), ::tolower);
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t\t};\n";


	mOutFileStream << "\t\tpublic static string GetEventName(EnumFmodEvent eIndex)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tif (eIndex < 0 || (int)eIndex >= mEventNames.Count)\n";
	mOutFileStream << "\t\t\t{\n";
	mOutFileStream << "\t\t\t\tUnityEngine.Debug.LogError(\"Fmod GetEventName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t\t}\n";
	mOutFileStream << "\t\t\treturn mEventNames[(int)eIndex];\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\tpublic static string GetBusName(EnumFmodBus eIndex)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tif (eIndex < 0 || (int)eIndex >= mBusNames.Count)\n";
	mOutFileStream << "\t\t\t{\n";
	mOutFileStream << "\t\t\t\tUnityEngine.Debug.LogError(\"Fmod GetBusName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t\t}\n";
	mOutFileStream << "\t\t\treturn mBusNames[(int)eIndex];\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\tpublic static string GetBankName(EnumFmodBank eIndex)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tif (eIndex < 0 || (int)eIndex >= mBankNames.Count)\n";
	mOutFileStream << "\t\t\t{\n";
	mOutFileStream << "\t\t\t\tUnityEngine.Debug.LogError(\"Fmod GetBankName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t\t}\n";
	mOutFileStream << "\t\t\treturn mBankNames[(int)eIndex];\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\tpublic static string GetAssetBundleName(EnumFmodBank eIndex)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tif (eIndex < 0 || (int)eIndex >= mBankAssetBudleNames.Count)\n";
	mOutFileStream << "\t\t\t{\n";
	mOutFileStream << "\t\t\t\tUnityEngine.Debug.LogError(\"Fmod GetAssetBundleName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t\t}\n";
	mOutFileStream << "\t\t\treturn mBankAssetBudleNames[(int)eIndex];\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "}\n";

	mOutFileStream.close();
	return true;
}

