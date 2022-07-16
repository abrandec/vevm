# VEVM Bugs

## Argument Parser
1. no checks on input can cause a crash on large inputs

## Hex Utils
1. hex_char2uint() no checks on EOF or newline cause characters to be converted to uint too