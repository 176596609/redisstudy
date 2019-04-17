/*
 * Copyright (c) 2009-2012, Pieter Noordhuis <pcnoordhuis at gmail dot com>
 * Copyright (c) 2009-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __INTSET_H
#define __INTSET_H
#include <stdint.h>

typedef struct intset {//���ϵ������� ������ʡ�ڴ� Ȼ��������ģ����ҵ�ʱ����ֲ��ұȼӿ졣 ���Ǵ����ǲ���ɾ���ȽϷѾ�
    uint32_t encoding;
    uint32_t length;
    int8_t contents[];
} intset;

intset *intsetNew(void);//����һ������set
intset *intsetAdd(intset *is, int64_t value, uint8_t *success);//����������һ��ֵ
intset *intsetRemove(intset *is, int64_t value, int *success);//������ɾ��һ��ֵ
uint8_t intsetFind(intset *is, int64_t value);//����Ԫ��  ��Ϊ���Ҷ��Ƕ��� ����ʱ�临�Ӷȶ��� O(N)
int64_t intsetRandom(intset *is);//��ȡ�����е�һ�����ֵ
uint8_t intsetGet(intset *is, uint32_t pos, int64_t *value);//��ȡ������һ���̶�λ�õ�ֵ
uint32_t intsetLen(intset *is);//��ȡ���ϵĳ���
size_t intsetBlobLen(intset *is);//��ȡ���ϵ������ṹ���content�Ĵ�С

#endif // __INTSET_H
