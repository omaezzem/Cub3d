/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:47:10 by mel-badd          #+#    #+#             */
/*   Updated: 2025/11/25 13:21:37 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int main(int ac, char **av)
{
	t_cub cub;

	init_cub(&cub);
	if (!pars_av(ac, av))
	{
		fprintf(stderr, "Error: Invalid arguments\n");
		return (EXIT_FAILURE);
	}
	if (!read_map(av[1], &cub))
	{
		fprintf(stderr, "Error: Failed to read map\n");
		return (EXIT_FAILURE);
	}
	if (!handle_colors_F(&cub))
	{
		fprintf(stderr, "Error: Invalid colors\n");
		return (EXIT_FAILURE);
	}
	if (!handle_colors_C(&cub))
	{
		fprintf(stderr, "Error: Invalid colors\n");
		return (EXIT_FAILURE);
	}
	if (!pars_map(&cub))
	{
		fprintf(stderr, "Error: Invalid map\n");
		return (EXIT_FAILURE);
	}
	change_space(&cub);
	cub.map_lines = ft_split(cub.map, '\n');
	if (!cub.map_lines)
	{
		fprintf(stderr, "Error: Failed to split map\n");
		return (EXIT_FAILURE);
	}
	find_p(&cub);
	
	// raycast=====================
	init_player_raycasting(&cub);
	cub.floor_color = 0x573D32;
	cub.ceiling_color = 0x87CEEB;
	mlx_initcub(&cub);

	// free_all(&cub);
	return (0);
}