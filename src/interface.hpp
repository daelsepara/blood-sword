#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "interface_story.hpp"

namespace Interface
{
    std::string AdventurerBackground(Character::Class &Character)
    {
        std::string Background = std::string(Character::ClassName[Character]) + "\n\n" + std::string(Character::Description[Character]);

        std::string Abilities = "";

        for (auto i = 0; i < Character::Abilities[Character].size(); i++)
        {
            if (i > 0)
            {
                Abilities += ", ";
            }

            Abilities += std::string(Abilities::Description[Character::Abilities[Character][i]]);
        }

        if (!Abilities.empty())
        {
            Background += "\n\nABILITIES: " + Abilities;
        }

        return Background;
    }

    Party::Base CreateParty(SDL_Window *Window, SDL_Renderer *Renderer, Book::Type Book)
    {
        auto Party = Party::Base();

        if (Window && Renderer)
        {
            auto FontSize = TTF_FontHeight(Fonts::Normal);

            auto Selection = std::vector<int>();

            auto Controls = std::vector<Button>();

            auto CurrentCharacter = 0;

            auto GridSize = (buttonw + 4 * text_space);

            auto ButtonX = (SCREEN_WIDTH - 6 * GridSize) / 2;

            auto ButtonY = SCREEN_HEIGHT - (buttonh + FontSize + 4 * text_space);

            auto BackgroundWidth = 3 * SCREEN_WIDTH / 5;

            auto BackgroundHeight = 12 * FontSize;

            Controls.push_back(Button(0, Assets::Get(Assets::Type::Warrior), 0, 1, 0, 0, ButtonX, ButtonY, intWH, Control::Type::WARRIOR));
            Controls.push_back(Button(1, Assets::Get(Assets::Type::Trickster), 0, 2, 1, 1, ButtonX + GridSize, ButtonY, intWH, Control::Type::TRICKSTER));
            Controls.push_back(Button(2, Assets::Get(Assets::Type::Sage), 1, 3, 2, 2, ButtonX + 2 * GridSize, ButtonY, intWH, Control::Type::SAGE));
            Controls.push_back(Button(3, Assets::Get(Assets::Type::Enchanter), 2, 4, 3, 3, ButtonX + 3 * GridSize, ButtonY, intWH, Control::Type::ENCHANTER));
            Controls.push_back(Button(4, Assets::Get(Assets::Type::Ok), 3, 5, 4, 4, ButtonX + 4 * GridSize, ButtonY, intWH, Control::Type::CONFIRM));
            Controls.push_back(Button(5, Assets::Get(Assets::Type::Back), 4, 5, 5, 5, ButtonX + 5 * GridSize, ButtonY, intWH, Control::Type::BACK));

            std::vector<Character::Class> Classes = {Character::Class::Warrior, Character::Class::Trickster, Character::Class::Sage, Character::Class::Enchanter};

            auto Hold = false;
            auto Selected = false;
            auto ScrollUp = false;
            auto ScrollDown = false;
            auto Current = 0;
            auto Done = false;

            auto SelectText = Graphics::CreateText("Choose adventurers to add to your party", Fonts::Normal, clrGR, TTF_STYLE_NORMAL);

            while (!Done)
            {
                Graphics::FillWindow(Renderer, intBK);

                if (Current >= 0 && Current < Classes.size())
                {
                    CurrentCharacter = Current;
                }

                if (CurrentCharacter >= 0 && CurrentCharacter < Classes.size())
                {
                    Graphics::RenderImage(Renderer, SelectText, (SCREEN_WIDTH - SelectText->w) / 2, (SCREEN_HEIGHT - BackgroundHeight) / 2 - (SelectText->h + 3 * text_space));

                    Graphics::PutText(Renderer, Interface::AdventurerBackground(Classes[CurrentCharacter]).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, BackgroundWidth, BackgroundHeight, (SCREEN_WIDTH - BackgroundWidth) / 2, (SCREEN_HEIGHT - BackgroundHeight) / 2);

                    Graphics::ThickRect(Renderer, BackgroundWidth + 2 * text_space, BackgroundHeight + 2 * text_space, (SCREEN_WIDTH - (BackgroundWidth + 2 * text_space)) / 2, (SCREEN_HEIGHT - (BackgroundHeight + 2 * text_space)) / 2, intGR, border_pts);
                }

                Graphics::RenderButtons(Renderer, Controls, Current, 0, border_pts);

                for (auto i = 0; i < Classes.size(); i++)
                {
                    if (Engine::InList(Selection, i) && i != Current)
                    {
                        Graphics::ThickRect(Renderer, Controls[i].W, Controls[i].H, Controls[i].X, Controls[i].Y, intGR, border_pts);
                    }
                }

                if (Current >= 0 && Current < Controls.size())
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }

                Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
                {
                    if (Current >= 0 && Current < Classes.size() && !Hold)
                    {
                        if (!Engine::InList(Selection, Current))
                        {
                            Selection.push_back(Current);
                        }
                        else
                        {
                            Engine::Erase(Selection, Current);
                        }
                    }
                    else if (Controls[Current].Type == Control::Type::CONFIRM && !Hold)
                    {
                        if (Selection.size() > 0)
                        {
                            Party.Members.clear();

                            auto Rank = Book::Ranks[Book][Selection.size() - 1];

                            for (auto i = 0; i < Selection.size(); i++)
                            {
                                Party.Members.push_back(Character::Create(Classes[Selection[i]], Book, Rank));
                            }
                        }

                        Done = true;
                    }
                    else if (Controls[Current].Type == Control::Type::BACK && !Hold)
                    {
                        Done = true;

                        Current = -1;

                        Selected = false;
                    }
                }
            }

            if (SelectText)
            {
                SDL_FreeSurface(SelectText);

                SelectText = NULL;
            }
        }

