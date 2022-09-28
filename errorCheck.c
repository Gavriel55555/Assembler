#include "headers.h"

/*func that checks all the problems that can occurs with the commas*/
int check_comma(char* line){
    char line_copy[MAX_SIZE_LINE] = {'\0'};
    int i=0;
    int j=0;
    while(line[i]){
        if(!isspace(line[i])){
            line_copy[j]=line[i];
            j++;
        }
        i++;
    }
    for(i=0;i<(strlen(line_copy)-1);i++){
        if(line_copy[i]==','&& line_copy[i+1]==','){
            return 1;
        }
    
    }    
    return 0;
}

/*----------------------------------------------------------------------------------------------------*/

/*func that checks all the problems that can occurs with the commas*/
int check_commaLABLE(FILE *fptr,char* line,char* nowW){
    int i=0;
    int j=0;
    int x=0;
    int count=0;
    char* dataLine=NULL;
    char line_for_data[MAX_SIZE_LINE] = {'\0'};
    char line_copy[MAX_SIZE_LINE] = {'\0'};
    if(is_reg(line,nowW)){
        return 10;
    }
    x=legal_str(line);
    if(x!=0){
        return x;
    }
    if(!strcmp(nowW,".data")){
        strcpy(line_copy,line);
        dataLine=strtok(line_copy," \t ");
        while(dataLine!=NULL){
            strcat(line_for_data,dataLine);
            dataLine=strtok(NULL," \t ");
        }
        while(line_for_data[j]!='\0'){
            j++;
        }
        if(strstr(line_for_data,",,") != NULL){
            return 1;/*2 commas*/
        }
        
        for(i = 0 ; i < strlen(line_for_data)-1 ; i++){
            if(line_for_data[i] == '-'){
                if(i == 0 && !isdigit(line_for_data[i+1]))
                return 2;/*if the first char is '-' and the next one isn't a number*/
                else if (!isdigit(line_for_data[i+1]) || line_for_data[i-1]!=',' )
                return 3;/*if there is a '-' without a comma before and a number afterwards*/
            }
            if(isalpha(line_for_data[i]) && line_for_data[i+1]==','){
                 return 4;/*if there is a comma at the beginning*/ 
                }
            else if(line_for_data[j-2]==','){
                 return 4;/*if there is a comma at the end*/
                }
            else if(line_for_data[j-2]=='-'){
                return 5;
            }
        }
        fprintf(fptr, "%s" , line);
        return 0;
    }            
    else if(!strcmp(nowW,"mov") || !strcmp(nowW,"cmp") || !strcmp(nowW,"add") || !strcmp(nowW,"sub") || !strcmp(nowW,"lea")||!strcmp(nowW,".struct")){
        for(i=0;i<strlen(line)-1;i++){
            if(line[i]==','){
                count+=1;
            }
        }
        if(count==1){/*only one right comma*/
            fprintf(fptr, "%s" , line);
            return 0;
        }
        return 4;
    }
    else{/*a order that does not need commas*/
        for(i=0;i<strlen(line)-1;i++){
            if(line[i]==','){
                count+=1;
            }
        }
        if(count==0){
            fprintf(fptr, "%s" , line);
            return 0;
        }
        else{
            return 4;
        }
    return 0;
    }
}

/*----------------------------------------------------------------------------------------------------*/

/*checks if there is a '#' before every numbers except from the .data and .struct*/
int check_hash(char* line,char* line_em){
    int i=0;
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char *nowW = NULL;  
    if(strlen(line_em)==0){
        return 0; 
    }
    strcpy(line_copy,line);
    nowW=strtok(line_copy," \t\n ");
    if(isLable(nowW)){
        nowW=strtok(NULL," ,\t\n ");/*only order*/
    }
    if (isOrder(nowW)){
        nowW=strtok(NULL," ,\t\n ");/*this will be checked word*/
        while(nowW!=NULL){
            if(isdigit(nowW[0]) || nowW[0] == '-'){
                return 1;                             
            }
            if(nowW[0] == '#'){
                if(nowW[1]!='-'&&!isdigit(nowW[1])){
                    return 1;
                }
                for (i=2 ; i< strlen(nowW)-2 ; i++){
                    if(!isdigit(nowW[i]))
                    return 1;
                }
            }
            nowW=strtok(NULL," ,\t\n ");
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------------------------------*/

/*checks if there is 2 registers in one command*/
int is_reg(char * line,char *nowW){
    char* dataLine=NULL;
    char line_copy[MAX_SIZE_LINE] = {'\0'};
    if(isOrder(nowW)){
        strcpy(line_copy,line);
        dataLine=strtok(line_copy," ,\t\n");
        if(isLable(dataLine)){
            dataLine=strtok(NULL," ,\t\n");
        }
        dataLine=strtok(NULL," ,\t\n");
        while(dataLine != NULL){
            if(dataLine[0]=='r' && strlen(dataLine) > 1){
                if(strlen(dataLine) > 2)
                return 1;
                else if(dataLine[1] == '8' || dataLine[1] == '9')
                return 1;   
            }
            dataLine=strtok(NULL," ,\t\n");
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------------------------------*/

/*checks if the .data and .string and .struct are inorder*/
int legal_str(char* line){
    char* dataLine=NULL;
    int i = 0;
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    strcpy(line_copy,line);
    dataLine=strtok(line_copy," ,\t\n");
    dataLine=strtok(NULL," ,\t\n");
    if(!strcmp(dataLine,".string")){
        dataLine=strtok(NULL," ,\t\n");
        if(dataLine[0]!='"'){
            printf("------%s\n-----------",dataLine);
            return 11;
        }
        for(i = strlen(line)-1 ; i>0 ; i--){
            if(!isspace(line[i])){
                if(line[i] != '"')
                return 11;
                else
                goto end_of_line;
            
            }
        }
    
    }
    if(!strcmp(dataLine,".data")){
        dataLine=strtok(NULL," ,\t ");
        if(!dataLine)
        return 12;
        while(dataLine != NULL ){
            if(dataLine[0]!='-' && dataLine[0]!='+' && !isdigit(dataLine[0])){
                return 12;
            }

            for(i = 1 ; i < strlen(dataLine) ; i++){
                if(dataLine[i] == '\n'){
                    goto end_of_line;
                }
                if(!isdigit(dataLine[i]))
                        return 12;
            }
            
            dataLine=strtok(NULL," ,\t ");
            if(dataLine[0] == '\n' || dataLine[strlen(dataLine)] == '\n')
            break;
        }
    }
    if(!strcmp(dataLine,".struct")){
        dataLine=strtok(NULL," ,\t\n");
        if(!dataLine)
            return 13;
        if(dataLine[0]!='-'&&dataLine[0]!='+'&&!isdigit(dataLine[0])){
            return 13;
        }
        for(i = 1 ; i < strlen(dataLine) ; i++){
            if(!isdigit(dataLine[i]))
                return 13;
        }
        dataLine=strtok(NULL," ,\t\n");
       if(dataLine[0]!='"'){
            return 11;
        }
        for(i = strlen(line)-1 ; i>0 ; i--){
            if(!isspace(line[i])){
                if(line[i] != '"')
                return 11;
                else
                goto end_of_line;
            }
        }
    }
    end_of_line:
    return 0;
}
