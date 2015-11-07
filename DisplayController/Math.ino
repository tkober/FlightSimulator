#include "Math.h"


int int_from_string(char *string, unsigned int length, int singend) {
    int result = 0;
    int factor = 1;
    int end = 0;
    int sign = 1;
    if (singend == 1) {
        end = 1;
        sign = string[0] == '-' ? -1 : 1;
    }
    for (int i = length - 1; i >= end; i--) {
        result += digit_from_char(string[i]) * factor;
        factor *= 10;
    }
    return result * sign;
}


int digit_from_char(char c) {
    switch (c) {
        case '1':
            return 1;
            
        case '2':
            return 2;
            
        case '3':
            return 3;
            
        case '4':
            return 4;
            
        case '5':
            return 5;
            
        case '6':
            return 6;
            
        case '7':
            return 7;
            
        case '8':
            return 8;
            
        case '9':
            return 9;
            
        default:
            return 0;
    }
}
