// Vanilla HashTable
#include "../stdafx.h"
#include "../VDefine.h"
#include "VHashTable.h"


VAPI(VanillaHashTable) VanillaHashTableCreate() {
	return new VHashTable;
}

VAPI(void) VanillaHashTableDestroy(VanillaHashTable HashTable) {
	if (!HashTable) {
		return;
	}
	delete HashTable;
}

VAPI(void) VanillaHashTableInsert(VanillaHashTable HashTable, VanillaText Key, VanillaBin Value) {
	if (!HashTable) {
		return;
	}
	VanillaString WS = Key;
	HashTable->insert(VHashTable::value_type(WS, Value));
}

VAPI(void) VanillaHashTableDelete(VanillaHashTable HashTable, VanillaText Key) {
	if (!HashTable) {
		return;
	}
	VanillaString WS = Key;
	HashTable->erase(WS);
}

VAPI(VanillaBin) VanillaHashTableGet(VanillaHashTable HashTable, VanillaText Key) {
	if (!HashTable) {
		return NULL;
	}
	VanillaString WS = Key;
	return (*HashTable) [WS];
}

VAPI(void) VanillaHashTableSet(VanillaHashTable HashTable, VanillaText Key, VanillaBin Value) {
	if (!HashTable) {
		return;
	}
	VanillaString WS = Key;
	(*HashTable) [WS] = Value;
}

VAPI(int) VanillaHashTableCount(VanillaHashTable HashTable) {
	return HashTable->size();
}

VAPI(VanillaHashTableItem) VanillaHashTableTravel(VanillaHashTable HashTable, VanillaHashTableTraveler* x) {
	if (!(*x)) {
		(*x) = new VHashTableTraveler;
		(*x)->x = HashTable->begin();
	} else {
		(*x)->x++;
	}
	if ((*x)->x == HashTable->end()) {
		delete (*x);
		return NULL;
	}
	return new VHashTableItem((*x)->x->first, (*x)->x->second);
}
