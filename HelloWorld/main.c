#include <stdio.h>
int find_max_index_c(int *array, int len)
{
    /*int i;
    int max = array[0];
    int index = 0;

    for (i = 1; i < len; i++) {
        if (array[i] > max) {
            max = array[i];
            index = i;
        }
    }

    return index;*/
    int i;
    int max = array[0];

    for (i = 1; i < len; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }

    return max;
}
int sort_c(int *array, int len)
{
    int i;
    int max_index;
    int tmp;

    for (i = 0; i < len; i++) {
        max_index = find_max_index_c(&array[i], len - i) + i;
        if (max_index != i) {
            tmp = array[i];
            array[i] = array[max_index];
            array[max_index] = tmp;
        }

    }

    return len;
}
int main()
{
    int a1[5] = {1, 4, 1, 1, 2};
    sort_c(&a1,5);

}