/* SDSLib, A C dynamic strings library
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
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

#ifndef __SDS_H
#define __SDS_H

#define SDS_MAX_PREALLOC (1024*1024)

#ifdef _WIN32
#include "Win32_Interop/Win32_Portability.h"
#include "Win32_Interop/win32_types.h"
#endif
#include <sys/types.h>
#include <stdarg.h>

typedef char *sds;

struct sdshdr {//c���Ե��ַ����ڰ�ȫ�� Ч�� �Լ����ܷ����޷�����redis��Ҫ��  ����c�����ַ���
    unsigned int len;//��¼buf���Ѿ�ʹ�õ��ֽ�����  ��ȡ����O(1) c���ԵĻ��������O(N)  �ž������������api�ᱣ֤�ַ������㹻�Ŀռ� ���������
    unsigned int free;//��¼δʹ�õ��ֽ�����
    char buf[];//�����ư�ȫ�� sdshdr�������Ա����ַ����������Ա�����������ݣ��򵥵Ķ�ȡ�ͻ��˷������κ���Ϣ���Ǳ��������BUF����
};

static inline size_t sdslen(const sds s) {//�ַ����ĳ���
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

static inline size_t sdsavail(const sds s) {//ʣ��Ŀռ�
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

sds sdsnewlen(const void *init, size_t initlen);//����һ���ַ��� ��ʼ���ڴ�Ϊinit ����Ϊinitlen  ע��:��������ݲ�һ�����ַ���
sds sdsnew(const char *init);//��ʼ��һ��C�ַ��� ��ʼ��Ϊ�ַ�������
sds sdsempty(void);//��ʼ��һ�����ַ���
size_t sdslen(const sds s);//��ȡsds�ĳ���
sds sdsdup(const sds s);//��ʼ��һ��sds�ַ��� ����һ��sds�ַ�����ȡ���ݺͳ���
void sdsfree(sds s);//�ͷ�����sds
size_t sdsavail(const sds s);//��ȡsds�Ŀ��ÿռ� free
sds sdsgrowzero(sds s, size_t len);//���ڴ���������len  Ȼ�����û���õ��ڴ��ʼ��Ϊ0
sds sdscatlen(sds s, const void *t, size_t len);////׷��len���ȵĶ��������ݵ� sds�ַ���
sds sdscat(sds s, const char *t);//׷�� C�ַ���t��sds�ַ�����β
sds sdscatsds(sds s, const sds t);//׷��sds�ַ����������ư�ȫ����ԭ����sds����
sds sdscpylen(sds s, const char *t, size_t len);//����t ���� Ϊlen���ڴ浽sds��
sds sdscpy(sds s, const char *t);////����t��sds 

sds sdscatvprintf(sds s, const char *fmt, va_list ap);
#ifdef __GNUC__
sds sdscatprintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sds sdscatprintf(sds s, const char *fmt, ...);
#endif

sds sdscatfmt(sds s, char const *fmt, ...);
sds sdstrim(sds s, const char *cset);//trim s��cset��һ��char����  �����window�����Trimһģһ��
void sdsrange(sds s, int start, int end);
void sdsupdatelen(sds s);//�������������ûɶ���� �������Ӣ��ע�� ��ֻ��һ��hack�÷�  ��ʵ��redis����û���ù��������
void sdsclear(sds s);//���һ��sds�ַ��� 
int sdscmp(const sds s1, const sds s2);
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);
void sdsfreesplitres(sds *tokens, int count);
void sdstolower(sds s);
void sdstoupper(sds s);
sds sdsfromlonglong(PORT_LONGLONG value);
sds sdscatrepr(sds s, const char *p, size_t len);
sds *sdssplitargs(const char *line, int *argc);
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);
sds sdsjoin(char **argv, int argc, char *sep);

/* Low level functions exposed to the user API */
sds sdsMakeRoomFor(sds s, size_t addlen);//�鿴��ǰfree�ռ��Ƿ����addlen  ���ǵĻ�������  ���ݵĴ�СΪ ��len+addlen��*2 �������1M����ôÿ�ζ�������1M
void sdsIncrLen(sds s, int incr);//�޸��ַ����ĳ��ȣ����ǲ���BUF��С�����޸ģ�ע��Ӣ��ע�������Լ�redis���÷�
sds sdsRemoveFreeSpace(sds s);//ȥ�������ڴ�
size_t sdsAllocSize(sds s);//����SDS��Ӧsdshdr����ռ�õ����пռ��С

#endif
