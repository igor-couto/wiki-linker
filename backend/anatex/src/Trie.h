#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <string>

struct TrieNode {
    bool isKey;
    TrieNode** son;

    TrieNode() : isKey(false), son(nullptr) {}
};

class Trie {
public:
    TrieNode* root;

    Trie();
    ~Trie();

    TrieNode* createNode();
    TrieNode* searchTrie(const std::string& key);  // Public interface
    TrieNode* addTrie(const std::string& key);
    void cleanup(TrieNode* node);

private:
    TrieNode* searchTrie(TrieNode* node, const std::string& key, size_t startPos);  // Private helper
    TrieNode* searchNodeInsertion(TrieNode* node, const std::string& key, size_t startPos = 0);
    TrieNode* addTrie(TrieNode* node, const std::string& key, size_t startPos = 0);
    TrieNode* allocateNode();
    TrieNode** allocateSon();

    int keyPosition(unsigned char key);
    int hashOne(unsigned char key);
    int hashTwo(unsigned char key);
};

#endif
