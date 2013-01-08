/*
 * Haoyu Xu
 * 2012.11.17
 */


#include "mazesolve.h"

using namespace std;


//check the height and weight of the maze
void checkMaze(FILE *fp,int* h, int* w){
    int rowcount=0;
    int heightcount=0;
    char buffer[100];
    while(fgets(buffer,sizeof(buffer),fp)!=NULL){
        rowcount = (strlen(buffer)-1)/2;
        heightcount++;
    }
    *w = rowcount;
    *h = (heightcount-1)/2;
    rewind(fp);
}

//construct the maze so that room and wall are right
void setupMaze(FILE *fp,int h, int w){
    int roomNum = h*w;
    //inialized rooms
    room* roomArray[roomNum];
    for(int i=0;i<roomNum;i++){
        roomArray[i] = new room();
        roomArray[i]->roomid = i;
        roomArray[i]->level = -1;
        roomArray[i]->roomr = NULL;
        roomArray[i]->rooml = NULL;
        roomArray[i]->roomt = NULL;
        roomArray[i]->roomb = NULL;
        roomArray[i]->parent = NULL;
        roomArray[i]->pass = false;
        roomArray[i]->cost = 100000;
    }
    //inialized walls
    int wallNum = 2*h*w-h-w;
    wall* wallArray[wallNum];
    for(int i=0;i<wallNum;i++){
        wallArray[i] = new wall();
        wallArray[i]->wallid = i;
        wallArray[i]->cost = -1;
        wallArray[i]->pass = false;
        wallArray[i]->status = false;
    }

    int heightcount=0;
    char buffer[133];
    // declear maze's entry info
    int startendNum=0;
        room* entry[2];
        for(int j=0;j<2;j++){
           entry[j] = new room();
        }
    room* start = new room();
    room* end = new room();

    while(fgets(buffer,sizeof(buffer),fp)!=NULL){
        //verital wall
        if(heightcount % 2 != 0){
            for(int i=0;i<2*w+1;i+=2){
             //check everyline to see if the room are connected
                //entry if there is left and right entry
                if(i==0 && buffer[i] != '|' ){
                    entry[startendNum++] = roomArray[heightcount/2*w];
                }
                else if(i==2*w && buffer[i] !='|' ){
                    entry[startendNum++] = roomArray[(heightcount+1)/2*w-1];
                }
                //check normal room
                else if(buffer[i] != '|' ){
                   roomArray[(heightcount-1)/2*w+i/2-1]->roomr = roomArray[(heightcount-1)/2*w+i/2];
                   roomArray[(heightcount-1)/2*w+i/2]->rooml = roomArray[(heightcount-1)/2*w+i/2-1];

                }
            }
            //construct vertical walls parameters
            for(int j=2;j<2*w;j+=2){
                    if(buffer[j] != '|'){
                    wallArray[(heightcount/2)*(2*w-1)+j/2-1]->cost = buffer[j]-'0';
                    wallArray[(heightcount/2)*(2*w-1)+j/2-1]->status = true;
                    }
            }

        }

        //horizontal wall
        else if(heightcount % 2 == 0 && heightcount!= 0 && heightcount!= 2*h){
            for(int i=1;i<2*w;i+=2){
                if(buffer[i] != '-'){
                    roomArray[(heightcount/2-1)*w+i/2]->roomb = roomArray[(heightcount/2)*w+i/2];
                    roomArray[(heightcount/2)*w+i/2]->roomt = roomArray[(heightcount/2-1)*w+i/2];
                }
            }
            //construct horizontal walls parameters
             for(int j=1;j<2*w;j+=2){
                    if(buffer[j] != '-'){
                    wallArray[(w-1)+(2*w-1)*(heightcount/2-1)+j/2]->cost = buffer[j]-'0';
                    wallArray[(w-1)+(2*w-1)*(heightcount/2-1)+j/2]->status = true;
                    }
            }

        }

        //find the start room and end room if it's the last or first row
        if(heightcount == 0){
            for(int i=1;i<2*w;i+=2){
                if(buffer[i] !='-'){
                    entry[startendNum++] = roomArray[i/2];
                }
            }
        }
        if(heightcount == 2*h){
            for(int i=1;i<2*w;i+=2){
                if(buffer[i] !='-'){
                    entry[startendNum++] = roomArray[(heightcount/2-1)*w+i/2];
                }
            }
        }
        heightcount++;
    }

    //check to see which one is the entrance
        int e_row0 = entry[0]->roomid / w;
        int e_row1 = entry[1]->roomid / w;
        int e_col0 = entry[0]->roomid % w;
        int e_col1 = entry[1]->roomid % w;
        if (e_row0 != e_row1){
            if (e_row0 < e_row1){
                start = entry[0];
                end = entry[1];
            } else {
                start = entry[1];
                end = entry[0];
            }
            } else {
                if (e_col0 < e_col1){
                    start = entry[0];
                    end = entry[1];
                } else {
            start = entry[1];
            end = entry[0];
            }
        }

    // see if is the type 1 maze or type 2 maze
    bool ismazeType1 = false;
    bool ismazeType2 = false;
    for(int i=0;i<wallNum;i++){
        if(wallArray[i]->cost > 0){
            ismazeType2 = true;
            break;
        }else ismazeType2 = false;
    }
    if(ismazeType2 == false){
        ismazeType1 = true;
    }

    //solve if it's type one maze, use breadth-first-search
    int maxlevel=0;
    if(ismazeType1 == true){
        int currentlevel = 0;

        roomArray[start->roomid]->level = 0;
        roomArray[start->roomid]->cost = 0;
        for(int j=0;j<roomNum;j++){
        for(int i=0;i<roomNum;i++){
            //check left, right, top, bottom
            if(roomArray[i]->rooml != NULL && roomArray[i]->rooml->level == -1 && roomArray[i]->level == currentlevel){
                roomArray[i]->rooml->level = currentlevel + 1;
                roomArray[i]->rooml->parent  = roomArray[i];
                roomArray[i]->rooml->cost = roomArray[i]->cost + 1;

            }

            if(roomArray[i]->roomr != NULL && roomArray[i]->roomr->level == -1 && roomArray[i]->level == currentlevel){
                roomArray[i]->roomr->level = currentlevel + 1;
                roomArray[i]->roomr->parent  = roomArray[i];
                roomArray[i]->roomr->cost = roomArray[i]->cost + 1;

            }

            if(roomArray[i]->roomt != NULL && roomArray[i]->roomt->level == -1 && roomArray[i]->level == currentlevel){
                roomArray[i]->roomt->level = currentlevel + 1;
                roomArray[i]->roomt->parent  = roomArray[i];
                roomArray[i]->roomt->cost = roomArray[i]->cost + 1;

            }

            if(roomArray[i]->roomb != NULL && roomArray[i]->roomb->level == -1 && roomArray[i]->level == currentlevel){
                roomArray[i]->roomb->level = currentlevel + 1;
                roomArray[i]->roomb->parent  = roomArray[i];
                roomArray[i]->roomb->cost = roomArray[i]->cost + 1;


            }

        }
            currentlevel++;
        }

        for(int i=0;i<roomNum;i++){
            if(roomArray[i]->level>maxlevel){
                maxlevel = roomArray[i]->level;
            }
        }

        //set the wall status except the entrance
        int i = end->roomid;
        while( roomArray[i]->parent !=NULL ){
            int j = roomArray[i]->roomid;
            int row = j / w ;
            int column = j % w ;
            roomArray[j]->pass = true;
            //if wall is left, set it to be pass
            if(roomArray[j]->parent == roomArray[j]->rooml){
                int left = (2*w-1)*row+column-1;
                wallArray[left]->pass = true;
            }
            else if(roomArray[j]->parent == roomArray[j]->roomr){
                int right = (2*w-1)*row+column;
                wallArray[right]->pass = true;
            }
            else if(roomArray[j]->parent == roomArray[j]->roomt){
                int top = (w-1) + (2*w-1)*(row-1) + column;
                wallArray[top]->pass = true;
            }
            else if(roomArray[j]->parent == roomArray[j]->roomb){
                int bottom = (w-1) + (2*w-1)*(row) + column;
                wallArray[bottom]->pass = true;
            }

            roomArray[i] = roomArray[i]->parent;
        }

            roomArray[end->roomid]->pass = true;


    } //end bsf if

    //solve type 2 maze
    if(ismazeType2 == true){

        roomArray[start->roomid]->cost = 0;
        for(int j=0;j<roomNum;j++){
           for(int i=0;i<roomNum;i++){
                int row = i / w ;
                int column = i % w ;

               //check the left room's cost

                if(roomArray[i]->rooml != NULL && roomArray[i]->rooml->cost != 100000){
                    int wallLeft = (2*w-1)*row+column-1;
                    if( (wallArray[wallLeft]->cost + roomArray[i-1]->cost) < roomArray[i]->cost ){
                        roomArray[i]->cost = wallArray[wallLeft]->cost + roomArray[i-1]->cost;
                        roomArray[i]->parent = roomArray[i-1];
                    }
                }

                //check the right room's cost
                else if(roomArray[i]->roomr != NULL && roomArray[i]->roomr->cost != 100000){
                    int wallRight = (2*w-1)*row+column;
                    if( (wallArray[wallRight]->cost + roomArray[i+1]->cost) < roomArray[i]->cost ){
                        roomArray[i]->cost = wallArray[wallRight]->cost + roomArray[i+1]->cost;
                        roomArray[i]->parent = roomArray[i+1];
                    }
                }

                //check the top room's cost
                else if(roomArray[i]->roomt != NULL && roomArray[i]->roomt->cost != 100000){
                    int wallTop = (w-1) + (2*w-1)*(row-1) + column;
                    if( (wallArray[wallTop]->cost + roomArray[i-w]->cost) < roomArray[i]->cost ){
                        roomArray[i]->cost = wallArray[wallTop]->cost + roomArray[i-w]->cost;
                        roomArray[i]->parent = roomArray[i-w];
                    }
                }

                                //check the bottom room's cost
                else if(roomArray[i]->roomb != NULL && roomArray[i]->roomb->cost != 100000){
                    int wallBottom = (w-1) + (2*w-1)*(row) + column;
                    if( (wallArray[wallBottom]->cost + roomArray[i+w]->cost) < roomArray[i]->cost ){
                        roomArray[i]->cost = wallArray[wallBottom]->cost + roomArray[i+w]->cost;
                        roomArray[i]->parent = roomArray[i+w];
                    }
                }


            }
        }

        //traverse the pass, set room and wall status

        int i = end->roomid;
        while( roomArray[i]->parent !=NULL ){
            int j = roomArray[i]->roomid;
            int row = j / w ;
            int column = j % w ;
            roomArray[j]->pass = true;
            //if wall is left, set it to be pass
            if(roomArray[j]->parent == roomArray[j]->rooml){
                int left = (2*w-1)*row+column-1;
                wallArray[left]->pass = true;
            }
            else if(roomArray[j]->parent == roomArray[j]->roomr){
                int right = (2*w-1)*row+column;
                wallArray[right]->pass = true;
            }
            else if(roomArray[j]->parent == roomArray[j]->roomt){
                int top = (w-1) + (2*w-1)*(row-1) + column;
                wallArray[top]->pass = true;
            }
            else if(roomArray[j]->parent == roomArray[j]->roomb){
                int bottom = (w-1) + (2*w-1)*(row) + column;
                wallArray[bottom]->pass = true;
            }

            roomArray[i] = roomArray[i]->parent;
        }

        roomArray[i]->pass = true;


    } //end for Dij if


    //start to printout the solution of the maze

        //print first line
        int startRow = start->roomid / w;
        int startColumn = start->roomid % w;
        int endRow = end->roomid / w;
        int endColumn = end->roomid % w;
        //print first column
        // if room start at top

        printf("+");
        for(int i=0;i<w;i++){
            if(i == startColumn && startRow == 0){
                printf("@+");
            }
            else if(i == endColumn && endRow == 0){
                printf("@+");
            }else printf("-+");
        }
        printf("\n");


        int rowcount = 0;
        for(int j=0;j<2*h-1;j++){

            //print odd row
            if(rowcount % 2 == 0){
                for(int i=0;i<2*w+1;i++){
                    if(i == 0){
                    if(startColumn == 0 && startRow == j/2 && startRow != 0){
                        printf("@");
                    }else if(endColumn == 0 && endRow == j/2){
                        printf("@");
                    }else printf("|");
                    }

                    else if (i == 2*w){
                    if(startColumn == w && startRow == j/2){
                        printf("@");
                    }else if(endColumn == w && endRow == j/2){
                        printf("@");
                    }else printf("|");

                    }


                    else if( i%2 != 0){
                        if(roomArray[(rowcount/2)*w+i/2]->pass == true){
                            printf("@");
                        }else printf(" ");
                    }else if( i%2 == 0){
                        if(wallArray[(rowcount/2)*(2*w-1)+i/2-1]->pass == true && wallArray[(rowcount/2)*(2*w-1)+i/2-1]->status == true){
                            printf("@");
                        }else if(wallArray[(rowcount/2)*(2*w-1)+i/2-1]->status == true && wallArray[(rowcount/2)*(2*w-1)+i/2-1]->pass == false){
                            printf(" ");
                        }else printf("|");
                    }
                }
            }


            //print even row
            else if(rowcount % 2 == 1){
                for(int i=0;i<2*w+1;i++){
                    if( i%2 == 0){
                        printf("+");
                    }
                    else if( i%2 != 0 ){
                        if(wallArray[(w-1)+(rowcount/2)*(2*w-1)+i/2]->pass == true && wallArray[(w-1)+(rowcount/2)*(2*w-1)+i/2]->status == true){
                            printf("@");
                        }else if(wallArray[(w-1)+(rowcount/2)*(2*w-1)+i/2]->status == true && wallArray[(w-1)+(rowcount/2)*(2*w-1)+i/2]->pass == false){
                            printf(" ");
                        }else printf("-");
                    }
                }
            }
            printf("\n");
            rowcount++;


        } //end while printing middle row

        //print last row
        if(startRow == h-1 || endRow == h-1){
        printf("+");
        for(int i=0;i<w;i++){
            if(i == startColumn && startRow == h-1){
                printf("@+");
            }
            else if(i == endColumn && endRow == h-1){
                printf("@+");
            }else printf("-+");
        }
        printf("\n");
        }


        if(ismazeType1 == true){

            printf("Starts in room at row = %d and column = %d.\n",start->roomid/w + 1, start->roomid % w + 1);
            printf("Ends in room at row = %d and column = %d.\n",end->roomid/w + 1, end->roomid % w + 1);
            printf("Cost of shortest path = %d.\n",end->cost);
            printf("Maximum level = %d.\n",maxlevel);

        }

        if(ismazeType2 == true){
            printf("Starts in room at row = %d and column = %d.\n",start->roomid/w + 1, start->roomid % w + 1);
            printf("Ends in room at row = %d and column = %d.\n",end->roomid/w + 1, end->roomid % w + 1);
            printf("Cost of shortest path = %d.\n",end->cost);

        }



}

void mazeSolve(FILE *fp){
    int h = 0;
    int w = 0;
    checkMaze(fp,&h,&w);
    setupMaze(fp,h,w);

}
