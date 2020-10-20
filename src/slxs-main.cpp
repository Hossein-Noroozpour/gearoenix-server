
#include "connection/slxs-conn-keys.hpp"
#include "connection/slxs-conn-listener.hpp"
#include "core/slxs-core-configurations.hpp"
#include "log/slxs-log.hpp"
#include "database/slxs-db-connection-manager.hpp"

// #include <boost/asio/dispatch.hpp>
// #include <boost/asio/strand.hpp>
// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
// #include <boost/beast/ssl.hpp>
// #include <boost/beast/version.hpp>
// #include <boost/config.hpp>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// namespace beast = boost::beast;   // from <boost/beast.hpp>
// namespace http = beast::http;     // from <boost/beast/http.hpp>
// namespace net = boost::asio;      // from <boost/asio.hpp>
// namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>
// using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

// // Return a reasonable mime type based on the extension of a file.
// beast::string_view mime_type(beast::string_view path) {
//   using beast::iequals;
//   auto const ext = [&path] {
//     auto const pos = path.rfind(".");
//     if (pos == beast::string_view::npos)
//       return beast::string_view{};
//     return path.substr(pos);
//   }();
//   if (iequals(ext, ".htm"))
//     return "text/html";
//   if (iequals(ext, ".html"))
//     return "text/html";
//   if (iequals(ext, ".php"))
//     return "text/html";
//   if (iequals(ext, ".css"))
//     return "text/css";
//   if (iequals(ext, ".txt"))
//     return "text/plain";
//   if (iequals(ext, ".js"))
//     return "application/javascript";
//   if (iequals(ext, ".json"))
//     return "application/json";
//   if (iequals(ext, ".xml"))
//     return "application/xml";
//   if (iequals(ext, ".swf"))
//     return "application/x-shockwave-flash";
//   if (iequals(ext, ".flv"))
//     return "video/x-flv";
//   if (iequals(ext, ".png"))
//     return "image/png";
//   if (iequals(ext, ".jpe"))
//     return "image/jpeg";
//   if (iequals(ext, ".jpeg"))
//     return "image/jpeg";
//   if (iequals(ext, ".jpg"))
//     return "image/jpeg";
//   if (iequals(ext, ".gif"))
//     return "image/gif";
//   if (iequals(ext, ".bmp"))
//     return "image/bmp";
//   if (iequals(ext, ".ico"))
//     return "image/vnd.microsoft.icon";
//   if (iequals(ext, ".tiff"))
//     return "image/tiff";
//   if (iequals(ext, ".tif"))
//     return "image/tiff";
//   if (iequals(ext, ".svg"))
//     return "image/svg+xml";
//   if (iequals(ext, ".svgz"))
//     return "image/svg+xml";
//   return "application/text";
// }

// // Append an HTTP rel-path to a local filesystem path.
// // The returned path is normalized for the platform.
// std::string path_cat(beast::string_view base, beast::string_view path) {
//   if (base.empty())
//     return std::string(path);
//   std::string result(base);
// #ifdef BOOST_MSVC
//   char constexpr path_separator = '\\';
//   if (result.back() == path_separator)
//     result.resize(result.size() - 1);
//   result.append(path.data(), path.size());
//   for (auto &c : result)
//     if (c == '/')
//       c = path_separator;
// #else
//   char constexpr path_separator = '/';
//   if (result.back() == path_separator)
//     result.resize(result.size() - 1);
//   result.append(path.data(), path.size());
// #endif
//   return result;
// }

// // This function produces an HTTP response for the given
// // request. The type of the response object depends on the
// // contents of the request, so the interface requires the
// // caller to pass a generic lambda for receiving the response.
// template <class Body, class Allocator, class Send>
// void handle_request(beast::string_view doc_root,
//                     http::request<Body, http::basic_fields<Allocator>> &&req,
//                     Send &&send) {
//   // Returns a bad request response
//   auto const bad_request = [&req](beast::string_view why) {
//     http::response<http::string_body> res{http::status::bad_request,
//                                           req.version()};
//     res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     res.set(http::field::content_type, "text/html");
//     res.keep_alive(req.keep_alive());
//     res.body() = std::string(why);
//     res.prepare_payload();
//     return res;
//   };

//   // Returns a not found response
//   auto const not_found = [&req](beast::string_view target) {
//     http::response<http::string_body> res{http::status::not_found,
//                                           req.version()};
//     res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     res.set(http::field::content_type, "text/html");
//     res.keep_alive(req.keep_alive());
//     res.body() = "The resource '" + std::string(target) + "' was not found.";
//     res.prepare_payload();
//     return res;
//   };

//   // Returns a server error response
//   auto const server_error = [&req](beast::string_view what) {
//     http::response<http::string_body>
//     res{http::status::internal_server_error,
//                                           req.version()};
//     res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     res.set(http::field::content_type, "text/html");
//     res.keep_alive(req.keep_alive());
//     res.body() = "An error occurred: '" + std::string(what) + "'";
//     res.prepare_payload();
//     return res;
//   };

//   // Make sure we can handle the method
//   if (req.method() != http::verb::get && req.method() != http::verb::head)
//     return send(bad_request("Unknown HTTP-method"));

//   // Request path must be absolute and not contain "..".
//   if (req.target().empty() || req.target()[0] != '/' ||
//       req.target().find("..") != beast::string_view::npos)
//     return send(bad_request("Illegal request-target"));

//   // Build the path to the requested file
//   std::string path = path_cat(doc_root, req.target());
//   if (req.target().back() == '/')
//     path.append("index.html");

//   // Attempt to open the file
//   beast::error_code ec;
//   http::file_body::value_type body;
//   body.open(path.c_str(), beast::file_mode::scan, ec);

//   // Handle the case where the file doesn't exist
//   if (ec == beast::errc::no_such_file_or_directory)
//     return send(not_found(req.target()));

//   // Handle an unknown error
//   if (ec)
//     return send(server_error(ec.message()));

//   // Cache the size since we need it after the move
//   auto const size = body.size();

//   // Respond to HEAD request
//   if (req.method() == http::verb::head) {
//     http::response<http::empty_body> res{http::status::ok, req.version()};
//     res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     res.set(http::field::content_type, mime_type(path));
//     res.content_length(size);
//     res.keep_alive(req.keep_alive());
//     return send(std::move(res));
//   }

//   // Respond to GET request
//   http::response<http::file_body> res{
//       std::piecewise_construct, std::make_tuple(std::move(body)),
//       std::make_tuple(http::status::ok, req.version())};
//   res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//   res.set(http::field::content_type, mime_type(path));
//   res.content_length(size);
//   res.keep_alive(req.keep_alive());
//   return send(std::move(res));
// }

int main(const int, const char* const* const)
{
    slxs::log::Log::initialize();

    LOG_D("SLXS application started.")

    slxs::core::Configurations::load();

    const int threads_count = slxs::core::Configurations::get_threads_count();

    boost::asio::io_context ioc { threads_count };

    boost::asio::ssl::context ctx { boost::asio::ssl::context::tlsv13 };

    slxs::connection::load_server_certificate(ctx);

    std::shared_ptr<slxs::database::ConnectionManager> db_manager(slxs::database::ConnectionManager::construct());

    std::make_shared<slxs::connection::Listener>(ioc, ctx)->run();

    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    for (auto thread_index = decltype(threads_count) { 0 };
         thread_index < threads_count; ++thread_index)
        threads.emplace_back([&ioc] { ioc.run(); });

    for (auto& thread : threads)
        thread.join();

    return EXIT_SUCCESS;
}