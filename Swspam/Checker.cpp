#include "stdafx.h"
#include "Checker.h"

//#include "../MAPI/MapiEx.h"
 
#include "../Swparser/MailParser.h"
#include "../Swsocket/CoreClass.h"
#include "../Swsocket/ClientClass.h"
#include "../Swzip/Compress.h"

#include <commctrl.h>
#include <conio.h>

//#include "../MAPI/MAPI.h"
//#include "../MAPI/MainFrm.h"


void CSpamContact::SetIp(unsigned int ip)
{
	        //CCoreSocket::Startup();

			char aIP[16];

			in_addr addr;
			addr.S_un.S_addr = ip;

			strcpy(aIP,inet_ntoa(addr));

			SetIp(aIP);

			SetIp(this->ReverseIP);
			//CCoreSocket::Cleanup();

}

void CSpamContact::SetIp(char *ip)
{
	strcpy(IP,ip);

	int iplen = strlen(ip);
	int zonecount = 0;
	int SavePos = 0;

	for (int i = 0 ; i <= iplen ; i++)
	{
		if (ip[i] == '.' || i == iplen)
		{
		   memset(IPs[zonecount],0,4);
		   memcpy(IPs[zonecount],ip+SavePos,i-SavePos);

		   iIPs[zonecount][0] = atoi(IPs[zonecount]);

		   zonecount ++;
		   SavePos = i+1;
		}
	
	}

	strcpy(ReverseIP,IPs[3]);
	strcat(ReverseIP,".");
	strcat(ReverseIP,IPs[2]);
	strcat(ReverseIP,".");
	strcat(ReverseIP,IPs[1]);
	strcat(ReverseIP,".");
	strcat(ReverseIP,IPs[0]);
}

int CSpamContact::GetNicWeb(char *ANic)
{

	ANic[0] = 0;
	//apnic 
	if ((iIPs[0][0] >= 58 && iIPs[0][0] <= 61) ||
		(iIPs[0][0] >= 124 && iIPs[0][0] <= 126) ||		
		(iIPs[0][0] >= 202 && iIPs[0][0] <= 203) ||
		(iIPs[0][0] >= 210 && iIPs[0][0] <= 211) ||
		(iIPs[0][0] >= 218 && iIPs[0][0] <= 222) ||
		iIPs[0][0] == 168 || (iIPs[0][0] == 159 && iIPs[1][0] == 226 ) ||
		(iIPs[0][0] == 139 && iIPs[1][0] == 175 ))
	{
	
		strcpy(ANic,"202.12.29.20");
		return APNIC;

	}
	//arin
	else if ((iIPs[0][0] >= 63 && iIPs[0][0] <= 76) ||
		     (iIPs[0][0] == 199) ||
			 (iIPs[0][0] >= 204 && iIPs[0][0] <= 209) ||
			 (iIPs[0][0] == 216) || iIPs[0][0] == 148 || iIPs[0][0] == 24 
			  ) 
	{
	
		strcpy(ANic,"192.149.252.44");
		return ARIN;
	}
	//RIPE
	else if ((iIPs[0][0] == 62) ||
		     (iIPs[0][0] >= 80 && iIPs[0][0] <= 91) ||
			 (iIPs[0][0] >= 193 && iIPs[0][0] <= 195) ||
			 (iIPs[0][0] >= 212 && iIPs[0][0] <= 213) ||
			 (iIPs[0][0] == 217) || iIPs[0][0] == 141)
	{
	
		strcpy(ANic,"193.0.0.214");
		return RIPE;
	}
	//LACNIC
	else if ((iIPs[0][0] >= 189 && iIPs[0][0] <= 190) ||
		     (iIPs[0][0] >= 200 && iIPs[0][0] <= 201) || iIPs[0][0] == 196  )
	{
	
		strcpy(ANic,"200.160.2.15");
		return LACNIC;
	}
	//AFRINIC
	else if (iIPs[0][0] == 41 )//|| iIPs[0][0] == 212))
	{
	
		strcpy(ANic,"196.216.2.1");
		return AFRINIC;
	}


return 0;
}

int CSpamContact::GetNic(char *ANic)
{
	//http://www.iana.org/assignments/ipv4-address-space

	ANic[0] = 0;
	//apnic 
	if ((iIPs[0][0] >= 58 && iIPs[0][0] <= 61) ||
		(iIPs[0][0] >= 124 && iIPs[0][0] <= 126) ||		
		(iIPs[0][0] >= 202 && iIPs[0][0] <= 203) ||
		(iIPs[0][0] >= 210 && iIPs[0][0] <= 211) ||
		(iIPs[0][0] >= 218 && iIPs[0][0] <= 222) ||
		iIPs[0][0] == 168)
	{
	
		strcpy(ANic,"202.12.29.13");
		return APNIC;

	}
	//arin
	else if ((iIPs[0][0] >= 63 && iIPs[0][0] <= 76) ||
		     (iIPs[0][0] == 199) ||
			 (iIPs[0][0] >= 204 && iIPs[0][0] <= 209) ||
			 (iIPs[0][0] == 216) || iIPs[0][0] == 148)
	{
	
		strcpy(ANic,"192.149.252.44");
		return ARIN;
	}
	//RIPE
	else if ((iIPs[0][0] == 62) ||
		     (iIPs[0][0] >= 80 && iIPs[0][0] <= 91) ||
			 (iIPs[0][0] >= 193 && iIPs[0][0] <= 195) ||
			 (iIPs[0][0] >= 212 && iIPs[0][0] <= 213) ||
			 (iIPs[0][0] == 217) )
	{
	
		strcpy(ANic,"193.0.0.135");
		return RIPE;
	}
	//LACNIC
	else if ((iIPs[0][0] >= 189 && iIPs[0][0] <= 190) ||
		     (iIPs[0][0] >= 200 && iIPs[0][0] <= 201)  )
	{
	
		strcpy(ANic,"200.160.2.15");
		return LACNIC;
	}
	//AFRINIC
	else if (iIPs[0][0] == 41 )//|| iIPs[0][0] == 212))
	{
	
		strcpy(ANic,"196.216.2.1");
		return AFRINIC;
	}


return 0;
}

CSpamContact::CSpamContact()
{
	bSpamCop = false;

	contact = NULL;
	netname = NULL;
	country = NULL;
	InetNumS = NULL;
	InetNumE = NULL;



 
/*	memset(&contact,0,sizeof(contact));
	memset(&netname,0,sizeof(netname));
	memset(&country,0,sizeof(country));
	memset(&InetNumS,0,sizeof(InetNumS));
	memset(&InetNumE,0,sizeof(InetNumE));*/

	CCoreSocket::Startup();

}

CSpamContact::~CSpamContact()
{

 
	CCoreSocket::Cleanup();

}


void CSpamContact::GetContact(char *_InetNumS , char* _InetNumE, char* _netname,char* _country,char* _contact)
{

	
	if (_InetNumS == NULL || _InetNumE == NULL || _netname == NULL || _country == NULL || _contact == NULL)
	{
		return;
	}
	
	_InetNumS[0] = 0;
	_InetNumE[0] = 0;
	_netname[0] = 0;
	_country[0] = 0;
	_contact[0] = 0;

	InetNumS = _InetNumS;
	InetNumE = _InetNumE;
	netname = _netname;
	country = _country;
	contact = _contact;


	char nic[16];
	memset(nic,0,16);

	int niccode = LACNIC;
	strcpy(nic,"200.160.2.15");
	//int niccode = GetNicWeb(nic);
	//if (nic[0] == 0) return ;

	//CCoreSocket::Startup();
	
	/*CWhoisClient ws;
	char ipstr[100];
	memset(ipstr,0,100);
	if (niccode == 2)
	{
	 strcpy(ipstr,"+");
	 strcat(ipstr,IP);
	}
	else
	{
	 strcpy(ipstr,IP);
	}

	HANDLE h = ws.Send(nic, ipstr, 3000, 10000);*/

	CHTTPclient http;
	HANDLE hand;
	if (niccode == APNIC)
	{
		string sendstr;
		sendstr = "searchtext=";
		sendstr = sendstr + IP;

		

		hand = http.IPPOST(nic,"www.apnic.net","/apnic-bin/whois.pl",(char *) sendstr.c_str(),2 * 60 * 1000, 2 * 60 * 1000);  	
	
	}else if  (niccode == ARIN)
	{
		string sendstr;
		sendstr = "queryinput=%2B";
		sendstr = sendstr + IP;

		hand = http.IPPOST(nic,"ws.arin.net","/whois", (char *) sendstr.c_str(),2 * 60 * 1000, 2 * 60 * 1000);  	
	
	}
	else if  (niccode == RIPE)
	{
		string sendstr;
		sendstr = "//fcgi-bin//whois?form_type=simple&full_query_string=&do_search=Search&searchtext=";
		sendstr = sendstr + IP;		

		hand = http.IPGET(nic,"www.ripe.net",(char *) sendstr.c_str(),2 * 60 * 1000, 2 * 60 * 1000);  	
	
	}
	else if  (niccode == LACNIC)
	{
		string sendstr;
		sendstr = "lg=SP&query=";
		sendstr = sendstr + IP;		

		hand = http.IPPOST(nic,"lacnic.net","/cgi-bin/lacnic/whois",(char *) sendstr.c_str(),2 * 60 * 1000, 2 * 60 * 1000);  	
	
	}
	else if  (niccode == AFRINIC)
	{
		string sendstr;
		sendstr = "searchtext=";
		sendstr = sendstr + IP;		

		hand = http.IPPOST(nic,"www.afrinic.net","/cgi-bin/whois",(char *) sendstr.c_str(),2 * 60 * 1000, 2 * 60 * 1000);  	
	
	}


	DWORD  h2 = WaitForSingleObject(hand, 3 * 60 * 1000); 

	if (h2 == WAIT_OBJECT_0)
	{
		 
		FILE *fp;
		fp = fopen(http.GetTempFilePath(),"r+b");
	 

		fseek(fp,0,SEEK_END);
		int filesize = ftell(fp);

		



		fseek(fp,0,SEEK_SET);

		HANDLE pHeap = GetProcessHeap();
		//char *buff = new char[filesize+1];
		char *buff = (char *) CHeap::GetHeap(filesize+1,&pHeap);
		memset(buff,0,filesize+1);

		int ReadSize = fread(buff,1,filesize,fp);
		//if (ReadSize != filesize) throw;
		fclose(fp);	  

		
		char *buffTrim = (char *) CHeap::GetHeap(filesize+1,&pHeap);

		//char *buffTrim = new char[filesize+1];

		CMailCodec cm;
		cm.TrimHTML(buff,buffTrim);


		//delete buff;
		CHeap::FreeHeap(buff,&pHeap);
		buff = buffTrim;

		int nicp = ParseLACNICHeader(buff);

	
	
		if (nicp == APNIC || nicp == AFRINIC )
		{
			ParseAPNIC(buff); 
		}
		else if (nicp == RIPE )
		{		
			ParseRIPE(buff);
		}
		else if (nicp == ARIN)
		{
			ParseARIN(buff);
			
			
			if (InetNumS[0] == 0 &&
			InetNumE[0] == 0 &&
			contact[0] == 0)
			{
				
				CHeap::FreeHeap(buff,&pHeap);

				CHTTPclient http2;
				HANDLE hand;

				string sendstr;
				sendstr = "queryinput=%2B";
				sendstr = sendstr + IP;

				hand = http2.IPPOST("192.149.252.44","ws.arin.net","/whois", (char *) sendstr.c_str(),2 * 60 * 1000, 2 * 60 * 1000);  	

				DWORD  h2 = WaitForSingleObject(hand, 3 * 60 * 1000); 

				if (h2 == WAIT_OBJECT_0)
				{
					 
					FILE *fp;
					fp = fopen(http2.GetTempFilePath(),"r+b");
				 

					fseek(fp,0,SEEK_END);
					int filesize = ftell(fp);


					fseek(fp,0,SEEK_SET);

					HANDLE pHeap = GetProcessHeap();					
					char *buff2 = (char *) CHeap::GetHeap(filesize+1,&pHeap);
					memset(buff2,0,filesize+1);

					int ReadSize = fread(buff2,1,filesize,fp);					
					fclose(fp);	 
					
					DeleteFile(http2.GetTempFilePath());

					
					char *buffTrim = (char *) CHeap::GetHeap(filesize+1,&pHeap);

					//char *buffTrim = new char[filesize+1];

					CMailCodec cm;
					cm.TrimHTML(buff2,buffTrim);


					//delete buff;
					CHeap::FreeHeap(buff2,&pHeap);
					buff = buffTrim;

					ParseARIN(buff);

			
				}
			}
		}  
		else if (nicp == LACNIC || nicp == BRNIC)
		{
		
			ParseLACNIC(buff);
			
			if (country[0] == 0 && nicp == BRNIC)
			strcpy(country,"BR");

		}	
	 
		//if (strlen(buff) == 0) throw;

		if (InetNumS[0] == 0 ||
			InetNumE[0] == 0 ||
			contact[0] == 0)
		{
			//throw ;
			//return;
		}
	
		//if (InetNumS[0] != 0)
		//{
		
			//GetSpamCop();
		
		//}

	 

		 
		//delete buff;
		CHeap::FreeHeap(buff,&pHeap);
		 
		
	}

	DeleteFile(http.GetTempFilePath());


 

	//CCoreSocket::Cleanup();

	//return contact;

}

