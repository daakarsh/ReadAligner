#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/search/search.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/std/span>
#include <seqan3/io/sequence_file/input.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <string>
#include <fstream>
#include <vector>
#include "seed.h"

 
using seqan3::operator""_dna5;

Reads parse_reads(std::string path) {
  Reads reads = Reads();
  std::ifstream file(path);
  std::string line;
  while(getline(file, line)) {
    std::string token;
    for (int i = 0; i < 3; i++) {
      getline(file, line);
      if (i == 0 && line.find("+")) {    // if we have a positive strand read
        std::string s, src, dir, seq;
        int startpos, len, refsize;
        std::stringstream ss(line);
        ss >> s >> src >> startpos >> len >> dir >> refsize >> seq;
        reads.positions.push_back(startpos);
      }
    }
  }
  return reads;
}

std::vector<seqan3::dna5_vector> parse_fastq(std::string path) {
  seqan3::sequence_file_input fin{path};
  std::vector<seqan3::dna5_vector> reads;
  for (auto & [seq, id, qual] : fin) {
    seqan3::dna5_vector read;
    for (auto s : seq) {
      read.push_back(s);
    }
    reads.push_back(read);
  }
  return reads;
}

seqan3::dna5_vector parse_fasta(std::string path) {
  seqan3::dna5_vector ref;
  seqan3::sequence_file_input fin{path};
  for (auto & [seq, id, qual] : fin) {
    for (auto s : seq) {
      ref.push_back(s);
    }
  }
  return ref;
}

void store_fmindex(seqan3::dna5_vector &ref) {
  seqan3::fm_index index{ref};
  {
    std::ofstream os{"index.file", std::ios::binary};
    cereal::BinaryOutputArchive oarchive{os};
    oarchive(index);
  }
}

// std::vector<int> seed(seqan3::dna5_vector &read, seqan3::dna5_vector &ref,
//                       int k, int gaps) {
//   seqan3::configuration const cfg = seqan3::search_cfg::max_error
//           {seqan3::search_cfg::substitution{(unsigned char)gaps}};
//   seqan3::fm_index index{ref};
//
//   std::vector<int> positions;
//   for (int i = 0; i < read.size() / k; i += k) {
//     seqan3::dna5_vector kmer;
//     int n = i + k + 1;
//     std::copy(read.begin() + i, read.begin() + n, std::back_inserter(kmer));
//     for (auto && pos : search(kmer, index, cfg)) {
//       positions.push_back(std::get<1>(pos));
//     }
//   }
//   return positions;
// }

// TODO: move this
struct cmd_arguments {
  std::filesystem::path ref_path{};
  std::filesystem::path reads_path{};
  std::filesystem::path maf_path{};
  int k;
  int gaps;
  int coverage;
};

void init_argparser(seqan3::argument_parser & parser, cmd_arguments & args) {
  parser.add_positional_option(args.ref_path, "provide a reference file (fasta)");
  parser.add_positional_option(args.reads_path, "provide a reads file (fastq)");
  parser.add_positional_option(args.maf_path, "provide a reads file (maf)");
  parser.add_option(args.k, 'k', "kmersize", "size of kmer to index");
  parser.add_option(args.gaps, 'g', "gaps", "gaps or wildcards allowed");
  parser.add_option(args.coverage, 'c', "coverage", "coverage low = 1, high = (k-1)");
}

int main(int argc, char ** argv)
{
  // parse args
  seqan3::argument_parser parser{"Basic-seeder", argc, argv};
  cmd_arguments args{};
  init_argparser(parser, args);

  try {
    parser.parse();
  } catch (seqan3::argument_parser_error const & ext) {
    seqan3::debug_stream << "[usage error] " << ext.what() << "\n";
    return -1;
  }

  seqan3::dna5_vector ref = parse_fasta(args.ref_path);
  Reads reads = parse_reads(args.maf_path);
  reads.sequences = parse_fastq(args.reads_path);

  int k = args.k;
  int gaps = args.gaps;
  int coverage = args.coverage;

  seqan3::fm_index index{ref};

  // store_fmindex(ref);

  for (int i = 0; i < reads.sequences.size(); i++) {
    int actualpos = reads.positions[i];
    seqan3::debug_stream << "ACTUAL POSITION: " << actualpos << '\n';
    std::vector<int> positions = seed(reads.sequences[i], index, k, gaps, coverage);
    for (int j = 0; j < positions.size(); j++) {
      if (positions[j] >= actualpos - 10 && positions[j] <= actualpos + 10) {
        seqan3::debug_stream << "closely matched at: " << positions[j] << '\n';
      }
    }
  }
}
