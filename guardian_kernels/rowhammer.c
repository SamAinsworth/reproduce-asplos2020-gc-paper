#include "../gem5-guardian/util/m5/m5op.h"

#define threshold 42949672
#define maxtime 40000
#define rowsize (64*1024)


//Mixed Congruential method from Numerical Recipes
unsigned int hash(unsigned int x) {
    x = x* 1664525;
    x = x+1013904223;
    return x;
}

int main() {
	
unsigned int rand = 1;
	
while(1) {

  uint64_t time;
      asm volatile("mov X0, #0x3;.word	0xff560110; mov %0, X0" : "=r" (time) : : "x0" );

  uint64_t address;// = m5_pop_queue(1);
      asm volatile("mov X0, #0x1\n\t.word 0xff570110\n\t mov %0, X0" : "=r" (address) : : "x0", "x1" );

  if(time < maxtime) continue;
  
  rand = hash(rand);
     

  if(rand < threshold){
     //really, want to refresh here, but this will do.
     uint64_t target = address + rowsize;
     __builtin_prefetch((const void *)target);
     uint64_t target2 = address - rowsize;
     __builtin_prefetch((const void *)target2);
  }
}

return 0;
}
