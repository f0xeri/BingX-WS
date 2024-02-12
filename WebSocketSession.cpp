//
// Created by user on 09.02.2024.
//

#include "WebSocketSession.hpp"

void WebSocketSession::run(std::string_view host, std::string_view port, std::string_view path) {
    host_ = host;
    path_ = path;
    resolver_.async_resolve(host.data(), port.data(), beast::bind_front_handler(&WebSocketSession::onResolve, shared_from_this()));
}

void WebSocketSession::onResolve(beast::error_code ec, tcp::resolver::results_type results) {
    if (ec) return fail(ec, "resolve");
    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
    beast::get_lowest_layer(ws_).async_connect(results, beast::bind_front_handler(&WebSocketSession::onConnect, shared_from_this()));
}

void WebSocketSession::onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
    if (ec) return fail(ec, "connect");
    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

    if(!SSL_set_tlsext_host_name(ws_.next_layer().native_handle(), host_.c_str())) {
        ec = beast::error_code(static_cast<int>(::ERR_get_error()),net::error::get_ssl_category());
        return fail(ec, "connect");
    }
    // host_ += ':' + std::to_string(ep.port());    // from official example, but this line breaks everything
    ws_.next_layer().async_handshake(ssl::stream_base::client, beast::bind_front_handler(&WebSocketSession::onSslHandshake, shared_from_this()));
}

void WebSocketSession::onSslHandshake(beast::error_code ec) {
    if (ec) return fail(ec, "ssl_handshake");
    beast::get_lowest_layer(ws_).expires_never();
    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
    ws_.async_handshake(host_, path_, beast::bind_front_handler(&WebSocketSession::onHandshake, shared_from_this()));
}

void WebSocketSession::onHandshake(beast::error_code ec) {
    if (ec) return fail(ec, "handshake");
}

void WebSocketSession::onWrite(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if(ec) return fail(ec, "write");
    ws_.async_read(buffer_, beast::bind_front_handler(&WebSocketSession::onRead, shared_from_this()));
}

void WebSocketSession::onRead(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec) return fail(ec, "read");
}

void WebSocketSession::onClose(beast::error_code ec) {
    if (ec) return fail(ec, "close");
}

void WebSocketSession::read() {
    ws_.async_read(buffer_, beast::bind_front_handler(&WebSocketSession::onRead, shared_from_this()));
}

void WebSocketSession::write(std::string_view data) {
    ws_.async_write(net::buffer(data), beast::bind_front_handler(&WebSocketSession::onWrite, shared_from_this()));
}