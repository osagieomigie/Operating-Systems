#include<iostream>
#include<dirent.h>
#include<cstring> // Using cstring 
#include <cerrno>

using namespace std;
 
string fullPath; 

void printContent(const char *addr, string path){

  DIR *directory_Pointer;
  directory_Pointer = opendir(addr);

  if(directory_Pointer == NULL){
    cout<<strerror(errno)<< endl;
    exit(1);
  }else{
    struct dirent *it;

    while((it = readdir(directory_Pointer))){

      string docName = it->d_name; 
      fullPath = path + "/" + docName; 

      if (it -> d_type == DT_REG){
        cout<<fullPath<< endl;
      }else {

        if ((it -> d_type == DT_DIR) && (strcmp(it->d_name,".") != 0) && (strcmp(it->d_name,"..") != 0)){
          printContent(fullPath.c_str(), fullPath);
        }
      }
    }
    closedir(directory_Pointer);
  }
}

int main(){
  const char *path[]= {"."}; 
  string dirPath = ".";
  printContent(*path, dirPath);

  return 0;  
}
