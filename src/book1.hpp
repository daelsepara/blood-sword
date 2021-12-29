#ifndef __BOOK1_HPP__
#define __BOOK1_HPP__

#include "story.hpp"

namespace Book1
{
    auto Stories = std::vector<Story::Base *>();

    class Story001 : public Story::Base
    {
    public:
        Story001()
        {
            Book = Book::Type::Book1;

            Id = 1;

            Text = "For the whole day now you have been approaching the smoke columns rising up vertically hundreds of metres from the flat plains of Krarth. The sky above is blue, cloudless and cold. A bitter wind blows across the dry, sere grass of the marshes and sends sluggish ripples through the puddles of mire which occupy the low ground. The monotony of the landscape is broken only by the occasional stunted willow growing by black, muddy pools and meres.\n\nKalugen's Keep is a monolithic black citadel looming ahead of you. Hoping to reach its gates before nightfall, you curse the throng of peasants and merchants milling towards it. You jostle through them towards the citadel -- this morning just an imperceptible speck across the bleak wastes, but now a monstrous edifice that seems to fill the horizon.\n\nYou know you have only until tomorrow to find a sponsor among the magi of Krarth, for tomorrow battle will be unleashed in the pits beneath the citadel. Fame and fortune will go to the Adventurers -- and their sponsor -- who return from the labyrinthine Battlepits with the Emblem of Victory. If you find no magus who is prepared to sponsor you this time, you will have to wait another long year for the next contest. A year through which the marsh waters will rise, drowning the land and the causeways leading to the citadel, rendering Kalugen's Keep impregnable. No one enters or leaves in that time except the magi on their flying carpets, and only in mid-year can the citizens emerge to sow their fields with corn and rice before the harsh winter months begin again.\n\nAt last you manage to break through the gabbling crowds of the baggage train and ride under the grey blocks of the massive gate, its portcullis open like the maw of a hungry god. The dour streets are festooned with the flags of the magi -- the one week of the year when this grim place is ablaze with colour, you reflect wryly -- and street criers call out the glories of their lords. In the central square you see a booth where the magi's stewards are registering the combatants whom their masters have employed for tomorrow's contest. Each hero, or group of heroes, must take their sponsor's pennant with them into the Battlepits, but as you scan the racks outside the booth you see that only three pennants remain. Three stewards stand by the pennants. Three magi seek champions.\n\nThe stewards grin sourly as they see you eyeing the pennants. Over the years they must have engaged dozens of brave Adventurers on their respective masters\" behalf -- and how many of those Adventurers went down into the Battlepits never to emerge? You scowl back at them grimly, but they only smile the broader. They know you must choose one of the pennants. As you debate the choice, an old merchant wrapped in greasy furs and obviously the worse for drink sees you contemplating the booth.";

            Image = "images/book1/merchants.png";

            TopImage = false;

            Choices.clear();
            Choices.push_back(Choice::Base("Ignore the drunken merchant and go over to the booth", {Book::Type::Book1, 452}));
            Choices.push_back(Choice::Base("Talk to the merchant", {Book::Type::Book1, 69}));
            Choices.push_back(Choice::Base("(TRICKSTER) Try something", {Book::Type::Book1, 58}, Character::Class::Trickster));
            Choices.push_back(Choice::Base("(SAGE) Try something", {Book::Type::Book1, 18}, Character::Class::Sage));
            Choices.push_back(Choice::Base("(ENCHANTER) Try something", {Book::Type::Book1, 398}, Character::Class::Enchanter));

            Controls = Story::Controls::Standard;
        }
    };

