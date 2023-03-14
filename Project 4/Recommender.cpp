#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "User.h"

#include <string>
#include <vector>
#include <algorithm>
#include <set>
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
    set<string> all_directors;
    set<string> all_actors;
    set<string> all_genres;
    set<Movie*> all_movies;
    vector<MovieAndRank> result;
    
    // Go through every movie and make master list of all directors, actors, and genres that the person has watched
    for (int i = 0; i < history.size(); i++) {
        Movie* m = m_mdb->get_movie_from_id(history[i]);
        vector<string> dirs = m->get_directors();
        vector<string> actors = m->get_actors();
        vector<string> genres = m->get_genres();
        vecIntoSet<string, string>(dirs, all_directors);
        vecIntoSet<string, string>(actors, all_actors);
        vecIntoSet<string, string>(genres, all_genres);
    }
    
    // Add all movies of user's directors into all_movies
    // For each user director, add all movies with that director into set
    for (set<string>::iterator it = all_directors.begin(); it != all_directors.end(); it++) {
        vecIntoSet<Movie*, Movie*>(m_mdb->get_movies_with_director(*it), all_movies);
    }
    
    // Add all movies of user's actors into all_movies
    for (set<string>::iterator it = all_actors.begin(); it != all_actors.end(); it++) {
        vecIntoSet<Movie*, Movie*>(m_mdb->get_movies_with_actor(*it), all_movies);
    }
    // Add all movies of user's genres into all_movies
    for (set<string>::iterator it = all_genres.begin(); it != all_genres.end(); it++) {
        vecIntoSet<Movie*, Movie*>(m_mdb->get_movies_with_genre(*it), all_movies);
    }
    
    // Remove history movies 39662 to 39548 ✓
    for(int i = 0; i < history.size(); i++) {
        all_movies.erase(m_mdb->get_movie_from_id(history[i]));
    }
    
    // Convert all into movie and ranks
    for (set<Movie*>::iterator it = all_movies.begin(); it != all_movies.end(); it++) {
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

MovieAndRank Recommender::movieToRankedMovie(Movie *m, set<string> &u_directors, set<string> &u_actors, set<string> &u_genres) const {
    int score = 0;
    vector<string> dirs = m->get_directors();
    vector<string> acts = m->get_actors();
    vector<string> gens = m->get_genres();
    
    // For each dir of movie, search and add if user has
    for(int i = 0; i < dirs.size(); i++) {
        if(u_directors.find(dirs[i]) != u_directors.end()) {
            score += 20;
        }
    }
    // For each actor of movie, search and add if user has
    for(int i = 0; i < acts.size(); i++) {
        if(u_actors.find(acts[i]) != u_actors.end()) {
            score += 30;
        }
    }
    // For each genre of movie, search and add if user has
    for(int i = 0; i < gens.size(); i++) {
        if(u_genres.find(gens[i]) != u_genres.end()) {
            score += 1;
        }
    }
    
    
    return MovieAndRank(m->get_id(), score);
}

template <typename one, typename two>
void Recommender::vecIntoSet(vector<one> v, set<two> &us) const {
    for(int i = 0; i < v.size(); i++) {
        us.insert(v[i]);
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
