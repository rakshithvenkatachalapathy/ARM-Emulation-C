#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static char unsigned_num[50];   //global variable
/**
 * Struct to store the canonical reprasentation of the given input
 * Stores the number in unsigned int ,width , start and end range
 */
struct nt_rep {
    unsigned int v;
    int width;
    int range_start;
    int range_end;
};

/**
 * Function to convert the string reprasentaion to int
 * @param str - string to be converted to an integer
 * @return - unsigned int equivalent of int
 */
unsigned int str_to_int(char *str) {
    int i, d;
    unsigned int val = 0;
    unsigned long checker = 0;
    unsigned long long max = 4294967296, min = 2147483648;
    int slen = strlen(str);
    if ((int) str[0] == 45) {
        for (i = 1; i < slen; i++) {
            val = val * 10;
            checker = checker * 10;
            d = (int) str[i] - '0';
            val = val + d;
            checker = checker + d;

        }
        val = val * -1;
    } else {
        for (i = 0; i < slen; i++) {
            val = val * 10;
            checker = checker * 10;
            d = (int) str[i] - '0';
            val = val + d;
            checker = checker + d;
        }
    }
    //Checks for overflow and under flow condition below
    if (checker > max) {
        printf("Error: %s Cannot fit into 32 bits\n", str);
        printf("USAGE: ./nt -b <bit width> -r <range of bits> <value>\n"
               "bit width (optional): 4, 8, 12, 16, 20, 24, 28, 32 (default)\n"
               "range of bits (optional): the first value in the range specifies the least significant bit (the rightmost bit) \n"
               "and the second value in the range specifies the most significant bit (the leftmost bit). The range values start at 0 and they are inclusive.\n"
               "value: prepend 0b if binary, 0x if hex, otherwise parsed as an integer with respect to the bid width\n");
        exit(0);
    } else if (checker > min && checker != val) {
        printf("Error: %s Cannot fit into 32 bits\n", str);
        printf("USAGE: ./nt -b <bit width> -r <range of bits> <value>\n"
               "bit width (optional): 4, 8, 12, 16, 20, 24, 28, 32 (default)\n"
               "range of bits (optional): the first value in the range specifies the least significant bit (the rightmost bit) \n"
               "and the second value in the range specifies the most significant bit (the leftmost bit). The range values start at 0 and they are inclusive.\n"
               "value: prepend 0b if binary, 0x if hex, otherwise parsed as an integer with respect to the bid width\n");
        exit(0);
    } else if (checker == 0 && (int) str[0] != 0 && strlen(str) > 1) {

        printf("Error: %s Cannot fit into 32 bits\n", str);
        printf("USAGE: ./nt -b <bit width> -r <range of bits> <value>\n"
               "bit width (optional): 4, 8, 12, 16, 20, 24, 28, 32 (default)\n"
               "range of bits (optional): the first value in the range specifies the least significant bit (the rightmost bit) \n"
               "and the second value in the range specifies the most significant bit (the leftmost bit). The range values start at 0 and they are inclusive.\n"
               "value: prepend 0b if binary, 0x if hex, otherwise parsed as an integer with respect to the bid width\n");
        exit(0);
    }
    return val;
}

/**
 * Function to convert binary to decimal
 * @param num - takes in a binary number and gives out its equivalent decimal forma
 * @return -  a decimal equivalent of the given number
 */
int bin_to_dec(long long num) {
    int dec = 0, i = 0, rem;
    while (num != 0) {
        rem = num % 10;
        num /= 10;
        dec += rem * pow(2, i);
        ++i;
    }
    return dec;
}

/**
 * Function to print the binary reprasentation in human readable form
 * @param num - a number to be convert to binary
 * @param width - bit width range
 */
void bin_rep_withspace(int num, int width) {
    int c, k, i = 1;
    for (c = width - 1; c >= 0; c--) {
        k = num >> c;
        if (k & 1) {
            printf("1");
            if (i % 4 == 0)
                printf(" ");
        } else {
            printf("0");
            if (i > 0 && i % 4 == 0)
                printf(" ");
        }
        i++;
    }
    printf("(base 2)\n");
}

