#include "QueryString.h"

#include <cstring>

using namespace std;

map<string, string> QS::parseQueryString(string qstring) {
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
