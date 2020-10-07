#ifndef SLXS_KEYS_HPP
#define SLXS_KEYS_HPP

#include <boost/asio/ssl/context.hpp>

namespace slxs {
void load_server_certificate(boost::asio::ssl::context& ctx) noexcept;
}

#endif