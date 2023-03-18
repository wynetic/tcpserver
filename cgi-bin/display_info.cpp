#include <fstream>
#include <string>

using namespace std;

// env keys: name, age

int main(int argc, char* argv[]) {
    char buffer[256] = {0};    
    sprintf(buffer, "My name is %s and I'm %d years old ;)\n", getenv("CGI_NAME"), stoi(getenv("CGI_AGE")));

    ofstream out("cgi-bin/tmp.txt", ios::out | ios::trunc);
    if (out.is_open())
        out << buffer;
    
    return 0;
}