import sys

import extern as ext


def str_diff(str1, str2):
    if len(str1) != len(str2):
        print('problem3: str_diff: str1 length {} not equal to str2 length {}'.format(len(str1), len(str2)))
        return sys.maxsize

    diff_count = 0
    for i in range(len(str1)):
        if str1[i] != str2[i]:
            diff_count += 1

    return diff_count


def run():
    with open(ext.FilenameIn) as f_in:
        pattern = f_in.readline().strip()
        genome = f_in.readline().strip()
        d = int(f_in.readline())

    positions = []

    for i in range(len(genome) - len(pattern) + 1):
        genome_window = genome[i:i + len(pattern)]
        d_curr = str_diff(pattern, genome_window)
        if d_curr <= d:
            positions.append(str(i))

    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write(' '.join(positions))
