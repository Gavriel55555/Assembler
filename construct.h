#include "const.h"

typedef struct node* macro;
typedef struct node {
    char * nameM;
    char * dataM; 

    /* pointer to the next macro */
     macro  next;
} item;

typedef struct node2* ext;
typedef struct node2 {
    char * nameE;

    /* pointer to the next macro */
     ext  next;
}item2;