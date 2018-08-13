#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"my_string.h"



void my_string_destroy(MY_STRING* phMy_string);
Status my_string_push_back(MY_STRING hMy_string, char letter);
Status my_string_pop_back(MY_STRING hMy_string);
int my_string_get_size(MY_STRING hMy_string);
int my_string_get_capacity(MY_STRING hMy_string);
char* my_string_at(MY_STRING hMy_string, int index);
Status my_string_extraction(MY_STRING hMy_string, FILE* fp);
char* my_string_c_str(MY_STRING hMy_string);
int my_string_compare(MY_STRING hMy_string1, MY_STRING hMy_string2);
Status my_string_concatenation(MY_STRING hMy_string_dest, MY_STRING hMy_string_addition);
Status get_line(MY_STRING hMy_string, FILE* fp);
Status item_assign(Item_ptr* item_handle, Item_ptr item);
void destroy2(Item_ptr* item_handle);

struct my_string
{
	void(*destroy) (MY_STRING* phMy_string);
	Status(*push_back)(MY_STRING hMy_string, char letter);
	Status(*pop_back) (MY_STRING hMy_string);
	int(*get_size) (MY_STRING hMy_string);
	int(*get_capacity) (MY_STRING hMy_string);
	char* (*at)(MY_STRING hMy_string, int index);
	Status(*extraction)(MY_STRING hMy_string, FILE* fp);
	char* (*c_str)(MY_STRING hMy_string);
	int(*string_compare)(MY_STRING hMy_string1, MY_STRING hMy_string2);
	Status(*concat)(MY_STRING hMy_string_dest, MY_STRING addition);
	Status(*get_line)(MY_STRING hMy_string, FILE* fp);
	Status(*item_assign)(Item_ptr* item_handle, Item_ptr item);
	void(*destroy2)(Item_ptr* item_handle);

	int size;
	int capacity;
	char* data;
};

typedef struct my_string My_string;
typedef My_string* my_string_ptr;


void init_functions(my_string_ptr pString)
{
	pString->destroy = my_string_destroy;
	pString->push_back = my_string_push_back;
	pString->pop_back = my_string_pop_back;
	pString->get_size = my_string_get_size;
	pString->get_capacity = my_string_get_capacity;
	pString->at = my_string_at;
	pString->extraction = my_string_extraction;
	pString->c_str = my_string_c_str;
	pString->string_compare = my_string_compare;
	pString->concat = my_string_concatenation;
	pString->get_line = get_line;
	pString->item_assign = item_assign;
	pString->destroy2 = destroy2;
}


MY_STRING my_string_init_default(void)
{

	my_string_ptr pString = (my_string_ptr)malloc(sizeof(My_string));
	if (pString != NULL)
	{
		init_functions(pString);

		pString->size = 0;
		pString->capacity = 8;
		pString->data = (char*)malloc(sizeof(char)*pString->capacity);
		if (pString->data == NULL)
		{
			free(pString);
			pString = NULL;
		}
	}
	return (MY_STRING)pString;
}

Status item_assign(Item_ptr* item_handle, Item_ptr item)
{
	my_string_ptr handle = (my_string_ptr)*item_handle;
	my_string_ptr pItem = (my_string_ptr)item;
	int i;
	if (handle == NULL)
	{
		handle = (my_string_ptr)my_string_init_default();
		if (handle == NULL)
		{
			return FAILURE;
		}
	}
	handle->size = 0;
	for (i = 0; i<pItem->get_size((MY_STRING)pItem); i++)
	{
		handle->push_back((MY_STRING)handle, pItem->data[i]);
	}
	// *handle = pItem;
	*item_handle = handle;
	return SUCCESS;
}
void destroy2(Item_ptr* item_handle)
{
	my_string_ptr pString = (my_string_ptr)*item_handle;
	if (pString != NULL)
	{
		pString->destroy(&pString);
		free(pString);
		*item_handle = NULL;
	}
}
MY_STRING my_string_init_c_str(const char* begin)
{


	int x = 0;
	int size = 0;
	my_string_ptr pString = (my_string_ptr)malloc(sizeof(My_string));


	while (begin[x] != '\0')
	{
		x++;
		size++;
	}

	if (pString != NULL)
	{
		init_functions(pString);
		pString->size = 0;
		pString->capacity = size + 5;
		pString->data = (char*)malloc(sizeof(char) * pString->capacity);
		if (pString->data == NULL)
		{
			free(pString);
			pString = NULL;
		}
	}

	x = 0;
	while (begin[x] != '\0')
	{
		pString->push_back((MY_STRING)pString, begin[x]);
		x++;
	}
	return (MY_STRING)pString;


}

void my_string_destroy(MY_STRING* phMy_string)
{
	my_string_ptr pString = (my_string_ptr)*phMy_string;

	if (pString != NULL)
	{
		free(pString->data);
		free(pString);
		*phMy_string = NULL;
	}
}

