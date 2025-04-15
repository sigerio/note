#include <stdio.h>
#include <string.h>
#include "calcu.h"
#include "func_inc.h"

Calculate my_cal = {NULL};

void cal_fun_register(void)
{

    my_cal.add = add_;
    my_cal.sub = subs_;
}



int main(void)
{
    cal_fun_register();
    int a = 10;
    int b = 5;
    printf("add = %d\n", my_cal.add(a, b));
    printf("sub = %d\n", my_cal.sub(a, b));

    return 0;
}