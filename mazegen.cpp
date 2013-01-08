/*
 * Haoyu Xu
 * 2012.11.17
 */

#include "mazegen.h"

unsigned int getByte(FILE *fp){
    unsigned int num;
    unsigned char buffer[4];
    fread(buffer,1,4,fp);
        num = (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + (buffer[3]);
        return num;
}

struct roomgen* Find(struct roomgen *x){
    if (x->parent == x){
        return x;
    }
    else return Find(x->parent);
}

void Union(struct roomgen* x,struct roomgen* y){
    struct roomgen* xRoot, *yRoot;
    xRoot = Find(x);
    yRoot = Find(y);
    xRoot->parent = yRoot;
}


void mazegen(FILE *fp,int weight, int height, bool info){
        int w=weight;
        int h=height;
        int roomNum = w*h;
        roomgen* roomArray[roomNum];
        for(int i=0;i<w*h;i++){
            roomArray[i] = new roomgen();
            roomArray[i]->number = i;
            roomArray[i]->parent = roomArray[i];
        }
        //construct the struct for wall
        int wallNum = 2*h*w-w-h;
        wallgen* wallArray[wallNum];
        for(int i=0;i<wallNum;i++){
            wallArray[i] = new wallgen();
            wallArray[i]->number = i;
            wallArray[i]->status = false;
        //assign adjency room to walls
        int j;
        int roomPrev,roomNext;
        j = i % (2*w-1);
        if(j<=w-2){
      //    printf("%d is vertical wall\n",i);
            roomPrev = i-(i/(2*w-1))*(w-1);
            roomNext = i-(i/(2*w-1))*(w-1)+1;
            wallArray[i]->prev = roomArray[roomPrev];
            wallArray[i]->next = roomArray[roomNext];
       //   printf("%d is the previous room\n",roomArray[roomPrev]->number);
       //   printf("%d is the next room\n\n",roomArray[roomNext]->number);

        }
        else{
       //   printf("%d is horizontal wall\n",i);
            roomPrev = i-(i/(2*w-1)+1)*(w-1);
            roomNext = i-(i/(2*w-1)+1)*(w-1)+w;
            wallArray[i]->prev = roomArray[roomPrev];
            wallArray[i]->next = roomArray[roomNext];
       //   printf("%d is the previous room\n",roomArray[roomPrev]->number);
       //   printf("%d is the next room\n\n",roomArray[roomNext]->number);
        }
        }

    //start to remove walls
        int deleteList[2*w*h-w-h];

        for(int i=0;i<2*w*h-w-h;i++){
            deleteList[i] = i;
        }

        int listLen = 2*w*h-w-h;
        int result; //result show which wall to remove
        int count = 0; //see whether it reaches w*h-1
        while(count<(w*h-1)){
        result = getByte(fp) % listLen;

        if(Find(wallArray[deleteList[result]]->prev)!=Find(wallArray[deleteList[result]]->next)){
        Union(wallArray[deleteList[result]]->prev,wallArray[deleteList[result]]->next);
    //  printf("result:%d\n",result);
        if(info == true){
            printf("Wall %d removed.\n",deleteList[result]);
        }
        wallArray[deleteList[result]]->status = true;
        for(int i=result;i<listLen;i++){
            deleteList[i] = deleteList[i+1];
        }

    /*    for(int i=0;i<listLen-1;i++){
            printf("%d\t",deleteList[i]);
        }
        printf("\n");  */
        listLen--;
        count++;
        }else if (Find(wallArray[deleteList[result]]->prev)==Find(wallArray[deleteList[result]]->next)){
    //      printf("result:%d",result);
    //      printf("Room %d and room %d cannot be merged.\n",wallArray[deleteList[result]]->prev->number,wallArray[deleteList[result]]->next->number);
            for(int i=result;i<listLen;i++){
            deleteList[i] = deleteList[i+1];
        }
     /*   printf("listLen is :%d\n",listLen);
        for(int i=0;i<listLen-1;i++){
            printf("%d\t",deleteList[i]);
        }
        printf("\n");  */
        listLen--;

        }
        }

     /*   for(int i=0;i<2*w*h-w-h;i++){
            printf("the number of wall %d is %d\n",i,wallArray[i]->status);
        }
            printf("\n");  */

        //print the maze
        //print the first row
        printf("+");
        for(int i=0;i<w;i++){
            if(i == 0){
                printf(" +");
            }else printf("-+");

        }
        printf("\n");
        //print the rest except the final row
        for(int i=0;i<2*h-1;i++){
            //print room row
            if(i % 2 == 0){
                printf("| ");
                for(int j=0;j<w-1;j++){
                    if(wallArray[j+i/2*(2*w-1)]->status == 1){
                        printf("  ");
                    }else printf("| ");
                }
                printf("|");
                printf("\n");
            }
            else if(i % 2 != 0){
                printf("+");
                for(int k=0;k<w;k++){
                    if(wallArray[w-1+i/2*(2*w-1)+k]->status == 1){
                        printf(" +");
                    }else printf("-+");
                }
                printf("\n");
            }
        }

        //print the bottom
        for(int i=0;i<w;i++){
            if(i == w-1){
                printf("+ ");
            }else printf("+-");
        }
        printf("+");
        printf("\n");
}
