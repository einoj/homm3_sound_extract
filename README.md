# How to Build
Run `make` in the top directory

# Usage
`./extract /path/to/Heroes3.snd` will extract 1015 .wav files to the sub directory ./snd

# How it works
First 4 bytes gives the number of files. This number is 1015.
Next follows 1015 48 byte chunks. The first 8 bytes of the chunk is the
filename. then there is 32 bytes of padding and then 8 bytes of offset size.
