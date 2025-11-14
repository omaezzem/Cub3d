/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 09:36:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/11/11 13:53:22 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

size_t ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return i;
}

void init_cub(t_cub *cub)
{
	cub->map = NULL;
	cub->texture = NULL;
	cub->NO = 0;
	cub->SO = 0;
	cub->WE = 0;
	cub->EA = 0;
	cub->F = 0;
	cub->C = 0;
	cub->_NO = NULL;
	cub->_SO = NULL;
	cub->map_lines = NULL;
	cub->_WE = NULL;
	cub->_EA = NULL;
	cub->_F = NULL;
	cub->_C = NULL;
}

int pars_av(int ac, char **av)
{
	(void)av;
	int	len;

	len = strlen(av[1]);
	if (len < 4)
		return (0);
	if (ac != 2)
		return (0);
	// if (strcmp(av[0], "cub3d") == 0)
	// 	return (-1);
	if (strcmp(av[1] + len - 4, ".cub") != 0)
		return (0);
	return (1);
}

void init_texture(char *path, t_cub *cub)
{
	if (strcmp(path, "NO") == 0)
		cub->NO += 1;
	else if (strcmp(path, "SO") == 0)
		cub->SO += 1;
	else if (strcmp(path, "WE") == 0)
		cub->WE += 1;
	else if (strcmp(path, "EA") == 0)
		cub->EA += 1;
	else if (strcmp(path, "F") == 0)
		cub->F += 1;
	else if (strcmp(path, "C") == 0)
		cub->C += 1;
}

int check_path(char *path)
{
	if (!path)
		return (0);
	int	i;

	i = 0;
	while (path[i])
		i++;
	i--;
	if (path[i - 4] == '.' && path[i - 3] == 'p' &&
		path[i - 2] == 'n' && path[i - 1] == 'g')
		return (6566);
	return (0);
}

int is_path(char **split, t_cub *cub, char *line)
{
	if (!split)
		return (0);
	if (strcmp(split[0], "F") == 0 || strcmp(split[0], "C") == 0)
	{
		init_texture(split[0], cub);
		return (1);
	}
	else if (strcmp(split[0], "NO") == 0 ||
		strcmp(split[0], "SO") == 0 ||
		strcmp(split[0], "WE") == 0 ||
		strcmp(split[0], "EA") == 0)
	{
		init_texture(split[0], cub);
		if (!check_path(split[1]))
			return (0);
	}
	else if (split[0][0] == '\n')
		return (1);
	else
	{
	if (cub->NO != 1 || cub->SO != 1 || cub->WE != 1 || cub->EA != 1 ||
		cub->F != 1 || cub->C != 1)
		return (0);
		cub->map = ft_strjoin(cub->map, line);
	}
	return (1);
}

void	join(t_cub *cub, char **path)
{
	if (!path)
		return ;
	if (strcmp(path[0], "NO") == 0)
		cub->_NO = ft_strjoin(cub->_NO, path[1]);
	else if (strcmp(path[0], "SO") == 0)
		cub->_SO = ft_strjoin(cub->_SO, path[1]);
	else if (strcmp(path[0], "WE") == 0)
		cub->_WE = ft_strjoin(cub->_WE, path[1]);
	else if (strcmp(path[0], "EA") == 0)
		cub->_EA = ft_strjoin(cub->_EA, path[1]);
	else if (strcmp(path[0], "F") == 0)
		cub->_F = ft_strjoin(cub->_F, path[1]);
	else if (strcmp(path[0], "C") == 0)
		cub->_C = ft_strjoin(cub->_C, path[1]);
}

int read_map(char *av, t_cub *cub)
{
	int fd;
	fd = open(av, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (0);
	}
	char *line = get_next_line(fd);
	char **split;
	// int k = 0;
	// while (line && line[k])
	// {
	// 	if (line[k] == '\n')
	// 		line[k] = ' ';
	// 	else if (line[k] == '\r')
	// 		line[k] = ' ';
	// 	else if (line[k] == '\t')
	// 		line[k] = ' ';
	// 	k++;
	// }
	while (line)
	{
		split = ft_split(line, ' ');
		if (!split)
		{
			free(line);
			close(fd);
			return (0);
		}
		if (!is_path(split, cub, line))
		{
			fprintf(stderr, "Error: Invalid path in map\n");
			// ft_free(split);
			free(line);
			close(fd);
			return (0);
		}
		join(cub, split);
		line = get_next_line(fd);
	}
	if (cub->NO != 1 || cub->SO != 1 || cub->WE != 1 || cub->EA != 1 ||
		cub->F != 1 || cub->C != 1)
	{
		fprintf(stderr, "Error: No textures defined\n");
		// ft_free(split);
		free(line);
		close(fd);
		return (0);
	}
	return (1);
}

int handle_colors_F(t_cub *cub)
{
	if (!cub->_F)
		return (0);
	int i = 0;
	char *temp = cub->_F;
	while (*temp)
	{
		if (*temp == ',')
			i++;
		temp++;
	}
	if (i != 2)
		return (0);
	char **split = ft_split(cub->_F, ',');
	if (!split || !split[0] || !split[1] || !split[2])
		return (0);
	int j = atoi(split[0]);
	if (j < 0 || j > 255)
		return (0);
	j = atoi(split[1]);
	if (j < 0 || j > 255)
		return (0);
	j = atoi(split[2]);
	if (j < 0 || j > 255)
		return (0);
	return (1);
}

