#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring> 

using namespace std;

typedef std::string SS;
typedef std::vector<SS> VS;

struct DEntry {
    SS fname = SS( 4096, 0);
    int size = 0;
    int actualSize = 0; 
    int ind = 0;
    bool tooManyBlocks = true;
    bool tooFewBlocks = false;
    bool hasCycle = true;
    bool sharesBlocks = true;
    int maxFileLength = 0; // used for determining threshold of file 
    set<int> blocks; // files blocks
};

static SS join( const VS & toks, const SS & sep) {
    SS res;
    bool first = true;
    for( auto & t : toks) { res += (first ? "" : sep) + t; first = false;}
    return res;
}

// Parameters:
//   blockSize - contains block size as read in from input
//   files - array containing the entries as read in from input
//   fat - array representing the FAT as read in from input
// Return value:
//   the function should return the number of free blocks
//   also, for ever entry in the files[] array, you need to set the appropriate flags:
//      i.e. tooManyBlocks, tooFewBlocks, hasCycle and sharesBlocks
int checkConsistency( int blockSize, std::vector<DEntry> & files, std::vector<int> & fat)
{
    set<int> usedBlocks; 
    //int usedBlocks = 0; 

    bool emptyFlag = false;

    // make the first entry contain no errors
    for(int i =0; i < files.size(); i++){

        files[i].hasCycle = false;
        files[i].tooFewBlocks = false;
        files[i].tooManyBlocks = false; 
        files[i].sharesBlocks = false;
        files[i].actualSize = 0;
        files[i].maxFileLength = 0; 
        emptyFlag = false;

        int current_block = files[i].ind;
        vector<bool> usedFrames; 
        for(int i = 0; i<200000; i++){
            usedFrames.push_back(false); 
        }

        while(1){ // loop until last block 

            //check to see if file is empty 
            if ((current_block == -1)){
                if(files[i].size > 0){
                    files[i].tooFewBlocks = true; 
                }
                emptyFlag = true;
                break;
            }

            if (fat[current_block] == -1){ // break if file at end 
                usedFrames[current_block] = true;
                usedBlocks.insert(current_block);
                files[i].blocks.insert(current_block); // keep track of blocks used
                files[i].maxFileLength = files[i].maxFileLength + 1 * blockSize; 
                //usedBlocks++;
                files[i].actualSize++;
                break;
            }else if ((fat[current_block] == current_block) || (usedFrames[fat[current_block]] == true)){  //check if current index is in the used block set                                       
                usedFrames[current_block] = true;
                usedBlocks.insert(current_block);
                files[i].hasCycle = true;
                files[i].blocks.insert(current_block);
                files[i].maxFileLength = files[i].maxFileLength + 1 * blockSize; 
                //usedBlocks++;
                files[i].actualSize++;
                break;
            }else{
                usedFrames[current_block] = true;
                usedBlocks.insert(current_block);
                files[i].blocks.insert(current_block);
                files[i].maxFileLength = files[i].maxFileLength + 1 * blockSize;  
                //usedBlocks++;
                files[i].actualSize++;
            }
            
            current_block = fat[current_block]; // pointer to next block
        }

        //check if blocks allocated are correct
        size_t findReg = files[i].fname.find("reg");
        
        if ((files[i].size > files[i].maxFileLength) && (!emptyFlag) &&(findReg==std::string::npos))
        {
            files[i].tooFewBlocks = true;
        }

        if ((files[i].size < files[i].maxFileLength) && (!emptyFlag)  &&(findReg==std::string::npos))
        {
            files[i].tooManyBlocks = true;
        }
    }

    //check for shared blocks 
    for (int i=0; i < files.size(); i++){
        for (int j = 0; j < files.size(); j++){
            if(i != j){
                std::vector<int> result;                    
                bool intersect = false;

                set_intersection(files[i].blocks.begin(),files[i].blocks.end(),files[j].blocks.begin(),files[j].blocks.end(), std::back_inserter(result)); 

                if (result.size() > 0){
                    files[i].sharesBlocks = true;
                }
            }
        }
    }


    // finally, return the number of free blocks
    return fat.size() - usedBlocks.size();
}

int main()
{
    try {
        // read in blockSize, nFiles, fatSize
        int blockSize, nFiles, fatSize;
        if( 3 != scanf( "%d %d %d", & blockSize, & nFiles, & fatSize))
            throw "cannot read blockSize, nFiles and fatSize";
        if( blockSize < 1 || blockSize > 1024) throw "bad block size";
        if( nFiles < 0 || nFiles > 50) throw "bad number of files";
        if( fatSize < 1 || fatSize > 200000) throw "bad FAT size";
        // read in the entries
        std::vector<DEntry> entries;
        for( int i = 0 ; i < nFiles ; i ++ ) {
            DEntry e;
            if( 3 != scanf( "%s %d %d", (char *) e.fname.c_str(), & e.ind, & e.size))
                throw "bad file entry";
            e.fname = e.fname.c_str();
            if( e.fname.size() < 1 || e.fname.size() > 16)
                throw "bad filename in file entry";
            if( e.ind < -1 || e.ind >= fatSize) throw "bad first block in file entry";
            if( e.size < 0 || e.size > 1073741824) throw "bad file size in file entry";
            entries.push_back( e);
        }
        // read in the FAT
        std::vector<int> fat( fatSize);
        for( int i = 0 ; i < fatSize ; i ++ ) {
            if( 1 != scanf( "%d", & fat[i])) throw "could not read FAT entry";
            if( fat[i] < -1 || fat[i] >= fatSize) throw "bad FAT entry";
        }

        // run the consistency check
        int nFreeBlocks = checkConsistency( blockSize, entries, fat);

        // format the output
        size_t maxflen = 0;
        for( auto & e : entries ) maxflen = std::max( maxflen, e.fname.size());
        SS fmt = "  %" + std::to_string( maxflen) + "s: %s\n";

        printf( "Issues with files:\n");
        for( auto & e : entries ) {
            VS issues;
            if( e.tooFewBlocks) issues.push_back( "not enough blocks");
            if( e.tooManyBlocks) issues.push_back( "too many blocks");
            if( e.hasCycle) issues.push_back( "contains cycle");
            if( e.sharesBlocks) issues.push_back( "shares blocks");
            printf( fmt.c_str(), e.fname.c_str(), join( issues, ", ").c_str());
        }
        printf( "Number of free blocks: %d\n", nFreeBlocks);
    }
    catch( const char * err) {
        fprintf( stderr, "Error: %s\n", err);
    }
    catch( ... ) {
        fprintf( stderr, "Errro: unknown.\n");
    }
    return 0;
}