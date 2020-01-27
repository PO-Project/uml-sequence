#ifndef _IACTOR_HPP_
#define _IACTOR_HPP_

#include "IDrawable.hpp"

struct IActor : public IDrawable
{
    bool selected{false};

    virtual Point getCenter() const = 0;
};

#endif