/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:37:22 by omaezzem          #+#    #+#             */
/*   Updated: 2025/11/25 15:58:37 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"
#include <math.h>

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

void init_player_raycasting(t_cub *cub)
{
    cub->ray.map_x = ft_strlen(cub->map_lines[0]);
    cub->ray.map_y = len_h(cub->map_lines);

    char o = cub->map_lines[cub->player_pos.y][cub->player_pos.x];

	cub->player.pos_x = cub->player_pos.x * T_SIZE + T_SIZE / 2;
	cub->player.pos_y = cub->player_pos.y * T_SIZE + T_SIZE / 2;

    if (o == 'N')
        cub->player.angle = 3 * M_PI / 2;
    else if (o == 'S')
        cub->player.angle = M_PI / 2;
    else if (o == 'E')
        cub->player.angle = 0;
    else if (o == 'W')
        cub->player.angle = M_PI;
    else
    {
        ft_putstr_fd("Error: invalid player orientation\n", 2);
        exit(EXIT_FAILURE);
    }
}

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

int is_wall(t_cub *cub, double x, double y)
{
    int map_x = (int)(x / T_SIZE);
    int map_y = (int)(y / T_SIZE);

    if (map_x < 0 || map_y < 0 ||
        map_x >= cub->ray.map_x ||
        map_y >= cub->ray.map_y)
        return 1;

    return (cub->map_lines[map_y][map_x] == '1');
}

double ft_calcul_vitaghors(t_cub *cub, double hit_x, double hit_y)
{
    double dx = hit_x - cub->player.pos_x;
    double dy = hit_y - cub->player.pos_y;
    return sqrt(dx * dx + dy * dy);
}

double h_check(t_cub *cub, double *hit_x, double *hit_y, double angle)
{
    double x;
    double y;
    double check_y;
    double step_x;
    double step_y;
    int f_up = (angle > M_PI && angle < 2 * M_PI);
    int f_down = !f_up;

    y = floor(cub->player.pos_y / T_SIZE) * T_SIZE;
    if (f_down)
        y += T_SIZE;
    
    x = cub->player.pos_x + (y - cub->player.pos_y) / tan(angle);
    
    step_y = T_SIZE;
    if (f_up)
        step_y = -T_SIZE;
    step_x = step_y / tan(angle);
    
    int iterations = 0;
    while (iterations++ < 1000)
    {
        if (f_up)
            check_y = y - 1;
        else
            check_y = y;
        
        if (check_y < 0 || check_y >= cub->ray.map_y * T_SIZE ||
            x < 0 || x >= cub->ray.map_x * T_SIZE)
            break;
            
        if (is_wall(cub, x, check_y))
        {
            *hit_x = x;
            *hit_y = y;
            return ft_calcul_vitaghors(cub, x, y);
        }
        x += step_x;
        y += step_y;
    }
    return 1e30;
}

double v_check(t_cub *cub, double *hit_x, double *hit_y, double angle)
{
    double x;
    double y;
    double check_x;
    double step_x;
    double step_y;
    int f_left = (angle > M_PI / 2 && angle < 3 * M_PI / 2);
    int f_right = !f_left;

    x = floor(cub->player.pos_x / T_SIZE) * T_SIZE;
    if (f_right)
        x += T_SIZE;
    
    y = cub->player.pos_y + (x - cub->player.pos_x) * tan(angle);
    
    step_x = T_SIZE;
    if (f_left)
        step_x = -T_SIZE;
    
    step_y = step_x * tan(angle);
    
    int iterations = 0;
    while (iterations++ < 1000)
    {
        if (f_left)
            check_x = x - 1;
        else
            check_x = x;
        
        if (check_x < 0 || check_x >= cub->ray.map_x * T_SIZE ||
            y < 0 || y >= cub->ray.map_y * T_SIZE)
            break;
            
        if (is_wall(cub, check_x, y))
        {
            *hit_x = x;
            *hit_y = y;
            return ft_calcul_vitaghors(cub, x, y);
        }
        x += step_x;
        y += step_y;
    }
    return 1e30;
}

double	ft_casting(t_cub *cub, double angle, int *is_vertical)
{
    cub->cast.h_hitx = 0;
    cub->cast.h_hity = 0;
    cub->cast.v_hitx = 0;
    cub->cast.v_hity = 0;
    double h_distance;
    double v_distance;
    
    h_distance = h_check(cub, &cub->cast.h_hitx, &cub->cast.h_hity, angle);
    v_distance = v_check(cub, &cub->cast.v_hitx, &cub->cast.v_hity, angle);
    
    if (v_distance < h_distance)
    {
        *is_vertical = 1;
        return v_distance;
    }
    else
    {
        *is_vertical = 0;
        return h_distance;
    }
}

void my_mlx_pixel_put(t_cub *data, int x, int y, int color)
{
    char *dst;

    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return;

    dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
    *(unsigned int *)dst = color;
}

void draw_vertical_wall_strip(t_cub *data, int x, int wall_top, int height_wall, int is_vertical)
{
    int y;
    int color;
    int wall_bottom = wall_top + height_wall;

    if (is_vertical)
        color = 0xFF0000;  // Red for vertical
    else
        color = 0x00FF00;  // Green for horizontal

    if (wall_top < 0)
        wall_top = 0;
    if (wall_bottom > HEIGHT)
        wall_bottom = HEIGHT;

    for (y = wall_top; y < wall_bottom; y++)
        my_mlx_pixel_put(data, x, y, color);
}
int ft_error(char *str)
{
    printf("%s\n", str);
    return 1;
}

void draw_frame(t_cub *cub)
{
    t_render r;

    cub->img = mlx_new_image(cub->mlx, WIDTH, HEIGHT);
    if (!cub->img)
        exit(ft_error("mlx_new_image failed"));

    cub->addr = mlx_get_data_addr(cub->img, &cub->bpp, &cub->line_len, &cub->endian);
    if (!cub->addr)
    {
        mlx_destroy_image(cub->mlx, cub->img);
        exit(ft_error("mlx_get_data_addr failed"));
    }

    draw_ceiling(cub);
    draw_floor(cub);

    r.projection_distance = (WIDTH / 2.0) / tan(FOV / 2.0);

    r.i = 0;
    while (r.i < WIDTH)
    {
        r.ray_angle = cub->player.angle - (FOV / 2.0)
                     + ((double)r.i * FOV / (double)WIDTH);

        while (r.ray_angle < 0)
            r.ray_angle += 2 * M_PI;
        while (r.ray_angle >= 2 * M_PI)
            r.ray_angle -= 2 * M_PI;

        r.distance = ft_casting(cub, r.ray_angle, &r.is_vertical);

        r.corrected_distance = r.distance * cos(r.ray_angle - cub->player.angle);
        if (r.corrected_distance < 1)
            r.corrected_distance = 1;

        r.wall_height = (T_SIZE / r.corrected_distance) * r.projection_distance;
        r.wall_top = (HEIGHT / 2.0) - (r.wall_height / 2.0);

        draw_vertical_wall_strip(
            cub, r.i, (int)r.wall_top, (int)r.wall_height, r.is_vertical);

        r.i++;
    }

    mlx_put_image_to_window(cub->mlx, cub->window, cub->img, 0, 0);
    mlx_destroy_image(cub->mlx, cub->img);
}
