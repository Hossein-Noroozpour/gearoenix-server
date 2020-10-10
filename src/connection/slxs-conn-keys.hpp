#ifndef SLXS_CONNECTION_KEYS_HPP
#define SLXS_CONNECTION_KEYS_HPP

#include <boost/asio/ssl/context.hpp>

namespace slxs::connection {
void load_server_certificate(boost::asio::ssl::context& ctx) noexcept;
}

#endif