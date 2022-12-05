//AddressBookForm.h
/************************************************************************************************
���ϸ�Ī: AddressBookForm.h
��    ��: API �ڷ��� �� �Լ�����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/04/28
*************************************************************************************************/
#ifndef _ADDRESSBOOKFORM_H
#define _ADDRESSBOOKFORM_H
#include <windows.h>

BOOL CALLBACK AddressBookFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnRecordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnCorrectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnEraseButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL AddressBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif	//_ADDRESSBOOKFORM_H