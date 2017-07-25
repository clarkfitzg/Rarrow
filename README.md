This is meant to be the minimum required to write a C binding to R using
only the `.Call()` interface.

```

library("dotCall")
x = rnorm(10)
xn = l2norm(x)

# Should be 0
xn - sum(x * x)

```
