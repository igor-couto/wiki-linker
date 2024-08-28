#include <dirent.h>
#include <stdexcept>
#include <locale>
#include <vector>
#include <filesystem>
#include <thread>
#include <chrono>
#include "Anatex.h"

Anatex::Anatex() {
    this->stopwords = new Trie();
    this->wikibase = new Trie();
}

Anatex::~Anatex() {
    delete stopwords;
    delete wikibase;
}

void Anatex::initialize() {
    std::ifstream stopwordsFile("../database/stopwords.txt");
    if (!stopwordsFile) {
        throw std::runtime_error("ERROR: Failed to access stopwords database");
    }

    std::ifstream wikibaseFile("../database/wikibase.txt");
    if (!wikibaseFile) {
        throw std::runtime_error("ERROR: Failed to access database");
    }

    std::cout << "Loading stopwords database..." << std::endl;
    loadStopWords(stopwordsFile);
    stopwordsFile.close();
    std::cout << "Stopwords database loaded!" << std::endl << "Loading database..." << std::endl;
    loadWikiTerms(wikibaseFile);
    wikibaseFile.close();
    std::cout << "Database loaded!" << std::endl;
}

void Anatex::loadStopWords(std::ifstream& stopwords) {
    std::string word;
    while (std::getline(stopwords, word)) {
        word = normalize(word);
        this->stopwords->addTrie(word);
    }
}

void Anatex::loadWikiTerms(std::ifstream& wikibase) {
    std::string key;
    while (std::getline(wikibase, key)) {
        key = normalize(key);
        this->wikibase->addTrie(key);
    }
}

void Anatex::annotateText(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::cout << "Opening input file..." << std::endl;

    std::ifstream file(inputFilePath);
    if (!file) {
        throw std::runtime_error("Failed to open input file.");
    }

    std::cout << "Opening output file..." << std::endl;
    std::ofstream output(outputFilePath);
    if (!output) {
        throw std::runtime_error("Failed to open output file.");
    }

    TrieNode* node = nullptr;
    std::string wordFile, str;
    std::vector<std::string> words;

    while (file >> wordFile) {
        str = normalize(wordFile); // Normalize the word
        words.push_back(str); // Add word to list of words

        // Attempt to match sequences of words
        std::string phrase;
        for (size_t i = 0; i < words.size(); ++i) {
            if (i > 0) phrase += " ";
            phrase += words[i];

            if (this->wikibase->searchTrie(phrase)) {
                // If a match is found, write the entire phrase as a link
                output << buildLink(phrase, phrase);
                words.clear(); // Clear matched words
                break;
            }
        }

        if (words.size() > 3) {
            // Avoid growing the phrase too long, flush the first word if no match
            output << words.front() << " ";
            words.erase(words.begin());
        }
    }

    // Output any remaining words
    for (const std::string& remainingWord : words) {
        output << remainingWord << " ";
    }
}

std::string Anatex::buildLink(const std::string& key, const std::string& word) {
    std::string link = "<a href=\"http://pt.wikipedia.org/wiki/";

    for (char c : key) {
        link += (c == ' ') ? '_' : c;
    }

    link += "\">" + word + "</a>";
    return link;
}

std::string Anatex::concatString(Queue& queue) {
    ListNode* lt = queue.getStart(); // Use the getter to access start
    if (!lt) {
        return ""; // Return an empty string if the list is empty
    }

    std::string str = *(lt->info); // Dereference the string pointer to get the string
    lt = lt->next;
    while (lt != nullptr) {
        str += " " + *(lt->info); // Correctly concatenate the string
        lt = lt->next;
    }
    return str;
}

int Anatex::writeWordToFile(std::ofstream& file, const std::string& word, int pos) {
    std::string wordWithSpace = word + ' ';
    file.write(wordWithSpace.c_str(), wordWithSpace.size());
    return pos + wordWithSpace.size();
}

std::string Anatex::normalize(const std::string& str) {
    std::string result;
    std::locale loc;

    for (char c : str) {
        // Convert to lowercase for comparison but preserve original case for final output
        char lower_c = std::tolower(c, loc);

        // Keep alphanumeric characters (a-z, 0-9) and spaces
        if (std::isalnum(lower_c, loc) || lower_c == ' ') {
            result += lower_c;
        }
    }

    return result;
}
