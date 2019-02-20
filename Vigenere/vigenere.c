// Dylan Roberts
// 6/16/18

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
char *readAndStrip(FILE *inFile, int *index);
char *readPTFileIntoArray(char *pFileName);
char *readKeyFileIntoArray(char *kFileName);
char *createCTArray(char *inPt, char *inKey);
void printout(char *myString);

int main(int argc, char **argv) // 1st parameter is for the name of the key file. 2nd Parameter is for name of the file to be encrypted.
{
	int i;
	char *toBeEncrypted = NULL; // char array for plain text.
	char *key = NULL;
	char *cipherText = NULL;
	// Need to have BOTH file names in order for this program to execute; otherwise, it will not execute with just the key or just the plain text.
	printf("\n\nVigenere Key:\n\n");
	if (!(key = readKeyFileIntoArray(argv[1]))) // if invalid file name (no-name), return 0.
		return 0;	
	printout(key);
	printf("\n\nPlaintext:\n\n");  
	if (!(toBeEncrypted = readPTFileIntoArray(argv[2]))) // if invalid file name (no-name), return 0.
		return 0;	
	printout(toBeEncrypted); 	
	cipherText = createCTArray(toBeEncrypted, key);
	//printf("key is %s", key);
	printf("\n\nCiphertext:\n\n");
	printout(cipherText);
	free(toBeEncrypted);
	free(key);
	free(cipherText);
	return 0;
}

char *readAndStrip(FILE *inFile, int *index) 
{
	char *myArray = malloc(sizeof(char) * (512 + 1)); // + 1 for null terminator since PDF states buffer needs to hold 512 LETTERS.
	char currChar = '\0';
	myArray[0] = '\0';
	while ((currChar = fgetc(inFile)) != EOF)
	{
		//printf("%c", currChar); // Print Char
		// skip currChar if it's not a letter	
		if (!isalpha(currChar))
			continue; // Skip
	
		currChar = tolower(currChar);
		myArray[(*index)++] = currChar;
		myArray[*index] = '\0'; // Append Null terminator at the end.
		if ((*index) == 512) // Don't allow the number of char's to go beyond 512.
			return myArray;		
		// may need to add new line if at 80 chars, verify.
	}
	return myArray;	
}

char *readKeyFileIntoArray(char *kFileName)
{
	if (kFileName == NULL) // Return null if invalid name.
		return NULL;	
	FILE *myFile = NULL;
	char *myNewKey = NULL;
	int index = 0;

	if (kFileName == NULL) // Return null if invalid name.
		return NULL;
	myFile = fopen(kFileName, "r");
	if (!(myFile = fopen(kFileName, "r")))
		return NULL;
	myNewKey = readAndStrip(myFile, &index);
	myNewKey[index] = '\0'; // append null terminator.
	return myNewKey;
}

char *readPTFileIntoArray(char *pFileName)
{
	if (pFileName == NULL) // Return null if invalid name.
		return NULL;
	FILE *myFile = NULL;
	char *myNewPt = NULL;
	int index = 0;
	myFile = fopen(pFileName, "r");	
	if (!(myFile = fopen(pFileName, "r")))
		return NULL;
	myNewPt = readAndStrip(myFile, &index);
	while (index < 512)
	{
		myNewPt[index++] = 'x'; // padd with x until there are 512 letters. 
	}
	myNewPt[index] = '\0'; // append null terminator. 
	return myNewPt;
}

char *createCTArray(char *inPt, char *inKey)
{
	int i, x, ptVal, keyVal, insertVal;
	char *cipher = malloc(sizeof(char) * (512 + 1)); // 512 + 1 (for null terminator).
	for (i = x = 0; i < 512; i++)
	{
		if (inKey[x] == '\0') // if end of key is reached, go back to the beginning of key and continue encryption.
			x = 0;
		keyVal = inKey[x] - 'a';
		ptVal = inPt[i] - 'a';
		insertVal = (keyVal + ptVal) % 26; 
		cipher[i] = tolower('a' + insertVal);

		x++;
	}
	//printf("cipher is %s", cipher);
	return cipher;
}

void printout(char *myString)
{
	int i = 0, j = 0;
	while (myString[i] != '\0')
	{
		if (j == 80)
		{
			j = 0;
			printf("\n");
		}
		printf("%c", myString[i]);
		j++;
		i++;
	}
	printf("\n");
}