unsigned long CSpamContact::GetIPValue()
{
  unsigned long IpValue=0;

	//CCoreSocket::Startup();

	   IpValue = inet_addr(ReverseIP);
	  
	//CCoreSocket::Cleanup();

  return IpValue;

}
void CSpamContact::ParseAPNIC(char *WhoisStr)
{

 



	int len = strlen(WhoisStr);
	int line = 0;
	int step = 0;
	char InetNumStr[100];
	int Count = 0;

	bool FoundRemark = false;
	int GetIdx = 0;

	memset(InetNumStr,0,100);



	for (int i = 0 ; i < len ; i++)
	{
	
		if (step == 0 && WhoisStr[i] == 10 && WhoisStr[i+1] == 10   )//&&
			//WhoisStr[i+2] == 13 && WhoisStr[i+3] == 10)
		{
			 
				
			line ++;

			//if (line == 2)
			//{
				//memmove(WhoisStr,WhoisStr+i+2,len-i);
				//break;
				step = 1;
				i = i + 1;


			//}
		
		} 
		else if (step == 1)
		{
		
			if (GetIdx == 0 && strnicmp(WhoisStr+i,"inetnum:",8) == 0  )
			{
			
				

				
				step = 2;
				i = i + 8;

			}
			else if (GetIdx == 1 && strnicmp(WhoisStr+i,"netname:",8) == 0  )
			{
			
				step = 2;
				i = i + 8;

			}
			else if (GetIdx == 2 && strnicmp(WhoisStr+i,"country:",8) == 0  )
			{
			
				step = 2;
				i = i + 8;

			}
			else if (GetIdx == 3 && strnicmp(WhoisStr+i,"e-mail:",7) == 0  )
			{
			
				step = 2;
				i = i + 7;

			} else	
			//if (!FoundRemark && strnicmp(WhoisStr+i,"remarks:",8) == 0)
			  
			if (!FoundRemark && (strnicmp(WhoisStr+i,"spam",4) == 0 || strnicmp(WhoisStr+i,"abuse",4) == 0))
			{
				int SaveStartPos = -1;
				int FoundAbusePos = -1;
				int FoundLine = 0;

				i = i + 4;
				for (int k = i  ; k < len ; k ++)
				{
					//if(strnicmp(WhoisStr+k,"abuse@",6) == 0 ||  strnicmp(WhoisStr+k,"spam@",5) == 0)
					if(WhoisStr[k] == '@')
					{
						if (SaveStartPos != -1)
						{
							FoundAbusePos = SaveStartPos;
						}
						else  //相連 key word : ispabuse@nrtc.coop
						{
							for(int j = k ; j > 0 ; j --)
							{
								if (!isalnum(WhoisStr[j]) && WhoisStr[j] != '@' && WhoisStr[j] != '.' && WhoisStr[j] != '_' && WhoisStr[j] != '-' ) 
								{
									FoundAbusePos = j + 1;
									break;
								}
							}
						}
					/*	else
						{
							FoundAbusePos = k;
						}*/
						
					}

					//if ((!isalpha(WhoisStr[k]) && WhoisStr[k] != '@' && WhoisStr[k] != '.' && WhoisStr[k] != '_' && WhoisStr[k] != '-') || WhoisStr[k] == 10)
					if ((!isalnum(WhoisStr[k])  && WhoisStr[k] != '@' && WhoisStr[k] != '.' && WhoisStr[k] != '_' && WhoisStr[k] != '-') || WhoisStr[k] == 10)
					{
						if (FoundAbusePos != -1)
						{
							//found abuse
							if (k - FoundAbusePos > 0)
							{
							  strncpy(contact,WhoisStr+FoundAbusePos, k - FoundAbusePos);
							  contact[k - FoundAbusePos] = 0;
							}

							if (strlen(contact) > 4)
							{
								FoundRemark = true;
								i = k;
							}
							break;
						
						} else if (WhoisStr[k] == 10)
						{
							if (FoundLine < 2) //允許兩行的尋找
							{
								FoundLine++;
								SaveStartPos = k + 1;

							}
							else
							{
								i = k;
								break;
							}
						}
						else
						{
							SaveStartPos = k + 1;
						}
					}
					
					

					

					
				}
			
			
			}


			
		}
		else if (step == 2)
		{
			if ( WhoisStr[i] != 10)
			{
			
				if ((Count == 0 && WhoisStr[i] != ' ') || Count > 0 )
				{
					if(GetIdx ==0 && Count < sizeof(InetNumStr))
					{
						InetNumStr[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 1 && Count < 255)
					{
						netname[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 2 && Count < 3 && Count <= 1)
					{
						country[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 3 && Count < 255 && !FoundRemark)
					{
						contact[Count] = WhoisStr[i];
						Count ++;
					}
				}

				
				
			}
			else if (Count > 0)
			{
				if(GetIdx ==0)
				{
					InetNumStr[Count] = 0;
				}
				else if (GetIdx == 1)
				{
				    netname[Count] = 0;
				}
				else if (GetIdx == 2)
				{
				    country[Count] = 0;
				}
				else if (GetIdx == 3)
				{
				    contact[Count] = 0;
				}

				
				GetIdx ++;
				Count = 0;
				step = 1;



				if (GetIdx == 4)
				{
					GetIdx = 0;
				}
			}
		
		}

	}

	//_cprintf("%s\r\n",Email);
	//TrimAt(Email);

	int inetlen = strlen(InetNumStr);

	int CharIdx = 0;
	step = 0; 
	for(int j = 0 ; j < inetlen ; j ++)
	{
		
		if (step ==0 && (isdigit(InetNumStr[j]) || InetNumStr[j] == '.') )
		{
			InetNumS[CharIdx] = InetNumStr[j];
			CharIdx++;
		
		}
		if (step ==0 && InetNumStr[j] == ' ')
		{
			step = 1;
			CharIdx = 0;
		}
		else if (step == 1 && (isdigit(InetNumStr[j]) || InetNumStr[j] == '.'))
		{
			InetNumE[CharIdx] = InetNumStr[j];	
			CharIdx++;
		
		}

	}

	/*strcpy(contact,"APNIC\n");
	strcat(contact,InetNumStr);
	strcat(contact,"\n");
	strcat(contact,netname);
	strcat(contact,"\n");
	strcat(contact,country);*/

}

void CSpamContact::ParseRIPE(char *WhoisStr)
{

 

	int len = strlen(WhoisStr);
	int line = 0;
	int step = 0;
	char InetNumStr[100];
	int Count = 0;

	int GetIdx = 0;

	bool FoundRemark = false;

	memset(InetNumStr,0,100);

	for (int i = 0 ; i < len ; i++)
	{
	
		if (step == 0 && WhoisStr[i] == 10 && WhoisStr[i+1] == 10   )//&&
			//WhoisStr[i+2] == 13 && WhoisStr[i+3] == 10)
		{
			 
				
			line ++;

			//if (line == 2)
			//{
				//memmove(WhoisStr,WhoisStr+i+2,len-i);
				//break;
				step = 1;
				i = i + 1;


			//}
		
		} 
		else if (step == 1)
		{
		
		
			
			
			if (GetIdx == 0 && strnicmp(WhoisStr+i,"inetnum:",8) == 0  )
			{
			
			

				step = 2;
				i = i + 8;

			}
			else if (GetIdx == 1 && strnicmp(WhoisStr+i,"netname:",8) == 0  )
			{
			
				step = 2;
				i = i + 8;

			}
			else if (GetIdx == 2 && strnicmp(WhoisStr+i,"country:",8) == 0  )
			{
			
				step = 2;
				i = i + 8;

			}
			else if (GetIdx == 3 && (strnicmp(WhoisStr+i,"abuse-mailbox:",14) == 0 ) )
			{
			
				step = 2;
				i = i + 14;

			}
			else if (GetIdx == 3 && (strnicmp(WhoisStr+i,"e-mail:",7) == 0) )
			{
			
				step = 2;
				i = i + 7;

			} else	// check remark
			//if (!FoundRemark && strnicmp(WhoisStr+i,"remarks:",8) == 0)
			if (!FoundRemark && (strnicmp(WhoisStr+i,"spam",4) == 0 || strnicmp(WhoisStr+i,"abuse",4) == 0))
			{
				int SaveStartPos = -1;
				int FoundAbusePos = -1;
				int FoundLine = 0;
				i = i + 4;
				for (int k = i  ; k < len ; k ++)
				{
					//if(strnicmp(WhoisStr+k,"abuse@",6) == 0 ||  strnicmp(WhoisStr+k,"spam@",5) == 0)
					if(WhoisStr[k] == '@')
					{
						if (SaveStartPos != -1)
						{
							FoundAbusePos = SaveStartPos;
						}
						else  //相連 key word : ispabuse@nrtc.coop
						{
							for(int j = k ; j > 0 ; j --)
							{
								if (!isalnum(WhoisStr[j]) && WhoisStr[j] != '@' && WhoisStr[j] != '.' && WhoisStr[j] != '_' && WhoisStr[j] != '-' ) 
								{
									FoundAbusePos = j + 1;
									break;
								}
							}
						}
					 
						
					}

					if ((!isalnum(WhoisStr[k])  && WhoisStr[k] != '@' && WhoisStr[k] != '.' && WhoisStr[k] != '_' && WhoisStr[k] != '-') || WhoisStr[k] == 10)
					{
						if (FoundAbusePos != -1)
						{
							//found abuse
							if (k - FoundAbusePos > 0)
							{
							  strncpy(contact,WhoisStr+FoundAbusePos, k - FoundAbusePos);
							  contact[k - FoundAbusePos] = 0;
							}

							if (strlen(contact) > 4)
							{
								FoundRemark = true;
								i = k;
							}

							break;
						
						}
						else if (WhoisStr[k] == 10)
						{
							if (FoundLine < 2) //允許兩行的尋找
							{
								FoundLine++;
								SaveStartPos = k + 1;

							}
							else
							{
								i = k;
								break;
							}
						}
						else
						{
							SaveStartPos = k + 1;
						}
					}
					
					

					

					
				}
			
			
			}



			

			
		}
		else if (step == 2)
		{
			if ( WhoisStr[i] != 10)
			{
				if ((Count == 0 && WhoisStr[i] != ' ') || Count > 0 )
				{
					if(GetIdx ==0 && Count < sizeof(InetNumStr))
					{
						InetNumStr[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 1 && Count < 255)
					{
						netname[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 2 && Count < 3 && Count <= 1)
					{
						country[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 3 && Count < 255 && !FoundRemark)
					{
						contact[Count] = WhoisStr[i];
						Count ++;
					}
				}

				
				
			}
			else if (Count > 0)
			{
				if(GetIdx ==0 )
				{
					InetNumStr[Count] = 0;
				}
				else if (GetIdx == 1 )
				{
				    netname[Count] = 0;
				}
				else if (GetIdx == 2 )
				{
				    country[Count] = 0;
				}
				else if (GetIdx == 3 )
				{
				    contact[Count] = 0;
				}

				
				GetIdx ++;
				Count = 0;
				step = 1;



				if (GetIdx == 4)
				{
					GetIdx = 0;
				}
			}
		
		}

	}

	//_cprintf("%s\r\n",Email);
	//TrimAt(Email);

	int inetlen = strlen(InetNumStr);

	int CharIdx = 0;
	step = 0; 
	for(int j = 0 ; j < inetlen ; j ++)
	{
		
		if (step ==0 && (isdigit(InetNumStr[j]) || InetNumStr[j] == '.') )
		{
			InetNumS[CharIdx] = InetNumStr[j];
			CharIdx++;
		
		}
		if (step ==0 && InetNumStr[j] == ' ')
		{
			step = 1;
			CharIdx = 0;
		}
		else if (step == 1 && (isdigit(InetNumStr[j]) || InetNumStr[j] == '.'))
		{
			InetNumE[CharIdx] = InetNumStr[j];	
			CharIdx++;
		
		}

	}

	 
	/*for(int j = 0 ; j < inetlen ; j ++)
	{
		if (InetNumStr[j] == ' ')
		{
		
			strncpy(InetNumS,InetNumStr,j);
			InetNumS[j] = 0;

			strncpy(InetNumE,InetNumStr+j+3,inetlen-j-3);
			InetNumE[inetlen-j-3] = 0;
			break;
			 
		}
		

	

	}*/

	/*strcpy(contact,"RIPE\n");
	strcat(contact,InetNumStr);
	strcat(contact,"\n");
	strcat(contact,netname);
	strcat(contact,"\n");
	strcat(contact,country);*/

}

char CSpamContact::ParseLACNICHeader(char *WhoisStr)
{

	int IgnoreLine = 0;

	int len = strlen(WhoisStr);
	for (int i = 0 ; i < len ; i++)
	{
		if ( WhoisStr[i] == 10 && ( WhoisStr[i+1] == '%' || WhoisStr[i+1] == '#'))
		{
			IgnoreLine++;

			if (IgnoreLine > 2)
			{
			
				for (int j=2 ; j < 42 ; j++)
				{
					if (strncmp(WhoisStr+i+j,"whois.apnic.net",15) == 0)
					{
						return 	APNIC;				
					}
					else if (strncmp(WhoisStr+i+j,"LACNIC",6) == 0  )
					{
						return LACNIC;
					}
					else if (strncmp(WhoisStr+i+j,"registro.br",11) == 0)
					{
					   return BRNIC;
					}
					else if (strncmp(WhoisStr+i+j,"RIPE",4) == 0)
					{
						return RIPE;
					}

					else if (strncmp(WhoisStr+i+j,"AfriNIC",7) == 0)
					{
						return AFRINIC;
					}
					else if (strncmp(WhoisStr+i+j,"ARIN",4) == 0)
					{
						return ARIN;
					}//registro.br

					 
					
					
				}
			
			}

		}

	}


	return 0;
}

void CSpamContact::ParseLACNIC(char *WhoisStr)
{
 



	int len = strlen(WhoisStr);
	int line = 0;
	int step = 0;
	
	char InetNumStr[100];
	int Count = 0;

	bool FoundRemark = false;
	int GetIdx = 0;

	memset(InetNumStr,0,100);

	for (int i = 0 ; i < len ; i++)
	{
	
		if (step == 0 && WhoisStr[i] == 10 && WhoisStr[i+1] == 10 )
		{
			 
				
			line ++;

			if (line == 2)
			{
				//memmove(WhoisStr,WhoisStr+i+2,len-i);
				//break;
				step = 1;
				i = i + 3;


			}
		
		} 
		else if (step == 1)
		{
		
			if (GetIdx == 0 &&  strnicmp(WhoisStr+i,"inetnum:",8) == 0 )
			{
			
			

				step = 2;
				i = i + 8;

			}else if (GetIdx == 1 && strnicmp(WhoisStr+i,"owner:",6) == 0  )
			{
			
				step = 2;
				i = i + 6;

			}
			else if (GetIdx == 2 && strnicmp(WhoisStr+i,"country:",8) == 0 && Count <= 1 )
			{
			
				step = 2;
				i = i + 8;

			}
			else if ((GetIdx == 2 || GetIdx == 3) && strnicmp(WhoisStr+i,"e-mail:",7) == 0  )
			{
			
				step = 2;
				i = i + 7;
				GetIdx = 3;

			}
			else	// check remark
			//if (!FoundRemark && strnicmp(WhoisStr+i,"remarks:",8) == 0)
			if (!FoundRemark && (strnicmp(WhoisStr+i,"spam",4) == 0 || strnicmp(WhoisStr+i,"abuse",4) == 0))
			{
				int SaveStartPos = -1;
				int FoundAbusePos = -1;
				int FoundLine = 0;

				i = i + 4;
				for (int k = i  ; k < len ; k ++)
				{
					//if(strnicmp(WhoisStr+k,"abuse@",6) == 0 ||  strnicmp(WhoisStr+k,"spam@",5) == 0)
					if(WhoisStr[k] == '@')
					{
						if (SaveStartPos != -1)
						{
							FoundAbusePos = SaveStartPos;
						}
						else //相連 key word : ispabuse@nrtc.coop
						{
							for(int j = k ; j > 0 ; j --)
							{
								if (!isalnum(WhoisStr[j]) && WhoisStr[j] != '@' && WhoisStr[j] != '.' && WhoisStr[j] != '_' && WhoisStr[j] != '-' ) 
								{
									FoundAbusePos = j + 1;
									break;
								}
							}
						}
						
					}

					//if ((!isalpha(WhoisStr[k]) && WhoisStr[k] != '@' && WhoisStr[k] != '.' && WhoisStr[k] != '_' && WhoisStr[k] != '-') || WhoisStr[k] == 10)
					if ((!isalnum(WhoisStr[k])  && WhoisStr[k] != '@' && WhoisStr[k] != '.' && WhoisStr[k] != '_' && WhoisStr[k] != '-') || WhoisStr[k] == 10)
					{
						if (FoundAbusePos != -1)
						{
							//found abuse
							
							if ( k - FoundAbusePos > 0)
							{
								strncpy(contact,WhoisStr+FoundAbusePos, k - FoundAbusePos);
								contact[k - FoundAbusePos] = 0;
							}

							if (strlen(contact) > 4)
							{
								FoundRemark = true;
								i = k;
							}
							break;
						
						}
						else if ( WhoisStr[k] == 10)
						{
							if (FoundLine < 2) //允許兩行的尋找
							{
								FoundLine++;
								SaveStartPos = k + 1;

							}
							else
							{
								i = k;
								break;
							}
						}
						else
						{
							SaveStartPos = k + 1;
						}
					}
					
					

					

					
				}
			
			
			}


			
			
		}
		else if (step == 2)
		{
			if ( WhoisStr[i] != 10)
			{
				if ((Count == 0 && WhoisStr[i] != ' ') || Count > 0 )
				{

					if(GetIdx ==0 && Count < sizeof(InetNumStr))
					{
						InetNumStr[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 1 && Count < 255)
					{
						netname[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 2 && Count < 3)
					{
						country[Count] = WhoisStr[i];
						Count ++;
					}
					else if (GetIdx == 3 && Count < 255 && !FoundRemark)
					{
						contact[Count] = WhoisStr[i];
						Count ++;
					}
				}

				
				
			}
			else if (Count > 0)
			{
				if(GetIdx ==0)
				{
					InetNumStr[Count] = 0;
				}
				else if (GetIdx == 1)
				{
				    netname[Count] = 0;
				}
				else if (GetIdx == 2)
				{
				    country[Count] = 0;
				}
				else if (GetIdx == 3)
				{
				    contact[Count] = 0;
				}

				
				GetIdx ++;
				Count = 0;
				step = 1;



				if (GetIdx == 4)
				{
					GetIdx = 0;
				}
			}
		
	 
		
		}

	}

	int inetlen = strlen(InetNumStr);

	//計算 Dot 數
	int DotCount = 0;
 
	for(int j = 0 ; j < inetlen ; j ++)
	{
		
		if (InetNumStr[j]  == '.')
		{
		
			DotCount++;
		
		}
		if (InetNumStr[j] == '/')
		{
		
			if (j > 0)
			{
				strncpy(InetNumS,InetNumStr,j);
				InetNumS[j] = 0;
			}

			for (int i = 0 ; i < 3 - DotCount  ; i ++)
			{
				strcat(InetNumS,".0");
			}
			
			
			char Subnet[5];
			memset(Subnet,0,5);

			if (inetlen-j-1 > 0)
			{
				strncpy(Subnet,InetNumStr+j+1,inetlen-j-1);
				Subnet[inetlen-j-1] = 0;
			}

			int iSubnet = 32 - atoi(Subnet);
			unsigned int SubnetLen = 1;
			
			for (int k = 0 ; k < iSubnet ; k++)
			{
				SubnetLen = SubnetLen * 2;
			}


			CSpamContact sc1,sc2;
			sc1.SetIp(InetNumS);
			unsigned Startip = sc1.GetIPValue();
			unsigned Endip =  Startip + SubnetLen -1;

			 
			sc2.SetIp(Endip);
			strcpy(InetNumE,sc2.IP);
		 
			break;
			 
		}
		

	

	}

	//_cprintf("%s\r\n",Email);
	//TrimAt(Email);
	/*strcpy(contact,"LACNIC\n");
	strcat(contact,InetNumStr);
	strcat(contact,"\n");
	strcat(contact,netname);
	strcat(contact,"\n");
	strcat(contact,country);*/

}


void CSpamContact::ParseARIN(char *WhoisStr)
{

 

	int len = strlen(WhoisStr);
	int line = 0;
	int step = 0;
	char InetNumStr[100];
	int Count = 0;
	bool FoundRemark = false;

	int GetIdx = 0;

	memset(InetNumStr,0,100);

	for (int i = 0 ; i < len ; i++)
	{
	
		if (step == 0 && WhoisStr[i] == 10   )//&&
			//WhoisStr[i+2] == 13 && WhoisStr[i+3] == 10)
		{
			 
				
			//line ++;

			//if (line == 2)
			//{
				//memmove(WhoisStr,WhoisStr+i+2,len-i);
				//break;
				step = 1;
				i = i + 1;


			//}
		
		} 
		else if (step == 1)
		{
		
			//if ((GetIdx == 0 || GetIdx == 4 ) && strnicmp(WhoisStr+i,"Country:",8) == 0)
			if ((GetIdx == 0  ) && strnicmp(WhoisStr+i,"Country:",8) == 0)
			{
			
			

				step = 2;
				i = i + 8;

			}
			else if (GetIdx == 1 && strnicmp(WhoisStr+i,"NetRange:",9) == 0  )
			{
			
				step = 2;
				i = i + 9;

			}
			else if (GetIdx == 2 && strnicmp(WhoisStr+i,"NetName:",8) == 0  )
			{
			
				step = 2;
				i = i + 8;

			}
			else if (GetIdx == 3 && strnicmp(WhoisStr+i,"Email:",6) == 0 )
			{
				step = 2;
				i = i + 6;
			
			}
			else if ((GetIdx == 3 ||GetIdx == 4) && strnicmp(WhoisStr+i,"RTechEmail:",11) == 0 )
			{
				step = 2;
				i = i + 11;
			
			}
			else if ((GetIdx == 3 || GetIdx == 4 || GetIdx == 5) && strnicmp(WhoisStr+i,"OrgAbuseEmail:",14) == 0  )
			{
			
				step = 2;
				i = i + 14;

			}
			else	// check remark
			//if (!FoundRemark && strnicmp(WhoisStr+i,"remarks:",8) == 0)
			if (!FoundRemark && (strnicmp(WhoisStr+i,"spam",4) == 0 || strnicmp(WhoisStr+i,"abuse",4) == 0))
			{
				int SaveStartPos = -1;
				int FoundAbusePos = -1;
				int FoundLine = 0;

				i = i + 4;
				for (int k = i  ; k < len ; k ++)
				{
					//if(strnicmp(WhoisStr+k,"abuse@",6) == 0 ||  strnicmp(WhoisStr+k,"spam@",5) == 0)
					if(WhoisStr[k] == '@')
					{
						if (SaveStartPos != -1)
						{
							FoundAbusePos = SaveStartPos;
						}
						else 
						{
							for(int j = k ; j > 0 ; j --)
							{
								if (!isalnum(WhoisStr[j]) && WhoisStr[j] != '@' && WhoisStr[j] != '.' && WhoisStr[j] != '_' && WhoisStr[j] != '-' ) 
								{
									FoundAbusePos = j + 1;
									break;
								}
							}
						}
					 
						
					}

					//if ((!isalpha(WhoisStr[k]) && WhoisStr[k] != '@' && WhoisStr[k] != '.' && WhoisStr[k] != '_' && WhoisStr[k] != '-') || WhoisStr[k] == 10)
					if ((!isalnum(WhoisStr[k])  && WhoisStr[k] != '@' && WhoisStr[k] != '.' && WhoisStr[k] != '_' && WhoisStr[k] != '-') || WhoisStr[k] == 10)
					{
						if (FoundAbusePos != -1)
						{
							//found abuse
							if (k - FoundAbusePos > 0)
							{
								strncpy(contact,WhoisStr+FoundAbusePos, k - FoundAbusePos);
								contact[k - FoundAbusePos] = 0;
							}

							if (strlen(contact) > 4)
							{
								FoundRemark = true;
								i = k;
							}
							break;
						
						}
						else if ( WhoisStr[k] == 10)
						{
							if (FoundLine < 2) //允許兩行的尋找
							{
								FoundLine++;
								SaveStartPos = k + 1;

							}
							else
							{
								i = k;
								break;
							}
						}
						else
						{
							SaveStartPos = k + 1;
						}
					}
			 
					
				}
			
			
			}




		
		
			

			
			
		}
		else if (step == 2)
		{
			if ( WhoisStr[i] != 10)
			{
				if ((Count == 0 && WhoisStr[i] != ' ') || Count > 0 )
				{

					if(GetIdx ==0 && Count < 3 && Count <= 1)
					{
						country[Count] = WhoisStr[i];
						Count ++;
						
					}
					else if (GetIdx == 1 && Count < sizeof(InetNumStr))
					{
						InetNumStr[Count] = WhoisStr[i];
						Count ++;
						
					}
					else if (GetIdx == 2 && Count < 255)
					{
						netname[Count] = WhoisStr[i];
						Count ++;
					}
					else if ((GetIdx == 3 || GetIdx == 4 || GetIdx == 5) && Count < 255 && !FoundRemark)
					{
						contact[Count] = WhoisStr[i];
						Count ++;
					}
				}

				
				
			}
			else if (Count > 0)
			{
				if(GetIdx ==0)
				{
					country[Count] = 0;
					
				}
				else if (GetIdx == 1)
				{
				    InetNumStr[Count] = 0;
				}
				else if (GetIdx == 2)
				{
				    netname[Count] = 0;
				}
				else if ((GetIdx == 3 || GetIdx == 4 || GetIdx == 5))
				{
				    contact[Count] = 0;
				}

				
				GetIdx ++;
				Count = 0;
				step = 1;



				if (GetIdx == 6)
				{
					GetIdx = 0;
				}
			}
		
		}
		
		

	}


	int inetlen = strlen(InetNumStr);
	//InetNumStr[inetlen-1] = 0;
	//inetlen--;

 
	int CharIdx = 0;
	step = 0; 
	for(int j = 0 ; j < inetlen ; j ++)
	{
		
		if (step ==0 && (isdigit(InetNumStr[j]) || InetNumStr[j] == '.') )
		{
			InetNumS[CharIdx] = InetNumStr[j];
			CharIdx++;
		
		}
		if (step ==0 && InetNumStr[j] == ' ')
		{
			step = 1;
			CharIdx = 0;
		}
		else if (step == 1 && (isdigit(InetNumStr[j]) || InetNumStr[j] == '.'))
		{
			InetNumE[CharIdx] = InetNumStr[j];	
			CharIdx++;
		
		}

	}

	/* 
	for(int j = 0 ; j < inetlen ; j ++)
	{
		if (InetNumStr[j] == ' ')
		{
		
			strncpy(InetNumS,InetNumStr,j);
			InetNumS[j] = 0;

			strncpy(InetNumE,InetNumStr+j+3,inetlen-j-3);
			InetNumE[inetlen-j-3] = 0;
			break;
			 
		}
		

	

	}*/

	//TrimAt(Email);
	/*strcpy(contact,"ARIN\n");
	strcat(contact,InetNumStr);
	strcat(contact,"\n");
	strcat(contact,netname);
	strcat(contact,"\n");
	strcat(contact,country);*/

}

void CSpamContact::TrimResponse(char *AStr)
{
	char ContactMail[5][200]; 
	char AbuseMail[200];
	char OtherMail[200];

	AbuseMail[0] = 0;
	OtherMail[0] = 0;	 

	int SavePos = 0;
	int MailCount = 0;
	int step = 0;

	int len = strlen(AStr);
	for (int i = 0 ; i < len ; i ++)
	{
	
		if (step == 0 && strnicmp(AStr+i,"Reporting addresses:",20) == 0) //先找 Reporting addresses:
		{
			step = 1;
			i += 19;
			/*
			if (MailCount < 5)
			{
				memset(ContactMail[MailCount],0,200);
				strncpy(ContactMail[MailCount],AStr+SavePos,i-SavePos);
				step = 1;
				MailCount ++;

			} */
		
		} else if(step == 1 &&  strnicmp(AStr+i,"mailto:",7) == 0 )  //找 mailto:
		{
		
			SavePos = i + 7;
			i += 6;
			step = 2;
		
		}
		else if (step == 2 && AStr[i] == '"') //找 " 
		{
			if (MailCount < 5)
			{
				memset(ContactMail[MailCount],0,200);
				if (i-SavePos > 0)
				{
					
					strncpy(ContactMail[MailCount],AStr+SavePos,i-SavePos);
					MailCount ++;
				}

				step = 1;
				
			} 
		
		}
		 
	}


	if (MailCount > 0)
	{
	
		for (int i = 0 ; i < MailCount ; i++)
		{
			//int len = strlen(ContactMail[i]);
			//for (int j = 0 ; j < len ; j++)
		//	{
			if (strstr(ContactMail[i],"spam") != NULL)
			{
			
					strcpy(AStr,ContactMail[i]);
					bSpamCop = true;
					return;
			
			}
			else if (strstr(ContactMail[i],"abuse") != NULL)
			{
					strcpy(AbuseMail,ContactMail[i]);
			}
			else
			{
					strcpy(OtherMail,ContactMail[i]);
			}
				 
		//	}
		
		}
	
	}


	if (AbuseMail[0] != 0)
	{
		strcpy(AStr,AbuseMail);
		bSpamCop = true;
	
	}
	else if (OtherMail[0] != 0)
	{
	    strcpy(AStr,OtherMail);	
		bSpamCop = true;
	
	}
	else
	{
		AStr[0] = 0;
		bSpamCop = false;
	
	}

}

char* CSpamContact::GetSpamCop()
{

	CHTTPclient http;

	string sendstr;
	sendstr = "/sc?track=";
	sendstr = sendstr + InetNumS;
	
 

    HANDLE hand = http.IPGET("220.130.117.23","www.spamcop.net",(char *) sendstr.c_str(),60 * 1000, 60 * 1000);  

    DWORD rtn = WaitForSingleObject(hand,60 * 1000);
	if  (rtn == WAIT_OBJECT_0)
	{
	
		FILE *fp;
		fp = fopen(http.GetTempFilePath(),"r+b");
	 

		fseek(fp,0,SEEK_END);
		int filesize = ftell(fp);

		fseek(fp,0,SEEK_SET);

		char *buff = new char[filesize+1];
		memset(buff,0,filesize+1);

		fread(buff,1,filesize,fp);



		//_cprintf("%s",buff);

		fclose(fp);

		TrimResponse(buff);
		strcpy(contact,buff);


		delete buff;
	
	}

	DeleteFile(http.GetTempFilePath());
	
	
	return contact;
}





//-------------------------------------------------------------------------------

CChecker::CChecker(CRITICAL_SECTION*  DBSection, char* DBFileName , char *IniFileName )
{
	 //WSADATA wsaData;
     //WSAStartup(MAKEWORD(2,2), &wsaData);
	 //OutputDebugString("tt");

	//SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);

	//throw "test";
	mDBSection = DBSection;

	m_subject = NULL;
	m_sender = NULL;
	m_receiveIP = NULL;

	
 


	CCoreSocket::Startup();

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];
	 //char IniPath[_MAX_PATH];


	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );

	   char IndexDbPath[_MAX_PATH];
	   char overflowDbPath[_MAX_PATH];
	   char datadbPath[_MAX_PATH];

	   strcpy(IniPath,drive);
	   strcat(IniPath,dir);	   
	   strcat(IniPath,IniFileName);

	   

	   strcpy(IndexDbPath,drive);
	   strcat(IndexDbPath,dir);

	   strcpy(DbSavePath,IndexDbPath);
	   
	   
	   strcpy(overflowDbPath,IndexDbPath);
	   strcpy(datadbPath,IndexDbPath);

	   strcat(IndexDbPath,"DB//");
	   strcat(IndexDbPath,DBFileName);
	   strcat(IndexDbPath,"1.db");
		   
	   strcat(overflowDbPath,"DB//");
	   strcat(overflowDbPath,DBFileName);
	   strcat(overflowDbPath,"2.db");

	   strcat(datadbPath,"DB//");
	   strcat(datadbPath,DBFileName);
	   strcat(datadbPath,"3.db");

	   
	 db = new CSPAMDB(mDBSection,IndexDbPath,overflowDbPath,datadbPath,1024 * 10);
	 db->OpenDB();


		/*char RangeDBFile[_MAX_PATH];
	    char RangeDBData[_MAX_PATH];

		strcpy(RangeDBFile,drive);
		strcat(RangeDBFile,dir);	 	

		strcpy(RangeDBData,drive);
		strcat(RangeDBData,dir);

		strcat(RangeDBFile,"DB//Contact.db");
		strcat(RangeDBData,"DB//ContactData.db");

	 ContactDB = new CTreeRangeDB(RangeDBFile,RangeDBData);*/

	// Rbl1[0] = 0;
	// Rbl2[0] = 0;

	 //GetPrivateProfileString("SysSetup","DnsIp","168.95.1.1",DnsIp,sizeof(DnsIp),IniPath); 
	 //GetPrivateProfileString("SysSetup","RBL1","",Rbl1,sizeof(Rbl1),IniPath); 
	 //GetPrivateProfileString("SysSetup","RBL2","",Rbl2,sizeof(Rbl2),IniPath); 

	 //memset(&mfileheader,0,sizeof(DataFileHeader));
	
	 /*char dmdbindexPath[_MAX_PATH];
	 char dmdboverPath[_MAX_PATH];
	 char dmdbdataPath[_MAX_PATH];

	 strcpy(dmdbindexPath,DbSavePath);
	 strcat(dmdbindexPath,"dmdb1.db");

	 strcpy(dmdboverPath,DbSavePath);
	 strcat(dmdboverPath,"dmdb2.db");

	 strcpy(dmdbdataPath,DbSavePath);
	 strcat(dmdbdataPath,"dmdb3.db");

	 dmDB = new CDB2(dmdbindexPath,dmdboverPath,dmdbdataPath,0,1024 * 10);*/

	 /*char ipdbindexPath[_MAX_PATH];
	 char ipdboverPath[_MAX_PATH];
	 char ipdbdataPath[_MAX_PATH];

	 strcpy(ipdbindexPath,DbSavePath);
	 strcat(ipdbindexPath,"DB//ipdb1.db");

	 strcpy(ipdboverPath,DbSavePath);
	 strcat(ipdboverPath,"DB//ipdb2.db");

	 strcpy(ipdbdataPath,DbSavePath);
	 strcat(ipdbdataPath,"DB//ipdb3.db");

	 
	 IpDB = new CDB(ipdbindexPath,ipdboverPath,ipdbdataPath,0,1024 * 10);*/

	 KeyCount = 0;

}

CChecker::~CChecker()
{
     //WSACleanup();

	CCoreSocket::Cleanup();

	db->CloseDB();
	delete db;
	 //delete ContactDB;
	 //delete dmDB;
	 //delete IpDB;
}

string CChecker::lowercase(string Astr)
{
   std::string ret_str = "";
   int Size = Astr.size();

   for (unsigned int index = 0; index < Size; index++)
     ret_str += tolower(Astr[index]);

    return ret_str;

}

bool CChecker::bPassKey(char * KeyStr ,int startpos  )
{
 
	bool brtn = false;
 
	 
    //長度要符合
	if (startpos == 3 && _strnicmp(KeyStr - startpos, "gif",3) == 0 )
	{
		brtn = true;
	} else if (startpos == 4 && _strnicmp(KeyStr - startpos, "aspx",4) == 0)
	{
	    brtn = true;
	} else if (startpos == 5 && _strnicmp(KeyStr - startpos, "shtml",5) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos , "jpg",3) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos , "htm",3) == 0)
	{
	    brtn = true;
	} else if (startpos == 4 && _strnicmp(KeyStr - startpos , "html",4) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos, "css",3) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos, "com",3) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos, "www",3) == 0)
	{
	    brtn = true;
	} else if (startpos == 2 && _strnicmp(KeyStr - startpos, "w3",2) == 0)
	{
	    brtn = true;
	} else if (startpos == 4 && _strnicmp(KeyStr - startpos, "info",4) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos, "org",3) == 0)
	{ 
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos, "php",3) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos, "net",3) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos, "asp",3) == 0)
	{
	    brtn = true;
	} else if (startpos == 3 && _strnicmp(KeyStr - startpos, "idv",3) == 0)
	{
	    brtn = true;
	}else if (startpos == 2 && _strnicmp(KeyStr - startpos, "tw",2) == 0)
	{
	    brtn = true;
	} else if (startpos == 5 && _strnicmp(KeyStr - startpos, "image",5) == 0)
	{
	    brtn = true;
	}  else if (startpos == 6 && _strnicmp(KeyStr - startpos, "images",6) == 0)
	{
	    brtn = true;
	} else if (startpos == 7 && _strnicmp(KeyStr - startpos, "cgi-bin",7) == 0)
	{
	    brtn = true;
	}else if (startpos == 5 && _strnicmp(KeyStr - startpos, "hinet",5) == 0)
	{
	    brtn = true;
	}


	return brtn;

}

