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

        auto FlashW = 3 * SCREEN_WIDTH / 5;

        auto FlashH = SCREEN_HEIGHT / 5;

        Graphics::PutTextBox(Renderer, Message.c_str(), Fonts::Normal, -1, clrWH, FlashColor, TTF_STYLE_NORMAL, FlashW, FlashH, (SCREEN_WIDTH - FlashW) / 2, (SCREEN_HEIGHT - FlashH) / 2);

        if (FlashColor == intBK)
        {
            Graphics::DrawRect(Renderer, FlashW, FlashH, (SCREEN_WIDTH - FlashW) / 2, (SCREEN_HEIGHT - FlashH) / 2, intWH);
        }

        SDL_RenderPresent(Renderer);

        SDL_Delay(Duration);
    }

    int Choose(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, std::vector<Assets::Type> Assets, std::vector<std::string> Captions, const char *Message)
    {
        auto Result = -1;

        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 4 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
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

    int SelectAdventurer(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, const char *SelectMessage)
    {
        auto Result = -1;
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 2 * Screen.TextBoxHeight / 5;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
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

    void ItemScreen(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, int Character, Equipment::Mode Mode)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 12 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
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
                Graphics::PutText(Renderer, ("The " + std::string(Character::ClassName[Party.Members[Character].Class]) + " is carrying too many items").c_str(), Fonts::Normal, 0, clrBK, intWH, TTF_STYLE_NORMAL, WindowTextWidth, FontSize, ButtonX, TextY);
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
                                if (!Engine::InList(Selection, i))
                                {
                                    New.push_back(Equipment[i]);
                                }
                            }

                            Equipment = New;

                            Selection.clear();

                            if (Equipment.size() == 0)
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

    void TakeScreen(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 12 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
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
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 12 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
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
                    if (!Equipment.empty())
                    {
                        Interface::ItemScreen(Window, Renderer, StoryScreen, Party, Story, Screen, Text, Offset, Character, Equipment::Mode::USE);
                    }

                    Current = -1;
                }
            }
        }
    }

    void Bye(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Button> &StoryScreen, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, SDL_Surface *Text, int Offset, std::string Bye)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowW = 3 * SCREEN_WIDTH / 5;
        auto WindowH = 8 * FontSize + Screen.IconSize;
        auto WindowX = (SCREEN_WIDTH - WindowW) / 2;
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
                auto FlashW = 3 * SCREEN_WIDTH / 5;

                auto FlashH = SCREEN_HEIGHT / 5;

                Graphics::PutTextBox(Renderer, DisplayMessage.c_str(), Fonts::Normal, -1, clrWH, Color, TTF_STYLE_NORMAL, FlashW, FlashH, (SCREEN_WIDTH - FlashW) / 2, Screen.TextBoxY + (Screen.TextBoxHeight - FlashH) / 2);

                if (Color == intBK)
                {
                    Graphics::DrawRect(Renderer, FlashW, FlashH, (SCREEN_WIDTH - FlashW) / 2, Screen.TextBoxY + (Screen.TextBoxHeight - FlashH), intWH);
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

            Screen.IconSize = (buttonw + 2 * text_space);
            Screen.BoxWidth = (SCREEN_WIDTH - 2 * Screen.IconSize) / 2 + text_space;
            Screen.BoxHeight = (SCREEN_HEIGHT - 3 * (buttonh + 2 * text_space));
            Screen.TextBoxWidth = Screen.BoxWidth;
            Screen.TextBoxHeight = Screen.BoxHeight;
            Screen.TextBoxX = Screen.IconSize / 2 + Screen.BoxWidth - text_space;
            Screen.TextBoxY = Screen.IconSize;
            Screen.TextBounds = Screen.TextBoxHeight - 2 * text_space;
            Screen.TextWidth = Screen.TextBoxWidth - 2 * text_space;
            Screen.InfoBoxWidth = Screen.TextBoxWidth - 2 * text_space;
            Screen.InfoBoxHeight = Screen.TextBoxHeight;
            Screen.InfoWidth = Screen.InfoBoxWidth - 2 * text_space;
            Screen.InfoBoxX = Screen.IconSize / 2;
            Screen.InfoBoxY = Screen.IconSize;
            Screen.ObjectSize = buttonw;

            while (!Quit)
            {
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
                    Controls = Story::ExitControls();
                }
                else if (Story->Type == Story::Type::Doom)
                {
                    Controls = Story::ExitControls();
                }
                else if (Story->Controls == Story::Controls::Standard)
                {
                    Controls = Story::StandardControls(Compact);
                }
                else if (Story->Controls == Story::Controls::Info)
                {
                    Controls = Story::InfoControls();
                }
                else
                {
                    Controls = Story::ExitControls();
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

                    if (Story->Text.empty() || Text == NULL)
                    {
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

                                auto Result = Combat::Result::NONE;

                                if (Load)
                                {
                                    Story->SetupCombat(Map, Party);

                                    Result = Interface::CombatScreen(Window, Renderer, Story, Map, Party);

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

                            if (Engine::IsAlive(Party) && !Engine::Paralyzed(Party))
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
                                else
                                {
                                    if (!Engine::IsAlive(Party))
                                    {
                                        Controls = Story::ExitControls();
                                    }
                                }

                                Selected = false;
                            }
                            else
                            {
                                CheckPartyStatus();

                                Controls = Story::ExitControls();
                            }
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
