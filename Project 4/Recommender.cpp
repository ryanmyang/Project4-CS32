#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "User.h"

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    m_udb = &user_database;
    m_mdb = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = m_udb->get_user_from_email(user_email);
    
    vector<string> history = user->get_watch_history();
    unordered_map<string, int> all_directors;
    unordered_map<string, int> all_actors;
    unordered_map<string, int> all_genres;
    unordered_set<Movie*> all_movies;
    vector<MovieAndRank> result;
    
    // Go through every movie and make master list of all directors, actors, and genres that the person has watched
    for (int i = 0; i < history.size(); i++) {
        Movie* m = m_mdb->get_movie_from_id(history[i]);
        vector<string> dirs = m->get_directors();
        vector<string> actors = m->get_actors();
        vector<string> genres = m->get_genres();
        vecIntounordered_map<string, string>(dirs, all_directors);
        vecIntounordered_map<string, string>(actors, all_actors);
        vecIntounordered_map<string, string>(genres, all_genres);
    }
    
    // Add all movies of user's directors into all_movies
    // For each user director, add all movies with that director into unordered_set
    for (unordered_map<string,int>::iterator it = all_directors.begin(); it != all_directors.end(); it++) {
        vecIntounordered_set<Movie*, Movie*>(m_mdb->get_movies_with_director(it->first), all_movies);
    }
    
    // Add all movies of user's actors into all_movies
    for (unordered_map<string,int>::iterator it = all_actors.begin(); it != all_actors.end(); it++) {
        vecIntounordered_set<Movie*, Movie*>(m_mdb->get_movies_with_actor(it->first), all_movies);
    }
    // Add all movies of user's genres into all_movies
    for (unordered_map<string,int>::iterator it = all_genres.begin(); it != all_genres.end(); it++) {
        vecIntounordered_set<Movie*, Movie*>(m_mdb->get_movies_with_genre(it->first), all_movies);
    }
    
    // Remove history movies 39662 to 39548 ✓
    for(int i = 0; i < history.size(); i++) {
        all_movies.erase(m_mdb->get_movie_from_id(history[i]));
    }
    
    // Convert all into movie and ranks
    for (unordered_set<Movie*>::iterator it = all_movies.begin(); it != all_movies.end(); it++) {
        result.push_back( movieToRankedMovie(*it, all_directors, all_actors, all_genres) );
    }
    
    sort(result.begin(), result.end(), [&](MovieAndRank& a, MovieAndRank& b) {
        return compareMovieAndRanks(*this, a, b);});
    
    vector<MovieAndRank> finalResult;
    
    for(int i = 0; i < result.size() && i < movie_count; i++) {
        finalResult.push_back(result[i]);
    }
    return finalResult;
}

MovieAndRank Recommender::movieToRankedMovie(Movie *m, unordered_map<string,int> &u_directors, unordered_map<string,int> &u_actors, unordered_map<string,int> &u_genres) const {
    int score = 0;
    vector<string> dirs = m->get_directors();
    vector<string> acts = m->get_actors();
    vector<string> gens = m->get_genres();
    
    // For each dir of movie, search and add if user has
    for(int i = 0; i < dirs.size(); i++) {
        score += 20 * u_directors[dirs[i]];
    }
    // For each actor of movie, search and add if user has
    for(int i = 0; i < acts.size(); i++) {
        score += 30 * u_actors[acts[i]];
    }
    // For each genre of movie, search and add if user has
    for(int i = 0; i < gens.size(); i++) {
        score += 1 * u_genres[gens[i]];
    }
    
    
    return MovieAndRank(m->get_id(), score);
}

template <typename one, typename two>
void Recommender::vecIntounordered_set(vector<one> v, unordered_set<two> &us) const {
    for(int i = 0; i < v.size(); i++) {
        us.insert(v[i]);
    }
}

template <typename one, typename two>
void Recommender::vecIntounordered_map(vector<one> v, unordered_map<two,int> &um) const {
    for(int i = 0; i < v.size(); i++) {
        um[v[i]]++;
    }
}

bool Recommender::compareMovieAndRanks(const Recommender& recommender, MovieAndRank& first, MovieAndRank& second) const {    // Put before if score is greater than next
    if (first.compatibility_score > second.compatibility_score) {
        return true;
    } else if (first.compatibility_score < second.compatibility_score) {
        return false;
    }
    Movie* m1 = recommender.m_mdb->get_movie_from_id(first.movie_id);
    Movie* m2 = recommender.m_mdb->get_movie_from_id(second.movie_id);

    
    if(m1->get_rating() > m2->get_rating()) {
        return true;
    } else if(m1->get_rating() < m2->get_rating()) {
        return false;
    }

    return (m1->get_title() < m2->get_title());
    
    
}
