#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>

using namespace std;

string getCountry(FILE *in){
    string country;
    char ch;
    fgets(&ch, 2, in);
    while (ch != ','){
        country += ch;
        fgets(&ch, 2, in);
    }
    return country;
}

int getScore(int n, FILE *in){
    char ch;
    int score = 0;
    fgets(&ch, 2, in);
    while (ch != ',' && ch != '\n' && !feof(in)){
        int numOfCh = ch - '0';
        score += score * 10 + numOfCh;
        fgets(&ch, 2, in);
    }
    return score;
}

void getVoices(int n, int numOfCountry, vector<string> &countries, vector<vector<pair<int, int>>> &voices, FILE *in){
    countries.push_back(getCountry(in));
    for (int i = 0; i < n; i++){
        voices[i].push_back(make_pair(getScore(n, in), numOfCountry));
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

void printFirstCountries(int n, vector<string> &countries, vector<pair<int, int>> &results, int numOfCountries, FILE *out){
    for (int i = n - 1; i >= n - numOfCountries; i--){
        fprintf(out, "%s,%i\n", &countries[results[i].second][0], results[i].first);
    }
}

int main(){
    FILE *in = fopen("eurovision.csv", "r"), *out = fopen("results.csv", "w");
    if (in == NULL){
        cout << "Cannot open file." << endl;
    } else {
        int n;
        fscanf(in, "%i", &n);
        vector<vector<pair<int, int>>> voices;
        vector<string> countries;
        vector<pair<int, int>> results;
        voices.resize(n);
        for (int i = 0; i < n; i++){
            getVoices(n, i, countries, voices, in);
        }
        calculateResults(n, voices, results);
        printFirstCountries(n, countries, results, 10, out);
    }
    fclose(in);
    fclose(out);
}
