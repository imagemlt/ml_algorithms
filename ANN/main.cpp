#include "bp.h"

BPNN* bp=NULL;
string network;
bool save;

void ctrl_handler(int a){
    if(bp!=NULL && save){
        bp->save(network);
    }
    exit(0);
}

void help(string argv0){
    cout<<"usage: "<<argv0<<endl;
    cout<<"-m 1|2 // mode, input 1 for training,2 for testing"<<endl;
    cout<<"-g 0|1|2|3 // game mode, 0 for headMove, 1 for sunglass, 2 for person, 3 for mood"<<endl;
    cout<<"-T <trainng path>"<<endl;
    cout<<"-t <test path>"<<endl;
    cout<<"-v <verify path>"<<endl;
    cout<<"[-n <network file>]"<<endl;
    cout<<"[-s // whether to save network]"<<endl;
    cout<<"[-h <hidden layer count>]"<<endl;
    cout<<"[-l <loop time>]"<<endl;
    cout<<"[-H // print this help]"<<endl;
}

int main(int argc,char *argv[]){
    string trainPath;
    string verifyPath;
    string testPath;
    int mode;
    int game=0;
    int loop=500;
    int begin=0;
    int layer=4;

    signal(SIGINT,ctrl_handler);
    while(true){
        if(!string("-H").compare(argv[begin])){
            help(argv[0]);
            return 0;
        }
        if(!string("-h").compare(argv[begin])){
            if(begin>=argc-1){
                cerr<<"incorrect layer count"<<endl;
            }
            stringstream ss(argv[begin+1]);
            ss>>layer;
            begin=begin+2;
            cout<<"hid layer count:"<<layer<<endl;
        }
        else if(!string("-g").compare(argv[begin])){
            if(begin>=argc-1){
                cerr<<"incorrect game Mode"<<endl;
            }
            stringstream ss(argv[begin+1]);
            ss>>game;
            begin=begin+2;
            cout<<"gamemode:"<<game<<endl;
        }
        else if(!string("-T").compare(argv[begin])){
            if(begin>=argc-1){
                cerr<<"incorrect trainPath"<<endl;
                return -1;
            }
            trainPath=argv[begin+1];
            begin=begin+2;
        }
        else if(!string("-v").compare(argv[begin])){
            if(begin>=argc-1){
                cerr<<"incorrect verifyPath"<<endl;
                return -1;
            }
            verifyPath=argv[begin+1];
            begin=begin+2;
        }
        else if(!string("-t").compare(argv[begin])){
            if(begin>=argc-1){
                cerr<<"incorrect testPath"<<endl;
                return -1;
            }
            testPath=argv[begin+1];
            begin=begin+2;
        }
        else if(!string("-m").compare(argv[begin])){
            if(begin>=argc-1){
                cerr<<"incorrect mode"<<endl;
                return -1;
            }
            string temp=argv[begin+1];
            stringstream st(temp);
            st>>mode;
            begin=begin+2;
        }
        else if(!string("-n").compare(argv[begin])){
            if(begin>=argc-1){
                cerr<<"incorrect network"<<endl;
                return -1;
            }
            network=argv[begin+1];
            begin=begin+2;
        }
        else if(!string("-s").compare(argv[begin])){
            save=true;
            begin=begin+1;
        }
        else if(!string("-l").compare(argv[begin])){
            if(begin>=argc-1){
                cerr<<"incorrect network"<<endl;
                return -1;
            }
            string temp=argv[begin+1];
            stringstream st(temp);
            st>>loop;
            cout<<"loop time:"<<loop<<endl;
            begin=begin+2;
        }
        else{
            begin=begin+1;
        }
        if(begin>=argc)break;
    }
    srand(time(0));

    //string trainPath="../train/all_train.list";
    //string verifyPath="../train/all_test2.list";
    //string testPath="../train/all_test1.list";
    cout<<"begin Network"<<endl;
    Generator*generator;
    switch(game){
        case 0:{
            generator=new headMoveGenerator();
            break;
        }
        case 1:{
            generator=new sunglassGenerator();
            break;
        }
        case 2:{
            generator=new personGenerator();
            break;
        }
        case 3:{
            generator=new moodGenerator();
            break;
        }
        default:{
            cerr<<"no such game"<<endl;
            return -1;
        }
    }

    switch(mode){
        case 1:{//训练模式
            //sunglassGenerator generator;
            BPNN bpnn(trainPath,verifyPath,testPath,*generator);
            bp=&bpnn;
            bpnn.build(960,layer,generator->getOutputSize());
            //bpnn.setGenerator(generator);
            if(!network.empty())bpnn.load(network);
            bpnn.train(loop);
            bpnn.test();
            if(save && !network.empty())bpnn.save(network);
            break;
        }
        case 2:{//测试模式
            //sunglassGenerator generator;
            if(network.empty()){
                cerr<<"test mode need network file"<<endl;
                return -1;
            }
            if(testPath.empty()){
                cerr<<"test mode need testPath"<<endl;
                return -1;
            }
            BPNN bpnn(trainPath,verifyPath,testPath,*generator);
            bp=&bpnn;
            bpnn.build(960,layer,generator->getOutputSize());
            //bpnn.setGenerator(generator);
            bpnn.load(network);
            bpnn.test();
            break;
        }
        default:{
            break;
        }
    }
}