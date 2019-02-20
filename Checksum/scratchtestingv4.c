#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool isOdd(int n);
int getIthBit(int n, int i);
void printBits(int val);
void setIthBit(long unsigned int *n, int i);
void testForLoop(void);
char *getBits(int n, int bitSize);
char getHexFromInt(int n);
long unsigned int add(unsigned int a, unsigned int b, int size); //  take in a and  b
char *convertToHex(unsigned int n, int checkSumSize);
int getIntFromBits(char *str); // atoi doesn't work good for this (doesn't display 0's), so building my own function.
long unsigned int appendBits(unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4, int checkSumSize);
void calc(char *str, int checkSumSize);

int main(void)
{

	calc("A love for tradition has never weakened a nation, indeed it has strengthened nations in their hour of peril.    Sir Winston Churchill\n", 32);
}

void calc(char *str, int checkSumSize)
{
	int count = strlen(str), incrementer, rem = count % (checkSumSize / 8), i;
	printf("Rem is %d", rem);
	long unsigned int result = 0;
	
	if (checkSumSize == 16)
		incrementer = 2;
	else if(checkSumSize == 32)
		incrementer = 4;
	// 8-bit approach
	/*
	for (int i = 0; i < 10; i++)
	{
		result = add('A', result);
		printf("\n%u", result);
	}
	printf("\n--%d--", '\n');
	result = add((unsigned int)'\n', result);
	printf("...%u", result); 
	*/
	long unsigned int temp;
	if (checkSumSize == 8)
	{
		for (i = 0; i < count; i++)
		{
			temp = str[i];
			result = add(temp, result, checkSumSize);
		}
	}
	else if (checkSumSize == 16)
	{
		for (i = 0; i < count - rem; i += incrementer)
		{
			temp = appendBits(str[i], str[i+1], 0, 0, checkSumSize);
	//		printf("\nAppend bits yields %u", appendBits(str[i], str[i+1], 0, 0, checkSumSize));
			result = add(temp, result, checkSumSize);
	//		printf("\n%u", getIntFromBits(getBits(result, checkSumSize)));
	//		printf(".. getBits: %s", getBits(result, checkSumSize));
		}
		if (rem == 1)
		{
			temp = appendBits('\n','X', 0, 0, checkSumSize);
	//		printf("\nAppend bits yields %u", appendBits('\n','X', 0, 0, checkSumSize));
			result = add(temp, result, checkSumSize);
	//		printf("\n%u", getIntFromBits(getBits(result, checkSumSize)));
	//		printf(".. getBits: %s", getBits(result, checkSumSize));	
		}
	}
	else if(checkSumSize == 32)
	{
		for (i = 0; i < count - rem; i += incrementer)
		{
			temp = appendBits(str[i], str[i+1], str[i+2], str[i+3], checkSumSize);
	//		printf("\nAppend bits yields %u", appendBits(str[i], str[i+1], str[i+2], str[i+3], checkSumSize));
			result = add(temp, result, checkSumSize);
	//		printf("\n%u", getIntFromBits(getBits(result, checkSumSize)));
	//		printf(".. getBits: %s", getBits(result, checkSumSize));
		}
		if (rem == 1)
			temp = appendBits('\n','X', 'X', 'X', checkSumSize);
		else if(rem == 2)
			temp = appendBits(str[i], '\n', 'X', 'X', checkSumSize);
		else if(rem == 3)
			temp = appendBits(str[i], str[i+1], '\n', 'X', checkSumSize);
	//		printf("\nAppend bits yields %u", appendBits('\n','X', str[i+2], str[i+3], checkSumSize));
		result = add(temp, result, checkSumSize);
	//		printf("\n%u", getIntFromBits(getBits(result, checkSumSize)));
	//		printf(".. getBits: %s", getBits(result, checkSumSize));	
	}
	return convertToHex(result, checkSumSize);	
}

long unsigned int appendBits(unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4, int checkSumSize)
{
	int k, l, i, j, n, a;
	long unsigned int appendNbr; 
	
	switch (checkSumSize)
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

void testForLoop(void)
{
	int i, j = 0;
	for (i = j = 0; i < 15; i++, j *= 2)
	{
		;
	}
	printf("i is now %d\n", i);
	printf("j is now %d", j);
	printf("\n%d", atoi("58"));
	
}

bool isOdd(int n)
{
	bool odd;
	if (n % 2 == 0)
		return false;
	else
		return true;
}

/*
void printBits(int val)
{
	unsigned int bitMask = 1;
	bitMask = bitMask << 31;
	for (int i = 0; i < 32; i++)
	{
		printf("%d", (bitMask & val) ? 1 : 0);
		bitMask = bitMask >> 1;
	}
}
*/

void printBits(int val)
{
	unsigned int bitMask = 1;
	bitMask = bitMask << 15;
	for (int i = 0; i < 16; i++)
	{
		printf("%d", (bitMask & val) ? 1 : 0);
		bitMask = bitMask >> 1;
	}
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
	//printBits(verdict);
	//return verdict;
}

void setIthBit(long unsigned int *n, int i)
{
	if (i < 0)
		return;
	unsigned bitMask = 1;
	bitMask = bitMask << i;

	*n = *n | bitMask;
}
long unsigned int add(unsigned int a, unsigned int b, int size) //  take in a and  b
{
	
	//unsigned int a = pow(2,7);
	//unsigned int b = pow(2,7);
	int rem = 0;
	int tempResult = 0;
	long unsigned int c = 0;
	//printf("%u", 128 + 128); //+ pow(2,6)));
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
		//printf("c is %d", c);
	
	}
	return c;
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

char *convertToHex(unsigned int n, int checkSumSize)
{
	int z = 0, i = 0, j = 0, temp;
	char charBuffer[(checkSumSize / 4) + 1];
	char nbrBuffer[checkSumSize];
	char *returnBuffer = malloc(sizeof(char) * ((checkSumSize / 4) + 1)); // + 1 for null terminator
	returnBuffer[0] = '\0';
	strcpy(nbrBuffer, getBits(n, checkSumSize));
	//printf("nbrBuffer is now %s.", nbrBuffer);
	
	for (i = 0; i < checkSumSize; i++)
	{
		charBuffer[j++] = nbrBuffer[i];
		if (j == 4)
		{
			charBuffer[4] = '\0';
		//	printf("\ncharBuffer [%s], ", charBuffer);
			temp = getIntFromBits(charBuffer); // atoi isn't working.
		//	printf("temp is %d\n", temp); fflush(stdout);
			j = 0;
			returnBuffer[z++] = getHexFromInt(temp);
		}
	}
	
	returnBuffer[(checkSumSize / 4)] = '\0';
	//printf("\nReturnbuff is [%s]", returnBuffer);
	return returnBuffer;
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