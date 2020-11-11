#include "Element.h"

Element::Element(std::vector<int> pinValues_, int ID_)
{
    nextGate = nullptr;
    inputPins = pinValues_;
    nextGatePin = -1; // undefined value
    outputValue = 0;
    ID = ID_;
    
    Element::Token tempToken;
    tempToken.pin = 0;
    tempToken.value = 0;

    updateInputPin(tempToken);
}

void Element::setOutputGate(Element* gate_, int pin_)
{
    nextGate = gate_;
    nextGatePin = pin_;
}

void Element::updateInputPin(Element::Token token_)
{
    inputPins[token_.pin] = token_.value;
    this->evaluate();

    /* If the output value changed, 
    the signal must propagate through the circuit */
    if (nextGate != nullptr)
    {
        Element::Token tmpToken;
        tmpToken.pin = nextGatePin;
        tmpToken.value = outputValue;

        // Updates the next gate.
        nextGate->updateInputPin(tmpToken);
    }
}

void And::evaluate()
{
    outputValue = 1;
    for (auto i : inputPins)
    {
        if (inputPins[i] == 0)
        {
            outputValue = 0;
            break;
        }
    }
}

void Or::evaluate()
{
    outputValue = 0;
    for (auto i : inputPins)
    {
        if (inputPins[i] == 1)
        {
            outputValue = 1;
            break;
        }
    }
}

void Not::evaluate()
{
    if (inputPins[0] == 1)
    {
        outputValue = 0;
    }
    else
    {
        outputValue = 1;
    }
} 

void Probe::evaluate() { return; }
void Clk::evaluate() { outputValue = inputPins[0]; }
void Signal::evaluate() { outputValue = inputPins[0]; }
/*
    Insert new elements evaluation methods (TODO : xor)
*/