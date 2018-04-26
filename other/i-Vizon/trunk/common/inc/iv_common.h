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
*   FILENAME            : C.h
*
*   DESCRIPTION         : Common system header files
*
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 22th Feb 2014
*
*   File Created.
*
* =============================================================================
*/
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <iostream>
#include <vector>

///!TODO: Use bits as enum value
typedef enum {
    IV_CHAR_FILE_OPEN_FAIL = 0,
    IV_CHAR_FILE_STATUS_FAIL
}IV_ERRORS_INFO;

const char* getCLErrorString(int err);
const char* getCustomErrorString(int err, IV_ERRORS_INFO info);
std::string getImgType(int imgTypeInt);


#define DEBUG_CL(err) \
    if(err< 0) { \
    std::cout<<"FILE: "<<__FILE__<<std::cout<<" Line Number: "<<__LINE__\
    <<" Function Name : "<<__func__<<"Error Name:" \
    <<getCLErrorString(err) \
    <<std::endl; \
    exit(err); }

#define DEBUG_IV(err, info) \
    if(err< 0) { \
    std::cout<<"Line Number: "<<__LINE__<<" Function Name : "<<__func__<<"Error Name:" \
    <<getCustomErrorString(err, info) \
    <<std::endl; \
    exit(err); }

#define DEBUG_VALUE(dbgMsg, value)    \
    std::cout<<"=====>"<<"   " <<dbgMsg<<"......."<<value<<std::endl;

#define DEBUG_STRING(dbgMsg)    \
    std::cout<<">>>>>>"<<dbgMsg<<std::endl;

#define ERROR_PRINT_VALUE(dbgMsg, value)    \
    { \
    std::cout<<"\n=====> Line Number: "<<__LINE__<<" Function Name :"<<__func__\
    <<"\n "<<dbgMsg<<" "<<value<<std::endl; \
    exit(0); \
    };

#define ERROR_PRINT_STRING(dbgMsg)    \
    {   \
    std::cout<<"\n=====> Line Number: "<<__LINE__<<" Function Name: "<<__func__\
    <<"\n "<<dbgMsg<<std::endl; \
    exit(0); \
    };

#ifdef IVIZON_DEBUG
    #define F_LOG LogBlock _l(__func__)
        struct LogBlock {
            const char *mLine;
            LogBlock(const char *line) : mLine(line) {
                std::cout<<mLine <<"  ----->#### Enter \n";
            }
            ~LogBlock() {
                std::cout<<mLine <<" <-----#### Leave \n";
            }
        };
#else
    #define F_LOG {}
    #define DEBUG_CL(err) {}
    #define DEBUG_STRING(dbgMsg) {}
    #define DEBUG_VALUE(dbgMsg, value) {}
#endif



// CV Defines
#define CV_8U   0
#define CV_8S   1
#define CV_16U  2
#define CV_16S  3
#define CV_32S  4
#define CV_32F  5
#define CV_64F  6

#define CV_CN_MAX     512

#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)

#define CV_CN_SHIFT   3
#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)
#define CV_DEPTH_MAX  (1 << CV_CN_SHIFT)

#define CV_MAKETYPE(depth,cn) (CV_MAT_DEPTH(depth) + (((cn)-1) << CV_CN_SHIFT))
#define CV_MAKE_TYPE CV_MAKETYPE

#define CV_8UC1 CV_MAKETYPE(CV_8U,1)
#define CV_8UC2 CV_MAKETYPE(CV_8U,2)
#define CV_8UC3 CV_MAKETYPE(CV_8U,3)
#define CV_8UC4 CV_MAKETYPE(CV_8U,4)
#define CV_8UC(n) CV_MAKETYPE(CV_8U,(n))

#define CV_8SC1 CV_MAKETYPE(CV_8S,1)
#define CV_8SC2 CV_MAKETYPE(CV_8S,2)
#define CV_8SC3 CV_MAKETYPE(CV_8S,3)
#define CV_8SC4 CV_MAKETYPE(CV_8S,4)
#define CV_8SC(n) CV_MAKETYPE(CV_8S,(n))

#define CV_16UC1 CV_MAKETYPE(CV_16U,1)
#define CV_16UC2 CV_MAKETYPE(CV_16U,2)
#define CV_16UC3 CV_MAKETYPE(CV_16U,3)
#define CV_16UC4 CV_MAKETYPE(CV_16U,4)
#define CV_16UC(n) CV_MAKETYPE(CV_16U,(n))

#define CV_16SC1 CV_MAKETYPE(CV_16S,1)
#define CV_16SC2 CV_MAKETYPE(CV_16S,2)
#define CV_16SC3 CV_MAKETYPE(CV_16S,3)
#define CV_16SC4 CV_MAKETYPE(CV_16S,4)
#define CV_16SC(n) CV_MAKETYPE(CV_16S,(n))

#define CV_32SC1 CV_MAKETYPE(CV_32S,1)
#define CV_32SC2 CV_MAKETYPE(CV_32S,2)
#define CV_32SC3 CV_MAKETYPE(CV_32S,3)
#define CV_32SC4 CV_MAKETYPE(CV_32S,4)
#define CV_32SC(n) CV_MAKETYPE(CV_32S,(n))

#define CV_32FC1 CV_MAKETYPE(CV_32F,1)
#define CV_32FC2 CV_MAKETYPE(CV_32F,2)
#define CV_32FC3 CV_MAKETYPE(CV_32F,3)
#define CV_32FC4 CV_MAKETYPE(CV_32F,4)
#define CV_32FC(n) CV_MAKETYPE(CV_32F,(n))

#define CV_64FC1 CV_MAKETYPE(CV_64F,1)
#define CV_64FC2 CV_MAKETYPE(CV_64F,2)
#define CV_64FC3 CV_MAKETYPE(CV_64F,3)
#define CV_64FC4 CV_MAKETYPE(CV_64F,4)
#define CV_64FC(n) CV_MAKETYPE(CV_64F,(n))


/// IV Data Types
//This is done to have more control on memory and range on numbers

typedef unsigned char   IV_8U;
typedef char            IV_8S;
typedef unsigned        IV_16U;
typedef signed          IV_16S;
typedef int             IV_32S;
typedef unsigned int    IV_32U;
typedef float           IV_32F;
typedef double          IV_64F;
#endif
