/*
 * ring_buffer.c
 *
 *  Created on: 01-Aug-2018
 *      Author: medprime
 */

#include "ring_buffer.h"

void Ring_Buffer_Init(Ring_Buffer_t* handle, char* buffer, uint16_t size)
    {
    handle->Buffer        =  buffer;
    handle->Read_Index    =  0;
    handle->Write_Index   =  0;
    handle->Size          =  size;
    }


uint8_t Ring_Buffer_Put_Char(Ring_Buffer_t* handle, char data)
    {

    if (handle->Write_Index == handle->Read_Index)
	{
        //over flow return 0 ?
	}

    handle->Buffer[handle->Write_Index] = data;

    handle->Write_Index++;

    if (handle->Write_Index == handle->Size)
	{
	handle->Write_Index = 0;
	}

    return 1;
    }


uint8_t Ring_Buffer_Get_Char(Ring_Buffer_t* handle, char* data)
    {

    if (handle->Read_Index == handle->Write_Index)
	{
	*data = 0;
	return 0;
	}

    *data = handle->Buffer[handle->Read_Index];

    handle->Read_Index++;

    if (handle->Read_Index == handle->Size)
	{
	handle->Read_Index = 0;
	}

    return 1;
    }


uint8_t Ring_Buffer_Peek_Char(Ring_Buffer_t* handle, char* data,
	uint16_t position)
    {

    if (position < Ring_Buffer_Get_Count(handle))
	{
	if (position + handle->Read_Index < handle->Size)
	    {
	    position = handle->Read_Index + position;
	    }
	else
	    {
	    position = handle->Size - (position + handle->Read_Index);
	    }
	*data = handle->Buffer[position];
	}
    else
	{
	*data = 0;
	return 0;
	}

    return 1;

    }


uint8_t Ring_Buffer_Search_Char(Ring_Buffer_t* handle, char data)
    {

    uint8_t  xreturn = 0;
    uint16_t temp = handle->Read_Index;

    while (temp != handle->Write_Index)
	{

	if (data == handle->Buffer[temp++])
	    {
	    xreturn = 1;
	    break;
	    }

	if(temp == handle->Size)
	    {
	    temp = 0;
	    }

	}

    return xreturn;
    }


uint8_t Ring_Buffer_Is_Full(Ring_Buffer_t* handle)
    {
    return (handle->Read_Index == handle->Write_Index ) ? 1 : 0;
    }


void Ring_Buffer_Flush(Ring_Buffer_t* handle)
    {
    handle->Read_Index = handle->Write_Index;
    }


uint16_t Ring_Buffer_Get_Count(Ring_Buffer_t* handle)
    {
    if (handle->Write_Index >= handle->Read_Index)
	{
	return (handle->Write_Index - handle->Read_Index);
	}
    return (handle->Size - (handle->Read_Index - handle->Write_Index ));
    }

