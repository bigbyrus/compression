#include <iostream>
#include <vector>
#include <fstream>
#include "Helper.hpp"
#include "HCTree.hpp"

using namespace std;

/* INCLUDE FANCYINPUT STREAM SO THINGS CAN BE EASIER TO MANAGE */
int main(int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>" << endl;
        return 1;
    }

    /* Open input_file and read it in binary mode */
    ifstream file(argv[1], ios::binary);
    if(!file){
        cerr << "Error opening file" << endl;
        return 1;
    }

    /* Initialze 4 KB buffer as ArrayList */
    vector<unsigned char> buffer(4096);
    /* Initialize array list with 2^8 elements to store byte frequencies */
    vector<int> counts(256, 0);

    /* use pointer to buffer to attempt to read 4KB of file      */
    /* then using file.gcount update the count of each byte read */
    while(file){
        file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        streamsize bytesRead = file.gcount();

        for(int i=0; i<bytesRead; i++){
            unsigned char b = buffer[i];
            counts[b]++;
        }
    }
    /* print counts 
    for (int i = 0; i < 256; i++) {
        if (counts[i] > 0) {
            cout << i << " : " << counts[i] << "\n";
        }
    } */

    /* creating objects from Helper.hpp and HCtree.hpp */
    FancyInputStream in(argv[1]);
    FancyOutputStream out(argv[2]);
    HCTree hufftree;

    /* build Huffman Tree based on counts */
    hufftree.build(counts);

    /* WRITE THE HEADER TO THE fancyOUTPUTstream FIRST */
    for(int i=0; i<256; i++){
        unsigned int count = counts[i];
        out.write_int(count);
    }

    /* THEN WRITE ENCODED VALUES TO fancyOUTPUTstream */
    int byte;
    while((byte = in.read_byte()) != -1){
        unsigned char symbol = static_cast<unsigned char>(byte);
        hufftree.encode(symbol, out);
    }
}