import extern as ext
import tools


def obtain_pattern(pattern, genome, d, patterns_map):
    frequency = 0
    pattern_rev = tools.complement(pattern[::-1])
    for i in range(len(genome) - len(pattern) + 1):
        genome_window = genome[i:i + len(pattern)]
        d_curr = tools.str_diff(pattern, genome_window)
        if d_curr <= d:
            frequency += 1

        d_curr_rev = tools.str_diff(pattern_rev, genome_window)
        if d_curr_rev <= d:
            frequency += 1

    if frequency not in patterns_map:
        patterns_map[frequency] = []

    patterns_map[frequency].append(pattern)


def iterate_strings(s, len_total, string_method):
    if len_total == len(s):
        string_method(s)
        return

    for chr in ext.NucleotideChars:
        pattern = s + chr
        iterate_strings(pattern, len_total, string_method)


def run():
    with open(ext.FilenameIn) as f_in:
        genome = f_in.readline().strip()
        k, d = tuple(map(lambda v: int(v), f_in.readline().split()))

    patterns_map = {}

    iterate_strings('', k, lambda s: obtain_pattern(s, genome, d, patterns_map))

    max_key = max(patterns_map)
    max_patterns = patterns_map[max_key]

    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write(' '.join(max_patterns))
