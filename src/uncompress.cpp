#include <iostream>
#include <vector>
#include <fstream>
#include "Helper.hpp"
#include "HCTree.hpp"

int main(int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>" << endl;
        return 1;
    }

    FancyInputStream in(argv[1]);
    if(!in.good()){
        cerr << "error opening input stream" << endl;
        return 1;
    }

    FancyOutputStream out(argv[2]);

    vector<int> freqs(256, 0);
    HCTree hufftree;

    /* read header */
    for(int i=0; i<256; i++){
        freqs[i] = in.read_int();
    }

    // print counts 
    //for (int i = 0; i < 256; i++) {
    //    if (freqs[i] > 0) {
    //        cout << i << " : " << freqs[i] << "\n";
    //    }
    //}
    hufftree.build(freqs);

    size_t original_size = 0;
    for(auto c : freqs){
        original_size += c;
    }
    
    for(size_t i = 0; i < original_size; ++i){
        unsigned char symbol = hufftree.decode(in); 
        //cout << "symbol: " << symbol << endl;
        out.write_byte(symbol); 
    }
    return 0;
}