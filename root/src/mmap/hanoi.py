from typing import List
import time
import sys
import itertools


def resolve_hanoi(stacks: List[List[int]]) -> List[List[int]]:
    """
    考え方: 一番下の円盤を最後の柱に移動する、そのために二番目以上の円盤を二つ目の柱に移す
    1. 二番目以上の円盤を二番目の柱へ移動する
    2. 一番目の円盤を最後の柱へ移動する
    3. 二番目以上の円盤を最後の柱へ移動する
    これを再帰的に行う。
    """
    resolve_hanoi_inner(stacks, len(stacks[0]), 0, 2)


def resolve_hanoi_inner(stacks: List[List[int]], pop_count: int, stack_from: int, stack_to: int):
    if pop_count == 1:
        stacks[stack_to].append(stacks[stack_from].pop())
        return
    next_to = choose_to(stack_from, stack_to)
    resolve_hanoi_inner(stacks, pop_count-1, stack_from, next_to)
    print_hanoi(hanoi)
    stacks[stack_to].append(stacks[stack_from].pop())
    print_hanoi(hanoi)
    resolve_hanoi_inner(stacks, pop_count-1, next_to, stack_to)


def choose_to(stack_from: int, stack_to: int) -> int:
    return (set(range(3))-set([stack_from, stack_to])).pop()


def print_hanoi(stacks: List[List[int]]):
    flatten = list(itertools.chain.from_iterable(stacks))
    h, w = len(flatten) + 3, max(flatten) + 2
    s = ''
    for i in range(h, -1, -1):
        for col in stacks:
            r = col[i] // 2 + 1 if i < len(col) else 0
            s += ('#'*r+'|'+'#'*r).center(w)
        s += '\n'
    sys.stdout.write('{}\033[{}A'.format(s, s.count('\n')))
    time.sleep(0.05)


if __name__ == '__main__':
    n = int(sys.argv[1])
    hanoi = [[x for x in range(2*n+1, 0, -2)], [], []]
    print_hanoi(hanoi)
    resolve_hanoi(hanoi)
    print_hanoi(hanoi)
