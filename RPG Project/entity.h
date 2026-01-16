#pragma once
#include <string>

// Base Entity class for npcs and interactable stuff
class Entity
{
protected:
    std::string name;
    std::string description;

public:
    // constructor
    Entity(const std::string& name, const std::string& description);

    // virtual destructor
    virtual ~Entity() = default;

    // get entity name
    std::string getName() const;

    // get entity description
    std::string getDescription() const;
};
