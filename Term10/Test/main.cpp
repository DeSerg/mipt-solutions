#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>


constexpr int Bytes2pow10 = 1024;

constexpr int BufferSize = 4096;

const char *ProtocolHttp = "http";

const char *ProtocolPrefixHttp = "http://";
const int ProtocolPrefixHttpLength = strlen(ProtocolPrefixHttp);

const char *ProtocolPrefixHttps = "https://";

const char *BodyDelimeter = "\r\n\r\n";
const int BodyDelimiterLength = strlen(BodyDelimeter);


class HttpFileDownloader {

public:

    bool file_download(const std::string &url, const std::string &filename) {
        std::string host_name;
        std::string path;
        int port = 0;
        if (!url_parse(url, host_name, path, port)) {
            return false;
        }

        std::vector<std::string> ip_addresses;
        if (!dns_lookup(host_name, ip_addresses)) {
            return false;
        }

        std::stringstream request;
        request << "GET " << path << " HTTP/1.1\r\n";
        request << "Host: " << host_name << "\r\n\r\n";

        for (auto &ip_address: ip_addresses) {
            try {
                if (http_get(request.str(), ip_address, port, filename)) {
                    std::cout << "File " << filename << " has been successfully loaded" << std::endl;
                    return true;
                }
            } catch (std::exception &ex) {
                std::cout << "Failed to perform http get for ip " << ip_address << ": " << ex.what() << std::endl;
            }
        }

        std::cout << "File " << filename << " was not loaded..." << std::endl;
        return false;
    }

private:

    bool url_parse(const std::string &url, std::string &host_name, std::string &path, int &port) {

        size_t protocol_offset = url.find("://");
        if (protocol_offset != std::string::npos) {
            auto protocol = url.substr(0, protocol_offset);
            if (protocol != ProtocolHttp) {
                std::cout << "Protocol " << protocol << " is not supported..." << std::endl;
                return false;
            }
            protocol_offset += 3;
        } else {
            protocol_offset = 0;
        }

        size_t slash_pos = url.find_first_of('/', protocol_offset);
        path = slash_pos == std::string::npos ? "" : url.substr(slash_pos);
        auto host_name_end = slash_pos != std::string::npos ? url.begin() + slash_pos : url.end();
        host_name = std::string(url.begin() + protocol_offset, host_name_end);

        size_t fragment_pos = path.find('#');
        if (fragment_pos != std::string::npos) {
            path = path.substr(0, fragment_pos);
        }

        size_t port_pos = host_name.find(':');
        std::string url_port = port_pos != std::string::npos ? host_name.substr(port_pos + 1) : "80";
        if (port_pos != std::string::npos) {
            host_name = host_name.substr(0, port_pos);
        }

        size_t query_pos = path.find('?');
        std::string query = query_pos != std::string::npos ? path.substr(query_pos + 1) : "";
        if (query_pos != std::string::npos) {
            path = path.substr(0, query_pos);
        }

        if (query.length() > 0)
        {
            path.reserve(path.length() + 1 + query.length());
            path.append("?").append(query);
        }

        if (host_name.empty()) {
            std::cout << "Failed to extract host from url" << std::endl;
            return false;
        }

        port = 0;
        std::stringstream(url_port) >> port;
        if (port == 0) {
            std::cout << "Failed to parse url: invalid port: " << url_port << std::endl;
            return false;
        }

        return true;
    }

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

    bool dns_lookup(const std::string &host_name, std::vector<std::string> &ip_addresses)
    {
        if (is_ipv6_address(host_name)) {
            std::cout << "IPv6 addresses are not supported" << std::endl;
            return false;
        }

        if (is_ipv4_address(host_name)) {
            ip_addresses.emplace_back(host_name);
            return true;
        }

        addrinfo hints{};
        char ip_address[INET6_ADDRSTRLEN];

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        addrinfo *res = nullptr;
        if (getaddrinfo(host_name.c_str(), NULL, &hints, &res) != 0) {
            std::cout << "Failed to get ip address of host " << host_name << std::endl;
            return false;
        }

        for(addrinfo *p = res; p != nullptr; p = p->ai_next) {
            auto ip = reinterpret_cast<struct sockaddr_in *>(p->ai_addr);
            void *address = &(ip->sin_addr);

            // convert IP to a string
            inet_ntop(p->ai_family, address, ip_address, sizeof(ip_address));
            ip_addresses.emplace_back(ip_address);
        }

        freeaddrinfo(res);
        return true;
    }

    int socket_connect(const std::string &ip_address, int port)
    {
        struct sockaddr_in sa{};

        sa.sin_family      = AF_INET;
        sa.sin_addr.s_addr = inet_addr(ip_address.c_str());   /* Server IP */
        sa.sin_port        = htons     (static_cast<uint16_t>(port));   /* Server Port number */

        int sd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sd > 0) {
            if (::connect(sd, (struct sockaddr*)&sa, sizeof(sa)) == 0) {
                return sd;
            }

            ::close(sd);
        }

        std::cout << "Socket connection error" << std::endl;
        return -1;

    }

    std::string file_size_readable(int file_size_bytes) {
        std::stringstream ss;
        if (file_size_bytes < Bytes2pow10) {
            ss << file_size_bytes << " b";
        } else if (file_size_bytes < Bytes2pow10 * Bytes2pow10) {
            ss << file_size_bytes / Bytes2pow10 << " K";
        } else {
            ss << file_size_bytes / Bytes2pow10 / Bytes2pow10 << " M";
        }

        return ss.str();
    }

    std::string header_value(const std::string &full_header, const std::string& header_name) {
        size_t pos = full_header.find(header_name);
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
        std::stringstream header;

        // download header
        while (delim_not_found) {
            bytes_received = ::recv(sd, buffer.data(), buffer.size(), 0);
            if (bytes_received <= 0) {
                std::cout << "Failed to read data from socket" << std::endl;
                ::close(sd);
                return false;
            }

            for (int i = 0; i < bytes_received; ++i) {
                header << buffer[i];
                delim_position = buffer[i] == BodyDelimeter[delim_position] ? ++delim_position : 0;
                if (delim_position >= BodyDelimiterLength) {
                    delim_not_found = false;
                    delim_end_ind = i + 1;
                    break;
                }
            }
        }

        // download file
        int bytes_expected = 0;
        std::string h = header.str();
        std::stringstream(header_value(h, "Content-Length")) >> bytes_expected;
        std::cout << "File size: " << file_size_readable(bytes_expected) << std::endl;

        std::cout.setf(std::ios_base::unitbuf);
        std::cout << std::setprecision(2) << std::fixed;

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

            std::cout << "\rProgress: " << 100.0 * bytes_written / bytes_expected << "%";
            fd.write(buffer.data(), bytes_received);
        }

        std::cout << std::endl;
        ::close(sd);
        return true;
    }

};


int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Invalid arguments. Usage: file_downloader url filename" << std::endl;
        return 1;
    }

    auto downloader = HttpFileDownloader();
    downloader.file_download(argv[1], argv[2]);

    return 0;
}
