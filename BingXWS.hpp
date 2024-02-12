//
// Created by user on 09.02.2024.
//

#ifndef TRADING_BINGXWS_HPP
#define TRADING_BINGXWS_HPP


#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/json/src.hpp>
#include <boost/json.hpp>
#include "WebSocketSession.hpp"
#include "KLineData.hpp"

namespace uuids = boost::uuids;

template<typename T>
class BingXWS : public WebSocketSession {
    std::string dataType_;
    std::function<void(T)> onData_;
public:
    explicit BingXWS(net::io_context& ioc, ssl::context& ctx) : WebSocketSession(ioc, ctx) {}
    void run(std::string_view path, std::string_view dataType, std::function<void(T)> onData) {
        WebSocketSession::run("open-api-swap.bingx.com", "443", path);
        dataType_ = dataType;
        onData_ = onData;
    }
    void onHandshake(beast::error_code ec) {
        WebSocketSession::onHandshake(ec);
        std::string data = std::format(R"("id": "{}", "reqType": "{}", "dataType": "{}")", uuids::to_string(uuids::random_generator()()), "sub", dataType_);
        write("{" + data + "}");
    }
    void onRead(beast::error_code ec, std::size_t bytes_transferred) {
        WebSocketSession::onRead(ec, bytes_transferred);
        auto msg = decompressGZip(beast::buffers_to_string(buffer_.data()));
        buffer_.consume(bytes_transferred);
        if (msg == "Ping") {
            write("Pong");
        }
        else {
            auto jsonData = boost::json::parse(msg);
            if (jsonData.at("data") != nullptr) {
                auto data = boost::json::value_to<T>(jsonData.at("data").at(0));
                onData_(data);
            }
            read();
        }
    }
};


#endif //TRADING_BINGXWS_HPP
