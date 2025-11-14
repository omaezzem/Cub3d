/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:17:27 by omaezzem          #+#    #+#             */
/*   Updated: 2025/11/13 13:37:59 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"
#include "cub.h"
#include <math.h>

static double normalize_angle(double angle)
{
    const double two_pi = 2.0 * M_PI;
    while (angle < 0.0)
        angle += two_pi;
    while (angle >= two_pi)
        angle -= two_pi;
    return angle;
}

void move_forward(t_cub *cub)
{
    double new_x;
    double new_y;

    new_x = cub->player.pos_x + cos(cub->player.angle) * MOVE_SPEED;
    new_y = cub->player.pos_y + sin(cub->player.angle) * MOVE_SPEED;

    /* Collision check (simple) */
    if (cub->map_lines[(int)new_y][(int)new_x] != '1')
    {
        cub->player.pos_x = new_x;
        cub->player.pos_y = new_y;
    }
}

void move_backward(t_cub *cub)
{
    double new_x;
    double new_y;

    new_x = cub->player.pos_x - cos(cub->player.angle) * MOVE_SPEED;
    new_y = cub->player.pos_y - sin(cub->player.angle) * MOVE_SPEED;

    if (cub->map_lines[(int)new_y][(int)new_x] != '1')
    {
        cub->player.pos_x = new_x;
        cub->player.pos_y = new_y;
    }
}

void move_left(t_cub *cub)
{
    double new_x;
    double new_y;
    double side_angle = cub->player.angle - M_PI_2; /* left = angle - 90° */

    new_x = cub->player.pos_x + cos(side_angle) * MOVE_SPEED;
    new_y = cub->player.pos_y + sin(side_angle) * MOVE_SPEED;

    if (cub->map_lines[(int)new_y][(int)new_x] != '1')
    {
        cub->player.pos_x = new_x;
        cub->player.pos_y = new_y;
    }
}

void move_right(t_cub *cub)
{
    double new_x;
    double new_y;
    double side_angle = cub->player.angle + M_PI_2; /* right = angle + 90° */

    new_x = cub->player.pos_x + cos(side_angle) * MOVE_SPEED;
    new_y = cub->player.pos_y + sin(side_angle) * MOVE_SPEED;

    if (cub->map_lines[(int)new_y][(int)new_x] != '1')
    {
        cub->player.pos_x = new_x;
        cub->player.pos_y = new_y;
    }
}

void rotate_left(t_cub *cub)
{
    cub->player.angle = normalize_angle(cub->player.angle - ROT_SPEED);
}

void rotate_right(t_cub *cub)
{
    cub->player.angle = normalize_angle(cub->player.angle + ROT_SPEED);
}

/* render_frame now uses angle-based movement */
int render_frame(t_cub *cub)
{
    if (cub->keys.w)
        move_forward(cub);
    if (cub->keys.s)
        move_backward(cub);
    if (cub->keys.a)
        move_left(cub);
    if (cub->keys.d)
        move_right(cub);
    if (cub->keys.left)
        rotate_left(cub);
    if (cub->keys.right)
        rotate_right(cub);

    draw_frame(cub);
    return (0);
}

int close_window(t_cub *cub)
{
    if (cub->tex_north.img)
        mlx_destroy_image(cub->mlx, cub->tex_north.img);
    if (cub->tex_south.img)
        mlx_destroy_image(cub->mlx, cub->tex_south.img);
    if (cub->tex_east.img)
        mlx_destroy_image(cub->mlx, cub->tex_east.img);
    if (cub->tex_west.img)
        mlx_destroy_image(cub->mlx, cub->tex_west.img);
    if (cub->window)
        mlx_destroy_window(cub->mlx, cub->window);
    exit(0);
    return (0);
}

int handling_keys(int keycode, t_cub *cub)
{
    if (keycode == KEY_ESC)
        close_window(cub);
    else if (keycode == KEY_W)
        cub->keys.w = 1;
    else if (keycode == KEY_S)
        cub->keys.s = 1;
    else if (keycode == KEY_A)
        cub->keys.a = 1;
    else if (keycode == KEY_D)
        cub->keys.d = 1;
    else if (keycode == KEY_LEFT)
        cub->keys.left = 1;
    else if (keycode == KEY_RIGHT)
        cub->keys.right = 1;
    return (0);
}

int key_release(int keycode, t_cub *cub)
{
    if (keycode == KEY_W)
        cub->keys.w = 0;
    else if (keycode == KEY_S)
        cub->keys.s = 0;
    else if (keycode == KEY_A)
        cub->keys.a = 0;
    else if (keycode == KEY_D)
        cub->keys.d = 0;
    else if (keycode == KEY_LEFT)
        cub->keys.left = 0;
    else if (keycode == KEY_RIGHT)
        cub->keys.right = 0;
    return (0);
}

void failed_mlx(void)
{
    ft_putstr_fd("Error\nFailed to initialize MLX\n", 2);
    exit(EXIT_FAILURE);
}

/* Mouse horizontal movement rotates the player angle */
int mouse_move(int x, int y, t_cub *cub)
{
    static int last_x = -1;
    int delta_x;
    double rotation;
    const double sensitivity = 0.002; /* tune this */

    (void)y; /* not used */

    if (last_x == -1)
    {
        last_x = x;
        return (0);
    }

    delta_x = x - last_x;
    if (delta_x != 0)
    {
        rotation = delta_x * sensitivity;
        cub->player.angle = normalize_angle(cub->player.angle + rotation);
    }

    last_x = x;
    return (0);
}

void mlx_initcub(t_cub *cub)
{
    cub->mlx = mlx_init();
    if (!cub->mlx)
        failed_mlx();

    cub->window = mlx_new_window(cub->mlx, WIDTH, HEIGHT, "CUB3D");
    if (!cub->window)
        failed_w();

    cub->keys.w = 0;
    cub->keys.s = 0;
    cub->keys.a = 0;
    cub->keys.d = 0;
    cub->keys.left = 0;
    cub->keys.right = 0;

    mlx_hook(cub->window, 2, 1L<<0, handling_keys, cub);
    mlx_hook(cub->window, 3, 1L<<1, key_release, cub);
    mlx_hook(cub->window, 17, 0, close_window, cub);
    mlx_hook(cub->window, 6, 1L<<6, mouse_move, cub);

    mlx_loop_hook(cub->mlx, render_frame, cub);
    mlx_loop(cub->mlx);
}