    class Story002 : public Story::Base
    {
    public:
        Story002()
        {
            Book = Book::Type::Book1;

            Id = 2;

            Text = "You look down with disgust on the bloated grey bodies of the corpses, now oozing a vile pus which stains the carpet hideously. On one of the dead hands you notice a <b>ruby ring</b>, which you may take if you wish.\n\nOne of the mirrors remains unbroken. There was no corpse behind this one, and when you go closer to inspect it you realise that it is actually a disguised secret door.";

            Image = "images/book1/filler1.png";

            TopImage = false;

            Choices.clear();
            Choices.push_back(Choice::Base("Slide back the mirror and see what lies beyond", {Book::Type::Book1, 379}));
            Choices.push_back(Choice::Base("Continue to the end of the corridor and try the bronze double doors", {Book::Type::Book1, 456}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party) { Equipment = {Item::RubyRing}; }
    };

    class Story003 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story003()
        {
            Book = Book::Type::Book1;

            Id = 3;

            Text = "You ascend to the topmost chamber of the Blue Tower. Cold wind whistles in through the gothic windows. Suddenly a black-gloved hand appears on the window-sill and the masked face of an Assassin rises into view. The Assassins you fled from have climbed the outer wall of the Tower in their determination to slay you.\n\n<b>NOTE</b>\n\nAny that you did not slay in the fight at the bottom of the Tower will now attempt again to murder you. By means of the arcane herbal drugs of their secret society, those that survived the earlier encounter have healed themselves.";

            MapFile = "maps/book1/map003.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            // TODO: only the survivng assassins from previous section should be setup here
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 1", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin, false, true, true));
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 2", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin, false, true, true));
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 3", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin, false, true, true));
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 4", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin, false, true, true));
        }

        void SetupCombat(Map::Base &Map, Party::Base &Party)
        {
            Map.Put(4, 0, Map::Object::Enemy, 0);
            Map.Put(0, 4, Map::Object::Enemy, 1);
            Map.Put(4, 8, Map::Object::Enemy, 2);
            Map.Put(8, 4, Map::Object::Enemy, 3);
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 513};
            }
            else
            {
                Destination = {Book::Type::Book1, 226};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story004 : public Story::Base
    {
    public:
        Story004()
        {
            Book = Book::Type::Book1;

            Id = 4;

            Text = "You unleash the power of the sceptre at the wall. This sends chips of stone flying in all directions, but it is not enough to blast a passage through.";

            Choices.clear();
            Choices.push_back(Choice::Base("Discard the sword", {Book::Type::Book1, 503}));
            Choices.push_back(Choice::Base("Discharge another 2 charges from the <b>steel sceptre</b>", {Book::Type::Book1, 503}, Choice::Type::Discharge, Item::Type::SteelSceptre, 2));

            Controls = Story::Controls::Standard;
        }
    };

    class Story005 : public Story::Base
    {
    public:
        Story005()
        {
            Book = Book::Type::Book1;

            Id = 5;

            Text = "You slip down through the rock like a ghost, feeling the solid surroundings only as a faint chill against your skin. It is rather like sinking through thick, cold tar.\n\nAt last Kief's spell brings you to the lower levels of the Battlepits. You emerge from a rough stone ceiling and drift gently down to solidify on a set of crude steps. An up-draught from below brings a stifling dank heat.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 356}; }
    };

    class Story006 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story006()
        {
            Book = Book::Type::Book1;

            Id = 6;

            Text = "You wrench up the grating and drop lithely to the floor in the midst of the astonished Barbarians. They reach for their axes, but they are taken by surprise.\n\n<b>NOTE</b>\n\nYou get one free action before the normal sequence of combat begins.";

            MapFile = "maps/book1/map006.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Battle.SurprisedEnemy = true;

            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 1", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 2", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 3", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 4", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            Choices.clear();

            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 43};
            }
            else
            {
                Destination = {Book::Type::Book1, 191};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story007 : public Story::Base
    {
    public:
        Story007()
        {
            Book = Book::Type::Book1;

            Id = 7;

            Image = "images/book1/filler2.png";

            TopImage = true;

            Text = "(TRICKSTER) A commanding voice rings out over the clash of swords and the screams of the crowd. A majestic, scarlet-robed figure sweeps forwards through the mob, which parts and shrinks away. The wounded guard lowers his sword. \"My lord,\" he says, \"I was about to apprehend this common thief.\"\n\n\"Be silent,\" commands the robed man. \"You were about to lose your life, you clumsy wretch. If you were in my service, I'd feed you to the nightgaunts. Begone!\"\n\nThe red-robed man turns to you as the guard blanches, bows stiffly and is gone. He smiles coldly at you. \"Your fencing technique is passable, and may be more rewarding to you than your thievery.\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 397}; }
    };

    class Story008 : public Story::Base
    {
    public:
        Story008()
        {
            Book = Book::Type::Book1;

            Id = 8;

            Text = "You glance around. Far across the cavern you can see the end of a tunnel that leads further into the Battlepits. A faint, winking light can be seen at the end of it. The shore of the lake is covered with several large mounds made of piled stones. These look suspiciously like graves to you. You can also see in your torchlight a large block of opaque, murky ice, and beyond that there is the dull glow of red ritual candles that cast a dull glow on a bronze shrine that has been set up against one of the cavern walls. Fiercely burning braziers stand to either side of the shrine.\n\nYou now have a number of options.";

            Choices.clear();
            Choices.push_back(Choice::Base("Explore one of the mounds by clearing away some of the rocks", {Book::Type::Book1, 42}, Choice::Type::SelectAdventurer, "Assign an adventurer to this task"));
            Choices.push_back(Choice::Base("Approach the shrine", {Book::Type::Book1, 71}));
            Choices.push_back(Choice::Base("Investigate the ice block", {Book::Type::Book1, 334}));
            Choices.push_back(Choice::Base("Leave the cavern by the tunnel", {Book::Type::Book1, 279}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story009 : public Story::Base
    {
    public:
        Story009()
        {
            Book = Book::Type::Book1;

            Id = 9;

            Text = "(SAGE) <i>The green potion is a healing draught. The black potion is chimera spittle, a delayed-action poison. The effervescent liquid is an antidote to chimera spittle. The slimy green potion is deadly. You cannot identify the colourless liquid.</i>";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 103}; }
    };

    class Story010 : public Story::Base
    {
    public:
        Story010()
        {
            Book = Book::Type::Book1;

            Id = 10;

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "He is outraged. He half rises from his chair, a tall reed-like figure that seems to unfold rather than stand. His hard thin fingers grip the edge of the table.\n\n\"I am astonished!\" he says at last through clenched teeth. Fangs, really. \"You have had the benefit of tuition from no less than Grandmaster Kief. I am the acknowledged authority on the Spiral of Gold game. I beat Magus Tor -- the true, original Magus Tor -- in a ten-game championship. Yet you cannot understand my explanation! Sickened though I am, I shall not slay you -- merely consign you to the lower depths.\" He lifts a hand from which tendrils of icy flame spread like a net, entangling you. You gasp in sudden pain, <b>LOSING ";

            auto Loss = Engine::Roll(2, 0);

            Text += std::to_string(Loss) + " ENDURANCE</b> as the net of the net of cold fire tightens.";

            if (Party.LastSelected >= 0 && Party.LastSelected < Party.Members.size() && Engine::IsAlive(Party.Members[Party.LastSelected]))
            {
                Engine::Gain(Party.Members[Party.LastSelected], -Loss);

                if (Engine::IsAlive(Party.Members[Party.LastSelected]))
                {
                    Text += "\n\nYou begin to sink into the floor...";
                }
            }
            else
            {
                Text += "\n\nYou begin to sink into the floor...";
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 5}; }
    };

    class Story011 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story011()
        {
            Book = Book::Type::Book1;

            Id = 11;

            Text = "You call the spell to mind. As soon as he sees you starting to concentrate, the Dirge-Man gives a distorted frown of distrust. \"Oh now, what's this?\" With a mad croak, he launches to the attack.";

            MapFile = "maps/book1/map010.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::DirgeMan, "DIRGE-MAN", 7, 6, 7, 13, 2, 0, 1, Assets::Type::DirgeMan));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            Choices.clear();

            if (Result == Combat::Result::ENTHRALED)
            {
                Destination = {Book::Type::Book1, 57};
            }
            else
            {
                Destination = {Book::Type::Book1, 22};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story012 : public Story::Base
    {
    public:
        Story012()
        {
            Book = Book::Type::Book1;

            Id = 12;

            Image = "images/book1/filler2.png";

            TopImage = false;

            Text = "You hurl the ice jewel down into the bubbling pool of magma. The Skiapyrs utter unhuman cries as they see it fall, glittering like frost with the magic it contains. As it strikes the magma, black slabs of cooled rock immediately form around it. The Skiapyrs tremble, their flames visibly dimming. They are the elemental spirits of the crater, and by draining its furnace-heat you have sapped much of their vitality. Nonetheless, they are not prepared to retreat. With cries of stormy vengeance, they bear down upon you.\n\n<b>NOTE</b>\n\nThe Skiapyrs, being ethereal creatures of flame, are able to walk across near-vertical surfaces. You are limited to the narrow ledge along the crater rim. Armour makes no difference to the damage inflicted when a Skiapyr strikes. The fiery claws of these creatures are not impeded even by strong steel plate.";

            MapFile = "maps/book1/map012.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            auto Character = Engine::First(Party, Item::Type::BlueIceJewel);

            if (Character >= 0 && Character < Party.Members.size())
            {
                Engine::Drop(Party.Members[Character], Item::Type::BlueIceJewel);
            }

            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR 1", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR 2", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR 3", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR 4", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR 5", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR 6", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 223}; }
    };

    class Story013 : public Story::Base
    {
    public:
        Story013()
        {
            Book = Book::Type::Book1;

            Id = 13;

            Image = "images/book1/elfin-lady.png";

            TopImage = false;

            Text = "The alcove is not very deep and is covered with a beautiful array of gold and blue mosaics that sparkle in the torchlight. You step into it, hoping to find a hidden door or niche. Instead you are momentarily blinded as the mosaics let out a blinding flash of light. You feel the room spinning uncontrollably. Lights dance and whirl across the walls, and there is an instant of confusion. You realise, too late, that you have walked into a Teleportation spell.\n\nIn an instant the spinning ceases and you are now in an ornate garden under a roof of coloured gems. Sitting on the edge of a gently bubbling fountain is an elfin-featured girl wrapped in a toga of green silk.";

            Choices.clear();
            Choices.push_back(Choice::Base("Attack her", {Book::Type::Book1, 524}));
            Choices.push_back(Choice::Base("Try talking to her", {Book::Type::Book1, 274}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story014 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story014()
        {
            Book = Book::Type::Book1;

            Id = 14;

            Text = "You are nearing the top of the steps leading up the inside of the Blue Tower. A guttural cry like the stalking growl of a great cat breaks through the sombre silence. A black-clad Assassin creeps down the stairs towards you -- he must have scaled the outside of the Tower when you barred the outside door. Assassins that you did not kill now stand in front of you on the stairs. They have restored their Endurance by the use of herbal potions. Even worse, you hear a movement and see that MAGUS VYL has risen from the dead once more and is right behind you! The situation looks bad.\n\n<b>NOTE</b>\n\nBecause he is a vampire, MAGUS VYL has the special ability to paralyse with his touch. Anyone (except a SAGE) wounded by him must take a Psychic Ability test. If they fail the test, they are paralysed and fall to the floor. They take no further part in the combat. SAGEs are immune because of their mind-over-body disciplines.";

            MapFile = "maps/book1/map014.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            // TODO: only the survivng assassins from previous section should be setup here
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 1", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin, false, true, false));
            Enemies.push_back(Enemy::Base(Enemy::Type::MagusVyl, "MAGUS VYL", 7, 9, 9, 35, 3, 0, 2, Assets::Type::MagusVyl));
        }

        void SetupCombat(Map::Base &Map, Party::Base &Party)
        {
            Map.Put(1, 0, Map::Object::Enemy, 0);
            Map.Put(1, 6, Map::Object::Enemy, 1);
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 513};
            }
            else if (Result == Combat::Result::VICTORY)
            {
                Destination = {Book::Type::Book1, 226};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story015 : public Story::Base
    {
    public:
        Story015()
        {
            Book = Book::Type::Book1;

            Id = 15;

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "\"Excellent.\" He conceals his Die behind a slender hand and smiles across the table at you as he considers his opening gambit.";

            Choices.clear();

            if (Engine::Count(Party) == 1)
            {
                Party.LastSelected = Engine::First(Party);

                Text += "\n\nGrandmaster Kief is ready.";

                Choices.push_back(Choice::Base("Choose your opening number", {Book::Type::Book1, 20}, Choice::Type::SelectDice, "Choose your opening number"));
            }
            else
            {
                Choices.push_back(Choice::Base("Decide who is playing against him", {Book::Type::Book1, -15}, Choice::Type::SelectAdventurer, "Decide who is playing against Grandmaster Kief"));
            }
        }
    };

    class Event015 : public Story::Base
    {
    public:
        Event015()
        {
            Book = Book::Type::Book1;

            Id = -15;

            DisplayId = 15;

            Text = "Grandmaster Kief is ready. Choose your opening number.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose your opening number", {Book::Type::Book1, 20}, Choice::Type::SelectDice, "Choose your opening number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story016 : public Story::Base
    {
    public:
        Story016()
        {
            Book = Book::Type::Book1;

            Id = 16;

            Text = "The tunnel is long and sinuous. At last it opens out into a cave where black water laps a shore of pebbles. Skirting this lake, you pass several mounds of large rocks which look distinctly like graves. Passing further across the cave, you discover a large block of murky ice, and beyond that you can see a kind of primitive shrine where ritual braziers burn fiercely. There are no other exits from the cave.\n\nWhat do you want to do now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Dig up a mound", {Book::Type::Book1, 42}));
            Choices.push_back(Choice::Base("Look at the shrine", {Book::Type::Book1, 71}));
            Choices.push_back(Choice::Base("Investigate the ice block", {Book::Type::Book1, 334}));
            Choices.push_back(Choice::Base("Leave the way you've just came from", {Book::Type::Book1, 279}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story017 : public Story::Base
    {
    public:
        Story017()
        {
            Book = Book::Type::Book1;

            Id = 17;

            Text = "\"A sensible decision,\" replies Icon. \"I see no reason to divide our forces when victory is so close at hand. Let our motto, then, be: Trust Shall Prevail!\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            if (Engine::Wounded(Party))
            {
                return {Book::Type::Book1, 51};
            }
            else
            {
                return {Book::Type::Book1, 27};
            }
        }
    };

    class Story018 : public Story::Base
    {
    public:
        Story018()
        {
            Book = Book::Type::Book1;

            Id = 18;

            Text = "(SAGE) You have spent years studying a hundred tomes and crumbling parchments full of ancient knowledge. Surely somewhere you must have read something concerning the magi of Krarth? You clear your mind of all distractions, and gradually the din of the crowds fades away. You see countless old documents with your inner eye, and mentally sift them until you have found what you seek.\n\n<i>The olive-green pennant represents Magus Kalugen, who is overlord of this citadel. A man of notorious cruelty, he has already won so many concessions this year from his fellow magi that he stands to gain very little from winning the contest. Magus Balhazar, whose pennant is a brilliant scarlet, is in quite the opposite position. He desperately needs to win the contest or he will have to cede much of his land -- including thousands of acres of valuable pine forest -- to his rivals. Magus Vyl is perhaps rather appropriately represented by a pennant of sombre black and purple. He is widely reputed to be one of the undead -- a vampire who shuns the day and stalks the night in search of blood.</i>";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 1}; }
    };

    class Story019 : public Story::Base
    {
    public:
        Story019()
        {
            Book = Book::Type::Book1;

            Id = 19;

            Text = "(SAGE) You take a close look at the insignia behind the altar. The three interlocking triangles form the crest of the House of Vyl, which is hardly surprising in view of where you are. Below them there is a smaller engraving, and you almost gasp aloud when you see it.\n\n<i>It is the five-pronged symbol of the Unholy Company of the Eternally Damned, a secret organisation of vampires!</i>";

            Choices.clear();
            Choices.push_back(Choice::Base("Search for a secret panel behind the altar", {Book::Type::Book1, 444}));
            Choices.push_back(Choice::Base("Get out of here right now", {Book::Type::Book1, 3}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story020 : public Story::Base
    {
    public:
        Story020()
        {
            Book = Book::Type::Book1;

            Id = 20;

            Text = "Kief opens his hand to display a 5.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            std::vector<int> Destinations = {50, 45, 40, 35, 30, 25};

            if (Party.LastValue >= 0 && Party.LastValue < 6)
            {
                return {Book::Type::Book1, Destinations[Party.LastValue]};
            }
            else
            {
                return {Book::Type::Book1, Destinations[Engine::Roll(1, 0) - 1]};
            }
        }
    };

    class Story021 : public Story::Base
    {
    public:
        Story021()
        {
            Book = Book::Type::Book1;

            Id = 21;

            Text = "The right-hand corridor goes only a short distance and then opens out into a large chamber. The double doors at the far end of the chamber glimmer with a dull blue sheen. To either side of you, steps lead up to a balcony.\n\nThe most amazing feature of this room is a giant chessboard that has been constructed in the middle of the floor. The chessboard is eight metres along each side, and the pieces are as tall as you are. A glowing sphere hanging from the ceiling casts a flood of light on this playing-area.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go back and take the left-hand passage instead", {Book::Type::Book1, 249}));
            Choices.push_back(Choice::Base("Go up the steps", {Book::Type::Book1, 82}));
            Choices.push_back(Choice::Base("Go and look at the chesspieces", {Book::Type::Book1, 422}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story022 : public Story::Base
    {
    public:
        Story022()
        {
            Book = Book::Type::Book1;

            Id = 22;

            Type = Story::Type::Doom;

            Text = "There is no way for you to cross the chasm. Though the Emblem of Victory stands in sight, it is unattainable. You espy a black-cloaked figure with a golden sword in hand climbing the atoll towards it. Icon the Ungodly. He will win the contest on behalf of Magus Uru, and reap the rewards of wealth and fame, while you will languish here forever in the stifling mists that fill the Battlepits of Krarth.\n\nYour quest ends in failure.";

            Choices.clear();

            Controls = Story::Controls::Exit;
        }
    };

    // TODO: this section describes an combat item effect that will be implemented in
    // the CombatScreen interface. It is preserved here for completeness.
    class Story023 : public Story::Base
    {
    public:
        Story023()
        {
            Book = Book::Type::Book1;

            Id = 23;

            Type = Story::Type::Info;

            Text = "You hurl the <b>dagger</b>. Roll to see if it hits Nebularon. If it does, he loses 1D-2 Endurance (that is, including his Armour Rating reduction). However, whether it hits him or not it doesn't return to you. While close to Nebularon, the spell that returns it to your hand is stifled by his anti-magical protection. You can retrieve it after the fight -- assuming you defeat him.";

            Choices.clear();

            Controls = Story::Controls::Info;
        }
    };

    // TODO: this section describes an item effect that will be implemented in
    // the StoryScreen interface. It is preserved here for completeness.
    class Story024 : public Story::Base
    {
    public:
        Story024()
        {
            Book = Book::Type::Book1;

            Id = 24;

            Type = Story::Type::Info;

            Text = "You unstopper the small bottle and drain its contents. Immediately you feel a wave of agony course through your body, accompanied by a buzzing sensation inside your head. Suddenly your vision seems very sharply defined -- the colours of everything around you become almost painfully vivid. You can hear the slightest noise. Your senses and agility have become cat-like. You have the strength of three men. And you may be dying.\n\nRoll one Die. If you get a 1 or 2, your heart bursts under the strain of the drug you have imbibed. If you roll 3 or more, you can increase your Fighting Prowess and Awareness both by one point -- but you must reduce your Psychic Ability and Endurance by the same amount. (If you had only one Endurance left, you die.) The effect lasts several days, and thus your scores remain as altered for the rest of this adventure.";

            Choices.clear();

            Controls = Story::Controls::Info;
        }
    };

    class Story025 : public Story::Base
    {
    public:
        Story025()
        {
            Book = Book::Type::Book1;

            Id = 25;

            Image = "images/book1/filler3.png";

            TopImage = true;

            Text = "After <i>recovering</i> at the start of the second Spiral, you have two heads and five tails. Grandmaster Kief has six coins, all heads.\n\n\"Allow me to point out something that cannot have escaped your notice,\" he says as you study the coins. \"You can display only a 1 now. That means that if I display anything more than 2 on my Die, you must lose. I hardly think we need play on, do you?\" He sweeps the coins off the table.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 55}; }
    };

    class Story026 : public Story::Base
    {
    public:
        Story026()
        {
            Book = Book::Type::Book1;

            Id = 26;

            Text = "The being spreads his hands over the surface of the table. As he draws them back, fourteen gleaming gold coins are revealed -- seven in a line in front of him, seven on your side of the table. Beside each line of coins rests a six-sided Die. All the coins are showing heads.\n\n\"I am called Kief,\" says the mysterious being. \"I am Grandmaster of this game, which the True Magi called the Spiral of Gold. Pay close attention as I explain it to you.\n\n\"We play in Rounds, called Spirals. In the first Spiral I shall secretly select a number on my Die, placing it under my hand with the number I have chosen uppermost. You do the same. Then we reveal and compare our chosen numbers. Suppose that I have the higher number. In this case you would lose some of your coins -- equal to the difference between our two chosen numbers. I do not get the coins you lose; they just vanish. All right, so in our example you've lost some of your coins. I wouldn't lose any, but the number I displayed on my Die is the number of coins I have to flip over from heads to tails. So if I displayed a 4 and you displayed a 3, you'd lose one coin and I'd have to flip over four of my coins from heads to tails.\n\n\"We then start the next Spiral by <i>recovering</i> -- that is, if either player has any coins showing tails, he can flip one of them over to heads again. Then we select numbers as before, and play proceeds until one player has no heads showing at the end of a Spiral. Then he's lost. \n\n\"There are three other rules you must remember. You cannot choose a number on your Die that is equal to or greater than the number of heads you have showing. That means that we can each put any number from 1 to 6 on the first Spiral, since we start with seven coins, all heads up. But if at some later point in the game I had only five heads showing, I'd have to choose a number from 1 to 4. Secondly, if we both choose the <i>same</i> number then that Spiral is a draw and neither player loses anything. Lastly, when you have to lose a number of coins you must take them from the heads, not the tails, among the coins you have left.\n\n\"Okay, do you understand how to play?\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Reply that you're ready for a game", {Book::Type::Book1, 15}));
            Choices.push_back(Choice::Base("Tell him that you've no idea what this game is about", {Book::Type::Book1, 10}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story027 : public Story::Base
    {
    public:
        Story027()
        {
            Book = Book::Type::Book1;

            Id = 27;

            Text = "Icon suddenly draws his sword and cuts powerfully -- a treacherous blow that inflicts eight Endurance points damage on the first player in the battle order. If still alive, you turn with a shout of rage to attack him.\n\nIcon only laughs: \"Surely you did not expect me to show honour to scum such as you?\" He crouches ready to meet your attack, hastily casting a spell that covers his black armour in a tracery of red sparks.\n\n<b>NOTE</b>\n\nIcon is immune to the Enthralment spell. He has cloaked himself in a spell of Retributive Fire. Anyone who strikes him in close combat -- with, a sword, for example, but not with an arrow -- is burned by this, losing one Endurance point regardless of armour.";

            MapFile = "maps/book1/map027.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::IconTheUngodly, "ICON THE UNGODLY", 8, 8, 7, 28, 2, 2, 2, Assets::Type::IconTheUngodly));
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 377}; }
    };

    class Story028 : public Story::Base
    {
    public:
        Story028()
        {
            Book = Book::Type::Book1;

            Id = 28;

            Text = "You snatch up the red pennant and hold it aloft. Two of the stewards look at each other barely concealing their frustration, and stomp off to the booth. The third rushes up to you excitedly. \"You have chosen the pennant of the esteemed and generous Magus Balhazar!\" he cries.\n\nJust at this moment a tall man dressed in scarlet robes sweeps majestically across the square towards you. The steward bows abjectly to him.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 397}; }
    };

    class Story029 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story029()
        {
            Book = Book::Type::Book1;

            Id = 29;

            Text = "You saunter over while the man is selecting a pastry from a tray. For some reason, no one else at the party comes to your aid.\n\n<b>NOTE</b>\n\nYou get one free round before he can act because he was not expecting your attack.";

            MapFile = "maps/book1/map029.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Battle.SurprisedEnemy = true;

            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::ManInBlue, "MAN IN BLUE", 7, 7, 7, 15, 1, 1, 0, Assets::Type::ManInBlue));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            Choices.clear();

            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 119};
            }
            else
            {
                Destination = {Book::Type::Book1, 184};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story030 : public Story::Base
    {
    public:
        Story030()
        {
            Book = Book::Type::Book1;

            Id = 30;

            Text = "\"A draw,\" says Kief flatly. \"Neither player loses. The next Spiral begins ...\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a number again", {Book::Type::Book1, 310}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    // TODO: verify 279
    class Story031 : public Story::Base
    {
    public:
        Story031()
        {
            Book = Book::Type::Book1;

            Id = 31;

            Text = "The door opens and you enter a small cavern. A man lies dead on the stone floor nearby. Another of Kalugen's guards, by the look of it, probably sent on some mad suicide mission through the underworld by his evil master. Your evil master, too, come to that...\n\nHe has something clutched in his hand. It is an <b>octagonal prism</b> of coloured glass. There are no other exits from the cavern. Nearby there is a marble dais.";

            Choices.clear();
            Choices.push_back(Choice::Base("Step on to the dais", {Book::Type::Book1, 386}));
            Choices.push_back(Choice::Base("Return and take the tunnel beyond the grille", {Book::Type::Book1, 279}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party) { Equipment = {Item::OctagonalPrism}; }
    };

    class Story032 : public Story::Base
    {
    public:
        Story032()
        {
            Book = Book::Type::Book1;

            Id = 32;

            Text = "You loot all the bodies in the room, quickly finding:\n\nfour <b>axes</b>\n\ntwenty <b>gold pieces</b>\n\nfour <b>breastplates</b> (Armour Rating: 1)\n\nthree <b>daggers</b>\n\nthirteen <b>shuriken</b> (useless to you)\n\na <b>vial of black liquid</b>";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Identify the <b>vial of black liquid</b>", {Book::Type::Book1, 463}, Character::Class::Sage, Item::Type::VialOfBlackLiquid));
            Choices.push_back(Choice::Base("Continue", {Book::Type::Book1, 236}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Equipment = {Equipment::Axe, Equipment::Axe, Equipment::Axe, Equipment::Axe, Equipment::Gold(20), Equipment::BreastPlate, Equipment::BreastPlate, Equipment::BreastPlate, Equipment::BreastPlate, Equipment::Dagger, Equipment::Dagger, Equipment::Dagger, Item::VialOfBlackLiquid};
        }
    };

    class Story033 : public Story::Base
    {
    public:
        Story033()
        {
            Book = Book::Type::Book1;

            Id = 33;

            Text = "You strain to manoeuvre the huge bones into place within the frame. Gradually the body of the long-dead giant is reconstructed. When all the sections of the skeleton are in place, a cold dry wind whips up around the atoll, keening hungrily. Skrymir's soul is returning from the dead.\n\nPatches of flesh-mould appear on the sere bones. A spark of life-light gleams inside the sockets of the skull. Veins and sinews spread, glistening tendrils, as the body re-forms from dust.\n\nSkrymir rises to stand before you, the iron frame now forming a suit of armour around his powerfully muscled body. He looks upon you as a man watches a fly. \"I have no heart!\" he bellows. \"You have neglected the heart of Skrymir! I must take yours instead!\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 397}; }
    };

    class Story034 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story034()
        {
            Book = Book::Type::Book1;

            Id = 34;

            Text = "You search for a concealed spring that might open the mirror. Finding none, you wrap your forearm in your cloak and smash hard at the mirror. It shatters at once, showering broken glass on the red carpet. Immediately a hulking shape launches itself from a secret alcove behind where the mirror hung. Almost before you can react, a slavering corpse is battling with you, swinging its great iron mace madly from side to side.";

            MapFile = "maps/book1/map034.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Corpse, "CORPSE 1", 5, 4, 2, 4, 1, 1, 0, Assets::Type::Corpse));
            Enemies.push_back(Enemy::Base(Enemy::Type::Corpse, "CORPSE 2", 5, 4, 2, 4, 1, 1, 0, Assets::Type::Corpse));
            Enemies.push_back(Enemy::Base(Enemy::Type::Corpse, "CORPSE 3", 5, 4, 2, 4, 1, 1, 0, Assets::Type::Corpse));
            Enemies.push_back(Enemy::Base(Enemy::Type::Corpse, "CORPSE 4", 5, 4, 2, 4, 1, 1, 0, Assets::Type::Corpse));
            Enemies.push_back(Enemy::Base(Enemy::Type::Corpse, "CORPSE 5", 5, 4, 2, 4, 1, 1, 0, Assets::Type::Corpse));
            Enemies.push_back(Enemy::Base(Enemy::Type::Corpse, "CORPSE 6", 5, 4, 2, 4, 1, 1, 0, Assets::Type::Corpse));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            Choices.clear();

            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 456};
            }
            else
            {
                Destination = {Book::Type::Book1, 2};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story035 : public Story::Base
    {
    public:
        Story035()
        {
            Book = Book::Type::Book1;

            Id = 35;

            Text = "You have him, and he knows it. After recovering at the start of the next Spiral, he has three heads and four tails. You have only six coins, but they are all heads. The best he can do is play a 2 now, and if you play a 5 that would still knock him out of the game.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 85}; }
    };

    class Story036 : public Story::Base
    {
    public:
        Story036()
        {
            Book = Book::Type::Book1;

            Id = 36;

            Text = "The door creaks open, admitting you to what seems to be a shrine to one of the strange demonic gods of Krarth. A gore-drenched pelt is draped over the obsidian block of the altar. Leaping red flames crackle in iron basins on either side. A pall of grey vapour hangs in the air. Just off to one side of the altar, you see a wrought-iron gate leading to another chamber.";

            Choices.clear();
            Choices.push_back(Choice::Base("Back out of here and continue up the stairs", {Book::Type::Book1, 3}));
            Choices.push_back(Choice::Base("Search the altar", {Book::Type::Book1, 243}));
            Choices.push_back(Choice::Base("Try the wrought-iron gate", {Book::Type::Book1, 526}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story037 : public Story::Base
    {
    public:
        Story037()
        {
            Book = Book::Type::Book1;

            Id = 37;

            Text = "The moment you pick up the sword, a wall appears, blocking the corridor. By experimenting, you find that the wall appears when you pick up the sword and disappears when you put it down. You cannot go on, and the thought of retracing your steps is not inviting.";

            Choices.clear();
            Choices.push_back(Choice::Base("Use the <b>steel sceptre</b>", {Book::Type::Book1, 4}, Choice::Type::Discharge, Item::Type::SteelSceptre, 1));
            Choices.push_back(Choice::Base("Leave the sword", {Book::Type::Book1, 503}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story038 : public Story::Base
    {
    public:
        Story038()
        {
            Book = Book::Type::Book1;

            Id = 38;

            Text = "The moment you lift the skull, its empty eye-sockets seem to glare at you, and the outlines of the room wash away like a watercolour in the rain. Suddenly you find yourself standing alone in a room filled with scrolls. The skull is no longer in your hands, but rests on an oak table beside you.\n\nThe available scrolls are:\n\nINVISIBILITY\n\nEnables the party to take the flee option on the next round without moving into an exit point.\n\nHEALING\n\nRestores 2D endurance up to the initial maximum.\n\nADJUST\n\nTake one point from an attribute (Endurance, Fighting Prowess, Awareness or Psychic Ability) and transfer it to another. The effect is permanent.\n\nTIME BLINK\n\nReturning time to the point when the combat started. The endurance of both player(s) and opponent(s) are restored to their initial values at the start of the combat.\n\nPRECOGNITION\n\nGlimpse into the past or future.\n\n<b>NOTE</b>\n\nThere are several copies of each scroll. Each scroll can be used only once. Once read, it will crumble to dust. None of the scrolls can be read in this room.";

            Choices.clear();
            Choices.push_back(Choice::Base("Take some of the scrolls", {Book::Type::Book1, -38}, Choice::Type::TakeScrolls, {Scroll::Invisibility, Scroll::Healing, Scroll::Adjustment, Scroll::TimeBlink, Scroll::Precognition}));
            Choices.push_back(Choice::Base("Use the <b>scorched skull</b> to leave", {Book::Type::Book1, 97}, Choice::Type::DropItem, Item::Type::ScorchedSkull));

            Controls = Story::Controls::Standard;
        }
    };

    class Event038 : public Story::Base
    {
    public:
        Event038()
        {
            Book = Book::Type::Book1;

            Id = -38;

            DisplayId = 38;

            Choices.clear();

            Controls = Story::Controls::Info;
        }

        Book::Destination Background(Party::Base &Party) { return {Book::Type::Book1, 38}; }
    };

    class Story039 : public Story::Base
    {
    public:
        Story039()
        {
            Book = Book::Type::Book1;

            Id = 39;

            Text = "You take a sip from the bottle before handing it to him. \"What an excellent vintage!\" you say, making a great show of smacking your lips. Reassured, the Dirge-Man takes a long drink himself. As he does, you surreptitiously drink a dose of the antidote. Taking back the chimera spittle, you suggest that the Dirge-Man carries you across the chasm and in return you will give him the rest of this fine 'liquor'. Suspecting nothing, he agrees and allows you to clamber up on to his broad leathery back before launching out into space ...";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 118}; }
    };

    class Story040 : public Story::Base
    {
    public:
        Story040()
        {
            Book = Book::Type::Book1;

            Id = 40;

            Text = "After recovering for the next <i>Spiral</i>, he has three heads and four tails. You have five coins, all heads.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a number for the next Spiral", {Book::Type::Book1, 60}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story041 : public Story::Base
    {
    public:
        Story041()
        {
            Book = Book::Type::Book1;

            Id = 41;

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "(TRICKSTER) You are now out of breath and your sword arm aches from parrying the crude blows of the guard. You begin to feel you were over-confident in taking him on. He has delayed you long enough for his companions to arrive. A spear prods into your back and a voice rings out: 'Drop your sword or I'll stick you like a pig.' You don't wait to be told twice, but drop your sword on to the cobblestones at your feet. You are frogmarched to the bleak dungeon of Magus Kalugen.";

            if (Engine::Count(Party) > 2)
            {
                Text += " Your companions are rounded up and taken there as well.";
            }
            else if (Engine::Count(Party) > 1)
            {
                Text += " Your companion is rounded up and taken there as well.";
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 469}; }
    };

    class Story042 : public Story::Base
    {
    public:
        Story042()
        {
            Book = Book::Type::Book1;

            Id = 42;

            Text = "You roll the heavy stones off the mound. The stones make an eerie clattering echo in the stillness of the cave as they roll away. Suddenly a dead white hand smashes through the rubble and grabs you by the wrist in a vice-like grip.\n\nYou are under a psychic attack.";

            Choices.clear();
            Choices.push_back(Choice::Base("Resist the psychic attack", {Book::Type::Book1, 447}, {Book::Type::Book1, 138}, Choice::Type::TestSelectedCharacter, Attributes::Type::PsychicAbility));

            Controls = Story::Controls::Standard;
        }
    };

    class Story043 : public Story::Base
    {
    public:
        Story043()
        {
            Book = Book::Type::Book1;

            Id = 43;

            Text = "The Barbarians jeer as you run for dear life. You reach a junction and skid to a halt on the marble floor. You must make a snap decision which way to go.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go left, along a corridor of black marble", {Book::Type::Book1, 354}));
            Choices.push_back(Choice::Base("Go right, along a white marble corridor", {Book::Type::Book1, 54}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story044 : public Story::Base
    {
    public:
        Story044()
        {
            Book = Book::Type::Book1;

            Id = 44;

            Text = "You pour the liquid down your throat. It burns as it flows into your stomach, doubling you up in agony. You writhe for a few seconds on the marble terrace, then die with the crazed laughter of the Hags ringing in your ears.";

            Choices.clear();
            Choices.push_back(Choice::Base("Sample another potion", {Book::Type::Book1, 103}));
            Choices.push_back(Choice::Base("Continue on your way", {Book::Type::Book1, 481}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            if (Party.LastSelected >= 0 && Party.LastSelected < Party.Members.size() && Engine::IsAlive(Party.Members[Party.LastSelected]))
            {
                Engine::Endurance(Party.Members[Party.LastSelected], 0);
            }
            else
            {
                auto Character = Engine::First(Party);

                if (Character >= 0 && Character < Party.Members.size() && Engine::IsAlive(Party.Members[Character]))
                {
                    Engine::Endurance(Party.Members[Character], 0);
                }
            }
        }
    };

    class Story045 : public Story::Base
    {
    public:
        Story045()
        {
            Book = Book::Type::Book1;

            Id = 45;

            Text = "After <i>recovering</i> for the next Spiral, he has three heads and four tails. You have only four coins -- all heads.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a number for the next Spiral", {Book::Type::Book1, 90}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story046 : public Story::Base
    {
    public:
        Story046()
        {
            Book = Book::Type::Book1;

            Id = 46;

            Text = "You take out the sparkling emerald gem that Larisha gave you. According to her, it is the last deadly breath of Astarandel the Dragonlord -- contained, she said, within a crystalline spell of confinement. You hurl the gem at Nebularon, diving back to take cover behind a pillar. The gem lands at his dusky feet and begins to shimmer, fading as his protective anti-magical power cancels out the Confinement spell.\n\n\"You fool,\" roars Nebularon, glancing down at it. \"How is this pebble meant to harm ... ?\"\n\nThere is a blinding flash of white light. Even sheltered by the pillar, you nearly black out in the wave of intense heat. Astarandel's last breath sounds like the eruption of a volcano. It is very much louder than Nebularon's dying screams ...";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 457}; }
    };

    class Story047 : public Story::Base
    {
    public:
        Story047()
        {
            Book = Book::Type::Book1;

            Id = 47;

            Text = "(SAGE) You are very careful not to splash any of the liquid on to your lips, knowing that Assassins usually carry the most virulent poisons with them. In fact, you recognise it as a preparation of the drug <i>vatravish</i>, a powerful stimulant that increases the user's speed and strength. Assassins sometimes use it because of their single-minded dedication to their cause, but you know that it can have unpleasant side-effects. You throw the bottle away.\n\nYou decide it is time to return to the main square and see about getting one of the other magi as a patron.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 443}; }
    };

    class Story048 : public Story::Base
    {
    public:
        Story048()
        {
            Book = Book::Type::Book1;

            Id = 48;

            Image = "images/book1/filler1.png";

            TopImage = false;

            Text = "You return to the gothic-arched vestibule. Looking again at the shining countenance on the wall, you wonder if it is indicating that you should take the left-hand passage. If so, you intend to do as it suggests. You advance down the passage.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 249}; }
    };

    class Story049 : public Story::Base
    {
    public:
        Story049()
        {
            Book = Book::Type::Book1;

            Id = 49;

            Text = "(ENCHANTER) \"There was the Warlock known as Icon the Ungodly,\" she replies. \"And two scurvy knaves who resisted all our blandishments. Lastly, there was a single Sage who survived the bridge crossing where all his comrades died. He helped us with our cooking...\" She glances at one of the cauldrons.\n\nYour Enthralment is slipping. On a whim you snatch the ladle out of a nearby cauldron and pour its noxious contents over her. There is a hissing as it dissolves the wretched Hag into a pool of greyish slime. The others scream and spit, but do nothing out of fear of your powers.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 67}; }
    };

    class Story050 : public Story::Base
    {
    public:
        Story050()
        {
            Book = Book::Type::Book1;

            Id = 50;

            Text = "\"Probably you see that you now cannot win by any strategy,\" he says smugly. Your three coins are all heads. He, after recovering for the next Spiral, has three heads and four tails. Before you can say anything, he reaches out and sweeps the remaining coins off the table.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 55}; }
    };

    class Story051 : public Story::Base
    {
    public:
        Story051()
        {
            Book = Book::Type::Book1;

            Id = 51;

            Text = "Icon steps forwards and shakes hands. \"First, let me attend to those wounds,\" he suggests. \"We shall need to be at full strength for the dangers we must yet face, and a small application of healing sorcery is what is needed here ...\"\n\nHe raises his hands and begins to chant in Harada, the language of his native land of Yamato. Seconds later, he gives a grin of feral mirth as noxious vapours trail across the ground towards you. He has cast the Mists of Death.\n\n<b>NOTE</b>\n\nEveryone in the party must take a PSYCHIC ABILITY test. Players who fail the test lose 2D endurance.";

            Choices.clear();
            Choices.push_back(Choice::Base("Resist the Mists of Death spell", {Book::Type::Book1, 27}, Choice::Type::EnemyCastSpell, Spell::Type::MistsOfDeath));

            Controls = Story::Controls::Standard;
        }
    };

    class Story052 : public Story::Base
    {
    public:
        Story052()
        {
            Book = Book::Type::Book1;

            Id = 52;

            Text = "You lift the olive-green pennant. \"You have chosen the standard of Magus Kalugen, overlord of this city,\" declares Kalugen's steward grandly. \"I am to escort you to his palace, the House of the. Fivefold Torments. There you will be received with the honour due to you.\" As he speaks, a number of soldiers draw up around you and you find that they are leading you off through the winding dark alleyways of the city. The jostling crowds miraculously melt away when they see the olive-green livery of the soldiers, and you soon find yourself walking through the main gate of Kalugen's palace, a massive, fire-blackened mansion-house with heavily barred windows and a high ordure-stained tower rising in one of its corners. The great gate of the mansion is swung shut behind you and you find yourself in a courtyard crowded with a group of raggedly dressed people waiting in line, clutching the petitions they hope to present to Kalugen. You are ushered through this mob into a dark hall whose ceiling is lost in the sooty gloom. You are led to the far end of it, where you can dimly make out a seated figure on a throne. You instinctively recognise him as Kalugen.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 471}; }
    };

    class Story053 : public Story::Base
    {
    public:
        Story053()
        {
            Book = Book::Type::Book1;

            Id = 53;

            Text = "(TRICKSTER) The usher looks shocked. \"I assure you,\" he says, spluttering with indignation, \"my loyalty to my master is beyond price!\"\n\nWell -- perhaps beyond the price you can afford, you think wryly.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 458}; }
    };

    class Story054 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story054()
        {
            Book = Book::Type::Book1;

            Id = 54;

            Image = "images/book1/assassins.png";

            TopImage = true;

            Text = "You come to a door and shoulder it open. Three wiry men dressed all in black look up from the body they've just slain with their daggers. A group of Assassins! To make matter worse, you can hear the Barbarians charging in pursuit of you. A moment later they burst into the room just behind you.\n\n<b>NOTE</b>\n\nIn any Round in which an Assassin is not adjacent to an opponent, he will throw a Shuriken.\n\nThe Assassins and the Barbarians are mutually hostile. That is, they will treat one another as an enemy when it comes to selecting opponents.";

            MapFile = "maps/book1/map054.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 1", 7, 6, 7, 6, 1, 0, 0, Assets::Type::NinjaAssassin, false, true, true));
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 2", 7, 6, 7, 6, 1, 0, 0, Assets::Type::NinjaAssassin, false, true, true));
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 3", 7, 6, 7, 6, 1, 0, 0, Assets::Type::NinjaAssassin, false, true, true));

            // TODO: only add the survivng barbarians from previous section should be setup here
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 1", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian, true, false, true));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 2", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian, true, false, true));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 354};
            }
            else
            {
                Destination = {Book::Type::Book1, 32};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story055 : public Story::Base
    {
    public:
        Story055()
        {
            Book = Book::Type::Book1;

            Id = 55;

            Text = "Grandmaster Kief stands, unfolding himself from his chair like a great robed stick-insect. \"Our game is at an end,\" he declares. \"It is time for your transition to the lower levels...\"\n\nHis hands come up, showering a web of sparks around you. You feel an acrid taste in your mouth, but no pain. For a moment you wonder what he has done to you, then you see. You are sinking into the floor of the amphitheatre!";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 5}; }
    };

    class Story056 : public Story::Base
    {
    public:
        Story056()
        {
            Book = Book::Type::Book1;

            Id = 56;

            Text = "You look out from the side terrace of the temple. A flight of unsupported marble stairs descends from here to a low table of rock. You can just make out two shimmering ovals of energy that hang in the air there.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go down the stairway", {Book::Type::Book1, 427}));
            Choices.push_back(Choice::Base("Go around to the other side of the temple", {Book::Type::Book1, 357}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story057 : public Story::Base
    {
    public:
        Story057()
        {
            Book = Book::Type::Book1;

            Id = 57;

            Text = "The Dirge-Man waits passively to be given his instructions. You climb up on to his wide leathery back and, clinging on for dear life, spur him into flight. He swoops out across the chasm, and you catch a dizzying glimpse of vertical precipices stretching right down into the bowels of the under-earth. Wind currents buffet you to and fro, but the Dirge-Man rides them expertly. In a few moments he sets you gently down on the far side. You hastily order him to fly off before the spell can fade.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 359}; }
    };

    class Story058 : public Story::Base
    {
    public:
        Story058()
        {
            Book = Book::Type::Book1;

            Id = 58;

            Text = "(TRICKSTER) Under the merchant's beer-stained fur jacket you notice a bulging money-pouch. Being a Trickster, your thoughts naturally turn to theft.";

            Choices.clear();
            Choices.push_back(Choice::Base("Try to rob the merchant", {Book::Type::Book1, 121}, {Book::Type::Book1, 181}, Character::Class::Trickster, Attributes::Type::Awareness));

            Controls = Story::Controls::Standard;
        }
    };

    class Story059 : public Story::Base
    {
    public:
        Story059()
        {
            Book = Book::Type::Book1;

            Id = 59;

            Text = "You heave the cage into view. The emerald scarab still hangs around the skeleton's neck: a thick green slime seems to be congealing on it from some deposit in the water. You quickly wrench the scarab off the neck and wipe away the hideous muck before the eidolon reconstitutes itself. Then you let the cage and its bones splash back into the lake, where they sink out of sight.";

            // TODO: verify if the scarab is taken as an item
            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Identify the scarab", {Book::Type::Book1, 352}, Character::Class::Sage));
            Choices.push_back(Choice::Base("Risk putting it on", {Book::Type::Book1, 187}, Choice::Type::SelectAdventurer, "Who shall wear the scarab?"));
            Choices.push_back(Choice::Base("Leave the scarab and go on your way", {Book::Type::Book1, 247}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story060 : public Story::Base
    {
    public:
        Story060()
        {
            Book = Book::Type::Book1;

            Id = 60;

            Text = "This time he opens his hand to reveal a 2.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            std::vector<int> Destinations = {65, 70, 75, 80, 286, 286};

            if (Party.LastValue >= 0 && Party.LastValue < 6)
            {
                return {Book::Type::Book1, Destinations[Party.LastValue]};
            }
            else
            {
                return {Book::Type::Book1, Destinations[Engine::Roll(1, 0) - 1]};
            }
        }
    };

    class Story061 : public Story::Base
    {
    public:
        Story061()
        {
            Book = Book::Type::Book1;

            Id = 61;

            Text = "You enter a room at the end of the corridor. Three Assassins in blood-drenched black robes and face masks lie in a heap at the centre of the room. Presumably they were another magus's champions, but fell foul of the Barbarian party. Searching the bodies, you find many of the Assassin's favoured missile weapon, the deadly throwing spike. Unfortunately you cannot use this type of weapon as you have not been trained to do so. There is also a smashed black vial that one of the Barbarians would seem to have ground underfoot.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 236}; }
    };

    class Story062 : public Story::Base
    {
    public:
        bool Visited = false;

        Story062()
        {
            Book = Book::Type::Book1;

            Id = 62;

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "You feel a wave of energy coursing through your body. You are about to take a second draught, but the Hag squatting by the cauldron snatches the ladle from your hand. \"Only one draught per person,\" she cackles. \"Otherwise it's poisonous, you see...\" You cannot drink the same potion again.\n\n<b>NOTE</b>\n\nYour Endurance score increased one point above its current maximum.";

            if (Party.LastSelected >= 0 && Party.LastSelected < Party.Members.size())
            {
                if (!Engine::Visited(Party, Party.LastSelected, {Book::Type::Book1, 62}))
                {
                    Engine::Visit(Party, Party.LastSelected, {Book::Type::Book1, 62});

                    auto Endurance = Engine::Maximum(Party.Members[Party.LastSelected], Attributes::Type::Endurance) + 1;

                    Engine::Maximum(Party.Members[Party.LastSelected], Attributes::Type::Endurance, Endurance);

                    Engine::Endurance(Party.Members[Party.LastSelected], Endurance);
                }
                else
                {
                    Text = "You are about to take a second draught, but the Hag squatting by the cauldron snatches the ladle from your hand. \"Only one draught per person,\" she cackles. \"Otherwise it's poisonous, you see...\" You cannot drink the same potion again.\n\n<b>NOTE</b>\n\nYour Endurance score increased one point above its current maximum.";
                }
            }

            Choices.clear();

            if (Engine::Count(Party) > 1)
            {
                Choices.push_back(Choice::Base("Let others drink the same potion", {Book::Type::Book1, -62}, Choice::Type::LoseMoney, "Who shall drink the bubbling green soup", 2));
            }
            else
            {
                Choices.push_back(Choice::Base("Try another potion you have not yet sampled", {Book::Type::Book1, 103}));
            }

            Choices.push_back(Choice::Base("Don't sample another potion", {Book::Type::Book1, 481}));
        }
    };

    class Event062 : public Story::Base
    {
    public:
        Event062()
        {
            Book = Book::Type::Book1;

            Id = -62;

            DisplayId = 62;

            Choices.clear();

            Controls = Story::Controls::Info;
        }

        Book::Destination Background(Party::Base &Party) { return {Book::Type::Book1, 62}; }
    };

    // TODO: this section describes an combat item effect that will be implemented in
    // the CombatScreen interface. It is preserved here for completeness.
    class Story063 : public Story::Base
    {
    public:
        Story063()
        {
            Book = Book::Type::Book1;

            Id = 63;

            Type = Story::Type::Info;

            Text = "As you point the ring at Nebularon, it bursts into flame. You <b>LOSE 1 endurance</b> as a searing pain shoots up your arm. If you are still alive, you watch the ring burn away to ashes. Nebularon laughs -- a harsh, hollow roar -- and mocks you: \"Come, mortal! Such tawdry baubles are of no use to you now.\"";

            Choices.clear();

            Controls = Story::Controls::Info;
        }
    };

    class Story064 : public Story::Base
    {
    public:
        Story064()
        {
            Book = Book::Type::Book1;

            Id = 64;

            Text = "You arrive at Balhazar's mansion as the temple gongs of the citadel sound out the hour of midnight. A long avenue flanked by cedar trees and decorated with gaily covered lanterns and embroidered banners on poles stretches to the white marble portico of the main entrance. Guards patrol up and down the avenue, but they seem to ignore most of the guests. However, they stare long and hard at you and are just about to step up and challenge you when Balhazar's usher rushes from the doorway of the house to greet you. He gestures you towards the doorway, behind which you can see a party in full swing. All the revellers wear masks concealing their identities. They are entertained by jugglers and musicians.\n\nThe usher offers you a tray with drinks and sweetmeats laid out on it. \"My master will speak to you of employment,\" he says. \"That is\" .. he gestures at the dozens of revellers -- \"if you have the ability to find him...\"\n\nYou stare in bewilderment at the masked party-goers. How are you to locate your prospective employer among them?";

            Choices.clear();
            Choices.push_back(Choice::Base("(TRICKSTER) Bribe the usher", {Book::Type::Book1, 53}, Character::Class::Trickster));
            Choices.push_back(Choice::Base("(SAGE) Use ESP", {Book::Type::Book1, 309}, Abilities::Type::ESP));
            Choices.push_back(Choice::Base("(ENCHANTER) Summon Faltyn", {Book::Type::Book1, 232}, Character::Class::Enchanter));
            Choices.push_back(Choice::Base("Use your powers of deduction to work out which of the revellers is Balhazar", {Book::Type::Book1, 458}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story065 : public Story::Base
    {
    public:
        Story065()
        {
            Book = Book::Type::Book1;

            Id = 65;

            Text = "After <i>recovering</i> for the next Spiral, he has two heads and five tails. You have only four coins left, but all are heads. He can play only a 1 now, so you automatically win.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 85}; }
    };

    class Story066 : public Story::Base
    {
    public:
        Story066()
        {
            Book = Book::Type::Book1;

            Id = 66;

            Text = "(SAGE) The Elf called upon the spirit-gods of his ancestors as he died. These are the eerie beings that now surround you in the fog, and they have come to wreak vengeance on you for slaying their blood-kin. You lose no time focusing your energy on Exorcising them, and fortunately it works. The spirits draw back and the fog disappears.\n\nYou can now loot the Elves' bodies if you wish. They had the following equipment:\n\nsix <b>swords</b>\n\n<b>bow</b>\n\neight <b>arrows</b>\n\nsix <b>leather jerkins</b> (Armour Rating: 1)";

            Bye = "After taking anything you want, you start to walk along the corridor.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Equipment = {Equipment::Sword, Equipment::Sword, Equipment::Sword, Equipment::Sword, Equipment::Sword, Equipment::Sword, Equipment::Bow, Equipment::Arrows(8), Equipment::LeatherJerkin, Equipment::LeatherJerkin, Equipment::LeatherJerkin, Equipment::LeatherJerkin, Equipment::LeatherJerkin, Equipment::LeatherJerkin, Equipment::LeatherJerkin};
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 503}; }
    };

    class Story067 : public Story::Base
    {
    public:
        Story067()
        {
            Book = Book::Type::Book1;

            Id = 67;

            Choices.clear();
            Choices.push_back(Choice::Base("Investigate the temple", {Book::Type::Book1, 298}));
            Choices.push_back(Choice::Base("Take the left-hand path around it", {Book::Type::Book1, 357}));
            Choices.push_back(Choice::Base("Try a potion", {Book::Type::Book1, 103}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story068 : public Story::Base
    {
    public:
        Story068()
        {
            Book = Book::Type::Book1;

            Id = 68;

            Text = "You approach a remarkable sight. A large basalt platform hangs suspended in the air, apparently unsupported, fifty metres above the cavern floor. On the ground directly underneath it stands a bronze gong.";

            Choices.clear();
            Choices.push_back(Choice::Base("Use the <b>bronze mallet</b>", {Book::Type::Book1, 508}, Item::Type::BronzeMallet));
            Choices.push_back(Choice::Base("Head straight for the Emblem of Victory", {Book::Type::Book1, 238}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story069 : public Story::Base
    {
    public:
        Story069()
        {
            Book = Book::Type::Book1;

            Id = 69;

            Text = "He lurches from side to side, struggling to focus on you. Giving vent to an enormous belch, he says, \"Lookin' fer employ with a magus, you'l l be, I don' wonder. There's three still wantin' champions fer the contest -- that's their pennants over by that booth ...\n\n\"Magus Vyl is the worst 'o the lot, but then 'e's a vampire an' I shouldn't speak ill 'o the dead. Kalugen -- lord 'o this city, as you'l l know -- well, 'e ain't much better, an' he's not in the contest to win, I've 'eard. But the third -- Magus Balhazar -- is the best of a bad bunch, \"cause 'e's got most to lose in the contest, an' 'e'l l be rewardin' 'is champions the best on account 'o that.\"\n\nAny hopes you might have of questioning the merchant further are soon dashed. Exhausted by the effort of speaking, he favours you with a broad grin and then falls like a plank on the cobblestones. A boot in the ribs does nothing to rouse him from his drunken stupor, so you decide to approach the booth.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 452}; }
    };

    class Story070 : public Story::Base
    {
    public:
        Story070()
        {
            Book = Book::Type::Book1;

            Id = 70;

            Text = "\"A draw,\" says Kief flatly. You proceed to the next Spiral, and after <i>recovering</i> he has four heads and three tails. He thus gains a little ground against your five heads, and you must choose your next number carefully.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose your next number", {Book::Type::Book1, 185}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story071 : public Story::Base
    {
    public:
        Story071()
        {
            Book = Book::Type::Book1;

            Id = 71;

            Text = "It is a simple shrine to Freya, the Mercanian fertility goddess.";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Say the appropriate words of reverence", {Book::Type::Book1, 153}, Character::Class::Sage));
            Choices.push_back(Choice::Base("Investigate the block of ice", {Book::Type::Book1, 334}));
            Choices.push_back(Choice::Base("Investigate one of the grave mounds", {Book::Type::Book1, 42}));
            Choices.push_back(Choice::Base("Leave via the tunnel", {Book::Type::Book1, 279}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story072 : public Story::Base
    {
    public:
        Story072()
        {
            Book = Book::Type::Book1;

            Id = 72;

            Text = "You reason that the Fetishists' lair cannot be far away. Sure enough, you soon spy a narrow cave entrance in the pebble-strewn slope. It is only about five metres off the path, so you feel no risk in edging down to investigate. Volcanic fumes seep out of the cave and drift down the slope, slowly adding to the swirling fog of mist far below. Holding your breath, you reach in a little way. Your hand encounters a corroded copper scroll-case containing an aged yellow parchment.";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Read the scroll", {Book::Type::Book1, 254}, Character::Class::Sage));
            Choices.push_back(Choice::Base("Continue along the path to the shrine", {Book::Type::Book1, 339}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story073 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story073()
        {
            Book = Book::Type::Book1;

            Id = 73;

            Text = "After some time they come back. They are slightly wounded, but seem to be in good spirits. \"That's better,\" says the red-bearded one. \"Now I've worked up a sweat.\"\n\n\"A sweat?\" guffaws one. \"Why, Torsvald, you must be getting old, by golly, if fighting little Assassins gets you winded!\"\n\n\"Ho,\" laughs Torsvald, \"maybe I am, but maybe it's not sweat at all but the grease off those slippery little runts, eh?\"\n\nThis looks like a good moment to ambush them. You dislodge the grille and it falls on Torsvald's head. Too bad it didn't hit a vital spot. You drop in their midst and get one free strike before they can collect their wits to fight.\n\n<b>NOTE</b>\n\nYou get one free action before the normal sequence of combat begins.";

            MapFile = "maps/book1/map073.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Battle.SurprisedEnemy = true;

            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 1", 8, 5, 7, 11, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 2", 8, 5, 7, 11, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 3", 8, 5, 7, 10, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 4", 8, 5, 7, 8, 1, 2, 1, Assets::Type::Barbarian));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            Choices.clear();

            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 506};
            }
            else
            {
                Destination = {Book::Type::Book1, 251};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story074 : public Story::Base
    {
    public:
        Story074()
        {
            Book = Book::Type::Book1;

            Id = 74;

            Text = "You search their bodies and find twenty <b>gold pieces</b>. You can also take their four <b>axes</b> and four <b>bronze breastplates</b> if you wish.\n\nReasoning that they will have already cleared out any treasure hoards to the left, you decide to head right. The corridor takes several sharp turns. You come to an object that sparkles in the light of the wall-lanterns. It is a sword, encrusted with gems of fabulous value. But as you are about to pick it up, your natural suspicion gets the better of you. Why should anyone leave a magnificent weapon like this just lying in the middle of a corridor?";

            Choices.clear();
            Choices.push_back(Choice::Base("Leave it and continue on your way", {Book::Type::Book1, 503}));
            Choices.push_back(Choice::Base("(ENCHANTER) Investigate it with Detect spells", {Book::Type::Book1, 387}, Character::Class::Enchanter));
            Choices.push_back(Choice::Base("Pick it up regardless of the consequences", {Book::Type::Book1, 37}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Equipment = {Equipment::Axe, Equipment::Axe, Equipment::Axe, Equipment::Axe, Equipment::Gold(20), Equipment::BreastPlate, Equipment::BreastPlate, Equipment::BreastPlate, Equipment::BreastPlate};
        }
    };

    class Story075 : public Story::Base
    {
    public:
        Story075()
        {
            Book = Book::Type::Book1;

            Id = 75;

            Text = "After <i>recovering</i> for the next Spiral, he has three heads and three tails. Your five coins now show three heads and two tails. \"Our positions are rather close, are they not?\" says Kief with a broad smile.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose your number for the next Spiral", {Book::Type::Book1, 225}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story076 : public Story::Base
    {
    public:
        Story076()
        {
            Book = Book::Type::Book1;

            Id = 76;

            Text = "You look at the four nearest mirrors. There might be a secret passage behind at least one of them, and you decide which one to try first.";

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Choices.clear();

            if (Engine::Count(Party) > 1)
            {
                Choices.push_back(Choice::Base("Look at the mirrors", {Book::Type::Book1, -76}, Choice::Type::SelectAdventurer, "Assign an adventurer to the task"));
            }
            else
            {
                Party.LastSelected = Engine::First(Party);

                Choices.push_back(Choice::Base("Look behind the first mirror", {Book::Type::Book1, 231}));
                Choices.push_back(Choice::Base("Look behind the second mirror", {Book::Type::Book1, 304}));
                Choices.push_back(Choice::Base("Look behind the third mirror", {Book::Type::Book1, 202}));
                Choices.push_back(Choice::Base("Look behind the fourth mirror", {Book::Type::Book1, 34}));
            }
        }
    };

    class Event076 : public Story::Base
    {
    public:
        Event076()
        {
            Book = Book::Type::Book1;

            Id = -76;

            DisplayId = 76;

            Choices.clear();
            Choices.push_back(Choice::Base("Look behind the first mirror", {Book::Type::Book1, 231}));
            Choices.push_back(Choice::Base("Look behind the second mirror", {Book::Type::Book1, 304}));
            Choices.push_back(Choice::Base("Look behind the third mirror", {Book::Type::Book1, 202}));
            Choices.push_back(Choice::Base("Look behind the fourth mirror", {Book::Type::Book1, 34}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story077 : public Story::Base
    {
    public:
        Story077()
        {
            Book = Book::Type::Book1;

            Id = 77;

            Text = "The corridor brings you to a T-junction. From here you can go straight on along the corridor or you can turn right down the narrow tunnel that joins it.";

            Choices.clear();
            Choices.push_back(Choice::Base("Continue heading straight on", {Book::Type::Book1, 338}));
            Choices.push_back(Choice::Base("Take the tunnel to your right", {Book::Type::Book1, 16}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story078 : public Story::Base
    {
    public:
        Story078()
        {
            Book = Book::Type::Book1;

            Id = 78;

            Text = "You materialise within the broken walls of an old shrine. You can see the temple where the Hags live, about two hundred metres away atop another pinnacle.\n\nExamining your surroundings, you notice a golden dish that glimmers in the sporadic light of the volcanic geysers outside. Grey stone gargoyles cling to the walls, the dancing shadows making them seem to grin. Beyond an exit at the rear of the shrine, a narrow path leads along the top of a knife-edge precipice.";

            Choices.clear();
            Choices.push_back(Choice::Base("Leave the shrine", {Book::Type::Book1, 92}));
            Choices.push_back(Choice::Base("Examine the gold dish first", {Book::Type::Book1, 507}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story079 : public Story::Base
    {
    public:
        Story079()
        {
            Book = Book::Type::Book1;

            Id = 79;

            Image = "images/book1/filler2.png";

            TopImage = false;

            Text = "The Dirge-Man settles on the steps above you, flexing his enormous leathery wings as if glad of the rest. You notice that he does not have the spiked talons of a normal Dirge-Man, but human hands. \"I could fly you across,\" he declares in his cracked voice. \"But a question has arisen. Essentially, what's in it for me?\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Use a <b>gilded bridle</b>", {Book::Type::Book1, 301}, Item::Type::GildedBridle));
            Choices.push_back(Choice::Base("Use a <b>chimera spittle</b>", {Book::Type::Book1, 321}, Item::Type::ChimeraSpittle));
            Choices.push_back(Choice::Base("(ENCHANTER) Enthral the Dirge-Man", {Book::Type::Book1, 11}, Character::Class::Enchanter));
            Choices.push_back(Choice::Base("You cannot do any of these", {Book::Type::Book1, 22}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story080 : public Story::Base
    {
    public:
        Story080()
        {
            Book = Book::Type::Book1;

            Id = 80;

            Text = "After <i>recovering</i> for the next Spiral, he has two heads and three tails. You have the same. This position only allows you both to play a 1 on the next Spiral, so you then each have three heads and two tails.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose your number for the next Spiral", {Book::Type::Book1, 320}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story081 : public Story::Base
    {
    public:
        Story081()
        {
            Book = Book::Type::Book1;

            Id = 81;

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "You lean breathless against the wall and count the cost of this desperate battle. The body of the vampire Magus lies on the cold flagstones at your feet.";

            if (!Engine::IsPresent(Party, Character::Class::Sage))
            {
                Text += " You leave and make your way up the stairs to the top of the Tower.";
            }
        }

        Book::Destination Continue(Party::Base &Party)
        {
            if (Engine::IsPresent(Party, Character::Class::Sage))
            {
                return {Book::Type::Book1, 416};
            }
            else
            {
                return {Book::Type::Book1, 14};
            }
        }
    };

    class Story082 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story082()
        {
            Book = Book::Type::Book1;

            Id = 82;

            Text = "You ascend the steps to the balcony and run straight into four bowmen crouching there in ambush. They wear red livery -- the champions of Magus Horg you think. The two nearest jump up with swords in their hands, while the other two notch arrows on to their bowstrings.";

            MapFile = "maps/book1/map082.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Bowmen, "BOWMAN 1", 7, 6, 7, 6, 1, 0, 0, Assets::Type::Bowmen, false, true, false));
            Enemies.push_back(Enemy::Base(Enemy::Type::Bowmen, "BOWMAN 2", 7, 6, 7, 6, 1, 0, 0, Assets::Type::Bowmen, false, true, false));
            Enemies.push_back(Enemy::Base(Enemy::Type::Bowmen, "BOWMAN 3", 7, 6, 7, 6, 1, 0, 0, Assets::Type::Bowmen, false, true, false));
            Enemies.push_back(Enemy::Base(Enemy::Type::Bowmen, "BOWMAN 4", 7, 6, 7, 6, 1, 0, 0, Assets::Type::Bowmen, false, true, false));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 48};
            }
            else
            {
                Destination = {Book::Type::Book1, 281};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story083 : public Story::Base
    {
    public:
        Story083()
        {
            Book = Book::Type::Book1;

            Id = 83;

            Text = "(TRICKSTER) You are too long in the tooth to be caught out here. Obviously the 'statues' are previous adventurers. But they must have thought there was something of interest in the alcoves to take the risk... Peering into an alcove, you notice a secret door at the back. Possibly you could open it and step through without touching the floor of the dais -- perhaps evading the petrifying spell. Then again, you may be wrong. And who wants to spend eternity as a lump of stone...?";

            Choices.clear();
            Choices.push_back(Choice::Base("You are prepared to run the risk", {Book::Type::Book1, 533}));
            Choices.push_back(Choice::Base("Return and take the other corridor", {Book::Type::Book1, 354}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story084 : public Story::Base
    {
    public:
        Story084()
        {
            Book = Book::Type::Book1;

            Id = 84;

            Text = "\"So you did.\" He grins, but does not seem grateful. \"Death would doubtless appreciate a sacrifice in return for the loss of my great soul, so prepare to die in my place. This is the gratitude of Skrymir!\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 342}; }
    };

    class Story085 : public Story::Base
    {
    public:
        Story085()
        {
            Book = Book::Type::Book1;

            Id = 85;

            Text = "Kief looks at you with a wry smile. \"<i>Very</i> few people have ever beaten me at the game,\" he says. \"No reward I can offer truly expresses my admiration. Nonetheless, I have a couple of items that might interest you.\"\n\nHe places two objects on the table. One is an <b>octagonal prism</b> of coloured glass. The other is a sparkling <b>blue ice jewel</b>. \"Take both,\" says Kief. \"I feel magnanimous. And now, prepare to enter the nether caverns...\"";

            Bye = "Flickering beams of light shoot from his fingertips, swiftly weaving a web of energy around you. You flinch momentarily before realising that the beams are not hurting you. What, then, are they for? You have your answer when you look down: you are sinking into the floor!";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party) { Equipment = {Item::OctagonalPrism, Item::BlueIceJewel}; }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 5}; }
    };

    class Story086 : public Story::Base
    {
    public:
        Story086()
        {
            Book = Book::Type::Book1;

            Id = 86;

            Image = "images/book1/filler1.png";

            TopImage = false;

            Text = "(ENCHANTER) You call the spell to mind and then cast it. There is no need to roll for this because you are not in a combat situation and do not need to hurry the enchantment. You are able to see any spells fluorescing with a bright aura under this spell\n\n<i>You see no sign of magic from the sarcophagi, though this does not necessarily mean they don't contain something magical. The dais of grey stone, however, flickers with powerful sorcery. As to whether that sorcery is benign or hostile -- you cannot tell.</i>";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 246}; }
    };

    class Story087 : public Story::Base
    {
    public:
        Story087()
        {
            Book = Book::Type::Book1;

            Id = 87;

            Text = "The last of them falls to the floor. As he dies, he reaches out towards the fire-burnt skull. His fingers twitch in front of its sightless sockets and he grunts something in the guttural Mercanian tongue. Then he dies. Examining their bodies, you find four <b>battleaxes</b> and four <b>breastplates</b> (Armour Rating 1) and <b>twenty gold pieces</b>.";

            Choices.clear();
            Choices.push_back(Choice::Base("Take the skull", {Book::Type::Book1, 38}, Choice::Type::TakeEquipment, {Item::ScorchedSkull}));
            Choices.push_back(Choice::Base("Leave this room and continue on your way", {Book::Type::Book1, 249}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Equipment = {Equipment::Axe, Equipment::Axe, Equipment::Axe, Equipment::Axe, Equipment::Gold(20), Equipment::BreastPlate, Equipment::BreastPlate, Equipment::BreastPlate, Equipment::BreastPlate};
        }
    };

    class Story088 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story088()
        {
            Book = Book::Type::Book1;

            Id = 88;

            Text = "You reach the other side of the bridge and step on to the terrace. The Hags glare at you but keep their distance.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            if (Engine::IsPresent(Party, Character::Class::Imragarn))
            {
                if (Engine::Count(Party, Code::Status::CROSSED_BRIDGE) >= (Engine::Count(Party) - 1))
                {
                    Destination = {Book::Type::Book1, 519};
                }
                else
                {
                    Destination = {Book::Type::Book1, 142};
                }
            }
            else
            {
                if (Engine::Count(Party, Code::Status::CROSSED_BRIDGE) >= Engine::Count(Party))
                {
                    Destination = {Book::Type::Book1, 98};
                }
                else
                {
                    Destination = {Book::Type::Book1, 142};
                }
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story089 : public Story::Base
    {
    public:
        Story089()
        {
            Book = Book::Type::Book1;

            Id = 89;

            Text = "Which item will you use?";

            Choices.clear();
            Choices.push_back(Choice::Base("Use an <b>ice jewel</b>", {Book::Type::Book1, 99}, Item::Type::BlueIceJewel));
            Choices.push_back(Choice::Base("... a <b>vellum scroll</b>", {Book::Type::Book1, 312}, Item::Type::VellumScroll));
            Choices.push_back(Choice::Base("... or a <b>ruby ring</b>", {Book::Type::Book1, 391}, Item::Type::RubyRing));
            Choices.push_back(Choice::Base("You do not have any of these, or choose not to use them", {Book::Type::Book1, 112}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story090 : public Story::Base
    {
    public:
        Story090()
        {
            Book = Book::Type::Book1;

            Id = 90;

            Text = "He chose a 2.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            std::vector<int> Destinations = {95, 100, 105, 286, 286, 286};

            if (Party.LastValue >= 0 && Party.LastValue < 6)
            {
                return {Book::Type::Book1, Destinations[Party.LastValue]};
            }
            else
            {
                return {Book::Type::Book1, Destinations[Engine::Roll(1, 0) - 1]};
            }
        }
    };

    class Story091 : public Story::Base
    {
    public:
        Story091()
        {
            Book = Book::Type::Book1;

            Id = 91;

            Image = "images/book1/filler2.png";

            TopImage = false;

            Text = "You throw yourself back down the corridor and snatch up the sword. In the same instant that you pick up the sword, a wall of solid stone appears behind you, blocking the corridor. The sword was a magical trap and you are now penned in at the end of a cul-de-sac. The Barbarians are now almost on top of you.";

            Choices.clear();
            Choices.push_back(Choice::Base("Drop the sword", {Book::Type::Book1, 264}));
            Choices.push_back(Choice::Base("Keep the sword and fight it out", {Book::Type::Book1, 314}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story092 : public Story::Base
    {
    public:
        Story092()
        {
            Book = Book::Type::Book1;

            Id = 92;

            Image = "images/book1/shabby-adventurers.png";

            TopImage = false;

            Text = "You wend your way along a narrow ridge rising like a sharp backbone out of the cavern floor. A murky sea of mist sits in the hollows below, illuminated now and then by the red lightning gouts of fire. The path brings you to a high-walled crater with a tall stone pylon built in the centre. The pylon has an entrance that is clearly visible, and there may be many treasure-laden chambers within. But you have to put aside such thoughts, because the pylon is entirely cut off by a moat of boiling lava and there is no way you could hope to reach it.\n\nThe path divides and snakes along the crater rim. You skirt the bubbling lava pit, slowly making your way towards an incline that leads off to the far end of the cavern.\n\nSuddenly two shabby Adventurers appear from behind a boulder near the lip of the crater. They look eerie in the glowering light thrown up from the lava below, clutching their swords like fiends out of the Pit. One of them grins crookedly, displaying rotten teeth. \"Your money,\" he says, \"<i>and</i> your life.\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 198}; }
    };

    class Story093 : public Story::Base
    {
    public:
        Story093()
        {
            Book = Book::Type::Book1;

            Id = 93;

            Text = "Larisha stands up, trailing her silk toga like a shadow. \"Return now to the magi's labyrinth,\" she says, lifting her hands to display dozens of jewelled rings. \"If I possessed a soul, perhaps I would wish you well...\"\n\nColours and sounds jangle and swirl about you. For a moment you are jerked off your feet, then you settle down on to a carpeted floor. The confusion of lights fades to reveal that you are now in a corridor lined with mirrors.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 326}; }
    };

    class Story094 : public Story::Base
    {
    public:
        Story094()
        {
            Book = Book::Type::Book1;

            Id = 94;

            Text = "There is a profusion of different costumes all around you. A man in a sequined mask capers about the room in jester's motley. A blond-wigged man dressed like a Mercanian reaver enters from the garden with a slim young lady on his arm. You notice two men standing beside a fountain in earnest conversation: one is dressed like a torturer, the other like a perfumed dandy. On a chaise-longue, you notice a fellow in a domino cape romancing a girl in black velvet.\n\nWhom will you approach?";

            Choices.clear();
            Choices.push_back(Choice::Base("Approach the jester", {Book::Type::Book1, 527}));
            Choices.push_back(Choice::Base("... the reaver", {Book::Type::Book1, 417}));
            Choices.push_back(Choice::Base("... the torturer", {Book::Type::Book1, 123}));
            Choices.push_back(Choice::Base("... the dandy", {Book::Type::Book1, 277}));
            Choices.push_back(Choice::Base("... or the domino", {Book::Type::Book1, 487}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story095 : public Story::Base
    {
    public:
        Story095()
        {
            Book = Book::Type::Book1;

            Id = 95;

            Text = "After <i>recovering</i> for the next Spiral, he has two heads and five tails. You lost a coin, leaving you with three heads.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose your number for the next Spiral", {Book::Type::Book1, 110}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story096 : public Story::Base
    {
    public:
        Story096()
        {
            Book = Book::Type::Book1;

            Id = 96;

            Text = "The sarcophagi are indeed built on a grand scale. You were not able to fully appreciate the size until you approached one close up. It is more than four metres long, big enough to hold the body of an Ogre or a Frost Giant. The carvings on the lids depict four old men in regal attire. Ancestors of the loathsome Magus Kalugen? There is little to suggest a family resemblance in the wise faces of these long-dead monarchs.";

            Choices.clear();
            Choices.push_back(Choice::Base("Pass on towards the beach", {Book::Type::Book1, 293}));
            Choices.push_back(Choice::Base("Open the first sarcophagus", {Book::Type::Book1, 351}));
            Choices.push_back(Choice::Base("... the second", {Book::Type::Book1, 186}));
            Choices.push_back(Choice::Base("... the third", {Book::Type::Book1, 403}));
            Choices.push_back(Choice::Base("... or the fourth", {Book::Type::Book1, 137}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story097 : public Story::Base
    {
    public:
        Story097()
        {
            Book = Book::Type::Book1;

            Id = 97;

            Image = "images/book1/filler1.png";

            TopImage = false;

            Text = "You reappear in the first room. You no longer have the skull, so you cannot make a second visit to the scriptorium. You step out of the room and continue along the tunnel.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 249}; }
    };

    class Story098 : public Story::Base
    {
    public:
        Story098()
        {
            Book = Book::Type::Book1;

            Id = 98;

            Text = "Now that the show is over, the disgusting Hags lope back to their cauldrons. You move across the terrace, feeling their rheumy eyes on you. Their repulsive familiars rush about under your feet -- rotting crows with eyes dangling from sockets, yellow-fanged rats, warty malformed toads with human faces, scuttling objects that suggest entrails on legs... They clutch at your heels as you walk past, and you feel you would prefer to be back on the bridge fighting than endure this horror. The Hags themselves crouch on the temple steps, gabbling to one another and shrieking with insane laughter.";

            Choices.clear();
            Choices.push_back(Choice::Base("Talk to the Hags", {Book::Type::Book1, 212}));
            Choices.push_back(Choice::Base("Go up the steps into the temple", {Book::Type::Book1, 298}));
            Choices.push_back(Choice::Base("Take the path around to the right of the temple", {Book::Type::Book1, 56}));
            Choices.push_back(Choice::Base("Walk around it to the left", {Book::Type::Book1, 357}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story099 : public Story::Base
    {
    public:
        Story099()
        {
            Book = Book::Type::Book1;

            Id = 99;

            Text = "You fling the ice jewel out from the ledge, calling upon its power. It chills the warm air currents that enable the Dirges to stay aloft, and they spiral down out of control into the mist-shrouded river that courses along the gorge far below. Breathing a sigh of relief, you make your way down to the platform at the bottom of the ledge.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 142}; }
    };

    class Story100 : public Story::Base
    {
    public:
        Story100()
        {
            Book = Book::Type::Book1;

            Id = 100;

            Text = "A draw. The next Spiral sees him <i>recovering</i> to four heads and three tails. You have lost any advantage you may have had, because you have four heads -- out of a total of four coins. You grit your teeth, determined to win back his gain in the next Spiral.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose your number for the next Spiral", {Book::Type::Book1, 125}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story101 : public Story::Base
    {
    public:
        Story101()
        {
            Book = Book::Type::Book1;

            Id = 101;

            Type = Story::Type::Doom;

            Image = "images/book1/filler3.png";

            TopImage = false;

            Text = "There is no way across the simmering pool of magma. You slump to the floor in despair. You have failed in your quest, but that is now the least of your worries. Trapped in the isolated pylon, you face a slow agonising death by starvation.";

            Choices.clear();

            Controls = Story::Controls::Exit;
        }
    };

    class Story102 : public Story::Base
    {
    public:
        Story102()
        {
            Book = Book::Type::Book1;

            Id = 102;

            Text = "(TRICKSTER) You flip the catch of the secret door and step over the dais carefully without setting foot on the floor of the alcove. The door leads to a narrow compartment where you find a gold ring.\n\n<i>Slipping this on, you sense that it is a Ring of Warding. It has four charges, and each charge will increase your Armour Rating by one for the duration of one combat.</i>\n\nYou step back into the room and return to try the passage of black marble.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            auto Character = Engine::Find(Party, Character::Class::Trickster);

            if (Character >= 0 && Character < Party.Members.size())
            {
                Party.Members[Character].Equipment.push_back(Item::RingOfWarding);
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 354}; }
    };

    class Story103 : public Story::Base
    {
    public:
        Story103()
        {
            Book = Book::Type::Book1;

            Id = 103;

            Text = "Which potion will you try?\n\n<b>NOTE</b>\n\nFor each potion you try you must part with two gold pieces.";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Examine the potions", {Book::Type::Book1, 9}, Character::Class::Sage));
            Choices.push_back(Choice::Base("Try the bubbling green soup", {Book::Type::Book1, 62}, Choice::Type::LoseMoney, "Who drinks the soup?", 2));
            Choices.push_back(Choice::Base("... the tarry black substance", {Book::Type::Book1, 299}, Choice::Type::LoseMoney, "Who shall try the tarry black substance?", 2));
            Choices.push_back(Choice::Base("... the effervescent liquid", {Book::Type::Book1, 358}, Choice::Type::LoseMoney, "Who shall try the effervescent liquid?", 2));
            Choices.push_back(Choice::Base("... the slimy green concoction", {Book::Type::Book1, 44}, Choice::Type::LoseMoney, "Who shall try the slimy green concoction?", 2));
            Choices.push_back(Choice::Base("... the colourless brew", {Book::Type::Book1, 409}, Choice::Type::LoseMoney, "Who shall try the colourless brew?", 2));
            Choices.push_back(Choice::Base("You cannot pay", {Book::Type::Book1, 481}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story104 : public Story::Base
    {
    public:
        Story104()
        {
            Book = Book::Type::Book1;

            Id = 104;

            Text = "(TRICKSTER) Unseen fingers grope at your backpack and belt as you pass through the waterfall. They are stealing from you -- a fact you realise too late when you emerge on the other side stripped of your possessions.\n\nYou turn and glare angrily into the waterfall. It is just a theatrical gesture -- you can do nothing to recover your possessions, as they have been taken by thieving Fossergrims. These water spirits are invulnerable to any sort of physical or magical coercion.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            auto OtherEquipment = 0;

            auto Character = Engine::Find(Party, Character::Class::Trickster);

            if (Character >= 0 && Character < Party.Members.size())
            {
                auto Equipment = std::vector<Equipment::Base>();

                for (auto i = 0; i < Party.Members[Character].Equipment.size(); i++)
                {
                    Equipment::Base &Item = Party.Members[Character].Equipment[i];

                    if (Item.Weapon == Equipment::Weapon::Sword || Item.Class == Equipment::Class::Armour)
                    {
                        Equipment.push_back(Item);
                    }
                    else
                    {
                        OtherEquipment++;
                    }
                }

                if (OtherEquipment > 0)
                {
                    Party.Members[Character].Equipment = Equipment;
                }
                else
                {
                    Party.Members[Character].Equipment.clear();
                }
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 267}; }
    };

    class Story105 : public Story::Base
    {
    public:
        Story105()
        {
            Book = Book::Type::Book1;

            Id = 105;

            Text = "After recovering for the next Spiral, he has three heads and three tails. You have only two heads and two tails -- a situation that forces you to play a 1 this time. Smiling, Kief opens his hand to show a 2.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 120}; }
    };

    class Story106 : public Story::Base
    {
    public:
        Story106()
        {
            Book = Book::Type::Book1;

            Id = 106;

            Choices.clear();
            Choices.push_back(Choice::Base("Use the <b>steel sceptre</b>", {Book::Type::Book1, 219}, Item::Type::SteelSceptre));
            Choices.push_back(Choice::Base("Use the <b>emerald scarab</b>", {Book::Type::Book1, 517}, Item::Type::EmeraldScarab));
            Choices.push_back(Choice::Base("You have none of these or choose not to use them", {Book::Type::Book1, 369}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story107 : public Story::Base
    {
    public:
        Story107()
        {
            Book = Book::Type::Book1;

            Id = 107;

            Image = "images/book1/filler1.png";

            TopImage = true;

            Text = "At last you manage to pull the cage up again. As it breaks the surface, the eidolon gives a forlorn howl and begins to wring its hands. It seems to be imploring you to leave its skeleton untouched, but you know that a monstrous being such as this can be shown no mercy. You tear the scarab amulet from the skeleton's neck. It gives a brief pulse of light. There is a scream from the eidolon, then it dissipates into empty air.";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Identify the <b>emerald scarab</b>", {Book::Type::Book1, 352}, Character::Class::Sage, Item::Type::EmeraldScarab));
            Choices.push_back(Choice::Base("Risk putting it on", {Book::Type::Book1, 187}, Choice::Type::SelectAdventurer, "Who shall wear the emerald scarab?"));
            Choices.push_back(Choice::Base("Leave the scarab and go on your way", {Book::Type::Book1, 247}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party) { Equipment = {Item::EmeraldScarab}; }
    };

    class Story108 : public Story::Base
    {
    public:
        Story108()
        {
            Book = Book::Type::Book1;

            Id = 108;

            Text = "(SAGE) You must attempt to awaken the latent powers of your 'inner eye'. Calling on the teachings of your Adept Masters, you begin to clear your mind of all distractions, trying to see beyond the natural spectrum.";

            Choices.clear();
            Choices.push_back(Choice::Base("Clear your mind", {Book::Type::Book1, 313}));
            Choices.push_back(Choice::Base("<i>Clear your mind</i>", {Book::Type::Book1, 263}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story109 : public Story::Base
    {
    public:
        Story109()
        {
            Book = Book::Type::Book1;

            Id = 109;

            Text = "(ENCHANTER) You are used to the wiles and evasive cunning of feckless Faltyns. \"No,\" you correct it sharply. \"Not any magus -- locate Magus Balhazar.\"\n\nThe Faltyn flickers across the edge of your vision -- a shimmering blue figure. A moment later it is back at your side. <i>Balhazar has instructed me not to point him out, it murmurs. He prefers that you use your initiative for this test. Still, I will levy no charge under the circumstances...</i>\n\n\"Get you gone, vile creature!\" you snap at it, alarming it into dematerialising at once.";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Try ESP", {Book::Type::Book1, 309}, Abilities::Type::ESP));
            Choices.push_back(Choice::Base("Try something else", {Book::Type::Book1, 458}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story110 : public Story::Base
    {
    public:
        Story110()
        {
            Book = Book::Type::Book1;

            Id = 110;

            Text = "His number this time is a 1, of course.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            std::vector<int> Destinations = {50, 120, 286, 286, 286, 286};

            if (Party.LastValue >= 0 && Party.LastValue < 6)
            {
                return {Book::Type::Book1, Destinations[Party.LastValue]};
            }
            else
            {
                return {Book::Type::Book1, Destinations[Engine::Roll(1, 0) - 1]};
            }
        }
    };

    class Story111 : public Story::Base
    {
    public:
        Story111()
        {
            Book = Book::Type::Book1;

            Id = 111;

            Choices.clear();
            Choices.push_back(Choice::Base("(TRICKSTER) Try something", {Book::Type::Book1, 329}, {Book::Type::Book1, 444}, Character::Class::Trickster, Attributes::Type::Awareness));

            Controls = Story::Controls::Standard;
        }
    };

    class Story112 : public Story::Base
    {
    public:
        Story112()
        {
            Book = Book::Type::Book1;

            Id = 112;

            Text = "You wedge back against some rock outcroppings on the ledge to give you partial cover from the Dirges' attack.\n\n<b>NOTE</b>\n\nIf the party includes a Sage and/or a Trickster, they can shoot twice at the Dirges (assuming they have arrows) before normal combat begins. It is not worth trying to get away, as they would only swoop in pursuit.";

            Bye = "You continue down the ledge to the platform.";

            MapFile = "maps/book1/map112.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Dirges, "DIRGES 1", 6, 5, 6, 5, 1, 2, 0, Assets::Type::Dirges));
            Enemies.push_back(Enemy::Base(Enemy::Type::Dirges, "DIRGES 2", 6, 5, 6, 5, 1, 2, 0, Assets::Type::Dirges));
            Enemies.push_back(Enemy::Base(Enemy::Type::Dirges, "DIRGES 3", 6, 5, 6, 5, 1, 2, 0, Assets::Type::Dirges));
            Enemies.push_back(Enemy::Base(Enemy::Type::Dirges, "DIRGES 4", 6, 5, 6, 5, 1, 2, 0, Assets::Type::Dirges));
            Enemies.push_back(Enemy::Base(Enemy::Type::Dirges, "DIRGES 5", 6, 5, 6, 5, 1, 2, 0, Assets::Type::Dirges));
            Enemies.push_back(Enemy::Base(Enemy::Type::Dirges, "DIRGES 6", 6, 5, 6, 5, 1, 2, 0, Assets::Type::Dirges));

            Battle.ShootingRounds = 2;
        }

        void SetupCombat(Map::Base &Map, Party::Base &Party)
        {
            Map.Put(3, 1, Map::Object::Enemy, 0);
            Map.Put(3, 4, Map::Object::Enemy, 1);
            Map.Put(3, 5, Map::Object::Enemy, 2);
            Map.Put(3, 7, Map::Object::Enemy, 3);
            Map.Put(3, 8, Map::Object::Enemy, 4);
            Map.Put(3, 10, Map::Object::Enemy, 5);
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 142}; }
    };

    class Story113 : public Story::Base
    {
    public:
        Story113()
        {
            Book = Book::Type::Book1;

            Id = 113;

            Text = "(TRICKSTER) You have to decide which of the two bridges to use. They appear identical, but in order to reach the further of the two you must first pass through the waterfall issuing from the gargoyle's mouth.";

            Choices.clear();
            Choices.push_back(Choice::Base("Cross via the nearer bridge", {Book::Type::Book1, 267}, Character::Class::Trickster));
            Choices.push_back(Choice::Base("Go through the waterfall to the other bridge", {Book::Type::Book1, 464}, Character::Class::Trickster));

            Controls = Story::Controls::Standard;
        }
    };

    class Story114 : public Story::Base
    {
    public:
        Story114()
        {
            Book = Book::Type::Book1;

            Id = 114;

            Text = "The moment you enter the gate, you are under assault by some unknown psychic force. You spin in the air as though in a tunnel of glaring blue light. Ahead is an area of darkness -- the destination of the gate. Behind, you can still see the distorted image of the area you have just come from: the table of rock, with the jangled edges of the stairway and temple beyond it.";

            Choices.clear();
            Choices.push_back(Choice::Base("(PARTY) Test Psychic Ability", {Book::Type::Book1, 428}, {Book::Type::Book1, -114}, Choice::Type::TestParty, Attributes::Type::PsychicAbility, Choice::Consequence::LoseItemOrEndurance));
            Choices.push_back(Choice::Base("Turn back and take the other gate", {Book::Type::Book1, 307}));

            Controls = Story::Controls::Standard;
        }
    };

    class Event114 : public Story::Base
    {
    public:
        Event114()
        {
            Book = Book::Type::Book1;

            Id = -114;

            DisplayId = 114;

            Choices.clear();

            Controls = Story::Controls::Info;
        }

        Book::Destination Background(Party::Base &Party) { return {Book::Type::Book1, 114}; }
    };

    class Story115 : public Story::Base
    {
    public:
        Story115()
        {
            Book = Book::Type::Book1;

            Id = 115;

            Text = "After <i>recovering</i> for the next Spiral, Kief has three heads and three tails. This contrasts rather strongly with your three heads -- and no tails. He watches you for a moment, then says: \"Well, you have reached a no-win situation. A Converging Spiral, as we call it.\" His hands snake out to sweep the coins from the table.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 55}; }
    };

    class Story116 : public Story::Base
    {
    public:
        Story116()
        {
            Book = Book::Type::Book1;

            Id = 116;

            Text = "\"Ah, it is well,\" says the ghost with a misty, emotionless smile. \"The loyal servants of Zyn shall share in his glory, and I shall reward you above all others.\" It passes its hands over the sarcophagus lid, which slowly rises into the air. You look upon a mouldered skeleton that clutches a lump of granite in its broken fingers.\n\n\"My mortal form...\" says the ghost wistfully. \"So long since I could taste the musky wines of Asmuly or feel a scented breeze from off the meadows... Take the stone! Take it quickly! I wish to wallow no longer in the memory of things lost to me.\"\n\nYou have no choice but to comply. Your agreement to aid the ghost has bereft you of any power to oppose him. You take the lump of granite from the skeleton's hands, and the sarcophagus slowly closes again.\n\nYou see that you hold a fossilised heart. \"It is the heart of the giant Skrymir,\" explains the ghost. \"He was destroyed by the True Magi, but he shall be my instrument of vengeance against their heirs, these mewling modern magi who have usurped the ancient grandeur...\" He flickers and seems to grow larger and more tenebrous for a moment, then settles down into a cold hard radiance. \"Go towards the atoll. I have no interest in this petty contest; it is of no concern to me whether you take the Emblem of Victory or not. Do so if you wish. However, on your way to the Emblem at the summit, you will pass through chambers where the sundered fragments of Skrymir's body lie -- his massive legs, his rib-cage, his arms and fleshless skull. Take them with you. At the summit, assemble them and place the fossilised heart in his dusty chest. Then stand you back, for the magic of Zyn shall roar forth from the cosmic interstices once again, as it did in times of old. Flesh shall clothe his yellowed bones; his heart shall beat and warm blood shall course through his veins; his eyes shall open and behold this travesty of ancient Krarth, and to the upstart magi he shall mete out a most fitting fate. Now, make ready to return below...\"\n\nHe lifts diaphanous hands. A stream of grey-blue lights surrounds you. The scene shifts, and once more you find yourself on the plain below the floating platform.\n\n<b>NOTE</b>\n\nYou can proceed as before -- but remember that you have the heart of Skrymir now and -- for the moment at least -- you cannot discard it.";

            Choices.clear();
            Choices.push_back(Choice::Base("Take the <b>heart of Skrymir</b>", {Book::Type::Book1, 238}, Choice::Type::TakeEquipment, {Item::HeartOfSkrymir}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story117 : public Story::Base
    {
    public:
        Story117()
        {
            Book = Book::Type::Book1;

            Id = 117;

            Text = "Skrymir rises from the ground, seeming to tower up and up. His beard flows like rivulets from a glacier; his eyes burn like frost. He raises his arms towards the cavern roof and gives a great wordless bellow of exultation. The walls seem to shake. If the magi in the city above hear it, they must be trembling now.\n\n\"Life!\" he thunders. \"To live again! To turn about the yoke of death and place it about the magi's necks! This is what I have dreamed of in my centuried sleep. And now, quake, you magi. Bolt the gates of your citadels. Marshal your armies and your puny magics. Skrymir strides the earth once more, and this time his iron-shod feet shall tread your mortal bodies into the mire!\"\n\nSkrymir is obviously getting ready to return to the surface to put some of his plans for vengeance into immediate effect.";

            Choices.clear();
            Choices.push_back(Choice::Base("Point out that you have just restored him to life", {Book::Type::Book1, 84}));
            Choices.push_back(Choice::Base("Say nothing", {Book::Type::Book1, 257}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story118 : public Story::Base
    {
    public:
        Story118()
        {
            Book = Book::Type::Book1;

            Id = 118;

            Text = "On the Dirge-Man's back, you are carried far out across the chasm. The roaring wind-currents seem to slam you to and fro at random, but the Dirge-Man is used to them and manoeuvres with a bizarre elegance. Halfway across, he brings up the matter of the chimera spittle: \"I think it is best to be quite candid. If you do not allow me to finish the liquor, I shall loop over and drop you into the chasm below.\"\n\n\"Do not be too hasty,\" you retort. \"The 'liquor' was in fact a deadly poison for which I had already taken the antidote. If you expect to receive the antidote yourself, you had better ensure a safe and comfortable landing.\"\n\nWhimpering in fear for his life, the Dirge-Man flutters in to a landing on the opposite rim of the chasm. You hand him the rest of the chimera spittle -- swearing that, although it looks like the poison, it is in fact the antidote. The Dirge-Man drinks eagerly, and you leave before he discovers the truth.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 359}; }
    };

    class Story119 : public Story::Base
    {
    public:
        Story119()
        {
            Book = Book::Type::Book1;

            Id = 119;

            Text = "Dejectedly you return to the square. The black and purple pennant has gone. You are relieved to see that one pennant remains, however: the olive-green banner of Magus Kalugen, who rules this city. The steward is huddled beside a brazier, trying to draw what warmth he can from the glimmering coals. As you stride over, he looks up. \"The hour is late and I am cold,\" he says. Take this banner and let me go home.\"\n\nYou are just about to do so when another hand, velvet-gloved and bearing many white-gold rings, reaches out to snatch it.\n\n\"I shall be Kalugen's champion,\" states the newcomer. He appears to be an Enchanter. He turns to smile at you -- a very cold and sinister smile. \"I am Dominus Quel. And you -- you are dust if you try to oppose me.\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Oppose him", {Book::Type::Book1, 473}));
            Choices.push_back(Choice::Base("Let him take the pennant", {Book::Type::Book1, 164}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story120 : public Story::Base
    {
    public:
        Story120()
        {
            Book = Book::Type::Book1;

            Id = 120;

            Text = "After <i>recovering</i> for the next Spiral, he has two heads and four tails. Your coins now show two heads and one tail. You must both play 1, drawing.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 115}; }
    };

    class Story121 : public Story::Base
    {
    public:
        Story121()
        {
            Book = Book::Type::Book1;

            Id = 121;

            Text = "(TRICKSTER) You saunter casually past the merchant and, reaching deftly inside his jacket, remove his doubtless ill-gotten gains. He is so drunk that he does not notice a thing.\n\n<i>Inspecting your haul, you find that you now have the grand sum of fifty gold pieces.</i>";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            auto Character = Engine::Find(Party, Character::Class::Trickster);

            if (Character >= 0 && Character < Party.Members.size())
            {
                Engine::GainGold(Party.Members[Character], 50);
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 344}; }
    };

    class Story122 : public Story::Base
    {
    public:
        Story122()
        {
            Book = Book::Type::Book1;

            Id = 122;

            Text = "(ENCHANTER) You cast the spell. You seem to stare into a vast endless void. Does this mean you have no future? Certainly that is one possible future -- the dark oblivion of death. But there are other possibilities also.";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Try something", {Book::Type::Book1, 108}, Character::Class::Sage));
            Choices.push_back(Choice::Base("Heave the iron chain out of the water", {Book::Type::Book1, 502}));
            Choices.push_back(Choice::Base("Tell the gondolier to carry on", {Book::Type::Book1, 247}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story123 : public Story::Base
    {
    public:
        Story123()
        {
            Book = Book::Type::Book1;

            Id = 123;

            Text = "You stride up to the man, saying, \"Balhazar, you are discovered!\" He turns with a kind of half smile, then shimmers and fades away! You look around to see all the other guests disappearing as well. Only one remains -- a man in a domino cape. When he stands up, you recognise the imposing height and imperious stance of Magus Balhazar.\n\n\"You have failed my test, I fear,\" says Balhazar as he unmasks. \"Regrettable, as I had hoped you would do better. Now it is time for you to leave.\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 119}; }
    };

    class Story124 : public Story::Base
    {
    public:
        Story124()
        {
            Book = Book::Type::Book1;

            Id = 124;

            Choices.clear();
            Choices.push_back(Choice::Base("Add IMRAGARN to the party", {Book::Type::Book1, -124}));
            Choices.push_back(Choice::Base("Leave him be", {Book::Type::Book1, 369}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "The scarab burns with a bright green light. Suddenly the man groans and sits up. Having restored him to life, the scarab crumbles away to dust. The man looks around, astonished. \"What happened?\" he says. The last thing I remember is my old chum Fashmar getting killed by that Frost-Wizard\n\nYou quickly fill him in. It seems he's been frozen in ice for almost a decade, since he entered the Battlepits with several companions as the champions of Magus Laglor. His name is Imragarn, and he is naturally grateful that you have restored him to life. In fact, he swears allegiance to you.\n\n<b>NOTE</b>\n\nYou can add him to your party.\n\nIMRAGARN\nFighting Prowess: 8\nDamage per blow: 1D+1\nPyschic Ability: 6\nAwareness: 6\nEndurance: 12.\n\nHe has no weapon. He is a second-rank Warrior.";

            auto Character = Engine::First(Party, Item::Type::EmeraldScarab);

            if (Character >= 0 && Character < Party.Members.size())
            {
                Engine::Drop(Party.Members[Character], Item::Type::EmeraldScarab);
            }

            if (Engine::Count(Party, Code::Status::STRIPPED_IMRAGARN) == 0)
            {
                Text += " He has leather armour (Armour Rating 1).";
            }

            Text += " It is possible to <i>flee</i> from a combat without Imragarn, but if you do, any Warriors in the party must lose fifty experience points from their awards at the end of the adventure.";
        }
    };

    class Event124 : public Story::Base
    {
    public:
        Event124()
        {
            Book = Book::Type::Book1;

            Id = -124;

            DisplayId = 124;

            Choices.clear();

            Controls = Story::Controls::Info;
        }

        Book::Destination Background(Party::Base &Party)
        {
            auto Imragarn = Character::Create(Character::Class::Imragarn, Book::Type::Book1, 2);

            if (Engine::Count(Party, Code::Status::STRIPPED_IMRAGARN) == 0)
            {
                Imragarn.Equipment.push_back(Equipment::LeatherArmour);
            }

            Party.Members.push_back(Imragarn);

            return {Book::Type::Book1, 369};
        }
    };

    class Story125 : public Story::Base
    {
    public:
        Story125()
        {
            Book = Book::Type::Book1;

            Id = 125;

            Image = "images/book1/filler2.png";

            TopImage = true;

            Text = "Kief reveals his number: a 3.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            std::vector<int> Destinations = {130, 95, 135, 286, 286, 286};

            if (Party.LastValue >= 0 && Party.LastValue < 6)
            {
                return {Book::Type::Book1, Destinations[Party.LastValue]};
            }
            else
            {
                return {Book::Type::Book1, Destinations[Engine::Roll(1, 0) - 1]};
            }
        }
    };

    class Story126 : public Story::Base
    {
    public:
        Story126()
        {
            Book = Book::Type::Book1;

            Id = 126;

            Choices.clear();
            Choices.push_back(Choice::Base("Fight him", {Book::Type::Book1, 371}));
            Choices.push_back(Choice::Base("Turn and run", {Book::Type::Book1, 154}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "You rush on, blindly barging the doors open so as to get out of the hail of razor-pointed arrows. You now stand in a circular chamber filled with coruscating blue light. A desperate, insane laugh seems to waver at the edge of your hearing. Your skin crawls with a terror you cannot define.";

            if (Engine::HasArmour(Party))
            {
                Text += " The blue light is causing your armour to rot and slough away.";
            }

            Text += "\n\nYou are not alone in the room. An indistinct, shimmering figure is now beside you. It is the creature known in Krarthian mythology as the Prince of Desolation. He steps in front of you, leering like a madman.";
        }
    };

    class Story127 : public Story::Base
    {
    public:
        Story127()
        {
            Book = Book::Type::Book1;

            Id = 127;

            Text = "You slay the Assassins. They have three daggers that you can take if you want. Their Shuriken throwing-spikes are useless to you. You also find a vial of black liquid which a Sage could perhaps identify.";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) Identify the <b>vial of black liquid</b>", {Book::Type::Book1, 463}, Character::Class::Sage, Item::Type::VialOfBlackLiquid));
            Choices.push_back(Choice::Base("Continue", {Book::Type::Book1, 236}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party) { Equipment = {Equipment::Dagger, Equipment::Dagger, Equipment::Dagger, Item::VialOfBlackLiquid}; }
    };

    class Story128 : public Story::Base
    {
    public:
        int Character = -1;

        Story128()
        {
            Book = Book::Type::Book1;

            Id = 128;

            Text = "(ENCHANTER) You step out on to the bridge. As you do, the rumbling voice of the gargoyle's head echoes across the cave: \"Return from darkness.\" In answer to this summons, a figure appears at the far end of the bridge. The Hags mutter excitedly as he steps rapidly forwards to confront you. A wall of force arises behind you, cutting off your retreat. You must face this foe alone. As he draws near, you recognise the coat-of-arms on his shield: a three-horned dragon in gold on an azure field. It is the insignia of Sir Guillarme le Cauchemar, an evil knight who was your deadliest foe -- until you slew him, months ago...\n\nGuillarme speaks, his voice sounding cold and tinny within his black helm. \"We meet again for one final encounter, you spell-tossing churl. This time it is I, Guillarme le Cauchemar, who shall send you down into the icy embrace of Lady Death.\" He gives a curt, contemptuous salute and then doses for the kill.\n\n<b>NOTE</b>\n\nGuillarme's Endurance and Awareness are the same as yours were at the start of the adventure. His Fighting Prowess is one point greater than your initial Fighting Prowess, while his Psychic Ability is two points lower. He inflicts the same damage as you do with each blow, and his Armour Rating is three.\n\nYou cannot <i>flee</i>, and your companions (if any) cannot intervene in any way.";

            MapFile = "maps/book1/mapsolo.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Battle.SoloCombat = -1;

            Character = -1;

            if (Engine::IsPresent(Party, Character::Class::Enchanter))
            {
                Character = Engine::Find(Party, Character::Class::Enchanter);

                Battle.SoloCombat = Character;

                auto Characters = Party.Members.size();

                if (Engine::IsPresent(Party, Character::Class::Imragarn))
                {
                    Characters--;
                }

                auto Rank = 2;

                if (Characters >= 4)
                {
                    Rank = 2;
                }
                else if (Characters >= 3)
                {
                    Rank = 3;
                }
                else if (Characters >= 2)
                {
                    Rank = 4;
                }
                else if (Characters >= 1)
                {
                    Rank = 8;
                }

                auto Guillarme = Character::Create(Character::Class::Enchanter, Book::Type::Book1, Rank);

                auto FightingProwess = Engine::Score(Guillarme, Attributes::Type::FightingProwess) + 1;
                auto PsychicAbility = std::max(0, Engine::Score(Guillarme, Attributes::Type::PsychicAbility) - 2);
                auto Endurance = Engine::Score(Guillarme, Attributes::Type::Endurance);
                auto Awareness = Engine::Score(Guillarme, Attributes::Type::Awareness);

                Enemies.push_back(Enemy::Base(Enemy::Type::Guillarme, "GUILLARME", FightingProwess, PsychicAbility, Awareness, Endurance, Guillarme.Damage, Guillarme.DamageModifier, 3, Assets::Type::Knight, true, true, false));
            }
        }

        void SetupCombat(Map::Base &Map, Party::Base &Party)
        {
            if (Engine::IsPresent(Party, Character::Class::Enchanter))
            {
                Map.Put(2, 2, Map::Object::Player, Character);
            }
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            if (Result == Combat::Result::VICTORY)
            {
                if (Engine::IsPresent(Party, Character::Class::Enchanter))
                {
                    Engine::GetCode(Party.Members[Character], Code::Status::CROSSED_BRIDGE);
                }
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 88}; }
    };

    class Story129 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story129()
        {
            Book = Book::Type::Book1;

            Id = 129;

            Text = "Echidna slithers forwards across the marble floor to attack you.\n\n<b>NOTE</b>\n\nHer bite contains a strong poison, and anyone wounded by her rolls one Die: on a roll of 1 to 5 the poison does not flow into the wound, but on a roll of 6 the player must lose 3D Endurance.";

            MapFile = "maps/book1/map129.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Echidna, "ECHIDNA", 8, 9, 8, 40, 2, 2, 2, Assets::Type::Demoness, true, false, false));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            if (Result == Combat::Result::ESCAPED)
            {
                if (ExitTriggered == 0)
                {
                    Destination = {Book::Type::Book1, 357};
                }
                else
                {
                    Destination = {Book::Type::Book1, 56};
                }
            }
            else
            {
                Destination = {Book::Type::Book1, 253};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story130 : public Story::Base
    {
    public:
        Story130()
        {
            Book = Book::Type::Book1;

            Id = 130;

            Image = "images/book1/filler1.png";

            TopImage = false;

            Text = "After <i>recovering</i> for the next Spiral, he has two heads and five tails. You have only two heads. You are wondering whether you have any chance of winning when Kief says: \"No, you don't. We have reached the Converging Spiral, where playing on would only prolong your inevitable defeat.\" He gathers the coins.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 55}; }
    };

    class Story131 : public Story::Base
    {
    public:
        Story131()
        {
            Book = Book::Type::Book1;

            Id = 131;

            Text = "You brandish the talisman. The baleful lava-light causes it to sparkle like the heart of a flame. The Skiapyrs stop, wavering like guttering candles. \"The talisman,\" says one, its voice a soft susurration of air. \"We await your wishes.\"\n\n\"Slay the brigands that pursue us,\" you command. Instantly the Skiapyrs turn upon the Adventurers you fought a few moments ago. As they do, their fiery faces change from expressions of serene obedience to masks of hate. \"Slay the interlopers!\" they shriek, reaching out with white-hot claws. In moments, only charred bones and the stench of roasted flesh remain to show that the Adventurers ever existed.\n\nNo, not only that. A second glance tells you that the Adventurers dropped something -- something that gleams like burnished gold in the light of the clustering Skiapyrs. When you go to investigate, you find a gilded bridle, which you can take if you want. The Skiapyrs watch wistfully as you depart.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party) { Equipment = {Item::GildedBridle}; }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 223}; }
    };

    class Story132 : public Story::Base
    {
    public:
        Story132()
        {
            Book = Book::Type::Book1;

            Id = 132;

            Text = "You set to work improvising a raft from the sections of wooden door. Another lump of wood -- apparently the bolt that once held the doors shut -- becomes your oar. When you are ready, you gently ease the raft out on to the moat. Globules of molten stone cling to the sides and hiss furiously, but the wood does not burn. Yet.\n\nYou have to row slowly across the magma, careful not to splash deadly streams of white-hot magma across your feet. So intent are you on this that you almost fail to notice a deadly battle that is being enacted on the ridge encircling the craters. Two unshaven Adventurers have encountered a horde of Skiapyrs -- malicious flame-demons that dwell in the magma. Balanced precariously on the ridge, the Adventurers struggle desperately but without hope. One falls as a Skiapyr's fiery claws rake his chest, plunging into the moat where the magma burns the flesh and bones away in seconds. The other, disarmed by a treacherous blow from behind, turns, clutching his wounded arm. Seeing you, he calls out a greeting and then jumps, preferring to take his own life rather than let the Skiapyrs steal his vital energies for themselves.\n\nYour raft strikes the crater rim and you hastily scramble up towards the ridge. The Skiapyrs see you and give out unearthly screams of delight as they scamper along the ridge towards you. Behind you, more Skiapyrs are awakened by the cries of their fellows and now pursue you up the ridge.";

            Choices.clear();
            Choices.push_back(Choice::Base("Use an <b>ice jewel</b>", {Book::Type::Book1, 12}, Item::Type::BlueIceJewel));
            Choices.push_back(Choice::Base("You do not have or choose not to use an <b>ice jewel</b>", {Book::Type::Book1, 376}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story133 : public Story::Base
    {
    public:
        Story133()
        {
            Book = Book::Type::Book1;

            Id = 133;

            Text = "\"Reward?\" He turns an icy stare upon you. \"Yes, you shall have your reward. You shall have the honour of being swatted by Skrymir's hand, you mortal vermin!\"\n\n<b>NOTE</b>\n\nYou face a truly awesome opponent this time. It would take the luck of the gods to defeat him.";

            MapFile = "maps/book1/map133.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::SkrymirTheGiant, "SKRYMIR THE GIANT", 9, 9, 8, 70, 4, 0, 3, Assets::Type::Ogre, true, true, false));
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 432}; }
    };

    class Story134 : public Story::Base
    {
    public:
        Story134()
        {
            Book = Book::Type::Book1;

            Id = 134;

            Text = "\"I need consider no more champions,\" says Balhazar. \"Now it is time to equip you for the contest tomorrow.\" He hands each player in the party an <b>opal medallion</b>; Balhazar tells you that this will allow him to monitor your progress through the Battlepits, and possibly intervene with magic or advice when you are in trouble. He also gives the first player in the battle order a strange item -- an octagonal glass prism. He's not sure what it does, but thinks you will find a use for it. Then a servant shows you to bedchambers where you may get some rest.\n\nYou are up before the dawn, practising your combat techniques and preparing your mind for the ordeal to come. Balhazar and his guards take you out. The streets are full of bleary-eyed revellers -- many of them have been up all night -- jostling one another for a better view. You see the curtained litter of Magus Vyl, who obviously does not enjoy the dawn, accompanied by the three dark-robed Assassins who will champion him. Several other magi stand upon a dais in urgent consultation, making last-minute deals and alliances before the contest begins.\n\nYou are escorted across the cold tundra. Other groups are also filing away from the citadel. Among them you notice a group of bronze-armoured Barbarians employed by Magus Tor and the Master Warlock, Icon the Ungodly, who carries the pen- nant of Magus Uru. At last you reach a huge portal set into a hillside. Balhazar ushers you in, and you step through into the greatest adventure of your life ...";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            for (auto i = 0; i < Party.Members.size(); i++)
            {
                if (Engine::IsAlive(Party.Members[i]))
                {
                    Party.Members[i].Equipment.push_back(Item::OpalMedallion);
                }
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 201}; }
    };

    class Story135 : public Story::Base
    {
    public:
        Story135()
        {
            Book = Book::Type::Book1;

            Id = 135;

            Text = "You drew. After <i>recovering</i> for the next Spiral, this leaves him with five heads and two tails. You have no coins to recover from tails, so you still have four heads.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose your next number", {Book::Type::Book1, 140}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story136 : public Story::Base
    {
    public:
        Story136()
        {
            Book = Book::Type::Book1;

            Id = 136;

            Text = "You descend into the torchlit gloom. The sound of dripping water reaches your ears and soon you are standing in a damp stairwell, with a single bricked archway in front of you and a tunnel leading off behind it. You follow the tunnel for a few steps then find another tunnel leading off to the right. As you peer down it you fancy you can make out a motionless figure standing in the gloom. Ahead, you can see that the passage ends in a huge ornamental doorway.";

            Choices.clear();
            Choices.push_back(Choice::Base("Approach the figure down the right-hand tunnel", {Book::Type::Book1, 278}));
            Choices.push_back(Choice::Base("Carry on up the corridor to the ornamental doorway", {Book::Type::Book1, 246}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story137 : public Story::Base
    {
    public:
        Story137()
        {
            Book = Book::Type::Book1;

            Id = 137;

            Text = "You wrench the huge stone lid of the sarcophagus aside. A slight breeze across the cavern stirs the old bones and mouldering grave-clothes, swiftly reduc- ing them to ash. A small item gleams dully amid the decayed remnants of the mummy. It is a <b>bronze key</b>.";

            Choices.clear();
            Choices.push_back(Choice::Base("Open the first sarcophagus", {Book::Type::Book1, 351}));
            Choices.push_back(Choice::Base("... the second", {Book::Type::Book1, 186}));
            Choices.push_back(Choice::Base("... the third", {Book::Type::Book1, 403}));
            Choices.push_back(Choice::Base("You have had enough of grave-rifling. Proceed to the beach", {Book::Type::Book1, 293}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Equipment.clear();

            if (!Engine::HasItem(Party, Item::Type::BronzeKey))
            {
                Equipment = {Item::BronzeKey};
            }
        }
    };

    class Story138 : public Story::Base
    {
    public:
        Story138()
        {
            Book = Book::Type::Book1;

            Id = 138;

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "(SEIZED PLAYER) You stare in horror at the dead hand holding your arm. Suddenly you give a wail of utter despair and burst into night-black flames. Within seconds your body is consumed, shrivelled away like a leaf burnt in a bonfire.";

            if (Party.LastSelected >= 0 && Party.LastSelected < Party.Members.size())
            {
                Engine::Endurance(Party.Members[Party.LastSelected], 0);
            }

            if (Engine::Count(Party) > 0)
            {
                Text += " Your companions can only stare as you die. They rake the sand of the beach over your ashes and go on with their quest.";
            }

            Text += "\n\n<b>NOTE</b>\n\nAny items you possessed are destroyed along with you.";
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 447}; }
    };

    class Story139 : public Story::Base
    {
    public:
        Story139()
        {
            Book = Book::Type::Book1;

            Id = 139;

            Text = "(SAGE) This is a shrine to Vactris, the ravenous household god of Magus Tor. Possibly you recall the party Tor engaged as champions, as you saw them on your way from Balhazar's mansion.\n\n<i>Certainly the shrine will be charged with accursed demon-magic and you should not stay here any longer than you have to.</i>";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 476}; }
    };

    class Story140 : public Story::Base
    {
    public:
        Story140()
        {
            Book = Book::Type::Book1;

            Id = 140;

            Text = "His number is 3.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            std::vector<int> Destinations = {145, 50, 150, 286, 286, 286};

            if (Party.LastValue >= 0 && Party.LastValue < 6)
            {
                return {Book::Type::Book1, Destinations[Party.LastValue]};
            }
            else
            {
                return {Book::Type::Book1, Destinations[Engine::Roll(1, 0) - 1]};
            }
        }
    };

    class Story141 : public Story::Base
    {
    public:
        Story141()
        {
            Book = Book::Type::Book1;

            Id = 141;

            Text = "The Barbarians swagger off down the corridor, discussing all the ale they'll be able to buy with their loot once they have won the contest. You smile wryly, muttering under your breath that this is a clear case of counting one's chickens...";

            Choices.clear();
            Choices.push_back(Choice::Base("(SAGE) This might be a good time to try an ESP scan of the vicinity", {{Book::Type::Book1, 297}, {Book::Type::Book1, 447}}, Choice::Type::RandomDestination, Character::Class::Sage, Abilities::Type::ESP));
            Choices.push_back(Choice::Base("Dislodge the grille and drop down to follow the Barbarians", {Book::Type::Book1, 477}));
            Choices.push_back(Choice::Base("Wait where you are, watching through the grille", {Book::Type::Book1, 73}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story142 : public Story::Base
    {
    public:
        Story142()
        {
            Book = Book::Type::Book1;

            Id = 142;

            Image = "images/book1/bridge.png";

            TopImage = true;

            Text = "The platform is a wide shelf of rock jutting out from the cave wall. Across the deep gorge you jeer at the ugly Hags, now incensed that you have defeated their 'pets'. The only way to get across the gorge is via one of two bridges that span the distance to the temple terrace. A waterfall cascades over the middle of the platform, bisecting it and cutting you off from the further of the two bridges. Looking up, you see a huge gargoyle head carved into the cave wall near the roof. The water issues from its mouth, cascades down in a torrent across the middle of the platform, then pours down into the swirling river far beneath you.\n\nAs you consider the two bridges, a booming voice makes you look up once more. The mouth of the gargoyle is moving, and by listening hard you can make sense out of the deep rumbling words. Over and over, it intones: \"Face that which you fear most, or confront a lesser foe.\"\n\nPresumably it is referring to the two bridges. You could cross the nearer bridge with no apparent trouble, but to get to the further one you must step through the waterfall. The gargoyle's words are weighty with ambiguity. Which to choose?\n\n<b>NOTE</b>\n\nAll players must cross. This must be done one at a time. The order in which they cross is up to them...";

            Choices.clear();
            Choices.push_back(Choice::Base("(WARRIOR)", {Book::Type::Book1, 478}, Character::Class::Warrior));
            Choices.push_back(Choice::Base("(TRICKSTER)", {Book::Type::Book1, 113}, Character::Class::Trickster));
            Choices.push_back(Choice::Base("(SAGE)", {Book::Type::Book1, 426}, Character::Class::Sage));
            Choices.push_back(Choice::Base("(ENCHANTER)", {Book::Type::Book1, 252}, Character::Class::Enchanter));

            Controls = Story::Controls::Standard;
        }
    };

    class Story143 : public Story::Base
    {
    public:
        Story143()
        {
            Book = Book::Type::Book1;

            Id = 143;

            Text = "(TRICKSTER) You feel a faint tug at your belt, and instantly realise that unseen fingers are trying to steal your possessions! \"Set a thief to catch a thief,\" you whisper to yourself, suddenly whirling around under the waterfall and fending the invisible hands away. One of them clutches something -- probably filched from another Adventurer who passed this way -- and you grab this as you retreat out of the water. You examine the item you have gained.\n\n<i>It is a phial of chimera spittle -- a deadly, delayed-action poison.</i>";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party) { Engine::Gain(Party, Character::Class::Trickster, {Item::ChimeraSpittle}); }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 267}; }
    };

    // TODO: this section describes an item effect that will be implemented in
    // the CombatScreen/StoryScreen interface. It is preserved here for completeness.
    class Story144 : public Story::Base
    {
    public:
        Story144()
        {
            Book = Book::Type::Book1;

            Id = 144;

            Type = Story::Type::Info;

            Text = "Did you also drink the <b>effervescent potion</b>? If so, you are all right because that was the antidote to the <b>chimera spittle</b> you imbibed. If you did not drink the <b>effervescent potion</b>, you die in terrible agony.";

            Choices.clear();

            Controls = Story::Controls::Info;
        }
    };

    class Story145 : public Story::Base
    {
    public:
        Story145()
        {
            Book = Book::Type::Book1;

            Id = 145;

            Text = "He reaches out to sweep away the coins. \"You have already lost,\" he explains. \"The next Spiral you can only choose a 1 and, by putting a 2, I can force you to lose one of your two remaining coins. Once that happens, you cannot go. So we need not prolong the conflict...\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 55}; }
    };

    class Story146 : public Story::Base
    {
    public:
        Story146()
        {
            Book = Book::Type::Book1;

            Id = 146;

            Text = "You slip the great gauntlet on to the skeleton's right hand as the skin starts to draw across it. Moments later, the body begins to throb with life and the pink bloom of blood washes through its veins as the stone heart reverts to flesh. Slabs of muscle swell across the bones, sewn with gristle and skin. Skrymir rises from the dead.\n\nHe stands before you, towering towards the cave roof like the shadow of a great glacier. His beard sparkles with icicles; his eyes blaze with cold fury.\n\n\"Skrymir stands upon Middle-Earth once more!\" he cries, shaking the rock walls with his voice of thunder. \"Legend reverberates to his battle-roar. Let the magi who crouch upon the old thrones of Krarth beware -- they shall not see another dawn, for the sky then shall be washed with their blood!\"\n\nHe turns and glares at the flickering beam of the Teleportation spell. He obviously intends to return to the surface, to the Great Hall where the magi are waiting for a champion to emerge.";

            Choices.clear();
            Choices.push_back(Choice::Base("Point out that you have just resurrected him", {Book::Type::Book1, 539}));
            Choices.push_back(Choice::Base("Keep quiet and let him depart", {Book::Type::Book1, 257}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story147 : public Story::Base
    {
    public:
        Story147()
        {
            Book = Book::Type::Book1;

            Id = 147;

            Text = "TRICKSTER) \"Why, this is nonsense!\" you cry, quickly turning on your charm. \"How can you describe yourself as soulless, you who are more lovely than any earthly woman? Regard your exquisite reflection in yonder fountain. Tell me whether it is not the face of a vibrant elemental persona, a passionate beauty who laughs in the face of the spiteful Fates!\" Larisha looks startled for a moment, then turns to regard herself in the crystal-clear water of the fountain. \"Why, you speak true,\" she murmurs. She uses her real voice now, not her power of telepathy, and her tones are so chillingly cold that you could well believe she has no soul \"Let us spite the Fates then -- those hideous crones! I'll give you another item as well, not because I must but because I choose to do so...\"\n\n<b>NOTE</b>\n\nYou now have two of the following: the <b>Dagger of Vislet</b>, the <b>Golden Snuff-Box</b> and the <b>Dragonlord Gem</b>.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Engine::Gain(Party, Character::Class::Trickster, {Item::DaggerOfVislet, Item::DaggerOfVislet, Item::GoldenSnuffBox, Item::GoldenSnuffBox, Item::DragonlordGem, Item::DragonlordGem});
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 203}; }
    };

    class Story148 : public Story::Base
    {
    public:
        Story148()
        {
            Book = Book::Type::Book1;

            Id = 148;

            Text = "Your reputation for daring adventure is not based on stupidity. You quickly back away from the room beyond the gate, then turn and scramble up the stairs towards the top of the Tower.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 3}; }
    };

    class Story149 : public Story::Base
    {
    public:
        Story149()
        {
            Book = Book::Type::Book1;

            Id = 149;

            Text = "Although he is a powerful and knowledgeable wizard, Balhazar is not used to using his magic in the thick of battle. He hesitates for a fatal split second too long, giving the Assassin time to throw his dagger. It spins in the air and impales Balhazar in the arm. Shocked, he pulls it free. You see that the dagger glistens with a coating of poison. Balhazar staggers and falls.\n\nYou turn to deal with the Assassin. To your amazement he has scaled a long drape and now stands at a window on the balcony above. You make for the stairs, but he hurls three spiked <i>sha-ken</i> in quick succession to delay you. Then he is gone into the night.\n\nYou go over to Balhazar. \"My magic will not cure this poison,\" he says weakly. \"The Assassin must have been sent by my rival, Magus Vyl. He employs an entire network of the scum.\" Balhazar coughs, obviously near death. \"My death deprives you of an employer. I hear that only Kalugen is still looking for champions, so you must return and take his pennant. I can perform but one service for you -- open the cabinet yonder and take what you find within...\"\n\nHe dies.";

            Choices.clear();
            Choices.push_back(Choice::Base("Open the cabinet", {Book::Type::Book1, 196}));
            Choices.push_back(Choice::Base("Return to the main square at once", {Book::Type::Book1, 119}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story150 : public Story::Base
    {
    public:
        Story150()
        {
            Book = Book::Type::Book1;

            Id = 150;

            Text = "After <i>recovering</i> for the next Spiral, You have only four coins left, but at least they are all heads. Behind his thin hand, Kief sets the Die to his chosen number.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose your next number", {Book::Type::Book1, 155}, Choice::Type::SelectDice, "Choose a number"));

            Controls = Story::Controls::Standard;
        }
    };

    class Story151 : public Story::Base
    {
    public:
        Story151()
        {
            Book = Book::Type::Book1;

            Id = 151;

            Bye = "You have no other way to go, so you make your way back to the ornamental doorway at the end of the main corridor.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "The passage ends in a heavy door. Grasping the iron ring in the middle, you haul it open and step into a small room. There are no other exits. On a table of red granite you see a steel sceptre that seems to glimmer with magical energy. Taking this, you see an indicator on the side that shows it has four charges. ";

            Equipment = {Item::SteelSceptre};

            if (!Engine::IsPresent(Party, Character::Class::Sage))
            {
                Text += "You used up 1 charge to test it and see what it does.";

                Engine::Discharge(Equipment[0], 1);
            }
            else
            {
                Text += " The SAGE recognizes this device from various historical documents.";
            }

            Text += "\n\n<b>NOTE</b>\n\nEach charge of the device releases a blast of destructive energy. You can use it in combat in place of the <i>Fight</i> option. It affects one monster in an adjacent square to the user, inflicting 5D damage. The blast of the sceptre always hits. Each time it is used in combat or non-combat situations, 1 charge is expended. When it reaches zero charges it ceases to function.";
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 246}; }
    };

    class Story152 : public Story::Base
    {
    public:
        Story152()
        {
            Book = Book::Type::Book1;

            Id = 152;

            Text = "A tunnel of worked stone leads off ahead of you from the jetty. It soon opens out into a small circular room with two exits. One is directly ahead of you: a rough rectangular opening in the stonework barred by a heavy iron grille with an even heavier iron chain and padlock locking it. The other exit leads off to your right, where you can see an archway and a corridor lit with torches in brackets. The corridor leads to a door. There are deep alcoves all along the corridor on either side.";

            Choices.clear();
            Choices.push_back(Choice::Base("Use a <b>bronze key</b>", {Book::Type::Book1, 221}, Item::Type::BronzeKey));
            Choices.push_back(Choice::Base("Use 1 charge of the <b>steel sceptre</b>", {Book::Type::Book1, 296}, Choice::Type::Discharge, Item::Type::SteelSceptre, 1));
            Choices.push_back(Choice::Base("You do not have any of these, or choose not to use them", {Book::Type::Book1, 316}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story153 : public Story::Base
    {
    public:
        Story153()
        {
            Book = Book::Type::Book1;

            Id = 153;

            Text = "(SAGE) You know the proper words of the prayer to this goddess. You speak them now, and the goddess responds by suffusing your spirit with the energy to do great good.\n\n<b>NOTE</b>\n\nIf you wish to use your power to Heal, double the amount of restored Endurance points. You can get this benefit only if you do the Healing right now, and it is a one-off benefit.";

            HealRate = 2;

            Choices.clear();
            Choices.push_back(Choice::Base("Have a look at the ice block", {Book::Type::Book1, 334}));
            Choices.push_back(Choice::Base("Dig up one of the mounds", {Book::Type::Book1, 42}));
            Choices.push_back(Choice::Base("Continue on your way", {Book::Type::Book1, 279}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story154 : public Story::Base
    {
    public:
        Story154()
        {
            Book = Book::Type::Book1;

            Id = 154;

            Choices.clear();
            Choices.push_back(Choice::Base("Run up the steps to the balcony", {Book::Type::Book1, 82}));
            Choices.push_back(Choice::Base("Return the way you came in", {Book::Type::Book1, 48}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "Arrows cut through the air as you race the length of the room. Three arrows strike home.\n";

            auto PartySize = Engine::Count(Party);

            if (PartySize > 0)
            {
                for (auto i = 0; i < 3; i++)
                {
                    auto Target = Engine::Find(Party, Engine::Roll(1, 0) % PartySize + 1);

                    if (Target >= 0 && Target < Party.Members.size())
                    {
                        auto Damage = Engine::Damage(Party.Members[Target], Engine::Roll(1, 0));

                        if (Damage > 0)
                        {
                            Text += "\n" + std::string(Character::ClassName[Party.Members[Target].Class]) + " dealt " + std::to_string(Damage) + " damage!";

                            if (!Engine::IsAlive(Party.Members[Target]))
                            {
                                Text += " " + std::string(Character::ClassName[Party.Members[Target].Class]) + " was killed!";
                            }
                        }
                    }
                }
            }

            if (Engine::IsAlive(Party))
            {
                Text += "\n\nThe chessmen provide you with momentary cover from which to consider your plan of action.";
            }
        }
    };

    class Story155 : public Story::Base
    {
    public:
        Story155()
        {
            Book = Book::Type::Book1;

            Id = 155;

            Text = "Kief's number this time is 3.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            std::vector<int> Destinations = {160, 165, 170, 286, 286, 286};

            if (Party.LastValue >= 0 && Party.LastValue < 6)
            {
                return {Book::Type::Book1, Destinations[Party.LastValue]};
            }
            else
            {
                return {Book::Type::Book1, Destinations[Engine::Roll(1, 0) - 1]};
            }
        }
    };

    class Story156 : public Story::Base
    {
    public:
        Book::Destination Destination = {};

        Story156()
        {
            Book = Book::Type::Book1;

            Id = 156;

            MapFile = "maps/book1/map156.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Bye = "";

            Text = "You come to a door and open it on to a scene of carnage. Two Barbarians are just putting an end to a blood-splattered, black-robed Assassin. Two other black-robed figures lie dead on the floor. The Assassins presumably served another magus, but they don't seem to have been a match for the Barbarians. Just as you burst into the room, the last of them, reacting instinctively, throws a Shuriken at you.";

            auto Target = Engine::Find(Party, 1);

            if (Target >= 0 && Target < Party.Members.size())
            {
                auto Damage = Engine::Damage(Party.Members[Target], Engine::Roll(1, -1));

                if (Damage > 0)
                {
                    Text += "\n\n" + std::string(Character::ClassName[Party.Members[Target].Class]) + " dealt " + std::to_string(Damage) + " damage!";

                    if (!Engine::IsAlive(Party.Members[Target]))
                    {
                        Text += " " + std::string(Character::ClassName[Party.Members[Target].Class]) + " was killed!";
                    }
                }
            }

            Text += "\n\nJust as he does so, a bone-shattering axe-blow levels him to the ground, and the Barbarians turn around to deal with you. You see the berserk fanaticism in their eyes and know they cannot be reasoned with. You also know that their two comrades will probably be racing down the corridor behind you even at this moment...\n\n<b>NOTE</b>\n\nYou cannot flee at first because you know the other two Barbarians must be somewhere behind you.";

            Battle.SurprisedEnemy = false;

            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 1", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian, true, false, false));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 2", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian, true, false, false));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 3", 8, 5, 7, 11, 1, 2, 1, Assets::Type::Barbarian, true, false, false, 6, 1, 3));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 4", 8, 5, 7, 11, 1, 2, 1, Assets::Type::Barbarian, true, false, false, 6, 1, 4));
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            Choices.clear();

            if (Result == Combat::Result::ESCAPED)
            {
                Bye = "You went back taking the other corridor.";

                Destination = {Book::Type::Book1, 354};
            }
            else
            {
                Destination = {Book::Type::Book1, 32};
            }
        }

        Book::Destination Continue(Party::Base &Party) { return Destination; }
    };

    class Story157 : public Story::Base
    {
    public:
        Story157()
        {
            Book = Book::Type::Book1;

            Id = 157;

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Text = "(SAGE) You step out on to the bridge and begin to make your way along it. You look down to see a surging river of miasma gushing along the bottom of the gorge a hundred metres below. Just as you reach the mid-point of the bridge, the gargoyle's head speaks again. \"Be mortal again,\" it says.\n\nAs if in answer, a dark shape begins to form on the bridge in front of you. Within moments, it is a thin pale woman who stands there. She sweeps forwards, drawing a sharp steel sword from under her torn black robes. Her eyes glimmer with recognition at the same instant that yours do. She is Nemesis, whose mother was a demon and whose father was a mortal man. She was your bitterest foe -- until the day you took her life, six years ago.\n\n\"Life and death are not immutable,\" groans the gargoyle. \"Fight to decide who shall live.\"\n\nNemesis gives a slight bow, which turns into a forward roll as she closes to strike. He sword slashes your arm, inflicting four Endurance points damage (less Armour Rating).\n";

            Choices.clear();

            if (Engine::IsPresent(Party, Character::Class::Sage))
            {
                auto Target = Engine::Find(Party, Character::Class::Sage);

                auto Damage = Engine::Damage(Party.Members[Target], 4);

                Text += "\n" + std::string(Character::ClassName[Party.Members[Target].Class]) + " dealt " + std::to_string(Damage) + " damage!";

                if (!Engine::IsAlive(Party.Members[Target]))
                {
                    Text += " " + std::string(Character::ClassName[Party.Members[Target].Class]) + " was killed!";
                }
                else
                {
                    Text += "\n\nYou lose no time in counter-attacking.\n\n<b>NOTE</b>\n\nShe has the same Fighting Prowess, Awareness, Endurance and Psychic Ability as you started the adventure with. She inflicts one more point of damage than you do each time she hits, but she has no armour.\n\nYou cannot retreat, as an energy-barrier now blocks the bridge behind you. Your companions, if any, cannot help you by any means. The only way you could get out of this battle would be to try Levitating yourself off the bridge.";

                    Choices.push_back(Choice::Base("(SAGE) Use Levitation", {Book::Type::Book1, 282}, Abilities::Type::Levitation));
                    Choices.push_back(Choice::Base("Fight Nemesis", {Book::Type::Book1, -157}));
                }
            }
            else
            {
                Choices.push_back(Choice::Base("Let others cross the bridge", {Book::Type::Book1, 88}));
            }
        }
    };

    class Event157 : public Story::Base
    {
    public:
        int Character = -1;

        Event157()
        {
            Book = Book::Type::Book1;

            Id = -157;

            DisplayId = 157;

            MapFile = "maps/book1/mapsolo.json";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Enemies.clear();

            Battle.SoloCombat = -1;

            Character = -1;

            if (Engine::IsPresent(Party, Character::Class::Sage))
            {
                Character = Engine::Find(Party, Character::Class::Sage);

                Battle.SoloCombat = Character;

                auto Characters = Party.Members.size();

                if (Engine::IsPresent(Party, Character::Class::Imragarn))
                {
                    Characters--;
                }

                auto Rank = 2;

                if (Characters >= 4)
                {
                    Rank = 2;
                }
                else if (Characters >= 3)
                {
                    Rank = 3;
                }
                else if (Characters >= 2)
                {
                    Rank = 4;
                }
                else if (Characters >= 1)
                {
                    Rank = 8;
                }

                auto Nemesis = Character::Create(Character::Class::Sage, Book::Type::Book1, Rank);

                auto FightingProwess = Engine::Score(Nemesis, Attributes::Type::FightingProwess);
                auto PsychicAbility = Engine::Score(Nemesis, Attributes::Type::PsychicAbility);
                auto Endurance = Engine::Score(Nemesis, Attributes::Type::Endurance);
                auto Awareness = Engine::Score(Nemesis, Attributes::Type::Awareness);

                Enemies.push_back(Enemy::Base(Enemy::Type::Nemesis, "NEMESIS", FightingProwess, PsychicAbility, Awareness, Endurance, Nemesis.Damage, Nemesis.DamageModifier + 1, 0, Assets::Type::Gargoyle, true, false, false));
            }
        }

        void SetupCombat(Map::Base &Map, Party::Base &Party)
        {
            if (Engine::IsPresent(Party, Character::Class::Sage))
            {
                Map.Put(2, 2, Map::Object::Player, Character);
            }
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            if (Result == Combat::Result::VICTORY)
            {
                if (Engine::IsPresent(Party, Character::Class::Sage))
                {
                    Engine::GetCode(Party.Members[Character], Code::Status::CROSSED_BRIDGE);
                }
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 88}; }
    };

    class Story158 : public Story::Base
    {
    public:
        Story158()
        {
            Book = Book::Type::Book1;

            Id = 158;

            Text = "As they behold the talisman, its burnished surface glowing like the sun in the reflected light from the lava pit, the Skiapyrs waver in awe. \"It is the sacred talisman,\" whispers one. \"Pass by in peace.\"\n\nYou hurry on before they can change their minds.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 223}; }
    };

    class Story159 : public Story::Base
    {
    public:
        Story159()
        {
            Book = Book::Type::Book1;

            Id = 159;

            Text = "You pass on up the tunnel until you reach another chamber. You must be half way to the summit by now. Another tunnel leads up from the far side of the room, but to reach it you must pass a roughly carved throne of rock to which a giant skeletal pelvis and legs are shackled.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party)
        {
            if (Engine::HasItem(Party, Item::Type::GiantsSkull))
            {
                return {Book::Type::Book1, 433};
            }
            else
            {
                return {Book::Type::Book1, 538};
            }
        }
    };

    class Story160 : public Story::Base
    {
    public:
        Story160()
        {
            Book = Book::Type::Book1;

            Id = 160;

            Text = "Kief shakes his head and mutters something, then scoops up the remaining coins. \"Well,\" he says as you give a grunt of surprise, \"surely you could see that you'd lose?\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 55}; }
    };

    class Story161 : public Story::Base
    {
    public:
        Story161()
        {
            Book = Book::Type::Book1;

            Id = 161;

            Text = "A harsh command issues from the skull, sounding like the tolling of a death-knell. The padlock opens and the <b>rib-cage</b> falls at the base of the petrified tree.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Equipment.clear();

            Limit = 0;

            if (Engine::HasItem(Party, Item::Type::FossilizedHeart))
            {
                auto Character = Engine::HasSpace(Party) ? Engine::FirstSpace(Party) : Engine::First(Party);

                Party.Members[Character].Equipment.push_back(Item::RibCage);
            }
            else
            {
                Equipment = {Item::RibCage};

                Limit = 1;
            }
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 159}; }
    };

    // TODO: pool money
    class Story162 : public Story::Base
    {
    public:
        Story162()
        {
            Book = Book::Type::Book1;

            Id = 162;

            Text = "\"Yes,\" says Kalugen, nodding. \"That's a good game. First of all we must decide how much we're going to stake. You first...\"\n\n<b>NOTE</b>\n\nOnly one person will actually be playing against Kalugen on behalf of the whole party. The others can give some or all of their money if they wish. Decide how much money you are going to stake. Even if you still have more money on you, you cannot use it if you do not declare it now as part of your stake.";

            Choices.clear();
            Choices.push_back(Choice::Base("Decide the total amount of money you are going to use to play the game", {Book::Type::Book1, 189}, Choice::Type::PoolMoney));

            Controls = Story::Controls::Standard;
        }
    };

    // TODO: cast spell. remove from spells called to mind.
    class Story163 : public Story::Base
    {
    public:
        Story163()
        {
            Book = Book::Type::Book1;

            Id = 163;

            Text = "(ENCHANTER) You easily weave the necessary enchantment. Images pour in, showing you parts of several possible futures.\n\n<i>You start back as you see yourself in this very chamber, locked in mortal combat with a ravening vampire-lord! The image disintegrates to show another possibility -- this time you are entering the room at the top of the Tower. An oppressive feeling of danger weighs upon you...</i>\n\nThe spell fades, returning your spirit to the present. What do you wish to do now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Continue searching the altar", {Book::Type::Book1, 444}));
            Choices.push_back(Choice::Base("You would rather leave to go on up the stairs", {Book::Type::Book1, 3}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story164 : public Story::Base
    {
    public:
        Story164()
        {
            Book = Book::Type::Book1;

            Id = 164;

            Type = Story::Type::Doom;

            Text = "Dominus Quel becomes Kalugen's champion on the morrow. As for you, there is nothing you can do but head south for other adventures. Perhaps you will return another year to take part in the contest of the Battlepits.";

            Choices.clear();

            Controls = Story::Controls::Exit;
        }
    };

    class Story165 : public Story::Base
    {
    public:
        Story165()
        {
            Book = Book::Type::Book1;

            Id = 165;

            Text = "Kief spreads his hands in a smug gesture. \"There you are,\" he says. \"As usual, I have manoeuvred my opponent into a no-win situation. It would be meaningless to play on from this point...\"";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 55}; }
    };

    class Story166 : public Story::Base
    {
    public:
        Story166()
        {
            Book = Book::Type::Book1;

            Id = 166;

            Text = "(TRICKSTER) \"Five now, five then,\" you tell him. You produce ten large gold coins and put five of them in his hand. You still keep your own hand closed around them.\n\nHe looks eagerly at the five coins in your other hand. \"The scarlet banner is the symbol of Magus Balhazar, who is noted for an almost human compassion at times. This quality sets him apart from the other magi, who have hearts of cold rock.\"\n\n\"Thanks for the advice,\" you say with a smile. You whip the first five coins out of his hand and give him the other five instead. \"Five before, and five after.\"\n\n\"You scurvy scum! You sly wretch! You pustule of gangrenous venom!\" he screams. \"Well, I lied, see. The scarlet banner's the worst one to take, in fact.\" Flushed with anger, he slopes back to where the other two stewards sit chortling gleefully.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Book::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 332}; }
    };

    class Story167 : public Story::Base
    {
    public:
        Story167()
        {
            Book = Book::Type::Book1;

            Id = 167;

            Text = "The gondolier speaks from behind his mournful mask. \"The fee is forty gold coins,\" he says, his voice thickly accented.";

            Choices.clear();
            Choices.push_back(Choice::Base("You are willing to pay what he demands", {Book::Type::Book1, 193}, Choice::Type::LoseMoney, "Who pays for the gondolier's fee (40 gold pieces)", 40));
            Choices.push_back(Choice::Base("You cannot or will not pay him forty gold pieces", {Book::Type::Book1, 516}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story398 : public Story::Base
    {
    public:
        Story398()
        {
            Book = Book::Type::Book1;

            Id = 398;

            Text = "(ENCHANTER) You consider your various spells. Two non-combat spells might be of use to you now. But which? Make your choice";

            Choices.clear();
            Choices.push_back(Choice::Base("Cast the Prediction spell", {Book::Type::Book1, 442}));
            Choices.push_back(Choice::Base("Opt for the Summon Faltyn spell", {Book::Type::Book1, 242}));

            Controls = Story::Controls::Standard;
        }
    };

    class Story452 : public Story::Base
    {
    public:
        Story452()
        {
            Book = Book::Type::Book1;

            Id = 452;

            Text = "You saunter up to the booth. The three pennants flutter forlornly in the evening breeze. You return the cold stares of the magi's stewards with a scowl.";

            Choices.clear();
            Choices.push_back(Choice::Base("Take the olive-green pennant", {Book::Type::Book1, 52}));
            Choices.push_back(Choice::Base("Take the scarlet pennant", {Book::Type::Book1, 28}));
            Choices.push_back(Choice::Base("Select the black and purple pennant", {Book::Type::Book1, 381}));
            Choices.push_back(Choice::Base("Try talking to the stewards first", {Book::Type::Book1, 227}));

            Controls = Story::Controls::Standard;
        }
    };

    auto story001 = Story001();
    auto story002 = Story002();
    auto story003 = Story003();
    auto story004 = Story004();
    auto story005 = Story005();
    auto story006 = Story006();
    auto story007 = Story007();
    auto story008 = Story008();
    auto story009 = Story009();
    auto story010 = Story010();
    auto story011 = Story011();
    auto story012 = Story012();
    auto story013 = Story013();
    auto story014 = Story014();
    auto story015 = Story015();
    auto event015 = Event015();
    auto story016 = Story016();
    auto story017 = Story017();
    auto story018 = Story018();
    auto story019 = Story019();
    auto story020 = Story020();
    auto story021 = Story021();
    auto story022 = Story022();
    auto story023 = Story023();
    auto story024 = Story024();
    auto story025 = Story025();
    auto story026 = Story026();
    auto story027 = Story027();
    auto story028 = Story028();
    auto story029 = Story029();
    auto story030 = Story030();
    auto story031 = Story031();
    auto story032 = Story032();
    auto story033 = Story033();
    auto story034 = Story034();
    auto story035 = Story035();
    auto story036 = Story036();
    auto story037 = Story037();
    auto story038 = Story038();
    auto event038 = Event038();
    auto story039 = Story039();
    auto story040 = Story040();
    auto story041 = Story041();
    auto story042 = Story042();
    auto story043 = Story043();
    auto story044 = Story044();
    auto story045 = Story045();
    auto story046 = Story046();
    auto story047 = Story047();
    auto story048 = Story048();
    auto story049 = Story049();
    auto story050 = Story050();
    auto story051 = Story051();
    auto story052 = Story052();
    auto story053 = Story053();
    auto story054 = Story054();
    auto story055 = Story055();
    auto story056 = Story056();
    auto story057 = Story057();
    auto story058 = Story058();
    auto story059 = Story059();
    auto story060 = Story060();
    auto story061 = Story061();
    auto story062 = Story062();
    auto event062 = Event062();
    auto story063 = Story063();
    auto story064 = Story064();
    auto story065 = Story065();
    auto story066 = Story066();
    auto story067 = Story067();
    auto story068 = Story068();
    auto story069 = Story069();
    auto story070 = Story070();
    auto story071 = Story071();
    auto story072 = Story072();
    auto story073 = Story073();
    auto story074 = Story074();
    auto story075 = Story075();
    auto story076 = Story076();
    auto event076 = Event076();
    auto story077 = Story077();
    auto story078 = Story078();
    auto story079 = Story079();
    auto story080 = Story080();
    auto story081 = Story081();
    auto story082 = Story082();
    auto story083 = Story083();
    auto story084 = Story084();
    auto story085 = Story085();
    auto story086 = Story086();
    auto story087 = Story087();
    auto story088 = Story088();
    auto story089 = Story089();
    auto story090 = Story090();
    auto story091 = Story091();
    auto story092 = Story092();
    auto story093 = Story093();
    auto story094 = Story094();
    auto story095 = Story095();
    auto story096 = Story096();
    auto story097 = Story097();
    auto story098 = Story098();
    auto story099 = Story099();
    auto story100 = Story100();
    auto story101 = Story101();
    auto story102 = Story102();
    auto story103 = Story103();
    auto story104 = Story104();
    auto story105 = Story105();
    auto story106 = Story106();
    auto story107 = Story107();
    auto story108 = Story108();
    auto story109 = Story109();
    auto story110 = Story110();
    auto story111 = Story111();
    auto story112 = Story112();
    auto story113 = Story113();
    auto story114 = Story114();
    auto event114 = Event114();
    auto story115 = Story115();
    auto story116 = Story116();
    auto story117 = Story117();
    auto story118 = Story118();
    auto story119 = Story119();
    auto story120 = Story120();
    auto story121 = Story121();
    auto story122 = Story122();
    auto story123 = Story123();
    auto story124 = Story124();
    auto event124 = Event124();
    auto story125 = Story125();
    auto story126 = Story126();
    auto story127 = Story127();
    auto story128 = Story128();
    auto story129 = Story129();
    auto story130 = Story130();
    auto story131 = Story131();
    auto story132 = Story132();
    auto story133 = Story133();
    auto story134 = Story134();
    auto story135 = Story135();
    auto story136 = Story136();
    auto story137 = Story137();
    auto story138 = Story138();
    auto story139 = Story139();
    auto story140 = Story140();
    auto story141 = Story141();
    auto story142 = Story142();
    auto story143 = Story143();
    auto story144 = Story144();
    auto story145 = Story145();
    auto story146 = Story146();
    auto story147 = Story147();
    auto story148 = Story148();
    auto story149 = Story149();
    auto story150 = Story150();
    auto story151 = Story151();
    auto story152 = Story152();
    auto story153 = Story153();
    auto story154 = Story154();
    auto story155 = Story155();
    auto story156 = Story156();
    auto story157 = Story157();
    auto event157 = Event157();
    auto story158 = Story158();
    auto story159 = Story159();
    auto story160 = Story160();
    auto story161 = Story161();
    auto story162 = Story162();
    auto story163 = Story163();
    auto story164 = Story164();
    auto story165 = Story165();
    auto story166 = Story166();
    auto story167 = Story167();
    auto story398 = Story398();
    auto story452 = Story452();

    void InitializeStories()
    {
        Book1::Stories = {
            &event015, &event038, &event062, &event076, &event114, &event124, &event157,
            &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009, &story010,
            &story011, &story012, &story013, &story014, &story015, &story016, &story017, &story018, &story019, &story020,
            &story021, &story022, &story023, &story024, &story025, &story026, &story027, &story028, &story029, &story030,
            &story031, &story032, &story033, &story034, &story035, &story036, &story037, &story038, &story039, &story040,
            &story041, &story042, &story043, &story044, &story045, &story046, &story047, &story048, &story049, &story050,
            &story051, &story052, &story053, &story054, &story055, &story056, &story057, &story058, &story059, &story060,
            &story061, &story062, &story063, &story064, &story065, &story066, &story067, &story068, &story069, &story070,
            &story071, &story072, &story073, &story074, &story075, &story076, &story077, &story078, &story079, &story080,
            &story081, &story082, &story083, &story084, &story085, &story086, &story087, &story088, &story089, &story090,
            &story091, &story092, &story093, &story094, &story095, &story096, &story097, &story098, &story099, &story100,
            &story101, &story102, &story103, &story104, &story105, &story106, &story107, &story108, &story109, &story110,
            &story111, &story112, &story113, &story114, &story115, &story116, &story117, &story118, &story119, &story120,
            &story121, &story122, &story123, &story124, &story125, &story126, &story127, &story128, &story129, &story130,
            &story131, &story132, &story133, &story134, &story135, &story136, &story137, &story138, &story139, &story140,
            &story141, &story142, &story143, &story144, &story145, &story146, &story147, &story148, &story149, &story150,
            &story151, &story152, &story153, &story154, &story155, &story156, &story157, &story158, &story159, &story160,
            &story161, &story162, &story164, &story164, &story165, &story166, &story167,
            &story398,
            &story452};
    }
}
#endif
