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

        void Event(Party::Base &Party)
        {
            Equipment = {Equipment::RubyRing};

            Limit = 1;
        }
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

            // TODO: verify which sword to discard
            Choices.clear();
            Choices.push_back(Choice::Base("Discard the <b>sword</b>", {Book::Type::Book1, 503}, Choice::Type::DropWeapon, Equipment::Weapon::Sword));
            Choices.push_back(Choice::Base("Discharge another 2 charges from the <b>steel sceptre</b>", {Book::Type::Book1, 503}, Choice::Type::Discharge, Equipment::Item::SteelSceptre, 2));

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
            SurprisedEnemy = true;

            Enemies.clear();
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 1", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 2", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 3", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
            Enemies.push_back(Enemy::Base(Enemy::Type::Barbarian, "BARBARIAN 4", 8, 5, 7, 12, 1, 2, 1, Assets::Type::Barbarian));
        }

        void SetupCombat(Map::Base &Map, Party::Base &Party)
        {
            Map.Put(2, 2, Map::Object::Enemy, 0);
            Map.Put(3, 3, Map::Object::Enemy, 1);
            Map.Put(2, 5, Map::Object::Enemy, 2);
            Map.Put(3, 5, Map::Object::Enemy, 3);
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
            Enemies.clear();

            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
            Enemies.push_back(Enemy::Base(Enemy::Type::Skiapyr, "SKIAPYR", 5, 7, 8, 10, 1, 0, 0, Assets::Type::Skiapyr));
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
            SurprisedEnemy = true;

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
            Choices.push_back(Choice::Base("Choose your number again", {Book::Type::Book1, 310}, Choice::Type::SelectDice, "Choose your number"));

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

            Text = "The door opens and you enter a small cavern. A man lies dead on the stone floor nearby. Another of Kalugen's guards, by the look of it, probably sent on some mad suicide mission through the underworld by his evil master. Your evil master, too, come to that...\n\nHe has something clutched in his hand. It is an octagonal prism of coloured glass. You may take this if you wish. There are no other exits from the cavern. Nearby there is a marble dais.";

            Choices.clear();
            Choices.push_back(Choice::Base("Step on to the dais", {Book::Type::Book1, 386}));
            Choices.push_back(Choice::Base("Return and take the tunnel beyond the grille", {Book::Type::Book1, 279}));

            Controls = Story::Controls::Standard;
        }
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
            Choices.push_back(Choice::Base("(SAGE) Identify the <b>vial of black liquid</b>", {Book::Type::Book1, 463}, Character::Class::Sage, Equipment::Item::VialOfBlackLiquid));
            Choices.push_back(Choice::Base("Continue", {Book::Type::Book1, 236}));

            Controls = Story::Controls::Standard;
        }

        void Event(Party::Base &Party)
        {
            Equipment = {Equipment::Axe, Equipment::Axe, Equipment::Axe, Equipment::Axe, Equipment::Gold(20), Equipment::BreastPlate, Equipment::BreastPlate, Equipment::BreastPlate, Equipment::BreastPlate, Equipment::Dagger, Equipment::Dagger, Equipment::Dagger, Equipment::VialOfBlackLiquid};
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
            Choices.push_back(Choice::Base("Use the <b>steel sceptre</b>", {Book::Type::Book1, 4}, Choice::Type::Discharge, Equipment::Item::SteelSceptre, 1));
            Choices.push_back(Choice::Base("Leave the <b>sword</b>", {Book::Type::Book1, 503}, Choice::Type::DropWeapon, Equipment::Weapon::Sword));

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
            Choices.push_back(Choice::Base("Use the <b>scorched skull</b> to leave", {Book::Type::Book1, 97}, Equipment::Item::ScorchedSkull));

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
            Choices.push_back(Choice::Base("Choose a number for the next Spiral", {Book::Type::Book1, 60}, Choice::Type::SelectDice, "Choose your number"));

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
            Choices.push_back(Choice::Base("Resist the psychic attack", {Book::Type::Book1, 447}, {Book::Type::Book1, 138}, Choice::Type::AttributeSelectedCharacter, Attributes::Type::PsychicAbility));

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
            Choices.push_back(Choice::Base("Choose a number for the next Spiral", {Book::Type::Book1, 90}, Choice::Type::SelectDice, "Choose your number"));

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
                Choices.push_back(Choice::Base("Let others drink the same potion", {Book::Type::Book1, -62}, Choice::Type::SelectAdventurer, "Who shall drink the bubbling green soup"));
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
    auto story069 = Story069();
    auto story398 = Story398();
    auto story452 = Story452();

    void InitializeStories()
    {
        Book1::Stories = {
            &event015, &event038, &event062,
            &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009, &story010,
            &story011, &story012, &story013, &story014, &story015, &story016, &story017, &story018, &story019, &story020,
            &story021, &story022, &story023, &story024, &story025, &story026, &story027, &story028, &story029, &story030,
            &story031, &story032, &story033, &story034, &story035, &story036, &story037, &story038, &story039, &story040,
            &story041, &story042, &story043, &story044, &story045, &story046, &story047, &story048, &story049, &story050,
            &story051, &story052, &story053, &story054, &story055, &story056, &story057, &story058, &story059, &story060,
            &story061, &story062, &story063, &story064, &story065, &story069,
            &story398,
            &story452};
    }
}
#endif