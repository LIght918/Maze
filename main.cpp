#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <math.h>
#include "mazesolve.h"
#include "mazegen.h"

using namespace std;
// check if the file exists and can be opened

struct pos{
    int data;
    string position;
};

/*
FILE* isvalidFile(char* FileName){
    struct stat info;
    // If reading the input file fails...
    if (lstat(FileName, &info) != 0) {
        // If the input file does not exist...
        if (errno == ENOENT || errno == ENOTDIR) {
            cerr << "\tERROR: Input file "
                    << FileName << " does not exist!" << endl;
            exit(1);
        }
        // If the input file's parent directory cannot be accessed...
        else if (errno == EACCES) {
            cerr << "\tERROR: Input file "
                    << FileName
                    << " cannot be opened - access denies!" << endl;
            exit(1);
        }
    }

    FILE* fp;
    fp = fopen(FileName, "rb");
    if (fp == NULL) {
        cerr << "Input file" << FileName << " cannot be opened - access denies!" << endl;
        exit(1);
    }
    return fp;
}
*/



bool ifisNumber(char* str){
    bool isNumber = true;
    for(unsigned int i=0;i<strlen(str);i++){
        int j = str[i]-'0';
        if(j<0 || j>9){
            isNumber = false;
        }
    }
    return isNumber;
}

bool ifisPos(char* str){
    bool pos = false;
    if (strstr(str, "-top") != NULL)
        pos = true;
    if (strstr(str, "-bottom") != NULL)
        pos = true;
    if (strstr(str, "-left") != NULL)
        pos = true;
    if (strstr(str, "-right") != NULL)
        pos = true;
    return pos;
}

int stringtoNum(char* str){
    int num = 0;
    for(unsigned int i=0;i<strlen(str);i++){
        num = num*10 + (str[i]-'0');
    }
    return num;
}

int main(int argc, char** argv)
{
    argc--,argv++;

    if(strcmp(*argv,"mazegen")==0){
        int w,h;
        bool info = false;
        argc--,argv++;

        //check to see if the weight and height is the right number
        if(argc<2){
            cerr << "Malformed command!" << endl;
            exit(1);
        }else if(ifisNumber(*argv)==true){
            w = stringtoNum(*argv);
            if (w < 1 || w > 64){
            cerr << "\tInvalid weight!" << endl;
            exit(1);
            }
            argc--,argv++;
        }else{
             cerr << "Malformed command. Input valid weight." << endl;
             exit(1);
        }

        if(ifisNumber(*argv)==true){
            h = stringtoNum(*argv);
            if (h < 1 || h > 64){
            cerr << "\tInvalid height!" << endl;
            exit(1);
            }
            argc--,argv++;
        }else{
             cerr << "Malformed command. Input valid height." << endl;
             exit(1);
        }

        if(strcmp(*argv,"-info")==0){
            info = true;
            argc--,argv++;
        }

        if (argc > 4){
            cerr << "Malformed command. Too many argument." << endl;
            exit(1);
        }

        // if no option is specified
        if (argc == 0){
            mazegen(stdin,w, h,info);
        }

        else{
            //setup position


            FILE *fp;
                //fp = isvalidFile(*argv);
                fp = fopen("small.bin","rb");
                mazegen(fp,w,h,info);
                fclose(fp);


        }
    }
/*
    else if(strcmp(*argv,"mazesolve") == 0){
        argc--,argv++;
        if (argc > 1){
            cerr << "Too many argument." << endl;
        }

        //if no file is specified, then read from stdin...

        //if file is specified...
        else {
            FILE *fp;
            fp = isvalidFile(*argv);
            mazeSolve(fp);
            fclose(fp);
        }
    }
    */



}
