#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <string>
#include <tuple>
#include <vector>

#include "engine.hpp"

namespace Choice
{
    enum class Type
    {
        Normal = 0,
        Ability,
        Attribute,
        Character,
        Equipment,
        Item,
        Weapon,
        Codeword,
        Shoot,
        Discharge,
        Pay,
        DropWeapon,
        DropItem
    };

    class Base
    {
    public:
        Engine::Destination Destination = {Book::Type::None, 0};

        Engine::Destination DestinationFail = {Book::Type::None, 0};

        Character::Class Character = Character::Class::None;

        Attributes::Type Attribute = Attributes::Type::None;

        Abilities::Type Ability = Abilities::Type::None;

        Code::Word Codeword = Code::Word::NONE;

        Choice::Type Type = Choice::Type::Normal;

        std::vector<Equipment::Base> Equipment = {};

        Equipment::Item Item = Equipment::Item::Any;

        Equipment::Weapon Weapon = Equipment::Weapon::None;

        std::string Text = "";

        std::string Bye = "";

        int Charge = 0;

        int Gold = 0;

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

        Base(const char *choice, Engine::Destination destination, Engine::Destination destinationFail, Character::Class character, Attributes::Type attribute)
        {
            Text = choice;

            Destination = destination;

            DestinationFail = destinationFail;

            Character = character;

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

        Base(const char *choice, Engine::Destination destination, Choice::Type type)
        {
            Text = choice;

            Destination = destination;

            Type = type;
        }

        Base(const char *choice, Engine::Destination destination, Equipment::Item item)
        {
            Text = choice;

            Destination = destination;

            Item = item;

            Type = Choice::Type::Item;
        }

        Base(const char *choice, Engine::Destination destination, Choice::Type type, Equipment::Item item)
        {
            Text = choice;

            Destination = destination;

            Type = type;

            Item = item;
        }

        Base(const char *choice, Engine::Destination destination, Choice::Type type, Equipment::Item item, int charge)
        {
            Text = choice;

            Destination = destination;

            Type = type;

            Item = item;

            Charge = charge;
        }

        Base(const char *choice, Engine::Destination destination, Equipment::Weapon weapon)
        {
            Text = choice;

            Destination = destination;

            Weapon = weapon;

            Type = Choice::Type::Weapon;
        }

        Base(const char *choice, Engine::Destination destination, Choice::Type type, Equipment::Weapon weapon)
        {
            Text = choice;

            Destination = destination;

            Type = type;

            Weapon = weapon;
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

        std::string MapFile = "";

        std::string Image = "";

        bool TopImage = true;

        std::vector<Choice::Base> Choices = {};

        std::vector<Enemy::Base> Enemies = {};

        std::vector<Equipment::Base> Equipment = {};

        int Limit = 0;

        Story::Controls Controls = Story::Controls::None;

        // Handle background events
        virtual Engine::Destination Background(Party::Base &Party) { return {Book::Type::None, 0}; };
        // Handle events before story branches
        virtual void Event(Party::Base &Party){};
        // Jump to next book/section
        virtual Engine::Destination Continue(Party::Base &Party) { return {Book::Type::None, 0}; };
        // Event handlers for combat
        virtual void SetupCombat(Map::Base &Map, Party::Base &Party){};
        virtual void AfterCombat(Party::Base &Party, Combat::Result Result){};

        Base()
        {
        }
    };

    std::vector<Button> StandardControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, Assets::Get(Assets::Type::Up), 0, 1, 0, 1, SCREEN_WIDTH - (buttonw + 4 * text_space), (buttonh + 3 * text_space), intWH, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, Assets::Get(Assets::Type::Down), 0, 2, 0, 2, SCREEN_WIDTH - (buttonw + 4 * text_space), SCREEN_HEIGHT - 3 * (buttonh + 2 * text_space) + text_space, intWH, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        auto IconSize = (buttonw + 2 * text_space);
        auto BoxWidth = (SCREEN_WIDTH - 2 * IconSize) / 2;
        auto OffsetX = IconSize / 2 + BoxWidth + text_space;
        auto OffsetY = SCREEN_HEIGHT - 2 * (IconSize - text_space);
        auto LastX = SCREEN_WIDTH - (2 * IconSize) - (3 * text_space);

        controls.push_back(Button(idx, Assets::Get(Assets::Type::Encyclopedia), idx, idx + 1, compact ? idx : 1, idx, OffsetX, OffsetY, intWH, Control::Type::ENCYCLOPEDIA));
        controls.push_back(Button(idx + 1, Assets::Get(Assets::Type::Map), idx, idx + 2, compact ? idx + 1 : 1, idx, OffsetX + gridsize, OffsetY, intWH, Control::Type::MAP));
        controls.push_back(Button(idx + 2, Assets::Get(Assets::Type::Game), idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 1, OffsetX + 2 * gridsize, OffsetY, intWH, Control::Type::GAME));
        controls.push_back(Button(idx + 3, Assets::Get(Assets::Type::Party), idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 2, OffsetX + 3 * gridsize, OffsetY, intWH, Control::Type::PARTY));
        controls.push_back(Button(idx + 4, Assets::Get(Assets::Type::Right), idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 3, OffsetX + 4 * gridsize, OffsetY, intWH, Control::Type::CONTINUE));
        controls.push_back(Button(idx + 5, Assets::Get(Assets::Type::Exit), idx + 4, idx + 5, compact ? idx + 5 : 1, idx + 4, LastX, OffsetY, intWH, Control::Type::EXIT));

        return controls;
    }

    std::vector<Button> ExitControls()
    {
        auto controls = std::vector<Button>();

        auto IconSize = (buttonw + 2 * text_space);
        auto OffsetY = SCREEN_HEIGHT - 2 * (IconSize - text_space);
        auto LastX = SCREEN_WIDTH - (2 * IconSize) - (3 * text_space);

        controls.push_back(Button(0, Assets::Get(Assets::Type::Exit), 0, 0, 0, 0, LastX, OffsetY, intWH, Control::Type::EXIT));

        return controls;
    }

    class NotImplemented : public Story::Base
    {
    public:
        NotImplemented()
        {
            Book = Book::Type::None;

            Id = -1;

            Text = "This section has not been implemented yet.";

            Controls = Story::Controls::None;
        }
    };

    auto notImplemented = NotImplemented();

    Story::Base *FindStory(int id, std::vector<Story::Base *> &Stories)
    {
        Story::Base *story = &notImplemented;

        bool found = false;

        if (Stories.size() > 0)
        {
            for (auto i = 0; i < Stories.size(); i++)
            {
                if (Stories[i]->Id == id)
                {
                    story = Stories[i];

                    found = true;

                    break;
                }
            }
        }

        return story;
    }
}
#endif