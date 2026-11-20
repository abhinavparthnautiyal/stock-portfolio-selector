# Stock Portfolio Selector

A C program that picks stocks to buy under a **budget** and a **risk limit**, and compares three algorithms on the same input: **Greedy**, **Dynamic Programming** and **Backtracking**.

Course: CSEG3053 Design and Analysis of Algorithms, UPES Dehradun

## Problem

**Target user:** a small investor with a fixed budget.

**Question:** given a list of stocks, each with a cost, an expected return and a risk score, which stocks should be bought to get the highest total return?

| Item | Description |
|---|---|
| Input | `data/assets.txt`, one stock per line: `name cost return risk` |
| Constraints | Total cost <= budget (1 to 2000). Risk of every bought stock <= risk limit (1 to 10). Each stock is bought at most once. |
| Output | The chosen stocks, total cost, total return and average risk |
| Success criterion | The highest total return that satisfies the constraints |

This is the **0/1 knapsack problem**: a stock can't be bought in part.

## Algorithms

| Module | Algorithm | Paradigm | File |
|---|---|---|---|
| Rank stocks by return per rupee | Merge sort | Divide and conquer | `src/sort.c` |
| Fast approximate pick | Greedy by best ratio | Greedy | `src/greedy.c` |
| Optimal pick | 0/1 knapsack table | Dynamic programming | `src/dp.c` |
| Exact check on small inputs | Include or skip each stock | Backtracking with pruning | `src/backtrack.c` |
| Best buy and sell window in a price history | Maximum subarray | Divide and conquer | `src/maxsub.c` |

### How each one works

- **Greedy:** sort by return/cost (best first), then buy each stock that still fits the budget and the risk limit. Fast, but not always optimal.
- **DP:** `dp[i][w]` = best return using the first `i` stocks with budget `w`.
  `dp[i][w] = max( dp[i-1][w], dp[i-1][w - cost] + return )`. The table is then walked backwards to find which stocks were bought.
- **Backtracking:** for each stock, try "buy" (only if it fits the budget and risk limit, otherwise pruned) and "skip". Keep the best complete selection.
- **Max subarray:** daily price changes are computed first. The best window is either in the left half, in the right half or crossing the middle, so the problem is split in two and the crossing case is done with two loops.

## Complexity

| Algorithm | Time | Extra space |
|---|---|---|
| Merge sort | Theta(n log n) in best, average and worst case | O(n) |
| Greedy (sort + scan) | O(n log n) | O(1) besides the output flags |
| DP knapsack (budget W) | Theta(n * W) | Theta(n * W) |
| Backtracking | O(2^n) worst case | O(n) recursion depth |
| Max subarray (divide and conquer) | Theta(n log n) | O(log n) |

**Recurrences**
- Merge sort and max subarray: `T(n) = 2T(n/2) + Theta(n)`. Master Theorem, case 2, gives `Theta(n log n)`.
- Backtracking without pruning: `T(n) = 2T(n-1) + O(1)`, which gives `O(2^n)`.

**Complexity classes**
- The decision version of 0/1 knapsack ("is a return of at least R possible within budget W?") is **NP-complete**.
- **Reduction from Subset Sum:** given numbers `a1..an` and a target `T`, make one stock per number with `cost = return = ai`, budget `T`, and required return `T`. A portfolio with return `T` exists exactly when a subset sums to `T`.
- The DP is **pseudo-polynomial**: it is polynomial in the value of `W`, not in the number of bits needed to write `W`. This does not contradict NP-completeness.
- Max subarray also has an `O(n)` solution (Kadane's algorithm). The divide-and-conquer version is used here to show the paradigm.

## Results

Measured on the development laptop (Windows, MinGW GCC 6.3, `-std=gnu99`). Random data comes from `rand()` with a fixed seed, so the same inputs repeat on the same machine. Full output is in `docs/benchmark_results.txt`.

| Scenario | n | Budget | Risk limit | Greedy | DP | Backtracking (calls) |
|---|---|---|---|---|---|---|
| Normal operation | 20 | 300 | 10 | 1433 | 1451 | 1451 (1,274,044) |
| Constraint shock | 20 | 150 | 5 | 565 | 583 | 583 (301) |
| Small input | 10 | 300 | 10 | 1022 | 1022 | 1022 (2,047) |
| Largest backtracking case | 25 | 300 | 10 | 1471 | 1504 | 1504 (18,368,957) |
| Large stress case | 1000 | 2000 | 10 | 11726 | 11726 | skipped (n > 25) |

**What the results show**
- DP and backtracking always agree, which confirms the DP answer is optimal.
- Greedy is never better than DP, and it is lower in the normal, shock and n = 25 cases (for example 1433 vs 1451).
- Backtracking calls grow roughly exponentially with n (2,047 at n = 10, about 1.3 million at n = 20, about 18 million at n = 25), while DP stays near 0.1 ms.
- On the n = 1000 stress case, greedy and DP give the same return, but DP takes longer (about 22 ms vs 0.4 ms). Greedy is a good heuristic with no optimality guarantee.

**Adversarial case (greedy fails):** one stock with cost 1 and return 2, another with cost 100 and return 190, budget 100. Greedy buys the first stock (best ratio) and cannot then afford the second, so it earns 2. The optimum is 190.

## Build and run

Requires GCC (tested with MinGW GCC 6.3 on Windows). No internet or external libraries needed.

```
gcc -Wall -Wextra -std=gnu99 src/main.c src/utils.c src/sort.c src/greedy.c src/dp.c src/backtrack.c src/maxsub.c -o portfolio
./portfolio          (Windows PowerShell: .\portfolio)
```

Menu options: show assets, run each method, compare all three, best holding window, change budget and risk limit.

**Benchmark**
```
gcc -Wall -Wextra -std=gnu99 src/benchmark.c src/utils.c src/sort.c src/greedy.c src/dp.c src/backtrack.c -o benchmark
./benchmark
```

**Tests** (run from the project root)
```
gcc -Wall -Wextra -std=gnu99 -Isrc tests/test_all.c src/utils.c src/sort.c src/greedy.c src/dp.c src/backtrack.c -o test_all
./test_all
```
24 tests: normal, boundary, invalid, adversarial, plus 300 random inputs comparing DP against backtracking. Results are in `docs/test_results.txt`.

## Project structure

```
src/        source code (main, sort, greedy, dp, backtrack, maxsub, utils, benchmark)
data/       assets.txt (stocks), prices.txt (price history)
tests/      test_all.c, bad_assets.txt
docs/       benchmark_results.txt, test_results.txt
```

## Limitations

- Costs and returns are whole numbers, because the DP table is indexed by cost.
- Budget is capped at 2000 and the DP table uses `n * W` memory.
- Backtracking is limited to 25 stocks because it takes exponential time.
- Returns and risk scores are given in the input file. The program does not predict them.
- Risk is a simple filter plus an average. It does not model correlation between stocks.
- This is a learning project, not financial advice.