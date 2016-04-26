#include "Math.h"


long int_from_string(char *string, unsigned int length, int signedNumber) {
    long result = 0;
    long factor = 1;
    int end = 0;
    long sign = 1;
    if (signedNumber == 1) {
        end = 1;
        sign = string[0] == '-' ? -1 : 1;
    }
    for (int i = length - 1; i >= end; i--) {
        long digit = digit_from_char(string[i]);
        digit = digit < 0 ? 0 : digit;
        result += digit * factor;
        factor *= 10;
    }
    return result * sign;
}


int is_number_string(char *string, unsigned int length, int signedNumber) {
  for (int i = signedNumber; i < length; i++) {
    if (digit_from_char(string[i]) < 0) {
      return 0;
    }
  }
  return 1;
}


int digit_from_char(char c) {
    switch (c) {
        case '0':
            return 0;

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
            return -1;
    }
}
