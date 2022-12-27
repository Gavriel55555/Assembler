#include "headers.h"
char labels[155][20]={"\0"};
char addressL[155][20]={'\0'};
char tmp_arr[8]={'\0'};
/*----------------------------------------------------------------------------------------------------*/

/*this func puts all the lablels and there addresses into the lists*/ 
void create_lable_list(FILE *file1){
    char *nowW = NULL;
    unsigned int count=99;
    int counts_r=0; /*to check if 2 registers appears in one line*/
    char * address="\0";
    int valid=0;
    int i=0;
    int k = 0;
    char arr[8]={'\0'};
    char line[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy2[MAX_SIZE_LINE] = {'\0'}; 
    rewind(file1);
    while (!feof(file1) && fgets(line , MAX_SIZE_LINE, file1)){
        strcpy(line_copy,line);
        strcpy(line_copy2,line);
        nowW = strtok(line_copy, " ,\t\n "); /* get first word */
        if(!strcmp(nowW,".entry")){
            continue;
        }
        else if(isLable(nowW)){/*if first word in line is lable - enter the correct binary code in list for it */
            count++;
            address=binary(count,arr);
            strcat(address,"10");
            strcpy(labels[i],nowW);
            strcpy(addressL[i],address);
            nowW = strtok(NULL," ,\t\n ");/*running till end of line in check*/
            i++;
            goto notLable;
        }
        else if(!strcmp(nowW,".extern")){
            nowW = strtok(NULL," ,\t\n ");
            if(i!=0){
            strcat(nowW,":");
            }
            else{
                nowW[strlen(nowW)]=':';
            }
            strcpy(labels[i],nowW);
            strcpy(addressL[i],"0000000001");
            i++;
        }
        else{
            count++;
            notLable:

            if(!strcmp(nowW,".string")||!strcmp(nowW,".data")||!strcmp(nowW,".struct")){
                goto there;
            }
            if(nowW!=NULL){
                nowW = strtok(NULL," ,\t\n ");/*running till end of line in check*/
            }
            while(nowW != NULL){
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
                valid=0;
                nowW = strtok(NULL," ,\t\n ");/*running till end of line in check*/
            }
            there:
            if(nowW && !strcmp(nowW,".string")){
                nowW = strtok(NULL," ,\t\n ");
                count+=(strlen(nowW)-2);
            }
            if(nowW && !strcmp(nowW,".data")){
                nowW = strtok(NULL," ,\t\n ");
                while(nowW!=NULL){
                    count++;
                    nowW = strtok(NULL," ,\t\n ");
                }
                count-=1;
            }
            if(nowW && !strcmp(nowW,".struct")){
                nowW = strtok(NULL," ,\t\n ");
                count++;
                nowW = strtok(NULL," ,\t\n ");
                count+=(strlen(nowW)-2);
            }

        }  
    }
}

/*----------------------------------------------------------------------------------------------------*/

/*This function is responsible for the final transition to binary*/
void prepBinary(FILE *file1){
    FILE *fptr;
    FILE *fptr2;
    int counter=0;
    int tmp_counter=0;
    int del=0;
    char line[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy2[MAX_SIZE_LINE] = {'\0'}; 
    char *nowW =NULL;
    char *nowW2 =NULL;
    fptr  = my_open("binary.ob","w+");
    fptr2= my_open("instruction.ob","w+");
    create_lable_list(file1);
    rewind(file1);
    while (!feof(file1) && fgets(line , MAX_SIZE_LINE, file1)){
        strcpy(line_copy,line);
        strcpy(line_copy2,line);
        nowW = strtok(line_copy, " \t "); /* get first word */
        if(isLable(nowW)){
            nowW=strtok(NULL," \t ");
            if(!strcmp(nowW,".data")||!strcmp(nowW,".string")||!strcmp(nowW,".struct")){
                nowW2=strtok(NULL,"\n");
               counter+= prep_instruction(fptr2,nowW,nowW2,tmp_counter);
            }
            counter+=words_handle(nowW,line_copy2,fptr,tmp_counter);
        }
        else if(isOrder(nowW)){
            counter+=words_handle(nowW,line_copy2,fptr,tmp_counter);
        }
    }
    rewind(fptr2);
    while (!feof(fptr2) && fgets(line , MAX_SIZE_LINE, fptr2)){
        fprintf(fptr, "%s" , line); 
    
    }
    fclose(file1);
    fclose(fptr);
    fclose(fptr2);
    del=remove("instruction.ob");
    if(del != 0) {
      printf("Error: unable to delete the file");
   }
}

/*----------------------------------------------------------------------------------------------------*/

/*checks if the word is a label and returns where the address of this label*/
int return4L(char* nameD){
    char new[MAX_SIZE_LINE]={'\0'};
    int j = 0;
    int i=0;

    for(i=0;i<strlen(nameD);i++){
        new[i]=nameD[i];
    }
    new[i]=':';
    while(strlen(labels[j])!=0){
        if(!strcmp(labels[j],new)){
            return j;
        }
        j++;
    }
    return 0; 
}

/*----------------------------------------------------------------------------------------------------*/


int checks4L(char* nameD){/*checks if the word is a label */
    char new[MAX_SIZE_LINE]={'\0'};
    int j = 0;
    int i=0;

    for(i=0;i<strlen(nameD);i++){
        new[i]=nameD[i];
    }
    new[i]=':';
    while(strlen(labels[j])!=0){
        if(!strcmp(labels[j],new)){
            return 1;
        }
        j++;
    }
    return 0; 
}

/*----------------------------------------------------------------------------------------------------*/

/*checks if the word is a label and returns where the address of this label*/
int checks4L2(char* nameD){
    char new[MAX_SIZE_LINE]={'\0'};
    int j = 0;
    int i=0;

    for(i=0;i<strlen(nameD);i++){
        new[i]=nameD[i];
    }
    new[i]=':';
    while(strlen(labels[j])!=0){
        if(!strcmp(labels[j],new)){
            return j;
        }
        j++;
    }
    return 0; 
}

/*----------------------------------------------------------------------------------------------------*/

/*this func gets the place of the address we want*/
char *check4add(int k){ 
    return addressL[k];
}

/*----------------------------------------------------------------------------------------------------*/

/*this func checks what kind of a operand it gets*/
char *the_order(char * data){
    char * word=NULL;
    to_malloc(word,10);

    if(data[0]=='#'){
        free(word);
        return "00";
    }
    if(!strcmp(data,"r7")|| !strcmp(data,"r6")|| !strcmp(data,"r5")|| !strcmp(data,"r4")||
       !strcmp(data,"r3")|| !strcmp(data,"r2")|| !strcmp(data,"r1")|| !strcmp(data,"r0")||
       !strcmp(data,"r7\n")|| !strcmp(data,"r6\n")|| !strcmp(data,"r5\n")|| !strcmp(data,"r4\n")||
       !strcmp(data,"r3\n")|| !strcmp(data,"r2\n")|| !strcmp(data,"r1\n")|| !strcmp(data,"r0\n")){
        free(word);
        return "11";
    }
    else if(checks4L(data)){
        free(word);
        return "01";
    } 
    else{
        free(word);
        return "10";
    }
}

/*----------------------------------------------------------------------------------------------------*/
/*this func convers all the operands to there binary form*/
int words_binary(char *order,char* line,FILE * fptr,int counter){
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char line_copy2[MAX_SIZE_LINE] = {'\0'}; 
    char * nowD=NULL;
    char  arr[20]={'\0'};
    char  arr2[MAX_SIZE_LINE]={'\0'};
    char  arr3[MAX_SIZE_LINE]={'\0'};
    char *str_num=NULL;
    int num=0;
    int size=0;
    int i=0;
    int j=0;
    int k=0;
    char *word=NULL;
    strcpy(line_copy,line);
    strcpy(line_copy2,line);
    if(isItReg(fptr,line_copy2,counter)){
        goto here;
    }
    word=strtok(line_copy," \t\n");
    if(isLable(word)){
        word=strtok(NULL," \t\n ");
         
    }
     
    if(!strcmp(word,"mov") || !strcmp(word,"add") || !strcmp(word,"cmp") || !strcmp(word,"sub") || !strcmp(word,"lea")){
        size = 2;
    
    }
    else if(!strcmp(word,"not") || !strcmp(word,"clr") || !strcmp(word,"inc") ||
            !strcmp(word,"dec") || !strcmp(word,"jmp") || !strcmp(word,"bne") ||
            !strcmp(word,"get") || !strcmp(word,"prn") || !strcmp(word,"jsr"))
                size = 1;
    
    word=strtok(NULL," ,\t\n");/*FOR THE FIRST WORD after the order*/
    while(i<size){
        if(!strcmp(word,"r0") || !strcmp(word,"r1") || 
        !strcmp(word,"r2") || !strcmp(word,"r3") || 
        !strcmp(word,"r4")|| !strcmp(word,"r5")|| !strcmp(word,"r6")|| 
        !strcmp(word,"r7")){
                if(!strcmp(word,"r0")){
                    nowD = "0000000000";
                }
                else if(!strcmp(word,"r1")){
                    if(i==0)
                    nowD = "0001000000";
                    else
                    nowD = "0000000100";
                }
                else if(!strcmp(word,"r2")){
                    if(i==0)
                    nowD = "0010000000"; 
                    else
                    nowD = "0000001000";
                }
                else if(!strcmp(word,"r3")){
                    if(i==0)
                    nowD = "0011000000";
                    else
                    nowD = "0000001100";
                }
                else if(!strcmp(word,"r4")){
                    if(i==0)
                    nowD = "0100000000"; 
                    else
                    nowD = "0000001000"; 
                }
                else if(!strcmp(word,"r5")){
                    if(i==0)
                    nowD = "0101000000"; 
                    else
                    nowD = "0000010100";
                }
                else if(!strcmp(word,"r6")){
                    if(i==0)
                    nowD = "0110000000";
                    else
                    nowD = "0000011000";
                }
                else if(!strcmp(word,"r7")){
                    if(i==0)
                    nowD = "0111000000";
                    else
                    nowD = "0000011100";
                }
                counter++;
                fprintf(fptr, "%s\n" , nowD);
                
                word=strtok(NULL," ,\t\n");
                i++;
            
        }
        else if(word[0]=='#'){
            for(j=1;j<strlen(word);j++){
                arr[j-1]=word[j];
            }
            num=atoi(arr);
            str_num=tmpBinary(num);
            fprintf(fptr, "%s" ,str_num);
            fprintf(fptr, "%s\n" , "00");
            counter++;
            word=strtok(NULL," ,\t\n");
            i++;
        }
        else if(checks4L(word)){
            num=return4L(word);
            counter++;
            fprintf(fptr, "%s\n" , addressL[num]);
            word=strtok(NULL," ,\t\n");
            i++;
        }
        else if(strstr(word,".") != NULL){
            j=0;
            while(word[j]!='.'){
                arr2[j]=word[j];   
                j++;
            }
            j++;
            while(word[j]){
                arr3[k]=word[j];
                j++;
                k++;
            }
            num=return4L(arr2);
            fprintf(fptr, "%s\n" , addressL[num]);
            counter++;
            num=0;
            num=atoi(arr3);
            str_num=tmpBinary(num);
            fprintf(fptr, "%s" , str_num);
            fprintf(fptr, "%s\n" , "00");
            counter++;
            
            word=strtok(NULL," ,\t\n");
            i++;

        }
        else{
            return counter;
        }
    }
    here:
    return counter;
}

/*----------------------------------------------------------------------------------------------------*/

/*this funcs checks if 2 registers have been called together */
int isItReg(FILE *fptr,char* line,int counter){
    char line_copy[MAX_SIZE_LINE] = {'\0'}; 
    char *word=NULL;
    char * count=NULL;
    strcpy(line_copy,line);
    word=strtok(line_copy," \t\n");
    if(!strcmp(word,"mov") || !strcmp(word,"add") || !strcmp(word,"cmp") || !strcmp(word,"sub") || !strcmp(word,"lea")){

        if(isLable(word)){
            word=strtok(NULL," \t\n ");
        }
        word=strtok(NULL," ,\t\n");/*FOR THE FIRST WORD after the order*/
        if(!strcmp(word,"r0") || !strcmp(word,"r1") || 
            !strcmp(word,"r2") || !strcmp(word,"r3") || 
            !strcmp(word,"r4")|| !strcmp(word,"r5")|| !strcmp(word,"r6")|| 
            !strcmp(word,"r7")){
            if(!strcmp(word,"r0")){
                count="0000";
            }
            else if(!strcmp(word,"r1")){
                count ="0001";
            }
             else if(!strcmp(word,"r2")){
                count ="0010";
            }
            else if(!strcmp(word,"r3")){
                count ="0011";
            }
            else if(!strcmp(word,"r4")){
                count ="0100";
            }
            else if(!strcmp(word,"r5")){
                count ="0101";
            }
            else if(!strcmp(word,"r6")){
                count ="0110";
            }
            else if(!strcmp(word,"r7")){
                count ="0111";
        }
        word=strtok(NULL," ,\t\n");
        if(!strcmp(word,"r0") || !strcmp(word,"r1") || 
            !strcmp(word,"r2") || !strcmp(word,"r3") || 
            !strcmp(word,"r4")|| !strcmp(word,"r5")|| !strcmp(word,"r6")|| 
            !strcmp(word,"r7")){
                fprintf(fptr, "%s" , count);
                if(!strcmp(word,"r0")){
                   fprintf(fptr, "%s" , "0000");
                }
                else if(!strcmp(word,"r1")){
                    fprintf(fptr, "%s" , "0001");
                }
                else if(!strcmp(word,"r2")){
                    fprintf(fptr, "%s" , "0010");
                }
                else if(!strcmp(word,"r3")){
                    fprintf(fptr, "%s" , "0011");
                }
                else if(!strcmp(word,"r4")){
                    fprintf(fptr, "%s" , "0100");
                }
                else if(!strcmp(word,"r5")){
                    fprintf(fptr, "%s" , "0101");
                }
                else if(!strcmp(word,"r6")){
                    fprintf(fptr, "%s" , "0110");
                }
                else if(!strcmp(word,"r7")){
                    fprintf(fptr, "%s" , "0111");
                }
                fprintf(fptr, "%s\n" , "00");
                counter++;
                return counter;       
        }
        }
    }
        
    return 0;
}

/*----------------------------------------------------------------------------------------------------*/
/*converts the num in base 10 to a binary with 8 digits*/
char* tmpBinary(unsigned int num){
    int j= 0;
    int cur=0;
        for(j=7;j>=0;j--){
        cur=num & 0x1;
        tmp_arr[j] = cur + '0';
        num  = num >> 1;
        
    }
    return tmp_arr;
}
