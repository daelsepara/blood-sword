#ifndef __INTERFACE_CHOICE_HPP__
#define __INTERFACE_CHOICE_HPP__

#include "interface_common.hpp"

namespace Interface
{
    void RenderChoiceScreen(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, int Current)
    {
        Interface::RenderLeftPanel(Window, Renderer, Party, Story, Screen, Controls);

        // render choice boxes
        for (auto i = 0; i < Controls.size(); i++)
        {
            if (Controls[i].Type == Control::Type::CHOICE)
            {
                Graphics::FillRect(Renderer, Controls[i].W + text_space, Controls[i].H + text_space, Controls[i].X - border_pts, Controls[i].Y - border_pts, intGR);
            }
        }

        Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);
    }

    void RenderMessage(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &ChoiceScreen, int Current, std::string Message, Uint32 FlashColor)
    {
        Uint32 Duration = 1500;

        Interface::RenderChoiceScreen(Window, Renderer, Party, Story, Screen, ChoiceScreen, Current);

        auto FlashW = 3 * Screen.Width / 5;

        auto FlashH = Screen.Height / 5;

        Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, FlashH, Screen.X + (Screen.Width - FlashW) / 2, Screen.Y + (Screen.Height - FlashH) / 2);

        if (FlashColor == intBK)
        {
            Graphics::DrawRect(Renderer, FlashW, FlashH, Screen.X + (Screen.Width - FlashW) / 2, Screen.Y + (Screen.Height - FlashH) / 2, intWH);
        }

        SDL_RenderPresent(Renderer);

        SDL_Delay(Duration);
    }

    int Choose(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &ChoiceScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Assets::Type> Assets, std::vector<std::string> Captions, const char *Message)
    {
        auto Result = -1;

        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * Screen.Width / 5;
        auto WindowH = 4 * FontSize + Screen.IconSize;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto ButtonX = WindowX + 2 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<Button> Controls = {};

        auto NumControls = 0;

        for (auto i = 0; i < Assets.size(); i++)
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets[NumControls]), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * (Screen.IconSize + 2 * text_space), WindowY + Screen.IconSize, intWH, Control::Type::CHOICE));

            NumControls++;
        }

        Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Back), NumControls > 0 ? NumControls - 1 : 0, NumControls, NumControls, NumControls, ButtonX + NumControls * (Screen.IconSize + 2 * text_space), WindowY + Screen.IconSize, intWH, Control::Type::BACK));

        auto Done = false;

        while (!Done)
        {
            Interface::RenderChoiceScreen(Window, Renderer, Party, Story, Screen, ChoiceScreen, -1);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, Message, Fonts::Normal, text_space, clrGR, intBK, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), ButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                if (Controls[Current].Type != Control::Type::BACK)
                {
                    if (Captions.size() > 0)
                    {
                        Interface::RenderChoiceCaption(Renderer, Controls[Current], Captions[Current], clrWH, intBK);
                    }
                }
                else
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    Result = -1;

                    Done = true;
                }
                else
                {
                    Result = Current;

                    Done = true;
                }
            }
        }

        return Result;
    }

    void ItemScreen(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &ChoiceScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, int Character, Equipment::Mode Mode)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * Screen.Width / 5;
        auto WindowH = 12 * FontSize + Screen.IconSize;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto WindowTextWidth = WindowW - 4 * text_space;
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + 2 * text_space;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;
        auto ScrollSpeed = 1;

        auto ItemOffset = 0;
        auto Limit = (8 * FontSize) / (2 * FontSize + 2 * text_space);
        auto Last = ItemOffset + Limit;

        std::vector<Equipment::Base> &Equipment = Party.Members[Character].Equipment;

        if (Last > Equipment.size())
        {
            Last = Equipment.size();
        }

        auto Fg = clrGR;

        auto Bg = intWH;

        auto Highlight = intBK;

        auto Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Mode);

        auto Done = false;

        auto Selection = std::vector<int>();

        while (!Done)
        {
            if (Equipment.size() > Party.Members[Character].Encumbrance)
            {
                Mode = Equipment::Mode::DROP;
            }

            Interface::RenderChoiceScreen(Window, Renderer, Party, Story, Screen, ChoiceScreen, -1);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            if (Mode == Equipment::Mode::DROP)
            {
                Graphics::PutText(Renderer, ("The " + std::string(Character::ClassName[Party.Members[Character].Class]) + " needs to drop items").c_str(), Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);
            }
            else
            {
                Graphics::PutText(Renderer, (std::string(Character::ClassName[Party.Members[Character].Class]) + "'s items").c_str(), Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);
            }

            // render choice boxes
            for (auto i = 0; i < Controls.size(); i++)
            {
                if (Controls[i].Type == Control::Type::CHOICE)
                {
                    if (Engine::InList(Selection, ItemOffset + i))
                    {
                        Graphics::ThickRect(Renderer, Controls[i].W, Controls[i].H, Controls[i].X, Controls[i].Y, intGR, border_pts);
                    }
                    else
                    {
                        Graphics::DrawRect(Renderer, Controls[i].W + 2 * border_pts, Controls[i].H + 2 * border_pts, Controls[i].X - border_pts, Controls[i].Y - border_pts, intGR);
                    }
                }
            }

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrBK, intWH);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    if (Equipment.size() > Party.Members[Character].Encumbrance)
                    {
                        Mode = Equipment::Mode::DROP;
                    }
                    else
                    {
                        Done = true;
                    }
                }
                else if (Controls[Current].Type == Control::Type::SCROLL_UP || (Controls[Current].Type == Control::Type::SCROLL_UP && Hold) || ScrollUp)
                {
                    if (ItemOffset > 0)
                    {
                        ItemOffset -= ScrollSpeed;

                        if (ItemOffset < 0)
                        {
                            ItemOffset = 0;
                        }

                        Last = ItemOffset + Limit;

                        if (Last > Equipment.size())
                        {
                            Last = Equipment.size();
                        }

                        Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Mode);

                        SDL_Delay(50);
                    }

                    if (ItemOffset <= 0)
                    {
                        Current = -1;

                        Selected = false;
                    }
                }
                else if (Controls[Current].Type == Control::Type::SCROLL_DOWN || (Controls[Current].Type == Control::Type::SCROLL_DOWN && Hold) || ScrollDown)
                {
                    if (Equipment.size() - Last > 0)
                    {
                        if (ItemOffset < Equipment.size() - Limit)
                        {
                            ItemOffset += ScrollSpeed;
                        }

                        if (ItemOffset > Equipment.size() - Limit)
                        {
                            ItemOffset = Equipment.size() - Limit;
                        }

                        Last = ItemOffset + Limit;

                        if (Last > Equipment.size())
                        {
                            Last = Equipment.size();
                        }

                        Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Mode);

                        SDL_Delay(50);

                        if (ItemOffset > 0)
                        {
                            Current = Interface::FindControl(Controls, Control::Type::SCROLL_DOWN);
                        }
                    }

                    if (Equipment.size() - Last <= 0)
                    {
                        Selected = false;

                        Current = -1;
                    }
                }
                else if (Controls[Current].Type == Control::Type::CHOICE && !Hold)
                {
                    if (!Engine::InList(Selection, Current + ItemOffset))
                    {
                        Selection.push_back(Current + ItemOffset);
                    }
                    else
                    {
                        Engine::Erase(Selection, Current + ItemOffset);
                    }
                }
                else if (Controls[Current].Type == Control::Type::DROP && !Hold)
                {
                    if (Equipment.size() > 0 && Selection.size() > 0)
                    {
                        std::string Message = "You are about to drop " + (Selection.size() > 1 ? std::string("several items") : "the " + Equipment[Selection[0]].String());

                        std::string Options = Selection.size() > 1 ? "Drop items" : ("Drop the " + Equipment[Selection[0]].String());

                        auto Result = Interface::Choose(Window, Renderer, ChoiceScreen, Party, Story, Screen, {Assets::Type::Ok}, {Options.c_str()}, Message.c_str());

                        if (Result == 0)
                        {
                            auto New = std::vector<Equipment::Base>();

                            for (auto i = 0; i < Equipment.size(); i++)
                            {
                                if (!Engine::InList(Selection, i) || (Equipment[i].Item == Equipment::Item::HeartOfSkrymir && Party.Book == Book::Type::Book1) || (Equipment[i].Item == Equipment::Item::ScorchedSkull && Party.Story == 38 && Party.Book == Book::Type::Book1))
                                {
                                    New.push_back(Equipment[i]);
                                }
                            }

                            Equipment = New;

                            Selection.clear();

                            if (Equipment.size() == 0 || (Mode == Equipment::Mode::DROP && Equipment.size() <= Party.Members[Character].Encumbrance))
                            {
                                Done = true;
                            }
                            else
                            {
                                ItemOffset = 0;

                                Last = ItemOffset + Limit;

                                if (Last > Equipment.size())
                                {
                                    Last = Equipment.size();
                                }

                                Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Mode);

                                Current = -1;
                            }
                        }
                    }
                }
            }
        }
    }

    int SelectAdventurer(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &ChoiceScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, const char *SelectMessage)
    {
        auto Result = -1;
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * Screen.Width / 5;
        auto WindowH = 2 * Screen.TextBoxHeight / 5;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + (WindowW - (Party.Members.size() + 1) * Screen.IconSize) / 2;

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        auto OffsetY = (WindowY + WindowH) - (Screen.IconSize + 2 * text_space + FontSize);

        auto Controls = std::vector<Button>();

        for (auto i = 0; i < Party.Members.size(); i++)
        {
            Controls.push_back(Button(i, Assets::Get(Party.Members[i].Asset), i > 0 ? i - 1 : i, i + 1, i, i, ButtonX + i * Screen.IconSize, OffsetY, intWH, Interface::PlayerControls[Party.Members[i].Class]));
        }

        auto idx = Controls.size();

        Controls.push_back(Button(idx, Assets::Get(Assets::Type::Back), idx > 0 ? idx - 1 : idx, idx, idx, idx, ButtonX + idx * Screen.IconSize, OffsetY, intWH, Control::Type::BACK));

        auto Done = false;

        auto MessageW = -1;

        auto MessageH = -1;

        TTF_SizeText(Fonts::Normal, SelectMessage, &MessageW, &MessageH);

        while (!Done)
        {
            Interface::RenderChoiceScreen(Window, Renderer, Party, Story, Screen, ChoiceScreen, -1);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, SelectMessage, Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, MessageW + 2 * text_space, RowHeight, WindowX + (WindowW - MessageW) / 2, TextY);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, 4);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrGR, intBK);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    Done = true;
                }
                else if (Current >= 0 && Current < Party.Members.size() && !Hold)
                {
                    Result = Current;

                    Done = true;
                }
            }
        }

        return Result;
    }

    std::vector<Button> CreateChoices(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Choice::Base> &Choices, Interface::ScreenDimensions &Screen, int Start, int Last, int Limit, SDL_Color Fg, Uint32 Bg, Uint32 Highlight)
    {
        auto ChoicesText = std::vector<std::string>();

        for (auto i = 0; i < Choices.size(); i++)
        {
            ChoicesText.push_back(Choices[i].Text);
        }

        return Interface::ChoiceList(Window, Renderer, ChoicesText, Screen, Start, Last, Limit, Fg, Bg, Highlight);
    }

    Attributes::Result Test(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &ChoiceScreen, Interface::ScreenDimensions &Screen, Story::Base *Story, Party::Base &Party, int Character, Attributes::Type Attribute)
    {
        auto Result = Attributes::Result::NONE;
        auto WindowW = 3 * Screen.Width / 5;
        auto WindowH = 4 * Screen.TextBoxHeight / 5;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto ColumnWidth = WindowW - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 3 * text_space;

        const char *ResistChoices[2] = {"TEST"};

        auto ResistControls = Graphics::CreateFixedTextButtons(ResistChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        ResistControls[0].Fg = clrWH;
        ResistControls[0].Highlight = intGR;
        ResistControls[0].Color = intBK;
        ResistControls[0].Type = Control::Type::TEST;

        const char *DoneChoices[1] = {"DONE"}; // end of psychic resistance check
        auto DoneControls = Graphics::CreateFixedTextButtons(DoneChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        DoneControls[0].Fg = clrWH;
        DoneControls[0].Highlight = intGR;
        DoneControls[0].Color = intBK;
        DoneControls[0].Type = Control::Type::BACK;

        SDL_Surface *dice[6];

        dice[0] = Assets::Copy(Assets::Type::Dice1);
        dice[1] = Assets::Copy(Assets::Type::Dice2);
        dice[2] = Assets::Copy(Assets::Type::Dice3);
        dice[3] = Assets::Copy(Assets::Type::Dice4);
        dice[4] = Assets::Copy(Assets::Type::Dice5);
        dice[5] = Assets::Copy(Assets::Type::Dice6);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<TextButton> &Controls = ResistControls;

        auto Done = false;

        auto CurrentStage = Attributes::Stage::START;

        Engine::Randomize();

        auto TestRolls = 2;

        std::vector<int> Rolls(TestRolls, 0);

        auto TestSum = 0;

        auto AttributeValue = 0;

        auto Equipment = Engine::Equipment(Party.Members[Character], Attribute, false);

        auto Weapons = Engine::Equipment(Party.Members[Character], Attribute, true);

        if (Attribute == Attributes::Type::FightingProwess)
        {
            AttributeValue = (Engine::FightingProwess(Party.Members[Character]) + (Equipment.size() > 0 ? Equipment[0].Score : 0) + (Weapons.size() > 0 ? Weapons[0].Score : 0));

            if (Weapons.empty() && !Engine::HasAbility(Party.Members[Character], Abilities::Type::UnarmedMartialArts))
            {
                AttributeValue -= 2;
            }
        }
        else if (Attribute == Attributes::Type::PsychicAbility)
        {
            AttributeValue = (Engine::PsychicAbility(Party.Members[Character]) + (Equipment.size() > 0 ? Equipment[0].Score : 0) - Party.Members[Character].Spells.size());
        }
        else if (Attribute == Attributes::Type::Awareness)
        {
            AttributeValue = (Engine::Awareness(Party.Members[Character]) + (Equipment.size() > 0 ? Equipment[0].Score : 0));
        }

        AttributeValue = std::max(0, AttributeValue);

        while (!Done)
        {
            // render current combat screen
            Interface::RenderChoiceScreen(Window, Renderer, Party, Story, Screen, ChoiceScreen, -1);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " TEST").c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY);

            Graphics::PutText(Renderer, Character::ClassName[Party.Members[Character].Class], Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 2 * RowHeight);

            Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + ": " + std::to_string(AttributeValue) + (Attribute == Attributes::Type::PsychicAbility && Party.Members[Character].Spells.size() > 0 ? (" (-" + std::to_string(Party.Members[Character].Spells.size())) : "")).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY + 3 * RowHeight);

            if (CurrentStage == Attributes::Stage::TEST && Result == Attributes::Result::NONE)
            {
                TestSum = 0;

                for (auto i = 0; i < TestRolls; i++)
                {
                    Rolls[i] = Engine::Roll(1, 0);

                    TestSum += Rolls[i];
                }

                TestSum = std::max(0, TestSum);

                auto TestResult = AttributeValue >= TestSum;

                if (TestResult)
                {
                    Result = Attributes::Result::SUCCESS;
                }
                else
                {
                    Result = Attributes::Result::FAILURE;
                }

                CurrentStage = Attributes::Stage::END;
            }
            else if (CurrentStage == Attributes::Stage::END)
            {
                // show casting results
                for (auto i = 0; i < TestRolls; i++)
                {
                    Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Screen.ObjectSize + 2 * text_space), TextY + 5 * RowHeight, Screen.ObjectSize, Screen.ObjectSize);
                }

                Graphics::PutText(Renderer, ("Test Score: " + std::to_string(TestSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + 6 * RowHeight + Screen.ObjectSize);

                if (Result == Attributes::Result::SUCCESS)
                {
                    Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " test passed!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + 7 * RowHeight + Screen.ObjectSize);
                }
                else
                {
                    Graphics::PutText(Renderer, (std::string(Attributes::Description[Attribute]) + " test failed!").c_str(), Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + 7 * RowHeight + Screen.ObjectSize);
                }
            }

            Graphics::RenderTextButtons(Renderer, Controls, FONT_BOOKMAN, Current, 24, TTF_STYLE_NORMAL);

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::TEST && !Hold)
                {
                    CurrentStage = Attributes::Stage::TEST;

                    Controls = DoneControls;
                }
                else if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    Done = true;
                }
            }
        }

        for (auto i = 0; i < 6; i++)
        {
            if (dice[i])
            {
                SDL_FreeSurface(dice[i]);

                dice[i] = NULL;
            }
        }

        return Result;
    }

    void ProcessSpell(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, std::vector<Button> &ChoiceScreen, Interface::ScreenDimensions &Screen, int Current, Spell::Type Spell)
    {
        std::string SpellString = std::string(Spell::Name[Spell]) + " was cast against " + (Engine::Count(Party) > 1 ? "the party!" : "you!");

        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, ChoiceScreen, Current, SpellString, intBK);

        if (Spell == Spell::Type::MistsOfDeath)
        {
            for (auto i = 0; i < Party.Members.size(); i++)
            {
                Character::Base &Character = Party.Members[i];

                if (Engine::IsAlive(Character))
                {
                    auto Damage = Engine::Roll(2, 0);

                    auto Result = Interface::Test(Window, Renderer, ChoiceScreen, Screen, Story, Party, i, Attributes::Type::PsychicAbility);

                    if (Result == Attributes::Result::FAILURE)
                    {
                        std::string DamageString = std::string(Character::ClassName[Character.Class]) + " dealt " + std::to_string(Damage) + " damage!";

                        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, ChoiceScreen, Current, DamageString, intBK);

                        Engine::Gain(Character, -Damage);
                    }
                    else
                    {
                        std::string ResistString = std::string(Character::ClassName[Character.Class]) + " resisted the " + std::string(Spell::Name[Spell]) + "!";

                        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, ChoiceScreen, Current, ResistString, intGR);
                    }

                    if (!Engine::IsAlive(Character))
                    {
                        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, ChoiceScreen, Current, std::string(Character::ClassName[Character.Class]) + " killed!", intBK);
                    }
                }
            }
        }
    }

    void TakeItems(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, std::string &Message)
    {
        auto Equipment = Choice.Equipment;

        for (auto i = 0; i < Equipment.size(); i++)
        {
            if (Equipment[i].Class != Equipment::Class::Gold && Equipment[i].Class != Equipment::Class::Arrow)
            {
                auto Character = Engine::First(Party);

                while (Character >= 0 && Character < Party.Members.size())
                {
                    std::string TakeMessage = "Give the " + Equipment[i].Name + " to";

                    Character = Engine::Count(Party) > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, TakeMessage.c_str()) : Engine::First(Party);

                    if (Character >= 0 && Character < Party.Members.size())
                    {
                        if ((Equipment[i].Weapon == Equipment::Weapon::Bow || Equipment[i].Class == Equipment::Class::Quiver) && Party.Members[Character].Class != Character::Class::Trickster && Party.Members[Character].Class != Character::Class::Sage)
                        {
                            Interface::RenderMessage(Window, Renderer, Party, Story, Screen, Controls, -1, (std::string(Character::ClassName[Party.Members[Character].Class]) + " cannot use the " + Equipment[i].Name), intBK);

                            if (Engine::Count(Party) == 1)
                            {
                                break;
                            }
                        }
                        else
                        {
                            Party.Members[Character].Equipment.push_back(Equipment[i]);

                            while (Party.Members[Character].Equipment.size() > Party.Members[Character].Encumbrance)
                            {
                                Interface::ItemScreen(Window, Renderer, Controls, Party, Story, Screen, Character, Equipment::Mode::DROP);
                            }

                            break;
                        }
                    }
                }
            }
            else if (Equipment[i].Class == Equipment::Class::Gold)
            {
                auto Gold = Equipment[i].Gold;

                while (Gold > 0)
                {
                    std::string TakeMessage = "Give the " + std::to_string(Gold) + (Gold != 1 ? " gold pieces" : " gold piece") + " to";

                    auto Character = Engine::Count(Party) > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, TakeMessage.c_str()) : Engine::First(Party);

                    if (Character >= 0 && Character < Party.Members.size())
                    {
                        auto Pouch = Engine::FirstPouch(Party.Members[Character]);

                        if (Pouch >= 0 && Pouch < Party.Members[Character].Equipment.size())
                        {
                            Gold = Engine::GainGold(Party.Members[Character], Gold);
                        }
                        else
                        {
                            Interface::RenderMessage(Window, Renderer, Party, Story, Screen, Controls, -1, "No space oeft for the gold!", intBK);

                            if (Engine::Count(Party) == 1)
                            {
                                Gold = 0;
                            }
                        }
                    }
                    else
                    {
                        Gold = 0;
                    }
                }
            }
            else if (Equipment[i].Class == Equipment::Class::Arrow)
            {
                auto Arrows = Equipment[i].Arrows;

                while (Arrows > 0)
                {
                    std::string TakeMessage = "Give the " + std::to_string(Arrows) + (Arrows != 1 ? " arrows" : " arrow") + " to";

                    auto Character = Engine::Count(Party) > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, TakeMessage.c_str()) : Engine::First(Party);

                    if (Character >= 0 && Character < Party.Members.size())
                    {
                        auto Quiver = Engine::FirstQuiver(Party.Members[Character]);

                        if (Quiver >= 0 && Quiver < Party.Members[Character].Equipment.size())
                        {
                            Arrows = Engine::GainArrows(Party.Members[Character], Arrows);
                        }
                        else
                        {
                            Interface::RenderMessage(Window, Renderer, Party, Story, Screen, Controls, -1, "No space left for the arrows!", intBK);

                            if (Engine::Count(Party) == 1)
                            {
                                Arrows = 0;
                            }
                        }
                    }
                    else
                    {
                        Arrows = 0;
                    }
                }
            }
        }
    }

    bool HasItem(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto Result = false;

        auto Item = Choice.Item;

        if (Engine::HasItem(Party, Item))
        {
            *Next = Interface::FindStory(Choice.Destination);

            Result = true;
        }
        else
        {
            if (Engine::Count(Party) > 1)
            {
                Message = "No one has the " + std::string(Equipment::ItemDescription[Item]) + "!";
            }
            else
            {
                auto First = Engine::First(Party, Item);

                Message = std::string(Character::ClassName[Party.Members[First].Class]) + " does not have the " + std::string(Equipment::ItemDescription[Item]) + "!";
            }
        }

        return Result;
    }

    bool CharacterHasItem(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto Result = false;

        auto Item = Choice.Item;

        auto Character = Choice.Character;

        if (Engine::IsPresent(Party, Character) && Engine::HasItem(Party, Item))
        {
            *Next = Interface::FindStory(Choice.Destination);

            Result = true;
        }
        else if (Engine::HasItem(Party, Item))
        {
            Message = std::string(Character::ClassName[Character]) + " not present in your party!";
        }
        else
        {
            if (Engine::Count(Party) > 1)
            {
                Message = "No one has the " + std::string(Equipment::ItemDescription[Item]) + "!";
            }
            else
            {
                auto First = Engine::First(Party);

                Message = std::string(Character::ClassName[Party.Members[First].Class]) + " does not have the " + std::string(Equipment::ItemDescription[Item]) + "!";
            }
        }

        return Result;
    }

    bool DropItem(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto Result = false;

        auto Item = Choice.Item;

        if (Engine::HasItem(Party, Item))
        {
            auto DropMessage = "Select whose " + std::string(Equipment::ItemDescription[Item]) + " to drop";

            auto Character = Engine::Count(Party, Item) > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, DropMessage.c_str()) : Engine::First(Party, Item);

            if (Character >= 0 && Character < Party.Members.size())
            {
                if (Engine::HasItem(Party.Members[Character], Item))
                {
                    Engine::Drop(Party.Members[Character], Item);

                    *Next = Interface::FindStory(Choice.Destination);

                    Result = true;
                }
                else
                {
                    Message = std::string(Character::ClassName[Party.Members[Character].Class]) + " does not have the " + std::string(Equipment::ItemDescription[Item]) + "!";
                }
            }
        }
        else
        {
            if (Engine::Count(Party) > 1)
            {
                Message = "No one has the " + std::string(Equipment::ItemDescription[Item]) + "!";
            }
            else
            {
                auto First = Engine::First(Party, Item);

                Message = std::string(Character::ClassName[Party.Members[First].Class]) + " does not have the " + std::string(Equipment::ItemDescription[Item]) + "!";
            }
        }

        return Result;
    }

    bool DropWeapon(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto Result = false;

        auto Weapon = Choice.Weapon;

        if (Engine::HasWeapon(Party, Weapon))
        {
            auto DropMessage = "Select whose " + std::string(Equipment::WeaponDescription[Weapon]) + " to drop";

            auto Character = Engine::Count(Party, Weapon) > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, DropMessage.c_str()) : Engine::First(Party, Weapon);

            if (Character >= 0 && Character < Party.Members.size())
            {
                if (Engine::HasWeapon(Party.Members[Character], Weapon))
                {
                    Engine::Drop(Party.Members[Character], Weapon);

                    *Next = Interface::FindStory(Choice.Destination);

                    Result = true;
                }
                else
                {
                    Message = std::string(Character::ClassName[Party.Members[Character].Class]) + " does not have the " + std::string(Equipment::WeaponDescription[Weapon]) + "!";
                }
            }
        }
        else
        {
            if (Engine::Count(Party) > 1)
            {
                Message = "No one has the " + std::string(Equipment::WeaponDescription[Weapon]) + "!";
            }
            else
            {
                auto First = Engine::First(Party, Weapon);

                Message = std::string(Character::ClassName[Party.Members[First].Class]) + " does not have the " + std::string(Equipment::WeaponDescription[Weapon]) + "!";
            }
        }

        return Result;
    }

    bool Discharge(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto Result = false;

        auto Item = Choice.Item;

        if (Engine::HasItem(Party, Item))
        {
            auto DischargeMessage = "Select whose " + std::string(Equipment::ItemDescription[Item]) + " to discharge";

            auto Character = Engine::Count(Party, Item) > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, DischargeMessage.c_str()) : Engine::First(Party, Item);

            if (Character >= 0 && Character < Party.Members.size())
            {
                if (Engine::HasItem(Party.Members[Character], Item))
                {
                    if (!Engine::Discharge(Party.Members[Character], Item, Choice.Charge))
                    {
                        Message = std::string(Equipment::ItemDescription[Item]) + " does not have enough charges!";
                    }
                    else
                    {
                        *Next = Interface::FindStory(Choice.Destination);

                        Result = true;
                    }
                }
                else
                {
                    Message = std::string(Character::ClassName[Party.Members[Character].Class]) + " does not have the " + std::string(Equipment::ItemDescription[Item]) + "!";
                }
            }
        }
        else
        {
            if (Engine::Count(Party) > 1)
            {
                Message = "No one has the " + std::string(Equipment::ItemDescription[Item]) + "!";
            }
            else
            {
                auto First = Engine::First(Party);

                Message = std::string(Character::ClassName[Party.Members[First].Class]) + " does not have the " + std::string(Equipment::ItemDescription[Item]) + "!";
            }
        }

        return Result;
    }

    bool TestCharacter(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto TestResult = false;

        if (Engine::IsPresent(Party, Choice.Character))
        {
            auto Character = Engine::Find(Party, Choice.Character);

            auto Result = Interface::Test(Window, Renderer, Controls, Screen, Story, Party, Character, Choice.Attribute);

            if (Result == Attributes::Result::SUCCESS)
            {
                *Next = Interface::FindStory(Choice.Destination);

                TestResult = true;
            }
            else
            {
                *Next = Interface::FindStory(Choice.DestinationFail);

                TestResult = true;
            }
        }
        else
        {
            Message = std::string(Character::ClassName[Choice.Character]) + " not present in your party!";
        }

        return TestResult;
    }

    void TestParty(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next)
    {
        auto TestResult = true;

        auto Consequence = Choice.Consequence;

        for (auto i = 0; i < Party.Members.size(); i++)
        {
            if (Engine::IsAlive(Party.Members[i]))
            {
                auto Result = Interface::Test(Window, Renderer, Controls, Screen, Story, Party, i, Choice.Attribute);

                if (Result == Attributes::Result::FAILURE)
                {
                    TestResult = false;

                    if (Consequence == Choice::Consequence::LoseItemOrEndurance)
                    {
                        auto ConsequenceChoice = Interface::Choose(Window, Renderer, Controls, Party, Story, Screen, {Assets::Type::Cancel, Assets::Type::Healing}, {"Item", "Endurance"}, "Lose one item or a point of endurance?");

                        if (ConsequenceChoice == 0)
                        {
                            auto Equipment = Party.Members[i].Equipment.size();

                            if (Equipment > 0)
                            {
                                while (Party.Members[i].Equipment.size() >= Equipment)
                                {
                                    Interface::ItemScreen(Window, Renderer, Controls, Party, Story, Screen, i, Equipment::Mode::DROP);
                                }
                            }
                            else
                            {
                                Engine::Gain(Party.Members[i], -1);
                            }
                        }
                        else if (ConsequenceChoice == 1)
                        {
                            Engine::Gain(Party.Members[i], -1);
                        }
                    }
                }
            }
        }

        if (TestResult && Engine::IsAlive(Party))
        {
            *Next = Interface::FindStory(Choice.Destination);
        }
        else if (!TestResult && Engine::IsAlive(Party))
        {
            *Next = Interface::FindStory(Choice.DestinationFail);
        }
    }

    void TestSelectedCharacter(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next)
    {
        auto Character = Party.LastSelected >= 0 && Party.LastSelected < Party.Members.size() && Engine::IsAlive(Party.Members[Party.LastSelected]) ? Party.LastSelected : Engine::First(Party);

        auto Result = Interface::Test(Window, Renderer, Controls, Screen, Story, Party, Character, Choice.Attribute);

        if (Result == Attributes::Result::SUCCESS)
        {
            *Next = Interface::FindStory(Choice.Destination);
        }
        else
        {
            *Next = Interface::FindStory(Choice.DestinationFail);
        }
    }

    bool HasAbility(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto Result = false;

        auto Ability = Choice.Ability;

        if (Engine::HasAbility(Party, Ability))
        {
            *Next = Interface::FindStory(Choice.Destination);

            Result = true;
        }
        else
        {
            if (Engine::Count(Party) > 1)
            {
                Message = "No one has the " + std::string(Abilities::Description[Ability]) + " ability!";
            }
            else
            {
                Message = "You do not have the " + std::string(Abilities::Description[Ability]) + " ability!";
            }
        }

        return Result;
    }

    bool LoseMoney(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto Result = false;

        auto Character = Engine::Count(Party) > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, Choice.SelectMessage.c_str()) : Engine::First(Party);

        if (Character >= 0 && Character < Party.Members.size())
        {
            if (Engine::CountMoney(Party.Members[Character]) >= Choice.Gold)
            {
                Party.LastSelected = Character;

                Engine::LoseMoney(Party.Members[Character], Choice.Gold);

                *Next = Interface::FindStory(Choice.Destination);

                Result = true;
            }
            else
            {
                Message = std::string(Character::ClassName[Party.Members[Character].Class]) + " does not have enough money!";
            }
        }

        return Result;
    }

    bool RandomDestination(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, Choice::Base &Choice, Story::Base **Next, std::string &Message)
    {
        auto Result = false;

        auto Character = Choice.Character;

        auto Ability = Choice.Ability;

        auto Destinations = Choice.Destinations;

        if (Character != Character::Class::None && !Engine::IsPresent(Party, Character))
        {
            Message = std::string(Character::ClassName[Character]) + " not present in your party!";
        }
        else if (Ability != Abilities::Type::None && !Engine::HasAbility(Party, Ability))
        {
            if (Engine::Count(Party) > 1)
            {
                Message = "No one has the " + std::string(Abilities::Description[Ability]) + " ability!";
            }
            else
            {
                Message = "You do not have the " + std::string(Abilities::Description[Ability]) + " ability!";
            }
        }
        else
        {
            if (Destinations.size() > 0)
            {
                auto BinSize = (6 / ((int)Destinations.size()));

                auto Roll = Engine::Roll(1, 0);

                for (auto i = 0; i < Destinations.size(); i++)
                {
                    if (Roll >= (i * BinSize + 1) && Roll <= ((i + 1) * BinSize))
                    {
                        *Next = Interface::FindStory(Destinations[i]);

                        Result = true;

                        break;
                    }
                }
            }
            else
            {
                Message = "Internal Error: Unable to turn to section";
            }
        }

        return Result;
    }

    Story::Base *ProcessChoices(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen)
    {
        Story::Base *Next = &Story::notImplemented;

        auto FlashMessage = false;

        auto FlashColor = intGR;

        std::string Message = "";

        Uint32 StartTicks = 0;

        Uint32 Duration = 3000;

        auto DisplayMessage = [&](std::string msg, Uint32 color)
        {
            FlashMessage = true;

            Message = msg;

            FlashColor = color;

            StartTicks = SDL_GetTicks();
        };

        auto RenderFlashMessage = [&]()
        {
            if (FlashMessage)
            {
                if ((SDL_GetTicks() - StartTicks) < Duration)
                {
                    auto FlashW = 3 * Screen.Width / 5;

                    auto FlashH = Screen.Height / 5;

                    Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, FlashH, Screen.X + (Screen.Width - FlashW) / 2, Screen.Y + Screen.TextBoxY + (Screen.TextBoxHeight - FlashH) / 2);

                    if (FlashColor == intBK)
                    {
                        Graphics::DrawRect(Renderer, FlashW, FlashH, Screen.X + (Screen.Width - FlashW) / 2, Screen.Y + Screen.TextBoxY + (Screen.TextBoxHeight - FlashH) / 2, intWH);
                    }
                }
                else
                {
                    FlashMessage = false;
                }
            }
        };

        if (Renderer && Story->Choices.size() > 0)
        {
            auto FontSize = TTF_FontHeight(Fonts::Normal);
            auto Selected = false;
            auto Current = -1;
            auto Done = false;
            auto ScrollUp = false;
            auto ScrollDown = false;
            auto Hold = false;
            auto ScrollSpeed = 1;

            auto Offset = 0;
            auto Limit = (Screen.TextBounds) / (3 * FontSize + 2 * text_space);
            auto Last = Offset + Limit;

            if (Last > Story->Choices.size())
            {
                Last = Story->Choices.size();
            }

            auto Fg = clrBK;

            auto Bg = intGR;

            auto Highlight = intBK;

            auto Controls = Interface::CreateChoices(Window, Renderer, Story->Choices, Screen, Offset, Last, Limit, Fg, Bg, Highlight);

            while (!Done)
            {
                Interface::RenderChoiceScreen(Window, Renderer, Party, Story, Screen, Controls, Current);

                if (Current >= 0 && Current < Controls.size())
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }

                RenderFlashMessage();

                Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

                if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
                {
                    if (Controls[Current].Type == Control::Type::SCROLL_UP || (Controls[Current].Type == Control::Type::SCROLL_UP && Hold) || ScrollUp)
                    {
                        if (Offset > 0)
                        {
                            Offset -= ScrollSpeed;

                            if (Offset < 0)
                            {
                                Offset = 0;
                            }

                            Last = Offset + Limit;

                            if (Last > Story->Choices.size())
                            {
                                Last = Story->Choices.size();
                            }

                            Controls = Interface::CreateChoices(Window, Renderer, Story->Choices, Screen, Offset, Last, Limit, Fg, Bg, Highlight);

                            SDL_Delay(50);
                        }

                        if (Offset <= 0)
                        {
                            Current = -1;

                            Selected = false;
                        }
                    }
                    else if (Controls[Current].Type == Control::Type::SCROLL_DOWN || (Controls[Current].Type == Control::Type::SCROLL_DOWN && Hold) || ScrollDown)
                    {
                        if (Story->Choices.size() - Last > 0)
                        {
                            if (Offset < Story->Choices.size() - Limit)
                            {
                                Offset += ScrollSpeed;
                            }

                            if (Offset > Story->Choices.size() - Limit)
                            {
                                Offset = Story->Choices.size() - Limit;
                            }

                            Last = Offset + Limit;

                            if (Last > Story->Choices.size())
                            {
                                Last = Story->Choices.size();
                            }

                            Controls = Interface::CreateChoices(Window, Renderer, Story->Choices, Screen, Offset, Last, Limit, Fg, Bg, Highlight);

                            SDL_Delay(50);

                            if (Offset > 0)
                            {
                                Current = Interface::FindControl(Controls, Control::Type::SCROLL_DOWN);
                            }
                        }

                        if (Story->Choices.size() - Last <= 0)
                        {
                            Selected = false;

                            Current = -1;
                        }
                    }
                    else if (Controls[Current].Type == Control::Type::CHOICE && !Hold)
                    {
                        auto Choice = Current + Offset;

                        if (Choice >= 0 && Choice < Story->Choices.size())
                        {
                            if (Story->Choices[Choice].Type == Choice::Type::Normal)
                            {
                                Next = Interface::FindStory(Story->Choices[Choice].Destination);

                                Done = true;
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::Character)
                            {
                                if (Engine::IsPresent(Party, Story->Choices[Choice].Character))
                                {
                                    Next = Interface::FindStory(Story->Choices[Choice].Destination);

                                    Done = true;
                                }
                                else
                                {
                                    DisplayMessage(std::string(Character::ClassName[Story->Choices[Choice].Character]) + " not present in your party!", intBK);
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::TestCharacter)
                            {
                                if (!Interface::TestCharacter(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::TestSelectedCharacter)
                            {
                                Interface::TestSelectedCharacter(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next);

                                Done = true;
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::HasAbility)
                            {
                                if (!Interface::HasAbility(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::Discharge)
                            {
                                if (!Interface::Discharge(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::DropWeapon)
                            {
                                if (!Interface::DropWeapon(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::DropItem)
                            {
                                if (!Interface::DropItem(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::SelectAdventurer)
                            {
                                auto Character = Engine::Count(Party) > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, Story->Choices[Choice].SelectMessage.c_str()) : Engine::First(Party);

                                if (Character >= 0 && Character < Party.Members.size())
                                {
                                    Party.LastSelected = Character;

                                    Next = Interface::FindStory(Story->Choices[Choice].Destination);

                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::LoseMoney)
                            {
                                if (!Interface::LoseMoney(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::SelectDice)
                            {
                                auto Value = Interface::Choose(Window, Renderer, Controls, Party, Story, Screen, {Assets::Type::Dice1, Assets::Type::Dice2, Assets::Type::Dice3, Assets::Type::Dice4, Assets::Type::Dice5, Assets::Type::Dice6}, {}, Story->Choices[Choice].SelectMessage.c_str());

                                if (Value >= 0 && Value < 6)
                                {
                                    Party.LastValue = Value;

                                    Next = Interface::FindStory(Story->Choices[Choice].Destination);

                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::CharacterHasItem)
                            {
                                if (!Interface::CharacterHasItem(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::HasItem)
                            {
                                if (!Interface::HasItem(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::EnemyCastSpell)
                            {
                                Interface::ProcessSpell(Window, Renderer, Party, Story, Controls, Screen, Current, Story->Choices[Choice].Spell);

                                if (Engine::IsAlive(Party))
                                {
                                    Next = Interface::FindStory(Story->Choices[Choice].Destination);
                                }
                                else
                                {
                                    Next = Story;
                                }

                                Done = true;
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::TakeEquipment)
                            {
                                Interface::TakeItems(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], Message);

                                Next = Interface::FindStory(Story->Choices[Choice].Destination);

                                Done = true;
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::TestParty)
                            {
                                Interface::TestParty(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next);

                                Done = true;
                            }
                            else if (Story->Choices[Choice].Type == Choice::Type::RandomDestination)
                            {
                                if (!Interface::RandomDestination(Window, Renderer, Party, Story, Screen, Controls, Story->Choices[Choice], &Next, Message))
                                {
                                    DisplayMessage(Message, intBK);
                                }
                                else
                                {
                                    Done = true;
                                }
                            }
                            else
                            {
                                DisplayMessage("Choice type not implemented yet!", intBK);
                            }
                        }
                    }
                    else if (Controls[Current].Type == Control::Type::BACK && !Hold)
                    {
                        Next = Story;

                        Done = true;
                    }
                }
            }
        }

        return Next;
    }

    Story::Base *RenderChoices(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen)
    {
        Story::Base *Next = &Story::notImplemented;

        if (Story->Choices.size() > 0)
        {
            Next = Interface::ProcessChoices(Window, Renderer, Party, Story, Screen);
        }
        else
        {
            Next = Interface::FindStory(Story->Continue(Party));
        }

        Next->DestinationReturn = {Story->Book, Story->Id};

        return Next;
    }
}
#endif
