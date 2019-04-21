import extern as ext
import problems.extern3 as ext3


def run():
    with open(ext.FilenameIn) as f_in:
        k = int(f_in.readline().strip())

    genome = ext3.circular_string(k)

    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write('{}\n'.format(genome))
