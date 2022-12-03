#include "ConcatStringTree.h"

int Max(const int& a, const int& b) {return (a > b) ? a : b;}
//==========================Node==============================
Node::Node(int lLen, int _Len, char* _data, Node* l, Node* r)
{
    leftLen = lLen;
    Len = _Len;
    data = _data;
    left = l;
    right = r;
    P_root = new ParentsTree();
}
void Node::Del_N()
{
    this->leftLen = this->Len = 0;
    this->data = nullptr;
    this->left = nullptr;
    this->right = nullptr;
    this->P_root = nullptr;

    delete this;
}
//==============================================================================
//==========================ConcatStringTree====================================
int ConcatStringTree::ID = 1;
ConcatStringTree::ConcatStringTree(const char * s)
{
    int len = 0;
    for(;s[len] != '\0'; ++len);
    char* ar = new char[len + 1];
    for(int i = 0; i < len; ar[i] = s[i], ++i);
    ar[len] = '\0';

    this-> root = new Node(0, len, ar, nullptr, nullptr);
    if(ConcatStringTree::ID > 1e7) throw std::overflow_error("Id is overflow!");

    this->root->id = (ConcatStringTree::ID)++;
    this->root->P_root->add(this->root->id);
}
ConcatStringTree::ConcatStringTree(Node* r)
{
    this-> root= r;
    if(ConcatStringTree::ID > 1e7) throw std::overflow_error("Id is overflow!");

    this ->root->id = (ConcatStringTree::ID)++;
    this->root->P_root->add(this->root->id);
}
//LENGTH
int ConcatStringTree::length() const {return (this->root) ? this->root->Len : 0;}
//GET
char ConcatStringTree::get(int index)
{
    if(index < 0 || index >= this->length()) throw std::out_of_range("Index of string is invalid!");
    Node* curr = this->root;

    while(!curr->data)
    {
        if(index < curr->leftLen) curr = curr->left;
        else{
            index -= curr ->leftLen;
            curr = curr -> right;
        }
    }
    return curr->data[index];
}
//INDEX OF
int ConcatStringTree::indexOf(char c)
{
    string a = this->getString();
    for(int i = 0 ; i < a.length(); ++i) {if(a[i] == c) return i;}

    return -1;
}
//TOSTRING
void ConcatStringTree::_toString(Node* root, stringstream& ss) const
{
    if(!root) return;
    if(root->data != nullptr) {ss << root->data; return;}
    _toString(root->left, ss);
    _toString(root->right, ss);
}

string ConcatStringTree::getString() const
{
    stringstream ss;
    this->_toString(this->root, ss);
    return ss.str();
}
string ConcatStringTree::toString() const
{
    stringstream ss;
    ss << "ConcatStringTree[\"";
    this ->_toString(this->root, ss);
    ss << "\"]";

    return ss.str();
}

void ConcatStringTree::_toStringPreOrder(Node* root, stringstream& ss) const
{
    if(!root) return;
    if(root->data != nullptr) {
        ss << "(LL=" << root->leftLen
           << ",L=" << root -> Len << ",\"" << root->data  << "\");";
        return;
    }

    ss << "(LL=" << root->leftLen << ",L=" << root -> Len << ",<NULL>);";
    _toStringPreOrder(root->left, ss);
    _toStringPreOrder(root->right, ss);
}

