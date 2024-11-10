/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoraji <yoraji@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 21:12:03 by yoraji            #+#    #+#             */
/*   Updated: 2024/11/06 21:12:07 by yoraji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_line(char *ptr)
{
	int		len;
	char	*line;
	int		added;

	if (!ptr)
		return (NULL);
	len = 0;
	while (ptr[len] && ptr[len] != '\n')
		len++;
	added = (ptr[len] == '\n');
	line = (char *)malloc(len + added + 1);
	if (!line)
		return (NULL);
	ft_strncpy(line, ptr, len + added);
	line[len + added] = '\0';
	return (line);
}

char	*update_buffer(char *buffer)
{
	char	*pos;
	char	*ptr;

	if (!buffer)
		return (NULL);
	pos = ft_strchr(buffer, '\n');
	if (!pos)
	{
		free(buffer);
		return (NULL);
	}
	if (*(pos + 1) == '\0')
	{
		free(buffer);
		return (NULL);
	}
	ptr = ft_strdup(pos + 1);
	free(buffer);
	return (ptr);
}

char	*handle_newline(char **ptr)
{
	char	*line;

	line = ft_line(*ptr);
	if (!line)
	{
		free(*ptr);
		*ptr = NULL;
		return (NULL);
	}
	*ptr = update_buffer(*ptr);
	return (line);
}

char	*process_read(int fd, char **ptr, char *tmp)
{
	ssize_t	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, tmp, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(*ptr), *ptr = NULL, NULL);
		tmp[bytes_read] = '\0';
		*ptr = ft_strjoin(*ptr, tmp);
		if (!*ptr)
			return (NULL);
		if (ft_strchr(*ptr, '\n'))
			return (handle_newline(ptr));
	}
	if (*ptr && **ptr)
	{
		tmp = ft_line(*ptr);
		return (free(*ptr), *ptr = NULL, tmp);
	}
	return (free(*ptr), *ptr = NULL, NULL);
}

char	*get_next_line(int fd)
{
	static char	*ptr[1024];
	char		*tmp;
	char		*line;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	if (ptr[fd] && ft_strchr(ptr[fd], '\n'))
		return (handle_newline(&ptr[fd]));
	tmp = malloc(BUFFER_SIZE + 1);
	if (!tmp)
	{
		if (ptr[fd])
		{
			free(ptr[fd]);
			ptr[fd] = NULL;
		}
		return (NULL);
	}
	line = process_read(fd, &ptr[fd], tmp);
	free(tmp);
	return (line);
}

// int	main()
// {
// 	int		fd;
// 	int		fd1;
// 	int		fd2;
// 	char	*line;

// 	fd = open("test.txt", O_RDONLY);
// 	fd1 = open("test1.txt", O_RDONLY);
// 	fd2 = open("test2.txt", O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("Error opening file");
// 		return (1);
// 	}
// 	line = get_next_line(fd);
// 	printf(":::::%s:::::\n", line);
// 	free(line);
// 	line = get_next_line(fd1);
// 	printf(":::::%s:::::\n", line);
// 	free(line);
// 	line = get_next_line(fd2);
// 	printf(":::::%s:::::\n", line);
// 	free(line);
// 	line = get_next_line(fd);
// 	printf(":::::%s:::::\n", line);
// 	free(line);
// 	line = get_next_line(fd);
// 	printf(":::::%s:::::\n", line);
// 	free(line);
// 	line = get_next_line(fd1);
// 	printf(":::::%s:::::\n", line);
// 	free(line);
// 	close(fd);
// 	close(fd1);
// 	close(fd2);
// }