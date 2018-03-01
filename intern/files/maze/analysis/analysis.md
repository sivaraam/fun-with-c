# Analysis of different algorithms for Maze solving

This document captures an analysis of different aspects of using different algorithms to solve the "maze problem" defined as, "Given a maze in the format of an image, find the shortest path from the source to the destination and highlight it."

## Algorithms tested

* Breadth-first search (BFS)
* A-star with Manhattan distance heuristic

## Maze image information

* Image format    : BMP (.bmp)
* Bytes per pixel : 3

| Image | Resolution (width\*height) | Clear pixel count |  % of clear pixels | Length of shortest path (BFS) |
|------:|:--------------------------:|------------------:|-------------------:|------------------------------:|
|   1   |           10\*10           |         43        |       43.00        |             15                |
|   2   |           15\*15           |        100        |       44.44        |             44                |
|   3   |           41\*41           |        801        |       47.65        |            308                |
|   4   |          201\*201          |      20692        |       51.22        |            596                |
|   5   |          401\*401          |      82724        |       51.45        |           1008                |
|   6   |         1940\*2001         |    2006882        |       51.70        |           4572                |
|   7   |         2001\*2001         |    2000001        |       49.95        |          24668                |
|   8   |         4001\*4001         |    8000001        |       49.98        |          62544                |
|   9   |         6001\*6001         |   18642114        |       51.77        |          19194                |
|  10   |        10001\*10001        |   50000001        |       49.99        |         173778                |
|  11   |        15001\*15001        |      NA           |         NA         |            NA                 |

## Running time

* Unit: second(s)
* Typical environment:

    - *OS*: Ubuntu 16.04
    - *Processor*: Intel i5 (7th gen) @ 3GHz
    - *Physical RAM*: 4GiB
    - *Swap space*  : 8GB

| Image | Breadth-first search (BFS) | A-star with Manhattan distance heuristic | % by which BFS is faster than A-star |
|------:|---------------------------:|-----------------------------------------:|-------------------------------------:|
| 1     |           0.003            |             0.003                        |            0.00                      |
| 2     |           0.005            |             0.002                        |         -150.00                      |
| 3     |           0.002            |             0.003                        |           33.00                      |
| 4     |           0.010            |             0.014                        |           28.50                      |
| 5     |           0.031            |             0.035                        |           11.42                      |
| 6     |           0.625            |             0.746                        |           16.22                      |
| 7     |           0.537            |             0.649                        |           17.25                      |
| 8     |           2.376            |             2.976                        |           20.16                      |
| 9     |            NA              |             9.460                        |             NA                       |
| 10    |            NA              |              NA                          |             NA                       |

## Node expansion

| Image | Breadth-first search (% expanded) | A-star with Manhattan distance heuristic (% expanded)| % by which A-star expanded fewer nodes than BFS |
|------:|----------------------------------:|-----------------------------------------------------:|------------------------------------------------:|
| 1     |             36 (83.72)            |                          27 (62.79)                  |                  25.00                          |
| 2     |             88 (88.00)            |                          72 (72.00)                  |                  18.18                          |
| 3     |            780 (97.38)            |                         770 (96.13)                  |                   1.28                          |
| 4     |          13866 (67.01)            |                       10104 (48.83)                  |                  27.13                          |
| 5     |          70167 (84.82)            |                       39943 (48.28)                  |                  43.07                          |
| 6     |       19494985 (97.14)            |                     1248266 (62.20)                  |                  35.97                          |
| 7     |         918257 (45.19)            |                      837378 (41.86)                  |                   8.80                          |
| 8     |        6201755 (77.52)            |                     6023496 (75.29)                  |                   2.87                          |
| 9     |               NA                  |                    18636058 (75.29)                  |                     NA                          |
| 10    |               NA                  |                    18636058 (99.97)                  |                     NA                          |
