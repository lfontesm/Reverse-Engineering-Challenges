# egg's Complex Validation

You can find the link to the challenge [here.](https://crackmes.one/crackme/5e2c557933c5d419aa013658)

As of Jan 11. I've solved this challenge using math. Granted, I'm not particularly satisfied with the solution, given the input becomes quite gigantic:

```
    ./keygenme $(python -c "print('E' * 464)") $(python -c "print('\x60'+ 'E'*24)")
    Correct serial key!
```

Currently, I'm working on a solution that will give a result like this, like one of the people that solved the keygen posted:
```
    ./keygenme IZIPIZI VBHPWYGBUTBOGPQJWYPBZAWPG
    Correct serial key!
```

When I find a suitable solution, I'll post it and edit this document accordingly. Note that the solution using math is probably much faster and desirable, given that the latter solution probably involves brute forcing. I'm just nitpicking on what the input on the former expands to:

```
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE `EEEEEEEEEEEEEEEEEEEEEEEE
```
Without further ado, let us begin.

First step is opening the binary with your favorite disassembler/reverse engineering tool. In my case I'll use [Ghidra.](https://ghidra-sre.org/)

The code itself is very straightforward:
![Screenshot_20210111_095226](https://user-images.githubusercontent.com/28660375/104185037-2a7e2300-53f3-11eb-917d-48b02713e813.png)

For someone who's only starting in this field (We've all been there) it may seem a little too much. But it's actually a very simple code, just a lot of operations. But, for everyone's sake, I've "translated" this into a more sanitized code. And I've called it... you guessed it, [sanitized_keygen.c](https://github.com/lfontesm/Reverse-Engineering-Challenges/blob/main/egg's%20keygenme%20-%20complex%20validation/sanitized_keygen.c), so if you feel it'll be easier to understand, take a look at that code.

But I want to call your attention specifically to:

```C
int key    =0;
int auxSalt=1;
int counter=0;
while (counter < serialLen){
    key   +=serialKey[counter]*auxSalt;
    auxSalt=serialKey[counter];
    counter++;
}
    ...
    
if (key == 153000 - username[0] * salt) ... 

```
Where `serialLen = 25` and `salt = username[1] + userLen`

Now this is pretty much all we need to reach the solution I did. We have all the tool need to convert this into an equantion:
* `auxSalt`'s initial value is 1.
*  We know for a fact that `serialLen` is always 25 (Refer to [sanitized_keygen.c](https://github.com/lfontesm/Reverse-Engineering-Challenges/blob/main/egg's%20keygenme%20-%20complex%20validation/sanitized_keygen.c)).
* For convenience, let's assume both the `username` and `serialKey` inputs will be constituded of the same character, for instance:
`./keygenme AAAAA AAAAAAAAAAAAAAAAAAAAAAAAA`
* With that assumption, we can call the character forming the input string `x` for the equation we'll be working with.

Before continuing, I'd like to highlight how the loop works.

![Screenshot_20210111_103707](https://user-images.githubusercontent.com/28660375/104189092-f86fbf80-53f8-11eb-88be-d4a7cf6a0933.png)








