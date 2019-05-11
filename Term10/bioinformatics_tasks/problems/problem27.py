import extern as ext
import problems.extern6 as ext6


def run():
    with open(ext.FilenameIn) as f_in:
        raw_permutation = f_in.readline().strip()[1:-1].split()

    list_permutation = ext6.rawPermutationToList(raw_permutation)
    N = len(list_permutation)
    list_permutation = [0] + list_permutation + [N + 1]
    breakpoints_number = ext6.breakpoints_number(list_permutation)

    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write('{}\n'.format(breakpoints_number))
