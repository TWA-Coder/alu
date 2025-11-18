// main.c
#include "scraper.h"

#define MAX_URLS 100

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <url1> <url2> ... <urlN>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pthread_t threads[MAX_URLS];
    ThreadData thread_data[MAX_URLS];
    
    int num_urls = argc - 1;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    for (int i = 0; i < num_urls; i++) {

        thread_data[i].url = argv[i + 1];

        // Allocate memory for each filename
        thread_data[i].filename = malloc(64);
        if (!thread_data[i].filename) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        snprintf(thread_data[i].filename, 64, "output_%d.html", i);

        if (pthread_create(&threads[i], NULL, fetch_url, &thread_data[i]) != 0) {
            fprintf(stderr, "Error creating thread for URL %s\n", argv[i + 1]);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < num_urls; i++) {
        pthread_join(threads[i], NULL);
        free(thread_data[i].filename); // Cleanup allocated memory
    }

    curl_global_cleanup();

    printf("All URLs have been processed.\n");
    return EXIT_SUCCESS;
}
