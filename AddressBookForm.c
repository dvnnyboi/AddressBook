//AddressBookForm.c
/************************************************************************************************
파일명칭: AddressBookForm.c
기    능: 주소록 GUI
작 성 자: 임다니엘
작성일자: 2022/04/28
*************************************************************************************************/
#include "AddressBookForm.h"
#include "AddressBook.h"
#include "FindingForm.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <commctrl.h>
#pragma warning (disable:4996)

//WInMain 정의
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int response;

	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDRESSBOOKFORM), NULL, AddressBookFormProc);

	return response;
}

/************************************************************************************************
명    칭: AddressBookFormProc
기    능: 윈도우 프로시져 함수
*************************************************************************************************/
BOOL CALLBACK AddressBookFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = AddressBookForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		ret = AddressBookForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_NOTIFY:
		ret = AddressBookForm_OnNotify(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = AddressBookForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: AddressBookForm_OnInitDialog
기    능: 윈도우가 생성되었을 때의 시나리오.
*************************************************************************************************/
BOOL AddressBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	Long count;
	Long i;
	LVCOLUMN column = { 0, };
	LVITEM item = { 0, };
	TCHAR number[64];
	Personal personal;

	//1.1.주소록을 만든다,
	addressBook = (AddressBook*)malloc(sizeof(AddressBook));
	AddressBook_Create(addressBook, 10000);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)addressBook);
	//1.2. 주소록에서 적재한다.
	count = Load(addressBook);
	//1.3. 리스트뷰 컨트롤의 헤더를 만든다.
	column.mask = LVCF_WIDTH | LVCF_TEXT;
	column.cx = 40;
	column.pszText = "번호";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&column);
	column.cx = 80;
	column.pszText = "성명";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&column);
	column.cx = 160;
	column.pszText = "주소";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)2, (LPARAM)&column);
	column.cx = 80;
	column.pszText = "전화번호";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)3, (LPARAM)&column);
	column.cx = 120;
	column.pszText = "이메일주소";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)4, (LPARAM)&column);
	//1.4. 적재한 개수만큼 리스트뷰 컨트롤에 항목을 추가한다.
	item.mask = LVIF_TEXT;
	i = 0;
	while (i < count) {
		item.iItem = i;
		personal = AddressBook_GetAt(addressBook, i);
		
		item.iSubItem = 0;
		sprintf(number, "%d", i + 1);
		item.pszText = number;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, (WPARAM)i, (LPARAM)&item);
		item.iSubItem = 1;
		item.pszText = personal.name;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
		item.iSubItem = 2;
		item.pszText = personal.address;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
		item.iSubItem = 3;
		item.pszText = personal.telephoneNumber;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
		item.iSubItem = 4;
		item.pszText = personal.emailAddress;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

		i++;
	}
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETEXTENDEDLISTVIEWSTYLE,
		(WPARAM)LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES, (LPARAM)LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	return TRUE;
}
/************************************************************************************************
명    칭: AddressBookForm_OnCommand
기    능: WM_COMMAND 하위 parameter
*************************************************************************************************/
BOOL AddressBookForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_RECORD:
		ret = AddressBookForm_OnRecordButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIND:
		ret = AddressBookForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_CORRECT:
		ret = AddressBookForm_OnCorrectButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_ERASE:
		ret = AddressBookForm_OnEraseButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_ARRANGE:
		ret = AddressBookForm_OnArrangeButtonClicked(hWnd, wParam, lParam);
		break;
	default: 
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: AddressBookForm_OnRecordButtonClicked
기    능: 사용자가 기재하기 버튼을 클릭했을때의 시나리오.
*************************************************************************************************/
BOOL AddressBookForm_OnRecordButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	TCHAR name[11];
	TCHAR address[32];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];
	TCHAR number[64];
	Long index;
	Long i;
	LVITEM item = { 0, };
	Personal personal;

	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1. 성명,주소,전화번호,이메일주소를 읽는다,
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)emailAddress);
		//2.2. 주소록에서 기재한다.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Record(addressBook, name, address, telephoneNumber, emailAddress);
		//2.3. 리스트뷰 컨트롤에 항목을 추가한다.
		item.mask = LVIF_TEXT;
		item.iItem = index;
		personal = AddressBook_GetAt(addressBook, index);

		item.iSubItem = 0;
		sprintf(number, "%d", index + 1);
		item.pszText = number;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, (WPARAM)0, (LPARAM)&item);

		item.iSubItem = 1;
		item.pszText = personal.name;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		item.iSubItem = 2;
		item.pszText = personal.address;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		item.iSubItem = 3;
		item.pszText = personal.telephoneNumber;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		item.iSubItem = 4;
		item.pszText = personal.emailAddress;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_NAME));
	}
	return TRUE;
}
/************************************************************************************************
명    칭: AddressBookForm_OnFindButtonClicked
기    능: 사용자가 찾기 버튼을 클릭했을때의 시나리오.
*************************************************************************************************/
BOOL AddressBookForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1. 찾기 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM),
			NULL, FindingFormProc);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: AddressBookForm_OnCorrectButtonClicked
