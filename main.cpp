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

struct node
{
    Pair data ;
    node* next;
};

class LinkedList
{
public:
    node* head;

public:
    LinkedList():head(nullptr){}

    void push(Pair x);
    void display();

};

class Hash{
public:
    int BUCKET = 20000 ;
    LinkedList *dictionary ;

    Hash() ;

    int makeHash(std::string key);
    void addElement(Pair data) ;
    void displayHash() ;
    bool getDefinition(std::string key, std::string definition, bool change);
};

void createDictionary(Hash&);
void menu(Hash&) ;

int main()
{
    Hash dictionary ;
    createDictionary(dictionary);
    //dictionary.displayHash() ;
    menu(dictionary);
    return 0;
}

inline void LinkedList::push(Pair x)
{
    node* newNode = new node;
    node* temp = new node;
    temp = head;
    newNode->data = x ;
    if(temp == nullptr)
    {
        newNode->next = nullptr;
        head = newNode;
        return;

    }

    while( temp->next != nullptr )
    {
        temp = temp->next ;
    }
    newNode->next = nullptr ;
    temp->next = newNode ;

}
Hash::Hash()
{
    dictionary = new LinkedList[BUCKET] ;
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
inline void Hash::addElement( Pair data )
{
    int hash = makeHash( getWordInUpperLetters(data.Key) ) ;
    dictionary[hash].push( data) ;
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
inline bool Hash::getDefinition(std::string key, std::string definition, bool change)
{
    int hash = makeHash(key);
    if(change == true )
        hash ++ ;
    node* current = dictionary[hash].head;

    while(current != nullptr )
    {
        if( current->data.Key == key ){
            if(!change)
            {
                std::cout << current->data.Value << '\n' ;
                return true ;
            } else if(change){
                current->data.Value = definition ;
                return true ;
            }

        }
        current = current->next;
    }
    return false ;
}
inline void Hash::displayHash()
{
    for( size_t i = 0 ; i < BUCKET ; i ++ ){
        std::cout << i << " | " ;
        node* current = dictionary[i].head;
        while(current != nullptr )
        {
            std::cout << current->data.Key << " " ;
            current = current->next;
        }
        std::cout << "\n" ;
    }
}
void menu(Hash& dictionary)
{
    std::string choice = "";
    std::cout << YELLOW "What do you want to do?" << std::endl;
    std::cout << "1. Look up for a meaning of words in a sentence." << std::endl;
    std::cout << "2. Insert your definition." << std::endl;
    std::cout << "3. Exit." WHITE << std::endl;
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
            bool check = dictionary.getDefinition( getWordInUpperLetters(wordsOfSentence[i]), "", false ) ;
            if(!check )
                std::cout << RED "The definition of this word is not available" NC ;
            std::cout << "\n" ;
        }
    }
    else if (choice == "2")
    {
        std::string word, definition ;
        std::cout << YELLOW "Enter a word: " WHITE << std::endl;
        std::cin >> word;
        std::cin.ignore(10000, '\n');
        std::cout
            << YELLOW "Enter a definition: " WHITE << std::endl;
        std::getline(std::cin, definition);

        if( dictionary.getDefinition( getWordInUpperLetters(word), definition, true) )
        {
            std::cout << "Everything OK! We changed this definition!\n" ;
        }
        else
        {
            std::cout << "Sorry, we does not have this word, so we can't change it." ;
        }
    }
    else if (choice == "3")
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
