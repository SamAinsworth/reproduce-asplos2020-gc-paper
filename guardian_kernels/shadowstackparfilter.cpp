#include <stack>
#include <inttypes.h>
#include "../gem5-guardian/util/m5/m5op.h"
#include <assert.h>
#include <stdio.h>

int main()
{
    std::deque<uint64_t> shadow;


    while(1) {

        int type;
        //asm volatile("mov X0, #0x0;.word	0xff560110; mov %0, X0" : "=r" (lastpush) : : "x0" );
        //lastpush = m5_top_queue(0);
        uint64_t addr;// = m5_pop_queue(1);
        asm volatile("mov X0, #0x1\n\t.word 0xff570110\n\t mov %0, X0\n\t mov %1, X1" : "=r" (addr), "=r" (type) : : "x0", "x1" );


        if(type ==3) {
            shadow.push_front(addr);
            //printf ("push %ld\n",addr);
        } else if(type == 4) {
            if(shadow.empty()) {
				asm volatile("mov X0, #0x0; mov X1, %0;.word	0xff580110" : : "r" (addr<<2) : "x0", "x1" );
                //m5_put_queue(0,((addr<<2)));
            } else {
                uint64_t comp = shadow.front();
                //printf ("pop %ld\n",addr);
                shadow.pop_front();
                if(comp != addr) {
                    printf ("%ld vses %ld\n", comp, addr);
                    //assert(0);
                }
            }
        } else {
            while(!shadow.empty()) {
                uint64_t data = shadow.back();
                shadow.pop_back();
                asm volatile("mov X0, #0x0; mov X1, %0;.word	0xff580110" : : "r" ((data<<2)&1) : "x0", "x1" );

                //m5_put_queue(0,(data<<2) & 1);
            }
            asm volatile("mov X0, #0x0; mov X1, %0;.word	0xff580110" : : "r" ((addr<<2)&2) : "x0", "x1" );
            //m5_put_queue(0,(addr<<2)&2);
        }
    }
}
