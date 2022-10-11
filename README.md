# GraphRanker

This C program was developed as one of the final examination projects required to obtain the BSc in Computer Science and Engineering at Politecnico di Milano.

The project obtained a grade of 30/30 cum laude. It is related to and based on concepts taught in the Algorithms and Principles of Computer Science course,
taught by prof. Alessandro Barenghi, particularly in the Algorithms and Data Structures module.

## Description
The program can manage and update a ranking of weighted directed graphs, keeping track of the $k$ best graphs.

The program receives two input parameters:
- the number of nodes in each graph `d`;
- the length of the ranking `k`.

After the parameters, the program can receive two commands as input:
- `AggiungiGrafo` (add graph), followed by the adjacency matrix;
- `TopK`.
