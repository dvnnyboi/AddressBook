//FindingForm.h
/************************************************************************************************
���ϸ�Ī: FindingForm.h
��    ��: API �ڷ��� �� �Լ�����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/04/29
*************************************************************************************************/
#ifndef _FINDINGFORM_H
#define _FINDINGFORM_H
#include <windows.h>

BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif //_FINDINGFORM_H