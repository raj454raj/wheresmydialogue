#include<bits/stdc++.h>
#define ll long long int
#define MATCH_CUTOFF 50
#define RED "\x1b[1;31m"
#define GREEN "\x1b[1;32m"
#define YELLOW "\x1b[1;33m"
#define BLUE "\x1b[1;34m"
#define MAGENTA "\x1b[1;35m"
#define CYAN "\x1b[1;36m"
#define RESET_COLOR "\x1b[0m"

using namespace std;

/*
 * Utilites
 */
// Convert string to integer
ll stringToInt(string s) {
    ll val;
    istringstream ss(s);
    ss >> val;
    return val;
}

// Convert integer to string
string toString(ll v) {
    ostringstream ss;
    ss << v;
    return ss.str();
}

// Convert timeObject to milliseconds
ll toMilliSeconds(pair< pair<ll, ll>, pair<ll, ll> > p) {
    return p.first.first * 3600000 + p.first.second * 60000 + p.second.first * 1000 + p.second.second;
}

// Split a string according to the delimiter specified
// and return a vector<string>
vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;

    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// Convert string to timeObject
pair< pair<ll, ll>, pair<ll, ll> > toTimeObject(string s) {
    vector<string> t, secs;
    t = split(s, ':', t);
    ll hours, mins, seconds, milliseconds;
    hours = stringToInt(t[0]);
    mins = stringToInt(t[1]);

    secs = split(t[2], ',', secs);
    seconds = stringToInt(secs[0]);
    milliseconds = stringToInt(secs[1]);

    return make_pair(make_pair(hours, mins), make_pair(seconds, milliseconds));
}

// Return a pair of long long ints for
// start and end of the interval
pair<ll, ll> getDuration(string s) {
    vector<string> timeObj;
    timeObj = split(s, ' ', timeObj);
    string start, end;
    start = timeObj[0];
    end = timeObj[2];

    pair< pair<ll, ll>, pair<ll, ll> > startObj, endObj;
    startObj = toTimeObject(start);
    endObj = toTimeObject(end);
    return make_pair(toMilliSeconds(startObj), toMilliSeconds(endObj));
}

// Convert all the characters of a string to Lowercase
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// Get the percentage match of two strings
double getMatch(set<string> s1, set<string> s2) {
    double perc;
    int total = s2.size();
    set<string>::iterator i, j;
    int count = 0;
    for(i = s1.begin() ; i != s1.end() ; ++i) {
        for(j = s2.begin() ; j != s2.end() ; ++j) {
            if(*i == *j)
                count++;
        }
    }
    perc = (double)(count * 100.0 / total);
    return perc;
}

// Convert milliseconds to a string
string millisecondsToTime(ll ins) {
    ll hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
    string finaltime="";

    while(ins >= 3600000) {
        hours++;
        ins -= 3600000;
    }
    string hrs, mins, secs, millisecs;
    hrs = toString(hours);
    if(hrs.size() <= 1)
        finaltime += "0";
    finaltime += hrs + ":";

    while(ins >= 60000) {
        minutes++;
        ins -= 60000;
    }
    mins = toString(minutes);
    if(mins.size() <= 1)
        finaltime += "0";
    finaltime += mins + ":";

    while(ins >= 1000) {
        seconds++;
        ins -= 1000;
    }
    secs = toString(seconds);
    if(secs.size() <= 1)
        finaltime += "0";
    finaltime += secs + ",";

    milliseconds = ins;
    millisecs = toString(milliseconds);
    if(millisecs.size() == 1)
        finaltime += "00";
    else if(millisecs.size() == 2)
        finaltime += "0";
    finaltime += millisecs;

    return finaltime;
}

// Replace all occurrences of a substring with given string
void replaceAll(string& str, const string& from, const string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        // In case 'to' contains 'from', like replacing 'x' with 'yx'
        start_pos += to.length();
    }
}

