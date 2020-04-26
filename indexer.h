#include "seqan/index.h"

using namespace seqan;

/* QGram index of size k=12 */
typedef Index<Dna5String, IndexQGram<UngappedShape<12>>> TIndex;

class Indexer
{
private:
    TIndex idx;

public:
    Indexer();
    ~Indexer();
    void buildIndex(Dna5String seq);
    TIndex getIndex();
};
