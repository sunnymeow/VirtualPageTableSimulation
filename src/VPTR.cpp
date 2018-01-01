//
//  VPTR.cpp
//  VPTR
//
//  Created by Sunny Lin on 12/3/17.
//  Copyright © 2017 Sunny Lin. All rights reserved.
//

#include "VPTR.hpp"
#include <ctime>
#include <cstdlib>
#include <map>
#include <iostream>
#include <unistd.h>

using namespace std;

frame::frame()
{
    this->Fnum = -1;
    count = -1;
}

void frame::setFrame(int Fnum)
{
    this->Fnum = Fnum;
    count = 0;
}

void frame::setCount()
{   count++;   }

int frame::getFrame()
{    return Fnum;   }

int frame::getCount()
{    return count;  }

page::page()
{
    this->Pnum = -1;
    this->Snum = -1;
    this->valid = false;
    this->dirty = false;
    f = nullptr;
}

void page::setPage(int Pnum)
{
    this->Pnum = Pnum;
    this->Snum = (Pnum+1) * 100 - (rand()% 50);
    this->valid = false;
    this->dirty = false;
    f = nullptr;
}

void page::frameIn(frame* newF)
{
    f = newF;
    f->setCount();
    valid = true;
    dirty = true;
}

void page::frameOut()
{
    f = nullptr;
    valid = false;
}

frame* page::getFramePt()
{   return f;   }

int page::getPage()
{   return Pnum;    }

int page::getSector()
{   return Snum;    }

bool page::getValid()
{   return valid;   }

bool page::getDirty()
{   return dirty;   }

VPTR::VPTR()
{
    table = nullptr;
    frames = nullptr;
    pc = nullptr;
    psize = 0;
    fsize = 0;
}

void VPTR::setVPTR(int psize, int fsize)
{
    this->psize = psize;
    this->fsize = fsize;
    this->pageFault = 0;
    
    srand((unsigned)time(0));
    int random;
    
    // assign PC instructions
    map<int, int> check;
    for (int i = 0; i < psize; i++) {
        random = (rand() % PageSize) + PageSize * i;
//        random = rand() % (psize * PageSize);  // PC range
//        while (check.find(random) != check.end() || random/PageSize != i) {
//            random = rand() % (psize * PageSize);
//        }
        check.insert(pair<int, int>(i, random));
    }
    
    pc = new int [psize];
    for (int i = 0; i < psize; i++) {
        pc[i] = check[i];
    }
    
    // clear hash table
    check.clear();
    
    // allocated frames
    for (int i = 0; i < fsize; i++) {
        random = rand() % TOTframe;  // frame # range is from 0 - 63 (total 64 frames)
        while (check.find(random) != check.end()) {
            random = rand() % TOTframe;
        }
        check.insert(pair<int, int>(i, random));
    }
    
    frames = new frame*[fsize];
    for (int i = 0; i < fsize; i++) {
        frames[i] = new frame();
        frames[i]->setFrame(check[i]);
    }
    
    // initialize VPTR
    table = new page*[psize];
    for (int i = 0; i < psize; i++) {
        table[i] = new page();
        table[i]->setPage(pc[i]/PageSize);
    }
    
}

void VPTR::runVPTR()
{
    for (int i = 0; i < psize; i++) {
        // not frame and job not done
        if (table[i]->getValid()==false && table[i]->getDirty()==false) {
            frame* fault;
            fault = FIFO();
            
            if (fault->getCount() == 0) {
                sleep(2);
                cout << "*** page fault occur! => bring in a new page ***" << endl;
                pageFault++;
            }

            for (int i = 0; i < psize; i++) {
                if (table[i]->getFramePt() == fault) {
                    sleep(2);
                    cout << "*** page fault occur! => swap a using page ***" << endl;
                    pageFault++;
                    table[i]->frameOut();
                    display();
                }
            }
            
            table[i]->frameIn(fault);
            
            cout << "logical address:  " << pc[i] << endl;
            cout << "physical address: " <<
            table[i]->getFramePt()->getFrame()*PageSize + pc[i]%PageSize << endl;
            cout << "page number:      " << pc[i]/PageSize << endl;
            cout << "offset:           " << pc[i]%PageSize << endl;
            display();
        }
    }
}

frame* VPTR::FIFO()
{
    frame* temp;
    int min;
    
    temp = frames[0];
    min = frames[0]->getCount();
    for (int i = 0; i < fsize; i++) {
        if (frames[i]->getCount() < min) {
            temp = frames[i];
            min = frames[i]->getCount();
        }
    }
    return temp;
}

void VPTR::intro()
{
    cout << "------------ set up --------------" << endl;
    cout << "Virtual Memory size: " << VM << endl;
    cout << "RAM size:          : " << RAM << endl;
    cout << "page/frame size    : " << PageSize << endl;
    cout << "Allocated frame #  : ";
    
    for (int i = 0; i < fsize; i++) {
        cout << frames[i]->getFrame() << " ";
    }
    cout << "\n----------------------------------" << endl;
}

void VPTR::end()
{
    cout << "---------- Program finished ------------" << endl;
    cout << "When program needs " << psize << " pages but has " << fsize << " allocated frames," << endl;
    cout << "there are " << pageFault << " page faults in total." << endl;
    cout << "----------------------------------------" << endl << endl;
}

void VPTR::display()
{
    cout << "|---------------------------------------|" << endl;
    cout << "|p#\t|f#\t|sec#\t|v/i\t|dir\t|" << endl;
    cout << "|---------------------------------------|" << endl;
    for (int i = 0; i < psize; i++) {
        cout << "|" << table[i]->getPage() << "\t|";
        if (table[i]->getFramePt() == nullptr) {
            cout << "NA\t|";
        }
        else {
            cout << table[i]->getFramePt()->getFrame() << "\t|";
        }
        cout << table[i]->getSector() << "\t|"
        << ((table[i]->getValid())?("v  "):("i  ")) << "\t|"
        << ((table[i]->getDirty())?("yes"):("no ")) << "\t|" << endl;
    }
    
    cout << "|---------------------------------------|" << endl << endl;

}
