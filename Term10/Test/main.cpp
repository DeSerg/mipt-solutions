#include <cstdio>
#include <memory.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


constexpr int BufferSize = 16384;

const char *ProtocolHttp = "http";

const char *ProtocolPrefixHttp = "http://";
const int ProtocolPrefixHttpLength = static_cast<int>(strlen(ProtocolPrefixHttp));


class HttpFileDownloader {

public:

    bool file_download(const std::string &url, const std::string &filename) {
        int ipv,r,port;
        std::string protocol, host_name, path, query, url_port;
        std::vector<std::string> ip_addresses;

        size_t pos2,pos3,pos4;

        int offset = 0;
        // detect protocol
        if (url.rfind(ProtocolPrefixHttp, 0) == 0) {
            offset = ProtocolPrefixHttpLength;
            protocol = ProtocolHttp;
        }

        size_t slash_pos_1 = url.find_first_of('/', offset + 1);

        path = slash_pos_1 == std::string::npos ? "" : url.substr(slash_pos_1);

        host_name = std::string( url.begin() + offset, slash_pos_1 != std::string::npos ? url.begin() + slash_pos_1 : url.end() );

        path = (pos2 = path.find('#')) != std::string::npos ? path.substr(0, pos2) : path;

        url_port = (pos3 = host_name.find(':')) != std::string::npos ? host_name.substr(pos3 + 1) : "";

        host_name = host_name.substr(0, pos3 != std::string::npos ? pos3 : host_name.length());

        protocol = offset > 2 ? url.substr(0, offset - 3) : "";

        query = (pos4 = path.find('?')) != std::string::npos ? path.substr(pos4+1) : "";

        path = pos4 != std::string::npos ? path.substr(0,pos4) : path;

        if (query.length() > 0)
        {
            path.reserve( path.length() + 1 + query.length() );
            path.append("?").append(query);
        }

        if (url_port.length()==0 && protocol.length()>0)
        {
            url_port = protocol=="http" ? "80" : "443";
        }

        if (host_name.empty()) {
            std::cout << "Failed to extract host from url" << std::endl;
            return false;
        }

        if (is_ipv6_address(host_name)) {
            std::cout << "IPv6 not supported" << std::endl;
            return false;
        }

        if (is_ipv4_address(host_name)) {
            ip_addresses.emplace_back(host_name);
        } else {
            ip_addresses = dns_lookup(host_name);
        }

        if (ip_addresses.empty()) {
            std::cout << "Failed to parse url: hostname not found";
            return false;
        }

        std::stringstream(url_port) >> port; //string to int
        std::stringstream request;
        request << "GET " << path << " HTTP/1.1\r\n";
        request << "Host: " << host_name << "\r\n\r\n";

        for(int i = 0; i < ip_addresses.size(); ++i) {
            if (http_get(request.str(), ip_addresses[i], port, filename)) {
                return true;
            }
        }

        return false;
    }

private:

