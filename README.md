# fast_mergesort
A mergesort implementation that is faster than quicksort. For details, refer to [Mergesort is faster than Quicksort](https://liwt31.github.io/2018/11/20/fast_merge/).

### Build and run
```
git clone https://github.com/liwt31/fast_mergesort.git
cd fast_mergesort
make run
```
If you didn't see Mergesort is faster. There are two ways to solve it:
##### Build and run inline-assembly optimized Mergesort code:
```
make asm
```
##### Run the binary which contains the optimized Mergesort code:
```
./bin/fast_mergesort.out
```
