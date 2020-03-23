#include <iostream>
#include <set>
#include <bitset>
#include <vector>
#include "MemoryAccess.h"
#include "InterStateBuffers.h"
#define NUMBER_SETS 64 
#define SET_SIZE 2
// Address is 12 BIT .
// SIZE OF MAIN MEMORY IS 32 MB
using namespace std;



class Cache{ 
    private:
    vector <vector <int> > CacheMem;
    vector <int> WriteBack; // The Dirty bit.
    int CacheSize, BlockSize,choice,waysofset;
    int numblocks;
    int coldmisses,datamiss;
    int accesses;
    int capacity;
    void divide (bitset <32> source , bitset <8> & byte1 , bitset<8> & byte2 , bitset <8> & byte3 , bitset <8> & byte4) {
		int k=0,l=0,m=0,n=0;
		for(int i = 0 ; i <= 7 ; i++)
			byte1[k++] = source[i];
		for(int i = 8 ; i <= 15 ; i++)
			byte2[l++] = source[i];
		for(int i = 16 ; i <= 23 ; i++)
			byte3[m++] = source[i];
		for(int i = 24 ; i <= 31 ; i++)
			byte4[n++] = source[i];

	}

	void unite (bitset <32> & output , bitset <8> byte1 , bitset<8>  byte2 , bitset <8>  byte3 , bitset <8> byte4) {
		int k=0,l=0,m=0,n=0;
		for (int i =0;i< 32 ; i++) {
			if (i<8) {
				output[i] = byte1[k];k++;
			} else if (i<16) {
				output[i] = byte2[l];l++;
			} else if (i<24) {
				output[i] = byte3[m];m++;
			} else {
				output[i] = byte4[n];n++;
			}
		}
	}

    public:
    Cache(){
        CacheMem.resize(128);
        WriteBack.resize(128);
    }
    Cache(int cs,int bs, int choice, int ways){ // Given in MB.
        CacheSize = cs;
        BlockSize = bs;
        this->choice = choice;
        waysofset = ways;
        numblocks = CacheSize/bs;
        CacheMem.resize(numblocks);
        for(int i = 0 ;  i < CacheMem.size() ; i++)
            CacheMem[i].resize(BlockSize+3);
        
        coldmisses = 0;
        datamiss = 0;
        accesses = 0;
        capacity = 0;
        
    }

    void ReadCache(MemoryAccess &memobject, InterStateBuffers &isb,int choice){ // choice 1 for word, choice 2 for byte
         bitset <12> address = isb.RZ.readInt();
         accesses++;
         isb.accesses_data++;
         int blockoffset = address.to_ulong() % BlockSize;
         int blocknumber = address.to_ullong() / BlockSize;
         int tag = blocknumber / numblocks;
         blocknumber %= numblocks;

         int validdata = CacheMem[blocknumber][0]; // validity
         int tagfound = CacheMem[blocknumber][1]; // tag 

         

         if(validdata == 0){
            coldmisses += 1;
         }

        if(tagfound == tag && validdata == 1){
            isb.hits_data++;
            int index = 3+blockoffset;
            CacheMem[blocknumber][2]++; // update hits

            if(choice == 1){ // if its loadword
                bitset <32> output;
                bitset <8> byte1,byte2,byte3,byte4;
                
                byte4 = CacheMem [blocknumber][index + 0];
                byte3 = CacheMem [blocknumber][index + 1];
                byte2 = CacheMem [blocknumber][index + 2];
                byte1 = CacheMem [blocknumber][index + 3];

                unite (output, byte1, byte2, byte3, byte4);
                isb.mem_register = output.to_ulong();
            }
            else if(choice == 2){
                int data = CacheMem[blocknumber][3 + blockoffset];
                isb.mem_register = data;
            }

        }

        if(tag != tagfound || validdata == 0 ){
            
            if(validdata == 1){
                isb.conflict_misses_data++;
            }
            if(validdata == 0){
                isb.cold_misses_data++;
            }
            if(capacity == numblocks){
                isb.capacity_misses_data++;
            }

            for(int i = 0 ; i < BlockSize ; i++){
                
                bitset <8> data = memobject.readByte(address.to_ulong() - blockoffset + i );
                CacheMem [blocknumber][i + 3] = data.to_ulong();
            }
            capacity++;
            CacheMem[blocknumber][0] = 1;
            int index = 3 + blockoffset;
            bitset <32> output;
            bitset <8> byte1,byte2,byte3,byte4;

            
                
                byte4 = CacheMem [blocknumber][index + 0];
                byte3 = CacheMem [blocknumber][index + 1];
                byte2 = CacheMem [blocknumber][index + 2];
                byte1 = CacheMem [blocknumber][index + 3];

                unite (output, byte1, byte2, byte3, byte4);

                isb.mem_register = output.to_ulong();
            
        }

        
         
    }

    void WriteCache(MemoryAccess &memobject, InterStateBuffers &isb, int choice){
            accesses++;
            isb.accesses_data++;
            bitset <12> address = isb.RZ.readInt();
            int blockoffset = address.to_ulong() % BlockSize;
            int blocknumber = address.to_ullong() / BlockSize;
            int tag = blocknumber / numblocks;
            blocknumber %= numblocks;
        

         if(choice == 1)
         {
            

            bitset <8> byte1,byte2,byte3,byte4;
            bitset <32> source = isb.RM.readBitset();

            int index = 3 + blockoffset;
            divide(source,byte1, byte2, byte3 , byte4); // byte1 lsb

            // Write to its position
            CacheMem[blocknumber][index + 0] = byte4.to_ulong();
            CacheMem[blocknumber][index + 1] = byte3.to_ulong();
            CacheMem[blocknumber][index + 2] = byte2.to_ulong();
            CacheMem[blocknumber][index + 3] = byte1.to_ulong();
            
            CacheMem[blocknumber][0] = 1; // tell that data is valid.
            memobject.writeWord(isb);
        }
        else if(choice == 2){
            bitset <8> byte;
            bitset <32> source;
            
            source = isb.RZ.readBitset();
            int index = 3 + blockoffset;

            for (int i = 0; i<8; i++) {
                byte[i] = source[i];
            }

            CacheMem[blocknumber][index] = byte.to_ulong();
            CacheMem[blocknumber][0] = 1;
            memobject.writeByte(isb);
        }

        capacity++;

    }

  
};