bool CChecker::bPassKey2(char * KeyStr )
{
 
	bool brtn = false;

 

	if (_strnicmp(KeyStr - 3, "gif",3) == 0)
	{
		brtn = true;
	} else if (_strnicmp(KeyStr - 4, "aspx",4) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 5, "shtml",5) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3 , "jpg",3) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3 , "htm",3) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 4 , "html",4) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3, "css",3) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3, "com",3) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3, "www",3) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 2, "w3",2) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 4, "info",4) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3, "org",3) == 0)
	{ 
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3, "php",3) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3, "net",3) == 0)
	{
	    brtn = true;
	} else if (_strnicmp(KeyStr - 3, "asp",3) == 0)
	{
	    brtn = true;
	}  else if (_strnicmp(KeyStr - 2, "tw",2) == 0)
	{
	    brtn = true;
	} 


	return brtn;

}

int CChecker::ParseKey(char *Source , StrList *KeyList , char* CutStr)
{
	
	if (Source == NULL) return 0;

	int CutStrLen = strlen(CutStr);

	int  save2 = -1 ,save3 = 0;
	int i = 0;
	int StrLen = strlen(Source);
	while( i <= StrLen )
	{

		if (save2 == -1)
		{
			
			if (strncmp(Source + i , CutStr,CutStrLen) == 0)			
			{
				
				save2 = i + CutStrLen;
				i += CutStrLen;				
				save3 = 0;		 
				
			}
			
		}
		else
		{
		
			 
			if (*(Source + i) == '/')
			{
				save3 = i;
			
			}else
			//等待字尾 , 使用次數多可以改用 map table
			if (*(Source + i) == ' ' ||
				*(Source + i) == '"'  ||
				*(Source + i) == '>' ||
				*(Source + i) == '<' ||
				*(Source + i) == '\'' ||
				*(Source + i) == '?' ||
				*(Source + i) == 13 ||
				(*(Source + i) & 0xFF) > 128 ||
				i   == StrLen
				)            
			{
			
				bool breaktag = false;
				int tmpi = 0;
				if (save3 > 0)
				{
				  tmpi = save3 + 1; // / 保留
				  breaktag = true;
				}
				else
				{
				  tmpi = i;
				}
				//if (i - save2 > 2 && !bPassKey2(Source + i  ))
				if (tmpi - save2 > 2)
				{
					
					std::string mstr;
					mstr.append(save2 + Source, tmpi - save2 );

					if (!breaktag)
						mstr.append("//",1);
				
					mstr = lowercase(mstr);
					KeyList->insert(KeyList->end(),mstr);
					
				}

				 
				 
				save2 = -1;
			}


		
		}


	 i++;
	}


	return KeyList->size();
}


int CChecker::ParseNumKey(char *Source , StrList *KeyList )
{
 
	if (Source == NULL) return 0;

	int  save2 = -1 ,save3 = 0;
	int i = 0;
	int StrLen = strlen(Source);
	char NumBuff[50];
	int NumCount = 0;
	memset(NumBuff,0,50);

	//此方法全型數字不試用 //限制數量 20 

	while( i <= StrLen && KeyList->size() <= 20)
	{

		if ((Source[i] & 0xFF) <= 128 )
		{
		if (save2 == -1)
		{
			
			//if (strncmp(Source + i , CutStr,CutStrLen) == 0)			
			if (isdigit(*(Source + i)) )
			{				
				save2 = i ;
				NumBuff[NumCount] = *(Source + i);
				NumCount++;
				//i += CutStrLen;				
				save3 = 0;		 				
			}
			
		}
		else
		{
		
			 
			if (isdigit(*(Source + i)) && NumCount < 20)
			{
			
				NumBuff[NumCount] = *(Source + i);
				NumCount++;
			
			} else
			//等待字尾 , 使用次數多可以改用 map table
			if (*(Source + i) == ' ' ||			
				*(Source + i) == 13 ||
				i   == StrLen ||
				NumCount >= 20
				)            
			{
			
				 
				if (NumCount >= 4 && NumCount <= 20)
				{
					
					//std::string mstr;
					//mstr.append(save2 + Source, tmpi - save2 );
				
					//mstr = lowercase(mstr);
					NumBuff[NumCount]  = 0;					
					KeyList->insert(KeyList->end(),NumBuff);
					
				}

				NumCount = 0;				 
				save2 = -1;
			}


		
		}
		}


	 i++;
	}


	return KeyList->size();
}


void  CChecker::GetCheckKeys(char* Source , char* BodySource, char * SenderName , char *Subject,char *ReceiveIP,bool SpamSubject)
{

	
	
	//Insert 要處理大小寫

    m_subject = Subject;
	m_sender = SenderName;
	m_receiveIP = ReceiveIP;

	int  save = -1 , save2 = -1 ,save3 = 0, saveDomain=-1;
	int i = 0;
	bool bGetDomain = true;
	bool bGetDot = true; //點只解到第一個 /
	int levelcount = 0;


	ResList.clear();
	DomainList.clear();

	//using namespace std ;
	//typedef list<string> StrList;
 
	//StrList mstrlist;
    //StrList Domainlist;
	StrList tmplist;
	//ResList.clear();
	//Domainlist.clear();
	tmplist.clear();

	std::string DomainStr;

	
	//先以 :// 解開
	if (ParseKey(Source,&tmplist,"://") == 0)
	{
		//如果空白在解 www.
		if (ParseKey(Source,&tmplist,"www.") == 0)
		{
			//如果還空白用純文字內容
			if(BodySource != NULL)
			{
				//先以 :// 解開
				if(ParseKey(BodySource,&tmplist,"://") == 0)
				{
					//如果空白在解 www.
					if(ParseKey(BodySource,&tmplist,"www.") == 0)
					{
						//拿出殺手賤
						ParseNumKey(BodySource,&tmplist);
						
					
					}
				}
			}
		
		}
	}



	if(BodySource != NULL)
	{
		GetGramKeys(BodySource);
	}
	

	tmplist.sort();
	tmplist.unique();

	CMailCodec MailCodec;

	StrList::iterator itor; 
	StrList::iterator enditor = tmplist.end();


	for (itor =  tmplist.begin(); itor != enditor ; itor++)
	{
		std::string tmpstdstr  = *itor;

		//Decode Unicode
		char *DecodeStr = MailCodec.UniDecode((char *) tmpstdstr.c_str());
		tmpstdstr = DecodeStr;
		MailCodec.FreeUniDecode(DecodeStr);

		//Decode QP % style
		DecodeStr = MailCodec.QPDecode((char *) tmpstdstr.c_str(),"%");
		tmpstdstr = DecodeStr;
		MailCodec.FreeQPDecode(DecodeStr);


		i = 0;
		bGetDot = true;
		save2 = 0;
		DomainStr = "";
		saveDomain = 0; //用途在找出 domain str
		int Size = tmpstdstr.size();

		while( i <= Size )
		{
			
			if (tmpstdstr[i] == '.' && bGetDot)
			{	 
				    std::string mstr;
					//if (i - save2 >= 2)
					if (i - save2 >= 2  && !bPassKey((char *) tmpstdstr.c_str() + i,i - save2))
					//if (i - save2 >= 2)
					{
					 
						 
						mstr.append(save2 + tmpstdstr.c_str() , i - save2);					 
						mstr = lowercase(mstr);
						ResList.insert(ResList.end(),mstr);
						 

						//if (mstrlist.size() > 50)
						//	break;

						 
					}

				    save2 = i+1; 
  				    
			} 
			else if(tmpstdstr[i] == '/' )
			{
				bGetDot = false;			 
				if (i - save2 > 2 )
				{
					if (!bPassKey((char *) tmpstdstr.c_str() + i,i - save2))
					{
				
						std::string mstr;
						mstr.append(save2 + tmpstdstr.c_str() , i - save2);					    
						mstr = lowercase(mstr);
						ResList.insert(ResList.end(),mstr);
						 

						//	if (mstrlist.size() > 50)
					    //		break;

						 
					}
				}

				if (saveDomain > -1)
				{
					  
					  DomainStr.append(saveDomain + tmpstdstr.c_str() , i - saveDomain);
					  
					  if (SimpleCheckDomain((char *)DomainStr.c_str()))
					  {
						DomainStr = lowercase(DomainStr);
						//OutputDebugString(DomainStr.c_str());
						//OutputDebugString("\n");
						DomainList.insert(DomainList.end(),DomainStr);
					  }
					  

					  saveDomain = -1;
				}

					

			
					save2 = i+1; 
				

			}
			else  if (i == Size ) 
			{
					std::string mstr;
					if (i - save2 >= 2  && !bPassKey((char *) tmpstdstr.c_str() + i,i - save2))
					//if (i - save2 >= 2)
					{
					 
						 
						mstr.append(save2 + tmpstdstr.c_str() , i - save2);					 
						mstr = lowercase(mstr);
						ResList.insert(ResList.end(),mstr);
						 

						//if (mstrlist.size() > 50)
						//	break;

						 
					}
			
			}

		 i++;	 
		}


	}

	ResList.sort();
	ResList.unique();

	//KeyCount = ResList.size(); //存放 Gram 及 SenderName 前的數量

	//檢查連結上限

	 DomainList.sort();
	 DomainList.unique();

	if (DomainList.size() > 20)
	{
		//判斷為大量連結
		ResList.clear();
		DomainList.clear();	
		//KeyCount = 0;
	}

	//CMainFrm *mfrm = (CMainFrm *) AfxGetMainWnd();
	//if (mfrm->Settings.SpamSubject)
	if (SpamSubject)
		GetGramKeys(Subject);


	//Add SenderName
	if (SenderName != NULL)
	{
		if (strlen(SenderName) > 0)
		{
			char *PersonalName = new char[strlen(SenderName) + 1];
			char *Email = new char[strlen(SenderName) + 1];

			PersonalName[0] = 0;
			Email[0] = 0;

			CEmail email;
		    email.SplitEmailAddress(SenderName,PersonalName,Email);

			if (PersonalName[0] != 0)
			  SecResList.insert(SecResList.end(),PersonalName);

			if (Email[0] != 0)
			  SecResList.insert(SecResList.end(),Email);

			delete PersonalName;
			delete Email;


		}
	}
	
	

	 
	// ResList.sort();
	// ResList.unique();

	 SecResList.sort();
	 SecResList.unique();

	 
	 //SecondaryResList.sort();
	 //SecondaryResList.unique();

	 //DomainList.sort();
	 //DomainList.unique();


	

    
	

	//return mstrlist; 

}

void CChecker::GetSpamData()
{
  
  double RtnValue = 0;
  //DataFileList mDataFileList;

  //CSPAMDB db("c:\\index.db","c:\\overflow.db","c:\\data.db",1024 * 10);
  std::string tmpstdstr ;
  StrList::iterator itor;

  DataList.clear();
  SecDataList.clear();

  //DataFileHeader fileheader;
  //memset(&fileheader,0,sizeof(DataFileHeader));
  //取出 header
  //db.SelectData(0,(char *) &fileheader, sizeof(DataFileHeader));
  
  //mDataFileList.clear(); 

#ifdef _SPAMLOG
  
  FILE *fp=NULL;
  char LogRootPath[_MAX_PATH];
  strcpy(LogRootPath,DbSavePath);
  strcat(LogRootPath,"log//");

   
	
	  char LogPath[_MAX_PATH];	
	  char SaveLogPath[_MAX_PATH];
	  GetTempFileName(LogRootPath, // directory for temp files 
        "SF",                    // temp file name prefix 
        0,                        // create unique name 
        LogPath);              // buffer for name
	  
	  strcpy(SaveLogPath,LogPath);
	  //strcat(LogPath,".txt");
	  //rename(SaveLogPath,LogPath);
	  fp = fopen(LogPath,"r+b");
      
  

  char *bk = "\r\n";
  char sp = char(9);
  char tempchar[255];

  if (m_subject != NULL)
  {
	  fwrite(m_subject,1,strlen(m_subject),fp);
	  fwrite(bk,1,2,fp);
	  fwrite(bk,1,2,fp);
  }

#endif

  //取 Receive IP 的 data
  if (m_receiveIP != NULL && strlen(m_receiveIP) > 0) //注意順序
  {
 

	 
	 #ifdef _SPAMLOG

	  fwrite(m_receiveIP,1,strlen(m_receiveIP),fp);	  
	  fwrite(&sp,1,1,fp);

	 #endif

	 DataFileRes mdatafile;
	 memset(&m_receiveDataFile,0,sizeof(SDataFile));

	 _cprintf("%s\r\n",m_receiveIP);

	 mdatafile = db->GetSpamData(m_receiveIP);

	  if (mdatafile.SearchRes == -1)
	  {
		  #ifdef _SPAMLOG
			fwrite(bk,1,2,fp); 
		  #endif
		  
		  //如果 key 不存在則 insert 空白
		  mdatafile.datafile.Status = 2; //IP data

		  int datapos = db->InsertSpamData((char *) m_receiveIP,&mdatafile.datafile);

		  m_receiveDataFile.DataFilePos = datapos;
		  m_receiveDataFile.mDataFile = mdatafile.datafile;

		  //DataList.insert(DataList.end(),mSDataFile);
	  }
	   else
	  {

		  #ifdef _SPAMLOG
		    itoa(mdatafile.datafile.OkRate,tempchar,10);
			fwrite(tempchar,1,strlen(tempchar),fp);
			fwrite(&sp,1,1,fp);

			itoa(mdatafile.datafile.BadRate,tempchar,10);
			fwrite(tempchar,1,strlen(tempchar),fp);
			fwrite(bk,1,2,fp);
		  #endif

		  
		  m_receiveDataFile.DataFilePos = mdatafile.SearchRes;		  
		  m_receiveDataFile.mDataFile = mdatafile.datafile;
		  m_receiveDataFile.mDataFile.UsedCount ++;
		  //mSDataFile.KeyType = 1;

		  int offset = FIELD_OFFSET(DataFile,UsedCount);			  
		  db->UpdateData((char*) &(m_receiveDataFile.mDataFile.UsedCount), sizeof(m_receiveDataFile.mDataFile.UsedCount),mdatafile.SearchRes + offset,0);
		  
	  }
  
  }

  //取得 mail content 的 data
  for (itor =  ResList.begin(); itor != ResList.end() ; itor++)
  {

	  tmpstdstr = *itor;
	  _cprintf("%s\r\n",tmpstdstr.c_str());
	  
	  //OutputDebugString(tmpstdstr.c_str());
	  //OutputDebugString("\n");

	  //長度不可大於 25 , 否則要處理截斷
	  if (tmpstdstr.length() >= 24)
	  {
			tmpstdstr.erase(tmpstdstr.begin() + 23 , tmpstdstr.end());
			
	  }

	 #ifdef _SPAMLOG

	  fwrite(tmpstdstr.c_str(),1,tmpstdstr.length(),fp);	  
	  fwrite(&sp,1,1,fp);

	 #endif

#ifdef _CONSOLEDBG
	  DWORD		dwWritten;
	  HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	  if (hConsole != NULL)
	  {
		    
		  //char *tmpchar = "ReceiveIP : ";

		  //WriteConsole(hConsole,tmpchar,strlen(tmpchar),&dwWritten,0);		 	
		  WriteConsole(hConsole,tmpstdstr.c_str(),tmpstdstr.length(),&dwWritten,0);		 
		  WriteConsole(hConsole,"\r\n",2,&dwWritten,0);

		 
	  }
#endif

	  DataFileRes mdatafile;
	  SDataFile mSDataFile;	
	 
	  
	  memset(&mdatafile,0,sizeof(DataFileRes));
	  memset(&mSDataFile,0,sizeof(SDataFile));


	  
	  mdatafile = db->GetSpamData((char *) tmpstdstr.c_str());

	  if (mdatafile.SearchRes == -1)
	  {
		  #ifdef _SPAMLOG
			fwrite(bk,1,2,fp); 
		  #endif
		  
		  //如果 key 不存在則 insert 空白
		  mdatafile.datafile.Status = 1;

		  int datapos = db->InsertSpamData((char *) tmpstdstr.c_str(),&mdatafile.datafile);

		  mSDataFile.DataFilePos = datapos;
		  mSDataFile.mDataFile = mdatafile.datafile;

		  DataList.insert(DataList.end(),mSDataFile);
	  }
	  else
	  {

		  #ifdef _SPAMLOG
		    itoa(mdatafile.datafile.OkRate,tempchar,10);
			fwrite(tempchar,1,strlen(tempchar),fp);
			fwrite(&sp,1,1,fp);

			itoa(mdatafile.datafile.BadRate,tempchar,10);
			fwrite(tempchar,1,strlen(tempchar),fp);
			fwrite(bk,1,2,fp);
		  #endif

		  
		  mSDataFile.DataFilePos = mdatafile.SearchRes;		  
		  mSDataFile.mDataFile = mdatafile.datafile;
		  mSDataFile.mDataFile.UsedCount ++;

		  int offset = FIELD_OFFSET(DataFile,UsedCount);			  
		  db->UpdateData((char*) &(mSDataFile.mDataFile.UsedCount), sizeof(mSDataFile.mDataFile.UsedCount),mdatafile.SearchRes + offset,0);
		  //UpdateSpamData 還會在更新一次 		 
		  DataList.insert(DataList.end(),mSDataFile);

		  //寫入 Send Server report UsedCount 標準攸關 report key 的準確程度
		  /*
		  if (mSDataFile.mDataFile.UsedCount > 3 && mSDataFile.mDataFile.BadRate > mSDataFile.mDataFile.OkRate && mSDataFile.mDataFile.OkRate <= 0)
		  {
		  
			  	SResult2 sres = dmDB->SelectKey((char *) tmpstdstr.c_str());

				if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
				{
					//找不到資料				
					DomainUpload du;
					du.ReportType = 0; //正常 Key
					du.isUpload = false;			 
					int datapos = dmDB->InsertData((char *)&du,sizeof(DomainUpload));		
			

					dmDB->InsertKey((char *) tmpstdstr.c_str(),datapos);	   
	  
				}
				
		  } else if (mSDataFile.mDataFile.UsedCount > 3 && mSDataFile.mDataFile.OkRate > mSDataFile.mDataFile.BadRate && mSDataFile.mDataFile.BadRate <= 0)
		  {
		  
				SResult2 sres = dmDB->SelectKey((char *) tmpstdstr.c_str());

				if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
				{
					//找不到資料				
					DomainUpload du;
					du.ReportType = 2; //正常 ok Key
					du.isUpload = false;			 
					int datapos = dmDB->InsertData((char *)&du,sizeof(DomainUpload));		
			

					dmDB->InsertKey((char *) tmpstdstr.c_str(),datapos);	   
	  
				}
		  
		  }*/

		  
	 
		  
	  }
  }

  /////////////////////////Secondary
  #ifdef _SPAMLOG
	char *tmpstr2 = "\r\n---------Secondary----------\r\n";
	fwrite(tmpstr2,1,strlen(tmpstr2),fp);	
  #endif
 
   for (itor =  SecResList.begin(); itor != SecResList.end() ; itor++)
  {

	  tmpstdstr = *itor;
	  _cprintf("%s\r\n",tmpstdstr.c_str());
	  
	  //OutputDebugString(tmpstdstr.c_str());
	  //OutputDebugString("\n");

	  //長度不可大於 25 , 否則要處理截斷
	  if (tmpstdstr.length() >= 24)
	  {
			tmpstdstr.erase(tmpstdstr.begin() + 23 , tmpstdstr.end());
			
	  }


#ifdef _CONSOLEDBG
	 
	  DWORD		dwWritten;
	  HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

	  if (hConsole != NULL)
	  {
		    
		  //char *tmpchar = "ReceiveIP : ";

		  //WriteConsole(hConsole,tmpchar,strlen(tmpchar),&dwWritten,0);		 	
		  WriteConsole(hConsole,tmpstdstr.c_str(),tmpstdstr.length(),&dwWritten,0);		 
		  WriteConsole(hConsole,"\r\n",2,&dwWritten,0);

		 
	  }
#endif

	 #ifdef _SPAMLOG

	  fwrite(tmpstdstr.c_str(),1,tmpstdstr.length(),fp);	  
	  fwrite(&sp,1,1,fp);

	 #endif
	  DataFileRes mdatafile;
	  SDataFile mSDataFile;	
	 
	  
	  memset(&mdatafile,0,sizeof(DataFileRes));
	  memset(&mSDataFile,0,sizeof(SDataFile));


	  
	  mdatafile = db->GetSpamData((char *) tmpstdstr.c_str());

	  if (mdatafile.SearchRes == -1)
	  {
		  #ifdef _SPAMLOG
			fwrite(bk,1,2,fp); 
		  #endif
		  
		  //如果 key 不存在則 insert 空白
		  mdatafile.datafile.Status = 1;

		  int datapos = db->InsertSpamData((char *) tmpstdstr.c_str(),&mdatafile.datafile);

		  mSDataFile.DataFilePos = datapos;
		  mSDataFile.mDataFile = mdatafile.datafile;
		  //mSDataFile.KeyType = 1; //Secondary

		  SecDataList.insert(SecDataList.end(),mSDataFile);
	  }
	  else
	  {

		  #ifdef _SPAMLOG
		    itoa(mdatafile.datafile.OkRate,tempchar,10);
			fwrite(tempchar,1,strlen(tempchar),fp);
			fwrite(&sp,1,1,fp);

			itoa(mdatafile.datafile.BadRate,tempchar,10);
			fwrite(tempchar,1,strlen(tempchar),fp);
			fwrite(bk,1,2,fp);
		  #endif

		  
		  mSDataFile.DataFilePos = mdatafile.SearchRes;		  
		  mSDataFile.mDataFile = mdatafile.datafile;
		  mSDataFile.mDataFile.UsedCount ++;
		  //mSDataFile.KeyType = 1;

		  int offset = FIELD_OFFSET(DataFile,UsedCount);			  
		  db->UpdateData((char*) &(mSDataFile.mDataFile.UsedCount), sizeof(mSDataFile.mDataFile.UsedCount),mdatafile.SearchRes + offset,0);
		  //UpdateSpamData 還會在更新一次 		 
		  SecDataList.insert(SecDataList.end(),mSDataFile);
 
		  
	  }
   } 

  #ifdef _SPAMLOG
	fwrite(bk,1,2,fp); 

	 
	
	double RateRes = GetSpamRate();	



	int     decimal,   sign;
    char    *buffer;
    int     precision = 10;
     

    buffer = _ecvt( RateRes, precision, &decimal, &sign );

	 
	//_gcvt(RateRes,3,tempchar);
	itoa( decimal,tempchar,10);

	//fwrite(dotstr,1,2,fp);
	fwrite(buffer,1,strlen(buffer),fp);
	fwrite(" : ",1,3,fp);
	fwrite(tempchar,1,strlen(tempchar),fp);
	fclose(fp);

	if (RateRes == 0.5)
	{
	   strcat(LogPath,"-Unknown.txt");
	   rename(SaveLogPath,LogPath);
	}
	else if (RateRes > 0.5)
	{
	   strcat(LogPath,"-Spam.txt");
	   rename(SaveLogPath,LogPath);
	}
	else
	{
		strcat(LogPath,"-Ok.txt");
	    rename(SaveLogPath,LogPath);
	}
  #endif
  
}

