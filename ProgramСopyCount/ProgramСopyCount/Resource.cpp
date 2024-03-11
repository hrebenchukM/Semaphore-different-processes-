
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include <windowsX.h>
#include <fstream>
#include <tchar.h>
#include"resource.h"
#include <string>
#include <ctime>
#include <iostream>

using namespace std;


void GetFileInfo(HWND hDlg) {
    int totalCopies = 0;
    int totalCharacters = 0;
    int totalSize = 0;
    string earliestDate;

   
    for (int i = 1; i <= 3; ++i) {
        string filename = "copymusic_" + to_string(i) + ".txt";
        ifstream file(filename);





        // проверка на то что файл такой есть
        if (file) {
            // каретка в файле сдвигается в конец файла( каретка -- это указатель внутри файла)
            file.seekg(0, ios::end);
            // данная функция получает текущую позицию в файле, что и есть его размером
            int fileSize = file.tellg();
            // каретку сдвигаем в начало
            file.seekg(0, ios::beg);

            // здесь считается кол-во символов в файле
            char ch;
            int numCharacters = 0;
            while (file.get(ch)) {
                numCharacters++;
            }

            //StackOverflow code
            struct stat filestatus;
            stat(filename.c_str(), &filestatus);
            struct tm* creationTime = localtime(&filestatus.st_mtime);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%c", creationTime);
            string fileDate(buffer);


            // кол-во копий
            ++totalCopies;
            // общее кол-во символов
            totalCharacters += numCharacters;
            // общий размер всех файлов
            totalSize += fileSize;

            // Обновляем самую раннюю дату, если текущая дата раньше
            if (i == 1 || fileDate < earliestDate) {
                earliestDate = fileDate;
            }

            file.close();
        }
    }

    // Устанавливаем полученную информацию в соответствующие элементы управления
    SetDlgItemInt(hDlg, IDC_EDIT1, totalCopies, FALSE); // Общее количество копий файлов
    SetDlgItemInt(hDlg, IDC_EDIT2, totalCharacters, FALSE); // Количество символов в файлах копий
    SetDlgItemInt(hDlg, IDC_EDIT3, totalSize, FALSE); // Общий размер файлов копий в байтах
    SetDlgItemTextA(hDlg, IDC_EDIT4, earliestDate.c_str()); // Дата и время самой ранней копии файла
}




BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        // Получаем информацию о файлах и выводим ее в соответствующие элементы управления
        GetFileInfo(hDlg);

        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,(DLGPROC) DialogProc);
    return 0;
}

//
//
//
//class DecodingThreadDlg
//{
//public:
//	DecodingThreadDlg(void);
//public:
//	~DecodingThreadDlg(void);
//	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
//	static DecodingThreadDlg* ptr;
//	void Cls_OnClose(HWND hwnd);
//	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
//	HWND hDialog;
//
//};
//
//
//DecodingThreadDlg* DecodingThreadDlg::ptr = NULL;
//
//
//
//
//DecodingThreadDlg::DecodingThreadDlg(void)
//{
//	ptr = this;
//}
//
//DecodingThreadDlg::~DecodingThreadDlg(void)
//{
//
//}
//
//void DecodingThreadDlg::Cls_OnClose(HWND hwnd)
//{
//	EndDialog(hwnd, 0);
//}
//
//DWORD WINAPI Decoding_Thread(LPVOID lp)
//{
//	DecodingThreadDlg* ptr = (DecodingThreadDlg*)lp;
//	char buf[4096];
//	TCHAR str[MAX_PATH];
//	_tcscpy(str, GetCommandLine());
//	TCHAR seps[] = TEXT(" ");
//	TCHAR* token;
//	TCHAR* last = nullptr;
//
//	token = _tcstok(str, seps);
//	while (token != NULL)
//	{
//		token = _tcstok(NULL, seps);
//		if (token)
//			last = token;
//	}
//	wsprintf(str, TEXT("copymusic_%s.txt"), last);
//	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, TEXT("{2525FD5F-12E6-47c0-838A-7C5CA1EBD169}"));
//	DWORD dwAnswer = WaitForSingleObject(hSemaphore, INFINITE);
//	if (dwAnswer == WAIT_OBJECT_0)
//	{
//		ifstream in(TEXT("coding.txt"), ios::binary | ios::in);
//		if (!in)
//		{
//			MessageBox(ptr->hDialog, TEXT("Ошибка открытия файла!"), TEXT("Семафор"), MB_OK | MB_ICONINFORMATION);
//			return 1;
//		}
//		ofstream out(str, ios::binary | ios::out | ios::trunc);
//		const int KEY = 100;
//		while (!in.eof())
//		{
//			in.read(buf, 4096);
//			int n = in.gcount();
//			for (int i = 0; i < n; i++)
//			{
//				buf[i] ^= KEY;
//			}
//			out.write(buf, n);
//		}
//		out.close();
//		in.close();
//		ReleaseSemaphore(hSemaphore, 1, NULL);
//		MessageBox(ptr->hDialog, TEXT("Чтение данных из файла coding.txt завершено!"), TEXT("Семафор"), MB_OK | MB_ICONINFORMATION);
//	}
//	return 0;
//}
//
//BOOL DecodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
//{
//	hDialog = hwnd;
//	CreateThread(NULL, 0, Decoding_Thread, this, 0, NULL);
//	return TRUE;
//}
//
//BOOL CALLBACK DecodingThreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
//		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
//	}
//	return FALSE;
//}
//
//
//int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
//{
//	DecodingThreadDlg dlg;
//	//return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DecodingThreadDlg::DlgProc);
//	return DialogBoxParamW(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, reinterpret_cast<DLGPROC>(&DecodingThreadDlg::DlgProc), 0);
//
//}
//


