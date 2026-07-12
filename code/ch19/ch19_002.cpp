Bucket 7:  [f1] [f2] [  ] [  ]       Bucket 12: [f3] [f4] [f5] [f6]
              |                              |
           i1 = 7                         i2 = 12

  Insert f7 (maps to buckets 7 and 12):
    Both full  ->  evict f3 from bucket 12 to its alternate (bucket 7 or another)
