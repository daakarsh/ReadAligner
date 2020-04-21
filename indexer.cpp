#include "indexer.h"

Indexer::Indexer()
{
}

Indexer::~Indexer()
{
}

void Indexer::buildIndex(DnaString seq)
{
    idx = TIndex(seq);
}

TIndex Indexer::getIndex()
{
    return idx;
}