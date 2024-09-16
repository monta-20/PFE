#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;


class canMessage {
public:
    DWORD id;
    vector<BYTE> data;
    int dlc;
};