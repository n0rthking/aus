#pragma once

#include <functional>

template<typename DataType, typename ResultType>
class Algorithm {
public:
    template<typename Iterator>
    void findElementsWithProperty(Iterator begin, Iterator end,
        std::function<bool(DataType)> predicate,
        ResultType& result,
        std::function<void(ResultType&, DataType)> inserter) {
        for (auto it = begin; it != end; ++it) {
            if (predicate(*it)) {
                inserter(result, *it);
            }
        }
    };
};
