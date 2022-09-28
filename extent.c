#include "headers.h"

/*this func creats to entry file*/
void to_ent(FILE* file1,char name[MAX_SIZE_WORD]){
    char *nowW = NULL;
    char line[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    FILE *fptr;
    int i=0;
    int x=0;
    int num=0;
    char arr_8[10]={'\0'};
    char * num_binary="\0";
    char base1[2]={'\0'};
    strcat(name,".ent");
    fptr  = my_open(name,"w+");
    while (!feof(file1) && fgets(line , MAX_SIZE_LINE, file1)){
        strcpy(line_copy,line);
        nowW = strtok(line_copy, " \t "); /* get first word */
        if(!strcmp(nowW,".entry")){
            
            nowW = strtok(NULL, " \t\n ");
            
            fprintf(fptr, "%s   " , nowW);
            i=checks4L2(nowW);
           
            num_binary=check4add(i);
            
            num=decimal_10(num_binary,x);
            
            nowW=Binary_10(num,arr_8);
           
            turn_32(nowW,base1);
            fprintf(fptr,"%c",base1[0]);
            fprintf(fptr,"%c\n",base1[1]);
        }
    }
    fclose(fptr);
}

/*----------------------------------------------------------------------------------------------------*/

/*this file creates the extern file*/
void to_ext(FILE* file1,char name[MAX_SIZE_WORD]){
    char *nowW = NULL;
    char line[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    FILE *fptr;
    ext list_ext=NULL;
    /*to_malloc(nowW,MAX_SIZE_LINE);*/
     while (!feof(file1) && fgets(line , MAX_SIZE_LINE, file1)){
     strcpy(line_copy,line);
        nowW = strtok(line_copy, " \t "); /* get first word */
        if(!strcmp(nowW,".extern")){
            nowW = strtok(NULL, " \t\n ");
            add2ext(&list_ext,nowW);
     }
    
    
    }
    strcat(name,".ext");
    fptr  = my_open(name,"w+");
    extern_list_address(file1,fptr,list_ext);
    fclose(fptr);
    fclose(file1);
}

/*----------------------------------------------------------------------------------------------------*/

/*returning the num in base 10*/
int decimal_10(char * data,int num){
    int j= 0;
    int cur=1;
    num=0;
    for(j=7;j>=0;j--) {
        num+=((data[j]-'0')*cur);
        cur=cur*2;
    }
    return num;
}

/*----------------------------------------------------------------------------------------------------*/

/*this func finds the addresses of the lables that been called after .extern*/
void extern_list_address(FILE *file1,FILE* fptr,ext list_ext){
    char *nowW = NULL;
    unsigned int count=99;
    int counts_r=0; /*to check if 2 registers appears in one line*/
    int valid=0;
    int i=0;
    int k = 0;
    char arr_8[10]={'\0'};
    char line[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy2[MAX_SIZE_LINE] = {'\0'}; 
    char * extBinary = "\0";
    char base32ext[2] = {'\0'};
    ext copyExt = NULL;
    rewind(file1);
    while (!feof(file1) && fgets(line , MAX_SIZE_LINE, file1)){
        strcpy(line_copy,line);
        strcpy(line_copy2,line);
        nowW = strtok(line_copy, " ,\t\n "); /* get first word */
        if(!strcmp(nowW,".entry")||!strcmp(nowW,".extern")){
            continue;
        }
        else if(isLable(nowW)){/*if first word in line is lable - enter the correct binary code in list for it */
            count++;
            
            nowW = strtok(NULL," ,\t\n ");/*running till end of line in check*/
            i++;
            goto notLable;
        }
        else{
            count++;
            notLable:
            if(nowW!=NULL){
                
                nowW = strtok(NULL," ,\t\n ");/*running till end of line in check*/
            }
            while(nowW != NULL){
                
                copyExt = list_ext;
                while(copyExt){
                    if(!strcmp(copyExt->nameE,nowW)){
                        count++;
                        extBinary=Binary_10(count,arr_8);
                        turn_32(extBinary,base32ext);
                        ;
                        fprintf(fptr,"%s    ",nowW);
                        fprintf(fptr,"%c",base32ext[0]);
                        fprintf(fptr,"%c\n",base32ext[1]);
                        goto extent;
                    }
                    copyExt=copyExt->next;
                }
                for (k = 0 ; k < strlen(nowW) ; k++){
                    if(nowW[k] == '.')
                        valid = 1;
                }
                if(valid==1){
                    count+=2;
                }
                else if(checks2r(nowW)){
                counts_r++;
                }
                if(counts_r!=2  && valid!=1){
                    count++;
                }
                extent:                
                valid=0;
                nowW = strtok(NULL," ,\t\n ");/*running till end of line in check*/
                copyExt=list_ext;
            }
        } 
          
    }
}

/*----------------------------------------------------------------------------------------------------*/

/*func to add the chars to linked list*/
void add2ext(ext *hptr,char arr[80]){
    int i=0;
    ext p1,p2;
    ext t;

    t=malloc(sizeof(ext));  /*we wake allocate space from the memory for the list*/
    if(!t){ /*if there is not enough space*/
        
        exit(0);
    }
    t->nameE =(char*)malloc(sizeof (char )*MAX_SIZE_LINE);
    for(i=0;i<80;i++){ /*putting the data in */
      (*t).nameE[i]=arr[i];
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
