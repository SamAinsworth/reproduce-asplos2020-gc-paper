#include <stdio.h>     
#include <assert.h>
#include <stdlib.h>    
#include "../gem5-guardian/util/m5/m5op.h"


int main(int argc, char **argv) {

  assert(argc==3);

  uint64_t min = atoll(argv[1]);
  uint64_t max = atoll(argv[2]);
  


  uint64_t size = (max-min)>>5;

  char* cfi =  (char*) malloc (size);

  while(1) {

    uint64_t address;// = m5_pop_queue(1);
    asm volatile("mov X0, #0x1\n\t.word 0xff570110\n\t mov %0, X0" : "=r" (address) : : "x0", "x1" );
    

    assert(address>= min && address <=max);  
    char lookup = cfi[(address-min) >> 5];
    assert(!(1<<(((address-min)>>2)*7) & lookup));
      

  }

}
