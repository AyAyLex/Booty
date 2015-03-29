#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CHAR_LIM (64)
// Maximum number of segments if MM Policy is Segmentation
#define SEG_LIM (10)

typedef struct ProcessStruct
{
	int pID;
	int arvTime;
	int memTime;
	// Use first element for VSP and PAG, otherwise array for SEG
	int addrSpace[SEG_LIM];
	struct ProcessStruct *Next;
	struct ProcessStruct *Prev;
} Process;

typedef struct MemoryStruct
{
	int pID;
	int arvTime;
	int endTime;
	int startAddr;
	int endAddr;
	struct ProcessStruct *Next;
	struct ProcessStruct *Prev;
} Memory;

// Function to print all process information
void print_all(int size, int policy, int param, int count, Process *head)
{
	printf("Memory Size: %d\n", size);
	printf("Memory Management Policy: %d\n", policy);
	printf("Policy Parameter: %d\n", param);
	printf("Number of Processes: %d\n", count);
	int i;
	Process *temp = head;
	while(temp != NULL)
	{
		printf("Process ID: %d\n", temp->pID);
		printf("Arrival Time: %d\n", temp->arvTime);
		printf("Lifetime in Memory: %d\n", temp->memTime);
		printf("Address Space: ");
		for(i = 0; i<SEG_LIM; i++)
		{
			if(temp->addrSpace[i] != 0)
			{
				printf("%d ", temp->addrSpace[i]);
			}
			else
			{
				i = SEG_LIM;
				printf("\n");
			}
		}
		temp = temp->Next;
	}
}

// Function for Memory Policy VSP
vsp(int size, int param, int count, Process *head)
{
	
}

// Function for Memory Policy PAG
pag(int size, int param, int count, Process *head)
{
	
}

// Function for Memory Policy SEG
seg(int size, int param, int count, Process *head)
{
	
}

