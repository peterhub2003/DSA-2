#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"
class ConcatStringTree;
class LitStringHash;
class ReducedConcatStringTree;

class AVL_Node
{
    private:
        int data;
        AVL_Node* l;
        AVL_Node* r;
        int height;

    public:
        AVL_Node(int _data, AVL_Node* _l = nullptr, AVL_Node* _r = nullptr): data(_data), l(_l), r(_r), height(1) {;}
        friend class ParentsTree;
        friend class Node;
};

class Node
{
public:
    class ParentsTree
    {
    public:

        ParentsTree(): root(nullptr), Size_tree(0) {;}
        //INSERT
        AVL_Node* _add(int _id, AVL_Node* r);
        void add(int _id);
        //Delete
        AVL_Node* _Del(AVL_Node* r, int _id);
        void Del(int _id);
        //Rotate LEFT
        AVL_Node* rotateLeft(AVL_Node* y);
        //Rotate RIGHT
        AVL_Node* rotateRight(AVL_Node* y);

        int SizeTree() const;
        int& getRef() const {return Ref;}
        AVL_Node* getMaximumNode(AVL_Node* n) const;
        int getBalanceFactor(AVL_Node* n) const;
        int getHeight(AVL_Node* n) const;

        void _toStringPreOrder(AVL_Node* r, stringstream& ss) const;
        string toStringPreOrder() const;

    private:
        AVL_Node* root;
        int Size_tree;
        int& Ref = Size_tree;
    };

private:
    int id;
    int leftLen;
    int Len;
    char* data;
    Node* left;
    Node* right;
    ParentsTree* P_root;

public:
    Node(int lLen, int _Len, char* _data, Node* l, Node* r);
    ParentsTree* getP_root() const {return this->P_root;}
    void Del_N();
    friend class ConcatStringTree;
    friend class LitStringHash;
    friend class ReducedConcatStringTree;
};

class ConcatStringTree {
protected:
    Node* root;
    static int ID;
public:

    ConcatStringTree(){root = nullptr;}
    ConcatStringTree(const char * s);
    ConcatStringTree(Node* r);

    int length() const;
    char get(int index);
    int indexOf(char c);

    void _toStringPreOrder(Node* root, stringstream& ss) const;
    string toStringPreOrder() const;

    void _toString(Node* root, stringstream& ss) const;
    string toString() const;
    string getString() const;

    ConcatStringTree concat(const ConcatStringTree & otherS) const;

    Node* _subString(int from, int to, Node* root) const;
    ConcatStringTree subString(int from, int to) const;

    ConcatStringTree reverse() const;
    Node* _reverse(Node* Root) const;

    void Del_Node(Node* c);
    ~ConcatStringTree();

    int getParTreeSize(const string & query) const;
    string getParTreeStringPreOrder(const string & query) const;
};

//==========================================================================================
class ReducedConcatStringTree; // forward declaration

class HashConfig {
public:
    HashConfig(){;}
    HashConfig(int, double, double, double, double, int);
private:
    int p;
    double c1, c2;
    double lambda;
    double alpha;
    int initSize;

    friend class ReducedConcatStringTree;
    friend class LitStringHash;
};

class Element{
private:
    char* key;
    Node* r;
    int& connect = r->getP_root()->getRef();;
public:
    Element(char* k, Node* _r): key(k), r(_r){
        connect = r->getP_root()->getRef();
    }
    char* getKey() const { return key;}
    Node* getValue() const { return r;}

    friend class LitStringHash;
};

class LitStringHash
{
    enum class State{NIL, NON_EMPTY, DELETED};
private:
    int p, numsOfBuckets, noEle;
    double c1, c2, alpha, lambda, lfactor;
    int lastIndex;

    Element** table;
    State* stage;

public:
    LitStringHash(const HashConfig& hashConfig);
    void Init();

    int Hashing(int Size, string s) const;
    int Hashing_hp(int Size, string s, int i) const;
    void reHashing();

    int get_idx(const char *s) const;
    Element* get(int idx) const;
    void insert(char *s, Node* r);
    void remove(char *s);

    int getLastInsertedIndex() const;
    string toString() const;

    ~LitStringHash();
};

class ReducedConcatStringTree: public ConcatStringTree{
public:
    static int ID;
public:
    ReducedConcatStringTree(const char * s, LitStringHash * litStringHash);
    ReducedConcatStringTree(Node* r);
    ReducedConcatStringTree concat(const ReducedConcatStringTree& other) const;

    LitStringHash* litStringHash;
};

#endif // __CONCAT_STRING_TREE_H__
