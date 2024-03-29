/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 17:58:15 by hyungdki          #+#    #+#             */
/*   Updated: 2023/07/25 14:15:18 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include "../minilibx/mlx.h"
# include "../gnl/get_next_line.h"
# include "../libft/libft.h"
# include "../ft_printf/ft_printf.h"

# define WINDOW_SIZE_X 1280
# define WINDOW_SIZE_Y 1024
# define ISO_X 0.866025403784439
# define ISO_Y 0.5
# if !defined(TRUE) && !defined(FALSE)
#  define TRUE 1
#  define FALSE 0
# endif
# ifndef T_NULL
#  define T_NULL (void *)0
# endif

typedef int	t_bool;

typedef union u_color
{
	unsigned int	color;
	unsigned char	trgb[4];
}	t_color;

typedef struct s_point
{
	double		x;
	double		y;
	double		z;
	int			rx2d;
	int			ry2d;
	double		x2d;
	double		y2d;
	t_color		color;
}	t_point;

typedef struct s_map
{
	t_point		**map;
	int			row;
	int			col;
	int			midpoint_x2d;
	int			midpoint_y2d;
	double		largest_x2d;
	double		largest_y2d;
	double		smallest_x2d;
	double		smallest_y2d;
	double		len_x2d;
	double		len_y2d;
	double		basic_len;
	int			fd;
}	t_map;

typedef struct s_fdf
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	char	*img_addr;
	int		win_size_x;
	int		win_size_y;
	int		bpp;
	int		size_line;
	int		endian;
	t_map	*map_ptr;
}	t_fdf;

// draw_diagonal1.c
void			draw_line_diagonal(t_fdf *fdf, t_map *map, int r, int c);
t_bool			is_flat(t_point p1, t_point p2, t_point p3, t_point p4);
//draw1.c
void			draw_line(t_fdf *fdf, t_point p1, t_point p2);
//draw3.c
void			mlx_pixel_put_at_mem(t_fdf *fdf, int x, int y, int color);
t_color			*calc_color(t_point sp, t_point bp, int np);
// error.c
void			err_init(char **argv);
void			err_msg(const char *msg, int exit_code, t_bool use_perror);
void			map_parsing_on_error(t_map *map, char **split_result);
// event.c
int				quit_program(int keycode, t_fdf *fdf);
int				press_cross_on_window_frame(t_fdf *fdf);
// fdf1.c
void			fdf1(char **argv);
void			fdf2(t_fdf *fdf, t_map *map);
// fdf2.c
void			convert_point(t_map *map);
void			calc_win_size(t_map *map);
void			enlarge_image(t_map *map);
void			draw(t_fdf *fdf, t_map *map);
// map_parsing.c
t_map			map_parsing(char **argv);
// util.c
void			free_2d_array(void **array);
unsigned int	hex_str_to_uint(char *str_color);
void			fdf_init(t_fdf *fdf, t_map *map);
void			map_init(t_map *map);

#endif