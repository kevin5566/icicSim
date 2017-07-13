# icicSim: gemtek testbed ICIC simulation
@ Kevin Cheng a.k.a kevin5566, NTUEE 2017

## Compile
* download `main.cpp` and `Def.h`
* `g++ main.cpp -o icicSim`

## Execute
* download `input.txt` 
* `./icicSim input.txt`

## Usage of `input.txt`
```clike=
3
0,0,40
1500,0,40
750,1299,40
77777777777777777777777777777777777777777777777700
00000000000000077777777777777700000000000000000000
77777777777777777777777777777777777777777777777700
1
70,43,1,0
2
-750,433,1,7
750,433,1,4
1
0,-866,1,7
```
* First line specify **Base Station number** (`BSnum`)
  + The next `BSnum` lines specify **Base Station `i` Info.**: x, y, Power
  + The next `BSnum` lines specify **RB Pa level**

* The next will contain `BSnum` parts, each part contains an integer(`k`) and `k` lines

* The `i`th part means **UE info.** in base station `i` 
  - The integer `k` specify **number of UEs**
  - The next `k` lines specify **UE info.**: x, y, isEdge, Pa_level(x, y are relative value to its serving BS)
