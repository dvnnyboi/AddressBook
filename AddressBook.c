//AddressBook.c
/************************************************************************************************
���ϸ�Ī: AddressBook.c
��    ��: �ּҷ� ���� �����ڵ�(�迭���̺귯�� ���)
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/05/17
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
��    Ī: AddressBook_Create
��    ��: �ּҷ��� �����.
��    ��: �Ҵ緮
��    ��: ����
*************************************************************************************************/
void AddressBook_Create(AddressBook* addressBook, Long capacity) {
	Create(&(addressBook->personals), capacity, sizeof(Personal));
	addressBook->capacity = capacity;
	addressBook->length = 0;
}
/************************************************************************************************
��    Ī: Load
��    ��: ��ũ���Ϸκ��� �ּҷ��� �Է¹޴´�.
��    ��: ����
��    ��: ����
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
��    Ī: Record
��    ��: �ּҷϿ� �����Ѵ�.
��    ��: ����, �ּ�, ��ȭ��ȣ, �̸����ּ�
��    ��: ��ġ
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
��    Ī: Find
��    ��: �ּҷϿ��� ã�´�.
��    ��: ����
��    ��: ��ġ��, ����
*************************************************************************************************/
void Find(AddressBook* addressBook, char(*name), Long* (*indexes), Long* count) {
	LinearSearchDuplicate(&(addressBook->personals), name, indexes, count, sizeof(Personal), CompareNames);
}
/************************************************************************************************
��    Ī: Correct
��    ��: �ּҷ��� ��ģ��.
��    ��: ��ġ, �ּ�, ��ȭ��ȣ, �̸����ּ�
��    ��: ��ġ
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
��    Ī: Erase
��    ��: �ּҷϿ��� �����.
��    ��: ��ġ
��    ��: ��ġ
*************************************************************************************************/
Long Erase(AddressBook* addressBook, Long index) {
	index = Delete(&(addressBook->personals), index, sizeof(Personal));

	addressBook->capacity--;
	addressBook->length--;

	return index;
}
/************************************************************************************************
��    Ī: Arrange
��    ��: �ּҷ��� �����Ѵ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void Arrange(AddressBook* addressBook) {
	SelectionSort(&(addressBook->personals), sizeof(Personal), ComparePersonals);
}
/************************************************************************************************
��    Ī: AddressBook_GetAt
��    ��: �ּҷ��� �׸��� �о�´�.
��    ��: ��ġ
��    ��: ����
*************************************************************************************************/
Personal AddressBook_GetAt(AddressBook* addressBook, Long index) {
	Personal personal;

	GetAt(&(addressBook->personals), index, &personal, sizeof(Personal));

	return personal;
}
/************************************************************************************************
��    Ī: Save
��    ��: �ּҷ��� ��ũ���Ϸ� �����Ѵ�.
��    ��: ����
��    ��: ����
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
��    Ī: AddressBook_Destroy
��    ��: �ּҷ��� ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void AddressBook_Destroy(AddressBook* addressBook) {
	Destroy(&(addressBook->personals));
}

//�Լ� ������ �Ű������� ���� �Լ�
//ã��
int CompareNames(void* one, void* other) {
	Personal* one_ = (Personal*)one;
	char(*name) = (char(*))other;

	return strcmp(one_->name, name);
}
//�����ϱ�
int ComparePersonals(void* one, void* other) {
	Personal* one_ = (Personal*)one;
	Personal* other_ = (Personal*)other;

	return strcmp(one_->name, other_->name);
}