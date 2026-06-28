#ifndef HCTREE_HPP
#define HCTREE_HPP
#include <queue>
#include <vector>
#include <fstream>
#include "Helper.hpp"
using namespace std;

/**
 * A Huffman Code Node class
 
class HCNode1{
    public:
        unsigned char byte;
        int freq;
        HCNode1* left;
        HCNode1* right;
        HCNode1* parent;

        Constructor that assigns a Node to have a byte and byte frequency, only leaves can use this 
        HCNode1(unsigned char byte, int freq) : byte(byte), freq(freq), left(nullptr), right(nullptr), parent(nullptr) {}

        Constructor that assigns two children with an empty root 
        HCNode1(HCNode1* left, HCNode1* right) : byte(0), freq(left->freq + right->freq), left(left), right(right), parent(nullptr) {}
};
*/

/**
 * A Huffman Node Comparator class to implement min-heap

class nodeFreqComp{
    public:
        bool operator()(HCNode1* a, HCNode1* b) const{
            return a->freq > b->freq;
        }
}; */

/**
 * A Huffman Code Tree class
 */
class HCTree {
    private:
        HCNode* root;
        vector<HCNode*> leaves;

        /* Use post-order traversal to recursively delete all HCNodes */
        void destructHelper(HCNode* node);

    public:
        /**
         * Constructor, which initializes everything to null pointers
         */
        HCTree() : root(nullptr) {
            leaves = vector<HCNode*>(256, nullptr);
        }

        /* need to write a destructor */
        ~HCTree();

        /**
         * Use the Huffman algorithm to build a Huffman coding tree.
         * PRECONDITION: freqs is a vector of ints, such that freqs[i] is the frequency of occurrence of byte i in the input file.
         * POSTCONDITION: root points to the root of the trie, and leaves[i] points to the leaf node containing byte i.
         */
        void build(const vector<int>& freqs);

        /**
         * Write to the given FancyOutputStream the sequence of bits coding the given symbol.
         * PRECONDITION: build() has been called, to create the coding tree, and initialize root pointer and leaves vector.
         */
        void encode(unsigned char symbol, FancyOutputStream & out) const;

        /**
         * Return symbol coded in the next sequence of bits from the stream.
         * PRECONDITION: build() has been called, to create the coding tree, and initialize root pointer and leaves vector.
         */
        unsigned char decode(FancyInputStream & in) const;
};
#endif // HCTREE_HPP