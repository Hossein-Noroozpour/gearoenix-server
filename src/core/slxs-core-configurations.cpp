#include "slxs-core-configurations.hpp"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "../log/slxs-log.hpp"

constexpr static const char* const cfg_file_name = "../configurations.conf";
constexpr static const char* const cert_file_name = "../cert.pem";
constexpr static const char* const key_file_name = "../key.pem";
constexpr static const char* const dh_file_name = "../dh.pem";

std::string slxs::core::Configurations::port;
std::string slxs::core::Configurations::key_password;
slxs::core::Configurations::Database slxs::core::Configurations::database = slxs::core::Configurations::Database::Unknown;
std::string slxs::core::Configurations::connection_string;
int slxs::core::Configurations::threads_count = -1;

std::string slxs::core::Configurations::cert;
std::string slxs::core::Configurations::cert_key;
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
    constexpr const char *const PORT = "port";
    constexpr const char *const KEY_PASS = "key-pass";
    constexpr const char *const DATABASE = "database";
    constexpr const char *const ORACLE = "oracle";
    constexpr const char *const CONNECTION_STRING = "connection-string";
    constexpr const char *const THREADS_COUNT = "threads-count";
    std::ifstream file(cfg_file_name);
    if (!file)
        LOG_F(cfg_file_name << " can not be opened.")
    while (file.good()) {
        std::string key;
        file >> key;
        LOG_D("Key of config is: " << key)
        if (PORT == key) {
            file >> port;
            LOG_D(PORT << " in config is: " << port)
        } else if (KEY_PASS == key) {
            file >> key_password;
            LOG_D(KEY_PASS << " in config is: " << key_password)
        } else if(DATABASE == key) {
            file >> key;
            if(ORACLE == key) {
                database = Database::Oracle;
            } else {
                LOG_F("Unexpected database configuration (" << key << ") available options are { " << ORACLE << " }")
            }
        } else if(CONNECTION_STRING == key) {
            std::getline(file, connection_string);
            boost::trim(connection_string);
            LOG_D(CONNECTION_STRING << " in config is: " << connection_string)
        } else if(THREADS_COUNT == key) {
            file >> threads_count;
            LOG_D(THREADS_COUNT << " in config is: " << threads_count)
        }
    }
    if (port.empty())
        LOG_F(PORT << " is not specified in config file.")
    if (key_password.empty())
        LOG_F(KEY_PASS << "  is not specified in config file.")
    if (Database::Unknown == database)
        LOG_F(DATABASE << "  is not specified in config file.")
    if (connection_string.empty())
    LOG_F(CONNECTION_STRING << "  is not specified in config file.")
    if (threads_count <= 0)
    LOG_F(THREADS_COUNT << "  is not specified in config file.")

    cert = read_entire_file(cert_file_name);
    cert_key = read_entire_file(key_file_name);
    dh = read_entire_file(dh_file_name);
}
