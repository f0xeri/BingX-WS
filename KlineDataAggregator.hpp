//
// Created by user on 09.02.2024.
//

#ifndef TRADING_KLINEDATAAGGREGATOR_HPP
#define TRADING_KLINEDATAAGGREGATOR_HPP

#include <map>
#include "KLineData.hpp"
#include "Indicator.hpp"

class KlineDataAggregator {
    std::vector<KLineData> data_;
    std::vector<std::unique_ptr<Indicator>> indicators_;

public:
    template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of<Indicator, T>::value>>
    void addIndicator(Args&& ...args) {
        indicators_.emplace_back(std::move(std::make_unique<T>(args...)));
    }

    void setData(KLineData data) {
        if (!data_.empty() && data_.back().time == data.time) {
            data_.back() = data;
            return;
        }
        else {
            data_.push_back(data);
            for (auto &indicator : indicators_) {
                indicator->updateLast(data_);
                indicator->printLast();
            }
        }
    }
};

#endif //TRADING_KLINEDATAAGGREGATOR_HPP
