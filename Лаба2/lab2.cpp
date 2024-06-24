#include <iostream>
#include <string>
#include <queue>
#include <fstream>

using namespace std;

struct TAVLTreeNode {
    string key;
    unsigned long long value;
    short balance;
    TAVLTreeNode *left;
    TAVLTreeNode *right;
    TAVLTreeNode *parent;
    TAVLTreeNode(string key1, unsigned long long value1, TAVLTreeNode *left1, TAVLTreeNode *right1, TAVLTreeNode *parent1, short balance1) {
        key = key1;
        value = value1;
        left = left1;
        right = right1;
        parent = parent1;
        balance = balance1;
    }
};

class TAVLTree {
public:
    TAVLTree() {}
    ~TAVLTree();
    void Insert(string key, unsigned long long value, bool &status);
    void Search(string key);
    void Delete(string key);
    void Print();
    void Save(string filename);
    void Load(string filename);
private:
    bool Insert(string key, unsigned long long value, TAVLTreeNode * node, bool &status);
    bool Search(string key, TAVLTreeNode * node );
    void LeftRotate(TAVLTreeNode *node);
    void RightRotate(TAVLTreeNode *node);
    bool BalancingForInsert(TAVLTreeNode *node);
    bool BalancingForDelete(TAVLTreeNode *node);
    bool GoingDeeper(TAVLTreeNode* node, string& key, unsigned long long& value);
    bool Delete(string key, TAVLTreeNode * node);
private:
    TAVLTreeNode *root = nullptr;
};


void DestroyTAVLTreeNode(TAVLTreeNode *x){ 
    x->left = nullptr;
    x->right = nullptr;
    x->parent = nullptr;
    delete x;
}

void DestroyTAVLTree(TAVLTreeNode *x){
    if(x == nullptr){
        return;
    }
    DestroyTAVLTree(x->left);
    DestroyTAVLTree(x->right);
    DestroyTAVLTreeNode(x);
    return;
}

TAVLTree::~TAVLTree() {
    DestroyTAVLTree(root);
}

