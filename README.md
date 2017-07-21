# icicSim: gemtek testbed ICIC simulation
@ Kevin Cheng a.k.a. kevin5566, NTUEE WMNLab 2017

## File Description
1. `main.cpp`: Main program to call all simulation procedure.
1. `Def.h`: Define all parameters and function.
1. `Def.cpp`: Implement of all function.
1. `makefile`: Compile.
1. `input.txt`: Sample input.

## Compile
* Download ALL files include `main.cpp`, `Def.h`, `Def.cpp`, `makefile`, `input.txt`
* `make clean` to remove previous compile result
* `make` to generate executable file: `icicSim`

## Execute
* Download sample input `input.txt` 
* `./icicSim input.txt`

## Usage of `input.txt`
```
2
0,0,-20
6,0,-20
77777777777777777777777777777744411111111111111144
00000000000000000000000000000000011111111111111100
4
2.8,0,2,7
2.4,0,2,7
0.9,0,1,4
2.2,0,2,7
1
-0.3,0,0,1
```
* First line specify **Base Station number** (`BSnum`)
  + The next `BSnum` lines specify **Base Station `i` Info.**: x, y, Power
  + The next `BSnum` lines specify **RB Pa Level**

* The next will contain `BSnum` parts, each part contains an integer(`k`) and `k` lines

* The `i`th part specify all **UE info.** in base station `i` 
  - The integer `k` specify **number of UEs** in base station `i`
  - The next `k` lines specify **UE `j` info.**: x, y, UePosition, Pa_level
    - x, y are **relative** value to its serving BS
    - UePosition=0, Cell-Center UE
    - UePosition=1, Cell-Middle UE
    - UePosition=2, Cell-Edge UE

# Sample Output 
![output](https://i.imgur.com/6quv6Bm.png)
