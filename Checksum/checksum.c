// Dylan Roberts
// 7/5/18
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Struct which contains both array of characters from file as well as a count  for characters.
typedef struct fileContents
{
	char *fContents;
	int characterCnt;	
}fileContents;

// Prototypes
void getCharArray(FILE *inFile, int *index, fileContents *inputFileContents);
fileContents* readAndProcessF(char *kFileName, fileContents *inputFileContents);
int validateSize(char **argv); // Parameter is checksum size.
void printout(char *fContents, int checkSumSize, long unsigned int checkSum, int characterCnt);
int getCheckSum(char *str, int checkSumSize);
int getIthBit(int n, int i);
void setIthBit(long unsigned int *n, int i);
long unsigned int add(long unsigned int a, long unsigned int b, int size); //  take in a and  b
long unsigned int appendBits(unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4, int checkSumSize);
void printStr(char *myString);

int main(int argc, char **argv) // 1st parameter is for the name of the key file. 2nd Parameter is for name of the file to be encrypted.
{
	int i;
	int checkSumSize; // char array for plain text.
	// int characterCnt;
	long unsigned int checkSum; // This will need to be initialized and allocated. 
	// char *fContents; // This will need to be initialized and allocated. 
	// Need to have BOTH file names in order for this program to execute; otherwise, it will not execute with just the key or just the plain text.
	fileContents *myFileContents = NULL;
	
	myFileContents = readAndProcessF(argv[1], myFileContents); // if invalid file name (no-name), return 0.
	
	// Ensure read and process returned non-null struct. 
	if (myFileContents == NULL)
		return 1; // Input error.
 	
	// Ensure that a valid char array exists for f-Contents.
	if (myFileContents->fContents == NULL)
		return 1;	
	checkSumSize = validateSize(argv); // if invalid bit#, return 0. validateSize will return 1 if invalid. 
	if (checkSumSize == 1)
		return 1;	
	printStr(myFileContents->fContents); // Prints string in proper format (adds newline at the beginning, and ensures only 80 chars per line).
	checkSum = getCheckSum(myFileContents->fContents, checkSumSize); // Calculate checksum. 
	printout(myFileContents->fContents, checkSumSize, checkSum, myFileContents->characterCnt); // Final printout
	free(myFileContents->fContents); // Deallocate char array
	free(myFileContents); // Deallocate struct
	return 0;
}

int getIthBit(int n, int i) // This checks the ith bit of int n to see if it is 1 or 0, and returns appropriately 
{
	unsigned int bitMask = 1;
	int verdict = 0;
	//verdict = (bitMask << i) & n;
	if ((bitMask << i) & n)
		return 1;
	else
		return 0;
}
void setIthBit(long unsigned int *n, int i) // This function sets the ith bit of int n to 1 if it's not 1 already
{
	if (i < 0)
		return;
	unsigned bitMask = 1;
	bitMask = bitMask << i; // left shift 1 by i

	*n = *n | bitMask; // "or" sets the ith bit to 1. Since this will not return anything, used *n to make sure that n changes, since reference variable. 
}
void getCharArray(FILE *inFile, int *index, fileContents *inputFileContents) 
{
	inputFileContents->fContents = malloc(sizeof(char) * (512 + 1)); // + 1 for null terminator since PDF states buffer needs to hold 512 LETTERS.
	char currChar = '\0'; // Initialize current character
	inputFileContents->fContents[0] = '\0'; // Initialize first character of f-Contents to null
	inputFileContents->characterCnt = 0;
	while ((currChar = fgetc(inFile)) != EOF)
	{
		inputFileContents->characterCnt++; // Add to character count.		
		inputFileContents->fContents[(*index)++] = currChar;
		inputFileContents->fContents[*index] = '\0'; // Append Null terminator at the end.
		if ((*index) == 512) // Don't allow the number of char's to go beyond 512.
			return;		
	}
}

