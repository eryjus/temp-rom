//===================================================================================================================
//  temp-rom.cc -- Source to build the binaries which get loaded on the temporary EEPROM
//
//        Copyright (c)  2022 -- Adam Clark
//        Licensed under "THE BEER-WARE LICENSE"
//        See License.md for details.
//
//  Since all memory access will be 16-bits at a time on the 16-Bit Computer, the best way to accomplish this is
//  with a 16-bit memory read.  This could be 2 sequential memory reads, but more efficient is to perform 2 parallel
//  memory reads, the most significant byte and the least significant byte at the same time.
//
//  Putting 2 EEPROM side-by-side, both reading the same address, returns 16-bits.  This is the basis for the 128K
//  memory in my 16-bit computer.
//
//  Now, for development purposes, I need something to read.  I could go with uninitialized RAM and have random bits
//  set, but that would not work well for debugging as the data would not be deterministic.  Instead, I need to be
//  able to set defined patterns on the ROM which can allow me to determine if the memory reads are working
//  properly.  Thus the purpose of this program.
//
//  This program opens up (rewrites) 2 binary files and outputs a pattern to each ROM.  The overall size of both
//  ROMs needs to be 32K (half the total addressable space, but to align with the EEPROM I have on-hand).
//
//  The patterns I chose to write to these ROM binaries are ascending/descending bit shifts (sometimes referred to
//  as a *Knight-Rider Effect*) and a repeating count from `0` to `255` (`0x00` to `0xff`); one pattern for each
//  ROM.
//
//  Note: This is not elegant code.  This is intended to be more of a brute-force approach.  Don't judge me.
//
//  -----------------------------------------------------------------------------------------------------------------
//
//     Date      Tracker  Version  Pgmr  Description
//  -----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2023-Nov-28  Initial  v1.0.0   ADCL  Initial version
//
//===================================================================================================================


#include <cstdio>
#include <cstdlib>


int main(void)
{
    FILE *msbRom = fopen("msb-rom.bin", "wb");
    FILE *lsbRom = fopen("lsb-rom.bin", "wb");

    if (!msbRom || !lsbRom) {
        fprintf(stderr, "Unable to open output files\n");
        return EXIT_FAILURE;
    }

    // -- loop through all 32K; let the compiler handle the math
    for (int i = 0; i < (32 * 1024); i ++) {
        switch (i % 16) {
        case 0:
        case 15:
            fputc(0x01, msbRom);
            break;

        case 1:
        case 14:
            fputc(0x02, msbRom);
            break;

        case 2:
        case 13:
            fputc(0x04, msbRom);
            break;

        case 3:
        case 12:
            fputc(0x08, msbRom);
            break;

        case 4:
        case 11:
            fputc(0x10, msbRom);
            break;

        case 5:
        case 10:
            fputc(0x20, msbRom);
            break;

        case 6:
        case 9:
            fputc(0x40, msbRom);
            break;

        case 7:
        case 8:
            fputc(0x80, msbRom);
            break;
        }

        fputc(i % 0x100, lsbRom);
    }

    fclose(msbRom);
    fclose(lsbRom);

    return EXIT_SUCCESS;
}