/**
 * Function to convert from binary to hexadecimal form
 * @param bin - the number to be converted to hexadecimal
 */
void bin_to_hex(long int bin, int width) {
    long int binary_val, hexadecimal_val = 0, i = 1, remainder, quotient;
    static int j = 0;
    static char hexadecimal[50] = "";
    binary_val = bin;
    while (binary_val != 0) {
        remainder = binary_val % 10;
        hexadecimal_val = hexadecimal_val + remainder * i;
        i = i * 2;
        binary_val = binary_val / 10;
    }
    quotient = hexadecimal_val;
    if (width == 32)
        width = 8;
    if (quotient == 0)
        hexadecimal[j++] = 48;

    while (quotient != 0) {
        remainder = quotient % 16;          //step 1
        if (remainder < 10)
            hexadecimal[j++] = 48 + remainder;   //step 2
        else
            hexadecimal[j++] = 55 + remainder;   //step 3
        quotient = quotient / 16;            //step 4
    }
    if (width % strlen(hexadecimal) == 0) {
        printf("%s", hexadecimal);
    }

}

/**
 * function to print the machine readable format
 * @param n - the number
 * @param width - bit width
 */
void bin_rep(int n, int width) {
    int c, k, i = 0, len;
    char hex[50] = "";
    char o = '1', z = '0';
    char dest[5] = {0};
    printf("0b");
    for (c = width - 1; c >= 0; c--) {
        k = n >> c;
        if (k & 1) {
            printf("1");
            strncat(hex, &o, 1);
        } else {
            printf("0");
            strncat(hex, &z, 1);
        }
    }
    printf(" (base 2)\n");
    len = strlen(hex);
    for (int j = 0; j <= len; j++)
        unsigned_num[j] = hex[j];

    /*printf("0x");
    while (i * 4 < len) {
        strncpy(dest, hex + (i * 4), 4);
        i++;
        bin_to_hex(atoi(dest), width);
    }
    printf(" (base 16)\n");
    if (width != 32) {

    }*/
}

/**
 * Function to convert hexdecimal to decimal form
 * @param hexVal - the hex value repasented as a string
 * @return - the deciamal equivalent of the string
 */

int hex_to_dec(char hexVal[100]) {

    int len = strlen(hexVal);
    // Initializing base value to 1, i.e 16^0
    int base = 1, dec_val = 0;
    // Extracting characters as digits from last character
    for (int i = len - 1; i >= 0; i--) {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value.
        if (hexVal[i] >= '0' && hexVal[i] <= '9') {
            dec_val += (hexVal[i] - 48) * base;
            // incrementing base by power
            base = base * 16;
        }
            // if character lies in 'A'-'F' , converting
            // it to integral 10 - 15 by subtracting 55
            // from ASCII value
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
            dec_val += (hexVal[i] - 55) * base;
            // incrementing base by power
            base = base * 16;
        }
    }
    return dec_val;
}

/**
 * Function to parse the given hexadecimal input to canonical form
 * @param str - hexadecimal string reprasentation  of the number
 * @return - an decimal equivalent
 */

int parse_hex(char str[50]) {
    int i = 0, j = 0;
    char temp[50] = "";
    int len = strlen(str);
    for (j = 0; j <= len; j++) {
        if (j == 0 || j == 1) {
            continue;
        } else {
            temp[i] = str[j];
            i++;
        }
    }
    if (strlen(temp) > 8) {
        printf("Error: %s Cannot fit into 32 bits\n", str);
        printf("USAGE: ./nt -b <bit width> -r <range of bits> <value>\n"
               "bit width (optional): 4, 8, 12, 16, 20, 24, 28, 32 (default)\n"
               "range of bits (optional): the first value in the range specifies the least significant bit (the rightmost bit) \n"
               "and the second value in the range specifies the most significant bit (the leftmost bit). The range values start at 0 and they are inclusive.\n"
               "value: prepend 0b if binary, 0x if hex, otherwise parsed as an integer with respect to the bid width\n");
        exit(0);
    }
    return hex_to_dec(temp);
}

