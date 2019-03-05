import operator
import functools
import numpy

import random
from copy import copy


def check_motifs(motifs):
    assert(motifs)
    assert(motifs[0])
    width = len(motifs[0])
    assert(all([len(motif) == width for motif in motifs]))


def hamming2(s1, s2):
    assert len(s1) == len(s2)
    return sum(c1 != c2 for c1, c2 in zip(s1, s2))


def find_consensus(motifs):
    check_motifs(motifs)
    width = len(motifs[0])

    consensus = []
    for i in range(width):
        count = {}
        for motif in motifs:
            count[motif[i]] = count.get(motif[i], 0) + 1
        consensus.append(max(count.items(), key=operator.itemgetter(1))[0])

    return consensus


ProbsInit = {'A': 0, 'T': 0, 'G': 0, 'C': 0}
ProbsInitPseudoCounts = {'A': 1, 'T': 1, 'G': 1, 'C': 1}


def find_probs(motifs, pseudocounts):
    check_motifs(motifs)
    width = len(motifs[0])
    height = len(motifs)
    probs = []

    for i in range(width):
        d = ProbsInitPseudoCounts if pseudocounts else ProbsInit
        probs.append(copy(d))
        for j in range(height):
            nucl = motifs[j][i]
            probs[i][nucl] += 1

    return probs


def prob(kmer, probs):
    return functools.reduce(operator.mul, [probs[i][nucl] for i, nucl in enumerate(kmer)], 1)


def find_motifs(probs, dna):
    k = len(probs)
    width = len(dna[0])

    motifs = []
    for line in dna:

        best_kmer = line[:k]
        max_prob = prob(best_kmer, probs)

        for p in range(1, width - k + 1):
            kmer = line[p: p + k]
            cprob = prob(kmer, probs)
            if cprob > max_prob:
                max_prob = cprob
                best_kmer = kmer

        motifs.append(best_kmer)

    return motifs


def score(motifs):
    consensus = find_consensus(motifs)
    sum = 0
    for motif in motifs:
        sum += hamming2(consensus, motif)

    return sum


def distanceBetweenPatternAndStrings(pattern, dna):
    check_motifs(dna)
    width = len(dna[0])

    k = len(pattern)
    distance = 0
    for line in dna:
        min_dist = hamming2(pattern, line[:k])
        for p in range(1, width - k + 1):
            kmer = line[p: p + k]
            dist = hamming2(pattern, kmer)
            if dist < min_dist:
                min_dist = dist

        distance += min_dist

    return distance


def greed_motif_search(dna, k, pseudocounts=False):
    check_motifs(dna)
    width = len(dna[0])

    best_motifs = []
    best_motifs_score = len(dna) * width

    for i in range(width - k + 1):
        motifs = [dna[0][i: i + k]]
        for j in range(1, len(dna)):
            probs = find_probs(motifs, pseudocounts)
            best_kmer = dna[j][:k]
            max_prob = prob(best_kmer, probs)

            for p in range(1, width - k + 1):
                kmer = dna[j][p: p + k]
                cprob = prob(kmer, probs)
                if cprob > max_prob:
                    max_prob = cprob
                    best_kmer = kmer

            motifs.append(best_kmer)

        motifs_score = score(motifs)
        if motifs_score < best_motifs_score:
            best_motifs = motifs
            best_motifs_score = motifs_score

    return best_motifs


def randomizedMitifSearch(dna, k):
    check_motifs(dna)
    width = len(dna[0])

    best_best_motifs = None
    best_best_score = len(dna) * width

    for i in range(1000):
        best_motifs = []
        for line in dna:
            start = random.randint(0, width - k)
            best_motifs.append(line[start: start + k])

        min_score = score(best_motifs)
        while True:
            probs = find_probs(best_motifs, pseudocounts=True)
            motifs = find_motifs(probs, dna)

            cscore = score(motifs)
            if cscore < min_score:
                best_motifs = motifs
                min_score = cscore

            else:
                if min_score < best_best_score:
                    best_best_motifs = best_motifs
                    best_best_score = min_score
                break

    return best_best_motifs


def generate_motif(line, probs):
    k = len(probs)
    width = len(line)
    distribution = []
    for p in range(width - k + 1):
        kmer = line[p: p + k]
        distribution.append(prob(kmer, probs))

    sum_probs = sum(distribution)
    distribution = [p / sum_probs for p in distribution]

    choices = numpy.arange(width - k + 1)
    return numpy.random.choice(choices, p=distribution)


def gibbsSampler(dna, k, N):
    check_motifs(dna)
    width = len(dna[0])
    height = len(dna)

    motifs = []
    for line in dna:
        start = random.randint(0, width - k)
        motifs.append(line[start: start + k])

    best_motifs = motifs

    for j in range(N):
        i = random.randint(0, height - 1)

        pref_motifs = motifs[: i]
        suf_motifs = motifs[i + 1:]
        removed_motif = pref_motifs + suf_motifs
        probs = find_probs(removed_motif, pseudocounts=True)

        motif_ind = generate_motif(dna[i], probs)
        motif = dna[i][motif_ind: motif_ind + k]

        motifs = pref_motifs + [motif] + suf_motifs

        if score(motifs) < score(best_motifs):
            best_motifs = motifs

    return best_motifs


def randRunner(dna, k, N):
    best_motifs = None
    best_score = len(dna) * len(dna[0])

    for i in range(20):
        motifs = gibbsSampler(dna, k, N)
        score_motifs = score(motifs)
        if score_motifs < best_score:
            best_motifs = motifs
            best_score = score_motifs

    return best_motifs