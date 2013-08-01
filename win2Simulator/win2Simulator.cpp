// win2Simulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


static void noDirError()
{
	fprintf(stderr,"\nError: No directory specified. win2Simulator needs a directory to execute.\n"
		"Usage: win2Simulator win\n"
		"Use win2Simulator -help to get more help.\n");
	exit(1);
}

static const char* WIN2SIMULATOR = "0.5"; // current binary release version
static void showHelp()
{
    printf("\nwin2Simulator version: %s (Built %s).\n", WIN2SIMULATOR, __DATE__);
	
	printf("  -help,-?  Show this message.\n");
	printf("  -f name   Execute file 'name' with remaining arguments in table 'arg'\n\n");
	
	// 	printf("Copyright (c) 2001,2002 Nick Trout.\n"
	// 		"Source code is released under the GNU GPL license.\n"
	// 	       "Homepage: http://doris.sourceforge.net/\n\n"
	// 		   "Doris comes with ABSOLUTELY NO WARRANTY; This is free software,\n"
	// 		   "and you are welcome to redistribute it under certain conditions.\n"
	// 		   "For details on warranty and conditions see accompanying file License.txt\n"
	// 		   "or visit the GNU website http://www.gnu.org/licenses/gpl.html\n\n");
	// 	printf("Doris uses " LUA_VERSION ", " LUA_COPYRIGHT "\n");
	// 	printf("by " LUA_AUTHORS ".\n");
	// 	printf("See: http://www.lua.org/ and http://www.lua-users.org/\n\n");
	// 	printf("Usage: doris [flags|script]\n");
	// 	printf("  -help,-?  Show this message.\n");
	// 	printf("  -f name   Execute file 'name' with remaining arguments in table 'arg'\n\n");
}

