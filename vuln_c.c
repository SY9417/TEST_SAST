#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void hardcoded_password_check() {
    char input[20];
    printf("Enter password: ");
    gets(input);  // ❌ buffer overflow
    if (strcmp(input, "123456") == 0) {  // ❌ hardcoded credential
        printf("Access granted\n");
    } else {
        printf("Access denied\n");
    }
}

void unsafe_system_command() {
    char command[100];
    printf("Enter system command: ");
    scanf("%s", command);
    system(command);  // ❌ command injection
}

void file_write() {
    FILE *fp;
    char filename[50], content[100];

    printf("Enter filename: ");
    scanf("%s", filename);

    printf("Enter content: ");
    scanf("%s", content);

    fp = fopen(filename, "w");  // ❌ no permission check
    fprintf(fp, "%s\n", content);
    fclose(fp);
}

void insecure_copy() {
    char src[20];
    char dst[10];
    printf("Enter a string: ");
    scanf("%s", src);
    strcpy(dst, src);  // ❌ buffer overflow
    printf("Copied: %s\n", dst);
}

void read_sensitive_file() {
    FILE *fp = fopen("/etc/passwd", "r");  // ❌ sensitive file
    char line[100];
    if (fp == NULL) {
        perror("File open error");
        return;
    }
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void memory_leak() {
    char *ptr = malloc(100);  // ❌ memory leak
    strcpy(ptr, "This is a test");
    printf("%s\n", ptr);
    // no free(ptr)
}

void use_after_free() {
    char *ptr = malloc(20);
    strcpy(ptr, "Hello");
    free(ptr);
    printf("%s\n", ptr);  // ❌ use after free
}

void unsafe_tmp_file() {
    char tmpfile[100];
    strcpy(tmpfile, "/tmp/mytemp.txt");  // ❌ predictable tmp file
    FILE *fp = fopen(tmpfile, "w+");
    if (fp) {
        fprintf(fp, "temp\n");
        fclose(fp);
    }
}

void null_pointer_deref() {
    char *ptr = NULL;
    printf("%c\n", ptr[0]);  // ❌ null dereference
}

int main() {
    hardcoded_password_check();
    unsafe_system_command();
    file_write();
    insecure_copy();
    read_sensitive_file();
    memory_leak();
    use_after_free();
    unsafe_tmp_file();
    null_pointer_deref();
    return 0;
}