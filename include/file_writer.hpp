//
// Created by fernando on 07/03/2022.
//

#ifndef LOGHELPER_FILE_DESCRIPTOR_HPP
#define LOGHELPER_FILE_DESCRIPTOR_HPP

#include <fstream>
#include <string>
#include <iostream>
#include <utility>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "common.hpp"

namespace log_helper {
    class FileBase {
    public:
        virtual bool write(const std::string &buffer) = 0;

        virtual std::string get_file_path() = 0;
    };

    /**
     * Only local file writing
     */
    class LocalFile : virtual public FileBase {
    protected:
        std::string file_path;
    public:
        explicit LocalFile(std::string file_path) {
            this->file_path = std::move(file_path);
            std::ofstream output(this->file_path, std::ios::app);
            if (output.fail()) {
                EXCEPTION_MESSAGE("[ERROR in create_log_file(char *)] Unable to open file " + this->file_path);
            }
        }

        bool write(const std::string &buffer) override {
            std::ofstream output(this->file_path, std::ios::app);
            if (output.good()) {
                output << buffer;
                output.close();
                return true;
            }
            EXCEPTION_MESSAGE("[ERROR Unable to open the file " + this->file_path + "]");
            return false;
        }

        std::string get_file_path() override { return this->file_path; }
    };

    /**
     * Networking file writing
     */
    class UDPFile : virtual public FileBase {
    protected:
        std::string server_ip;
        int32_t port;
        int32_t client_socket;
        struct sockaddr_in server_address;
    public:
        UDPFile(std::string server_ip, const int32_t port)
                : server_ip(std::move(server_ip)), port(port), server_address({}) {
            //  Prepare our context and socket
            // Filling server information
            this->client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            this->server_address.sin_family = AF_INET;
            this->server_address.sin_port = htons(this->port);
            inet_pton(AF_INET, this->server_ip.c_str(), &(this->server_address.sin_addr.s_addr));
            if (this->client_socket < -1) {
                EXCEPTION_MESSAGE("[ERROR Unable to create a socket]");
            }
        }

        bool write(const std::string &buffer) override {
            auto error = sendto(this->client_socket, buffer.data(), buffer.size(),
                                MSG_CONFIRM, (const struct sockaddr *) &this->server_address,
                                sizeof(this->server_address));

            if (error < 0) {
                EXCEPTION_MESSAGE("[ERROR Unable to send the message " + buffer + "]");
                return false;
            }
            return true;
        }

        std::string get_file_path() override { return this->server_ip; }

    };

    /**
     * To use both methods TODO: NOT TESTED
     */
    class LocalAndUDPFile : public LocalFile, public UDPFile {
    public:

        LocalAndUDPFile(const std::string &file_path, const std::string &server_ip, const int32_t port)
                : LocalFile(file_path), UDPFile(server_ip, port) {}

        bool write(const std::string &buffer) final {
            return LocalFile::write(buffer) && UDPFile::write(buffer);
        }

        std::string get_file_path() override { return "LOCAL:" + this->file_path + " IP:" + this->server_ip; }

    };

} /*END NAMESPACE LOG_HELPER*/
#endif //LOGHELPER_FILE_DESCRIPTOR_HPP
