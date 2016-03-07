# MapReduce

MapReduce is a generic programming model which, in his “Google ver-
sion”, allows users to specify a map function that processes a key-value
pair to generate a set of intermediate key-value pairs, and a commutative
and associative reduce function that merges all intermediate values associ-
ated with the same intermediate key.

Nowadays many real world tasks are expressible using this model, both in
distributed and parallel systems and architectures.

This project wants to develop and implement a MapReduce C++11
library, based on pthread, suitable for POSIX shared memory multicore
architectures.
