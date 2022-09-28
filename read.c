
#include "headers.h"

/*this func returns the file with the macros */
void prepMacro(FILE *file1,char name[MAX_SIZE_WORD]){
    char line[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy2[MAX_SIZE_LINE] = {'\0'};
    char line_copy3[MAX_SIZE_LINE] = {'\0'}; 
    FILE *fptr;
    macro tmp_macro=NULL;
    char *nowW =NULL;
    char *nowD =NULL;
    char *tmp = NULL;
    char labels[155][20]={"\0"};
    int check = 0;
    int counter=0;
    int i = 0;
    int j=0;
    int k=0;
    int x=0;
    int p=0;
    int two_commas=0;
    fptr = my_open(name,"w+");
    to_malloc(nowW,MAX_SIZE_LINE);/* current word pointer */
    to_malloc(nowD,(MAX_SIZE_LINE*MAX_SIZE_MACRO));/*current data pointer*/
    to_malloc(tmp,MAX_SIZE_LINE);
    while (!feof(file1) && fgets(line , MAX_SIZE_LINE, file1)){
        for(j=0;j<strlen(line);j++){
                line_copy2[k]='\0';
                k++;
        }
        k=0;
        i++;
        strcpy(line_copy,line);
        strcpy(line_copy3,line);

        for(j=0;j<strlen(line);j++){
            if(!isspace(line[j])){
                line_copy2[k]=line[j];
                k++;
            }
        }
        k=0;
        p=check_hash(line_copy3,line_copy2);/*check if there is a # before every num*/
        if(strlen(line_copy2)!=0){
            nowW = strtok(line_copy, " \t\n "); /* get first word */
            two_commas=check_comma(line);
            if(two_commas){
                first_error(check,1);
                check = 1;
            }
        }
        if(strlen(line_copy2)==0){
            
        }
        
        else if(p){
            first_error(check,9);
            check=1;
        }
        else if(nowW[0]==';'){
            continue;
        }
        else if(isLable(nowW))
        {
            if(strlen(nowW)>30){
                first_error(check,14);
                check=1;
            }
            else if( !strcmp(nowW,"mov:") || !strcmp(nowW,"add:") || !strcmp(nowW,"cmp:") || !strcmp(nowW,"sub:") || 
                !strcmp(nowW,"lea:") || !strcmp(nowW,"not:") || !strcmp(nowW,"clr:") || !strcmp(nowW,"inc:") ||
                !strcmp(nowW,"dec:") || !strcmp(nowW,"jmp:") || !strcmp(nowW,"bne:") || !strcmp(nowW,"get:") || 
                !strcmp(nowW,"prn:") || !strcmp(nowW,"jsr:") || !strcmp(nowW,"hlt:") || !strcmp(nowW,"rts:")){
                    first_error(check,7);
                    check=1;
                }  
            else{
                if(checks4LM(nowW,labels)==0){
                    strcpy(labels[counter],nowW);
                    counter++;
                    nowW=strtok(NULL," \t\n ");
                    x=check_commaLABLE(fptr,line,nowW);
                    if((x!= 0)){
                    first_error(check,x);
                    check = 1;
                    }
                }
                else{
                   first_error(check,15);
                   check=1; 
                }
            }
        }
        else if( isOrder(nowW)){
            x=check_commaLABLE(fptr,line,nowW);
            if((x!= 0)){
            first_error(check,x);
            check = 1;
            }
        }
        else if(strcmp(nowW,"macro")==0){
            nowW = strtok(NULL ,"\t\n ");
            tmp_macro=find_endM(file1,tmp_macro,nowW);
        }
       else if(checks4M(tmp_macro,nowW)==1){
           nowD=data_impM(tmp_macro,nowW);
           fprintf(fptr, "%s" , nowD);
        }
        else if((!strcmp(nowW,".entry")) || (!strcmp(nowW,".extern")))
        {
            fprintf(fptr, "%s" , line);
        }
        else{
            /*error*/
            first_error(check,6);
            check=1;
            continue;
        }
    }
    fclose(file1);
    fclose(fptr);
}

/*----------------------------------------------------------------------------------------------------*/

/*This function is used in order to return the correct error according to error encountered*/
void first_error(int check , int error_num){
    if(check != 1){
        if(error_num == 1){
            printf("ERROR! - two commas in a row were given.\n");
        }
        else if (error_num == 2){
            printf("ERROR! - first number recived in data data isn't legal\n");
        }
        else if (error_num == 3){
            printf("ERROR! - misdefinition of negetive number\n");
        }
        else if (error_num == 4){
            printf("ERROR! - comma is missing/misdefined \n");
        }
        else if (error_num == 5){
            printf("ERROR! - '-' at the end without a number\n");
        }
        else if (error_num == 6){
            printf("ERROR! - unexisting definition received\n");
        }
        else if (error_num == 7){
            printf("ERROR! - illegal lable name received\n");
        }
        else if (error_num == 8){
            printf("ERROR! - line is over the limit (80 chars)\n");
        }
        else if (error_num == 9){
            printf("ERROR! - '#' is missing\n");
        }
        else if (error_num == 10){
            printf("ERROR! - illegal register definition\n");
        }
        else if (error_num == 11){
            printf("ERROR! - illegal string declaration\n");
        }
        else if (error_num == 12){
            printf("ERROR! - illegal data declaration\n");
        }
        else if (error_num == 13){
            printf("ERROR! - illegal struct declaration\n");
        }
        else if (error_num == 14){
            printf("ERROR! - lable defined is out of legal size\n");
        }
        else if (error_num == 15){
            printf("ERROR! - lable was allready defind\n");
        }
    }
}
int checks4LM(char* nameD,char labels[155][20]){/*checks if the word is a label */
    int j = 0;
    while(strlen(labels[j])!=0){
        if(!strcmp(labels[j],nameD)){
            return 1;
        }
        j++;
    }
    return 0; 
}


