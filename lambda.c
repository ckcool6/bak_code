#include <stdio.h>

#define LAMBDA(ret_type, _body) ({ ret_type _ _body _; })


int main(int argc, char const *argv[])
{
    /* code */
    int (*max)(int, int) = LAMBDA(int, (int x, int y) {
         return x > y ? x : y;
         });

    int res = max(2, 8);
    
    printf("%d", res);
    return 0;
}
