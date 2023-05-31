#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Config.h"

class SourceWriter {
   private:
    std::stringstream ss_;
    std::vector<int> columns_;
    int lines_ = 1;
    bool volatile_ = false;
    char lastChar_ = 0;
    int initialColumn_ = 0;
    const Config& config_;

   public:
    SourceWriter(const Config& config) : config_(config) { columns_.push_back(0); }
    SourceWriter(const Config& config, int initialColumn) : config_(config) {
        initialColumn_ = initialColumn;
        columns_.push_back(initialColumn);
    }

    SourceWriter& operator<<(const std::string& str) {
        for (char c : str) {
            if (c == '\n') {
                lines_++;
                columns_.push_back(0);
            } else if (c == '\t') {
                columns_[columns_.size() - 1] += config_.get<int>("tab_width");
            } else {
                columns_[columns_.size() - 1]++;
            }
            lastChar_ = c;
            if (!volatile_) {
                ss_ << c;
            }
        }
        return *this;
    }

    std::string str() { return ss_.str(); }
    std::stringstream& ss() { return ss_; }
    const std::vector<int>& columns() { return columns_; }
    int firstLineColumn() { return columns_.front() - initialColumn_; }
    int lines() const { return lines_; }
    void set_volatile(bool v) { volatile_ = v; }
    bool isLastCharWhiteSpace() const { return ' ' == lastChar_ || '\t' == lastChar_; }
};
