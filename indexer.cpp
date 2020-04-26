#include "indexer.h"

Indexer::Indexer()
{
}

Indexer::~Indexer()
{
}

void Indexer::buildIndex(Dna5String seq)
{
    idx = TIndex(seq);
}

TIndex Indexer::getIndex()
{
    return idx;
}