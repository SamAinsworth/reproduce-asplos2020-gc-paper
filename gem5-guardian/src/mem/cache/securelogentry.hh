
#ifndef __SECURELOGENTRY_HH__
#define __SECURELOGENTRY_HH__

#include <algorithm>
#include <fstream>
#include <queue>
#include <vector>

#include "cpu/base.hh"

//#include "cpu/minor/cpu.hh"
#include <iostream>
#include <string>

#include "cpu/thread_context.hh"
#include "mem/cache/base.hh"
#include "mem/packet.hh"
#include "mem/request.hh"

#define NUMBEROFCHECKERCORES 12
#define QUEUESIZE 64
#define ENUMSIZE 9
#define NUMBEROFCOUNTERS 8
#define NUMBEROFHISTOENTRIES 500

#define BIGBUCKETLIMIT 20480000


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

        struct sprogram{
                int map[ENUMSIZE];
                int counter;
                int corestart;
                int coreend;

        };

class securelogentry {







        static securelogentry queues [NUMBEROFCHECKERCORES][QUEUESIZE+NUMBEROFCHECKERCORES+1];
        static int queue_end[NUMBEROFCHECKERCORES];
        static int queue_start[NUMBEROFCHECKERCORES];
        static std::queue<uint64_t> pausedOn[NUMBEROFCHECKERCORES];

                        static histoEntry histoEntries[NUMBEROFHISTOENTRIES];
                        static histoEntry bigBucket;
                static uint64_t maxHistoSize;
                static uint64_t minTime;
                static uint64_t maxTime;
                static uint64_t meanTime;
                static uint64_t times;


        static bool filterb [ENUMSIZE];
        static std::vector<sprogram> programs;


        public:



                static uint64_t mapped;
        static uint64_t mapped_to_core[NUMBEROFCHECKERCORES];
        static uint64_t fifo_occupancy[NUMBEROFCHECKERCORES][QUEUESIZE+NUMBEROFCHECKERCORES+1];
                static int queue_size[NUMBEROFCHECKERCORES];


        static uint64_t minaddr;
        static uint64_t maxaddr;


        static int getNextCore(int limit, sprogram s) {
                int nextCore = -1;

                for (int x=s.corestart; x<=s.coreend; x++) {
                        if (queue_size[x]<limit) {
                                limit = queue_size[x];
                                nextCore = x;
                }



        }
                        return nextCore;
}

        static uint64_t counters[NUMBEROFCOUNTERS];

        static bool pause[NUMBEROFCHECKERCORES+1];

        static bool flush;

        static int ready;

        static uint64_t started;

                   static void printHisto() {

                                   if (times==0) return;
                                   
                                   
                   std::ofstream outfile;

				   outfile.open("m5out/delays.txt", std::ios::trunc);

                                               std::cout << "Delays: mean " << meanTime/times << " max " << maxTime << " min " << minTime << " ps\n";

                        std::cout << "size : number : min : max : mean" << std::endl;
                        
                        for (int x=0; x<NUMBEROFHISTOENTRIES; x++) {
                                        histoEntry h = histoEntries[x];
                                        std::cout << (x*maxHistoSize) / NUMBEROFHISTOENTRIES << " : " << h.number << " : " << ((h.minimum == (uint64_t) -1)? 0ul : h.minimum) << " : " << h.maximum << " : " << (h.number==0? 0ul : h.mean/h.number) << std::endl;
										outfile << (h.number==0? (x*maxHistoSize) / NUMBEROFHISTOENTRIES : h.mean/h.number) << " " << h.number << std::endl;
                    }
                    histoEntry h = bigBucket;
                     std::cout << "bigbucket : " << h.number << " : " << ((h.minimum == (uint64_t) -1)? 0ul : h.minimum) << " : " << h.maximum << " : " << (h.number==0? 0ul : h.mean/h.number) << std::endl;
					    outfile.close();
                }


