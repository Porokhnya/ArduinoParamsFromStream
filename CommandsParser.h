#ifndef _COMMANDS_PARSER_H
#define _COMMANDS_PARSER_H
//-------------------------------------------------------------------------------------------
#include <Arduino.h>
//-------------------------------------------------------------------------------------------
typedef void (*OnParamReceived)(const char* param, const char* value);
//-------------------------------------------------------------------------------------------
class CommandsParser
{
	public:
		CommandsParser(Stream* s) : pStream(s){}
		CommandsParser(Stream& s) : pStream(&s){}

		// handler - user-defined event func
		// paramDelimiter - param name and param value delimiter
		// в stopDelimiters - command delimiters
		void begin(OnParamReceived handler, const char* paramDelimiter="=", const char* stopDelimiters="\r\n;&")
		{
			pUserFunc = handler;
			pParamDelimiter = paramDelimiter;
			pStopDelimiters = stopDelimiters;
			workBuffer = "";
			
		}
		
		void update()
		{
				while(pStream && pStream->available())
				{
					char ch = pStream->read();
					if(strchr(pStopDelimiters,ch))
					{
						// find stop char
						// look for possible key _delimiters_ value pair
						const char* pDelim = strstr(workBuffer.c_str(),pParamDelimiter);
						if(pDelim)
						{
							// find delimiters, read key/value
							
							// key before delimiter
							const char* beginStr = workBuffer.c_str();
							workBuffer[pDelim - beginStr] = 0; // add zero after key
							
							// call user func
							pUserFunc(beginStr,pDelim + strlen(pParamDelimiter));

							workBuffer = ""; // clear buffer
						}
						else
								workBuffer = "";// delimiters not found - bad format
					}
					else
					 workBuffer += ch; // store current char into the buffer
				} // while
		}
		
	private:
	
		Stream* pStream;
		const char* pParamDelimiter;
		const char* pStopDelimiters;
		OnParamReceived pUserFunc;
		
		String workBuffer;
};
//-------------------------------------------------------------------------------------------
#endif
