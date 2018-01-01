//
//  VPTR.hpp
//  VPTR
//
//  Created by Sunny Lin on 12/3/17.
//  Copyright © 2017 Sunny Lin. All rights reserved.
//

#ifndef VPTR_hpp
#define VPTR_hpp
#include <stdio.h>

const int VM = 16000;
const int RAM = 1024;
const int PageSize = 16;
const int TOTframe = RAM/PageSize;

class frame
{
private:
    int Fnum;   // frame number
    int count;  // counter for FIFO
public:
    frame();
    void setFrame(int Fnum);
    void setCount();
    int getFrame();
    int getCount();
    ~frame() {};
};

class page: public frame
{
private:
    int Pnum;   // page number
    int Snum;   // sector number
    bool valid; // V/I bit
    bool dirty; // dirty bit
    frame* f;
public:
    page();
    void setPage(int Pnum);
    void frameIn (frame* newF);
    void frameOut ();
    frame* getFramePt();
    int getPage();
    int getSector();
    bool getValid();
    bool getDirty();
    ~page() {};
};

class VPTR: public page
{
private:
    page** table;
    frame** frames;
    int* pc;
    int psize;
    int fsize;
    int pageFault;
public:
    VPTR();
    void setVPTR(int psize, int fsize);
    void runVPTR();
    frame* FIFO();
    void intro();
    void end();
    void display();
    ~VPTR() {};
};

#endif /* VPTR_hpp */
