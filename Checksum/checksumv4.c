// Dylan Roberts
// 7/5/18
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct fileContents
{
	char *fContents;
	int characterCnt;	
}fileContents;

void getCharArray(FILE *inFile, int *index, fileContents *inputFileContents);
fileContents* readAndProcessF(char *kFileName, fileContents *inputFileContents);
int getCheckSum(int checkSumSize, fileContents *inputFileContents);
int validateSize(int cSSize); // Parameter is checksum size.
void printout(char *fContents, int checkSumSize, int checksum, int characterCnt);
char *getBits(int n);
unsigned int calc16Bits(fileContents *inputFileContents, int checkSumSize);
int getIthBit(int n, int i);
void setIthBit(unsigned int *n, int i);
bool isOdd(int n);
char getHexFromInt(int n);
char *convertToHex(unsigned int n, int checkSumSize);
int getIntFromBits(char *str);

int main(int argc, char **argv) // 1st parameter is for the name of the key file. 2nd Parameter is for name of the file to be encrypted.
{
	int i;
	int checkSumSize; // char array for plain text.
	// int characterCnt;
	int checksum; // This will need to be initialized and allocated. 
	// char *fContents; // This will need to be initialized and allocated. 
	// Need to have BOTH file names in order for this program to execute; otherwise, it will not execute with just the key or just the plain text.
	fileContents *myFileContents = NULL;
	/*
	myFileContents = readAndProcessF(argv[1], myFileContents); // if invalid file name (no-name), return 0.
	
	if (myFileContents == NULL)
		return 1; // Input error.
 	
	if (myFileContents->fContents == NULL)
		return 1;	
	fflush(stdout);
	checkSumSize = validateSize(atoi(argv[2])); // if invalid bit#, return 0. validateSize will return 1 if invalid. 
	if (checkSumSize == 1)
		return 1;		
	checksum = getCheckSum(checkSumSize,myFileContents);
	printout(myFileContents->fContents, checkSumSize,checksum, myFileContents->characterCnt);
	free(myFileContents->fContents);
	free(myFileContents);
	return 0;
	*/
	myFileContents = calloc(1, sizeof(fileContents));
	myFileContents->fContents = malloc(sizeof(char) * 512);
	myFileContents->characterCnt = 4;
	strcpy(myFileContents->fContents, "what");
	//char myCharArray[10] = "thisccoo";
	printf("checksum is %d", getCheckSum(16, myFileContents));
}
bool isOdd(int n)
{
	bool odd;
	if (n % 2 == 0)
		return false;
	else
		return true;
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
	//getBits(verdict);
	//return verdict;
}
void setIthBit(unsigned int *n, int i)
{
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

int validateSize(int cSSize)
{
		
	if ((cSSize != 8) && (cSSize != 16) && (cSSize != 32))
	{
		fprintf(stderr,"Valid checksum sizes are 8, 16, or 32\n");
		return 1; // Return 1, if validation error. 
	}
	return cSSize;
}

unsigned int calc16Bits(fileContents *inputFileContents, int checkSumSize)
{
	unsigned int xOrResult = 0;
	int i = 0, j = 1, n, rem;
	bool odd;
	unsigned int appendNbr; 
	rem = inputFileContents->characterCnt % (checkSumSize / 8);
	printf(", rem is %d", rem);
	/*
	odd = isOdd(inputFileContents->characterCnt); 
	if (odd)
		endVal = inputFileContents->characterCnt - 1;
	else
		endVal = inputFileContents->characterCnt - 2;
	*/
	fflush(stdout);
	for (i; i < (inputFileContents->characterCnt - rem); i += 2) // += 2 for 16 bit, += 4 for 32bit
	{ 
		appendNbr = 0;
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(inputFileContents->fContents[i], n))
				setIthBit(&appendNbr, n + 8);
		}
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(inputFileContents->fContents[j], n))
				setIthBit(&appendNbr, n);
		}
		j += 2; // will need to change this to +=4 (for 32-bit), also will need to add k += 4 and l += 4 ( for 32 bit)
	xOrResult = xOrResult ^ (appendNbr);		
		
	}
	j = inputFileContents->characterCnt - 1;
	if (rem != 0)
	{
		switch (rem)
		{
			case 1: // if remainder of 1:
			{
				appendNbr = 0;
				for (n = 7; n >= 0; n--)
				{
					if (getIthBit(inputFileContents->fContents[i], n))
						setIthBit(&appendNbr, n + 8);
				}
				for (n = 7; n >= 0; n--)
				{
					if (getIthBit('X', n))
						setIthBit(&appendNbr, n);
				}
			}
			break;
		}
	}
	/*
	if (odd)
	{
		appendNbr = 0;
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(inputFileContents->fContents[i], n))
				setIthBit(&appendNbr, n + 8);
		}
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit('X', n))
				setIthBit(&appendNbr, n);
		}
	} 
	else
	{
		appendNbr = 0;
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(inputFileContents->fContents[i], n))
				setIthBit(&appendNbr, n + 8);		
		}
		for (n = 7; n >= 0; n--)
		{
			if (getIthBit(inputFileContents->fContents[j], n))
				setIthBit(&appendNbr, n);						
		}
	} */
	printf("\nPrinting bits..");
	printf("%s", getBits(xOrResult)); printf("\n");
	printf("Real checksum is [%u]\n", xOrResult);
	return xOrResult;
}

