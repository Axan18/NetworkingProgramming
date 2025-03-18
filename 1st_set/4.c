#include <stdbool.h>
#include <stdio.h>
static bool is_printable_str(const char* str);

int main(void)
{
    char test1[] = "Hello, World!0"; 
    char test2[] = "Hello, World!";
    char test3[] = { 'A', 'B', 'C', 120, 'D'};
    char test4[] = { 'X', 'Y', 'Z', 120, 'G','\0'};
    char test5[] = { 'A', 'B', 'C', 127, 'D', 44};
    
    printf("Test 1: %s\n", is_printable_str(test1) ? "true" : "false");
    printf("Test 2: %s\n", is_printable_str(test2) ? "true" : "false");
    printf("Test 3: %s\n", is_printable_str(test3) ? "true" : "false");
    printf("Test 4: %s\n", is_printable_str(test4) ? "true" : "false");
    printf("Test 5: %s\n", is_printable_str(test5) ? "true" : "false");
}

bool is_printable_str(const char* str){
    const char* p = str;
    while (*p !='\0')
    {
        if(*p <32 || *p >126)
            return false;
        p++;
    }
    return true;
}