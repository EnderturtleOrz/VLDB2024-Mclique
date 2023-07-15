# POMA

This repository contains c++ codes and datasets for the paper:

> Efficient Maximal Motif-Clique Enumeration over Large Heterogeneous Information Networks

## Environment

The codes of POMA are implemented and tested under the following development environment:

- Hardware : Intel(R) Xeon(R) Gold 6226R 2.90GHz CPU and 512GB of memory.

- Operation System : Ubuntu 20.04.4 LTS (GNU/Linux 5.13.0-40-generic x86_64)

  

## Datasets

We use five real HINs: Instacart, WordNet, DBLP,DBpedia, and Freebase. The dataset file in this Google driven [link](https://drive.google.com/file/d/1RNVe5LcwN_j5S5XoJJgVNcHMuBaezlVW/view?usp=sharing). The detailed statistics and original sources of these datasets are in our paper.

## How to Run the Codes

### A. Code Compilation

You can just use the following commands:

```shell
make clean
cmake .
make
```

### B. Data format

The data format is:

1. the number of vertices `n`, the number of vertices `m`

2. the edges
3. the vertex type of the all vertices. 

```
n,m
id_1, id_2
id_3, id_4
....
type_1, type_2, type_3, ..., type_n
```



### C. Run Code

> You run the shell scripts like `xxx.sh` under META/scripts and POMA/scripts for the five datasets.
> You may need to modify the paths in the shell scripts to meet the corresponding folders.

Such as you can just run `bash run_dblp.sh` to run our POMA algorithm for the dblp dataset.

Also, you can use the following parameters to obtain the different version of POMA.

| **-ORDER 0**      | **not use order POMA-O**                            |
| ----------------- | --------------------------------------------------- |
| **-InitVF3 1**    | **not use the fast candidate set algorithm POMA-C** |
| **-PIVOT 0**      | **not use motif-pivot POMA-P**                      |
| **--NotReduce 1** | **not use graph reduce POMA-R**                     |

For the degree and degeneracy order, you can:

| **-ORDER 1** | **degree order**     |
| ------------ | -------------------- |
| **-ORDER 2** | **degeneracy order** |

For the Scalability experiment, you can just run `bash run_dblp_sca.sh` to obtain the result for the dblp dataset.

Similarly, you can modify the dataset name to obtain the results for other datasets.

### **C. Data Download**

**You can download the datasets from the following Google driven link:**

https://drive.google.com/file/d/1RNVe5LcwN_j5S5XoJJgVNcHMuBaezlVW/view?usp=sharing



