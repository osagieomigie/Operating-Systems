#include <iostream> 
#include <stdio.h>
#include <map>
#include <cstring> 
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>

#define MAX_FILE_LENGTH 512 

using namespace std; 

int main(int argc, char* const argv[]){

	//check command line arguments
	if( argc != 3) {
	    fprintf(stderr, "Usage: Emulates the 'find . –type f utility' \n \tFirst Argument: Search file extension\n \t Second Argument: Total amount to print\n");
	    exit(-1);
  	}

	char outPut[MAX_FILE_LENGTH];
 	int limit = stoi(argv[2]); 
 	int limitControl = 1; 
 	long long totalFilesLength = 0; 
	map<int,vector<string> > mp;
	map<int,vector<string> >::reverse_iterator it;

	struct stat buff;

	FILE *fp = popen("./myFind", "r");

	while (fgets(outPut, MAX_FILE_LENGTH, fp) != NULL){
		
		int outputLen = strlen(outPut) - 1;
		int argLen = strlen(argv[1]);
		int startSearch =  outputLen-argLen; 

		string result = strndup(outPut, outputLen);
		string resultExtension = result.substr(startSearch,argLen);
		stat(result.c_str(), &buff);
		if (resultExtension.compare(argv[1]) == 0){
			if (mp.find(buff.st_size) == mp.end()){
				vector<string>v;
				v.push_back(result);
				mp.insert(pair<int,vector<string> >(buff.st_size,v));
			}else{
				mp[buff.st_size].push_back(result);
			}
		}
	}

	// return result 
	for (it = mp.rbegin(); it != mp.rend(); it++){
		for(int i =0; i<(*it).second.size(); i++){
			if (limitControl <= limit){
				cout<<(*it).second[i]<<" "<<(*it).first<<endl;
				totalFilesLength += (*it).first;
				limitControl++;
			}else{
				break;
			}
		}
    }

    cout<<"Total size: "<<totalFilesLength<<endl; 
	pclose(fp); 

	return 0; 
}