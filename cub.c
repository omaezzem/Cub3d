/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:47:10 by mel-badd          #+#    #+#             */
/*   Updated: 2025/11/18 16:20:24 by omaezzem         ###   ########.fr       */
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
	// Split map into 2D array for raycasting
	cub.map_lines = ft_split(cub.map, '\n');
	if (!cub.map_lines)
	{
		fprintf(stderr, "Error: Failed to split map\n");
		return (EXIT_FAILURE);
	}
	
	// Find player position
	find_p(&cub);
	printf("Player position: (%d, %d)\n", cub.player_pos.x, cub.player_pos.y);
	
	// ===== RAYCASTING SETUP =====
	
	// Initialize player for raycasting (convert from grid position to double)
	init_player_raycasting(&cub);
	
	// Load textures
	// load_textures(&cub);
	
	// Initialize MLX and start rendering
	cub.player_pos.x *= T_SIZE;
	cub.player_pos.y *= T_SIZE;
	cub.floor_color = 0x573D32;
	cub.ceiling_color = 0x87CEEB;  // RGB for sky blue
	mlx_initcub(&cub);
	
	// Clean up (won't reach here because mlx_loop is infinite)
	// free_all(&cub);
	return (0);
}