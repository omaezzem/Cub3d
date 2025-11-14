/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:47:27 by mel-badd          #+#    #+#             */
/*   Updated: 2025/11/13 13:38:15 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
#define CUB_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <math.h>
# include <mlx.h>
# include "get_next_line/get_next_line.h"

# define HEIGHT 980
# define WIDTH 1580
# define FOV (M_PI / 3.0)   /* 60 degrees */

# define MOVE_SPEED 0.1
# define ROT_SPEED 0.05

# define KEY_W      13
# define KEY_A      0
# define KEY_S      1
# define KEY_D      2
# define KEY_LEFT   123
# define KEY_RIGHT  124
# define KEY_ESC    53

typedef struct s_keys
{
    int w;
    int a;
    int s;
    int d;
    int left;
    int right;
}   t_keys;

typedef struct s_vector
{
    int x;
    int y;
    int *_x;
    int *_y;
}               t_vector;

typedef struct s_player
{
    double  pos_x;      // Player position (world coords)
    double  pos_y;
    double  angle;      // Player viewing angle in radians (0 = east, PI/2 = south, etc.)
}   t_player;

typedef struct s_ray
{
    double  camera_x;      // X-coordinate in camera space ([-1,1])
    double  ray_dir_x;     // Ray direction (computed from angle)
    double  ray_dir_y;
    int     map_x;         // Current square of the map
    int     map_y;
    double  side_dist_x;   // Distance to next x or y side
    double  side_dist_y;
    double  delta_dist_x;  // Distance between x/y sides along ray
    double  delta_dist_y;
    double  perp_wall_dist;// Perpendicular distance to wall
    int     step_x;        // Step direction (-1 or +1)
    int     step_y;
    int     hit;           // Was a wall hit?
    int     side;          // NS or EW wall hit?
    int     line_height;   // Height of line to draw
    int     draw_start;    // Lowest pixel to fill for the stripe
    int     draw_end;      // Highest pixel to fill for the stripe
}   t_ray;

typedef struct s_texture
{
    void    *img;
    char    *addr;
    int     width;
    int     height;
    int     bpp;
    int     line_len;
    int     endian;
}   t_texture;

typedef struct s_cub
{
    /* MLX related */
    void        *mlx;
    void        *window;
    void        *img;
    char        *addr;
    int         bpp;
    int         line_len;
    int         endian;
    int         win_w;
    int         win_h;

    /* Map related */
    char        *map;
    char        **map_lines;

    /* Texture paths (strings) */
    char        *texture;
    char        *_NO;
    char        *_SO;
    char        *_WE;
    char        *_EA;
    char        *_F;
    char        *_C;

    /* Texture flags (int) */
    int         NO;
    int         SO;
    int         WE;
    int         EA;
    int         F;
    int         C;

    /* Player */
    t_vector    player_pos;
    t_player    player;

    /* Raycasting textures */
    t_texture   tex_north;
    t_texture   tex_south;
    t_texture   tex_east;
    t_texture   tex_west;

    /* Colors */
    int         floor_color;
    int         ceiling_color;

    /* Keys */
    t_keys      keys;
}   t_cub;

/* Get next line functions */
char    *creat_text(int fd, char *str);
char    *get_current_line(char *line);
char    *next_list(char *text);
char    *get_next_line(int fd);

/* String utilities */
char    *ft_substr(char *s, unsigned int start, size_t len);
char    *ft_strdup(char *s1);
char    *ft_strchr(const char *s, int c);
char    *ft_strjoin(char *s1, char *s2);
size_t  ft_strlen(char *s);
char    **ft_split(char *s, char c);

/* Parsing functions */
void    init_cub(t_cub *cub);
int     pars_av(int ac, char **av);
void    init_texture(char *path, t_cub *cub);
int     check_path(char *path);
int     is_path(char **split, t_cub *cub, char *line);
void    join(t_cub *cub, char **path);
int     read_map(char *av, t_cub *cub);
int     handle_colors_F(t_cub *cub);
int     handle_colors_C(t_cub *cub);
int     number_of_char(t_cub *cub);
int     pars_map(t_cub *cub);
void    change_space(t_cub *cub);
int     handle_map(t_cub *cub);
void    find_p(t_cub *game);

/* MLX functions */
int     len_h(char **line);
void    mlx_initcub(t_cub *cub);
void    failed_w(void);
void    ft_putstr_fd(char *s, int fd);
void    ft_putchar_fd(char c, int fd);

/* Raycasting functions (angle-based) */
void    init_player_raycasting(t_cub *cub);
void    load_textures(t_cub *cub);
void    load_texture(t_cub *cub, t_texture *tex, char *path);
void    draw_frame(t_cub *cub);
void    init_ray(t_cub *cub, t_ray *ray, int x);
void    perform_dda(t_cub *cub, t_ray *ray);
void    calculate_wall_distance(t_cub *cub, t_ray *ray);
void    draw_wall(t_cub *cub, t_ray *ray, int x);
int     get_texture_color(t_cub *cub, t_texture *tex, t_ray *ray, int y);
void    my_mlx_pixel_put(t_cub *cub, int x, int y, int color);
void    draw_ceiling(t_cub *cub);
void    draw_floor(t_cub *cub);

/* Input & movement helpers */
int     key_press(int keycode, t_cub *cub);
int     key_release(int keycode, t_cub *cub);
void    handle_keys(t_cub *cub);            /* called every frame to apply movement */
void    update_player(t_cub *cub);           /* update player position/angle from keys */
void    move_player(t_cub *cub, double dir); /* dir = +1 forward, -1 backward */
void    strafe_player(t_cub *cub, double dir); /* dir = +1 right, -1 left */
void    rotate_player(t_cub *cub, double dir); /* dir = +1 right, -1 left */

/* Utility */
// static double normalize_angle(double angle);
#endif /* CUB_H */
