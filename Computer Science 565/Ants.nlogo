patches-own [
  chemical             ;; amount of chemical on this patch
  food                 ;; amount of food on this patch (0, 1, or 2)
  nest?                ;; true on nest patches, false elsewhere
  nest-scent           ;; number that is higher closer to the nest
]

turtles-own[
  oxygen
]

to setup
  clear-all
  set-default-shape turtles "bug"
  setup-patches
  reset-ticks
end

to setup-patches
  setup-nest
  setup-food
end

to setup-nest
  ask patches[
    set nest? (distancexy 0 0) < 5
    set nest-scent 200 - distancexy 0 0
  ]
end

to setup-food
  ask patches[


    if food-rule-one [
    let randomVal random 2
    ifelse not (nest? = true)[
      if randomVal = 1
      [ set pcolor green
        set food food + 1]
    ]
    [ set pcolor blue ]

    ]
    if food-rule-two [
      let randomVal random 100
      ifelse not (nest? = true)[
        if randomVal = 1
        [ set pcolor green
          set food food + 400]
      ]
      [ set pcolor blue ]

    ]
    if adjustable-food-rule[
      let randomval random 100
      ifelse not (nest? = true)[
        if randomVal < food-distribution
        [ set pcolor green
          set food food + food-density ]
      ]
      [ set pcolor blue ]
    ]
  ]
end


to go
  if count turtles < number-of-ants[
    ifelse count turtles - number-of-ants < 10
    [ create-turtles (number-of-ants - (count turtles))[
      set size 1
      set color red
      set xcor ((random 5) - 2)
      set ycor ((random 5) - 2)
      set oxygen 100
    ] ]
    [ create-turtles 10[
      set size 1
      set color red
      set xcor ((random 5) - 2)
      set ycor ((random 5) - 2)
      set oxygen 100
    ] ]
  ]

  move-turtles
  diffuse chemical (diffusion-rate / 40)
  ask patches
  [ set chemical chemical * (30 - evaporation-rate) / 30  ;; slowly evaporate chemical
    recolor-patch ]
  tick
end

to recolor-patch
  ifelse nest?
  [ set pcolor blue ]
  [ ifelse food > 0
    [ set pcolor green ]
    [ ifelse show-chemicals
      [ set pcolor scale-color violet chemical 0 100 ]
      [ set pcolor black]
    ]
  ]
end

to move-turtles
  let movement 1
  ask turtles[
    suffocate
    ifelse color = red [
      if pcolor = green[
        set food food - 1
        set color yellow
      ]
      set movement choose-heading
    ]
    [ ifelse color = yellow
      [ return-to-nest ]
      [ ifelse pcolor = green[
        set food food - 1
        set color yellow
      ]
      [ return-to-nest ]
      ] ]
    ifelse color = yellow[
      set chemical chemical + food-pheromone
    ]
    [if movement = 1[
      set chemical chemical + 1
    ]]
    if chemical > 1000[
        set chemical 1000
      ]
    wiggle
    if is-patch? patch-ahead 1[
      if not ( (count ( turtles-on ( patch-ahead 1 ) ) ) >= 20 )[
        ifelse always-move
        [ forward 1 ]
        [ forward movement ]
      ]
    ]

  ]
end

to suffocate
  if oxygen <= 0 and color = red[
    ;show "dead"
    set chemical chemical - 250
    if chemical < 0
    [set chemical 0]
    die
  ]
  if not (nest?)[
    set oxygen oxygen - 10
    ;show count turtles-on neighbors
    set oxygen oxygen + (floor( 100 / (count turtles-on patch xcor ycor)))
    if oxygen >= 500[
      set oxygen 500
    ]
  ]

end

