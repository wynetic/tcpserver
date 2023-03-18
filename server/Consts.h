#ifndef SERVER_CONSTS_H
#define SERVER_CONSTS_H

#pragma once

#include <string>

namespace Consts {
    const int MAX_CLIENTS = 32;

    const std::string SERVER_DATA_PATH = "data/";
    const std::string HOME_PAGE = "static/index.html";
    const std::string FAVICON = "static/favicon.ico";
    const std::string CGI_BIN = "cgi-bin/";
} // namespace Consts

#endif
