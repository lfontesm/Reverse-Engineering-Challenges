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
Where `serialLen = 25` and `salt = username[1] + userLen` (Refer to [sanitized_keygen.c](https://github.com/lfontesm/Reverse-Engineering-Challenges/blob/main/egg's%20keygenme%20-%20complex%20validation/sanitized_keygen.c), we'll be forcing the third case of the if/else statement)

Now this is pretty much all we need to reach the solution I did. We have all the tool need to convert this into an equantion:
* `auxSalt`'s initial value is 1.
*  We know for a fact that `serialLen` is always 25 (Refer to [sanitized_keygen.c](https://github.com/lfontesm/Reverse-Engineering-Challenges/blob/main/egg's%20keygenme%20-%20complex%20validation/sanitized_keygen.c)).
*  For convenience, let's assume both the `username` and `serialKey` inputs will be constituded of the same character, for instance:
`./keygenme AAAAA AAAAAAAAAAAAAAAAAAAAAAAAA`
*  With that assumption, we can call the character forming the input string `x` for the equation we'll be working with.

Before continuing, I'd like to highlight how the loop works, using a shorter version of `serialKey`, with `serialLen` of 4.

![Screenshot_20210111_104310](https://user-images.githubusercontent.com/28660375/104189737-d88ccb80-53f9-11eb-9890-653f2a690f1c.png)

The order of operation goes
```
    key     = [username[0] = 'A'] * 1;
    auxSalt = [username[0] = 'A'];
```
Which means, in terms of math operations:
```
    key = x * 1;
```
On the next iteration, we'll have:

![Screenshot_20210111_104517](https://user-images.githubusercontent.com/28660375/104189936-20135780-53fa-11eb-9e61-197e59233e85.png)

```
    key     = [username[1] = 'A'] * [username[0] = 'A'];
    auxSalt = [username[1] = 'A'];
```
Which means, in terms of math operations:
```
    key = x * x;
```
The point I'm trying to make is, given whichever length `serialKey` will have, the corresponding summation of operations will be:
```
    key = x + x^(serialLen-1)
```
In this case:
```
    key = x + x^3
```

Back to the matter at hand, we find that the final equation will have the form of:
```
    key = x + x^24
```
Now lets remind ourselves about `if (key == 153000 - username[0] * salt)`. Assume `userLen = n`. This means:
```
    1. key = 153000 - x * (x + n)
    2. x + x^24 = 153000 - x^2 + n * x
    3. x + n*x + 25*x^2 - 153000 = 0
```
This is not yet the final solution, since it's still lacking thought. But with it, you can already understand how to go on. I'll use a graphic calculator to show how to use this equation. My graphic calculator of choice is [Desmos.](https://www.desmos.com/)

Here's the input with `n` as `userLen`:

![Screenshot_20210111_112438](https://user-images.githubusercontent.com/28660375/104194069-9c5c6980-53ff-11eb-8e95-6913c56cd961.png)

And where the graphic crosses the absissa:

![Screenshot_20210111_112530](https://user-images.githubusercontent.com/28660375/104194198-bdbd5580-53ff-11eb-9b2a-7502548527f9.png)

Sure enough, if we use what we've found as input for [sanitized_keygen.c](https://github.com/lfontesm/Reverse-Engineering-Challenges/blob/main/egg's%20keygenme%20-%20complex%20validation/sanitized_keygen.c), knowing that 78 is the ascii representation of the letter `'K'`, we get the confirmation that the program has passed the first check `if (key == 153000 - username[0] * salt)`:
```
./sanitized_keygen $(python -c "print('K'*164)") $(python -c "print('K'*25)")

Third case
Key:                      135075
username[0] * salt:       17925
key + username[0] * salt: 153000
Got here
(a-b<0.0):                                                False
(0<serialKey[0] - serialKey[19]):                         False
(serialKey[12]+serialKey[7]<140):                         False
(serialKey[10]*serialKey[8]<=serialKey[16]*serialKey[4]): True
Incorrect serial key
```
Note that we still have 3 conditions to meet. Which are:
```C
if (((a-b<0.0) && (0<serialKey[0] - serialKey[19]) &&
     (serialKey[12]+serialKey[7]<140)) && 
     (serialKey[10]*serialKey[8]<=serialKey[16]*serialKey[4])){
         puts("Correct serial Key!");
         return 0;
}
```
Where `a = pow(serialKey[19], serialKey[1]);` and `b = pow(serialKey[0], serialKey[2]);`

Note that condition 3 is the easiest to meet, you could play with the slidebar on [Desmos](https://www.desmos.com/) and find a value of `x` that satisfies said condition. I think `'D'` does it, but still it wouldn't meet the first two.
Also note how both condition 1 and 2 are intertwined. For condition 1, we need `b > a`, and since our inputs are all composed by the same characters, if you pay close attention, all we need is `'serialKey[0] > serialKey[any other pos]`, and that would automatically satisfy condition 2. Now all we need to do is tweek the equation a little bit and finally get to where we want. Lets assume `m = serialKey[0]`, the new order of operation will look like this:

The order of operation goes
```
    key     = [username[0] =  m ] * 1;
    auxSalt = [username[0] =  m ];
```
Which means, in terms of math operations:
```
    key = m * 1;
```
On the next iteration, we'll have:
```
    key     = [username[1] = 'A'] * [username[0] =  m ];
    auxSalt = [username[1] = 'A'];
```
Which means, in terms of math operations:
```
    key = 'A' * m;
```
And the new equation will look like this:
```
    key = m + x * m + 23x^2
```
This means:
```
    1. key = 153000 - x * (x + n)
    2. m + x * m + x^23 = 153000 - x^2 + n * x
    3. m(1 + x) + n * x + 24x^2 - 153000 = 0
```
Will give us:

![Screenshot_20210111_140257](https://user-images.githubusercontent.com/28660375/104214052-c28d0400-5415-11eb-8302-cd2212150ad9.png) ![Screenshot_20210111_140359](https://user-images.githubusercontent.com/28660375/104214176-e2bcc300-5415-11eb-80a9-daed4a5d0172.png)



