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

`EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE ```EEEEEEEEEEEEEEEEEEEEEEEE`
