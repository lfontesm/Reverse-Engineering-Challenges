# n30np14gu3's Password Keeper

You can find the link to this challenge [here.](https://crackmes.one/crackme/5e68f77d33c5d4439bb2de0c)

First things first, I'll try to explain the inner workings of it the best I can, but there's a lot of things I want to talk about, so in order to avoid this post becoming dull or tiresome, I'll cut right to the chase most times, while still providing resources for people that want to learn more about techniques used in this crackme.

This crackme is targeted for the Windows platform, which means you will need necessary knowledge on the [PE binary format.](https://docs.microsoft.com/en-us/windows/win32/debug/pe-format) If you don't, go ahead and read up on it, [it's super necessary.](https://giphy.com/gifs/masvidal-super-necessary-TgP6vdMSAknVWzoC05/fullscreen)

The VM I use for Windows Reverse Engineering is [Flare VM](https://github.com/fireeye/flare-vm), it comes with a very useful set of tools and very rarely do I need to inflastall new tools. This should be more than enough for most levels of reverse engineers.

This crackme comes with 2 files:
* The executable PasswordKeeper.exe
* A database called passwords.db
