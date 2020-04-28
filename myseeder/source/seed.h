#ifndef SEED_H
#define SEED_H

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/search/search.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <fstream>
#include <vector>

/*
 * seed.h
 */

struct Reads {
    std::vector<int> positions;
    std::vector<seqan3::dna5_vector> sequences;
};

std::vector<int> seed(seqan3::dna5_vector &read, seqan3::fm_index<seqan3::dna5, seqan3::text_layout::single> &index,
                      int k, int gaps, int coverage);

#endif
