// scraper.c
#include "scraper.h"

// Callback function to write data received from cURL
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    FILE* fp = (FILE*)userp;
    size_t written = fwrite(contents, size, nmemb, fp);
    return written;
}

// Function to download HTML content from a URL and save to a file
void download_html(const char* url, const char* filename) {
    CURL* curl;
    FILE* fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(filename, "wb");
        if (!fp) {
            perror("Failed to open file");
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "Failed to fetch %s: %s\n", url, curl_easy_strerror(res));
        }

        // Cleanup
        fclose(fp);
        curl_easy_cleanup(curl);
    }
}

// Thread function to fetch a URL
void* fetch_url(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    download_html(data->url, data->filename);
    return NULL;
}