double CChecker::GetSpamRate(double LimitRate)
{
	DataFileList::iterator itor;
	SDataFile tmpDataFile ;
	int indentcount = 0;

	double SpamRate = 0.5 , tmpSpamRate = 0,TotalSpamRate = 1, TotalOkRate = 1;
    double DataOkRate = 0.0 , DataSpamRate = 0.0;
	

	//先看 receieve data
	/*
	
	  在可辨識 Receieve IP 的情形下 , 不看內容直接判斷
	  ,目前未處理 IP 誤判的情形 

	*/
	if (m_receiveIP != NULL  && strlen(m_receiveIP) > 0)
	{
		DataOkRate = m_receiveDataFile.mDataFile.OkRate;
		DataSpamRate = m_receiveDataFile.mDataFile.BadRate;

		

		if (DataOkRate < 0 ) DataOkRate = 0;
		if (DataSpamRate < 0) DataSpamRate = 0;	

		if (DataOkRate != 0 || DataSpamRate != 0)
		{
			

			if (DataSpamRate > 0 && DataOkRate <= 0 && m_receiveDataFile.mDataFile.SpamCount >= 1)
			{
				SpamRate = 2;
			}
			else
			{
				SpamRate = DataSpamRate / (DataOkRate + DataSpamRate);
				SpamRate = min(SpamRate,0.99);
				SpamRate = max(SpamRate,0.01);
			
			}

			/*
			if (SpamRate > 0.5)
			{
				if (AllByStr2.size() > 0)
				{
					char path_buffer[_MAX_PATH];
					char drive[_MAX_DRIVE];
					char dir[_MAX_DIR];
					char fname[_MAX_FNAME];
					char ext[_MAX_EXT];
	   
					HINSTANCE hInstance = GetModuleHandle(NULL);
					GetModuleFileName(hInstance, path_buffer, MAX_PATH);

					_splitpath( path_buffer, drive, dir, fname, ext );

					char FileLogPath[_MAX_PATH];   

					strcpy(FileLogPath,drive);
					strcat(FileLogPath,dir);				
					strcat(FileLogPath,"MailPathLog.txt"); //Log Path

					
					FILE *fp2=NULL;
					fp2 = fopen(FileLogPath,"r+b");
					if (fp2 != NULL)
					{	
						AllByStr2 = AllByStr2 + "\r\n";
						fseek(fp2,0,SEEK_END);
						fwrite(AllByStr2.c_str(),1,AllByStr2.size(),fp2);
						fclose(fp2);	
					}
				}
			
			}*/
			//可辨識則直接回傳
			return SpamRate;

		}

	}
  
	
	//fwrite(&m_Bucket->IndexData[sres.FreePos],sizeof(char),sizeof(m_Bucket->IndexData[sres.FreePos]),IndexDBfp);		 
 
	for (itor =  DataList.begin(); itor != DataList.end() ; itor++)
	{
	
		tmpDataFile = *itor;
		//tmpSpamRate = ((double)tmpDataFile.mDataFile.BadRate / (double)mfileheader.TotalBad);
		//tmpSpamRate = tmpSpamRate / tmpSpamRate + ((double) tmpDataFile.mDataFile.OkRate / (double)mfileheader.TotalOk);

		 
		if (tmpDataFile.mDataFile.BadRate != 0 || tmpDataFile.mDataFile.OkRate != 0)
		{
		  indentcount++;
		} 

		DataOkRate = tmpDataFile.mDataFile.OkRate;
		DataSpamRate = tmpDataFile.mDataFile.BadRate;

		if (DataOkRate < 0 ) DataOkRate = 0;
		if (DataSpamRate < 0) DataSpamRate = 0;		

		if (DataOkRate == 0 && DataSpamRate == 0)
		{
				tmpSpamRate = 0.5;		
		}
		else
		{
			double KeyTotalRate = DataOkRate + DataSpamRate;

			tmpSpamRate = min(((double)DataSpamRate / KeyTotalRate),1);
			tmpSpamRate = tmpSpamRate / (tmpSpamRate + min(((double) DataOkRate / (double)KeyTotalRate),1));
			tmpSpamRate = min(tmpSpamRate,0.99);
			tmpSpamRate = max(tmpSpamRate,0.01);
		}		

		//if (tmpDataFile.mDataFile.BadRate != 0 || tmpDataFile.mDataFile.OkRate != 0)
		//{
		 TotalSpamRate = TotalSpamRate * tmpSpamRate;
		 //TotalOkRate = TotalOkRate * (1-TotalSpamRate);		
		 TotalOkRate = TotalOkRate * (1-tmpSpamRate);		
		//}
	
	}


	 
	SpamRate = TotalSpamRate / (TotalSpamRate + TotalOkRate);
	 

	//無法辨識不可超過 設定
	//if (indentcount  < KeyCount * LimitRate)
	if (indentcount  < ResList.size() * LimitRate || SpamRate == 0.5 )
	{
		//SpamRate = 0.5;
		//使用 SecData
		indentcount = 0;

	    SpamRate = 0.5;
		tmpSpamRate = 0;
		TotalSpamRate = 1;
		TotalOkRate = 1;
        DataOkRate = 0.0 ;
		DataSpamRate = 0.0;
	
	 
		for (itor =  SecDataList.begin(); itor != SecDataList.end() ; itor++)
		{
	
			tmpDataFile = *itor; 
		 
			if (tmpDataFile.mDataFile.BadRate != 0 || tmpDataFile.mDataFile.OkRate != 0)
			{
				indentcount++;
			} 

			DataOkRate = tmpDataFile.mDataFile.OkRate;
			DataSpamRate = tmpDataFile.mDataFile.BadRate;

			if (DataOkRate < 0 ) DataOkRate = 0;
			if (DataSpamRate < 0) DataSpamRate = 0;		

			if (DataOkRate == 0 && DataSpamRate == 0)
			{
				tmpSpamRate = 0.5;		
			}
			else
			{
			
				double KeyTotalRate = DataOkRate + DataSpamRate;

				tmpSpamRate = min(((double)DataSpamRate / KeyTotalRate),1);
				tmpSpamRate = tmpSpamRate / (tmpSpamRate + min(((double) DataOkRate / (double)KeyTotalRate),1));
				tmpSpamRate = min(tmpSpamRate,0.99);
				tmpSpamRate = max(tmpSpamRate,0.01);
			}		

	 
		 TotalSpamRate = TotalSpamRate * tmpSpamRate;		 		
		 TotalOkRate = TotalOkRate * (1-tmpSpamRate);		
		 
	
		}
	 
		SpamRate = TotalSpamRate / (TotalSpamRate + TotalOkRate);

		//取消 Secondary 的 limitRate 限制
		/*if (indentcount  < SecResList.size() * LimitRate  )
		{
			SpamRate = 0.5;
		}*/

		//最後  limit rate 檢查
		if (indentcount  < (DataList.size() + SecResList.size()) * LimitRate  )
		{
			SpamRate = 0.5;
		}

	}


			/*if (SpamRate > 0.5)
			{
				if (AllByStr2.size() > 0)
				{
					char path_buffer[_MAX_PATH];
					char drive[_MAX_DRIVE];
					char dir[_MAX_DIR];
					char fname[_MAX_FNAME];
					char ext[_MAX_EXT];
	   
					HINSTANCE hInstance = GetModuleHandle(NULL);
					GetModuleFileName(hInstance, path_buffer, MAX_PATH);

					_splitpath( path_buffer, drive, dir, fname, ext );

					char FileLogPath[_MAX_PATH];   

					strcpy(FileLogPath,drive);
					strcat(FileLogPath,dir);				
					strcat(FileLogPath,"MailPathLog.txt"); //Log Path

					
					FILE *fp2=NULL;
					fp2 = fopen(FileLogPath,"r+b");
					if (fp2 != NULL)
					{	
						AllByStr2 = AllByStr2 + "\r\n";
						fseek(fp2,0,SEEK_END);
						fwrite(AllByStr2.c_str(),1,AllByStr2.size(),fp2);
						fclose(fp2);	
					}
				}
			
			}*/


	
	return SpamRate;
}

void CChecker::UpdateSpamRate(int OKFix, int BadFix,bool AutoUpdate )
{
	DataFileList::iterator itor;
	SDataFile tmpDataFile ;
	//bool AutoUpdate = false;

	//if ((OKFix > 0 && BadFix == 0) || (OKFix == 0 && BadFix > 0))
	//{
	//	AutoUpdate = true;
	//}


	if (BadFix > OKFix)
	{
		m_receiveDataFile.mDataFile.SpamCount ++;
	}
	else if(BadFix < 0 || OKFix > 0) //學習為正常或正常
	{
		m_receiveDataFile.mDataFile.SpamCount = 0;
	}



	//更新 Receive IP
	if (m_receiveIP != NULL  && strlen(m_receiveIP) > 0)
	{

		if (AutoUpdate)
	   {
		   if (m_receiveDataFile.mDataFile.BadRate <= 0 && BadFix > 0)
		   {
				m_receiveDataFile.mDataFile.BadRate = m_receiveDataFile.mDataFile.BadRate + BadFix;
		   }
		   else if (m_receiveDataFile.mDataFile.OkRate <= 0 && OKFix > 0)
		   {
			    m_receiveDataFile.mDataFile.OkRate = m_receiveDataFile.mDataFile.OkRate + OKFix;		   
		   }
	   }
	   else
	   {
			m_receiveDataFile.mDataFile.BadRate = m_receiveDataFile.mDataFile.BadRate + BadFix;
			m_receiveDataFile.mDataFile.OkRate = m_receiveDataFile.mDataFile.OkRate + OKFix;
	   }

	   
	   //m_receiveDataFile.mDataFile.Status = tmpDataFile.mDataFile.Status;


	   db->UpdateData((char*) &m_receiveDataFile.mDataFile , sizeof(m_receiveDataFile.mDataFile),m_receiveDataFile.DataFilePos,0);

	}

	for (itor =  DataList.begin(); itor != DataList.end() ; itor++)
	{
	   tmpDataFile = *itor;	  

	   if (AutoUpdate)
	   {
		   if (tmpDataFile.mDataFile.BadRate <= 0 && BadFix > 0)
		   {
				tmpDataFile.mDataFile.BadRate = tmpDataFile.mDataFile.BadRate + BadFix;
		   }
		   else if (tmpDataFile.mDataFile.OkRate <= 0 && OKFix > 0)
		   {
			    tmpDataFile.mDataFile.OkRate = tmpDataFile.mDataFile.OkRate + OKFix;		   
		   }
	   }
	   else
	   {
			tmpDataFile.mDataFile.BadRate = tmpDataFile.mDataFile.BadRate + BadFix;
			tmpDataFile.mDataFile.OkRate = tmpDataFile.mDataFile.OkRate + OKFix;
	   }

	   
	   //tmpDataFile.mDataFile.Status = tmpDataFile.mDataFile.Status;


	   db->UpdateData((char*) &tmpDataFile.mDataFile , sizeof(tmpDataFile.mDataFile),tmpDataFile.DataFilePos,0);
    //UpdateData((char *) tmpDataFile,sizeof(tmpDataFile),FIELD_OFFSET(DataFile,Status));

	
	}

	for (itor =  SecDataList.begin(); itor != SecDataList.end() ; itor++)
	{
	   tmpDataFile = *itor;	  

	   if (AutoUpdate)
	   {
		   if (tmpDataFile.mDataFile.BadRate <= 0 && BadFix > 0)
		   {
				tmpDataFile.mDataFile.BadRate = tmpDataFile.mDataFile.BadRate + BadFix;
		   }
		   else if (tmpDataFile.mDataFile.OkRate <= 0 && OKFix > 0)
		   {
			    tmpDataFile.mDataFile.OkRate = tmpDataFile.mDataFile.OkRate + OKFix;		   
		   }
	   }
	   else
	   {
			tmpDataFile.mDataFile.BadRate = tmpDataFile.mDataFile.BadRate + BadFix;
			tmpDataFile.mDataFile.OkRate = tmpDataFile.mDataFile.OkRate + OKFix;
	   }

	   
	   //tmpDataFile.mDataFile.Status = tmpDataFile.mDataFile.Status;


	   db->UpdateData((char*) &tmpDataFile.mDataFile , sizeof(tmpDataFile.mDataFile),tmpDataFile.DataFilePos,0);
    //UpdateData((char *) tmpDataFile,sizeof(tmpDataFile),FIELD_OFFSET(DataFile,Status));

	
	}


}
void CChecker::ResolveIP(char *IP , char *Result)
{

	char DnsIp[16];		    
	GetPrivateProfileString("SysSetup","DnsIp","168.95.1.1",DnsIp,sizeof(DnsIp),IniPath); 
	
	Result[0] = 0;

				CDNSclient dns;
				HANDLE ch = dns.Resolve(DnsIp,(char *)IP,qtPTR);	 
				WaitForSingleObject(ch,INFINITE);	
					strcpy(Result,dns.GetQueryResult()); 

 

}


bool CChecker::CheckRBL(char* IP)
{

    char DnsIp[16];		    
	GetPrivateProfileString("SysSetup","DnsIp","168.95.1.1",DnsIp,sizeof(DnsIp),IniPath); 

	char Rbl1[255];
	char Rbl2[255];

	Rbl1[0] = 0;
	Rbl2[0] = 0;

	GetPrivateProfileString("SysSetup","RBL1","rbl.softworking.com",Rbl1,sizeof(Rbl1),IniPath); 
	GetPrivateProfileString("SysSetup","RBL2","sbl-xbl.spamhaus.org",Rbl2,sizeof(Rbl2),IniPath); 
	
	//ip 反轉
	    char DomainStr[255];
		char Check1[255];
		char Check2[255];
		int len = strlen(IP);
		char Zone1[4];
		char Zone2[4];
		char Zone3[4];
		char Zone4[4];

		memset(Zone1,0,4);
		memset(Zone2,0,4);
		memset(Zone3,0,4);
		memset(Zone4,0,4);

		int ZoneIdx = 0;
		int SavePos = 0;

		for(int i= 0 ; i < len ; i++)
		{
			if (IP[i] == '.')
			{
			
				if (ZoneIdx == 0)
				{					
					if (i - SavePos > 0)
					strncpy(Zone4,IP + SavePos, i - SavePos);
					//ZoneIdx ++;
				}
				else if (ZoneIdx == 1)
				{
					if (i - SavePos > 0)
					strncpy(Zone3,IP + SavePos, i - SavePos);
					//ZoneIdx ++;
				}
				else if (ZoneIdx == 2)
				{
					if (i - SavePos > 0)
					strncpy(Zone2,IP + SavePos, i - SavePos);

					if (len-i > 0)
					strncpy(Zone1,IP + i + 1, len-i);
					 
					break;
				}
			 

				ZoneIdx ++;
				SavePos = i + 1;

			}
		}

		strcpy(DomainStr,Zone1);
		strcat(DomainStr,".");
		strcat(DomainStr,Zone2);
		strcat(DomainStr,".");
		strcat(DomainStr,Zone3);
		strcat(DomainStr,".");
		strcat(DomainStr,Zone4);
		strcpy(Check1,DomainStr);		
		strcpy(Check2,DomainStr);

		//strcat(Check1,".rbl.softworking.com");
		//strcat(Check2,".sbl-xbl.spamhaus.org");
		
		strcat(Check1,".");
		strcat(Check2,".");
		strcat(Check1,Rbl1);
		strcat(Check2,Rbl2);
		

		//check softworking
		char Result[255];	 
		Result[0] = 0;

		if (Rbl1[0] != 0)
		{
		
			

			CDNSclient dns;
			HANDLE ch = dns.Resolve(DnsIp,(char *)Check1,qtA);	 
			WaitForSingleObject(ch,INFINITE);	
				strcpy(Result,dns.GetQueryResult()); 

			if (strlen(Result) == 0 && Rbl2[0] != 0)
			{
				//Sleep(1000); //wait one second then retry
				//retry once
				CDNSclient dns;
				HANDLE ch = dns.Resolve(DnsIp,(char *)Check2,qtA);	 
				WaitForSingleObject(ch,INFINITE);	
					strcpy(Result,dns.GetQueryResult()); 
			
			}

		}

        if (strlen(Result) == 0)
		{			
			return false;			
		}
		else
		{
			return true;
		}



}

HRESULT CChecker::ResolveDomains()
{
	char DnsIp[16];		    
	GetPrivateProfileString("SysSetup","DnsIp","168.95.1.1",DnsIp,sizeof(DnsIp),IniPath); 

	StrList::iterator itor; 
 
	std::string tmpstdstr ;
	//char host_address[256];

	HRESULT rc = S_OK;
	 
	//數量過多
    if (DomainList.size() > 20 ) return S_FALSE;
	
	for (itor =  DomainList.begin(); itor != DomainList.end() ; itor++)
	{
         	
		        tmpstdstr = *itor; 
				
				char QueryResult[255];
				QueryResult[0] = 0;
				
 
				/*CDNSclient *dns = new CDNSclient();			 	
				HANDLE ch = dns->Resolve("168.95.1.1",(char *)tmpstdstr.c_str(),qtA);	 
				WaitForSingleObject(ch,INFINITE);	
					strcpy(QueryResult,dns->GetQueryResult()); 
				delete dns;*/


				//Beep(1000,500);
				CDNSclient dns;
				HANDLE ch = dns.Resolve(DnsIp,(char *)tmpstdstr.c_str(),qtA);	 
				WaitForSingleObject(ch,INFINITE);	
					strcpy(QueryResult,dns.GetQueryResult()); 



				if (QueryResult[0] != 0)
				{				
					//Beep(3000,100);
					DomainResList.insert(DomainResList.end(),QueryResult);
				}
				else
				{
					//Beep(2000,1000);
					//OutputDebugString(tmpstdstr.c_str());
					//OutputDebugString("\n");

				}

			
			
	}

	DomainResList.sort();
	DomainResList.unique();

	return rc;
	 

}

void CChecker::ReportSysMail(char* MsgID , char *MailContent )
{

	if (MsgID == NULL || MsgID[0] == 0 ||
		 MailContent == NULL || MailContent[0] ==0) return;
	//建立 Mail File

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];


	   //char szFileName[MAX_PATH];
       HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );   
				
	
				char MailPath[_MAX_PATH];	  

				strcpy(MailPath,drive);
				strcat(MailPath,dir);
				strcat(MailPath,"Report//");

				//char MsgIdStr[255];
				//itoa(MsgID,MsgIdStr,10);
				strcat(MailPath,MsgID);
				strcat(MailPath,".eml");

				FILE *fp=NULL;
				fp = fopen(MailPath , "w+b");
		
				ReportMailData rpmail;

				memset(&rpmail,0,sizeof(ReportMailData));

				rpmail.Type = 1;

				//strcpy(rpmail.MailTime , MailTime);			 
				//strcpy(rpmail.Subject , m_subject);
				//strcpy(rpmail.IP , m_receiveIP);		
		
				fwrite(&rpmail,sizeof(char),sizeof(ReportMailData) , fp);
				fwrite(MailContent , sizeof(char) , strlen(MailContent),fp);


				fclose(fp);

}
 


