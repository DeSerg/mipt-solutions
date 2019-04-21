
class Edge:
    def __init__(self, weight, i, j):
        self.weight = weight
        self.i = i
        self.j = j

class Vertex:

    def __init__(self):
        self.distance = 0
        self.children = []
        self.parents = []
        self.optimum_parent = None


def build_graph(down_matrix, right_matrix):
    N = len(down_matrix) + 1
    M = len(down_matrix[0])

    table = [[Vertex() for _ in range(M)] for _ in range(N)]

    for i, weights in enumerate(down_matrix):
        for j, weight in enumerate(weights):
            table[i][j].children.append(Edge(weight, i + 1, j))
            table[i + 1][j].parents.append(Edge(weight, i, j))

    for i, weights in enumerate(right_matrix):
        for j, weight in enumerate(weights):
            table[i][j].children.append(Edge(weight, i, j + 1))
            table[i][j + 1].parents.append(Edge(weight, i, j))

    return table


def longest_path(table):
    N = len(table)
    M = len(table[0])

    for i, row in enumerate(table):
        for j, vert in enumerate(row):
            if not vert.parents:
                continue

            max_dist = 0
            max_vert = None
            for parent in vert.parents:
                p_i = parent.i
                p_j = parent.j
                p_dist = table[p_i][p_j].distance + parent.weight
                if p_dist > max_dist:
                    max_dist = p_dist
                    max_vert = (p_i, p_j)

            table[i][j].optimum_parent = max_vert
            table[i][j].distance = max_dist

    path = []
    path_i = N - 1
    path_j = M - 1
    while path_i > 0 or path_j > 0:
        path.append((path_i, path_j))

        v_next = table[path_i][path_j].optimum_parent
        path_i, path_j = v_next[0], v_next[1]

    path.append((0, 0))

    return table[N - 1][M - 1].distance, path


def longest_distance(down, right):
    table = build_graph(down, right)
    distance, _ = longest_path(table)
    return distance
