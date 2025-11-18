// scraper.h
#ifndef SCRAPER_H
#define SCRAPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>

// Struct to hold thread data
typedef struct {
    const char* url;        // URL to fetch
    const char* filename;   // Output filename
} ThreadData;

// Function prototypes
void* fetch_url(void* arg);
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
void download_html(const char* url, const char* filename);

#endif // SCRAPER_H