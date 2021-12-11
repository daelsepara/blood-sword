#ifndef __INTERFACE_STORY_HPP__
#define __INTERFACE_STORY_HPP__

#include "interface_choice.hpp"
#include "interface_combat.hpp"

namespace Interface
{
    void RenderStoryScreen(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls, int Current, SDL_Surface *Text, int Offset)
    {
        Interface::RenderLeftPanel(Window, Renderer, Party, Story, Screen, Controls);

        if (!Story->Text.empty() && !Story->Image.empty() && Text)
        {
            Graphics::RenderImage(Renderer, Text, Screen.TextBoxX + text_space, Screen.TextBoxY + text_space, Screen.TextBounds, Offset);
        }
        else if (!Story->Text.empty() && Text)
        {
            Graphics::RenderText(Renderer, Text, 0, Screen.TextBoxX + text_space, Screen.TextBoxY + text_space, Screen.TextBounds, Offset);
        }

        Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);
    }

    void RenderMessage(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &StoryScreen, SDL_Surface *Text, int Offset, std::string Message, Uint32 FlashColor)
    {
        Uint32 Duration = 1500;

        Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

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

    int Roll(SDL_Window *Window, SDL_Renderer *Renderer, Story::Base *Story, Party::Base &Party, std::vector<Button> &StoryScreen, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, int DieRolls, int Modifier, std::string Message)
    {
        auto WindowW = 6 * Screen.IconSize;
        auto WindowH = 4 * Screen.IconSize;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto ColumnWidth = WindowW - 4 * text_space;
        auto RowHeight = TTF_FontHeight(Fonts::Normal);
        auto TextY = WindowY + 2 * text_space;

        auto TextButtonX = WindowX + 2 * text_space;
        auto TextButtonY = (WindowY + WindowH) - (text_buttonh + 2 * text_space);
        auto TextWidth = WindowW - 3 * text_space;

        const char *ResistChoices[2] = {"ROLL"};

        auto ResistControls = Graphics::CreateFixedTextButtons(ResistChoices, 1, text_buttonw, text_buttonh, text_space, TextButtonX, TextButtonY);
        ResistControls[0].Fg = clrWH;
        ResistControls[0].Highlight = intGR;
        ResistControls[0].Color = intBK;
        ResistControls[0].Type = Control::Type::ROLL;

        const char *DoneChoices[1] = {"DONE"};
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

        auto CurrentStage = Engine::RollStage::START;

        Engine::Randomize();

        std::vector<int> Rolls(DieRolls, 0);

        auto RollSum = -1;

        while (!Done)
        {
            // render current combat screen
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, Message.c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, ColumnWidth, RowHeight, TextButtonX, TextY);

            if (CurrentStage == Engine::RollStage::ROLL && RollSum == -1)
            {
                RollSum = 0;

                for (auto i = 0; i < DieRolls; i++)
                {
                    Rolls[i] = Engine::Roll(1, 0);

                    RollSum += Rolls[i];
                }

                RollSum = std::max(0, RollSum + Modifier);

                CurrentStage = Engine::RollStage::END;
            }
            else if (CurrentStage == Engine::RollStage::END)
            {
                for (auto i = 0; i < DieRolls; i++)
                {
                    Graphics::StretchImage(Renderer, dice[Rolls[i] - 1], TextButtonX + i * (Screen.ObjectSize + 2 * text_space), TextY + 2 * RowHeight, Screen.ObjectSize, Screen.ObjectSize);
                }

                Graphics::PutText(Renderer, ("Score (" + (Modifier >= 0 ? ("+" + std::to_string(Modifier)) : std::to_string(Modifier)) + "): " + std::to_string(RollSum)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, TextWidth, RowHeight, TextButtonX, TextY + 4 * RowHeight + Screen.ObjectSize);
            }

            Graphics::RenderTextButtons(Renderer, Controls, FONT_BOOKMAN, Current, 24, TTF_STYLE_NORMAL);

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::ROLL && !Hold)
                {
                    CurrentStage = Engine::RollStage::ROLL;

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

        return RollSum;
    }

    int Choose(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, std::vector<Assets::Type> Assets, std::vector<std::string> Captions, const char *Message)
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
            Controls.push_back(Button(NumControls, Assets::Get(Assets[NumControls]), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * (Screen.IconSize + 2 * text_space), WindowY + Screen.IconSize, intBK, Control::Type::CHOICE));

            NumControls++;
        }

        Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Back), NumControls > 0 ? NumControls - 1 : 0, NumControls, NumControls, NumControls, ButtonX + NumControls * (Screen.IconSize + 2 * text_space), WindowY + Screen.IconSize, intBK, Control::Type::BACK));

        auto Done = false;

        while (!Done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            Graphics::PutText(Renderer, Message, Fonts::Normal, text_space, clrGR, intWH, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), ButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                if (Controls[Current].Type != Control::Type::BACK)
                {
                    Interface::RenderChoiceCaption(Renderer, Controls[Current], Captions[Current], clrBK, intWH);
                }
                else
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrBK, intWH);
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

    Abilities::Type Abilities(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, int Adventurer, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset)
    {
        auto Result = Abilities::Type::None;

        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 9 * Screen.IconSize;
        auto WindowH = 3 * Screen.IconSize;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + 2 * text_space;
        auto OffsetY = (WindowY + WindowH) - (Screen.IconSize + FontSize + border_pts);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<Abilities::Type> Abilities = {};

        Character::Base &Character = Party.Members[Adventurer];

        auto NumControls = 0;

        std::vector<Button> Controls = {};

        if (Engine::HasAbility(Character, Abilities::Type::Ambidextrousness))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Ambidextrousness, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::AMBIDEXTROUSNESS));

            Abilities.push_back(Abilities::Type::Ambidextrousness);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::UnarmedMartialArts))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::UnarmedMartialArts, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::UNARMED_MARTIAL_ARTS));

            Abilities.push_back(Abilities::Type::UnarmedMartialArts);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Archery))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Archery, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::ARCHERY));

            Abilities.push_back(Abilities::Type::Archery);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Dodging))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Dodging, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::DODGING));

            Abilities.push_back(Abilities::Type::Dodging);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::QuickThinking))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::QuickThinking, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::QUICKTHINKING));

            Abilities.push_back(Abilities::Type::QuickThinking);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Quarterstaff))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Quarterstaff, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::QUARTERSTAFF));

            Abilities.push_back(Abilities::Type::Quarterstaff);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Healing))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Healing), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::HEAL));

            Abilities.push_back(Abilities::Type::Healing);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Exorcism))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Exorcism, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::EXORCISM));

            Abilities.push_back(Abilities::Type::Exorcism);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::ESP))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::ESP, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::ESP));

            Abilities.push_back(Abilities::Type::ESP);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::ParanormalSight))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::ParanormalSight, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::PARANORMAL_SIGHT));

            Abilities.push_back(Abilities::Type::ParanormalSight);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::Levitation))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Levitation, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::LEVITATION));

            Abilities.push_back(Abilities::Type::Levitation);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::CallToMind))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::CallToMind), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::CALL));

            Abilities.push_back(Abilities::Type::CallToMind);

            NumControls++;
        }

        if (Engine::HasAbility(Character, Abilities::Type::CastSpell))
        {
            Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::CastSpell, 0x66), NumControls > 0 ? NumControls - 1 : 0, NumControls + 1, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::CAST));

            Abilities.push_back(Abilities::Type::CastSpell);

            NumControls++;
        }

        Controls.push_back(Button(NumControls, Assets::Get(Assets::Type::Back), NumControls > 0 ? NumControls - 1 : 0, NumControls, NumControls, NumControls, ButtonX + NumControls * Screen.IconSize, OffsetY, intWH, Control::Type::BACK));

        auto Done = false;

        while (!Done)
        {
            // render current story screen
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, "Use Ability", Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, WindowW - 2 * text_space, TTF_FontHeight(Fonts::Normal), ButtonX - text_space, TextY);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    Done = true;
                }
                else if (Current >= 0 && Current < Abilities.size())
                {
                    Result = Abilities[Current];

                    Done = true;
                }
            }
        }

        return Result;
    }

    int SelectAdventurer(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, const char *SelectMessage, int Current)
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
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

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

    int SelectAdventurer(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, const char *SelectMessage)
    {
        return Interface::SelectAdventurer(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, SelectMessage, 0);
    }

    void DistributePoints(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, Attributes::Type Attribute, int Points)
    {
        if (Engine::Count(Party) > 1)
        {
            auto Character = 0;

            while (Points > 0)
            {
                std::string Message = std::to_string(Points) + " " + std::string(Attributes::Description[Attribute]) + " left to distribute";

                Character = Interface::SelectAdventurer(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, Message.c_str(), Character);

                if (Character >= 0 && Character < Party.Members.size())
                {
                    if (Engine::IsAlive(Party.Members[Character]))
                    {
                        Engine::Gain(Party.Members[Character], Attribute, 1);

                        Points--;
                    }
                    else
                    {
                        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, (std::string(Character::ClassName[Party.Members[Character].Class]) + " is dead."), intBK);
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            auto Character = Engine::First(Party);

            Engine::Gain(Party.Members[Character], Attribute, Points);
        }
    }

    void Heal(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, int Adventurer, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 6 * Screen.IconSize;
        auto WindowH = 3 * Screen.IconSize;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + 2 * text_space;
        auto OffsetY = (WindowY + WindowH) - (Screen.IconSize + FontSize + border_pts);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Done = false;
        auto Current = 0;

        auto Controls = std::vector<Button>();
        Controls.push_back(Button(0, Assets::Get(Assets::Type::Healing), 0, 1, 0, 0, ButtonX, OffsetY, intBK, Control::Type::HEAL));
        Controls.push_back(Button(1, Assets::Get(Assets::Type::HeartPlus), 0, 2, 1, 1, ButtonX + Screen.IconSize, OffsetY, intBK, Control::Type::ENDURANCE_ADD));
        Controls.push_back(Button(2, Assets::Get(Assets::Type::HeartMinus), 1, 3, 2, 2, ButtonX + 2 * Screen.IconSize, OffsetY, intBK, Control::Type::ENDURANCE_SUB));
        Controls.push_back(Button(3, Assets::Get(Assets::Type::Back), 2, 3, 3, 3, ButtonX + 3 * Screen.IconSize, OffsetY, intBK, Control::Type::BACK));

        Character::Base &Character = Party.Members[Adventurer];

        auto Limit = Engine::Endurance(Character) - 1;

        auto EndurancePoints = 1;

        while (!Done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            Graphics::PutText(Renderer, ("ENDURANCE: " + std::to_string(EndurancePoints)).c_str(), Fonts::Normal, text_space, clrBK, intWH, TTF_STYLE_NORMAL, WindowW - 4 * text_space, TTF_FontHeight(Fonts::Normal), ButtonX - text_space, TextY);

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
                    Done = true;
                }
                else if (Controls[Current].Type == Control::Type::HEAL && !Hold)
                {
                    auto Endurance = EndurancePoints * Interface::Roll(Window, Renderer, Story, Party, StoryScreen, Screen, Text, Offset, 1, -2, "Attempt Healing");

                    if (Endurance > 0)
                    {
                        Engine::Gain(Character, -EndurancePoints);

                        Interface::DistributePoints(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, Attributes::Type::Endurance, Endurance);
                    }
                    else
                    {
                        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, "The healing attempt was unsuccessful!", intBK);
                    }

                    Limit = Engine::Endurance(Character) - 1;

                    EndurancePoints = 1;
                }
                else if (Controls[Current].Type == Control::Type::ENDURANCE_ADD && !Hold)
                {
                    if (EndurancePoints < Limit)
                    {
                        EndurancePoints++;
                    }
                }
                else if (Controls[Current].Type == Control::Type::ENDURANCE_SUB && !Hold)
                {
                    if (EndurancePoints > 1)
                    {
                        EndurancePoints--;
                    }
                }
            }
        }
    }

    void ItemScreen(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, int Character, Equipment::Mode Mode)
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

            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

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

                        auto Result = Interface::Choose(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, {Assets::Type::Ok}, {Options.c_str()}, Message.c_str());

                        if (Result == 0)
                        {
                            auto New = std::vector<Equipment::Base>();

                            for (auto i = 0; i < Equipment.size(); i++)
                            {
                                if (!Engine::InList(Selection, i) || !Equipment[i].CanDrop)
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

    int CallToMind(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, std::vector<Button> &StoryScreen, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, Character::Base &Character, Control::Type Mode)
    {
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

        auto Result = -1;

        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 10 * Screen.IconSize;
        auto WindowH = 8 * Screen.IconSize;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.Y + (Screen.Height - WindowH) / 2;
        auto WindowButtonX = WindowX + 4 * text_space;
        auto WindowButtonY = WindowY + FontSize + 4 * text_space;
        auto WindowButtonGridX = Screen.IconSize + 2 * text_space;
        auto WindowButtonGridY = Screen.IconSize + 2 * text_space;

        auto RenderFlashMessage = [&]()
        {
            if (FlashMessage)
            {
                if ((SDL_GetTicks() - StartTicks) < Duration)
                {
                    auto FlashW = Screen.ObjectSize * 8;

                    auto FlashH = Screen.ObjectSize * 2;

                    Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, FlashH, WindowX + (WindowW - FlashW) / 2, WindowY + (WindowH - FlashH) / 2);

                    if (FlashColor == intBK)
                    {
                        Graphics::DrawRect(Renderer, FlashW, FlashH, WindowX + (WindowW - FlashW) / 2, WindowY + (WindowH - FlashH) / 2, intWH);
                    }
                }
                else
                {
                    FlashMessage = false;
                }
            }
        };

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        std::vector<Button> Controls = {};

        Controls.push_back(Button(0, Assets::Get(Assets::Type::VolcanoSpray, Engine::WasCalledToMind(Character, Spell::Type::VolcanoSpray) ? 0xFF : 0x66), 0, 1, 0, 7, WindowButtonX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(1, Assets::Get(Assets::Type::Nighthowl, Engine::WasCalledToMind(Character, Spell::Type::Nighthowl) ? 0xFF : 0x66), 0, 2, 1, 8, WindowButtonX + WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(2, Assets::Get(Assets::Type::WhiteFire, Engine::WasCalledToMind(Character, Spell::Type::WhiteFire) ? 0xFF : 0x66), 1, 3, 2, 9, WindowButtonX + 2 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(3, Assets::Get(Assets::Type::Swordthrust, Engine::WasCalledToMind(Character, Spell::Type::Swordthrust) ? 0xFF : 0x66), 2, 4, 3, 10, WindowButtonX + 3 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(4, Assets::Get(Assets::Type::EyeOfTheTiger, Engine::WasCalledToMind(Character, Spell::Type::EyeOfTheTiger) ? 0xFF : 0x66), 3, 5, 4, 11, WindowButtonX + 4 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(5, Assets::Get(Assets::Type::ImmediateDeliverance, Engine::WasCalledToMind(Character, Spell::Type::ImmediateDeliverance) ? 0xFF : 0x66), 4, 6, 5, 12, WindowButtonX + 5 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(6, Assets::Get(Assets::Type::MistsOfDeath, Engine::WasCalledToMind(Character, Spell::Type::MistsOfDeath) ? 0xFF : 0x66), 5, 6, 6, 6, WindowButtonX + 6 * WindowButtonGridX, WindowButtonY, intWH, Control::Type::CAST));
        Controls.push_back(Button(7, Assets::Get(Assets::Type::TheVampireSpell, Engine::WasCalledToMind(Character, Spell::Type::TheVampireSpell) ? 0xFF : 0x66), 7, 8, 0, 7, WindowButtonX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(8, Assets::Get(Assets::Type::SheetLightning, Engine::WasCalledToMind(Character, Spell::Type::SheetLightning) ? 0xFF : 0x66), 7, 9, 1, 8, WindowButtonX + WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(9, Assets::Get(Assets::Type::GhastlyTouch, Engine::WasCalledToMind(Character, Spell::Type::GhastlyTouch) ? 0xFF : 0x66), 8, 10, 2, 9, WindowButtonX + 2 * WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(10, Assets::Get(Assets::Type::NemesisBolt, Engine::WasCalledToMind(Character, Spell::Type::NemesisBolt) ? 0xFF : 0x66), 9, 11, 3, 10, WindowButtonX + 3 * WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(11, Assets::Get(Assets::Type::ServileEnthralment, Engine::WasCalledToMind(Character, Spell::Type::ServileEnthralment) ? 0xFF : 0x66), 10, 12, 4, 11, WindowButtonX + 4 * WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::CAST));
        Controls.push_back(Button(12, Assets::Get(Assets::Type::Back), 11, 12, 5, 12, WindowButtonX + 5 * WindowButtonGridX, WindowButtonY + WindowButtonGridY, intWH, Control::Type::BACK));

        auto Done = false;

        while (!Done)
        {
            // render current combat screen
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::PutText(Renderer, (std::string("Select a spell to ") + std::string(Mode == Control::Type::CALL ? "call to mind" : "forget")).c_str(), Fonts::Normal, text_space, clrWH, intBK, TTF_STYLE_NORMAL, WindowW - 5 * text_space, TTF_FontHeight(Fonts::Normal), WindowButtonX - text_space, WindowY + text_space);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, border_pts);

            if (Current >= 0 && Current < Controls.size())
            {
                // render spell details
                if (Current == Spell::All.size())
                {
                    Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                }
                else
                {
                    Spell::Base &Spell = Spell::All[Current];

                    Graphics::PutText(Renderer, ("(" + std::string(Spell::ClassDescription[Spell.Class]) + ") " + Spell.Name + " - Complexity Level: " + std::to_string(Spell.Complexity) + "\n\n" + Spell.Description).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, WindowW - 5 * text_space, WindowH - (WindowButtonY + 2 * WindowButtonGridY), WindowButtonX - text_space, WindowButtonY + 2 * WindowButtonGridY);
                }
            }

            RenderFlashMessage();

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK)
                {
                    Done = true;
                }
                else if (Controls[Current].Type == Control::Type::CAST)
                {
                    if (Mode == Control::Type::CALL)
                    {
                        if (Engine::WasCalledToMind(Character, Spell::All[Current].Type))
                        {
                            DisplayMessage((Spell::All[Current].Name + " already called to mind!"), intBK);
                        }
                        else
                        {
                            Result = Current;

                            Done = true;
                        }
                    }
                    else if (Mode == Control::Type::FORGET)
                    {
                        if (!Engine::WasCalledToMind(Character, Spell::All[Current].Type))
                        {
                            DisplayMessage((Spell::All[Current].Name + " not called to mind!"), intBK);
                        }
                        else
                        {
                            Result = Current;

                            Done = true;
                        }
                    }
                }
            }
        }

        return Result;
    }

    void TakeScreen(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset)
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

        std::vector<Equipment::Base> &Equipment = Story->Equipment;

        if (Last > Equipment.size())
        {
            Last = Equipment.size();
        }

        auto Fg = clrGR;
        auto Bg = intWH;
        auto Highlight = intBK;

        auto Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Equipment::Mode::TAKE);

        auto Done = false;

        auto Selection = std::vector<int>();

        std::string TakeLimit = "";

        if (Story->Limit > 0 && Limit < Story->Equipment.size())
        {
            if (Story->Limit == 1)
            {
                TakeLimit = "Select which item to take";
            }
            else
            {
                TakeLimit = "You may take up to " + std::to_string(Story->Limit) + " items";
            }
        }
        else
        {
            TakeLimit = "Select items to take";
        }

        while (!Done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            Graphics::PutText(Renderer, TakeLimit.c_str(), Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);

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
                    Done = true;
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

                        Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Equipment::Mode::TAKE);

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

                        Controls = Interface::EquipmentList(Window, Renderer, Equipment, WindowW, WindowH, ButtonX, TextY + FontSize, ItemOffset, Last, Limit, Fg, Bg, Highlight, Equipment::Mode::TAKE);

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
                        if ((Story->Limit > 0 && Selection.size() <= Story->Limit) || Story->Limit == 0)
                        {
                            Selection.push_back(Current + ItemOffset);
                        }
                    }
                    else
                    {
                        Engine::Erase(Selection, Current + ItemOffset);
                    }
                }
                else if (Controls[Current].Type == Control::Type::TAKE && !Hold)
                {
                    if (Equipment.size() > 0 && Selection.size() > 0)
                    {
                        for (auto i = 0; i < Selection.size(); i++)
                        {
                            if (Equipment[Selection[i]].Class != Equipment::Class::Gold && Equipment[Selection[i]].Class != Equipment::Class::Arrow)
                            {
                                auto Character = Engine::First(Party);

                                while (Character >= 0 && Character < Party.Members.size())
                                {
                                    std::string TakeMessage = "Give the " + Equipment[Selection[i]].Name + " to";

                                    Character = Engine::Count(Party) > 1 ? Interface::SelectAdventurer(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, TakeMessage.c_str()) : Engine::First(Party);

                                    if (Character >= 0 && Character < Party.Members.size())
                                    {
                                        if (Engine::IsAlive(Party.Members[Character]))
                                        {
                                            if ((Equipment[Selection[i]].Weapon == Equipment::Weapon::Bow || Equipment[Selection[i]].Class == Equipment::Class::Quiver) && Party.Members[Character].Class != Character::Class::Trickster && Party.Members[Character].Class != Character::Class::Sage)
                                            {
                                                Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, (std::string(Character::ClassName[Party.Members[Character].Class]) + " cannot use the " + Equipment[Selection[i]].Name), intBK);

                                                if (Engine::Count(Party) == 1)
                                                {
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                Party.Members[Character].Equipment.push_back(Equipment[Selection[i]]);

                                                while (Party.Members[Character].Equipment.size() > Party.Members[Character].Encumbrance)
                                                {
                                                    Interface::ItemScreen(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, Character, Equipment::Mode::DROP);
                                                }

                                                break;
                                            }
                                        }
                                        else
                                        {
                                            Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, (std::string(Character::ClassName[Party.Members[Character].Class]) + " is dead."), intBK);
                                        }
                                    }
                                }
                            }
                            else if (Equipment[Selection[i]].Class == Equipment::Class::Gold)
                            {
                                auto Gold = Equipment[Selection[i]].Gold;

                                while (Gold > 0)
                                {
                                    std::string TakeMessage = "Give the " + std::to_string(Gold) + (Gold != 1 ? " gold pieces" : " gold piece") + " to";

                                    auto Character = Engine::Count(Party) > 1 ? Interface::SelectAdventurer(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, TakeMessage.c_str()) : Engine::First(Party);

                                    if (Character >= 0 && Character < Party.Members.size())
                                    {
                                        if (Engine::IsAlive(Party.Members[Character]))
                                        {
                                            auto Pouch = Engine::FirstPouch(Party.Members[Character]);

                                            if (Pouch >= 0 && Pouch < Party.Members[Character].Equipment.size())
                                            {
                                                Gold = Engine::GainGold(Party.Members[Character], Gold);
                                            }
                                            else
                                            {
                                                Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, "No space oeft for the gold!", intBK);

                                                if (Engine::Count(Party) == 1)
                                                {
                                                    Gold = 0;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, (std::string(Character::ClassName[Party.Members[Character].Class]) + " is dead."), intBK);
                                        }
                                    }
                                    else
                                    {
                                        Gold = 0;
                                    }
                                }
                            }
                            else if (Equipment[Selection[i]].Class == Equipment::Class::Arrow)
                            {
                                auto Arrows = Equipment[Selection[i]].Arrows;

                                while (Arrows > 0)
                                {
                                    std::string TakeMessage = "Give the " + std::to_string(Arrows) + (Arrows != 1 ? " arrows" : " arrow") + " to";

                                    auto Character = Engine::Count(Party) > 1 ? Interface::SelectAdventurer(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, TakeMessage.c_str()) : Engine::First(Party);

                                    if (Character >= 0 && Character < Party.Members.size())
                                    {
                                        if (Engine::IsAlive(Party.Members[Character]))
                                        {
                                            auto Quiver = Engine::FirstQuiver(Party.Members[Character]);

                                            if (Quiver >= 0 && Quiver < Party.Members[Character].Equipment.size())
                                            {
                                                Arrows = Engine::GainArrows(Party.Members[Character], Arrows);
                                            }
                                            else
                                            {
                                                Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, "No space left for the arrows!", intBK);

                                                if (Engine::Count(Party) == 1)
                                                {
                                                    Arrows = 0;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, (std::string(Character::ClassName[Party.Members[Character].Class]) + " is dead."), intBK);
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

                    Equipment.clear();

                    Selection.clear();

                    Done = true;
                }
            }
        }
    }

    void ManageAdventurer(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, int Character)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * Screen.Width / 5;
        auto WindowH = 12 * FontSize + Screen.IconSize;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto WindowTextWidth = WindowW - 4 * text_space;
        auto ColumnWidth = WindowTextWidth / 2;
        auto MidColumn = WindowX + ColumnWidth + 2 * text_space;
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + 2 * text_space;
        auto OffsetY = (WindowY + WindowH) - (Screen.IconSize + FontSize + border_pts);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Current = 0;

        auto Controls = std::vector<Button>();

        std::vector<Equipment::Base> &Equipment = Party.Members[Character].Equipment;
        std::vector<Abilities::Type> &Abilities = Party.Members[Character].Abilities;

        Controls.push_back(Button(0, Assets::Get(Assets::Type::Encyclopedia), 0, 1, 0, 0, ButtonX, OffsetY, intBK, Control::Type::INFO));
        Controls.push_back(Button(1, Assets::Get(Assets::Type::Stats), 0, 2, 1, 1, ButtonX + Screen.IconSize, OffsetY, intBK, Control::Type::STATS));
        Controls.push_back(Button(2, Assets::Get(Assets::Type::Ability), 1, 3, 2, 2, ButtonX + 2 * Screen.IconSize, OffsetY, intBK, Control::Type::ABILITY));
        Controls.push_back(Button(3, Assets::Get(Assets::Type::Items), 2, 4, 3, 3, ButtonX + 3 * Screen.IconSize, OffsetY, intBK, Control::Type::ITEMS));
        Controls.push_back(Button(4, Assets::Get(Assets::Type::Back), 3, 4, 3, 3, ButtonX + 4 * Screen.IconSize, OffsetY, intBK, Control::Type::BACK));

        auto Done = false;

        while (!Done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intWH);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            Graphics::PutText(Renderer, Character::ClassName[Party.Members[Character].Class], Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);

            if (Current >= 0 && Current < Controls.size())
            {
                if (Controls[Current].Type == Control::Type::STATS)
                {
                    Interface::CharacterSheet(Renderer, Party.Members[Character], Fonts::Normal, WindowTextWidth, ButtonX, TextY, intWH, true);
                }
                else if (Controls[Current].Type == Control::Type::ITEMS && Equipment.size() > 0)
                {
                    for (auto i = 0; i < Equipment.size(); i++)
                    {
                        Graphics::PutText(Renderer, Equipment[i].String().c_str(), Fonts::Normal, 0, clrGR, intWH, TTF_STYLE_NORMAL, ColumnWidth, FontSize, i < 5 ? ButtonX : MidColumn, TextY + 2 * FontSize + (i < 5 ? i : i - 5) * FontSize);
                    }
                }
                else
                {
                    Graphics::PutText(Renderer, Character::Description[Party.Members[Character].Class], Fonts::Normal, 0, clrGR, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY + 2 * FontSize);
                }
            }
            else
            {
                Graphics::PutText(Renderer, Character::Description[Party.Members[Character].Class], Fonts::Normal, 0, clrGR, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY + 2 * FontSize);
            }

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, 4);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrBK, intWH);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::BACK && !Hold)
                {
                    Done = true;
                }
                else if (Controls[Current].Type == Control::Type::ITEMS && !Hold)
                {
                    if (Engine::IsAlive(Party.Members[Character]) && !Equipment.empty())
                    {
                        Interface::ItemScreen(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, Character, Equipment::Mode::USE);
                    }

                    Current = -1;
                }
                else if (Controls[Current].Type == Control::Type::ABILITY && !Hold)
                {
                    if (!Abilities.empty() && Engine::IsAlive(Party.Members[Character]))
                    {
                        while (true)
                        {
                            auto Result = Interface::Abilities(Window, Renderer, StoryScreen, Party, Character, Story, Screen, Text, Offset);

                            if (Result == Abilities::Type::Healing)
                            {
                                if (Engine::Endurance(Party.Members[Character]) > 1)
                                {
                                    Interface::Heal(Window, Renderer, StoryScreen, Party, Character, Story, Screen, Text, Offset);
                                }
                                else
                                {
                                    Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, "You do not not have enough ENDURANCE!", intBK);
                                }
                            }
                            else if (Result == Abilities::Type::Ambidextrousness || Result == Abilities::Type::UnarmedMartialArts || Result == Abilities::Type::Dodging)
                            {
                                Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, "This ability is always in effect!", intGR);
                            }
                            else if (Result == Abilities::Type::QuickThinking || Result == Abilities::Type::Archery || Result == Abilities::Type::Quarterstaff)
                            {
                                Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, "This ability can only be used during combat", intGR);
                            }
                            else if (Result == Abilities::Type::Exorcism || Result == Abilities::Type::ESP || Result == Abilities::Type::ParanormalSight || Result == Abilities::Type::Levitation || Result == Abilities::Type::CastSpell)
                            {
                                Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, "This ability can only be used in certain situations", intBK);
                            }
                            else if (Result == Abilities::Type::CallToMind)
                            {
                                auto CalledToMind = Interface::CallToMind(Window, Renderer, Party, Story, StoryScreen, Screen, Text, Offset, Party.Members[Character], Control::Type::CALL);

                                if (CalledToMind >= 0 && CalledToMind < Spell::All.size())
                                {
                                    Party.Members[Character].Spells.push_back(Spell::All[CalledToMind]);

                                    auto Forget = -1;

                                    while (Party.Members[Character].Spells.size() > Party.Members[Character].SpellLimit)
                                    {
                                        Forget = Interface::CallToMind(Window, Renderer, Party, Story, StoryScreen, Screen, Text, Offset, Party.Members[Character], Control::Type::FORGET);

                                        if (Forget >= 0 && Forget < Spell::All.size())
                                        {
                                            auto Result = Engine::Find(Party.Members[Character], Spell::All[Forget].Type);

                                            if (Result >= 0 && Result < Party.Members[Character].Spells.size())
                                            {
                                                Party.Members[Character].Spells.erase(Party.Members[Character].Spells.begin() + Result);
                                            }
                                        }
                                    }

                                    if (Forget != CalledToMind)
                                    {
                                        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, Spell::All[CalledToMind].Name + " called to mind!", intGR);
                                    }
                                }
                            }
                            else if (Result == Abilities::Type::None)
                            {
                                break;
                            }
                        }
                    }
                    else if (!Engine::IsAlive(Party.Members[Character]))
                    {
                        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, (std::string(Character::ClassName[Party.Members[Character].Class]) + " is dead."), intBK);
                    }
                    else
                    {
                        Interface::RenderMessage(Window, Renderer, Party, Story, Screen, StoryScreen, Text, Offset, "You cannot use abilities at this time!", intBK);
                    }

                    Current = -1;
                }
            }
        }
    }

    void Bye(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, std::string Bye)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * Screen.Width / 5;
        auto WindowH = 8 * FontSize + Screen.IconSize;
        auto WindowX = Screen.X + (Screen.Width - WindowW) / 2;
        auto WindowY = Screen.TextBoxY + (Screen.TextBoxHeight - WindowH) / 2;
        auto WindowTextWidth = WindowW - 4 * text_space;
        auto TextY = WindowY + 2 * text_space;
        auto ButtonX = WindowX + 2 * text_space;
        auto OffsetY = (WindowY + WindowH) - (Screen.IconSize + FontSize + border_pts);

        auto Hold = false;
        auto Selected = false;
        auto ScrollUp = false;
        auto ScrollDown = false;
        auto Done = false;
        auto Current = 0;

        std::vector<Button> Controls = {Button(0, Assets::Get(Assets::Type::Right), 0, 0, 0, 0, ButtonX, OffsetY, intWH, Control::Type::CONTINUE)};

        while (!Done)
        {
            Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, StoryScreen, -1, Text, Offset);

            Graphics::FillRect(Renderer, WindowW, WindowH, WindowX, WindowY, intGR);

            Graphics::DrawRect(Renderer, WindowW, WindowH, WindowX, WindowY, intBK);

            Graphics::PutText(Renderer, Bye.c_str(), Fonts::Normal, 0, clrWH, intGR, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);

            Graphics::RenderButtons(Renderer, Controls, Current, text_space, 4);

            if (Current >= 0 && Current < Controls.size())
            {
                Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intGR);
            }

            Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold, 50);

            if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
            {
                if (Controls[Current].Type == Control::Type::CONTINUE && !Hold)
                {
                    Done = true;
                }
            }
        }
    }

    void ProcessStory(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story)
    {
        Interface::ScreenDimensions Screen;

        if (Window && Renderer)
        {
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

            auto ShowMessage = [&](std::string DisplayMessage, int Color)
            {
                auto FlashW = 3 * Screen.Width / 5;

                auto FlashH = Screen.Height / 5;

                Graphics::PutTextBox(Renderer, DisplayMessage.c_str(), Fonts::Normal, -1, clrWH, Color, TTF_STYLE_NORMAL, FlashW, FlashH, (Screen.Width - FlashW) / 2, Screen.TextBoxY + (Screen.TextBoxHeight - FlashH) / 2);

                if (Color == intBK)
                {
                    Graphics::DrawRect(Renderer, FlashW, FlashH, (Screen.Width - FlashW) / 2, Screen.TextBoxY + (Screen.TextBoxHeight - FlashH) / 2, intWH);
                }
            };

            auto RenderFlashMessage = [&]()
            {
                if (FlashMessage)
                {
                    if ((SDL_GetTicks() - StartTicks) < Duration)
                    {
                        ShowMessage(Message, FlashColor);
                    }
                    else
                    {
                        FlashMessage = false;
                    }
                }
            };

            auto CheckPartyStatus = [&]()
            {
                if (Engine::Paralyzed(Party))
                {
                    if (Party.Members.size() > 1)
                    {
                        DisplayMessage("The entire party has been paralyzed. The adventure end here.", intBK);
                    }
                    else
                    {
                        DisplayMessage("You were paralyzed. The adventure end here.", intBK);
                    }
                }
                else if (!Engine::IsAlive(Party))
                {
                    if (Party.Members.size() > 1)
                    {
                        DisplayMessage("The entire party is dead. The adventure end here.", intBK);
                    }
                    else
                    {
                        DisplayMessage("You have died. The adventure end here.", intBK);
                    }
                }
            };

            auto FontSize = TTF_FontHeight(Fonts::Normal);

            std::vector<Button> Controls = {};

            auto Quit = false;

            Screen.X = 0;
            Screen.Y = 0;
            Screen.Width = SCREEN_WIDTH;
            Screen.Height = SCREEN_HEIGHT;
            Screen.IconSize = (buttonw + 2 * text_space);
            Screen.BoxWidth = (Screen.Width - 2 * Screen.IconSize) / 2 + text_space;
            Screen.BoxHeight = (Screen.Height - 3 * (buttonh + 2 * text_space));
            Screen.TextBoxWidth = Screen.BoxWidth;
            Screen.TextBoxHeight = Screen.BoxHeight;
            Screen.TextBoxX = Screen.X + Screen.IconSize / 2 + Screen.BoxWidth - text_space;
            Screen.TextBoxY = Screen.Y + Screen.IconSize;
            Screen.TextBounds = Screen.TextBoxHeight - 2 * text_space;
            Screen.TextWidth = Screen.TextBoxWidth - 2 * text_space;
            Screen.InfoBoxWidth = Screen.TextBoxWidth - 2 * text_space;
            Screen.InfoBoxHeight = Screen.TextBoxHeight;
            Screen.InfoWidth = Screen.InfoBoxWidth - 2 * text_space;
            Screen.InfoBoxX = Screen.X + Screen.IconSize / 2;
            Screen.InfoBoxY = Screen.Y + Screen.IconSize;
            Screen.ObjectSize = buttonw;

            while (!Quit)
            {
                Party.Book = Story->Book;

                Party.Story = Story->Id;

                // make a backup of current party
                auto SaveParty = Party;

                SDL_Surface *Text = NULL;

                auto RunOnce = true;

                if (RunOnce)
                {
                    RunOnce = false;

                    auto Jump = Story->Background(Party);

                    if (Jump.first != Book::Type::None)
                    {
                        Story = Interface::FindStory(Jump);

                        continue;
                    }

                    Story->Event(Party);
                }

                // generate story text/image surface
                if (!Story->Image.empty() && !Story->Text.empty())
                {
                    Text = Graphics::CreateTextAndImage(Story->Text.c_str(), Story->Image.c_str(), FONT_BOOKMAN, FontSize, clrBK, intGR, Screen.TextWidth, TTF_STYLE_NORMAL, Story->TopImage);
                }
                else if (!Story->Text.empty())
                {
                    Text = Glyphs::FormattedText(Story->Text.c_str(), FONT_BOOKMAN, FontSize, clrBK, Screen.TextWidth);
                }

                auto Compact = (Text && Text->h <= Screen.TextBounds - 2 * text_space) || !Text;

                if (!Engine::IsAlive(Party))
                {
                    Controls = Story::ExitControls(Compact);
                }
                else if (Story->Type == Story::Type::Doom)
                {
                    Controls = Story::ExitControls(Compact);
                }
                else if (Story->Controls == Story::Controls::Standard)
                {
                    Controls = Story::StandardControls(Compact);
                }
                else if (Story->Controls == Story::Controls::Info)
                {
                    Controls = Story::InfoControls(Compact);
                }
                else
                {
                    Controls = Story::ExitControls(Compact);
                }

                auto ScrollSpeed = 20;
                auto Hold = false;
                auto Selected = false;
                auto Current = -1;
                auto Offset = 0;
                auto Transition = false;
                auto ScrollUp = false;
                auto ScrollDown = false;

                CheckPartyStatus();

                while (!Transition)
                {
                    Interface::RenderStoryScreen(Window, Renderer, Party, Story, Screen, Controls, Current, Text, Offset);

                    RenderFlashMessage();

                    if (Current >= 0 && Current < Controls.size())
                    {
                        Graphics::RenderCaption(Renderer, Controls[Current], clrWH, intBK);
                    }

                    if ((Story->Text.empty() || Text == NULL) && Engine::IsAlive(Party) && !Engine::Paralyzed(Party))
                    {
                        SDL_RenderPresent(Renderer);

                        Current = Interface::FindControl(Controls, Control::Type::CONTINUE);

                        Selected = true;
                    }
                    else
                    {
                        Input::GetInput(Renderer, Controls, Current, Selected, ScrollUp, ScrollDown, Hold);
                    }

                    if ((Selected && Current >= 0 && Current < Controls.size()) || ScrollUp || ScrollDown || Hold)
                    {
                        if (Controls[Current].Type == Control::Type::SCROLL_UP || (Controls[Current].Type == Control::Type::SCROLL_UP && Hold) || ScrollUp)
                        {
                            if (Text)
                            {
                                if (Offset > 0)
                                {
                                    Offset -= ScrollSpeed;
                                }

                                if (Offset < 0)
                                {
                                    Offset = 0;
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::SCROLL_DOWN || (Controls[Current].Type == Control::Type::SCROLL_DOWN && Hold) || ScrollDown)
                        {
                            if (Text)
                            {
                                if (Text->h >= (Screen.TextBounds))
                                {
                                    if (Offset < (Text->h - Screen.TextBounds))
                                    {
                                        Offset += ScrollSpeed;
                                    }

                                    if (Offset > (Text->h - Screen.TextBounds))
                                    {
                                        Offset = Text->h - Screen.TextBounds;
                                    }
                                }
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::ENCYCLOPEDIA && !Hold)
                        {
                            DisplayMessage("Not yet implemented!", intBK);

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::MAP && !Hold)
                        {
                            DisplayMessage("Not yet implemented!", intBK);

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::PARTY && !Hold)
                        {
                            auto Character = Party.Members.size() > 1 ? Interface::SelectAdventurer(Window, Renderer, Controls, Party, Story, Screen, Text, Offset, "Select adventurer to manage") : 0;

                            if (Character >= 0 && Character < Party.Members.size())
                            {
                                Interface::ManageAdventurer(Window, Renderer, Controls, Party, Story, Screen, Text, Offset, Character);
                            }

                            Current = -1;

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::GAME && !Hold)
                        {
                            DisplayMessage("Not yet implemented!", intBK);

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::CONTINUE && !Hold)
                        {
                            if (!Story->MapFile.empty() && Story->Enemies.size() > 0)
                            {
                                auto Map = Map::Base();

                                auto Load = Map.Load(Story->MapFile.c_str());

                                if (Load)
                                {
                                    Story->SetupCombat(Map, Party);

                                    auto Result = Interface::CombatScreen(Window, Renderer, Story, Map, Party);

                                    if (Result == Combat::Result::NONE)
                                    {
                                        Transition = true;

                                        Quit = true;

                                        continue;
                                    }
                                    else
                                    {
                                        Story->AfterCombat(Party, Result);
                                    }
                                }
                            }

                            if (Engine::Remaining(Party) > 0)
                            {
                                if (Story->Equipment.size() > 0)
                                {
                                    Interface::TakeScreen(Window, Renderer, Controls, Party, Story, Screen, Text, Offset);
                                }

                                if (Story->Equipment.size() > 0)
                                {
                                    continue;
                                }

                                // handle encumbrance
                                for (auto i = 0; i < Party.Members.size(); i++)
                                {
                                    while (Party.Members[i].Equipment.size() > Party.Members[i].Encumbrance)
                                    {
                                        Interface::ItemScreen(Window, Renderer, Controls, Party, Story, Screen, Text, Offset, i, Equipment::Mode::DROP);
                                    }
                                }

                                auto Next = Interface::RenderChoices(Window, Renderer, Party, Story, Screen);

                                if (Next->Id != Story->Id || Story->Book != Next->Book)
                                {
                                    if (!Story->Bye.empty())
                                    {
                                        Interface::Bye(Window, Renderer, Controls, Party, Story, Screen, Text, Offset, Story->Bye);
                                    }

                                    Story = Next;

                                    Transition = true;
                                }
                            }
                            else
                            {
                                CheckPartyStatus();

                                Controls = Story::ExitControls(Compact);
                            }

                            Selected = false;
                        }
                        else if (Controls[Current].Type == Control::Type::BACK && !Hold)
                        {
                            auto Next = Interface::FindStory(Story->DestinationReturn);

                            if (Next->Id != Story->Id || Story->Book != Next->Book)
                            {
                                Story = Next;

                                Transition = true;
                            }
                        }
                        else if (Controls[Current].Type == Control::Type::EXIT && !Hold)
                        {
                            Transition = true;

                            Quit = true;
                        }
                    }
                }

                if (Text)
                {
                    SDL_FreeSurface(Text);

                    Text = NULL;
                }
            }
        }
    }

    void StoryScreen(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Book::Destination Destination)
    {
        auto Story = Interface::FindStory(Destination);

        Interface::ProcessStory(Window, Renderer, Party, Story);
    }
}
#endif
