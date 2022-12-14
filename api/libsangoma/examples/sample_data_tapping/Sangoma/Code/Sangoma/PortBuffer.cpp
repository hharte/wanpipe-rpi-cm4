#if defined WIN32 && defined NDEBUG
#pragma optimize("gt",on)
#endif

#include "PortBuffer.h"
#include "sangoma_interface.h"

#define ERR_PORT_BUFFER	if(0)printf("Error:%s():line:%d: ", __FUNCTION__, __LINE__);if(0)printf

///////////////////////////////////////////////////////////////////////////////////////////////
///	\fn	        PortBuffer::PortBuffer(unsigned int buffer_size_in_bytes)
///	\brief	    Structure containing a buffer for the port to read from or write to. The
///			    size of buffer is indicated so that it can be allocated dynamically.
///
/// \param[in]  buffer_size_in_bytes - The size of the port buffer in bytes. 
///	\author	    David Rokhvarg (davidr@sangoma.com)
///	\date	    11/13/2007
///////////////////////////////////////////////////////////////////////////////////////////////
PortBuffer::PortBuffer(unsigned int buffer_size_in_bytes)
{
	BufferSize = buffer_size_in_bytes;
	Buffer = (unsigned char*)malloc(buffer_size_in_bytes);
	ApiHeader = new wp_api_hdr_t();

	memset(ApiHeader, 0x00, sizeof(wp_api_hdr_t));
}

PortBuffer::~PortBuffer()
{
	free(Buffer);
	delete ApiHeader;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///	\fn		PortBuffer::GetPortBuffer()
///	\brief	Returns pointer to Buffer.
///	\return	pointer to Buffer.
///	\author	David Rokhvarg (davidr@sangoma.com)
///	\date	11/15/2007
///////////////////////////////////////////////////////////////////////////////////////////////
unsigned char* PortBuffer::GetPortBuffer()
{
	return Buffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///	\fn		PortBuffer::GetPortBufferSize()
///	\brief	Returns the total Buffer size.
///	\return	Buffer size.
///	\author	David Rokhvarg (davidr@sangoma.com)
///	\date	11/15/2007
///////////////////////////////////////////////////////////////////////////////////////////////
unsigned int PortBuffer::GetPortBufferSize()
{
	return BufferSize;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///	\fn		PortBuffer::GetUserDataBuffer()
///	\brief	Returns pointer to buffer containng user data.
///			When data is recieved, this buffer will hold rx data.
///			When data is transmitted, this buffer should contain tx data before Write() is called.
///	\return	pointer to buffer containing user data.
///	\author	David Rokhvarg (davidr@sangoma.com)
///	\date	11/15/2007
///////////////////////////////////////////////////////////////////////////////////////////////
unsigned char *PortBuffer::GetUserDataBuffer()
{
	return GetPortBuffer();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///	\fn		PortBuffer::GetMaximumUserDataBufferSize()
///	\brief	Returns maximum size of user data buffer. This is the maximum length of user data
///			which can be transmitted/received in one buffer.
///	\return	maximum size of user data buffer.
///	\author	David Rokhvarg (davidr@sangoma.com)
///	\date	11/15/2007
///////////////////////////////////////////////////////////////////////////////////////////////
unsigned int PortBuffer::GetMaximumUserDataBufferSize()
{
	return GetPortBufferSize();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///	\fn		PortBuffer::GetUserDataLength()
///	\brief	Returns current length of user data at DataBuffer.
///			When data is recieved, GetUserDataLength() will return length of rx data.
///	\return	current length of user data at DataBuffer..
///	\author	David Rokhvarg (davidr@sangoma.com)
///	\date	11/15/2007
///////////////////////////////////////////////////////////////////////////////////////////////
unsigned int PortBuffer::GetUserDataLength()
{
	return ApiHeader->data_length;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///	\fn		PortBuffer::SetUserDataLength(unsigned int user_data_length)
///	\brief	Set length of user data at DataBuffer.
///			When data is transmitted, SetUserDataLength() will indicate to Sangoma API how many
///			should be transmitted.
///	\return	there is no return value.
///	\author	David Rokhvarg (davidr@sangoma.com)
///	\date	11/15/2007
///////////////////////////////////////////////////////////////////////////////////////////////
void PortBuffer::SetUserDataLength(unsigned int user_data_length)
{
	ApiHeader->data_length = user_data_length;
}
