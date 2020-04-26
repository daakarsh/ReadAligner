#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>
#include <queue>
#include <ctime>
#include "indexer.h"
#include "seqan/seeds.h"

class Seeder
{
private:
public:
    Seeder();
    ~Seeder();
    void extendLordFast(Dna5String, Dna5String, TIndex, unsigned, unsigned);
    // void extendRHat(CharString, CharString, unsigned, unsigned);
};
