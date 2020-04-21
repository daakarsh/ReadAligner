#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>
#include <queue>
#include "indexer.h"
#include "seqan/seeds.h"

class Seeder
{
private:
public:
    Seeder();
    ~Seeder();
    void extendLordFast(DnaString, DnaString, TIndex, unsigned);
    void extendRHat(DnaString, DnaString, unsigned, unsigned);
};
