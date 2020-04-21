#include "seqan/index.h"

using namespace seqan;

/* QGram index of size k=12 */
typedef Index<DnaString, IndexQGram<UngappedShape<12>>> TIndex;

class Indexer
{
private:
    TIndex idx;

public:
    Indexer();
    ~Indexer();
    void buildIndex(DnaString seq);
    TIndex getIndex();
};
