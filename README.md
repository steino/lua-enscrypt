# lua-enscrypt

A lua binding for the EnScrypt password hashing library.

Requires [EnScrypt](https://github.com/Novators/EnScrypt).

Functions:

**hash(password, salt, iterations)**

- password - string
- salt - 64 character hex string
- iterations - number of iterations

Usage:

```lua
local enscrypt = require'enscrypt'
local hash = enscrypt.hash(password, salt, iterations)
```

Install:

Just run 'make' followed by 'make install'.

Tests:

Run 'make test' to run tests.
