# ifndef _SIMULATOR_H_
# define _SIMULATOR_H_

#include "Element.h"
#include <string>

class Simulator
{
    typedef struct event
    {
        double moment;
        Element* targetEl;
    }Event;

    void loadCircuit(const std::string& filepath);
    void simulate(const std::string& filepath);

public:
    // 
    Simulator();
private:
    std::vector<Element*> logicElements;
    std::vector<Element*> probes;
    double duration;

    /*
     The vector contains all event(s) in the circuit with calculated
     exact time(s) of occurence and their target element(s) which are the
     cause(s) of the change(s) (clock generators and other signals).
    */
    std::vector<Event> events;
};

# endif