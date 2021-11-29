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

            Text = "For the whole day now you have been approaching the smoke columns rising up vertically hundreds of metres from the flat plains of Krarth. The sky above is blue, cloudless and cold. A bitter wind blows across the dry, sere grass of the marshes and sends sluggish ripples through the puddles of mire which occupy the low ground. The monotony of the landscape is broken only by the occasional stunted willow growing by black, muddy pools and meres.\n\nKalugen's Keep is a monolithic black citadel looming ahead of you. Hoping to reach its gates before nightfall, you curse the throng of peasants and merchants milling towards it. You jostle through them towards the citadel -- this morning just an imperceptible speck across the bleak wastes, but now a monstrous edifice that seems to fill the horizon.\n\nYou know you have only until tomorrow to find a sponsor among the magi of Krarth, for tomorrow battle will be unleashed in the pits beneath the citadel. Fame and fortune will go to the Adventurers -- and their sponsor -- who return from the labyrinthine Battlepits with the Emblem of Victory. If you find no magus who is prepared to sponsor you this time, you will have to wait another long year for the next contest. A year through which the marsh waters will rise, drowning the land and the causeways leading to the citadel, rendering Kalugen's Keep impregnable. No one enters or leaves in that time except the magi on their flying carpets, and only in mid-year can the citizens emerge to sow their fields with corn and rice before the harsh winter months begin again.\n\nAt last you manage to break through the gabbling crowds of the baggage train and ride under the grey blocks of the massive gate, its portcullis open like the maw of a hungry god. The dour streets are festooned with the flags of the magi -- the one week of the year when this grim place is ablaze with colour, you reflect wryly -- and street criers call out the glories of their lords. In the central square you see a booth where the magi's stewards are registering the combatants whom their masters have employed for tomorrow's contest. Each hero, or group of heroes, must take their sponsor's pennant with them into the Battlepits, but as you scan the racks outside the booth you see that only three pennants remain. Three stewards stand by the pennants. Three magi seek champions.\n\nThe stewards grin sourly as they see you eyeing the pennants. Over the years they must have engaged dozens of brave Adventurers on their respective masters' behalf -- and how many of those Adventurers went down into the Battlepits never to emerge? You scowl back at them grimly, but they only smile the broader. They know you must choose one of the pennants. As you debate the choice, an old merchant wrapped in greasy furs and obviously the worse for drink sees you contemplating the booth.";

            Image = "images/book1/merchants.png";

            TopImage = false;

            Choices.clear();
            Choices.push_back(Choice::Base("Ignore the drunken merchant and go over to the booth", {Book::Type::Book1, 452}));
            Choices.push_back(Choice::Base("Talk to the merchant", {Book::Type::Book1, 69}));
            Choices.push_back(Choice::Base("[TRICKSTER] Try something", {Book::Type::Book1, 58}, Character::Class::Trickster));
            Choices.push_back(Choice::Base("[SAGE] Try something", {Book::Type::Book1, 18}, Character::Class::Sage));
            Choices.push_back(Choice::Base("[ENCHANTER] Try something", {Book::Type::Book1, 398}, Character::Class::Enchanter));

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
        Engine::Destination Destination = {};

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
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 1", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 2", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 3", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 4", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));

            Enemies[0].CanMove = false;
            Enemies[1].CanMove = false;
            Enemies[2].CanMove = false;
            Enemies[3].CanMove = false;
            Enemies[0].CanShoot = true;
            Enemies[1].CanShoot = true;
            Enemies[2].CanShoot = true;
            Enemies[3].CanShoot = true;
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
            Choices.clear();

            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 513};
            }
            else
            {
                Destination = {Book::Type::Book1, 226};
            }
        }

        Engine::Destination Continue(Party::Base &Party) { return Destination; }
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

        Engine::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 356}; };
    };

    class Story006 : public Story::Base
    {
    public:
        Engine::Destination Destination = {};

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

        Engine::Destination Continue(Party::Base &Party) { return Destination; }
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

            Text = "(TRICKSTER) A commanding voice rings out over the clash of swords and the screams of the crowd. A majestic, scarlet-robed figure sweeps forwards through the mob, which parts and shrinks away. The wounded guard lowers his sword. 'My lord,' he says, 'I was about to apprehend this common thief.'\n\n'Be silent,' commands the robed man. 'You were about to lose your life, you clumsy wretch. If you were in my service, I'd feed you to the nightgaunts. Begone!'\n\nThe red-robed man turns to you as the guard blanches, bows stiffly and is gone. He smiles coldly at you. 'Your fencing technique is passable, and may be more rewarding to you than your thievery.'";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Engine::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 397}; };
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

            Text = "(SAGE)\n\n<i>The green potion is a healing draught. The black potion is chimera spittle, a delayed-action poison. The effervescent liquid is an antidote to chimera spittle. The slimy green potion is deadly. You cannot identify the colourless liquid.</i>";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Engine::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 103}; };
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
            Text = "He is outraged. He half rises from his chair, a tall reed-like figure that seems to unfold rather than stand. His hard thin fingers grip the edge of the table.\n\n'I am astonished!' he says at last through clenched teeth. Fangs, really. 'You have had the benefit of tuition from no less than Grandmaster Kief. I am the acknowledged authority on the Spiral of Gold game. I beat Magus Tor -- the true, original Magus Tor -- in a ten-game championship. Yet you cannot understand my explanation! Sickened though I am, I shall not slay you -- merely consign you to the lower depths.' He lifts a hand from which tendrils of icy flame spread like a net, entangling you. You gasp in sudden pain, <b>losing ";

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

        Engine::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 5}; };
    };

    class Story011 : public Story::Base
    {
    public:
        Engine::Destination Destination = {};

        Story011()
        {
            Book = Book::Type::Book1;

            Id = 11;

            Text = "You call the spell to mind. As soon as he sees you starting to concentrate, the Dirge-Man gives a distorted frown of distrust. 'Oh now, what's this?' With a mad croak, he launches to the attack.";

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

        Engine::Destination Continue(Party::Base &Party) { return Destination; }
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

        Engine::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 223}; }
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
        Engine::Destination Destination = {};

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
            Enemies.push_back(Enemy::Base(Enemy::Type::NinjaAssassin, "ASSASSIN 1", 7, 6, 7, 5, 1, 0, 0, Assets::Type::NinjaAssassin));
            Enemies.push_back(Enemy::Base(Enemy::Type::MagusVyl, "MAGUS VYL", 7, 9, 9, 35, 3, 0, 2, Assets::Type::MagusVyl));

            Enemies[0].CanMove = false;
            Enemies[0].CanShoot = true;
        }

        void SetupCombat(Map::Base &Map, Party::Base &Party)
        {
            Map.Put(1, 0, Map::Object::Enemy, 0);
            Map.Put(1, 6, Map::Object::Enemy, 1);
        }

        void AfterCombat(Party::Base &Party, Combat::Result Result)
        {
            Choices.clear();

            if (Result == Combat::Result::ESCAPED)
            {
                Destination = {Book::Type::Book1, 513};
            }
            else
            {
                Destination = {Book::Type::Book1, 226};
            }
        }

        Engine::Destination Continue(Party::Base &Party) { return Destination; }
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
            Text = "'Excellent.' He conceals his Die behind a slender hand and smiles across the table at you as he considers his opening gambit.";
            
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

        Engine::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 1}; };
    };

    class Story058 : public Story::Base
    {
    public:
        Story058()
        {
            Book = Book::Type::Book1;

            Id = 58;

            Text = "[TRICKSTER] Under the merchant's beer-stained fur jacket you notice a bulging money-pouch. Being a Trickster, your thoughts naturally turn to theft.";

            Choices.clear();
            Choices.push_back(Choice::Base("Try to rob the merchant", {Book::Type::Book1, 121}, {Book::Type::Book1, 181}, Character::Class::Trickster, Attributes::Type::Awareness));

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

            Text = "He lurches from side to side, struggling to focus on you. Giving vent to an enormous belch, he says, 'Lookin' fer employ with a magus, you'll be, I don' wonder. There's three still wantin' champions fer the contest -- that's their pennants over by that booth ...\n\n'Magus Vyl is the worst o' the lot, but then 'e's a vampire an' I shouldn't speak ill o' the dead. Kalugen -- lord o' this city, as you'll know -- well, 'e ain't much better, an' he's not in the contest to win, I've 'eard. But the third -- Magus Balhazar -- is the best of a bad bunch, 'cause 'e's got most to lose in the contest, an' 'e'll be rewardin' 'is champions the best on account o' that.'\n\nAny hopes you might have of questioning the merchant further are soon dashed. Exhausted by the effort of speaking, he favours you with a broad grin and then falls like a plank on the cobblestones. A boot in the ribs does nothing to rouse him from his drunken stupor, so you decide to approach the booth.";

            Choices.clear();

            Controls = Story::Controls::Standard;
        }

        Engine::Destination Continue(Party::Base &Party) { return {Book::Type::Book1, 452}; };
    };

    class Story398 : public Story::Base
    {
    public:
        Story398()
        {
            Book = Book::Type::Book1;

            Id = 398;

            Text = "[ENCHANTER] You consider your various spells. Two non-combat spells might be of use to you now. But which? Make your choice";

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
    auto story018 = Story018();
    auto story058 = Story058();
    auto story069 = Story069();
    auto story398 = Story398();
    auto story452 = Story452();

    void InitializeStories()
    {
        Book1::Stories = {
            &event015,
            &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009, &story010,
            &story011, &story012, &story013, &story014, &story015, &story018,
            &story058,
            &story069,
            &story398,
            &story452};
    }
}
#endif