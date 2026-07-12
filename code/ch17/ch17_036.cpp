w=0   w=1   w=2   w=3   w=4   w=5   w=6   w=7   w=8   w=9  w=10  w=11
i=0 [] [  T     F     F     F     F     F     F     F     F     F     F     F  ]
i=1 3  [  T     F     F     T     F     F     F     F     F     F     F     F  ]
i=2 7  [  T     F     F     T     F     F     F     T     F     F     F     T  ]
i=3 1  [  T     T     F     T     F     F     F     T     F     F     T     T  ]
i=4 8  [  T     T     F     T     F     F     F     T     T     T     T     T  ]
i=5 4  [  T     T     F     T     T     T     F     T     T     T     T     T  ]

dp[5][11] = true -> subset exists: {3, 8} or {7, 4} or {3, 7, 1}
