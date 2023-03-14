#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

MovieDatabase::MovieDatabase()
{
    m_loaded = false;
}

MovieDatabase::~MovieDatabase() {
    for(int i = 0; i < allMoviePtrs.size(); i++) {
        delete allMoviePtrs[i];
    }
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

        
        Movie* mPtr = new Movie(id, name, year, dirsv, actorsv, genresv, rating);
        m_movies.insert(id, mPtr);
        for(int i = 0; i < dirsv.size(); i++) {
            m_directorToMovies.insert(toLower(dirsv[i]), mPtr); // issue is im inserting the address of
        }
//        //testing
//        TreeMultimap<string,Movie*>::Iterator it = m_directorToMovies.find(dirsv[0]);
//        while(it.is_valid()) {
//            cerr << "Director: " << dirsv[0] << it.get_value()->get_title() << endl;
//            it.advance();
//        }
        
        
        for(int i = 0; i < actorsv.size(); i++) {
            m_actorToMovies.insert(toLower(actorsv[i]), mPtr);
        }
        for(int i = 0; i < genresv.size(); i++) {
            m_genreToMovies.insert(toLower(genresv[i]), mPtr);
        }
        string junk; getline(infile, junk);

    }

    m_loaded = true;
    return true;
    
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string,Movie*>::Iterator it = m_movies.find(id);
    if(it.is_valid()) {
        return it.get_value();
    }
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*> mv;
    TreeMultimap<string,Movie*>::Iterator it = m_directorToMovies.find(toLower(director));
    while(it.is_valid()) {
//        cerr << it.get_value()->get_title();
        mv.push_back(it.get_value());
        it.advance();
    }
    return mv;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    vector<Movie*> mv;
    TreeMultimap<string,Movie*>::Iterator it = m_actorToMovies.find(toLower(actor));
    while(it.is_valid()) {
//        cerr << it.get_value()->get_title();
        mv.push_back(it.get_value());
        it.advance();
    }
    return mv;}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector<Movie*> mv;
    TreeMultimap<string,Movie*>::Iterator it = m_genreToMovies.find(toLower(genre));
    while(it.is_valid()) {
//        cerr << it.get_value()->get_title();
        mv.push_back(it.get_value());
        it.advance();
    }
    return mv;}

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

string MovieDatabase::toLower(string s) const {
    string result;
    for(int i = 0; i < s.size(); i++) {
        result += tolower(s.at(i));
    }
    return result;
}
