#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std; 

vector<int> referenceString; 
vector<int>::iterator it;

// searches ahead to see if string appears in any of the next 4 positions of the reference string
list<int> searchAhead(int startPosition,int frames){
	list<int> occurances;
	int endSearch = startPosition+frames-1; 

	for (; startPosition < endSearch; startPosition++){
		occurances.push_back(referenceString[startPosition]);
	}
	return occurances; 
}

void Optimal(int frames){

	int pageFaults = 0;  
	int lastUsed = 0; 
	int refIndex = 0; 
	list<int> popCandidate; 
	list<int>::iterator listit;
	std::vector<int> tFrames;

	int end = referenceString.size(); 

	while(end > 0){

		lastUsed = referenceString[refIndex];
		if(tFrames.size() == frames){
				
			//page already in frame 
			it = find(tFrames.begin(), tFrames.end(), lastUsed);
			if (it != tFrames.end()){
			}else{
				popCandidate = searchAhead(refIndex+1, frames);

				for(int i =0; i<tFrames.size(); i++){
					listit = find(popCandidate.begin(), popCandidate.end(), tFrames[i]);
					if(listit == popCandidate.end()){
						tFrames[i] = lastUsed; 
						pageFaults++;
			 			break;
					}
				}
			}
		}else{
			it = find(tFrames.begin(), tFrames.end(), lastUsed);
			if (it == tFrames.end()){
				tFrames.push_back(lastUsed);
				pageFaults++;
			}
		} 

		end--; 
		refIndex++; 
	}

	cout << "Optimal: "<<endl; 
	cout <<"  - frames: ";
	for(int i = 0; i< tFrames.size(); i++){
		cout<<tFrames[i] <<" ";
	}
	cout<< "\n"; 
	cout << "  - page faults: "<<pageFaults<<endl; 
}

void LRU(int frames){
	int pageFaults = 0; 
	int lastUsed = 0; 
	int refIndex = 0; 
	std::vector<int> tFrames;
	std::list<int> popCandidate;

	int end = referenceString.size(); 

	while(end > 0){

		lastUsed = referenceString[refIndex]; 

		if(tFrames.size() == frames){
			//page already in frame 
			it = find(tFrames.begin(), tFrames.end(), lastUsed);
			if (it != tFrames.end()){
				popCandidate.remove(lastUsed);
				popCandidate.push_back(lastUsed);
			}else{
				int temp = popCandidate.front();
				popCandidate.pop_front();
				for(int i =0; i< tFrames.size(); i++){
					if (tFrames[i] == temp){
						tFrames[i] = lastUsed;
					}
				} 
				popCandidate.push_back(lastUsed);
				pageFaults++; 
			}
		}else{
			it = find(tFrames.begin(), tFrames.end(), lastUsed);
			if (it == tFrames.end()){
				tFrames.push_back(lastUsed);
				pageFaults++;
				popCandidate.push_back(lastUsed);
			}
		} 
		end--; 
		refIndex++; 
	}

	cout << "LRU: "<<endl; 
	cout <<"  - frames: ";
	for(int i = 0; i< tFrames.size(); i++){
		cout<<tFrames[i] <<" ";
	}
	cout<< "\n"; 
	cout << "  - page faults: "<<pageFaults<<endl; 
}

void Clock(int frames){
	int pageFaults = 0; 
	int lastUsed = 0; 
	int currentHand = 0; 
	int refIndex = 0; 
	std::vector<int> tFrames;
	std::vector<int>::iterator vit;
	vector<int> refBit;

	int end = referenceString.size(); 

	for(int i = 0; i < frames; i++){
		refBit.push_back(1);
	}

	while(end > 0){
		lastUsed = referenceString[refIndex]; 

		if(tFrames.size() == frames){
			//page already in frame 
			it = find(tFrames.begin(), tFrames.end(), lastUsed);
			if (it != tFrames.end()){
				//reset reference bit  
				for(int i = 0; i< tFrames.size(); i++){
					if(tFrames[currentHand] == lastUsed){
						refBit[currentHand] = 1; 
					}
				}
			}else{
				bool done = false; 
				while(!done){
					if(refBit[currentHand] == 1){
						refBit[currentHand] = 0;
					}else{
						if (refBit[currentHand] == 0){
							tFrames[currentHand] = lastUsed;
							refBit[currentHand] = 1;
							currentHand = (currentHand + 1) % frames; 
							done = true; 
							break;
						}
					}
				}
				pageFaults++; 
			}
		}else{
			it = find(tFrames.begin(), tFrames.end(), lastUsed);
			if (it == tFrames.end()){
				tFrames.push_back(lastUsed);
				pageFaults++;
				currentHand = (currentHand + 1) % frames;
			}
		}
		end--; 
		refIndex++; 
	}

	cout << "Clock: "<<endl; 
	cout <<"  - frames: ";
	for(int i = 0; i< tFrames.size(); i++){
		cout<<tFrames[i] <<" ";
	}
	cout<< "\n"; 
	cout << "  - page faults: "<<pageFaults<<endl; 
}

int main(int argc, char const *argv[]){

	int frames = stoi(argv[1]); 
	int tmpRString; 
    
	while(cin >> tmpRString){
		referenceString.push_back(tmpRString);
	}

	Optimal(frames);
	LRU(frames);
	Clock(frames);

	return 0; 
}