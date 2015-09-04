/*
 * LinkList.c
 *
 *  Created on: Sep 4, 2015
 *      Author: Jacken
 */

#include <stdio.h>
#include <stdlib.h>
#include "LinkList.h"

#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

struct test_list {
	struct list_head node;
	int* num;
};

int main(int argc, char *argv[])
{
	LIST_HEAD(head);
	struct test_list *tmp;
    struct list_head *iterator, *q;
    int i;

    for (i = 0 ; i < 5 ; i++) {
        tmp = malloc(sizeof(struct test_list));
        if (tmp == NULL) {
        	continue;
        }
        tmp->num = malloc(sizeof(int));
        if (tmp->num == NULL) {
        	continue;
        }
        *(tmp->num) = i;
//        list_add(&tmp->node, &head);
        AddNodeIntoList(&tmp->node, &head);
    }

    list_for_each(iterator, &head) {
        printf("%d\n", *(list_entry(iterator, struct test_list, node)->num));
    }

	list_for_each_safe(iterator, q, &head) {
		tmp = list_entry(iterator, struct test_list, node);
		printf("Free number: %d\n", *(tmp->num));
//		list_del(iterator);
		DelNodeFromList(iterator);
		free(tmp->num);
		free(tmp);
	}
	return 0;
}


#if 0	//Example
struct num {
     struct list_head node;
     int number;
};

int main(int argc, char *argv[])
{
	LIST_HEAD(head);
    struct num *tmp;
    struct list_head *iterator;
    int i;

    for (i=0 ; i<5 ; i++) {
        tmp = malloc(sizeof(struct num));
        tmp->number = i;
        list_add(&tmp->node, &head);
    }

    list_for_each(iterator, &head) {
        printf("%d\n", list_entry(iterator, struct num, node)->number);
    }
    return 0;
}
#endif

#if 0	//Example
struct kool_list {
	int to;
	struct list_head list;
	int from;
};

int main(int argc, char **argv)
{
	struct kool_list *tmp;
	struct list_head *pos, *q;
	unsigned int i;

	struct kool_list mylist;
	INIT_LIST_HEAD(&mylist.list); /*初始化链表头*/

	/* 给mylist增加元素 */
	for (i=5; i!=0; --i)	{
		tmp= (struct kool_list *)malloc(sizeof(struct kool_list));

		/* 或者INIT_LIST_HEAD(&tmp->list); */
		printf("enter to and from:");
		scanf("%d %d", &tmp->to, &tmp->from);

		list_add(&(tmp->list), &(mylist.list));
		/* 也可以用list_add_tail() 在表尾增加元素*/
	}
	printf("\n");

	printf("traversing the list using list_for_each()\n");
	list_for_each(pos, &mylist.list) {

		/* 在这里 pos->next 指向next 节点, pos->prev指向前一个节点.这里的节点是
			struct kool_list类型. 但是，我们需要访问节点本身，

			而不是节点中的list字段，宏list_entry()正是为此目的。*/

		tmp= list_entry(pos, struct kool_list, list);

		printf("to= %d from= %d\n", tmp->to, tmp->from);

	}
	printf("\n");
	/* 因为这是循环链表，也可以以相反的顺序遍历它，
	*为此，只需要用'list_for_each_prev'代替'list_for_each'，

	* 也可以调用list_for_each_entry() 对给定类型的节点进行遍历。
	* 例如:
	*/
	printf("traversing the list using list_for_each_entry()\n");
	list_for_each_entry(tmp, &mylist.list, list)
	printf("to= %d from= %d\n", tmp->to, tmp->from);
	printf("\n");

	/*现在，我们可以释放 kool_list节点了.我们本可以调用 list_del()删除节点元素，

	* 但为了避免遍历链表的过程中删除元素出错，因此调用另一个更加安全的宏 list_for_each_safe()，

	* 具体原因见后面的分析
	* */
	printf("deleting the list using list_for_each_safe()\n");
	list_for_each_safe(pos, q, &mylist.list) {
		tmp= list_entry(pos, struct kool_list, list);
		printf("freeing item to= %d from= %d\n", tmp->to, tmp->from);
		list_del(pos);
		free(tmp);
	}

	return 0;
}
#endif
