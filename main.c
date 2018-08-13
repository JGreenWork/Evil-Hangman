#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "my_vector.h"
#include "my_string.h"


struct node;
typedef struct node Node;
typedef Node* Node_ptr;
struct node
{
MY_STRING keyString;
MY_VECTOR words;
Node_ptr next;
};


Node_ptr insertNode(Node_ptr head, MY_STRING key, MY_STRING word);
void deleteList(Node_ptr* head);
Node_ptr keySearch(Node_ptr head, MY_STRING key, MY_STRING word);
void traverseList(Node_ptr head);
Node_ptr popMaxNode(Node_ptr* head);
void clear_keyboard_buffer(void);
void clear_vector(MY_VECTOR hVector);
void clear_string(MY_STRING hString);




int main(int argc, char* argv[])
{
	//	MY_VECTOR hVector;
	MY_VECTOR hGame;
	MY_STRING hString, hStringKey, stringDisplay, testString, tempString;
	FILE* fp;
	int i, j, k, wordsize, guessAmount,is_key_blank, victory;
	char cont, hardmode;
	char letters[26];
	Node_ptr head = NULL, temp = NULL;

	hString = my_string_init_default();
	stringDisplay = my_string_init_default();
	hStringKey = my_string_init_default();
	testString=my_string_init_default();
	tempString=my_string_init_default();
	hGame = my_vector_init_default(testString->destroy2, testString->item_assign);
	MY_VECTOR stringLength[30];
	
	for(i = 0; i<30; i++)
	{
		stringLength[i] = my_vector_init_default(hString->destroy2, hString->item_assign);
	}

	fp = fopen("dictionary.txt", "r");
	if(fp == NULL)
	{
		printf("Failed to open file\n");
		exit(1);
	}
	while(hString->extraction(hString, fp))
	{
		i = hString->get_size(hString);
		stringLength[i]->push_back(stringLength[i], hString);
	}
	hString->destroy(&hString);

	do
	{
		victory = 0;
		printf("Ready to be stumped?!\n");
		printf("Do you want to play on hard mode?  Press Y for yes.\n");
		scanf(" %c", &hardmode);
		clear_keyboard_buffer();

		printf("Please enter a word length to play: ");
		scanf("%d", &wordsize);
		clear_keyboard_buffer();

		while (wordsize >= 30 || wordsize<0 || stringLength[wordsize]->get_size(stringLength[wordsize]) == 0)
		{
			printf("No words found.  Please select another word length:\n");
			scanf("%d", &wordsize);
			clear_keyboard_buffer();
		}
		printf("How many guesses would you like?\n");
		scanf("%d", &guessAmount);
		clear_keyboard_buffer();
		while (guessAmount <= 0)
		{
			printf("You cannot choose negative guesses.  Enter a different number: \n");
			scanf("%d", &guessAmount);
			clear_keyboard_buffer();
		}
		printf("Your wordsize was: %d.\n", wordsize);
		printf("There are %d possibilities for that word length and you\nhave %d guesses.... good luck.\n", stringLength[wordsize]->get_size(stringLength[wordsize]), guessAmount);
		
		clear_vector(hGame);

		for (i = 0; i<stringLength[wordsize]->get_size(stringLength[wordsize]); i++)
		{
			hGame->push_back(hGame, (MY_STRING)stringLength[wordsize]->at(stringLength[wordsize], i));
		}
		
		for (i = 0; i<wordsize; i++)
		{
			stringDisplay->push_back(stringDisplay, '-');
		}
		for (i = 0; i < guessAmount; i++)
		{

			printf("Enter your guess: ");
			scanf("%c", &letters[i]);
			clear_keyboard_buffer();

			letters[i] = toupper(letters[i]);

			if (i != 0)
			{
				for (j = 0; j < i; j++)
				{
					if (letters[i] == letters[j])
					{
						printf("You have already used that letter.  Please choose again: ");
						scanf("%c", &letters[i]);
						letters[i] = toupper(letters[i]);
						clear_keyboard_buffer();
						j=0;
					}
				}
			}
			if (i == 0)
			{
				printf("%c\n", letters[i]);
			}
			else
			{
				for (j = 0; j <= i; j++)
				{
					printf("%c ", letters[j]);
				}
				printf("\n");
			}

			for (j = 0; j < hGame->get_size(hGame); j++)
			{
				for (k = 0; k < ((MY_STRING)hGame->at(hGame, j))->get_size(hGame->at(hGame, j)); k++)
				{
					testString->push_back(testString, *((MY_STRING)hGame->at(hGame, j))->at(hGame->at(hGame, j), k));
				}


				//testString = (MY_STRING)hGame->at(hGame, j);

				for (k = 0; k < wordsize; k++)
				{
					if (*testString->at(testString, k) == tolower(letters[i]))
					{
						hStringKey->push_back(hStringKey, tolower(letters[i]));
					}
					else
					{
						hStringKey->push_back(hStringKey, '-');
					}
				}

				head = keySearch(head, hStringKey, (MY_STRING)hGame->at(hGame, j));

				for (k = 0; k < wordsize; k++)
				{
					hStringKey->pop_back(hStringKey);
				}
				clear_string(testString);
			}
			if (tolower(hardmode) != 'y')
			{
				traverseList(head);
			}


			if (head->next != NULL)
			{
				temp = popMaxNode(&head);
				deleteList(&head);
				head = temp;
			}
			//printf("Words before deletion: %d\n", hGame->get_size(hGame));

			clear_vector(hGame);

			//printf("Words after deletion: %d\n", hGame->get_size(hGame));
			for (k = 0; k < head->words->get_size(head->words); k++)
			{
				hGame->push_back(hGame, (MY_STRING)(head->words->at(head->words, k)));
			}
			//printf("Words in new head: %d\n", hGame->get_size(hGame));
			
			is_key_blank = 1;

			for (k = 0; k < head->keyString->get_size(head->keyString); k++)
			{
				if (*head->keyString->at(head->keyString, k) != '-')
				{
					is_key_blank = 0;
					break;
				}
			}
			if (is_key_blank != 1)
			{
				guessAmount++;
				printf("Correct!\n");
				for (k = 0; k < stringDisplay->get_size(stringDisplay); k++)
				{
					tempString->push_back(tempString, *stringDisplay->at(stringDisplay, k));
				}
				clear_string(stringDisplay);
				for (k = 0; k < tempString->get_size(tempString); k++)
				{
					if (*tempString->at(tempString, k) != '-')
					{
						stringDisplay->push_back(stringDisplay, *tempString->at(tempString, k));
					}
					else
					{
						stringDisplay->push_back(stringDisplay, *head->keyString->at(head->keyString, k));
					}
				}
				clear_string(tempString);
				for (k = 0; k < stringDisplay->get_size(stringDisplay); k++)
				{
					if(*stringDisplay->at(stringDisplay, k) == '-')
					{
						victory = 0;
						break;
					}
					else
					{
						victory = 1;
					}
				}
				if (victory == 1)
				{
					printf("CONGRATULATIONS!  You beat Evil Hangman!  Now try it again without cheating.\n");
					deleteList(&head);
					break;
				}
				else
				{
					printf("You have %d guesses left!\n\n", guessAmount - i - 1);
				}
			}
			else
			{
				printf("BZZZZZT!  You have %d guesses left!\n\n", guessAmount - i-1);
			}
			deleteList(&head);
			printf("%s\n", stringDisplay->c_str(stringDisplay));

		}
		if (victory == 0)
		{
			printf("Sorry for your loss.  Your word was:  %s.\n", ((MY_STRING)hGame->at(hGame, 0))->c_str(hGame->at(hGame, rand() % hGame->get_size(hGame))));
		}
		clear_string(stringDisplay);
		fclose(fp);
		printf("Do you want to play again?  Press Y for yes.\n");
		scanf(" %c", &cont);
		clear_keyboard_buffer();
	}
	while(tolower(cont) == 'y');


	
	//hGame->destroy(&hGame);
	for (i = 0; i < 30; i++)  //destroys word vectors, no longer needed.  
	{
		stringLength[i]->destroy(&stringLength[i]);
	}
	
	//hString->destroy(&hString);
	stringDisplay->destroy(&stringDisplay);
	hStringKey->destroy(&hStringKey);
	testString->destroy(&testString);
	tempString->destroy(&tempString);
	return 0;
}

