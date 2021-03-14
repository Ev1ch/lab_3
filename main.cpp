#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#define YELLOW "\033[33m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define WHITE "\e[37m"
#define NC "\e[0m"

struct Pair
{
    std::string key, value;
};

struct Node
{
    Pair data;
    Node *next;
};

class LinkedList
{
public:
    Node *head;

public:
    LinkedList() : head(nullptr) {}

    void push(Pair x);
    void display();
};

class HashTable
{
public:
    int BUCKET = 20;
    LinkedList *dictionary;

    HashTable();

    int makeHashTable(std::string key);
    void addElement(Pair data);
    void displayHashTable();
    bool getDefinition(std::string key);
    void rebuild();
};

void createDictionary(HashTable &);
void menu(HashTable &);

int main()
{
    HashTable dictionary;
    createDictionary(dictionary);
    menu(dictionary);
    return 0;
}

void HashTable::rebuild()
{
    this->BUCKET = BUCKET * 2;
    LinkedList *rebuilding = new LinkedList[BUCKET];
    for (long long i = 0; i < BUCKET / 2; i++)
    {
        Node *current = dictionary[i].head;
        while (current != nullptr)
        {
            int HashTable = makeHashTable(current->data.key);
            rebuilding[HashTable].push(current->data);
            current = current->next;
        }
    }
    delete[] dictionary;
    dictionary = rebuilding;
}

void LinkedList::push(Pair x)
{
    Node *newNode = new Node;
    Node *temp = new Node;
    temp = head;
    newNode->data = x;
    if (temp == nullptr)
    {
        newNode->next = nullptr;
        head = newNode;
        return;
    }

    while (temp->next != nullptr)
    {
        temp = temp->next;
    }
    newNode->next = nullptr;
    temp->next = newNode;
}

HashTable::HashTable()
{
    dictionary = new LinkedList[BUCKET];
}

std::string getWordInUpperLetters(std::string word)
{
    std::string upperWord = "";
    for (int i = 0; i < word.size(); i++)
    {
        if (isalpha(word[i]))
        {
            upperWord += toupper(word[i]);
        }
    }
    return upperWord;
}

void HashTable::addElement(Pair data)
{
    int HashTable = makeHashTable(getWordInUpperLetters(data.key));
    dictionary[HashTable].push(data);
}

int HashTable::makeHashTable(std::string key)
{
    int HashTable;
    for (size_t i = 1; i <= key.length(); i++)
    {
        HashTable += (int)key[i - 1] * i;
    }

    return HashTable % BUCKET;
}

bool HashTable::getDefinition(std::string key)
{
    int HashTable = makeHashTable(key);
    Node *current = dictionary[HashTable].head;

    while (current != nullptr)
    {
        if (current->data.key == key)
        {
            std::cout << current->data.value << std::endl;
            return true;
        }
        current = current->next;
    }
    return false;
}

void HashTable::displayHashTable()
{
    for (size_t i = 0; i < BUCKET; i++)
    {
        std::cout << i << " | ";
        Node *current = dictionary[i].head;
        while (current != nullptr)
        {
            std::cout << current->data.key << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}

void menu(HashTable &dictionary)
{
    std::string choice = "";
    std::cout << YELLOW "What do you want to do?" << std::endl;
    std::cout << "1. Look up for a meaning of words in a sentence." << std::endl;
    std::cout << "2. Exit." WHITE << std::endl;
    std::cin >> choice;
    std::cout << std::endl;

    std::string sentence, definition;
    std::string wordsOfSentence[100];
    if (choice == "1")
    {
        std::cout << YELLOW "Enter a sentence: " WHITE << std::endl;
        int amountOfWords = 0, lastSpace = 0;
        std::cin.ignore(1);
        getline(std::cin, sentence);
        sentence += " ";
        while (sentence.find(' ', lastSpace) != -1)
        {
            wordsOfSentence[amountOfWords++] = sentence.substr(lastSpace, sentence.find(' ', lastSpace) - lastSpace);
            lastSpace = sentence.find(' ', lastSpace) + 1;
        }
        for (int i = 0; i < amountOfWords; i++)
        {
            std::cout << getWordInUpperLetters(wordsOfSentence[i]) << std::endl;
            bool check = dictionary.getDefinition(getWordInUpperLetters(wordsOfSentence[i]));
            if (!check)
                std::cout << RED "The definition of this word is not available" NC;
            std::cout << "\n";
        }
    }
    else if (choice == "2")
    {
        std::cout
            << YELLOW "Goodbye." NC
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
    menu(dictionary);
}

void createDictionary(HashTable &dictionary)
{
    std::fstream dictionaryFile("dictionary.txt");
    if (dictionaryFile.is_open())
    {
        int wordsNumber = 0;
        std::string currentLine;
        while (std::getline(dictionaryFile, currentLine))
        {

            if (wordsNumber >= 0.8 * dictionary.BUCKET)
            {
                dictionary.rebuild();
            }

            wordsNumber++;
            std::string currentWord = currentLine.substr(0, currentLine.find(';')),
                        currentDefinition = currentLine.substr(currentLine.find(';') + 2);
            Pair data;
            data.key = currentWord, data.value = currentDefinition;
            dictionary.addElement(data);
        }
        std::cout << GREEN "Dictionary is imported." NC << std::endl;
    }
    else
    {
        std::cout << RED "Something wrong with file..." NC << std::endl;
    }
}
