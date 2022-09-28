#include "headers.h"




/*----------------------------------------------------------------------------------------------------*/
/*This function is used in order to convert the received number to its binary form*/

char* binary(unsigned int num,char arr[8]){
    int j= 0;
    int cur=0;
    for(j=7;j>=0;j--) {
        arr[j] = '0';
    }
    arr[8]='\0';
    for(j=7;j>=0;j--) {
        cur=num & 0x1;
        arr[j] = cur + '0';
        num  = num >> 1;
        
    }
    return arr;
}

/*----------------------------------------------------------------------------------------------------*/
/*checks what kind of a order it is  for the binary form*/
int words_handle(char *order,char* line,FILE * fptr,int counter){
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char * nowD=NULL;
    char  *cur=NULL;
    int tmp_counter=0;
    int i=0;
    char *word=NULL;
    strcpy(line_copy,line);
    word=strtok(line_copy," \t\n");
    if(isLable(word)){
        word=strtok(NULL," \t\n ");
    }
    word=strtok(NULL," ,\t\n");/*FOR THE FIRST WORD after the order*/
    if( !strcmp(order,"mov") || !strcmp(order,"add") || !strcmp(order,"sub") || !strcmp(order,"cmp") || !strcmp(order,"lea")){
            if(!strcmp(order,"mov")){ /*3 words orders*/
            nowD="0000";
            }
             else if(!strcmp(order,"cmp")){
            nowD = "0001";
            }
            else if(!strcmp(order,"add")){
            nowD = "0010";
            }
            else if(!strcmp(order,"sub")){
            nowD = "0011";
            }
            else if(!strcmp(order,"lea")){
            nowD = "0110"; 
            }
            fprintf(fptr, "%s" , nowD);
            while(i<2){
                cur=the_order(word);
                fprintf(fptr, "%s" , cur);
                word=strtok(NULL,",\t\n ");
                i++;
        }
        i=0;
     }
    else if( !strcmp(order,"not") || !strcmp(order,"clr") || 
        !strcmp(order,"inc") || !strcmp(order,"dec") || 
        !strcmp(order,"jmp")|| !strcmp(order,"bne")|| !strcmp(order,"get")|| 
        !strcmp(order,"prn")|| !strcmp(order,"jsr")){
            if(!strcmp(order,"not")){/*2 words orders*/
                nowD = "0100"; 
            }
            else if(!strcmp(order,"clr")){
                nowD = "0101"; 
               
            }
            else if(!strcmp(order,"inc")){
                nowD = "0111"; 
            }
            else if(!strcmp(order,"dec")){
                nowD = "1000"; 
            }
            else if(!strcmp(order,"jmp")){
                nowD = "1001"; 
            }
            else if(!strcmp(order,"bne")){
                nowD = "1010"; 
            }
            else if(!strcmp(order,"get")){
                nowD = "1011"; 
            }
            else if(!strcmp(order,"prn")){
                nowD = "1100"; 
            }
            else if(!strcmp(order,"jsr")){
                nowD = "1101"; 
            }
            fprintf(fptr, "%s" , nowD);
            fprintf(fptr, "%s" , "00");
            cur=the_order(word);
            fprintf(fptr, "%s" , cur);
        }
    else if(!strcmp(order,"rts") || !strcmp(order,"hlt")){/*1 words orders*/
            if(!strcmp(order,"rts")){
                nowD = "1110";
            }
            else if(!strcmp(order,"hlt")){
                nowD = "1111"; 
        }
        fprintf(fptr, "%s" , nowD);
        fprintf(fptr, "%s" , "0000");
        
    }
    else{
        nowD="HERE";
        goto here;
    }
    
    fprintf(fptr, "%s\n" , "00");
    counter++;
    counter+=words_binary(order,line, fptr , tmp_counter);
    here:
    return counter;
}

/*----------------------------------------------------------------------------------------------------*/

/*this func convers all the instruction to there binary form*/
int prep_instruction(FILE *fptr ,char * order ,char * data,int counter){
    int i=0;
    int j=0;
    int k=0;
    int x;
    char info[10]={'\0'};
    char *arr1="\0";
    char *arr2="\0";
    if(!strcmp(order,".struct")){
        arr2=strtok(data," ,\t ");
        x=atoi(arr2);
        if(x<0){
                info[0]='1';
                info[1]='1';
            }
            else{
                info[0]='0';
                info[1]='0';
            }
        arr2=tmpBinary(x);
        for(j=2;j<10;j++){
            info[j]=arr2[j-2];
        }
        counter+=1;
        fprintf(fptr, "%s\n" , info);
        arr2=strtok(NULL," ,\t ");
        j=0;
        while(arr2[j+1]){
            if(arr2[j]!='"'){
                x=arr2[j];
                if(x>=0){
                    info[0]='0';
                    info[1]='0';
                }
                arr1=tmpBinary(x);
                for(k=2;k<10;k++){
                    info[k]=arr1[k-2];
                }
            counter+=1;
            fprintf(fptr, "%s\n" , info);
            }
            j++; 
        }
        counter+=1;
        fprintf(fptr, "%s\n" , "0000000000");        
    }
    else if(!strcmp(order,".data")){
        arr2=strtok(data," ,\t ");
        while(arr2!=NULL){
            x=atoi(arr2);
            if(x<0){
                info[0]='1';
                info[1]='1';
            }
            else{
                info[0]='0';
                info[1]='0';
            }
            arr2=tmpBinary(x);
            for(j=2;j<10;j++){
                info[j]=arr2[j-2];
            }
            counter+=1;
            fprintf(fptr, "%s\n" , info);
            arr2=strtok(NULL," ,\t ");

        }
    }
    else{
        info[0]='0';
        info[1]='0';
        while(data[i+1]){
            if(data[i]!='"'){
                x=data[i];
                arr2=tmpBinary(x);
                for(j=2;j<10;j++){
                    info[j]=arr2[j-2];
                }
                counter+=1;
                fprintf(fptr, "%s\n" , info); 
            }
            i++;
        }
        counter+=1;
        fprintf(fptr, "%s\n" , "0000000000"); 
    }
    return counter;
}

/*----------------------------------------------------------------------------------------------------*/
/*checks if a word is one of the registers*/
int checks2r(char *word){
    if(!strcmp(word,"r0") || !strcmp(word,"r1") || 
        !strcmp(word,"r2") || !strcmp(word,"r3") || 
        !strcmp(word,"r4")|| !strcmp(word,"r5")|| !strcmp(word,"r6")|| 
        !strcmp(word,"r7")){
            return 1;             
    }
    return 0;
}
