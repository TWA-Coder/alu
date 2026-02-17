#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

// Huffman Tree Node
typedef struct MinHeapNode {
    char data;          // Character
    unsigned freq;      // Frequency
    struct MinHeapNode *left, *right;
} MinHeapNode;

// Min Heap Collection
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    MinHeapNode **array;
} MinHeap;

// Bit Writing Buffer
typedef struct {
    unsigned char buffer;
    int count;
    FILE *fp;
} BitWriter;

// Bit Reading Buffer
typedef struct {
    unsigned char buffer;
    int count;
    FILE *fp;
} BitReader;

// --- Helper Functions for Min Heap ---

MinHeapNode* newNode(char data, unsigned freq) {
    MinHeapNode* temp = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

MinHeapNode* extractMin(MinHeap* minHeap) {
    MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

int isLeaf(MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    MinHeapNode *left, *right, *top;
    MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// --- Encoding Table Generation ---

void getCodes(MinHeapNode* root, int arr[], int top, char codes[256][MAX_TREE_HT]) {
    if (root->left) {
        arr[top] = 0;
        getCodes(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        getCodes(root->right, arr, top + 1, codes);
    }
    if (isLeaf(root)) {
        for (int i = 0; i < top; ++i) {
            codes[(unsigned char)root->data][i] = arr[i] + '0';
        }
        codes[(unsigned char)root->data][top] = '\0';
    }
}

// --- Bit I/O ---

void initBitWriter(BitWriter *bw, FILE *fp) {
    bw->buffer = 0;
    bw->count = 0;
    bw->fp = fp;
}

void writeBit(BitWriter *bw, int bit) {
    bw->buffer = (bw->buffer << 1) | (bit & 1);
    bw->count++;
    if (bw->count == 8) {
        fputc(bw->buffer, bw->fp);
        bw->buffer = 0;
        bw->count = 0;
    }
}

void flushBitWriter(BitWriter *bw) {
    if (bw->count > 0) {
        bw->buffer <<= (8 - bw->count);
        fputc(bw->buffer, bw->fp);
    }
}

void initBitReader(BitReader *br, FILE *fp) {
    br->buffer = 0;
    br->count = 0; // Empty
    br->fp = fp;
}

int readBit(BitReader *br) {
    if (br->count == 0) {
        int c = fgetc(br->fp);
        if (c == EOF) return -1;
        br->buffer = (unsigned char)c;
        br->count = 8;
    }
    int bit = (br->buffer >> (br->count - 1)) & 1;
    br->count--;
    return bit;
}

// --- Compression ---

void compressFile(const char *inputFile, const char *outputFile) {
    int freq[256] = {0};
    FILE *in = fopen(inputFile, "rb"); // Binary mode for exact byte count
    if (!in) { printf("Error opening input file.\n"); return; }
    
    // 1. Calculate Frequency
    int c;
    long total_bytes = 0;
    while ((c = fgetc(in)) != EOF) {
        freq[c]++;
        total_bytes++;
    }
    rewind(in);
    
    if (total_bytes == 0) {
       printf("File is empty.\n");
       fclose(in);
       return;
    }

    // 2. Build Tree arrays
    char data[256];
    int freq_nonzero[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[size] = (char)i;
            freq_nonzero[size] = freq[i];
            size++;
        }
    }
    
    MinHeapNode* root = buildHuffmanTree(data, freq_nonzero, size);
    
    // 3. Generate Codes
    char codes[256][MAX_TREE_HT] = {{0}};
    int arr[MAX_TREE_HT], top = 0;
    getCodes(root, arr, top, codes);
    
    // 4. Write Compressed File
    FILE *out = fopen(outputFile, "wb");
    if (!out) { printf("Error opening output file.\n"); fclose(in); return; }
    
    // Header: Write Total Bytes (long) + Unique Char Count (int)
    fwrite(&total_bytes, sizeof(long), 1, out);
    fwrite(&size, sizeof(int), 1, out);
    
    // Header: Write Frequency Table (Char + Freq)
    for (int i = 0; i < size; i++) {
        fwrite(&data[i], sizeof(char), 1, out);
        fwrite(&freq_nonzero[i], sizeof(int), 1, out);
    }
    
    // Body: Write Packed Bits
    BitWriter bw;
    initBitWriter(&bw, out);
    
    while ((c = fgetc(in)) != EOF) {
        char *code = codes[c];
        for (int i = 0; code[i] != '\0'; i++) {
            writeBit(&bw, code[i] - '0');
        }
    }
    flushBitWriter(&bw);
    
    long compressed_size = ftell(out);
    
    printf("\nCompression Complete:\n");
    printf("Original Size: %ld bytes\n", total_bytes);
    printf("Compressed Size: %ld bytes\n", compressed_size);
    printf("Savings: %.2f%%\n", (1.0 - (double)compressed_size/total_bytes) * 100);
    
    fclose(in);
    fclose(out);
}

// --- Decompression ---

void decompressFile(const char *inputFile, const char *outputFile) {
    FILE *in = fopen(inputFile, "rb");
    if (!in) { printf("Error opening compressed file.\n"); return; }
    
    // 1. Read Header
    long total_bytes;
    int size;
    if (fread(&total_bytes, sizeof(long), 1, in) != 1) {
        printf("Error reading header (total bytes).\n"); fclose(in); return;
    }
    if (fread(&size, sizeof(int), 1, in) != 1) {
        printf("Error reading header (size).\n"); fclose(in); return;
    }
    
    char data[256];
    int freq[256];
    for (int i = 0; i < size; i++) {
        fread(&data[i], sizeof(char), 1, in);
        fread(&freq[i], sizeof(int), 1, in);
    }
    
    // 2. Rebuild Tree
    MinHeapNode* root = buildHuffmanTree(data, freq, size);
    
    // 3. Decode Bits
    FILE *out = fopen(outputFile, "wb"); // Write binary to preserve newlines exactly
    if (!out) { printf("Error opening output file.\n"); fclose(in); return; }
    
    BitReader br;
    initBitReader(&br, in);
    
    MinHeapNode *curr = root;
    long decoded_count = 0;
    
    while (decoded_count < total_bytes) {
        int bit = readBit(&br);
        if (bit == -1) break; // Should not happen if count matches
        
        if (bit == 0) curr = curr->left;
        else curr = curr->right;
        
        if (isLeaf(curr)) {
            fputc(curr->data, out);
            curr = root;
            decoded_count++;
        }
    }
    
    printf("\nDecompression Complete.\n");
    printf("Restored: %ld bytes\n", decoded_count);
    
    fclose(in);
    fclose(out);
}

int main() {
    int choice;
    char filename[100];
    
    printf("--- Log Compression Utility (Huffman Coding) ---\n");
    
    while(1) {
        printf("\n1. Compress File\n");
        printf("2. Decompress into 'decompressed.log'\n");
        printf("3. Exit\n");
        printf("Select: ");
        if (scanf("%d", &choice) != 1) break;
        
        switch(choice) {
            case 1:
                printf("Enter filename to compress: ");
                scanf("%s", filename);
                compressFile(filename, "compressed.log");
                break;
            case 2:
                // For simplicity, we assume we always decompress 'compressed.log'
                printf("Decompressing 'compressed.log'...\n");
                decompressFile("compressed.log", "decompressed.log");
                break;
            case 3:
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
