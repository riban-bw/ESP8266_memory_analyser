/** memanalyser - A tool for analysing memory consumption for compiled ELF file for ESP8266
 *
 *  License     GPL V3
 *  Copyright   riban 2016
 *  Author      Brian Walton brian@riban.co.uk
 *  Derived from ESP8266_memory_analyszer by Andrey Filimonov (Sermus)
 */

#include <stdio.h> //provides sscanf for hex to long conversion
#include <iostream> //provides std input and output (cin, cout, etc.)
#include <iomanip> //provides output formatting (columns)
#include <vector> //provides vector used to hold each line of std input
#include "version.h"

using namespace std;

class cSection
{
    public:
        cSection(){};
        cSection(const string& sName, const string& sDesc) :
            name(sName),
            desc(sDesc),
            size(-1)
            {}
        string name;
        string desc;
        long size;
};

int main(int argc, char *argv[])
{
    if(argc >1)
    {
        string sArg1(argv[1]);
        if(sArg1 == "-v" || sArg1 == "--version")
        {
            cout << "memanalyser V" << AutoVersion::MAJOR << "." << AutoVersion::MINOR;
            if(AutoVersion::BUILD != 0)
                cout << "." << AutoVersion::BUILD;
            cout << " Copyright riban 2016";
            cout << endl;
            return 0;
        }
        cerr << "Usage: objdump -t | memanalyser" << endl;
        return -1;
    }
    //read std input in to list (vector) of strings, one line per vector element
    vector<string> vLines; //holds each line from std input
    string sInput;
    while(getline(cin, sInput))
        vLines.push_back(sInput);

    //list  (vector) of firmware sections
    vector<cSection> vSections;
    vSections.push_back(cSection("data", "Initialized Data (RAM)"));
    vSections.push_back(cSection("rodata", "ReadOnly Data (RAM)"));
    vSections.push_back(cSection("bss", "Uninitialized Data (RAM)"));
    vSections.push_back(cSection("text", "Cached Code (IRAM)"));
    vSections.push_back(cSection("irom0_text", "Uncached Code (SPI)"));

    //RAM values
    const long lAvailableDRAM = 32786;
    const long lAvailableIRAM = 81920;

    //print header
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "   Section|                   Description| Start (hex)|   End (hex)|Used space" << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    //iterate through each firmware section
    for(vector<cSection>::iterator itSection = vSections.begin(); itSection != vSections.end(); ++itSection)
    {
        string sSectionStartToken = " _" + itSection->name + "_start";
        string sSectionEndToken = " _" + itSection->name + "_end";
        long lSectionStart = -1L; //address of section start
        long lSectionEnd = -1L; //address of section end
        //iterate through each line of std input
        for(vector<string>::iterator itLines = vLines.begin(); itLines != vLines.end(); ++itLines)
        {
            if(itLines->find(sSectionStartToken) != string::npos)
            {
                //found start token - need first column
                size_t nEnd = itLines->find(" ");
                string sStart = itLines->substr(0, nEnd);
                sscanf(sStart.c_str(), "%lX", (unsigned long*)&lSectionStart); // string to long
            }
            if(itLines->find(sSectionEndToken) != string::npos)
            {
                //found end token - need first column
                size_t nEnd = itLines->find(" ");
                string sStart = itLines->substr(0, nEnd);
                sscanf(sStart.c_str(), "%lX", (unsigned long*)&lSectionEnd); // string to long
            }
            if(lSectionStart != -1L && lSectionEnd != -1L)
            {
                //both start and end found so we are done for ths section
                break;
            }
        }
        itSection->size = lSectionEnd - lSectionStart;
        //print table row
        cout << setw(10) << itSection->name << "|" << setw(30) << itSection->desc << "|" << setw(12) << hex << lSectionStart << "|" << setw(12) << lSectionEnd << "|" << setw(8) << dec << itSection->size << endl;
    }
    //print footer
    long lTotalRamUsed = vSections[0].size + vSections[1].size + vSections[2].size;
    cout << "Total Used RAM : " << lTotalRamUsed << endl;
    cout << "Free RAM : " << lAvailableIRAM - lTotalRamUsed << endl;
    cout << "Free IRam : " << lAvailableDRAM - vSections[3].size << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    return 0;
}
