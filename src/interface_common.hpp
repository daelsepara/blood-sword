#ifndef __INTERFACE_COMMON_HPP__
#define __INTERFACE_COMMON_HPP__

#include "book1.hpp"

namespace Interface
{
    // character class to control type mapping
    std::map<Character::Class, Control::Type> PlayerControls = {
        {Character::Class::Warrior, Control::Type::WARRIOR},
        {Character::Class::Trickster, Control::Type::TRICKSTER},
        {Character::Class::Sage, Control::Type::SAGE},
        {Character::Class::Enchanter, Control::Type::ENCHANTER},
        {Character::Class::Imragarn, Control::Type::IMRAGARN}};

    struct ScreenDimensions
    {
        int ObjectSize = 0;
        int IconSize = 0;
        int BoxWidth = 0;
        int BoxHeight = 0;

        int TextBoxWidth = 0;
        int TextBoxHeight = 0;
        int TextBoxX = 0;
        int TextBoxY = 0;
        int TextWidth = 0;
        int TextBounds = 0;

        int InfoBoxWidth = 0;
        int InfoBoxHeight = 0;
        int InfoBoxX = 0;
        int InfoBoxY = 0;
        int InfoWidth = 0;
    };

    void DisplayParty(SDL_Renderer *Renderer, Party::Base &Party, Interface::ScreenDimensions &Screen)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);

        for (auto i = 0; i < Party.Members.size(); i++)
        {
            auto EquipmentFPR = Engine::Equipment(Party.Members[i], Attributes::Type::FightingProwess, false);
            auto EquipmentPSY = Engine::Equipment(Party.Members[i], Attributes::Type::PsychicAbility, false);
            auto EquipmentAWR = Engine::Equipment(Party.Members[i], Attributes::Type::Awareness, false);
            auto Weapons = Engine::Equipment(Party.Members[i], Attributes::Type::FightingProwess, true);

            auto FightingProwess = Engine::FightingProwess(Party.Members[i]) + (EquipmentFPR.size() > 0 ? EquipmentFPR[0].Score : 0) + (Weapons.size() > 0 ? Weapons[0].Score : 0);
            auto PsychicAbility = Engine::PsychicAbility(Party.Members[i]) + (EquipmentPSY.size() > 0 ? EquipmentPSY[0].Score : 0) - Party.Members[i].Spells.size();
            auto Awareness = Engine::Awareness(Party.Members[i]) + (EquipmentAWR.size() > 0 ? EquipmentAWR[0].Score : 0);
            auto Endurance = Engine::Endurance(Party.Members[i]);
            auto Armour = Engine::Armour(Party.Members[i]);
            auto Damage = Party.Members[i].Damage;
            auto DamageModifier = Party.Members[i].DamageModifier + (Weapons.size() > 0 ? Weapons[0].Damage : 0);

            if (Weapons.empty() == 0 && !Engine::HasAbility(Party.Members[i], Abilities::Type::UnarmedMartialArts))
            {
                FightingProwess = std::max(0, FightingProwess - 2);

                DamageModifier -= 2;
            }

            Graphics::RenderImage(Renderer, Assets::Get(Party.Members[i].Asset), Screen.InfoBoxX + text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + FontSize);
            Graphics::PutText(Renderer, Character::ClassName[Party.Members[i].Class], Fonts::Normal, 0, clrGR, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize));
            Graphics::PutText(Renderer, ("RNK: " + std::to_string(Party.Members[i].Rank)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 3 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize));
            Graphics::PutText(Renderer, ("FPR: " + std::to_string(FightingProwess)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + FontSize);
            Graphics::PutText(Renderer, ("AWR: " + std::to_string(Awareness)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 3 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + FontSize);
            Graphics::PutText(Renderer, ("ARM: " + std::to_string(Armour)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 5 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + FontSize);
            Graphics::PutText(Renderer, ("PSY: " + std::to_string(PsychicAbility) + (Party.Members[i].Spells.size() > 0 ? (" (-" + std::to_string(Party.Members[i].Spells.size()) + ")") : "")).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + 2 * FontSize);
            Graphics::PutText(Renderer, ("END: " + std::to_string(Endurance)).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 3 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + 2 * FontSize);
            Graphics::PutText(Renderer, ("DMG: " + (std::to_string(Damage) + "D" + (DamageModifier < 0 ? "" : "+") + std::to_string(DamageModifier))).c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoBoxX + 5 * Screen.IconSize + 2 * text_space, Screen.InfoBoxY + i * (Screen.IconSize + FontSize) + 2 * FontSize);
        }

        // stats/info box
        Graphics::DrawRect(Renderer, Screen.InfoBoxWidth, Screen.InfoBoxHeight, Screen.InfoBoxX, Screen.InfoBoxY, intWH);
    }

    void RenderLeftPanel(SDL_Window *Window, SDL_Renderer *Renderer, Party::Base &Party, Story::Base *Story, Interface::ScreenDimensions &Screen, std::vector<Button> &Controls)
    {
        auto FontSize = TTF_FontHeight(Fonts::Normal);

        std::string title_string = "";

        if (!Story->Title.empty())
        {
            title_string = Story->Title;

            SDL_SetWindowTitle(Window, Story->Title.c_str());
        }
        else
        {
            if (Story->Id != -1)
            {
                auto StoryId = Story->Id;

                if (StoryId < 0 && Story->DisplayId >= 0)
                {
                    StoryId = Story->DisplayId;
                }

                title_string = "Blood Sword - " + std::string(Book::Title[Story->Book]) + ": " + std::string(3 - std::to_string(std::abs(StoryId)).length(), '0') + std::to_string(std::abs(StoryId));

                SDL_SetWindowTitle(Window, title_string.c_str());
            }
            else
            {
                title_string = "Blood Sword - " + std::string(Book::Title[Story->Book]) + ": Not Implemented Yet";

                SDL_SetWindowTitle(Window, title_string.c_str());
            }
        }

        Graphics::FillWindow(Renderer, intBK);

        // title string
        Graphics::PutText(Renderer, title_string.c_str(), Fonts::Normal, 0, clrWH, intBK, TTF_STYLE_NORMAL, Screen.InfoWidth, FontSize, Screen.InfoBoxX, Screen.InfoBoxY - (FontSize + text_space));

        // party display
        Interface::DisplayParty(Renderer, Party, Screen);

        // text box
        Graphics::FillRect(Renderer, Screen.TextBoxWidth, Screen.TextBoxHeight, Screen.TextBoxX, Screen.TextBoxY, intWH);
    }

    void RenderChoiceCaption(SDL_Renderer *renderer, Button &control, std::string caption, SDL_Color color, Uint32 bg)
    {
        auto captionx = control.X - text_space;

        auto captiony = control.Y + control.H + text_space;

        if (caption.length() > 0)
        {
            auto captionw = 0;

            auto captionh = 0;

            TTF_SizeText(Fonts::Caption, caption.c_str(), &captionw, &captionh);

            Graphics::PutText(renderer, caption.c_str(), Fonts::Caption, border_pts, color, bg, TTF_STYLE_NORMAL, captionw + 2 * text_space, captionh, captionx, captiony);
        }
    }

    void CharacterSheet(SDL_Renderer *Renderer, Character::Base &Character, TTF_Font *Font, int TextWidth, int X, int Y, Uint32 Bg, bool FlipColors)
    {
        auto FontSize = TTF_FontHeight(Font);

        // add equipment bonuses
        auto EquipmentFPR = Engine::Equipment(Character, Attributes::Type::FightingProwess, false);
        auto EquipmentPSY = Engine::Equipment(Character, Attributes::Type::PsychicAbility, false);
        auto EquipmentAWR = Engine::Equipment(Character, Attributes::Type::Awareness, false);
        auto Weapons = Engine::Equipment(Character, Attributes::Type::FightingProwess, true);

        auto FightingProwess = Engine::FightingProwess(Character) + (EquipmentFPR.size() > 0 ? EquipmentFPR[0].Score : 0) + (Weapons.size() > 0 ? Weapons[0].Score : 0);
        auto PsychicAbility = Engine::PsychicAbility(Character) + (EquipmentPSY.size() > 0 ? EquipmentPSY[0].Score : 0) - Character.Spells.size();
        auto Awareness = Engine::Awareness(Character) + (EquipmentAWR.size() > 0 ? EquipmentAWR[0].Score : 0);
        auto Endurance = Engine::Endurance(Character);
        auto Armour = Engine::Armour(Character);
        auto Damage = Character.Damage;
        auto DamageModifier = Character.DamageModifier + (Weapons.size() > 0 ? Weapons[0].Damage : 0);

        if (Weapons.empty() && !Engine::HasAbility(Character, Abilities::Type::UnarmedMartialArts))
        {
            FightingProwess = std::max(0, FightingProwess - 2);

            DamageModifier -= 2;
        }

        if (Engine::HasStatus(Character, Spell::Type::EyeOfTheTiger))
        {
            auto EyeResult = Engine::GetStatus(Character, Spell::Type::EyeOfTheTiger);

            if (EyeResult >= 0 && EyeResult < Character.SpellStatus.size())
            {
                FightingProwess += std::get<3>(Character.SpellStatus[EyeResult]);

                DamageModifier += std::get<3>(Character.SpellStatus[EyeResult]);
            }
        }

        Graphics::PutText(Renderer, Character::ClassName[Character.Class], Font, 0, FlipColors ? clrBK : clrGR, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y);
        Graphics::PutText(Renderer, std::string("RANK: " + std::to_string(Character.Rank)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + (FontSize + 2));
        Graphics::PutText(Renderer, std::string("FIGHTING PROWESS: " + std::to_string(FightingProwess)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 2 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("PSYCHIC ABILITY: " + std::to_string(PsychicAbility) + (Character.Spells.size() > 0 ? (" (-" + std::to_string(Character.Spells.size()) + ")") : "")).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 3 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("AWARENESS: " + std::to_string(Awareness)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 4 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ENDURANCE: " + std::to_string(Endurance)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 5 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("ARMOUR RATING: " + std::to_string(Armour)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 6 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("DAMAGE: " + std::to_string(Damage) + (DamageModifier >= 0 ? "D+" : "D") + std::to_string(DamageModifier)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 7 * (FontSize + 2));
        Graphics::PutText(Renderer, std::string("EXPERIENCE: " + std::to_string(Character.ExperiencePoints)).c_str(), Font, 0, FlipColors ? clrGR : clrWH, Bg, TTF_STYLE_NORMAL, TextWidth, FontSize, X, Y + 8 * (FontSize + 2));
    }

    std::vector<Button> EquipmentList(SDL_Window *Window, SDL_Renderer *Renderer, std::vector<Equipment::Base> &Equipment, int WindowW, int WindowH, int WindowTextX, int WindowTextY, int Start, int Last, int Limit, SDL_Color Fg, Uint32 Bg, Uint32 Highlight, Equipment::Mode Mode)
    {
        auto IconSize = (buttonw + 2 * text_space);
        auto FontSize = TTF_FontHeight(Fonts::Normal);
        auto WindowTextWidth = WindowW - (IconSize + 5 * text_space);
        auto OffsetY = (WindowTextY + WindowH) - (IconSize + 2 * (FontSize + text_space));

        auto Controls = std::vector<Button>();

        if (Equipment.size() > 0)
        {
            for (auto i = 0; i < Last - Start; i++)
            {
                auto index = Start + i;

                auto y = (i > 0 ? Controls[i - 1].Y + Controls[i - 1].H + 3 * text_space : WindowTextY + 2 * text_space);

                Controls.push_back(Button(i, Graphics::CreateHeaderButton(Window, FONT_BOOKMAN, FontSize, Equipment[index].String().c_str(), Fg, Bg, WindowTextWidth - 2 * text_space, FontSize * 2, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, WindowTextX, y, Highlight, Control::Type::CHOICE));

                Controls[i].W = Controls[i].Surface->w;

                Controls[i].H = Controls[i].Surface->h;
            }
        }

        auto idx = (int)Controls.size();

        if (Equipment.size() > Limit)
        {
            if (Start > 0)
            {
                Controls.push_back(Button(idx, Assets::Get(Assets::Type::Up), idx, idx, idx, idx + 1, WindowTextX + Controls[0].W + 3 * text_space, WindowTextY + 2 * text_space, Highlight, Control::Type::SCROLL_UP));

                idx += 1;
            }

            if (Equipment.size() - Last > 0)
            {
                Controls.push_back(Button(idx, Assets::Get(Assets::Type::Down), idx, idx, Start > 0 ? idx - 1 : idx, idx + 1, WindowTextX + Controls[0].W + 3 * text_space, Controls[Last - Start - 1].Y - border_pts, Highlight, Control::Type::SCROLL_DOWN));

                idx += 1;
            }
        }

        idx = (int)Controls.size();

        if (Mode == Equipment::Mode::USE)
        {
            Controls.push_back(Button(idx, Assets::Get(Assets::Type::Use), idx, idx + 1, idx > 0 ? idx - 1 : idx, idx, WindowTextX, OffsetY, Highlight, Control::Type::USE));
            Controls.push_back(Button(idx + 1, Assets::Get(Assets::Type::Transfer), idx, idx + 2, idx > 0 ? idx - 1 : idx + 1, idx + 1, WindowTextX + IconSize, OffsetY, Highlight, Control::Type::TRANSFER));
            Controls.push_back(Button(idx + 2, Assets::Get(Assets::Type::Cancel), idx + 1, idx + 3, idx > 0 ? idx - 1 : idx + 2, idx + 2, WindowTextX + 2 * IconSize, OffsetY, Highlight, Control::Type::DROP));
            Controls.push_back(Button(idx + 3, Assets::Get(Assets::Type::Back), idx + 2, idx + 3, idx > 0 ? idx - 1 : idx + 3, idx + 3, WindowTextX + 3 * IconSize, OffsetY, Highlight, Control::Type::BACK));
        }
        else if (Mode == Equipment::Mode::DROP)
        {
            Controls.push_back(Button(idx, Assets::Get(Assets::Type::Cancel), idx, idx, idx > 0 ? idx - 1 : idx, idx, WindowTextX, OffsetY, Highlight, Control::Type::DROP));
        }
        else if (Mode == Equipment::Mode::TAKE)
        {
            Controls.push_back(Button(idx, Assets::Get(Assets::Type::Use), idx, idx + 1, idx > 0 ? idx - 1 : idx, idx, WindowTextX, OffsetY, Highlight, Control::Type::TAKE));
            Controls.push_back(Button(idx + 1, Assets::Get(Assets::Type::Back), idx, idx + 1, idx > 0 ? idx - 1 : idx + 1, idx + 1, WindowTextX + IconSize, OffsetY, Highlight, Control::Type::BACK));
        }

        return Controls;
    }

    int FindControl(std::vector<Button> &controls, Control::Type control)
    {
        auto found = -1;

        for (auto i = 0; i < controls.size(); i++)
        {
            if (controls[i].Type == control)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    Story::Base *FindStory(Book::Destination destination)
    {
        Story::Base *Next = &Story::notImplemented;

        auto Book = destination.first;

        auto Story = destination.second;

        if (Book == Book::Type::Book1)
        {
            Next = Story::FindStory(Story, Book1::Stories);
        }

        Next->Book = Book;

        Next->Id = Story;

        return Next;
    }
}
#endif
