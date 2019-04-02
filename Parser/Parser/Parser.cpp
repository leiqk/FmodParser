// Parser.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FileAnalysis.h"
#include "COneFile.h"

int main()
{
	//to c# 0 ； to ts 1  to c#tolua 2
	int nFileType = 0;

	cout << "please input file type (0 : c# , 1 ts, 2 : c#tolua):" << endl;
	cin >> nFileType;

	COneFile onefile;
	onefile.ReadIt("GUIDs.txt");

	vector<string> vecFileLines = onefile.getFileLines();
	int nFileLine = vecFileLines.size();

	CFileAnalysis analysisItem;
	analysisItem.AddOneFileContent(vecFileLines);
	if (analysisItem.AnalysisItem())
	{
		cout << "guid file analysis success!" << endl;
		cout << "event num :" << analysisItem.GetGuidEvent().size() << endl;
		for (auto item : analysisItem.GetGuidEvent())
		{
			cout << item.first.c_str() << "\t" << item.second.c_str() << endl;
		}
		cout << "bus num :" << analysisItem.GetGuidBus().size() << endl;
		for (auto item : analysisItem.GetGuidBus())
		{
			cout << item.first.c_str() << "\t" << item.second.c_str() << endl;
		}
		cout << "bank num :" << analysisItem.GetGuidBank().size() << endl;
		for (auto item : analysisItem.GetGuidBank())
		{
			cout << item.first.c_str() << "\t" << item.second.c_str() << endl;
		}
	}
	
	if (1 == nFileType)
	{
		analysisItem.WriteItemToTypeScript("FmodData.ts");
	}
	else if (0 == nFileType)
	{
		analysisItem.WriteItemToCSharp("FmodData.cs");
	}
	else
	{
		analysisItem.WriteItemToCSharpToLua("FmodData.cs");
	}

	system("pause");
    return 0;
}

