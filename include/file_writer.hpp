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
    struct FileBase {
        virtual bool write(const std::string &buffer) {
            THROW_EXCEPTION("[Declare only the LocalFile, UDPFile, or LocalAndUDP classes]\n");
        }
    };

    /**
     * Only local file writing
     */
    struct LocalFile : virtual public FileBase {
        std::string file_path;

        explicit LocalFile(std::string file_path) : file_path(std::move(file_path)) {
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

    };

    /**
     * Networking file writing
     */
    struct UDPFile : virtual public FileBase {
        std::string server_ip;
        int32_t port;
        int32_t client_socket;
        struct sockaddr_in server_address;

        UDPFile(std::string server_ip, const int32_t port)
                : server_ip(std::move(server_ip)), port(port), server_address({}) {
            //  Prepare our context and socket
            // Filling server information
            this->client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            this->server_address.sin_family = AF_INET;
            this->server_address.sin_port = htons(this->port);
            // this->server_address.sin_addr.s_addr = INADDR_ANY;
            // store this IP address in sa:
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
    };

    /**
     * To use both methods TODO: NOT TESTED
     */
    struct LocalAndUDPFile : public LocalFile, public UDPFile {
        LocalAndUDPFile(const std::string &file_path, const std::string &server_ip, const int32_t port)
                : LocalFile(file_path), UDPFile(server_ip, port) {
        }

        bool write(const std::string &buffer) final {
            return LocalFile::write(buffer) && UDPFile::write(buffer);
        }
    };

} /*END NAMESPACE LOG_HELPER*/
#endif //LOGHELPER_FILE_DESCRIPTOR_HPP
