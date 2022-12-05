//FindingForm.c
/************************************************************************************************
파일명칭: FindingForm.c
기    능: 찾기윈도우 관리
작 성 자: 임다니엘
작성일자: 2022/04/29
*************************************************************************************************/
#include "FindingForm.h"
#include "AddressBook.h"
#include "resource.h"
#include <stdlib.h>
#include <commctrl.h>
/************************************************************************************************
명    칭: FindingFormProc
기    능: 찾기윈도우 프로시져 함수
*************************************************************************************************/
BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = FindingForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		ret = FindingForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_NOTIFY:
		ret = FindingForm_OnNotify(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = FindingForm_OnClose(hWnd, wParam, lParam);
		break;
	default :
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: FindingForm_OnInitDialog
기    능: 윈도우가 생성되었을 때의 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN column = { 0, };

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

	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETEXTENDEDLISTVIEWSTYLE,
		(WPARAM)LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnCommand
기    능: WM_COMMAND
*************************************************************************************************/
BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_FIND:
		ret = FindingForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnFindButtonClicked
기    능: 사용자가 찾기버튼을 눌렀을 때의 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hAddressBookFormWnd;
	AddressBook* addressBook;
	Long(*indexes);
	Long count;
	Long i;
	TCHAR name[11];
	TCHAR number[64];
	LVITEM item = { 0, };
	Personal personal;

	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1. 성명을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)name);
		//2.2. 주소록 윈도우를 찾는다.
		hAddressBookFormWnd = FindWindow("#32770", "주소록");
		//2.3. 주소록윈도우의 주소록에서 찾는다.
		addressBook = (AddressBook*)GetWindowLong(hAddressBookFormWnd, GWL_USERDATA);
		indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
		}
		Find(addressBook, name, &indexes, &count);
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
		//2.4. 리스트뷰 컨트롤에서 항목을 모두 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, 0, 0);
		//2.5. 리스트뷰 컨트롤에 찾은 개수만큼 항목들을 추가한다.
		item.mask = LVIF_TEXT;
		i = 0;
		while (i < count) {
			item.iItem = i;
			personal = AddressBook_GetAt(addressBook, indexes[i]);

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
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnNotify
기    능: WM_NOTIFY
*************************************************************************************************/
BOOL FindingForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (((LPNMHDR)lParam)->idFrom) {
	case IDC_LIST_PERSONALS:
		ret = FindingForm_OnItemDoubleClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: FindingForm_OnItemDoubleClicked
기    능: 사용자가 항목을 더블클릭했을 때의 시나리오
*************************************************************************************************/
BOOL FindingForm_OnItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hAddressBookFormWnd;
	LVITEM item = { 0, };
	LVITEM state = { 0, };
	TCHAR name[11];
	TCHAR address[32];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];
	Long index;
	Long(*indexes);
	Long i;
	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//3.1. 리스트뷰 컨트롤에서 위치를 읽는다.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);
		indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
		//3.2. 리스트뷰 컨트롤에서 항목을 읽는다.
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
		//3.3. 주소록 윈도우를 찾는다.
		hAddressBookFormWnd = FindWindow("#32770", "주소록");
		//3.4. 주소록 윈도우의 리스트뷰 컨트롤에서 항목을 선택한다.
		i = indexes[index];
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_LIST_PERSONALS), LVM_SETSELECTIONMARK, (WPARAM)0, (LPARAM)i);
		state.mask = LVIF_STATE;
		state.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		state.state = LVIS_SELECTED | LVIS_FOCUSED;
		
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_LIST_PERSONALS), LVM_SETITEMSTATE, (WPARAM)i, (LPARAM)&state);
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_LIST_PERSONALS), WM_SETFOCUS,0,0);
		
		//3.5. 주소록윈도우에 성명, 주소, 전화번호, 이메일주소를 쓴다.
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_EDIT_NAME), WM_SETTEXT, 0, (LPARAM)name);
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, 0, (LPARAM)address);
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, 0, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, 0, (LPARAM)emailAddress);


		SetFocus(GetDlgItem(hAddressBookFormWnd, IDC_EDIT_ADDRESS));

		if (indexes != NULL) {
			free(indexes);
		}
		
		EndDialog(hWnd, 0);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnClose
기    능: 사용자가 닫기버튼을 눌렀을 때의 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long(*indexes);

	indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
	if (indexes != NULL) {
		free(indexes);
	}
	//4.1. 찾기윈도우를 닫는다.
	EndDialog(hWnd, 0);

	return TRUE;
}