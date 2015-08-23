#ifndef __TEXT_FIXER_HPP__
#define __TEXT_FIXER_HPP__

namespace TextFixerMode
{
    enum mode_t
    {
        fanfic = 0,
        codeSpaces
    };
}

void Fix( TextFixerMode::mode_t fixMode );

#endif