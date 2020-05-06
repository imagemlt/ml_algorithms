#ifndef __BP_H__
#define __BP_H__

#include<cmath>
#include<time.h>
#include<sstream>
#include<signal.h>
#include "headMoveGenerator.h"
#include "sunglassGenerator.h"
#include "personGenerator.h"
#include "moodGenerator.h"
//#include "booleanPersonGenerator.h"

class BPNN{
    public:
    BPNN(string trainpath,string verifypath,string testpath,Generator&generator);
    double sigmoid(double x);
    void build(double inNum,double hidNum,double outNum);
    void feedForward();
    void backPropagate(double N=0.3);
    void train(int loop=500);
    double verify();
    void test();
    void save(string network);
    void load(string network);
    void setGenerator(Generator&generator);
    private:
    Generator&generator;
    string trainPath;
    string verifyPath;
    string testPath;
    double inNum;
    double hidNum;
    double outNum;
    vector<double> hidOutput;
    vector<double> outOutput;
    vector<double> hidOutputError;// = [0.0]*self.hidNum
	vector<double> outOutputError;// = [0.0]*self.outNum
    vector<double> inOutput;
    
		// weights matrix
	vector<vector<double> >	wIn2Hid ;//= [[0.0]*self.hidNum for i in range(self.inNum)]
	vector<vector<double> >	wHid2Out ;//= [[random()/10]*self.outNum for j in range(self.hidNum)]
    vector<double> targets;
    vector<vector<double> > bestWIn2Hid;
    vector<vector<double> > bestWHid2Out;
    int argmax(vector<double> vect);
};

void ctrl_handler(int a);

#endif