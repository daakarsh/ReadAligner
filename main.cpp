#include "seeder.h"
#include "seqan/seq_io.h"
#include <fstream>

// void seed_test()
// {
//     DnaString ref = "AGCTAGCTAGCTAGCTAGCT";
//     DnaString read = "AGCTAAGCTTAGCCT";

//     Seed<Simple> seed(0, 0, 4, 4);
//     Score<int, Simple> scoringScheme(0, -1, -1);
//     extendSeed(seed, ref, read, EXTEND_RIGHT, scoringScheme, 3, GappedXDrop());
//     std::cout << infix(ref, beginPositionH(seed), endPositionH(seed)) << std::endl;
//     std::cout << infix(read, beginPositionV(seed), endPositionV(seed)) << std::endl;
// }

int main()
{
    CharString id;
    Dna5String ref;
    SeqFileIn refGenomeFile("sample.fasta");
    readRecord(id, ref, refGenomeFile);
    Indexer indexer;
    indexer.buildIndex(ref);
    StringSet<CharString> ids;
    StringSet<Dna5String> reads;
    StringSet<Dna5String> quals;
    SeqFileIn readsFile("sd_0001.fastq");
    readRecords(ids, reads, quals, readsFile);
    std::cout << length(reads) << std::endl;
    std::string line;
    std::ifstream myFile("start_positions.txt");
    unsigned t = 0;
    unsigned ref_start;
    Seeder seeder;
    unsigned delta[6];
    for (unsigned i = 0; i < 6; i++)
    {
        delta[i] = 0;
    }
    std::vector<unsigned> candidateWindows;
    unsigned min_elem;
    while (getline(myFile, line))
    {
        // std::cout << line << std::endl;
        ref_start = std::stol(line);
        candidateWindows = seeder.extendLordFast(ref, reads[t], indexer, 12 /*change this to k used in index*/, 64);
        for (auto it = candidateWindows.begin(); it < candidateWindows.end(); it++)
        {
            *it = std::abs(int(*it - ref_start));
        }
        min_elem = *(min_element(candidateWindows.begin(), candidateWindows.end()));
        // std::cout << "Distance from start " << min_elem << std::endl;
        if (min_elem == 0)
        {
            delta[0]++;
        }
        if (min_elem <= 10)
        {
            delta[1]++;
        }
        if (min_elem <= 20)
        {
            delta[2]++;
        }
        if (min_elem <= 50)
        {
            delta[3]++;
        }
        if (min_elem <= 100)
        {
            delta[4]++;
        }
        if (min_elem <= 5000)
        {
            delta[5]++;
        }
        getline(myFile, line);
        t += 2;
        std::cout << "Progress: " << (t * 100) / length(reads) << "%" << std::endl;
        if (t % 50 == 0)
        {
            for (unsigned i = 0; i < 6; i++)
            {
                std::cout << "Delta " << i << " " << (2 * delta[i] * 100) / t << std::endl;
            }
        }
    }
    myFile.close();
    for (unsigned i = 0; i < 6; i++)
    {
        std::cout << "Delta " << i << " " << (2 * delta[i] * 100) / t << std::endl;
    }

    // seed_test();
    // for (unsigned i = 0; i < length(reads); i += 2)
    // {
    //     seeder.extendLordFast(ref, reads[i], indexer.getIndex(), 12, 256);
    //     std::cin.ignore();
    // }

    return 0;
}