/**
 * Function to
 * @param bin
 * @return
 */
int bin_to_dec2(char bin[]) {
    unsigned int d, v;
    int b = 1, len = strlen(bin);
    for (int i = len - 1; i >= 0; i--) {
        d = bin[i] - '0';
        v += d * b;
        b = b * 2;
    }
    return v;
}

/**
 * Function to parse the binary input to canonical form
 * @param st - string reprasentaion of the binary input
 * @return - the decimal equivalent
 */
int parse_bin(char st[50]) {
    int i = 0, j = 0, ret;
    char temp[50];
    int len = strlen(st);
    for (j = 0; j <= len; j++) {
        if (j == 0 || j == 1) {
            continue;
        } else {
            temp[i] = st[j];
            i++;
        }
    }
    ret = bin_to_dec2(temp);
    return ret;
}

/**
 * Function used to set range
 * @param s - start
 * @param e -end
 * @param range  - the input range from the user
 */
void set_range(int *s, int *e, char range[]) {

    /*int len = 0, j = 1;
    char *tok = strtok(range, ",");
    len = strlen(tok);
    if (len != 2) {
        printf("Cannot have range more than 2\n");
        exit(0);
    }
    while (tok != 0) {
        printf(" %s\n", tok);
        if (j == 1)
            *s = str_to_int(tok);

        if (j == 2)
            *e = str_to_int(tok);
        j++;
        tok = strtok(0, range);
    }*/
    /* if ((*s || *e) < 0 || (*s || *e) > 31) {
         printf("Invalid range\n");
         exit(0);
     }*/

    char *tok = strtok(range, ",");
    *s = str_to_int(tok);

    tok = strtok(NULL, ",");
    *e = str_to_int(tok);
    if ((*s) < 0 || (*s) > 31) {
        printf("Invalid range\n");
        exit(0);
    } else if ((*e) < 0 || (*e) > 31) {
        printf("Invalid range\n");
        exit(0);
    }
}

/**
 * Function to parse the input into binary,decimal or hexadecimal
 * @param v  -the struct univisgned address
 * @param num - the given user input
 */
void set_num(int *v, char num[50]) {
    if (num[1] == 'b')
        *v = parse_bin(num);
    else if (num[1] == 'x')
        *v = parse_hex(num);
    else
        *v = str_to_int(num);
}

/**
 * Function for bit twiddling
 * @param val - value
 * @param start - start
 * @param end -end
 * @param pInt -
 */
void set_range_canonical(unsigned int *val, int start, int end) {
    unsigned int y;
    unsigned int v = *val;
    y = (v >> start) & (1 << (end - start) + 1) - 1;
    *val = y;
}

/**
 * Function to print unsigned int rep
 * @param val - canoical form
 * @param width - bit width
 */

void unsigned_int_rep(unsigned int val, int width) {
    int r = 0, q = 0, i = 0, begin;
    int c, sign, offset, end;
    long n;
    char my_str[50] = "", usign_str[50] = "", ch;

    if (val == 0) {
        printf("%s (base 10 unsigned)\n", "0");
        return;
    }
    //printf("usi: %s\n", unsigned_num);
    /*if (width != 32) {
        if (unsigned_num[0] == '-') {  // Handle negative integers
            sign = -1;
        }
        if (sign == -1) {               // Set starting position to convert
            offset = 1;
        } else {
            offset = 0;
        }
        n = 0;
        for (c = offset; unsigned_num[c] != '\0'; c++) {
            n = n * 10 + unsigned_num[c] - '0';
        }
        if (sign == -1) {
            n = -n;
        }
        val = bin_to_dec(n);
    }*/

    set_range_canonical(&val, 0, width - 1);
    //printf("val : %d\n", val);
    while (val > 0) {
        r = val % 10;
        q = val / 10;
        ch = r + '0';
        my_str[i] = ch;
        i++;
        val = q;
    }
    //printf("my str:%s\n", my_str);
    end = strlen(my_str) - 1;
    for (begin = 0; begin < strlen(my_str); begin++) {
        usign_str[begin] = my_str[end];
        end--;
    }
    usign_str[begin] = '\0';
    printf("%s (base 10 unsigned)\n", usign_str);
}

