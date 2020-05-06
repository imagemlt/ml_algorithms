#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include<iostream>
#include<vector>
#include<fstream>
#include<string>

using namespace std;

class Generator{
    public:
    virtual vector<double>getTarget(string path,string filename) = 0;
    virtual vector<double>getInput(string path,string filename) = 0;
    virtual int getOutputSize() = 0;
    protected:
    vector<double>target;
    vector<double>input;
};

#endif