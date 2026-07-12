Trie construction:
  root -> a
        -> h -> e [*he]
               -> i -> s [*his]
        -> s -> h -> e [*she]
        -> h -> e -> r -> s [*hers]

Failure links:
  "h"   -> root   (no proper suffix in trie)
  "he"  -> root   (no suffix)
  "i"   -> root
  "his" -> "s"    (suffix "is" not in trie;
                    suffix "s" not in trie;
                    actually "is" -- link to root)
  "sh"  -> "h"    (suffix "h" is in trie)
  "she" -> "he"   (suffix "he" is in trie)
  "he"  -> root
  "her" -> root
  "hers"-> "rs"   (no match, link chain)

Search in "ahishers":
  pos 0: 'a' -> root (no match)
  pos 1: 'h' -> state "h" (no pattern end)
  pos 2: 'i' -> state "his" -- MATCH "his" at 1
  pos 3: 's' -> follow link chain to "s"
  pos 4: 'h' -> state "sh"
  pos 5: 'e' -> state "she" -- MATCH "she" at 3
           follow dict_link -> state "he"
           -- MATCH "he" at 3
  pos 6: 'r' -> state "her"
  pos 7: 's' -> state "hers" -- MATCH "hers" at 4

  (pos 2 'i' + 's' also covers "is" but "is"
   is not in our pattern set)
