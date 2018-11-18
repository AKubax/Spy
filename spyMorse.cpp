#include "../AKTools/AKQueue.h"
#include <thread>
#include <chrono>
#include <cctype>
#include <conio.h>
#include <windows.h>

const int DotDurationMSec = 250;
const int DotDuration     = 1;

const int DashDuration            = 3 * DotDuration;
const int SpaceInLetterDuration   = 1 * DotDuration;
const int SpaceInWordDuration     = 3 * DotDuration;
const int SpaceInSentenceDuration = 7 * DotDuration;

const int DumpYShift = 500;
const int DumpXShift = 75;

const int BeepFreq = 575;


AKQueue<char> TheQueue(30);

const char* morseAlphabet[26] = { ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };
const char* morseDigits[10]   = { "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." };


void dump(AKQueue<char>& queue, int x, int y){
    POINT prevPos = txGetConsoleCursorPos();

    txSetConsoleCursorPos(x, y);
    for(const char* ptr = queue.getBuf(); ptr < queue.getBuf() + queue.getSz(); ptr++){
        if(queue.empty()) { $r; }

        else if(queue.getBegin() < queue.getEnd()? (ptr >= queue.getBegin() && ptr < queue.getEnd()) : ( ptr >= queue.getBegin() || ptr < queue.getEnd() ) ) { $b; }
        else                                                                                            { $r; }

        printf("%c", (isspace(*ptr) || !(*ptr))? '_' : *ptr);

    }

    $b;

    POINT fontSize = txGetConsoleFontSize();

    txSetConsoleCursorPos(x, y + fontSize.y);
    printf("%*s", queue.getSz() + 20, "");
    txSetConsoleCursorPos(x, y + 2 * fontSize.y);
    printf("%*s", queue.getSz() + 20, "");

    txSetConsoleCursorPos(x, y - fontSize.y);
    printf("%*s", queue.getSz() + 20, "");
    txSetConsoleCursorPos(x, y - 2 * fontSize.y);
    printf("%*s", queue.getSz() + 20, "");

    txSetConsoleCursorPos(x + (queue.getEnd() - queue.getBuf()) * fontSize.x, y + 1 * fontSize.y);
    printf("^");
    txSetConsoleCursorPos(x + (queue.getEnd() - queue.getBuf()) * fontSize.x, y + 2 * fontSize.y);
    printf("T = 0x%p (%d)", queue.getEnd(), queue.getEnd() - queue.getBuf());

    $y;
    txSetConsoleCursorPos(x + (queue.getBegin() - queue.getBuf()) * fontSize.x, y - 1 * fontSize.y);
    printf("v");
    txSetConsoleCursorPos(x + (queue.getBegin() - queue.getBuf()) * fontSize.x, y - 2 * fontSize.y);
    printf("H");
    printf(" = 0x%p (%d)", queue.getBegin(), queue.getBegin() - queue.getBuf());


    $d
    txSetConsoleCursorPos(prevPos.x, prevPos.y);
}


void sleepFor(unsigned int n);
void sleepFor(unsigned int n){
    std::this_thread::sleep_for(std::chrono::milliseconds(DotDurationMSec * n));
}

void printMorseLetter(const char* c);
void printMorseLetter(const char* c){
    for(const char* ptr = c; *ptr != '\0'; ptr++){

        printf("%c", *ptr);
        dump(TheQueue, DumpXShift, DumpYShift);

        if(*ptr == '.') Beep(BeepFreq, DotDurationMSec *  DotDuration);
        else            Beep(BeepFreq, DotDurationMSec * DashDuration);

        if(*(ptr + 1) == '\0') break;

        sleepFor(SpaceInLetterDuration);
    }
}

void printLetter(char c);
void printLetter(char c){


    if(std::isspace(c)){
        (c == ' ' || c == '\t')? printf("%2c", c) : printf("%c", c);
        if(c == '\r') printf("\n");
        sleepFor(SpaceInSentenceDuration);
    }
    else if(std::isalpha(c)){
        printMorseLetter(morseAlphabet[std::toupper(c) - 'A']);
        sleepFor(SpaceInWordDuration);
        printf("%1c", ' ');
    }
    else if(std::isdigit(c)){
        printMorseLetter(morseDigits[c - '0']);
        sleepFor(SpaceInWordDuration);
        printf("%1c", ' ');
    }


}

void printFunc();
void printFunc(){
    for(;;){
        if(!TheQueue.empty())  printLetter(TheQueue.pop());
    }
}


int main(){
//    TheQueue.push('a');
//    TheQueue.push('\t');
//    TheQueue.push('b');
//    TheQueue.push(' ');
//    TheQueue.push('c');
//    TheQueue.push('\n');
//    TheQueue.push('d');

    InitializeCriticalSection(&CriticalSection);


    std::thread t2(printFunc);

    while(1){
        char c = getch();

        TheQueue.push(c);

    }

    t2.join();
}
