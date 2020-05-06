#ifndef __MOOD_GENERATOR_H__
#define __MOOD_GENERATOR_H__

#include "Generator.h"

static string moods[20]={
            "happy",
            "sad",
            "angry",
            "neutral",
        };

class moodGenerator:public Generator{
    public:
    moodGenerator();
    virtual vector<double> getTarget(string path,string filename);
    virtual vector<double> getInput(string path,string filename);
    virtual int getOutputSize();
};

#endif