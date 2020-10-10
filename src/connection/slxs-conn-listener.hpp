#ifndef SLXS_CONNECTION_LISTENER_HPP
#define SLXS_CONNECTION_LISTENER_HPP

#include <boost/beast/ssl.hpp>
#include <memory>

namespace slxs::connection {
class Listener final : public std::enable_shared_from_this<Listener> {
private:
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::io_context& ioc;
    boost::asio::ssl::context& ctx;

    void do_accept() noexcept;
    void on_accept(
        boost::beast::error_code ec,
        boost::asio::ip::tcp::socket socket) noexcept;

public:
    Listener(
        boost::asio::io_context& ioc,
        boost::asio::ssl::context& ctx) noexcept;

    void run() noexcept;
};
} // namespace slxs

#endif