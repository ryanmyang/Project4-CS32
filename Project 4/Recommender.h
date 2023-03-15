#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>


class UserDatabase;
class MovieDatabase;
class Movie;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    const UserDatabase* m_udb;
    const MovieDatabase* m_mdb;
    MovieAndRank movieToRankedMovie(Movie *m, std::unordered_map<std::string,int> &u_directors, std::unordered_map<std::string,int> &u_actors, std::unordered_map<std::string,int> &u_genres) const;
    template <typename one, typename two>
    void vecIntounordered_set(std::vector<one> v, std::unordered_set<two> &us) const;
    template <typename one, typename two>
    void vecIntounordered_map(std::vector<one> v, std::unordered_map<two,int> &um) const;
    bool compareMovieAndRanks(const Recommender& recommender, MovieAndRank& first, MovieAndRank& second) const;
};

#endif // RECOMMENDER_INCLUDED
