#ifndef ANATEX_H
#define ANATEX_H

#include <string>
#include <unordered_set>
#include <locale>
#include <codecvt>
#include "Trie.h"

class Anatex {
public:
    void initialize();
    void annotateText(const std::string& inputFilePath, const std::string& outputFilePath);

private:
    Trie trie;
    std::unordered_set<std::wstring> stopWords;

    std::unordered_set<std::wstring> loadStopWords(const std::string& filename);
    void loadWikiBase(const std::string& filename);
    std::wstring processText(const std::wstring& originalText);
};

#endif
