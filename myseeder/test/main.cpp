#include "seqan/seq_io.h"
#include "seqan/seeds.h"
#include <fstream>
#include <iostream>
#include "myseeder.h"

int main(int argc, char **argv) {

  if (argc != 4) {
    printf("Usage: ./main <reference.fasta> <reads.fastq> <reads.maf>\n");
    exit(0);
  }
  

  return 0;
}
