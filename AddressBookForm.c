//AddressBookForm.c
/************************************************************************************************
���ϸ�Ī: AddressBookForm.c
��    ��: �ּҷ� GUI
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/04/28
*************************************************************************************************/
#include "AddressBookForm.h"
#include "AddressBook.h"
#include "FindingForm.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <commctrl.h>
#pragma warning (disable:4996)

//WInMain ����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int response;

	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDRESSBOOKFORM), NULL, AddressBookFormProc);

	return response;
}

/************************************************************************************************
��    Ī: AddressBookFormProc
��    ��: ������ ���ν��� �Լ�
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
��    Ī: AddressBookForm_OnInitDialog
��    ��: �����찡 �����Ǿ��� ���� �ó�����.
*************************************************************************************************/
BOOL AddressBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	Long count;
	Long i;
	LVCOLUMN column = { 0, };
	LVITEM item = { 0, };
	TCHAR number[64];
	Personal personal;

	//1.1.�ּҷ��� �����,
	addressBook = (AddressBook*)malloc(sizeof(AddressBook));
	AddressBook_Create(addressBook, 10000);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)addressBook);
	//1.2. �ּҷϿ��� �����Ѵ�.
	count = Load(addressBook);
	//1.3. ����Ʈ�� ��Ʈ���� ����� �����.
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
	//1.4. ������ ������ŭ ����Ʈ�� ��Ʈ�ѿ� �׸��� �߰��Ѵ�.
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
��    Ī: AddressBookForm_OnCommand
��    ��: WM_COMMAND ���� parameter
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
��    Ī: AddressBookForm_OnRecordButtonClicked
��    ��: ����ڰ� �����ϱ� ��ư�� Ŭ���������� �ó�����.
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
		//2.1. ����,�ּ�,��ȭ��ȣ,�̸����ּҸ� �д´�,
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)emailAddress);
		//2.2. �ּҷϿ��� �����Ѵ�.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Record(addressBook, name, address, telephoneNumber, emailAddress);
		//2.3. ����Ʈ�� ��Ʈ�ѿ� �׸��� �߰��Ѵ�.
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
��    Ī: AddressBookForm_OnFindButtonClicked
��    ��: ����ڰ� ã�� ��ư�� Ŭ���������� �ó�����.
*************************************************************************************************/
BOOL AddressBookForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1. ã�� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM),
			NULL, FindingFormProc);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: AddressBookForm_OnCorrectButtonClicked
��    ��: ����ڰ� ��ġ�� ��ư�� Ŭ���������� �ó�����.
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
		//4.1. ����Ʈ�� ��Ʈ�ѿ��� ��ġ�� �д´�.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, 0, 0);
		//4.2. �ּ�, ��ȭ��ȣ, �̸����ּҸ� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)emailAddress);
		//4.3. �ּҷϿ��� ��ģ��.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = Correct(addressBook, index, address, telephoneNumber, emailAddress);
		//4.4. ����Ʈ�� ��Ʈ�ѿ��� �׸��� ��ģ��.
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
��    Ī: AddressBookForm_OnEraseButtonClicked
��    ��: ����ڰ� ����� ��ư�� Ŭ���������� �ó�����.
*************************************************************************************************/
BOOL AddressBookForm_OnEraseButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	TCHAR number[64];
	LVITEM item = { 0, };
	Long index;
	Long i;
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1. ����Ʈ�� ��Ʈ�ѿ��� ��ġ�� �д´�.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0);
		//5.2. �ּҷϿ��� �����.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		Erase(addressBook, index);
		//5.3. ����Ʈ�� ��Ʈ�ѿ��� �׸��� �����.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEITEM, (WPARAM)index, (LPARAM)0);
		//5.4. ����Ʈ�� ��Ʈ�ѿ��� ���� �׸���� ��ȣ�� �ٽ� �ű��.
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
��    Ī: AddressBookForm_OnArrangeButtonClicked
��    ��: ����ڰ� �����ϱ� ��ư�� Ŭ���������� �ó�����.
*************************************************************************************************/
BOOL AddressBookForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;
	TCHAR number[64];
	LVITEM item = { 0, };
	Long i;
	Personal personal;

	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1. �ּҷϿ��� �����Ѵ�.
		addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
		Arrange(addressBook);
		//6.2. ����Ʈ�� ��Ʈ�ѿ��� ��� �׸��� �����.
		SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_DELETEALLITEMS, (WPARAM)0, (LPARAM)0);
		//6.3. ������ŭ ����Ʈ�� ��Ʈ�ѿ� �׸��� �߰��Ѵ�.
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
��    Ī: AddressBookForm_OnNotify
��    ��: WM_NOTIFY ����
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
��    Ī: AddressBookForm_OnItemDoubleClicked
��    ��: ����ڰ� �׸��� ����Ŭ���������� �ó�����.
*************************************************************************************************/
BOOL AddressBookForm_OnItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LVITEM item = { 0, };
	Long index;
	TCHAR name[11];
	TCHAR address[32];
	TCHAR telephoneNumber[12];
	TCHAR emailAddress[32];

	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//7.1. ����Ʈ�� ��Ʈ�ѿ��� ��ġ�� �д´�.
		index = SendMessage(GetDlgItem(hWnd, IDC_LIST_PERSONALS), LVM_GETSELECTIONMARK, (WPARAM)0, (LPARAM)0);
		//7.2. ����Ʈ�� ��Ʈ�ѿ��� �׸��� �д´�.
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
		//7.3. ����, �ּ�, ��ȭ��ȣ, �̸����ּҸ� ����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_SETTEXT, 0, (LPARAM)name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_ADDRESS), WM_SETTEXT, 0, (LPARAM)address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_TELEPHONENUMBER), WM_SETTEXT, 0, (LPARAM)telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EMAILADDRESS), WM_SETTEXT, 0, (LPARAM)emailAddress);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: AddressBookForm_OnClose
��    ��: ����ڰ� �ݱ��ư�� Ŭ���������� �ó�����.
*************************************************************************************************/
BOOL AddressBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	AddressBook* addressBook;

	addressBook = (AddressBook*)GetWindowLong(hWnd, GWL_USERDATA);
	if (addressBook != NULL) {
		//8.1.�ּҷϿ��� �����Ѵ�.
		Save(addressBook);
		//8.2. �ּҷ��� �����.
		AddressBook_Destroy(addressBook);
		free(addressBook);
	}
	//8.3. �����츦 �ݴ´�.
	EndDialog(hWnd, 0);

	return TRUE;
}