// Function to read an command line argument input file and store the data in a
// linked-list
int read_file(char argv[])
{
	FILE *input_file;
	FILE *output_file;
	char tempChar[CHAR_LIM];
	char lowerChar[CHAR_LIM];
	char tempStr[CHAR_LIM];
	int i;
	int tempInt;
	int memSize;
	int memPol;
	int polParam;
	int procCount = 0;
	int segSize = 0;
	Process *headProc = NULL;
	Process *tempProc = NULL;
	
	// Open input file
	if((input_file = fopen(argv, "r")) == NULL)
	{
		printf("Unknown file: %s\n", argv);
		exit(3);
	}
	// Create output file
	output_file = fopen("Output.txt", "w");
	
	// Read input file
	while(fscanf(input_file, "%s",tempChar) != EOF)
	{
		// Search for keywords by making everything lowercase
		for(i = 0; i<strlen(tempChar); i++)
		{
			lowerChar[i] = tolower(tempChar[i]);
		}
		// Null terminate last character
		lowerChar[i] = '\0';
		
		// Keyword "size:" for Memory Size Value
		if(strcmp(lowerChar, "size:") == 0)
		{
			fscanf(input_file, "%d", &tempInt);
			memSize = tempInt;
		}
		// Keyword "management" for Memory Management Policy
		else if(strcmp(lowerChar, "management") == 0)
		{
			fscanf(input_file, "%s %s", tempStr, tempStr);
			if(strcmp(tempStr, "VSP") == 0)
			{
				memPol = 1;
			}
			else if(strcmp(tempStr, "PAG") == 0)
			{
				memPol = 2;
			}
			else if(strcmp(tempStr, "SEG") == 0)
			{
				memPol = 3;
			}
			else
			{
				// Do nothing, not a Memory Management Policy
				printf("Not a valid Memory Management Policy.\n");
			}
		}
		// Keyword "parameter:" for Policy Parameter
		else if(strcmp(lowerChar, "parameter:") == 0)
		{
			fscanf(input_file, "%d", &tempInt);
			polParam = tempInt;
		}
		// All processes linked together
		// Keyword "id:" for Process ID
		else if(strcmp(lowerChar, "id:") == 0)
		{
			fscanf(input_file, "%d", &tempInt);
			procCount++;
			if(procCount == 1) // First Process
			{
				Process* nextProc = (Process*)malloc(sizeof(Process));
				nextProc->pID = tempInt;
				nextProc->Next = NULL;
				nextProc->Prev = NULL;
				headProc = nextProc;
				tempProc = headProc;
			}
			else // Every other process
			{
				Process* nextProc = (Process*)malloc(sizeof(Process));
				nextProc->pID = tempInt;
				nextProc->Next = NULL;
				nextProc->Prev = tempProc;
				tempProc->Next = nextProc;
				tempProc = nextProc;
			}
		}
		// Keyword "arrival" for Arrival Time
		else if(strcmp(lowerChar, "arrival") == 0)
		{
			fscanf(input_file, "%s %d", tempStr, &tempInt);
			tempProc->arvTime = tempInt;
		}
		// Keyword "lifetime" for Lifetime in Memory
		else if(strcmp(lowerChar, "lifetime") == 0)
		{
			fscanf(input_file, "%s %s %d", tempStr, tempStr, &tempInt);
			tempProc->memTime = tempInt;
		}
		// Keyword "space:" for Address Space
		else if(strcmp(lowerChar, "space:") == 0)
		{
			i = 0; // Reset i to fill address space appropriately
			if(memPol == 1) // VSP Policy
			{
				fscanf(input_file, "%d", &tempInt);
				if(tempInt > memSize)
				{
					printf("Process address size larger ");
					printf("than total memory size.\n");
					exit(3);
				}
				tempProc->addrSpace[i] = tempInt; // Only 1 element needed
			}
			else if(memPol == 2) // PAG Policy
			{
				fscanf(input_file, "%d", &tempInt);
				if(tempInt > memSize)
				{
					printf("Process address size larger ");
					printf("than total memory size.\n");
					exit(3);
				}
				tempProc->addrSpace[i] = tempInt; // Only 1 element needed
			}
			else if(memPol == 3) // SEG Policy
			{
				for(i = 0; i<SEG_LIM; i++)
				{
					if(fscanf(input_file, "%s", tempStr) != EOF)
					{
						int j;
						// Search for keywords by making everything lowercase
						for(j = 0; j<strlen(tempStr); j++)
						{
							lowerChar[j] = tolower(tempStr[j]);
						}
						// Null terminate last character
						lowerChar[j] = '\0';
						// If segment numbers are finished
						if(strcmp(lowerChar, "process") == 0)
						{
							i = SEG_LIM;
							segSize = 0;
						}
						else // Store segment values
						{
							// Convert String to Integer
							tempInt = (int)strtol(tempStr, NULL, 10);
							segSize += tempInt;
							if(segSize > memSize)
							{
								printf("Process address size larger ");
								printf("than total memory size.\n");
								exit(3);
							}
							tempProc->addrSpace[i] = tempInt; 
						}
					}
					else
					{
						// EOF
					}
				}
			}
			else
			{
				// Do nothing, not a Memory Management Policy
			}
		}
		else
		{
			// Do nothing, not a keyword to save
		}
	} // End while loop for reading file
	
	// Close input file
	fclose(input_file);
	
	// Print all process info for check
	print_all(memSize, memPol, polParam, procCount, headProc);
	
	// VSP Memory Policy
	if(memPol == 1)
	{
		vsp(memSize, polParam, procCount, headProc);
	}
	// PAG Memory Policy
	else if(memPol == 2)
	{
		pag(memSize, polParam, procCount, headProc);
	}
	// SEG Memory Policy
	else if(memPol == 3)
	{
		seg(memSize, polParam, procCount, headProc);
	}
	else
	{
		// Do nothing, not a memory policy
	}
	
	// Close output file
	fclose(output_file);
	return 0;
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		exit(2);
	}
	read_file(argv[1]);
	return 0;
}
