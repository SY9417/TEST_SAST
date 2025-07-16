#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

using namespace std;

void insecure_strcpy() {
    char src[100];
    char dst[10];
    cout << "Input string: ";
    cin >> src;
    strcpy(dst, src);  // ❌ buffer overflow
    cout << "Copied: " << dst << endl;
}

void command_injection() {
    char cmd[100];
    cout << "Enter command: ";
    cin >> cmd;
    system(cmd);  // ❌ unsafe
}

void weak_auth() {
    string user, pass;
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
    if (user == "admin" && pass == "admin123")  // ❌ hardcoded creds
        cout << "Welcome Admin\n";
    else
        cout << "Login failed\n";
}

void null_deref() {
    char *ptr = nullptr;
    cout << ptr[0] << endl;  // ❌ null pointer
}

void memory_leak() {
    char *buffer = new char[50];  // ❌ leak
    strcpy(buffer, "Memory Leak Here!");
    cout << buffer << endl;
    // no delete
}

void use_after_delete() {
    char *data = new char[20];
    strcpy(data, "Test");
    delete[] data;
    cout << data << endl;  // ❌ use-after-free
}

void insecure_file_access() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    ofstream file(filename);  // ❌ no path sanitization
    file << "This is test" << endl;
    file.close();
}

void unsafe_tmpfile() {
    char *tmp = (char*)"/tmp/mylog.txt";  // ❌ predictable
    ofstream file(tmp);
    file << "Log entry\n";
    file.close();
}

void access_sensitive_file() {
    ifstream file("/etc/shadow");  // ❌ sensitive
    string line;
    if (!file.is_open()) return;
    while (getline(file, line))
        cout << line << endl;
    file.close();
}

int main() {
    insecure_strcpy();
    command_injection();
    weak_auth();
    null_deref();
    memory_leak();
    use_after_delete();
    insecure_file_access();
    unsafe_tmpfile();
    access_sensitive_file();
    return 0;
}