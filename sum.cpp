#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <vector>
#include <cstring> 
#include <cmath>
#include <fstream>

using namespace std;

pthread_mutex_t count_mutex;
vector<int>initialArray;
vector<int>sumArray; 

struct arguments
{
	int startIndex; 
	int endIndex;
	bool lastThread;  
	int threadNumber; 
}arguments;

void * sum(void* argument){ 
	pthread_mutex_lock(&count_mutex);

	struct arguments *args = (struct arguments *)argument;
	int sum = 0;
	int start = args->startIndex;
	int end = args->endIndex;
	
	if (!(args->lastThread)){ 
		for(int i=start; i<=end; i++){ 
			sum += initialArray[i];
		}
	}
	else{
		for(int i=start; i<end; i++){ 
			sum += initialArray[i];
		}
	}
   cout<<"Thread number: "<< args->threadNumber<<"  sum: "<<sum<<endl; 
   sumArray.push_back(sum);
   pthread_mutex_unlock(&count_mutex);

   pthread_exit(NULL);
}

int main(int argc, const char *argv[]){

	pthread_mutex_init(&count_mutex, NULL);
    string fileName = argv[1];
    ifstream file;
    
    int totalThreads = stoi(argv[2]);
    long tNums; 
    long tmpNums; 
    long total = 0;
    string value;
    pthread_t id[totalThreads];

    //check command line arguments
	if( argc != 3) {
	    fprintf(stderr, "Usage: A multithreaded program for calculating the sum of an array' \n \tFirst Argument: A text file containing the numbers (each number on it own line) \n \t Second Argument: Total amount of threads\n");
	    exit(-1);
  	}
    file.open(fileName);
    
    while (getline(file, value)){
        initialArray.push_back(stoi(value));
    }

    tNums = initialArray.size(); 

    for(int i=1; i <= totalThreads; i++){

    	long status; 

    	if (i == 1){
    		tmpNums = ceil(tNums/totalThreads); 
    		arguments.startIndex = 0; 
    		arguments.endIndex = tmpNums; 
    		arguments.threadNumber = i;

    		// handles the case in the theres only 1 thread to be created
    		if (totalThreads != 1){ 
    			arguments.lastThread = false;  
    		}else{
    			arguments.lastThread = true;
    		}
    		status = pthread_create(&id[i], NULL, sum, (void *)&arguments);
    		pthread_join(id[i], NULL);
    	}
    	else{
    		int tmpEndIndex = arguments.endIndex+1;
    		tmpNums = floor(tNums/totalThreads); 
    		arguments.startIndex = tmpEndIndex;
    		arguments.threadNumber = i;

    		if (i == totalThreads){
    			arguments.endIndex = initialArray.size();
    			arguments.lastThread = true;  
    			status =  pthread_create(&id[i], NULL, sum, (void *)&arguments);
    			pthread_join(id[i], NULL);
    		}else {
    			arguments.endIndex = tmpNums + tmpEndIndex;
    			arguments.lastThread = false; 
    			status =  pthread_create(&id[i], NULL, sum, (void *)&arguments);
    			pthread_join(id[i], NULL);
    		}
    	}

    	if(status){
    		printf("Error in creating thread\n");
    		exit(-1);
    	}

    	tNums = tNums - tmpNums;
    }
 
 	for(int i = 0; i< sumArray.size(); i++){
			total += sumArray[i];
	} 

    pthread_mutex_destroy( &count_mutex);
	cout<<"Sum = "<< total<<endl; 
}