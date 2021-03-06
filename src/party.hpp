#ifndef __PARTY_HPP__
#define __PARTY_HPP__

#include <vector>

#include "book.hpp"
#include "enemy.hpp"

namespace Party
{
    class SurvivingEnemies
    {
    public:
        std::vector<Enemy::Base> Enemies = {};

        Book::Type Book = Book::Type::None;

        int Story = 0;

        SurvivingEnemies(Book::Type book, int story, std::vector<Enemy::Base> enemies) : Enemies(enemies), Book(book), Story(story)
        {
        }
    };

    class Base
    {
    public:
        // adventurers
        std::vector<Character::Base> Members = {};

        // completed books
        std::vector<Book::Type> Completed = {};

        // surviving enemies from previous sections
        std::vector<Party::SurvivingEnemies> Enemies = {};

        // code words
        std::vector<Code::Word> Codewords = {};

        // Book number
        Book::Type Book = Book::Type::None;

        // Section Number
        int Story = 0;

        // Pooled Money
        int Gold = 0;

        // Last selected character
        int LastSelected = -1;
        // Last selected value
        int LastValue = -1;

        // for keeping track of locations/actions that can only be visited/done once per character class
        std::map<Character::Class, std::vector<Book::Destination>> Visited = {};
    };
}
#endif
