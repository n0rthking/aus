#pragma once

#include <complexities/complexity_analyzer.h>
#include <iterator>
#include <list>
#include <random>
#include <vector>

namespace ds::utils
{
    /**
     * @brief Common base for list analyzers.
     */
    template<class List>
    class ListAnalyzer : public ComplexityAnalyzer<List>
    {
    protected:
        explicit ListAnalyzer(const std::string& name);

    protected:
        void beforeOperation(List& structure) override;
        size_t getRandomIndex() const;
        int getRandomData() const;

    private:
        void insertNElements(List& list, size_t n);

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngIndex_;
        size_t index_;
        int data_;
    };

    /**
     * @brief Analyzes complexity of the insert operation.
     */
    template<class List>
    class ListInsertAnalyzer : public ListAnalyzer<List>
    {
    public:
        explicit ListInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(List& structure) override;
    };

    /**
     * @brief Analyzes complexity of the erase operation.
     */
    template<class List>
    class ListRemoveAnalyzer : public ListAnalyzer<List>
    {
    public:
        explicit ListRemoveAnalyzer(const std::string& name);

    protected:
        void executeOperation(List& structure) override;
    };

    /**
     * @brief Container for all list analyzers.
     */
    class ListsAnalyzer : public CompositeAnalyzer
    {
    public:
        ListsAnalyzer() :
            CompositeAnalyzer("Lists")
        {
            this->addAnalyzer(std::make_unique<ListInsertAnalyzer<std::vector<int>>>("vector-insert"));
            this->addAnalyzer(std::make_unique<ListInsertAnalyzer<std::list<int>>>("list-insert"));
            this->addAnalyzer(std::make_unique<ListRemoveAnalyzer<std::vector<int>>>("vector-remove"));
            this->addAnalyzer(std::make_unique<ListRemoveAnalyzer<std::list<int>>>("list-remove"));
        }
    };

    template<class List>
    ListAnalyzer<List>::ListAnalyzer(const std::string& name) :
        ComplexityAnalyzer<List>(name, std::function<void(List&, size_t)>()),
        //                             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        // TODO 01 Namiesto defaultne vytvorenÈho - pr·zdneho - funkËnÈho objektu je potrebnÈ
        //         parameter spr·vne inicializovaù!
        rngData_(std::random_device()()),
        rngIndex_(std::random_device()()),
        index_(0),
        data_(0)
    {
    }

    template<class List>
    void ListAnalyzer<List>::beforeOperation(List& structure)
    {
        std::uniform_int_distribution<size_t> indexDist(0, structure.size() - 1);
        index_ = indexDist(this->rngIndex_);
        data_ = rngData_();
    }

    template<class List>
    size_t ListAnalyzer<List>::getRandomIndex() const
    {
        return index_;
    }

    template<class List>
    int ListAnalyzer<List>::getRandomData() const
    {
        return data_;
    }

    template<class List>
    void ListAnalyzer<List>::insertNElements(List& list, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            list.push_back(rngData_());
        }
    }

    template <class List>
    ListInsertAnalyzer<List>::ListInsertAnalyzer(const std::string& name) :
        ListAnalyzer<List>(name)
    {
    }

    template <class List>
    void ListInsertAnalyzer<List>::executeOperation(List& structure)
    {
        // TODO 01
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template <class List>
    ListRemoveAnalyzer<List>::ListRemoveAnalyzer(const std::string& name) :
        ListAnalyzer<List>(name)
    {
    }

    template <class List>
    void ListRemoveAnalyzer<List>::executeOperation(List& structure)
    {
        // TODO 01
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }
}
