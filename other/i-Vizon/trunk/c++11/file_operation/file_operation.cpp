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
*   FILENAME            : file_operation.cpp
*
*   DESCRIPTION         : Wrapper for all file I/O
*
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE
*   File Created        : 22th Mar 2014
*
* =============================================================================
*/
#include "file_operation.h"

void iv::FileIO::readCharFile(std::string &filePath, char* &bufferPtr, size_t &fileSize)
{
    struct stat statusBuffer;
    _fileHandle = open(filePath.c_str(), O_RDONLY);
    if(_fileHandle == -1)
        DEBUG_IV(-1, (IV_ERRORS_INFO)IV_CHAR_FILE_OPEN_FAIL);

    if(fstat(_fileHandle, &statusBuffer) != 0 || (!S_ISREG(statusBuffer.st_mode)))
    {
        DEBUG_IV(-1, (IV_ERRORS_INFO)IV_CHAR_FILE_OPEN_FAIL);
    }

    fileSize = statusBuffer.st_size;

    bufferPtr = (char*)malloc(fileSize);
    read(_fileHandle, bufferPtr, fileSize);
    close(_fileHandle);

    //Alternative method
    /* Read text file and place content into buffer */
//    text_handle = fopen(TEXT_FILE, "r");
//    if(text_handle == NULL) {
//       perror("Couldn't find the text file");
//       exit(1);
//    }
//    fseek(text_handle, 0, SEEK_END);
//    text_size = ftell(text_handle)-1;
//    rewind(text_handle);
//    text = (char*)calloc(text_size, sizeof(char));
//    fread(text, sizeof(char), text_size, text_handle);
//    fclose(text_handle);

}
