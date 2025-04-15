#ifndef __FUNC_INC_H
#define __FUNC_INC_H

typedef void (*func)(int, int);

typedef struct 
{
    int (*add)(int, int);
    int (*sub)(int, int);
}Calculate;


#endif


