#ifndef PARSE_REQUEST_H
#define PARSE_REQUEST_H

#pragma once

#include <string>
#include <map>

using namespace std;

namespace rp {
    bool isCGI(string& path);

    string getHTTPMethod(string request);
    string getURI(string request);
    string getFileExtension(string& file_path);
    string getQueryString(string& URI);
    string rmQueryString(string& URI);
    map<string, string> parseQueryString(string qstring);
} // rp

#endif
