# n30np14gu3's Password Keeper

You can find the link to this challenge [here.](https://crackmes.one/crackme/5e68f77d33c5d4439bb2de0c)

First things first, I'll try to explain the inner workings of it the best I can, but there's a lot of things I want to talk about, so in order to avoid this post becoming dull or tiresome, I'll cut right to the chase most times, while still providing resources for people that want to learn more about techniques used in this crackme.

This crackme is targeted for the Windows platform, which means you will need necessary knowledge on the [PE binary format.](https://docs.microsoft.com/en-us/windows/win32/debug/pe-format) If you don't, go ahead and read up on it, [it's super necessary.](https://giphy.com/gifs/masvidal-super-necessary-TgP6vdMSAknVWzoC05/fullscreen)

The VM I use for Windows Reverse Engineering is [Flare VM](https://github.com/fireeye/flare-vm), it comes with a very useful set of tools and very rarely do I need to inflastall new tools. This should be more than enough for most levels of reverse engineers.

### First step - Detonation.

This crackme comes with 2 files:
* The executable PasswordKeeper.exe
* A database called passwords.db

If you execute the binary, it will ask for a password, if you type `"test"`, it will produce the output:

![Screenshot_20210119_131924](https://user-images.githubusercontent.com/28660375/105062088-f8a33700-5a58-11eb-8578-95a4ef70d66a.png)
If you think like me, you'd look for the strings that are being shown in the output inside the binary and set breakpoints near them to look for the password. I said in the beggining of the post that I wouldn't beat around the bush too much, so long story short, it doesn't work. The strings are being constructed during runtime, meaning they are obfuscated.

### Second step - Static analysis. Open the binary on your favorite disassembler
