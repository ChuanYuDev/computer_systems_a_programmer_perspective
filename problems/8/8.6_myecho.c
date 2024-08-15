#include <stdio.h>
int string_print(char **sp, char *str)
{
    int count = 0;
    while(*sp)
    {
        printf("    %s[%2d]: %s\n", str, count, *sp);
        sp++;
        count++;
    }
    return 0;
}

int main(int argc, char **argv, char **envp)
{
    printf("Command-line arguments:\n");

    string_print(argv, "argv");

    printf("\nEnvironment variables:\n");

    string_print(envp, "envp");

    return 0;
}