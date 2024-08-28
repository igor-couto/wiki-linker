#include "Trie.h"
#include <iostream>

#define SIZE 190

Trie::Trie() : root(nullptr) {
    root = createNode();
}

Trie::~Trie() {
    cleanup(root);
}

TrieNode* Trie::createNode() {
    TrieNode* node = nullptr;
    try {
        node = allocateNode();
    } catch (std::bad_alloc& e) {
        std::cerr << "FATAL ERROR: insufficient memory!" << std::endl;
        cleanup(root);
        exit(EXIT_FAILURE);
    }
    node->isKey = false;
    node->son = nullptr;
    return node;
}

TrieNode* Trie::allocateNode() {
    TrieNode* node = new TrieNode();
    return node;
}

TrieNode** Trie::allocateSon() {
    TrieNode** sons = new TrieNode*[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        sons[i] = nullptr;
    }
    return sons;
}

int Trie::keyPosition(unsigned char key) {
    if (key >= 'a' && key <= 'z') {
        return key - 'a';
    } else if (key >= 'A' && key <= 'Z') {
        return key - 'A' + 26;
    } else if (key >= '0' && key <= '9') {
        return key - '0' + 52;
    } else {
        // Extend this as needed for other characters
        return -1;
    }
}

int Trie::hashOne(unsigned char key) {
    return key % SIZE;
}

int Trie::hashTwo(unsigned char key) {
    return (key / SIZE) % SIZE;
}

TrieNode* Trie::searchTrie(const std::string& key) {
    return searchTrie(root, key, 0);
}

TrieNode* Trie::searchTrie(TrieNode* node, const std::string& key, size_t startPos) {
    TrieNode* current = node;
    size_t pos = startPos;
    
    while (current && pos < key.length()) {
        int index = keyPosition(static_cast<unsigned char>(key[pos]));
        if (index == -1 || !current->son || !current->son[index]) {
            return nullptr;
        }
        current = current->son[index];
        ++pos;
    }
    
    if (current && current->isKey) {
        return current;
    }
    return nullptr;
}

TrieNode* Trie::searchNodeInsertion(TrieNode* node, const std::string& key, size_t startPos) {
    TrieNode* current = node;
    size_t pos = startPos;
    
    while (pos < key.length()) {
        int index = keyPosition(static_cast<unsigned char>(key[pos]));
        if (index == -1) {
            ++pos;
            continue;
        }
        
        if (!current->son) {
            current->son = allocateSon();
        }
        
        if (!current->son[index]) {
            return current;
        }
        
        current = current->son[index];
        ++pos;
    }
    
    return current;
}

TrieNode* Trie::addTrie(const std::string& key) {
    return addTrie(root, key, 0);
}

TrieNode* Trie::addTrie(TrieNode* node, const std::string& key, size_t startPos) {
    if (!node) return nullptr;
    
    TrieNode* current = node;
    size_t pos = startPos;
    
    while (pos < key.length()) {
        int index = keyPosition(static_cast<unsigned char>(key[pos]));
        if (index == -1) {
            ++pos;
            continue;
        }
        
        if (!current->son) {
            current->son = allocateSon();
        }
        
        if (!current->son[index]) {
            current->son[index] = createNode();
        }
        
        current = current->son[index];
        ++pos;
    }
    
    current->isKey = true;
    return current;
}

void Trie::cleanup(TrieNode* node) {
    if (!node) return;
    
    if (node->son) {
        for (int i = 0; i < SIZE; ++i) {
            if (node->son[i]) {
                cleanup(node->son[i]);
            }
        }
        delete[] node->son;
    }
    delete node;
}
