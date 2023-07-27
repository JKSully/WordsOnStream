# Words on Stream bot via CLI

## How to use:
1. Compile using the provided CMakeLists.txt file
```console
cd ${PATH_TO_REPO}/
mkdir build && cd build
cmake ..
make
```

2. Run the program
```console
${PATH_TO_REPO}/build/words letterbank
```

3. Optional Arguments
```console
[--missing OR -m]
# letter bank includes a missing letter
[--length OR -l] REQUIRED_ARGUMENT
# REQUIRED ARGUMENT is an integer value of the minimum size of the word
[--fake OR -f] 
# letterbank bank has a fake letter
```
