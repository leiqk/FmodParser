#include "stdafx.h"
#include "FileAnalysis.h"
#include <algorithm>
#include "string"

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
	mOutFileStream << "*   descript:  fmod音效数据通用类\n";
	//mOutFileStream << "*     author:  leiqk 314332613@qq.com\n";
	//mOutFileStream << "*       date:  \n";
	mOutFileStream << "*      other:  本文件由其他文件通过工具自动生成而来，请勿手动修改\n";
	mOutFileStream << "*/\n";
	mOutFileStream << "\n";
	mOutFileStream << "using System;\n";
	mOutFileStream << "using System.Collections.Generic;\n";
	mOutFileStream << "using System.Linq;\n";
	mOutFileStream << "using System.Text;\n";
	
	//定义枚举
	mOutFileStream << "public class FmodData\n";
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


	//定义list
	mOutFileStream << "\n";

	mOutFileStream << "\tprivate static List<string> mEventNames = new List<string>\n";
	mOutFileStream << "\t{\n";

	for (auto item : mMapGuidEvent)
	{
		string sItem = item.second;
		sItem.insert(0, "\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t};\n";

	mOutFileStream << "\tprivate static List<string> mBusNames = new List<string>\n";
	mOutFileStream << "\t{\n";
	for (auto item : mMapGuidBus)
	{
		string sItem = item.second;
		sItem.insert(0, "\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t};\n";
	
	mOutFileStream << "\tprivate static List<string> mBankNames = new List<string>\n";
	mOutFileStream << "\t{\n";

	mOutFileStream << "\t\t\"Master_Bank.strings\",\n";
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");
		sItem.insert(0, "\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t};\n";
	

	mOutFileStream << "\tprivate static List<string> mBankAssetBudleNames = new List<string>\n";
	mOutFileStream << "\t{\n";

	mOutFileStream << "\t\t\"sound/master_bank.strings\",\n";
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");
		sItem.insert(0, "sound/");
		sItem.insert(0, "\t\t\"");
		sItem.append("\",\n");		
		transform(sItem.begin(), sItem.end(), sItem.begin(), ::tolower);
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t};\n";


	mOutFileStream << "\tpublic static string GetEventName(EnumFmodEvent eIndex)\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif (eIndex < 0 || (int)eIndex >= mEventNames.Count)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tUnityEngine.Debug.LogError(\"Fmod GetEventName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn mEventNames[(int)eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "\tpublic static string GetBusName(EnumFmodBus eIndex)\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif (eIndex < 0 || (int)eIndex >= mBusNames.Count)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tUnityEngine.Debug.LogError(\"Fmod GetBusName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn mBusNames[(int)eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "\tpublic static string GetBankName(EnumFmodBank eIndex)\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif (eIndex < 0 || (int)eIndex >= mBankNames.Count)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tUnityEngine.Debug.LogError(\"Fmod GetBankName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn mBankNames[(int)eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "\tpublic static string GetAssetBundleName(EnumFmodBank eIndex)\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif (eIndex < 0 || (int)eIndex >= mBankAssetBudleNames.Count)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tUnityEngine.Debug.LogError(\"Fmod GetAssetBundleName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn mBankAssetBudleNames[(int)eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "}\n";

	mOutFileStream.close();
	return true;
}

bool CFileAnalysis::WriteItemToTypeScript(string sPath)
{
	mOutFileStream.open(sPath, std::ios::out | std::ios::trunc);
	if (!mOutFileStream.is_open())
	{
		cout << "error cs file open failed";
		return false;
	}

	mOutFileStream << "/* file name:  FmodData.ts" << "\n";
	mOutFileStream << "*   descript:  fmod common data file\n";
	//mOutFileStream << "*     author:  leiqk 314332613@qq.com\n";
	//mOutFileStream << "*       date:  \n";
	mOutFileStream << "*      other:  auto generate by tools , do not modify\n";
	mOutFileStream << "*/\n";
	mOutFileStream << "\n";

	//定义枚举
	mOutFileStream << "export module FmodData\n";
	mOutFileStream << "{\n";
	mOutFileStream << "\texport enum EnumFmodEvent\n";
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

	mOutFileStream << "\texport enum EnumFmodBus\n";
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

	mOutFileStream << "\texport enum EnumFmodBank\n";
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


	//定义list
	mOutFileStream << "\n";
	//mOutFileStream << "\tclass FmodData\n";
	//mOutFileStream << "\t{\n";
	mOutFileStream << "\tvar _EventNames =\n";
	mOutFileStream << "\t{\n";

	int nIndex = 0;
	for (auto item : mMapGuidEvent)
	{
		string sItem = item.second;
		string strInsert = std::to_string(nIndex);
		strInsert = "\t\t" + strInsert + " : \"";
		sItem.insert(0, strInsert);
		sItem.append("\",\n");		
		mOutFileStream << sItem.c_str();
		nIndex++;
	}
	mOutFileStream << "\t};\n";

	mOutFileStream << "\tvar _BusNames =\n";
	mOutFileStream << "\t{\n";
	nIndex = 0;
	for (auto item : mMapGuidBus)
	{
		string sItem = item.second;
		string strInsert = std::to_string(nIndex);
		strInsert = "\t\t" + strInsert + " : \"";
		sItem.insert(0, strInsert);
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
		nIndex++;
	}
	mOutFileStream << "\t};\n";

	mOutFileStream << "\tvar _BankNames =\n";
	mOutFileStream << "\t{\n";

	mOutFileStream << "\t\t0 : \"Master_Bank.strings.bank\",\n";
	nIndex = 1;
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");

		string strInsert = std::to_string(nIndex);
		strInsert = "\t\t" + strInsert + " : \"";
		sItem.insert(0, strInsert);
		sItem.append(".bank");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
		nIndex++;
	}
	mOutFileStream << "\t};\n";

	/*
	mOutFileStream << "\tvar _BankAssetBudleNames =\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\t0 : \"sound/master_bank.strings\",\n";
	nIndex = 1;
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");
		sItem.insert(0, "sound/");
		string strInsert = std::to_string(nIndex);
		strInsert = "\t\t" + strInsert + " : \"";
		sItem.insert(0, strInsert);
		sItem.append("\",\n");
		transform(sItem.begin(), sItem.end(), sItem.begin(), ::tolower);
		mOutFileStream << sItem.c_str();
		nIndex++;
	}
	mOutFileStream << "\t};\n";
	*/
	

	mOutFileStream << "\texport function getEventName(eIndex : EnumFmodEvent) : string\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif(eIndex < 0 || eIndex >= EnumFmodEvent.Event_Size_Max)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tconsole.error(\"FmodData getEventName error index : \" + eIndex);\n";
	mOutFileStream << "\t\t\treturn \"\";\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn _EventNames[eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "\texport function getBusName(eIndex : EnumFmodBus) : string\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif(eIndex < 0 || eIndex >= EnumFmodBus.Bus_Size_Max)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tconsole.error(\"FmodData getBusName error index : \" + eIndex);\n";
	mOutFileStream << "\t\t\treturn \"\";\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn _BusNames[eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "\texport function getBankName(eIndex : EnumFmodBank) : string\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif(eIndex < 0 || eIndex >= EnumFmodBank.Bank_Size_Max)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tconsole.error(\"FmodData getBankName error index : \" + eIndex);\n";
	mOutFileStream << "\t\t\treturn \"\";\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn _BankNames[eIndex];\n";
	mOutFileStream << "\t}\n";
	//mOutFileStream << "\texport function getAssetBundleName(eIndex : EnumFmodBank) : string\n";
	//mOutFileStream << "\t{\n";
	//mOutFileStream << "\t\tif(eIndex < 0 || eIndex >= EnumFmodEvent.Event_Size_Max)\n";
	//mOutFileStream << "\t\t{\n";
	//mOutFileStream << "\t\t\tconsole.error(\"FmodData getAssetBundleName error index : \" + eIndex);\n";
	//mOutFileStream << "\t\t\treturn \"\";\n";
	//mOutFileStream << "\t\t}\n";
	//mOutFileStream << "\t\treturn this._BankAssetBudleNames[eIndex];\n";
	//mOutFileStream << "\t}\n";
	mOutFileStream << "}\n";

	mOutFileStream.close();
	return true;
}

bool CFileAnalysis::WriteItemToCSharpToLua(string sPath)
{
	mOutFileStream.open(sPath, std::ios::out | std::ios::trunc);
	if (!mOutFileStream.is_open())
	{
		cout << "error cs file open failed";
		return false;
	}

	mOutFileStream << "/* file name:  FmodData.cs" << "\n";
	mOutFileStream << "*   descript:  fmod音效数据通用类\n";
	//mOutFileStream << "*     author:  leiqk 314332613@qq.com\n";
	//mOutFileStream << "*       date:  \n";
	mOutFileStream << "*      other:  本文件由其他文件通过工具自动生成而来，请勿手动修改\n";
	mOutFileStream << "*/\n";
	mOutFileStream << "\n";
	mOutFileStream << "using System;\n";
	mOutFileStream << "using System.Collections.Generic;\n";
	mOutFileStream << "using System.Linq;\n";
	mOutFileStream << "using System.Text;\n";
	mOutFileStream << "using GameFrameLua;\n"; 
	mOutFileStream << "\n";

	//定义枚举
	mOutFileStream << "[CSLStub.Out(\"Game/FmodSound\")]\n";
	mOutFileStream << "public class FmodData\n";
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


	//定义list
	mOutFileStream << "\n";

	mOutFileStream << "\tprivate static CSLList<string> mEventNames = new CSLList<string>\n";
	mOutFileStream << "\t{\n";

	for (auto item : mMapGuidEvent)
	{
		string sItem = item.second;
		sItem.insert(0, "\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t};\n";

	mOutFileStream << "\tprivate static CSLList<string> mBusNames = new CSLList<string>\n";
	mOutFileStream << "\t{\n";
	for (auto item : mMapGuidBus)
	{
		string sItem = item.second;
		sItem.insert(0, "\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t};\n";

	mOutFileStream << "\tprivate static CSLList<string> mBankNames = new CSLList<string>\n";
	mOutFileStream << "\t{\n";

	mOutFileStream << "\t\t\"Master_Bank.strings\",\n";
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");
		sItem.insert(0, "\t\t\"");
		sItem.append("\",\n");
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t};\n";


	mOutFileStream << "\tprivate static CSLList<string> mBankAssetBudleNames = new CSLList<string>\n";
	mOutFileStream << "\t{\n";

	mOutFileStream << "\t\t\"sound/master_bank.strings\",\n";
	for (auto item : mMapGuidBank)
	{
		string sItem = item.second;
		sItem.replace(0, 6, "");
		sItem.insert(0, "sound/");
		sItem.insert(0, "\t\t\"");
		sItem.append("\",\n");
		transform(sItem.begin(), sItem.end(), sItem.begin(), ::tolower);
		mOutFileStream << sItem.c_str();
	}
	mOutFileStream << "\t};\n";


	mOutFileStream << "\tpublic static string GetEventName(EnumFmodEvent eIndex)\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif (eIndex < 0 || (int)eIndex >= mEventNames.Count)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tLuaDebug.error(\"Fmod GetEventName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn mEventNames[(int)eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "\tpublic static string GetBusName(EnumFmodBus eIndex)\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif (eIndex < 0 || (int)eIndex >= mBusNames.Count)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tLuaDebug.error(\"Fmod GetBusName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn mBusNames[(int)eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "\tpublic static string GetBankName(EnumFmodBank eIndex)\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif (eIndex < 0 || (int)eIndex >= mBankNames.Count)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tLuaDebug.error(\"Fmod GetBankName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn mBankNames[(int)eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "\tpublic static string GetAssetBundleName(EnumFmodBank eIndex)\n";
	mOutFileStream << "\t{\n";
	mOutFileStream << "\t\tif (eIndex < 0 || (int)eIndex >= mBankAssetBudleNames.Count)\n";
	mOutFileStream << "\t\t{\n";
	mOutFileStream << "\t\t\tLuaDebug.error(\"Fmod GetAssetBundleName error index : \" + (int)eIndex);\n";
	mOutFileStream << "\t\t\treturn string.Empty;\n";
	mOutFileStream << "\t\t}\n";
	mOutFileStream << "\t\treturn mBankAssetBudleNames[(int)eIndex];\n";
	mOutFileStream << "\t}\n";
	mOutFileStream << "}\n";

	mOutFileStream.close();
	return true;
}


