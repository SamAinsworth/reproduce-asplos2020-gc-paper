

#ifndef __LOADSTORELOGENTRY_HH__
#define __LOADSTORELOGENTRY_HH__

#include <algorithm>
#include <fstream>
#include <vector>

#include "cpu/base.hh"
#include "cpu/minor/cpu.hh"
#include "cpu/thread_context.hh"
#include "mem/cache/base.hh"
#include "mem/packet.hh"
#include "mem/request.hh"

#define NUMBEROFCHECKERCORESPERCORE 12
#define NUMBEROFMAINCORES 1
#define logsize 4608  //2304 in past
#define TIMEOUT 5000
#define AIMD 1

#define NUMBEROFHISTOENTRIES 100


using namespace TheISA;


struct histoEntry {
        uint64_t number;
        uint64_t mean;
        uint64_t minimum;
        uint64_t maximum;

        void add_entry(uint64_t time) {
                mean += time;
                number++;

                if (time<minimum) minimum = time;
                if (time>maximum) maximum = time;

        }

        static histoEntry merge (histoEntry entry1, histoEntry entry2) {
                histoEntry nh;
                nh.number = entry1.number + entry2.number;
                nh.maximum = (entry2.maximum > entry1.maximum) ? entry2.maximum : entry1.maximum;
                nh.minimum = (entry2.minimum > entry1.minimum) ? entry1.minimum : entry2.minimum;
                nh.mean = entry1.mean + entry2.mean;

                return nh;
        }

        histoEntry() {
                number = mean = maximum = 0;
                minimum = (uint64_t)-1;
        }
};

struct miniContext {

    FloatReg floatRegs[NumFloatRegs];
    IntReg intRegs[NumIntRegs];


    CCReg ccRegs[NumCCRegs];


    MiscReg miscRegs[NumMiscRegs];

    TheISA::PCState pcState;

    MiscReg CPSR;

    bool initialized;

    miniContext() {
        initialized = false;
    }

};



miniContext
m_serialize(ThreadContext &tc);


void m_copyRegs(ThreadContext* tc, miniContext m);

class loadstorelogentry {






    public:
        bool load;
        bool isSC;
        Addr addr;
        std::vector<uint8_t> data;
        uint64_t extra_data;
        bool valid;
        uint64_t time;

                static histoEntry histoEntries[NUMBEROFHISTOENTRIES];
                static uint64_t maxHistoSize;

        static std::vector<loadstorelogentry> entries;
        static std::vector<uint64_t> timestamps;
        static std::vector<uint64_t> committedInstructions;

                static int timeout[NUMBEROFMAINCORES];
        static int current_segment_to_fill[NUMBEROFMAINCORES];
        static int current_entry[NUMBEROFMAINCORES];
        static int current_size[NUMBEROFMAINCORES];
        static bool waiting_to_checkpoint[NUMBEROFMAINCORES];
        static uint64_t meanTime;
        static uint64_t maxTime;
        static uint64_t minTime;
        static uint64_t times;
        static bool ready[NUMBEROFMAINCORES];
        static uint64_t timestamp[NUMBEROFMAINCORES];
        static uint64_t committed_timestamp[NUMBEROFMAINCORES];
        static bool earlyCommit[NUMBEROFMAINCORES];

        static uint64_t should_cache_wait[NUMBEROFMAINCORES][NUMBEROFMAINCORES];



        static std::vector<BaseCPU*> baseCPUs;
        static std::vector<int> entryIndices;
        static std::vector<int> accessedThisRound;
        static miniContext previousThreadContext[NUMBEROFMAINCORES];
        static std::vector<bool> segmentFree;
        static std::vector<bool> readyToCommit;

            static void printHisto() {
                        std::cout << "size : number : min : max : mean" << std::endl;
                        for (int x=0; x<NUMBEROFHISTOENTRIES; x++) {
                                        histoEntry h = histoEntries[x];
                                        std::cout << (x*maxHistoSize) / NUMBEROFHISTOENTRIES << " : " << h.number << " : " << ((h.minimum == (uint64_t) -1)? 0ul : h.minimum) << " : " << h.maximum << " : " << (h.number==0? 0ul : h.mean/h.number) << std::endl;
                    }
                }

