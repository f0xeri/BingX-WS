//
// Created by user on 09.02.2024.
//

#ifndef TRADING_INDICATOR_HPP
#define TRADING_INDICATOR_HPP

#include <cstdint>
#include <map>
#include <ranges>
#include <numeric>
#include "KLineData.hpp"

class Indicator {
public:
    Indicator() = default;
    virtual void updateLast(const std::vector<KLineData> &data) = 0;
    virtual void printLast() = 0;
    virtual ~Indicator() = default;
};

class SMA : public Indicator {
    uint64_t period_;
public:
    std::vector<std::pair<uint64_t, double>> movingAverage_{};
    explicit SMA(uint64_t period) : period_(period) {};
    [[maybe_unused]] void recalcMovingAverage(const std::vector<KLineData> &data) {
        movingAverage_.clear();
        if (data.size() < period_) {
            return;
        }
        movingAverage_.reserve(data.size() - period_ + 1);
        for (auto i = period_; i <= data.size(); ++i) {
            double sum = std::accumulate(data.begin() + i - period_, data.begin() + i, 0.0, [](double sum, const KLineData &data) {
                return sum + data.close;
            });
            movingAverage_.emplace_back(data[i - 1].time, sum / period_);
        }
    }

    void updateLast(const std::vector<KLineData> &data) override {
        if (data.size() <= period_) {
            return;
        }
        auto prev = data.end() - 1;
        double sum = std::accumulate(prev - period_, prev, 0.0, [](double sum, const KLineData &data) {
            return sum + data.close;
        });
        movingAverage_.emplace_back(prev->time, sum / period_);
    }

    void printLast() override {
        if (!movingAverage_.empty()) {
            std::cout << "SMA" << period_ << ": " << movingAverage_.back().second << "\ttimestamp: " << movingAverage_.back().first << "\n";
        }
    }
};

#endif //TRADING_INDICATOR_HPP
