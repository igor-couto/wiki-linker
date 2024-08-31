#include "Anatex.h"
#include <fstream>
#include <sstream>
#include <algorithm>

void Anatex::initialize() {
    stopWords = loadStopWords("../database/stopwords.txt");
    loadWikiBase("../database/wikibase.txt");
}

std::unordered_set<std::wstring> Anatex::loadStopWords(const std::string& filename) {
    std::unordered_set<std::wstring> stopWords;
    std::wifstream file(filename);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
    std::wstring word;
    while (file >> word) {
        stopWords.insert(word);
    }
    return stopWords;
}

void Anatex::loadWikiBase(const std::string& filename) {
    std::wifstream file(filename);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
    std::wstring title;
    while (getline(file, title)) {
        std::transform(title.begin(), title.end(), title.begin(), ::towlower);
        trie.insert(title);
    }
}

void Anatex::annotateText(const std::string& inputFilePath, const std::string& outputFilePath) {

    std::wifstream inputFile(inputFilePath);
    inputFile.imbue(std::locale(inputFile.getloc(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream buffer;
    buffer << inputFile.rdbuf();
    std::wstring originalText = buffer.str();

    std::wstring annotatedText = processText(originalText);

    std::wofstream outputFile(outputFilePath);
    outputFile.imbue(std::locale(outputFile.getloc(), new std::codecvt_utf8<wchar_t>));
    outputFile << annotatedText;
}

std::wstring Anatex::processText(const std::wstring& originalText) {
    std::wstring annotatedText;
    int i = 0;
    int n = originalText.size();

    std::wstring lowerText = originalText;
    std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::towlower);

    while (i < n) {
        if (iswspace(originalText[i]) || iswpunct(originalText[i])) {
            annotatedText += originalText[i];
            i++;
            continue;
        }

        // Find the longest matching phrase in the Trie (using lowercase text)
        int length = 0;
        std::wstring matchedLink = trie.search(lowerText.substr(i), length);

        if (length > 0) {
            std::wstring matchedTextOriginal = originalText.substr(i, length);
            std::wstring matchedTextLower = lowerText.substr(i, length);

            // Ensure we are matching complete words/phrases
            if ((i + length == n || !iswalnum(originalText[i + length])) && (i == 0 || !iswalnum(originalText[i - 1]))) {
                if (stopWords.find(matchedTextLower) == stopWords.end()) {
                    annotatedText += L"<a href=\"" + matchedLink + L"\">" + matchedTextOriginal + L"</a>";
                } else {
                    annotatedText += matchedTextOriginal;
                }
                i += length;
            } else {
                annotatedText += originalText[i];
                i++;
            }
        } else {
            annotatedText += originalText[i];
            i++;
        }
    }
    return annotatedText;
}
