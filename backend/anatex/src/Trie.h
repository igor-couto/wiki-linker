#ifndef TRIE_H
#define TRIE_H

#include <map>
#include <string>

struct TrieNode {
    std::map<wchar_t, TrieNode*> children;
    std::wstring wikiLink;
    bool isEndOfTitle = false;
};

class Trie {
public:
    TrieNode* root;

    Trie();

    void insert(const std::wstring& title);

    std::wstring search(const std::wstring& text, int& length);
};

#endif 