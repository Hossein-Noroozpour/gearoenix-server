#ifndef SLXS_DATABASE_ORACLE_CONNECTION_MANAGER_HPP
#define SLXS_DATABASE_ORACLE_CONNECTION_MANAGER_HPP

class Environment;
class StatelessConnectionPool;

namespace slxs::database::oracle {
    class ConnectionManager final {
    private:
        Environment *environment = nullptr;
        StatelessConnectionPool* pool = nullptr;
    public:
        ConnectionManager() noexcept;
        ~ConnectionManager() noexcept;
    };
}

#endif