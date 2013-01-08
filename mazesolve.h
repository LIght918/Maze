/*
 * Haoyu Xu
 * 2012.11.17
 */



#ifndef MAZESOLVE_H
#define	MAZESOLVE_H

#include <iostream>
#include <stdio.h>
#include <string.h>


void checkMaze(FILE *fp,int* h, int* w);
void setupMaze(FILE *fp,int h, int w);
void mazeSolve(FILE *fp);

struct room{
    int roomid;
    struct room* rooml;
    struct room* roomr;
    struct room* roomt;
    struct room* roomb;
    struct room* parent;
    int level;
    bool pass;
    int cost;
};

struct wall{
    int wallid;
    int cost;
    bool pass;
    bool status;
};



#endif	/* MAZESOLVE_H */


