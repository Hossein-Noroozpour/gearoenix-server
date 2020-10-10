#include "slxs-conn-listener.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>

#include "../core/slxs-core-configurations.hpp"
#include "../log/slxs-log.hpp"

void slxs::connection::Listener::do_accept() noexcept
{
    acceptor.async_accept(
        boost::asio::make_strand(ioc),
        boost::beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
}

void slxs::connection::Listener::on_accept(
    boost::beast::error_code ec,
    boost::asio::ip::tcp::socket socket) noexcept
{
    if (ec) {
        LOG_E("Problem in accepting: " << ec.message());
    } else {
        // std::make_shared<session>(std::move(socket), ctx_, doc_root_)->run();
    }
    do_accept();
}

slxs::connection::Listener::Listener(
    boost::asio::io_context& ioc,
    boost::asio::ssl::context& ctx) noexcept
    : acceptor(ioc)
    , ioc(ioc)
    , ctx(ctx)
{
    const boost::asio::ip::tcp::endpoint endpoint {
        boost::asio::ip::make_address("0.0.0.0"),
        static_cast<unsigned short>(std::stoi(core::Configurations::get_port()))
    };

    boost::beast::error_code ec;

    acceptor.open(endpoint.protocol(), ec);
    if (ec) {
        LOG_F("Error in opening of acceptor: " << ec.message());
    }

    acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        LOG_F("Error in reuse of address in acceptor: " << ec.message());
    }

    acceptor.bind(endpoint, ec);
    if (ec) {
        LOG_F("Error in binding acceptor to endpoint: " << ec.message());
    }

    acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        LOG_F("Error in listening by acceptor: " << ec.message());
    }
}

void slxs::connection::Listener::run() noexcept
{
    do_accept();
}