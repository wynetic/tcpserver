#include "ParseRequest.h"

#include <iostream>
#include <cstring>

using namespace std;

namespace rp {
    string getHTTPMethod(string request) {
        return request.substr(0, request.find(" "));
    }

    string getURI(string request) {
        string method = getHTTPMethod(request);
        char URI[256];
        int j = 0;
        for (int i = method.length() + 2; request[i] != ' '; i++) {
            URI[j] = request[i];
            j++;
        }
        URI[j] = '\0';
        return string(URI);
    }

    string getFileExtension(string& file_path) {
        int pos = file_path.find(".");
        if (pos == string::npos) // npos is a max value of std::size_t (returned when function fails)
            return "";
        return file_path.substr(pos + 1, file_path.length());
    }

    bool isCGI(string& path) {
        if (path.find("cgi-bin/") == string::npos)
            return false;
        return true;
    }
    
    string getQueryString(string& URI) {
        int pos = URI.find("?");
        if (pos == string::npos)
            return "";
        return URI.substr(pos + 1, URI.length());
    }

    string rmQueryString(string& URI) { // remove query string from request path
        // char path[256];
        int pos_start = URI.find("/"), pos_end = URI.find("?");
        if (pos_start == string::npos || pos_end == string::npos)
            return "";
        return URI.substr(pos_start + 1, pos_end - pos_start - 1);
    }

    map<string, string> parseQueryString(string qstring) {
        map<string, string> dict;
        char key[256], value[256];
        int k = 1, v = 0, j = 0, len = qstring.length();
        for (int i = 0; i <= len; i++) {
            if (qstring[i] == '&' || i == len) {
                value[j] = '\0';
                dict.insert(pair<string, string>(string(key), string(value)));
                bzero(key, sizeof(key));
                bzero(value, sizeof(value));
                k = 1, v = 0, j = 0;
                continue;
            }

            if (qstring[i] == '=') {
                key[j] = '\0';
                k = 0, v = 1, j = 0;
                continue;
            }

            if (k == 1)
                key[j++] = qstring[i];
            else if (v == 1)
                value[j++] = qstring[i];
        }

        return dict;
    }
} // namespace rp