string ConcatStringTree::toStringPreOrder() const
{
    stringstream ss;
    ss << "ConcatStringTree[";
    this->_toStringPreOrder(this->root, ss);

    string r = ss.str(); if(r[r.length()-1]==';') {r.pop_back();}
    r += ']';
    return r;
}
//CONCAT
ConcatStringTree ConcatStringTree::concat(const ConcatStringTree& other) const
{
    ConcatStringTree* res = nullptr;
    if(this->root && other.root) {res = new ConcatStringTree(new Node(this->length(), this->length() + other.length(), nullptr, this->root, other.root));}
    else{
        if(!this->root) {res = new ConcatStringTree(new Node(0, other.length(), nullptr, nullptr, other.root));}
        else {res = new ConcatStringTree(new Node(this->length(), this->length(), nullptr, this->root, nullptr));}
    }

    if(this->root) this->root->P_root->add(res->root->id);
    if(other.root) other.root->P_root->add(res->root->id);

    return *res;
}
//SUBSTRING
Node* ConcatStringTree::_subString(int from, int to, Node* root) const
{
    if(root->data != nullptr)
    {
        char* res = new char[to - from + 2];
        for(int i = from; i <= to; res[i - from] = root->data[i], ++i);
        res[to - from + 1] = '\0';
        return new Node(0, to - from + 1, res, nullptr, nullptr);
    }

    Node* l = nullptr; Node* r = nullptr;
    if(root->leftLen <= from) {
        r = _subString(from - root->leftLen, to - root->leftLen, root->right);
    }
    else if(root->leftLen > to){
        l = _subString(from, to, root->left);
    }
    else{
        l = _subString(from, root->leftLen-1, root->left);
        r = _subString(0, to - root->leftLen, root->right);
    }

    if(!l) {return new Node(0, r->Len, nullptr, nullptr, r);}
    else if(!r) {return new Node(l->Len, l->Len, nullptr, l, nullptr);}
    else {return new Node(l->Len, l->Len + r->Len, nullptr, l, r);}
}
ConcatStringTree ConcatStringTree::subString(int from, int to) const
{
    if(from < 0 || from >= this->length() || to < 0 || to >= this->length())
        throw std::out_of_range("Index of string is invalid!");
    if(from >= to) throw std::logic_error("Invalid range!");

    ConcatStringTree* res = new ConcatStringTree(this->_subString(from, to - 1, this->root));
    return *res;
}
//REVERSE
 Node* ConcatStringTree::_reverse(Node* root) const
 {
     if(!root) return nullptr;
     if(root->data != nullptr) {
        //reverse const char
        int s = root->Len;
        char* res = new char[s+1];
        for(int i = 0; i < s; ++i){res[i] = root->data[s - i - 1];}
        res[s] = '\0';

        return new Node(0,root->Len, res, nullptr, nullptr);
     }
     Node* l = _reverse(root->right);
     Node* r = _reverse(root->left );

     if(l && r) {return new Node(l->Len, l->Len + r->Len, nullptr, l, r);}
     else{
        if(!l) {return new Node(0, r->Len, nullptr, nullptr, r);}
        else {return new Node(l->Len, l->Len, nullptr, l, nullptr);}
     }
 }
 ConcatStringTree ConcatStringTree::reverse() const
 {
    ConcatStringTree* res = new ConcatStringTree(this->_reverse(this->root));
    return *res;
 }
 int ConcatStringTree::getParTreeSize(const string & query) const
 {
    Node* curr = this->root;
    for(int i =0; i < (int)query.length(); ++i)
    {
        if(query[i] != 'l' && query[i] != 'r') throw std::runtime_error("Invalid character of query");
        if(!curr) throw std::runtime_error("Invalid query: reaching NULL");
        if(query[i] == 'l') {curr = curr->left;}
        else {curr = curr->right;}
    }

    return curr-> P_root-> SizeTree();
 }

 string ConcatStringTree::getParTreeStringPreOrder(const string & query) const
 {
    Node* curr = this->root;
    for(int i =0; i < (int)query.length(); ++i)
    {
        if(query[i] != 'l' && query[i] != 'r') throw std::runtime_error("Invalid character of query");
        if(!curr) throw std::runtime_error("Invalid query: reaching NULL");
        if(query[i] == 'l') {curr = curr->left;}
        else {curr = curr->right;}
    }
    return curr->P_root->toStringPreOrder();
 }
void ConcatStringTree::Del_Node(Node* c)
{
    if(!c) return;
    if(c->P_root == nullptr) return;
    if(c == this->root) c->P_root->Del(c->id);
    if(c->P_root->SizeTree() == 0) {
        if(c->left  && c->left->P_root->SizeTree()  != 0){c->left ->P_root->Del(c->id);}
        if(c->right && c->right->P_root->SizeTree() != 0){c->right->P_root->Del(c->id);}
        Del_Node(c->left);
        Del_Node(c->right);
        c->Del_N();
    }
    return;
}

