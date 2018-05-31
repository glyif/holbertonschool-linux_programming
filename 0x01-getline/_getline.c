#include "_getline.h"

/**
 * mem_reset - null chars every byte (clears a string)
 * @buff: buffer to clear
 * @bytes: number of bytes
 *
 * Return: pointer to beginning of cleared buff
 */
char *mem_reset(char *buff, int bytes)
{
	int i = 0;

	while (i < bytes)
		buff[i++] = '\0';

	return (buff);
}

/**
 * _realloc - reallocates a memory chunck
 * @buff: pointer to area of memory to reallocate
 * @buff_size: size of ptr
 * @new_size: size of new memory chunck
 *
 * Return: pointer to new memory chunck
 */
void *_realloc(void *buff, unsigned int buff_size, unsigned int new_size)
{
	char *p;
	unsigned int i;

	if (buff == NULL)
	{
		p = malloc(new_size);
		if (p == NULL)
			return NULL;
		p = mem_reset(p, new_size);
		return (p);
	}

	if (new_size == 0)
	{
		free(buff);
		return (NULL);
	}

	if (buff_size == new_size)
		return (buff);

	p = malloc(new_size);

	if (p == NULL)
		return (NULL);

	p = mem_reset(p, new_size);

	for (i = 0; i < buff_size && i < new_size; i++)
		p[i] = ((char *)buff)[i];

	free(buff);

	return (p);
}

/**
 * _read - wrapper around read that reallocates as needed
 *
 * @fd: file descriptor of the file to use for reading
 * @buffer: address of pointer to input commands buffer
 * @limit: maxsize of input character string, realloc if necessary
 *
 * Return: number of characters written
 */
ssize_t _read(int fd, char **buffer, size_t limit)
{
	unsigned int i, j;
	size_t charcount, iterations;


	charcount = 0;
	iterations = 1;
	j = 0;
	i = -1;

	while (i != 0)
	{
		i = read(fd, (*buffer + j), limit);

		if (i <= 0)
			continue;

		charcount += i;
		j += i;

		if (charcount % limit == 0)
		{
			iterations++;
			*buffer = _realloc(*buffer, charcount, (limit * iterations));
		}
	}
	return ((ssize_t)charcount);
}

/**
 * _getline - getline implementation
 * @fd: file descriptor
 *
 * Return: pointer to the beginning of the buffer read
 */
char *_getline(const int fd)
{
	static char *buffer;

	buffer = malloc(BUFSIZE);

	ssize_t count;

	count = _read(fd, &buffer, READ_SIZE);

	if (count == 0)
	{
		free(buffer);
		return NULL;
	}

	if (buffer[count - 1] == '\n') {
		buffer[count - 1] = '\0';
	}

	return (buffer);
}
