#include <stdio.h> //provides sscanf for hex to long conversion
#include <iostream> //provides std input and output (cin, cout, etc.)
#include <iomanip> //provides output formatting (columns)
#include <vector> //provides vector used to hold each line of std input

using namespace std;

main()
{
    //read std input in to vector of strings, one line per vector element
    vector<string> vLines;
    string sInput;
    while(getline(cin, sInput))
        vLines.push_back(sInput);

    //array of strings, one per firmware section
    string sections[]
    {
        "data",
        "rodata",
        "bss",
        "text",
        "irom0_text"
    };

    //array of strings, providing description of eachfirmware section
    string sectionsDesc[]
    {
        "Initialized Data (RAM)",
        "ReadOnly Data (RAM)",
        "Uninitialized Data (RAM)",
        "Cached Code (IRAM)",
        "Uncached Code (SPI)"
    };

    //RAM values
    const int availableDRAM = 32786;
    const int availableIRAM = 81920;

    //print header
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "   Section|                   Description| Start (hex)|   End (hex)|Used space" << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    long totalRamUsed = 0L;
    long totalIRamUsed = 0L;
    //iterate through each firmware section
    for(int i = 0; i < 5; i++)
    {
        string sectionStartToken = " _" + sections[i] + "_start";
        string sectionEndToken = " _" + sections[i] + "_end";
        long sectionStart = -1L;
        long sectionEnd = -1L;
        //iterate through each line of std input
        for(int j = 0; j < vLines.size(); j++)
        {
            string currentLine = vLines[j];
            if(currentLine.find(sectionStartToken) != string::npos)
            {
                //found start token - need first column
                size_t nEnd = currentLine.find(" ");
                string sStart = currentLine.substr(0, nEnd);
                sscanf(sStart.c_str(), "%lX", &sectionStart); // string to long
            }
            if (currentLine.find(sectionEndToken) != string::npos)
            {
                //found end token - need first column
                size_t nEnd = currentLine.find(" ");
                string sStart = currentLine.substr(0, nEnd);
                sscanf(sStart.c_str(), "%lX", &sectionEnd); // string to long
            }
            if(sectionStart != -1L && sectionEnd != -1L)
            {
                //both start and end found so we are done for ths section
                break;
            }
        }
        long sectionLength = sectionEnd - sectionStart;
        if(i < 3)
        {
            //total ram is sum of first three sections
            totalRamUsed += sectionLength;
        }
        if(i == 3)
        {
            //total iram calculated from available minus used by text section
            totalIRamUsed = availableDRAM - sectionLength;
        }
        //print table row
        cout << setw(10) << sections[i] << "|" << setw(30) << sectionsDesc[i] << "|" << setw(12) << hex << sectionStart << "|" << setw(12) << sectionEnd << "|" << setw(8) << dec << sectionLength << endl;
    }
    //print footer
    cout << "Total Used RAM : " << totalRamUsed << endl;
    cout << "Free RAM : " << availableIRAM - totalRamUsed << endl;
    cout << "Free IRam : " << totalIRamUsed << endl;
    cout << "------------------------------------------------------------------------------" << endl;
}