ConcatStringTree::~ConcatStringTree()
{
    Del_Node(this->root);
}
 //=========================================================================
 //=======================================ParentTree========================
 AVL_Node* Node::ParentsTree::getMaximumNode(AVL_Node* n) const
 {
    AVL_Node* curr = n;
    while(curr && curr->r) {curr = curr->r;}
    return curr;
 }
 int Node::ParentsTree::getHeight(AVL_Node* n) const
 {
    return (n) ? n->height : 0;
 }
 int Node::ParentsTree::getBalanceFactor(AVL_Node* n) const
 {
     if(!n) return 0;
     return this->getHeight(n->l) - this->getHeight(n->r);
 }
 //ROTATE
 AVL_Node* Node::ParentsTree::rotateLeft(AVL_Node* y)
 {
    AVL_Node* x = y->r;
    AVL_Node* z = x->l;
    x->l = y;
    y->r = z;
    x->height = 1 + Max(getHeight(x->l), getHeight(x->r));
    y->height = 1 + Max(getHeight(y->l), getHeight(y->r));

    return x;
 }
 AVL_Node* Node::ParentsTree::rotateRight(AVL_Node* y)
 {
    AVL_Node* x = y->l;
    AVL_Node* z = x->r;
    x->r = y;
    y->l = z;
    x->height = 1 + Max(getHeight(x->l), getHeight(x->r));
    y->height = 1 + Max(getHeight(y->l), getHeight(y->r));

    return x;
 }
 //ADD
 AVL_Node* Node::ParentsTree::_add(int _id, AVL_Node* root)
 {
    if(!root) {return new AVL_Node(_id);}
    if(_id < root->data) {root->l = _add(_id, root->l);}
    else if(_id > root->data) {root-> r = _add(_id, root->r);}
    else return root;

    root->height = 1 + Max(getHeight(root->l), getHeight(root->r));
    int bFactor = getBalanceFactor(root);

    if(bFactor > 1)
    {
        if(_id < root->l->data) {return rotateRight(root);}
        else if(_id > root->l->data)
        {
            root->l = rotateLeft(root->l);
            return rotateRight(root);
        }
    }
    else if(bFactor < -1)
    {
        if(_id > root->r->data) {return rotateLeft(root);}
        else if(_id < root->r->data)
        {
            root->r = rotateRight(root->r);
            return rotateLeft(root);
        }
    }
    return root;
 }

 void Node::ParentsTree::add(int _id) {this->root = this->_add(_id, this->root); ++this->Size_tree; return;}

 int Node::ParentsTree::SizeTree() const { return this->Size_tree;}
 void Node::ParentsTree::_toStringPreOrder(AVL_Node* r, stringstream& ss) const
 {
    if(!r) return;
    ss << "(id=" << r->data << ");";
    _toStringPreOrder(r->l, ss);
    _toStringPreOrder(r->r, ss);
 }

 string Node::ParentsTree::toStringPreOrder() const
 {
    stringstream ss;
    ss << "ParentsTree[";
    this->_toStringPreOrder(this->root, ss);
    string s = ss.str(); if(s[s.length()-1] == ';') {s.pop_back();}
    s += ']';

    return s;
 }
 //DEL
 AVL_Node* Node::ParentsTree::_Del(AVL_Node* Tree, int _id)
 {
    if(!Tree) return Tree;
    else if(_id < Tree->data) {Tree->l = _Del(Tree->l, _id);}
    else if(_id > Tree ->data) {Tree->r = _Del(Tree->r, _id);}
    else{
        if(!Tree ->l || !Tree->r)
        {
            AVL_Node* c = (Tree->l) ? Tree->l : Tree->r;
            if(!c){
                AVL_Node* c = Tree;
                Tree = nullptr;
            }
            else{*Tree = *c;}
            delete c;
            --this->Size_tree;
        }
        else{
            AVL_Node* c = this->getMaximumNode(Tree->l);
            Tree->data = c->data;
            Tree->l = _Del(Tree->l, c->data);
        }
    }
    if(!Tree) return Tree;

    //Update the balance
    if(this->getBalanceFactor(Tree) > 1)
    {
        if(this->getBalanceFactor(Tree->l) > 1){return this->rotateRight(Tree);}
        else if(this->getBalanceFactor(Tree->l) < -1)
        {
            Tree->l = this->rotateLeft(Tree->l);
            return this->rotateRight(Tree);
        }
    }
    else if(this->getBalanceFactor(Tree) < -1)
    {
        if(this->getBalanceFactor(Tree->r) < -1){return this->rotateLeft(Tree);}
        else if(this->getBalanceFactor(Tree->r) > 1)
        {
            Tree->r = this->rotateRight(Tree->r);
            return this->rotateLeft(Tree);
        }
    }
    return Tree;
 }

 void Node::ParentsTree::Del(int _id)
 {
    this->root = this->_Del(this->root, _id);
    return;
 }

