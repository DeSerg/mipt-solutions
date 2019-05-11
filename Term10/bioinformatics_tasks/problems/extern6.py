from copy import copy


def rawPermutationToList(raw_permutation):
    result = []
    for element in raw_permutation:
        result.append(int(element))

    return result


def listPermutationToRaw(list_permutation):
    result = []
    for element in list_permutation:
        if element < 0:
            result.append(str(element))

        else:
            result.append('+' + str(element))

    return result


def reverse_in_permutation(permutation, from_index, to_index):
    reversed_part = permutation[from_index: to_index + 1][::-1]
    reversed_part = list(map(lambda x: -x, reversed_part))
    first_part = permutation[:from_index]
    last_part = permutation[to_index + 1:]
    return first_part + reversed_part + last_part


def greedy_sorting(perm):

    answer = []
    for i in range(len(perm)):
        target = i + 1
        if perm[i] == target:
            answer.append(copy(perm))
            continue

        try:
            t_ind = perm.index(target)
            perm = reverse_in_permutation(perm, i, t_ind)
            answer.append(copy(perm))

            perm[i] = target
            answer.append(copy(perm))

        except:
            t_ind = perm.index(-target, i)
            perm = reverse_in_permutation(perm, i, t_ind)
            answer.append(copy(perm))

    return answer


def breakpoints_number(permutation):
    count = 0
    for i in range(1, len(permutation)):
        prev = permutation[i - 1]
        cur = permutation[i]

        if (prev >= 0 and prev + 1 == cur) or (prev < 0 and prev + 1 == cur):
            continue

        count += 1

    return count
