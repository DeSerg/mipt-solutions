from copy import copy, deepcopy


class Graph:
    def __init__(self, adj_map, adj_map_inv):
        self.adj_map = adj_map
        self.adj_map_inv = adj_map_inv

    def findEulerPath(self):

        if not self.adj_map:
            return []

        adj_map = deepcopy(self.adj_map)

        to_vert = None
        from_vert = None
        found = False
        for v in adj_map:
            if len(adj_map[v]) > len(self.adj_map_inv.get(v, [])):
                to_vert = v
                found = True
            elif len(adj_map[v]) < len(self.adj_map_inv.get(v, [])):
                from_vert = v
                found = True

        if found:
            adj_map[from_vert] += [to_vert]

        edje_count = {}
        for vert, edges in adj_map.items():
            edje_count[vert] = len(edges)

        circuit = []
        v1 = next(iter(adj_map))
        stack = [v1]
        curr_v = v1

        while stack:
            if edje_count.get(curr_v, 0) > 0:
                stack.append(curr_v)

                adjs = adj_map[curr_v]
                next_vert = adjs[0]
                adjs = adjs[1:]
                if not adjs:
                    adj_map.pop(curr_v)
                else:
                    adj_map[curr_v] = adjs
                edje_count[curr_v] -= 1
                curr_v = next_vert
            else:
                circuit.append(curr_v)
                curr_v = stack.pop()

        circuit = circuit[::-1][:-1]
        if found:
            indp = 0
            for i in range(1, len(circuit)):
                if circuit[indp] == from_vert and circuit[i] == to_vert:
                    return circuit[i:] + circuit[:i]
                indp = i

        return circuit



def graph_from_kmers(kmers):
    adj_map = {}
    adj_map_inv = {}
    for kmer in kmers:
        prefix = kmer[:-1]
        suffix = kmer[1:]
        adj_map[prefix] = adj_map.get(prefix, []) + [suffix]
        adj_map[suffix] = adj_map.get(suffix, [])
        adj_map_inv[suffix] = adj_map_inv.get(suffix, []) + [prefix]
        adj_map_inv[prefix] = adj_map_inv.get(prefix, [])

    return Graph(adj_map, adj_map_inv)


def reconstruct_dna(kmers):
    graph = graph_from_kmers(kmers)
    path = graph.findEulerPath()
    genome = path[0]
    for i in range(1, len(path)):
        genome += path[i][-1]

    return genome


def _generate_kmers_binary(cur, length, kmers):
    if len(cur) == length:
        kmers.append(cur)
        return

    cur_zero = cur + '0'
    cur_one = cur + '1'
    _generate_kmers_binary(cur_zero, length, kmers)
    _generate_kmers_binary(cur_one, length, kmers)


def generate_kmers_binary(length):
    kmers = []
    _generate_kmers_binary('', length, kmers)
    return kmers


def circular_string(k):
    kmers = generate_kmers_binary(k)
    dna = reconstruct_dna(kmers)
    return dna[:-(k - 2)]