int main(int argc, char *argv[]) {

    // If filename not provided
    if(argc < 3) {
        cout << "Please pass the filename as the argument: ./a.out filename.srt\n";
        return 0;
    }

    // Declarations
    ifstream file;
    string line, dialogue, complete_line, dur;
    int cnt, found, flag, words, wordstillnow, pos;
    ll time_instant;
    double match_percentage;
    pair<ll, ll> totalDuration;
    vector<string> dlg, fullLine;
    set<string> dialogueSet, lineSet;
    string instant;
    vector<int> results;
    int line_number = 1;
    int choice;
    string movie_filename(argv[2]);
    replaceAll(movie_filename, " ", "\\ ");

    cout << "Enter the dialogue to be searched: ";
    getline(cin, dialogue);
    dialogue = toLower(dialogue);
    dlg = split(dialogue, ' ', dlg);
    copy(dlg.begin(), dlg.end(), inserter(dialogueSet, dialogueSet.begin()));

    // Open the file
    file.open(argv[1]);

    // If the file exists
    if (file.is_open()) {

        complete_line = dur = "";
        cnt = 0;
        flag = 0;
        results.push_back(0);
        while (getline(file, line)) {
            // Remove all the carriage returns
            line.erase(remove(line.begin(), line.end(), '\r'), line.end());

            // cnt = 0  Do nothing
            // cnt = 1  Store the string in dur
            // cnt = 2  Store the string in complete_line

            // If line is not empty
            if (line.compare("")) {
                if(!cnt) {
                    cnt++;
                    continue;
                }

                if (line.find("-->") != string::npos) {
                    dur += line;
                }
                else {
                    if (complete_line != "")
                        complete_line += " ";
                    complete_line += line;
                }
                cnt++;
            }
            else {

                // Get the starting and ending time of the interval
                totalDuration = getDuration(dur);

                complete_line = toLower(complete_line);
                fullLine = split(complete_line, ' ', fullLine);
                copy(fullLine.begin(), fullLine.end(), inserter(lineSet, lineSet.begin()));

                // Compute the match percentage
                match_percentage = getMatch(lineSet, dialogueSet);

                // Main condition for showing/not showing matching results
                if (match_percentage >= MATCH_CUTOFF) {

                    // Total word count of complete_line
                    words = count(complete_line.begin(), complete_line.end(), ' ') + 1;

                    // Position of first word in complete_line
                    pos = complete_line.find(dlg[0]);

                    // If first word is not found and still the match
                    // is greater than cutoff just return the start
                    if(pos == string::npos)
                        time_instant = totalDuration.first;
                    else {
                        wordstillnow = count(complete_line.begin(), complete_line.begin() + pos, ' ') + 1;
                        // Compute the exact instant time where the word is voiced
                        time_instant = totalDuration.first + pos * (totalDuration.second - totalDuration.first) / complete_line.size();
                    }

                    // Convert milliseconds to readable format
                    instant = millisecondsToTime(time_instant);

                    results.push_back(time_instant / 1000);
                    cout << BLUE << line_number << ". " << RESET_COLOR << instant << " " << match_percentage << "%  ";
                    line_number++;

                    // Loop through the complete_string and color the matching words
                    for(int i = 0 ; i < fullLine.size() ; ++i) {
                        if(find(dlg.begin(), dlg.end(), fullLine[i]) != dlg.end()) {
                            cout << RED << " " << fullLine[i] << RESET_COLOR << " ";
                        }
                        else
                            cout << fullLine[i] << " ";
                    }
                    cout << endl;
                    // Atleast one match found
                    flag = 1;
                }

                // Reset loop variables
                complete_line = dur = "";
                fullLine.clear();
                lineSet.clear();
                cnt = 0;
            }
        }

        // If no match found
        if(!flag) {
            cout << "Please try to be more precise...\n";
        }
        else {
            cout << "The result number for your required dialogue: ";
            scanf("%d", &choice);

            string final_command = "vlc " + movie_filename + " --start-time=";

            while (1) {
                if (choice && choice < results.size()) {

                    final_command += toString((ll)results[choice] - 1);
                    // Minor hack to give accurate results ;)
                    final_command += ".1";
                    const char *command = final_command.c_str();
                    system(command);
                    break;

                }
                else {

                    cout << RED << "Invalid choice. Give input between 1-" << results.size() - 1 << RESET_COLOR << endl;
                    cout << "New choice: ";
                    scanf("%d", &choice);

                }
            }
        }
        file.close();
    }
    else {
        cout << "Input file not found";
    }
    return 0;
}
