#include <stdio.h>
#include <string.h>
#include "md5.h"

#define BUFFER_SIZE 1024

void add (const char * filename);
void test(const char * filename);

void strip(const char * filename);



void main (int iArgC, char *apszArgV[])
{

    if (strcmp("add", apszArgV[1]) == 0) {
        add(apszArgV[2]);
    } else if (strcmp("test", apszArgV[1]) == 0) {

        test(apszArgV[2]);

    } else if (strcmp("strip", apszArgV[1]) == 0) {
        strip(apszArgV[2]);

    } else {
        printf("Wrong argument passed\n");
    }


}

void add (const char * filename)
{
    FILE *f;
    MD5_CTX ctx;
    BYTE byHash[MD5_BLOCK_SIZE];
    BYTE byBuffer[BUFFER_SIZE];
    int iReadBytes = 0;

    f = fopen (filename, "rb+");
    if (f != NULL) {

        md5_init(&ctx);

        while (!feof(f)) {
            iReadBytes = fread(byBuffer, sizeof(BYTE), BUFFER_SIZE, f);

            if (iReadBytes < BUFFER_SIZE) break;
        }
        md5_update(&ctx, byBuffer, iReadBytes);
        md5_final(&ctx, byHash);

        //add checksum to the end of the file
        for (int i = 0; i < MD5_BLOCK_SIZE; i ++) {
            fwrite (&byHash[i], 1, 1, f);
        }
        printf("%d\n", iReadBytes);
        putchar('\n');
        fclose (f);


    }



}

void test(const char * filename){
    FILE *f;
    MD5_CTX ctx;
    BYTE byHash[MD5_BLOCK_SIZE];
    BYTE byHashRead[MD5_BLOCK_SIZE];
    BYTE byBuffer[BUFFER_SIZE];
    int iReadBytes = 0;
    int pass = 1;


    f = fopen (filename, "rb+");
    if (f != NULL) {

        md5_init(&ctx);

        while (!feof(f)) {
            iReadBytes = fread(byBuffer, sizeof(BYTE), BUFFER_SIZE, f);

            if (iReadBytes < BUFFER_SIZE) break;
        }


        printf("this is size of file: %d\n", iReadBytes - MD5_BLOCK_SIZE);

        md5_update(&ctx, byBuffer, iReadBytes - MD5_BLOCK_SIZE);
        md5_final(&ctx, byHash);
        fseek(f, -MD5_BLOCK_SIZE, SEEK_END);
        fread(byHashRead, sizeof(BYTE), MD5_BLOCK_SIZE, f);



        printf("\n");

        pass = pass && !memcmp(byHash, byHashRead, MD5_BLOCK_SIZE);

        if(pass == 1){
            printf("%s", "Test passed. \n");
        } else {
            printf("%s", "Test failed. \n");
        }


    }
    fclose(f);


}

void strip(const char * filename){

    FILE *f;
    MD5_CTX ctx;
    BYTE byHash[MD5_BLOCK_SIZE];
    BYTE byHashRead[MD5_BLOCK_SIZE];
    BYTE byBuffer[BUFFER_SIZE];
    BYTE bufferOfFileWithMD5[BUFFER_SIZE + MD5_BLOCK_SIZE];
    int iReadBytes = 0;
    int pass = 1;


    f = fopen (filename, "rb+");
    if (f != NULL) {

        md5_init(&ctx);

        while (!feof(f)) {
            iReadBytes = fread(byBuffer, sizeof(BYTE), BUFFER_SIZE, f);

            if (iReadBytes < BUFFER_SIZE) break;
        }


        printf("this is size of file: %d\n", iReadBytes - MD5_BLOCK_SIZE);

        md5_update(&ctx, byBuffer, iReadBytes - MD5_BLOCK_SIZE);
        md5_final(&ctx, byHash);
        fseek(f, -MD5_BLOCK_SIZE, SEEK_END);
        fread(byHashRead, sizeof(BYTE), MD5_BLOCK_SIZE, f);



        printf("\n");

        pass = pass && !memcmp(byHash, byHashRead, MD5_BLOCK_SIZE);

        fclose(f);


        if(pass){
            f = fopen (filename, "rb+");
            while (!feof(f)) {
                iReadBytes = fread(bufferOfFileWithMD5, sizeof(BYTE), BUFFER_SIZE + MD5_BLOCK_SIZE, f);

                if (iReadBytes < BUFFER_SIZE) break;
            }


            fclose(f);
            f = fopen (filename, "wb");
            fwrite(bufferOfFileWithMD5, iReadBytes - MD5_BLOCK_SIZE, sizeof(BYTE), f);
            fclose(f);

        }

    }

}

