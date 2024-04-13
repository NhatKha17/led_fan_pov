/**
 * @file      	cbuffer.c
 * @copyright	Triet Luu License
 * @license
 * @version   	1.0.0 
 * @date		15/01/2024
 * @brief     	Circular Buffer
 *            	This Circular Buffer is safe to use in IRQ with single reader,
 *            	single writer. No need to disable any IRQ.
 *
 *            	Capacity = <size> - 1
 * @note      	None
 * @example   	None
 */

/* Includes ----------------------------------------------------------- */
#include <cbuffer.h>
/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
void cb_init(cbuffer_t *cb, void *buf, uint32_t size)
{

#ifdef debug
	assert_param(size > 1);
	assert_param(size <= CB_MAX_SIZE);
#endif

	// Limit max size of cbuffer
	cb->data = (uint8_t *)buf;
	cb->size = (cb->size > CB_MAX_SIZE) ? CB_MAX_SIZE : size;
	cb->reader = -1;
	cb->writer = -1;
	cb->overflow = 0;
	cb->active = true;
}

void cb_clear(cbuffer_t *cb)
{
	cb->reader = -1;
	cb->writer = -1;
	cb->overflow = 0;
	cb->active = true;
}

uint32_t cb_read(cbuffer_t *cb, void *buf, uint32_t nbytes)
{
	if (!(cb->active))
		return false;

	uint16_t index_read;
	for (index_read = 1; index_read <= nbytes; index_read++)
	{
		// Check read full
		if ((cb->reader + 1) % cb->size == cb->writer)
		{
			// Increase reader
			cb->reader = (cb->reader + 1) % cb->size;
			// Assign data from cbuffer to read buffer
			*(uint8_t *)(buf + (index_read - 1)) = *(cb->data + cb->reader);
			// Assign data to overflow (2 is read all data)
			cb->overflow = 2;
			// Reset reader and writer
			cb->reader = -1;
			cb->writer = -1;
			return index_read;
		}
		else if (cb->writer == -1)
		// Check if there is no data but user want to read
		{
			// Assign data to overflow (2 is read all data)
			cb->overflow = 2;
			return false;
		}
		else
		{
			// Assign data to overflow (0 is not writer and read full)
			cb->overflow = 0;
			// Increase reader
			cb->reader = (cb->reader + 1) % cb->size;
			// Assign data from cbuffer to read buffer
			*(uint8_t *)(buf + (index_read - 1)) = *(cb->data + cb->reader);
		}
		// Check if read enough required data
		if (index_read == nbytes)
			return nbytes;
	}
	return true;
}

uint32_t cb_write(cbuffer_t *cb, void *buf, uint32_t nbytes)
{
	if (!(cb->active))
		return false;

	uint16_t index_write;
	for (index_write = 1; index_write <= nbytes; index_write++)
	{
		// Check overflow data when writing
		if ((cb->writer + 1) % cb->size == cb->reader)
		{
			// Assign data to overflow (1 is can not writer more data)
			cb->overflow = 1;
			return (index_write - 1);
		}
		else
		{
			// Assign data to overflow (0 is not writer and read full)
			cb->overflow = 0;
			// Increase writer
			cb->writer = (cb->writer + 1) % cb->size;
			// Assign data from writer buffer to cbuffer
			*(cb->data + cb->writer) = *(uint8_t *)(buf + (index_write - 1));
		}

		// Check if write enough required data
		if (index_write == nbytes)
			return nbytes;

		// Check if amount data is equal than size of cbuffer
		if (index_write == cb->size)
		{
			// Assign data to overflow (1 is can not writer more data)
			cb->overflow = 1;
			return index_write;
		}
	}
	return true;
}

uint16_t cb_data_count(cbuffer_t *cb)
{
	if (!(cb->active))
		return false;

	uint16_t num_empty_bytes_current;
	// Check if reader is reading and not read all data
	if (cb->reader != -1)
	{
		if (cb->reader > cb->writer)
		{
			num_empty_bytes_current = cb->size - (cb->reader - cb->writer);
		}
		if (cb->reader < cb->writer)
		{
			num_empty_bytes_current = cb->writer - cb->reader;
		}
	}
	else
	{
		// Check if read all data
		if (cb->overflow == 2)
			num_empty_bytes_current = 0;
		// Check if this is the first time read when cbuffer init
		else
			num_empty_bytes_current = cb->writer + 1;
	}
	return num_empty_bytes_current;
}

uint16_t cb_space_count(cbuffer_t *cb)
{
	if (!(cb->active))
		return false;

	return cb->size - cb_data_count(cb);
}
/* End of file -------------------------------------------------------- */
