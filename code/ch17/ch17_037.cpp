i=5, w=11: dp[5][11]=T, dp[4][11]=T -> skip nums[4]=4
i=4, w=11: dp[4][11]=T, dp[3][11]=T -> skip nums[3]=8
i=3, w=11: dp[3][11]=T, dp[2][11]=T -> skip nums[2]=1
i=2, w=11: dp[2][11]=T, dp[1][11]=F -> take nums[1]=7, w=11-7=4
i=1, w=4:  dp[1][4]=F -> take nums[0]=3, w=4-3=1
i=0, w=1:  base case

Subset: {3, 7} -> 3+7 = 10 ... wait, recheck.