//==============================HASH CONFIG ======================================
//==============================HASH CONFIG ======================================
HashConfig::HashConfig(int _p,
                       double _c1,
                       double _c2,
                       double _lambda,
                       double _alpha,
                       int Size)
{
    this->p = _p;
    this->c1 = _c1;
    this->c2 = _c2;
    this->lambda = _lambda;
    this->alpha = _alpha;
    this->initSize = Size;
}
//================================================================================
LitStringHash::LitStringHash(const HashConfig& hashConfig){
   p = hashConfig.p;
   c1 = hashConfig.c1;
   c2 = hashConfig.c2;
   lambda = hashConfig.lambda;
   alpha = hashConfig.alpha;
   numsOfBuckets = hashConfig.initSize;
   lastIndex = -1;
   noEle = 0;
   lfactor = 0;

   this->Init();
}

void LitStringHash::Init()
{
   table = new Element*[numsOfBuckets];
   stage = new State[numsOfBuckets];
   for(int i = 0; i < numsOfBuckets; table[i] = nullptr,stage[i] = State::NIL, ++i);
}

int LitStringHash::Hashing(int Size, string s) const{
    int sum = 0, pro_p = 1;
    for(int i = 0; i < (int)s.length(); ++i)
    {
        sum += pro_p*s[i];
        pro_p *= this->p;
    }
    return sum % Size;
}

int LitStringHash::Hashing_hp(int Size, string s, int i) const{
    int res = Hashing(Size, s) + c1*i + c2*i*i;
    return res % Size;
}

void LitStringHash::reHashing(){
    int new_size = alpha*numsOfBuckets;
    Element** new_table = new Element*[new_size];
    State* new_stage = new State[new_size];

    for(int i = 0; i < new_size;
        new_table[i] = nullptr, new_stage[i] = State::NIL, ++i);

    for(int i = 0; i < numsOfBuckets; ++i)
    {
        if(stage[i] == State::NON_EMPTY)
        {

            int j = 0;
            while(j < new_size)
            {
                int idx = this->Hashing_hp(new_size, table[i]->key, j);
                if(new_stage[idx] != State::NON_EMPTY)
                {
                    new_table[idx] = table[i];
                    new_stage[idx] = State::NON_EMPTY;
                    this->lastIndex = idx;
                    break;
                }
                ++j;
            }
        }
    }
    this->numsOfBuckets = new_size;
    this->table = new_table;
    this->stage = new_stage;
}

string LitStringHash::toString() const{

    stringstream ss;
    ss << "LitStringHash[";
    for(int i = 0; i < numsOfBuckets; ++i)
    {
        if(stage[i] != State::NON_EMPTY) {ss << "();";}
        else {
            ss << "(litS=\"" << table[i]->key << "\");";
        }
    }
    string res = ss.str();
    if(res[res.length()-1] == ';') {res.pop_back();}
    res += ']';
    return res;
}

int LitStringHash::getLastInsertedIndex() const{
    return lastIndex;
}

int LitStringHash::get_idx(const char* s) const
{
    int idx = -1;
    int i = 0;

    while(i < this->numsOfBuckets)
    {
        idx = this->Hashing_hp(numsOfBuckets, s, i);
        if(table[idx] && table[idx]->key == (string)s) return idx;
        ++i;
    }
    if(i == this->numsOfBuckets) idx = -1;
    return idx;
}

Element* LitStringHash::get(int idx) const{
    return this->table[idx];
}

