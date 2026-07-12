Wavelet tree for "abracadabra":

              [a b c d r]
              /         \
          [a b]         [c d r]
          / \           / \
        [a] [b]      [c] [d r]
                            / \
                          [d] [r]

Root bitvector (left=0, right=1):
  a b r a c a d a b r a
  0 0 1 0 1 0 1 0 0 1 0   (a,b go left; r,c,d go right)

Left child bitvector (a vs b):
  a b . a . a . a b . a
  0 1 . 0 . 0 . 0 1 . 0   (a=0, b=1)

Right child bitvector (c vs d,r):
  . . r . c . d . . r .
  . . 1 . 0 . 0 . . 1 .   (c=0, d,r go right)

Far-right child (d vs r):
  . . r . . . d . . r .
  . . 1 . . . 0 . . 1 .   (d=0, r=1)
