//
//  movie.hpp
//
//  Created by Gebremedhin Abreha on 21/09/2023.
//

#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <stdio.h>
#include <string>

class Movie {
    int mId;
    std::string mName;
public:
    Movie (const int& id, const std::string& name):
    mId(id),mName(name) { }
    
    inline std::string getName() const { return mName;}
    
    inline int getId () const { return mId; }
};

#endif /* MOVIE_HPP */
