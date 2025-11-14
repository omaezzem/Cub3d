/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:21:54 by omaezzem          #+#    #+#             */
/*   Updated: 2025/11/14 18:22:02 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

#define TILE_SIZE 64
#define MAP_NUM_ROWS 11
#define MAP_NUM_COLS 15
#define WINDOW_WIDTH (MAP_NUM_COLS * TILE_SIZE)
#define WINDOW_HEIGHT (MAP_NUM_ROWS * TILE_SIZE)
#define FOV_ANGLE (60 * (M_PI / 180))
#define WALL_STRIP_WIDTH 1
#define NUM_RAYS (WINDOW_WIDTH / WALL_STRIP_WIDTH)

typedef struct s_map
{
	int	grid[MAP_NUM_ROWS][MAP_NUM_COLS];
}	t_map;

typedef struct s_player
{
	float	x;
	float	y;
	float	radius;
	int		turn_direction;
	int		walk_direction;
	float	rotation_angle;
	float	move_speed;
	float	rotation_speed;
}	t_player;

typedef struct s_ray
{
	float	ray_angle;
	float	wall_hit_x;
	float	wall_hit_y;
	float	distance;
	bool	was_hit_vertical;
	bool	is_ray_facing_down;
	bool	is_ray_facing_up;
	bool	is_ray_facing_right;
	bool	is_ray_facing_left;
}	t_ray;

typedef struct s_game
{
	t_map			map;
	t_player		player;
	t_ray			rays[NUM_RAYS];
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	bool			running;
}	t_game;

void	init_map_data(t_game *game)
{
	int	grid_data[MAP_NUM_ROWS][MAP_NUM_COLS];
	int	i;
	int	j;

	grid_data[0] = (int[]){1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	grid_data[1] = (int[]){1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1};
	grid_data[2] = (int[]){1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1};
	grid_data[3] = (int[]){1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1};
	grid_data[4] = (int[]){1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1};
	grid_data[5] = (int[]){1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1};
	grid_data[6] = (int[]){1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	grid_data[7] = (int[]){1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	grid_data[8] = (int[]){1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1};
	grid_data[9] = (int[]){1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	grid_data[10] = (int[]){1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	i = -1;
	while (++i < MAP_NUM_ROWS)
	{
		j = -1;
		while (++j < MAP_NUM_COLS)
			game->map.grid[i][j] = grid_data[i][j];
	}
}

bool	has_wall_at(t_game *game, float x, float y)
{
	int	map_grid_index_x;
	int	map_grid_index_y;

	if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
		return (true);
	map_grid_index_x = (int)floor(x / TILE_SIZE);
	map_grid_index_y = (int)floor(y / TILE_SIZE);
	return (game->map.grid[map_grid_index_y][map_grid_index_x] != 0);
}

void	render_map_tile(t_game *game, int i, int j)
{
	int			tile_x;
	int			tile_y;
	SDL_Rect	rect;

	tile_x = j * TILE_SIZE;
	tile_y = i * TILE_SIZE;
	if (game->map.grid[i][j] == 1)
		SDL_SetRenderDrawColor(game->renderer, 34, 34, 34, 255);
	else
		SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
	rect = (SDL_Rect){tile_x, tile_y, TILE_SIZE, TILE_SIZE};
	SDL_RenderFillRect(game->renderer, &rect);
	SDL_SetRenderDrawColor(game->renderer, 34, 34, 34, 255);
	SDL_RenderDrawRect(game->renderer, &rect);
}

void	render_map(t_game *game)
{
	int	i;
	int	j;

	i = -1;
	while (++i < MAP_NUM_ROWS)
	{
		j = -1;
		while (++j < MAP_NUM_COLS)
			render_map_tile(game, i, j);
	}
}

void	init_player(t_game *game)
{
	game->player.x = WINDOW_WIDTH / 2;
	game->player.y = WINDOW_HEIGHT / 2;
	game->player.radius = 3;
	game->player.turn_direction = 0;
	game->player.walk_direction = 0;
	game->player.rotation_angle = M_PI / 2;
	game->player.move_speed = 2.0;
	game->player.rotation_speed = 2 * (M_PI / 180);
}

void	update_player(t_game *game)
{
	float	move_step;
	float	new_player_x;
	float	new_player_y;

	game->player.rotation_angle += game->player.turn_direction
		* game->player.rotation_speed;
	move_step = game->player.walk_direction * game->player.move_speed;
	new_player_x = game->player.x + cos(game->player.rotation_angle)
		* move_step;
	new_player_y = game->player.y + sin(game->player.rotation_angle)
		* move_step;
	if (!has_wall_at(game, new_player_x, new_player_y))
	{
		game->player.x = new_player_x;
		game->player.y = new_player_y;
	}
}

void	render_player(t_game *game)
{
	SDL_Rect	rect;

	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
	rect = (SDL_Rect){
		(int)(game->player.x - game->player.radius),
		(int)(game->player.y - game->player.radius),
		(int)(game->player.radius * 2),
		(int)(game->player.radius * 2)
	};
	SDL_RenderFillRect(game->renderer, &rect);
}

float	normalize_angle(float angle)
{
	angle = fmod(angle, 2 * M_PI);
	if (angle < 0)
		angle = (2 * M_PI) + angle;
	return (angle);
}

float	distance_between_points(float x1, float y1, float x2, float y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void	init_horz_intersection(t_game *game, t_ray *ray, float *xintercept,
		float *yintercept)
{
	*yintercept = floor(game->player.y / TILE_SIZE) * TILE_SIZE;
	*yintercept += ray->is_ray_facing_down ? TILE_SIZE : 0;
	*xintercept = game->player.x + (*yintercept - game->player.y)
		/ tan(ray->ray_angle);
}

void	calc_horz_steps(t_ray *ray, float *xstep, float *ystep)
{
	*ystep = TILE_SIZE;
	*ystep *= ray->is_ray_facing_up ? -1 : 1;
	*xstep = TILE_SIZE / tan(ray->ray_angle);
	*xstep *= (ray->is_ray_facing_left && *xstep > 0) ? -1 : 1;
	*xstep *= (ray->is_ray_facing_right && *xstep < 0) ? -1 : 1;
}

bool	find_horz_wall_hit(t_game *game, t_ray *ray, float *horz_wall_hit_x,
		float *horz_wall_hit_y)
{
	float	xintercept;
	float	yintercept;
	float	xstep;
	float	ystep;
	float	next_x;
	float	next_y;

	init_horz_intersection(game, ray, &xintercept, &yintercept);
	calc_horz_steps(ray, &xstep, &ystep);
	next_x = xintercept;
	next_y = yintercept;
	while (next_x >= 0 && next_x <= WINDOW_WIDTH && next_y >= 0
		&& next_y <= WINDOW_HEIGHT)
	{
		if (has_wall_at(game, next_x, next_y - (ray->is_ray_facing_up ? 1 : 0)))
		{
			*horz_wall_hit_x = next_x;
			*horz_wall_hit_y = next_y;
			return (true);
		}
		next_x += xstep;
		next_y += ystep;
	}
	return (false);
}

void	init_vert_intersection(t_game *game, t_ray *ray, float *xintercept,
		float *yintercept)
{
	*xintercept = floor(game->player.x / TILE_SIZE) * TILE_SIZE;
	*xintercept += ray->is_ray_facing_right ? TILE_SIZE : 0;
	*yintercept = game->player.y + (*xintercept - game->player.x)
		* tan(ray->ray_angle);
}

void	calc_vert_steps(t_ray *ray, float *xstep, float *ystep)
{
	*xstep = TILE_SIZE;
	*xstep *= ray->is_ray_facing_left ? -1 : 1;
	*ystep = TILE_SIZE * tan(ray->ray_angle);
	*ystep *= (ray->is_ray_facing_up && *ystep > 0) ? -1 : 1;
	*ystep *= (ray->is_ray_facing_down && *ystep < 0) ? -1 : 1;
}

bool	find_vert_wall_hit(t_game *game, t_ray *ray, float *vert_wall_hit_x,
		float *vert_wall_hit_y)
{
	float	xintercept;
	float	yintercept;
	float	xstep;
	float	ystep;
	float	next_x;
	float	next_y;

	init_vert_intersection(game, ray, &xintercept, &yintercept);
	calc_vert_steps(ray, &xstep, &ystep);
	next_x = xintercept;
	next_y = yintercept;
	while (next_x >= 0 && next_x <= WINDOW_WIDTH && next_y >= 0
		&& next_y <= WINDOW_HEIGHT)
	{
		if (has_wall_at(game, next_x - (ray->is_ray_facing_left ? 1 : 0), next_y))
		{
			*vert_wall_hit_x = next_x;
			*vert_wall_hit_y = next_y;
			return (true);
		}
		next_x += xstep;
		next_y += ystep;
	}
	return (false);
}

void	set_ray_hit_data(t_game *game, t_ray *ray, float horz_dist,
		float vert_dist)
{
	float	horz_x;
	float	horz_y;
	float	vert_x;
	float	vert_y;

	find_horz_wall_hit(game, ray, &horz_x, &horz_y);
	find_vert_wall_hit(game, ray, &vert_x, &vert_y);
	ray->wall_hit_x = (horz_dist < vert_dist) ? horz_x : vert_x;
	ray->wall_hit_y = (horz_dist < vert_dist) ? horz_y : vert_y;
	ray->distance = (horz_dist < vert_dist) ? horz_dist : vert_dist;
	ray->was_hit_vertical = (vert_dist < horz_dist);
}

void	cast_ray(t_game *game, t_ray *ray)
{
	float	horz_wall_hit_x;
	float	horz_wall_hit_y;
	float	vert_wall_hit_x;
	float	vert_wall_hit_y;
	float	horz_hit_distance;
	float	vert_hit_distance;
	bool	found_horz;
	bool	found_vert;

	found_horz = find_horz_wall_hit(game, ray, &horz_wall_hit_x,
			&horz_wall_hit_y);
	found_vert = find_vert_wall_hit(game, ray, &vert_wall_hit_x,
			&vert_wall_hit_y);
	if (found_horz)
		horz_hit_distance = distance_between_points(game->player.x,
				game->player.y, horz_wall_hit_x, horz_wall_hit_y);
	else
		horz_hit_distance = FLT_MAX;
	if (found_vert)
		vert_hit_distance = distance_between_points(game->player.x,
				game->player.y, vert_wall_hit_x, vert_wall_hit_y);
	else
		vert_hit_distance = FLT_MAX;
	set_ray_hit_data(game, ray, horz_hit_distance, vert_hit_distance);
}

void	init_ray_directions(t_ray *ray)
{
	ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle < M_PI;
	ray->is_ray_facing_up = !ray->is_ray_facing_down;
	ray->is_ray_facing_right = ray->ray_angle < 0.5 * M_PI
		|| ray->ray_angle > 1.5 * M_PI;
	ray->is_ray_facing_left = !ray->is_ray_facing_right;
}

void	cast_all_rays(t_game *game)
{
	float	ray_angle;
	int		i;

	ray_angle = game->player.rotation_angle - (FOV_ANGLE / 2);
	i = -1;
	while (++i < NUM_RAYS)
	{
		game->rays[i].ray_angle = normalize_angle(ray_angle);
		init_ray_directions(&game->rays[i]);
		cast_ray(game, &game->rays[i]);
		ray_angle += FOV_ANGLE / NUM_RAYS;
	}
}

void	render_rays(t_game *game)
{
	int	i;

	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 76);
	i = -1;
	while (++i < NUM_RAYS)
	{
		SDL_RenderDrawLine(game->renderer,
			(int)game->player.x, (int)game->player.y,
			(int)game->rays[i].wall_hit_x, (int)game->rays[i].wall_hit_y);
	}
}

void	handle_keydown(t_game *game, SDL_Keycode key)
{
	if (key == SDLK_UP)
		game->player.walk_direction = 1;
	else if (key == SDLK_DOWN)
		game->player.walk_direction = -1;
	else if (key == SDLK_RIGHT)
		game->player.turn_direction = 1;
	else if (key == SDLK_LEFT)
		game->player.turn_direction = -1;
	else if (key == SDLK_ESCAPE)
		game->running = false;
}

void	handle_keyup(t_game *game, SDL_Keycode key)
{
	if (key == SDLK_UP || key == SDLK_DOWN)
		game->player.walk_direction = 0;
	else if (key == SDLK_RIGHT || key == SDLK_LEFT)
		game->player.turn_direction = 0;
}

void	handle_events(t_game *game)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			game->running = false;
		else if (e.type == SDL_KEYDOWN)
			handle_keydown(game, e.key.keysym.sym);
		else if (e.type == SDL_KEYUP)
			handle_keyup(game, e.key.keysym.sym);
	}
}

void	update_game(t_game *game)
{
	update_player(game);
	cast_all_rays(game);
}

void	render_game(t_game *game)
{
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);
	render_map(game);
	render_rays(game);
	render_player(game);
	SDL_RenderPresent(game->renderer);
}

void	game_loop(t_game *game)
{
	while (game->running)
	{
		handle_events(game);
		update_game(game);
		render_game(game);
		SDL_Delay(16);
	}
}

bool	init_sdl(t_game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (false);
	game->window = SDL_CreateWindow("Raycasting Engine",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!game->window)
		return (false);
	game->renderer = SDL_CreateRenderer(game->window, -1,
			SDL_RENDERER_ACCELERATED);
	if (!game->renderer)
		return (false);
	return (true);
}

void	cleanup_sdl(t_game *game)
{
	if (game->renderer)
		SDL_DestroyRenderer(game->renderer);
	if (game->window)
		SDL_DestroyWindow(game->window);
	SDL_Quit();
}

int	main(void)
{
	t_game	game;

	game.window = NULL;
	game.renderer = NULL;
	game.running = true;
	if (!init_sdl(&game))
	{
		cleanup_sdl(&game);
		return (1);
	}
	init_map_data(&game);
	init_player(&game);
	game_loop(&game);
	cleanup_sdl(&game);
	return (0);
}