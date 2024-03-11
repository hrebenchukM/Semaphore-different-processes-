#include "ParentProcessDlg.h"

ParentProcessDlg* ParentProcessDlg::ptr = NULL;

ParentProcessDlg::ParentProcessDlg(void)
{
	ptr = this;
}

ParentProcessDlg::~ParentProcessDlg(void)
{
	
}

void ParentProcessDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void MessageAboutError(DWORD dwError)
{
	LPVOID lpMsgBuf = NULL;
	TCHAR szBuf[300]; 
	BOOL fOK = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	if(lpMsgBuf != NULL)
	{
		wsprintf(szBuf, TEXT("Ошибка %d: %s"), dwError, lpMsgBuf); 
		MessageBox(NULL, szBuf, TEXT("Сообщение об ошибке"), MB_OK | MB_ICONSTOP); 
		LocalFree(lpMsgBuf);
	}
}

void ParentProcessDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if(IDC_BUTTON1 == id)
	{
		CreateSemaphore(NULL, 1, 3, TEXT("{2525FD5F-12E6-47c0-838A-7C5CA1EBD169}"));//каждый момнт времени из 3 может работать только 1 . Из семафоры мьютекс сделали только контролируем количество процессов 3 и все.
		STARTUPINFO st = {sizeof(st)};
		PROCESS_INFORMATION pr;
		TCHAR filename[20];
		wsprintf(filename, TEXT("%s"), TEXT("CodingThread.exe"));
		if (!CreateProcess(NULL, filename, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))//По факту из парент процесс приложения мы запускам программу CodingThread.exe и эта пррограмма создает coding.txt и выполняется код того приложения другого кодируется из music.txt в  coding.txt  
		{
			MessageAboutError(GetLastError());
			return;
		}
		CloseHandle(pr.hThread);
		CloseHandle(pr.hProcess);


		ZeroMemory(&st, sizeof(st));
		st.cb = sizeof(st);
		for(int i = 1; i <= 3; i++)//две копии файла после декадирования
		{
			TCHAR buf[30];
			ZeroMemory(&st, sizeof(st));//с какого адреса сколько байт надо обнулить стартапинфо. Чтоб не создавать заново обнуляем старую переменную
			st.cb = sizeof(st);
			wsprintf(buf, TEXT("DecodingThread.exe %d"), i);
			if (!CreateProcess(NULL, buf, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))
			{
				MessageAboutError(GetLastError());
			}
			CloseHandle(pr.hThread);
			CloseHandle(pr.hProcess);	
		}



		////////////////////////HW
		ZeroMemory(&st, sizeof(st));
		st.cb = sizeof(st);
		TCHAR buf2[30];
		wsprintf(buf2, TEXT("%s"), TEXT("ProgramСopyCount.exe"));
		if (!CreateProcess(NULL, buf2, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))
		{
			MessageAboutError(GetLastError());
		}
		CloseHandle(pr.hThread);
		CloseHandle(pr.hProcess);
		/////////////////////////
	}
}

BOOL CALLBACK ParentProcessDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}