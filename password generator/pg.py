from random import choices
from string import ascii_letters, digits, punctuation

length = int(input())
print(''.join(choices(list(ascii_letters+digits+punctuation), k=length)))
