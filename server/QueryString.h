#ifndef QUERY_STRING_H
#define QUERY_STRING_H

#pragma once

#include <map>
#include <string>

using namespace std;

class QS {
    map<string, string> parseQueryString(string qstring);
};

#endif
