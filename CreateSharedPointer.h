#pragma once
#include <memory>

class IPassValues;

static class CreateSharedPointer
{
public:
    template <class T, typename = std::enable_if_t<std::is_base_of<IPassValues, T>::value>>
    static std::shared_ptr<T> ReturnSharedFromThis(T* type);
    
};

template <class T, typename = std::enable_if_t<std::is_base_of<IPassValues, T>::value>>
std::shared_ptr<T> CreateSharedPointer::ReturnSharedFromThis(T* type)
{
    void* values {type->GetValues()};
    std::shared_ptr<T> sharedPointer {std::make_shared<T>()};
    sharedPointer->SetValues(values);
    
    return sharedPointer;
}
