#ifndef __BOOK__HPP__
#define __BOOK__HPP__

#include <map>

namespace Book
{
    enum class Type
    {
        None = 0,
        Book1,
        Book2,
        Book3,
        Book4,
        Book5
    };

    std::map<Book::Type, const char *> Title = {
        {Book::Type::None, "Blood Sword"},
        {Book::Type::Book1, "The Battlepits of Krarth"},
        {Book::Type::Book2, "The Kingdom of Wyrd"},
        {Book::Type::Book3, "The Demon's Claw"},
        {Book::Type::Book4, "Doomwalk"},
        {Book::Type::Book5, "The Walls of Spyte"}};
}
#endif
