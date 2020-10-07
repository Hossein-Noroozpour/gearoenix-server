#include "slxs-keys.hpp"

#include <boost/asio/buffer.hpp>
#include <cstddef>
#include <memory>

#include "slxs-configurations.hpp"

void slxs::load_server_certificate(boost::asio::ssl::context& ctx) noexcept
{
    ctx.set_password_callback(
        [](std::size_t, boost::asio::ssl::context_base::password_purpose) {
            return Configurations::get_key_password();
        });

    ctx.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::single_dh_use);

    ctx.use_certificate_chain(boost::asio::buffer(
        Configurations::get_cert().data(), Configurations::get_cert().size()));

    ctx.use_private_key(boost::asio::buffer(Configurations::get_key().data(),
                            Configurations::get_key().size()),
        boost::asio::ssl::context::file_format::pem);

    ctx.use_tmp_dh(boost::asio::buffer(Configurations::get_dh().data(),
        Configurations::get_dh().size()));
}