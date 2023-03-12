#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

MovieDatabase::MovieDatabase()
{
    // Replace this line with correct code.
}

bool MovieDatabase::load(const string& filename)
{
    int count = 0;
    if(m_loaded) {return false;}

    ifstream infile(filename);
    if(!infile) {
//        cerr << "Error, file not loaded" << endl;
        return false;
    }

    while(infile) {
        count++;
        string id; getline(infile, id);
        string name; getline(infile, name);
        string year; getline(infile, year);
        string dirs; getline(infile, dirs);
        vector<string> dirsv = split(dirs);
        string actors; getline(infile, actors);
        vector<string> actorsv = split(actors);
        string genres; getline(infile, genres);
        vector<string> genresv = split(genres);
        float rating; infile >> rating; // Need to do the fix thing??
        infile.ignore(10000, '\n');

        
        Movie m(id, name, year, dirsv, actorsv, genresv, rating);
        m_movies.insert(id, m);

        string junk; getline(infile, junk);

    }

    return true;
    
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string,Movie>::Iterator it = m_movies.find(id);
    if(it.is_valid()) {
        return &it.get_value();
    }
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    return vector<Movie*>();  // Replace this line with correct code.
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    return vector<Movie*>();  // Replace this line with correct code.
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    return vector<Movie*>();  // Replace this line with correct code.
}

vector<string> MovieDatabase::split(string s) {
    string word = "";
    vector<string> vs;
    for (int i = 0; i < s.length(); i++) {
        if(s.at(i) == ',') {
            i++;
            vs.push_back(word);
            word = "";
            continue;
        }
        try {
        word += s.at(i);
        }
        catch(...) {
            cerr << s << "num" << i  << " length: " << s.length() << endl;
        }
    }
    if(word.size() > 0) {vs.push_back(word);}
    return vs;
}
