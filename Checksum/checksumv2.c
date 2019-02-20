// Dylan Roberts
// 7/5/18
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct fileContents
{
	char *fContents;
	int characterCnt;	
}fileContents;

void getCharArray(FILE *inFile, int *index, fileContents *inputFileContents);
fileContents* readAndProcessF(char *kFileName, fileContents *inputFileContents);
int validateSize(char **argv); // Parameter is checksum size.
void printout(char *fContents, int checkSumSize, long unsigned int checkSum, int characterCnt);
char *getBits(int n, int bitSize);
int getCheckSum(char *str, int checkSumSize);
int getIthBit(int n, int i);
void setIthBit(long unsigned int *n, int i);
char getHexFromInt(int n);
long unsigned int add(unsigned int a, unsigned int b, int size); //  take in a and  b
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
	
	if (myFileContents == NULL)
		return 1; // Input error.
 	
	if (myFileContents->fContents == NULL)
		return 1;	
	checkSumSize = validateSize(argv); // if invalid bit#, return 0. validateSize will return 1 if invalid. 
	if (checkSumSize == 1)
		return 1;	
	printStr(myFileContents->fContents);
	checkSum = getCheckSum(myFileContents->fContents, checkSumSize);
	printout(myFileContents->fContents, checkSumSize, checkSum, myFileContents->characterCnt);
	free(myFileContents->fContents);
	free(myFileContents);
	return 0;
}

int getIthBit(int n, int i)
{
	unsigned int bitMask = 1;
	int verdict = 0;
	//verdict = (bitMask << i) & n;
	if ((bitMask << i) & n)
		return 1;
	else
		return 0;
}
void setIthBit(long unsigned int *n, int i)
{
	if (i < 0)
		return;
	unsigned bitMask = 1;
	bitMask = bitMask << i;

	*n = *n | bitMask;
}
void getCharArray(FILE *inFile, int *index, fileContents *inputFileContents) 
{
	inputFileContents->fContents = malloc(sizeof(char) * (512 + 1)); // + 1 for null terminator since PDF states buffer needs to hold 512 LETTERS.
	char currChar = '\0';
	inputFileContents->fContents[0] = '\0';
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
		inputFileContents = malloc(sizeof(inputFileContents));
	if (kFileName == NULL) // Return null if invalid name.
		return NULL;
	FILE *myFile = NULL;
	inputFileContents->fContents = NULL;
	int index = 0;
	
	if (kFileName == NULL) // Return null if invalid name.
		return NULL;
	
	myFile = fopen(kFileName, "r");
	if (!(myFile = fopen(kFileName, "r")))
		return NULL;
	getCharArray(myFile, &index, inputFileContents);
	inputFileContents->fContents[index] = '\0'; // append null terminator.
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
	int count = strlen(str), incrementer, rem = count % (checkSumSize / 8), i;
	long unsigned int checkSum = 0;
	
	if (checkSumSize == 16)
		incrementer = 2;
	else if(checkSumSize == 32)
		incrementer = 4;

	long unsigned int temp;
	if (checkSumSize == 8)
	{
		for (i = 0; i < count; i++)
		{
			temp = str[i];
			checkSum = add(temp, checkSum, checkSumSize);
		}
	}
	else if (checkSumSize == 16)
	{
		for (i = 0; i < count - rem; i += incrementer)
		{
			temp = appendBits(str[i], str[i+1], 0, 0, checkSumSize);
			checkSum = add(temp, checkSum, checkSumSize);

		}
		if (rem == 1)
		{
			temp = appendBits('\n','X', 0, 0, checkSumSize); printf("X");
			checkSum = add(temp, checkSum, checkSumSize);
		}
	}
	else if(checkSumSize == 32)
	{
		for (i = 0; i < count - rem; i += incrementer)
		{
			temp = appendBits(str[i], str[i+1], str[i+2], str[i+3], checkSumSize);
			checkSum = add(temp, checkSum, checkSumSize);
		}
		if (rem == 1)
			{temp = appendBits('\n','X', 'X', 'X', checkSumSize); printf("XXX");}
		else if(rem == 2)
			{temp = appendBits(str[i], '\n', 'X', 'X', checkSumSize); printf("XX");}
		else if(rem == 3)
			{temp = appendBits(str[i], str[i+1], '\n', 'X', checkSumSize); printf("X");}
		checkSum = add(temp, checkSum, checkSumSize);
	}
	return checkSum; // convertToHex(checkSum, checkSumSize);	
}

char getHexFromInt(int n)
{
	char returnChar;
	if (n < 10)
		return n + '0';
	else
	{
		switch (n)
		{
			case 10:
				returnChar = 'a';
				break;
			case 11:
				returnChar = 'b';
				break;
			case 12:
				returnChar = 'c';
				break;
			case 13:
				returnChar = 'd';
				break;
			case 14:
				returnChar = 'e';
				break;
			case 15:
				returnChar = 'f';
				break;
		}
	}
	return returnChar;
}

char *getBits(int n, int bitSize)
{
	unsigned int bitMaskA = 1;
	bitMaskA = bitMaskA << (bitSize - 1);
	char *strForBits = malloc(sizeof(char) * (bitSize + 1)); // + 1 for null terminator.
	
	for (int i = 0; i < bitSize; i++)
	{
		strForBits[i] = (bitMaskA & n) ? '1' : '0';
		bitMaskA = bitMaskA >> 1;
	}
	strForBits[bitSize] = '\0';
	return strForBits;
}

void printout(char *fContents, int checkSumSize, long unsigned int checkSum, int characterCnt)
{
	int rem = characterCnt % (checkSumSize / 8);
	if (checkSumSize == 16)
		characterCnt += (rem == 1);
	else if((checkSumSize == 32) && (rem != 0))
	{
		characterCnt += (4 - rem);
	}
	printf("\n");
	printf("%2d bit checksum is %8lx for all %4d chars\n", checkSumSize, checkSum, characterCnt);		
}

long unsigned int add(unsigned int a, unsigned int b, int size) //  take in a and  b
{
	int rem = 0;
	int tempResult = 0;
	long unsigned int c = 0;
	for (int i = 0; i < size; i++)
	{
		tempResult = 0;
		tempResult += getIthBit(a, i);
		tempResult += getIthBit(b, i);
		tempResult += rem;
		switch (tempResult)
		{
			case 3:
				setIthBit(&c, i);
				rem = 1;
				break;
			case 2:
				// don't set ith bit.
				rem = 1;
				break;
			case 1:
				setIthBit(&c, i);
				rem = 0;
				break;
			case 0:
				// don't set ith bit
				rem = 0;
				break;
		}	
	}
	return c;
}

long unsigned int appendBits(unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4, int checkSumSize)
{
	int k, l, i, j, n, a;
	long unsigned int appendNbr; 
	
	switch (checkSumSize) // For initializing variables. 
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
		if (getIthBit(val1, n))
			setIthBit(&appendNbr, n + 24 - a);
	}		
	for (n = 7; n >= 0; n--)
	{
		if (getIthBit(val2, n))
			setIthBit(&appendNbr, n + 16 - a);
	}	
	if (checkSumSize == 32)
	{
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(val3, n))
				setIthBit(&appendNbr, n + 8);
		}
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(val4, n))
				setIthBit(&appendNbr, n);
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
		if (j == 80)
		{
			j = 0;
			printf("\n");
		}
		printf("%c", myString[i]);
		j++;
		i++;
	}
}