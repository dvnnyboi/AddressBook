//FindingForm.c
/************************************************************************************************
���ϸ�Ī: FindingForm.c
��    ��: ã�������� ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/04/29
*************************************************************************************************/
#include "FindingForm.h"
#include "AddressBook.h"
#include "resource.h"
#include <stdlib.h>
#include <commctrl.h>
/************************************************************************************************
��    Ī: FindingFormProc
��    ��: ã�������� ���ν��� �Լ�
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
��    Ī: FindingForm_OnInitDialog
��    ��: �����찡 �����Ǿ��� ���� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LVCOLUMN column = { 0, };

	column.mask = LVCF_WIDTH | LVCF_TEXT;
	column.cx = 40;
	column.pszText = "��ȣ";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&column);
	column.cx = 80;
	column.pszText = "����";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&column);
	column.cx = 160;
	column.pszText = "�ּ�";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)2, (LPARAM)&column);
	column.cx = 80;
	column.pszText = "��ȭ��ȣ";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)3, (LPARAM)&column);
	column.cx = 120;
	column.pszText = "�̸����ּ�";
	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_INSERTCOLUMN, (WPARAM)4, (LPARAM)&column);

	SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_SETEXTENDEDLISTVIEWSTYLE,
		(WPARAM)LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, (LPARAM)LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return TRUE;
}
/************************************************************************************************
��    Ī: FindingForm_OnCommand
��    ��: WM_COMMAND
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
��    Ī: FindingForm_OnFindButtonClicked
��    ��: ����ڰ� ã���ư�� ������ ���� �ó�����.
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
		//2.1. ������ �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)name);
		//2.2. �ּҷ� �����츦 ã�´�.
		hAddressBookFormWnd = FindWindow("#32770", "�ּҷ�");
		//2.3. �ּҷ��������� �ּҷϿ��� ã�´�.
		addressBook = (AddressBook*)GetWindowLong(hAddressBookFormWnd, GWL_USERDATA);
		indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
		}
		Find(addressBook, name, &indexes, &count);
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
		//2.4. ����Ʈ�� ��Ʈ�ѿ��� �׸��� ��� �����.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, 0, 0);
		//2.5. ����Ʈ�� ��Ʈ�ѿ� ã�� ������ŭ �׸���� �߰��Ѵ�.
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
��    Ī: FindingForm_OnNotify
��    ��: WM_NOTIFY
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
��    Ī: FindingForm_OnItemDoubleClicked
��    ��: ����ڰ� �׸��� ����Ŭ������ ���� �ó�����
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
		//3.1. ����Ʈ�� ��Ʈ�ѿ��� ��ġ�� �д´�.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);
		indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
		//3.2. ����Ʈ�� ��Ʈ�ѿ��� �׸��� �д´�.
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
		//3.3. �ּҷ� �����츦 ã�´�.
		hAddressBookFormWnd = FindWindow("#32770", "�ּҷ�");
		//3.4. �ּҷ� �������� ����Ʈ�� ��Ʈ�ѿ��� �׸��� �����Ѵ�.
		i = indexes[index];
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_LIST_PERSONALS), LVM_SETSELECTIONMARK, (WPARAM)0, (LPARAM)i);
		state.mask = LVIF_STATE;
		state.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		state.state = LVIS_SELECTED | LVIS_FOCUSED;
		
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_LIST_PERSONALS), LVM_SETITEMSTATE, (WPARAM)i, (LPARAM)&state);
		SendMessage(GetDlgItem(hAddressBookFormWnd, IDC_LIST_PERSONALS), WM_SETFOCUS,0,0);
		
		//3.5. �ּҷ������쿡 ����, �ּ�, ��ȭ��ȣ, �̸����ּҸ� ����.
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
��    Ī: FindingForm_OnClose
��    ��: ����ڰ� �ݱ��ư�� ������ ���� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	Long(*indexes);

	indexes = (Long(*))GetWindowLong(hWnd, GWL_USERDATA);
	if (indexes != NULL) {
		free(indexes);
	}
	//4.1. ã�������츦 �ݴ´�.
	EndDialog(hWnd, 0);

	return TRUE;
}