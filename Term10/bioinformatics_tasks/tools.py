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


def complement(pattern):

    pattern_compl = []
    for chr in pattern:
        if chr == ext.CharCytosine:
            pattern_compl.append(ext.CharGuanine)
        elif chr == ext.CharGuanine:
            pattern_compl.append(ext.CharCytosine)
        elif chr == ext.CharAdenine:
            pattern_compl.append(ext.CharThymine)
        elif chr == ext.CharThymine:
            pattern_compl.append(ext.CharAdenine)
        else:
            print('tools.py: complement: invalid char in pattern: {}'.format(chr))

    return ''.join(pattern_compl)