static string creatDirAndCpoy2It(string & srcDirname, string & dirfiles)
{
	_finddata_t file;
    long lf;

	string desDir;

	//"C:\\WINDOWS\\*.*"
	//_findfirst返回的是long型; long __cdecl _findfirst(const char *, struct _finddata_t *)
    if((lf = _findfirst(dirfiles.c_str(), &file))==-1l)
        cout<<"no file found!\n";
    else
    {
		string filename;
        //cout<<"\n文件列表:\n";
		//int __cdecl _findnext(long, struct _finddata_t *);如果找到下个文件的名字成功的话就返回0,否则返回-1
        while( _findnext( lf, &file ) == 0 )
        {
			if(file.attrib != _A_SUBDIR)
			{
				//cout<<file.name<<endl;
				
				filename = file.name;
				
				int pos = filename.find(".mif");
				
				if (pos != std::string::npos)
				{
					filename = filename.substr(0, pos);
					
					//cout<<filename<<endl;
					
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
            //if(file.attrib == _A_NORMAL)cout<<"  普通文件  ";
            //else if(file.attrib == _A_RDONLY)cout<<"  只读文件  ";
            //else if(file.attrib == _A_HIDDEN )cout<<"  隐藏文件  ";
			// else if(file.attrib == _A_SYSTEM )cout<<"  系统文件  ";
            //else if(file.attrib == _A_SUBDIR)cout<<"  子目录  ";
            //else cout<<"  存档文件  ";
            //cout<<endl;
			//////////////////////////////////////////////////////////////////////////
        }
		
		if (filename.length() <= 0)
		{
			fprintf(stderr,"\nError: No mif file specified. win2Simulator needs a mif file to execute.\n"
				"Usage: win2Simulator win\n"
				"Use win2Simulator -help to get more help.\n");
			exit(1);
		}
		else 
		{
			desDir = srcDirname.append("\\").append(filename);
			_mkdir(desDir.c_str());
		}
		
		ostringstream osxcopycmd; 
		osxcopycmd<< "xcopy   \"" << dirfiles << "\"   \"" <<desDir.append("\\") << "\"    /y/i"; 
		
		//cout << osxcopycmd.str().c_str() << std::endl;
		
		system(osxcopycmd.str().c_str());
		
    }
    _findclose(lf);

	return desDir;
}

// static bool GetWinRarPath(char* path)  
// {  
//     HKEY hKEY;   
//     LPCTSTR Rgspath = _T("Applications//WinRAR.exe//shell//open//command");   
//     LONG ret = RegOpenKeyEx(HKEY_CLASSES_ROOT, Rgspath, 0, KEY_READ, &hKEY);   
// 	
//     if(ret != ERROR_SUCCESS)   
//     {   
//         RegCloseKey(hKEY);   
//         //AfxMessageBox(_T("open Reg Error"));   
// 		cout << "open Reg Error" << endl;
//         return false;   
//     }   
//     // 读取键值内容   
//     DWORD type = REG_SZ;   
//     //BYTE UserInfo[255];  
//     DWORD dwInfoSize = 255;   
//     ret = RegQueryValueEx(hKEY, NULL, NULL, &type, (BYTE*)path, &dwInfoSize);   
//     if(ret!=ERROR_SUCCESS)  
//     {   
//         //AfxMessageBox(_T("not install BREW"));   
//         RegCloseKey(hKEY);   
// 		cout << "not install BREW, please install it first." << endl;
//         return false;   
//     }  
//     return true;  
// }  

static bool readAndWriteFile(const string & srcFile, const string & destFile, const string &fileDir, int flag = 0)
{
	ifstream ifs(srcFile.c_str(), ifstream::in);
	if(ifs.fail())
	{
		fprintf(stderr,"\nError: can't open BREW_Emu.dat\n");
		exit(1);
	}
	

	ofstream ofs(destFile.c_str(), ofstream::out);
	if (ofs.fail())
	{
		fprintf(stderr,"\nError: can't open BREW_Emu_tmp.dat\n");
		exit(1);
	}

	string line;
	bool b_editOk = false;
	while (getline(ifs, line, '\n'))
	{
		if (!b_editOk && flag == 1)
		{
			if (line.find("AppletDir=") != string::npos)
			{
				line = "AppletDir=";
				line.append(fileDir);
				//cout << line << endl;
			}
			else if (line.find("MIFDir=") != string::npos)
			{
				line = "MIFDir=";
				line.append(fileDir);
				//cout << line << endl;
			}
			else if (line.find("SpecifyMIF=") != string::npos)
			{
				line = "SpecifyMIF=";
				line.append("1");
				//cout << line << endl;
				b_editOk = true;
			}
		}
		
		ofs << line << endl;
	}
	
	ifs.close();
	ofs.close();

	return true;
}

static void set_Brew_emu_value(const string & destDir, const char *pPath)
{
	 string str_brew_emu(pPath);
	 str_brew_emu.append("\\bin\\BREW_Emu.dat");

	 string str_brew_emu_temp(pPath);
	 str_brew_emu_temp.append("\\bin\\BREW_Emu_tmp.dat");

	 readAndWriteFile(str_brew_emu, str_brew_emu_temp, destDir, 1);

	 readAndWriteFile(str_brew_emu, str_brew_emu + ".bak", destDir);

	 readAndWriteFile(str_brew_emu_temp, str_brew_emu, destDir);

	if( remove(str_brew_emu_temp.c_str()) != 0)
		 perror("Error deleting file");
	else
		 puts("File successfully deleted");

	 //string str_bew_emu_bak(pPath);
	 //str_bew_emu_bak.append("\\bin\\BREW_Emu.dat.bak");

	 //ifs.open(str_brew_emu_temp);


	// ostringstream osxcmd; 
	//osxcmd<< "\"if   exist  \"" << str_brew_emu_temp << 
	//	 "\"  ("  <<endl 
	//	 << "  move  \"" << str_brew_emu << "\"  \"" << str_brew_emu + ".bak" << "\"" << endl
	//	 << "  move  \"" << str_brew_emu_temp << "\"  \""  << str_brew_emu << "\"" << endl 
	//	 << ")\""; 
	 
	// cout << osxcmd.str().c_str() << std::endl;
	 
	// system(osxcmd.str().c_str());

	// system("pause");


	 //if exist %tmpFile% (
	 //	 move %desFile%	%desFile%.bak
	 //	 move %tmpFile%	%desFile%
	 //	)

	 //cout << str_brew_emu << endl;

}

int main(int argc, char* argv[])
{
	string srcDirname; //源目录
	string dirfiles;
	
	if (argc <= 1)
	{
		noDirError();
	}
	else if (argc > 1 && (strcmp(argv[1], "-help") )  == 0 || strcmp(argv[1], "?") == 0)
	{
		showHelp();
		exit(0);
	}
	else if (argc >= 2 && strcmp(argv[1], "-f") == 0)
	{
		if (argc < 3)
		{
			noDirError();
		}
		
		srcDirname = argv[2];
	}
	else 
	{
		srcDirname = argv[1];
	}
	
	dirfiles.append(srcDirname).append("\\*.*");

	string destDir = creatDirAndCpoy2It(srcDirname, dirfiles);


	char *pPath;
	pPath = getenv("BREWDIR");

	set_Brew_emu_value(destDir, pPath);

	system("cls");
	string brewSimulator(pPath);
	brewSimulator.append("\\bin\\BREW_Simulator.exe");
	system(brewSimulator.c_str());
	
	//system("pause");
	
	return 0;
}