char *convertToHex(unsigned int n, int checkSumSize)
{
	int z = 0, i = 0, j = 0, temp;
	char charBuffer[(checkSumSize / 4) + 1];
	char nbrBuffer[checkSumSize];
	char *returnBuffer = malloc(sizeof(char) * ((checkSumSize / 4) + 1)); // + 1 for null terminator
	returnBuffer[0] = '\0';
	strcpy(nbrBuffer, getBits(n));
	printf("nbrBuffer is now %s.", nbrBuffer);
	
	for (i = 0; i < checkSumSize; i++)
	{
		charBuffer[j++] = nbrBuffer[i];
		if (j == 4)
		{
			charBuffer[4] = '\0';
			printf("\ncharBuffer [%s], ", charBuffer);
			temp = getIntFromBits(charBuffer); // atoi isn't working.
			printf("temp is %d\n", temp); fflush(stdout);
			j = 0;
			returnBuffer[z++] = getHexFromInt(temp);
		}
	}
	
	returnBuffer[(checkSumSize / 4)] = '\0';
	printf("\nReturnbuff is [%s]", returnBuffer);
	return returnBuffer;
}

int getCheckSum(int checkSumSize, fileContents *inputFileContents)
{
	int n = inputFileContents->fContents[0];
	unsigned int returnVal;
	/*
	unsigned int xOrResult = 0;
	// Might need to have case statements then functions for each bit case (8, 16, 32).

	xOrResult = xOrResult << (checkSumSize - 1);
	
	// if checksum size is 16, append the 1010101 and 101001 together and then run the same loop.
	// if checksum size is 32, append 101011 111011 111011 1011011 together and run the same loop
	*/
	switch(checkSumSize)
	{
		case 16:
			returnVal = calc16Bits(inputFileContents, checkSumSize);
			//convertToHex(returnVal, checkSumSize);
			convertToHex(returnVal, 16); // used to be 1544		
			return returnVal;
			break;
		default:
			printf("made it to default");
			break;
	}
	
	/*
	for (int i = 0; i < inputFileContents->characterCnt; i++)
		xOrResult = inputFileContents->fContents[i] ^ xOrResult;		
	
	printf("xOrResult: ");
	
	// print 2 hex characteres if 8-bit checksum, 
	// print 4 hex characters if 16-bit checksum
	// print 8 hex  characters if 32-bit checksum. 
	getBits(xOrResult); printf("\n");
	
	return -478; // try this first. 
	*/
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

int getIntFromBits(char *str) // atoi doesn't work good for this (doesn't display 0's), so building my own function.
{
	int i, j;
	int len = strlen(str);
	j = len;
	unsigned int bitmask = 1;
	int result = 0;
	bitmask = bitmask << len;
	for (i = 0; i < len; i++)
	{
		j--;
		if (str[i] == '1')
			result += pow(2,j);
		//printf("\n-- i is %d, j is %d, result is %d. ", i, j, result);
		bitmask = bitmask >> 1;
	}
	return result;
}

char *getBits(int n)
{
	unsigned int bitMaskA = 1;
	bitMaskA = bitMaskA << 15;
	char *strForBits = malloc(sizeof(char) * (16 + 1)); // + 1 for null terminator.
	
	for (int i = 0; i < 16; i++)
	{
		strForBits[i] = (bitMaskA & n) ? '1' : '0';
		bitMaskA = bitMaskA >> 1;
	}
	strForBits[17] = '\0';
	return strForBits;
}

void printout(char *fContents, int checkSumSize, int checksum, int characterCnt)
{
	int i = 0, j = 0;
	while (fContents[i] != '\0')
	{
		if (j == 80)
		{
			j = 0;
			printf("\n");
		}
		printf("%c", fContents[i]);
		j++;
		i++;
	}
	printf("\n");
	printf("%2d bit checksum is %8lx for all %4d chars\n", checkSumSize, checksum, characterCnt);		
	
}
