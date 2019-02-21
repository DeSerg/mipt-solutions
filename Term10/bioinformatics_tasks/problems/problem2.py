import sys

import extern as ext


def run():
    with open(ext.FilenameIn) as f_in:
        genome = f_in.readline()

    g_count = 0
    c_count = 0

    min_diff = sys.maxsize
    min_diff_indices = []

    for ind, genome_symbol in enumerate(genome, 1):
        if genome_symbol == ext.CharGuanine:
            g_count += 1
        elif genome_symbol == ext.CharCytosine:
            c_count += 1

        diff_current = g_count - c_count
        if diff_current == min_diff:
            min_diff_indices.append(ind)
        elif diff_current < min_diff:
            min_diff = diff_current
            min_diff_indices = [ind]

    min_diff_indices = list(map(lambda v: str(v), min_diff_indices))
    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write(' '.join(min_diff_indices))
