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
*   FILENAME            : string_search.cpp
*
*   DESCRIPTION         : To demonstrate the reduction alogorithm with a string
*                         search. OpenCLInAction Chapter 11
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE
*   File Created        : 17th Mar 2014
*
* =============================================================================
*/
#include <cl_wrapper.hpp>
#include <file_operation.h>

int main()
{
    //CLWrapper Variables
    iv::CLSetup         cl;
    iv::Program*        pProgram;
    iv::KernelLauncher* pKl;
    size_t globalWrkItems;
    size_t localWrkItems;
    std::vector<std::string>  kernelFiles;
    std::string file = "string_search.cl";
    kernelFiles.push_back(file);

    //Test specific variables
    size_t numComputeUnits;
    size_t  maxGrpSize, localMemSize, preferredWrkSize;

    cl.init();

    numComputeUnits     = cl.getMaxComputeUnits();
    maxGrpSize          = cl.getMaxWorkGroupSize();
    localMemSize        = cl.getLocalMemSize();

    globalWrkItems = numComputeUnits * maxGrpSize;
    localWrkItems = maxGrpSize;

    std::cout<<"Number of Compute Units   : "<<numComputeUnits<<std::endl
             <<"Max Work Group Size       : "<<maxGrpSize<<std::endl
             <<"Local Memory Size         : "<<localMemSize/1024<<"k"<<std::endl;

//=========================================================================
    //    Argument 2
    std::string textFile("string_search.txt");
    char *textBuffer = NULL;
    iv::FileIO charFile;
    size_t textSize;
    charFile.readCharFile(textFile, textBuffer, textSize);
    iv::Buffer* dTextBuffer;
    dTextBuffer = cl.createBuffer(textSize, CL_MEM_READ_ONLY |CL_MEM_COPY_HOST_PTR, textBuffer);

    std::cout<<"\nText file size :" << textSize <<std::endl;
    //Argument 1
    /// @TIPS:  C++ taken the last array data as NULL not in C
    char pattern[16] = "thatwithhavefro";
    /// @TIPS: Hack to insert 16th letter in char array in C++
    pattern[15]='m';

    //Argument 3
    int charPerItem = (textSize / globalWrkItems) + 1;
    std::cout<<"Characters per item :" << charPerItem <<std::endl;

    //Argument 4
    int result[4] = {0, 0, 0, 0};
    iv::Buffer* dResult = cl.createBuffer(sizeof(result), CL_MEM_READ_WRITE  | CL_MEM_COPY_HOST_PTR, result);

//=========================================================================
    pProgram = cl.createProgram(kernelFiles);
    pProgram->buildProgram();

    std::string kernelName = "string_search";
    pKl      = pProgram->createKernelLauncher(kernelName);


    pKl->pGlobal(globalWrkItems)->pLocal(localWrkItems);
    pKl->pArg(pattern)
       ->pArg(dTextBuffer->getMem())
       ->pArg(charPerItem)
       ->pArg(dResult->getMem());

    pKl->run();

    dResult->read(result, sizeof(result));
    printf("\nResults: \n");
    printf("Number of occurrences of 'that': %d\n", result[0]);
    printf("Number of occurrences of 'with': %d\n", result[1]);
    printf("Number of occurrences of 'have': %d\n", result[2]);
    printf("Number of occurrences of 'from': %d\n", result[3]);

    return 0;
}
