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




[Paper on format string vulnerability.](https://crypto.stanford.edu/cs155old/cs155-spring08/papers/formatstring-1.2.pdf)
