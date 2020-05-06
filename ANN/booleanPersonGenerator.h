#ifndef __BOOLEAN_PERSON_GENERATOR_H__
#define __BOOLEAN_PERSON_GENERATOR_H__

#include "Generator.h"


class booleanPersonGenerator:public Generator{
    public:
    booleanPersonGenerator();
    virtual vector<double> getTarget(string path,string filename);
    virtual vector<double> getInput(string path,string filename);
    virtual int getOutputSize();
};

#endif