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

Right at the beginnig we can see the program is acessing `fs:[30]`. This is the [PEB.](https://en.wikipedia.org/wiki/Process_Environment_Block) If you are not familiar with it, I encourage you to look it up, malware commonly access this region in orther to to stealthy operations such as dynammically resolve IAT. Which is exactly what this code is doing. Lets take a look at it. Open the screenshot bellow in another tab because it will be useful.

![Screenshot_20210119_133658](https://user-images.githubusercontent.com/28660375/105064287-723c2480-5a5b-11eb-9caa-0007a3599f8a.png)

The screenshot is already telling a whole tail. I'll try to explain this the best way I can, since I'm only a curious student, I may get some things wrong, but I'm confident that I have enough knowlegde on what I'm about to explain. I'll use WinDbg for this one.

Inside WinDbg, you can use `dt _peb @$peb` to access it's fields and their respective mapping in the VA.

![Screenshot_20210119_134458](https://user-images.githubusercontent.com/28660375/105065356-8b91a080-5a5c-11eb-868a-adf64ed1b464.png)

If you pay close attention to the IDA screenshot, you'll see the program making access to what could be struct fields in the PEB (`mox eax, [eax+0xc]`, `mov esi, [eax+0x18]`). The process is trying to access the modules loaded along with the binary, they are a doubly linked list and we can access them in WinDbg.

PEB + 0xc = PEB_LDR_DATA

![Screenshot_20210119_135149](https://user-images.githubusercontent.com/28660375/105066329-82550380-5a5d-11eb-920a-d961aebb33ed.png)

PEB_LDR_DATA + 0xc = InLoadOrderModuleList.

![Screenshot_20210119_135346](https://user-images.githubusercontent.com/28660375/105066649-c6480880-5a5d-11eb-9771-454d242e41dd.png)

InLoardOrderModuleList is the doubly linked list we were talking about. You can see it's fields on the screenshot above. The pointer to the next item in the list is in offset 0. In offset 0x18 is the DllBase, meaning, the starting virtual address in which the library is loaded in the virtual memory. Note that in the screenshot above, the module it's pointing to is the binary itself, and it will always be for every binary, the first item on the InLoadOrderModuleList will always be the binary itself.

It then collects the DllBase, and access the offset 0x3C on it, which contains the offset in file to the PE signature, and uses said offset to access the export table. **Corkami** has done an fantastic job in providing [visual documentation for this.](https://github.com/corkami/pics/blob/master/binary/pe102/pe102.svg) The program checks if it successfuly found an ET (our executable doens't have one), if it doens't find, it iterates trought the InLoardOrderModuleList again until it finds one. 

Upon successfully finding an ET, The program accesses the fields NumberOfNames and AddressOfNames to iterate throught the names of the symbols inside the memory (note that ET is [defined as a struct](fumalwareanalysis.blogspot.com/2011/12/malware-analysis-tutorial-8-pe-header.html)). It then computes a checksum using the symbol name and checks for a specific, hardcoded, checksum, which corresponds the the checksum generated by the function we are looking for.

![Screenshot_20210119_141819](https://user-images.githubusercontent.com/28660375/105069790-2f7d4b00-5a61-11eb-8fab-23d027f37039.png) ![Screenshot_20210119_141854](https://user-images.githubusercontent.com/28660375/105069876-4a4fbf80-5a61-11eb-8ca6-aeff93241d2c.png) ![Screenshot_20210119_141930](https://user-images.githubusercontent.com/28660375/105069922-5b003580-5a61-11eb-962e-42af514423e6.png) ![Screenshot_20210119_142013](https://user-images.githubusercontent.com/28660375/105070012-75d2aa00-5a61-11eb-8d46-2d9d2293c99c.png)

It then creates a stack string, wich is another form of string obfuscation commonly used by malware. By now you can already understand why strings like `Invalid Password` couldn't be found in the binary. After the string is completely formed, it uses it as an argument to the function obtained throught the previous checksum computation. Hex-Rays pre-calculates it and makes our lives easier.

![Screenshot_20210119_143023](https://user-images.githubusercontent.com/28660375/105071071-e29a7400-5a62-11eb-865f-f8309da52224.png) ![Screenshot_20210119_143316](https://user-images.githubusercontent.com/28660375/105071392-4886fb80-5a63-11eb-9e40-03cd7065d31a.png)

From the screenshots above we can tell it uses the string as `"user32.dll"` as an argument to the function retrieved from the export table of a certain module. Everything will come to light during dynamic analysis, but the function being dynamically resolve is `LoardLibraryA`.

---

###### Useful links

* PEB
  * https://secureyourit.co.uk/wp/2020/04/12/walking-the-peb-with-vba-x64/
  * https://www.ired.team/miscellaneous-reversing-forensics/windows-kernel-internals/exploring-process-environment-block
  * https://www.ired.team/miscellaneous-reversing-forensics/windows-kernel-internals/exploring-process-environment-block





