#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() 
{
    char email[256];
    char command[512];
    struct stat st = {0};
    const char *home = getenv("HOME");
    if (home == NULL) 
    {
        fprintf(stderr, "Could not get HOME environment variable\n");
        return 1;
    }
    char ssh_dir[512];
    snprintf(ssh_dir, sizeof(ssh_dir), "%s/.ssh", home);
    if (stat(ssh_dir, &st) == -1) 
    {
        if (mkdir(ssh_dir, 0700)) 
        {
            fprintf(stderr, "Failed to create .ssh directory\n");
            return 1;
        }
    }
    printf("Enter your GitHub email: ");
    if (fgets(email, sizeof(email), stdin) == NULL) 
    {
        fprintf(stderr, "Error reading email\n");
        return 1;
    }
    email[strcspn(email, "\n")] = '\0';
    snprintf(command, sizeof(command), "ssh-keygen -t rsa -b 4096 -C \"%s\" -f ~/.ssh/id_rsa -N \"\"", email);
    if (system(command)) 
    {
        fprintf(stderr, "Failed to generate SSH key\n");
        return 1;
    }
    if (system("eval $(ssh-agent -s)")) 
    {
        fprintf(stderr, "Failed to start SSH agent\n");
        return 1;
    }
    if (system("ssh-add ~/.ssh/id_rsa")) 
    {
        fprintf(stderr, "Failed to add SSH key to agent\n");
        return 1;
    }
    if (system("xclip -selection clipboard < ~/.ssh/id_rsa.pub")) 
    {
        fprintf(stderr, "Failed to copy public key to clipboard\n");
        return 1;
    }
    printf("SSH key generated and public key copied to clipboard.\n");
    return 0;
}
