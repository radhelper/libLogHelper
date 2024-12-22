//
// Created by fernando on 07/03/2022.
//

#ifndef LOGHELPER_FILE_DESCRIPTOR_HPP
#define LOGHELPER_FILE_DESCRIPTOR_HPP

#include <string>
#include <netinet/in.h>

namespace log_helper {
    class FileBase {
    public:
        virtual ~FileBase() = default;

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
        explicit LocalFile(std::string file_path);

        bool write(const std::string &buffer) override;

        std::string get_file_path() override;
    };

    /**
     * Networking file writing
     */
    class UDPFile : virtual public FileBase {
    protected:
        std::string server_ip;
        int32_t port;
        int32_t client_socket;
        sockaddr_in server_address;
        bool is_ecc_enabled;

    public:
        UDPFile(std::string server_ip, int32_t port, bool is_ecc_enabled);

        bool write(const std::string &buffer) override;

        std::string get_file_path() override;
    };

    /**
     * To use both methods
     */
    class LocalAndUDPFile final : public LocalFile, public UDPFile {
    public:
        LocalAndUDPFile(const std::string &file_path, const std::string &server_ip, int32_t port,
                        bool is_ecc_enabled);

        bool write(const std::string &buffer) override;

        std::string get_file_path() override;
    };
} /*END NAMESPACE LOG_HELPER*/
#endif //LOGHELPER_FILE_DESCRIPTOR_HPP
