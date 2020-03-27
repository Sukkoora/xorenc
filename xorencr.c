#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//encryption algo
char * xorencrypt(char*, size_t, char*); 


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("USAGE: ./xorenc INPUT\n");
        return -1;
    }
    char *addr = argv[1];
    char *key = (char *) malloc(128*sizeof(char));

    printf("What is the key you want to encrypt with?\n");
    scanf("%s", key);

    FILE *fileptr;
    char *buffer;
    long filelen;
    
    fileptr = fopen(addr, "rb");  // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc((filelen)*sizeof(char)); // Enough memory for file 

    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file

    //strcat(addr, ".enc");   //append .enc to filename
    fileptr = fopen(addr, "wb");

    buffer = xorencrypt(buffer,filelen,key);

    fwrite(buffer, filelen, 1, fileptr);
    fclose(fileptr);
}

char * xorencrypt(char * message, size_t messagelen, char * key) {
    size_t keylen = strlen(key);

    char * encrypted = malloc(messagelen);

    int i;
    for(i = 0; i < messagelen; i++) {
        encrypted[i] = message[i] ^ key[i % keylen];
    }
    return encrypted;
}
