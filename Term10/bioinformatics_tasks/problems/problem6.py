import extern as ext
import problems.extern2 as ext2

def run():
    with open(ext.FilenameIn) as f_in:
        k, t = tuple(map(lambda v: int(v), f_in.readline().split()))
        dna = []
        for i in range(t):
            dna.append(f_in.readline().strip())

    best_motifs = ext2.greed_motif_search(dna, k, pseudocounts=False)
    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write('\n'.join(best_motifs))
