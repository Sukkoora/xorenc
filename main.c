#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int flag ;
flag quiet = 0;  //Silencer
flag enc = 0; //encrypt File
flag key_known = 0;


int qprintf(char *);
char * xorencrypt(char*, size_t, char*); 


int main(int argc, char** argv) {

    char addr[128];
    char key[128]; 

    //Handling args
    if (argc < 2) {
        printf("No input provided.\n");
        return -1;
    }
    for(int i = 1; i < argc; i++) {
      for (int j = 1; argv[i][0] == '-' && argv[i][j] != 0; j++) {
        if (argv[i][j] == 'q' ) quiet = 1;
        if (argv[i][j] == 'e' ) enc = 1;
        if (argv[i][j] == 'k' ) {
          key_known = 1;
          strcpy(key, argv[i+1]);
        }
      }
    }
    
    // Address of file has to be the last arg provided
    strcpy(addr, argv[argc - 1]);

    // again asking for key //
    if(!key_known) {
      printf("What is the key you want to encrypt with?\n");
      scanf("%s", key);
    }
    // -------- //


    // Handling the file //
    FILE *fileptr;
    char *buffer;
    long filelen;
    
    fileptr = fopen(addr, "rb");          // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc((filelen)*sizeof(char)); // Enough memory for file 

    fread(buffer, filelen, 1, fileptr);   // Read in the entire file (to buffer)
    fclose(fileptr);                      // Close the file
    // ----------------- //

    // Encrypting string in buffer //
    buffer = xorencrypt(buffer,filelen,key);
    // --------------------------- //

    if (enc) {
      //Write encrypted data to file
      fileptr = fopen(addr, "wb");
      fwrite(buffer, filelen, 1, fileptr);
      fclose(fileptr);
      qprintf("File encrypted.\n");
      return 2;
    }
    
    //print buffer
    qprintf("Decrypted file:\n");
    for(int i = 0; i < filelen; i++)
      printf("%c", buffer[i]);
    qprintf("--- END OF FILE ---\n");
    return 1;
}

int qprintf(char * str) {
  if(!quiet) return printf("%s", str);
  else return 0;
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