to-report choose-heading
  let nestPatch patch 0 0
  let farthestPatches sort-by [ [farPatch otherFarPatch] -> ([nest-scent] of farPatch) < ([nest-scent] of otherFarPatch) ] neighbors
  let farthestPatch item 0 farthestPatches
  let secondFarthestPatch item 1 farthestPatches
  let farthestPheromone [chemical] of farthestPatch
  let secondFarthestPheromone [chemical] of secondFarthestPatch
  let probabilityOfMoving probability-of-moving farthestPheromone secondFarthestPheromone
  let randomVal random-float 1
  let return 1
  ifelse randomVal < probabilityOfMoving [
    if farthestPheromone + secondFarthestPheromone > 0 [
      let probsGonnaGoFar (farthestPheromone / (farthestPheromone + secondFarthestPheromone))
      ;let probsGonnaGoLessFar (secondFarthestPheromone / (farthestPheromone + secondFarthestPheromone))
      set randomVal random-float 1
      ifelse randomVal < probsGonnaGoFar
      [ ifelse count turtles-on farthestPatch < 20
        [ face farthestPatch ]
        [ ifelse count turtles-on secondFarthestPatch < 20
          [ face secondFarthestPatch]
          [ set return 0 ]
        ]
      ]
      [ ifelse count turtles-on secondFarthestPatch < 20
        [ face secondFarthestPatch ]
        [ ifelse count turtles-on farthestPatch < 20
          [ face farthestPatch]
          [ set return 0 ]
        ]
      ]
    ]
  ]
  [ set return 0 ]
  report return
end

to-report probability-of-moving [ x y ]
  report ( 0.5 * ( 1 + tanh ( (x + y) / 100 - 1 ) ) )
end

to-report tanh [ x ]
  report (exp (2 * x) - 1) / (exp (2 * x) + 1)
end

to return-to-nest
  ifelse nest? [
    die
  ]
  [
    let myPatch patch xcor ycor
    let closestPatch myPatch
    ask [neighbors] of myPatch[
      if nest-scent > [nest-scent] of closestPatch[
        set closestPatch self
      ]
    ]
    face patch 0 0
  ]

end

to wiggle
  rt random 40
  lt random 40
  if not can-move? 1
  [ if color != yellow
    [ set color blue ]
    return-to-nest ]
end
@#$#@#$#@
GRAPHICS-WINDOW
210
10
944
433
-1
-1
6.0
1
10
1
1
1
0
0
0
1
-60
60
-34
34
0
0
1
ticks
30.0

BUTTON
-1
10
62
43
NIL
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
-1
42
62
75
NIL
go\n
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

SLIDER
-1
73
171
106
number-of-ants
number-of-ants
0
5000
5000.0
1
1
NIL
HORIZONTAL

SLIDER
-1
105
171
138
evaporation-rate
evaporation-rate
0
30
1.0
1
1
NIL
HORIZONTAL

SLIDER
-1
137
171
170
diffusion-rate
diffusion-rate
0
40
1.0
1
1
NIL
HORIZONTAL

SWITCH
-1
169
140
202
show-chemicals
show-chemicals
0
1
-1000

SWITCH
61
41
193
74
food-rule-two
food-rule-two
0
1
-1000

SWITCH
61
10
193
43
food-rule-one
food-rule-one
1
1
-1000

SWITCH
-2
200
126
233
always-move
always-move
0
1
-1000

SWITCH
-1
231
168
264
adjustable-food-rule
adjustable-food-rule
1
1
-1000

SLIDER
-4
263
168
296
food-distribution
food-distribution
0
100
3.0
1
1
NIL
HORIZONTAL

SLIDER
-4
294
168
327
food-density
food-density
0
100
100.0
1
1
NIL
HORIZONTAL

SLIDER
-4
326
168
359
food-pheromone
food-pheromone
0
100
10.0
1
1
NIL
HORIZONTAL

@#$#@#$#@
## WHAT IS IT?

This model is an example of quantitative stigmergy where the ants use chemical concentration in the environment to decide their actions.

## HOW IT WORKS

1) Ants looking for food (away from nest) will deposit 1 unit of pheromone per visited       site.  
2) Ants coming back with food to the nest will deposit 10 units of pheromone per visited 	   site.  
3) There’s a limit of 1000 units of pheromone per site.  
4) Pheromones evaporate at a rate of 1/30 per time step (adjustable), and diffuse at a 	     rate of 1/40 per time step (adjustable).  
5) Red ants are ants leaving the nest looking for food, yellow ants are ants that have 	     found food, and blue ants are ants that hit the edge of the map.  
6) Red ants decide on where to move by choosing the two furthest tiles from the nest and 	   then choosing which tile to go to based on the concentration of pheromone on that 	     tile.
7) Ants have a small wiggle in their step to make their movements look more natural.
8) Exactly ten ants will leave the nest per time step. Maximum number of ants per site is    20. When an ant wants to move to a full site, it will move to the other site (unless 	   both sites are full then it will not move).
9) If an ant is on a tile with 20 ants for too long, it will be suffocated due to lack of    oxygen and die. This helps to prevent huge clumps of ants, especially on the edges of     the world where they would normally get stuck. This is done by giving ants "oxygen",      and allowing them to breathe on every movement, the ant can only breathe a certain        amount of oxygen, which is divided by however many ants are on the same tile as the       ant.
10) Ants that suffocate release a distress pheromone that helps to negate the food            pheromone.
11) When an ant returning to the nest reaches the nest, it goes into the nest to relax        for a quick moment before heading out again. This helps reduce traffic on the nest as     the ant does not take up space when it is inside of the nest. This also resets the        ants oxygen and color.

