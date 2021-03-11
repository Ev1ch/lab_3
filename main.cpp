#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#define YELLOW "\033[33m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define WHITE "\e[37m"
#define NC "\e[0m"

class Dictionary
{
private:
    int dictionarySize = 20000;
    std::pair<std::string, std::string> *dictionary;

    int makeHash(std::string key)
    {
        int hash;
        for (size_t i = 1; i <= key.length(); i++)
        {
            hash += (int)key[i - 1] * i;
        }

        return hash % dictionarySize;
    }

    void createDictionary(std::string dictionaryFileName)
    {
        std::fstream dictionaryFile(dictionaryFileName);
        this->dictionary = new std::pair<std::string, std::string>[dictionarySize];

        if (dictionaryFile.is_open())
        {
            int wordsNumber = 0;
            std::string currentLine;
            while (std::getline(dictionaryFile, currentLine))
            {
                wordsNumber++;
                std::string currentWord = currentLine.substr(0, currentLine.find(';')),
                            currentDefinition = currentLine.substr(currentLine.find(';') + 2);
                this->insertWord(currentWord, currentDefinition, false);
            }
            std::cout << GREEN "Dictionary is imported." NC << std::endl;
        }
        else
        {
            std::cout << RED "Something wrong with file..." NC << std::endl;
        }
    }

public:
    Dictionary(std::string dictionaryFileName)
    {
        createDictionary(dictionaryFileName);
    }

    void insertWord(std::string key, std::string value, bool toPrint)
    {
        int currentHash = this->makeHash(key);
        if (this->dictionary[currentHash].first.empty())
        {
            this->dictionary[currentHash] = {key, value};
            if (toPrint)
            {
                std::cout << GREEN "Word was inserted." NC << std::endl;
            }
        }
        else if (this->dictionary[currentHash].first == key)
        {
            if (toPrint)
            {
                std::string choice = "";
                std::cout << RED "Word already exists." << std::endl;
                std::cout << "Do you want to replace its definition?" << std::endl;
                std::cout << "1. Yes" << std::endl;
                std::cout << "2. No" WHITE << std::endl;
                std::cin >> choice;

                if (choice == "1")
                {
                    this->dictionary[currentHash].second = value;
                    std::cout << GREEN "Word was inserted." NC << std::endl;
                }
                else
                {
                    std::cout << RED "Word was not inserted." NC << std::endl;
                }
            }
            else
            {
                this->dictionary[currentHash].second = value;
            }
        }
        else
        {
            while (!this->dictionary[currentHash].first.empty())
            {
                currentHash++;
                if (currentHash > this->dictionarySize)
                {
                    std::cout << RED "Dictionary is full." NC << std::endl;
                    return;
                }
            }
            this->dictionary[currentHash] = {key, value};
        }
    }

    void getDefinition(std::string key)
    {
        int hash = this->makeHash(key);
        bool isFound = true;
        while (this->dictionary[hash].first != key)
        {
            if (hash > this->dictionarySize)
            {
                isFound = false;
                break;
            }
            hash++;
        }
        if (isFound)
        {
            std::cout << WHITE "- " << this->dictionary[hash].second << std::endl;
        }
        else
        {
            std::cout << RED "The definition of this word is not available" NC << std::endl;
        }
    }

    void printDictionary()
    {
        for (size_t i = 0; i < this->dictionarySize; i++)
        {
            std::cout << "[" << i + 1 << "] " << this->dictionary[i].first << " | " << this->dictionary[i].second << std::endl;
        }
    }
    std::string getWordInUpperLetters( std::string word ){
        std::string upperWord = "" ;
        for( int i = 0 ; i < word.size() ; i ++ ){
            upperWord += toupper(word[i]) ;
        }
        return upperWord ;
    }
    void menu()
    {
        std::string choice = "";
        std::cout << YELLOW "What do you want to do?" << std::endl;
        std::cout << "1. Look up for a word." << std::endl;
        std::cout << "2. Insert your definition." << std::endl;
        std::cout << "3. Exit." WHITE << std::endl;
        std::cin >> choice;
        std::cout << std::endl;

        std::string word, definition;
        if (choice == "1")
        {
            std::cout << YELLOW "Enter a word: " WHITE << std::endl;
            std::cin >> word;
            word = getWordInUpperLetters(word) ;
            getDefinition(word);
        }
        else if (choice == "2")
        {
            std::cout << YELLOW "Enter a word: " WHITE << std::endl;
            std::cin >> word;
            std::cin.ignore(10000, '\n');
            std::cout
                << YELLOW "Enter a definition: " WHITE << std::endl;
            std::getline(std::cin, definition);
            this->insertWord(word, definition, true);
        }
        else if (choice == "3")
        {
            std::cout
                << YELLOW "Godbye." NC
                << std::endl;
            return;
        }
        else
        {
            std::cout
                << RED "Wrong choice. Try again." NC
                << std::endl;
        }
        std::cout << std::endl;
        this->menu();
    }
};

int main()
{
    Dictionary dictionary("dictionary.txt");
    dictionary.menu();

    return 0;
}
