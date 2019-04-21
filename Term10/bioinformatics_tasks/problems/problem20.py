import extern as ext


def coins_num_for_change(money, coins):
    min_coin = min(coins)
    coins_arr = [0 for _ in range(min_coin)]
    coins_arr.append(1)

    for i in range(min_coin + 1, money + 1):
        min_coins_num = money
        for coin in coins:
            coins_ind = i - coin
            if coins_ind >= 0:
                coins_num = coins_arr[coins_ind] + 1
                if coins_num < min_coins_num:
                    min_coins_num = coins_num

        coins_arr.append(min_coins_num)

    return coins_arr[money]


def run():
    with open(ext.FilenameIn) as f_in:
        money = int(f_in.readline().strip())
        coins = list(map(lambda n: int(n), f_in.readline().strip().split(',')))

    coins_num = coins_num_for_change(money, coins)
    with open(ext.FilenameOut, 'w') as f_out:
        f_out.write('{}'.format(coins_num))
