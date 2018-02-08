#include "SPArrayList.h"
#include <string.h> 
#include <stdlib.h>

SPArrayList* spArrayListCreate(int maxSize) {
	if (maxSize <= 0) {
		return NULL;
	}

	SPArrayList *al = (SPArrayList*)(malloc(sizeof(SPArrayList)));
	if ((void*)al == NULL) {
		return NULL;
	}

	al->elements = (int*)(malloc(sizeof(int) * maxSize));
	if ((void*)(al->elements) == NULL) {
		free(al);
		return NULL;
	}

	al->maxSize = maxSize;
	al->actualSize = 0;

	return al;
}

/*
* Copies array elements to other array.
* Assumes number_of_elements < dst.length, src.length
*/
static void copyElements(int* src, int* dst, int number_of_elements) {
	int i;
	
	for (i = 0; i < number_of_elements; i++) {
		dst[i] = src[i];
	}
}

SPArrayList* spArrayListCopy(SPArrayList* src) {
	if ((void*)src == NULL) {
		return NULL;
	}

	SPArrayList *al = (SPArrayList*)(malloc(sizeof(SPArrayList)));
	if ((void*)al == NULL) {
		return NULL;
	}

	al->elements = (int*)(malloc(sizeof(int) * src->maxSize));
	if ((void*)(al->elements) == NULL) {
		free(al);
		return NULL;
	}

	copyElements(src->elements, al->elements, src->actualSize);

	al->maxSize = src->maxSize;
	al->actualSize = src->actualSize;

	return al;
}

void spArrayListDestroy(SPArrayList* src) {
	if ((void*)src == NULL) {
		return;
	}

	free(src->elements);
	free(src);
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src) {
	int i;

	if ((void*)src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	// clear the array
	for (i = 0; i < src->actualSize; i++) {
		(src->elements)[i] = 0;
	}

	src->actualSize = 0;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index) {
	int i;

	if ((void*)src == NULL || index < 0 || index > src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if (src->actualSize == src->maxSize) {
		return SP_ARRAY_LIST_FULL;
	}

	// shift the array!
	for (i = src->actualSize - 1; i >= index; i--) {
		(src->elements)[i + 1] = (src->elements)[i];
	}

	// insert new val
	(src->elements)[index] = elem;
	(src->actualSize)++;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem) {
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem) {
	if ((void*)src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if (src->actualSize == src->maxSize) {
		return SP_ARRAY_LIST_FULL;
	}

	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index) {
	int i;

	if ((void*)src == NULL || index < 0 || index >= src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	// redundant but specified 
	if (src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}

	// shift the array!
	for (i = index + 1; i < src->actualSize; i++) {
		(src->elements)[i - 1] = (src->elements)[i];
	}

	(src->actualSize)--;

	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src) {
	if ((void*)src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if (src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}

	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src) {
	if ((void*)src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	if (src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}

	return spArrayListRemoveAt(src, src->actualSize - 1);
}

int spArrayListGetAt(SPArrayList* src, int index) {
	if ((void*)src == NULL || index < 0 || index >= src->actualSize) {
		return -1;
	}

	return (src->elements)[index];
}

int spArrayListGetFirst(SPArrayList* src) {
	return spArrayListGetAt(src, 0);
}

int spArrayListGetLast(SPArrayList* src) {
	if ((void*)src == NULL) {
		return -1;
	}

	return spArrayListGetAt(src, src->actualSize - 1);
}

int spArrayListMaxCapacity(SPArrayList* src) {
	if ((void*)src == NULL) {
		return -1;
	}

	return src->maxSize;
}

int spArrayListSize(SPArrayList* src) {
	if ((void*)src == NULL) {
		return -1;
	}

	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src) {
	if ((void*)src == NULL) {
		return false;
	}

	return src->actualSize == src->maxSize;
}

bool spArrayListIsEmpty(SPArrayList* src) {
	if ((void*)src == NULL) {
		return false;
	}

	return src->actualSize == 0;
}