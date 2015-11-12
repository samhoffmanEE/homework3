#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

#define TABLE_SIZE 4032768

class Bloom{
private:
  bitset<4032768> table; //2^15
  int numHashs = 10;
  unsigned int *seedarr;

public:

  Bloom(){
    this->seedarr = seeds();
  }
  ~Bloom(){
    delete[] seedarr;
    //cout << "Deleted." << endl; //(checks if it runs)
  }
  unsigned int* seeds(){
    srand(time(NULL));
    unsigned int * seedvals = new unsigned int[numHashs];
    for (int i = 0; i < numHashs; i++){
      seedvals[i] = rand();
    }
    return seedvals;
  }
  unsigned int* getseeds(){
    return seedarr;
  }
  unsigned int Murmur2(const void * key, int len, unsigned int seed ){
  // 'm' and 'r' are mixing constants generated offline.
  // They're not really 'magic', they just happen to work well.

  const unsigned int m = 0x5bd1e995;
  const int r = 24;

  // Initialize the hash to a 'random' value

  unsigned int h = seed ^ len;

  // Mix 4 bytes at a time into the hash

  const unsigned char * data = (const unsigned char *)key;

  while(len >= 4)
  {
    unsigned int k = *(unsigned int *)data;

    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    data += 4;
    len -= 4;
  }

  // Handle the last few bytes of the input array

  switch(len){
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
        h *= m;
  };

  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
};
  unsigned int* hashing(string key){
    unsigned int *hashed = new unsigned int[numHashs];
    unsigned int *s = getseeds();
    for (int i =0; i< numHashs; i++){
      hashed[i] = Murmur2(&key, key.length(), s[i])%TABLE_SIZE;
    }
    return hashed;
  }
  void add(string key){
    int indx = 0;
    unsigned int *hashes = hashing(key);
    for (int i = 0; i < numHashs; i++){
        table[hashes[i]] = 1;
    }
    delete[] hashes;
  }

  bool is_in(string key){
    bool checked = false;
    unsigned int *hashindx = hashing(key);
    for (int i = 0; i < numHashs; i++){
      if (table[hashindx[i]] == 0){
        checked = false;
      }
      else{
        checked = true;
      }
    }
    return checked;
  };

};



int main(){
  Bloom bfilter;;
  /*unsigned int *test;
  test = bfilter.getseeds();
  for (int i = 0; i < 10; i++){   //(used for testing)
    cout << test[i] << endl;
  }
  unsigned int *h;
  h = bfilter.hashing("hello");
  for (int i = 0; i < 10; i++){    //(used for testing)
    cout << h[i] << endl;
  }*/
//created the bloom filter
  string line;
  ifstream dict("wrds.txt"); //opens it
  if (dict.is_open()){
    while (getline(dict, line)){
      bfilter.add(line);
    }
    dict.close();
  }
// read in the dictionary

//check if its in there.
  string word = "";
//interface for the user
  cout << "Check a word to see if it is in the dictionary." << endl;
  cin >> word;
  if (bfilter.is_in(word) == true){
    cout << word << " is probably a real word" << endl;
  }
  else{
    cout << word << " is probably not a real word" << endl;
  }
}
