#include "HCTree.hpp"
#include "Helper.hpp"
#include <queue>
#include <stack>
#include <vector>
#include <fstream>

using namespace std;


/* helper function to recursively delete the Huffman Tree when it goes out of scope */
void HCTree::destructHelper(HCNode* node){
    if(!node)
        return;
    destructHelper(node->c0);
    destructHelper(node->c1);
    delete node;
}

/* destructor implementation */
HCTree::~HCTree() {
    destructHelper(root);
}

/* implementation of build() function described in HCTree.hpp */
void HCTree::build(const vector<int>& freqs){
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    /* build disjoint set of single node trees*/
    /* add pointers to each node into the 'leaves' arraylist */
    for(size_t i=0; i<freqs.size(); i++){
        if(freqs[i]>0){
            HCNode* node = new HCNode(freqs[i], (unsigned char) i);
            leaves[i] = node;
            pq.push(node);
        }
    }

    /* build up Huffman Tree */
    while(pq.size() > 1){
        HCNode* left = pq.top();
        pq.pop();
        HCNode* right = pq.top();
        pq.pop();

        HCNode* parent = new HCNode(left, right);
        left->p = parent;
        right->p = parent;

        pq.push(parent);
    }

    /* assign root of HCTree to the object */
    if(!pq.empty()){
        root = pq.top();
    }
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    HCNode* node = leaves[symbol];
    if(!node) 
        return;
    stack<int> bits;

    /* Climb up, push bits onto stack */ 
    while(node->p){
        if (node->p->c0 == node)
            bits.push(0);
        else
            bits.push(1);
        node = node->p;
    }

    /* (root → leaf) */
    while (!bits.empty()) {
        //cout<<bits.top()<<endl;
        out.write_bit(bits.top());
        bits.pop();
    }
}

unsigned char HCTree::decode(FancyInputStream & in) const{
    HCNode* current = root;
    while(current->c0 != nullptr || current->c1 != nullptr){ 
        int bit = in.read_bit();
        //cout<<bit<<endl;
        if(bit == 0)
            current = current->c0;
        else
            current = current->c1;
    }
    return current->symbol;
}