        static void addToHisto (uint64_t time) {
                        while (maxHistoSize < time) {
                                maxHistoSize *=2;
                                for (int x=0; x<(NUMBEROFHISTOENTRIES); x+=2) {
                                   histoEntries[x/2] = histoEntry::merge(histoEntries[x],histoEntries[x+1]);
                                }
                                for (int x=NUMBEROFHISTOENTRIES/2; x<NUMBEROFHISTOENTRIES; x++) {
                                        histoEntries[x] = histoEntry();
                                }


                        }

                                                        uint64_t index = (NUMBEROFHISTOENTRIES*time) / maxHistoSize;

                                histoEntries[index].add_entry(time);
                }


        loadstorelogentry (bool isLoad, bool isStoreConditional ,Addr address, uint8_t* ld_data, uint8_t size, uint64_t secondary_data, uint64_t t) {
            load = isLoad;
            isSC = isStoreConditional;
            if (ld_data) { data.assign(ld_data,&ld_data[size]);
            /*if (size>9)  {printf("size %d\n", size);
                                for (auto i = data.begin(); i != data.end(); ++i)    std::cout << *i << ' ';
                                        std::cout << "\n";
                        }*/

                                }
            addr = address;
            extra_data = secondary_data;
            valid = true;
            time = t;

        }

        loadstorelogentry () {
            load = false;
            isSC = false;
            addr = 0;
            valid = false;
            extra_data = 0;
            time = 0;
        }

        bool do_read(PacketPtr pkt, bool firstRound, int id) {


            //assert(load || isSC);
            //assert(pkt->isRead());

            //assert(addr == pkt->req->getVaddr()); //TODO: vaddr?




            if (addr != pkt->req->getVaddr() || load != pkt->isRead()) {
                //	assert(0);

                if (load && pkt->isRead()) {
                                        int64_t diff = (int64_t)pkt->req->getVaddr() - (int64_t) addr;
                                        if (diff < data.size()) {
                                                //printf("read match close enough! queue %ld vs new %ld on %d\n", addr,  pkt->req->getVaddr(), id);
                                                pkt->deleteData();
                                                 pkt->dataStatic(data.data()+diff);
                                                 return true;
                                        }

                                }

                if (!load && !pkt->isRead()) {
                    int64_t diff =(int64_t) addr - (int64_t)pkt->req->getVaddr();
                    if (diff < 3 && diff > -3) {
                        //printf("write match close enough! queue %ld vs new %ld on %d\n", addr,  pkt->req->getVaddr(), id);
                        //std::cout << pkt->cmdString() << "\n";
                        return true;
                    }
                }

                 //if (firstRound) printf("vaddrs: queue %c %ld vs new %c %ld on %d\n", load? 'r':'w' ,addr, pkt->isRead()? 'r':'w', pkt->req->getVaddr(), id);
                //std::cout << pkt->cmdString() << "\n";

                //std::cout << pkt->req->
                return false;
            } else if (!firstRound) {
               // printf("weird match! vaddrs: queue %c %lX vs new %c %lX on %d\n", load? 'r':'w' ,addr, pkt->isRead()? 'r':'w', pkt->req->getVaddr(), id);
                //std::cout << pkt->cmdString() << "\n";
            } else {


                //
                //PacketPtr pkt = Packet::createRead(memReq);
                uint64_t newTime = curTick() - time;

                minTime = std::min(minTime,newTime);
                maxTime = std::max(maxTime,newTime);

                                                addToHisto(newTime);

                meanTime = (meanTime*times+newTime)/(times+1);
                times++;
            }

            if (load)
            {   pkt->deleteData(); //TODO: should not be necessary...

                pkt->dataStatic(data.data());
                /*if (data.size()>9) {
                                        printf("load size %ld at %lX\n", data.size(), pkt->req->getVaddr());
                                        for (auto i = data.begin(); i != data.end(); ++i)    std::cout << *i << ' ';
                                        std::cout << "\n";
                                }*/

            }
            else if (isSC) {
                pkt->req->setExtraData(extra_data);
            }

            return true;
        }

        static bool do_write(loadstorelogentry l, int cpuID) {
            int size_of_segment = logsize/NUMBEROFCHECKERCORESPERCORE;

                        assert(current_entry[cpuID] < size_of_segment);

            entries[current_segment_to_fill[cpuID]*size_of_segment + current_entry[cpuID]] = l;
            /*if (l.load)
            printf("writing entry %d,%d, addr %ld, read\n", current_segment_to_fill, current_entry, l.addr);
            else
            printf("writing entry %d,%d, addr %ld, write\n", current_segment_to_fill, current_entry, l.addr);*/
            current_entry[cpuID]++;
            current_size[cpuID]+= l.load? 2 : 3;

            //if (current_entry[cpuID] == size_of_segment) {
            if (current_size[cpuID] >= size_of_segment-2 ) {

                return true;
            }
            return false;

        }

