#include<iostream>
#include<cstring>
#include<vector>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<ctype.h> 

using namespace std; 

struct processStruct
{
	int name; 
	int arrival; 
	int burstTime;
	int timeLeft;  
};

// vector of structs sort function 
bool customSort(const processStruct &a, const processStruct &b){
	return a.burstTime > b.burstTime;
}

void pop_front(vector<processStruct> &v)
{
    if (v.size() > 0) {
        v.erase(v.begin());
    }
}

// shortest job first simulation
void shortestJobFirst( vector<processStruct> &p){
	reverse(p.begin(),p.end());
	vector<string> states;
	vector<processStruct> readyQueue;
	vector<double> countWaitTimes;  
    int currentTime = 0; 
    int job_remaining = p.size();   
    int job_size = p.size(); 
    double average_wait = 0; 
    processStruct cpu;
    bool cpuEmpty = true;  

    //initialize states array 
    for(int i =0 ; i < p.size(); i++){
    	states.push_back("\t");  
    }

    //initialize count waits array 
    for(int i =0 ; i < p.size(); i++){
    	countWaitTimes.push_back(0.0);  
    }

    cout<<"Time\t";
    for (int i =0; i<p.size(); i++){
    	cout<<"P"<<i<<"\t"; 
    }
    cout<<"\n"; 
    cout<<"------------------------------------------------------------------------"<<endl; 

   	while(1){
   		
   		processStruct tmpPro = p.back(); 

   		if(job_remaining == 0){
   			break; 
   		} 

   		//Done executing job 
   		if((cpu.burstTime == 0) && (!cpuEmpty)){
   			//cout<<"done: "<<cpu.name<<endl;
   			states[cpu.name] = "\t";
   			cpuEmpty = true; 
   			job_remaining--; 
   			continue; 
   		}

   		// add new processes to ready queue 
   		if (tmpPro.arrival == currentTime){
   			readyQueue.push_back(tmpPro); 
   			p.pop_back(); 
   			states[tmpPro.name] = ".\t";
   			continue; 
   		}

   		// load job from sorted ready queue to CPU 
   		if ((!readyQueue.empty()) && (cpuEmpty)){
   			sort(readyQueue.begin(), readyQueue.end(), customSort);
   			cpu = readyQueue.back();
   			cpuEmpty = false; 
   			readyQueue.pop_back();
   			continue; 
   		}

   		//execute one burst of job 
   		if(!cpuEmpty){  
   			cpu.burstTime--;

			states[cpu.name] = "#\t";    			
   			for(int j = 0; j<readyQueue.size(); j++){
				if((readyQueue[j].name != cpu.name) && (j != cpu.name)){
					states[readyQueue[j].name] = ".\t";
				}
   			}
   		}

   		//print states
   		cout<<currentTime<<"\t"; 
   		for(int i = 0; i<states.size(); i++){
   			cout<<states[i];
   			if (states[i] == ".\t"){
   				countWaitTimes[i] += 1; 
   			}
   		}
   		cout<<"\n";

   		//increment time 
   		currentTime++; 
   	}

   	cout<<"------------------------------------------------------------------------"<<endl; 

   	// print wait times and sum up 
	for(int i = 0; i<job_size; i++){
		cout<<"P"<<i<<"  waited "<<countWaitTimes[i]<<" sec."<<endl; 
		average_wait += countWaitTimes[i];
	}

	average_wait /= job_size; 

	cout<<"Average waiting time = "<<average_wait<<" sec."<<endl; 
} 

