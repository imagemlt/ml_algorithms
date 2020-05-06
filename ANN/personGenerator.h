#ifndef __PERSON_GENERATOR_H__
#define __PERSON_GENERATOR_H__

#include "Generator.h"

static string faces[20]={
            "an2i",
            "at33",
            "boland",
            "bpm",
            "ch4f",
            "cheyer",
            "choon",
            "danieln",
            "glickman",
            "karyadi",
            "kawamura",
            "kk49",
            "megak",
            "mitchell",
            "night",
            "phoebe",
            "saavik",
            "steffi",
            "sz24",
            "tammo"
        };

class personGenerator:public Generator{
    public:
    personGenerator();
    virtual vector<double> getTarget(string path,string filename);
    virtual vector<double> getInput(string path,string filename);
    virtual int getOutputSize();
};

#endif