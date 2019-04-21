import extern as ext
import problems.extern3 as ext3


def run():
    with open(ext.FilenameIn) as f_in:
        k = int(f_in.readline().strip())
        kmers = [line.strip() for line in f_in.readlines()]

    genome = ext3.reconstruct_dna(kmers)

    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write('{}\n'.format(genome))
