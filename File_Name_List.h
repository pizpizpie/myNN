#pragma once

#include <windows.h>

#include "Parameters.h"

using namespace std;

#define MAX_DIR_NAME	(30)

class Files {
private:
	char name[MAX_PATH];

public:
	Files() {
		ZeroMemory(name, sizeof(char) * MAX_PATH);
	}

	char* Get_Name() {
		return name;
	}
};

class File_Name_List{

private:

	// 読み込むファイル
	Files files[MAX_FILES];

	// 読み込んだファイルの「個数」
	int length;

public:

	Files* Get_Files(int i) {
		return &files[i];
	}

	int Get_Length() {
		return length;
	}

	void Show() {
		for(int i = 0; i < length; i++) {
			std::cout << files[i].Get_Name() << std::endl;
		}

		getch();
	}

	File_Name_List (char path[]) {

		WIN32_FIND_DATA win32fd;
		HANDLE hFind;

		char directly_name[MAX_DIR_NAME + 3];
		char directly_name_tmp[MAX_DIR_NAME];

		strcpy(directly_name, path);
		strcpy(directly_name_tmp, path);

		// FindFirstFile()で扱えるようにするため、"*.*"を付与
		strcat(directly_name, "*.*");
		std::cout << "current directry is: " << directly_name << std::endl;

		hFind = FindFirstFile(directly_name, &win32fd);

		if(hFind == INVALID_HANDLE_VALUE) {
			std::cout << "in dir("<< directly_name << ") : Could NOT find such file or directly.\n" << std::endl;
			getch();
			// return -1;
			exit(1);
		}

		char file_name_tmp[MAX_PATH]={};

		int num_loaded_files = 0;

		// path内に存在するファイル・フォルダを1個ずつ読み込んでいく
		do {
			// 見つかったのがディレクトリだったら何もしない
			if(win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				;

			else {
				strcpy(file_name_tmp, win32fd.cFileName);
				file_name_tmp[strlen(file_name_tmp) + 1] = '\0';
				strcpy(files[num_loaded_files].Get_Name(), file_name_tmp);
				num_loaded_files++;
			}
		} while(FindNextFile(hFind, &win32fd));

		FindClose(hFind);

		// 読み込んだファイルの「個数」を記録
		length = num_loaded_files;
	}
};