void CChecker::ReportMail(char RpType  ,char* MsgID ,  char *SenderName , char *MailContent )
{

	//RpType = 1->Spam , 2->OK , 3->Spam-OK , 4 Ok->Spam 

	//檢查 ip 不能為 local lan

	 


		if (MsgID == NULL ||  MailContent == NULL || SenderName == NULL
			|| MsgID[0] == 0 ||  MailContent[0] == 0 || SenderName[0] == 0) return;

		/*	char ipdbindexPath[_MAX_PATH];
			char ipdboverPath[_MAX_PATH];
			char ipdbdataPath[_MAX_PATH];

			strcpy(ipdbindexPath,DbSavePath);
			strcat(ipdbindexPath,"DB//ipdb1.db");

			strcpy(ipdboverPath,DbSavePath);
			strcat(ipdboverPath,"DB//ipdb2.db");

			strcpy(ipdbdataPath,DbSavePath);
			strcat(ipdbdataPath,"DB//ipdb3.db");

	 
			CDB IpDB(ipdbindexPath,ipdboverPath,ipdbdataPath,0,1024 * 10);*/


			if (m_subject == NULL || m_receiveIP == NULL || strlen(m_receiveIP) > 15 ) return;

			//檢查 IP 的合法性 
			int DotCount = 0;
			int len = strlen(m_receiveIP);
			for (int i = 0 ; i < len ; i++)
			{
				if (m_receiveIP[i] == '.')
				{
					DotCount ++;
				}
		
			}

			if (DotCount != 3) return;

			if (strlen(m_subject) >= 254)
			{
					m_subject[254] = 0;
			}

			if (strlen(SenderName) >= 33)
			{
				SenderName[33] = 0;
			}

			/*SResult sres = IpDB.SelectKey((char *) m_receiveIP);

			if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
			{
					//找不到資料

					ReportData rpdata;
					memset(&rpdata,0,sizeof(ReportData));

					//strcpy(rpdata.MailTime , MailTime);			 
					//strcpy(rpdata.Subject , m_subject);

					if (RpType == 1 || RpType == 4)
					{
						rpdata.ReportType = 1; //spam
	
					} else if (RpType == 2 || RpType == 3)
					{
			
						rpdata.ReportType = 2; //ok
					}

			 
					int datapos = IpDB.InsertData((char *)&rpdata,sizeof(ReportData));		
			 

					IpDB.InsertKey((char *) m_receiveIP,datapos);	   
	  
			}
			else
			{  	   
				ReportData rpdata;
				IpDB.SelectData(sres.DataFilePos ,(char *)&rpdata , sizeof(rpdata));

				if (RpType != rpdata.ReportType)
				{

					if (RpType == 1 || RpType == 4)
					{
						rpdata.ReportType = 1; //spam
	
					} else if (RpType == 2 || RpType == 3)
					{
			
						rpdata.ReportType = 2; //ok
					}

					rpdata.isUpload = false;			
		
					IpDB.UpdateData((char*) &rpdata , sizeof(rpdata),sres.DataFilePos,0);
				}
			}  */    

			//if (RpType == 1 || RpType == 4)
			//{
				//建立 Mail File
				char MailPath[_MAX_PATH];	  

				strcpy(MailPath,DbSavePath);
				strcat(MailPath,"Report//");

				//char MsgIdStr[255];
				//itoa(MsgID,MsgIdStr,10);
				strcat(MailPath,MsgID);
				strcat(MailPath,".eml");

				FILE *fp=NULL;
				fp = fopen(MailPath , "w+b");
		
				ReportMailData rpmail;

				memset(&rpmail,0,sizeof(ReportMailData));

				strcpy(rpmail.Sender , SenderName);			 
				strcpy(rpmail.Subject , m_subject);
				strcpy(rpmail.IP , m_receiveIP);

				if (RpType == 3 || RpType == 2 )
					rpmail.Type = 2;		
		
				fwrite(&rpmail,sizeof(char),sizeof(ReportMailData) , fp);

				if (RpType == 1 || RpType == 4 ) //正常信只回報 subject
				fwrite(MailContent , sizeof(char) , strlen(MailContent),fp);


				fclose(fp);

			//} else if (RpType == 3 || RpType == 2 )
			//{
	
	
				//char MailPath[_MAX_PATH];	  

				//strcpy(MailPath,DbSavePath);
				//strcat(MailPath,"Report//");

			 
				//strcat(MailPath,MsgID);
				//strcat(MailPath,".eml");

				//DeleteFile(MailPath);
			//}
 
	


}
void CChecker::WriteReportLog(char ReportType)
{

/*	std::string tmpstdstr ;
    StrList::iterator itor;

  for (itor =  ResList.begin(); itor != ResList.end() ; itor++)
  {

	  tmpstdstr = *itor;
	  //長度不可大於 49 , 否則要處理截斷
		if (tmpstdstr.length() >= 49)
		{
			tmpstdstr.erase(tmpstdstr.begin() + 48 , tmpstdstr.end());			
		}

	
		SResult2 sres = dmDB->SelectKey((char *) tmpstdstr.c_str());

		if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
		{
			//找不到資料
			// mDataFile.SearchRes = -1;
			DomainUpload du;
			du.ReportType = ReportType;
			du.isUpload = false;
		 
			int datapos = dmDB->InsertData((char *)&du,sizeof(DomainUpload));		
			dmDB->InsertKey((char *) tmpstdstr.c_str(),datapos);	   
	  
		}
		else
		{   
	   
			//SelectData(sres.DataFilePos,(char *) &mDataFile.datafile , sizeof(mDataFile.datafile));   
			//mDataFile.SearchRes = sres.DataFilePos;
		}      
  }

  WriteBSLog(ReportType);
  */
}

/*

void CChecker::WriteBSLog(char ReportType)
{

  if (m_subject != NULL && m_subject[0] != 0)
  {
  
		 //長度不可大於 49 , 否則要處理截斷
		if (strlen(m_subject) >= 49)
		{
			m_subject[49] = 0;
		}
	  
		char indexPath[_MAX_PATH];
		char overPath[_MAX_PATH];
		char dataPath[_MAX_PATH];

		strcpy(indexPath,DbSavePath);
		strcat(indexPath,"bsdb1.db");

		strcpy(overPath,DbSavePath);
		strcat(overPath,"bsdb2.db");

		strcpy(dataPath,DbSavePath);
		strcat(dataPath,"bsdb3.db");

		CDB2 bsdb(indexPath,overPath,dataPath,0,1024*10);
			
	  
		if (ReportType == 0 || ReportType == 1 || ReportType ==5)//spam
		{
		
			SResult2 sres = bsdb.SelectKey(m_subject);

			if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
			{
				//找不到資料
				 
				BlockSubject bs;
				bs.ReportType = 0;
				bs.isUpload = false;

				time_t rawtime;
				time ( &rawtime );

				bs.AddTime = rawtime;
		 
				int datapos = bsdb.InsertData((char *)&bs,sizeof(BlockSubject));		
				bsdb.InsertKey(m_subject,datapos);	   
	  
			}
			else
			{   
	   
				//SelectData(sres.DataFilePos,(char *) &mDataFile.datafile , sizeof(mDataFile.datafile));   
				//mDataFile.SearchRes = sres.DataFilePos;
			} 

		} 
		else if (ReportType ==4) //SpamToOK
		{
			SResult2 sres = bsdb.SelectKey(m_subject);

			if (sres.FindPosInKey == -1 && sres.FindPosInOvr == -1)
			{
				//找不到資料			
				BlockSubject bs;
				bs.ReportType = 0;
				bs.isUpload = false;
		 
				int datapos = bsdb.InsertData((char *)&bs,sizeof(BlockSubject));		
				bsdb.InsertKey(m_subject,datapos);	   
	  
			}
			else
			{   
	   
				BlockSubject bs;
				bs.ReportType = 0;
				bs.isUpload = true;

				bsdb.UpdateData((char*) &bs , sizeof(BlockSubject),sres.DataFilePos,0);
			} 
		
		}

  }



}*/

 



void CChecker::SendReportToServer(char* ServerIP , char* DogID , HWND ProgressBar )
{
   //Send IP data to Server and get Latest Data

	 

	  

	 char DbSavePath[_MAX_PATH];

	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	   HINSTANCE hInstance = GetModuleHandle(NULL);
       GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	   _splitpath( path_buffer, drive, dir, fname, ext );   

	   strcpy(DbSavePath,drive);
	   strcat(DbSavePath,dir);


	char datapath[_MAX_PATH];
	

	strcpy(datapath,DbSavePath);
	strcat(datapath,"Report//");

	/*char rpdb1path[_MAX_PATH];
	char rpdb2path[_MAX_PATH];
	char rpdb3path[_MAX_PATH];

	strcpy(rpdb1path,DbSavePath);
	strcat(rpdb1path,"DB//ipdb1.db"); 

	strcpy(rpdb2path,DbSavePath);
	strcat(rpdb2path,"DB//ipdb2.db");

	strcpy(rpdb3path,DbSavePath);
	strcat(rpdb3path,"DB//ipdb3.db");

	 
	 FILE *fp1,*fp2,*fp3;

	 fp1 = fopen(rpdb1path,"rb");
	 fp2 = fopen(rpdb2path,"rb");
	 fp3 = fopen(rpdb3path,"r+b");

	 if (fp1 == NULL || fp2 == NULL || fp3 == NULL) return;

	 string OKSendStr;
	 string SPAMSendStr;

	 	//跳掉 index header
			fseek(fp1,sizeof(IndexHeader),SEEK_SET);
			//fseek(fp3,sizeof(DataFileHeader),SEEK_SET);
			while ( !feof( fp1 ))
			{ 
					Bucket m_Bucket;
					memset(&m_Bucket,0,sizeof(Bucket));
					int ReadSize = fread(&m_Bucket,sizeof(char),sizeof(Bucket),fp1);
					if (ReadSize == 0 ) break;

					if (m_Bucket.Status == 0)
					{					
						for (int i = 0 ; i < DefaultBucketSize ; i++)
						{
						    if(m_Bucket.IndexData[i].Status == 1)
							{
							 
								
								ReportData rpdata;
								fseek(fp3,m_Bucket.IndexData[i].DataFilePos,SEEK_SET);
								if (fread(&rpdata,sizeof(char),sizeof(ReportData),fp3) > 0)
								{
									if(!rpdata.isUpload)
									{
									    
										if (rpdata.ReportType == 1)
										{
											SPAMSendStr = SPAMSendStr + m_Bucket.IndexData[i].KeyData;								 						 
											SPAMSendStr = SPAMSendStr + char(13);
										}else if (rpdata.ReportType == 2)
										{
											OKSendStr = OKSendStr + m_Bucket.IndexData[i].KeyData;								 						 
											OKSendStr = OKSendStr + char(13);
										
										} 

										fseek(fp3,m_Bucket.IndexData[i].DataFilePos,SEEK_SET);
										rpdata.isUpload = true;
										fwrite(&rpdata,sizeof(char),sizeof(ReportData),fp3);
									}

									
								}
 
							}
						}

						if (m_Bucket.ChainData != -1)
						{
					
						//處理  chain						 
						int chain = m_Bucket.ChainData;			 
						IndexData m_IndexData;
						memset(&m_IndexData,0,sizeof(IndexData));
						while (chain != -1)
						{
							
							fseek(fp2,chain,SEEK_SET);
							fread(&m_IndexData,sizeof(char),sizeof(IndexData),fp2);			


							if (m_IndexData.Status != 2)
							{
								//讀出資料	
								ReportData rpdata;
								fseek(fp3,m_IndexData.DataFilePos,SEEK_SET);
								if (fread(&rpdata,sizeof(char),sizeof(ReportData),fp3) > 0)
								{
									if(!rpdata.isUpload)
									{
										
										if (rpdata.ReportType == 1)
										{
											SPAMSendStr = SPAMSendStr + m_IndexData.KeyData;
											SPAMSendStr = SPAMSendStr + char(13);
										}
										else if (rpdata.ReportType == 2)
										{
											OKSendStr = OKSendStr + m_IndexData.KeyData;								 						 
											OKSendStr = OKSendStr + char(13);
										
										}

										fseek(fp3,m_IndexData.DataFilePos,SEEK_SET);
										rpdata.isUpload = true;
										fwrite(&rpdata,sizeof(char),sizeof(rpdata),fp3);
									}
										 
								}
							}				 

						    chain = m_IndexData.ChainData;			
						}
					
						}
					}					 

			}
 
	 fclose(fp1);
	 fclose(fp2);
	 fclose(fp3);

	 

	 string FinalSendStr;
	 CCompress cc;
	 CMailCodec MailCodec;
	 HRESULT res;

	 

	 if (SPAMSendStr.size() > 0 )
	 {

		//壓縮字串
		char SendStrLenc[50];
	

		int SendStrLen = SPAMSendStr.size();
		itoa(SendStrLen,SendStrLenc,10);

	  

		char *strbuf = (char *) SPAMSendStr.c_str();	 
		unsigned int outsize = cc.EstimateOutBuffSize(SendStrLen);
		char *outbuff = new char [outsize];
		memset(outbuff,0,outsize);

	

		char *SendBuf = NULL;
		int SendSize = 0;
		bool bCompress = false; 

	 
		if ( (res = cc.DoCompress((unsigned char*)strbuf,SendStrLen,(unsigned char *) outbuff,&outsize)) == S_OK)
		{
			SendBuf = outbuff;
			SendSize = outsize;
			bCompress  = true;
		} else if (res == NO_NEED_COMPRESS)
		{
			SendBuf = strbuf;
			SendSize = SendStrLen;
		}

		//Base64 2編碼		
	    char *EncodeStr = MailCodec.Base64Encode2(SendBuf,SendSize);
		 
		
		FinalSendStr = FinalSendStr + "orglen=" ;
		FinalSendStr = FinalSendStr + SendStrLenc;
		FinalSendStr = FinalSendStr + "&key=";
		FinalSendStr = FinalSendStr + EncodeStr;	
	
		//SendStr.ReleaseBuffer();
		MailCodec.FreeBase64Encode(EncodeStr);
		

		delete outbuff;
		

		if (bCompress) 
		{
			FinalSendStr = FinalSendStr + "&cp=1";
		}
		else
		{
			FinalSendStr = FinalSendStr + "&cp=0";
		}

	
	}


	 

	 
	 //OK Send
	 if (OKSendStr.size() > 0 )
	 {

		//壓縮字串
		char SendStrLenc[50];
	

		int SendStrLen = OKSendStr.size();
		itoa(SendStrLen,SendStrLenc,10);

	    

		char *strbuf = (char *) OKSendStr.c_str();	 
		unsigned int outsize = cc.EstimateOutBuffSize(SendStrLen);
		char *outbuff = new char [outsize];
		memset(outbuff,0,outsize);


	 

	

		char *SendBuf = NULL;
		int SendSize = 0;
		bool bCompress = false; 

	 
		if ( (res = cc.DoCompress((unsigned char*)strbuf,SendStrLen,(unsigned char *) outbuff,&outsize)) == S_OK)
		{
			SendBuf = outbuff;
			SendSize = outsize;
			bCompress  = true;
		} else if (res == NO_NEED_COMPRESS)
		{
			SendBuf = strbuf;
			SendSize = SendStrLen;
		}

		 

		//Base64 2編碼		
	    char *EncodeStr = MailCodec.Base64Encode2(SendBuf,SendSize);
		 
		if (FinalSendStr.size() > 0)
		{
		  FinalSendStr = FinalSendStr + "&orglen2=" ;
		}
		else
		{
		  FinalSendStr = FinalSendStr + "orglen2=" ;
		}

		 

		FinalSendStr = FinalSendStr + SendStrLenc;
		FinalSendStr = FinalSendStr + "&key2=";
		FinalSendStr = FinalSendStr + EncodeStr;	
	
		//SendStr.ReleaseBuffer();
		MailCodec.FreeBase64Encode(EncodeStr);
		

	 

		delete outbuff;
		

		if (bCompress) 
		{
			FinalSendStr = FinalSendStr + "&cp2=1";
		}
		else
		{
			FinalSendStr = FinalSendStr + "&cp2=0";
		}

	
	}

  

	if (FinalSendStr.size() > 0)
	{
   
		

			FinalSendStr = FinalSendStr + "&DogId=";
			FinalSendStr = FinalSendStr + DogID; 
		
		//http 送出 
			CHTTPclient http; 
 
			char *finalsendbuf = (char *) FinalSendStr.c_str();
			HANDLE hand = http.IPPOST(ServerIP,"www.softworking.com","/GetReportIp.asp",finalsendbuf,60 * 1000, 60 * 1000);
			

			DWORD rtn = WaitForSingleObject(hand,5);
			while (rtn != WAIT_OBJECT_0)
			{ 
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
			}
			
		 

			DeleteFile(http.GetTempFilePath());
			//FinalSendStr.ReleaseBuffer();
		 

	 }

	 	//刪除本次更新

		   // DeleteFile(rpdb1path);
			//DeleteFile(rpdb2path);
			//DeleteFile(rpdb3path);

			//Send All Mail
*/

	 WIN32_FIND_DATA FindFileData;
     HANDLE hFind;

	 char FindPath[_MAX_PATH];
	 strcpy(FindPath,DbSavePath);
	 strcat(FindPath,"Report//*.*"); 
	 int SendFileCount = 0;

	 //得到  total number

	 hFind = FindFirstFile(FindPath,&FindFileData);
 	 if (hFind != INVALID_HANDLE_VALUE)
	 {
	 
		 //read file
		 if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		 {
		     SendFileCount++;
		 }

		 while(FindNextFile(hFind, &FindFileData))
		 {
			
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				SendFileCount++;
			}		
		 
		 }

		 CloseHandle(hFind);
	 

	 }
	 
	 if (ProgressBar != NULL)
	 {
	 
        SendMessage(ProgressBar, PBM_SETRANGE, 0,
                MAKELPARAM(0, SendFileCount)); 

		SendMessage(ProgressBar, PBM_SETSTEP, 1,
                0);  //step = 1

	 }

	

	 hFind = FindFirstFile(FindPath,&FindFileData);
 	 if (hFind != INVALID_HANDLE_VALUE)
	 {
	 
		 //read file
		 if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		 {
		  char DBPath[_MAX_PATH];
		  strcpy(DBPath , DbSavePath);
		  strcat(DBPath,"Report//"); 
		  strcat(DBPath , FindFileData.cFileName);
		  SendMailData(ServerIP,DBPath,DogID);

			if (ProgressBar != NULL)
			{	 
				SendMessage(ProgressBar, PBM_STEPIT, 0, 0); 
			}
		 }

		 while(FindNextFile(hFind, &FindFileData))
		 {
			
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				char DBPath[_MAX_PATH];
				strcpy(DBPath , DbSavePath);
				strcat(DBPath,"Report//"); 
				strcat(DBPath , FindFileData.cFileName);
				SendMailData(ServerIP,DBPath,DogID);

				if (ProgressBar != NULL)
				{	 
					SendMessage(ProgressBar, PBM_STEPIT, 0, 0); 
				}
			}

			Sleep(1000); //延遲傳送
			
		 
		 }
	 

	 }

	 FindClose(hFind);


 
	


}

void CChecker::SendMailData(char *ServerIP , char *FilePath ,char* DogID)
{
		 //read file 

		  FILE *fp= NULL;
		  fp = fopen(FilePath , "rb");

		  if (fp != NULL)
		  {
			  fseek(fp,0,SEEK_END);

			  int filesize = ftell(fp);

			  char *buff = new char[filesize];
			  //memset(buff,0,filesize+6);

			  //strcpy(buff,"mail=");

			  fseek(fp,0,SEEK_SET);
			  fread(buff,1,filesize,fp);

			  CCompress cc;	 	 
			  unsigned int outsize = cc.EstimateOutBuffSize(filesize);
		
			  char *outbuff = new char[outsize];
			  
			  char *FinalBuff = NULL;
			  int FinalSize = 0;	
			  bool bCompress = false;

			  HRESULT res;
			  if ( (res = cc.DoCompress((unsigned char*)buff,filesize,(unsigned char *) outbuff,&outsize)) == S_OK)
			  {
					FinalBuff = outbuff;
					FinalSize = outsize;
					bCompress = true;
		 
			  } 
			  else if (res == NO_NEED_COMPRESS)
			  {
					FinalBuff = buff;
					FinalSize = filesize;
			  }

			  //Base64 2編碼
			  CMailCodec MailCodec;
			  char *EncodeStr = MailCodec.Base64Encode2(FinalBuff,FinalSize);

			  string FinalSendStr ;
			  char LenStr[255];
			  itoa(filesize,LenStr, 10);

			  FinalSendStr = "OrgLen=" ;
			  FinalSendStr = FinalSendStr + LenStr;
			  FinalSendStr = FinalSendStr + "&Mail=";
			  FinalSendStr = FinalSendStr + EncodeStr;
			  FinalSendStr = FinalSendStr + "&DogId=";
			  FinalSendStr = FinalSendStr + DogID;

			  if (bCompress)
			  {
				FinalSendStr = FinalSendStr + "&cp=1";
			  }
			  else
			  {
			    FinalSendStr = FinalSendStr + "&cp=0";
			  }


			  CHTTPclient http; 
			  HANDLE hand = 0;
			   
			  hand = http.IPPOST(ServerIP,"www.softworking.com","/SendReportMail.asp",(char *)FinalSendStr.c_str(),60 * 1000, 60 * 1000);
			   

			  MailCodec.FreeBase64Encode(EncodeStr);
			  delete outbuff;


			  

			  DWORD rtn = WaitForSingleObject(hand,5);
			  while (rtn != WAIT_OBJECT_0)
			  { 
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
			  }
			
				fclose(fp);

				DeleteFile(http.GetTempFilePath());

				if (http.ByteSend > 0 ) //基本傳送驗證
				{
					DeleteFile(FilePath);
				}

		  
			  
			  delete buff;
		  }


}

