/*
 * Haoyu Xu
 * 2012.11.17
 */


#ifndef MAZEGEN_H
#define	MAZEGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string>


struct roomgen{
    struct roomgen* parent;
    int number;
};

struct wallgen{
    int number;
    struct roomgen* prev;
    struct roomgen* next;
    bool status;
};

unsigned int getByte(FILE *fp);
struct roomgen* Find(struct roomgen *x);
void Union(struct roomgen* x,struct roomgen* y);
void mazegen(FILE *fp,int weight, int height, bool info);

#endif	/* MAZEGEN_H */
