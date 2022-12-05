//AddressBook.h
/************************************************************************************************
파일명칭: AddressBook.h
기    능: 주소록 자료형 및 함수 선언
작 성 자: 임다니엘
작성일자: 2022/05/17
*************************************************************************************************/
#ifndef _ADDRESSBOOK_H
#define _ADDRESSBOOK_H
#include "Array.h"

typedef signed long int Long;
typedef struct _personal {
	char name[11];
	char address[32];
	char telephoneNumber[12];
	char emailAddress[32];
}Personal;
typedef struct _addressBook {
	Array personals;
	Long capacity;
	Long length;
}AddressBook;

void AddressBook_Create(AddressBook* addressBook, Long capacity);
Long Load(AddressBook* addressBook);
Long Record(AddressBook* addressBook, char (*name), char(*address), char(*telephoneNumber), char(*emailAddress));
void Find(AddressBook* addressBook, char(*name), Long* (*indexes), Long* count);
Long Correct(AddressBook* addressBook, Long index, char(*address), char(*telephoneNumber), char(*emailAddress));
Long Erase(AddressBook* addressBook, Long index);
void Arrange(AddressBook* addressBook);
Personal AddressBook_GetAt(AddressBook* addressBook, Long index);
Long Save(AddressBook* addressBook);
void AddressBook_Destroy(AddressBook* addressBook);

//함수 포인터형 매개변수로 사용될 함수
int CompareNames(void* one, void* other);
int ComparePersonals(void* one, void* other);

#endif //_ADDRESSBOOK_H