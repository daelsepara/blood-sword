#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <vector>

#include "book.hpp"
#include "character.hpp"

namespace Party
{
    class Base
    {
    public:
        std::vector<Character::Base> Members = {};

        // This list is for tracking completed books
        std::vector<Book::Type> Completed = {};

        Book::Type Type = Book::Type::None;

        int Story = 0;
    };
}
#endif