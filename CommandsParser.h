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

		// handler - пользовательская функция обработки входящих параметров
		// в paramDelimiter - разделитель параметра и команды
		// в stopDelimiters - разделители команд между собой
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
						// встретили стоповый символ - разделитель команд
						// смотрим, есть ли у нас пара ключ-разделитель-значение
						const char* pDelim = strstr(workBuffer.c_str(),pParamDelimiter);
						if(pDelim)
						{
							// есть разделитель, читаем ключ и значение
							
							// ключ будет до разделителя
							const char* beginStr = workBuffer.c_str();
							workBuffer[pDelim - beginStr] = 0; // завершаем строку нулём по центру, чтобы не дёргать память
							
							// вызываем пользовательскую функцию
							pUserFunc(beginStr,pDelim + strlen(pParamDelimiter));

							workBuffer = ""; // очищаем буфер
						}
						else
								workBuffer = "";// разделитель не найден, просто очищаем входной буфер
					}
					else
					 workBuffer += ch; // просто запоминаем текущий символ в буфер	
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
