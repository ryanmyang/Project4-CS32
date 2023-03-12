#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>

#include <iostream>
#include <fstream>

using namespace std;

UserDatabase::UserDatabase()
{
    m_loaded = false;
}

bool UserDatabase::load(const string& filename)
{
    if(m_loaded) {return false;}
    
    ifstream infile(filename);
    if(!infile) {
//        cerr << "Error, file not loaded" << endl;
        return false;
    }
    
    while(infile) {
        string name;
        if(!getline(infile, name)) {break;};
        string email; getline(infile, email);
        vector<string> movies;
        int numMovies; infile >> numMovies; // Need to do the fix thing??
        infile.ignore(10000, '\n');
        for(int i = 0; i < numMovies; i++) {
            string movie; getline(infile, movie);
            movies.push_back(movie);
        }
        User u(name, email, movies);
        m_users.insert(email, u);
        
        string junk;
        if(!getline(infile, junk)) {break;};

    }
    
    return true;
    
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string,User>::Iterator it = m_users.find(email);
    if(it.is_valid()) {
        return &it.get_value();
    }
    return nullptr;
}