void clear_keyboard_buffer(void)
{
char c;

scanf("%c", &c);
while(c!= '\n')
{
	scanf("%c", &c);
}
return;
}

Node_ptr insertNode(Node_ptr head, MY_STRING key, MY_STRING word)
{
	int i;
	Node_ptr temp = (Node_ptr)malloc(sizeof(Node));
	if (temp == NULL)
	{
		printf("Failed to create new node.\n");
		exit(1);
	}
	else
	{
		temp->keyString = my_string_init_default();
		temp->words = my_vector_init_default(temp->keyString->destroy2, temp->keyString->item_assign);

		for (i = 0; i<key->get_size(key); i++)
		{
			temp->keyString->push_back(temp->keyString, (*key->at(key, i)));
		}
		temp->words->push_back(temp->words, word);

		temp->next = head;
		head = temp;
		return head;
	}
}

void deleteList(Node_ptr* head)
{

	Node_ptr temp = (Node_ptr)*head;
	Node_ptr temp2 = temp;

	while (temp != NULL)
	{
		temp2 = temp->next;
		temp->keyString->destroy(&temp->keyString);
		temp->words->destroy(&temp->words);
		free(temp);
		temp = temp2;
	}
	*head = NULL;
}

Node_ptr keySearch(Node_ptr head, MY_STRING keyFind, MY_STRING word)
{
	Node_ptr temp;
	temp = head;

	if (temp == NULL)
	{
		temp = insertNode(temp, keyFind, word);
		return temp;
	}
	else
	{
		while (temp != NULL)
		{
			if (temp->keyString->string_compare(temp->keyString, keyFind) == 0)
			{
				//right key
				temp->words->push_back(temp->words, word);
				return head;
			}
			else
			{
				temp = temp->next;
			}
		}
		temp = head;
		temp = insertNode(temp, keyFind, word);
		return temp;
	}
}

