#include<stdio.h>
#include<malloc.h>

typedef struct node node;
typedef node* avlTree;

struct node{
    unsigned char height;
    int key;
    node* left;
    node* right;
};

unsigned short height(node* root){
    return root ? root->height : 0;
}

int bFactor(node* root){
    return height(root->right) - height(root->left);
}

void fixHeight(node* root){
    unsigned char hLeft = height(root->left);
    unsigned char hRight = height(root->right);
    root->height = (hLeft > hRight ? hLeft : hRight) + 1;
}

node* rightTurn(node* root){  ///правый малый поворот
    node* q = root->left;
    root->left = q->right;
    q->right = root;
    fixHeight(root);
    fixHeight(q);
    return q;
}

node* leftTurn(node* root){  /// левый малый поворот
    node* q = root->right;
    root->right = q->left;
    q->left = root;
    fixHeight(root);
    fixHeight(q);
    return q;
}

node* balance(node* root){ /// балансировка узла root
    fixHeight(root);
    if (bFactor(root) == 2){
        if (bFactor(root->right) < 0)
            root->right = rightTurn(root->right);
        return leftTurn(root);
    }
    if (bFactor(root) == -2){
        if (bFactor(root->left) > 0)
            root->left = leftTurn(root->left);
        return rightTurn(root);
    }
    return root; /// балансировка не нужна
}

node* insert(node* root, int k){ /// вставка ключа k в дерево с корнем root
    if(!root){
        root = (node*)malloc(sizeof(node));
        root->key = k;
        root->left = NULL;
        root->right = NULL;
        root->height = 1;
        return root;
    }
    if(k < root->key)
        root->left = insert(root->left, k);
    else
        root->right = insert(root->right, k);
    return balance(root);
}

void dfsFreeAVL(node* p){
    if (p){
        dfsFreeAVL(p->left);
        dfsFreeAVL(p->right);
        free(p);
    }
}

int main(){
    avlTree myTreeHaHa = {NULL};
    int n, nextElem;
    if(!scanf("%d", &n))
        return 0;
    for (int i = 0; i < n; i++){
        if(!scanf("%d", &nextElem))
            return 0;
        myTreeHaHa = insert(myTreeHaHa, nextElem);
    }
    printf("%d", height(myTreeHaHa));
    dfsFreeAVL(myTreeHaHa);
}
