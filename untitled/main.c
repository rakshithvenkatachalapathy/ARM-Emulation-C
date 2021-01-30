#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <math.h>

struct nt_rep {
    unsigned int v;
    int width;
    int range_start;
    int range_end;
};

int str_to_int(char *s) {
    int i;
    int d;
    int v = 0;
    int slen = strlen(s);

    if ((int) s[0] == 45) {

        for (i = 1; i < slen; i++) {

            v = v * 10;
            d = (int) s[i] - '0';
            v = v + d;
        }
        v = v * -1;

    } else {
        for (i = 0; i < slen; i++) {

            v = v * 10;
            d = (int) s[i] - '0';
            v = v + d;
        }
    }

    return v;
}

void bin_to_hex(long int bin) {
    long int binary_val, hexadecimal_val = 0, i = 1, remainder;

    binary_val = bin;
    while (binary_val != 0) {
        remainder = binary_val % 10;
        hexadecimal_val = hexadecimal_val + remainder * i;
        i = i * 2;
        binary_val = binary_val / 10;
    }
    printf("%lX", hexadecimal_val);

}

void bin_rep(int n) {
    int c, k, i = 0;
    char hex[100] = "";
    char o = '1', z = '0';
    char dest[5] = {0};   // 4 chars + terminator

    printf("0b");
    for (c = 31; c >= 0; c--) {
        k = n >> c;

        if (k & 1) {
            printf("1");
            //hex[i++]=1;
            strncat(hex, &o, 1);

        } else {
            printf("0");
            // hex[i++]=0;
            strncat(hex, &z, 1);


        }
    }
    printf(" (base 2)");
    printf("\n");

    //long int hex1 = atoi(hex);

    int len = strlen(hex);
    printf("0x");
    while (i * 4 < len) {
        strncpy(dest, hex + (i * 4), 4);
        i++;

        //printf("\nloop %d : %s\n", i, dest);


        bin_to_hex(atoi(dest));
    }

    printf(" (base 16)\n");

}

int hex_to_dec(char hex[100]) {

    int decimal;
    int i = 0, val, len;

    decimal = 0;

    len = strlen(hex);
    len--;

    for (i = 0; hex[i] != '\0'; i++) {

        if (hex[i] >= '0' && hex[i] <= '9') {
            val = hex[i] - 48;
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            val = hex[i] - 97 + 10;
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            val = hex[i] - 65 + 10;
        }

        decimal += val * pow(16, len);
        len--;
    }

    printf("Hexadecimal number = %s\n", hex);
    //printf("Decimal number = %lld\n", decimal);

    unsigned int val1 = (unsigned int) decimal;

    return decimal;

}

int parse_hex(char st[50]) {
    int i = 0, j = 0;
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

    printf("%s\n", temp);
    hex_to_dec(temp);
}

int bin_to_dec(long long n) {

    int dec = 0, i = 0, rem;
    while (n != 0) {
        rem = n % 10;
        n /= 10;
        dec += rem * pow(2, i);
        ++i;
    }

    printf("dec x: %d", dec);

    return dec;
}

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

    printf("%s\n", temp);
    long num = atoi(temp);
    ret = bin_to_dec(num);
    return ret;
}

void set_num(int *v, char num[50]) {
    if (num[1] == 'b')
        *v = parse_bin(num);
    else if (num[1] == 'x')
        *v = parse_hex(num);
    else
        *v = str_to_int(num);
}

void set_range(int *s, int *e, char range[]) {
    for (int i = 0; i <= strlen(range); i++) {
        if (i == 0)
            *s = range[i] - '0';
        else if (i == 1);
        else if (i == 2)
            *e = range[i] - '0';
    }

}

void parse_input(int argc, char **argv, struct nt_rep *p) {
    int i = 0, val, width, start, end;
    char num[50], range[50];
    //char *ch;
    for (i = 1; i <= argc - 1; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    if (argc - 1 == 2) {
        strcpy(num, argv[2]);
        printf(": %s", num);
        set_num(&val, num);
        width = 32;
        start = 0;
        end = 0;

    } else if (argc - 1 == 4) {
        width = atoi(argv[3]);
        strcpy(num, argv[4]);
        set_num(&val, num);
        start = 0;
        end = 0;
    } else if (argc - 1 == 6) {
        width = atoi(argv[3]);
        strcpy(range, argv[5]);
        strcpy(num, argv[6]);
        set_num(&val, num);
        set_range(&start, &end, range);
    }
    p->v = (unsigned int) val;
    p->width = width;
    p->range_start = start;
    p->range_end = end;
    printf("\nval: %u", p->v);
    printf("\nwid: %d", p->width);
    printf("\nsta: %d", p->range_start);
    printf("\nend: %d", p->range_end);

}

void unsigned_int_rep(unsigned int n2) {


    int r, q, i = 0, begin;
    char str[50], r1[50];
    char ch;

    while (n2 > 0) {
        r = n2 % 10;
        q = n2 / 10;


        ch = r + '0';
        str[i] = ch;
        i++;

        n2 = q;

    }

    int end = strlen(str) - 1;

    for (begin = 0; begin < strlen(str); begin++) {
        r1[begin] = str[end - 1];
        end--;
    }

    r1[begin] = '\0';

    printf("%s\n", r1);


}


int main(int argc, char **argv) {
    struct nt_rep ntr;
    //printf("%s == %d\n", argv[1], str_to_int(argv[1]));

    //unsigned int u = str_to_int(argv[1]);
    char st[] = "0x2C9";

    // parse_input(argc, argv, &ntr);

    unsigned_int_rep(-713);

    int a = 10;

    //printf("%d", (int) a - '0');

    return 0;
}