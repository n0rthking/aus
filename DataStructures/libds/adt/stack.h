#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_hierarchy.h>
#include <libds/amt/explicit_sequence.h>

namespace ds::adt {

    template <typename T>
    class Stack :
        virtual public ADT
    {
    public:
        virtual void push(T element) = 0;
        virtual T& peek() = 0;
        virtual T pop() = 0;
    };

    //----------

    template<typename T>
    class ImplicitStack :
        public Stack<T>,
        public ADS<T>
    {
    public:
        ImplicitStack();
        ImplicitStack(const ImplicitStack& other);

        void push(T element) override;
        T& peek() override;
        T pop() override;

    private:
        amt::IS<T>* getSequence() const;
    };

    //----------

    template<typename T>
    class ExplicitStack :
        public Stack<T>,
        public ADS<T>
    {
    public:
        ExplicitStack();
        ExplicitStack(const ExplicitStack& other);

        void push(T element) override;
        T& peek() override;
        T pop() override;

    private:
        amt::SinglyLS<T>* getSequence() const;
    };

    //----------

    template<typename T>
    ImplicitStack<T>::ImplicitStack() :
        ADS<T>(new amt::IS<T>())
    {
    }

    template<typename T>
    ImplicitStack<T>::ImplicitStack(const ImplicitStack& other) :
        ADS<T>(new amt::IS<T>(), other)
    {
    }

    template<typename T>
    void ImplicitStack<T>::push(T element)
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    T& ImplicitStack<T>::peek()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    T ImplicitStack<T>::pop()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    amt::IS<T>* ImplicitStack<T>::getSequence() const
    {
        return dynamic_cast<amt::IS<T>*>(this->memoryStructure_);
    }

    template<typename T>
    ExplicitStack<T>::ExplicitStack() :
        ADS<T>(new amt::SinglyLS<T>())
    {
    }

    template<typename T>
    ExplicitStack<T>::ExplicitStack(const ExplicitStack& other) :
        ADS<T>(new amt::SinglyLS<T>(), other)
    {
    }

    template<typename T>
    void ExplicitStack<T>::push(T element)
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    T& ExplicitStack<T>::peek()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    T ExplicitStack<T>::pop()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename T>
    amt::SinglyLS<T>* ExplicitStack<T>::getSequence() const
    {
        return dynamic_cast<amt::SinglyLS<T>*>(this->memoryStructure_);
    }
}

