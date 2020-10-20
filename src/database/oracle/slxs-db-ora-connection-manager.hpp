#ifndef SLXS_DATABASE_ORACLE_CONNECTION_MANAGER_HPP
#define SLXS_DATABASE_ORACLE_CONNECTION_MANAGER_HPP

#include "../slxs-db-connection-manager.hpp"

namespace oracle::occi {
    class Environment;
    class StatelessConnectionPool;
}

namespace slxs::database::oracle {
    class ConnectionManager final: public database::ConnectionManager {
    private:
        ::oracle::occi::Environment *environment = nullptr;
        ::oracle::occi::StatelessConnectionPool* pool = nullptr;
    public:
        ConnectionManager() noexcept;
        ~ConnectionManager() noexcept final;
    };
}

#endif