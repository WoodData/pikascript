/*
 * This file is part of the PikaScript project.
 * http://github.com/pikastech/pikascript
 *
 * MIT License
 *
 * Copyright (c) 2021 lyon 李昂 liang6516@outlook.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __PIKA__PARSER__H
#define __PIKA__PARSER__H
#include "PikaVM.h"
#include "dataQueueObj.h"
#include "dataStack.h"

typedef struct Asmer_t {
    char* asm_code;
    uint8_t block_deepth_now;
    uint8_t is_new_line;
    char* line_pointer;
} Asmer;

char* Parser_multiLineToAsm(Args* outBuffs, char* multiLine);
char* instructUnit_fromAsmLine(Args* outBuffs, char* pikaAsm);
char* Parser_byteCodeToAsm(Args* outBuffs, char* pikaByteCode);
ByteCodeFrame* byteCodeFrame_appendFromAsm(ByteCodeFrame* bf, char* pikaAsm);
int bytecodeFrame_fromMultiLine(ByteCodeFrame* bytecode_frame,
                                            char* multi_line);
void Parser_compilePyToBytecodeArray(char* lines);

#endif