fileContents* readAndProcessF(char *kFileName, fileContents *inputFileContents)
{
	if (inputFileContents == NULL)
		inputFileContents = malloc(sizeof(inputFileContents)); // allocate memory for struct
	if (kFileName == NULL) // Return null if invalid name.
		return NULL;
	FILE *myFile = NULL;
	inputFileContents->fContents = NULL;
	int index = 0;
	
	if (kFileName == NULL) // Return null if invalid name.
		return NULL;
	
	myFile = fopen(kFileName, "r"); // Open contents file for reading.
	if (!(myFile = fopen(kFileName, "r"))) // return null if unable to open
		return NULL;
	getCharArray(myFile, &index, inputFileContents); // get char array which will be added to inputFileContents->f-Contents
	return inputFileContents;
}

int validateSize(char **argv)
{	
	int cSSize;
	if (argv[2] == NULL)
	{
		fprintf(stderr,"Valid checksum sizes are 8, 16, or 32\n");
		return 1; // Return 1, if validation error. 
	}
	cSSize = atoi(argv[2]);
	
	if ((cSSize != 8) && (cSSize != 16) && (cSSize != 32))
	{
		fprintf(stderr,"Valid checksum sizes are 8, 16, or 32\n");
		return 1; // Return 1, if validation error. 
	}
	return cSSize;
}

int getCheckSum(char *str, int checkSumSize)
{
	int count = strlen(str), incrementer, rem = count % (checkSumSize / 8), i; // Initialize local variables.
	long unsigned int checkSum = 0;
	
	if (checkSumSize == 16) // incrementer is based on checkSumSize
		incrementer = 2;
	else if(checkSumSize == 32)
		incrementer = 4;

	long unsigned int temp; // Needs to be long to accomodate size of 32 bit. 
	if (checkSumSize == 8)
	{
		for (i = 0; i < count; i++)
		{
			temp = str[i]; // If 8 bit, simply get each character and add it to checksum
			checkSum = add(temp, checkSum, checkSumSize);
		}
	}
	else if (checkSumSize == 16)
	{
		for (i = 0; i < count - rem; i += incrementer)
		{
			// If 16 bit, append bits for two characters at a time, then add that result to checksum
			temp = appendBits(str[i], str[i+1], 0, 0, checkSumSize);
			checkSum = add(temp, checkSum, checkSumSize);

		}
		if (rem == 1)
		{
			// If there is a remainder, and 16 bit, then only a remainder of 1 is possible, and that would be the newline char, then only 1 padding char 'X' is needed
			temp = appendBits('\n','X', 0, 0, checkSumSize); printf("X");
			checkSum = add(temp, checkSum, checkSumSize);
		}
	}
	else if(checkSumSize == 32)
	{
		for (i = 0; i < count - rem; i += incrementer)
		{
			// If 32 bit, apppend 4 characters at a time, then add that result to checkSum
			temp = appendBits(str[i], str[i+1], str[i+2], str[i+3], checkSumSize);
			checkSum = add(temp, checkSum, checkSumSize);
		}
		if (rem == 1) // If remainder is 1, that remainder would just be the newline, then would need to add 3 'X' padding chars
			{temp = appendBits('\n','X', 'X', 'X', checkSumSize); printf("XXX");}
		else if(rem == 2) // If remainder is 2, that remainder would be the next ith char + new line, then would need to add 2 'X' padding chars
			{temp = appendBits(str[i], '\n', 'X', 'X', checkSumSize); printf("XX");}
		else if(rem == 3) // If remainder is 3, that remainder would be the next ith char plus next ith char plus newline, then add 1 padding 'X' char
			{temp = appendBits(str[i], str[i+1], '\n', 'X', checkSumSize); printf("X");}
		checkSum = add(temp, checkSum, checkSumSize);
	}
	return checkSum; // convertToHex(checkSum, checkSumSize);	
}

