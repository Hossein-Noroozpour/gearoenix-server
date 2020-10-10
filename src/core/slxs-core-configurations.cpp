#include "slxs-core-configurations.hpp"

#include <fstream>

#include "../log/slxs-log.hpp"

constexpr static const char* const cfg_file_name = "../configurations.conf";
constexpr static const char* const cert_file_name = "../cert.pem";
constexpr static const char* const key_file_name = "../key.pem";
constexpr static const char* const dh_file_name = "../dh.pem";

std::string slxs::core::Configurations::port;
std::string slxs::core::Configurations::key_password;

std::string slxs::core::Configurations::cert;
std::string slxs::core::Configurations::key;
std::string slxs::core::Configurations::dh;

std::string read_entire_file(const char* const file_name) noexcept
{
    std::ifstream file(file_name);
    if (!file)
        LOG_F(file_name << " can not be opened.")
    file.seekg(0, std::ios::end);
    std::string buffer(file.tellg(), ' ');
    file.seekg(0);
    file.read(&buffer[0], buffer.size());
    return buffer;
}

void slxs::core::Configurations::load() noexcept
{
    std::ifstream file(cfg_file_name);
    if (!file)
        LOG_F(cfg_file_name << " can not be opened.")
    while (!file.eof()) {
        std::string key;
        file >> key;
        LOG_D("Key of config is: " << key)
        if ("port" == key) {
            file >> port;
            LOG_D("port in config is: " << port)
        } else if ("key-pass" == key) {
            file >> key_password;
            LOG_D("key password in config is: " << key_password)
        }
    }
    if (port.empty())
        LOG_F("port is not specified in config file.")
    if (key_password.empty())
        LOG_F("key password is not specified in config file.")

    cert = read_entire_file(cert_file_name);
    key = read_entire_file(key_file_name);
    dh = read_entire_file(dh_file_name);
}
