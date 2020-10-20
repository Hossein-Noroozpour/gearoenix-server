#ifndef SLXS_CORE_CONFIGURATIONS_HPP
#define SLXS_CORE_CONFIGURATIONS_HPP

#include <string>

namespace slxs::core {
class Configurations final {
public:
    enum struct Database {
        Unknown,
        Oracle,
        SqlServer,
        MySql,
        PostgresSql,
    };
private:
    static std::string port;
    static std::string key_password;
    static Database database;
    static std::string connection_string;

    static std::string cert;
    static std::string cert_key;
    static std::string dh;

public:
    static void load() noexcept;

    static const std::string& get_port() noexcept { return port; }
    static const std::string& get_key_password() noexcept { return key_password; }
    static Database get_database() noexcept { return database; }
    static const std::string& get_connection_string() noexcept { return connection_string; }

        static const std::string& get_cert() noexcept { return cert; }
    static const std::string& get_cert_key() noexcept { return cert_key; }
    static const std::string& get_dh() noexcept { return dh; }
};
} // namespace slxs

#endif