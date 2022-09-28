#include "headers.h"


FILE *my_open (char *name, char *mode){ /*func to open file*/
   FILE *fp;
   fp = fopen (name, mode);
   if (fp == NULL){
     fprintf(stderr,"can't open file\n"); /*if something worng with the file*/
     exit (1);
   }
   return (fp);
}

int main(int argc, char *argv[]) {
    int i = 1;
    int j = 0;   
    FILE *file1;
    FILE *fptr1;
    FILE *fptr2;
    int del;
    char data[MAX_SIZE_WORD]={'\0'};
    char data_copy[MAX_SIZE_WORD]={'\0'};
    char data_copy2[MAX_SIZE_WORD]={'\0'};
    char data_copy3[MAX_SIZE_WORD]={'\0'};
    char data_copy4[MAX_SIZE_WORD]={'\0'};
    for(i=1;i<argc;i++){
        strcpy(data,argv[i]);
        strcpy(data_copy,data);
        strcpy(data_copy2,data);
        strcpy(data_copy3,data);
        strcpy(data_copy4,data);
        while(data[j]!= '\0'){
          j++;
        }
        data[j] = '.';                   
        data[j+1] = 'a';
        data[j+2] = 's';

        file1 = my_open(data,"r");
        strcat(data_copy,".am");
        prepMacro(file1,data_copy);
        fptr1= my_open(data_copy,"r");/*we closed it in prepBinary*/
        prepBinary(fptr1);
        strcat(data_copy2,".ob");
        fptr2=my_open("binary.ob","r");
        base_32(fptr2,data_copy2);
        fptr1= my_open(data_copy,"r");
        to_ent(fptr1,data_copy3);
        rewind(fptr1);
        to_ext(fptr1,data_copy4);
        j=0;
         while(data[j]!= '\0'){
          data[j]='\0';
          j++;
        }
        j=0;
    }
      del=remove("binary.ob");
      if(del != 0) {
      printf("Error: unable to delete the file");
      }
    return 0;
}

