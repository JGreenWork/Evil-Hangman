#ifndef MY_STRING_H
#define MY_STRING_H
#include "my_vector.h"

#ifndef STATUS_H
#define STATUS_H
enum status { FAILURE, SUCCESS };
typedef enum status Status;
#endif

struct my_string_public;
typedef struct my_string_public* MY_STRING;

struct my_string_public
{
	void(*destroy)(MY_STRING* phMy_string);
	Status(*push_back) (MY_STRING hMy_string, char letter);
	Status(*pop_back)(MY_STRING hMy_string);
	int(*get_size)(MY_STRING hMy_string);
	int(*get_capacity)(MY_STRING hMy_string);
	char* (*at)(MY_STRING hMy_string, int index);
	Status(*extraction)(MY_STRING hMy_string, FILE* fp);
	char* (*c_str)(MY_STRING hMy_string);// (MAY RESIZE FOR NULL TERM)
	//string_compare
	//POST: returns -1 if string 1 < string 2
	//     returns 0 if strings are equivalent
	//       returns 1 if
	int(*string_compare)(MY_STRING hMy_string1, MY_STRING hMy_string2);
	//concatenation (put two strings together)
	Status(*concat)(MY_STRING hMy_string_dest, MY_STRING addition);
	Status(*get_line)(MY_STRING hMy_string, FILE* fp);
	Status(*item_assign)(Item_ptr* item_handle, Item_ptr item);
	Status(*destroy2) (Item_ptr* item_handle);
	//copy a string?
	//assignment for complete and independent copies of these things
};


// Pre-Condition:NONE
// Post-Condition:We get a MY_STRING object that can ho//ld 8 characters but currently holds none
MY_STRING my_string_init_default(void);
MY_STRING my_string_init_c_str(const char*);

#endif