void LitStringHash::insert(char *s, Node* c){
    if(!this->table){this->Init();}
    if(s == nullptr) return;
    int indexOfDeleted = -1;
    int i = 0;

    while(i < this->numsOfBuckets)
    {
        int idx = this->Hashing_hp(numsOfBuckets, s, i);
        if(stage[idx] == State::NIL && indexOfDeleted != -1) {break;}
        else if(stage[idx] == State::NIL && indexOfDeleted == -1 || stage[idx] == State::NON_EMPTY && table[idx]->connect == 0)
        {
            table[idx] = new Element(s, c);
            stage[idx] = State::NON_EMPTY;
            this->lastIndex = idx;
            break;
        }
        else{
            if(stage[idx] == State::DELETED && indexOfDeleted == -1)
            { indexOfDeleted = idx;}
            else{
                if(table[idx] && table[idx]->key == (string)s) {
                    return;
                }
            }
        }
        ++i;
    }
    if(i == numsOfBuckets && indexOfDeleted == -1)
        throw runtime_error("No possible slot");

    if(indexOfDeleted != -1)
    {
        table[indexOfDeleted] = new Element(s, c);
        stage[indexOfDeleted] = State::NON_EMPTY;
        this->lastIndex = indexOfDeleted;
    }

    this->noEle += 1;
    this->lfactor = (this->noEle*1.0)/(int)(this->numsOfBuckets);

    if(this->lfactor > this->lambda) {this->reHashing();}

    return;
}

void LitStringHash::remove(char *s){
    if(!this->table) this->Init();
    int i = 0;
    while(i < numsOfBuckets)
    {
        int idx = this->Hashing_hp(numsOfBuckets,s, i);
        if(stage[idx] == State::NIL) {break;}
        if(table[idx] && table[idx]->key == (string)s){
            if(table[idx]->connect == 0){
                delete table[idx];
                stage[idx] = State::DELETED;
                table[idx] = nullptr;
                --this->noEle;
            }
            break;
        }
        ++i;
    }

    if(!this->noEle) {
        delete[] stage;
        for(int i = 0; i < this->numsOfBuckets; ++i) {delete table[i];}
        delete[] table;
        table = nullptr;
        this->lastIndex = -1;
    }
    return;
}

LitStringHash::~LitStringHash(){
    if(this->lastIndex != -1)
    {
        delete[] stage;
        for(int i = 0; i < this->numsOfBuckets; ++i)
        { if(table[i]) delete table[i];}
        delete[] table;
    }
}
//=================================================================================================
int ReducedConcatStringTree::ID = 1;

ReducedConcatStringTree::ReducedConcatStringTree(const char* s, LitStringHash *litStringHash){
    if(ReducedConcatStringTree::ID > 1e7) throw std::overflow_error("Id is overflow!");

    int idx = litStringHash->get_idx(s);
    int len = 0;
    for(;s[len] != '\0'; ++len);

    if(idx == -1){
        char* ar = new char[len + 1];
        for(int i = 0; i < len; ar[i] = s[i],++i); ar[len] = '\0';
        this->root = new Node(0, len, ar, nullptr, nullptr);
        this->root->id = (ReducedConcatStringTree::ID)++;
        this->root->P_root->add(this->root->id);
        litStringHash->insert(ar, this->root);
    }
    else {
        this->root = litStringHash->get(idx)->getValue();
    }
}

ReducedConcatStringTree::ReducedConcatStringTree(Node* r){
    if(ReducedConcatStringTree::ID > 1e7) throw std::overflow_error("Id is overflow!");
    this->root = r;

    this->root->id = (ReducedConcatStringTree::ID)++;
    this->root->P_root->add(this->root->id);
}
ReducedConcatStringTree ReducedConcatStringTree::concat(const ReducedConcatStringTree& other) const{
    ReducedConcatStringTree* res = nullptr;
    if(this->root && other.root) {res = new ReducedConcatStringTree(new Node(this->length(), this->length() + other.length(), nullptr, this->root, other.root));}
    else{
        if(!this->root) {res = new ReducedConcatStringTree(new Node(0, other.length(), nullptr, nullptr, other.root));}
        else {res = new ReducedConcatStringTree(new Node(this->length(), this->length(), nullptr, this->root, nullptr));}
    }

    if(this->root) this->root->P_root->add(res->root->id);
    if(other.root) other.root->P_root->add(res->root->id);

    return *res;
}

//==================================================

