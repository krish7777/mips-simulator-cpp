#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

vector<int> mem(1024);
int PC = 0;
map<string, int> reg;
map<string, int> labels;
void printReg()
{
    cout << "$s0 = " << reg["$s0"] << " ";
    cout << "$s1 = " << reg["$s1"] << " ";
    cout << "$s2 = " << reg["$s2"] << " ";
    cout << "$s3 = " << reg["$s3"] << " ";
    cout << "$s4 = " << reg["$s4"] << " ";
    cout << "$s5 = " << reg["$s5"] << " ";
    cout << "$s6 = " << reg["$s6"] << " ";
    cout << "$t0 = " << reg["$t0"] << " ";
    cout << "$t1 = " << reg["$t1"] << " ";
}

void li(string regName, int val)
{
    reg[regName] = val;
    //cout << "li called" << endl;
}

void la(string regName1, int val, string regName2)
{
    int address = reg[regName2] + val;
    reg[regName1] = address;
    //cout << "lw called" << endl;
}

void lui(string regName, int val)
{
    reg[regName] = val;
    //cout << "la called" << endl;
}

void lw(string regName1, int val, string regName2)
{
    int address = reg[regName2] + val;
    reg[regName1] = mem[address];
    //cout << "lw called" << endl;
}

void sw(string regName1, int val, string regName2)
{
    int address = reg[regName2] + val;
    mem[address] = reg[regName1];
    //cout << "sw called" << endl;
}

void add(string r1, string r2, string r3)
{
    reg[r1] = reg[r2] + reg[r3];
    //cout << "add called" << endl;
}
void addi(string r1, string r2, int r3)
{
    reg[r1] = reg[r2] + r3;
    //cout << "addi called" << endl;
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
    //cout << "sub called" << endl;
}

int main()
{

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

    ifstream file("bubble.asm");
    vector<vector<string>> programStack;

    int dataCounter = 0;

    string program;
    int takingData = 0;
    while (getline(file, program))
    {
        replace(program.begin(), program.end(), ',', ' ');

        //program.erase(remove(program.begin(), program.end(), ','), program.end()); //to remove commas
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
                //cout << "got .data " << endl;
                takingData = 2;
                continue;
            }
            if (takingData == 2)
            {
                //cout << "expected array:   " << item << endl;
                takingData = 3;
                continue;
            }
            if (takingData == 3)
            {
                //cout << "expected .word   " << item << endl;
                takingData = 1;
                continue;
            }
            if (takingData == 1)
            {
                if (!item.compare(".globl") || !item.compare(".text"))
                {
                    takingData = 0;
                    //   cout << "expected .text   " << item << endl;
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

            item.erase(remove_if(item.begin(),
                                 item.end(),
                                 bind(isspace<char>,
                                      placeholders::_1,
                                      locale::classic())),
                       item.end());
            if (*(item.rbegin()) == ':')
            {

                item = item.substr(0, item.size() - 1);
                //  cout << "item is " << item << "PC is " << PC << endl;
                labels[item] = PC;

                continue;
            }

            if (commentFlag == 0 && takingData == 0)
            {
                progstack = 1;
                item.erase(remove_if(item.begin(),
                                     item.end(),
                                     bind(isspace<char>,
                                          placeholders::_1,
                                          locale::classic())),
                           item.end());
                if (!item.compare("jr"))
                {
                    //cout << "i am here";
                    finish = 1;
                    break;
                }
                //cout << "item is " << item << " pc is " << PC << endl;
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

        // for (int i = 0; i < splittedStrings.size(); i++)
        // {
        //     cout << splittedStrings[i] << "\n";
        // }

        //cout << "\n\n";
    }

    // for (int i = 0; i < programStack.size(); i++)
    // {
    //     for (int j = 0; j < programStack[i].size(); j++)
    //     {
    //         cout << programStack[i][j] << "-";
    //     }
    //     cout << endl;
    // }
    //cout << programStack.size();
    for (int i = 0; i < programStack.size(); i++)
    {
        // cout << "i am inside";
        //printReg();
        // cout << "\nMEMOry\n";
        // for (int i = 0; i < 7; i++)
        // {
        //     cout << mem[i] << "  ";
        // }
        // cout << "\n";
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

            // cout << programStack[i][0] << "-" << programStack[i][1] << "-" << programStack[i][2] << endl;
            // cout << programStack[i][1] << "-" << number << "-" << reg[subReg] << "-" << subReg << endl
            //      << endl;

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
            //cout << "j called" << endl;
        }
        else if (!programStack[i][0].compare("bgt"))
        {
            int first = reg[programStack[i][1]];
            int second = reg[programStack[i][2]];
            if (first > second)
                i = labels[programStack[i][3]] - 1;
            //cout << "bgt called" << endl;
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
            //cout << "bne called" << endl;
            if (first != second)
                i = labels[programStack[i][3]] - 1;
        }
        else if (!programStack[i][0].compare("beq"))
        {
            int first = reg[programStack[i][1]];
            int second = reg[programStack[i][2]];
            //cout << "beq called" << first << "  " << second << endl;
            if (first == second)
                i = labels[programStack[i][3]] - 1;
        }
        else if (!programStack[i][0].compare("slt"))
        {
            int first = reg[programStack[i][2]];
            int second = reg[programStack[i][3]];
            //cout << "slt called" << endl;
            if (first < second)
                reg[programStack[i][1]] = 1;
            else
                reg[programStack[i][1]] = 0;
        }
    }

    //cout << reg["$s0"] << " " << reg["$s1"] << " " << reg["$s2"];
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