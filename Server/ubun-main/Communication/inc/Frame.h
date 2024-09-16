#pragma once

#include <iostream>
#include <windows.h>
#include <vector>

class Frame {
public:
   

    
    DWORD id;
    int Da;
    int Db;
    int Dc;
    int D00;
    int D11;
    int D22;
    int D33;
    int D44;
    int D55;
    int D66;
    int D77;
    int D88;
    int D99;
    int D100;
    int D110;
    int D120;
    int D130;
    int D140;
    int D150;
    int D160;
    int D170;
    int D180;
    int D190;
    int D200;
    int dlc;
    std::vector<BYTE> DataStore;
};