## HOW TO USE IT

Choose a food rule using the food rule tabs.

Show the pheromone trails released by the ants with show-chemicals.

Adjust the probability of an adjustable food rule's patch spawning with the distribution slider, and adjust the amount of food on that patch with the density slider.

Dynamically change the number of ants leaving the nest with the number-of-ants slider.

Change evaporation rate and diffusion rate with the appropriate sliders.

If always-move is switched on, ants will always move, regardless of is their probability has determined they will not move, this helps reduce clumping of ants which results in ants stacking due to pheromone concentrations building up.


## THINGS TO NOTICE

There are no well defined patterns that I could see. After 8400 ticks, on the first food rule, the map had roughly 20% of the screen stil green. Note: this means that the ants only ate roughly 30% of the board, as food rule one has a 50% spawn rate. This is with always move off. Due to these awful numbers I had to adjust the simulation to have always move enabled, as that gave more defined results. 

On food rule one, with always move enabled and 5000 ants, it took the ants roughly 386 ticks to find all of the food.

On food rule two under the same settings, it took the ants roughly 3800 ticks to find all of the food.

## THINGS TO TRY

The only way to find any sort of patterns is to play around with evaporation rate and diffusion rate, with always move on, and to have a reasonable amount of ants (not too many and not too few).

## EXTENDING THE MODEL

To extend this model to have more defined patterns I would assume that checking for the tiles with the highest and second highest concentration of pheromone, and then finding the furthest of these two, would have a more defined effect than the way it is currently set. I think that making ants who have found food drop more pheromone would also help, as 10 does not seem to be enough. Setting food pheromone to 100 helped significantly.

## NETLOGO FEATURES

None that I can think of, other than chemicals.

## RELATED MODELS

Ant model.

## CREDITS AND REFERENCES

Ant model.
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

sheep
false
15
Circle -1 true true 203 65 88
Circle -1 true true 70 65 162
Circle -1 true true 150 105 120
Polygon -7500403 true false 218 120 240 165 255 165 278 120
Circle -7500403 true false 214 72 67
Rectangle -1 true true 164 223 179 298
Polygon -1 true true 45 285 30 285 30 240 15 195 45 210
Circle -1 true true 3 83 150
Rectangle -1 true true 65 221 80 296
Polygon -1 true true 195 285 210 285 210 240 240 210 195 210
Polygon -7500403 true false 276 85 285 105 302 99 294 83
Polygon -7500403 true false 219 85 210 105 193 99 201 83

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

wolf
false
0
Polygon -16777216 true false 253 133 245 131 245 133
Polygon -7500403 true true 2 194 13 197 30 191 38 193 38 205 20 226 20 257 27 265 38 266 40 260 31 253 31 230 60 206 68 198 75 209 66 228 65 243 82 261 84 268 100 267 103 261 77 239 79 231 100 207 98 196 119 201 143 202 160 195 166 210 172 213 173 238 167 251 160 248 154 265 169 264 178 247 186 240 198 260 200 271 217 271 219 262 207 258 195 230 192 198 210 184 227 164 242 144 259 145 284 151 277 141 293 140 299 134 297 127 273 119 270 105
Polygon -7500403 true true -1 195 14 180 36 166 40 153 53 140 82 131 134 133 159 126 188 115 227 108 236 102 238 98 268 86 269 92 281 87 269 103 269 113

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270
@#$#@#$#@
NetLogo 6.0
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180
@#$#@#$#@
0
@#$#@#$#@
