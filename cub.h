#ifndef CUB_H
#define CUB_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <math.h>
# include "minilibx-linux/mlx.h"
# include "get_next_line/get_next_line.h"

# define HEIGHT 980
# define WIDTH 1580
# define FOV (M_PI / 3.0)   /* 60 degrees */

# define MOVE_SPEED 0.1
# define ROT_SPEED 0.05

# define KEY_W        119   // 'w'
# define KEY_A        97    // 'a'
# define KEY_S        115   // 's'
# define KEY_D        100   // 'd'
# define KEY_LEFT     65361 // Left arrow
# define KEY_RIGHT    65363 // Right arrow
# define KEY_ESC      65307 // Escape

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
    double  pos_x;
    double  pos_y;
    double  angle;
}   t_player;

typedef struct s_ray
{
    float   rayAngle;
    float   wallHitX;
    float   wallHitY;
    float   distance;
    int     wasHitVertical;
    int     isRayFacingDown;
    int     isRayFacingUp;
    int     isRayFacingRight;
    int     isRayFacingLeft;
    int     line_height;
    int     draw_start;
    int     draw_end;
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
size_t	ft_strlen(char *s);
/* String utilities */
char    *ft_substr(char *s, unsigned int start, size_t len);
char    *ft_strdup(char *s1);
char    *ft_strchr(char *s, int c);
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

/* Raycasting functions */
void    init_player_raycasting(t_cub *cub);
void    load_textures(t_cub *cub);
void    load_texture(t_cub *cub, t_texture *tex, char *path);
void    draw_frame(t_cub *cub);

/* New raycasting core functions */
void    ray_init(t_ray *ray, float rayAngle);
void    ray_cast(t_ray *ray, t_player *p, char **map);
void    cast_all_rays(t_cub *cub);
int     map_has_wall_at(char **map, float x, float y);
void    draw_wall(t_cub *cub, t_ray *ray, int x);
int     get_texture_color(t_cub *cub, t_texture *tex, t_ray *ray, int y);
void    my_mlx_pixel_put(t_cub *cub, int x, int y, int color);
void    draw_ceiling(t_cub *cub);
void    draw_floor(t_cub *cub);

/* Utility functions */
float   normalize_angle(float angle);
float   distance_between_points(float x1, float y1, float x2, float y2);

/* Input & movement helpers */
int     key_press(int keycode, t_cub *cub);
int     key_release(int keycode, t_cub *cub);
void    handle_keys(t_cub *cub);
void    update_player(t_cub *cub);
void    move_player(t_cub *cub, double dir);
void    strafe_player(t_cub *cub, double dir);
void    rotate_player(t_cub *cub, double dir);

#endif /* CUB_H */