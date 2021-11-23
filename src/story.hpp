#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <string>
#include <tuple>
#include <vector>

#include "BloodSword.hpp"

namespace Choice
{
    enum class Type
    {
        Normal = 0,
        Ability,
        Attribute,
        Character,
        Equipment,
        Codeword
    };

    class Base
    {
    public:
        Engine::Destination Destination = {Book::Type::None, 0};

        Character::Class Character = Character::Class::None;

        Attributes::Type Attribute = Attributes::Type::None;

        Abilities::Type Ability = Abilities::Type::None;

        Code::Word Codeword = Code::Word::NONE;

        Choice::Type Type = Choice::Type::Normal;

        std::vector<Equipment::Base> Equipment = {};

        std::string Text = "";

        std::string Bye = "";

        Base(const char *choice, Engine::Destination destination)
        {
            Text = choice;

            Destination = destination;

            Type = Choice::Type::Normal;
        }

        Base(const char *choice, Engine::Destination destination, Character::Class character)
        {
            Text = choice;

            Destination = destination;

            Character = character;

            Type = Choice::Type::Character;
        }

        Base(const char *choice, Engine::Destination destination, Attributes::Type attribute)
        {
            Text = choice;

            Destination = destination;

            Attribute = attribute;

            Type = Choice::Type::Attribute;
        }

        Base(const char *choice, Engine::Destination destination, Abilities::Type ability)
        {
            Text = choice;

            Destination = destination;

            Ability = ability;

            Type = Choice::Type::Ability;
        }

        Base(const char *choice, Engine::Destination destination, Code::Word codeword)
        {
            Text = choice;

            Destination = destination;

            Codeword = codeword;

            Type = Choice::Type::Codeword;
        }

        Base(const char *choice, Engine::Destination destination, std::vector<Equipment::Base> equipment)
        {
            Text = choice;

            Destination = destination;

            Equipment = equipment;

            Type = Choice::Type::Equipment;
        }
    };
}

namespace Story
{
    enum class Type
    {
        Normal = 0,
        Doom
    };

    enum class Controls
    {
        None = 0,
        Standard
    };

    class Base
    {
    public:
        Story::Type Type = Story::Type::Normal;
        Book::Type Book = Book::Type::None;
        int Id = -1;
        int DisplayId = -1;
        std::string Text = "";
        std::string Title = "";
        std::string Image = "";
        std::string Map = "";

        std::vector<Choice::Base> Choices = {};
        std::vector<Enemy::Base> Enemies = {};
        Story::Controls Controls = Story::Controls::None;

        // Handle background events
        virtual Engine::Destination Background(Party::Base &Party) { return {Book::Type::None, 0}; };
        // Handle events before story branches
        virtual void Event(Party::Base &Party){};
        // Jump to next book/section
        virtual Engine::Destination Continue(Party::Base &Party) { return {Book::Type::None, 0}; };

        Base()
        {
        }
    };

    class NotImplemented : public Story::Base
    {
    public:
        NotImplemented()
        {
            Book = Book::Type::None;

            Id = -1;

            Title = "Not implemented yet";

            Controls = {};
        }
    };

    auto notImplemented = NotImplemented();

    Story::Base &FindStory(int id, std::vector<std::reference_wrapper<Story::Base>> &Stories)
    {
        Story::Base &story = notImplemented;

        if (Stories.size() > 0)
        {
            for (auto i = 0; i < Stories.size(); i++)
            {
                if (Stories[i].get().Id == id)
                {
                    story = Stories[i].get();

                    break;
                }
            }
        }

        return story;
    }
}
#endif