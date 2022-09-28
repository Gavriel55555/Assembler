#include "headers.h"

macro *macro_head = NULL; /* pointer to macro head */
char arr_lable[MAX_SIZE_LINE][20];
int curr=0;

/*----------------------------------------------------------------------------------------------------*/

/*This fuction is used in order to determinate if received string represents a lable*/
int isLable(char *word){
    int length = strlen(word);
    int i = 0;
    if(!isalpha(word[i]))/*first char has to be a letter*/
    return 0;
    i++;
    for(i = 1 ; i < (length-1) ; i++){
        if(!isalnum(word[i]))
        return 0;
    }
    if(word[length-1]==':'){
        return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------------------------------------*/

/*This fuction is used in order to determinate if a receievd string is a Order-word*/
int isOrder(char *word){
    char *words[16] = {"mov", "cmp" , "add" , "sub" , "not" , "clr" , "lea" ,"inc" , "dec" ,"jmp" , "bne" , "get" ,"prn" , "jsr" , "rts" , "hlt"};
    int i = 0;
    for (i = 0 ; i <16 ; i++){
        if(strcmp(word,words[i])==0){
        return 1;
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------------------------------*/

/*This fuction adds a new macro to the macro list*/
void add2list(macro *hptr,char arr[80],char *data){ /*func to add chars to linked list*/
    int i=0;
    macro p1=NULL;
    macro p2=NULL;
    macro t=NULL;

    t=malloc(sizeof(macro));  /*we wake allocate space from the memory for the list*/
    if(!t){ /*if there is not enough space*/
        
        exit(0);
    }
    t->nameM =(char*)malloc(sizeof (char )*MAX_SIZE_LINE);
    for(i=0;i<80;i++){ /*putting the data in */
      (*t).nameM[i]=arr[i];
    }
    t->dataM = (char *)malloc(sizeof (char )*MAX_SIZE_LINE * MAX_SIZE_MACRO);
    for(i=0;i<80;i++){ /*putting the data in */
      (*t).dataM[i]=data[i];
    }
    p1=*hptr;
    while(p1){ /*go to the end and adding*/
        p2=p1;
        p1=(*p1).next;
        
    }
    if(p1==*hptr){ /*if it is the first time we adding*/
       *hptr=t;
       (*t).next=p1; 
       
    }
    else{
        p2->next=t;
        t->next=p1;
        
    }
}

/*----------------------------------------------------------------------------------------------------*/

/*This function checks if a received word represent an existing macro inside our macro list*/
int checks4M(macro m,char* nameD){
    while(m){
        if(!strcmp(m->nameM,nameD)){
            return 1;
        }
         m=m->next;
    }
    return 0; 
    
}

/*----------------------------------------------------------------------------------------------------*/

/*This function returns the macro's data saved in the list*/
char* data_impM(macro m,char* nameD){
    while(strcmp(m->nameM,nameD)){
        m=m->next;
    }
    if(strcmp(m->nameM,nameD)==0){
       return m->dataM;
    }
 return "";
}

/*----------------------------------------------------------------------------------------------------*/

/*This function is used in order to run the currently read macro until the 'endmacro' command and returning it*/
macro find_endM(FILE *file1,macro tmp_macro,char* nowW){
    char * tmp=NULL;
    int j=0;
    int k=0;
    char line[MAX_SIZE_LINE] = {'\0'};
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy2[MAX_SIZE_LINE] = {'\0'}; 
    char nowD[100]={'\0'};
    beforeWhile:
    fgets(line , MAX_SIZE_LINE, file1);/*receives next line*/
    strcpy(line_copy,line);
    tmp= strtok(line_copy, " \t ");   
      for(j=0;j<strlen(line);j++){
            if(!isspace(line[j])){
                line_copy2[k]=line[j];
                k++;
            }
        }
        k=0;
         if(strlen(line_copy2)==0||line_copy2[0]==';'){
            goto beforeWhile;/*if empty/command line was enterd at the first line in macro*/
        }
    while(strcmp(tmp,"endmacro\n")&&strcmp(tmp,"endmacro")){/*as long as haven't encountered the 'endmacro' order*/
        for(j=0;j<strlen(line);j++){
                line_copy2[k]='\0';
                k++;
        }
        k=0;
        strcat(nowD,line);/*add it to existing string*/
        inWhile:
        
        fgets(line , MAX_SIZE_LINE, file1);
         for(j=0;j<strlen(line);j++){
            if(!isspace(line[j])){
                line_copy2[k]=line[j];
                k++;
            }
        }
        k=0;
        if(strlen(line_copy2)==0||line_copy2[0]==';'){
            goto inWhile;/*if empty/command line was enterd after the first line in macro*/
        }
        else{
        strcpy(line_copy,line);
        tmp= strtok(line_copy, " \t\n ");
        }
    }
    add2list(&tmp_macro,nowW,nowD);/*adding macro to the macro list*/
    return tmp_macro;
}

/*----------------------------------------------------------------------------------------------------*/

/*This function is used in order to allocate memory accordingly*/
void to_malloc(char *toM, int sizeM) {
    toM =(char *)malloc(sizeof (char )*sizeM);
    if (!toM) {
        printf("Fatal: failed to allocate memory.\n");
        exit(0);
    }
}
