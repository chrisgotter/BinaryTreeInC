//so that visual studios will stop telling me that 
//there is something wrong with string functions 

#define BUFFER 300
//standard windows specific system commands
#ifdef _WIN32
  #define PAUSE system("pause");
  #define CLEAR system("cls");
  #define FLUSH fflush(stdin);
  #define _CRT_SECURE_NO_WARNINGS
//more generic substitutes
#else
  #define FLUSH while( getchar() != '\n');
  #define PAUSE fseek(stdin,0, SEEK_END);\
			          printf("Press 'Enter' to continue: ... ");\
			          FLUSH
  #define CLEAR system("clear");
#endif
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//buffer variables which have made global to reduce the need to generate such arrays
char valueBufferA[BUFFER];
char valueBufferB[BUFFER];
char valueBufferC[BUFFER];