/**
 * Function to print the signed value of the canonical form
 * @param num - canonical number value
 * @param width - bitwidth
 */
void signed_int_rep(unsigned int num, int width) {
    char ch, str[35], result[35] = "";
    int res = 0, i = 0, rem = 0, q = 0;

    if (num == 0) {
        printf("%s (base 10 signed)\n", "0");
        return;
    }
    if (num >= ((1 << width - 1))) {
        num = num % (1 << width - 1);
        num = (1 << width - 1) - num;
        res = 1;
    }
    while (num > 0) {
        rem = num % 10;
        q = num / 10;
        ch = rem + '0';
        str[i] = ch;
        i++;
        num = q;
    }
    if (res == 1)
        str[i] = '-';
    i = 0;
    for (int j = strlen(str) - 1; j >= 0; j--) {
        result[i] = str[j];
        i++;
    }
    printf("%s (base 10 signed)\n", result);

}

/**
 * Function to parse the user input based on the normal,bit width or twidddling form
 * @param argc - input parameter count
 * @param argv -input agrs from command line
 * @param p -struct for canonical reprasentation
 */
void parse_input(int argc, char **argv, struct nt_rep *p) {
    int val, width, start, end;
    char num[50] = "", range[50] = "";
    if (argc == 1) {
        printf("USAGE: ./nt -b <bit width> -r <range of bits> <value>\n"
               "bit width (optional): 4, 8, 12, 16, 20, 24, 28, 32 (default)\n"
               "range of bits (optional): the first value in the range specifies the least significant bit (the rightmost bit) \n"
               "and the second value in the range specifies the most significant bit (the leftmost bit). The range values start at 0 and they are inclusive.\n"
               "value: prepend 0b if binary, 0x if hex, otherwise parsed as an integer with respect to the bid width\n");
        exit(0);
    }
    if (argc == 2) {
        strcpy(num, argv[1]);
        set_num(&val, num);
        width = 32;
        start = 0;
        end = 0;
    } else if (argc == 4) {
        width = atoi(argv[2]);
        strcpy(num, argv[3]);
        set_num(&val, num);
        start = 0;
        end = 0;
    } else if (argc == 6) {
        width = atoi(argv[2]);
        strcpy(range, argv[4]);
        strcpy(num, argv[5]);
        set_num(&val, num);
        set_range(&start, &end, range);
        set_range_canonical(&val, start, end);
    }
    p->v = (unsigned int) val;
    p->width = width;
    p->range_start = start;
    p->range_end = end;
}

void hex_rep(unsigned int num, int width) {
    unsigned int val;
    char rev_str[50] = "", hexadecimal[50] = "";
    int i = 0;

    if (width != 32) {
        set_range_canonical(&num, 0, width - 1);
    }
    while (num != 0) {
        val = num & 0xF;
        num = num >> 4;
        if (val <= 9)
            rev_str[i] = 48 + val;
        else if (val >= 10 || val <= 15)
            rev_str[i] = 55 + val;


        i++;
    }
    while (width == 32 && strlen(rev_str) != 8) {
        rev_str[i] = 48;
        i++;
    }
    i = 0;
    for (int j = strlen(rev_str) - 1; j >= 0; j--) {
        hexadecimal[i] = rev_str[j];
        i++;
    }
    printf("0x%s (base 16)\n", hexadecimal);

}

/**
 * Main fucnction
 * @param argc  -argument count
 * @param argv - program arguments
 * @return
 */
int main(int argc, char *argv[]) {
    struct nt_rep ntr;
    parse_input(argc, argv, &ntr);
    //bin_rep(ntr.v, ntr.width);
    bin_rep_withspace(ntr.v, ntr.width);
    bin_rep(ntr.v, ntr.width);
    hex_rep(ntr.v, ntr.width);
    unsigned_int_rep(ntr.v, ntr.width);
    signed_int_rep(ntr.v, ntr.width);

    return 0;
}
