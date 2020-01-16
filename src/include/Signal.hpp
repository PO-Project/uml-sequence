#ifndef _SIGNAL_HPP_
#define _SIGNAL_HPP_

#include "IActor.hpp"
#include "Arrow.hpp"

class Signal : public IActor
{

};

class InformationSignal : public Signal
{
    Line line;
};

class ProcessSwitchSignal : public Signal
{
    Arrow arrow;
};

#endif