void TAVLTree::LeftRotate(TAVLTreeNode *x){
    TAVLTreeNode* y = new TAVLTreeNode(x->right->key, x->right->value, x->right->left, x->right->right, x->right->parent, x->right->balance);
    if(x->right->right != nullptr){
        x->right->right->parent = y;
    }
    if(x->right->left != nullptr){
        x->right->left->parent = y;
    }
    TAVLTreeNode* tmp = x->right;
    x->right = y->left;
    if(y->left != nullptr){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == nullptr){
        root = y;
    } else if(x->parent->left != nullptr && x->parent->left->key == x->key){
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    DestroyTAVLTreeNode(tmp);
}

void TAVLTree::RightRotate(TAVLTreeNode *y){
    TAVLTreeNode* x = new TAVLTreeNode(y->left->key, y->left->value, y->left->left, y->left->right, y->left->parent, y->left->balance);
    if(y->left->right != nullptr){
        y->left->right->parent = x;
    }
    if(y->left->left != nullptr){
        y->left->left->parent = x;
    }
    TAVLTreeNode* tmp = y->left;
    y->left = x->right;
    if(x->right != nullptr){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == nullptr){
        root = x;
    } else if(y->parent->left != nullptr && y->parent->left->key == y->key){
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
    DestroyTAVLTreeNode(tmp);
}

void TAVLTree::Insert(string key, unsigned long long value, bool &status) {
    // Если дерево пустое, тогда создадим корень.
    if(root == nullptr) {
        root = new TAVLTreeNode(key, value, nullptr, nullptr, nullptr, 0);
        status = true;
        return;
    }
    // Иначе -- добавим в дерево.
    Insert(key, value, root, status);
}

bool TAVLTree::BalancingForInsert(TAVLTreeNode *node){
    if(node->parent->left != nullptr && node->parent->left->key == node->key) { // если от родителя мы пошли налево, то прибавляем в баланс родителя
        node->parent->balance++;
    } else { // иначе, убавляем от баланса
        node->parent->balance--;
    }
    if(node->parent->balance == 2) { // перевороты делаем либо когда у родителя баланс 2, либо когда -2
        if(node->balance == 1){ // в текущей ноде баланс может быть либо 1, либо -1 (при нуле мы бы вернули true и не зашли бы сюда)
            node->balance = 0;
            node->parent->balance = 0;
            RightRotate(node->parent);
        } else { // -1
            short a = node->right->balance;
            LeftRotate(node);
            RightRotate(node->parent->parent);
            if(a == -1){
                node->balance = 1;
            } else {
                node->balance = 0;
            }
            if(a == 1){
                node->parent->right->balance = -1;
            } else {
                node->parent->right->balance = 0;
            }
            node->parent->balance = 0;
        }
        return true;
    } else if(node->parent->balance == -2) {
        if(node->balance == -1){
            node->balance = 0;
            node->parent->balance = 0;
            LeftRotate(node->parent);
        } else { // 1
            short a = node->left->balance;
            RightRotate(node);
            LeftRotate(node->parent->parent);
            if(a == 1){
                node->balance = -1;
            } else {
                node->balance = 0;
            }
            if(a == -1){
                node->parent->left->balance = 1;
            } else {
                node->parent->left->balance = 0;
            }
            node->parent->balance = 0;
        }
        return true;
    }
    return false; // при любых поворотах у родителя будет баланс 0, поэтому возвращаем false при 1 или -1
}

bool TAVLTree::Insert( string key, unsigned long long value, TAVLTreeNode *node, bool &status ) {
    bool test = false;  // флаг, показывающий, нужно ли ещё работать с балансами. при true перестаём их трогать
    if(node->key == key) {  // ключ уже есть в дереве
        status = false;
        return true;
    } else if(key < node->key && node->left == nullptr) {  
        node->left = new TAVLTreeNode(key, value, nullptr, nullptr, node, 0);
        status = true;
        node->balance++;
    } else if (key < node->key && node->left != nullptr) {
        test = Insert(key, value, node->left, status);
    } else if(key > node->key && node->right == nullptr) {
        node->right = new TAVLTreeNode(key, value, nullptr, nullptr, node, 0);
        status = true;
        node->balance--;
    } else if (key > node->key && node->right != nullptr) {
        test = Insert(key, value, node->right, status);
    }
    if((test) || node->balance == 0 || node->parent == nullptr){ // либо ранее встретили ноль, либо стоим на нуле, либо мы уже в корне
        return true;
    }
    return BalancingForInsert(node); // балансируем дерево
}

void PrintAVLNodeElems(TAVLTreeNode* x, int h){
    if(x!=nullptr){
        PrintAVLNodeElems(x->right,h+1);
        for(int i = 0 ; i < h ; i++){
            cout << '\t';
        }
        cout << x->value << '\n';
        PrintAVLNodeElems(x->left,h+1);
    }
}


void TAVLTree::Print(){
    PrintAVLNodeElems(root,0);
}

bool TAVLTree::Search(string key, TAVLTreeNode * node){
    if(node == nullptr){
        return false;
    }
    if(node->key == key){
        cout << "OK: " << node->value << '\n';
        return true;
    }
    bool tmp;
    if(key < node->key){
        tmp = Search(key, node->left);
    } else {
        tmp = Search(key, node->right);
    }
    return tmp;
}

void TAVLTree::Search(string key){
    bool tmp = Search(key, root);
    if(!tmp){ // если не нашли искомый ключ
        cout << "NoSuchWord\n";
    }
    return;
}

bool TAVLTree::BalancingForDelete(TAVLTreeNode* node){
// тоже, что и с добавлением: делаем перестановки тогда, когда в ноде баланс 2 или -2 (тут я уже смотрю на саму ноду, а не на родителя, потому что балансы по проходу сразу менял)
// тут уже случев может быть три: либо у ребёнка баланс 0, либо 1, либо -1
    if(node->balance == 2) { 
        if(node->left->balance != 0){
            if(node->left->balance == 1){
                node->left->balance = 0;
                node->balance = 0;
                RightRotate(node);
            } else { // -1
                short a = node->left->right->balance;
                LeftRotate(node->left);
                RightRotate(node);
                if(a == -1){
                    node->parent->left->balance = 1;
                } else {
                    node->parent->left->balance = 0;
                }
                if(a == 1){
                        node->balance = -1;
                } else {
                        node->balance = 0;
                }
                node->parent->balance = 0;
            }
        } else { // при нуле у родителя мы не меняем баланс, поэтому спокойно можем вернуть true
            node->left->balance = -1;
            node->balance = 1;
            RightRotate(node);
            return true;
        }
    } else if(node->balance == -2) {
        if(node->right->balance != 0){
            if(node->right->balance == -1){
                node->right->balance = 0;
                node->balance = 0;
                LeftRotate(node);
            } else { // 1
                short a = node->right->left->balance;
                RightRotate(node->right);
                LeftRotate(node);
                if(a == -1){
                    node->balance = 1;
                } else {
                    node->balance = 0;
                }
                if(a == 1){
                    node->parent->right->balance = -1;
                } else {
                    node->parent->right->balance = 0;
                }
                node->parent->balance = 0;
            }
        } else { // 0
            node->right->balance = 1;
            node->balance = -1;
            LeftRotate(node);
            return true;
        }
    }
    return false;
}

bool TAVLTree::GoingDeeper(TAVLTreeNode* node, string& key, unsigned long long& value){ // замена удаляемой ноде
    bool test = false;
    if(node->left == nullptr){
        key = node->key;
        value = node->value; // заменяем значения нашей ноде на ту, на которую заменяем её
        if(node->parent->left != nullptr && node->parent->left->key == node->key){
            node->parent->left = node->right;
        } else {
            node->parent->right = node->right;
        }
        if(node->right != nullptr){
            node->right->parent = node->parent;
        }
        DestroyTAVLTreeNode(node);
        return false;
    } else {
        node->balance--;
        test = GoingDeeper(node->left, key, value); // замена = самый левый в правом поддереве
        if(test){
            node->balance++;
        }
    }
    if(node->balance == 2 || node->balance == -2){
        return BalancingForDelete(node);
    }
    if((test) || node->balance == 1 || node->balance == -1){
        return true;
    }
    return false;
}

void TAVLTree::Delete(string key) {
    if(root == nullptr) { // если дерево пустое
        cout << "NoSuchWord" << '\n';
        return;
    }
    if(root->key == key) { // если удалить надо корень
        if(root->right == nullptr){ //если у корня нет правого ребёнка, то корнем станет его левый сын, либо дерево просто станет пустым
            TAVLTreeNode *tmp = root->left;
            DestroyTAVLTreeNode(root);
            root = tmp;
            if(tmp != nullptr){
                    root->parent = nullptr;
            }
            cout << "OK" << '\n';
            return;
        }
        root->balance++; // берём замену из правой ветви, поэтому прибавляем к балансу
        bool test = GoingDeeper(root->right,root->key,root->value); // ищем замену корню
        if(test){ // если балансы уже менять не надо, возвращаем всё как есть
            root->balance--;
        }
        test = BalancingForDelete(root); // при удалении баланс текущей ноды мог поменяться, поэтому проверим её на балансировку
        cout << "OK" << '\n';
        return;
    }
    Delete(key, root); // иначе ищем в дерево ноду, которую надо удалить
}

bool TAVLTree::Delete( string key, TAVLTreeNode *node ) {
    bool test = false; // флаг, показывающий, нужно ли балансировать
    if(node == nullptr) { // если удалять нечего
        cout << "NoSuchWord" << '\n';
        return true;
    } else if(node->key < key) {
        node->balance++; // удаляем ноду справа, значит баланс увеличится
        test = Delete(key, node->right);
        if(test){ // если балансы в порядке
            node->balance--;
        }
        if((test) && node->balance != 2 && node->balance != -2){ // при переворотах баланс текущей ноды мог поменяться
            return true;
        }
    } else if(node->key > key) {
        node->balance--; // удаляем ноду слева, значит баланс уменьшится
        test = Delete(key, node->left);
        if(test){
            node->balance++;
        }
        if((test) && node->balance != 2 && node->balance != -2){
            return true;
        }
    } else {
        if(node->left == nullptr && node->right == nullptr){
            if(node->parent->left != nullptr && node->parent->left->key == node->key){ // если пошли от родителя влево, то зануляем ссылку на его левого ребёнка
                node->parent->left = nullptr;
            } else { // иначе, зануляем ссылку на правого
                node->parent->right = nullptr;
            }
            cout << "OK" << '\n';
            DestroyTAVLTreeNode(node); 
            return test;
        } else if(node->left == nullptr){
// если пошли от родителя влево, то заменяем ссылку на его левого ребёнка ссылкой на левого ребёнка текущей ноды
            if(node->parent->left != nullptr && node->parent->left->key == node->key){ 
                node->parent->left = node->right;
                node->right->parent = node->parent;
            } else { // иначе заменяем ссылку на его правого ребёнка ссылкой на левого ребёнка текущей ноды
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            cout << "OK" << '\n';
            DestroyTAVLTreeNode(node);
            return test;
        } else if(node->right == nullptr){
            if(node->parent->left != nullptr && node->parent->left->key == node->key){
                node->parent->left = node->left;
                node->left->parent = node->parent;
            } else {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
            cout << "OK" << '\n';
            DestroyTAVLTreeNode(node);
            return test;
        } else {
            node->balance++;
            test = GoingDeeper(node->right,node->key,node->value); // ищем замену ноде, которую надо удалить
            if(test){
               node->balance--;
            }
            cout << "OK" << '\n';
        }
    }
    if(node->balance == 2 || node->balance == -2){
        return BalancingForDelete(node);
    }
    if((test) || node->balance == 1 || node->balance == -1 || node->parent == nullptr){
        return true;
    }
    return false;
}


void TAVLTree::Save(string filename){
    cout << "OK\n";
    ofstream TAVLTree (filename); // создаём файл с указанным именем и открываем его на поток вывода
    if(root == nullptr){ // если дерево пустое, то ничего не выводим и закрываем поток вывода
        TAVLTree.close();
        return;
    }
    queue<TAVLTreeNode *> q;
    q.push(root);
    TAVLTreeNode * tmp;
    while(q.size() != 0){ // записываем бфсом все ноды в файл
        tmp = q.front();
        q.pop();
        if(tmp->right != nullptr){
            q.push(tmp->right);
        }
        if(tmp->left != nullptr){
            q.push(tmp->left);
        }
        TAVLTree << tmp->key << " " << tmp->value << " ";
    }
    TAVLTree << '\n';
    TAVLTree.close(); // закрываем поток вывода
}

void TAVLTree::Load(string filename){
    cout << "OK\n";
    if(root != nullptr){ // удаляем всё дерево
        DestroyTAVLTree(root->right);
        DestroyTAVLTree(root->left);
        delete root;
        root = nullptr;
    }
    ifstream TAVLTree (filename); 
    string key;
    unsigned long long value;
    bool non; // в данном случае у нас всегда будет OK
    while(TAVLTree >> key >> value){
        Insert(key, value, non);
    }
    TAVLTree.close();
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    TAVLTree T; 
    string cmd, key; 
    unsigned long long value;
    short size; // для убережения от отсутствия конфликтов в форе
    bool status; // флаг для инсерта
    string cmd2, filename; // команда для работы с файлами и название файла, с которым будет производиться работа
    while(cin >> cmd){
        if(cmd[0] == '+'){
            cin >> key >> value;
            size = key.size();
            for(int i=0;i<size;i++){ //приводим все буквы к нижнему регистру
                key[i] = tolower(key[i]);
            }
            T.Insert(key, value, status);
            if(status){
                cout << "OK\n";
            } else {
                cout << "Exist\n";
            }
        } else if(cmd[0] == '-'){
            cin >> key;
            size = key.size();
            for(int i=0;i<size;i++){
                key[i] = tolower(key[i]);
            }
            T.Delete(key);
        } else if(cmd[0] == '!'){
            cin >> cmd2 >> filename;
            if(cmd2 == "Save"){
                T.Save(filename);
            } else {
                T.Load(filename);
            }
        } else {
            key = cmd;
            size = key.size();
            for(int i=0;i<size;i++){
                key[i] = tolower(key[i]);
            }
            T.Search(key);
        }
   //   T.Print();
    }
}
