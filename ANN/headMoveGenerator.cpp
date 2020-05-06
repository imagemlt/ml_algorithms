#include "headMoveGenerator.h"

headMoveGenerator::headMoveGenerator(){       //this->path=path;
        this->target=vector<double>(4);
        this->input=vector<double>(961);
}

int headMoveGenerator::getOutputSize(){
    return 4;
}

vector<double> headMoveGenerator::getInput(string path,string filename){
        string realfile;
        string subname;
        size_t pos=filename.find_last_of("/");
        //cout<<pos<<endl;
        //cout<<filename.length()<<endl;
        subname=filename.substr(pos+1)+".data";
        //cout<<subname<<endl;
        if(path.find("_train")!=string::npos){
            realfile="./train/"+subname;
        }
        else if(path.find("_test1")!=string::npos){
            realfile="./test/"+subname;
        }
        else if(path.find("_test2")!=string::npos){
            realfile="./verify/"+subname;
        }
        ifstream ifs(realfile);
        //cout<<realfile<<endl;
        double tmp;
        if(!ifs.is_open()){
            //cout<<"input err"<<endl;
            return this->input;
        }
        int position=0;
        while(true){
            ifs>>tmp;
            if(ifs.eof())break;
            this->input[position]=tmp;
            position++;
        }
        ifs.close();
        return this->input; 
    }

vector<double> headMoveGenerator::getTarget(string path,string filename){
    int dire;
            //cout<<line<<line.find("_straight_")<<" "<<line.find("_right_")<<" "<<line.find("_left_")<<" "<<line.find("_up_")<<endl;
    if(filename.find("_straight_")!=string::npos)dire=0;
    if(filename.find("_right_")!=string::npos)dire=1;
    if(filename.find("_left_")!=string::npos)dire=2;
    if(filename.find("_up_")!=string::npos)dire=3;
        //vector<double> targets;
    for(int m=0;m<4;m++){
        this->target[m]=0.1;
    }
    this->target[dire]=0.9;
    /*for(int m=0;m<4;m++){
        cout<<this->target[m]<<'\t';
    }*/
    //cout<<endl;
    return this->target;     
}
