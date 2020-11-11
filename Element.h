#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <vector>

/*
    Class used by logic elements.
*/


class Element
{
    typedef struct token
    {
        int pin;
        int value;
    }Token;

public:
    Element() = delete;
    // The constructor automatically generates the required number of input pins based on the length of the pinValues_ vector. 
    // The output pin is left disconnected.
    Element(std::vector<int> pinValues_, int ID_);
    // Inserts/updates the logic value of the pin Token.pin to the value Token.value.
    // If the nextGate is connected, the change will propagate through the circuit.
    void updateInputPin(Token token_);
    // Connects the gate's output to the next gate's corresponding input pin.
    void setOutputGate(Element* gate_, int pin_);
    int getOutput() { return outputValue; };
    int getID() { return ID; };
protected:
    /* Evaluates the output value of the gate based on current state of the input pins.
       Called every time the input pin(s) change their value. 
       Virtual method, the output is evaluated differently for every derived class. */
    virtual void evaluate();
    int outputValue;
    int ID;
    Element* nextGate;
    std::vector<int> inputPins;
    int nextGatePin; // Needed for generating tokens to update the next logic gate.
};

class Not : public Element
{
public:
    Not(std::vector<int> pinValues_, int ID_) : Element(pinValues_, ID_) {};
protected:
    void evaluate() override;
};

class And : public Element
{
public:
    And(std::vector<int> pinValues_, int ID_) : Element(pinValues_, ID_) {};
protected:
    void evaluate() override;
};

class Or : public Element
{
public:
    Or(std::vector<int> pinValues_, int ID_) : Element(pinValues_, ID_) {};
protected:
    void evaluate() override;
};

class Probe : public Element
{
public:
    Probe(std::vector<int> pinValues_, int ID_) : Element(pinValues_, ID_) {};
protected:
    void evaluate() override; // This method will never be called for probes
};

class Clk : public Element
{
public:
    Clk(std::vector<int> pinValues_, int ID_) : Element(pinValues_, ID_) {};
protected:
    /* Just feeds the input to the output */
    void evaluate() override; 
};

class Signal : public Element
{
public:
    Signal(std::vector<int> pinValues_, int ID_) : Element(pinValues_, ID_) {};
protected:
    /* Just feeds the input to the output */
    void evaluate() override;    
};

/* 
    To add new elements, insert a derived class from case Element in the same fashion classes NOT, AND and OR were derived.
*/

#endif