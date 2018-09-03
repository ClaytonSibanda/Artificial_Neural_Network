#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
using namespace std;


class Neuron;

typedef vector<Neuron> Layer;

//**************************** class Neuron ****************************************
class Neuron{

public:
    Neuron(unsigned numOutputs, unsigned m_myIndex,unsigned _layerNum);
    void setOutputVal(double inputVal){m_outputVal=inputVal;}
    double getOutputVal(void) const{return m_outputVal;}
    void feedForward(Layer &prevLayer);

private:
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
   // static double randomWeight(void){return rand()/double(RAND_MAX); } not for now
    double m_outputVal;
    vector<double> m_outputWeights;
    unsigned m_myIndex;
    unsigned layerNum;

};

double Neuron::transferFunction(double x) {

    return 1/(1+exp(-x));
}

double Neuron::transferFunctionDerivative(double x) {

    return (exp(-x))/pow((1+exp(-x)),2);
}

void Neuron::feedForward(Layer &prevLayer) {
    double sum=0.0;
    //sum the previous layers outputs (which are our inputs)

    for(unsigned n=0;n<prevLayer.size();++n){
        cout<<"output "<<n<<" is "<<prevLayer[n].getOutputVal()<<" index is "<<m_myIndex<<endl;
        sum+=prevLayer[n].getOutputVal()*prevLayer[n].m_outputWeights[m_myIndex];
    }
cout<<"sum is "<<sum<<endl;
    m_outputVal = Neuron::transferFunction(sum);
    cout<<"output is "<<m_outputVal<<endl;

}


//helper function used when populating the constructor

void neuronHelper(unsigned layerNum, unsigned m_myIndex,vector<double> &m_outputWeights){
    if(layerNum==0 && m_myIndex==0){
        m_outputWeights= {0.1,0.4};
    }
    if(layerNum==0 && m_myIndex==1){
        m_outputWeights= {0.2,1.0};
    }
    if(layerNum==0 && m_myIndex==2){
        m_outputWeights= {0.5,-0.6};
    }
    if(layerNum==0 && m_myIndex==3){// bias neuron, every layer has a bia neuron
        m_outputWeights= {1,1};
    }//bias neuron
    //for layer2

    if(layerNum==1 && m_myIndex==0){
        m_outputWeights= {0.8};
    }
    if(layerNum==1 && m_myIndex==1){
        m_outputWeights= {1.0};
    }
    if(layerNum==1 && m_myIndex==2){//bias neuron
        m_outputWeights= {1.0};
    }
}

Neuron::Neuron(unsigned numOutputs, unsigned _myIndex, unsigned _layerNum):m_myIndex(_myIndex),layerNum(_layerNum) {
//    for(unsigned c=0;c<numOutputs;++c){
//       cout<<"neuron here\n";
//
//    }

neuronHelper(_layerNum,_myIndex,m_outputWeights);
//    cout<<"numOutputs "<< numOutputs<<endl;
//    for(unsigned c=0;c<numOutputs;++c){
//        cout<<m_myIndex<<" "<<m_outputWeights[c]<<endl;
//
//    }

}

//***************************  class Neuron ****************************************

class Net{
public:
    Net(const vector<unsigned> &topology);
    void feedForward(const vector<double> &inputVals);
    void getResults(vector<double> resultVals) const{};

private:
    vector<Layer> m_layers; //n_layers[layerNum][neuronNum] Layer is a vector of neurons

};


void Net::feedForward(const vector<double> &inputVals){
    //assert(inputVals.size()==m_layers[0].size()-1);
    //Assign the input values into the input neurons
 cout<<"net feedfoward\n";
    for(unsigned i=0;i<inputVals.size();++i){
        m_layers[0][i].setOutputVal(inputVals[i]);
    }

    //forward propagate i.e looping over neurons and tell them to feed forward

    for(unsigned layerNum =1; layerNum<m_layers.size();++layerNum){
        Layer &prevLayer =m_layers[layerNum-1];
        for(unsigned n=0;n<m_layers[layerNum].size()-1;++n){
            m_layers[layerNum][n].feedForward(prevLayer);
        }
    }

}

Net::Net(const vector<unsigned> &topology){
    Layer layer;
    unsigned numLayers =topology.size();

    for(unsigned layerNum=0;layerNum<numLayers;++layerNum){
        m_layers.push_back(layer);
        unsigned numOutputs = layerNum==topology.size()-1?0: topology[layerNum+1];
        cout<<"\nLayereNum "<<layerNum<<endl;
        for(unsigned neuronNum=0;neuronNum<=topology[layerNum];++neuronNum){//also includes the bias nueron

            m_layers.back().push_back(Neuron(numOutputs,neuronNum,layerNum));
            cout<<"numOutputs "<<numOutputs<<" neuronNum "<<neuronNum<<"\n";
        }

        // Force the bias node's output to 1.0 (it was the last neuron pushed in this layer):
        m_layers.back().back()//back()
        .setOutputVal(1.0);
    }
}

int main() {

vector<unsigned > topology;// represents layers e.g 3,2,1. 3 neurons for layer1, 2 neurons for layer2 etc
topology.push_back(3);
topology.push_back(2);
topology.push_back(1);



Net myNet(topology);


vector<double> inputVals{1.3,2.7,0.8};
vector<double> targetVals;
vector<double> resultVals;

myNet.feedForward(inputVals);
myNet.getResults(resultVals);




    return 0;
}