//AddressBook.c
/************************************************************************************************
파일명칭: AddressBook.c
기    능: 주소록 관리 원시코드(배열라이브러리 사용)
작 성 자: 임다니엘
작성일자: 2022/05/17
*************************************************************************************************/
#include "AddressBook.h"
#include <stdio.h>
#include <string.h>
#pragma warning (disable:4996)
#if 0
int main(int argc, char* argv[]) {
	AddressBook addressBook;
	Personal personal;
	Long index;
	Long(*indexes);
	Long count;
	Long i;

	AddressBook_Create(&addressBook, 3);

	printf("\n*Load*\n");
	count = Load(&addressBook);
	i = 0;
	while (i < count) {
		personal = AddressBook_GetAt(&addressBook, i);
		printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);
		i++;
	}

	printf("\n*Record*\n");
	index = Record(&addressBook, "Hong", "SeochoDong, Seoul", "01012345678", "hong@naver.com");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);
	index = Record(&addressBook, "Kim", "BangYiDong, Seoul", "01087654321", "kim@naver.com");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);
	index = Record(&addressBook, "Ko", "OcheonEup, Pohang", "01056781234", "ko@naver.com");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);
	index = Record(&addressBook, "Choi", "YangjaeDong, Seoul", "01043218765", "choi@gmail.com");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);
	index = Record(&addressBook, "Hong", "SajikDong, Busan", "01098765432", "hgd@gmail.com");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);
	
	printf("\n*Find*\n");
	Find(&addressBook, "Hong", &indexes, &count);
	i = 0;
	while (i < count) {
		personal = AddressBook_GetAt(&addressBook, indexes[i]);
		printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);
		i++;
	}
	if (indexes != NULL) {
		free(indexes);
	}

	printf("\n*Correct*\n");
	index = Correct(&addressBook, 1, "DogokDong, Seoul", "01087654321", "gildongK@naver.com");
	personal = AddressBook_GetAt(&addressBook, index);
	printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);

	printf("\n*Erase*\n");
	index = Erase(&addressBook, 1);
	if (index == -1) {
		printf("Erased!\n");
	}

	printf("\n*Arrange*\n");
	Arrange(&addressBook);
	i = 0;
	while (i < addressBook.length) {
		personal = AddressBook_GetAt(&addressBook, i);
		printf("%s\t   %s\t%s\t%s\n", personal.name, personal.address, personal.telephoneNumber, personal.emailAddress);
		i++;
	}

	Save(&addressBook);

	AddressBook_Destroy(&addressBook);

	return 0;
}
#endif
/************************************************************************************************
명    칭: AddressBook_Create
기    능: 주소록을 만든다.
입    력: 할당량
출    력: 없음
*************************************************************************************************/
void AddressBook_Create(AddressBook* addressBook, Long capacity) {
	Create(&(addressBook->personals), capacity, sizeof(Personal));
	addressBook->capacity = capacity;
	addressBook->length = 0;
}
/************************************************************************************************
명    칭: Load
기    능: 디스크파일로부터 주소록을 입력받는다.
입    력: 없음
출    력: 개수
*************************************************************************************************/
Long Load(AddressBook* addressBook) {
	Personal personal;
	Long index;
	FILE* file;
	size_t flag;

	file = fopen("AddressBook.dat", "rb");
	if (file != NULL) {
		flag = fread(&personal, sizeof(Personal), 1, file);
		while (!feof(file) && flag != 0) {
			if (addressBook->length < addressBook->capacity) {
				index = Store(&(addressBook->personals), addressBook->length, &personal, sizeof(Personal));
			}
			else {
				index = AppendFromRear(&(addressBook->personals), &personal, sizeof(Personal));
				addressBook->capacity++;
			}
			addressBook->length++;
			flag = fread(&personal, sizeof(Personal), 1, file);
		}
		fclose(file);
	}
	return addressBook->length;
}
/************************************************************************************************
명    칭: Record
기    능: 주소록에 기재한다.
입    력: 성명, 주소, 전화번호, 이메일주소
출    력: 위치
*************************************************************************************************/
Long Record(AddressBook* addressBook, char(*name), char(*address), char(*telephoneNumber), char(*emailAddress)) {
	Personal personal;
	Long index;

	strcpy(personal.name, name);
	strcpy(personal.address, address);
	strcpy(personal.telephoneNumber, telephoneNumber);
	strcpy(personal.emailAddress, emailAddress);

	if (addressBook->length < addressBook->capacity) {
		index = Store(&(addressBook->personals), addressBook->length, &personal, sizeof(Personal));
	}
	else {
		index = AppendFromRear(&(addressBook->personals), &personal, sizeof(Personal));
		addressBook->capacity++;
	}
	addressBook->length++;

	return index;
}
/************************************************************************************************
명    칭: Find
기    능: 주소록에서 찾는다.
입    력: 성명
출    력: 위치들, 개수
*************************************************************************************************/
void Find(AddressBook* addressBook, char(*name), Long* (*indexes), Long* count) {
	LinearSearchDuplicate(&(addressBook->personals), name, indexes, count, sizeof(Personal), CompareNames);
}
/************************************************************************************************
명    칭: Correct
기    능: 주소록을 고친다.
입    력: 위치, 주소, 전화번호, 이메일주소
출    력: 위치
*************************************************************************************************/
Long Correct(AddressBook* addressBook, Long index, char(*address), char(*telephoneNumber), char(*emailAddress)) {
	Personal personal;

	GetAt(&(addressBook->personals), index, &personal, sizeof(Personal));

	strcpy(personal.address, address);
	strcpy(personal.telephoneNumber, telephoneNumber);
	strcpy(personal.emailAddress, emailAddress);

	index = Modify(&(addressBook->personals), index, &personal, sizeof(Personal));

	return index;
}
/************************************************************************************************
명    칭: Erase
기    능: 주소록에서 지운다.
입    력: 위치
출    력: 위치
*************************************************************************************************/
Long Erase(AddressBook* addressBook, Long index) {
	index = Delete(&(addressBook->personals), index, sizeof(Personal));

	addressBook->capacity--;
	addressBook->length--;

	return index;
}
/************************************************************************************************
명    칭: Arrange
기    능: 주소록을 정리한다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void Arrange(AddressBook* addressBook) {
	SelectionSort(&(addressBook->personals), sizeof(Personal), ComparePersonals);
}
/************************************************************************************************
명    칭: AddressBook_GetAt
기    능: 주소록의 항목을 읽어온다.
입    력: 위치
출    력: 개인
*************************************************************************************************/
Personal AddressBook_GetAt(AddressBook* addressBook, Long index) {
	Personal personal;

	GetAt(&(addressBook->personals), index, &personal, sizeof(Personal));

	return personal;
}
/************************************************************************************************
명    칭: Save
기    능: 주소록을 디스크파일로 저장한다.
입    력: 없음
출    력: 개수
*************************************************************************************************/
Long Save(AddressBook* addressBook) {
	Personal personal;
	Long i = 0;
	FILE* file;

	file = fopen("addressBook.dat", "wb");
	if (file != NULL) {
		while (i < addressBook->length) {
			GetAt(&(addressBook->personals), i, &personal, sizeof(Personal));
			fwrite(&personal, sizeof(Personal), 1, file);
			i++;
		}
		fclose(file);
	}
	return addressBook->length;
}
/************************************************************************************************
명    칭: AddressBook_Destroy
기    능: 주소록을 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void AddressBook_Destroy(AddressBook* addressBook) {
	Destroy(&(addressBook->personals));
}

//함수 포인터 매개변수로 사용될 함수
//찾기
int CompareNames(void* one, void* other) {
	Personal* one_ = (Personal*)one;
	char(*name) = (char(*))other;

	return strcmp(one_->name, name);
}
//정리하기
int ComparePersonals(void* one, void* other) {
	Personal* one_ = (Personal*)one;
	Personal* other_ = (Personal*)other;

	return strcmp(one_->name, other_->name);
}