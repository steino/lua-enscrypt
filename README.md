# lua-enscrypt

A lua binding for the EnScrypt password hashing library.

Requires [EnScrypt](https://github.com/Novators/EnScrypt).

Functions:

**hash(password, hash, iterations)**

- password - string
- hash - 64 character hex string
- iterations - number of iterations

Usage:

```lua
local enscrypt = require'enscrypt'
local hash = enscrypt.hash(password, hash, iterations)
```
