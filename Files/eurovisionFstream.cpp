#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

string getCountry(int &pos, string &str){
    string country;
    for (; str[pos] != ','; pos++){
        country += str[pos];
    }
    pos++;
    return country;
}

int getScore(int &pos, string &str){
    int score = 0;
    for (; str[pos] != ','; pos++){
        score += score * 10 + str[pos] - '0';
    }
    pos++;
    return score;
}

void getVoices(int n, int numOfCountry, string &str, vector<string> &countries, vector<vector<pair<int, int>>> &voices){
    str += ',';
    int pos = 0;
    countries.push_back(getCountry(pos, str));
    for (int j = 0; j < n; j++){
        voices[j].push_back(make_pair(getScore(pos, str), numOfCountry));
    }
}

void calculateResults(int n, vector<vector<pair<int, int>>> &voices, vector<pair<int, int>> &results){
    for (int i = 0; i < n; i++){
        results.push_back(make_pair(0, i));
    }
    for (int i = 0; i < n; i++){
        sort(voices[i].begin(), voices[i].end());
        results[voices[i][n - 1].second].first += 12;
        results[voices[i][n - 2].second].first += 10;
        for (int j = 8; j >= 1; j--){
            results[voices[i][n + j - 11].second].first += j;
        }
    }
    sort(results.begin(), results.end());
}

void printFirstCountries(int n, vector<string> &countries, vector<pair<int, int>> &results, int numOfCountries, ofstream &out){
    for (int i = n - 1; i >= n - numOfCountries; i--){
        out << countries[results[i].second] << ',' << results[i].first << endl;
    }
}

int main(){
    ifstream in("eurovision.csv");
    ofstream out("results.csv");
    if (in == NULL){
        cout << "Cannot open file." << endl;
    } else {
        int n;
        in >> n;
        vector<vector<pair<int, int>>> voices;
        vector<string> countries;
        vector<pair<int, int>> results;
        voices.resize(n);
        for (int i = 0; i < n; i++){
            string str;
            in >> str;
            getVoices(n, i, str, countries, voices);
        }
        calculateResults(n, voices, results);
        printFirstCountries(n, countries, results, 10, out);
    }
    in.close();
    out.close();
}
