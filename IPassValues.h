#pragma once

class IPassValues
{
    virtual void* GetValues() const = 0;

    virtual void SetValues(void* data) = 0;    
};
