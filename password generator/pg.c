#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_LENGTH 256

char *generateRandomString(int length)
{
    const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    char *randomString = malloc((length + 1) * sizeof(char));

    for (int i = 0; i < length; i++)
    {
        randomString[i] = characters[rand() % strlen(characters)];
    }
    randomString[length] = '\0';

    return randomString;
}

int main()
{
    srand(time(NULL));

    int length;
    printf("Enter the length: ");
    scanf("%d", &length);

    char *randomString = generateRandomString(length);
    printf("%s\n", randomString);

    free(randomString);

    return 0;
}