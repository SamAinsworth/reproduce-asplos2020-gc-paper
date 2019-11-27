/*
 * Copyright (c) 2009 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Ali Saidi
 */

#include <cstdio>
#include <string>

#include "cpu/thread_context.hh"
#include "debug/SyscallVerbose.hh"
#include "kern/linux/linux.hh"
#include "sim/process.hh"
#include "sim/system.hh"

int
Linux::openSpecialFile(std::string path, LiveProcess *process,
                       ThreadContext *tc)
{
    DPRINTF(SyscallVerbose, "Opening special file: %s\n", path.c_str());

    bool matched = false;
    std::string data;

    if (path.compare(0, 13, "/proc/meminfo") == 0) {
        data = Linux::procMeminfo(process, tc);
        matched = true;
    } else if (path.compare(0, 11, "/etc/passwd") == 0) {
        data = Linux::etcPasswd(process, tc);
        matched = true;
    } else if (path.compare(0, 15, "/proc/self/maps") == 0) {
        data = Linux::mapsInfo(process, tc);
        matched = true;
    }

    if (matched) {
        FILE *f = tmpfile();
        int fd = fileno(f);
        size_t ret M5_VAR_USED = fwrite(data.c_str(), 1, data.size(), f);
        assert(ret == data.size());
        rewind(f);
        return fd;
    } else {
        warn("Attempting to open special file: %s. Ignoring. Simulation may "
             "take un-expected code path or be non-deterministic until proper "
             "handling is implemented.\n", path.c_str());
        errno = EACCES;
        return -1;
    }
}

std::string
Linux::procMeminfo(LiveProcess *process, ThreadContext *tc)
{
    return csprintf("MemTotal:%12d kB\nMemFree: %12d kB\n",
            process->system->memSize() >> 10,
            process->system->freeMemSize() >> 10);
}

std::string
Linux::etcPasswd(LiveProcess *process, ThreadContext *tc)
{
    return csprintf("gem5-user:x:1000:1000:gem5-user,,,:%s:/bin/bash\n",
                    process->getcwd());
}


std::string
Linux::mapsInfo(LiveProcess *process, ThreadContext *tc)
{
return csprintf("00400000-0040c000 r-xp 00000000 08:02 448045                             /bin/cat\n\
0060b000-0060c000 r--p 0000b000 08:02 448045                             /bin/cat\n\
0060c000-0060d000 rw-p 0000c000 08:02 448045                             /bin/cat\n\
01a45000-01a66000 rw-p 00000000 00:00 0                                  [heap]\n\
7f03d4bce000-7f03d4ea6000 r--p 00000000 08:02 134853                     /usr/lib/locale/locale-archive\n\
7f03d4ea6000-7f03d5066000 r-xp 00000000 08:02 146594                     /lib/x86_64-linux-gnu/libc-2.23.so\n\
7f03d5066000-7f03d5266000 ---p 001c0000 08:02 146594                     /lib/x86_64-linux-gnu/libc-2.23.so\n\
7f03d5266000-7f03d526a000 r--p 001c0000 08:02 146594                     /lib/x86_64-linux-gnu/libc-2.23.so\n\
7f03d526a000-7f03d526c000 rw-p 001c4000 08:02 146594                     /lib/x86_64-linux-gnu/libc-2.23.so\n\
7f03d526c000-7f03d5270000 rw-p 00000000 00:00 0 \n\
7f03d5270000-7f03d5296000 r-xp 00000000 08:02 146575                     /lib/x86_64-linux-gnu/ld-2.23.so\n\
7f03d5464000-7f03d5467000 rw-p 00000000 00:00 0 \n\
7f03d5473000-7f03d5495000 rw-p 00000000 00:00 0 \n\
7f03d5495000-7f03d5496000 r--p 00025000 08:02 146575                     /lib/x86_64-linux-gnu/ld-2.23.so\n\
7f03d5496000-7f03d5497000 rw-p 00026000 08:02 146575                     /lib/x86_64-linux-gnu/ld-2.23.so\n\
7f03d5497000-7f03d5498000 rw-p 00000000 00:00 0 \n\
7ffeb8bf3000-7ffeb8c14000 rw-p 00000000 00:00 0                          [stack]\n\
7ffeb8cbe000-7ffeb8cc1000 r--p 00000000 00:00 0                          [vvar]\n\
7ffeb8cc1000-7ffeb8cc3000 r-xp 00000000 00:00 0                          [vdso]\n\
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]");

}