        static void addToHisto (uint64_t time) {



                        times++;
                        meanTime+=time;
                        if (maxTime < time) maxTime=time;
                        if (minTime > time) minTime = time;


                        if (time > BIGBUCKETLIMIT) {
                                            //std::cout << "bigbucket" << time << " at cycle " << curTick() << "\n";
                                                        bigBucket.add_entry(time);
                                                        return;
                        }

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


    enum instType {Load,Store,IndirectBranch,Call,Return,Memflush,Barrier,Comms,Block};


        instType insttype;
        uint64_t data;
        uint64_t data2;
        uint64_t time;
        int from;

        uint64_t curtick;


        securelogentry (instType it, uint64_t d = 0, uint64_t d2 = 0, uint64_t t = 0)  {
                insttype = it;
                data = d;
                data2 = d2;
                time = t;
                curtick = curTick();
                from = 0;

        }

                securelogentry ()  {


        }


        static void GetNextLine(std::string& line, std::ifstream& m_InFile)
{

        while (std::getline(m_InFile, line))    // If a line was successfully read
        {
            // See if we found a valid line
            if (line.length() > 0 && line[0] != '#')
                break; // Done
            line.clear();
        }
}

        static void init() { //Called from O3CPU's cpu.cc
                pause[0] = false;
                for (int x=0;x<NUMBEROFCHECKERCORES; x++) {
                        queue_end[x]=0;
                        queue_start[x]=0;
                        queue_size[x]=0;
                        pause[x+1]=true;
                        //pausedOn[x]=-1;

                }
                for (int x=0; x<ENUMSIZE; x++)
                  filterb[x]=false;


                std::ifstream m_InFile;

                        //TODO: file IO
                        std::cout << "loading secmap\n";

                        m_InFile.open ("./secmap.ini");
                        std::string line;
                        GetNextLine(line,m_InFile);
                        int cores = 0;

                        while (!line.empty()) {
                                sprogram p;
                                p.corestart = cores;
                                p.coreend = cores+ atoi(line.c_str()) -1;
                                cores = p.coreend+1;
                                std::cout << "program range " << p.corestart << " to " << p.coreend << "\n";
                                p.counter = p.corestart;
                                GetNextLine(line,m_InFile);
                                for (unsigned i=0; i<line.length(); ++i)
                                {
                                        char A = line.at(i);
                                        p.map[i] = (A == '-' ? -1 : (A == '|') ? -2: (A == '0') ? 0 : p.corestart + (A > '9')? (A &~ 0x20) - 'A' + 10: (A - '0') );
                                        filterb[i] |= (A != '0');
                                }

                                programs.push_back(p);
                                GetNextLine(line,m_InFile);
                        }

                        ready = cores;
                        printf("mappers to initialize: %d\n", ready);
                        m_InFile.close();

        }

        static void add_to_queue(securelogentry s, int queue, int writer = 0) {
                mapped_to_core[queue]++;
                s.from = writer;
                //assert(!pause[writer]);

                                //std::cout << queue << " add size " << queue_size[queue] << "writer " << writer << "\n";


                assert(queue_size[queue] != QUEUESIZE + NUMBEROFCHECKERCORES+1);


                queues[queue][queue_end[queue]] = s;
                queue_end[queue] = (queue_end[queue] + 1) % QUEUESIZE;
                queue_size[queue]++;
                if (queue_size[queue]>=QUEUESIZE && !pause[writer]) {
                        //std::cout << "pause " << writer << " on " << queue <<"\n";
                        pause[writer] = true;
                        pausedOn[queue].push(writer);
            }
                if (queue_size[queue]==1 && pause[queue+1]) {
                         //std::cout << "unpause " << queue+1 << "\n";
                         pause[queue+1] = false;
                 }

        }

        static void core_queuewrite(uint64_t data, uint64_t target, int writer) {
                //assert(target==1);
                                                        //std::cout << "Writing " << data << " to queue " << target << "\n";

                securelogentry q(Comms,data);
                add_to_queue(q,target,writer);
        }

        static void send_messages(int threadid, uint64_t data1,uint64_t data2,uint64_t data3) {

                securelogentry q1(Comms,data1);
                securelogentry q2(Comms,data2);
                securelogentry q3(Comms,data3);
                map_before(q1);
                map_before(q2);
                map(q3);
        }


        static uint64_t pop_from_queue(int queue, int item) {

            assert(!pause[queue]);
            queue-=1;

            if (started==0) started=curTick();

            //std::cout << queue << " rem \n";

            //std::cout << queue << " pop size " << queue_size[queue] << "\n";


            assert(queue_size[queue] != 0);


            securelogentry s = queues[queue][queue_start[queue]];


            if (s.curtick > started) addToHisto(curTick()-s.curtick);


            //std::cout << "instType " << s.insttype << " data " << s.data <<"\n";

            uint64_t result = (item == 0) ? (uint64_t)s.insttype : ((item == 1) ? s.data :  ((item == 2) ? s.data2 : ((item == 4) ? s.from : s.time)));
                        queue_start[queue] = (queue_start[queue] + 1) % QUEUESIZE;
                queue_size[queue]--;
                if (queue_size[queue]<=QUEUESIZE) {
                while (!pausedOn[queue].empty()) {
                        //std::cout << "unpause " << pausedOn[queue].front() << "\n";
                        pause[pausedOn[queue].front()] = false;
                        pausedOn[queue].pop();
                }
        }

                if (queue_size[queue]==0) {
                         pause[queue+1] = true;
                         //std::cout << "pause " << queue+1 << "\n";
                 }

                //std::cout << "return " << result << "\n";
                return result;

        }


        static uint64_t top_from_queue(int queue, int item) {
            queue-=1;
            assert(queue_size[queue] != 0);

            securelogentry s = queues[queue][queue_start[queue]];

            uint64_t result = (item == 0) ? (uint64_t)s.insttype : ((item == 1) ? s.data :  ((item == 2) ? s.data2 : ((item == 4) ? s.from : s.time)));
                return result;

        }

        static void map(securelogentry s) {
                mapped++;
                for (sprogram& p: programs) {
                        int val = p.map[s.insttype];
                        if (val) {
                                if (val>0) {
                                        //std::cout << "Adding to queue " << val -1 << "\n";
                                        add_to_queue(s,val-1);
                                } else if (val==-1) {
                                        //counter mode
                                        add_to_queue(s,p.counter);
                                        p.counter++;
                                        //printf("here %d\n",p.counter);
                                        if (p.counter > p.coreend) {
                                                p.counter = p.corestart;
                                                //printf("wrap %d\n",p.counter);
                                        }
                                } else {
                                        //blockermode
                                        add_to_queue(s,p.counter);
                                        if (queue_size[p.counter]>=QUEUESIZE-2) {
                                                int newcore = getNextCore(QUEUESIZE>>1,p);
                                                //std::cout<< "blocker " << p.counter << " to " << newcore << "\n";

                                                if (newcore != -1) {

                                                        add_to_queue(securelogentry(Block,newcore),p.counter);
                                                        p.counter = newcore;
                                                }
                                        }
                                }
                        }
                }
        }


        static void map_before(securelogentry s) {
                mapped++;
                for (sprogram& p: programs) {
                        int val = p.map[s.insttype];
                        if (val) {
                                if (val>0) {
                                        //std::cout << "Adding to queue " << val -1 << "\n";
                                        add_to_queue(s,val-1);
                                } else if (val==-1) {
                                        //counter mode
                                        add_to_queue(s,p.counter);
                                } else {
                                        //blockermode
                                        add_to_queue(s,p.counter);
                                        if (queue_size[p.counter]>=QUEUESIZE-2) {
                                                int newcore = getNextCore(QUEUESIZE>>1,p);
                                                if (newcore != -1) {

                                                        add_to_queue(securelogentry(Block,newcore),p.counter);
                                                        p.counter = newcore;
                                                }
                                        }
                                }
                        }
                }
        }

        static void filter(securelogentry s) {

                if (!filterb[s.insttype]) return;
                if (ready > 0) return;
                                assert(!securelogentry::pause[0]);
                map(s);

        }



};


#endif
