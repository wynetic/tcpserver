#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>

using namespace std;

class HTTPResponse {
    public:
        string response;

        HTTPResponse(int status, string conn, string ct); // constructor
        HTTPResponse(int status, string conn); // constructor
        ~HTTPResponse(); // destructor
        void setResponseHeader(int status);
        void setConnection(string conn);
        void setContentType(string type);
        void addData(string data);
        const char* getFinalResponse();
    
    private:
        const string protocol = "HTTP/1.1";
        // status code
        const string OK = "200 OK";
        const string NotFound = "404 Not Found";
        const string BadRequest = "400 Bad Request";
        const string NotImplemented = "501 Not Implemented";
        const string newline = "\r\n";

        // image types
        const string ct_jpg = "Content-Type: image/jpeg\r\n";
        const string ct_png = "Content-Type: image/png\r\n";
        const string ct_gif = "Content-Type: image/gif\r\n";
        const string ct_ico = "Content-Type: image/x-icon\r\n";
        const string ct_webp = "Content-Type: image/webp\r\n";
        const string ct_tiff = "Content-Type: image/tiff\r\n";
        const string ct_svg = "Content-Type: image/svg+xml\r\n";
        const string ct_djvu = "Content-Type: image/vnd.djvu\r\n";

        // audio types
        const string ct_wav = "Content-Type: audio/x-wav\r\n";

        // text types
        const string ct_css = "Content-Type: text/css\r\n";
        const string ct_html = "Content-Type: text/html\r\n";
        const string ct_txt = "Content-Type: text/plain\r\n";
        const string ct_csv = "Content-Type: text/csv\r\n";
        const string ct_xml = "Content-Type: text/xml\r\n";

        // application types
        const string ct_pdf = "Content-Type: application/pdf\r\n";
        const string ct_ogg = "Content-Type: application/ogg\r\n";
        const string ct_json = "Content-Type: application/json\r\n";
        const string ct_js = "Content-Type: application/javascript\r\n";
        const string ct_zip = "Content-Type: application/zip\r\n";

        // video types
        const string ct_mpeg = "Content-Type: video/mpeg\r\n";
        const string ct_mp4 = "Content-Type: video/mp4\r\n";
        const string ct_webm = "Content-Type: video/webm\r\n";
};

#endif