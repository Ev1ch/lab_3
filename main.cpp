#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <iostream>

class Dictionary
{
private:
    std::string dictionaryName;
    std::fstream dictionaryFile;
    std::vector<std::string> dictionary;

    int hash(std::string key)
    {
        int hash = 0;
        for (size_t i = 0; i < key.length(); i++)
            hash += key[i] * (i + 1);
        return hash % this->dictionary.size();
    }

    std::string toUpper(std::string str)
    {
        for (std::string::size_type i = 0; i < str.length(); ++i)
            str[i] = std::toupper(str[i]);
        return str;
    }

public:
    void getWord(std::string key)
    {
        if (this->dictionary[this->hash(key)] == "")
        {
            std::cout << "Word doesn't has an explanation" << std::endl;
        }
        else
        {
            std::cout << key << ": " << this->dictionary[this->hash(key)] << std::endl;
        }
    }

    void insertWord(){};

    void removeWord(){};

    void importDictionary(std::string dictionaryName)
    {
        this->dictionaryName = dictionaryName;
        this->dictionaryFile.open(this->dictionaryName);

        if (this->dictionaryFile.is_open())
        {
            std::string line, value;
            int key = 0, dictionarySize = 0;

            while (getline(this->dictionaryFile, line))
            {
                dictionarySize += 1;
            }
            this->dictionary.resize(dictionarySize, "");
            this->dictionaryFile.clear();
            this->dictionaryFile.seekg(0, std::ios::beg);

            while (getline(this->dictionaryFile, line))
            {
                key = this->hash(line.substr(0, line.find(";")));
                value = line.substr(line.find(";") + 2);
                dictionary[key] = value;
            }
            std::cout << "Dictionary is imported" << std::endl;
        }
        else
        {
            std::cout << "File can't be open" << std::endl;
        }
    }
};

int main()
{
    Dictionary dictionary;
    dictionary.importDictionary("dictionary.txt");
    dictionary.getWord("kkkk");
    return 0;
}