#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<bitset>
#include<sstream>
using namespace std;

int convert_bitset(bitset<32> a){
    int i = 0;
    int n = 0;
    for(i = 31;i >=0;--i){
        n = n*2 + a[i];
    }
    return n;
    //for printing as hexadecimal, use 
    //"cout << hex << n << dec;"
}
