#include "../gem5-guardian/util/m5/m5op.h"

int main() {
	
uint64_t loads = 0;

uint64_t base = 0;

uint64_t top = 100000000l;
	
while(1) {
  uint64_t address;// = m5_pop_queue(1);
      asm volatile("mov X0, #0x1\n\t.word 0xff570110\n\t mov %0, X0" : "=r" (address) : : "x0", "x1" );

  if(address < top && address > base){
    loads++;
	if(loads==50) {
	  m5_add_counter(1,loads);
	  loads = 0;
	}
  }
}

return 0;
}
