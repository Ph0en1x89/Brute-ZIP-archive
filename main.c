#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <zlib.h>

#define MAX_PASSWORD_LEN 1000
#define MAX_LINE_LEN 1000

// Function to check if a password is correct for a given zip archive
bool check_password(const char* zip_file, const char* password) {
    gzFile fp = gzopen(zip_file, "rb");
    if (fp == NULL) {
        printf("Failed to open zip file.\n");
        return false;
    }

    // Set the password for the zip archive
    int ret = gzsetparams(fp, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        printf("Failed to set password.\n");
        gzclose(fp);
        return false;
    }
    gzsetfilename(fp, zip_file);

    // Try to read the first file from the zip archive
    char buffer[1024];
    int len = gzread(fp, buffer, sizeof(buffer));
    if (len < 0) {
        printf("Failed to read zip file.\n");
        gzclose(fp);
        return false;
    }

    // If we successfully read a file, the password is correct
    gzclose(fp);
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <zip_file> <password_file>\n", argv[0]);
        return 1;
    }

    const char* zip_file = argv[1];
    const char* password_file = argv[2];
    char password[MAX_PASSWORD_LEN];
    FILE* fp = fopen(password_file, "r");
    if (fp == NULL) {
        printf("Failed to open password file.\n");
        return 1;
    }

    // Read each password from the file and try to open the zip archive with it
    while (fgets(password, MAX_LINE_LEN, fp) != NULL) {
        // Remove newline character from password
        size_t len = strlen(password);
        if (password[len-1] == '\n') {
            password[len-1] = '\0';
        }

        if (check_password(zip_file, password)) {
            printf("Password is: %s\n", password);
            fclose(fp);
            return 0;
        }
    }

    printf("Could not find correct password.\n");
    fclose(fp);
    return 1;
}
