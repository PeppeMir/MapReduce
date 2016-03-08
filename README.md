# A C++11 MapReduce library

MapReduce is a generic programming model which, in his “Google version”, allows users to specify a map function that processes a key-value pair to generate a set of intermediate key-value pairs, and a commutative and associative reduce function that merges all intermediate values associated with the same intermediate key.

Nowadays many real world tasks are expressible using this model, both in distributed and parallel systems and architectures.

## Supported architectures

All **POSIX shared memory multicore** architectures are supported, but in particular some optimizations have been made for both Intel Xeon (8 double hyperthreaded cores) and Intel Xeon Phi (60 four hardware hyperthreaded cores).

## Usage example: Word Count

```c++
#include "MapReduce.hpp"
#include "LineBasedRecordFormat.hpp"
#include "RecordSelfContentKeyValueFormat.hpp"

/*** (line_num, doc_line) ==> list( (word, 1) ) ***/
void mapf(const int& line_num, const string& doc_line, ResultsWrapper<pair<string, int>>& results_wrapper)
{
  istringstream iss(doc_line);
  do
  {
    string word;
    iss >> word;
    if (word != "")
      results_wrapper.emit(pair<string, int>(std::move(word), 1));
  } 
  while (iss);
}

/*** (word, [1,1,...,1]) ==> list( m ), with m = sum(1,1,...,1) ***/
void reducef(const string& word, const list<int>& values, ResultsWrapper<pair<string, int>>& results_wrapper)
{
  int result = 0;
  for (std::list<int>::const_iterator iterator = values.begin(), end = values.end(); iterator != end; ++iterator)
    result += *iterator;
	
	results_wrapper.emit(pair<string, int>(std::move(word), result));
}

int compare(const string& s1, const string& s2)
{
    return s1.compare(s2);
}

int main()
{
  // define formats that will be used to extract Records from the input file and initial (key, value) from each Record
  LineBasedRecordFormat<string> lbrf;
  RecordSelfContentKeyValueFormat<string> kvf;
    
  MapReduce<string, int, string, string, int> wc("./dir_path/", thread_num, &lbrf, &kvf, compare, mapf, reducef);
  wc.run();
  
  return 0;
}
```

## Sample performance benchmark for sorting algorithms

![ScreenShot](https://raw.githubusercontent.com/PeppeMir/MapReduce/master/plots/Sorting%20comparison.png)

## Sample performance benchmark for coarse grain computation

In the following plots we can find performance benchmarks relative to scalability and efficiency of the Map and Reduce phases w.r.t. the expected one (the one provided by the ideal model).

### Map Scalability

![ScreenShot](https://raw.githubusercontent.com/PeppeMir/MapReduce/master/plots/Benchmark%20of%20Map%20Scalability%20on%20Intel%20Xeon%20Phi%20(coarse%20grain%20computation).png)

### Map Efficiency

![ScreenShot](https://raw.githubusercontent.com/PeppeMir/MapReduce/master/plots/Benchmark%20of%20Map%20Efficiency%20on%20Intel%20Xeon%20Phi%20(coarse%20grain%20computation).png)

### Reduce Scalability

![ScreenShot](https://raw.githubusercontent.com/PeppeMir/MapReduce/master/plots/Benchmark%20of%20Reduce%20Scalability%20(coarse%20grain%20computation).png)

### Reduce Efficiency

![ScreenShot](https://raw.githubusercontent.com/PeppeMir/MapReduce/master/plots/Benchmark%20of%20Reduce%20Efficiency%20on%20Intel%20Xeon%20Phi%20(coarse%20grain%20computation).png)
