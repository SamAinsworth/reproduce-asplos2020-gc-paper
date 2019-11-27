
#include "mem/cache/loadstorelogentry.hh"
#include "sim/syscall_emul.hh"

        int loadstorelogentry::current_segment_to_fill[NUMBEROFMAINCORES]={0};//,NUMBEROFCHECKERCORESPERCORE,NUMBEROFCHECKERCORESPERCORE*2};
        int loadstorelogentry::current_entry[]={0};
        int loadstorelogentry::current_size[]={0};

        uint64_t loadstorelogentry::timestamp[NUMBEROFMAINCORES]={1};//,1};
        int loadstorelogentry::timeout[NUMBEROFMAINCORES] = {TIMEOUT};//,TIMEOUT,TIMEOUT};
        uint64_t loadstorelogentry::committed_timestamp[]={0};
        bool loadstorelogentry::waiting_to_checkpoint[]={false};
                bool loadstorelogentry::earlyCommit[]={false};

        bool loadstorelogentry::ready[] = {false};
        uint64_t loadstorelogentry::should_cache_wait[NUMBEROFMAINCORES][NUMBEROFMAINCORES] = {{0}};

                uint64_t loadstorelogentry::meanTime=0;
        uint64_t loadstorelogentry::maxTime=0;
        uint64_t loadstorelogentry::minTime=(uint64_t)-1;
        uint64_t loadstorelogentry::times=0;

                 histoEntry loadstorelogentry::histoEntries[NUMBEROFHISTOENTRIES]={histoEntry()};
        uint64_t loadstorelogentry::maxHistoSize = 100000;

        std::vector<loadstorelogentry> loadstorelogentry::entries(logsize*NUMBEROFMAINCORES);
        std::vector<uint64_t> loadstorelogentry::timestamps(NUMBEROFCHECKERCORESPERCORE*NUMBEROFMAINCORES+NUMBEROFMAINCORES,1);
                std::vector<uint64_t> loadstorelogentry::committedInstructions(NUMBEROFCHECKERCORESPERCORE*NUMBEROFMAINCORES,0);



        std::vector<bool> loadstorelogentry::segmentFree(NUMBEROFCHECKERCORESPERCORE*NUMBEROFMAINCORES,true);
        std::vector<bool> loadstorelogentry::readyToCommit(NUMBEROFCHECKERCORESPERCORE*NUMBEROFMAINCORES,false);



std::vector<BaseCPU*> loadstorelogentry::baseCPUs(NUMBEROFCHECKERCORESPERCORE*NUMBEROFMAINCORES+NUMBEROFMAINCORES);
std::vector<int> loadstorelogentry::entryIndices(NUMBEROFCHECKERCORESPERCORE*NUMBEROFMAINCORES);
std::vector<int> loadstorelogentry::accessedThisRound(NUMBEROFCHECKERCORESPERCORE*NUMBEROFMAINCORES);
  miniContext loadstorelogentry::previousThreadContext[] = {miniContext()};


            using namespace TheISA;
        miniContext
m_serialize(ThreadContext &tc)
{


    miniContext m;

    for (int i = 0; i < NumFloatRegs; ++i)
        m.floatRegs[i] = tc.readFloatRegBitsFlat(i);

    for (int i = 0; i < NumIntRegs; ++i)
        m.intRegs[i] = tc.readIntRegFlat(i);

#ifdef ISA_HAS_CC_REGS
    for (int i = 0; i < NumCCRegs; ++i)
        m.ccRegs[i] = tc.readCCReg(i);
#endif

        for (int i=0; i< NumMiscRegs; ++i)
                m.miscRegs[i] = tc.readMiscRegNoEffect(i);


        // setMiscReg "with effect" will set the misc register mapping correctly.
    // e.g. updateRegMap(val)
    m.CPSR = tc.readMiscRegNoEffect(MISCREG_CPSR);


        m.pcState = tc.pcState();
        m.initialized = true;

        return m;
}

void m_copyRegs(ThreadContext* tc, miniContext m)
{


    for (int i = 0; i < NumFloatRegs; ++i)
        tc->setFloatRegFlat(i, m.floatRegs[i]);

    for (int i = 0; i < NumIntRegs; ++i)
        tc->setIntRegFlat(i, m.intRegs[i]);


    for (int i = 0; i < NumCCRegs; ++i)
        tc->setCCReg(i, m.ccRegs[i]);


        for (int i=0; i< NumMiscRegs; ++i)
                tc->setMiscRegNoEffect(i, m.miscRegs[i]);

                    tc->setMiscReg(MISCREG_CPSR, m.CPSR);

    tc->pcStateNoRecord(m.pcState);


    // Invalidate the tlb misc register cache
    tc->getITBPtr()->invalidateMiscReg();
    tc->getDTBPtr()->invalidateMiscReg();


    // thread_num and cpu_id are deterministic from the config
}


    void add_cpu(BaseCPU* cpu, int cpuID) {
        if (cpuID>=NUMBEROFCHECKERCORESPERCORE*NUMBEROFMAINCORES+NUMBEROFMAINCORES) return;

    loadstorelogentry::baseCPUs[cpuID] = cpu;
    std::cout << "got cpu " << cpuID << std::endl;
}

