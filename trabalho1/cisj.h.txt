#ifndef __CISJ_H__
#define __CISJ_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POW_2(num) (1<<(num))
#define VALID_J(j, s) ((POW_2(s-1)) >= j)

typedef struct node_set {
	int* nodes;
	ssize_t size;
	ssize_t offset;
} node_set;

node_set* cis(int i, int s);

#endif