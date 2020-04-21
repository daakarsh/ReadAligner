#include "seeder.h"
#include "seqan/seq_io.h"
#include <fstream>

void seed_test()
{
    DnaString ref = "AGCTAGCTAGCTAGCTAGCT";
    DnaString read = "AGCTAAGCTTAGCCT";

    Seed<Simple> seed(0, 0, 4, 4);
    Score<int, Simple> scoringScheme(0, -1, -1);
    extendSeed(seed, ref, read, EXTEND_RIGHT, scoringScheme, 3, GappedXDrop());
    std::cout << infix(ref, beginPositionH(seed), endPositionH(seed)) << std::endl;
    std::cout << infix(read, beginPositionV(seed), endPositionV(seed)) << std::endl;
}

int main()
{
    String<char> id;
    DnaString ref;
    SeqFileIn refGenomeFile("sample.fasta");
    readRecord(id, ref, refGenomeFile);
    Indexer indexer;
    indexer.buildIndex(ref);
    StringSet<String<char>> ids;
    StringSet<DnaString> reads;
    StringSet<DnaString> quals;
    SeqFileIn readsFile("sd_0001.fastq");
    readRecords(ids, reads, quals, readsFile);
    std::string line;
    std::ifstream myFile("sd_0001.maf");
    getline(myFile, line);
    unsigned t = 0;
    Seeder seeder;
    while (getline(myFile, line))
    {
        size_t pos = 0;
        std::string token;
        for (int i = 0; i < 3; i++)
        {
            pos = line.find(" ");
            token = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        std::cout << token << std::endl;
        for (short i = 0; i < 3; i++)
        {
            getline(myFile, line);
        }
        seeder.extendLordFast(ref, reads[t], indexer.getIndex(), 32);
        // seeder.extendRHat(ref, reads[t], 14, 2048);
        t++;
    }
    myFile.close();
    // seed_test();
    return 0;
}
