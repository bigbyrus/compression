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

    /* creating objects from Helper.hpp and HCtree.hpp */
    FancyInputStream in(argv[1]);
    FancyOutputStream out(argv[2]);
    HCTree hufftree;
    cout << in.filesize() << endl;

    /* use FancyInputStream to read file */
    vector<int> counts(256, 0);
    int b;
    while((b = in.read_byte()) != -1){
        counts[(unsigned char)b]++;
    }

    size_t original_size = 0;
    for(auto c : counts){
        original_size += c;
    }
    if(original_size==0){
        return 0;
    }

    /* build Huffman Tree based on counts */
    hufftree.build(counts);

    /* WRITE THE HEADER TO THE fancyOUTPUTstream FIRST */
    for(int i=0; i<256; i++){
        unsigned int count = counts[i];
        out.write_int(count);
    }

    /* THEN WRITE ENCODED VALUES TO fancyOUTPUTstream */
    in.reset();
    int byte;
    while((byte = in.read_byte()) != -1){
        unsigned char symbol = static_cast<unsigned char>(byte);
        //cout << "symbol: " << symbol << endl;
        hufftree.encode(symbol, out);
    }
}