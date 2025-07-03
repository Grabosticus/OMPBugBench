# OMPBugBench

OMPBugBench is a benchmark, consisting of 150 OpenMP code examples written in C. 

## Structure
The code examples are divided into three categories: **Syntax/Semantic Errors**, **Race Conditions**, and **Deadlocks**. 
Each category contains 50 examples and is divided into multiple subcategories. 
The directories with the subcategories contain a `origin`-file, in which the origin of each code example is given.
Each subcategory directory contains a `Label`- and `Bug`-directory. 
The `Bug`-directory contains the actual errors in the code examples and the `Label`-directory contains explanations of the code, 
the bugs as well as the difficulty of a given code example, as well as code with a possible fix for the mentioned bugs.

## Categories
The following sections present the different types of errors in each category.
### Syntax/Semantic Errors
We collected a total of 50 OpenMP-related syntax/semantic errors. The errors either lead to the program not compiling (syntax error) or to the program outputting a wrong result (semantic error).
Although race conditions and deadlocks are also semantic errors, in this benchmark they have their own category.
These errors have been further divided into the following types:
#### Incorrect Encoding
A category of errors that arise from the use of invalid characters in OpenMP pragma statements. Some IDEs, such as IntelliJ, do not highlight these characters as erroneous.
#### Invalid Pragma Clause
These are errors which arise from incorrect usage of OpenMP pragma statements e.g. forgetting certain keywords or using wrong arguments for `depend()`-clauses.
#### Misspellings
These are errors arising from the misspelling of pragma clauses e.g. writing `prgama` instead of `pragma`.
#### Wrong Lock Usage
These are errors arising from using OpenMP's `omp_lock_t` or `omp_nest_lock_t` incorrectly e.g. using the operations for one type on the other.
#### Wrong Pragma Clause Combination
These are errors related to the combination or permutation of different clauses in a pragma statement e.g. declaring a variable as `shared` and `private` in the same clause
#### Wrong Pragma Curly Braces
These are errors that arise from an invalid usage or placement of curly braces after pragma statements.
#### Combination
Contains code examples with multiple errors from multiple other subcategories of Syntax/Semantic Errors

### Race Conditions
We collected a total of 50 race conditions. Most race conditions have been taken from the **DataRaceBench** [1][2] benchmark suite. The race conditions have been divided into the following subcategories:
#### Lost Update
A Lost Update is a type of race condition, where multiple threads write simultaneously to a shared variable, without any synchronization. This can lead to the update of one thread being immediately overwritten by another one, resulting in non-deterministic behavior.
#### Multiple Race Types
This subcategory is for code examples that contain multiple types of race conditions at the same time. Examples in this subcategory may contain Read-Modify-Write races next to Lost Updates, etc.
#### Non-Atomic Check-Then-Act
A Non-Atomic Check-Then-Act race condition refers to a situation where one thread checks a condition and acts on it, while another thread is changing the condition.
#### Read-Modify-Write
A Read-Modify-Write race condition occurs when multiple threads read from a variable and update it based on the read value, simultaneously. This type of race occurs often when using increment operations such as `var++`.
#### Unsafe Read
An Unsafe Read is a type of race condition that occurs when a thread reads a variable that is simultaneously being updated by another thread. This can lead to non-deterministic results, since threads might read either the updated variable value or the old one.

### Deadlocks
We collected a total of 50 deadlocks. The deadlocks have been divided into the following subcategories:
#### Circular Wait Deadlock
A Circular Wait Deadlock arises, when two or more threads/processes wait for another thread/process to release a resource or finish an operation. Since both threads wait for each other, the programs hangs indefinitely. In the benchmark, this deadlock subcategory contains only circular waits of length n >= 3.
#### Communication Deadlock
A Communication Deadlock in the benchmark is a deadlock related to blocked communication between threads. Since OpenMP does not natively support blocked communication, the examples in the benchmark simulate it using shared variables.
#### Recursive Deadlock
A Recursive Deadlock is a deadlock that occurs when a thread is "waiting for itself". This can happen when a thread tries to lock a resource twice. Since the resource is already locked, the thread waits for itself to release the resource, leading to an infinite wait.
#### Resource Deadlock
A Resource Deadlock occurs, when a thread locks a resource X and wants to lock a resource Y, held by another thread. When the other thread now tries to lock resource X, both threads wait for the other to release the held resource, leading to an infinite wait.
#### Synchronization Deadlock
A Synchronization Deadlock in the benchmark refers to a situation where a deadlock occurs because of an erroneous synchronization between two or more threads. An example of such a deadlock is when not all threads in a team reach a specific barrier statement, leading to the threads waiting infinitely at the barrier synchronization point.

## References
[1]: Chunhua Liao, Pei-Hung Lin, Joshua Asplund, Markus Schordan, and Ian
Karlin. DataRaceBench: A Benchmark Suite for Systematic Evaluation of
Data Race Detection Tools. In Proceedings of the International Conference
for High Performance Computing, Networking, Storage and Analysis (SC ’17),
pages 11:1–11:14, Denver, CO, USA, 2017. ACM. Best Paper Finalist. \
[2]: Pei-Hung Lin and Chunhua Liao. High-precision evaluation of both static and
dynamic tools using dataracebench. In International Workshop on Software
Correctness for HPC Applications (Correctness), SC21, 2021.

