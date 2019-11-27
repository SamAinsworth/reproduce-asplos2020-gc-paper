#include <stack>
#include <queue>
#include <inttypes.h>
#include "../gem5-guardian/util/m5/m5op.h"
#include <assert.h>
#include <stdio.h>

std::stack<uint64_t> shadow;

int currentTarget=0;

std::queue<uint64_t> queues [12];


void clearQueue (int queue) {
    currentTarget = queue;

    while(!queues[queue].empty()) {
        uint64_t typeaddr = queues[queue].front();
        queues[queue].pop();
        int type = typeaddr & 3;
        uint64_t addr = typeaddr >>2;

        if(type == 2) {
            clearQueue(addr);
            return;
        } else if(type == 1) { //push
            shadow.push(addr);
        } else { //pop
				uint64_t comp;
				if(shadow.size()>0) {
					comp = shadow.top();
                    shadow.pop(); 
				}
                else comp = 0; //this check handles checkpoint simulation, where we start the shadow somewhere in the middle of execution.
            if(comp != addr) {
                printf ("%ld vs %ld\n", comp, addr);
                //assert(0);
            }
        }
    }
}


int main()
{


    while(1) {

        int from;// = m5_top_queue(4);
        asm volatile("mov X0, #0x4;.word	0xff560110; mov %0, X0" : "=r" (from) : : "x0" );
        uint64_t typeaddr;// = m5_pop_queue(1);
        asm volatile("mov X0, #0x1\n\t.word 0xff570110\n\t mov %0, X0" : "=r" (typeaddr) : : "x0", "x1" );


        if(from == currentTarget) {

            int type = typeaddr & 3;
            uint64_t addr = typeaddr >>2;
            if(type ==2) {
                clearQueue(addr);
                //printf ("push %ld\n",addr);
            } else if(type == 1) {
                shadow.push(addr);
            } else {
				uint64_t comp;
				if(shadow.size()>0) {
					comp = shadow.top();
                    shadow.pop(); 
				}
                else comp = 0; //this check handles checkpoint simulation, where we start the shadow somewhere in the middle of execution.
                if(comp != addr) {
                    printf ("%ld vsus %ld\n", comp, addr);
                    assert(0);
                }
            }
        } else {
            queues[from].push(typeaddr);
        }
    }
}
