#include <iostream>
#include <exception>
#include <cstdlib>
#include <filesystem>
#include <thread>
#include <chrono>
#include "Anatex.h"

namespace fs = std::filesystem;

int main() 
{
    try 
    {
        Anatex anatex;
        anatex.initialize();
        
        std::string inputFolder = "../input";
        std::string outputFolder = "../output";

        while (true) {
            for (const auto& entry : fs::directory_iterator(inputFolder)) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    std::string inputFilePath = entry.path().string();
                    std::string outputFileName = entry.path().stem().string() + ".html";
                    std::string outputFilePath = outputFolder + "/" + outputFileName;

                    anatex.annotateText(inputFilePath, outputFilePath);

                    std::cout << "Processed " << inputFilePath << " -> " << outputFilePath << std::endl;

                    fs::remove(entry.path());
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    } catch (const std::exception& e) 
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return EXIT_FAILURE;
    } catch (...) 
    {
        std::cerr << "An unknown error occurred." << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return EXIT_FAILURE;
    }
    
    std::cout << "Press Enter to exit...";
    std::cin.get(); 
    return EXIT_SUCCESS;
}
