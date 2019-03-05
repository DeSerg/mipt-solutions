import extern as ext
import problems.extern2 as ext2


def run():
    with open(ext.FilenameIn) as f_in:
        pattern = f_in.readline().strip()
        strings = f_in.readline().strip().split()

    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write('{}\n'.format(ext2.distanceBetweenPatternAndStrings(pattern, strings)))