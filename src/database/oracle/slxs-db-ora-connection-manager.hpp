#ifndef SLXS_DATABASE_ORACLE_CONNECTION_MANAGER_HPP
#define SLXS_DATABASE_ORACLE_CONNECTION_MANAGER_HPP

#include "../slxs-db-connection-manager.hpp"
#include <oci.h>

namespace slxs::database::oracle {
    class ConnectionManager final: public database::ConnectionManager {
    private:
        OCIEnv * environment = nullptr;
        OCIError *error = nullptr;
        OCISession *session = nullptr;
        OCIServer *server = nullptr;
        OCISvcCtx *service_context = nullptr;
    public:
        ConnectionManager() noexcept;
        ~ConnectionManager() noexcept final;
    };
}

#endif