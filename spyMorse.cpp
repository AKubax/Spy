#include "../AKTools/AKQueue.h"
#include <thread>
#include <chrono>
#include <cctype>
#include <conio.h>
#include <windows.h>


AKQueue<char> TheQueue(80);

const char* morseAlphabet[26] = { ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };
const char* morseDigits[10]   = { "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." };

void sleepFor(unsigned int n);
void sleepFor(unsigned int n){
    std::this_thread::sleep_for(std::chrono::milliseconds(250 * n));
}

void printMorseLetter(const char* c);
void printMorseLetter(const char* c){
    for(const char* ptr = c; *ptr != '\0'; ptr++){
        printf("%c", *ptr);

        if(*ptr == '.') Beep(575, 250);
        else            Beep(575, 750);

        if(*(ptr + 1) == '\0') break;

        printf(" ");
        sleepFor(1);
    }
}

void printLetter(char c);
void printLetter(char c){
    if(std::isspace(c)){
        (c == ' ' || c == '\t')? printf("%7c", c) : printf("%c", c);
        if(c == '\r') printf("\n");
        sleepFor(7);
    }
    else if(std::isalpha(c)){
        printMorseLetter(morseAlphabet[std::toupper(c) - 'A']);
        sleepFor(3);
        printf("%3c", ' ');
    }
    else if(std::isdigit(c)){
        printMorseLetter(morseDigits[c - '0']);
        sleepFor(3);
        printf("%3c", ' ');
    }
}

void printFunc();
void printFunc(){
    for(;;) if(!TheQueue.empty()) printLetter(TheQueue.pop());
}


int main(){
//    TheQueue.push('a');
//    TheQueue.push('\t');
//    TheQueue.push('b');
//    TheQueue.push(' ');
//    TheQueue.push('c');
//    TheQueue.push('\n');
//    TheQueue.push('d');

    std::thread t2(printFunc);

    while(1){
        char c = getch();

        TheQueue.push(c);
    }

    t2.join();
}
