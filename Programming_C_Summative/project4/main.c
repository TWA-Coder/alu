#include "main.h"


int main() {
    int* dataset = NULL;
    int size = 0;
    char filename[100];   // FIXED

    printf("Enter filename to load data: ");
    scanf("%99s", filename);   // safer scanf

    load_data(filename, &dataset, &size);
    
    process_user_input(dataset, size);
    
    printf("Enter filename to save data: ");
    scanf("%99s", filename);

    save_data(filename, dataset, size);

    free_dataset(dataset);
    return 0;
}