void printout(char *fContents, int checkSumSize, long unsigned int checkSum, int characterCnt)
{
	checkSum = (unsigned int)checkSum; // truncate bits for output by casting to unsigned int.
	int rem = characterCnt % (checkSumSize / 8); // get remainder, which will always be 0 if dealing with 8-bit. 
	if (checkSumSize == 16)
		characterCnt += (rem == 1); // If remainder is 1, and checksum size is 16, add 1 to the character count (1 X).
	else if((checkSumSize == 32) && (rem != 0))
		characterCnt += (4 - rem); // if checkSumSize is 32, add to character count the number of 'X's that are added
	printf("\n");
	printf("%2d bit checksum is %8lx for all %4d chars\n", checkSumSize, checkSum, characterCnt);		
}

long unsigned int add(long unsigned int a, long unsigned int b, int size) //  take in a and  b
{
	int rem = 0;
	long unsigned int tempResult = 0;
	long unsigned int c = 0;
	for (int i = 0; i < size; i++)
	{
		tempResult = 0;
		tempResult += getIthBit(a, i); // get ith bit of the first number, and add to temporary result (1 or 0)
		tempResult += getIthBit(b, i); // get ith bit of the 2nd number, and add to temporary result (1 or 0)
		tempResult += rem; // add the remainder (either 1 or 0).
		switch (tempResult)
		{
			case 3: // it tempResult is 3.
				setIthBit(&c, i); // set ith bit of the value that will be returned.
				rem = 1; // set remainder to 1.
				break; // go to next iteration
			case 2: // it tempResult is 2.
				// don't set ith bit of value that will be returned, leave it at 0.
				rem = 1; // set remainder to 1
				break; // go to next iteration
			case 1: // it tempResult is 1.
				setIthBit(&c, i); // Set ith bit of the value that will be returned.
				rem = 0; // set remainder to 0.
				break; // go to next iteration
			case 0: // it tempResult is 0.
				// don't set ith bit
				rem = 0; // set remainder to 0.
				break; // Go to next iteration.
		}	
	}
	return c; // return the new value.
}

long unsigned int appendBits(unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4, int checkSumSize)
{
	int k, l, i, j, n, a;
	long unsigned int appendNbr; 
	
	// The purpose of this function is to append bits. Example of 16 bit
	// if value 1 is 10110110 and value 2 is 00010110, result would be 00000000000000001011011000010110, similar to strcat
	// Example of a 32 bit
	// if value 1 is 10011011 value 2 is 10000000 value 3 is 11111111 value 4 is 01010110, return value would be 10011011100000001111111101010110
	
	switch (checkSumSize) // For initializing variables, which will differ based on size of checksum.
	{
		case 16:
			i = 0, j = 1, a = 16, k = i, l = j;
			break;
		case 32:
			k = 0, l = 1, i = 2, j = 3, a = 0;
	}	
	appendNbr = 0;
	for (n = 7; n >= 0; n--)
	{
		if (getIthBit(val1, n)) // get ith bit from value 1.
			setIthBit(&appendNbr, n + 24 - a); // if ith bit of value 1 is a 1, set the appropriate bit to 1 for the number that will be returned.
	}		
	for (n = 7; n >= 0; n--)
	{
		if (getIthBit(val2, n)) // get ith bit from value 2.
			setIthBit(&appendNbr, n + 16 - a); // if ith bit of value 2 is a 1, set the appropriate bit to 1 for the number that will be returned.
	}	
	if (checkSumSize == 32)
	{
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(val3, n)) // get ith bit from value 3.
				setIthBit(&appendNbr, n + 8); // if ith bit of value 3 is a 1, set the appropriate bit to 1 for the number that will be returned.
		}
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(val4, n)) // get ith bit from value 4.
				setIthBit(&appendNbr, n); // if ith bit of value 4 is a 1, set the appropriate bit to 1 for the number that will be returned.
		}
	}
	return appendNbr;
}

void printStr(char *myString)
{
	int i = 0, j = 0;
	printf("\n");
	while (myString[i] != '\0')
	{
		if (j == 80) // If 80 chars is reached, add newline and reset counter j
		{
			j = 0;
			printf("\n");
		}
		printf("%c", myString[i]);
		j++; // Increment j and i
		i++;
	}
}