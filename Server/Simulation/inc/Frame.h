#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;


class Frame {
public:
    DWORD id = 0x18DA60F9;
    int Da,Db,Dc,D00,D11,D22,D33,D44,D55,D66,D77,D88,D99,D100,D110,D120,D130,D140,D150,D160,D170,D180,D190,D200;
    int dlc;
    vector<BYTE> DataStore;
};