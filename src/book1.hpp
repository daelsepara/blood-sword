#ifndef __BOOK1_HPP__
#define __BOOK1_HPP__

#include "story.hpp"

namespace Book1
{
    auto Stories = std::vector<std::reference_wrapper<Story::Base>>();

    class Story001 : public Story::Base
    {
    public:
        Story001()
        {
            Book = Book::Type::Book1;

            Id = 1;

            Text = "For the whole day now you have been approaching the smoke columns rising up vertically hundreds of metres from the flat plains of Krarth. The sky above is blue, cloudless and cold. A bitter wind blows across the dry, sere grass of the marshes and sends sluggish ripples through the puddles of mire which occupy the low ground. The monotony of the landscape is broken only by the occasional stunted willow growing by black, muddy pools and meres.\n\nKalugen's Keep is a monolithic black citadel looming ahead of you. Hoping to reach its gates before nightfall, you curse the throng of peasants and merchants milling towards it. You jostle through them towards the citadel -- this morning just an imperceptible speck across the bleak wastes, but now a monstrous edifice that seems to fill the horizon.\n\nYou know you have only until tomorrow to find a sponsor among the magi of Krarth, for tomorrow battle will be unleashed in the pits beneath the citadel. Fame and fortune will go to the Adventurers -- and their sponsor -- who return from the labyrinthine Battlepits with the Emblem of Victory. If you find no magus who is prepared to sponsor you this time, you will have to wait another long year for the next contest. A year through which the marsh waters will rise, drowning the land and the causeways leading to the citadel, rendering Kalugen's Keep impregnable. No one enters or leaves in that time except the magi on their flying carpets, and only in mid-year can the citizens emerge to sow their fields with corn and rice before the harsh winter months begin again.\n\nAt last you manage to break through the gabbling crowds of the baggage train and ride under the grey blocks of the massive gate, its portcullis open like the maw of a hungry god. The dour streets are festooned with the flags of the magi -- the one week of the year when this grim place is ablaze with colour, you reflect wryly - and street criers call out the glories of their lords. In the central square you see a booth where the magi's stewards are registering the combatants whom their masters have employed for tomorrow's contest. Each hero, or group of heroes, must take their sponsor's pennant with them into the Battlepits, but as you scan the racks outside the booth you see that only three pennants remain. Three stewards stand by the pennants. Three magi seek champions.\n\nThe stewards grin sourly as they see you eyeing the pennants. Over the years they must have engaged dozens of brave Adventurers on their respective masters' behalf -- and how many of those Adventurers went down into the Battlepits never to emerge? You scowl back at them grimly, but they only smile the broader. They know you must choose one of the pennants. As you debate the choice, an old merchant wrapped in greasy furs and obviously the worse for drink sees you contemplating the booth.";

            Choices.clear();
            Choices.push_back(Choice::Base("Ignore the drunken merchant and go over to the booth", {Book::Type::Book1, 452}));
            Choices.push_back(Choice::Base("Talk to the merchant", {Book::Type::Book1, 69}));
            Choices.push_back(Choice::Base("TRICKSTER wishes to act", {Book::Type::Book1, 58}, Character::Class::Trickster));
            Choices.push_back(Choice::Base("SAGE wishes to try something", {Book::Type::Book1, 18}, Character::Class::Sage));
            Choices.push_back(Choice::Base("ENCHANTER wishes something", {Book::Type::Book1, 398}, Character::Class::Enchanter));

            Controls = Story::Controls::Standard;
        }
    };

    auto story001 = Story001();
    
    void InitializeStories()
    {
        Book1::Stories = {
            story001
        };
    }
}
#endif