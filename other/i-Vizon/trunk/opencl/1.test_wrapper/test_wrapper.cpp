/*
****************************************************************************
BSD License
Copyright (c) 2014, i-Vizon
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the i-Vizon.
4. Neither the name of the i-Vizon nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Mageswaran.D ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Mageswaran.D BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

****************************************************************************
*/
/*
* =============================================================================
*
*
*   FILENAME            : Buffer.h
*
*   DESCRIPTION         : A wrapper library for OpenCL and its native counter part
*                         intialization. With boost thread support.
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 1st Mar 2014
*
*   File Created.
*
* =============================================================================
*/
/************************Using CLWrapper Class
 *  All CL programs needs to do some device and kernel specific
 *  things again and again to run an even a simple matrix addition or
 *  reduction algorithm. So I decided to write a wrapper class to minimize
 *  the work done in non-logic part of the code. And the classes are
 *  1. CLSetup Class
 *  2. Program Class
 *  3. Memory & Image Classes
 *  4. KernelLauncher Class
 *
 *  Output:
 *      GlobalId.xGlobalId.y.LocalId.xLocalId.y ...
 *      .
 *      .
 *      .
 */
#include <cl_wrapper.hpp>

//Global Dimension
int GROWS = 16;
int GCOLS = 16;
//Local Dimension
int LROWS = 4;
int LCOLS = 4;
int main(int argc, char* argv[])
{
    if(argc < 2)
        argv[1] = "wrk_grp_info.cl";

    iv::CLSetup         cl;
    iv::Program*        prog;
    iv::Buffer*         buff1d;
    iv::KernelLauncher* kl;
    std::vector<std::string> kernelFiles;
    std::string  file=  "wrk_grp_info.cl";


    kernelFiles.push_back(file);
    //Test PRoject Variables
    float               res[GROWS * GCOLS];

    long maxGrpThreads;


    cl.init();

    //Test specific variables
    int numComputeUnits;
    int  maxGrpSize, localMemSize, preferredWrkSize;
    int globalWrkItems;
    int localWrkItems;
    numComputeUnits     = cl.getMaxComputeUnits();
    maxGrpSize          = cl.getMaxWorkGroupSize();
    localMemSize        = cl.getLocalMemSize();

    globalWrkItems = numComputeUnits * maxGrpSize;
    localWrkItems = maxGrpSize;

    std::cout<<"Number of Compute Units   : "<<numComputeUnits<<std::endl
             <<"Max Work Group Size       : "<<maxGrpSize<<std::endl
             <<"Local Memory Size         : "<<localMemSize/1024<<"k"<<std::endl;


    std::cout<<"Number of Platforms "<<cl.getNumberOfPlatforms()<<std::endl;

    cl.getDeviceName();

    std::cout<<"Number of Devices "<<cl.getNumberOfDevices()<<std::endl;
    std::cout<<"Number of Compute Units "<<cl.getMaxComputeUnits()<<std::endl;

    numComputeUnits = cl.getMaxComputeUnits();
    maxGrpThreads   = cl.getMaxWorkGroupSize();

    prog = cl.createProgram(kernelFiles);
    prog->buildProgram();


    buff1d = cl.createBuffer(GROWS * GCOLS * sizeof(float), CL_MEM_WRITE_ONLY, NULL);

    std::string kernelName = "id_check";
    kl = prog->createKernelLauncher(kernelName);
    kl->pGlobal(GROWS,GCOLS)->pLocal(LROWS,LCOLS);
    kl->pArg(buff1d->getMem());
    kl->run();

    buff1d->read(res, sizeof(float)* GROWS * GCOLS);

    std::cout<<"\n";
    for(int gj=0; gj<GROWS; gj++) {
        for(int gi=0; gi<GCOLS; gi++) {
            printf(" %6.2f", res[gj*GCOLS+gi]);
            if(((gi + 1) % LCOLS) == 0) std::cout<<"|";
        }
        if(((gj + 1) % LROWS) == 0) std::cout<<"\n";
        printf("\n");
    }

    std::cout<<"\n\nDone Testing OpenCL Wrappers!\n";
    return 0;
}

/**********************************OUTPUT
 ./test_wrapper
Number of Platforms 1
Number of Devices 1
Number of Compute Units 139852725092354
Kernel No :1 Name :id_check
   0.00 100.10| 200.00 300.10| 400.00 500.10| 600.00 700.10| 800.00 900.10| 1000.00 1100.10| 1200.00 1300.10| 1400.00 1500.10|
   1.01 101.11| 201.01 301.11| 401.01 501.11| 601.01 701.11| 801.01 901.11| 1001.01 1101.11| 1201.01 1301.11| 1401.01 1501.11|

   2.00 102.10| 202.00 302.10| 402.00 502.10| 602.00 702.10| 802.00 902.10| 1002.00 1102.10| 1202.00 1302.10| 1402.00 1502.10|
   3.01 103.11| 203.01 303.11| 403.01 503.11| 603.01 703.11| 803.01 903.11| 1003.01 1103.11| 1203.01 1303.11| 1403.01 1503.11|

   4.00 104.10| 204.00 304.10| 404.00 504.10| 604.00 704.10| 804.00 904.10| 1004.00 1104.10| 1204.00 1304.10| 1404.00 1504.10|
   5.01 105.11| 205.01 305.11| 405.01 505.11| 605.01 705.11| 805.01 905.11| 1005.01 1105.11| 1205.01 1305.11| 1405.01 1505.11|

   6.00 106.10| 206.00 306.10| 406.00 506.10| 606.00 706.10| 806.00 906.10| 1006.00 1106.10| 1206.00 1306.10| 1406.00 1506.10|
   7.01 107.11| 207.01 307.11| 407.01 507.11| 607.01 707.11| 807.01 907.11| 1007.01 1107.11| 1207.01 1307.11| 1407.01 1507.11|

   8.00 108.10| 208.00 308.10| 408.00 508.10| 608.00 708.10| 808.00 908.10| 1008.00 1108.10| 1208.00 1308.10| 1408.00 1508.10|
   9.01 109.11| 209.01 309.11| 409.01 509.11| 609.01 709.11| 809.01 909.11| 1009.01 1109.11| 1209.01 1309.11| 1409.01 1509.11|

  10.00 110.10| 210.00 310.10| 410.00 510.10| 610.00 710.10| 810.00 910.10| 1010.00 1110.10| 1210.00 1310.10| 1410.00 1510.10|
  11.01 111.11| 211.01 311.11| 411.01 511.11| 611.01 711.11| 811.01 911.11| 1011.01 1111.11| 1211.01 1311.11| 1411.01 1511.11|

  12.00 112.10| 212.00 312.10| 412.00 512.10| 612.00 712.10| 812.00 912.10| 1012.00 1112.10| 1212.00 1312.10| 1412.00 1512.10|
  13.01 113.11| 213.01 313.11| 413.01 513.11| 613.01 713.11| 813.01 913.11| 1013.01 1113.11| 1213.01 1313.11| 1413.01 1513.11|

  14.00 114.10| 214.00 314.10| 414.00 514.10| 614.00 714.10| 814.00 914.10| 1014.00 1114.10| 1214.00 1314.10| 1414.00 1514.10|
  15.01 115.11| 215.01 315.11| 415.01 515.11| 615.01 715.11| 815.01 915.11| 1015.01 1115.11| 1215.01 1315.11| 1415.01 1515.11|



Done Testing OpenCL Wrappers!

 */
