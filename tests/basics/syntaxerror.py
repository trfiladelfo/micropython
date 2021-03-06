# test syntax errors

def test_syntax(code):
    try:
        exec(code)
        print("no SyntaxError")
    except SyntaxError:
        print("SyntaxError")

# can't assign to literals
test_syntax("1 = 2")
test_syntax("'' = 1")
test_syntax("{} = 1")

# can't assign to comprehension
test_syntax("(i for i in a) = 1")

# can't assign to function
test_syntax("f() = 1")

# can't assign to power
test_syntax("f**2 = 1")

# can't assign to power of composite
test_syntax("f[0]**2 = 1")

# can't assign to empty tuple
test_syntax("() = 1")

# can't have multiple *x on LHS
test_syntax("*a, *b = c")

# can't do augmented assignment to tuple
test_syntax("a, b += c")
test_syntax("(a, b) += c")

# can't do augmented assignment to list
test_syntax("[a, b] += c")

# non-default argument can't follow default argument
test_syntax("def f(a=1, b): pass")

# can't delete these things
test_syntax("del ()")
test_syntax("del f()")
test_syntax("del f[0]**2")
test_syntax("del (a for a in a)")

# must be in a "loop"
test_syntax("break")
test_syntax("continue")

# must be in a function
test_syntax("return")
test_syntax("yield")
test_syntax("nonlocal a")

# errors on uPy but shouldn't
#test_syntax("global a; global a")
#test_syntax("def f():\n a = 1\n global a")

# default except must be last
test_syntax("try:\n a\nexcept:\n pass\nexcept:\n pass")

# can't have multiple * or **
test_syntax("f(*a, *b)")
test_syntax("f(**a, **b)")

# LHS of keywords must be id's
test_syntax("f(1=2)")

# non-keyword after keyword
test_syntax("f(a=1, 2)")

# doesn't error on uPy but should
#test_syntax("f(1, i for i in i)")

# all elements of dict/set must be pairs or singles
test_syntax("{1:2, 3}")
test_syntax("{1, 2:3}")

# can't mix non-bytes with bytes when concatenating
test_syntax("'abc' b'def'")

# can't reuse same name for argument
test_syntax("def f(a, a): pass")

# nonlocal must exist in outer function/class scope
test_syntax("def f():\n def g():\n  nonlocal a")
