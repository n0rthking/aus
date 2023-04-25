#pragma once

class Algorithm {
public:
    template<typename Iterator, typename PredicateType, typename ResultType, typename InserterType>
    static void findElementsWithProperty(Iterator begin, Iterator end, PredicateType predicate, ResultType& result, InserterType inserter) {
        for (auto it = begin; it != end; ++it) {
            if (predicate(*it)) {
                inserter(result, *it);
            }
        }
    };
};
