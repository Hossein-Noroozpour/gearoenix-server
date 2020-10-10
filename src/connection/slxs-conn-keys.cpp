#include "slxs-conn-keys.hpp"

#include <boost/asio/buffer.hpp>
#include <cstddef>
#include <memory>

#include "../core/slxs-core-configurations.hpp"

void slxs::connection::load_server_certificate(boost::asio::ssl::context& ctx) noexcept
{
    ctx.set_password_callback(
        [](std::size_t, boost::asio::ssl::context_base::password_purpose) {
            return core::Configurations::get_key_password();
        });

    ctx.set_options(
        boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::single_dh_use);

    ctx.use_certificate_chain(boost::asio::buffer(
        core::Configurations::get_cert().data(),
        core::Configurations::get_cert().size()));

    ctx.use_private_key(boost::asio::buffer(
                            core::Configurations::get_key().data(),
                            core::Configurations::get_key().size()),
        boost::asio::ssl::context::file_format::pem);

    ctx.use_tmp_dh(boost::asio::buffer(
        core::Configurations::get_dh().data(),
        core::Configurations::get_dh().size()));
}