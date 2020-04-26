#include "seeder.h"

Seeder::Seeder()
{
}

Seeder::~Seeder()
{
}

struct Anchor
{
    unsigned r, g, s;
    Anchor(unsigned r, unsigned g, unsigned s)
    {
        this->r = r;
        this->g = g;
        this->s = s;
    }
};

bool comparator(const Anchor &lhs, const Anchor &rhs)
{
    return lhs.s < rhs.s;
}

void Seeder::extendLordFast(Dna5String refGenome, Dna5String read, TIndex idx, unsigned k, unsigned Space)
{
    time_t start_time = time(NULL);
    std::vector<Anchor> matches;
    Seed<Simple> seed;
    Finder<TIndex> finder(idx);
    Dna5String anchor;
    for (unsigned i = 0; i <= length(read) - k; i += Space)
    {
        anchor = infix(read, i, i + k);
        clear(finder);
        while (find(finder, anchor))
        {
            seed = Seed<Simple>(beginPosition(finder), i, endPosition(finder), i + k);
            Score<int, Simple> scoringScheme(0, -1, -1);
            extendSeed(seed, refGenome, read, EXTEND_RIGHT, scoringScheme, 2, GappedXDrop());
            matches.push_back(Anchor(i, beginPosition(finder), endPositionH(seed) - beginPositionH(seed)));
        }
        if (time(NULL) - start_time > 60)
        {
            std::cout << "Time Limit Exceeded" << std::endl;
            return;
        }
    }
    std::cout << "matches done" << std::endl;
    std::vector<Anchor> longestMatches;
    unsigned max_elem = (*(max_element(matches.begin(), matches.end(), &comparator))).s;
    for (auto it = matches.begin(); it < matches.end(); it++)
    {
        if ((*it).s == max_elem)
        {
            longestMatches.push_back(*it);
        }
    }
    std::cout << "longest matches done" << std::endl;
    matches.clear();
    matches.shrink_to_fit();
    std::vector<unsigned> windowScore(length(refGenome) / length(read));
    for (unsigned i = 0; i < length(refGenome); i += length(read))
    {
        for (auto it = longestMatches.begin(); it != longestMatches.end(); it++)
        {
            if ((*it).g > i && (*it).g + (*it).s < i + (2 * length(read)))
            {
                windowScore[(i / length(read))] += ((*it).s);
            }
        }
    }
    std::cout << "window scores done" << std::endl;
    std::vector<unsigned> candidateWindow;
    max_elem = *(max_element(windowScore.begin(), windowScore.end()));
    for (unsigned i = 0; i < (length(refGenome) / length(read)); i++)
    {
        if (windowScore[i] > (max_elem / 2))
        {
            candidateWindow.push_back(i);
        }
    }
    std::cout << "candidate windows done" << std::endl;
    windowScore.clear();
    windowScore.shrink_to_fit();
    std::cout << "Candidate Window old: ";
    for (auto it = candidateWindow.begin(); it != candidateWindow.end(); it++)
    {
        std::cout << (*it) * length(read) << " ";
    }
    std::cout << std::endl;

    bool found;
    for (auto it = candidateWindow.begin(); it != candidateWindow.end(); it++)
    {
        found = false;
        for (unsigned i = 0; i <= length(read) - k; i++)
        {
            anchor = infix(read, i, i + k);
            clear(finder);
            while (find(finder, anchor))
            {
                if (beginPosition(finder) > ((*it) * length(read)) + i && beginPosition(finder) < (((*it) * length(read)) + i + length(read)))
                {
                    *it = beginPosition(finder) - i;
                    found = true;
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
        if (!found)
        {
            *it = length(refGenome);
        }
    }

    std::cout << "Candidate Window new: ";
    for (auto it = candidateWindow.begin(); it != candidateWindow.end(); it++)
    {
        if (*it < length(refGenome))
        {
            std::cout << (*it) << " ";
        }
    }
    std::cout << std::endl;
}

// void Seeder::extendRHat(Dna5String refGenome, Dna5String read, unsigned k, unsigned L)
// {
//     std::map<DnaString, std::vector<unsigned>> windowLists;
//     DnaString kmer;
//     for (unsigned i = 0; i < (2 * length(refGenome) / L); i++)
//     {
//         for (unsigned j = 0; j <= L - k; j++)
//         {
//             kmer = infix(refGenome, (i * L / 2) + j, (i * L / 2) + j + k);
//             if (windowLists.find(kmer) == windowLists.end())
//             {
//                 std::vector<unsigned> windowList;
//                 windowList.push_back(i);
//                 windowLists.insert(std::pair<DnaString, std::vector<unsigned>>(kmer, windowList));
//             }
//             else
//             {
//                 if (windowLists[kmer].back() != i)
//                 {
//                     windowLists[kmer].push_back(i);
//                 }
//             }
//         }
//     }
//     DnaString seed = infix(read, length(read) - (L / 4), length(read) + (L / 4));
//     std::vector<unsigned> windowScores(windowLists.size());
//     for (unsigned i = 0; i <= (L / 2) - k; i++)
//     {
//         kmer = infix(read, i, i + k);
//         for (auto it = windowLists[kmer].begin(); it != windowLists[kmer].end(); it++)
//         {
//             windowScores[*it]++;
//         }
//     }
//     std::cout << "Candidate Window: ";
//     std::priority_queue<std::pair<unsigned, int>> q;
//     for (unsigned i = 0; i < windowScores.size(); ++i)
//     {
//         q.push(std::pair<unsigned, int>(windowScores[i], i));
//     }
//     int M = 4; // number of indices we need
//     for (int i = 0; i < M; ++i)
//     {
//         int Mi = q.top().second;
//         std::cout << Mi * (L / 2) << " ";
//         q.pop();
//     }
//     std::cout << std::endl;
// }
