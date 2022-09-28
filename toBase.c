#include "headers.h"

char base[32]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

/*----------------------------------------------------------------------------------------------------*/

/*this func converts the number from binary to 32 base*/
void base_32(FILE *file1,char name[MAX_SIZE_WORD]){
    int count1=100;
    FILE* fptr;
    char *binary_count;
    char base1[2]={'\0'};
    char base2[2]={'\0'}; 
    char base3[2]={'\0'}; 
    char arr_8[10]={'\0'};
    char line[MAX_SIZE_LINE] = {'\0'};
    fptr  = my_open(name,"w+");  
    while(!feof(file1) && fgets(line , MAX_SIZE_LINE, file1)){
        binary_count=Binary_10(count1,arr_8);
        turn_32(binary_count,base1);
        base2[0]=base1[0];
        base2[1]=base1[1];
        
        fprintf(fptr,"%c",base2[0]);
        fprintf(fptr,"%c    ",base2[1]);
        count1++;
        turn_32(line,base3);
        fprintf(fptr,"%c",base3[0]);
        fprintf(fptr,"%c\n",base3[1]);
    }
    fclose(file1);
    fclose(fptr);
}

/*----------------------------------------------------------------------------------------------------*/

/*converts numbers from binary to 32 base*/
void turn_32(char * data,char base1[2]){
    int i=0;
    int j=1;
    int num=0;
    for(i=9;i>=0;i--){
        num=0;
        num+=data[i]-'0';
        i--;
        num+=(data[i]-'0')*2;
        i--;
        num+=(data[i]-'0')*4;
        i--;
        num+=(data[i]-'0')*8;
        i--;
        num+=(data[i]-'0')*16;

        base1[j]=base[num];
        j--;
    }
}

/*----------------------------------------------------------------------------------------------------*/

/*converts to 10 digits binary*/
char* Binary_10(unsigned int num,char tmp_arr1[10]){
    int j= 0;
    int cur=0;
    for(j=9;j>=0;j--) {
        cur=num & 0x1;
        tmp_arr1[j] = cur + '0';
        num  = num >> 1;
        
    }
    return tmp_arr1;
}


