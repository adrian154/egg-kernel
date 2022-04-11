#include "print.h"

static inline void print_unsigned_integer(uintmax_t integer, int base, int full_length, bool pad) {

    char buf[64];
    const char *DIGITS = "0123456789abcdef";

    for(int i = 0; i < 64; i++) {
        buf[i] = DIGITS[0];
    }

    int index = 0;
    do {
        buf[index] = DIGITS[integer % base];
        integer /= base;
        index++;
    } while(integer != 0);

    // the previous loop extracts lower digits first, so we print in reverse order
    for(int i = (pad ? full_length : index) - 1; i >= 0; i--) {
        putchar(buf[i]);
    }

}

static inline void print_signed_integer(intmax_t integer, int base) {
    
    if(integer < 0) {
        integer = -integer;
        putchar('-');
    }

    print_unsigned_integer(integer, base, 0, false);

}

void print(const char *str) {
    while(*str != '\0') {
        putchar(*str);
        str++;
    }
}

// non-standard printf!!
void printf(const char *format, ...) {

    va_list args;
    va_start(args, format);

    // iterate over chars
    for(int i = 0; format[i] != '\0'; i++) {

        if(format[i] == '%') {
            
            i++;

            // format specifiers
            switch(format[i]) {
                case 'd': // 'd' = decimal 
                    print_signed_integer(va_arg(args, int), 10);
                    break;
                case 'p': // 'p' = arbitrary base (base itself is specified as a hex nibble; binary = %p2, octal = %p8, decimal = %pa, hex = %pf)
                    i++;
                    if(format[i] == '\0') return;
                    int base = format[i] - '0';
                    print_unsigned_integer(va_arg(args, intmax_t), base, 0, false);
                    break;
                case 'b': // 'b' = zero-padded binary
                    i++;
                    switch(format[i]) {
                        case 'b': print_unsigned_integer(va_arg(args, uint32_t), 2, 8, true); break;
                        case 'w': print_unsigned_integer(va_arg(args, uint32_t), 2, 16, true); break;
                        case 'd': print_unsigned_integer(va_arg(args, uint32_t), 2, 32, true); break;
                        case 'q': print_unsigned_integer(va_arg(args, uint64_t), 2, 64, true); break;
                        case '\0': return;
                    }
                    break;
                case 'x': // 'x' = zero-padded hexadecimal
                    i++;
                    switch(format[i]) {
                        case 'b': print_unsigned_integer(va_arg(args, uint32_t), 16, 2, true); break;
                        case 'w': print_unsigned_integer(va_arg(args, uint32_t), 16, 4, true); break;
                        case 'd': print_unsigned_integer(va_arg(args, uint32_t), 16, 8, true); break;
                        case 'q': print_unsigned_integer(va_arg(args, uint64_t), 16, 16, true); break;
                        case '\0': return;
                    }
                    break;
                case 'c': // 'c' = character
                    putchar((char)va_arg(args, int));
                    break;
                case 's': // 's' = string
                    print(va_arg(args, char *));
                    break;
                case '%': // '%' = '%'
                    putchar('%');
                    break;
                case '\0':
                    // Expected a format specifier but reached EOI. Exit 
                    return;
                default:
                    // Unknown format specifier, just skip it.
                    break;
            }

        } else {
            putchar(format[i]);
        }

    }

    va_end(args);

}