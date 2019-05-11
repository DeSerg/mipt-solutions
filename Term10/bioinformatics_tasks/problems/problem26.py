import extern as ext
import problems.extern6 as ext6


def run():
    with open(ext.FilenameIn) as f_in:
        raw_permutation = f_in.readline().strip()[1:-1].split()

    list_permutation = ext6.rawPermutationToList(raw_permutation)
    sorting = ext6.greedy_sorting(list_permutation)
    raw_sorting = list(map(lambda element: ext6.listPermutationToRaw(element), sorting))
    raw_lines = ['(' + ' '.join(perm) + ')' for perm in raw_sorting]

    with open(ext.FilenameOut, 'w') as f_out:
        for raw_line in raw_lines:
            f_out.write(raw_line)
            f_out.write('\n')
