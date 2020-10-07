#ifndef SLXS_CONFIGURATIONS_HPP
#define SLXS_CONFIGURATIONS_HPP

#include <string>

namespace slxs {
class Configurations final {
private:
    static std::string port;
    static std::string key_password;

    static std::string cert;
    static std::string key;
    static std::string dh;

public:
    static void load() noexcept;

    static const std::string& get_port() noexcept { return port; }
    static const std::string& get_key_password() noexcept { return key_password; }

    static const std::string& get_cert() noexcept { return cert; }
    static const std::string& get_key() noexcept { return key; }
    static const std::string& get_dh() noexcept { return dh; }
};
} // namespace slxs

#endif