#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int global_var = 10;

void process_data(int* data, int size) {
    for(int i = 0; i < size; i++) {
        data[i] = data[i] * 2;
    }
}

int check_condition(int value) {
    if (value > 20) {
        return 1;
    } else {
        return 0;
    }
}

void print_result(int* data, int size, int flag) {
    if (flag) {
        printf("Condition met! Data: ");
    } else {
        printf("Condition not met! Data: ");
    }
    for (int i=0; i<size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
}

int main() {
    int size = 5;
    int* data = (int*)malloc(size * sizeof(int));
    if (data == NULL) return 1;

    for (int i=0; i<size; i++) {
        data[i] = i + 1;
    }
    
    global_var += 5;
    process_data(data, size);
    
    int result = check_condition(global_var + data[0]);
    print_result(data, size, result);
    
    free(data);
    return 0;
}
