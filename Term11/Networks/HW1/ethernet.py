import sys
import random
import time

TACTS_NUM_PER_FRAME = 24


class Station:
    def __init__(self, ind, packages_num):
        self.ind = ind
        self.packages_num = packages_num


class CollisionState:

    def __init__(self, stations):
        self.iteration_num = 0
        self.stations = stations

        self.wait_for_interval_min = None
        self.waiting_station = None

    def start_next_iteration(self):
        self.iteration_num += 1

        if self.iteration_num > 16:
            print('Too many attempts while resolving collision. Aborting')
            return False

        wait_for_max = 2 ** min(10, self.iteration_num)
        wait_for_interval_min = wait_for_max
        waiting_station = None
        for i in range(len(self.stations)):
            wait_for = random.randrange(wait_for_max)
            if wait_for < wait_for_interval_min:
                wait_for_interval_min = wait_for
                waiting_station = self.stations[i]
                continue

            if wait_for == wait_for_interval_min:
                waiting_station = None

        self.wait_for_interval_min = wait_for_interval_min
        self.waiting_station = waiting_station
        return True


def loop(stations):

    time_position = 0

    stations_with_data = [station for station in stations if station.packages_num > 0]

    while stations_with_data:

        collision = CollisionState(stations_with_data)
        if not collision.start_next_iteration():
            return

        waiting_station = collision.waiting_station
        time_position += collision.wait_for_interval_min
        while waiting_station is None:
            if not collision.start_next_iteration():
                return
            waiting_station = collision.waiting_station
            time_position += collision.wait_for_interval_min

        print('{}: station {} starts sending its frame'.format(time_position, waiting_station.ind))

        waiting_station.packages_num -= 1
        time_position += TACTS_NUM_PER_FRAME
        stations_with_data = [station for station in stations if station.packages_num > 0]
        time.sleep(0.2)

    print('All packages have been successfully sent')


def main(argv):
    if len(argv) < 1:
        print('Argument 1 should be number of stations')
        return

    try:
        N = int(argv[0])
    except:
        print('Argument 1 should be number of stations')
        return

    if N < 1 or N > 1024:
        print('Number of stations must be positive and not greater than 1024')

    stations = [Station(i, 1) for i in range(N)]
    loop(stations)


if __name__ == '__main__':
    main(sys.argv[1:])
