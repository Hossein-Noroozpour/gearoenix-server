#include "slxs-conn-session.hpp"
#include "../log/slxs-log.hpp"
#include <boost/asio/dispatch.hpp>

void slxs::connection::Session::set_timeout() noexcept
{
    boost::beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(300));
}

void slxs::connection::Session::on_run() noexcept
{
    set_timeout();
    stream.async_handshake(
        boost::asio::ssl::stream_base::server,
        boost::beast::bind_front_handler(&Session::on_handshake, shared_from_this()));
}

void slxs::connection::Session::on_handshake(boost::beast::error_code ec) noexcept
{
    if (ec) {
        LOG_E("Hand shaking failed: " << ec.message())
        return;
    }

    do_read();
}

void slxs::connection::Session::do_read() noexcept
{
    req = {};
    set_timeout();
    boost::beast::http::async_read(stream, buffer, req,
        boost::beast::bind_front_handler(&Session::on_read, shared_from_this()));
}

void slxs::connection::Session::on_read(boost::beast::error_code ec, std::size_t bytes_transferred) noexcept
{
    boost::ignore_unused(bytes_transferred);

    if (ec == boost::beast::http::error::end_of_stream) {
        return do_close();
    }

    if (ec) {
        LOG_E("Read failed: " << ec.message())
        return;
    }

    // Send the response
    // \todo handle_request(*doc_root_, std::move(req_), lambda_);
}

void slxs::connection::Session::on_write(bool close, boost::beast::error_code ec, std::size_t bytes_transferred) noexcept
{
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        LOG_E("Write failed: " << ec.message())
        return;
    }

    if (close) {
        return do_close();
    }
    res = nullptr;
    do_read();
}

void slxs::connection::Session::do_close() noexcept
{
    set_timeout();
    stream.async_shutdown(
        boost::beast::bind_front_handler(&Session::on_shutdown, shared_from_this()));
}

void slxs::connection::Session::on_shutdown(boost::beast::error_code ec) noexcept
{
    if (ec) {
        LOG_E("Shutdown error: " << ec.message())
        return;
    }
}

slxs::connection::Session::Session(boost::asio::ip::tcp::socket&& socket, boost::asio::ssl::context& ctx) noexcept
    : stream(std::move(socket), ctx)
// , send_lambda(*this)
{
}

void slxs::connection::Session::run() noexcept
{
    boost::asio::dispatch(
        stream.get_executor(),
        boost::beast::bind_front_handler(&Session::on_run, shared_from_this()));
}