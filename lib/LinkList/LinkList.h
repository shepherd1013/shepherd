/*
 * LinkList.h
 *
 *  Created on: Sep 4, 2015
 *      Author: root
 */

#ifndef LINKLIST_H_
#define LINKLIST_H_
#include "List.h"

#ifdef __cplusplus
extern "C" {
#endif


void AddNodeIntoList(struct list_head *node, struct list_head *stList);
void DelNodeFromList(struct list_head *node);

#ifdef __cplusplus
}
#endif

#endif /* LINKLIST_H_ */
