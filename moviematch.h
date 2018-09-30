/*
moviematch.h
Amir Yousef
11-21-2014
*/

#ifndef _MOVIEMATCH_H
#define _MOVIEMATCH_H

#include <stdint.h>			// uint32_t
#include <xstring.h>		// fsu::String
#include <graph.h>			// fsu::ALUGraph
#include <bfsurvey.h>		// fsu::BFSurvey
#include <hashclasses.h>	// hashclass::KISS
#include <hashtbl.h>		// fsu::HashTable
#include <vector.h>			// fsu::Vector
#include <graph_util.h>		// ShowAL
#include <survey_util.h>	// WriteData
#include <string.h>			// string
#include <cstdlib> 			// size_t
#include <iostream>			// std::cout
#include <fstream>			// ios::in|ios::out
#include <sstream>			// std::stringstream

// types used
typedef uint32_t Vertex;							// vertex
typedef fsu::String Name;              				// string
typedef fsu::ALUGraph <Vertex> Graph;				// graph
typedef fsu::BFSurvey <Graph> BFS;					// bfsurvey
typedef hashclass::KISS <Name> Hash;				// hash kiss
typedef fsu::HashTable <Name, Vertex, Hash> AA;  	// hash array
typedef fsu::Vector <Name> Vect;					// vector

class MovieMatch
{
public:
// default constructor
MovieMatch () : bfs_(g_)	{}

// this method uses the data in the file to build the underlying symbol graph for the game
bool Load (const char* filename)
{
size_t name = 0;
size_t movie = 0;
std::ifstream myFile;
myFile.open(filename);
std::string oneLine;
Line(myFile);
for(size_t i = 0; i < name_.Size(); i++)
{
g_.PushVertex();
++name;
for(size_t j = 0; j < name_[i].Size(); j++)
{
g_.PushVertex();
++name;
}
size_t vertex = 0;
g_.AddEdge(movie, vertex);
}
for(size_t k = movie + 1; k <= movie + name; k++)
{
g_.AddEdge(movie, k);
}
myFile.close();
return true;
}

// this method establishes actor as the base actor "Kevin Bacon" and performs a BFS from the base actor in the graph
bool Init (const char* actor)
{
if (strlen (actor) > 0)
return true;
else
return false;
}

// this method returns -3 (name is not in DB), -2 (name is not reachable) and -1 (name entered is a movie)
long MovieDistance (const char* actor)
{
size_t movie_ = 0;
size_t actor_ = 0;
size_t counter = 0;
movie_ = vrtx_.Get (actor);
bfs_.Search (actor_);
if(actor_ == movie_)
{
return -1;
}
else
if (bfs_.Parent()[actor_] != actor_)
{
actor_ = bfs_.Parent()[actor_];
counter++;
return -2;
}
else
{
std::cout << "Actor is not in the DB." << std::endl;
return -3;
}
}

// this method, depicts the internal structure of the MovieMatch objects
void Dump (std::ostream& os)
{
ShowAL (g_,os);
WriteData (bfs_,os);
vrtx_.Dump (os);
for (size_t i = 0; i < name_.Size(); ++i)
{
os << "name_[" << i << "] = " << name_[i] << '\t';
os << "vrtx_[" << name_[i] << "] = " << vrtx_[name_[i]] << '\n';
}
vrtx_.Analysis (std::cout);
}

// this method outputs the entire path as an actor-movie chain connecting actor_to baseActor_
void ShowPath (std::ostream& os) const
{
ShowAL (g_,os);
WriteData (bfs_,os);
vrtx_.Dump (os);
for (size_t i = 0; i < name_.Size(); ++i)
{
os << "name_[" << i << "] = " << name_[i] << '\t';
os << "vrtx_[" << name_[i] << "] = " << vrtx_[name_[i]] << '\n';
}
vrtx_.Analysis (std::cout);
}

// this method outputs the name stored in actor_ followed by the names of all vertices that are adjacent to actor_
void ShowStar (Name name, std::ostream& os) const
{
fsu::ALUGraph <Name>::AdjIterator i;
fsu::String actor = name;
ShowAL (g_, os);
WriteData (bfs_, os);
vrtx_.Dump (os);
for (size_t i = 0; i < path_.Size(); ++i)
{
os << name << "path_[" << i << "] = " << name_[i] << '\t';
os << name << "vrtx_[" << name_[i] << "] = " << vrtx_[name_[i]] << '\n';
}
vrtx_.Analysis (std::cout);
}

// this method provides hints intended to be helpful when a name is not found in the DB  
void Hint (Name name, std::ostream& os) const
{
fsu::ALUGraph <Name>::AdjIterator i;
fsu::String actor = name;
for (size_t i = 0; i < path_.Size(); ++i)
{
os << name << "path_[" << i << "] = " << name_[i] << '\t';
os << name << "vrtx_[" << name_[i] << "] = " << vrtx_[name_[i]] << '\n';
}
vrtx_.Analysis (std::cout);
}

private:
Graph g_;
BFS bfs_;
AA vrtx_;
Hash hash_;
Vect name_;
Name actor_;
Vertex vertex_;
fsu::List <Vertex> path_;

static fsu::Vector <Name> Line (std::ifstream& is)
{
fsu::String line;
char delim = '/';
line.GetLine(is);
char* name_buffer = new char [1 + line.Size()];
size_t pos = 0, next = 0;
fsu::Vector <Name> movie;
while (pos < line.Size ())
{
next = line.Position (delim,pos);
for (size_t i = pos; i < next; ++i)
{
name_buffer [i - pos]= line[i];
}
name_buffer [next-pos] = '\0';
movie.PushBack(name_buffer);
pos = ++next; // skip delimiter
}
delete [] name_buffer;
return movie;
}

};

#endif