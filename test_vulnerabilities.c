# -*- coding: utf-8 -*-
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for system()
#include <unistd.h> // for access()
#include <fcntl.h> // for open()
#include <sys/stat.h> // for open()
#include <time.h> // for time() and srand()

// 사용자 입력 시뮬레이션 함수
char* get_user_input(const char* prompt) {
    printf("%s", prompt);
    static char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // 개행 문자 제거
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

    printf("--- Semgrep p/security 테스트 코드 ---\n\n");

    // 1. 버퍼 오버플로우 (Buffer Overflow) - CWE-119, CWE-120
    // 행안부 가이드: '메모리 버퍼 오버플로우' 관련
    printf("[취약점 1] 버퍼 오버플로우 (CWE-120) - strcpy 사용\n");
    if (argc > 1) {
        // 외부 입력이 버퍼 크기보다 클 경우 오버플로우 발생
        strcpy(buffer, argv[1]); // <--- Semgrep이 탐지할 가능성 높음
        printf("Buffer content: %s\n", buffer);
    } else {
        printf("Usage: %s <long_string_to_overflow>\n", argv[0]);
    }
    printf("----------------------------------------\n\n");

    // 2. 명령어 삽입 (Command Injection) - CWE-78
    // 행안부 가이드: '운영체제 명령어 삽입' 관련
    printf("[취약점 2] 명령어 삽입 (CWE-78) - system() 사용\n");
    char* input = get_user_input("명령어를 입력하세요 (예: ls -la): ");
    if (input) {
        sprintf(command, "echo \"%s\"", input); // <--- Semgrep이 탐지할 가능성 높음
        printf("실행될 명령어: %s\n", command);
        system(command);
    }
    printf("----------------------------------------\n\n");

    // 3. 안전하지 않은 임시 파일 생성 (Insecure Temporary File Creation) - CWE-377
    // 행안부 가이드: '안전하지 않은 임시파일 생성' 관련
    printf("[취약점 3] 안전하지 않은 임시 파일 생성 (CWE-377) - tmpnam() 사용\n");
    // tmpnam()은 예측 가능한 파일 이름을 생성하여 경쟁 조건에 취약함
    tmpnam(temp_filename); // <--- Semgrep이 탐지할 가능성 높음
    printf("생성될 임시 파일 이름: %s\n", temp_filename);
    // 실제 파일 생성 로직 (예시)
    int fd = open(temp_filename, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd != -1) {
        printf("임시 파일 생성 성공: %s\n", temp_filename);
        close(fd);
    } else {
        perror("임시 파일 생성 실패");
    }
    printf("----------------------------------------\n\n");

    // 4. 포맷 스트링 취약점 (Format String Vulnerability) - CWE-134
    // 행안부 가이드: '포맷 스트링 삽입' 관련
    printf("[취약점 4] 포맷 스트링 취약점 (CWE-134)\n");
    char* user_provided_string = get_user_input("출력할 문자열을 입력하세요 (예: %x %x %x): ");
    if (user_provided_string) {
        printf(user_provided_string); // <--- Semgrep이 탐지할 가능성 높음
        printf("\n");
    }
    printf("----------------------------------------\n\n");

    // 5. 하드코드된 비밀번호 (Hardcoded Password) - CWE-798
    // 행안부 가이드: '하드코드된 중요정보' 관련
    printf("[취약점 5] 하드코드된 비밀번호 (CWE-798)\n");
    const char* admin_password = "very_secret_hardcoded_password"; // <--- Semgrep이 탐지할 가능성 높음
    printf("관리자 비밀번호가 코드에 직접 포함되어 있습니다.\n");
    printf("----------------------------------------\n\n");

    return 0;
}
