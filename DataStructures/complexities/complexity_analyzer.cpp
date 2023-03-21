#include <complexities/complexity_analyzer.h>
#include <filesystem>

namespace ds::utils
{
    Analyzer::Analyzer(std::string name) :
        name_(std::move(name))
    {
    }

    const std::string& utils::Analyzer::getName() const
    {
        return name_;
    }

    CompositeAnalyzer::CompositeAnalyzer(const std::string& name) :
        Analyzer(name)
    {
    }

    void CompositeAnalyzer::analyze()
    {
        for (const auto& analyzer : analyzers_)
        {
            analyzer->analyze();
        }
    }

    void CompositeAnalyzer::setOutputDirectory(std::string path)
    {
        for (const auto& analyzer : analyzers_)
        {
            analyzer->setOutputDirectory(path);
        }
    }

    void CompositeAnalyzer::setReplicationCount(size_t count)
    {
        for (const auto& analyzer : analyzers_)
        {
            analyzer->setReplicationCount(count);
        }
    }

    void CompositeAnalyzer::setStepSize(size_t size)
    {
        for (const auto& analyzer : analyzers_)
        {
            analyzer->setStepSize(size);
        }
    }

    void CompositeAnalyzer::setStepCount(size_t count)
    {
        for (const auto& analyzer : analyzers_)
        {
            analyzer->setStepCount(count);
        }
    }

    void CompositeAnalyzer::addAnalyzer(std::unique_ptr<Analyzer> analyzer)
    {
        analyzers_.push_back(std::move(analyzer));
    }

    const std::vector<std::unique_ptr<Analyzer>>& CompositeAnalyzer::getAnalyzers()
    {
        return analyzers_;
    }

    LeafAnalyzer::LeafAnalyzer(const std::string& name) :
        Analyzer(name),
        outputDir_("."),
        replicationCount_(DEFAULT_REPLICATION_COUNT),
        stepSize_(DEFAULT_STEP_SIZE),
        stepCount_(DEFAULT_STEP_COUNT),
        wasSuccessful_(false)
    {
    }

    void LeafAnalyzer::setOutputDirectory(std::string path)
    {
        outputDir_ = std::move(path);
    }

    void LeafAnalyzer::setReplicationCount(size_t count)
    {
        replicationCount_ = count;
    }

    void LeafAnalyzer::setStepSize(size_t size)
    {
        stepSize_ = size;
    }

    void LeafAnalyzer::setStepCount(size_t count)
    {
        stepCount_ = count;
    }

    std::filesystem::path LeafAnalyzer::getOutputPath() const
    {
        return absolute(std::filesystem::path(outputDir_) / this->getName() += ".csv");
    }

    bool LeafAnalyzer::wasSuccessful() const
    {
        return wasSuccessful_;
    }

    void LeafAnalyzer::resetSuccess()
    {
        wasSuccessful_ = false;
    }

    void LeafAnalyzer::setSuccess()
    {
        wasSuccessful_ = true;
    }

    size_t LeafAnalyzer::getReplicationCount() const
    {
        return replicationCount_;
    }

    size_t LeafAnalyzer::getStepSize() const
    {
        return stepSize_;
    }

    size_t LeafAnalyzer::getStepCount() const
    {
        return stepCount_;
    }
}