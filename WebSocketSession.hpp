//
// Created by user on 09.02.2024.
//

#ifndef TRADING_WEBSOCKETSESSION_HPP
#define TRADING_WEBSOCKETSESSION_HPP

#include <memory>
#include "BoostIncludes.hpp"
#include "Utils.hpp"

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
    tcp::resolver resolver_;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> ws_;
public:
    beast::flat_buffer buffer_;
    std::string host_;
    std::string path_;

    explicit WebSocketSession(net::io_context& ioc, ssl::context& ctx) : resolver_(net::make_strand(ioc)), ws_(net::make_strand(ioc), ctx) {};
    virtual void run(std::string_view host, std::string_view port, std::string_view path);
    virtual void onResolve(beast::error_code ec, tcp::resolver::results_type results);
    virtual void onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
    virtual void onSslHandshake(beast::error_code ec);
    virtual void onHandshake(beast::error_code ec);
    virtual void onWrite(beast::error_code ec, std::size_t bytes_transferred);
    virtual void onRead(beast::error_code ec, std::size_t bytes_transferred);
    virtual void onClose(beast::error_code ec);

    virtual void read();
    virtual void write(std::string_view data);
};


#endif //TRADING_WEBSOCKETSESSION_HPP
