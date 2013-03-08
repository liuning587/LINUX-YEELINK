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
//#define REMOTE_IP "192.168.1.120"


int s;

/*****************************************************************************

post data to yeelink eg:12.34

*****************************************************************************/

void send_data_to_yeelink_double(char *device_num,char *sensor_num,char *api_key,char val,char val2)
{

char tmp[6];

char strbuf[512];
char pstr7[17]="{\"value\":";
char *pstr8="}\r\n";

memset(strbuf,0x00,512);
 if(val>10)
{
  tmp[0]=val/10+0x30;
	tmp[1]=val%10+0x30;
}else
{
  	tmp[0]=0x30;
	tmp[1]=val+0x30;
}
tmp[2]=0x2e; //小数点
 if(val2>10)
{
	tmp[3]=val2/10+0x30;
	tmp[4]=val2%10+0x30;
}else
{
  	tmp[3]=0x30;
	tmp[4]=val2+0x30;
}
tmp[5]='\0';
strcat(pstr7,tmp);
strcat(pstr7,pstr8);

//method
strcat(strbuf,"POST /v1.0/device/");
strcat(strbuf,device_num);
strcat(strbuf,"/sensor/");
strcat(strbuf,sensor_num);
strcat(strbuf,"/datapoints HTTP/1.0\r\n");

//head
strcat(strbuf,"Host: api.yeelink.net\r\nAccept: */*\r\nU-ApiKey: ");
strcat(strbuf,api_key);
strcat(strbuf,"\r\n");
strcat(strbuf,"Content-Length: 15\r\n");
strcat(strbuf,"Content-type: application/json;charset=utf-8\r\n");
strcat(strbuf,"Connection: close\r\n\r\n");
strcat(strbuf,pstr7);
printf("%s\n",pstr7);
   if(send(s,strbuf,strlen(strbuf),0)<0)
      {
         perror("send");
         exit(1);
      }
}
//**************************************************************
//                       main
//************************************************************

int   main(int argc,char *argv[])
{
   struct sockaddr_in addr ;
    char mybuffer[256];

 	//yeelink
	char *DEVICE_NUM="1847";   //设备编号
	char *SENSOR_NUM="2326";   //传感器编号
	char *API_KEY="c3b3d0671f3d962ee2b8aaa1cece81";	  //API-KEY
	char sensor_data1=12;
	char sensor_data2=34;


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

	bzero(mybuffer,sizeof(mybuffer));

  //post data to yeelink
   send_data_to_yeelink_double(DEVICE_NUM,SENSOR_NUM,API_KEY,sensor_data1,sensor_data2);

   printf("----------Recive------------\n");
   recv(s ,mybuffer,sizeof(mybuffer),0);
   printf("%s\n",mybuffer);

   while(1)
   {
         bzero(mybuffer,sizeof(mybuffer));
         recv(s ,mybuffer,sizeof(mybuffer),0);
  //       printf("received:%s\n",mybuffer);
   }
}

