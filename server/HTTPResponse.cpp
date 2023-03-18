#include "HTTPResponse.h"

#include <string>

using namespace std;

HTTPResponse::HTTPResponse(int status, string conn, string ct) {
    response = protocol + " ";
    setResponseHeader(status);
    setConnection(conn);
    setContentType(ct);
}

HTTPResponse::HTTPResponse(int status, string conn) {
    response = protocol + " ";
    setResponseHeader(status);
    setConnection(conn);
}

HTTPResponse::~HTTPResponse() {
    response = "";
}

void HTTPResponse::setResponseHeader(int status) {
    switch (status) {
        case 200:
            response += OK;
            response += newline;
            break;
        case 404:
            response += NotFound;
            response += newline;
            break;
        case 400:
            response += BadRequest;
            response += newline;
            break;
        case 501:
            response += NotImplemented;
            response += newline;
            break;
    }
}

void HTTPResponse::setConnection(string conn) {
    response += ("Connection: " + conn);
    response += newline;
}

void HTTPResponse::setContentType(string type) {
    if (type.compare("html") == 0)
        response += ct_html;
    else if (type.compare("css") == 0)
        response += ct_css;
    else if (type.compare("js") == 0)
        response += ct_js;
    else if (type.compare("txt") == 0)
        response += ct_txt;
    else if (type.compare("csv") == 0)
        response += ct_csv;
    else if (type.compare("xml") == 0)
        response += ct_xml;
    else if (type.compare("jpg") == 0 || type.compare("jpeg") == 0)
        response += ct_jpg;
    else if (type.compare("png") == 0)
        response += ct_png;
    else if (type.compare("gif") == 0)
        response += ct_gif;
    else if (type.compare("ico") == 0)
        response += ct_ico;
    else if (type.compare("webp") == 0)
        response += ct_webp;
    else if (type.compare("tiff") == 0)
        response += ct_tiff;
    else if (type.compare("svg") == 0)
        response += ct_svg;
    else if (type.compare("djvu") == 0)
        response += ct_djvu;
    else if (type.compare("wav") == 0)
        response += ct_wav;
    else if (type.compare("pdf") == 0)
        response += ct_pdf;
    else if (type.compare("ogg") == 0)
        response += ct_ogg;
    else if (type.compare("json") == 0)
        response += ct_json;
    else if (type.compare("zip") == 0)
        response += ct_zip;
    else if (type.compare("mpeg") == 0)
        response += ct_mpeg;
    else if (type.compare("mp4") == 0)
        response += ct_mp4;
    else if (type.compare("webm") == 0)
        response += ct_webm;
        
    response += newline;
}

void HTTPResponse::addData(string data) {
    response += data;
}

const char* HTTPResponse::getFinalResponse() {
    return response.c_str();
}
