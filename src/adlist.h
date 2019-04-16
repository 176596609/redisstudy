/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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
#ifdef _WIN32
#include "Win32_Interop/Win32_Portability.h"
#endif

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */

typedef struct listNode {
    struct listNode *prev;//ǰ�ýڵ�
    struct listNode *next;//���ýڵ�
    void *value;//�ڵ�ֵ
} listNode;

typedef struct listIter {
    listNode *next;
    int direction;
} listIter;

typedef struct list {
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
    PORT_ULONG len;
} list;

/* Functions implemented as macros */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
list *listCreate(void);//����һ���������κνڵ�� o1
void listRelease(list *list);//�ͷ��б� on
list *listAddNodeHead(list *list, void *value);//�б�ͷ������һ���ڵ� o1
list *listAddNodeTail(list *list, void *value);//�б�β������һ���ڵ� o1
list *listInsertNode(list *list, listNode *old_node, void *value, int after);//���б��old_nodeǰ����ߺ������һ���ڵ� o1
void listDelNode(list *list, listNode *node); // ɾ��һ���ڵ�  o1
listIter *listGetIterator(list *list, int direction);//��ȡ����ĵ����� �ܼ򵥵�һ��ʵ��
listNode *listNext(listIter *iter);//������������ ���ݵ�����ǰ���ߺ�
void listReleaseIterator(listIter *iter);//���ٵ����� �򵥵�free
list *listDup(list *orig);//����һ������
listNode *listSearchKey(list *list, void *key);//����KEY O(N)
listNode *listIndex(list *list, PORT_LONG index);//��������indexλ��ֵ ����Ϊ����
void listRewind(list *list, listIter *li);//��ȡһ������ĵ�����
void listRewindTail(list *list, listIter *li);//��ȡһ������ĵ�����
void listRotate(list *list);//�������β�ڵ� ժ�����ŵ�ͷ�����

/* Directions for iterators */
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
