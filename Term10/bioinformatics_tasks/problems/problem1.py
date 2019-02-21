import extern as ext

def calc_clumps(genome_window, clump_len, clump_min_count, nice_clump_set):

    clump_dict = {}

    for i in range(len(genome_window) - clump_len + 1):
        clump = genome_window[i:i + clump_len]
        if clump not in clump_dict:
            clump_dict[clump] = 0
        clump_dict[clump] += 1

    for clump, count in clump_dict.items():
        if count >= clump_min_count:
            nice_clump_set.add(clump)


def run():

    with open(ext.FilenameIn) as f_in:
        genome = f_in.readline()
        k, L, t = tuple(map(lambda v: int(v), f_in.readline().split()))

    genome_len = len(genome)
    if genome_len < L:
        print('run: len of genome {} is less than L ({})'.format(genome, L))

    nice_clump_set = set()

    for i in range(genome_len - L + 1):
        genome_window = genome[i : i + L]
        calc_clumps(genome_window, k, t, nice_clump_set)

    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write(' '.join(nice_clump_set))
