#ifndef ANATEX_H_INCLUDED
#define ANATEX_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>

#include "Queue.h"
#include "Trie.h"

class Anatex
{
    private:
        Trie* stopwords;
        Trie* wikibase;

    public:
        Anatex();
        ~Anatex();

        void initialize();
        void annotateText(const std::string& inputFilePath, const std::string& outputFilePath);

    private:
        void loadStopWords(std::ifstream& stopwords);
        void loadWikiTerms(std::ifstream& wikibase);
        int writeWordToFile(std::ofstream& file, const std::string& word, int pos);
        std::string concatString(Queue& queue);
        std::string buildLink(const std::string& key, const std::string& word);
        std::string normalize(const std::string& str);
};

#endif