// Round Robin simulation
void RoundRobin( vector<processStruct> &p, int tSlice){
	reverse(p.begin(),p.end());
	vector<string> states;
	vector<processStruct> readyQueue;
	vector<double> countWaitTimes;  
    int currentTime = 0; 
    int job_remaining = p.size();   
    int job_size = p.size(); 
    double average_wait = 0; 
    processStruct cpu;
    bool cpuEmpty = true;  


    //initialize states array 
    for(int i =0 ; i < p.size(); i++){
    	states.push_back("\t");  
    }

    //initialize count waits array 
    for(int i =0 ; i < p.size(); i++){
    	countWaitTimes.push_back(0.0);  
    }

    cout<<"Time\t";
    for (int i =0; i<p.size(); i++){
    	cout<<"P"<<i<<"\t"; 
    }
    cout<<"\n"; 
    cout<<"------------------------------------------------------------------------"<<endl; 

   	while(1){
   		
   		processStruct tmpPro = p.back(); 

   		if(job_remaining == 0){
   			break; 
   		} 

   		//Done executing job 
   		if((cpu.burstTime == 0) && (!cpuEmpty)){
   			states[cpu.name] = "\t";
   			cpuEmpty = true; 
   			job_remaining--; 
   			continue; 
   		}

   		// put job that exceeds time slice back to ready queue 
   		if((cpu.timeLeft == 0) && (cpu.burstTime != 0) && (!cpuEmpty)){
   			states[cpu.name] = ".\t"; //set job to waiting state 
   			cpuEmpty = true; 
   			readyQueue.push_back(cpu); // put job back to ready queue
   			continue; 
   		}

   		// add new processes to ready queue 
   		if (tmpPro.arrival == currentTime){ 
   			tmpPro.timeLeft = tSlice; // set time slice for every new job 
   			readyQueue.push_back(tmpPro); 
   			p.pop_back(); 
   			states[tmpPro.name] = ".\t";
   			continue; 
   		}

   		// load job from ready queue to CPU 
   		if ((!readyQueue.empty()) && (cpuEmpty)){
   			cpu = readyQueue.front();
   			cpu.timeLeft = tSlice; // reset time slice 
   			cpuEmpty = false; 
   			pop_front(readyQueue);
   			continue; 
   		}

   		//execute one burst of job 
   		if(!cpuEmpty){
        cpu.burstTime--;
   			cpu.timeLeft--; 
        states[cpu.name] = "#\t";    			
   			for(int j = 0; j<readyQueue.size(); j++){
				  if((readyQueue[j].name != cpu.name) && (j != cpu.name)){
            states[readyQueue[j].name] = ".\t";
				  }
   			}
   		}

   		//print states
   		cout<<currentTime<<"\t"; 
   		for(int i = 0; i<states.size(); i++){
   			cout<<states[i];
   			if (states[i] == ".\t"){
   				countWaitTimes[i] += 1; 
   			}
   		}
   		cout<<"\n";

   		//increment time 
   		currentTime++; 
   	}

   	cout<<"------------------------------------------------------------------------"<<endl; 

   	// print wait times and sum up 
	for(int i = 0; i<job_size; i++){
		cout<<"P"<<i<<"  waited "<<countWaitTimes[i]<<" sec."<<endl; 
		average_wait += countWaitTimes[i];
	}

	average_wait /= job_size; 

	cout<<"Average waiting time = "<<average_wait<<" sec."<<endl; 
} 

int main(int argc, char const *argv[])
{
	if(argc < 3){
    	cout<<"Usage:\n 1st argument: configuration file (arrival & burst time per line);\n 2nd argument: scheduler algorithm ('RR' or 'SJF');\n 3rd argument (only use if using RR): time slice"<<endl;
  		exit(-1);
    }

	string fileName = argv[1];
	string scheduler = argv[2];
	ifstream file;
	string value;

	vector<processStruct> process; 

	file.open(fileName);
    
    int start = 0; 
    while (getline(file, value)){
    	stringstream sstream(value);
    	int arrival, burstTime; 

    	sstream >> arrival >> burstTime; 

      processStruct tmpS; 
      tmpS.arrival = arrival;
      tmpS.burstTime = burstTime;
      tmpS.name = start;

      process.push_back(tmpS); 
      start++; 
    }

  
  	if((scheduler.compare("RR")) == 0){
  		if (argc == 4){
  			int rrSlice = stoi(argv[3]); 
			RoundRobin(process, rrSlice);
		}else{
    		cout<<"Usage:\n 1st argument: configuration file (arrival & burst time per line);\n 2nd argument: scheduler algorithm ('RR' or 'SJF');\n 3rd argument (only use if using RR): time slice"<<endl;
  			exit(-1);
  		}
  	}

  	if((scheduler.compare("SJF")) == 0){
  		if (argc == 3){ 
			shortestJobFirst(process);
		}else{
  			cout<<"This is an implementation of Non-preemptive shortest job first"<<endl;
    		cout<<"Usage:\n 1st argument: configuration file (arrival & burst time per line);\n 2nd argument: scheduler algorithm ('RR' or 'SJF');\n 3rd argument (only use if using RR): time slice"<<endl;
			exit(-1);	
		}
  	}

	return 0;
}