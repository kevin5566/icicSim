# icicSim: gemtek testbed ICIC simulation
@ Kevin Cheng a.k.a. kevin5566, NTUEE WMNLab 2017

## Compile
* Download ALL files include `main.cpp`, `Def.h`, `Def.cpp`, `makefile`, `input.txt`
* `make clean` for remove previous compile result
* `make` to generate executable file: `icicSim`

## Execute
* Download sample input `input.txt` 
* `./icicSim input.txt`

## Usage of `input.txt`
```
3
0,0,40
1500,0,40
750,1299,40
77777777777777700000000000000077777777777777700000
77777777777777777777777777777700000000000000000000
00000000000000077777777777777777777777777777700000
1
70,43,1,7
2
-750,433,1,7
750,433,1,7
1
0,-866,1,7
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
![output](http://i.imgur.com/sUjXxq6.png)
