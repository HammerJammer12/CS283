#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *);
void reverseString(char *, int);
void wordPrint(char*, int);

//add additional prototypes here


int setup_buff(char *buff, char *userStr, int len){
    //#4:  Implement the setup buff as per the directions
	 //itterate through each character in the string using a pointer
	 //only add one space for each consecutive whitespace, using bool "whiteSpaceAdded" to track this
	 //keep track of the size in a stringLength int
	 //add .'s until buffer length full (add .'s equal to BUFFER_SZ - stringLength)
	 //return stringLength
	 int stringLen = 0;
	 bool whiteSpaceAdded = false;
	 while (*userStr != '\0'){

		if(stringLen > len){
			return -1;	
		}

	 	if (*userStr == ' ' || *userStr == '\t' || *userStr == '\n' || *userStr == '\v' || *userStr == '\r'
		|| *userStr == '\f'){
			if(whiteSpaceAdded){
				userStr++;
				continue;
			} else {
				*buff = ' ';
				buff++;
				userStr++;
				stringLen++;
				whiteSpaceAdded = true;
			}
		} else {
			*buff = *userStr;
			buff++;
			userStr++;
			stringLen++;
			whiteSpaceAdded = false;
		}
	 }

	 int periodsToAdd = len - stringLen;
	 for (int i = 0; i < periodsToAdd; i++){
		*buff = '.';
		buff++;
	 }
	 
    return stringLen;  
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff){
	int wordCount = 0;
	bool inWord = false;

	
	while(*buff != '\0'){

	 	if (*buff == ' ' || *buff == '\t' || *buff == '\n' || *buff == '\v' || *buff == '\r'
		|| *buff == '\f'){
			if(inWord){
				inWord = false;
				buff++;
			} else {
				buff++;
			}
		} else {
			if(inWord){
				buff++;
			} else {
				inWord = true;
				wordCount++;
			}
		}
	}
   return wordCount;
}

void reverseString(char *buff,int strLen){
	char *end = buff;
	char *start = buff;
	char temp;

	for (int i = 0; i < strLen; i++) end++;
	end--;

	while(start < end){
		temp = *start;
		*start = *end;
		*end = temp;
		start++;
		end--;
	}
	return;	
}

void wordPrint(char *buff, int strLen){
	char *currentChar = buff;
	char *wordStart = NULL;
	int wordCount = 0;
	printf("Word Print\n");
	printf("----------\n");
	
	for (int i = 0; i < strLen; i++){

		//start of word
	 	if (!((*currentChar == ' ' || *currentChar == '\t' || *currentChar == '\n' || *currentChar == '\v' || *currentChar == '\r'
		|| *currentChar == '\f')) && wordStart == NULL){
			wordStart = currentChar;	
		}
	
		//end of word
	 	if (((*currentChar == ' ' || *currentChar == '\t' || *currentChar == '\n' || *currentChar == '\v' || *currentChar == '\r'
		|| *currentChar == '\f') || currentChar == buff + strLen -1) && wordStart != NULL){
		
	 		if (!(*currentChar == ' ' || *currentChar == '\t' || *currentChar == '\n' || *currentChar == '\v' || *currentChar == '\r'
			|| *currentChar == '\f')){
				currentChar++;
			}
			
			wordCount++;
			int wordLength = currentChar - wordStart;
			printf("%d. ", wordCount);
			while (wordStart < currentChar){
				printf("%c", *wordStart);
            wordStart++;
			}
			printf(" (%d)\n", wordLength);
			wordStart = NULL;
		}

		currentChar++;
	}
	return;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //#1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //     the conditional will pass if argc < 2 which is the case if argv[1] does not exist, if argc >= 2, argv[1]
	 //     exists and its safe to check 
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //#2 Document the purpose of the if statement below
    //     exit code 1 means command line problem, if there are less than 3 arguments, based on the desired input
	 //     format, we know the command line input is not valid so we can exit with error code 1. this allows us to check
	 //     argv[1] and argv[2] safely going forward as we know they exist
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //#3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
	 buff = (char*) malloc(BUFFER_SZ * sizeof(char*));
	 if (buff == NULL){	
	 	exit(99);
	 }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
		 case 'r':
				reverseString(buff,user_str_len);
				printf("Reversed String: %s", buff);
		 		break;
		 case 'w':
		 		wordPrint(buff, user_str_len);
		 		break;
		 case 'x':
		 		printf("Not Implemented!!!\n");
		 		break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
	 free(buff);
    exit(0);
}

//:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          including the buffer and length is good practice as it allows the functions to be used outside of the
//          context of main() without anything breaking from having assumptions of the buff 
