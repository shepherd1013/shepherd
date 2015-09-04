/*
 * LinkList.c
 *
 *  Created on: Sep 4, 2015
 *      Author: Jacken
 */
#include <stdio.h>
#include "List.h"

#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

void AddNodeIntoList(struct list_head *node, struct list_head *stList)
{
	list_add(node, stList);
}

void DelNodeFromList(struct list_head *node)
{
	list_del(node);
}
