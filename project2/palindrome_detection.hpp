#pragma once
#include <cstddef>
#include <cstdio>

bool is_palindrom(const unsigned char* wordStart, int wordLength){
    const unsigned char* left = wordStart;
    const unsigned char* right = left + wordLength - 1;
    while (left < right) {
        if (*left != *right)
            return false;
        left++;
        right--;
    }
    return true;
}

bool word_parser(unsigned char* wordStart, int wordLength){
    unsigned char* p = wordStart;
    for (int i = 0; i < wordLength; i++) {
        if (*p < 'A' || (*p > 'Z' && *p < 'a') || *p > 'z')
            return false;
        if (*p >= 'A' && *p <= 'Z')
            *p += ('a' - 'A');
        p++;
    }
    return true;
}

char* datagram_stream(unsigned char* buff, char* result, size_t bufferSize){ // size check!!!!!!!!!!!!!!!!!!!!!
    unsigned char* letter = buff;
    if (*letter == ' ') return NULL;
    
    unsigned char* wordStart = letter;
    int wordCount = 0, palindromCount = 0, wordLength = 0;

    while (*letter) {
        if (*letter == ' ') { // end of the word
            if (*(letter + 1) == ' ' || *(letter + 1) == '\0') // if two spaces together || space on the end -> ERROR
                return NULL;
            if (wordLength > 0 && word_parser(wordStart, wordLength)) { //word processing
                wordCount++;
                if (is_palindrom(wordStart, wordLength))
                    palindromCount++;
            }else{
                return NULL;
            }
            wordStart = letter + 1;
            wordLength = 0;
        } else {
            wordLength++;
        }
        letter++;
    }

    if (wordLength > 0 && word_parser((unsigned char*)wordStart, wordLength)) { // last word
        wordCount++;
        if (is_palindrom(wordStart, wordLength))
            palindromCount++;
    }else{
        return NULL;
    }
    if (!result) return NULL;
        snprintf(result, bufferSize, "%d/%d", palindromCount, wordCount);
    return result;
}

bool containsZeroByte(unsigned char* buf, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (buf[i] == '\0') {
            return true;
        }
    }
    return false;
}