#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <vector>

#include "book.hpp"
#include "character.hpp"
#include "codeword.hpp"
#include "monster.hpp"

namespace Party
{
    class SurvivingMonsters
    {
    public:
        std::vector<Monster::Base> Monsters = {};

        Book::Type Book = Book::Type::None;

        int Story = 0;

        SurvivingMonsters(Book::Type book, int story, std::vector<Monster::Base> monsters) : Monsters(monsters), Book(book), Story(story)
        {
        }
    };

    class Base
    {
    public:
        std::vector<Character::Base> Members = {};

        // completed books
        std::vector<Book::Type> Completed = {};

        // surviving monsters from previous sections
        std::vector<Party::SurvivingMonsters> Monsters = {};

        // list of codewords
        std::vector<Code::Word> Codewords = {};

        Book::Type Book = Book::Type::None;

        int Story = 0;
    };
}
#endif