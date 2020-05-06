#ifndef __SUNGLASS_GENERATOR_H__
#define __SUNGLASS_GENERATOR_H__

#include "Generator.h"

class sunglassGenerator:public Generator{
    public:
    sunglassGenerator();
    virtual vector<double> getTarget(string path,string filename);
    virtual vector<double> getInput(string path,string filename);
    virtual int getOutputSize();
};

#endif