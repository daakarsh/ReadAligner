# install biopython before running this script
# conda install -c conda-forge biopython

from Bio import AlignIO

outfile = open('start_positions.txt', 'w+')

for multiple_alignment in AlignIO.parse("./ReadAligner/sd_0001.maf", "maf"):
    # print("printing a new multiple alignment")

    for seqrec in multiple_alignment:
        outfile.write(str(seqrec.annotations["start"])+'\n')
        # print("starts at %s on the %s strand of a sequence %s in length, and runs for %s bp" %
        #       (seqrec.annotations["start"],
        #        seqrec.annotations["strand"],
        #        seqrec.annotations["srcSize"],
        #        seqrec.annotations["size"]))
        break
