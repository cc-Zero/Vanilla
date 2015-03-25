// Vanilla SkinDB
#include "../stdafx.h"
#include "../VDefine.h"
#include "../VStruct.h"
#include "VHashTable.h"
#include "VSkinDB.h"


VanillaVoid WriteFileBin(FILE* File, VanillaBin Bin) {
	int Length = Bin->Length;
	fwrite(&Length, 1, 4, File);
	fwrite(Bin->Address, 1, Length, File);
}

VanillaBin ReadFileBin(FILE* File) {
	int Length = 0;
	/*前4字节数据长度*/
	fread(&Length, 1, 4, File);
	/*分配内存空间*/
	VanillaByte* Buffer = new VanillaByte[Length];
	/*读取数据到内存*/
	fread(Buffer, 1, Length, File);
	VanillaBin b = MALLOCVBIN();
	MAKEVBINP(b, Buffer, Length);
	return b;
}

VanillaVoid WriteFileText(FILE* File, VanillaText String) {
	int Length = strlen(String);
	fwrite(&Length, 1, 4, File);
	fwrite(String, 1, Length, File);
}

VanillaText ReadFileText(FILE* File) {
	int Length = 0;
	/*前4字节文本长度*/
	fread(&Length, 1, 4, File);
	/*分配内存空间*/
	char* Buffer = new char[Length + 1];
	/*读取文本到内存*/
	fread(Buffer, 1, Length, File);
	Buffer [Length] = 0;
	return Buffer;
}

VanillaVoid WriteFileInt(FILE* File, VanillaInt Value) {
	fwrite(&Value, 1, 4, File);
}

VanillaInt ReadFileInt(FILE* File) {
	VanillaInt Buffer = 0;
	fread(&Buffer, 1, 4, File);
	return Buffer;
}

VAPI(VanillaSkinDB) VanillaSkinDBCreate(VanillaText Name) {
	return new VSkinDB(Name);
}

VAPI(VanillaVoid) VanillaSkinDBDestroy(VanillaSkinDB SkinDB) {
	delete SkinDB;
}

VAPI(VanillaVoid) VanillaSkinDBSet(VanillaSkinDB SkinDB, VanillaText Name, VanillaBin Data) {
	VanillaHashTableSet(SkinDB->HashTable, Name, Data);
}

VAPI(VanillaBin) VanillaSkinDBGet(VanillaSkinDB SkinDB, VanillaText Name) {
	return VanillaHashTableGet(SkinDB->HashTable, Name);
}

VAPI(VanillaVoid) VanillaSkinDBDelete(VanillaSkinDB SkinDB, VanillaText Name) {
	delete VanillaHashTableGet(SkinDB->HashTable, Name);
	VanillaHashTableDelete(SkinDB->HashTable, Name);
}

VAPI(bool) VanillaSkinDBSave(VanillaSkinDB SkinDB, VanillaText FileName) {
	if (!SkinDB) {
		return false;
	}
	FILE* File = fopen(FileName, "wb");
	if (!File) {
		return false;
	}

	fwrite("Vanilla!", 1, 8, File);
	WriteFileText(File, SkinDB->Name);
	WriteFileInt(File, VanillaHashTableCount(SkinDB->HashTable));

	VanillaHashTableTraveler Traveler = NULL;
	VanillaHashTableItem Item = NULL;
	while ((Item = VanillaHashTableTravel(SkinDB->HashTable, &Traveler)) != NULL) {
		WriteFileText(File, (VanillaText)Item->Key.c_str());
		WriteFileBin(File, Item->Value);
		delete Item;
	}
	fclose(File);
	return true;
}

VAPI(VanillaSkinDB) VanillaSkinDBLoad(VanillaText FileName) {
	FILE* File = fopen(FileName, "rb");
	if (!File) {
		return NULL;
	}
	char FileHead [9];
	FileHead [8] = 0;
	/*验证文件前8字节Vanilla!标志*/
	fread(&FileHead, 1, 8, File);
	if (strcmp(FileHead, "Vanilla!") != 0) {
		return NULL;
	}
	VanillaSkinDB SkinDB = new VSkinDB;
	/*读取资源包名称*/
	SkinDB->Name = ReadFileText(File);
	/*读取资源包资源数量*/
	int Count = ReadFileInt(File);
	for (int i = 0;i < Count;i++)
	{
		/*读出资源*/
		VanillaText Name = ReadFileText(File);
		VanillaBin Data = ReadFileBin(File);
		VanillaHashTableSet(SkinDB->HashTable, Name, Data);
		delete Name;
	}
	fclose(File);
	return SkinDB;
}

VSkinDB::VSkinDB() {
	this->Name = NULL;
	this->HashTable = new VHashTable;
}

VSkinDB::VSkinDB(VanillaText Name) {
	this->Name = Name;
	this->HashTable = new VHashTable;
}

VSkinDB::~VSkinDB() {
	if (this->HashTable) {
		VanillaHashTableTraveler Traveler = NULL;
		VanillaHashTableItem Item = NULL;
		while ((Item = VanillaHashTableTravel(this->HashTable, &Traveler))) {
			delete Item->Value;
		}
		delete this->HashTable;
	}
}