int handle_colors_C(t_cub *cub)
{
	if (!cub->_C)
		return (0);
	int i = 0;
	char *temp = cub->_C;
	while (*temp)
	{
		if (*temp == ',')
			i++;
		temp++;
	}
	if (i != 2)
		return (0);
	char **split = ft_split(cub->_C, ',');
	if (!split || !split[0] || !split[1] || !split[2])
		return (0);
	int j = atoi(split[0]);
	if (j < 0 || j > 255)
		return (0);
	j = atoi(split[1]);
	if (j < 0 || j > 255)
		return (0);
	j = atoi(split[2]);
	if (j < 0 || j > 255)
		return (0);
	return (1);
}

int number_of_char(t_cub *cub)
{
	int i = 0;
	int j = 0;
	int f = 0;
	int c = 0;
	if (!cub->map)
		return (0);
	while (cub->map[i])
	{
		if (cub->map[i] == '1')
			j++;
		else if (cub->map[i] == '0')
			f++;
		else if (cub->map[i] == 'N')
			c++;
		i++;
	}
	// printf("%s\n", cub->map);
	// printf("j = %d, f = %d, c = %d\n", j, f, c);
	if (j < 1 || f < 1 || c != 1)
		return (0);
	return (1);
}

int pars_map(t_cub *cub)
{
	if (!cub->map)
		return (0);
	char **map_lines = ft_split(cub->map, '\n');
	if (!map_lines)
		return (0);
	int i = 0;
	int j = 0;
	while (map_lines[0][i])
	{
		if (map_lines[0][i] != '1'
			&& map_lines[0][i] != ' ' && map_lines[0][i] != '\t')
			return (0);
		i++;
	}
	i = 0;
	while (map_lines[j])
	{
		i = 0;
		if (map_lines[j][i] != '1' && map_lines[j][i] != ' ' && map_lines[j][i] != '\t')
			return (0);
		while (map_lines[j][i])
		{
			if (map_lines[j][i] != '1' && map_lines[j][i] != ' ' &&
				map_lines[j][i] != '\t' && map_lines[j][i] != '0' && map_lines[j][i] != 'N')
				return (0);
			i++;
		}
		if (map_lines[j][i - 1] != '1'
			&& map_lines[j][i - 1] != ' ' && map_lines[j][i - 1] != '\t')
			return (0);
		if (map_lines[j + 1] == NULL)
			break;
		j++;
	}
	while (map_lines[j][i])
	{
		if (map_lines[j][i] != '1'
			&& map_lines[j][i] != ' ' && map_lines[j][i] != '\t')
			return (0);
		i++;
	}
	if (!number_of_char(cub))
		return (0);
	return (1);
}

void change_space(t_cub *cub)
{
	int i = 0;
	// int j = 0;
	if (!cub->map)
		return ;
	while (cub->map[i])
	{
		if (cub->map[i] == ' ')
			cub->map[i] = '1';
		// else if (cub->map[i] == '\t')
		// {
		// 	int j = 0;
		// 	while (j <= 4)
		// 	{
		// 		printf("here\n");
		// 		cub->map[i] = '1';
		// 		j++;
		// 	}
		// }
		i++;
	}
	// while (cub->map[j])
	// {
	// 	if (cub->map[j] == 'N')
	// 		cub->map[j] = '0';
	// 	j++;
	// }
}

// int map_correct(t_cub *cub)
// {
// 	int x
// }
// int check_zero(char *cub)
// {
// 	if (cub->pos.x == )
// 	return 0;
// }

int handle_map(t_cub *cub)
{
	int	row;
	int	col;

	if (!cub->map_lines)
		return 0;
	row = 0;
	int i = 0;
	while (cub->map_lines[row])
	{
		col = 0;
		while (cub->map_lines[row][col])
		{
			if (cub->map_lines[row][col] == '0')
			{
				cub->player_pos._x[i] = row;
				cub->player_pos._y[i] = col;
				return 0;
			}
			col++;
			i++;
		}
		row++;
	}
	// if (check_zero(cub) == 0)
	// {
	// 	fprintf(stderr, "Error: Map is not correct\n");
	// 	return 0;
	// }
	return 1;
}

void	find_p(t_cub *cub)
{
	int	y;
	int	x;
	int	found = 0;

	y = 0;
	while (cub->map_lines[y])
	{
		x = 0;
		while (cub->map_lines[y][x])
		{
			if (cub->map_lines[y][x] == 'N' || cub->map_lines[y][x] == 'S' ||
				cub->map_lines[y][x] == 'E' || cub->map_lines[y][x] == 'W')
			{
				cub->player_pos.x = x;
				cub->player_pos.y = y;
				found = 1;
				break;
			}
			x++;
		}
		if (found)
			break;
		y++;
	}

	if (!found)
	{
		ft_putstr_fd("Error: Player not found in map\n", 2);
		exit(EXIT_FAILURE);
	}
}
