## Case 1: Benchmark memory allocation and pagefaults

```
make pagefaults
```

## Case 2: Benchmark page-faults and random reads

```
make numa
```

## Basic profiling with perf
Record a trace with this command, possibly cancling the execution early
```
perf record --call-graph dwarf -F99 -e cpu-cycles,faults -- ./main <i>
```
Then access the report
```
perf report
```