void traverseList(Node_ptr head)
{
	Node_ptr temp = head;

	while (temp != NULL)
	{
		printf("%s: %d words found.\n", temp->keyString->c_str(temp->keyString), temp->words->get_size(temp->words));
		temp = temp->next;
	}
}

Node_ptr popMaxNode(Node_ptr* head)
{
	Node_ptr temp = (Node_ptr)* head;
	Node_ptr temp2 = temp;
	int max;

	if (temp != NULL)
	{
		if (temp->next == NULL)
		{
			return temp;
		}
		else
		{
			max = temp->words->get_size(temp->words);
			temp = temp->next;

			while (temp != NULL)
			{
				if (temp->words->get_size(temp->words) > max)
				{
					max = temp->words->get_size(temp->words);
					temp2 = temp;
					temp = temp->next;
				}
				else
				{
					temp = temp->next;
				}
			}
			
			temp = (Node_ptr)* head;
			if (temp == temp2)
			{
				*head = temp->next;
				temp2->next = NULL;
				return temp2;
			}
			else
			{
				while (temp->next != temp2)
				{
					temp = temp->next;
				}
				temp->next = temp->next->next;
				temp2->next = NULL;
				return temp2;
			}
		}

}

}
void clear_vector(MY_VECTOR hVector)
{
	if (hVector->get_size(hVector) > 0)
	{
		while (hVector->get_size(hVector) > 0)
		{
			hVector->pop_back(hVector);
		}
	}

}
void clear_string(MY_STRING hString)
{
	if (hString->get_size(hString) > 0)
	{
		while (hString->get_size(hString) > 0)
		{
			hString->pop_back(hString);
		}
	}

}
