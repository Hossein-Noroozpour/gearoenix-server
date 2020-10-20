#include "slxs-db-ora-connection-manager.hpp"
#include "../../core/slxs-core-configurations.hpp"
#include "../../log/slxs-log.hpp"
#include <occi.h>

slxs::database::oracle::ConnectionManager::ConnectionManager() noexcept {
    const auto& con_str = core::Configurations::get_connection_string();
    const auto threads_count = core::Configurations::get_threads_count();
    constexpr const char USER_DEL [] = ";User ID=";
    std::string tmp = con_str.substr(con_str.find(USER_DEL)).substr(sizeof(USER_DEL) - 1);
    const auto user = tmp.substr(0, tmp.find(';'));
    constexpr const char PASS_DEL [] = ";Password=";
    tmp = con_str.substr(con_str.find(PASS_DEL)).substr(sizeof(PASS_DEL) - 1);
    const auto pass = tmp.substr(0, tmp.find(';'));
    LOG_D("Oracle username and password is (" << user << ", " << pass << ")")

    environment = ::oracle::occi::Environment::createEnvironment (::oracle::occi::Environment::DEFAULT);
    if(nullptr == environment) LOG_F("Can not set oracle environment.")
    pool = environment->createStatelessConnectionPool(
            user, pass, con_str, threads_count * 16, threads_count + 1, threads_count,
            ::oracle::occi::StatelessConnectionPool::HOMOGENEOUS );
    if(nullptr == pool) LOG_F("Can not connect to oracle.")
}

slxs::database::oracle::ConnectionManager::~ConnectionManager() noexcept {
    environment->terminateStatelessConnectionPool(pool);
    pool = nullptr;
    ::oracle::occi::Environment::terminateEnvironment(environment);
    environment = nullptr;
}
