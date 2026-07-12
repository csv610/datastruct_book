c = '{'  ->  push: stack = ['{']
c = '['  ->  push: stack = ['{', '[']
c = '('  ->  push: stack = ['{', '[', '(']
c = ')'  ->  top is '('  ->  pop: stack = ['{', '[']
c = ']'  ->  top is '['  ->  pop: stack = ['{']
c = '}'  ->  top is '{'  ->  pop: stack = []
Result: true