기    능: 사용자가 고치기 버튼을 클릭했을때의 시나리오.
*************************************************************************************************/
BOOL AddressBookForm_OnCorrectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	TCHAR address[32];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];
	LVITEM item = { 0, };
	Long index;
	Personal personal;

	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1. 리스트뷰 컨트롤에서 위치를 읽는다.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);
		//4.2. 주소, 전화번호, 이메일주소를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)emailAddress);
		//4.3. 주소록에서 고친다.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Correct(addressBook, index, address, telephoneNumber, emailAddress);
		//4.4. 리스트뷰 컨트롤에서 항목을 고친다.
		item.mask = LVIF_TEXT;
		item.iItem = index;

		personal = AddressBook_GetAt(addressBook, index);
		item.iSubItem = 2;
		item.pszText = personal.address;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		item.iSubItem = 3;
		item.pszText = personal.telephoneNumber;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		item.iSubItem = 4;
		item.pszText = personal.emailAddress;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)index, (LPARAM)&item);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_NAME));
	}
	return TRUE;
}
/************************************************************************************************
명    칭: AddressBookForm_OnEraseButtonClicked
기    능: 사용자가 지우기 버튼을 클릭했을때의 시나리오.
*************************************************************************************************/
BOOL AddressBookForm_OnEraseButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	TCHAR number[64];
	LVITEM item = { 0, };
	Long index;
	Long i;
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1. 리스트뷰 컨트롤에서 위치를 읽는다.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0);
		//5.2. 주소록에서 지운다.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		Erase(addressBook, index);
		//5.3. 리스트뷰 컨트롤에서 항목을 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEITEM, (WPARAM)index, (LPARAM)0);
		//5.4. 리스트뷰 컨트롤에서 지운 항목부터 번호를 다시 매긴다.
		item.mask = LVIF_TEXT;
		item.iSubItem = 0;
		i = index;
		while (i < addressBook->length) {
			item.iItem = i;
			sprintf(number, "%d", i + 1);
			item.pszText = number;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
			i++;
		}
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");

		SetFocus(GetDlgItem(hWnd, IDC_EDIT_NAME));
	}
	return TRUE;
}
/************************************************************************************************
명    칭: AddressBookForm_OnArrangeButtonClicked
기    능: 사용자가 정리하기 버튼을 클릭했을때의 시나리오.
*************************************************************************************************/
BOOL AddressBookForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	TCHAR number[64];
	LVITEM item = { 0, };
	Long i;
	Personal personal;

	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1. 주소록에서 정리한다.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		Arrange(addressBook);
		//6.2. 리스트뷰 컨트롤에서 모든 항목을 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, (WPARAM)0, (LPARAM)0);
		//6.3. 개수만큼 리스트뷰 컨트롤에 항목을 추가한다.
		item.mask = LVIF_TEXT;

		i = 0;
		while (i < addressBook->length) {
			item.iItem = i;
			personal = AddressBook_GetAt(addressBook, i);
			item.iSubItem = 0;
			sprintf(number, "%d", i + 1);
			item.pszText = number;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTITEM, (WPARAM)0, (LPARAM)&item);

			item.iSubItem = 1;
			item.pszText = personal.name;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
			item.iSubItem = 2;
			item.pszText = personal.address;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
			item.iSubItem = 3;
			item.pszText = personal.telephoneNumber;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);
			item.iSubItem = 4;
			item.pszText = personal.emailAddress;
			SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETITEMTEXT, (WPARAM)i, (LPARAM)&item);

			i++;
		}
	}
	return TRUE;
}
/************************************************************************************************
명    칭: AddressBookForm_OnNotify
기    능: WM_NOTIFY 하위
*************************************************************************************************/
BOOL AddressBookForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (((LPNMHDR)lParam)->idFrom) {
	case IDC_LIST_PERSONALS:
		ret = AddressBookForm_OnItemDoubleClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: AddressBookForm_OnItemDoubleClicked
기    능: 사용자가 항목을 더블클릭했을때의 시나리오.
*************************************************************************************************/
BOOL AddressBookForm_OnItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LVITEM item = { 0, };
	Long index;
	TCHAR name[11];
	TCHAR address[32];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];

	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//7.1. 리스트뷰 컨트롤에서 위치를 읽는다.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0);
		//7.2. 리스트뷰 컨트롤에서 항목을 읽는다.
		item.mask = LVIF_TEXT;
		item.iItem = index;

		item.iSubItem = 1;
		item.cchTextMax = 11;
		item.pszText = name;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		item.iSubItem = 2;
		item.cchTextMax = 32;
		item.pszText = address;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		item.iSubItem = 3;
		item.cchTextMax = 12;
		item.pszText = telephoneNumber;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		item.iSubItem = 4;
		item.cchTextMax = 32;
		item.pszText = emailAddress;
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETITEMTEXT, (WPARAM)index, (LPARAM)&item);
		//7.3. 성명, 주소, 전화번호, 이메일주소를 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, 0, (LPARAM)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, 0, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, 0, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, 0, (LPARAM)emailAddress);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: AddressBookForm_OnClose
기    능: 사용자가 닫기버튼을 클릭했을때의 시나리오.
*************************************************************************************************/
BOOL AddressBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;

	addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
	if (addressBook != NULL) {
		//8.1.주소록에서 저장한다.
		Save(addressBook);
		//8.2. 주소록을 지운다.
		AddressBook_Destroy(addressBook);
		free(addressBook);
	}
	//8.3. 윈도우를 닫는다.
	EndDialog(hWnd, 0);

	return TRUE;
}