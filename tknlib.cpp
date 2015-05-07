#include "tknlib.h"

using namespace std;

void padding(char*  in, int len, char* out)
{
    int dif = len - strlen(in);
    bzero(out,4);

    while(dif > 0){
        strcat(out, "0");
        dif--;
    }
    strcat(out, in);
}


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

void encaps(const char* type, const char* cmd, const char* dst, const char* src, const char* data, char* packet)
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



}

bool issw(char* packet)
{
    char tmp[2];
    copy(packet, packet+2, tmp);

    if (*tmp == 's' && *(tmp+1) == 'w' )
    {
        return true;
    }
    return false;
}

bool iscl(char* packet)
{
    char tmp[2];
    copy(packet, packet+2, tmp);

    if (*tmp == 'c' && *(tmp+1) == 'l' )
    {
        return true;
    }
    return false;
}

bool issr(char* packet)
{
    char tmp[2];
    copy(packet, packet+2, tmp);

    if (*tmp == 's' && *(tmp+1) == 'r' )
    {
        return true;
    }
    return false;
}

bool issp(char* packet)
{
    char tmp[2];
    copy(packet, packet+2, tmp);

    if (*tmp == 's' && *(tmp+1) == 'p' )
    {
        return true;
    }
    return false;
}

bool isfirst(char* packet)
{
    char tmp[2];
    copy(packet+2, packet+4, tmp);

    if (*tmp == 'c' && *(tmp+1) == 't' )
    {
        return true;
    }
    return false;
}

int getip(char* packet)
{
    char res[4];
    res[0] = packet[4];
    res[1] = packet[5];

    return atoi(res);
}

int getdst(char* packet)
{
    char res[4];
    res[0] = packet[4];
    res[1] = packet[5];
    res[2] = packet[6];
    res[3] = packet[7];

    return atoi(res);
}

int getsrc(char* packet)
{
    char res[4];
    res[0] = packet[8];
    res[1] = packet[9];
    res[2] = packet[10];
    res[3] = packet[11];

    return atoi(res);
}

void chdst(char* buf, char* dst, char* packout)
{
    char tmp[120];
    copy(buf+8, buf+128, tmp);
    strncpy(packout, buf, 4);
    strcat(packout, dst);
    strcat(packout, tmp);
}

void chsrc(char* buf, char* src, char* packout)
{
    char tmp[116];
    copy(buf+12, buf+128, tmp);
    strncpy(packout, buf, 8);
    strcat(packout, src);
    strcat(packout, tmp);
}

void chtype(char* buf, char* type, char* packout)
{
    char tmp[126];
    copy(buf+2, buf+128, tmp);
    strcpy(packout, type);
    strcat(packout, tmp);
}

void chdir(char* buf, char* packout)
{
    char src[4];
    char dst[4];
    char tmp[116];
    bzero(src,4);
    bzero(dst,4);
    bzero(tmp,116);
    copy(buf+4, buf+8, src);
    copy(buf+8, buf+12, dst);
    copy(buf+12, buf+128, tmp);
    strncpy(packout, buf, 4);
    strcat(packout, dst);
    strcat(packout, src);
    strcat(packout, tmp);
    cout << "src is:" << src << endl;
    cout << "dst is:" << dst << endl;
    cout << "tmp is:" << tmp << endl;

}

// void chsrc(char* buf, char* src, char* packout)
// {
//     packet[8] = src[0];
//     packet[9] = src[1];
//     packet[10] = src[2];
//     packet[11] = src[3];
// }

// void chtype(char* packet, char* type)
// {
//     // strcpy(packet, type);
//     // strncat(packet, type[0], 2);
//     copy(type+1, type+3, packet);
// }


// void chdir(char* packet)
// {
//     char tmp[4];

//     tmp[0] = packet[4];
//     tmp[1] = packet[5];
//     tmp[2] = packet[6];
//     tmp[3] = packet[7];

//     packet[4] = packet[8];
//     packet[5] = packet[9];
//     packet[6] = packet[10];
//     packet[7] = packet[11];

//     packet[8] = tmp[0];
//     packet[9] = tmp[1];
//     packet[10] = tmp[2];
//     packet[11] = tmp[3];


// }