HRESULT CChecker::ParseKeyFile(CRITICAL_SECTION*  DBSection,char *FileName, HWND ProgressBar)
{
 	 char path_buffer[_MAX_PATH];
	 char drive[_MAX_DRIVE];
	 char dir[_MAX_DIR];
	 char fname[_MAX_FNAME];
	 char ext[_MAX_EXT];

	 char PgPath[_MAX_PATH];
	 char Db1Path[_MAX_PATH];
	 char Db2Path[_MAX_PATH];
	 char Db3Path[_MAX_PATH];


	  //char szFileName[MAX_PATH];
      HINSTANCE hInstance = GetModuleHandle(NULL);
      GetModuleFileName(hInstance, path_buffer, MAX_PATH);

	  _splitpath( path_buffer, drive, dir, fname, ext );
	  strcpy(PgPath,drive);
	  strcat(PgPath,dir);

	  strcpy(Db1Path,PgPath);
	  strcpy(Db2Path,PgPath);
	  strcpy(Db3Path,PgPath);

	  strcat(Db1Path,"DB//102db1.db");
	  strcat(Db2Path,"DB//102db2.db");
	  strcat(Db3Path,"DB//102db3.db");



	FILE *fp = fopen(FileName, "rb");
	if (fp != NULL)
	{
	
		int readbyte = 0;
 

		 //HMODULE SpamMod;
		 //SpamMod = LoadLibrary("Swspam.dll");
		 //SPAMDB pspamdb;

		 //pspamdb = (SPAMDB) GetProcAddress(SpamMod, "createSPAMDBObject");
		 //CSPAMDB *spdb = pspamdb();

		 CSPAMDB spdb(DBSection,Db1Path,Db2Path,Db3Path,1024 * 10);
	   

		//CSPAMDB spdb(Db1Path,Db2Path,Db3Path,1024 * 10);
		 ZipHeader zipheader;
		 fread(&zipheader,1,sizeof(zipheader),fp);

	 if (zipheader.OrgLen > 0)
	 {

		 char* DataBuffer = new char[zipheader.OrgLen];
		  
		 int BufferSize=0;
		 
		 
		 if (zipheader.isCompress)
		 {
		 
			    //HMODULE ZipMod;
				//ZipMod = LoadLibrary("Swzip.dll");
				CCompress pzip;
				//pzip = (Compress) GetProcAddress(ZipMod, "createZipObject");
				//CCompress *cc = pzip();

				char *DataTemp = new char[zipheader.ThisLen];
				int readsize = fread(DataTemp,1,zipheader.ThisLen,fp);

				if (readsize  == zipheader.ThisLen)
				{
				  pzip.DeCompress((unsigned char *) DataTemp,zipheader.ThisLen,(unsigned char *)DataBuffer,&zipheader.OrgLen);
				  
				}

				BufferSize = zipheader.OrgLen;

				//delete cc;	
				delete DataTemp;

				//FreeLibrary(ZipMod);
		 }
		 else
		 {

			    int readsize = fread(DataBuffer,1,zipheader.OrgLen,fp);
				if (readsize  != zipheader.ThisLen)
				{
					//DataBuffer[0] = 0 ;
				}
				else
				{
					BufferSize = readsize;
					 
				}
		 
		 }

		 if (BufferSize > 0)
		 {
		 
		 
			 int ReadPos = 0;
			 

			 while(ReadPos < BufferSize)
			 {
				UpdateHeader updateheader;
				memcpy(&updateheader , DataBuffer + ReadPos , sizeof(UpdateHeader));
				ReadPos += sizeof(UpdateHeader);

				if (ProgressBar != NULL)
				{
	 
					 SendMessage(ProgressBar, PBM_SETRANGE, 0,
						MAKELPARAM(0, updateheader.ItemCount)); 

					 SendMessage(ProgressBar, PBM_SETSTEP, 1,
						0);  //step = 1

					 SendMessage(ProgressBar, PBM_SETPOS, 0,
						0);  //step = 1

				


					 /*MSG msg;
					 while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
					 {
							TranslateMessage(&msg);
							DispatchMessage(&msg);
					 }*/

					

				 }


				 
				int SavePos = ReadPos;
				for (int i = ReadPos ; i < ReadPos + updateheader.DataLen ; i++)
				{
				
						MSG msg;
						while (::PeekMessage(&msg, NULL,   // pump message until none
									NULL, NULL, PM_REMOVE))   // are left in the queue
						{
									 
										    TranslateMessage(&msg);
											DispatchMessage(&msg);

											if (msg.message == WM_QUIT)
												return -1;
									 
						}
						

					    if (DataBuffer[i] == char(13))
						{
							
							 
							if (ProgressBar != NULL)
							{	 
								SendMessage(ProgressBar, PBM_STEPIT, 0, 0); 
								//PostMessage(ProgressBar, PBM_STEPIT, 0, 0); 
								/*MSG msg;
								while (::PeekMessage(&msg, NULL,   // pump message until none
									NULL, NULL, PM_REMOVE))   // are left in the queue
								{
									 
										    TranslateMessage(&msg);
											DispatchMessage(&msg);

											if (msg.message == WM_QUIT)
												return -1;
									 
								}*/

								 
							}

							Sleep(1);

							char tmpstr[50];
							memset(tmpstr , 0 , 50);

							if (i - SavePos > 0)
							{
								
								strncpy(tmpstr,DataBuffer + SavePos , i - SavePos );
							}

							DataFileRes FileRes = spdb.GetSpamData(tmpstr);

							DataFile df;
							memset(&df,0,sizeof(df));
							//df.Status = 1; //used

							

							//0 : good , 1 : bad  , 2 : reset , 3 : public gateway , 4 : Enews				
							if (updateheader.Type == 0)
							{
								df.BadRate = 0;
								df.OkRate = 1;							
								df.Status = 2; //IP

								if (FileRes.SearchRes == -1)
								{
									spdb.InsertSpamData(tmpstr,&df);
								}
								else
								{
									
									//更正
									if (df.UsedCount <= 0 )
									spdb.UpdateData((char *)&df,sizeof(df),FileRes.SearchRes);
								}

								 
							}
							else if (updateheader.Type == 1)
							{							 
								df.BadRate = 1;
								df.OkRate = 0;
								df.Status = 2; //IP

								if (FileRes.SearchRes == -1)
								{
									spdb.InsertSpamData(tmpstr,&df);
								}
								else
								{
									//更正
									if (df.UsedCount <= 0 )
									spdb.UpdateData((char *) &df,sizeof(df),FileRes.SearchRes);

								}
							}
							else if (updateheader.Type == 2)
							{							 
								df.BadRate = 0;
								df.OkRate = 0;
								df.Status = 2; //IP

								if (FileRes.SearchRes == -1)
								{
									//不存在就不用修正了
									//spdb->InsertSpamData(tmpstr,&df);
								}
								else
								{
									//更正
									spdb.UpdateData((char *) &df,sizeof(df),FileRes.SearchRes);

								}
							}
							else if (updateheader.Type == 3)
							{							 
							
								if (FileRes.SearchRes == -1)
								{
									df.BadRate = 0;
									df.OkRate = 0;
									df.Status = 3; //gateway


									spdb.InsertSpamData(tmpstr,&df);
								}
								else
								{
									//更正
									//if (df.UsedCount <= 0)
									//{
									//df.BadRate = 0;
									//	df.OkRate = 0;
									//	df.Status = 3; //gateway

									//}
									//else
									//{									
										df.Status = 3; //gateway
									//}
									spdb.UpdateData((char *) &df,sizeof(df),FileRes.SearchRes);

								}
							}
							else if (updateheader.Type == 4)
							{							 
							

								if (FileRes.SearchRes == -1)
								{
								 
									df.BadRate = 0;
									df.OkRate = 1;
									df.Status = 4; //enews

									spdb.InsertSpamData(tmpstr,&df);
								}
								else
								{
									if (df.UsedCount <= 0)
									{
										df.BadRate = 0;
										df.OkRate = 1;
										df.Status = 4; //enews
									}
									else
									{
										df.Status = 4; //enews
									}

									//更正
									spdb.UpdateData((char *) &df,sizeof(df),FileRes.SearchRes);

								}
							}
						 
							
						    SavePos = i + 1;
						}
				}

				ReadPos = updateheader.DataLen + ReadPos;

				
			    
			 }

	 
		 
		 }

		 delete DataBuffer;
		}
	}

	//更新 last version
	return S_OK; 
}
 
 

double CChecker::GetDomainSpamRate()
{
 #ifdef _SPAMLOG
  FILE *fp=NULL;
  char LogRootPath[_MAX_PATH];
  strcpy(LogRootPath,DbSavePath);
  strcat(LogRootPath,"log//");

   
	
	  char LogPath[_MAX_PATH];	
	  char SaveLogPath[_MAX_PATH];
	  GetTempFileName(LogRootPath, // directory for temp files 
        "SF",                    // temp file name prefix 
        0,                        // create unique name 
        LogPath);              // buffer for name
	  
	  strcpy(SaveLogPath,LogPath);
	  //strcat(LogPath,".txt");
	  //rename(SaveLogPath,LogPath);
	  fp = fopen(LogPath,"r+b");
      
  

  char *bk = "\r\n";
  char sp = char(9);
  char tempchar[255];

  if (m_subject != NULL)
  {
	  fwrite(m_subject,1,strlen(m_subject),fp);
	  fwrite(bk,1,2,fp);
	  fwrite(bk,1,2,fp);
  }
 #endif
	
  StrList::iterator itor;
 	
  double SpamRate = 0.5 , tmpSpamRate = 0,TotalSpamRate = 1, TotalOkRate = 1;
  double DataOkRate = 0.0 , DataSpamRate = 0.0;
  
  std::string tmpstdstr ;

  for (itor =  DomainResList.begin(); itor != DomainResList.end() ; itor++)
  { 
	  tmpstdstr = *itor;
	   //長度不可大於 25 , 否則要處理截斷
	  if (tmpstdstr.length() >= 24)
	  {
			tmpstdstr.erase(tmpstdstr.begin() + 23 , tmpstdstr.end());
			
	  }

	  #ifdef _SPAMLOG

	  fwrite(tmpstdstr.c_str(),1,tmpstdstr.length(),fp);	  
	  fwrite(&sp,1,1,fp);

	  #endif

	  DataFileRes mdatafile;
	  SDataFile mSDataFile;	
	 
	  
	  memset(&mdatafile,0,sizeof(DataFileRes));
	  memset(&mSDataFile,0,sizeof(SDataFile));  
	  
	  mdatafile = db->GetSpamData((char *) tmpstdstr.c_str());
	  if (mdatafile.SearchRes == -1)
	  {
		  #ifdef _SPAMLOG
			fwrite(bk,1,2,fp); 
		  #endif

		  mdatafile.datafile.Status = 1;
		  int datapos = db->InsertSpamData((char *) tmpstdstr.c_str(),&mdatafile.datafile);

		  mSDataFile.DataFilePos = datapos;
		  mSDataFile.mDataFile = mdatafile.datafile;

		  SecDataList.insert(SecDataList.end(),mSDataFile);
	  
	  }
	  else
	  {
	      
		   #ifdef _SPAMLOG
		    itoa(mdatafile.datafile.OkRate,tempchar,10);
			fwrite(tempchar,1,strlen(tempchar),fp);
			fwrite(&sp,1,1,fp);

			itoa(mdatafile.datafile.BadRate,tempchar,10);
			fwrite(tempchar,1,strlen(tempchar),fp);
			fwrite(bk,1,2,fp);
		  #endif

		  mSDataFile.DataFilePos = mdatafile.SearchRes;		  
		  mSDataFile.mDataFile = mdatafile.datafile;
		  mSDataFile.mDataFile.UsedCount ++;

		  int offset = FIELD_OFFSET(DataFile,UsedCount);			  
		  db->UpdateData((char*) &(mSDataFile.mDataFile.UsedCount), sizeof(mSDataFile.mDataFile.UsedCount),mdatafile.SearchRes + offset,0);
		  //UpdateSpamData 還會在更新一次 		 
		  SecDataList.insert(SecDataList.end(),mSDataFile);

		  DataOkRate = mdatafile.datafile.OkRate;
		  DataSpamRate = mdatafile.datafile.BadRate;

			if (DataOkRate < 0 ) DataOkRate = 0;
			if (DataSpamRate < 0) DataSpamRate = 0;		

			if (DataOkRate == 0 && DataSpamRate == 0)
			{
				tmpSpamRate = 0.5;		
			}
			else
			{
				double KeyTotalRate = DataOkRate + DataSpamRate;

				tmpSpamRate = min(((double)DataSpamRate / KeyTotalRate),1);
				tmpSpamRate = tmpSpamRate / (tmpSpamRate + min(((double) DataOkRate / (double)KeyTotalRate),1));
				tmpSpamRate = min(tmpSpamRate,0.99);
				tmpSpamRate = max(tmpSpamRate,0.01);
			}		

		 
		 TotalSpamRate = TotalSpamRate * tmpSpamRate;		 	
		 TotalOkRate = TotalOkRate * (1-tmpSpamRate);
		  
	  }


  }

  SpamRate = TotalSpamRate / (TotalSpamRate + TotalOkRate);

  #ifdef _SPAMLOG
	fwrite(bk,1,2,fp); 

	 
	
	double RateRes =SpamRate;

	int     decimal,   sign;
    char    *buffer;
    int     precision = 10;
     

    buffer = _ecvt( RateRes, precision, &decimal, &sign );

	 
	//_gcvt(RateRes,3,tempchar);
	itoa( decimal,tempchar,10);

	//fwrite(dotstr,1,2,fp);
	fwrite(buffer,1,strlen(buffer),fp);
	fwrite(" : ",1,3,fp);
	fwrite(tempchar,1,strlen(tempchar),fp);
	fclose(fp);


	   strcat(LogPath,"-IP.txt");
	   rename(SaveLogPath,LogPath);

  #endif


  return SpamRate;
}

void CChecker::GetGramKeys( char* InStr)
{
	int GramKeyLimit = 20;
	int GramKeyLimitIdx = 0;

	//(Body[i] & 0xFF) > 128
	if (InStr == NULL) return ;

	std::string MultiStr;
	std::string SingleStr;


	int len = strlen(InStr);

	for (int i = 0 ; i < len ; i ++)
	{
		if ((InStr[i] & 0xFF) > 128)
		{
			MultiStr.append(InStr+i,2);
			i++;
		}
		else
		{
			SingleStr.append(InStr+i,1);		
		}

	
	}

	//multi byte
	const char *tmpp = MultiStr.c_str();
	for (int j = 0 ; j < MultiStr.size(); j +=2)
	{
	
		char tempstr[5];
		memset(tempstr,0,5);
		memcpy(tempstr,tmpp+j,4);

		SecResList.insert(SecResList.end(),tempstr);

		GramKeyLimitIdx ++;
		if (GramKeyLimitIdx > GramKeyLimit)
			break;
	
	}

	//sigle
	int SavePos = 0;
	GramKeyLimitIdx = 0;
	
	tmpp = SingleStr.c_str();
	for (int k = 0 ; k < SingleStr.size() ; k ++)
	{
	
		if (tmpp[k] == ' ')
		{
	
			char tempstr[24]; //largest size
			memset(tempstr,0,24);

			if (k - SavePos <= 24  && k - SavePos >= 4)  //最短 4 最長 24
			{
				memcpy(tempstr,tmpp+SavePos,k - SavePos);
				ToGramKey(tempstr);
				SecResList.insert(SecResList.end(),tempstr);

				GramKeyLimitIdx ++;
				
			}
		
			SavePos = k + 1;
		} 
		else if(k + 1  == SingleStr.size())
		{
		
			char tempstr[24]; //largest size
			memset(tempstr,0,24);

			if (k + 1 - SavePos <= 24  && k + 1 - SavePos >= 4)  //最短 4 最長 24
			{
				memcpy(tempstr,tmpp+SavePos,k + 1 - SavePos);
				ToGramKey(tempstr);
				SecResList.insert(SecResList.end(),tempstr);

				GramKeyLimitIdx ++;
			}
		
		}

		if (GramKeyLimitIdx > GramKeyLimit)
					break;
		
		
	
	}

	if (SavePos ==0 && SingleStr.size() >= 4 && SingleStr.size() <= 24)
	{	
		SingleStr = lowercase(SingleStr);
		SecResList.insert(SecResList.end(),SingleStr);
	}



}

bool CChecker::SimpleCheckDomain(char* Domain)
{

	int len = strlen(Domain);
	int DotCount = 0;

	for(int i = 0 ; (i < len) && (i < 50)  ; i ++)
	{
	
		if (Domain[i] == '.')
		{
			//DotCount ++;

			//if (DotCount >= 3)
				return true;
		}
	
	}

	return false;
}

char *CChecker::GetKeyGenStr()
{

  
  StrList::iterator itor; 
  Checkerstdstr = ""; 
  
  for (itor =  ResList.begin(); itor != ResList.end() ; itor++)
  {
		Checkerstdstr = Checkerstdstr + *itor;

		//if (itor != ResList.end())
		Checkerstdstr = Checkerstdstr + "\r";
  }

  
  return (char *) Checkerstdstr.c_str();
}

void CChecker::GetDomainIdent(char* ReceiveBy , char* DomainIdent)
{
 static char *DomainList[] =
  {".com.tw", //twnic zone
   ".net.tw",
   ".org.tw",
   ".gov.tw",
   ".idv.tw",
   ".game.tw",
   ".ebiz.tw",
   ".club.tw",
   ".edu.tw",
   ".tw",
   ".com.nz", //networksolution
   ".co.uk",
   ".com",
   ".net",
   ".org",
   ".info",
   ".biz",
   ".tv",
   ".us",
   ".cc",
   ".name",
   ".biz",
   ".de",
   ".be",
   ".at",
   ".co.jp", //jpnic
   ".ad.jp",
   ".ac.jp",
   ".go.jp",
   ".or.jp",
   ".ne.jp",
   ".gr.jp",
   ".ed.jp",
   ".lg.jp",
   ".jp",
   ".com.cn",//cnnic
   ".ac.cn",
   ".org.cn",
   ".edu.cn",
   ".net.cn",
   ".adm.cn",
   ".gov.cn",
   ".cn",
   ".com.hk",//hknic
   ".net.hk",
   ".org.hk",
   ".edu.hk",
   ".gov.hk",
   ".idv.hk",
   ".hk",
   ".co.kr",//krnic
   ".ac.kr",
   ".cc.kr",
   ".go.kr",
   ".ne.kr",
   ".cr.kr",
   ".re.kr",
   ".pe.kr",
   ".mil.kr",
   ".kr"};

   if (ReceiveBy == NULL || DomainIdent == NULL) return ;
   

   int DomainSize = sizeof(DomainList) / sizeof(DomainList[0]);

   int len = strlen(ReceiveBy);
   
   char *revstr = _strrev(_strdup(ReceiveBy));

   for (int i=0 ; i < DomainSize ; i ++)
   {
	   	char *DomainRevStr = _strrev(_strdup(DomainList[i]));
		int DomainLen = strlen(DomainList[i]);
	    
		if (strnicmp(DomainRevStr,revstr,DomainLen) == 0)
		{
			free(DomainRevStr);
			free(revstr);

			//求第二個 . 
			for (int j = (len - DomainLen -1) ; j >= 0 ; j --)
			{
				if (ReceiveBy[j] == '.' )
				{
					int ResLen = len - j - DomainLen ;
					if (ResLen > 0)
					{
						strncpy(DomainIdent,ReceiveBy+j+1, ResLen);
						DomainIdent[ResLen-1] = 0;
					}

					break;
				} 
				else if (j == 0)
				{
				

					int ResLen = len - DomainLen ;
					if (ResLen > 0)
					{
						strncpy(DomainIdent,ReceiveBy, ResLen);
						DomainIdent[ResLen] = 0;
					}

				}
			}


		
			return;
		}

		free(DomainRevStr);
   }

   free(revstr);


}


void  CChecker::GetLastReceiveBy(char* header , char* ReceiveBy )
{
	if (header == NULL || ReceiveBy == NULL) return ;

	//求出長度
	int len = strlen(header);
	char ParseStep = 0;
	int BreakPos = 0;
	int SpPos = 0;
	int ByPos = 0;
	char TempByStr[255];
	char FromStr[255];

	memset(FromStr,0,255);
	memset(ReceiveBy,0,sizeof(ReceiveBy));

	//如果沒有 ident string , 則取 last receieve 的 by 
	for(int i=len-1 ; i >=0  ; i--)
	{
	
		if ( header[i+1] != 0x20 && header[i+1] != 9 && header[i] == 10 && header[i-1] == 13)
		{
			//Save Break Line position
			BreakPos = i - 2;
			i = i - 1;
			ParseStep = 1;

			memset(FromStr,0,255);
			memset(ReceiveBy,0,sizeof(ReceiveBy));
		} 
		else if (ParseStep == 1)
		{
			if (strnicmp(header+i-3,"by ",3) == 0  )
			{

				ParseStep = 2;
				ByPos = i-4;
				memset(TempByStr,0,255);
				if (SpPos - i > 0)
				{
					
					strncpy(TempByStr,header+i,SpPos - i);
				}
			

				i = i - 3;

			}
			else if (header[i] == 0x20)
			{
				SpPos = i;
			}

		} 
		else if (ParseStep == 2)
		{
		
			if (strnicmp(header+i-15,"Received: from ",15) == 0 && ( i-15 <= 0 || strnicmp(header+i-17,"\r\n",2) == 0)  )
			{
				ParseStep = 0;

				char TempStr[255];
				memset(TempStr,0,255);

				if (ByPos - i > 0)
				strncpy(TempStr,header+i,ByPos - i);
				strcpy(FromStr,TempStr);
				//放入 FromStr
				memset(ReceiveBy,0,sizeof(ReceiveBy));
				strcpy(ReceiveBy,TempByStr);
				//放入 ReceiveBy
			
						
				i = i - 15;
				
			}
			
		}
	
	}

	//取出 receieveBy 的 domain ,  作為 indentstr
	GetDomainIdent(ReceiveBy,ReceiveBy);
	//取出正確 Domainident
}

void CChecker::ToGramKey(char *AStr)
{
	char TmpStr[255] = {0};

	int templen = 0;
	int len = strlen(AStr);
	for (int i = 0 ; i < len ; i++)
	{
		if ( (AStr[i] >= 65 && AStr[i] <= 90) ||
			  (AStr[i] >= 97 && AStr[i] <= 122) )
		{
		  TmpStr[templen] =	tolower(AStr[i]);
		  templen++;
		}

	}

	strcpy(AStr,TmpStr);

}
void CChecker::TrimPathKey(char *Path)
{

	int DotCount = 0;
	int CutPos = 0;

	int len = strlen(Path);

	for (int i = 0 ; i < len ; i++)
	{
	
		if ( Path[i] == '.' )
		{
			DotCount ++;
			if (DotCount == 1)
				CutPos = i + 1;		
		}
	
	}

	if (DotCount >= 2 )
	{
		strcpy(Path,Path+CutPos);	
	}


}

int CChecker::GetPathIdent(char *Path1 , char *Path2) //回傳第幾個 receieve
{

	int len1 = strlen(Path1);
	int len2 = strlen(Path2);

	char *FindPath = NULL;
	char *FindPath2 = NULL;
	int len = 0;
	int ResultPos = -1;
	//int eqpos = -1;
	int MatchCount = 0;

	if (len1 < 0 || len2 < 0 ) return ResultPos;

	if (len1 > len2)
	{
		FindPath = Path2;
		FindPath2 = Path1;
		len = len2;
	}
	else
	{
		FindPath = Path1;
		FindPath2 = Path2;
		len = len1;
	}



	for (int i = 0 ; i < len ; i ++)
	{
	
		if (FindPath[i] != FindPath2[i])
		{
			//eqpos = i - 1;
			break;
		}
		else
		{
			MatchCount ++;
		}
	
	}



	if (MatchCount > 0 )
	{
		for (int i = 0 ; i < MatchCount ; i ++)
		{
		
			if (FindPath[i] == '|')
			{
				ResultPos ++;			
			}

		}
	
	}

	//if (ResultPos >0 ) ResultPos ++;


	
	return ResultPos;
}

void  CChecker::GetHeaderKeyStrIP(char* header , char* headerkey , char *IP)
{
	char rc = 0;

	char headerkeystr[255]; 

	strcpy(headerkeystr,headerkey);
	strcat(headerkeystr,": ");

	char ParseStep = 0;
	int FromSavePos = 0;
	

	if ( header == NULL || IP == NULL ||
		header[0] == 0 ) return ;

	int len = strlen(header);
	int keylen = strlen(headerkeystr);
	//int KeyLen = strlen(KeyStr);
	 

	for (int i=0 ; i < len ; i ++)
	{
		if (ParseStep == 0 && strnicmp(header+i,headerkeystr,keylen) == 0)		
		{			 
		 
			ParseStep = 1;
			//ReceivedCount ++;
		 

			i = i + (keylen - 1);
			FromSavePos = i + 1;			
		}
		else if (ParseStep == 1 )
		{
		
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{
				//換行了 . 
				ParseStep = 0;

				strncpy(IP,header + FromSavePos, i - FromSavePos);
				IP[i - FromSavePos] =0;
			
				break;


			}
			 
		 
		
		}

	}








}

