#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    const char *text = "hello world";
    char command[256];
    snprintf(command, sizeof(command), "echo '%s' | xclip -selection clipboard", text);
    int result = system(command);
    if (result != 0) 
    {
        fprintf(stderr, "Failed to copy to clipboard\n");
        return 1;
    }
    printf("Copied to clipboard: %s\n", text);
    return 0;
}
