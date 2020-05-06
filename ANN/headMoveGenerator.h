#ifndef __HEADMOVE_GENERATOR_H__
#define __HEADMOVE_GENERATOR_H__

#include "Generator.h"

class headMoveGenerator:public Generator{
    public:
    headMoveGenerator();   
    virtual vector<double> getTarget(string path,string filename);
    virtual vector<double> getInput(string path,string filename);
    virtual int getOutputSize();
};

#endif