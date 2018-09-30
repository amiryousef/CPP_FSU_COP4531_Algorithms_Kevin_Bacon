/*
KevinBacon.cpp
10/20/14

Copyright 2014, R.C. Lacher
*/


#include <moviematch.h>
#include <xstring.h>
#include <timer.h>

// in lieu of makefile
#include <xstring.cpp>
#include <bitvect.cpp>
#include <primes.cpp>
#include <hashfunctions.cpp>
#include <timer.cpp>
// */

// set up buffer system for read loop
static size_t buffSize = 100;
static char* buffer = new char [buffSize + 1];

fsu::String GetName (std::istream& is)
// reads to end of line, including blanks
{
size_t currSize = 0;
char x;
std::cin >> x;
while ((x != '\n') && (!is.eof()))
{
if (currSize == buffSize)  // need more buffer
{
buffSize += 100;
char* newbuffer = new char [buffSize + 1];
for (size_t i = 0; i < currSize; ++i)
{
newbuffer[i] = buffer[i];
}
delete [] buffer;
buffer = newbuffer;
}
buffer[currSize++] = x;
x = is.get();
}
buffer[currSize] = '\0';
return buffer;
} // end read block

int main(int argc, char* argv[])
{
if (argc < 3)
{
std::cout << "arguments required:\n"
<< " 1: database file name\n"
<< " 2: root actor name\n";
return 0;
}
bool VERBOSE = 0;
if (argc > 3)
VERBOSE = 1;

MovieMatch mm;

// set up timer for Load call
fsu::Timer timer;
fsu::Instant time;
timer.EventReset();
bool success = mm.Load(argv[1]);
time = timer.EventTime();
if (!success)
{
std::cout << " ** KB: Failure to Load " << argv[1] << '\n';
return EXIT_FAILURE;
}
else
{
std::cout << " Load time: ";
time.Write_seconds(std::cout,2);
std::cout << " sec\n";
}
if (!mm.Init(argv[2]))
{
std::cout << " ** failure to Init " << argv[2] << '\n';
return EXIT_FAILURE;
}
if (VERBOSE) mm.Dump(std::cout);
std::cout << "\nWelcome to MovieMatch ( " << argv[2] << " )\n";
fsu::String actor  = "1";
fsu::String answer = "yes";
long kbn = 0;

while (1)
{
std::cout << "Enter actor name ('0' to quit): ";
actor = GetName(std::cin);
if (actor[0] == '0') break;

kbn = mm.MovieDistance(actor.Cstr());
if (kbn == -3)
{
std::cout << " name \'" << actor << "\' not in DB \'" << argv[1] << "\'\n";
mm.Hint(actor,std::cout);
}
else if (kbn == -2)
{
std::cout << " name \'" << actor << "\' unreachable from base actor \'" << argv[2] << "\'\n";
mm.ShowStar(actor, std::cout);
}
else if (kbn == -1)
{
std::cout << " name \'" << actor << "\' is a movie, not an actor\n";
std::cout << "  Do you want details? ";
std::cin >> answer;
if (answer.Element(0) == 'y' || answer.Element(0) == 'Y')
{
std::cout << "   The movie is:\n";
mm.ShowStar(actor, std::cout);
}
}
else
{
std::cout << " The KB Number of \'" << actor << "\' is: "
<< kbn
<< '\n';
std::cout << "  Do you want proof? ";
std::cin >> answer;
if (answer.Element(0) == 'y' || answer.Element(0) == 'Y')
{
std::cout << "   A connecting path is:\n";
mm.ShowPath(std::cout);
std::cout << "   The path is minimal because it was found with BFS [ref graph theory].\n";
}
}
} // end while
delete [] buffer;
std::cout << "Thank you for playing Kevin Bacon\n";
return EXIT_SUCCESS;
}
