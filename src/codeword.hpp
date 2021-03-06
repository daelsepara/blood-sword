#ifndef __CODEWORD_HPP__
#define __CODEWORD_HPP__

#include <map>

namespace Code
{
    enum class Word
    {
        NONE = 0,
        BILE,
        CHIROGRAPH,
        CYCLOPS,
        DELIVERANCE,
        ELEKTRA,
        FLAG,
        FLITTERMOUSE,
        FOSSIL,
        GELID,
        GJALLER,
        GLIMPSE,
        HEOROT,
        IMPERIUM,
        IMPROBITY,
        IRE,
        LETHE,
        MYTHAGO,
        NEXUS,
        ROUT,
        RUBY,
        SELENE,
        SPECULUM,
        SWORD,
        TABULA,
        TOKEN,
        UPHEAVAL,
        WONDERLAND
    };

    // for character specific codewords
    enum class Status
    {
        NONE = 0,
        CROSSED_BRIDGE,
        STRIPPED_IMRAGARN,
        PRINCE_OF_DESOLATION
    };
}
#endif
