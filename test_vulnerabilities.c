# -*- coding: utf-8 -*-
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for system()
#include <unistd.h> // for access()
#include <fcntl.h> // for open()
#include <sys/stat.h> // for open()
#include <time.h> // for time() and srand()

// ����� �Է� �ùķ��̼� �Լ�
char* get_user_input(const char* prompt) {
    printf("%s", prompt);
    static char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // ���� ���� ����
        buffer[strcspn(buffer, "\n")] = 0;
        return buffer;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    char buffer[10];
    char user_data[256];
    char command[256];
    char temp_filename[L_tmpnam];
    const char* user_format_string = "%s, %d\n"; // Example format string

    printf("--- Semgrep p/security �׽�Ʈ �ڵ� ---\n\n");

    // 1. ���� �����÷ο� (Buffer Overflow) - CWE-119, CWE-120
    // ��Ⱥ� ���̵�: '�޸� ���� �����÷ο�' ����
    printf("[����� 1] ���� �����÷ο� (CWE-120) - strcpy ���\n");
    if (argc > 1) {
        // �ܺ� �Է��� ���� ũ�⺸�� Ŭ ��� �����÷ο� �߻�
        strcpy(buffer, argv[1]); // <--- Semgrep�� Ž���� ���ɼ� ����
        printf("Buffer content: %s\n", buffer);
    } else {
        printf("Usage: %s <long_string_to_overflow>\n", argv[0]);
    }
    printf("----------------------------------------\n\n");

    // 2. ��ɾ� ���� (Command Injection) - CWE-78
    // ��Ⱥ� ���̵�: '�ü�� ��ɾ� ����' ����
    printf("[����� 2] ��ɾ� ���� (CWE-78) - system() ���\n");
    char* input = get_user_input("��ɾ �Է��ϼ��� (��: ls -la): ");
    if (input) {
        sprintf(command, "echo \"%s\"", input); // <--- Semgrep�� Ž���� ���ɼ� ����
        printf("����� ��ɾ�: %s\n", command);
        system(command);
    }
    printf("----------------------------------------\n\n");

    // 3. �������� ���� �ӽ� ���� ���� (Insecure Temporary File Creation) - CWE-377
    // ��Ⱥ� ���̵�: '�������� ���� �ӽ����� ����' ����
    printf("[����� 3] �������� ���� �ӽ� ���� ���� (CWE-377) - tmpnam() ���\n");
    // tmpnam()�� ���� ������ ���� �̸��� �����Ͽ� ���� ���ǿ� �����
    tmpnam(temp_filename); // <--- Semgrep�� Ž���� ���ɼ� ����
    printf("������ �ӽ� ���� �̸�: %s\n", temp_filename);
    // ���� ���� ���� ���� (����)
    int fd = open(temp_filename, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd != -1) {
        printf("�ӽ� ���� ���� ����: %s\n", temp_filename);
        close(fd);
    } else {
        perror("�ӽ� ���� ���� ����");
    }
    printf("----------------------------------------\n\n");

    // 4. ���� ��Ʈ�� ����� (Format String Vulnerability) - CWE-134
    // ��Ⱥ� ���̵�: '���� ��Ʈ�� ����' ����
    printf("[����� 4] ���� ��Ʈ�� ����� (CWE-134)\n");
    char* user_provided_string = get_user_input("����� ���ڿ��� �Է��ϼ��� (��: %x %x %x): ");
    if (user_provided_string) {
        printf(user_provided_string); // <--- Semgrep�� Ž���� ���ɼ� ����
        printf("\n");
    }
    printf("----------------------------------------\n\n");

    // 5. �ϵ��ڵ�� ��й�ȣ (Hardcoded Password) - CWE-798
    // ��Ⱥ� ���̵�: '�ϵ��ڵ�� �߿�����' ����
    printf("[����� 5] �ϵ��ڵ�� ��й�ȣ (CWE-798)\n");
    const char* admin_password = "very_secret_hardcoded_password"; // <--- Semgrep�� Ž���� ���ɼ� ����
    printf("������ ��й�ȣ�� �ڵ忡 ���� ���ԵǾ� �ֽ��ϴ�.\n");
    printf("----------------------------------------\n\n");

    return 0;
}
