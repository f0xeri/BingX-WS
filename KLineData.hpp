//
// Created by user on 09.02.2024.
//

#ifndef TRADING_KLINEDATA_HPP
#define TRADING_KLINEDATA_HPP

#include <cstdint>
#include <boost/json/conversion.hpp>
#include "Utils.hpp"

struct KLineData {
    double open;
    double close;
    double high;
    double low;
    double volume;
    uint64_t time;

    friend std::ostream &operator<<(std::ostream &os, const KLineData &data) {
        os << "open: " << data.open << "\tclose: " << data.close << "\thigh: " << data.high << "\tlow: " << data.low << "\tvolume: " << data.volume << "\ttime: " << data.time;
        return os;
    }
};

static void tag_invoke(boost::json::value_from_tag, boost::json::value &jv, const KLineData &c) {
    jv = {
            {"o", c.open},
            {"c", c.close},
            {"h", c.high},
            {"l", c.low},
            {"v", c.volume},
            {"T", c.time},
    };
}

static KLineData tag_invoke(boost::json::value_to_tag<KLineData>, const boost::json::value &jv) {
    KLineData c{};
    boost::json::object const &obj = jv.as_object();
    return KLineData{
            std::stod(value_to<std::string>(obj.at("o"))),
            std::stod(value_to<std::string>(obj.at("c"))),
            std::stod(value_to<std::string>(obj.at("h"))),
            std::stod(value_to<std::string>(obj.at("l"))),
            std::stod(value_to<std::string>(obj.at("v"))),
            value_to<uint64_t>(obj.at("T"))
    };
}

#endif //TRADING_KLINEDATA_HPP