Status my_string_push_back(MY_STRING hMy_string, char letter)
{
	char* temp;
	int i;
	my_string_ptr string = (my_string_ptr)hMy_string;
	if (string->size >= string->capacity)
	{
		string->capacity *= 2;
		temp = (char*)malloc(sizeof(char)*string->capacity);
		if (temp == NULL)
		{
			string->capacity /= 2;
			return FAILURE;
		}
		for (i = 0; i<string->size; i++)
		{
			temp[i] = string->data[i];
		}
		free(string->data);
		string->data = temp;
	}
	string->data[string->size] = letter;
	string->size++;
	return SUCCESS;
}

Status my_string_pop_back(MY_STRING hMy_string)
{
	my_string_ptr string = (my_string_ptr)hMy_string;
	if (string->size > 0)
	{
		string->size--;
		return SUCCESS;
	}
	return FAILURE;
}

int my_string_get_size(MY_STRING hMy_string)
{
	my_string_ptr string = (my_string_ptr)hMy_string;
	return string->size;
}

int my_string_get_capacity(MY_STRING hMy_string)
{
	my_string_ptr string = (my_string_ptr)hMy_string;
	return string->capacity;
}

char* my_string_at(MY_STRING hMy_string, int index)
{
	my_string_ptr letter = (my_string_ptr)hMy_string;
	if (index<0 || index >= letter->size)
	{
		return NULL;
	}
	return &(letter->data[index]);
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp)
{
	char c;

	my_string_ptr pString = (my_string_ptr)hMy_string;
	if (fp != NULL)
	{
		pString->size = 0;

		fscanf(fp, " ");
		c = getc(fp);
		while (!(isalpha(c)) && c != EOF)
		{
			c = getc(fp);
		}
		if (isalpha(c))
			pString->push_back((MY_STRING)pString, c);
		while (!(isspace(c)) && c != EOF)
		{
			c = getc(fp);
			if (isspace(c))
			{
				ungetc(c, fp);
				break;
			}
			else
			{
				if (c == EOF)
				{
					break;
				}
				pString->push_back((MY_STRING)pString, c);
			}
		}
		if (pString->get_size((MY_STRING)pString) == 0)
		{
			return FAILURE;
		}
		return SUCCESS;
	}
	return FAILURE;
}

char* my_string_c_str(MY_STRING hMy_string)
{
	int i;
	my_string_ptr cstring = (my_string_ptr)hMy_string;

	if (cstring->size >= cstring->capacity)
	{
		char* temp;
		cstring->capacity += 5;
		temp = (char*)malloc(sizeof(char) * cstring->capacity);
		if (temp == NULL)
		{
			cstring->capacity -= 5;
			return NULL;
		}
		for (i = 0; i<cstring->size; i++)
		{
			temp[i] = cstring->data[i];
		}
		free(cstring->data);
		cstring->data = temp;
	}
	cstring->data[cstring->size] = '\0';
	return cstring->at((MY_STRING)cstring, 0);
}

int my_string_compare(MY_STRING hMy_string1, MY_STRING hMy_string2)
{
	my_string_ptr pString1 = (my_string_ptr)hMy_string1;
	my_string_ptr pString2 = (my_string_ptr)hMy_string2;

	int size1;
	int size2;
	int n = 0;

	size1 = pString1->get_size((MY_STRING)pString1);
	size2 = pString2->get_size((MY_STRING)pString2);

	while (n<size1 && n<size2)
	{
		if ((int)*pString1->at((MY_STRING)pString1, n) < (int)*pString2->at((MY_STRING)pString2, n))
			return -1;
		if ((int)*pString1->at((MY_STRING)pString1, n) > (int)*pString2->at((MY_STRING)pString2, n))
			return 1;
		n++;
	}

	if (size1<size2)
		return -1;
	if (size1>size2)
		return 1;
	else
	{
		return 0;
	}
}

Status my_string_concatenation(MY_STRING hMy_string_dest, MY_STRING hMy_string_addition)
{
	my_string_ptr dest = (my_string_ptr)hMy_string_dest;
	my_string_ptr addition = (my_string_ptr)hMy_string_addition;

	int size2;
	int i;
	if (addition->size == 0)
		return FAILURE;
	if ((size2 = dest->get_size((MY_STRING)addition)) == 0)
	{
		return FAILURE;
	}

	for (i = 0; i<size2; i++)
	{
		dest->push_back((MY_STRING)dest, *addition->at((MY_STRING)addition, i));
	}
	return SUCCESS;
}
Status get_line(MY_STRING hMy_string, FILE* fp)
{
	char c;

	my_string_ptr pString = (my_string_ptr)hMy_string;
	if (fp != NULL)
	{
		pString->size = 0;
		c = getc(fp);
		while (c != '\n')
		{
			if (feof(fp))
			{
				break;
			}
			pString->push_back((MY_STRING)pString, c);
			c = getc(fp);
		}
		return SUCCESS;
	}
	return FAILURE;
}
