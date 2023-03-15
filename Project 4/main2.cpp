#include <cassert>
#include <iostream>
#include <chrono>
#include <string>
#include "UserDatabase.h"
#include "User.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "Recommender.h"
#include "treemm.h"

  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to made debugging easier, so you can replace the string
  // literals with the names of those smaller files.

const std::string USER_DATAFILE1  = "users1.txt";
const std::string MOVIE_DATAFILE1 = "movies1.txt";
const std::string USER_DATAFILE = "users.txt";
const std::string MOVIE_DATAFILE = "movies.txt";

int main()
{
	std::cout << "Running tests." << std::endl;
	

	auto start = std::chrono::steady_clock::now();

	UserDatabase udb;
	assert(udb.load(USER_DATAFILE));

	auto stop = std::chrono::steady_clock::now();

	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) << "ms to load USER_DATAFILE." << std::endl;

	start = std::chrono::steady_clock::now();

	assert(udb.get_user_from_email("AbFow2483@charter.net")->get_full_name() == "Abdullah Fowler");
	assert(udb.get_user_from_email("LOwen62@mail.com")->get_watch_history().size() == 41);
	assert(udb.get_user_from_email("KellaDel78@aol.com")->get_full_name() == "Kellan Delaney");
	assert(udb.get_user_from_email("LOWEN62@mail.com") == nullptr); // search must be case-sensitive
	assert(udb.get_user_from_email("LOwen62@mail.co") == nullptr);

	stop = std::chrono::steady_clock::now();

	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) << "ms to load a few users." << std::endl;

	start = std::chrono::steady_clock::now();

	MovieDatabase mdb;
	assert(mdb.load(MOVIE_DATAFILE));

	stop = std::chrono::steady_clock::now();

	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) << "ms to load MOVIE_DATAFILE." << std::endl;

	start = std::chrono::steady_clock::now();

	// CASE INSENSITIVE SEARCHES
	const Movie* arbalest = mdb.get_movie_from_id("Id02298");
	assert(arbalest != nullptr && arbalest->get_title() == "The Arbalest");
	assert(mdb.get_movie_from_id("ID0") == nullptr);
	const std::vector<Movie*> direcs = mdb.get_movies_with_director("JIM SHARMAN");
	assert(!direcs.empty());
	const std::vector<Movie*> actors = mdb.get_movies_with_actor("Keanu ReeveS");
	assert(!actors.empty());
	const std::vector<Movie*> genres = mdb.get_movies_with_genre("crime");
	assert(!genres.empty());
	const std::vector<Movie*> emptyGenres = mdb.get_movies_with_genre("Cri");
	assert(emptyGenres.empty());

	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) << "ms to load some movies." << std::endl;

	stop = std::chrono::steady_clock::now();

	start = std::chrono::steady_clock::now();

	UserDatabase udb1;
	assert(udb1.load(USER_DATAFILE1));

	stop = std::chrono::steady_clock::now();

	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) << "ms to load USER_DATAFILE1." << std::endl;

	start = std::chrono::steady_clock::now();

	assert(udb1.get_user_from_email("climberkip@gmail.com") != nullptr);
	assert(udb1.get_user_from_email("caimberkip@gmail.com") == nullptr);
	assert(udb1.get_user_from_email("czimberkip@gmail.com") == nullptr);

	MovieDatabase mdb1;
	assert(mdb1.load(MOVIE_DATAFILE1));

	stop = std::chrono::steady_clock::now();

	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) << "ms to load MOVIE_DATAFILE1." << std::endl;

	start = std::chrono::steady_clock::now();

	Recommender r(udb1, mdb1);
	const std::vector<MovieAndRank> recommendations = r.recommend_movies("climberkip@gmail.com", 100);

	stop = std::chrono::steady_clock::now();

	std::cout << "Took " << (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) << "ms to recommend movies." << std::endl;

	assert(recommendations.size() == 5);
	assert(recommendations.at(0).movie_id == "ID00004"
	&& recommendations.at(1).movie_id == "ID00008"
	&& recommendations.at(2).movie_id == "ID00007"
	&& recommendations.at(3).movie_id == "ID00005"
	&& recommendations.at(4).movie_id == "ID00006");
	assert(recommendations.at(0).compatibility_score == 166
	&& recommendations.at(1).compatibility_score == 32
	&& recommendations.at(2).compatibility_score == 2
	&& recommendations.at(3).compatibility_score == 2
	&& recommendations.at(4).compatibility_score == 2);

	const std::vector<MovieAndRank> recommendations2 = r.recommend_movies("climberkip@gmail.com", -5);
	assert(recommendations2.empty());

	TreeMultimap<int, int>::Iterator it1;  // doesn't point to any value
	assert(!it1.is_valid());


	TreeMultimap<std::string, int> tmm;
	tmm.insert("carey", 5);
	tmm.insert("carey", 7);
	tmm.insert("carey", 6);
	tmm.insert("david", 25);
	tmm.insert("david", 425);
	TreeMultimap<std::string, int>::Iterator it = tmm.find("carey"); // prints 5, 6, and 7 in some order
	assert(it.is_valid());
	bool a = false, b = false, c = false;
	int n = 0;
	while (it.is_valid())
	{
		if (it.get_value() == 5) a = true;
		else if (it.get_value() == 6) b = true;
		else if (it.get_value() == 7) c = true;
		++n;
		it.advance();
	}
	assert(n == 3);
	assert(a && b && c); // iterator iterates through 5, 6, 7
	it = tmm.find("laura");
	assert(!it.is_valid());

	int testInt = 5;
	TreeMultimap<std::string, int> f;
	f.insert("test", testInt);
	testInt = 10;
	it = f.find("test");
	assert(it.is_valid() && it.get_value() == 5); // TreeMultimap must store a COPY of the value

	
	std::cout << "Passed all tests." << std::endl;
}
