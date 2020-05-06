#include "bp.h"

BPNN::BPNN(string trainpath,string verifypath,string testpath,Generator&generator):generator(generator){
    this->trainPath = trainpath;
    this->verifyPath = verifypath;
    this->testPath = testpath;
    //this->generator=generator;
}

int BPNN::argmax(vector<double> x){
    int res=0;
    double ans=0;
    for(int m=0;m<x.size();m++){
        if(x[m]>ans){
            res=m;
            ans=x[m];
        }
    }
    return res;
}

double BPNN::sigmoid(double x){
    return 1 / (1 + exp(-x));
}

void BPNN::build(double inNum,double hidNum,double outNum){
    this->inNum = inNum + 1;
    this->hidNum = hidNum + 1;
    this->outNum = outNum;
    this->outNum=generator.getOutputSize();
    cout<<"outNum:"<<this->outNum<<endl;

    int i,j,k;
    for(i=0;i<this->hidNum;i++){
        this->hidOutput.push_back(0.0);
        this->hidOutputError.push_back(0.0);
    }
    this->hidOutput[0] = 1.0;

    for(i=0;i<this->outNum;i++){
        this->outOutput.push_back(0.0);
        this->outOutputError.push_back(0.0);
    }
	
    for(i=0;i<this->inNum;i++){
        vector<double> temp;
        for(j=0;j<this->hidNum;j++){
            temp.push_back(0.0);
        }
        this->wIn2Hid.push_back(temp);
    }

    for(i=0;i<this->hidNum;i++){
        vector<double> temp;
        for(j=0;j<this->outNum;j++){
            temp.push_back((rand()%100/(double)101)/(10.0));
        }
        this->wHid2Out.push_back(temp);
    }

    //for(i=1;i<this->inNum;i++){
    //    this->inOutput.push_back(0.0);
    //}

}

void BPNN::feedForward(){
    int i,j,k;
    for(j=1;j<this->hidNum;j++){
        double sum=0.0;
        for(i=0;i<this->inNum;i++){
            sum += this->inOutput[i]*this->wIn2Hid[i][j];        
        }
        //cout<<this->sigmoid(sum)<<endl;
        this->hidOutput[j] = this->sigmoid(sum);
    }
    for(k=0;k<this->outNum;k++){
        double sum=0.0;
        for(j=0;j<this->hidNum;j++){
            sum += this->hidOutput[j]*this->wHid2Out[j][k];
        }
        //cout<<this->sigmoid(sum)<<endl;
        this->outOutput[k] = this->sigmoid(sum);
    }
}

void BPNN::backPropagate(double N){
   int i,j,k;
   for(k=0;k<this->outNum;k++){
       double error = this->targets[k]-this->outOutput[k];
       //cout<<error<<endl;
       //cout<<this->outOutputError[k]<<" "<<(1-this->outOutput[k])<<" "<<error<<endl;
       this->outOutputError[k] = this->outOutput[k] * (1-this->outOutput[k]) * error;
   }

   for(j=0;j<this->hidNum;j++){
       double error = 0.0;
       for(k=0;k<this->outNum;k++){
           error += this->outOutputError[k]*this->wHid2Out[j][k];
       }
       this->hidOutputError[j] = this->hidOutput[j]*(1-this->hidOutput[j])*error;
   }

   for(j=0;j<this->hidNum;j++){
       for(k=0;k<this->outNum;k++){
           //cout<<N<<' '<<this->outOutputError[k]<<' '<<this->hidOutput[j]<<endl;;
           double change=N*this->outOutputError[k]*this->hidOutput[j];
           //cout<<change<<endl;
           this->wHid2Out[j][k]+=change;
       }
   }

   for(i=0;i<this->inNum;i++){
       for(j=1;j<this->hidNum;j++){
           double change=N*this->hidOutputError[j]*this->inOutput[i];
           this->wIn2Hid[i][j] += change;
       }
   }
}

void BPNN::setGenerator(Generator&generator){
    this->generator=generator;
}

void BPNN::train(int loop){
    double best=0.0;
    int tc=0;
    for(int l=0;l<loop;l++){
        ifstream ifs(this->trainPath);
        string line;
        while(true){
            getline(ifs,line);
            if(ifs.eof())break;
            //cout<<"mmp"<<endl;
            this->targets=this->generator.getTarget(this->trainPath,line);
            //cout<<"mmp1"<<endl;
            this->inOutput=this->generator.getInput(this->trainPath,line);
            //cout<<"mmp2"<<endl;
            this->feedForward();
            this->backPropagate();
            //cout<<this->inOutput.size()<<endl;
            tc+=1;
            if(tc%60==0){
                //cout<<"start "<<tc/60<<"th verify"<<endl;
                double v=this->verify();
                if(v>best){
                    best=v;
                    this->bestWHid2Out.clear();
                    this->bestWIn2Hid.clear();
                    for(int p=0;p<this->wHid2Out.size();p++){
                        vector<double> temp;
                        for(int q=0;q<this->wHid2Out[p].size();q++){
                            temp.push_back(this->wHid2Out[p][q]);
                        }
                        this->bestWHid2Out.push_back(temp);
                    }
                    for(int p=0;p<this->wIn2Hid.size();p++){
                        vector<double> temp;
                        for(int q=0;q<this->wIn2Hid[p].size();q++){
                            temp.push_back(this->wIn2Hid[p][q]);
                        }
                        this->bestWIn2Hid.push_back(temp);
                    }
                }
                cout<<tc/60<<"th verify accuricy:"<<v<<";best accuricy:"<<best<<endl;
            }
        }
    ifs.close();
    }
    this->wHid2Out=this->bestWHid2Out;
    this->wIn2Hid=this->bestWIn2Hid;
}


