#include "slxs-db-connection-manager.hpp"
#include "../core/slxs-core-configurations.hpp"
#include "oracle/slxs-db-ora-connection-manager.hpp"
#include "../log/slxs-log.hpp"

slxs::database::ConnectionManager *slxs::database::ConnectionManager::construct() noexcept {
    switch(core::Configurations::get_database()) {
        case core::Configurations::Database::Oracle:
            return new oracle::ConnectionManager();
        default:
            LOG_F("Unexpected/Unimplemented database interface request.")
    }
}
