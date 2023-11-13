#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h> // For dirname
#include <sys/stat.h>
#include "file_count.h"

int countFiles(const char *directoryPath) {
    DIR *d;
    struct dirent *dir;
    struct stat statbuf;
    int count = 0;
    const char* prefix = "_MEI";
    int prefixLength = strlen(prefix);
    char parentPath[1024];

    // Copy the directory path to a temporary variable as dirname might modify it
    strncpy(parentPath, directoryPath, sizeof(parentPath));
    parentPath[sizeof(parentPath) - 1] = '\0'; // Ensure null-termination

    // Get the parent directory
    char *parentDir = dirname(parentPath);

    d = opendir(parentDir);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            // Check if the entry name starts with the given prefix
            if (strncmp(dir->d_name, prefix, prefixLength) == 0) {
                char fullPath[1024];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", parentDir, dir->d_name);

                // Use lstat to get info about the entry
                if (lstat(fullPath, &statbuf) == 0) {
                    // Check if it's a directory
                    if (S_ISDIR(statbuf.st_mode)) {
                        count++;
                    }
                }
            }
        }
        closedir(d);
    } else {
        perror("opendir");
        return -1; // Return -1 to indicate an error
    }

    return count;
}
