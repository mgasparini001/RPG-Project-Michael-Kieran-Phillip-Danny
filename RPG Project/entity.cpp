#include "entity.h"
#include <string>
using std::string;

// entity constructor
Entity::Entity(const string& name, const string& description)
    : name(name), description(description) {
}

// get entity name
string Entity::getName() const {
    return name;
}

// get entity description
string Entity::getDescription() const {
    return description;
}
