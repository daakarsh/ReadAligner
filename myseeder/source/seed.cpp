#include "seed.h"

/*
 * seed.cpp
 *
 * seed functions go here
 */

std::vector<int> seed(seqan3::dna5_vector &read, seqan3::fm_index<seqan3::dna5, seqan3::text_layout::single> &index,
                      int k, int gaps, int coverage) {
    seqan3::configuration const cfg = seqan3::search_cfg::max_error
          {seqan3::search_cfg::substitution{(unsigned char)gaps}};

    std::vector<int> positions;
    for (int i = 0; i < read.size() - k; i += (k - coverage)) {
        seqan3::dna5_vector kmer;
        int n = i + k + 1;
        std::copy(read.begin() + i, read.begin() + n, std::back_inserter(kmer));
        for (auto && pos : search(kmer, index, cfg)) {
            positions.push_back(std::get<1>(pos));
        }
    }
    return positions;
}
