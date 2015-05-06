#include "tknlib.h"

using namespace std;

int  parse(char *line, char **words)
{
	 int count = 0;
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *words++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
    count++; 
	}
     *words = '\0';                 /* mark the end of argument list  */
	return count-1;
}

int length(char* s)
{
	int endpoint = 0;
	while(s[endpoint])
		endpoint++;
	return endpoint;
}

void itoa(int i,char *characters)
{
    int place=0;
    int temp1=i;
    int temp2=i;
    int num_of_digits=0;
    memset(characters,'\0', sizeof(characters));
    while(temp1>=10)
    {
        temp1/=10;
        num_of_digits++;
    }
    num_of_digits++;
    place = num_of_digits-1;
    while(temp2>=10)
    {
        num_of_digits++;
        int remain = temp2%10;
        temp2/=10;
        characters[place]='0'+remain;
        place--;
    }
    if(temp2<10)
    {
        characters[place] = '0'+temp2; 
    }
    //return num_of_digits;
}

int char_s_to_int(char *char_s)
{
    int number = 0;
    while((*char_s >= '0') && (*char_s <= '9'))
    {
        number = (number * 10) + (*char_s - '0');
        char_s++;
    }
    return number;
}

void encaps(char* type, char* cmd, char* dst, char* src, char* data, char* packet)
{
 
    char ttl[] = "99";
    char crc[] = "111111";
    char length[] = "00128";
    strcpy(packet, type);
    strcat(packet, cmd);
    strcat(packet, dst);
    strcat(packet, src);
    strcat(packet, ttl);
    strcat(packet, length);
    strcat(packet, data);
    // strcat(packet, crc);

}

int route(char* packet, char *port[], int portcount)
{
    char dst[4];
    char src[4];
    dst[0] = packet[2];
    dst[1] = packet[3];
    dst[2] = packet[4];
    dst[3] = packet[5];
    src[0] = packet[10];
    src[1] = packet[11];
    src[2] = packet[12];
    src[3] = packet[13];

    // cout << "you are coming from: " << atoi(src)  < <endl;
    // cout << "port[0] " << port[]  << endl;

    if (atoi(src) != atoi(port[1]))
    {
        // cout << "you are going to: " << atoi(dst)  << " by " << port[1] << endl;
        return atoi(port[1]);
    }
    else
    {
        for (int i = 1; i < portcount-1 ; ++i)
        {
            if (strcmp(dst,port[i]))
            {
                return atoi(port[i]);
            }
        }
        return atoi(port[portcount-1]);
    }


}

bool issw(char* packet)
{
    if (packet[0] == 's' && packet[1] == 'w')
    {
        return true;
    }
    return false;
}

bool iscl(char* packet)
{
    if (packet[0] == 'c' && packet[1] == 'l')
    {
        return true;
    }
    return false;
}
bool issr(char* packet)
{
    if (packet[0] == 's' && packet[1] == 'r')
    {
        return true;
    }
    return false;
}

bool isfirst(char* packet)
{
    if (packet[2] == 'c' && packet[3] == 't')
    {
        return true;
    }
    return false;
}


