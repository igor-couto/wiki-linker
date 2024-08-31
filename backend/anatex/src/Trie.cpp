#include "Trie.h"

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(const std::wstring& title) {
    TrieNode* node = root;
    for (wchar_t c : title) {
        if (node->children.find(c) == node->children.end()) {
            node->children[c] = new TrieNode();
        }
        node = node->children[c];
    }
    node->isEndOfTitle = true;
    node->wikiLink = L"https://en.wikipedia.org/wiki/" + title;
}

std::wstring Trie::search(const std::wstring& text, int& length) {
    TrieNode* node = root;
    std::wstring result = L"";
    length = 0;
    int maxLength = 0;

    for (int i = 0; i < text.size(); ++i) {
        wchar_t c = text[i];
        if (node->children.find(c) == node->children.end()) {
            break;
        }
        node = node->children[c];
        if (node->isEndOfTitle) {
            result = node->wikiLink;
            maxLength = i + 1;
        }
    }

    length = maxLength;
    return result;
}