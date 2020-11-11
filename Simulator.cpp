#include "Simulator.h"
#include <exception>
#include <fstream>
#include <iostream>

enum LOGIC_TYPE {PROBE, CLK, SIGNAL, NOT, AND, OR};

Simulator::Simulator()
{
    duration = 0;
}

bool isGenerator(LOGIC_TYPE lt)
/*
 Function needs to update if adding new signal generators.
 */
{
    if (lt == LOGIC_TYPE::SIGNAL || lt == LOGIC_TYPE::CLK)
    {
        return true;
    }
    return false;
}

bool isElement(LOGIC_TYPE lt)
{
    if (lt != LOGIC_TYPE::SIGNAL && lt != LOGIC_TYPE::CLK && lt != LOGIC_TYPE::PROBE)
    {
        return true;
    }
    return false;
}

bool isProbe(LOGIC_TYPE lt)
{
    return lt == LOGIC_TYPE::PROBE;
}

void Simulator::loadCircuit(const std::string& filepath)
{
    /* Clears the simulator memory if the simulation should run
    more than once */
    logicElements.clear();
    probes.clear();
    events.clear();
    duration = 0;

    std::ifstream in;
    
    try // Assuming the file is correctly written (see the docs)
    {
        in.open(filepath, std::ios::in);
        std::string temp = "";
        
        int currRow = 1;
        int elementCnt = 0;


        while(getline(in, temp))
        {
            if (currRow == 1)
            {
                duration = std::stod(temp);
            }
            else if (currRow == 2)
            {
                elementCnt = std::stod(temp);
            }
            else if (currRow - 2 <= elementCnt) // instancing elements
            {
                int tempID = 0, i = 0;
                LOGIC_TYPE tempLT;
                Element* tempEl;

                while (temp[i] != ' ') // parsing ID
                {
                    tempID *= 10;
                    tempID += temp[i];
                }
                i++; // parsing logic type
                /*
                    INSERT NEW ELEMENT ID HERE
                */
                switch(temp[i])
                {
                    case '0': tempID = LOGIC_TYPE::PROBE; break;
                    case '1': tempID = LOGIC_TYPE::CLK; break;
                    case '2': tempID = LOGIC_TYPE::SIGNAL; break;
                    case '3': tempID = LOGIC_TYPE::NOT; break;
                    case '4': tempID = LOGIC_TYPE::AND; break;
                    case '5': tempID = LOGIC_TYPE::OR; break;
                    default: throw "Unsupported ID // most likely caused by irregular software extension. Check the documentation for troubleshooting //"; break;
                }

                i += 2;

                if ( isElement(tempLT) ) // instantiate a logic element
                { 
                    if (tempLT == LOGIC_TYPE::NOT)
                    {
                        std::vector<int> tempValues;
                        tempValues.push_back(0);
                        tempEl = new Not(tempValues, tempID);
                    }
                    else
                    {
                        std::vector<int> tempValues;
                        /*
                            Fill the temp values vector with exact
                            number of zeroes.
                        */

                        int tempPinCnt = temp[i] - '0';
                        i++;
                        while (i != ' ' && i < temp.length())
                        {
                            tempPinCnt *= 10;
                            tempPinCnt += temp[i] - '0';
                            i++;
                        }

                        for (int j = 0; j < tempPinCnt; j++)
                        {
                            tempValues.push_back(0);
                        }
                        if (tempLT == LOGIC_TYPE::AND)
                        {
                            tempEl = new And(tempValues, tempID);
                        }
                        else // OR
                        {
                            tempEl = new Or(tempValues, tempID);
                        }

                    }
                    logicElements.push_back(tempEl);
                }
                else if ( isProbe(tempLT) )
                {
                    std::vector<int> emptyVector;
                    tempEl = new Probe(emptyVector, tempID);
                    probes.push_back(tempEl);
                }
                else
                /* if generator, it is required to
                    update the event vector.
                    */
                {
                    if (tempLT == LOGIC_TYPE::SIGNAL)
                    {
                        std::vector<int> singleVector;
                        singleVector.push_back(0);

                        tempEl = new Signal(singleVector, tempID);

                        while (i < temp.length())
                        {
                            std::string tempchg = "";

                            while (temp[i] != ' ' && temp[i] < temp.length())
                            {
                                tempchg += temp[i++];
                            }
                            i ++;
                            Simulator::Event ev;
                            ev.moment = std::stod(tempchg);
                            ev.targetEl = tempEl;
                            events.push_back(ev);
                        }

                        logicElements.push_back(tempEl);
                    }
                    else // CLK
                    { // generate all the timestamps until the end of the simulation
                        std::string tempchg = "";
                        std::vector<int> singleVector;
                        singleVector.push_back(0);

                        tempEl = new Clk(singleVector, tempID);

                        while (temp[i] < temp.length())
                        {
                            tempchg += temp[i++];
                        }
                        double freq = std::stod(tempchg);
                        double deltaTime = 1 / freq;
                        
                        int k = 1;
                        while(deltaTime*k <= duration)
                        {
                            Simulator::Event ev;
                            ev.moment = deltaTime * k;
                            ev.targetEl = tempEl;
                            events.push_back(ev);
                            k++;
                        }

                        logicElements.push_back(tempEl);
                    }
                }
            }
            else // connecting the elements
            {
                /* code */
            }
            currRow++;
        }
        
    }
    catch(const std::exception& e)
    {
        std::cout << "Could not read the circuit properly. The file is missing or corrupt." << std::endl;
        std::cout << e.what() << std::endl;
        exit(1);
    }
    
}