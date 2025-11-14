/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 12:52:51 by omaezzem          #+#    #+#             */
/*   Updated: 2025/11/14 13:06:23 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

#include "../cub.h"
#include <math.h>

// -------------------------
// Player Initialization
// -------------------------
void	init_player_raycasting(t_cub *cub)
{
	char	o = cub->map_lines[cub->player_pos.y][cub->player_pos.x];

	cub->player.pos_x = cub->player_pos.x + 0.5;
	cub->player.pos_y = cub->player_pos.y + 0.5;

	if (o == 'N')
		cub->player.angle = 3 * M_PI / 2; // (3 * 180) / 2 = 270
 	else if (o == 'S')
		cub->player.angle = M_PI / 2; // 180 / 2 = 90
	else if (o == 'E')
		cub->player.angle = 0; // 0
	else if (o == 'W')
		cub->player.angle = M_PI; // 180 
	else
	{
		ft_putstr_fd("Error: invalid player orientation\n", 2);
		exit(EXIT_FAILURE);
	}
}

// -------------------------
// Texture loading (same)
// -------------------------
void	load_textures(t_cub *cub)
{
	cub->tex_north.img = NULL;
	cub->tex_south.img = NULL;
	cub->tex_east.img = NULL;
	cub->tex_west.img = NULL;
	printf("Skipping textures - using solid colors\n");
}

void	load_texture(t_cub *cub, t_texture *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(cub->mlx, path, 
		&tex->width, &tex->height);
	if (!tex->img)
	{
		ft_putstr_fd("Error\nFailed to load texture: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd("\n", 2);
		exit(EXIT_FAILURE);
	}
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, 
		&tex->line_len, &tex->endian);
}

// -------------------------
// Drawing helpers
// -------------------------
void	my_mlx_pixel_put(t_cub *cub, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return;
	dst = cub->addr + (y * cub->line_len + x * (cub->bpp / 8));
	*(unsigned int *)dst = color;
}
 //drawing skyyy =============
void	draw_ceiling(t_cub *cub)
{
	int x;
	int y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			my_mlx_pixel_put(cub, x, y, cub->ceiling_color);
			x++;
		}
		y++;
	}
}
// =============================



// drawing flooor ===========
void	draw_floor(t_cub *cub)
{
	int y;
	int x;

	y = HEIGHT / 2;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			my_mlx_pixel_put(cub, x, y, cub->floor_color);
			x++;
		}
		y++;
	}
}
// ==============================

// -------------------------
// Ray initialization (angle-based)
// -------------------------
void	init_ray(t_cub *cub, t_ray *ray, int x)
{
	double	ray_angle;

	// Calculate the angle for this ray
	ray_angle = cub->player.angle - (FOV / 2.0) + ((double)x / WIDTH) * FOV;
	// 270 - (60 / 2) = 240 
	ray->ray_dir_x = cos(ray_angle); // cos(240) = 0.3
	ray->ray_dir_y = sin(ray_angle); // sin(240) = 0.9

	ray->map_x = (int)cub->player.pos_x;
	ray->map_y = (int)cub->player.pos_y;

	ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	ray->hit = 0;

	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (cub->player.pos_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - cub->player.pos_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (cub->player.pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - cub->player.pos_y) * ray->delta_dist_y;
	}
}

// -------------------------
// DDA, Wall distance & drawing (same logic)
// -------------------------
void	perform_dda(t_cub *cub, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (cub->map_lines[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}

void	calculate_wall_distance(t_cub *cub, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - cub->player.pos_x + 
			(1 - ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - cub->player.pos_y + 
			(1 - ray->step_y) / 2) / ray->ray_dir_y;
	
	ray->line_height = (int)(HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_end >= HEIGHT)
		ray->draw_end = HEIGHT - 1;
}

void	draw_wall(t_cub *cub, t_ray *ray, int x)
{
	int	y;
	int	color;

	if (ray->side == 0 && ray->ray_dir_x > 0)
		color = 0xFF0000; // East
	else if (ray->side == 0 && ray->ray_dir_x < 0)
		color = 0x00FF00; // West
	else if (ray->side == 1 && ray->ray_dir_y > 0)
		color = 0x0000FF; // South
	else
		color = 0xFFFF00; // North
	
	if (ray->side == 1)
		color = (color >> 1) & 8355711;
	for (y = ray->draw_start; y < ray->draw_end; y++)
		my_mlx_pixel_put(cub, x, y, color);
}

// -------------------------
// Frame drawing
// -------------------------
void	draw_frame(t_cub *cub)
{
	int		x;
	t_ray	ray;

	cub->img = mlx_new_image(cub->mlx, WIDTH, HEIGHT);
	cub->addr = mlx_get_data_addr(cub->img, &cub->bpp, &cub->line_len, &cub->endian);
	draw_ceiling(cub);
	draw_floor(cub);

	for (x = 0; x < WIDTH; x++)
	{
		init_ray(cub, &ray, x);
		perform_dda(cub, &ray);
		calculate_wall_distance(cub, &ray);
		draw_wall(cub, &ray, x);
	}
	mlx_put_image_to_window(cub->mlx, cub->window, cub->img, 0, 0);
	mlx_destroy_image(cub->mlx, cub->img);
}
