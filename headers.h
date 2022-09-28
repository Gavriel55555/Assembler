#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "construct.h"

/*read*/
void prepMacro(FILE *,char[MAX_SIZE_WORD]);
void first_error(int , int );
int checks4LM(char* ,char[155][20]);

/*main*/
FILE *my_open (char *, char *);


/* utils */
int isLable(char *);
int isOrder(char *);
void add2list(macro *,char [80] ,char *);
int checks4M(macro ,char*);
char* data_impM(macro ,char*);
macro find_endM(FILE *,macro,char *);
void to_malloc(char *, int);


/*errorCheck*/
int check_comma(char*);
int check_commaLABLE(FILE * ,char* ,char*);
int check_hash(char*,char * );
int is_reg(char * ,char *);
int legal_str(char* );


/*prep*/
char* binary(unsigned int,char[MAX_SIZE_ARR]);
int words_handle(char *,char *,FILE * ,int);
int prep_instruction(FILE *,char *  ,char * ,int );
int checks2r(char *);

/*secondPass*/
void create_lable_list(FILE *);
void prepBinary(FILE *);
int return4L(char* );
int checks4L(char* );
int checks4L2(char* );
char *check4add(int );
char *the_order(char * );
int words_binary(char *,char* ,FILE * , int);
int isItReg(FILE *,char*,int );
char* tmpBinary(unsigned int);


/*toBase*/
void base_32(FILE *,char[MAX_SIZE_WORD]);
void turn_32(char *,char[2]);
char* Binary_10(unsigned int,char[10]);
int decimal_10(char * ,int );

/*extent*/
void to_ent(FILE* ,char[MAX_SIZE_WORD]);
void to_ext(FILE* ,char[MAX_SIZE_WORD]);
int decimal_10(char * ,int );
void extern_list_address(FILE *,FILE* fptr,ext list_ext );
void add2ext(ext *,char [80]);
