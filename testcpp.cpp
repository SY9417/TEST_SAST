#include <iostream>
#include <cstring>     // strcpy
#include <cstdlib>     // system, getenv
#include <cstdio>      // sprintf, tmpnam
#include <fstream>     // ofstream
#include <ctime>       // srand
#include <unistd.h>    // access()

using namespace std;

// 사용자 입력 함수
string get_input(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int main(int argc, char* argv[]) {
    char buffer[8];
    char temp_file[L_tmpnam];
    char command[128];

    cout << "--- Semgrep C++ 취약점 테스트 ---" << endl << endl;

    // 1. Buffer Overflow (CWE-120)
    cout << "[1] 버퍼 오버플로우 (strcpy 사용)" << endl;
    if (argc > 1) {
        strcpy(buffer, argv[1]); // ?? Semgrep 탐지 가능 (unbounded-copy-to-stack-buffer)
        cout << "Buffer: " << buffer << endl;
    } else {
        cout << "Usage: ./a.out <string>" << endl;
    }
    cout << endl;

    // 2. Command Injection (CWE-78)
    cout << "[2] 명령어 삽입 (system 사용)" << endl;
    string cmd = get_input("명령어 입력: ");
    sprintf(command, "ls %s", cmd.c_str()); // ?? 탐지 가능 (dont-call-system)
    system(command);
    cout << endl;

    // 3. 임시파일 생성 (CWE-377)
    cout << "[3] 예측 가능한 임시 파일 생성 (tmpnam)" << endl;
    tmpnam(temp_file); // ?? Semgrep이 커스텀 룰로 탐지 가능
    cout << "임시 파일 이름: " << temp_file << endl;
    ofstream ofs(temp_file);
    ofs << "This is insecure temp file test" << endl;
    ofs.close();
    cout << endl;

    // 4. Format String 취약점 (CWE-134)
    cout << "[4] 포맷 스트링 취약점 (printf)" << endl;
    string fmt = get_input("출력할 문자열을 입력하세요 (예: %x %x %x): ");
    printf(fmt.c_str()); // ?? 포맷 스트링 취약점 (Semgrep으로 탐지 가능하게 룰 작성 필요)
    cout << endl;

    // 5. 하드코드된 비밀번호 (CWE-798)
    cout << "[5] 하드코드된 비밀번호" << endl;
    const string admin_pass = "admin1234!"; // ?? 하드코드된 민감정보
    cout << "비밀번호는 코드에 직접 포함되어 있습니다." << endl;
    cout << endl;

    return 0;
}
