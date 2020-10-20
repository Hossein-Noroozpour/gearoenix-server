#ifndef SLXS_DATABASE_CONNECTION_MANAGER_HPP
#define SLXS_DATABASE_CONNECTION_MANAGER_HPP

namespace slxs::database {
    class ConnectionManager {
    protected:
        ConnectionManager() noexcept = default;
    public:
        [[nodiscard]] static ConnectionManager* construct() noexcept;
        virtual ~ConnectionManager() noexcept = default;
    };
}

#endif