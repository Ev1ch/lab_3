#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <list>

#define YELLOW "\033[33m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define WHITE "\e[37m"
#define NC "\e[0m"

struct Pair{
    std::string Key, Value ;
};

class Hash{
public:
    int BUCKET = 20000;
    std::list<Pair> *dictionary ;

    Hash() ;

    int makeHash(std::string key);
    void addElement(Pair data) ;
    void displayHash() ;
    bool getDefinition(std::string key);
};

void createDictionary(Hash&);
void menu(Hash&) ;

int main()
{
    Hash dictionary ;
    createDictionary(dictionary);
    menu(dictionary);
    return 0;
}

Hash::Hash()
{
    dictionary = new std::list<Pair>[BUCKET] ;
}
inline void Hash::addElement( Pair data )
{
    int hash = makeHash( data.Key ) ;
    dictionary[hash].push_back(data) ;
}
inline int Hash::makeHash(std::string key)
{
    int hash;
    for (size_t i = 1; i <= key.length(); i++)
    {
        hash += (int)key[i - 1] * i;
    }

    return hash % BUCKET ;
}
inline bool Hash::getDefinition(std::string key){
    int hash = makeHash(key) ;
    for( auto u : dictionary[hash]){
        if( u.Key == key ){
            std::cout << u.Value << '\n' ;
            return true;
        }
    }
    return false ;
}
std::string getWordInUpperLetters( std::string word )
{
    std::string upperWord = "" ;
    for( int i = 0 ; i < word.size() ; i ++ )
    {
        /// In case, if we get a sentence like this : Car is something, I guess.
        if( isalpha(word[i]) )
            upperWord += toupper(word[i]) ;
    }
    return upperWord ;
}
inline void Hash::displayHash()
{
    for (size_t i = 0; i < BUCKET ; i++)
    {
        for( auto u : dictionary[i]){
            std::cout << u.Key << ' ' << u.Value << '\n' ;
        }
        std::cout << '\n' ;
    }
}
void menu(Hash& dictionary)
{
    std::string choice = "";
    std::cout << YELLOW "What do you want to do?" << std::endl;
    std::cout << "1. Look up for a meaning of words in a sentence." << std::endl;
    std::cout << "2. Exit." WHITE << std::endl;
    std::cin >> choice;
    std::cout << std::endl;

    std::string sentence, definition;
    std::string wordsOfSentence[100] ;
    if (choice == "1")
    {
        std::cout << YELLOW "Enter a sentence: " WHITE << std::endl;
        int amountOfWords = 0, lastSpace = 0 ;
        std::cin.ignore(1) ;
        getline(std::cin, sentence) ;
        sentence += " " ;
        while(sentence.find(' ', lastSpace) != -1 )
        {
            wordsOfSentence[ amountOfWords ++ ] = sentence.substr(lastSpace, sentence.find(' ', lastSpace) - lastSpace) ;
            lastSpace = sentence.find(' ', lastSpace) + 1;
        }
        for(int i = 0 ; i < amountOfWords ; i ++ )
        {
            std::cout << '\n' << getWordInUpperLetters(wordsOfSentence[i]) << '\n';
            bool check = dictionary.getDefinition( getWordInUpperLetters(wordsOfSentence[i]) ) ;
            if(!check )
                std::cout << RED "The definition of this word is not avalible" NC ;
            std::cout << "\n" ;
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
void createDictionary(Hash& dictionary)
{
    std::fstream dictionaryFile("dictionary.txt");
    if (dictionaryFile.is_open())
    {
        int wordsNumber = 0;
        std::string currentLine;
        while (std::getline(dictionaryFile, currentLine))
        {
            wordsNumber++;
            std::string currentWord = currentLine.substr(0, currentLine.find(';')),
                        currentDefinition = currentLine.substr(currentLine.find(';') + 2);
            Pair data ;
            data.Key = currentWord, data.Value = currentDefinition ;
            dictionary.addElement( data );
        }
        std::cout << GREEN "Dictionary is imported." NC << std::endl;
    }
    else
    {
        std::cout << RED "Something wrong with file..." NC << std::endl;
    }
}
