#include <ctype.h>      // For character type functions
#include <stdlib.h>     // For standard library functions, like malloc
#include <stdio.h>      // For standard input/output functions
#include <unistd.h>     // For POSIX operating system API
#include <string.h>     // For string manipulation functions
#include <dirent.h>     // For directory traversal functions
#include <sys/stat.h>   // For retrieving information about files
#include <stdbool.h>    // For boolean type
#include <fcntl.h>      // For file control options

#define HASH_SIZE 127   // Define hash table size for storing the dictionary

// Struct to represent a node in the linked list for each word entry
typedef struct WordNode {
    char* word;                  // Word stored as a string
    struct WordNode* next;       // Pointer to the next node in the list
} WordNode;

WordNode* wordMap[HASH_SIZE];    // Hash table for the dictionary, with chaining for collision resolution

// Hash function for words, based on the first character
unsigned int hashWord(const char* word) {
    return (unsigned int)word[0] % HASH_SIZE;
}

// Function to create a new word node in the dictionary
WordNode* makeWordNode(char* word) {
    WordNode* node = malloc(sizeof(WordNode));  // Allocate memory for the node
    node->word = strdup(word);                  // Copy the word into the node
    node->next = NULL;                          // Initialize the next pointer to NULL
    return node;
}

// Function to insert a word into the dictionary
void insertWord(char* word) {
    unsigned int idx = hashWord(word);  // Calculate the hash index for the word
    WordNode* node = makeWordNode(word); // Create a new word node
    // Insert the node into the hash table, maintaining order
    if (!wordMap[idx] || strcmp(word, wordMap[idx]->word) < 0) {
        node->next = wordMap[idx];
        wordMap[idx] = node;
    } else {
        WordNode* curr = wordMap[idx];
        // Traverse the linked list to find the correct insertion point
        while (curr->next && strcmp(word, curr->next->word) > 0) {
            curr = curr->next;
        }
        node->next = curr->next;
        curr->next = node;
    }
}

// Function to load words from a dictionary file into the hash table
void loadDict(const char* path) {
    int fd = open(path, O_RDONLY);  // Open the dictionary file
    if (fd == -1) {
        perror("Error opening dictionary");
        return;
    }
    char buf[100], word[100];
    int bytesRead, wIdx = 0;
    // Read the dictionary file and insert words into the hash table
    while ((bytesRead = read(fd, buf, sizeof(buf) - 1)) > 0) {
        for (int i = 0; i < bytesRead; ++i) {
            if (!isspace(buf[i]) && buf[i] != '\n') {
                word[wIdx++] = buf[i];
            } else if (wIdx > 0) {
                word[wIdx] = '\0';
                insertWord(word);  // Insert the word with current capitalization
                // Convert word to uppercase and insert again for case-insensitive comparison
                for (int j = 0; word[j] != '\0'; j++) {
                    word[j] = toupper(word[j]);
                }
                insertWord(word);
                wIdx = 0;
            }
        }
    }
    if (bytesRead == -1) perror("Error reading dictionary");
    close(fd);  // Close the dictionary file
}

// Forward declaration for processFile function
void processFile(const char* path);

// Function to recursively traverse directories and process text files
void traverseDir(const char* basePath) {
    DIR *dir = opendir(basePath);  // Open the directory
    if (!dir) return;

    struct dirent *dp;
    // Iterate over the entries in the directory
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] == '.') continue;  // Skip hidden files and directories

        char path[1000];
        snprintf(path, sizeof(path), "%s/%s", basePath, dp->d_name);  // Construct the full path

        struct stat statPath;
        stat(path, &statPath);  // Get information about the path

        // Recursively traverse subdirectories or process text files
        if (S_ISDIR(statPath.st_mode)) {
            traverseDir(path);
        } else if (strstr(dp->d_name, ".txt")) {
            processFile(path);
        }
    }
    closedir(dir);  // Close the directory
}

// Function to report a spelling error
void reportSpellingError(const char* word, const char* path, int line, int col, bool isCommaBefore) {
    if (isCommaBefore) {
        // Report an error if there is no space between a word and a preceding comma
        printf("Spelling error in '%s' at line %d, column %d: ',%s' no space between word and comma.\n",
               path, line, col, word);
    } else {
        // Report a standard spelling error
        printf("Spelling error in '%s' at line %d, column %d: '%s' incorrect spelling.\n",
               path, line, col, word);
    }
}

// Additional functions like hasIncorrectMidWordCapitalization, matchesCapitalizationRules, findWord, checkWord, isAlphaOrHyphen, cleanAndCheckWord, updateLineAndColumn, and extractWords would follow here, similar to the earlier explanations.

// Main function
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <dict_path> <file_path>...\n", argv[0]);
        return 1;
    }
    memset(wordMap, 0, sizeof(wordMap));  // Initialize the hash table
    loadDict(argv[1]);  // Load the dictionary

    struct stat statPath;
    // Process each file or directory passed as argument
    for (int i = 2; i < argc; i++) {
        stat(argv[i], &statPath);  // Get information about the path
        if (S_ISDIR(statPath.st_mode)) {
            traverseDir(argv[i]);  // Process directory
        } else if (S_ISREG(statPath.st_mode)) {
            processFile(argv[i]);  // Process file
        } else {
            printf("Skipping unsupported: %s\n", argv[i]);  // Skip unsupported file types
        }
    }
    return 0;
}