char CChecker::GetKeyPosIP(char* header ,int KeyPos , char *IP)
{
	char rc = 0;

	char Received[255]; 
	char ParseStep = 0;
	int FromSavePos = 0;
	int ReceivedCount = 0;


 
	bool KeyPosFound = false;
	bool LocalLan = false;

	//string AllByStr; 

	if ( header == NULL || IP == NULL ||
		header[0] == 0 ) return -1;

	int len = strlen(header);
	//int KeyLen = strlen(KeyStr);
	 

	for (int i=0 ; i < len ; i ++)
	{
		if (ParseStep == 0 && strnicmp(header+i,"Received: from ",15) == 0)		
		{			 
		 
			ParseStep = 1;
			ReceivedCount ++;
		 

			i = i + 14;
			FromSavePos = i + 1;			
		}
		else if (ParseStep == 1 )
		{
		
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{
				//換行了 .
				ParseStep = 0;

				if (ReceivedCount == KeyPos)
				{				
					

					memset(Received,0,255);
					if (i-FromSavePos-1 > 0)
					{
						strncpy(Received,header+FromSavePos, i-FromSavePos-1);
						KeyPosFound = true;
					}

					//i = i + 2;
					//BySavePos = i + 1;
					//ParseStep = 0;

					break;

				}
			}
			
		 
		
		}

	}


	if (KeyPosFound)
	{
	
		
						char TempIP[17];
						memset(TempIP,0,17);
						int DotCount = 0;
						int idx = 0;
						int idxpos = -1;
						int len = strlen(Received);

						//如果最後是 digital 則補上空白
						if (isdigit(Received[len-1]))
						{
							Received[len] = ' ';
							Received[len+1] = 0;
							len++;
						}
						
						
					    for (int i = 0 ; i < len ; i++)
						{
						
							if (isdigit(Received[i]) ) //遇防 Seednet 的 Received: from [218.84.134.166] (port=3228 helo=139.175.54.239)
							{
							  if (Received[i-1] == '=')
							  {
								  LocalLan = false;
								  break;
							  }

								if (idxpos == -1)
									idxpos = i;

								TempIP[idx] = Received[i];
								idx ++;
							}
							else if (Received[i] == '.' &&  idx > 0)
							{
								TempIP[idx] = Received[i];
								idx ++;
								DotCount ++;

								if (i - idxpos > 3 || idx >= 16)
								{
									//不是ip
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,17);
									LocalLan = false;
								}
								else if (DotCount == 2)
								{
									if (strncmp(TempIP,"192.168",7) == 0 || strncmp(TempIP,"10.",3) == 0 ||strncmp(TempIP,"127.0",5) == 0)
									{ 
										
										LocalLan = true;								
										//break;
							 
									} 
									else if (strncmp(TempIP,"172.",4) == 0)
									{
										//locallan
										char LocalLanCheckStr[7];
										memset(LocalLanCheckStr,0,7);

										if (idx - 4 - 1 > 0)
										strncpy(LocalLanCheckStr,TempIP + 4,idx - 4 - 1);

										int IpValue = atoi(LocalLanCheckStr);
										if (IpValue >= 16 && IpValue <= 31)
										{													
										    LocalLan = true;

										
											//break;										
										}						
									}
								
								}

								idxpos = i+1;
								
							}
							else
							{
								if (DotCount == 3 && TempIP[idx] != '.') //預防 adsl-126.49.155.info.com.ph ([222.126.49.155]) 
								{
									TempIP[idx+1] = 0; //得到 IP 了
									IP[0] = 0; //ip ini										

										//第一個有效 IP 產生
										if (strlen(TempIP) > 7)
										{										
											strcpy(IP,TempIP);
											//IPfound = true;										
										}					
								}
							
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,17);
									LocalLan = false;
							}
						}
	
	
	}
	else
	{
		IP[0] = 0;
		rc = 0;
	}

	if (!KeyPosFound)
	{
		rc = -2;
	}


	if (LocalLan)
	{
	  rc = 1;
	}



	return rc;

}

char CChecker::GetKeyStrIP(char* header ,char *KeyStr , char *IP)
{
	char rc = 0;

	char Received[255];
	//char ByStr[255];
	//char PrevByStr[255];
	//memset(PrevByStr,0,255);

	//char KeyByStr[255];
	//int ReceivedCount = 0;
	//int ReceivedIdent = -1;
	char ParseStep = 0;
	//int BySavePos = 0;
	int FromSavePos = 0;


 
	bool KeyStrFound = false;
	bool LocalLan = false;

	//string AllByStr; 

	if ( header == NULL || IP == NULL || KeyStr == NULL ||
		header[0] == 0 || KeyStr[0] == 0) return -1;

	int len = strlen(header);
	int KeyLen = strlen(KeyStr);
	 

	for (int i=0 ; i < len ; i ++)
	{
		if (ParseStep == 0 && strnicmp(header+i,"Received: from ",15) == 0)		
		{			 
		 
			ParseStep = 1;	
		 

			i = i + 14;
			FromSavePos = i + 1;			
		}
		else if (ParseStep == 1 )
		{
		
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{
				//換行了 .
				ParseStep = 0;
			}
			else if (strnicmp(header+i,KeyStr,KeyLen) == 0)
			{				
				

				memset(Received,0,255);
				if (i-FromSavePos-1 > 0)
				strncpy(Received,header+FromSavePos, i-FromSavePos-1);
				KeyStrFound = true;

				//i = i + 2;
				//BySavePos = i + 1;
				//ParseStep = 0;

				break;

			}
		 
		
		}

	}


	if (KeyStrFound)
	{
	
		
						char TempIP[17];
						memset(TempIP,0,17);
						int DotCount = 0;
						int idx = 0;
						int idxpos = -1;
						int len = strlen(Received);

						//如果最後是 digital 則補上空白
						if (isdigit(Received[len-1]))
						{
							Received[len] = ' ';
							Received[len+1] = 0;
							len++;
						}
						
						
					    for (int i = 0 ; i < len ; i++)
						{
						
							if (isdigit(Received[i]) ) //遇防 Seednet 的 Received: from [218.84.134.166] (port=3228 helo=139.175.54.239)
							{
							  if (Received[i-1] == '=')
							  {
								  LocalLan = false;
								  break;
							  }

								if (idxpos == -1)
									idxpos = i;

								TempIP[idx] = Received[i];
								idx ++;
							}
							else if (Received[i] == '.' &&  idx > 0)
							{
								TempIP[idx] = Received[i];
								idx ++;
								DotCount ++;

								if (i - idxpos > 3 || idx >= 16)
								{
									//不是ip
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,17);
									LocalLan = false;
								}
								else if (DotCount == 2)
								{
									if (strncmp(TempIP,"192.168",7) == 0 || strncmp(TempIP,"10.",3) == 0 ||strncmp(TempIP,"127.0",5) == 0)
									{ 
										
										LocalLan = true;								
										//break;
							 
									} 
									else if (strncmp(TempIP,"172.",4) == 0)
									{
										//locallan
										char LocalLanCheckStr[7];
										memset(LocalLanCheckStr,0,7);

										if (idx - 4 - 1 > 0)
										strncpy(LocalLanCheckStr,TempIP + 4,idx - 4 - 1);

										int IpValue = atoi(LocalLanCheckStr);
										if (IpValue >= 16 && IpValue <= 31)
										{													
										    LocalLan = true;

										
											//break;										
										}						
									}
								
								}

								idxpos = i+1;
								
							}
							else
							{
								if (DotCount == 3 && TempIP[idx] != '.') //預防 adsl-126.49.155.info.com.ph ([222.126.49.155]) 
								{
									TempIP[idx+1] = 0; //得到 IP 了
									IP[0] = 0; //ip ini										

										//第一個有效 IP 產生
										if (strlen(TempIP) > 7)
										{										
											strcpy(IP,TempIP);
											//IPfound = true;										
										}					
								}
							
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,17);
									LocalLan = false;
							}
						}
	
	
	}
	else
	{
		IP[0] = 0;
		rc = 0;
	}

	if (!KeyStrFound)
	{
		rc = -2;
	}


	if (LocalLan)
	{
	  rc = 1;
	}



	return rc;
	

}

bool  CChecker::GetMailCharset(char* MailContent , char* Charset)
{

   
   CEmail cem;

   MailData *mb = cem.GetMailData(MailContent);
   if (mb == NULL)
   {
	  return false;						 					
   }


   MailHeader *mh = cem.GetMailHeader(mb->mailHeader);

   if (mh->Content_Type->charset[0] != 0)
   {
	    cem.TrimQuote(mh->Content_Type->charset , '"'); 
	    strcpy(Charset , mh->Content_Type->charset);
		
		cem.FreeMailHeader(mh);
		cem.FreeMailData(mb);

		return true;
   }
   else
   if (mh->Content_Type->boundary[0] != 0)
   {
	   MailBoundary* m_MailBoundary = 
					cem.GetBoundary(mb->mailBody,mh->Content_Type->boundary);

	    MailBoundary* ptr =  m_MailBoundary;
								
		while (ptr)
		{
			if (ptr->IsRealData && ptr->BoundHeader->Content_Type->charset[0] != 0)
			{
				cem.TrimQuote(ptr->BoundHeader->Content_Type->charset , '"');
				strcpy(Charset , ptr->BoundHeader->Content_Type->charset);
				cem.FreeBoundary(m_MailBoundary);
				cem.FreeMailHeader(mh);
				cem.FreeMailData(mb);

				return true;


			}

			ptr = ptr->next;	
		}

		cem.FreeBoundary(m_MailBoundary);
   
   }

   return false;

}

bool  CChecker::CheckLocalMail(char* header)
{

		

	char Received[30][255];
	//char ByStr[255];
	//char PrevByStr[255];
	//memset(PrevByStr,0,255);

	//char KeyByStr[255];
	int ReceivedCount = 0;
	int ReceivedIdent = -1;
	char ParseStep = 0;
	int BySavePos = 0;
	int FromSavePos = 0;

	//memset(AllByStr2,0,255);
	

	//bool SameByPath = false;
	//string AllByStr;

	//AllByStr2 = "";
	
	//AllByStr = "";
	//bool StopGo = false;

	if ( header == NULL  ) return false;
	int len = strlen(header);

	/*char clen[255];
	itoa(len,clen,10);
	OutputDebugString(clen);
	OutputDebugString("\r\n");*/


	//char *AHeader = new char[len+1];
	//strcpy(AHeader , header);
	 //OutputDebugString(header);

	for (int i=0 ; i < len ; i ++)
	{
		if (ParseStep == 0 && strnicmp(header+i,"Received: from ",15) == 0)		
		{			 
		 
			ParseStep = 1;	
		 

			i = i + 14;
			FromSavePos = i + 1;			
		}
		else if (ParseStep == 1 )
		{
		
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{
				//換行了 .
				ParseStep = 0;
			}
			else if (strnicmp(header+i,"by ",3) == 0)
			{
				memset(Received[ReceivedCount],0,255);
				if (i-FromSavePos-1 > 0 )
				{
				
					strncpy(Received[ReceivedCount],header+FromSavePos, i-FromSavePos-1);
					ReceivedCount++;

					if (ReceivedCount >= 30) break; 
				}

				i = i + 2;
				BySavePos = i + 1;
				ParseStep = 0;

			}
		
		}	
	}


	int LocalLanCount = 0;
	ReceivedIdent = 0;

 
		 
					//取 ip					
				bool IPfound = false;
				for(int k = ReceivedIdent ; k < ReceivedCount ; k ++)
				{
					    if (IPfound) break;

						char TempIP[17];
						memset(TempIP,0,17);
						int DotCount = 0;
						int idx = 0;
						int idxpos = -1;
						int len = strlen(Received[k]);

						//如果最後是 digital 則補上空白
						if (isdigit(Received[k][len-1]))
						{
							Received[k][len] = ' ';
							Received[k][len+1] = 0;
							len++;
						}
						
						
					    for (int i = 0 ; i < len ; i++)
						{
						
							if (isdigit(Received[k][i]) ) //遇防 Seednet 的 Received: from [218.84.134.166] (port=3228 helo=139.175.54.239)
							{
							  if (Received[k][i-1] == '=')
							    break;

								if (idxpos == -1)
									idxpos = i;

								TempIP[idx] = Received[k][i];
								idx ++;
							}
							else if (Received[k][i] == '.' &&  idx > 0)
							{
								TempIP[idx] = Received[k][i];
								idx ++;
								DotCount ++;

								if (i - idxpos > 3 || idx >= 16)
								{
									//不是ip
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,17);
								}
								else if (DotCount == 2)
								{
									if (strncmp(TempIP,"192.168",7) == 0 || strncmp(TempIP,"10.",3) == 0 ||strncmp(TempIP,"127.0",5) == 0)
									{ 
										
										LocalLanCount++;
										break;
							 
									} 
									else if (strncmp(TempIP,"172.",4) == 0)
									{
										//locallan
										char LocalLanCheckStr[7];
										memset(LocalLanCheckStr,0,7);
										if (idx - 4 - 1 > 0)
										strncpy(LocalLanCheckStr,TempIP + 4,idx - 4 - 1);

										int IpValue = atoi(LocalLanCheckStr);
										if (IpValue >= 16 && IpValue <= 31)
										{													
										    LocalLanCount++; 
											break;										
										}						
									}
								
								}

								idxpos = i+1;
								
							}
							else
							{
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,17);
							}
						}
				}
	
 


    //delete AHeader;

	if (LocalLanCount == ReceivedCount)
	{
		return true;
	}
	else
	{
		return false;
	}
 
}

void  CChecker::WriteMailPathData(MailPath *mailpath)
{
	if (mailpath->NextWrite == -2) return;

		 char path_buffer[_MAX_PATH];
		 char drive[_MAX_DRIVE];
		 char dir[_MAX_DIR];
		 char fname[_MAX_FNAME];
		 char ext[_MAX_EXT];

	   
		 HINSTANCE hInstance = GetModuleHandle(NULL);
         GetModuleFileName(hInstance, path_buffer, MAX_PATH);

		 _splitpath( path_buffer, drive, dir, fname, ext );

		 char FilePath[_MAX_PATH];	   
		 

		 strcpy(FilePath,drive);
		 strcat(FilePath,dir);	    
		 
		 strcat(FilePath,"MailPath2.dat"); 

		FILE *fp=NULL;
		fp = fopen(FilePath,"r+b");

		if (fp != NULL)
		{

			if (mailpath->NextWrite == -1)
			{
	
				//接尾端
				    mailpath->NextWrite = 1;
					fseek(fp,0,SEEK_END);			 
					fwrite(mailpath,1,sizeof(MailPath),fp);
			}
			else
			{
					MailPath tmpmailpath;
					while(fread(&tmpmailpath,1,sizeof(tmpmailpath),fp))
					{
						if (strcmp(tmpmailpath.PathKey,mailpath->PathKey) == 0) 
						{

								int filepos = ftell(fp);
								fseek(fp,filepos - sizeof(MailPath),SEEK_SET);
								fwrite(mailpath,1,sizeof(MailPath),fp);

								break;


						}
			

					}
			}

			fclose(fp);

		}




}

/*
void  CChecker::WriteWhoisInfo(char* IP)
{

	if (IP == NULL || IP[0] ==0)  return;
	
	//check contact db
	CSpamContact sc;
	sc.SetIp(IP);
	
	if (ContactDB->Search(sc.GetIPValue(),ContactDB->FirstRecPos) == -1)
	{
	
		//找不到 db
		sc.GetContact();

		if (sc.contact[0] != 0 && sc.InetNumS[0] != 0 && sc.InetNumE[0] != 0)
		{
			Contact ct;
			memset(&ct,0,sizeof(Contact));

			strcpy(ct.country,sc.country);
			strcpy(ct.Email,sc.contact);
			strcpy(ct.netname,sc.netname);

		    int DataFilePos =	ContactDB->InsertData((char *) &ct,sizeof(ct));
			if (DataFilePos > 0)
			{
			
				CSpamContact m_sc1;	
				m_sc1.SetIp(sc.InetNumS);

				CSpamContact m_sc2;	
				m_sc2.SetIp(sc.InetNumE);


				ContactDB->InserKey(m_sc1.GetIPValue(),	m_sc2.GetIPValue(),ContactDB->FirstRecPos,DataFilePos);	
			}

		
		}
			
	}



}*/

char CChecker::GetHeaderIP(char* Subject , char* header , char *IP,MailPath *mailpath)
{
	char rc = 0;

	char Received[10][255];
	char ByStr[255];
	char PrevByStr[255];
	memset(PrevByStr,0,255);

	char KeyByStr[255];
	int ReceivedCount = 0;
	int ReceivedIdent = -1;
	char ParseStep = 0;
	int BySavePos = 0;
	int FromSavePos = 0;

	//memset(AllByStr2,0,255);
	

	bool SameByPath = false;
	string AllByStr;

	//AllByStr2 = "";
	
	//AllByStr = "";
	//bool StopGo = false;

	if ( header == NULL || IP == NULL ||mailpath == NULL) return -1;
	int len = strlen(header);
	 

	for (int i=0 ; i < len ; i ++)
	{
		if (ParseStep == 0 && strnicmp(header+i,"Received: from ",15) == 0)		
		{			 
		 
			ParseStep = 1;	
		 

			i = i + 14;
			FromSavePos = i + 1;			
		}
		else if (ParseStep == 1 )
		{
		
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{
				//換行了 .
				ParseStep = 0;

				if (ReceivedCount == 0)
				{
					//第一個 receieve 找不到 by 則不處理 ip
					//gatelock bug : 
					//Received: from delhi-202.54.107-167.vsnl.net.in (delhi-202.54.107-167.vsnl.net.in [202.54.107.167] (may be forged))
					//Received: from ([220.138.98.150]) by higp10.gatelock.com.tw with ISVW_POP3 3.51.1031(7.000-1011,2.711.00); Sat, 02 Jul 2005 21:56:00 +0800
					//by ms55.hinet.net (8.8.8/8.8.8) with SMTP id SAA22376;
					//Sat, 2 Jul 2005 18:40:24 +0800 (CST)
					IP[0]  = 0;
					return 0;
				}

			}
			else if (strnicmp(header+i,"by ",3) == 0)
			{
				if (ReceivedCount >= 10)
					break;
				
				memset(Received[ReceivedCount],0,255);
				if (i-FromSavePos-1 > 0)
				{
					strncpy(Received[ReceivedCount],header+FromSavePos, i-FromSavePos-1);
					ReceivedCount++;
				}

				i = i + 2;
				BySavePos = i + 1;
				ParseStep = 2;

			}
		
		}
		else if (ParseStep == 2 )
		{
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{	//換行了 .
				ParseStep = 0;
			}
			if (header[i] == 0x20 || header[i] == ';' )
			{ 
				ParseStep = 0;	
				memset(ByStr,0,255);
				
				if (i-BySavePos > 0)
				{
					
                    if (i-BySavePos > 0)
					strncpy(ByStr,header+BySavePos, i-BySavePos);

				
					//AllByStr2 = AllByStr2 + ByStr ;					 
					//AllByStr2 = AllByStr2 + '|'; 
					

					TrimPathKey(ByStr);

					if (ReceivedCount == 1)
					{
						strcpy(KeyByStr,ByStr);	
						
						//strcpy(PrevByStr,ByStr);
					}

					
					//else
					//{
					
						//if (!StopGo && stricmp(ByStr,PrevByStr) == 0 )
						//{						
							//SameByPath = true;
						//}
						//else 
						//{
							//SameByPath = false;
							//StopGo = true;
						//}

						//strcpy(PrevByStr,ByStr);

					//}

					AllByStr = AllByStr + ByStr ;
					//AllByStr = AllByStr + char(10); 
					AllByStr = AllByStr + '|'; 
				}
			
			}
		}
	}



	if (ReceivedCount == 1)
	{
		ReceivedIdent = 0;
	}
	//else if (SameByPath)
	//{
		//ReceivedIdent = ReceivedCount -1 ;
	//}
	else if (ReceivedCount > 0 && Subject != NULL && strlen(Subject) > 0)
	{
		 //加入深度 	
		 //char Deepc[25];
		 //itoa(ReceivedCount,Deepc,10);
		 //strcat(KeyByStr,Deepc);

		 //取出 subject key
		 int subkeylen = strlen(Subject);
		 char subkey[5];
		 memset(subkey,0,5);
		 //subkey[5] = 0;
		 
		 std::string MultiStr;
		 std::string SingleStr;

		 //中英文分離
		 for (int i = 0 ; i < subkeylen ; i ++)
		 {		 
			 if ((Subject[i] & 0xFF) > 128)
			 {
				MultiStr.append(Subject+i,2);
				i++;
			 }
			 else
			 {
				SingleStr.append(Subject+i,1);		
			 }			
		 
		 }

		 //只取中文
		 if (MultiStr.size() > 0 )
		 {		 
			 if (MultiStr.size() > 2)
			 {
				int midlen = MultiStr.size() / 2;			
				strncpy(subkey,MultiStr.c_str() + midlen,4);
			 }
			 else
			 {
				strncpy(subkey,MultiStr.c_str(),4);
			 }		 
		 }
		 else
		 {
			 if (subkeylen > 2)
			 {
				int midlen = subkeylen / 2;			
				strncpy(subkey,Subject + midlen,4);
			 }
			 else
			 {
				strncpy(subkey,Subject,4);
			 }	 
		 
		 }

		 

		  
		
		 char path_buffer[_MAX_PATH];
		 char drive[_MAX_DRIVE];
		 char dir[_MAX_DIR];
		 char fname[_MAX_FNAME];
		 char ext[_MAX_EXT];

	   
		 HINSTANCE hInstance = GetModuleHandle(NULL);
         GetModuleFileName(hInstance, path_buffer, MAX_PATH);

		 _splitpath( path_buffer, drive, dir, fname, ext );

		 char FilePath[_MAX_PATH];	   
		 //char FileLogPath[_MAX_PATH];	//Log Path

		 strcpy(FilePath,drive);
		 strcat(FilePath,dir);	  
	   
		 //strcpy(FileLogPath,FilePath);
		 //strcat(FileLogPath,"MailPathLog.txt"); //Log Path

		
	   
		 strcat(FilePath,"MailPath2.dat"); 

		FILE *fp=NULL;
		fp = fopen(FilePath,"r+b");

		if (fp != NULL)
		{
		
			MailPath tmpmailpath;
			memset(&tmpmailpath,0,sizeof(tmpmailpath));
			bool ByPass = false;
		 

			int tmpIdent = -1;
			int MintmpIdent = -1;
			bool HasRepeat = false;
			bool FoundKeyStr = false;
			 


			while(fread(&tmpmailpath,1,sizeof(tmpmailpath),fp))
			{
				 

				if (strcmp(tmpmailpath.PathKey,KeyByStr) == 0) 
				{
					FoundKeyStr  = true;
				
					for (int k = 0 ; k < 5 ; k ++)
					{
					
						 //確認 subject
						if (strcmp(subkey,tmpmailpath.PathSubject[k]) != 0) //不跟重覆的 subject 比對
						{
							tmpIdent = GetPathIdent(tmpmailpath.PathStr[k] ,(char *) AllByStr.c_str());
							
							if (tmpIdent < MintmpIdent || MintmpIdent == -1)
							{
								MintmpIdent = tmpIdent ;
							}
							//if (tmpIdent > MaxtmpIdent)
							//	MaxtmpIdent = tmpIdent ; //取最大值 ==>改為取最小

						}	
						else
						{
							HasRepeat = true;
						}
					
					}
					/*
					if (mailpath.CurrSize != AllByStr.size())//避免重覆
					{
						strcpy(mailpath.PrevPath,mailpath.CurrPath); //move curr to prev					
						strcpy(mailpath.CurrPath,AllByStr.c_str());
						mailpath.PrevSize = mailpath.CurrSize;
						mailpath.CurrSize = AllByStr.size(); //由 len 改為 bystr len

						ReceivedIdent = GetPathIdent(mailpath.PrevPath , mailpath.CurrPath);

						int filepos = ftell(fp);
						fseek(fp,filepos - sizeof(mailpath),SEEK_SET);
						fwrite(&mailpath,1,sizeof(mailpath),fp);
					}
					else
					{
						//與再之前一個比較
						if (mailpath.PrevSize > 0 )
						  ReceivedIdent = GetPathIdent(mailpath.PrevPath ,(char *) AllByStr.c_str());

						ByPass = true;
					}*/

					if (!HasRepeat) //有重復不異動 record
					{
						//
						strcpy(tmpmailpath.PathStr[tmpmailpath.NextWrite],AllByStr.c_str());
						strcpy(tmpmailpath.PathSubject[tmpmailpath.NextWrite],subkey);

						if (tmpmailpath.NextWrite + 1 < 5)
						{
							tmpmailpath.NextWrite++;
						}
						else
						{
							tmpmailpath.NextWrite = 0;
						}

						/*int filepos = ftell(fp);
						fseek(fp,filepos - sizeof(mailpath),SEEK_SET);
						fwrite(&mailpath,1,sizeof(mailpath),fp);*/
						memcpy(mailpath,&tmpmailpath,sizeof(tmpmailpath));
						
					}
					else
					{
						mailpath->NextWrite = -2; //代表重覆
					
					}

					ReceivedIdent = MintmpIdent;



					break;
				
				}

			}

			if (ReceivedIdent == -1 && !HasRepeat && !FoundKeyStr) //沒找到 , 寫入尾端
			{
				if (strlen(KeyByStr) > 0)
				{
					memset(mailpath,0,sizeof(MailPath));
					strcpy(mailpath->PathKey,KeyByStr);
					mailpath->NextWrite = -1; //固意填入讓外部可以知到要寫入尾端
					strcpy(mailpath->PathStr[0],AllByStr.c_str());
					strcpy(mailpath->PathSubject[0],subkey);

					//fseek(fp,0,SEEK_END);			 
					//fwrite(&mailpath,1,sizeof(mailpath),fp);
				}
			}
	
			fclose(fp);
		}
		
					/*FILE *fp2=NULL;
					fp2 = fopen(FileLogPath,"r+b");
					if (fp2 != NULL)
					{	
						AllByStr = AllByStr + "\r\n";
						fseek(fp2,0,SEEK_END);
						fwrite(AllByStr.c_str(),1,AllByStr.size(),fp2);
						fclose(fp2);	
					}*/


	}

	int LocalLanCount = 0;

	if (ReceivedIdent > -1)
	{
	
		 
					//取 ip
						 
					
				bool IPfound = false;
				for(int k = ReceivedIdent ; k < ReceivedCount ; k ++)
				{
					    if (IPfound) break;

						char TempIP[17];
						memset(TempIP,0,17);
						int DotCount = 0;
						int idx = 0;
						int idxpos = -1;
						int len = strlen(Received[k]);

						//如果最後是 digital 則補上空白
						if (isdigit(Received[k][len-1]))
						{
							Received[k][len] = ' ';
							Received[k][len+1] = 0;
							len++;
						}
						
						
					    for (int i = 0 ; i < len ; i++)
						{
						
							if (isdigit(Received[k][i]) ) //遇防 Seednet 的 Received: from [218.84.134.166] (port=3228 helo=139.175.54.239)
							{
							  if (Received[k][i-1] == '=')
							    break;

								if (idxpos == -1)
									idxpos = i;

								TempIP[idx] = Received[k][i];
								idx ++;
							}
							else if (Received[k][i] == '.' &&  idx > 0)
							{
								TempIP[idx] = Received[k][i];
								idx ++;
								DotCount ++;

								if (i - idxpos > 3 || idx >= 16)
								{
									//不是ip
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,17);
								}
								else if (DotCount == 2)
								{
									if (strncmp(TempIP,"192.168",7) == 0 || strncmp(TempIP,"10.",3) == 0 ||strncmp(TempIP,"127.0",5) == 0)
									{ 
										
										LocalLanCount++;
										break;
							 
									} 
									else if (strncmp(TempIP,"172.",4) == 0)
									{
										//locallan
										char LocalLanCheckStr[7];
										memset(LocalLanCheckStr,0,7);
										if (idx - 4 - 1 > 0)
										strncpy(LocalLanCheckStr,TempIP + 4,idx - 4 - 1);

										int IpValue = atoi(LocalLanCheckStr);
										if (IpValue >= 16 && IpValue <= 31)
										{													
										    LocalLanCount++; 
											break;										
										}						
									}
								
								}

								idxpos = i+1;
								
							}
							else
							{
								if (DotCount == 3 && TempIP[idx] != '.') //預防 adsl-126.49.155.info.com.ph ([222.126.49.155]) 
								{
									TempIP[idx+1] = 0; //得到 IP 了
									IP[0] = 0; //ip ini	

									

										//第一個有效 IP 產生
										if (strlen(TempIP) > 7)
										{											
											DataFileRes mdatafile;
											memset(&mdatafile,0,sizeof(SDataFile));

											mdatafile = db->GetSpamData(TempIP);

											if (mdatafile.SearchRes != -1)
											{
												if (mdatafile.datafile.Status == 3)
												{
													//skip gateway
												}
												else
												{
													strcpy(IP,TempIP);
													IPfound = true;
												//	return 1; //先取第一個就好 for dogid : 904
												
												}
											}
											else
											{
												strcpy(IP,TempIP);
												IPfound = true;
												//return 1; //先取第一個就好
											}

										
											//return 1;
										}
										//else
										//{
						
										    //idx = 0;
											//DotCount = 0;
											//idxpos = -1;
											//memset(TempIP,0,16);
							 
										//}

						
								}
							
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,17);
							}
						}
				}
	
	}


	if (LocalLanCount == ReceivedCount)
	{
		rc = 1; 
	}

	return rc;

}

