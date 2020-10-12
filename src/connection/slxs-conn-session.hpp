#ifndef SLXS_CONNECTION_SESSION_HPP
#define SLXS_CONNECTION_SESSION_HPP

#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

namespace slxs::connection {
class Session : public std::enable_shared_from_this<Session> {
    // struct SendLambda {
    //     Session& self;

    //     explicit SendLambda(Session& self)
    //         : self(self)
    //     {
    //     }

    //     template <bool isRequest, class Body, class Fields>
    //     void operator()(http::message<isRequest, Body, Fields>&& msg) const
    //     {
    //         // The lifetime of the message has to extend
    //         // for the duration of the async operation so
    //         // we use a shared_ptr to manage it.
    //         auto sp = std::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));

    //         // Store a type-erased version of the shared
    //         // pointer in the class to keep it alive.
    //         self.res = sp;

    //         // Write the response
    //         http::async_write(
    //             self.stream, *sp, beast::bind_front_handler(&Session::on_write, self.shared_from_this(), sp->need_eof()));
    //     }
    // };

    boost::beast::ssl_stream<boost::beast::tcp_stream> stream;
    boost::beast::flat_buffer buffer;
    boost::beast::http::request<boost::beast::http::string_body> req;
    std::shared_ptr<void> res;
    // SendLambda send_lambda;

    void set_timeout() noexcept;
    void on_run() noexcept;
    void on_handshake(boost::beast::error_code ec) noexcept;
    void do_read() noexcept;
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred) noexcept;
    void on_write(bool close, boost::beast::error_code ec, std::size_t bytes_transferred) noexcept;
    void do_close() noexcept;
    void on_shutdown(boost::beast::error_code ec) noexcept;

public:
    Session(boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx) noexcept;
    void run() noexcept;
};
}

#endif