void BPNN::test(){
    double right=0;
    double wrong=0;
    ifstream testSet(this->testPath);
    //cout<<"testPath:"<<this->verifyPath<<endl;
    if(!testSet.is_open()){
        cout<<"testSet not open"<<endl;
    }
    string line;
    while(true){
        getline(testSet,line);
        //cout<<"TestFile:"<<line<<endl;
        if(testSet.eof())break;

        this->inOutput=this->generator.getInput(this->testPath,line);
        
        int dire;/*
        if(line.find("_straight_")!=string::npos)dire=0;
        if(line.find("_right_")!=string::npos)dire=1;
        if(line.find("_left_")!=string::npos)dire=2;
        if(line.find("_up_")!=string::npos)dire=3;
        //else dire=0;
        size_t found=line.find_last_of("/");
        string fname=line.substr(found+1);
        ifstream ifs2("./test/"+fname+".data");
        if(!ifs2.is_open()){
                cout<<"open verify file failed:"<<fname<<endl;
            }
            double tmp;
            int pos=0;
            while(true){
                ifs2>>tmp;
                if(ifs2.eof())break;
                this->inOutput[pos]=tmp;
                pos++;
            }
            //cout<<this->inOutput.size()<<endl;
        ifs2.close();
        */
        this->feedForward();
        dire=argmax(this->generator.getTarget(this->testPath,line));
        int ans=argmax(this->outOutput);
        //cout<<"argmax"<<maxans<<" "<<argmax<<" "<<dire<<endl;
        if(ans == dire)right+=1;
        else wrong+=1;
        
    }
    //cout<<wrong<<" "<<right+wrong<<endl;
        double ans=1.0*right/(right+wrong);
        cout<<"Test Set Accuracy:"<<ans<<endl;
}


double BPNN::verify(){
    double right=0;
    double wrong=0;
    ifstream testSet(this->verifyPath);
    //cout<<"testPath:"<<this->verifyPath<<endl;
    if(!testSet.is_open()){
        cout<<"testSet not open"<<endl;
    }
    string line;
    while(true){
        getline(testSet,line);
        //cout<<"TestFile:"<<line<<endl;
        if(testSet.eof())break;

        this->inOutput=this->generator.getInput(this->verifyPath,line);
        
        int dire;/*

        */
        this->feedForward();
        dire=argmax(this->generator.getTarget(this->verifyPath,line));
        int ans=argmax(this->outOutput);
        //for(int m=0;m<this->outOutput.size();m++){
        //    cout<<this->outOutput[m]<<"\t";
        //}
        
        //cout<<"argmax:"<<ans<<" "<<" "<<dire<<endl;
        if(ans == dire)right+=1;
        else wrong+=1;
        
    }
    //cout<<wrong<<" "<<right+wrong<<endl;
        double ans=1.0*right/(right+wrong);
        return ans;
}

void BPNN::save(string network){
    cout<<"save to "<<network<<endl;
    ofstream ofs(network);
    ofs<<this->bestWHid2Out.size()<<" "<<this->bestWHid2Out[0].size()<<endl;
    for(int i=0;i<this->bestWHid2Out.size();i++){
        for(int j=0;j<this->bestWHid2Out[i].size();j++){
            ofs<<this->bestWHid2Out[i][j]<<" ";
        }
        ofs<<endl;
    }
    ofs<<this->bestWIn2Hid.size()<<" "<<this->bestWIn2Hid[0].size()<<endl;
    for(int i=0;i<this->bestWIn2Hid.size();i++){
        for(int j=0;j<this->bestWIn2Hid[i].size();j++){
            ofs<<this->bestWIn2Hid[i][j]<<" ";
        }
        ofs<<endl;
    }
    ofs.close();
}

void BPNN::load(string network){
    cout<<"load from "<<network<<endl;
    ifstream ifs(network);
    if(!ifs.is_open()){
        cerr<<"net file not found"<<endl;
        return;
    }
    int i,j;
    double temp;
    ifs>>i>>j;
    //cout<<i<<" "<<j<<endl;
    for(int m=0;m<i;m++){
        //vector<double> tmp;
        for(int n=0;n<j;n++){
            ifs>>temp;
            //cout<<temp<<endl;
            this->wHid2Out[m][n]=temp;
        }
        
    }
    ifs>>i>>j;
    for(int m=0;m<i;m++){
        //vector<double> tmp;
        for(int n=0;n<j;n++){
            ifs>>temp;
            this->wIn2Hid[m][n]=temp;
        }
    }
    cout<<"导入成功"<<endl;
}



