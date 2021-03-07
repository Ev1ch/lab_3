#include <string>
#include <fstream>
#include <iostream>

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
                this->insertWord(currentWord, currentDefinition);
            }
        }
        else
        {
            std::cout << "Something wrong with file..." << std::endl;
        }
    }

public:
    Dictionary(std::string dictionaryFileName)
    {
        createDictionary(dictionaryFileName);
    }

    void insertWord(std::string key, std::string value)
    {
        int currentHash = this->makeHash(key);

        if (this->dictionary[currentHash].first.empty())
        {
            this->dictionary[currentHash] = {key, value};
        }
        else
        {
            while (!this->dictionary[currentHash].first.empty())
            {
                currentHash++;
            }
            this->dictionary[currentHash] = {key, value};
        }
    }

    std::string getDefinition(std::string key)
    {
        int hash = this->makeHash(key);
        while (this->dictionary[hash].first != key)
        {
            hash++;
        }
        return this->dictionary[hash].second;
    }

    void printDictionary()
    {
        for (size_t i = 0; i < this->dictionarySize; i++)
        {
            std::cout << "[" << i + 1 << "] " << this->dictionary[i].first << " | " << this->dictionary[i].second << std::endl;
        }
    }
};

int main()
{
    Dictionary dictionary("dictionary.txt");
    std::cout << dictionary.getDefinition("B") << std::endl;
}