#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include "Movie.h"
#include "treemm.h"

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    TreeMultimap<std::string,Movie*> m_movies;
    TreeMultimap<std::string,Movie*> m_directorToMovies;
    TreeMultimap<std::string,Movie*> m_actorToMovies;
    TreeMultimap<std::string,Movie*> m_genreToMovies;
    bool m_loaded;
    std::string toLower(std::string s) const;
    std::vector<std::string> split(std::string s);
    std::vector<Movie*> allMoviePtrs;
    
};

#endif // MOVIEDATABASE_INCLUDED
