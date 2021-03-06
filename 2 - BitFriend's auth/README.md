# BitFriend's auth

This one is a really good one, since involves different areas of knowledge and is not obnoxiously hard. Although, I did spend more time than I'd like to admit on this one, since I literally forgot tool like [pwntools](https://github.com/Gallopsled/pwntools) existed, since I've been in somewhat of an hiatus from pwn exercises and exploit development.

Without further ado. Lets jump into it.

Executing the code, we can see the code is asking for a name and a password.

![Screenshot_20210113_100955](https://user-images.githubusercontent.com/28660375/104456518-8da0be80-5587-11eb-8a22-bac26de0987e.png)

Great, seems to be very straightforward! Now to our same old ritual. Open the file on your favorite debuggind tool. On Linux I often use [Ghidra.](https://ghidra-sre.org/)

I'll be covering 4 functions inside this code. Lets start with main:

![Screenshot_20210113_102020](https://user-images.githubusercontent.com/28660375/104457553-f76d9800-5588-11eb-8dee-20dfd4d4c354.png)

Perfect. Not much to look at. There's basically a call to a user defined function called `auth`, other than that it's very simple program logic, so lets look through it.

1. Program asks for our name and take the input from stdin.
1. Shows us a greeting message and ask for the password, that is probably collected with the `auth` function.

Before jumping into the auth function, I want to call your attention to line 14 on the decompiler. Seems like Ghidra decompiled a call to `printf` as `printf(userInput)`. That's interesting, could it be a mistake? If this is really how the program was made, this could be used to exploit a very known vulnerability, called [format string vulnerability.](https://www.securecoding.com/blog/format-string-vulnerability/) I'll also leave a link to a paper that goes into **heavy** detail on it. I highly recommend you reading it if you are interested in security. I'll leave it as a footnote.

Anyhow, lets check it out.

![Screenshot_20210113_104028](https://user-images.githubusercontent.com/28660375/104459665-ce023b80-558b-11eb-9cf0-929b08b77680.png)

Huh... now that is indeed interesting. So right out of the bat we know that theres an exploitable vulnerability in this code. Lets continue onto `auth()`, lets see how that function works.

![Screenshot_20210113_105024](https://user-images.githubusercontent.com/28660375/104460671-25ed7200-558d-11eb-96ad-2411590bfe73.png)

Lets walkthrough it.

1. Line 7, the program allocates a string with size of 0x40.
1. Line 8, the program points the address of a function called `normal` into offset +0x68 of the string. That's weird, the string was allocated with size of 0x40. This scream vulnerable code (and indeed it is, it's a form of heap overflow).
1. Line 9, `fgets` on stdin again, but argument `int size` is of value 1000, when `__s` is only of size 0x40. We can see where this is going.
1. Line 10, calls the function at offset +0x68 of the string.

This function `normal()` that is being pointed by the string is only a call to `puts("Wrong password!")`.

![Screenshot_20210113_111311](https://user-images.githubusercontent.com/28660375/104463283-571b7180-5590-11eb-8574-565bff8e0159.png)

I'm sure by now you must be wondering: "Okay, but where is the routine for when we get the correct password?". And indeed, where is it? On the symbol tree vison on Ghidra we see an interesting symbol that we didn't see referenced throughout the code: a function called `authenticated()`.

![Screenshot_20210113_111531](https://user-images.githubusercontent.com/28660375/104463559-afeb0a00-5590-11eb-86a9-f1f51e0115ab.png)

It's very similar to the `normal()` function, except this prints `"Right password"`. So those were the 4 functions I wanted to cover. By now we have all the tools necessary to complete the challenge. It's time for a dynamic analysis. Boot up your favorite debugger, and lets do it. I'll be using GDB + [Gef.](https://github.com/hugsy/gef)

Let's execute the code once and see what is happening inside `auth()`. I executed until the moment `normal()` is pointed by `string+0x68`.

![Screenshot_20210113_112721](https://user-images.githubusercontent.com/28660375/104464985-4ff56300-5592-11eb-8896-3acdd790bada.png)
![Screenshot_20210113_112940](https://user-images.githubusercontent.com/28660375/104465284-a4004780-5592-11eb-869f-134e0cc511f4.png)

Good. Everything is working how we expected. Lets try and see where the function `authenticated()` is located:

![Screenshot_20210113_113150](https://user-images.githubusercontent.com/28660375/104465575-f2154b00-5592-11eb-8afe-445e3d076aeb.png)
![Screenshot_20210113_113234](https://user-images.githubusercontent.com/28660375/104465674-0eb18300-5593-11eb-94dd-6920321868c9.png)

So at `0x000055555555519c` we got the function that we are aiming for. And the program is working as expected, calling `normal()`, that is pointed by `string+0x68`. 

![Screenshot_20210113_113842](https://user-images.githubusercontent.com/28660375/104466350-e6765400-5593-11eb-95e6-0087c63d6e78.png)
![Screenshot_20210113_114021](https://user-images.githubusercontent.com/28660375/104466562-22111e00-5594-11eb-8aa3-27e3197a1c4b.png)

Perfect. Now we can try messing around with the input a little bit. We'll be working with this input, what we're trying to accomplish here is to overwrite the address of `normal()` with `BBBBBBBB`, by passing a string that contains 0x68 A's, and ending with 8 B's. This should overflow the heap space allocated for the string, and write the B's at offset + 0x68 on it, therefore overwriting the function that was pointed by it. Lets see what happens.

![Screenshot_20210113_114926](https://user-images.githubusercontent.com/28660375/104467705-6650ee00-5595-11eb-8f40-92185af8ca73.png)

Sure enough, it works as expected. Now I'm sure most of you sohuld be thinking of overwriting it with something useful. A function address, perhaps? We already got the address of `authenticated()`... shall we try?

![Screenshot_20210113_120701](https://user-images.githubusercontent.com/28660375/104469951-dceeeb00-5597-11eb-9c39-41df5165ecb2.png)

Since we are now working with bytes, we should use a proper input source. We'll redirect the input file to the program:

![Screenshot_20210113_115636](https://user-images.githubusercontent.com/28660375/104468603-669db900-5596-11eb-9e20-050b1c96bb34.png)
![Screenshot_20210113_115716](https://user-images.githubusercontent.com/28660375/104468693-7e753d00-5596-11eb-9da9-2ec857650587.png)
![Screenshot_20210113_115744](https://user-images.githubusercontent.com/28660375/104468747-8df48600-5596-11eb-83aa-d271e52ccd41.png)

Sure enough, working perfectly. Now the problem is solved, onto the next one... right? Well... not really. Lets try passing the same input without attaching the debugger:

![Screenshot_20210113_120736](https://user-images.githubusercontent.com/28660375/104470028-f8f28c80-5597-11eb-9ce0-633b54125d12.png)

What is going on? It was just working, so why is it crashing?
The answer is [ASLR](https://www.fireeye.com/blog/threat-research/2020/03/six-facts-about-address-space-layout-randomization-on-windows.html), in summary, what's going on is in each execution, the address space of the program gets randomized. But this begs the question: so why when we executed the program on the debugger, the addresses of functions never changed?
The answer is that GDB disables ASLR by default.

And this is where the format string vulnerability we saw earlier comes in hand. Assembly language uses relative offsets to make references to memory location, meaning, no matter where they are mapped in memory, the offsets between two functions will always be the same. Firing back GDB again, lets take a look at that interesting stack string. But first of, let me just clarify something. I've been calling it stack string just for the sake of convenience and understanding, in actuality, this is located at `.rodata` section:
![Screenshot_20210114_100424](https://user-images.githubusercontent.com/28660375/104594448-eb004280-564f-11eb-9d6b-22c59615a1bc.png)

But unless you have an understanding about the ELF file format, it doesn't matter, you can just call it a stack string, but make sure you read the ELF format documentation :). You can find it easily online. You can even `man elf`.

Now let us really take a look at that string:

![Screenshot_20210114_095820](https://user-images.githubusercontent.com/28660375/104593892-21898d80-564f-11eb-95f7-a748d99a028b.png)
![Screenshot_20210114_100750](https://user-images.githubusercontent.com/28660375/104594756-606c1300-5650-11eb-9693-36e1525027ae.png)

It seems to be cut off. Lets take a look at the code and see if we find it's original pointer:
![Screenshot_20210114_101003](https://user-images.githubusercontent.com/28660375/104594965-af19ad00-5650-11eb-80b8-ec3424225aa2.png)

We can see a lot of references to adresses like `0x5555555560xx`. Lets try printing one of those.
![Screenshot_20210114_101110](https://user-images.githubusercontent.com/28660375/104595086-d7a1a700-5650-11eb-85d0-03e443e850f3.png)

Great, it is for a fact a string that was used inside of the code. That means we can calculate the offset from the address we got using the format string vulnerability to the `authenticated()` function.

The address we got was: `0x555555556030` and as we say earlier the address of `authenticated()` was `0x55555555519c`. Calculating the relative offset gives us: `0x555555556030 - 0x55555555519c = 0xe94`. Meaning `authenticated()` is located 0xe94 "below" the string during the code execution. And thats pretty much it. This is what we needed to complete the challenge. I'll use pwntools to execute the exploit. I won't cover how the tool works because first, I'm not an expert on it, and second, it's not the intention of the series to explain how tool works. The documentation is very good and easy to understand. I recommend you taking a look at it.

So we write the program that will deliver the malicious input, and boom:
![Screenshot_20210114_101959](https://user-images.githubusercontent.com/28660375/104595960-11bf7880-5652-11eb-849c-b6ba3b4b4bb2.png)

It's done! [Here's the source for authPwn.py,](https://github.com/lfontesm/Reverse-Engineering-Challenges/blob/main/BitFriend's%20auth/authPwn.py) it's really simple. I'll try to leave some important links as footnotes as well.

This crackme involved really important knowledges, such as: format string vulnerability, return oriented programming, heap overflow, and exploit development. And the best part is, it wasn't super difficult! This is an amazing crackme for people that are only starting, so I though it would be a good idea to bring this here. If there's any concept you didn't understand, feel free to contact me, but I highly encourage you to look on your own. Have fun :)

---

[ROP - Return Oriented Programming](https://en.wikipedia.org/wiki/Return-oriented_programming)

[Paper on format string vulnerability.](https://crypto.stanford.edu/cs155old/cs155-spring08/papers/formatstring-1.2.pdf)
