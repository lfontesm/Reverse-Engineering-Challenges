#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int salt;

#define itob(a) ((a) == 1?"True":"False")

int main(int argc,char **argv){
    if (argc < 3){
        printf("Usage: %s <username> <serial key>\n",*argv);
    }
    else{
        char *username =argv[1];
        char *serialKey=argv[2];

        int userLen=strlen(username);
        if (userLen < 4){
            puts("Invalid username");
        }
        else{
            int serialLen=strlen(serialKey);
            if (serialLen == 25){
                int thirdCharInName=username[2];
                int lastCharInName =username[(userLen-1)];
                if (lastCharInName < thirdCharInName){
                    puts("First case");
                    salt=username[1] + ((int) thirdCharInName / userLen) - lastCharInName;
                }
                else if(thirdCharInName < lastCharInName){
                    puts("Second case");
                    salt=username[1] + ((int)lastCharInName / userLen) - thirdCharInName;
                }
                else {
                    puts("Third case");
                    salt=username[1] + userLen;
                }

                int key    =0;
                int auxSalt=1;
                int counter=0;
                while (counter < serialLen){
                    key   +=serialKey[counter]*auxSalt;
                    auxSalt=serialKey[counter];
                    counter++;
                }
                
                printf("Key:                      %d\n", key);
                printf("username[0] * salt:       %d\n", username[0] * salt);
                printf("key + username[0] * salt: %d\n", key + username[0] * salt);
                if (key == 153000 - username[0] * salt){

                    puts("Got here");

                    double a=pow(serialKey[19], serialKey[1]);
                    double b=pow(serialKey[0], serialKey[2]);

                    puts(itob((a-b<0.0)));
                    puts(itob((0<serialKey[0] - serialKey[19])));
                    puts(itob((serialKey[12]+serialKey[7]<140)));
                    puts(itob((serialKey[10]*serialKey[8]<=serialKey[16]*serialKey[4])));


                    if (((a-b<0.0) && (0<serialKey[0] - serialKey[19]) &&
                       (serialKey[12]+serialKey[7]<140)) && 
                       (serialKey[10]*serialKey[8]<=serialKey[16]*serialKey[4])){
                           puts("Correct serial Key!");
                           return 0;
                    }
                }
                puts("Incorrect serial key");
            }
            else {
                puts("Invalid serial key");
            }
                
        }
        
    }
    return 0;
}
    
