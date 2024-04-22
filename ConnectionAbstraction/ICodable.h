#pragma once
#include "Packet.h"

#include <vector>
#include <list>

class ICodable
{
public:
    virtual ~ICodable() = default;
    virtual void Code(sf::Packet& packet) = 0;
    virtual void Decode(sf::Packet& packet) = 0;
};

inline sf::Packet& operator << (sf::Packet& packet, ICodable& codable)
{
    codable.Code(packet);
    return packet;
}

inline sf::Packet& operator >> (sf::Packet& packet, ICodable& codable)
{
    codable.Decode(packet);
    return packet;
}

template <typename T, typename = std::enable_if_t<std::is_base_of<ICodable, T>::value>>
class CodablePointerVector : public std::vector<std::shared_ptr<T>>, public ICodable
{
    virtual void Code(sf::Packet& packet) override
    {
        const sf::Uint64 count{ this->size() };
        packet << count;
        for (const auto& codable : *this)
        {
            codable->Code(packet);
        }
    }

    virtual void Decode(sf::Packet& packet) override
    {
        sf::Uint64 count{ this->size() };
        packet >> count;
        for (sf::Uint64 i = 0; i < count; i++)
        {
            std::shared_ptr<T> codable{ std::make_shared<T>() };
            codable->Decode(packet);
            this->emplace_back(codable);
        }
    }
};

template <typename T, typename = std::enable_if_t<std::is_base_of<ICodable, T>::value>>
class CodablePointerList : public std::list<std::shared_ptr<T>>, public ICodable
{
    virtual void Code(sf::Packet& packet) override
    {
        const sf::Uint64 count{ this->size() };
        packet << count;
        for (const auto& codable : *this)
        {
            codable->Code(packet);
        }
    }

    virtual void Decode(sf::Packet& packet) override
    {
        sf::Uint64 count{ this->size() };
        packet >> count;
        for (sf::Uint64 i = 0; i < count; i++)
        {
            std::shared_ptr<T> codable{ std::make_shared<T>() };
            codable->Decode(packet);
            this->emplace_back(codable);
        }
    }
};