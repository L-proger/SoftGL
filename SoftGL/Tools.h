#ifndef Tools_h__
#define Tools_h__

template<typename T>
void PrintBits(const T& val)
{
	int countBytes = sizeof(T);
	int countBits = countBytes * 8;
	char* ptr = (char*)&val;

	for(int i = 0; i < countBytes; i++)
	{
		char b = *ptr;
		for(int j = 7; j >= 0; j--)
		{
			char val = (b & (1 << j)) >> j;
			printf("%i", val);
		}
		printf(" ");
		ptr++;
	}
	printf("\n");

}
#endif // Tools_h__
