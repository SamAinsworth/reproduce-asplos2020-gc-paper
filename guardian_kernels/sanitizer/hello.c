#include <stdio.h>

int main()
{
   // printf() displays the string inside quotation
long long int y[10];


for(long int x=0; x<10; x++) {
   y[x]= x*x;
}   
   
for(long long int x=0; x<1000000000ll; x++)
   if((x*x + y[x%10])<40l || ((x&(1<<20)-1) == 0))printf("Hello, World! %lld\n", x*x + y[x%10]);
   return 0;
}
