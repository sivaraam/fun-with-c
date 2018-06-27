## Chess move validator

A simple library that can be used to validate a basic chess move. Note that the
library does not validate every single chess move. For example, it doesn't
detect check-mates and  it doesn't ensure whether a move is check relieving in
case of a check.

### Building

#### Library
```
make lib
```

This would generate a shared library that could be used by other programs.

#### Example program
```
make chess_test.out
```

This would build an example program to play around with.
