// SPDX-License-Identifier: Dual BSD/GPL
#ifndef HELLO1_H
#define HELLO1_H

#include <linux/ktime.h>
#include <linux/list.h>

struct hello_item {
	struct list_head list;
	ktime_t time;
};

int print_hello(uint count);
void cleanup_list(void);

#endif // HELLO1_H
