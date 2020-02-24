#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

vector<int> mem(1024);
int PC = 0;
map<string, int> reg;
map<string, int> labels;

void li(string regName, int val)
{
    reg[regName] = val;
}

void la(string regName1, int val, string regName2)
{
    int address = reg[regName2] + val;
    reg[regName1] = address;
}

void lui(string regName, int val)
{
    reg[regName] = val;
}

void lw(string regName1, int val, string regName2)
{
    int address = reg[regName2] + val;
    reg[regName1] = mem[address];
}

void sw(string regName1, int val, string regName2)
{
    int address = reg[regName2] + val;
    mem[address] = reg[regName1];
}

void add(string r1, string r2, string r3)
{
    reg[r1] = reg[r2] + reg[r3];
}
void addi(string r1, string r2, int r3)
{
    reg[r1] = reg[r2] + r3;
}

void sub(string r1, string r2, string r3)
{
    int number;
    istringstream iss(r3);
    iss >> number;
    if (number == 0)
        reg[r1] = reg[r2] - reg[r3];
    else
        reg[r1] = reg[r2] - number;
}

int main()
{
    //INITIALISATION OF REGISTER VALUES
    reg.insert(pair<string, int>("$zero", 0));
    reg.insert(pair<string, int>("$s0", 0));
    reg.insert(pair<string, int>("$s1", 0));
    reg.insert(pair<string, int>("$s2", 0));
    reg.insert(pair<string, int>("$s3", 0));
    reg.insert(pair<string, int>("$s4", 0));
    reg.insert(pair<string, int>("$s5", 0));
    reg.insert(pair<string, int>("$s6", 0));
    reg.insert(pair<string, int>("$s7", 0));
    reg.insert(pair<string, int>("$t0", 0));
    reg.insert(pair<string, int>("$t1", 0));
    reg.insert(pair<string, int>("$t2", 0));
    reg.insert(pair<string, int>("$t3", 0));
    reg.insert(pair<string, int>("$t4", 0));
    reg.insert(pair<string, int>("$t5", 0));
    reg.insert(pair<string, int>("$t6", 0));
    reg.insert(pair<string, int>("$t7", 0));
    reg.insert(pair<string, int>("$t8", 0));
    reg.insert(pair<string, int>("$t9", 0));
    reg.insert(pair<string, int>("$a0", 0));
    reg.insert(pair<string, int>("$a1", 0));
    reg.insert(pair<string, int>("$a2", 0));
    reg.insert(pair<string, int>("$a3", 0));
    reg.insert(pair<string, int>("$v0", 0));
    reg.insert(pair<string, int>("$v1", 0));
    reg.insert(pair<string, int>("$at", 0));
    reg.insert(pair<string, int>("$k0", 0));
    reg.insert(pair<string, int>("$k1", 0));
    reg.insert(pair<string, int>("$gp", 0));
    reg.insert(pair<string, int>("$sp", 0));
    reg.insert(pair<string, int>("$fp", 0));
    reg.insert(pair<string, int>("$ra", 0));

    //READING PROGRAM FROM FILE
    ifstream file("bubble.asm");
    vector<vector<string>> programStack;

    int dataCounter = 0;

    string program;
    int takingData = 0;
    while (getline(file, program))
    {
        replace(program.begin(), program.end(), ',', ' '); //remove commas

        stringstream ss(program);
        string item;
        vector<string> splittedStrings;

        int commentFlag = 0;

        int progstack = 0;
        int finish = 0;

        while (getline(ss, item, ' '))
        {
            if (!item.compare(""))
                continue;
            if (!item.compare(".data"))
            {
                takingData = 2;
                continue;
            }
            if (takingData == 2)
            {
                takingData = 3;
                continue;
            }
            if (takingData == 3)
            {
                takingData = 1;
                continue;
            }
            if (takingData == 1)
            {
                if (!item.compare(".globl") || !item.compare(".text"))
                {
                    takingData = 0;
                    continue;
                }
                else
                {
                    int number;
                    istringstream iss(item);
                    iss >> number;

                    mem[dataCounter] = number;

                    dataCounter++;
                }
            }

            if ((*item.begin()) == '#')
                commentFlag = 1;
            //remove unnecessary spaces
            item.erase(remove_if(item.begin(),
                                 item.end(),
                                 bind(isspace<char>,
                                      placeholders::_1,
                                      locale::classic())),
                       item.end());
            if (*(item.rbegin()) == ':')
            {

                item = item.substr(0, item.size() - 1);
                labels[item] = PC;

                continue;
            }

            if (commentFlag == 0 && takingData == 0)
            {
                progstack = 1;
                //remove spaces
                item.erase(remove_if(item.begin(),
                                     item.end(),
                                     bind(isspace<char>,
                                          placeholders::_1,
                                          locale::classic())),
                           item.end());
                if (!item.compare("jr"))
                {
                    finish = 1;
                    break;
                }
                splittedStrings.push_back(item);
            }
        }

        if (finish == 1)
            break;

        commentFlag = 0;
        if (progstack == 1)
        {
            programStack.push_back(splittedStrings);
            PC++;
        }
    }
    for (int i = 0; i < programStack.size(); i++)
    {

        if (!programStack[i][0].compare("add"))
        {
            add(programStack[i][1], programStack[i][2], programStack[i][3]);
        }
        else if (!programStack[i][0].compare("addi"))
        {
            int number;
            istringstream iss(programStack[i][3]);
            iss >> number;
            addi(programStack[i][1], programStack[i][2], number);
        }
        else if (!programStack[i][0].compare("sub"))
        {
            sub(programStack[i][1], programStack[i][2], programStack[i][3]);
        }
        else if (!programStack[i][0].compare("li"))
        {
            int number;
            istringstream iss(programStack[i][2]);
            iss >> number;
            li(programStack[i][1], number);
        }

        else if (!programStack[i][0].compare("lw"))
        {

            string subNumber = programStack[i][2].substr(0, programStack[i][2].length() - 5);
            int number;
            istringstream iss(subNumber);
            iss >> number;
            number = number / 4;
            string subReg = programStack[i][2].substr(programStack[i][2].length() - 4, 3);
            lw(programStack[i][1], number, subReg);
        }
        else if (!programStack[i][0].compare("sw"))
        {
            string subNumber = programStack[i][2].substr(0, programStack[i][2].length() - 5);
            int number;
            istringstream iss(subNumber);
            iss >> number;
            number = number / 4;
            string subReg = programStack[i][2].substr(programStack[i][2].length() - 4, 3);
            sw(programStack[i][1], number, subReg);
        }
        else if (!programStack[i][0].compare("la"))
        {
            string subNumber = programStack[i][2].substr(0, programStack[i][2].length() - 5);
            int number;
            istringstream iss(subNumber);
            iss >> number;
            number = number / 4;
            string subReg = programStack[i][2].substr(programStack[i][2].length() - 4, 3);
            la(programStack[i][1], number, subReg);
        }
        else if (!programStack[i][0].compare("lui"))
        {
            int number;
            istringstream iss(programStack[i][2]);
            iss >> number;
            lui(programStack[i][1], number);
        }
        else if (!programStack[i][0].compare("j"))
        {
            i = labels[programStack[i][1]] - 1;
        }
        else if (!programStack[i][0].compare("bgt"))
        {
            int first = reg[programStack[i][1]];
            int second = reg[programStack[i][2]];
            if (first > second)
                i = labels[programStack[i][3]] - 1;
        }
        else if (!programStack[i][0].compare("blt"))
        {
            int first = reg[programStack[i][1]];
            int second = reg[programStack[i][2]];
            if (first < second)
                i = labels[programStack[i][3]] - 1;
        }
        else if (!programStack[i][0].compare("bne"))
        {
            int first = reg[programStack[i][1]];
            int second = reg[programStack[i][2]];

            if (first != second)
                i = labels[programStack[i][3]] - 1;
        }
        else if (!programStack[i][0].compare("beq"))
        {
            int first = reg[programStack[i][1]];
            int second = reg[programStack[i][2]];

            if (first == second)
                i = labels[programStack[i][3]] - 1;
        }
        else if (!programStack[i][0].compare("slt"))
        {
            int first = reg[programStack[i][2]];
            int second = reg[programStack[i][3]];

            if (first < second)
                reg[programStack[i][1]] = 1;
            else
                reg[programStack[i][1]] = 0;
        }
    }

    cout << "REGISTERS\n";

    map<std::string, int>::iterator it = reg.begin();

    while (it != reg.end())
    {
        // Accessing KEY from element pointed by it.
        string word = it->first;

        // Accessing VALUE from element pointed by it.
        int count = it->second;

        cout << word << " :: " << count << endl;

        // Increment the Iterator to point to next entry
        it++;
    }

    cout << "\nMEMORY\n";
    for (int i = 0; i < mem.size(); i++)
    {
        cout << mem[i] << "  ";
    }
}