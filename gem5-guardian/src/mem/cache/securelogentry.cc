        #include "securelogentry.hh"

        securelogentry securelogentry::queues
           [NUMBEROFCHECKERCORES][QUEUESIZE+NUMBEROFCHECKERCORES+1];
        int securelogentry::queue_end[NUMBEROFCHECKERCORES] = {0};
        int securelogentry::queue_start[NUMBEROFCHECKERCORES] = {0};
        int securelogentry::queue_size[NUMBEROFCHECKERCORES] = {0};
        std::queue<uint64_t> securelogentry::pausedOn[NUMBEROFCHECKERCORES];
        uint64_t securelogentry::counters[NUMBEROFCOUNTERS] = {0};

        bool securelogentry::filterb [ENUMSIZE] = {false,false,false,
                   false,false,false,false,false,false};
        std::vector<sprogram> securelogentry::programs;
        bool securelogentry::pause[NUMBEROFCHECKERCORES+1] =  {false};
        uint64_t securelogentry::minaddr = (uint64_t) -1;
        uint64_t securelogentry::maxaddr = (uint64_t) 0;


                         histoEntry securelogentry::histoEntries
                         [NUMBEROFHISTOENTRIES]={histoEntry()};
                         histoEntry securelogentry::bigBucket = histoEntry();
        uint64_t securelogentry::maxHistoSize = 100000;

                uint64_t securelogentry::minTime=(uint64_t) -1;
                uint64_t securelogentry::maxTime=0;
                uint64_t securelogentry::meanTime=0;
                uint64_t securelogentry::times=0;
    int securelogentry::ready = 0;
    //set by fetch1 and securelogentry.hh, for now.


    uint64_t securelogentry::started = 0;
        bool securelogentry::flush = false;

        uint64_t securelogentry::mapped = 0;
        uint64_t securelogentry::mapped_to_core[NUMBEROFCHECKERCORES] = {0};
        uint64_t securelogentry::fifo_occupancy[NUMBEROFCHECKERCORES]
               [QUEUESIZE+NUMBEROFCHECKERCORES+1] = {{0}};
