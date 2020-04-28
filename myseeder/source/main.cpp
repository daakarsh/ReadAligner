#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/search/search.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/std/span>
#include <seqan3/io/sequence_file/input.hpp>
#include <string>
#include <fstream>
 
using seqan3::operator""_dna5;

struct Reads {
  std::vector<int> positions;
  std::vector<seqan3::dna5_vector> sequences;
};
 
Reads parse_reads(std::string path) {
  Reads reads = Reads();
  std::ifstream file(path);
  std::string line;
  while(getline(file, line)) {
    int startpos = 0;
    int endpos = 0;
    std::string token;
    for (int i = 0; i < 3; i++) {
      getline(file, line);
      if (i == 0 && line.find("+")) {    // if we have a positive strand read
        std::string s, src, dir, seq;
        int startpos, len, refsize;
        std::stringstream ss(line);
        ss >> s >> src >> startpos >> len >> dir >> refsize >> seq;
        //std::vector<std::string> tokens;
        //for (int f = 0; f < 7; f++) {    // store the position and sequence
        //  startpos = endpos;
        //  endpos = line.find(" ");
        //  token = line.substr(startpos, endpos);
        //  tokens.push_back(token);
        //}
        // seqan3::debug_stream << "read----------------------------------" << '\n';
        // seqan3::debug_stream << "startpos: " << startpos << '\n';
        reads.positions.push_back(startpos);
        seqan3::dna5_vector dnaseq = seq;
        reads.sequences.push_back(dnaseq);
      }
    }
  }
  return reads;
}

seqan3::dna5_vector parse_fasta(std::string path) {
  seqan3::dna5_vector ref;
  seqan3::sequence_file_input fin{path};
  for (auto & [seq, id, qual] : fin) {
    seqan3::debug_stream << "reading in: " << id << '\n';
    for (auto s : seq) {
      ref.push_back(s);
    }
  }
  return ref;
}

int main()
{
  seqan3::dna5_vector
                text{"C-CTGTCTGAAGGATGAGTGTCAGCCAGTGTAACCCGATGAGCTACCCAGTAGTCGAACTGGGCCAGACAACCCGGCGCTAATGCACTCA"_dna5};
  seqan3::dna5_vector
                read{"CAC"_dna5};


  // test
  seqan3::dna5_vector ref = parse_fasta("small.fasta");
  Reads reads = parse_reads("small_0001.maf");
  seqan3::debug_stream << reads.positions << '\n';
  seqan3::debug_stream << reads.sequences << '\n';
  seqan3::debug_stream << ref << '\n';
  seqan3::debug_stream << "==========\n";

  seqan3::fm_index index{text};
 
  seqan3::configuration const cfg = seqan3::search_cfg::max_error{seqan3::search_cfg::substitution{1}};
 
  // for (auto && pos : search(read, index, cfg))
  // {
  //   seqan3::debug_stream << "At position " << pos << ": "
  //                        << std::span{std::data(text) + pos.second, 3}
  //                        << '\n';
  // }
}
