#include <iostream>
#include <memory>
#include <string>
#include "BoostIncludes.hpp"
#include "BingXWS.hpp"
#include "KlineDataAggregator.hpp"

int main(int argc, char *argv[]) {
    net::io_context ioc{};
    ssl::context sslc{ssl::context::tlsv13_client};
    KlineDataAggregator data{};
    data.addIndicator<SMA>(3);
    data.addIndicator<SMA>(7);

    auto setDataCallback = [&data](KLineData kLineData) { data.setData(kLineData); };
    std::make_shared<BingXWS<KLineData>>(ioc, sslc)->run("/swap-market", "BTC-USDT@kline_1m", setDataCallback);
    try {
        ioc.run();
    }
    catch (std::exception &ex) {
        std::cout << ex.what() << "\n";
    }
    return 0;
}