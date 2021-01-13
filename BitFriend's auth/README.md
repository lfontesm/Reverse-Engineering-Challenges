# BitFriend's auth

This one is a really good one, since involves different areas of knowledge and is not obnoxiously hard. Although, I did spend more time than I'd like to admit on this one, since I literally forgot tool like [pwntools](https://github.com/Gallopsled/pwntools) existed, since I've been in somewhat of an hiatus from pwn exercises and exploit development.

Without further ado. Lets jump into it.

Executing the code, we can see the code is asking for a name and a password.

![Screenshot_20210113_100955](https://user-images.githubusercontent.com/28660375/104456518-8da0be80-5587-11eb-8a22-bac26de0987e.png)

Great, seems to be very straightforward! Now to our same old ritual. Open the file on your favorite debuggind tool. On Linux I often use [Ghidra.](https://ghidra-sre.org/)

I'll be covering 3 functions inside this code. Lets start with main:

![Screenshot_20210113_102020](https://user-images.githubusercontent.com/28660375/104457553-f76d9800-5588-11eb-8dee-20dfd4d4c354.png)

Perfect. Not much to look at. There's basically a call to a user defined function called `auth`, other than that it's very simple program logic, so lets look through it.

1. Program asks for our name and take the input from stdin.
2. Shows us a greeting message and ask for the password, that is probably collected with the `auth` function.

Before jumping into the auth function, I want to call your attention to line 14 on the decompiler. Seems like Ghidra decompiled a call to `printf` as `printf(userInput)`. That's interesting, could it be a mistake? If this is really how the program was made, this could be used to exploit a very known vulnerability, called [format string vulnerability.](https://www.securecoding.com/blog/format-string-vulnerability/) I'll also leave a link to a paper that goes into **heavy** detail on it. I highly recommend you reading it if you are interested in security. I'll leave it as a footnote.

Anyhow, lets check it out.

![Screenshot_20210113_104028](https://user-images.githubusercontent.com/28660375/104459665-ce023b80-558b-11eb-9cf0-929b08b77680.png)

Huh... now that is indeed interesting. So right out of the bat we know that theres an exploitable vulnerability in this code. Lets continue onto `auth()`, lets see how that function works.

![Screenshot_20210113_105024](https://user-images.githubusercontent.com/28660375/104460671-25ed7200-558d-11eb-96ad-2411590bfe73.png)

Lets walkthrough it.

1.
1.



[Paper on format string vulnerability.](https://crypto.stanford.edu/cs155old/cs155-spring08/papers/formatstring-1.2.pdf)
