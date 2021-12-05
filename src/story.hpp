#ifndef __STORY_HPP__
#define __STORY_HPP__

#include "battle.hpp"
#include "choice.hpp"

namespace Story
{
    enum class Type
    {
        Normal = 0,
        Doom,
        Info
    };

    enum class Controls
    {
        None = 0,
        Standard,
        Exit,
        Info
    };

    class Base
    {
    public:
        Story::Type Type = Story::Type::Normal;

        Book::Type Book = Book::Type::None;

        Book::Destination DestinationReturn = {Book::Type::None, 0};

        int Id = -1;

        int DisplayId = -1;

        std::string Text = "";

        std::string Bye = "";

        std::string Title = "";

        std::string MapFile = "";

        std::string Image = "";

        bool TopImage = true;

        std::vector<Choice::Base> Choices = {};

        std::vector<Equipment::Base> Equipment = {};

        int Limit = 0;

        Battle::Base Battle = Battle::Base();

        std::vector<Enemy::Base> Enemies = {};

        Story::Controls Controls = Story::Controls::None;

        // background events
        virtual Book::Destination Background(Party::Base &Party) { return {Book::Type::None, 0}; };
        
        // events before story branches
        virtual void Event(Party::Base &Party){};
        
        // jump to next book/section
        virtual Book::Destination Continue(Party::Base &Party) { return {Book::Type::None, 0}; };
        
        // event handlers before combat
        virtual void SetupCombat(Map::Base &Map, Party::Base &Party){};
        
        // event handlers after combat
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

    std::vector<Button> InfoControls()
    {
        auto controls = std::vector<Button>();

        auto IconSize = (buttonw + 2 * text_space);
        auto OffsetY = SCREEN_HEIGHT - 2 * (IconSize - text_space);
        auto LastX = SCREEN_WIDTH - (2 * IconSize) - (3 * text_space);

        controls.push_back(Button(0, Assets::Get(Assets::Type::Back), 0, 0, 0, 0, LastX, OffsetY, intWH, Control::Type::BACK));

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