void CChecker::MainTainRBL(char *ServerIP ,bool BadIp,char *IP)
{

		    CHTTPclient http; 
 
			char SendBuf[255];
			strcpy(SendBuf,"//MainTainRBL.asp?");

			if (BadIp)
			{
		 	 strcat(SendBuf,"Bad=1");			
			}
			else
			{
				strcat(SendBuf,"Bad=0");				
			}
			strcat(SendBuf,"&IP=");
			strcat(SendBuf,IP);

			HANDLE hand = http.IPGET(ServerIP,"www.softworking.com",SendBuf,60 * 1000, 60 * 1000);
			

			DWORD rtn = WaitForSingleObject(hand,5);
			while (rtn != WAIT_OBJECT_0)
			{ 
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
			}
			
		 
			//FILE *fp = NULL;
			//fp = fopen(http.GetTempFilePath() , "rb");

			//int CheckRes=-1;
			//fscanf(fp,"%d\n",&CheckRes);
	 
			//fclose(fp);

			DeleteFile(http.GetTempFilePath());

			 

}

int CChecker::CheckServerSpam(char *ServerIP ,char* DogID , char *IP)
{

			CHTTPclient http; 
 
			char SendBuf[255];
			strcpy(SendBuf,"//CheckServerSpam.asp?");
			strcat(SendBuf,"DogId=");
			strcat(SendBuf,DogID);
			strcat(SendBuf,"&IP=");
			strcat(SendBuf,IP);

			HANDLE hand = http.IPGET(ServerIP,"www.softworking.com",SendBuf,60 * 1000, 60 * 1000);
			

			DWORD rtn = WaitForSingleObject(hand,5);
			while (rtn != WAIT_OBJECT_0)
			{ 
				//處理 waitting msg dump
				MSG msg;
				while (::PeekMessage(&msg, NULL,   // pump message until none
						NULL, NULL, PM_REMOVE))   // are left in the queue
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				rtn = WaitForSingleObject(hand,5);
			}
			
		 
			FILE *fp = NULL;
			fp = fopen(http.GetTempFilePath() , "rb");

			int CheckRes=-1;
			fscanf(fp,"%d\n",&CheckRes);
	 
			fclose(fp);

			DeleteFile(http.GetTempFilePath());

			return CheckRes;


}


/*
char CChecker::GetHeaderIP(char* header , char *IP)
{

	char ParseStep = 0;
	char FromStr[255];
	char FromStr2[255];
	char ByStr[255];
	int FromSavePos = 0;
	int BySavePos = 0;
	int From2SavePos = 0;
	

	if ( header == NULL || IP == NULL ) return -1;
	int len = strlen(header);

	for (int i=0 ; i < len ; i ++)
	{
	
	
		if (ParseStep == 0 && strnicmp(header+i,"Received: from ",15) == 0)		
		{
			From2SavePos = 0;
			ParseStep = 1;		
			i = i + 14;
			FromSavePos = i + 1;
		}
		else if (ParseStep == 1 )
		{
			
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{
				//換行了 .
				ParseStep = 0;

			}
			else if (strnicmp(header+i,"by ",3) == 0)
			{
				memset(FromStr,0,255);
				strncpy(FromStr,header+FromSavePos, i-FromSavePos-1);

				//trim left
				int nontrimpos = 0;
				for (int k = 0 ; k < i-FromSavePos-1 ; k ++)
				{
					if (FromStr[k] != 0x20)
					{
						nontrimpos = k;
						break;
					}
				}

				if (nontrimpos > 0 )
				{
					strcpy(FromStr,FromStr+nontrimpos);
				}



				if (strnicmp(FromStr,"unknown",7) != 0)
				{
				   	i = i + 2;
					BySavePos = i + 1;
					ParseStep = 2;
				}
			
						
			}
		
		}else if (ParseStep == 2 )
		{
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{	//換行了 .
				ParseStep = 0;
			}
			if (header[i] == 0x20 || header[i] == ';' )
			{ 
				ParseStep = 0;
				From2SavePos = 0;
				memset(ByStr,0,255);
				strncpy(ByStr,header+BySavePos, i-BySavePos);

				//找 host name
				int len = strlen(FromStr);
				int FromFirstPosEnd = 0;
				int DotPos1 = 0;
				int DotPos2 = 0;
				int DotPos3 = 0;

			

				//dot pos1
				for (int i = 0 ;  i < len ; i++)
				{
					if (FromStr[i] == '.' && DotPos1 == 0)
					{
						DotPos1 = i+1;
					} else
					if (FromStr[i] == 0x20  || i == len -1)
					{
						FromFirstPosEnd = i;
						break;
					}

				}

				memset(FromStr2,0,255);
				//int From2Pos=0;
				From2SavePos = 0;
				for (i = FromFirstPosEnd ; i < len ; i++)
				{
				
					if (FromStr[i] == '(')
					{
						From2SavePos = i + 1;
						 
					
					} else if (From2SavePos > 0 && FromStr[i] == ')')
					{
						
						strncpy(FromStr2,FromStr+From2SavePos,i-From2SavePos);
						

						//處理 from2
						//[139.175.252.113] helo=vmail.du.net.tw =>type1
						//butter.books.com.tw [219.84.164.9] =>type2
						//HELO mx2.url.com.tw =>type1
						//[192.168.16.21] => type2
						int len = strlen(FromStr2);
						char type=2;

						for (int j=0 ; j < len ; j++)
						{
							if (strnicmp(FromStr2+j,"helo",4) == 0) //ehlo 可能也有
							{
								strcpy(FromStr2,FromStr2+j+5);
								type = 1;
								From2SavePos = 0; // type 1 不做位移 , 從 From Str 的頭開使找
								break;
							}						
							 
						}

						if (type == 2)
						{
							for (int j=0 ; j < len ; j++)
							{
								if (FromStr2[j] == 0x20  )
								{
									FromStr2[j] = 0;
									break;
								}					
							}
						
						}
						break;					
					}
				
				}
			    
				len = strlen(FromStr2);
				for (i = 0 ;  i < len ; i++)
				{
					if (FromStr2[i] == '.' && DotPos2 == 0)
					{
						DotPos2 = i+1;
						break;
					}  

				}

				len = strlen(ByStr);
				for (i = 0 ;  i < len ; i++)
				{
					if (ByStr[i] == '.' && DotPos3 == 0)
					{
						DotPos3 = i+1;
						break;
					}  

				}

				//compare
				if ((DotPos1 > 0 || DotPos2 > 0) && FromFirstPosEnd > 0)
				{
				
					if (DotPos1 > 0 && strnicmp(FromStr + DotPos1 , ByStr + DotPos3 , FromFirstPosEnd - DotPos1) ==0 ||
						( DotPos2 > 0 && strnicmp(FromStr2 + DotPos2 , ByStr + DotPos3 , strlen(FromStr2) - DotPos2) ==0))
					{
						//Same Host
						ParseStep = 0;
					}
					else
					{
						//取 ip
						char TempIP[16];
						memset(TempIP,0,16);
						int DotCount = 0;
						int idx = 0;
						int idxpos = -1;
						int len = strlen(FromStr);


						for (int i = From2SavePos ; i < len ; i++)
						{
						
							if (isdigit(FromStr[i]))
							{
								if (idxpos == -1)
									idxpos = i;

								TempIP[idx] = FromStr[i];
								idx ++;
							}
							else if (FromStr[i] == '.' &&  idx > 0)
							{
								TempIP[idx] = FromStr[i];
								idx ++;
								DotCount ++;

								if (i - idxpos > 3 || idx >= 16)
								{
									//不是ip
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,16);
								}
								else if (DotCount == 2)
								{
									if (strncmp(TempIP,"192.168",7) == 0 || strncmp(TempIP,"10.",3) == 0 ||strncmp(TempIP,"127.0",5) == 0)
									{ 
										
										ParseStep = 0;
										break;
							 
									} 
									else if (strncmp(TempIP,"172.",4) == 0)
									{
										//locallan
										char LocalLanCheckStr[7];
										memset(LocalLanCheckStr,0,7);
										strncpy(LocalLanCheckStr,TempIP + 4,idx - 4 - 1);

										int IpValue = atoi(LocalLanCheckStr);
										if (IpValue >= 16 && IpValue <= 31)
										{													
										 
											ParseStep = 0;
											break;
										}						
									}
								
								}

								idxpos = i+1;
								
							}
							else
							{
								if (DotCount == 3 && TempIP[idx] != '.') //預防 adsl-126.49.155.info.com.ph ([222.126.49.155]) 
								{
									TempIP[idx+1] = 0; //得到 IP 了	

									*///檢查 trust ip
								/*	if ( strncmp(TempIP,"139.175.54",10) == 0 ||
										 strncmp(TempIP,"139.175.252",11) == 0 || //seednet
										 strncmp(TempIP,"211.20.188",10) == 0 ||  //pchome
										 strncmp(TempIP,"210.59.228",10) == 0 || //url
										 strncmp(TempIP,"168.95.5",8) == 0 ||
										 strncmp(TempIP,"168.95.4",8) == 0 ||//hinet
										 strncmp(TempIP,"210.17.38",9) == 0 || //sina
										 strncmp(TempIP,"61.64.127",9) == 0 || //sonet
										 strncmp(TempIP,"203.79.224",10) == 0 || //apol.com.tw
										 strncmp(TempIP,"210.242.13.251",14) == 0 || 
										 strncmp(TempIP,"210.242.13.252",14) == 0 ||
										 strncmp(TempIP,"210.242.13.253",14) == 0 //hinet vmsmtp
						 
						 
									  ) 
									{
										idx = 0;
										DotCount = 0;
										idxpos = -1;
									}
									else
									{*//*
										  
										IP[0] = 0; //ip ini	
										//第一個有效 IP 產生
										if (strlen(TempIP) > 7)
										{											
											strcpy(IP,TempIP);
											return 1;
										}
										else
										{
						
										    idx = 0;
											DotCount = 0;
											idxpos = -1;
											memset(TempIP,0,16);
							 
										}

						
									//}
								}
							
									idx = 0;
									DotCount = 0;
									idxpos = -1;
									memset(TempIP,0,16);
							}
						}
					}
				}
			}
		}
	}

	IP[0] = 0; //ip ini
	//找不到 IP 了.
	return 0;

}*/
/*
char CChecker::GetHeaderIP(char* header , char *IP, char *ReceiveTime)
{


 

	if ( header == NULL || IP == NULL || ReceiveTime == NULL) return -1;
	
	//StrList idenList;
	//求出長度
	int len = strlen(header);
	char ParseStep = 0;
	int BreakPos = 0;
	int SpPos = 0;
	int ByPos = 0;
	//char ByStr[255];
	char FromStr[255];
	char ReceiveStr[255];
	int ReceiveCount = 0;
	//bool FindIdent = false;
	bool LocalIP = false;
	bool IPFirstDot = false;
	bool IPFirstByte = false;

	char TempIP[16];
	//char FirstIP[16];
	//char IdentIP[16];

	//bool SkipChar = false;

	//char IP[15];
	//memset(IP,0,sizeof(IP));

	memset(FromStr,0,255);
	memset(ReceiveStr,0,255);
	memset(TempIP,0,16);
	//memset(FirstIP,0,16);
	//memset(IdentIP,0,16);

	
	

	//解開 identstr //不使用 identstr 了
	/*
	int indentlen = strlen(indentstr);
	int SavePos = 0;

	for(int j = 0 ; j < indentlen ; j++)
	{
	
		if (*(indentstr + j) == '|')
		{
			if (j-SavePos > 0)
			{
				char tempstr[255];
				memset(tempstr,0,255);
				strncpy(tempstr,indentstr+SavePos,j-SavePos);

				idenList.insert(idenList.end(),tempstr);
			}

			SavePos = j + 1;
		}
	
	}

	idenList.sort();
	idenList.unique();

	indentstr[0] = 0 ; //清空 ident str

	// 正向解 找到符合條件後 , 再確定是 locallan
	int idx = 0;
	int DotCount = 0;
	int IPLen = 0;
	 

    for (int i=0 ; i < len ; i ++)
	{
	
	
		//if (ParseStep == 0 && strnicmp(header+i,"Received: from ",15) == 0)
		if (ParseStep == 0 && strnicmp(header+i,"Received: ",10) == 0)
		{		
		  ParseStep = 1;
		  i = i + 10;

		  memset(FromStr,0,255);
		  memset(ReceiveStr,0,255);

		  ReceiveCount ++;
		  DotCount = 0;
		  LocalIP = false;
		  IPFirstDot = false;
		  IPFirstByte = false;
		  IPLen = 0;



		}
	    else if (ParseStep == 1)
		{
		
					
			if (header[i] == 13 && header[i+1] == 10 && header[i+2] != 9 && header[i+2] != 32)
			{
				//換行了 .
				ParseStep = 0;

			}else

			if (!IPFirstByte && ((header[i] == '(' && isdigit(header[i+1]) || (header[i] == '[' && isdigit(header[i+1])))))
			{
				IPFirstByte = true;
			}else
			找 IP ,  在 ( 後第一位必須要數字或 [ 後第一位是數字
			if (IPFirstByte && (isdigit(header[i]) || header[i] == '.'))
			//if ((isdigit(header[i]) || header[i] == '.'))
			{
			
				if (!IPFirstDot)
				{
				
					if (isdigit(header[i]))
					{
						IPFirstDot = true;	
					}
					else
					{
						continue;
					}

				}
				
				if (idx == 0)
				{
					LocalIP = false;
					DotCount = 0;
					IPLen = 0;				
		  
					
		  
				} 
				else if (idx >= 16 )
				{
				    idx = 0;
					IPFirstDot = false;
					IPFirstByte = false;
					continue; //過長應不是 ip
				}

				

				TempIP[idx] = header[i];
				idx ++;

				IPLen ++;

				if (header[i] == '.')
				{					
					DotCount++;

					//檢查是否是 Local Lan
					// 192.168.  , 10. , 172.16 - 172.31 , 127.0.0.1

					
					if (DotCount == 2 ) 
					{
						if (strncmp(TempIP,"192.168",7) == 0 || strncmp(TempIP,"10.",3) == 0 ||strncmp(TempIP,"127.0",5) == 0)
						{ 
							LocalIP = true;
							ParseStep = 0;
							//continue; //直接跳掉
						} 
						else if (strncmp(TempIP,"172.",4) == 0)
						{
							//locallan
							char LocalLanCheckStr[7];
							memset(LocalLanCheckStr,0,7);
							strncpy(LocalLanCheckStr,TempIP + 4,IPLen - 4 - 1);

							int IpValue = atoi(LocalLanCheckStr);
							if (IpValue >= 16 && IpValue <= 31)
							{													
								LocalIP = true;
								ParseStep = 0;
								//continue; //直接跳掉
							}						
						}
					}
				}
			 
			
			}
			else
			{
				if (DotCount == 3 && TempIP[IPLen-1] != '.') //預防 adsl-126.49.155.info.com.ph ([222.126.49.155]) 
				{
					TempIP[IPLen] = 0; //得到 IP 了	
					
					//檢查 trust ip
					if ( strncmp(TempIP,"139.175.54",10) == 0 ||
						 strncmp(TempIP,"139.175.252",11) == 0 || //seednet
						 strncmp(TempIP,"211.20.188",10) == 0 ||  //pchome
						 strncmp(TempIP,"210.59.228",10) == 0 || //url
						 strncmp(TempIP,"168.95.5",8) == 0 ||
						 strncmp(TempIP,"168.95.4",8) == 0 ||//hinet
						 strncmp(TempIP,"210.17.38",9) == 0 || //sina
						 strncmp(TempIP,"61.64.127",9) == 0 || //sonet
						 strncmp(TempIP,"203.79.224",10) == 0 || //apol.com.tw
						 strncmp(TempIP,"210.242.13.251",14) == 0 || 
						 strncmp(TempIP,"210.242.13.252",14) == 0 ||
						 strncmp(TempIP,"210.242.13.253",14) == 0 ||//hinet vmsmtp
						 //strncmp(TempIP,"219.84.164.9",14) == 0 //butter.books.com.tw
						 
						  ) 
					{
						ParseStep = 1;
						//idx = 0;
					}
					else
					{
					    //ParseStep = 2; 
						IP[0] = 0; //ip ini	
						//第一個有效 IP 產生
						if (strlen(TempIP) > 7)
						{											
							strcpy(IP,TempIP);
							return 1;
						}
						else
						{
						
							ParseStep = 1;
							//idx = 0;
						}

						
					}
				}
				else
				{
					ParseStep = 1;
				}

				idx = 0;
				IPFirstDot = false;
				IPFirstByte = false;
				
			}
		}
 
 
	 

	
	//}

 
	//IP[0] = 0; //ip ini
	//找不到 IP 了.
	//return 0;

	 
 

}*/

 
 
 /*

__declspec(dllexport) void  DllSendDomainToServer(char* ServerIP)
{
	CCoreSocket::Startup();
	CChecker::SendDomainLogToServer(ServerIP);
	CCoreSocket::Cleanup();


}
*/

/*
void CChecker::WriteMailPath()
{

				if (strlen(AllByStr2) > 0)
				{
					char path_buffer[_MAX_PATH];
					char drive[_MAX_DRIVE];
					char dir[_MAX_DIR];
					char fname[_MAX_FNAME];
					char ext[_MAX_EXT];
	   
					HINSTANCE hInstance = GetModuleHandle(NULL);
					GetModuleFileName(hInstance, path_buffer, MAX_PATH);

					_splitpath( path_buffer, drive, dir, fname, ext );

					char FileLogPath[_MAX_PATH];   

					strcpy(FileLogPath,drive);
					strcat(FileLogPath,dir);				
					strcat(FileLogPath,"MailPathLog.txt"); //Log Path

					
					FILE *fp2=NULL;
					fp2 = fopen(FileLogPath,"r+b");
					if (fp2 != NULL)
					{	
						strcat(AllByStr2,"\r\n");

						fseek(fp2,0,SEEK_END);
						fwrite(AllByStr2,1,strlen(AllByStr2),fp2);
						fclose(fp2);	
					}
				}

}*/


__declspec(dllexport) void  DllSendReport(char* ServerIP , char* DogId  , HWND ProgressBar  )
{
	
	

	CCoreSocket::Startup();

	CChecker::SendReportToServer(ServerIP,DogId,ProgressBar);
	CCoreSocket::Cleanup();


}
 
 

__declspec(dllexport) HRESULT  DllParseKeyFile(char *FileName, HWND ProgressBar)
{
	//CCoreSocket::Startup();
	return CChecker::ParseKeyFile(NULL,FileName,ProgressBar);

	//暫時不傳入 Db Section 
	//避免 LiveUpdate.exe 需要重編

	//CCoreSocket::Cleanup();


}
