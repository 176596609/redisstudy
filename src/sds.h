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

struct sdshdr {//c语言的字符串在安全性 效率 以及功能方面无法满足redis的要求  兼容c语言字符串
    unsigned int len;//记录buf中已经使用的字节数量  获取长度O(1) c语言的话必须遍历O(N)  杜绝缓冲区溢出（api会保证字符串有足够的空间 避免溢出）
    unsigned int free;//记录未使用的字节数量
    char buf[];//二进制安全的 sdshdr不但可以保存字符串，还可以保存二进制数据，简单的读取客户端发来的任何信息都是保存在这个BUF里面
};

static inline size_t sdslen(const sds s) {//字符串的长度
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

static inline size_t sdsavail(const sds s) {//剩余的空间
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

sds sdsnewlen(const void *init, size_t initlen);//生成一个字符串 初始化内存为init 长度为initlen  注意:保存的内容不一定是字符串
sds sdsnew(const char *init);//初始化一个C字符串 初始化为字符串长度
sds sdsempty(void);//初始化一个空字符串
size_t sdslen(const sds s);//获取sds的长度
sds sdsdup(const sds s);//初始化一个sds字符串 从另一个sds字符串获取内容和长度
void sdsfree(sds s);//释放整个sds
size_t sdsavail(const sds s);//获取sds的可用空间 free
sds sdsgrowzero(sds s, size_t len);//将内存扩大到至少len  然后多余没有用的内存初始化为0
sds sdscatlen(sds s, const void *t, size_t len);////追加len长度的二进制数据到 sds字符串
sds sdscat(sds s, const char *t);//追加 C字符串t到sds字符串结尾
sds sdscatsds(sds s, const sds t);//追加sds字符串（二进制安全）到原来的sds上面
sds sdscpylen(sds s, const char *t, size_t len);//拷贝t 长度 为len的内存到sds上
sds sdscpy(sds s, const char *t);////拷贝t到sds 

sds sdscatvprintf(sds s, const char *fmt, va_list ap);
#ifdef __GNUC__
sds sdscatprintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sds sdscatprintf(sds s, const char *fmt, ...);
#endif

sds sdscatfmt(sds s, char const *fmt, ...);
sds sdstrim(sds s, const char *cset);//trim s，cset是一个char集合  这个和window里面的Trim一模一样
void sdsrange(sds s, int start, int end);
void sdsupdatelen(sds s);//这个函数看起来没啥蛋用 看上面的英文注释 这只是一个hack用法  事实上redis根本没有用过这个函数
void sdsclear(sds s);//清空一个sds字符串 
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
sds sdsMakeRoomFor(sds s, size_t addlen);//查看当前free空间是否大于addlen  不是的话就扩容  扩容的大小为 （len+addlen）*2 如果大于1M，那么每次额外扩容1M
void sdsIncrLen(sds s, int incr);//修改字符串的长度，但是不对BUF大小进行修改，注意英文注释里面以及redis的用法
sds sdsRemoveFreeSpace(sds s);//去掉空闲内存
size_t sdsAllocSize(sds s);//返回SDS对应sdshdr对象占用的所有空间大小

#endif
