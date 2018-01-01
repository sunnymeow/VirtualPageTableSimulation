//
//  main.cpp
//  VPTR
//
//  Created by Sunny Lin on 12/3/17.
//  Copyright © 2017 Sunny Lin. All rights reserved.
//

#include <iostream>
#include "VPTR.hpp"
using namespace std;

int main() {
    
    VPTR* mytable = new VPTR();
    int page, frame;
    char response;
    
    do {
        cout << "# of pages in your program? ";
        cin >> page;
        cout << "# of frames allocated for this program? ";
        cin >> frame;
        
        if (page <= 0 || frame <= 0) {
            cout << "Invalid page/frame number!! Please re-enter." << endl;
            cout << "# of pages in your program? ";
            cin >> page;
            cout << "# of frames allocated for this program? ";
            cin >> frame;
        }
        
        mytable->setVPTR(page, frame);
        mytable->intro();
        mytable->display();
        mytable->runVPTR();
        mytable->end();
        
        cout << "Do you want to continue? (Y/N) ";
        cin >> response;
        cout << endl;

    }while (response != 'n' && response != 'N');
    
    cout << "Have a good day :)" << endl << endl;
    return 0;
}