        static bool do_read(PacketPtr pkt, ThreadContext* tc) {
            int size_of_segment = logsize/NUMBEROFCHECKERCORESPERCORE;


            int id = tc->contextId()-NUMBEROFMAINCORES;

            int mainCPUID = id/NUMBEROFCHECKERCORESPERCORE;
            if (loadstorelogentry::baseCPUs[tc->contextId()]->getContext(0)->status() == ThreadContext::Status::Suspended || ((MinorCPU*)(loadstorelogentry::baseCPUs[tc->contextId()]))->isDraining)
            {
                //printf("should be suspended on %d\n", id);
                return false;
            }

            bool accessed = false;

            assert(tc->contextId() == baseCPUs[id+NUMBEROFMAINCORES]->getContext(0)->contextId());

            bool foundAlready = true;

            if (!(entryIndices[id] >= size_of_segment) && entries[size_of_segment*id+entryIndices[id]].valid && !entries[size_of_segment*id+entryIndices[id]].do_read(pkt,true, id)) {
                //if (entryIndices[id]>0) entryIndices[id]-=1;
                foundAlready = false;
            }

           // int expectedMatch = entryIndices[id];

            if (!foundAlready) {
                for (int x=-1; x<3 && x>-3; x= x>0? -x-1:-x) {
                    int entry = entryIndices[id]+x>0 && entryIndices[id]+x < size_of_segment?entryIndices[id]+x:0;
                    if (entries[size_of_segment*id+entry].do_read(pkt,false, id) && entries[size_of_segment*id+entry].valid) {
                        foundAlready = true;
                        //printf("matched at %d, expected %d\n", entry, entryIndices[id]);
                        entryIndices[id] = entry;

                        break;
                    }
                }
            }

            if (foundAlready) {
                entryIndices[id]++;
                accessedThisRound[id]++;
                accessed = true;
            } else if (pkt->req->isPrefetch()) {
                accessed = true;
                foundAlready = true;
                printf("prefetch %ld on %d, ignored\n", pkt->req->getVaddr(), id);
            } else if (pkt->isWrite()) {
                             //   printf("missing write %ld on %d\n", pkt->req->getVaddr(), id);


                                accessed = true;
                                foundAlready = true;
                        }
             else if (previousThreadContext[mainCPUID].initialized) {
                assert(!pkt->req->isUncacheable());
                if (accessedThisRound[id] < (size_of_segment-1) || entryIndices[id] < (size_of_segment-1) ) {
                         //               printf("not found %lX %c on %d, expected at %d, %lX\n", pkt->req->getVaddr(), pkt->isRead()? 'r':'w', id, entryIndices[id], entries[size_of_segment*id+entryIndices[id]].addr);
                //std::cout << pkt->cmdString() << "\n";
                /*for (int x=0; x<20; x++) {
                        int y=std::min(std::max(entryIndices[id]+x-10,0),size_of_segment-1);
                        printf("%d : %ld\n",y, entries[size_of_segment*id+y].addr);
                }*/
                //getchar();
                        }
            }


            if (!foundAlready || !entries[size_of_segment*id+entryIndices[id]].valid || accessedThisRound[id] >= size_of_segment || !previousThreadContext[mainCPUID].initialized) {
                //suspend CPU, maybe reawaken O3.
                entryIndices[id] = 0;
                accessedThisRound[id] = 0;
               // printf("sleeping checker cpu %d\n", id);
                loadstorelogentry::baseCPUs[id+NUMBEROFMAINCORES]->drain();


            }

            return accessed;

        }


        static void print_times() {
            std::cout << "Delays: mean " << meanTime/times << " max " << maxTime << " min " << minTime << " ps\n";
            std::ofstream outfile;

  outfile.open("delays.txt", std::ios_base::app);
  outfile << "Delays: mean " << meanTime/times << " max " << maxTime << " min " << minTime << " ps\n";
  outfile.close();
  printHisto();
        }

};


void add_cpu(BaseCPU* cpu, int cpuID);


#endif