        return Party;
    }

    void MainScreen(SDL_Window *Window, SDL_Renderer *Renderer, Book::Destination Destination)
    {
        // initialize RNG
        Engine::Randomize();

        // initialize books
        Book1::InitializeStories();

        Control::Type ControlTypes[4] = {Control::Type::NEW, Control::Type::LOAD, Control::Type::ENCYCLOPEDIA, Control::Type::EXIT};

        const char *MainChoices[4] = {"NEW GAME", "LOAD GAME", "ABOUT", "EXIT"};

        auto TextButtonX = (SCREEN_WIDTH - 4 * (text_buttonw + text_space)) / 2;

        auto TextButtonY = SCREEN_HEIGHT - (text_buttonh + 6 * text_space);

        auto Controls = Graphics::CreateFixedTextButtons(MainChoices, 4, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);

        for (auto i = 0; i < Controls.size(); i++)
        {
            Controls[i].Fg = clrWH;
            Controls[i].Highlight = intGR;
            Controls[i].Color = intBK;
            Controls[i].Type = ControlTypes[i];
        }

        auto Authors = Graphics::CreateText("DAVE MORRIS & OLIVER JOHNSON's", Fonts::Normal, clrWH, TTF_STYLE_NORMAL);
        auto Logo = Graphics::CreateImage("images/blood-sword.png");

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;
        auto Done = false;

        if (Window && Renderer)
        {
            while (!Done)
            {
                Graphics::FillWindow(Renderer, intBK);

                if (Logo && Authors)
                {
                    auto LogoY = (TextButtonY - (Logo->h + Authors->h + text_space)) / 2;

                    Graphics::RenderImage(Renderer, Authors, (SCREEN_WIDTH - Authors->w) / 2, LogoY);

                    Graphics::RenderImage(Renderer, Logo, (SCREEN_WIDTH - Logo->w) / 2, LogoY + (Authors->h + text_space));
                }

                Graphics::RenderTextButtons(Renderer, Controls, Fonts::Normal, Current, TTF_STYLE_NORMAL);

                Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
                {
                    if (Controls[Current].Type == Control::Type::NEW && !Hold)
                    {
                        // TODO: select number and composition of party
                        auto Party = Interface::CreateParty(Window, Renderer, Destination.first);

                        if (Party.Members.size() > 0)
                        {
                            Interface::StoryScreen(Window, Renderer, Party, Destination);
                        }

                        Current = -1;

                        Selected = false;
                    }
                    else if (Controls[Current].Type == Control::Type::EXIT && !Hold)
                    {
                        Done = true;

                        Current = -1;

                        Selected = false;
                    }
                }
            }
        }

        if (Logo)
        {
            SDL_FreeSurface(Logo);

            Logo = NULL;
        }

        if (Authors)
        {
            SDL_FreeSurface(Authors);

            Authors = NULL;
        }
    }
}
#endif
