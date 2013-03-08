/*client.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#define PORT 80
#define REMOTE_IP "202.136.56.203"

//#define PORT 4321
//#define REMOTE_IP "192.168.1.104"



int s ;
char value_buf[10]={0};

//*******************************************
//
//*******************************************
void getdata_from_yeelink(char *dev_id,char *sr_id,char *api_key)
{
  char sendbuf[512];
	char *str1="GET /v1.0/device/";
	char *str2="/sensor/";
	char *str3="/datapoints HTTP/1.1\r\nHost: api.yeelink.net\r\nAccept: */*\r\n";
	//send http request

	printf("tcp prepare data!--\n");
	bzero(sendbuf,sizeof(sendbuf));
	strcat(sendbuf,str1);
	strcat(sendbuf,dev_id);
	strcat(sendbuf,str2);
	strcat(sendbuf,sr_id);
	strcat(sendbuf,str3);
	strcat(sendbuf,api_key);
	strcat(sendbuf,"\r\n\r\n");

   if(send(s,sendbuf,strlen(sendbuf),0)<0)
      {
         perror("send");
         exit(1);
      }

   printf("tcp send ok!--\n");


}

//******************************************
//
//*******************************************
void yeelink_getdata_unpack(char *strbuf,char *val_buf)
{
    char *cnt_len="Content-Length: ";
    char val_len;
    char *val="value";

    char *len_addr;
    char *val_addr;
    unsigned char i;

    len_addr=strstr(strbuf,cnt_len);
    if(len_addr)
    {
        len_addr+=16;
        val_len=(*len_addr-0x30)*10;
        val_len+=(*(++len_addr)-0x30);
        if(val_len>44)
        {
            val_len-=44;

            val_addr=strstr(strbuf,val);
            if(val_addr)
                {
                val_addr+=7;
                for(i=0;i<val_len;i++)
                    {
                    val_buf[i]=*(val_addr++);
                    }
                }else{
                printf("Value Not Found!\n");
                }

        }

    }
    else
    printf("Not Found!\n");

}
//*******************************************
//                   MAIN
//*******************************************
int   main(int argc,char *argv[])
{

   struct sockaddr_in addr ;
    char mybuffer[512];

   //for yeelink get data----
	char *device_id="1847";
	char *sensor_id="2326";
	char *api_key="U-ApiKey: c3b3d0671f3d962ee2b8aaa1cece81";

   if( (s=socket(AF_INET,SOCK_STREAM,0))<0 )  //IPV4 TCP
   {
      perror("socket");
      exit(1);
   }
   else
   {
    	printf("socket created .\n");
    	printf("socked id: %d \n",s);
	}

   bzero(&addr,sizeof(addr));
   addr.sin_family =AF_INET;  //IPV4
   addr.sin_port=htons(PORT); //SERVER PORT
   addr.sin_addr.s_addr=inet_addr(REMOTE_IP);  //SERVER IP

   if(connect(s,(struct sockaddr *)&addr,sizeof(addr))<0)
   {
      perror("connect");
      exit(1);
   }
   else
   {
   	   printf("connected ok!\n");
   	   printf("remote ip:%s\n",REMOTE_IP);
   	   printf("remote port:%d\n",PORT);
    }

	bzero(mybuffer,512);
//send get command
	getdata_from_yeelink(device_id,sensor_id,api_key);

   recv(s ,mybuffer,sizeof(mybuffer),0);
   printf("---------Recived-----------\n");
    yeelink_getdata_unpack(mybuffer,value_buf);
    printf("Value:%s\n",value_buf);
   while(1)
   {
         bzero(mybuffer,sizeof(mybuffer));
         recv(s ,mybuffer,sizeof(mybuffer),0);
//		if(strlen(mybuffer)>0) printf("%s\n",mybuffer);
   }
}