    bool is_ipv6_address(const std::string& str)
    {
        struct sockaddr_in6 sa{};
        return inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr)) != 0;
    }

    bool is_ipv4_address(const std::string& str)
    {
        struct sockaddr_in sa{};
        return inet_pton(AF_INET, str.c_str(), &(sa.sin_addr)) != 0;
    }

    //----------------------------------------------------------------------
    std::vector<std::string> dns_lookup(const std::string &host_name)
    {
        std::vector<std::string> output;

        addrinfo hints{};
        addrinfo *res = nullptr;
        addrinfo *p = nullptr;

        int status, ai_family;
        char ip_address[INET6_ADDRSTRLEN];

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if ((status = getaddrinfo(host_name.c_str(), NULL, &hints, &res)) != 0) {
            //cerr << "getaddrinfo: "<< gai_strerror(status) << endl;
            return output;
        }

        //cout << "DNS Lookup: " << host_name << " ipv:" << ipv << endl;

        for(p = res;p != nullptr; p = p->ai_next) {
            void *addr;
            auto ip = reinterpret_cast<struct sockaddr_in *>(p->ai_addr);
            addr = &(ip->sin_addr);

            // convert the IP to a string
            inet_ntop(p->ai_family, addr, ip_address, sizeof(ip_address));
            output.emplace_back(ip_address);
        }

        freeaddrinfo(res); // free the linked list

        return output;
    }

    int socket_connect(const std::string &ip_address, int port)
    {
        int sd = -1;
        struct sockaddr_in sa{};

        memset (&sa, '\0', sizeof(sa));
        sa.sin_family      = AF_INET;
        sa.sin_addr.s_addr = inet_addr(ip_address.c_str());   /* Server IP */
        sa.sin_port        = htons     (static_cast<uint16_t>(port));   /* Server Port number */

        sd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sd > 0 && ::connect(sd, (struct sockaddr*)&sa, sizeof(sa)) == 0) {
            return sd;
        }

        std::cout << "Socket connect error" << std::endl;
        return -1;

    }

    std::string header_value(const std::string &full_header, const std::string& header_name)
    {
        size_t pos = full_header.find(header_name); //case sensitive but probably shouldn't be
        std::string r;
        if (pos != std::string::npos)
        {
            size_t begin = full_header.find_first_not_of(": ", pos + header_name.length());
            size_t until = full_header.find_first_of("\r\n\t ", begin + 1);
            if (begin != std::string::npos && until != std::string::npos)
            {
                r = full_header.substr(begin, until - begin);
            }
        }

        return r;
    }

    bool http_get(const std::string& request, const std::string& ip_address, int port, const std::string& fname)
    {
        std::stringstream header;
        std::string body_delimeter("\r\n\r\n");
        auto body_delimiter_len = static_cast<int>(body_delimeter.length());


        std::ofstream fd(fname.c_str());
        if (!fd.good()) {
            std::cout << "Failed to open file" << std::endl;
            return false;
        }

        int sd = socket_connect(ip_address, port);
        if (sd <= 0) {
            std::cout << "Failed to create socket connection" << std::endl;
            return false;
        }

        auto bytes_sent = ::send(sd, request.c_str(), request.length(), 0);
        if (bytes_sent != request.length()) {
            std::cout << "Failed to send request" << std::endl;
            ::close(sd);
            return false;
        }

        std::vector<char> buffer(BufferSize);

        bool delim_not_found = true;
        int delim_position = 0;
        int delim_end_ind = 0;
        ssize_t bytes_received = 0;

        while (delim_not_found) {
            bytes_received = ::recv(sd, buffer.data(), buffer.size(), 0);
            if (bytes_received <= 0) {
                std::cout << "Failed to read data from socket" << std::endl;
                ::close(sd);
                return false;
            }

            for (int i = 0; i < bytes_received; ++i) {
                header << buffer[i];
                delim_position = buffer[i] == body_delimeter[delim_position] ? ++delim_position : 0;
                if (delim_position >= body_delimiter_len) {
                    delim_not_found = false;
                    delim_end_ind = i;
                    break;
                }
            }
        }

        int bytes_expected;
        std::string h = header.str();
        std::stringstream(header_value(h, "Content-Length")) >> bytes_expected;

        int bytes_written = bytes_received - delim_end_ind;
        fd.write(buffer.data() + delim_end_ind, bytes_written);

        while (bytes_written < bytes_expected) {
            bytes_received = ::recv(sd, buffer.data(), buffer.size(), 0);
            if (bytes_received <= 0) {
                std::cout << "Failed to read data from socket" << std::endl;
                ::close(sd);
                return false;
            }

            if (bytes_written + bytes_received > bytes_expected) {
                bytes_received = bytes_expected - bytes_written;
            }
            bytes_written += bytes_received;

            fd.write(buffer.data(), bytes_received);
        }

        ::close(sd);
        return true;
    }


};


int main(int argc, char* argv[])
{
//    if (argc < 3) {
//        cout << "Invalid arguments" << endl;
//    }
//     download(argv[1], argv[2]);
    auto downloader = HttpFileDownloader();
    downloader.file_download("http://www.cplusplus.com/ico/social_googleplus.png", "img.png");

    return 0;
}
