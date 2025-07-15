#include <iostream>
#include <cstring>     // strcpy
#include <cstdlib>     // system, getenv
#include <cstdio>      // sprintf, tmpnam
#include <fstream>     // ofstream
#include <ctime>       // srand
#include <unistd.h>    // access()

using namespace std;

// ����� �Է� �Լ�
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

    cout << "--- Semgrep C++ ����� �׽�Ʈ ---" << endl << endl;

    // 1. Buffer Overflow (CWE-120)
    cout << "[1] ���� �����÷ο� (strcpy ���)" << endl;
    if (argc > 1) {
        strcpy(buffer, argv[1]); // ?? Semgrep Ž�� ���� (unbounded-copy-to-stack-buffer)
        cout << "Buffer: " << buffer << endl;
    } else {
        cout << "Usage: ./a.out <string>" << endl;
    }
    cout << endl;

    // 2. Command Injection (CWE-78)
    cout << "[2] ��ɾ� ���� (system ���)" << endl;
    string cmd = get_input("��ɾ� �Է�: ");
    sprintf(command, "ls %s", cmd.c_str()); // ?? Ž�� ���� (dont-call-system)
    system(command);
    cout << endl;

    // 3. �ӽ����� ���� (CWE-377)
    cout << "[3] ���� ������ �ӽ� ���� ���� (tmpnam)" << endl;
    tmpnam(temp_file); // ?? Semgrep�� Ŀ���� ��� Ž�� ����
    cout << "�ӽ� ���� �̸�: " << temp_file << endl;
    ofstream ofs(temp_file);
    ofs << "This is insecure temp file test" << endl;
    ofs.close();
    cout << endl;

    // 4. Format String ����� (CWE-134)
    cout << "[4] ���� ��Ʈ�� ����� (printf)" << endl;
    string fmt = get_input("����� ���ڿ��� �Է��ϼ��� (��: %x %x %x): ");
    printf(fmt.c_str()); // ?? ���� ��Ʈ�� ����� (Semgrep���� Ž�� �����ϰ� �� �ۼ� �ʿ�)
    cout << endl;

    // 5. �ϵ��ڵ�� ��й�ȣ (CWE-798)
    cout << "[5] �ϵ��ڵ�� ��й�ȣ" << endl;
    const string admin_pass = "admin1234!"; // ?? �ϵ��ڵ�� �ΰ�����
    cout << "��й�ȣ�� �ڵ忡 ���� ���ԵǾ� �ֽ��ϴ�." << endl;
    cout << endl;

    return 0;
}
